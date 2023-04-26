#include "clean_file.h"

void print_err(const char* const msg)
{
	print_err_format("%s", msg);
}

void print_err_format(const char* const format, ...)
{
	va_list args = NULL;
	va_start(args, format);
	LPSTR buf = NULL;
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPSTR)&buf,
		0,
		NULL
	);
	print_valist(DRAKRED, format, args);
	print(DRAKRED, "%s\n", buf);
	LocalFree(buf);
}

// ��ָ����ɫ��ӡ�ı�
void print(WORD color, const char* const format, ...)
{
	va_list args = NULL;
	va_start(args, format);
	print_valist(color, format, args);
}

void print_valist(WORD color, const char* const format, va_list args)
{
	// ����������ɫ
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
	vprintf(format, args);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
}