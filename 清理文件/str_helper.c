#include <string.h>
#include "str_helper.h"

// �Ӳ����ַ������ұߵ�λ��
char* mystrrstr(const char* str, const char* substr)
{
	char* cur;
	char* last = NULL;
	if (!str || !substr)
		return NULL;
	cur = strstr(str, substr);
	while (cur)
	{// ���ϲ����ַ������ظ����ִ���ֱ���ҵ����һ���ظ����Ӵ�Ϊֹ
		last = cur;
		cur = strstr(last + 1, substr);
	}
	return last;
}


// ɾ����β���ַ�
char* trim_end(char* str, char ch)
{
	size_t len = strlen(str);
	if (str[len - 1] == ch)
		str[len - 1] = '\0';
	return str;
}