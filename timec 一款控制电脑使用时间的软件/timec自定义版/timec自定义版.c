#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <direct.h>
#include <io.h>

#define MAXPATH 512
#define SIZE 6

int  pass(void);     //��֤����
void shut(void);     //�Զ���ػ�
void open_readmetxt(void); //�鿴ʹ��˵��
void hwn(void);      //���ش���
void countDown(void);//������ʾ��
int  hyke(void);     //ע���
void flush(void);    //���������
//�����ļ�
int  copyfile(char *fileread, char *filewrite);
void passwd(void);

int main(void)
{
	int time;

	while (1)
	{
		system("color 07 && title timec�Զ���� && mode con cols=50 lines=20");//������ɫ ���� ���ڴ�С
		
		printf("\t   ***��ӭʹ��timec�Զ����***\n");
		printf("\t\t 0��ȡ���ػ�\n");
		printf("\t\t 1��10���Ӻ�ػ�\n");
		printf("\t\t 2��30���Ӻ�ػ�\n");
		printf("\t\t 3��50���Ӻ�ػ�\n");
		printf("\t\t 4��60���Ӻ�ػ�\n");
		printf("\t\t 5��80���Ӻ�ػ�\n");
		printf("\t\t 6��100���Ӻ�ػ�\n");
		printf("\t\t 7���Զ���ػ�ʱ��\n");
		printf("\t\t 8���鿴ʹ��˵��\n");
		printf("\t\t 9�����������\n");
		printf("\t\t 10���˳�����\n\n");
		//printf("��ܰ��ʾ��������ź��밴�س���\n");
		printf("\n\t��������ţ�");

		while (scanf_s("%d", &time) != 1)//�����ַ�����
		{
			printf("\n������������ԣ�");
			flush();//��ջ����������ַ�
		}
		flush();
		
		
		switch (time)
		{
		case 0:pass(); break;
		case 1:system("shutdown -s -t 600"); hwn(); countDown(); break;
		case 2:system("shutdown -s -t 1800"); hwn(); Sleep(1200000); countDown(); break;
		case 3:system("shutdown -s -t 3000"); hwn(); Sleep(2400000); countDown(); break;
		case 4:system("shutdown -s -t 3600"); hwn(); Sleep(3000000); countDown(); break;
		case 5:system("shutdown -s -t 4800"); hwn(); Sleep(4200000); countDown(); break;
		case 6:system("shutdown -s -t 6000"); hwn(); Sleep(5400000); countDown(); break;
		case 7:shut(); break;
		case 8:open_readmetxt(); break;
		case 9:hyke(); break;
		case 10:exit(EXIT_SUCCESS); break;
		}
	}

	return 0;
}



