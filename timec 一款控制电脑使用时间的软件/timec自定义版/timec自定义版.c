#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <direct.h>
#include <io.h>

#define SIZE 6
#define MAX_SIZE 600

int  password(void);          //��֤����
void shut_down(void);         //�Զ���ػ�
void open_readme_txt(void);   //�鿴ʹ��˵��
void hwn(void);			      //���ش���
void countdown(void);	      //������ʾ��
int  add_start(void);	      //���������
void flush(void);		      //���������
void fgets_n(char *str);      //���fgets��ȡ��'\n'
void Initialize(char *str1, char *str2, int n);//��ʼ������
int  copyfile(char *fileread, char *filewrite);//�����ļ�
void del_start_password(void);//ɾ����������֤����

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
		printf("\t\t 8���鿴ʹ��˵�� \n");
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
		case 0:password(); break;
		case 1:system("shutdown -s -t 600"); hwn(); countdown(); break;
		case 2:system("shutdown -s -t 1800"); hwn(); Sleep(1200000); countdown(); break;
		case 3:system("shutdown -s -t 3000"); hwn(); Sleep(2400000); countdown(); break;
		case 4:system("shutdown -s -t 3600"); hwn(); Sleep(3000000); countdown(); break;
		case 5:system("shutdown -s -t 4800"); hwn(); Sleep(4200000); countdown(); break;
		case 6:system("shutdown -s -t 6000"); hwn(); Sleep(5400000); countdown(); break;
		case 7:shut_down(); break;
		case 8:open_readme_txt(); break;
		case 9:add_start(); break;
		case 10:exit(EXIT_SUCCESS); break;
		}
	}

	return 0;
}



