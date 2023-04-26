#include "str_util.h"
//��wcharתΪchar
char* wchar_to_char(const wchar_t* w_str, char* c_str, int dest_size)
{
	char* defaultChar = "*";
	int size = WideCharToMultiByte(CP_ACP, 0, w_str, -1, c_str, dest_size, defaultChar, NULL);
	return c_str;
}

//��charתΪwchar
wchar_t* char_to_wchar(const char* c_str, wchar_t* w_str, int dest_size)
{
	MultiByteToWideChar(CP_ACP, 0, c_str, -1, w_str, dest_size);// ��charתΪTCHAR  
	return w_str;
}

// ɾ��ĩβ�Ļ��з�
void del_last_newline(char* str)
{
	while (*str != '\0')
	{
		str++;
	}
	str--;
	if (*str == '\n')
		*str = '\0';
}

// ɾ��ĩβ�Ļ��з�
void del_last_newline_w(wchar_t* str)
{
	wchar_t* find;
	if ((find = wcschr(str, L'\n')))
		*find = '\0';
}

// Path: str_util.h