int pass(void)//������֤
{
	system("title ���������룬������1234�޸ġ� && cls");

	int i;
	FILE *fpwrite, *fpread;
	errno_t err;
	char pass[] = { "123456" };//����Ĭ������
	char newpass[7] = { 0 };//����������
	char tmppass[7] = { 0 };//��ʱ��������


	printf("       ��ܰ��ʾ�����������룬������1234�޸�\n\n\n\n\n\n");
	printf("��Ҫȡ���ػ������������룺");
	
	scanf_s("%s",tmppass,7);//���뵱ǰ����
	flush();


	if (!strcmp(tmppass, "1234"))//�������������бȽ�
	{
		system("title Ĭ��������ڽ����ļ��в��� && cls");
		
		printf("       ��ܰ��ʾ��Ĭ��������ڽ����ļ��в���\n\n\n\n\n\n");
		printf("������Ĭ�����루6λ������");
		
		scanf_s("%s", &tmppass, 7);//����Ĭ������
		flush();
		
		
		if (!strcmp(tmppass, pass))//�������������Ĭ��������бȽ�
		{
			while (1)
			{
				printf("\n\n�����������루6λ������");
				
				scanf_s("%s", &newpass, 7);//����������
				flush();
				
				
				if (strlen(newpass) < SIZE)//�ж������볤��
					printf("\n\n���������̫����̫�̣���Ҫ6λ���֣���");
				else
					break;
			}
			
			if ((err = fopen_s(&fpwrite,"passwd.dat", "w")) != 0)//�򿪼�¼������ļ�,�����ж��ļ��Ƿ�������
			{
				MessageBox(NULL, TEXT("�����ļ���������\n���Թ���ԱȨ���������ԣ�"), TEXT("ERROR"), MB_OK | MB_ICONERROR);//������ʾ��
				exit(EXIT_FAILURE);
			}
			
			for (i = 0; i <= 6;i++)  //��������д���ļ�
			{
				newpass[i] = newpass[i] - 't' + 'i' - 'm' / 'e';//�����ļ�����
				fputc(newpass[i], fpwrite);//������ܺ�����ݵ��ļ�
			}

			if (fclose(fpwrite) == EOF)//�ر��ļ�
			{
				MessageBox(NULL, TEXT("�ļ�д�����\n�����Ƿ�����������ռ�ã�\n�������ԣ�"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
				return 1;
			}
		}
		else
		{
			printf("\n\n\t\t�������");
			Sleep(1000);
		}

	}
	else//ȡ���ػ�
	{

		if ((err = fopen_s(&fpread, "passwd.dat", "r")) != 0)//�򿪼�¼������ļ�,�����ж��ļ��Ƿ�������
		{
			MessageBox(NULL, TEXT("�����ļ���ȡ����\n�������Ŀ¼�Ƿ��С�passwd.dat���ļ������û�У����޸����롣"), TEXT("ERROR"), MB_OK | MB_ICONERROR);//������ʾ��
			return 1;
		}


		for (i = 0; i <= 6; i++)  //���ļ��ж�ȡ���ݵ�ch��EOF���ļ�������־��
		{
			newpass[i] = fgetc(fpread);//���ļ���ȡ����
			newpass[i] = newpass[i] + 't' - 'i' + 'm' * 'e';//�����ļ����ݲ��浽����
		}

		if (!strcmp(tmppass, newpass))//������������������е�������бȽ�
			system("shutdown -a");
		else
		{
			printf("\n\n\t\t�������");
			Sleep(1000);
		}
		fclose(fpread); //�ر��ļ�
	
	}
	return 0;
}



//�鿴ʹ��˵��
void open_readmetxt(void)
{
	char path[MAXPATH];

	_getcwd(path, MAXPATH);//��ȡ�ļ�·��
	strcat_s(path, MAXPATH, "\\readme.txt");
	if (_access(path, 0))//�жϡ�readme.txt���Ƿ���ڡ������������ַ��������ֱ�Ӵ��ļ�
		system("start https://github.com/cdmxz/C-language-program/blob/master/timec%20%E4%B8%80%E6%AC%BE%E6%8E%A7%E5%88%B6%E7%94%B5%E8%84%91%E4%BD%BF%E7%94%A8%E6%97%B6%E9%97%B4%E7%9A%84%E8%BD%AF%E4%BB%B6/readme.txt");
	else
		system("start readme.txt");
}



void hwn(void)//���ش���
{
	HWND hwnd;
	hwnd = FindWindow("ConsoleWindowClass", NULL);
	if (hwnd)
		ShowWindow(hwnd, SW_HIDE);
}


void countDown(void)
{
	MessageBox(NULL, TEXT("���ĵ��Ի���10���Ӿ�Ҫ�ػ���\n�����ùػ�׼����"), TEXT("��ܰ��ʾ"), MB_OK | MB_ICONWARNING);
}



//�Զ���ػ�
void shut(void)
{
	char shut[25] = { "shutdown -s -t " };
	char tmp[7] = { 0 };
	system("cls");
	printf("\nע�⣺Ŀǰ�Զ���ʱ��ػ����ܲ����ڹػ�ǰ������ʾ��,");
	printf("��ע��鿴ϵͳ��ʾ���������ʱ�䲻�ܳ���99999�롣\n\n\n\n\n");
	printf("��ܰ��ʾ��ֻ���������ּ��ɣ��������뵥λ\n");
	printf("\t  ������������ػ�����λ���룩��");
	
	scanf_s("%s", tmp ,7);
	flush();

	strcat_s(shut, 25, tmp);//�ϲ�����
	
	system(shut);//ִ�йػ�����
	Sleep(2000);
	exit(EXIT_SUCCESS);
}



//���ע���
int hyke(void)
{
	system("cls && title ��������� PS������7����������");

	char *szSubKey = { "Software\\Microsoft\\Windows\\CurrentVersion\\Run" }; //ϵͳ������·��
	char *del = { "Software\\Microsoft\\Windows\\CurrentVersion\\Run\\timecyincang" };
	char folderName[] = { "C:\\Program Files\\timec" };//�����ļ��е�·��
	char buffer[MAXPATH];//���浱ǰ����·��
	
	//ע���������·���Լ�Ҫ���Ƶ���·��
	char *timec_Y = { "C:\\Program Files\\timec\\timec���ذ�.exe" };  
	char *timec_Z = { "C:\\Program Files\\timec\\timec�Զ����.exe" };

	_getcwd(buffer, MAXPATH);//��ȡ��ǰ����·��
	
	HKEY hKey;
	int i;


	printf("\n\t����3�鿴ʹ�ð���������4ɾ��������\n\n");
	printf("������Ҫ����������İ汾��\n\n");
	printf("\t��1.timec���ذ� 2.timec�Զ���棩\n\n\n\n");
	printf("    ��������ţ�");
	
	while (scanf_s("%d", &i) != 1)//�����ַ�����
	{
		printf("\n������������ԣ�");
		flush();
	}
	flush();


	switch (i)
	{
	case 1:
	{
		system("taskkill /f /t /im timec���ذ�.exe");//�ڸ����ļ�ǰ����timec���ذ�.exe�Ľ��̣�����ʧ��
		system("color 04 && cls");

		printf("\n\n\n\nע�⣺���⵽ɱ�����أ��������������������\n\n��������޷�����������\n\n\n");
		Sleep(3000);

		if (_access(folderName, 0))//�ж��ļ����Ƿ����
		if (_mkdir(folderName))	   //�������򴴽��ļ���
		{
			MessageBox(NULL, TEXT("����Ŀ¼ʧ�ܣ����Թ���ԱȨ�����ԣ�"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
			exit(EXIT_FAILURE);
		}

		strcat_s(buffer, MAXPATH, "\\timec���ذ�.exe");//����ǰ��·��
		copyfile(buffer, timec_Y);//�����ļ�
		
		SetFileAttributes(timec_Y, FILE_ATTRIBUTE_HIDDEN);//����Ϊ�����ļ�
		SetFileAttributes(folderName, FILE_ATTRIBUTE_HIDDEN);//����Ϊ�����ļ�

		//��ע��������� 
		if (RegOpenKeyEx(HKEY_CURRENT_USER, szSubKey, 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)
		{  //���һ����Key,������ֵ
			RegSetValueEx(hKey, "timecyincang", 0, REG_SZ, (BYTE *)timec_Y, strlen(timec_Y));
			RegCloseKey(hKey);//�ر�ע���
			
			printf("\t\t    ��ӳɹ���\n\n\t         ");
			system("pause");
			return 0; 
		}
		else
		{
			MessageBox(NULL, TEXT("���ʧ�ܣ�"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
			remove("C:\\Program Files\\timec\\timec���ذ�.exe");
			return 1;
		}
		break;
	}

	case 2:
	{

		//�˴����ϴ�Դ���������ͬ���ʲ�дע�͡���ʵ��������~
		system("color 04 && cls");
		
		printf("\n\n\n\nע�⣺���⵽ɱ�����أ��������������������\n\n��������޷�����������\n\n\n");
		Sleep(3000);

		if (_access(folderName, 0))//�ж��ļ����Ƿ����
			if (_mkdir(folderName))			//�������򴴽��ļ���
			{
				MessageBox(NULL, TEXT("����Ŀ¼ʧ�ܣ����Թ���ԱȨ�����ԣ�"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
				exit(EXIT_FAILURE);
			}
		SetFileAttributes(timec_Z, FILE_ATTRIBUTE_HIDDEN);//����Ϊ�����ļ�
		SetFileAttributes(folderName, FILE_ATTRIBUTE_HIDDEN);//����Ϊ�����ļ�

		strcat_s(buffer, MAXPATH, "\\timec�Զ����.exe");//����ǰ��·��
		copyfile(buffer, timec_Z);//�����ļ�

		if (RegOpenKeyEx(HKEY_CURRENT_USER, szSubKey, 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)
		{
			RegSetValueEx(hKey, "timeczidingyi", 0, REG_SZ, (BYTE *)timec_Z, strlen(timec_Z));
			RegCloseKey(hKey);
			
			printf("\t\t    ��ӳɹ���\n\n\t         ");
			system("pause");
			return 0;
		}
		else
		{
			MessageBox(NULL, TEXT("���ʧ�ܣ�"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
			remove("C:\\Program Files\\timec\\timec�Զ����.exe");
			return 1;
		}
		break;
	}

	case 3:
		MessageBox(NULL, TEXT("1�����ÿ����Զ�����ǰ���Թ���ԱȨ�����б����\n��ֹȨ�޲������ʧ�ܡ�\n2�����ÿ����Զ�����ǰ��ȷ����timec���ذ桱��\n��timec�Զ���桱��ͬһĿ¼��\n3�����ÿ����Զ�����ǰ��ȷ����timec���ذ桱��\n��timec�Զ���桱��ԭ�ļ���δ���޸ġ�\n4�������������ʧЧ����鿴��C:\\Program Files\\time��Ŀ¼����û��\n��timec�Զ���� ����timec���ذ桱������\n���߼���������Ȩ���Ƿ�ɱ������ˡ�\n5���ڵ�ǰҳ������4ɾ����������\n6����������������޸�ϵͳע���\n���ܻ���ɵ��Բ��ȶ��������ã�\n7��ʹ�ñ�����������κκ��������ԭ���߳е�\n��һ��ʹ�ñ��������Ϊͬ������\nע�������������κθ��£�ˡ������֪ͨ��"), TEXT("ʹ�ð���"), MB_OK);
		break;

	case 4://���ļ���ɾ���ļ�
	{
		passwd();//��֤����
		system("cls");
		printf("\n\n\n\n\n\nע�⣺���⵽ɱ�����أ��������������������\n\n\n");
		Sleep(3000);
		system("cls && mode con cols=53 lines=30 && title ����ɾ��...");

		//��������
		printf("\n���ڽ���timec���ذ�Ľ���...\n");
		Sleep(2000);
		system("taskkill /f /t /im timec���ذ�.exe");
		
		//ɾ������
		printf("\n\n����ɾ���ļ�...");
		if (remove("C:\\Program Files\\timec\\timec���ذ�.exe"))
			printf("\nɾ��timec���ذ�ʧ�ܣ�\n��δ���timec���ذ��������ɲ���ᡣ\n\n");
		else
			printf("\nɾ��timec���ذ�ɹ���\n\n");

		if (remove("C:\\Program Files\\timec\\timec�Զ����.exe"))
			printf("ɾ��timec�Զ����ʧ�ܣ�\n��δ���timec�Զ�����������ɲ���ᡣ\n\n");
		else
			printf("ɾ��timec�Զ����ɹ���\n\n");

		//ɾ�������ļ�
		printf("\n����ɾ�������ļ�...\n");
		if (remove("C:\\Users\\Timec_config.txt"))
			printf("ɾ�������ļ�ʧ�ܣ�\n���ֶ����롰C:\\Users��Ŀ¼ɾ����Timec_config.txt����\n\n");
		else
			printf("ɾ�������ļ��ɹ���\n\n");

		//ɾ��Ŀ¼
		printf("\n����ɾ����װĿ¼...\n");
		system("rd /s /q C:\\\"Program Files\"\\timec");
		
		//ɾ��ע���
		printf("\n\n����ɾ��timec���ذ�ע���������...\n");
		system("reg delete \"HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run\"  /v timecyincang /f");
		printf("\n����ɾ��timec�Զ����ע���������...\n");
		system("reg delete \"HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run\"  /v timeczidingyi /f");
		
		putchar('\n');
		system("pause");
		break;
	}

	case 7:break;

	default:printf("\n\t\t �������"); Sleep(1000); break;
	}
	return 0;
}


//���������
void flush(void)
{
	int ch;
	while ((ch = getchar()) != '\n' && ch != EOF)
		;
}


//�����ļ�
int copyfile(char *fileread, char *filewrite)
{
	FILE *fpread, *fpwrite;  
	errno_t err;
	int ch; 
	
	
	if ((err = fopen_s(&fpread, fileread, "rb")) != 0 )//�Զ����Ʒ�ʽ���ļ�
	{
		MessageBox(NULL, TEXT("�޷������ļ���\n���顰timec�Զ����.exe���͡�timec���ذ�.exe���Ƿ���ͬһĿ¼��\n"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
		exit(EXIT_FAILURE);
	}

	if ((err = fopen_s(&fpwrite, filewrite, "wb")) != 0)
	{
		//MessageBox(NULL, TEXT("�޷������ļ������Թ���ԱȨ�����ԣ�\n"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
		perror("Error");
		exit(EXIT_FAILURE);
	}

	do
	{
		ch = fgetc(fpread);
		fputc(ch, fpwrite);
	} while (ch != EOF);

	fclose(fpread);//�ر��ļ�
	fclose(fpwrite);
	return 0;
}


void passwd(void)//ɾ����������֤����
{
	FILE *fpread;
	errno_t err;

	int i;
	char newpass[7] = { 0 };//����������
	char tmppass[7] = { 0 };//��ʱ��������

	while (1)
	{
		system("cls && title ɾ������������Ҫ������ȷ������");

		printf("\n\t\t  ����1234�˳���\n         ���������룬��������������0���ġ�");
		printf("\n\n\nɾ������������Ҫ�������롣\n\n��������ȷ�����룺");
		scanf_s("%s", tmppass, 7);
		flush();

		if (!strcmp(tmppass, "1234"))//����1234�˳�
			exit(EXIT_SUCCESS);

		if ((err = fopen_s(&fpread, "passwd.dat", "r")) != 0)//�򿪼�¼������ļ�,�����ж��ļ��Ƿ�������
		{
			MessageBox(NULL, TEXT("�����ļ���ȡ����\n�������Ŀ¼�Ƿ��С�passwd.dat���ļ������û�У����޸����롣"), TEXT("ERROR"), MB_OK | MB_ICONERROR);//������ʾ��
			exit(EXIT_SUCCESS);
		}

		for (i = 0; i <= 6; i++)  //���ļ��ж�ȡ���ݵ�ch��EOF���ļ�������־��
		{
			newpass[i] = fgetc(fpread);//���ļ���ȡ����
			newpass[i] = newpass[i] + 't' - 'i' + 'm' * 'e';//�����ļ����ݲ��浽����
		}

		if (!strcmp(tmppass, newpass))//������������������е�������бȽ�
			break;
		else
		{
			printf("\n\n\t\t�������");
			Sleep(1000);
		}

		fclose(fpread); //�ر��ļ�
		system("cls");
	}

}