int password(void)//������֤
{
	system("title ���������룬������1234�޸ġ� && cls");

	int i;
	FILE *fpwrite, *fpread;
	errno_t err;
	char password[] = { "123456" };//����Ĭ������
	char new_password[7] = { 0 };  //����������
	char temp_password[7] = { 0 }; //��ʱ��������


	printf("       ��ܰ��ʾ�����������룬������1234�޸�\n\n\n\n\n\n");
	printf("��Ҫȡ���ػ������������룺");

	scanf_s("%s", temp_password, 7);//���뵱ǰ����
	flush();


	if (!strcmp(temp_password, "1234"))//�������������бȽ�
	{
		system("title Ĭ��������ڽ����ļ��в��� && cls");

		printf("       ��ܰ��ʾ��Ĭ��������ڽ����ļ��в���\n\n\n\n\n\n");
		printf("������Ĭ�����루6λ������");

		scanf_s("%s", &temp_password, 7);//����Ĭ������
		flush();


		if (!strcmp(temp_password, password))//�������������Ĭ��������бȽ�
		{
			while (1)
			{
				printf("\n\n�����������루6λ������");

				scanf_s("%s", &new_password, 7);//����������
				flush();


				if (strlen(new_password) < SIZE)//�ж������볤��
					printf("\n\n���������̫����̫�̣���Ҫ6λ���֣���");
				else
					break;
			}

			if ((err = fopen_s(&fpwrite, "Password.dat", "w")) != 0)//�򿪼�¼������ļ�,�����ж��ļ��Ƿ�������
			{
				MessageBox(NULL, TEXT("�����ļ���������\n���Թ���ԱȨ���������ԣ�"), TEXT("ERROR"), MB_OK | MB_ICONERROR);//������ʾ��
				exit(EXIT_FAILURE);
			}

			for (i = 0; i <= 6; i++)  //��������д���ļ�
			{
				new_password[i] = new_password[i] - 't' + 'i' - 'm' / 'e';//�����ļ�����
				fputc(new_password[i], fpwrite);//������ܺ�����ݵ��ļ�
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
		if ((err = fopen_s(&fpread, "Password.dat", "r")) != 0)//�򿪼�¼������ļ�,�����ж��ļ��Ƿ�������
		{
			MessageBox(NULL, TEXT("�����ļ���ȡ����\n���鱾����Ŀ¼�Ƿ��С�Password.dat������ļ������û�У����޸����롣"), TEXT("ERROR"), MB_OK | MB_ICONERROR);//������ʾ��
			return 1;
		}


		for (i = 0; i <= 6; i++)  //���ļ��ж�ȡ���ݵ�ch��EOF���ļ�������־��
		{
			new_password[i] = fgetc(fpread);//���ļ���ȡ����
			new_password[i] = new_password[i] + 't' - 'i' + 'm' * 'e';//�����ļ����ݲ��浽����
		}

		if (!strcmp(temp_password, new_password))//������������������е�������бȽ�
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
void open_readme_txt(void)
{
	char path[MAX_PATH];

	_getcwd(path, MAX_PATH); //��ȡ�ļ�·��
	strcat_s(path, MAX_PATH, "\\readme.txt");
	if (_access(path, 0))//�жϡ�readme.txt�����Ƿ��ڱ�Ŀ¼���򡣲����������ַ��������ֱ�Ӵ��ļ�
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


void countdown(void)
{
	MessageBox(NULL, TEXT("���ĵ��Ի���10���Ӿ�Ҫ�ػ���\n�����ùػ�׼����"), TEXT("��ܰ��ʾ"), MB_OK | MB_ICONWARNING);
}



//�Զ���ػ�
void shut_down(void)
{
	char shut[25] = { "shutdown -s -t " };
	char temp[7] = { 0 };
	system("cls");
	printf("\nע�⣺Ŀǰ�Զ���ʱ��ػ����ܲ����ڹػ�ǰ������ʾ��,");
	printf("��ע��鿴ϵͳ��ʾ���������ʱ�䲻�ܳ���99999�롣\n\n\n\n\n");
	printf("��ܰ��ʾ��ֻ���������ּ��ɣ��������뵥λ\n");
	printf("\t  ������������ػ�����λ���룩��");

	scanf_s("%s", temp, 7);
	flush();

	strcat_s(shut, 25, temp);//�ϲ�����

	system(shut);//ִ�йػ�����
	Sleep(2000);
	exit(EXIT_SUCCESS);
}



//���ע���
int add_start(void)
{
	system("cls && title ��������� PS������7����������");

	char *szSubKey = { "Software\\Microsoft\\Windows\\CurrentVersion\\Run" }; //ϵͳ������·��
	char folderName[MAX_PATH];//�����ļ��е�·��
	char buffer[MAX_PATH];    //���浱ǰ����·��
	char timec_Y[MAX_PATH];   
	char timec_Z[MAX_PATH];
	char config_file_path[MAX_PATH];
	char folder_path[MAX_PATH];
	char cmd[MAX_PATH];
	char SystemRoot[SIZE];      //����ϵͳ���̷�
	char PROGRAMFILES[MAX_PATH];//�������Ĭ�ϰ�װĿ¼

	HKEY hKey;
	int i;
	char ch;


	_getcwd(buffer, MAX_PATH);//��ȡ��ǰ����·��

	if (GetEnvironmentVariable("SYSTEMDRIVE", SystemRoot, SIZE) == 0)//��ȡϵͳ���̷�
		strcpy_s(SystemRoot, SIZE, "C:");//��ȡʧ����Ĭ��ΪC:

	if (GetEnvironmentVariable("PROGRAMFILES", folderName, MAX_PATH) == 0)//�ӻ���������ȡ����Ĭ�ϰ�װĿ¼
	{//��ȡʧ��
		sprintf_s(PROGRAMFILES, MAX_PATH, "%s\\Program Files (x86)", SystemRoot);
		if (_access(PROGRAMFILES, 0))//�ж�Ĭ�ϰ�װĿ¼��Program Files (x86)���Ƿ����
			sprintf_s(folderName, MAX_PATH, "%s\\Program Files\\timec", SystemRoot);//��������ʹ�á�Program Files��Ŀ¼
		else
			sprintf_s(folderName, MAX_PATH, "%s\\timec", PROGRAMFILES);//������ʹ�á�Program Files (x86)��Ŀ¼
	}
	else//��ȡ�ɹ�
		strcat_s(folderName, MAX_PATH, "\\timec");

	sprintf_s(timec_Y, MAX_PATH, "%s\\timec���ذ�.exe", folderName);    //ע���������·���Լ����Ƶ�Ŀ���ļ���·��
	sprintf_s(timec_Z, MAX_PATH, "%s\\timec�Զ����.exe", folderName);  //ע���������·���Լ����Ƶ�Ŀ���ļ���·��
	sprintf_s(cmd, MAX_PATH, "attrib +s +h \"%s\" ", folderName);
	sprintf_s(config_file_path, MAX_PATH, "%s\\Timec_config.txt", SystemRoot);
	strcpy_s(folder_path, MAX_PATH, folderName);


	printf("ע�⣺��ӻ�ɾ��������ǰ���Թ���ԱȨ�����б������\n\n");
	printf("\t      1�����timec���ذ�������\n\n");
	printf("\t      2�����timec�Զ����������\n\n");
	printf("\t      3���鿴ʹ�ð���\n\n");
	printf("\t      4��ɾ��������\n\n");
	printf("\t      5����������timec���ذ������ļ�\n\n");

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
		system("cls");

		printf("\n\n\n\nע�⣺���⵽ɱ�����أ��������������������\n\n��������޷�����������\n\n\n");
		Sleep(3000);

		if (_access(folderName, 0))//�ж��ļ����Ƿ����
			if (_mkdir(folderName))	   //�������򴴽��ļ���
			{
				MessageBox(NULL, TEXT("����Ŀ¼ʧ�ܣ����Թ���ԱȨ�����ԣ�"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
				exit(EXIT_FAILURE);
			}

		strcat_s(buffer, MAX_PATH, "\\timec���ذ�.exe");//����ǰ��·��
		copyfile(buffer, timec_Y);//�����ļ�

		system(cmd);//����Ϊ�����ļ�

		//��ע��������� 
		if (RegOpenKeyEx(HKEY_CURRENT_USER, szSubKey, 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)
		{  //���һ����Key,������ֵ
			RegSetValueEx(hKey, "timec���ذ�", 0, REG_SZ, (BYTE *)timec_Y, strlen(timec_Y));
			RegCloseKey(hKey);//�ر�ע���

			//printf("\t\t    ��ӳɹ���\n\n\t         ");
			printf("\t\t    ��ӳɹ���\n\n");
			printf("�Ƿ����ڴ�timec���ذ棨y/n����\n");
			scanf_s("%c", &ch, 1);
			flush();
			if (ch == 'y' || ch == 'Y')
			{
				sprintf_s(buffer, MAX_PATH, " \"%s\" ", timec_Y);
				system(timec_Y);
			}
			system("pause");
			return 0;
		}
		else
		{
			MessageBox(NULL, TEXT("���ʧ�ܣ�"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
			remove(folderName);
			return 1;
		}
		break;
	}

	case 2:
	{
		//�˴����ϴ�Դ���������ͬ���ʲ�дע�͡���ʵ��������~
		system("cls");

		printf("\n\n\n\nע�⣺���⵽ɱ�����أ��������������������\n\n��������޷�����������\n\n\n");
		Sleep(3000);

		if (_access(folderName, 0))//�ж��ļ����Ƿ����
			if (_mkdir(folderName))			//�������򴴽��ļ���
			{
				MessageBox(NULL, TEXT("����Ŀ¼ʧ�ܣ����Թ���ԱȨ�����ԣ�"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
				exit(EXIT_FAILURE);
			}

		system(cmd);//����Ϊ�����ļ�

		strcat_s(buffer, MAX_PATH, "\\timec�Զ����.exe");//����ǰ��·��
		copyfile(buffer, timec_Z);//�����ļ�

		if (RegOpenKeyEx(HKEY_CURRENT_USER, szSubKey, 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)
		{
			RegSetValueEx(hKey, "timec�Զ����", 0, REG_SZ, (BYTE *)timec_Z, strlen(timec_Z));
			RegCloseKey(hKey);

			printf("\t\t    ��ӳɹ���\n\n\t         ");
			system("pause");
			return 0;
		}
		else
		{
			MessageBox(NULL, TEXT("���ʧ�ܣ�"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
			remove(folderName);
			return 1;
		}
		break;
	}

	case 3:
		MessageBox(NULL, TEXT("1�����ÿ����Զ�����ǰ���Թ���ԱȨ�����б����\n��ֹȨ�޲������ʧ�ܡ�\n2�����ÿ����Զ�����ǰ��ȷ����timec���ذ桱��\n��timec�Զ���桱��ͬһĿ¼��\n3�����ÿ����Զ�����ǰ��ȷ����timec���ذ桱��\n��timec�Զ���桱��ԭ�ļ���δ���޸ġ�\n4�������������ʧЧ����鿴��C:\\Program Files\\time��Ŀ¼����û��\n��timec�Զ���� ����timec���ذ桱������\n���߼���������Ȩ���Ƿ�ɱ������ˡ�\n5���ڵ�ǰҳ������4ɾ����������\n6����������������޸�ϵͳע���\n���ܻ���ɵ��Բ��ȶ��������ã�\n7��ʹ�ñ�����������κκ��������ԭ���߳е�\n��һ��ʹ�ñ��������Ϊͬ������\nע�������������κθ��£�ˡ������֪ͨ��"), TEXT("ʹ�ð���"), MB_OK);
		break;

	case 4://���ļ���ɾ���ļ�
	{
		del_start_password();//��֤����
		system("cls");
		printf("\n\n\n\n\n\nע�⣺1�����Թ���ԱȨ�޴򿪴������\n      2�����⵽ɱ�����أ��������������������\n\n\n");
		Sleep(3000);                     
		system("cls && mode con cols=53 lines=30 && title ����ɾ��...");

		//��������
		printf("\n���ڽ���timec���ذ�Ľ���...\n");
		Sleep(2000);
		system("taskkill /f /t /im timec���ذ�.exe");

		//ɾ������
		printf("\n\n����ɾ���ļ�...");
		if (remove(timec_Y))
			printf("\nɾ��timec���ذ�ʧ�ܣ�\n��δ���timec���ذ��������ɲ���ᡣ\n\n");
		else
			printf("\nɾ��timec���ذ�ɹ���\n\n");

		if (remove(timec_Z))
			printf("ɾ��timec�Զ����ʧ�ܣ�\n��δ���timec�Զ�����������ɲ���ᡣ\n\n");
		else
			printf("ɾ��timec�Զ����ɹ���\n\n");

		//ɾ�������ļ�
		printf("\n����ɾ�������ļ�...\n");
		if (remove("C:\\Timec_config.txt"))
			printf("ɾ�������ļ�ʧ�ܣ�\n���ֶ�����C�̸�Ŀ¼ɾ����Timec_config.txt����\n\n");
		else
			printf("ɾ�������ļ��ɹ���\n\n");

		//ɾ��Ŀ¼
		printf("\n����ɾ����װĿ¼...\n");
		sprintf_s(cmd, MAX_PATH, "rd /s /q \"%s\"", folderName);
		system(cmd);

		//ɾ��ע���
		printf("\n\n����ɾ��timec���ذ�ע���������...\n");
		system("reg delete \"HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run\"  /v timec���ذ� /f");
		printf("\n����ɾ��timec�Զ����ע���������...\n");
		system("reg delete \"HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run\"  /v timec�Զ���� /f");

		putchar('\n');
		system("pause");
		break;
	}

	case 5://�������������ļ�
	{

		system("title ��������timec���ذ������ļ� && mode con cols=72 lines=17");
	
		char c;
		unsigned i, j = 0;
		int  front_hour, front_min, behind_hour, behind_min;
		char config_content[MAX_SIZE] = { 0 };				//���潫Ҫд�������ļ�������
		char temp_time_period[15] = { 0 }, time_period[15]; //���������е�ʱ���
		char run_time[5], break_time[5];					//���������е�ʱ�����Ϣʱ��
		char date[100] = { 0 };								//�������ÿ���������е�����

		FILE *fpwrite;
		errno_t err;



/***********************/
begin://goto����ǩ
/***********************/

		system("cls && mode con cols=72 lines=25");

		printf("\n\n���������ÿ�������е�ʱ��Σ���ʽ��xx:xx - xx:xx���벻Ҫ�������ı�㡣\n��ܰ��ʾ������00:00 - 00:00����������ʱ��Ρ�����");
		while (1)
		{
			Initialize(temp_time_period, time_period, 2);

			fgets(time_period, 15, stdin);
			rewind(stdin);
			fgets_n(time_period);	//���fgets��ȡ��'\n'

			for (i = 0; i < 15; i++)//��time_period�����ݸ��Ƶ�temp_time_period
				temp_time_period[i] = time_period[i];

			//�ж�����ĸ�ʽ�Ƿ���ȷ
			if (strchr(date, '��') || strchr(date, '����'))
			{
				printf("\n�벻Ҫ�������ķ��ţ����������룺");
				continue;
			}

			if (strlen(time_period) != 13 || time_period[6] != '-')//�ж������ʽ��xx:xx-xx:xx����xx:xx - xx:xx
			{
				if (strlen(time_period) != 11 || time_period[5] != '-')
				{
					printf("\n��ʽ�������磺08:00 - 14:00���������ԣ�");
					continue;
				}
			}

			for (i = 0; i < 15; i++)
			{
				if (time_period[i] == ':' || time_period[i] == '-')
					time_period[i] = ' ';
			}

			sscanf_s(time_period, "%d %d %d %d", &front_hour, &front_min, &behind_hour, &behind_min);

			//�ж������ʱ���Ƿ�����
			if ((front_hour > 24 || front_hour < 0) || (behind_hour > 24 || behind_hour < 0) || (front_min > 60 || front_min < 0) || (behind_min > 60 || behind_min < 0))
			{
				printf("\n��ʽ�������磺08:00 - 14:00���������ԣ�");
				continue;
			}
			if (front_hour == 24 || behind_hour == 24)//�ж�����ʱ��Сʱ�����Ƿ�Ϊ24
			{
				if (front_hour == 24 && behind_hour == 24)//�ж�����ʱ��Сʱ�����Ƿ�Ϊ24
				{
					temp_time_period[1] = temp_time_period[0] = '0';
					if (time_period[8] == '2' && time_period[9] == '4')
						temp_time_period[9] = temp_time_period[8] = '0';
					else
						temp_time_period[7] = temp_time_period[6] = '0';
					printf("\n��ʽ����Ӧ����Ϊ��%s�������ԣ�", temp_time_period);
				}
				else if (front_hour == 24)//�ж�ǰʱ��Сʱ�����Ƿ�Ϊ24
				{
					temp_time_period[1] = temp_time_period[0] = '0';
					printf("\n��ʽ����Ӧ����Ϊ��%s�������ԣ�", temp_time_period);
				}
				else if (behind_hour == 24)//�жϺ�ʱ��Сʱ�����Ƿ�Ϊ24
				{
					if (time_period[8] == '2' && time_period[9] == '4')
						temp_time_period[9] = temp_time_period[8] = '0';
					else
						temp_time_period[7] = temp_time_period[6] = '0';
					printf("\n��ʽ���󣬺�ʱ��Ӧ����Ϊ��%s�������ԣ�", temp_time_period);
				}
				continue;
			}

			//�ж������Ƿ�Ϊ00:00 - 00:00
			if (front_hour == 0 && behind_hour == 0 && front_min == 0 && behind_min == 0)
				break;

			//�жϺ�ʱ���Ƿ��ǰʱ�̴�
			if (front_hour > behind_hour)
			{
				printf("\nǰʱ��Ҫ�Ⱥ�ʱ��С�������ԣ�");
				continue;
			}
			else if (front_hour == behind_hour)
			{
				if (front_min > behind_min)
				{
					printf("\nǰʱ��Ҫ�Ⱥ�ʱ��С�������ԣ�");
					continue;
				}
				else if (front_min == behind_min)
				{
					printf("\nǰʱ��Ҫ�Ⱥ�ʱ��С���Ҳ�����ȡ������ԣ�");
					continue;
				}
			}

			//�ж�ǰʱ�����ʱ���Ƿ����С��5����
			front_hour = (front_hour * 3600) + (front_min * 60);
			behind_hour = (behind_hour * 3600) + (behind_min * 60);
			if ((behind_hour - front_hour) < 300)
			{
				printf("\nǰʱ�����ʱ�̱������5���ӡ������ԣ�");
				continue;
			}
			break;
		}


		printf("\n\n���������ÿ���������е����ڣ����磺05��\n��ܰ��ʾ�����Ҫ��������������Ӣ�Ķ��ŷָ��������磺1,5,10,25,30��\n����000�������������ڡ�����");
		while (1)
		{
			Initialize(date, NULL, 1);
			fgets(date, 100, stdin);
			rewind(stdin);
			fgets_n(date);

			if (strchr(date, '��'))
			{
				printf("\n�벻Ҫ�������ķ��ţ����������룺");
				continue;
			}
			if (strlen(date) <= 1)
			{
				printf("\n��ʽ�������������루���磺01,05,10����");
				continue;
			}
			break;
		}


		printf("\n\n���������ÿ�������е�ʱ��\n��Ҳ���ǵ���ÿ������XX���Ӻ�ǿ�ƹػ�����λ�����ӣ���");
		while (1)
		{
			while (scanf_s("%d", &i) != 1)
			{
				printf("\n������������ԣ�");
				flush();
			}
			flush();
			if (i > 999)
			{
				printf("\n���ֵ���ܳ���999�����������룺");
				continue;
			}
			sprintf_s(run_time, 5, "%d", i);
			break;
		}


		printf("\n\n���������ÿ����Ϣʱ��\n��Ҳ���ǵ���ÿ��ǿ�ƹػ�����Ҫ��XX���Ӻ����ʹ�á���λ�����ӣ���");
		while (1)
		{
			while (scanf_s("%d", &i) != 1)
			{
				printf("\n������������ԣ�");
				flush();
			}
			flush();
			if (i > 999)
			{
				printf("\n���ֵ���ܳ���999�����������룺");
				continue;
			}
			sprintf_s(break_time, 5, "%d", i);
			break;
		}

		printf("\n\n�����뵱�����������е�ʱ����������Ϣʱ��������ʱ�����Թػ�ʱ��\n����λ���롣���ֵ60�룩��");
		while (1)
		{
			while (scanf_s("%d", &i) != 1)
			{
				printf("\n������������ԣ�");
				flush();
			}
			flush();
			if (i > 60)
			{
				printf("\n���ֵ���ܳ���60�룬���������룺");
				continue;
			}
			break;
		}

		system("cls && mode con cols=47 lines=15");
		printf("��ǰ���õĲ���Ϊ��\n\n");

		if (strcmp(temp_time_period, "00:00-00:00") == 0)
			printf("����ÿ�������е�ʱ��Σ� %s������������ʱ��Σ�\n", temp_time_period);

		else if (strcmp(temp_time_period, "00:00 - 00:00") == 0)
			printf("����ÿ�������е�ʱ��Σ� %s������������ʱ��Σ�\n", temp_time_period);

		else
			printf("����ÿ�������е�ʱ��Σ� %s", temp_time_period);

		if (strcmp(date, "00") == 0)
			printf("\n����ÿ���������е����ڣ� %s���������������ڣ�", date);
		else
			printf("\n����ÿ���������е����ڣ� %s ��", date);

		printf("\n����ÿ�������е�ʱ�䣺   %s ��", run_time);
		printf("\n����ÿ����Ϣ��ʱ�䣺     %s ��", break_time);

		while (1)
		{
			printf("\n\n\t      �Ƿ�ȷ������Y/N��");
			printf("\n\t�����룺");
			scanf_s("%c", &c, 1);
			flush();

			if (c == 'y' || c == 'Y')
				break;
			else if (c == 'n' || c == 'N')
			{
				Initialize(run_time, break_time, 2);
				goto begin;
			}
			else
				continue;
		}

		sprintf_s(config_content, MAX_PATH, "���Կ����������е�ʱ�� = %s ;\n"
			"���Թػ���ǿ����Ϣ��ʱ�� = %s ;\n"
			"����ÿ�������е�ʱ��� = %s ;\n"
			"����ÿ���������е����� = %s ;\n"
			"��������Ϣʱ��������ʱ�ػ�ʱ�� = %d ;", run_time, break_time, temp_time_period, date, i);
		strcat_s(config_content, MAX_SIZE, "\n\nע�⣺\n"
			"1�������Կ����������е�ʱ�䡱�͡����Թػ���ǿ����Ϣ��ʱ�䡱����ֵ�ĵ�λ��Ϊ�֣�ֵ��Χ5~999��\n"
			"2��������ÿ�������е�ʱ��Ρ���ֵ�ĸ�ʽΪ��xx:xx-xx:xx�����벻Ҫ�������ı�㣨�磺����������\n"
			"3��ÿһ��ĩβ�ķֺ���Ӣ�ķֺţ����������뱣��ԭ��ʽ��\n"
			"4��������ֳ������������У����ڡ�timec�Զ���桱���´��������ļ�����ӡ�-reset����������timec���ذ档");

		//�������ļ�
		if ((err = fopen_s(&fpwrite, config_file_path, "w")) != 0)
		{
			remove(folder_path);
			perror("\n���������ļ�ʧ��");
			system("pause");
			exit(EXIT_FAILURE);
		}
		fputs(config_content, fpwrite);//�������ļ��������
		fclose(fpwrite);
		
		printf("\n\t        ���óɹ���\n\t      ");
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


	if ((err = fopen_s(&fpread, fileread, "rb")) != 0)//�Զ����Ʒ�ʽ���ļ�
	{
		MessageBox(NULL, TEXT("�޷������ļ���\n���顰timec�Զ����.exe���͡�timec���ذ�.exe���Ƿ���ͬһĿ¼��\n"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
		exit(EXIT_FAILURE);
	}

	if ((err = fopen_s(&fpwrite, filewrite, "wb")) != 0)
	{
		//MessageBox(NULL, TEXT("�޷������ļ������Թ���ԱȨ�����ԣ�\n"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
		perror("Error");
		system("pause");
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



void del_start_password(void)//ɾ����������֤����
{
	FILE *fpread;
	errno_t err;

	int i;
	char new_password[7] = { 0 };//����������
	char temp_password[7] = { 0 };//��ʱ��������

	while (1)
	{
		system("cls && title ɾ������������Ҫ������ȷ������");

		printf("\n\t\t  ����1234�˳���\n         ���������룬��������������0���ġ�");
		printf("\n\n\nɾ������������Ҫ�������롣\n\n��������ȷ�����룺");
		scanf_s("%s", temp_password, 7);
		flush();

		if (!strcmp(temp_password, "1234"))//����1234�˳�
			exit(EXIT_SUCCESS);

		if ((err = fopen_s(&fpread, "Password.dat", "r")) != 0)//�򿪼�¼������ļ�,�����ж��ļ��Ƿ�������
		{
			MessageBox(NULL, TEXT("�����ļ���ȡ����\n���鱾����Ŀ¼�Ƿ��С�Password.dat������ļ������û�У����޸����롣"), TEXT("ERROR"), MB_OK | MB_ICONERROR);//������ʾ��
			exit(EXIT_SUCCESS);
		}

		for (i = 0; i <= 6; i++)  //���ļ��ж�ȡ���ݵ�ch��EOF���ļ�������־��
		{
			new_password[i] = fgetc(fpread);//���ļ���ȡ����
			new_password[i] = new_password[i] + 't' - 'i' + 'm' * 'e';//�����ļ����ݲ��浽����
		}

		if (!strcmp(temp_password, new_password))//������������������е�������бȽ�
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


//���fgets��ȡ��'\n'
void fgets_n(char *str)
{
	char *find;
	if ((find = strchr(str, '\n')))
		*find = '\0';
}


//��ʼ������
void Initialize(char *str1, char *str2, int n)
{
	while (*str1 != '\0')
	{
		if (n == 1)
			*str1++ = '\0';
		else if (n == 2)
			*str2++ = *str1++ = '\0';
	}
}