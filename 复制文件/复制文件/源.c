#include <stdio.h>
#include <Windows.h>
#include <stdlib.h>
#include <errno.h>
#include <io.h>
#include <sys/stat.h>
#include <direct.h>
#include <shlobj.h>   
#include <time.h>
#pragma comment(lib, "shell32.lib")


#define WHITE   FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE
#define DRAKWHITE   FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY
#define DRAKBLUE  FOREGROUND_BLUE | FOREGROUND_INTENSITY
#define YELLOW  FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY
#define DRAKRED  FOREGROUND_RED | FOREGROUND_INTENSITY
#define RED  FOREGROUND_RED
#define DRAKGREEN FOREGROUND_INTENSITY|FOREGROUND_GREEN

#define ISFILE_FILE 1
#define ISFILE_DIR  2
#define ISFILE_UNKNOWN 3
#define ISFILE_INVALID -1

// 以指定颜色打印字符
void print_char(char ch, WORD color);
// 以指定颜色打印文本
void print_text(const char* const str, BOOL addLinebreak, WORD color);
// 删除字符末尾的一个字符
void del_last_ch(char* str, char ch);
// 字符串转BOOL值。转换成功返回TRUE，失败返回FALSE
BOOL to_boolean(char* str, BOOL* boolValue);
// 导出指定目录下所有文件名到文件
void export_filelist(const char* const path, const char* const fileName);
// 获取文件大小
long long get_filesize(const char* const fileName);
// 打印进度
void print_progress(char ch, int num, int color);
// 是否为排除复制的文件
BOOL is_excluded_file(const char* const fileName);
// 从路径中获取文件名（包括扩展名）
int get_filename(const char* fullPath, char* fileName, size_t size);
// 批量复制文件
void bulk_copy(const char* const listFile, char* sourceDir, char* destDir, int* numOfFailedFile, int* numOfFailedDir, int* totalFile, int overWrite);
// 复制文件
BOOL copy_file(const char* const source, const char* const dest, BOOL overWrite, BOOL printProgress);
// 创建目录
BOOL create_dir(const char* const dirName);
// 读取排除复制的文件名列表
char** read_excluded_filelist(const char* const listName);
void free_excluded_filelist(char** fileList);
// 给定一个路径，判断是文件还是目录
int is_file(const char* const path);
// 返回获取当前exe所在的目录路径，路径结尾不包括“\\”
void get_module_dir(char* ret_dirpath, size_t size);
// 在一个字符串中查找另一个字符串末尾的字符
char* find_endofstr(const char* str, const char* subStr);
void print_err(const char* const msg);

char** g_excluded_files = NULL;


