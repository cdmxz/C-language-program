#include <stdio.h>
#include <windows.h>
#include <direct.h>
#define MAXPATH 512

void copyfile(char *fileread, char *filewrite);
void flush(void);

int main(void)
{
	while(1)
	{
	system("title ��������� && mode con cols=35 lines=10 && color 07"); 
	char *szSubKey = { "Software\\Microsoft\\Windows\\CurrentVersion\\Run" }; //ϵͳ������·��
	char *del = { "Software\\Microsoft\\Windows\\CurrentVersion\\Run\\������" };
	char folderName[] = { "C:\\Program Files\\������" };//�����ļ��е�·��
	char buffer[MAXPATH];//���浱ǰ����·��
	
	//ע���������·���Լ�Ҫ���Ƶ���·��
	char *pc_lock = { "C:\\Program Files\\������\\������.exe" };  
	
	HKEY hKey;
	int i;
	
	_getcwd(buffer, MAXPATH);//��ȡ��ǰ��������Ŀ¼
		
	printf("\n\t  ����1���������\n\n\t  ����2�鿴ʹ�ð���\n\n\t  ����3ɾ��������\n\n");
	printf("��������ţ�");
	while (scanf_s("%d", &i) != 1)//�����ַ�����
	{
		printf("\n������������ԣ�");
		flush(); 
	}
	flush(); 
	
	
	if(i == 1)
	{
		system("color 04 && cls");
		printf("\n\nע�⣺���⵽ɱ�����أ��������������������\n��������޷�����������\n\n");
		Sleep(3000);
		
		if (_access(folderName, 0))//�ж��ļ����Ƿ����
			if (_mkdir(folderName))	   //�������򴴽��ļ���
			{
				MessageBox(NULL, TEXT("����Ŀ¼ʧ�ܣ����Թ���ԱȨ�����ԣ�"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
				exit(EXIT_FAILURE);
			}

		strcat_s(buffer, MAXPATH, "\\������.exe");//����ǰ��·��
		copyfile(buffer, pc_lock);//�����ļ�

		//��ע��������� 
		if (RegOpenKeyEx(HKEY_CURRENT_USER, szSubKey, 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)
		{  //���һ����Key,������ֵ
			RegSetValueEx(hKey, "������", 0, REG_SZ, (BYTE *)pc_lock, strlen(pc_lock));
			RegCloseKey(hKey);//�ر�ע���
			printf("\t    ��ӳɹ���\n\n\t  ");
			system("pause"); 
		}
		else
		{
			printf("\t\t���ʧ�ܣ�\n\n\t");
			system("pause"); 
			return -1;
		}
	
	}

	else if(i == 2)
		MessageBox(NULL, TEXT("1�����ÿ����Զ�����ǰ���Թ���ԱȨ�����б������ֹȨ�޲������ʧ�ܡ�\n2�����ÿ����Զ�����ǰ��ȷ��������������.exe���͡�������.exe����ͬһĿ¼��\n3�������������ʧЧ������ɱ������������������������������������������"), TEXT("ʹ�ð���"), MB_OK);

	else if(i == 3)//���ļ���ɾ���ļ�
	{
		system("cls && title ɾ��������");
		printf("\n\n\n\nע�⣺���⵽ɱ�����أ��������������������\n\n\n");
		Sleep(3000);
		system("cls && mode con cols=53 lines=20 && title ����ɾ��...");

		//��������
		printf("\n���ڽ����������Ľ���...\n");
		Sleep(2000);
		system("taskkill /f /t /im ������.exe");
		
		//ɾ���ļ�
		printf("\n\n����ɾ���ļ�...");
		if (remove(pc_lock))
			printf("\nɾ��������.exeʧ�ܣ�\n\n");
		else
			printf("\nɾ��������.exe�ɹ���\n\n");
		
		//ɾ��Ŀ¼
		printf("\n����ɾ����װĿ¼...\n");
		system("rd /s /q C:\\\"Program Files\"\\������");
		
		//ɾ��ע���
		printf("\n\n����ɾ��������ע���������...\n");
		system("reg delete \"HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run\"  /v ������ /f");
		
		putchar('\n');
		system("pause");
	}
	else
	{
		printf("\n\t\t �������"); 
		Sleep(1000); 
	}
}
	
	return 0;
} 

//�����ļ�
void copyfile(char *fileread, char *filewrite)
{
	FILE *fpread;   //����ǰ�ļ���·��
	FILE *fpwrite;  //���ƺ��ļ���·��
	errno_t err;
	int ch; 
	
	
	if ((err = fopen_s(&fpread, fileread, "rb")) != 0 )//�Զ����Ʒ�ʽ���ļ�
	{
		MessageBox(NULL, TEXT("�޷����ļ���\n���顰������.exe���͡�����������.exe���Ƿ���ͬһĿ¼��\n"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
		exit(EXIT_FAILURE);
	}
	if ((err = fopen_s(&fpwrite, filewrite, "wb")) != 0)
	{
		MessageBox(NULL, TEXT("�޷������ļ������Թ���ԱȨ�����ԣ�\n"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
		exit(EXIT_FAILURE);
	}

	do
	{
		ch = fgetc(fpread);
		fputc(ch, fpwrite);
	} while (ch != EOF);

	fclose(fpread);//�ر��ļ�
	fclose(fpwrite);
}


//���������
void flush(void)
{
	int ch;
	while ((ch = getchar()) != '\n' && ch != EOF)
		;
}
