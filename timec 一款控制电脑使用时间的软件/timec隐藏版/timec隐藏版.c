#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>
#include <direct.h>
#include <io.h>

#define SIZE 100
#define MAX_SIZE 600

int  GetTime(void);         //�Ƚϵ�ǰʱ��͵�����������ʱ��
int  WriteTime(void);       //д����Թػ�ʱ����´�������ʱ��
void ShutDown(void);	    //ִ�йػ�����
int  Read_File(int* RunTime, int* BreakTime, char* Time_Period, char* Date, int N);//�������ļ���ȡ���������е�ʱ�����Ϣʱ��
void Shutdown_Tip(const long sec);//������ػ�ʱ�仹��3����ʱ����ػ���ʾ
void FirstStart(char* cmd);	//��һ�������Զ������
void Flush(void);		    //���������
void Fgets_n(char* str);    //���fgets��ȡ��'\n'
void Initialize(char* str1, char* str2, int n);//��ʼ������
void CopyFileToStartFolder(wchar_t* source);//���������ģʽ2���Ƶ�����Ŀ¼

/***************************************************************************************************************
*    ��ǰʱ��Ϊ2019��11��23��11:20�����趨��������40���Ӿ�Ҫ��Ϣ10���Ӻ�������У���������ֻ����ÿ���10:00-   *
*    13:00���С�                                                                                               *
*    Ҳ����˵Ҫ�ڵ�ǰʱ��Ļ����ϼ���50���ӣ�40���Ӻ�ػ�+10������Ϣ����������´򿪵��ԡ�				 	   *
*    ��Ϊ��������40���Ӻ�ػ������Ե��Թػ���ʱ����2019��11��23��12:00��						               *
*    �ټ���10������Ϣʱ�䣬������Ҫ��2019��11��23��12:10������ٴδ򿪵��ԣ��������ʾ�ػ���				   *
*    �������10:00-13:00���ʱ�����򿪵��ԣ�Ҳ����ʾ�ػ���												   *
****************************************************************************************************************/


char time_dat_path[SIZE];   //�����ļ�·��
char config_file_path[SIZE];//�����ļ�·��
char folder_path[SIZE];     //�ļ���·��
char dest_path[SIZE];      //ע���������·���Լ����Ƶ�Ŀ���ļ���·��
wchar_t W_dest_path[SIZE]; //ע���������·���Լ����Ƶ�Ŀ���ļ���·�����ַ���
FILE* fp;
errno_t err;


int main(int argc, char* argv[])
{
	size_t PtNumOfCharConverted;
	wchar_t W_folder_path[SIZE];        //�����ļ���·�����ַ���
	char cmd[SIZE];			            //����cmd����
	wchar_t W_SystemRoot[SIZE];         //����ϵͳ���̷����ַ���
	char C_SystemRoot[SIZE] = { '\0' }; //����ϵͳ���̷�


	if (GetEnvironmentVariable(L"SYSTEMDRIVE", W_SystemRoot, SIZE) == 0) //��ȡϵͳ���̷�
		wcscpy_s(W_SystemRoot, SIZE, L"C:");                             //��ȡʧ����Ĭ��ΪC:
	wcstombs_s(&PtNumOfCharConverted, C_SystemRoot, SIZE, W_SystemRoot, SIZE); //��wchar_t����תΪchar����


	sprintf_s(folder_path, SIZE, "%s\\ProgramData\\timec", C_SystemRoot); //�ļ���·��
	mbstowcs_s(&PtNumOfCharConverted, W_folder_path, SIZE, folder_path, SIZE);//��char����תΪwchar_t����

	sprintf_s(config_file_path, SIZE, "%s\\Users\\Public\\Timec_config.txt", C_SystemRoot);//�����ļ�·��
	sprintf_s(time_dat_path, SIZE, "%s\\time.dat", folder_path);          //�����ļ�·��
	sprintf_s(dest_path, SIZE, "%s\\timec���ذ�.exe", folder_path);       //ע���������·���Լ����Ƶ�Ŀ���ļ���·��
	swprintf_s(W_dest_path, SIZE, L"%s\\timec���ذ�.exe", W_folder_path);  //ע���������·���Լ����Ƶ�Ŀ���ļ���·�����ַ���
	sprintf_s(cmd, SIZE, "attrib +s +h \"%s\" ", folder_path);            //��Ŀ¼���ϵͳ��������


	//���س������д���
	HWND hwnd = FindWindow(TEXT("ConsoleWindowClass"), NULL);
	if (hwnd)
		ShowWindow(hwnd, SW_HIDE);

	if (argc == 2)                 //�ж��Ƿ�����ˡ�-reset����������timec���ذ�
	{
		if (strcmp(argv[1], "-reset") == 0)
			FirstStart(cmd);
	}
	else if (_access(time_dat_path, 0)) //�ж������ļ���time.dat���Ƿ����                                   
		FirstStart(cmd);       //����������õ�һ����������

	GetTime();
	WriteTime();
	ShutDown();
	return 0;
}


