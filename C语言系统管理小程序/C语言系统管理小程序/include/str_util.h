#pragma once
#include <windows.h>
//��charתΪwchar
wchar_t* char_to_wchar(const char* c_str, wchar_t* w_str, int dest_size);
//��wcharתΪchar
char* wchar_to_char(const wchar_t* w_str, char* c_str, int dest_size);
// ɾ��ĩβ�Ļ��з�
void del_last_newline(char* str);
// ɾ��ĩβ�Ļ��з�
void del_last_newline_w(wchar_t* str); 
void del_lastch(char* str, char ch);