int main(int argc, char* argv[])
{
    system("title 复制文件");
    if (argc < 4)
    {
        print_text("命令：复制文件.exe <源路径(文件/文件夹)> <目标路径(文件/文件夹)> <是否覆盖(true/false)>", TRUE, YELLOW);
        return -1;
    }
    char source[MAX_PATH], dest[MAX_PATH], temp[MAX_PATH], dir[MAX_PATH];
    BOOL overWrite;
    char listFile[MAX_PATH] = "FileList.txt";
    int numOfFailedFile = 0, numOfFailedDir = 0, totalFile = 0;
    clock_t startTime, stopTime;
    startTime = clock();

    // 读取排除复制的文件名
    g_excluded_files = read_excluded_filelist("文件名.txt");
    // 获取当前exe所在的目录路径
    get_module_dir(dir, sizeof(dir));
    // 文件列表路径
    sprintf_s(listFile, sizeof(listFile), "%s\\FileList.txt", dir);
    strcpy_s(source, sizeof(source), argv[1]);
    strcpy_s(dest, sizeof(dest), argv[2]);
    // 将字符转小写
    _strlwr_s(source, sizeof(source));
    _strlwr_s(dest, sizeof(dest));

    if (strcmp(source, dest) == 0)
    {
        print_text("源路径与目标路径不能相同！", TRUE, YELLOW);
        return -1;
    }
    if (strcmp(source, "desktop") == 0)
    {
        if (!SHGetSpecialFolderPathA(NULL, source, CSIDL_DESKTOPDIRECTORY, FALSE))
        {
            print_text("无法获取桌面路径！", TRUE, YELLOW);
            return -1;
        }
    }
    else
        strcpy_s(source, sizeof(source), argv[1]);
    if (strcmp(dest, "desktop") == 0)
    {
        if (!SHGetSpecialFolderPathA(NULL, dest, CSIDL_DESKTOPDIRECTORY, FALSE))
        {
            print_text("无法获取桌面路径！", TRUE, YELLOW);
            return -1;
        }
    }
    else
        strcpy_s(dest, sizeof(dest), argv[2]);
    if (!to_boolean(argv[3], &overWrite))
    {
        print_text("参数“是否覆盖(true/false)”输入错误！", TRUE, YELLOW);
        return;
    }
    if (_access(source, 0) == -1)
    {
        puts("源路径无效！");
        return -1;
    }
    if (is_file(source) == ISFILE_DIR)
    {
        // 删除路径结尾的“\\”
        del_last_ch(source, '\\');
        del_last_ch(dest, '\\');
        // 创建目标文件夹
        create_dir(dest);
        // 导出源文件和目录的路径到列表文件
        export_filelist(source, listFile);
        // 批量复制文件
        bulk_copy(listFile, source, dest, &numOfFailedFile, &numOfFailedDir, &totalFile, overWrite);
        sprintf_s(temp, sizeof(temp), "\n\n源文件数：%d，失败文件数：%d，失败文件夹数：%d", totalFile, numOfFailedFile, numOfFailedDir);
        print_text(temp, TRUE, RED);
        remove(listFile);
    }
    else
    {
        if (!copy_file(source, dest, overWrite, TRUE))
            numOfFailedFile++;
    }
    stopTime = clock();
    printf("\n共耗时：%.2lf秒\n", ((double)stopTime - startTime) / CLOCKS_PER_SEC);
    if (numOfFailedDir != 0 || numOfFailedFile != 0)
        system("pause");
    free_excluded_filelist(g_excluded_files);
    return 0;
}

// 批量复制文件
void bulk_copy(const char* const listFile, char* sourceDir, char* destDir, int* numOfFailedFile, int* numOfFailedDir, int* totalFile, int overWrite)
{
    if (!listFile || !numOfFailedFile || !numOfFailedDir || !totalFile)
        return;
    FILE* fp = NULL;
    char temp[MAX_PATH], sourceFile[MAX_PATH], destFile[MAX_PATH];
    int retVal;
    *numOfFailedFile = 0;
    *numOfFailedDir = 0;
    *totalFile = 0;
    // 读取列表文件里的文件名
    if (fopen_s(&fp, listFile, "r"))
    {
        print_err(NULL);
        return;
    }
    while (!feof(fp))
    {
        memset(temp, 0, sizeof(temp));
        memset(sourceFile, 0, sizeof(sourceFile));
        fgets(temp, sizeof(temp), fp);
        del_last_ch(temp, '\n');
        if (temp[0] == '\0')
            continue;
        get_filename(temp, sourceFile, sizeof(sourceFile));
        if (is_excluded_file(sourceFile))
        {
            sprintf_s(temp, sizeof(temp), "排除复制：%s", sourceFile);
            print_text(temp, TRUE, DRAKWHITE);
            continue;
        }
        retVal = is_file(temp);
        // 将源文件路径替换为目标文件路径
        // 例如：源文件路径：C:\\test\\file\\1.txt 替换后的路径：D:\\file\\1.txt
        char* find = find_endofstr(temp, sourceDir);
        if (!find)
        {
            sprintf_s(temp, sizeof(temp), "无法替换路径，跳过复制：%s", sourceDir);
            print_text(temp, TRUE, DRAKRED);
        }
        if (*(find + 1) == '\\')
            find += 2;

        sprintf_s(destFile, sizeof(destFile), "%s\\%s", destDir, find);
        if (retVal == ISFILE_DIR)
        {
            if (!create_dir(destFile))
                (*numOfFailedDir)++;
        }
        else if (retVal == ISFILE_FILE)
        {
            if (!copy_file(temp, destFile, overWrite, FALSE))
                (*numOfFailedFile)++;
        }
        else
        {
            sprintf_s(temp, sizeof(temp), "复制：%s失败\n原因：未知文件\n", sourceDir);
            print_text(temp, FALSE, DRAKRED);
            (*numOfFailedFile)++;
        }
        (*totalFile)++;
    }
    fclose(fp);
}