/*****************************************************************************
					�ж��ڵ�ǰʱ���ڵ����Ƿ�������
*****************************************************************************/
int GetTime(void)
{
	unsigned i, j;
	long long ShutDownTime;
	int  temp, sec;
	char temp_time[SIZE] = { 0 }, behind_time[15] = { 0 };
	char command[100];
	char last_time[SIZE];        //�����´������е�ʱ��������������ϴιرպ��ʱ�������Ϣʱ�䣩
	char time_now[SIZE];         //���浱ǰʱ��
	char shut_down_time[SIZE];   //������Թػ�ʱ��
	char date[100] = { 0 };	     //������������е�����
	char time_period[15] = { 0 };//������������е�ʱ���
	char C_tip[100] = { 0 };     //��������ǰ����ʱ����ػ���ʾ
	wchar_t W_tip[100] = { 0 };  //��������ǰ����ʱ����ػ���ʾ

	const wchar_t ShutdownKey[] = { L"SYSTEM\\CurrentControlSet\\Control\\Windows" };//ϵͳ������·��
	size_t pReturnValue;
	HKEY hkey;
	FILETIME file_time;
	DWORD dwsize;
	SYSTEMTIME system_time;
	ULARGE_INTEGER ui;
	time_t current_time, shutdown_time = 0;

	//��ȡ1970-01-01����ǰ������
	struct tm  tm_current_time;
	time(&current_time);
	localtime_s(&tm_current_time, &current_time);//����ȡ������ת��Ϊ����ʱ��


	sec = Read_File(NULL, NULL, time_period, date, 34);//��ȡ���������е�ʱ��κ�����


	//�жϵ�ǰʱ���Ƿ���ʱ�����
	if (strcmp(time_period, "00:00-00:00") != 0)//�ж��Ƿ���ʱ�������
	{
		sprintf_s(C_tip, 100, "����ÿ��ġ�%s�������б����ԣ�\n�����Լ�����%d���ڹػ���", time_period, sec);
		mbstowcs_s(&pReturnValue, W_tip, SIZE, C_tip, SIZE);//��charתΪwchar_t

		for (i = 0; i < 15; i++)//���'-'��':'
		{
			if (time_period[i] == '-' || time_period[i] == ':')
			{
				for (j = i; j < 14; j++)
					time_period[j] = time_period[j + 1];
			}
		}

		for (i = 4; i < strlen(time_period); i++)//�ָ��ǰʱ�̺ͺ�ʱ�̣��ָ�ǰ��time_period[15]={"08:00-16:00"}���ָ��ǰʱ�̣�behind_time[15]={"08:00"}����ʱ�̣�time_period[15]={"16:00"}��
		{
			behind_time[i - 4] = time_period[i];
			if (i == (strlen(time_period) - 1))
				time_period[4] = '\0';
		}


		sprintf_s(temp_time, 15, "%d%d", tm_current_time.tm_hour, tm_current_time.tm_min);//����ǰʱ��д������


		if (strcmp(temp_time, time_period) > 0 && strcmp(temp_time, behind_time) < 0)     //�жϵ�ǰʱ���Ƿ��������е�ʱ�����
			;
		else
		{
			system("shutdown -a");
			sprintf_s(command, 100, "shutdown -f -s -t %d", sec);
			system(command);//ִ�йػ�����
			MessageBox(NULL, (W_tip), TEXT("���棺"), MB_OK | MB_ICONWARNING);
			system(command);//ִ�йػ�����
			exit(EXIT_SUCCESS);
		}
		Initialize(temp_time, NULL, 1);//�������
	}

	Initialize(C_tip, NULL, 1);
	memset(W_tip, 0, SIZE);


	if (strcmp(date, "00") != 0)//�жϵ�ǰ�����Ƿ�������
	{
		sprintf_s(C_tip, 100, "����ÿ���µġ�%s�������б����ԣ�\n�����Լ�����%d���ڹػ���", date, sec);
		mbstowcs_s(&pReturnValue, W_tip, SIZE, C_tip, SIZE);//��charתΪwchar_t

		for (i = 0; i <= 100; i++)
		{
			Initialize(temp_time, NULL, 1);
			if (i == 100)
			{
				system("shutdown -a");
				sprintf_s(command, 100, "shutdown -f -s -t %d", sec);
				system(command);//ִ�йػ�����
				MessageBox(NULL, (W_tip), TEXT("���棺"), MB_OK | MB_ICONWARNING);
				system(command);//ִ�йػ�����
				exit(EXIT_SUCCESS);
			}

			if (i == 0)
			{
				temp_time[0] = date[0];
				temp_time[1] = date[1];
			}
			else if (date[i] == ',')
			{
				temp_time[0] = date[i - 2];
				temp_time[1] = date[i - 1];
			}
			sscanf_s(temp_time, "%d", &temp);

			if (temp == tm_current_time.tm_mday)
				break;
		}
	}

	Initialize(C_tip, NULL, 1);
	memset(W_tip, 0, SIZE);


	if ((err = fopen_s(&fp, time_dat_path, "r")) != 0)//�򿪡�time.dat��
		return 1;                 //�����ʧ�ܾͷ���������ִ�йػ�����

	if (!_filelength(_fileno(fp)))//�ж��ļ��Ƿ�Ϊ��
	{
		fclose(fp);
		return 1;
	}


	for (i = 0; i < 4; i++)       //�ӡ�time.dat����ȡ����
	{
		if (i == 1)               //�ӡ�time.dat����ȡ��һ�У����Թػ���ʱ�������
			fgets(shut_down_time, SIZE, fp);
		else if (i == 2)          //��time.dat����ȡ�ڶ��У������´������е�ʱ�������
			fgets(last_time, SIZE, fp);
		else if (i == 3)          //�ӡ�time.dat����ȡ�����У���ʾ
			fgets(C_tip, 100, fp);
	}
	fclose(fp);

	//���fgets��ȡ�Ļ��з�
	Fgets_n(shut_down_time);
	Fgets_n(last_time);

	//������ļ��ж�ȡ�������ַ�
	strcpy_s(shut_down_time, strlen(shut_down_time) + 1, shut_down_time + 22);
	strcpy_s(last_time, strlen(last_time) + 1, last_time + 28);

	//��1970-01-01 00:00:00����ǰʱ�������д�뵽����
	sprintf_s(time_now, SIZE, "%lld", current_time);


	if (strcmp(shut_down_time, time_now) > 0)//�жϵ�ǰʱ���Ƿ�С�ڵ��Թػ�ʱ��
	{
		if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, ShutdownKey, 0, KEY_READ, &hkey) != ERROR_SUCCESS)//��ȡ�����ϴιػ�ʱ��
			ShutDown();

		if (RegQueryValueEx(hkey, TEXT("ShutdownTime"), NULL, NULL, NULL, &dwsize) == ERROR_SUCCESS && sizeof(file_time) == dwsize)
		{
			RegQueryValueEx(hkey, TEXT("ShutdownTime"), NULL, NULL, (LPBYTE)&file_time, &dwsize);

			FileTimeToSystemTime(&file_time, &system_time);
			ui.LowPart = file_time.dwLowDateTime;
			ui.HighPart = file_time.dwHighDateTime;
			shutdown_time = (LONGLONG)(ui.QuadPart - 116444736000000000) / 10000000;
		}
		RegCloseKey(hkey);

		sscanf_s(shut_down_time, "%lld", &ShutDownTime);//��������Ĺػ�ʱ��д������

		/*
		struct tm tm_Shut_Down_Time;
		localtime_s(&tm_Shut_Down_Time, &shutdown_time);
		printf("�ػ�ʱ�䣺%4d-%02d-%02d %02d:%02d:%02d\n", tm_Shut_Down_Time.tm_year + 1900, tm_Shut_Down_Time.tm_mon + 1, tm_Shut_Down_Time.tm_mday, tm_Shut_Down_Time.tm_hour, tm_Shut_Down_Time.tm_min, tm_Shut_Down_Time.tm_sec);
		*/
		if ((current_time - shutdown_time) < 0)  //����ȡ�Ĺػ�ʱ���Ƿ����
			ShutDown();
		else
		{
			if ((current_time - shutdown_time) > 300)//�жϵ�ǰʱ����ػ�ʱ���Ƿ�������5����
				return 0;                            //�����ǰʱ����ػ�ʱ��������5���Ӿ�����д��ػ�ʱ�䲢ִ�йػ�
		}

		//�����ǰʱ����ػ�ʱ�����С��5���Ӿ������ǰʱ������´ιػ�ʱ�仹�ж��ٷ�
		system("shutdown -a");
		sprintf_s(command, 100, "shutdown -f -s -t %lld", (ShutDownTime - current_time));//��ʱ���д������
		system(command);                         //ִ�йػ�����
		Shutdown_Tip((long)(ShutDownTime - current_time));
	}
	else if (strcmp(last_time, time_now) > 0)//�����ǰʱ����ڵ��Թػ�ʱ�䣬���жϵ�ǰʱ���Ƿ�С�ڵ�����������ʱ��
	{
		//�����ǰʱ��С�ڵ�����������ʱ�䣬��ִ�йػ����������ػ���ʾ
		system("shutdown -a");
		sprintf_s(command, 100, "shutdown -f -s -t %d", sec);
		system(command);//ִ�йػ�����
		MessageBox(NULL, (W_tip), TEXT("���棡"), MB_OK | MB_ICONWARNING);//�����ػ���ʾ
		system(command);//ִ�йػ�����
		exit(EXIT_SUCCESS);
	}
	return 0;
}


