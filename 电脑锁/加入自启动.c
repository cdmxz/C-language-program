#include <stdio.h>
#include <windows.h>
#include <direct.h>
#define MAXPATH 1024

int main(void)
{
	char *szSubKey = { "Software\\Microsoft\\Windows\\CurrentVersion\\Run" }; //ϵͳ������·��
	char copy[MAXPATH] = { "copy \"" };//copy������ļ�
	char buffer[MAXPATH];//���浱ǰ����·��
	char path[MAXPATH] = { "\\������.exe\" C:\\ProgramData /y" };//Ŀ���ļ���·��
	char *pclock = { "C:\\ProgramData\\������.exe" };//ע�������������·��
	
	HKEY hKey;
	int i;
	system("title ��������� && mode con cols=33 lines=10"); 
	printf("\n\t����1���������\n\t����2�鿴ʹ�ð���\n\t����3ɾ��������\n\n");
	printf("��������ţ�");
	while (scanf_s("%d", &i) != 1)//�����ַ�����
	{
		printf("\n������������ԣ�");
		rewind(stdin);
	}
	rewind(stdin);
	
	switch (i)
	{
	case 1:
	{
		system("cls");
		system("color 04");
		printf("\n\n\n\nע�⣺���⵽ɱ�����أ��������������������\n��������޷�����������\n\n\n");
		Sleep(3000);
		_getcwd(buffer, MAXPATH);//��ȡ��ǰ����ľ���·��
		strcat_s(copy,sizeof(copy), buffer);//��һ�κϲ�����
		strcat_s(copy, sizeof(copy), path);//�ڶ��κϲ�����
		system(copy);//ִ�и����ļ�����

		//��ע��������� 
		if (RegOpenKeyEx(HKEY_CURRENT_USER, szSubKey, 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)
		{  //���һ����Key,������ֵ
			RegSetValueEx(hKey, "������", 0, REG_SZ, (BYTE *)pclock, strlen(pclock));
			RegCloseKey(hKey);//�ر�ע���
			printf("\t\t��ӳɹ���\n\n\t");
			system("pause"); 
			return 0;
		}
		else
		{
			printf("\t\t���ʧ�ܣ�\n\n\t");
			system("pause"); 
			return -1;
		}
		break;
	}

	case 2:
		MessageBox(NULL, TEXT("1�����ÿ����Զ�����ǰ���Թ���ԱȨ�����б������ֹȨ�޲������ʧ�ܡ�\n2�����ÿ����Զ�����ǰ��ȷ��������������.exe���͡�������.exe����ͬһĿ¼��\n3�������������ʧЧ������ɱ������������������������������������������"), TEXT("ʹ�ð���"), MB_OK);
		break;

	case 3://���ļ���ɾ���ļ�
	{
		system("del \"C:\\ProgramData\\������.exe\"");
		system("pause");
		break;
	}
	default:printf("\n\t\t �������"); Sleep(1000); break;
	}
	return 0;
} 
