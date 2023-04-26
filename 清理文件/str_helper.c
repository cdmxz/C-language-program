#include <string.h>
#include "str_helper.h"

// 从查找字符串最右边的位置
char* mystrrstr(const char* str, const char* substr)
{
	char* cur;
	char* last = NULL;
	if (!str || !substr)
		return NULL;
	cur = strstr(str, substr);
	while (cur)
	{// 不断查找字符串中重复的字串，直到找到最后一个重复的子串为止
		last = cur;
		cur = strstr(last + 1, substr);
	}
	return last;
}


// 删除结尾的字符
char* trim_end(char* str, char ch)
{
	size_t len = strlen(str);
	if (str[len - 1] == ch)
		str[len - 1] = '\0';
	return str;
}