/*****************************************************************************
д����Թرպ���Ϣ��ʱ�䣬�������´�������ʱ�䡣
���磺����������40������Ϣ10���ӡ�
Ҳ����˵�����´���������ʱ��Ҫ�ڵ�ǰʱ���ϼ���40��������ʱ���10������Ϣʱ�䡣
******************************************************************************/
int WriteTime(void)
{
	time_t last_time;
	struct tm p;
	int min, hour, day, mon, year, run_time, break_time;
	long sec = 0;

	//��ȡ���������е�ʱ�����Ϣʱ��
	Read_File(&run_time, &break_time, NULL, NULL, 12);


	if ((err = fopen_s(&fp, time_dat_path, "w")) != 0)//�������ļ�
		return 1;


	time(&last_time);                      //��ȡ��ǰʱ�������
	localtime_s(&p, &last_time);
	min = p.tm_min + run_time + break_time;//�ڵ�ǰ���ӵĻ����ϼ���xx���ӵ�����ʱ���xx������Ϣʱ��
	hour = p.tm_hour;
	day = p.tm_mday;
	mon = p.tm_mon + 1;
	year = p.tm_year + 1900;

	while (min >= 60)
	{
		min -= 60;
		hour++;
	}

	while (hour >= 24)
	{
		hour -= 24;
		day++;
	}
	//�ж��·�
	if (mon == 1 || mon == 3 || mon == 5 || mon == 7 || mon == 8 || mon == 10 || mon == 12)//�жϵ�ǰ�·��Ƿ�Ϊ����
	{
		while (day > 31)
		{
			day -= 31;
			mon++;
		}
	}

	if (mon == 4 || mon == 6 || mon == 9 || mon == 11)//�жϵ�ǰ�·��Ƿ�ΪС��
	{
		while (day > 30)
		{
			day -= 30;
			mon++;
		}
	}

	if (mon == 2)
	{
		if (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0))//�жϵ�ǰ����Ƿ�Ϊ����
		{
			while (day > 29)
			{
				day -= 29;//����2����29��
				mon++;
			}
		}
	}
	else
	{
		while (day > 28)
		{
			day -= 28;//ƽ��2����28��
			mon++;
		}
	}

	while (mon > 12) //�жϵ�ǰ�·��Ƿ񳬹�12��
	{
		mon -= 12;
		year++;
	}

	run_time *= 60;
	break_time *= 60;

	sec = sec + (long)last_time + (long)run_time;
	fprintf(fp, "���Թػ���ʱ���������%ld\n", sec);//���ļ�д������´�������ʱ�������
	sec += (long)break_time;
	fprintf(fp, "�����´������е�ʱ���������%ld\n", sec);
	fprintf(fp, "��ǰΪ��Ϣʱ��,�´δ˵�����������ʱ��Ϊ��%02d��%02d��%02d��%02dʱ%02d�ֺ�    ���Լ����ػ���", year, mon, day, hour, min);//���ļ�д����ʾ��Ϣ
	fclose(fp);
	return 0;
}



