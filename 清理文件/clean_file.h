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
// ��ָ����ɫ��ӡ�ı�
void print(WORD color, const char* const format, ...);
void print_valist(WORD color, const char* const format, va_list args);
// ��ӡ����
void print_err(const char* const msg);
void print_err_format(const char* const format, ...);

// **************** main.c ****************
// ��ȡ�ļ���չ��
void get_ext(const char* filename, char* ext, int ext_len);
void del_file(const char* filename);
void del_dir(const char* dir);
bool is_empty_dir(const char* dir);
void get_dirname(const char* fullpath, char* buf, int buf_len);
// �Ƿ����ָ������չ��
bool is_contain_ext(const char** exts, const char* filename);
void delete_files(const char* dir, const char** exts, bool del_emptydir);

// ��ȡ�ļ���С
long long get_filesize(char* filename);