#include "arr_helper.h"

int compare(const void* arg1, const void* arg2);

// 比较函数
int compare(const void* arg1, const void* arg2)
{
	char* a1 = *(char**)arg1;
	char* a2 = *(char**)arg2;
	return _strcmpi(*(char**)arg1, *(char**)arg2);
}

char* arr_findfirst(const char** arr, unsigned arr_len, const char* elem)
{
	char** result = NULL;
	// 函数指针
	int (*call)(const void*, const void*) = compare;
	result = (char**)_lfind(&elem, arr, &arr_len, sizeof(char*), call);
	if (result)
		return *result;
	return NULL;
}

bool is_contian(const char** arr, unsigned arr_len, const char* elem)
{
	if (arr == NULL || elem == NULL || arr_len == 0)
		return false;
	char* p = arr_findfirst(arr, arr_len, elem);
	return p;// 非0值为true，0为false
}