/*****************************************************************************
				 �������ļ���ȡ���������е�ʱ���Լ���Ϣʱ��
*****************************************************************************/
int Read_File(int* RunTime, int* BreakTime, char* Time_Period, char* Date, int N)
{
	int ch, sec = 10, i = 0, j = 0;
	int run_time = 40;	         //Ĭ��������ʱ�䣨��λ���֣�
	int break_time = 10;         //Ĭ����Ϣʱ�䣨��λ���֣�
	char config[100] = { 0 };    //������ļ���ȡ�Ĳ���
	char time_period[15] = { 0 };//�����е�ʱ���
	char date[100] = { 0 };      //����ÿ���������е�����


	//�ȷ���Ĭ�ϲ���
	if (N == 1)
		*RunTime = run_time;
	else if (N == 12)
	{
		*RunTime = run_time;
		*BreakTime = break_time;
	}
	else if (N == 34)
	{
		strcpy_s(Time_Period, 15, "00:00-00:00");
		strcpy_s(Date, 100, "00");
	}


	//�������ļ���ȡ����
	if ((err = fopen_s(&fp, config_file_path, "r")) != 0)
		return sec;


	if ((ch = fgetc(fp)) == EOF)//�ж��ļ��Ƿ�Ϊ��
	{
		MessageBox(NULL, TEXT("��timec���ذ桱�����ļ�����\n���ڡ�timec�Զ���桱�������������ļ���\n����ӡ�-reset����������timec���ذ档"), TEXT("����"), MB_OK | MB_ICONERROR);
		return sec;
	}


	while ((ch = fgetc(fp)) != EOF)
	{
		if (ch == '=')        //�ж�ch�Ƿ�Ϊ��=��
		{
			while ((ch = fgetc(fp)) != ';')//��ȡ��=������;���ڵĲ���
			{
				if (ch == ' ')//�ж�ch�Ƿ�Ϊ�ո�
				{
					i = 0;
					continue;//�����˴�ѭ����������ȡ�Ŀո�
				}
				config[i] = ch;
				i++;
			}
		}

		if (ch == '\n')       //�ж��Ƿ������һ��
		{
			if (j == 0)       //�ж��Ƿ�����ڶ���
				sscanf_s(config, "%d", &run_time);   //�ѵ�һ�е����ݱ��浽run_time����һ�е������ǵ��������е�ʱ��
			else if (j == 1)  //�ж��Ƿ����������
				sscanf_s(config, "%d", &break_time); //�ѵڶ��е����ݱ��浽break_time���ڶ��е������ǵ��Թػ�����Ϣ��ʱ��	
			else if (j == 2)  //�ж��Ƿ����������
			{
				if (strchr(config, '��') || strchr(config, '����'))
					MessageBox(NULL, TEXT("��⵽��timec���ذ桱�����ļ������ķ��ţ�\n���ڡ�timec�Զ���桱���������������ļ���\n����ӡ�-reset����������timec���ذ档"), TEXT("����"), MB_OK | MB_ICONERROR);

				for (i = 0; i < 15; i++)//���������е�ʱ���
					time_period[i] = config[i];
			}
			else if (j == 3)//�ж��Ƿ����������
				strcpy_s(date, 100, config);//����ÿ���������е�����
			else if (j == 4)
			{
				sscanf_s(config, "%d", &sec);
				if (sec < 0 && sec > 60)
					sec = 10;
			}
			else if (j == 5)
				break;

			j++;
			i = 0;
			Initialize(config, NULL, 1);//����ÿ��ѭ��ǰ����
		}
	}

	if (strchr(config, '��'))
	{
		MessageBox(NULL, TEXT("��⵽��timec���ذ桱�����ļ������ķ��ţ�\n���ڡ�timec�Զ���桱���������������ļ���\n����ӡ�-reset����������timec���ذ档"), TEXT("����"), MB_OK | MB_ICONERROR);
		return sec;
	}

	//���ش������ļ���ȡ��������
	if (N == 1)
		*RunTime = run_time;
	else if (N == 12)
	{
		*RunTime = run_time;
		*BreakTime = break_time;
	}
	else if (N == 34)
	{
		strcpy_s(Time_Period, 15, time_period);
		strcpy_s(Date, 100, date);
	}
	return sec;
}



