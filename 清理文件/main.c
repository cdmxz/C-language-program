#include "arr_helper.h"
#include "clean_file.h"
#include "str_helper.h"
#include "../list/list/list.h"
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <direct.h>
#include <sys/stat.h>


#pragma comment(lib, "../list/x64/Debug/list.lib")


// 要删除的文件扩展名
char* exts[] = { ".log",".tmp",".temp" };

List list = LIST_DEFAULT;

long long totalFile = 0, totalDir = 0, totalErr = 0;
long long delFile = 0, delDir = 0, delErr = 0;

void write_file(const char* filename, List list)
{
	FILE* fp = NULL;
	if (fopen_s(&fp, filename, "w"))
	{
		print_err_format(__FUNCSIG__" 无法写入文件：%s\n", filename);
		return;
	}
	for (int i = 0; i < list->curLen; i++)
	{
		fputs(get_byindex(list, i), fp);
		fputc('\n', fp);
	}
	fclose(fp);
}

int main(void)
{
	const char* dir = "F:\\C语言";
	list = create_list();
	delete_files(dir, exts, true);
	printf("\n\n遍历 %lld个文件，%lld个目录，错误数：%lld\n", totalFile, totalDir, totalErr);
	printf("删除 %lld个文件，%lld个空目录，删除失败数：%lld\n", delFile, delDir, delErr);
	write_file("大文件.txt", list);
	free_list(list);
	getchar();
	return 0;
}

void delete_files(const char* dir, const char** exts, bool del_emptydir)
{
	char findPath[MAX_PATH];   // 当前查找路径
	char basePath[MAX_PATH];   // 相对查找路径不包括 *.*
	char tmp[512];
	long long file_size;
	struct __finddata64_t filedata;
	intptr_t handle;

	// 处理路径
	strcpy_s(basePath, _countof(basePath), dir);
	trim_end(basePath, '\\');
	sprintf_s(findPath, _countof(findPath), "%s\\*.*", basePath);

	// 查找第一个文件
	handle = _findfirst64(findPath, &filedata);
	if (handle == -1)
	{
		print_err_format("\n无法搜索路径：%s\n原因：", findPath);
		totalErr++;
		return;
	}
	// 继续查找文件
	do
	{
		if (strcmp(filedata.name, ".") == 0 || strcmp(filedata.name, "..") == 0)
			continue;
		if (filedata.attrib & _A_SUBDIR)
		{	// 是目录
			totalDir++;
			strcpy_s(tmp, _countof(tmp), basePath);
			sprintf_s(tmp, _countof(tmp), "%s\\%s", tmp, filedata.name);
			if (is_empty_dir(tmp))
			{
				print(YELLOW, "删除目录：%s\n", tmp);
				delDir++;
				del_dir(tmp);
			}
			else
			{
				delete_files(tmp, exts, del_emptydir);
			}
			continue;
		}
		if (filedata.attrib & _A_SYSTEM)
		{// 跳过系统文件
			continue;
		}
		strcpy_s(tmp, _countof(tmp), basePath);
		sprintf_s(tmp, _countof(tmp), "%s\\%s", tmp, filedata.name);
		// 不是系统文件
		if (is_contain_ext(exts, filedata.name))
		{
			print(DRAKWHITE, "删除文件：%s\n", tmp);
			delFile++;
			del_file(tmp);
		}
		file_size = get_filesize(tmp);
		if (file_size >= 300LL * 1024LL * 1024LL)
		{
			sprintf_s(tmp, _countof(tmp), "%s\t大小：%.2fMB", tmp, file_size / 1024.0 / 1024.0);
			print(DRAKBLUE, "大文件：%s\n", tmp);
			append_elem(list, create_elem(tmp));
		}
		totalFile++;
	} while (_findnext64(handle, &filedata) == 0);
	_findclose(handle);
}

void get_dirname(const char* fullpath, char* buf, int buf_len)
{
	char drive[5], dir[MAX_PATH];
	_splitpath_s(fullpath, drive, _countof(drive), dir, _countof(dir), NULL, 0, NULL, 0);
	sprintf_s(buf, buf_len, "%s\\%s", drive, dir);
}

bool is_empty_dir(const char* dir)
{
	char tmp[MAX_PATH];
	struct __finddata64_t filedata;
	intptr_t handle;
	bool is_empty = true;

	// 处理路径
	strcpy_s(tmp, _countof(tmp), dir);
	trim_end(tmp, '\\');
	sprintf_s(tmp, _countof(tmp), "%s\\*.*", tmp);

	handle = _findfirst64(tmp, &filedata);
	if (handle == -1)
	{
		print_err_format(__FUNCSIG__" error!\ndir=%s\n", dir);
		return false;
	}
	do
	{
		if (strcmp(filedata.name, ".") != 0 && strcmp(filedata.name, "..") != 0)
			is_empty = false;
	} while (_findnext64(handle, &filedata) == 0);
	_findclose(handle);
	return is_empty;
}

// 是否包含指定的扩展名
bool is_contain_ext(const char** exts, const char* filename)
{
	if (!exts || !filename)
		return false;
	char ext[MAX_PATH];
	get_ext(filename, ext, _countof(ext));
	return is_contian(exts, 3, ext);
}

// 获取文件扩展名
void get_ext(const char* filename, char* ext, int ext_len)
{
	char* result = strrchr(filename, '.');
	if (!result)
	{
		ZeroMemory(ext, ext_len);
		return;
	}
	strcpy_s(ext, ext_len, result);
}

void del_file(const char* filename)
{
	// 失败返回 -1
	if (remove(filename))
	{
		delErr++;
		print_err_format("删除：%s失败\n原因：", filename);
	}
}

void del_dir(const char* dir)
{
	// 失败返回 -1
	if (_rmdir(dir))
	{
		delErr++;
		print_err_format("删除：%s失败\n原因：", dir);
	}
}

// 获取文件大小
long long get_filesize(char* filename)
{
	struct _stat64 statbuf;
	if (_stat64(filename, &statbuf))
		return -1;
	return statbuf.st_size;
}