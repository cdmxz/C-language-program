#pragma once
#include <windows.h>
//将char转为wchar
wchar_t* char_to_wchar(const char* c_str, wchar_t* w_str, int dest_size);
//将wchar转为char
char* wchar_to_char(const wchar_t* w_str, char* c_str, int dest_size);
// 删除末尾的换行符
void del_last_newline(char* str);
// 删除末尾的换行符
void del_last_newline_w(wchar_t* str); 
void del_lastch(char* str, char ch);