// 以指定颜色打印文本
void print_text(const char* const str, BOOL addLinebreak, WORD color)
{
    if (!str)
        return;
    // 设置字体颜色
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
    if (addLinebreak)
        printf("%s\n", str);
    else
        printf("%s", str);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
}

// 以指定颜色打印字符
void print_char(char ch, WORD color)
{
    // 设置字体颜色
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
    putchar(ch);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
}

// 删除字符末尾的一个字符
void del_last_ch(char* str, char ch)
{
    size_t len = strlen(str);
    if (len == 0)
        return;
    if (str[len - 1] == ch)
        str[len - 1] = '\0';
}

// 字符串转BOOL值。转换成功返回TRUE，失败返回FALSE
BOOL to_boolean(char* str, BOOL* boolValue)
{
    size_t size = strlen(str) + 1;
    BOOL  retVal = TRUE;
    if (size - 1 == 0)
        return FALSE;
    char* lower = (char*)calloc(size, sizeof(char));
    if (!lower)
        return FALSE;
    strcpy_s(lower, size, str);
    _strlwr_s(lower, size); // 将字符转小写
    if (strcmp(lower, "true") == 0)
        *boolValue = TRUE;
    else if (strcmp(lower, "false") == 0)
        *boolValue = FALSE;
    else
    {
        retVal = FALSE;
        boolValue = NULL;
    }
    free(lower);
    return retVal;
}

// 导出指定目录下所有文件名到文件
void export_filelist(const char* const path, const char* const fileName)
{
    char cmd[MAX_PATH];
    if (_access(fileName, 0) == 0)
        remove(fileName);
    sprintf_s(cmd, sizeof(cmd), "dir \"%s\" /B /OG /S > \"%s\"", path, fileName);
    system(cmd);
    sprintf_s(cmd, sizeof(cmd), "dir \"%s\" /B /OG /S /AH >> \"%s\"", path, fileName);
    system(cmd);
}

// 获取文件大小
long long get_filesize(const char* const fileName)
{
    struct _stat64 info;
    return (_stat64(fileName, &info) == -1) ? -1 : info.st_size;
}

// 打印进度
void print_progress(char ch, int num, int color)
{
    printf("当前进度：");
    for (int i = 0; num != 0 && i < num; i++)
        print_char(ch, color);
}

// 是否为排除复制的文件
BOOL is_excluded_file(const char* const fileName)
{
    char** list = g_excluded_files;
    if (!fileName || !list)
        return FALSE;
    while (*list)
    {
        if (strcmp(fileName, *list) == 0)
            return TRUE;
        list++;
    }
    return FALSE;
}

// 从路径中获取文件名（包括扩展名）
int get_filename(const char* fullPath, char* fileName, size_t size)
{
    char* buf = NULL;
    char* name = NULL;
    char* token = NULL;
    char* nextToken = NULL;
    size_t bufsize;
    if (_access(fullPath, 0) == -1 || !fileName || size == 0)
        return -1;
    bufsize = strlen(fullPath) + 1;
    buf = (char*)calloc(bufsize, sizeof(char));
    if (!buf)
        return -1;
    memcpy_s(buf, bufsize, fullPath, strlen(fullPath) + 1);
    token = strtok_s(buf, "\\", &nextToken);
    while (token)
    {
        name = token;
        token = strtok_s(NULL, "\\", &nextToken);
    }
    if (!name)
        return -1;
    memcpy_s(fileName, size, name, strlen(name) + 1);
    free(buf);
    return 0;
}