/*****************************************************************************
							   ��һ����������
******************************************************************************/
void FirstStart(char* cmd)
{
	//��ʾ�������д���
	HWND hwnd = FindWindow(TEXT("ConsoleWindowClass"), NULL);
	if (hwnd)
		ShowWindow(hwnd, SW_SHOW);

	system("title ��һ������... && mode con cols=52 lines=17");

	HKEY hkey;
	char c;
	unsigned i, j = 0;
	int  front_hour, front_min, behind_hour, behind_min;
	char config_content[MAX_SIZE] = { 0 };		//���潫Ҫд�������ļ�������               
	wchar_t W_path[MAX_PATH];				    //����timec���ذ�.exe��·��    
	char temp_time_period[15], time_period[15]; //���������е�ʱ���
	char run_time[5], break_time[5];	        //���������е�ʱ�����Ϣʱ��
	char date[100] = { 0 };						//�������ÿ���������е�����
	wchar_t* lpSubKey = { L"Software\\Microsoft\\Windows\\CurrentVersion\\Run" }; //ϵͳ������·��


	for (i = 5; i > 0; i--)
	{
		printf("\t      timec���ذ� ��һ������...\n\n\n\n\n\n\t\t    ���ڳ�ʼ��...\n\n\n\n\n");
		printf("��ܰ��ʾ�����Թ���ԱȨ�����б���������˳�ɱ�������\n\n\n");
		printf("\t\t\t%d", i);
		Sleep(1000);
		system("cls");
	}

	/***********************/
begin://goto����ǩ
/***********************/

	system("cls && mode con cols=72 lines=25");

	printf("��ܰ��ʾ�����������趨���������ڡ�timec�Զ���桱����9������5�޸ġ�");
	printf("\n\n���������ÿ�������е�ʱ��Σ���ʽ��xx:xx - xx:xx���벻Ҫ�������ı�㡣\n��ܰ��ʾ������00:00 - 00:00����������ʱ��Ρ�����");
	while (1)
	{
		Initialize(temp_time_period, time_period, 2);

		fgets(time_period, 15, stdin);
		rewind(stdin);
		Fgets_n(time_period);	//���fgets��ȡ��'\n'

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


	printf("\n\n���������ÿ���������е����ڣ����磺05��\n��ܰ��ʾ�����Ҫ��������������Ӣ�Ķ��ŷָ��������磺1,5,10,25,30��\n����00�������������ڡ�����");
	while (1)
	{
		Initialize(date, NULL, 1);
		fgets(date, 100, stdin);
		rewind(stdin);
		Fgets_n(date);

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
			Flush();
		}
		Flush();
		if (i > 999)
		{
			printf("\n���ֵ���ܳ���999�֣����������룺");
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
			Flush();
		}
		Flush();
		if (i > 999)
		{
			printf("\n���ֵ���ܳ���999�֣����������룺");
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
			Flush();
		}
		Flush();
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
		Flush();

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


	//����Ŀ¼
	if (_access(folder_path, 0) == -1)//�ж�Ŀ¼�Ƿ����
	{
		if (_mkdir(folder_path) != 0)
		{
			MessageBox(NULL, TEXT("����Ŀ¼ʧ�ܣ����Թ���ԱȨ�����б������"), TEXT("ERROR"), MB_YESNO | MB_ICONERROR);
			exit(EXIT_FAILURE);
		}
		system(cmd);//ִ��cmd��������Ŀ¼
	}

	//���������ļ�
	if ((err = fopen_s(&fp, config_file_path, "w")) != 0)
	{
		perror("\n���������ļ�ʧ��");
		system("pause");
		exit(EXIT_FAILURE);
	}
	fputs(config_content, fp);//�������ļ��������
	fclose(fp);

	system("cls && title ������������������˳�ɱ�� &&mode con cols=72 lines=25");
	printf("\n\n\n\n\nע�⣺������������������⵽ɱ�����أ��������������������\n\n\t                ��������޷�����������\n\n\n");
	Sleep(3000);


	//���������ļ�
	if ((err = fopen_s(&fp, time_dat_path, "w")) != 0)
	{
		perror("\n���������ļ�ʧ��");
		system("pause");
		exit(EXIT_FAILURE);
	}
	fclose(fp);

	//�����ļ�
	GetModuleFileName(NULL, W_path, MAX_PATH); //����windows api��á�timec���ذ�.exe����·��
	if (!CopyFile(W_path, W_dest_path, FALSE))
	{
		if (MessageBox(NULL, TEXT("�����ļ�ʧ�ܣ����Թ���ԱȨ�����Ի�����һ��ģʽ�����������\n\t�Ƿ�����һ��ģʽ�����������"), TEXT("����"), MB_YESNO | MB_ICONERROR) == IDNO)
			exit(EXIT_FAILURE);
		else
			CopyFileToStartFolder(W_path);
	}


	//��ע������������
	if (RegOpenKeyEx(HKEY_CURRENT_USER, lpSubKey, 0, KEY_ALL_ACCESS, &hkey) == ERROR_SUCCESS)
	{  //���һ����Key,������ֵ
		RegSetValueExA(hkey, "timec���ذ�", 0, REG_SZ, (BYTE*)dest_path, strlen(dest_path));
		RegCloseKey(hkey);//�ر�ע���

		i = MessageBox(NULL, TEXT("����������ɹ�����������Ч���Ƿ�����������\nע�⣺1������ȡ��������������timec�Զ����ɾ����\n            2������ǰ�뱣����������ϣ�"), TEXT("�ɹ�"), MB_YESNO | MB_ICONQUESTION);
		if (i == IDYES)
		{
			system("shutdown -a");
			system("shutdown -r -t 00");
		}
	}
	else
	{
		if (MessageBox(NULL, TEXT("���������ʧ�ܣ����Թ���ԱȨ�����Ի�����һ��ģʽ�����������\n\t�Ƿ�����һ��ģʽ�����������"), TEXT("ʧ��"), MB_OK | MB_ICONERROR) == IDNO)
		{
			remove(folder_path);
			exit(EXIT_SUCCESS);
		}
		else
			CopyFileToStartFolder(W_path);
	}
	exit(EXIT_SUCCESS);
}



/*****************************************************************************
							 ִ�йػ�����
*****************************************************************************/
void ShutDown(void)
{
	int  run_time;
	char command[40];

	//�������ļ���ȡ���������е�ʱ��
	Read_File(&run_time, NULL, NULL, NULL, 1);
	run_time *= 60;//�ѷ�ת��Ϊ��

	system("shutdown -a");
	sprintf_s(command, 40, "shutdown -f -s -t %d", run_time);
	system(command);
	Shutdown_Tip(run_time);
}



/*****************************************************************************
				��ǰʱ�����ػ�ʱ�仹��3��ʱ����ػ���ʾ
*****************************************************************************/
void Shutdown_Tip(const long sec)
{
	long second = sec - 180;
	char tip[SIZE];

	if (second <= 180)
	{
		system("shutdown -a");
		sprintf_s(tip, SIZE, "shutdown -f -s -t %d -c ���� %d�� ��Ҫ�ػ��ˣ�������׼����", second, second / 60);
		system(tip);
		Initialize(tip, NULL, 1);
	}
	else
	{
		Sleep(second * 1000);
		system("shutdown -a");
		system("shutdown -f -s -t 180 -c ���� 3�� ��Ҫ�ػ��ˣ�������׼����");
	}

	second -= 5;
	if (second <= 5)
	{
		system("shutdown -a");
		sprintf_s(tip, SIZE, "shutdown -f -s -t %d -c ���� %d�� ��Ҫ�ػ��ˣ�������׼����", second, second);
	}
	else
	{
		Sleep(second * 1000);
		system("shutdown -a");
		system("shutdown -f -s -t 5 -c ���� 5�� ��Ҫ�ػ��ˣ�������׼����");
	}
	exit(EXIT_SUCCESS);
}


/*****************************************************************************
								���������
*****************************************************************************/
void Flush(void)
{
	int ch;
	while ((ch = getchar()) != '\n' && ch != EOF)
		;
}


/*****************************************************************************
							���fgets��ȡ��'\n'
*****************************************************************************/
void Fgets_n(char* str)
{
	char* find;
	if ((find = strchr(str, '\n')))
		*find = '\0';
}


/*****************************************************************************
								��ʼ������
*****************************************************************************/
void Initialize(char* str1, char* str2, int n)
{
	while (*str1 != '\0')
	{
		if (n == 1)
			*str1++ = '\0';
		else if (n == 2)
			*str2++ = *str1++ = '\0';
	}
}


/*****************************************************************************
					   ���������ģʽ2���Ƶ�����Ŀ¼
*****************************************************************************/
void CopyFileToStartFolder(wchar_t* source)
{
	wchar_t W_DestPath[MAX_PATH];

	if (GetEnvironmentVariable(L"APPDATA", W_DestPath, SIZE) == 0) //��ȡ�û��ļ���
		wcscpy_s(W_DestPath, SIZE, L"C:\\Users\\Administrator\\AppData\\Roaming\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\\timec���ذ�.exe");//��ȡʧ����Ĭ��·��
	else
		wcscat_s(W_DestPath, MAX_PATH, L"\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\\timec���ذ�.exe");

	//�����ļ�
	if (!CopyFile(source, W_DestPath, FALSE))
	{
		//printf("%d", GetLastError());
		MessageBox(NULL, TEXT("�����ļ�ʧ�ܣ����Թ���ԱȨ�����ԡ�"), TEXT("����"), MB_OK | MB_ICONERROR);
		exit(EXIT_FAILURE);
	}

	int i = MessageBox(NULL, TEXT("����������ɹ�����������Ч���Ƿ�����������\nע�⣺1������ȡ��������������timec�Զ����ɾ����\n            2������ǰ�뱣����������ϣ�"), TEXT("�ɹ�"), MB_YESNO | MB_ICONQUESTION);
	if (i == IDYES)
	{
		system("shutdown -a");
		system("shutdown -r -t 00");
		exit(EXIT_SUCCESS);
	}
	else if (i == IDNO)
		exit(EXIT_SUCCESS);
}