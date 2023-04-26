#include <stdio.h>
#include <windows.h>
#include <stdbool.h>

enum Color
{
	WHITE = FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
	DRAKWHITE = FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
	DRAKBLUE = FOREGROUND_BLUE | FOREGROUND_INTENSITY,
	YELLOW = FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY,
	DRAKRED = FOREGROUND_RED | FOREGROUND_INTENSITY,
	RED = FOREGROUND_RED,
	DRAKGREEN = FOREGROUND_INTENSITY | FOREGROUND_GREEN
};

// **************** error_print.c ****************
// 以指定颜色打印文本
void print(WORD color, const char* const format, ...);
void print_valist(WORD color, const char* const format, va_list args);
// 打印错误
void print_err(const char* const msg);
void print_err_format(const char* const format, ...);

// **************** main.c ****************
// 获取文件扩展名
void get_ext(const char* filename, char* ext, int ext_len);
void del_file(const char* filename);
void del_dir(const char* dir);
bool is_empty_dir(const char* dir);
void get_dirname(const char* fullpath, char* buf, int buf_len);
// 是否包含指定的扩展名
bool is_contain_ext(const char** exts, const char* filename);
void delete_files(const char* dir, const char** exts, bool del_emptydir);

// 获取文件大小
long long get_filesize(char* filename);