// 打印已复制大小
void print_size(double cursize, double totalsize)
{
    if (totalsize < 1048576.0)
        printf("%.2lfKB/%.2lfKB", cursize / 1024.0, totalsize / 1024.0);
    else if (totalsize < 1073741824.0)
        printf("%.2lfKB(%.2lfMB)/%.2lfKB(%.2lfMB)", cursize, cursize / 1048576.0, totalsize, totalsize / 1048576.0);
    else
        printf("%.2lfKB(%.2lfGB)/%.2lfKB(%.2lfGB)", cursize, cursize / 1073741824.0, totalsize, totalsize / 1073741824.0);
}

// 复制文件
BOOL copy_file(const char* const source, const char* const dest, BOOL overWrite, BOOL printProgress)
{
    FILE* fpRead, * fpWrite;
    size_t readLen;
    double curSize = 0.0;
    double totalSize = get_filesize(source);
    char msg[MAX_PATH], err[MAX_PATH], fileName[MAX_PATH], name[MAX_PATH], ext[MAX_PATH];
    char* buf = NULL;
    size_t bufSize = totalSize > 1073741824.0 ? 104857600U : 4096U;
    if (_access(source, 0) == -1)
    {
        strerror_s(err, sizeof(err), errno);
        sprintf_s(msg, sizeof(msg), "复制：%s失败\n原因：%s", source, err);
        print_text(msg, TRUE, DRAKRED);
        return FALSE;
    }
    // 获取文件名
    _splitpath_s(source, NULL, 0, NULL, 0, name, sizeof(name), ext, sizeof(ext));
    sprintf_s(fileName, sizeof(fileName), "%s%s", name, ext);
    // 如果目标文件存在
    if (_access(dest, 0) != -1 && !overWrite)
    {
        sprintf_s(msg, sizeof(msg), "跳过复制：%s", source);
        print_text(msg, TRUE, DRAKRED);
        return TRUE;
    }
    // 打开文件
    if (fopen_s(&fpRead, source, "rb") || fopen_s(&fpWrite, dest, "wb"))
    {
        strerror_s(err, sizeof(err), errno);
        sprintf_s(msg, sizeof(msg), "复制：%s失败\n原因：%s\n", source, err);
        print_text(msg, TRUE, DRAKRED);
        return FALSE;
    }
    buf = (char*)malloc(bufSize);
    if (!buf)
        return FALSE;
    memset(buf, 0, bufSize);
    sprintf_s(msg, sizeof(msg), "正在复制：%s\n", source);
    print_text(msg, FALSE, DRAKBLUE);
    while ((readLen = fread(buf, sizeof(char), bufSize, fpRead)) != 0)
    {
        curSize += readLen;
        if (!fwrite(buf, sizeof(char), readLen, fpWrite))
        {
            strerror_s(err, sizeof(err), errno);
            sprintf_s(msg, sizeof(msg), "复制：%s失败\n原因：%s\n", source, err);
            print_text(msg, TRUE, DRAKRED);
            fclose(fpRead);
            fclose(fpWrite);
            free(buf);
            return FALSE;
        }
        // 打印进度条
        if (printProgress == TRUE && totalSize != -1)
        {
            double curProg = curSize / totalSize * 100; // 当前进度
            print_progress('>', (int)((curProg / 2) + 0.5), DRAKGREEN);
            printf("%.2lf%% ", curProg);// 打印百分比
            print_size(curSize, totalSize);// 打印已复制大小
            putchar('\r');
        }
    }
    /*  if (totalSize != -1)
          putchar('\n');*/
    free(buf);
    fclose(fpRead);
    fclose(fpWrite);
    return TRUE;
}

