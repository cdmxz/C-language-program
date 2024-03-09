#include "str_util.h"
//将wchar转为char
char* wchar_to_char(const wchar_t* w_str, char* c_str, int dest_size)
{
	char* defaultChar = "*";
	WideCharToMultiByte(CP_ACP, 0, w_str, -1, c_str, dest_size, defaultChar, NULL);
	return c_str;
}

//将char转为wchar
wchar_t* char_to_wchar(const char* c_str, wchar_t* w_str, int dest_size)
{
	MultiByteToWideChar(CP_ACP, 0, c_str, -1, w_str, dest_size);// 将char转为TCHAR  
	return w_str;
}

// 删除末尾的换行符
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

// 删除末尾的换行符
void del_last_newline_w(wchar_t* str)
{
	wchar_t* find = wcschr(str, L'\n');
	if (find)
		*find = '\0'; 
}

void del_lastch(char* str, char ch)
{
	char* p = str;
	while (*p)
		p++;
	p--;
	while (p != str)
	{
		if (*p == ch)
		{
			*p = '\0';
			break;
		}
		p++;
	}
}

// Path: str_util.h