// 创建目录（支持多级目录）
BOOL create_dir(const char* const dirName)
{
    char msg[MAX_PATH], err[MAX_PATH], tmpPath[MAX_PATH];
    const char* pCur = dirName;
    memset(tmpPath, 0, sizeof(tmpPath));
    int pos = 0;

    // 如果目录存在
    if (_access(dirName, 0) != -1)
    {
        sprintf_s(msg, sizeof(msg), "跳过创建：%s", dirName);
        print_text(msg, TRUE, DRAKRED);
        return TRUE;
    }
    while (*pCur++) // 创建多级目录
    {
        tmpPath[pos++] = *(pCur - 1);
        if (*pCur == '/' || *pCur == '\0')
        {
            if (_access(tmpPath, 0) != 0 && strlen(tmpPath) > 0)
            {
                if (_mkdir(tmpPath) == -1)
                {
                    strerror_s(err, sizeof(err), errno);
                    sprintf_s(msg, sizeof(msg), "创建目录：%s失败\n原因：%s", dirName, err);
                    print_text(msg, TRUE, RED);
                    return FALSE;
                }
            }
        }
    }
    sprintf_s(msg, sizeof(msg), "创建目录：%s", dirName);
    print_text(msg, TRUE, YELLOW);
    return TRUE;
}

// 读取排除复制的文件名列表
char** read_excluded_filelist(const char* const listName)
{
    FILE* fp = NULL;
    char** fileList = NULL;
    char buf[MAX_PATH];
    int line = 0, i = 0;
    size_t size;

    if (!listName)
        return NULL;
    if (fopen_s(&fp, listName, "r"))
        return NULL;
    if (!fp)
        return NULL;
    while (!feof(fp))// 统计文件行数
    {
        fgets(buf, sizeof(buf), fp);
        line++;
        memset(buf, 0, sizeof(buf));
    }
    fseek(fp, 0, SEEK_SET);// 重置文件指针
    fileList = (char**)calloc((size_t)line + 1, sizeof(char*));
    if (!fileList)
    {
        fclose(fp);
        return NULL;
    }
    while (!feof(fp))
    {
        fgets(buf, sizeof(buf), fp);
        del_last_ch(buf, '\n');
        size = strlen(buf) + 1;
        *fileList = (char*)calloc(size, sizeof(char));
        if (!(*fileList))
        {
            fclose(fp);
            return NULL;
        }
        strcpy_s(*fileList, size, buf);
        memset(buf, 0, sizeof(buf));
        fileList++;
        i++;
    }
    // 让指针指向开始的地址
    while (i--)
        fileList--;
    return fileList;
}

void free_excluded_filelist(char** fileList)
{
    if (!fileList)
        return;
    char** list = fileList;
    while (*list)
    {
        free(*list);
        list++;
    }
    free(fileList);
}

// 给定一个路径，判断是文件还是目录
int is_file(const char* const path)
{
    struct _stat info;
    if (_stat(path, &info) == -1)
        return ISFILE_INVALID;
    if (_S_IFDIR & info.st_mode)
        return ISFILE_DIR;
    else if (_S_IFREG & info.st_mode)
        return ISFILE_FILE;
    else
        return ISFILE_UNKNOWN;
}

// 返回获取当前exe所在的目录路径，路径结尾不包括“\\”
void get_module_dir(char* ret_dirpath, size_t size)
{
    if (!ret_dirpath || size == 0)
        return;
    char curpath[MAX_PATH], drive[MAX_PATH], dir[MAX_PATH];
    GetModuleFileNameA(NULL, curpath, sizeof(curpath));
    _splitpath_s(curpath, drive, sizeof(drive), dir, sizeof(dir), NULL, 0, NULL, 0);
    del_last_ch(dir, '\\');
    sprintf_s(ret_dirpath, size, "%s%s", drive, dir);
}

// 在一个字符串中查找另一个字符串末尾的字符
char* find_endofstr(const char* str, const char* subStr)
{
    char* find = strstr(str, subStr);
    return (find) ? find + strlen(subStr) - 1 : NULL;
}

void print_err(const char* const msg)
{
    char err[MAX_PATH], errmsg[MAX_PATH * 2];
    strerror_s(err, sizeof(err), errno);
    if (msg != NULL)
        sprintf_s(errmsg, sizeof(errmsg), "%s\n%s\n", msg, err);
    else
        sprintf_s(errmsg, sizeof(errmsg), "%s\n", err);
    print_text(errmsg, FALSE, RED);
}

