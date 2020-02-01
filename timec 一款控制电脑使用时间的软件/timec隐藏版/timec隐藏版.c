#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <direct.h>
#include <io.h>

#define SIZE 50
#define MAX_SIZE 600

int  GetTime(void);         //�Ƚϵ�ǰʱ��͵�����������ʱ��
int  WriteTime(void);       //д����Թػ�ʱ����´�������ʱ��
void ShutDown(void);	    //ִ�йػ�����
int  Read_File(int *RunTime, int *BreakTime, char *Time_Period, char *Date, int N);//�������ļ���ȡ���������е�ʱ�����Ϣʱ��
void Shutdown_Tip(const long sec);//������ػ�ʱ�仹��3����ʱ����ػ���ʾ
void FirstStart(void);	    //��һ�������Զ������
void Flush(void);		    //���������
void Fgets_n(char *str);    //���fgets��ȡ��'\n'
void Initialize(char *str1, char *str2, int n);//��ʼ������


/***************************************************************************************************************
*    ���磺��������40���ӣ�Ҫ��Ϣ10���Ӻ���ܼ������С�														   *
*    ���統ǰʱ��Ϊ2019��11��23��11:20�����趨��������40���Ӿ�Ҫ��Ϣ10���ӣ���������ֻ����10:00-13:00���С�    *
*    Ҳ����˵Ҫ�ڵ�ǰʱ��Ļ����ϼ���50���ӣ�40���Ӻ�ػ�+10������Ϣ����������´򿪵��ԡ�				 	   *
*    ��Ϊ��������40���Ӻ�ػ�����ʱ��ʱ����2019��11��23��12:00���ټ���10������Ϣʱ�䣬						   *
*    ������Ҫ��2019��11��23��12:10������ٴδ򿪵��ԣ��������ʾ�ػ���										   *
*    �������10:00-13:00���ʱ�����򿪵��ԣ�Ҳ����ʾ�ػ���												   *
****************************************************************************************************************/


//�����ļ�·��
char time_dat_path[] = { "C:\\Program Files\\timec\\time.dat" };
char config_file_path[] = { "C:\\Users\\Timec_config.txt" };
char folder_path[] = { "C:\\Program Files\\timec" };


int main(int argc, char *argv[])
{
	//���س������д���
	HWND hwnd = FindWindow("ConsoleWindowClass", NULL);
	if (hwnd)
		ShowWindow(hwnd, SW_HIDE);

	if (argc == 2)
	{
		if (strcmp(argv[1], "-reset") == 0)
		{
			if (_access(folder_path, 0))//�ж�Ŀ¼�Ƿ����
			{
				if (_mkdir(folder_path) != 0)
				{
					MessageBox(NULL, TEXT("����Ŀ¼ʧ�ܣ����Թ���ԱȨ�����У�"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
					exit(EXIT_FAILURE);
				}
			}
			SetFileAttributes(folder_path, FILE_ATTRIBUTE_HIDDEN);//����windows api����Ŀ¼
			FirstStart();
		}
	}
	//�ж������ļ���time.dat���Ƿ����
	if (_access(time_dat_path, 0))
	{//�������򴴽�Ŀ¼���õ�һ����������
		if (_access(folder_path, 0))//�ж�Ŀ¼�Ƿ����
		{
			if (_mkdir(folder_path) != 0)
			{
				MessageBox(NULL, TEXT("����Ŀ¼ʧ�ܣ����Թ���ԱȨ�����У�"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
				exit(EXIT_FAILURE);
			}
		}
		SetFileAttributes(folder_path, FILE_ATTRIBUTE_HIDDEN);//����windows api����Ŀ¼
		FirstStart();
	}

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
	int  temp;
	char temp_time[SIZE] = { 0 }, behind_time[15] = { 0 };
	char command[100] = { "shutdown -s -t " };
	char last_time[SIZE];        //�����´������е�ʱ��������������ϴιرպ��ʱ�������Ϣʱ�䣩
	char time_now[SIZE];         //���浱ǰʱ��
	char shut_down_time[SIZE];   //������Թػ�ʱ��
	char date[100] = { 0 };	     //������������е�����
	char time_period[15] = { 0 };//������������е�ʱ���
	char tip[100] = { 0 };	     //��������ǰ����ʱ����ػ���ʾ
	const char lpSubKey[] = { "SYSTEM\\CurrentControlSet\\Control\\Windows" };
	
	FILE *fp;
	errno_t err;
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


	Read_File(NULL, NULL, time_period, date, 34);//��ȡ���������е�ʱ��κ�����


	//�жϵ�ǰʱ���Ƿ���ʱ�����
	if (strcmp(time_period, "00:00-00:00") != 0)//�ж��Ƿ���ʱ�������
	{
		strcat_s(tip, 100, "���ڡ�");
		strcat_s(tip, 100, time_period);
		strcat_s(tip, 100, "�������б����ԣ�\n�����Լ����ػ���");

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

		//����ǰʱ��д������
		sprintf_s(temp_time, 15, "%d%d", tm_current_time.tm_hour, tm_current_time.tm_min);

		//�жϵ�ǰʱ���Ƿ��������е�ʱ�����
		if (strcmp(temp_time, time_period) > 0 && strcmp(temp_time, behind_time) < 0)
			;
		else
		{
			system("shutdown -a");
			system("shutdown -s -t 10");//ִ�йػ�����
			MessageBox(NULL, (tip), TEXT("���棺"), MB_OK | MB_ICONWARNING);
			system("shutdown -s -t 00");//ִ�йػ�����
		}
		Initialize(temp_time, NULL, 1);//�������
	}
	
	Initialize(tip, NULL, 1);


	//�жϵ�ǰ�����Ƿ�������
	if (strcmp(date, "000") != 0)
	{
		strcat_s(tip, 100, "����ÿ���µġ�");
		strcat_s(tip, 100, date);
		strcat_s(tip, 100, "�������б����ԣ�\n�����Լ����ػ���");

		for (i = 0; i <= 100; i++)
		{
			Initialize(temp_time, NULL, 1);
			if (i == 100)
			{
				system("shutdown -a");
				system("shutdown -s -t 10");//ִ�йػ�����
				MessageBox(NULL, (tip), TEXT("���棺"), MB_OK | MB_ICONWARNING);
				system("shutdown -s -t 00");//ִ�йػ�����
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

	Initialize(tip, NULL, 1);

	//�򿪡�time.dat��,���ж��ļ��Ƿ�������
	if ((err = fopen_s(&fp, time_dat_path, "r")) != 0)
		return 1;//�����ʧ�ܾͷ���������ִ�йػ�����

	if (!_filelength(_fileno(fp)))//�ж��ļ��Ƿ�Ϊ��
	{
		fclose(fp);
		return 1;
	}

	//�ӡ�time.dat����ȡ����
	for (i = 0; i < 4; i++)
	{
		if (i == 1)
			//�ӡ�time.dat����ȡ��һ�У����Թػ���ʱ�������
			fgets(shut_down_time, SIZE, fp);
		else if (i == 2)
			//��time.dat����ȡ�ڶ��У������´������е�ʱ�������
			fgets(last_time, SIZE, fp);
		else if (i == 3)
			//�ӡ�time.dat����ȡ�����У���ʾ
			fgets(tip, 100, fp);
	}
	fclose(fp);
	//���fgets��ȡ�Ļ��з�
	Fgets_n(shut_down_time);
	Fgets_n(last_time);

	//������ļ��ж�ȡ�������ַ�
	strcpy_s(shut_down_time, strlen(shut_down_time) + 1, shut_down_time + 22);
	strcpy_s(last_time, strlen(last_time) + 1, last_time + 28);

	//��1970-01-01 00:00:00����ǰʱ�������д�뵽����
	sprintf_s(time_now, SIZE, "%lld", (long long)current_time);

	//�жϵ�ǰʱ���Ƿ�С�ڵ��Թػ�ʱ��
	if (strcmp(shut_down_time, time_now) > 0)
	{
		//��ȡ�����ϴιػ�ʱ��
		if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpSubKey, 0, KEY_READ, &hkey) != ERROR_SUCCESS)
			ShutDown();

		if (RegQueryValueEx(hkey, TEXT("ShutdownTime"), NULL, NULL, NULL, &dwsize) == ERROR_SUCCESS && dwsize == sizeof(file_time))
		{
			RegQueryValueEx(hkey, TEXT("ShutdownTime"), NULL, NULL, (LPBYTE)&file_time, &dwsize);

			//FileTimeToSystemTime��FILETIMEʱ��ת��ΪSYSTEMTIMEʱ��
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
		if ((long long)(current_time - shutdown_time) < 0)//����Ƿ�����
			ShutDown();

		if ((long long)(current_time - shutdown_time) > 300)//�жϵ�ǰʱ����ػ�ʱ���Ƿ�������5����
			return 0;//�����ǰʱ����ػ�ʱ��������5���Ӿ�����д��ػ�ʱ�䲢ִ�йػ�

		//�����ǰʱ����ػ�ʱ�����С��5���Ӿ������ǰʱ������´ιػ�ʱ�仹�ж��ٷ�
		sprintf_s(temp_time, SIZE, "%lld", (long long)(ShutDownTime - current_time));//��ʱ���д������
		strcat_s(command, 100, temp_time);
		system("shutdown -a");
		system(command);//ִ�йػ�����
		Shutdown_Tip((long)(ShutDownTime - current_time));
	}
	//�����жϵ�ǰʱ���Ƿ�С�ڵ�����������ʱ��
	else if (strcmp(last_time, time_now) > 0)
	{
		//�����ǰʱ��С�ڵ�����������ʱ���ִ��10��ػ����������ػ���ʾ
		system("shutdown -a");
		system("shutdown -s -t 10");
		//�����ػ���ʾ
		MessageBox(NULL, (tip), TEXT("���棡"), MB_OK | MB_ICONWARNING);
		system("shutdown -s -t 00");
		exit(EXIT_SUCCESS);
	}
	return 0;
}


/*****************************************************************************
д����Թرպ���Ϣ��ʱ�䣬�������´�������ʱ�䡣
���磺����������40������Ϣ10���ӣ�
Ҳ����˵�����´���������ʱ��Ҫ�ڵ�ǰʱ���ϼ���40��������ʱ���10������Ϣʱ�䡣
******************************************************************************/
int WriteTime(void)
{
	FILE *fp;
	errno_t err;
	time_t last_time;
	struct tm p;
	int min, hour, day, mon, year, run_time, break_time;
	long sec = 0;
	
	//��ȡ���������е�ʱ�����Ϣʱ��
	Read_File(&run_time, &break_time, NULL, NULL, 12);

	//���ļ�,�����ж��ļ��Ƿ�������
	if ((err = fopen_s(&fp, time_dat_path, "w")) != 0)
		return 1;


	time(&last_time);//��ȡ��ǰʱ�������
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
		if (year % MAX_SIZE == 0 || (year % 4 == 0 && year % 100 != 0))//�жϵ�ǰ����Ƿ�Ϊ����
		{
			//printf("������\n");
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
	//���ļ�д������´�������ʱ�������
	fprintf(fp, "���Թػ���ʱ���������%ld\n", sec);
	sec += (long)break_time;
	fprintf(fp, "�����´������е�ʱ���������%ld\n", sec);
	//���ļ�д����ʾ��Ϣ
	fprintf(fp, "��ǰΪ��Ϣʱ��,�´δ˵�����������ʱ��Ϊ��%02d��%02d��%02d��%02dʱ%02d�ֺ�    ���Լ����ػ���", year, mon, day, hour, min);
	fclose(fp);
	return 0;
}


/*****************************************************************************
							 ִ�йػ�����
*****************************************************************************/
void ShutDown(void)
{
	int  run_time;
	char command[22] = { "shutdown -s -t " };
	char temp[7];

	//�������ļ���ȡ���������е�ʱ��
	Read_File(&run_time, NULL, NULL, NULL, 1);
	run_time *= 60;//�ѷ�ת��Ϊ��

	sprintf_s(temp, 7, "%d", run_time);
	strcat_s(command, 22, temp);
	system(command);
	Shutdown_Tip(run_time);
}


/*****************************************************************************
				 �������ļ���ȡ���������е�ʱ���Լ���Ϣʱ��
*****************************************************************************/
int Read_File(int *RunTime, int *BreakTime, char *Time_Period, char *Date, int N)
{
	int ch, i, j = 0;
	int run_time = 40;	//Ĭ��������ʱ�䣨��λ���֣�
	int break_time = 10;//Ĭ����Ϣʱ�䣨��λ���֣�
	char config[100] = { 0 };//������ļ���ȡ�Ĳ���
	char time_period[12] = { "00:00-00:00" };//Ĭ�������е�ʱ���
	char temp[15];
	char *p = config;
	FILE *fp;
	errno_t err;


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
		for (i = 0; i < 12; i++)
			Time_Period[i] = time_period[i];
		for (i = 0; i < 3; i++)
			Date[i] = '0';
	}


	//�������ļ���ȡ����
	if ((err = fopen_s(&fp, config_file_path, "r")) != 0)//�ж��ļ��Ƿ�������
		return 1;


	if ((ch = fgetc(fp)) == EOF)  //�ж��ļ��Ƿ�Ϊ��
	{
		MessageBox(NULL, TEXT("��timec���ذ桱�����ļ�����\n���ڡ�timec�Զ���桱�������������ļ���\n����ӡ�-reset����������timec���ذ档"), TEXT("����"), MB_OK | MB_ICONERROR);
		return 1;
	}


	while ((ch = fgetc(fp)) != EOF)
	{
		if (ch == '=')        //�ж�ch�Ƿ�Ϊ��=��
		{
			while ((ch = fgetc(fp)) != ';')
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
				sscanf_s(config, "%d", &run_time); //�ѵ�һ�е����ݱ��浽run_time����һ�е������ǵ��������е�ʱ��
			else if (j == 1)//�ж��Ƿ����������
				sscanf_s(config, "%d", &break_time); //�ѵڶ��е����ݱ��浽break_time���ڶ��е������ǵ��Թػ�����Ϣ��ʱ��	
			else if (j == 2)//�ж��Ƿ����������
			{
				if (strchr(config, '��') || strchr(config, '����'))
				{
					MessageBox(NULL, TEXT("��⵽��timec���ذ桱�����ļ������ķ��ţ�\n���ڡ�timec�Զ���桱���������������ļ���\n����ӡ�-reset����������timec���ذ档"), TEXT("����"), MB_OK | MB_ICONERROR);
					return 1;
				}
				for (i = 0; i < 15; i++)
					temp[i] = config[i];
			}
			else if (j == 3)
				break;

			j++;
			i = 0;
			Initialize(config, NULL, 1);
		}
	}

	if (strchr(config, '��'))
	{
		MessageBox(NULL, TEXT("��⵽��timec���ذ桱�����ļ������ķ��ţ�\n���ڡ�timec�Զ���桱���������������ļ���\n����ӡ�-reset����������timec���ذ档"), TEXT("����"), MB_OK | MB_ICONERROR);
		return 1;
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
		for (i = 0; i < 15; i++)
			Time_Period[i] = temp[i];

		while (*Date++ = *p++)
			;
	}
	return 0;
}


/*****************************************************************************
							   ��һ����������
******************************************************************************/
void FirstStart(void)
{
	//��ʾ�������д���
	HWND hwnd = FindWindow("ConsoleWindowClass", NULL);
	if (hwnd)
		ShowWindow(hwnd, SW_SHOW);

	system("title ��һ������... && mode con cols=52 lines=17");

	char c;
	unsigned i, j = 0;
	int  front_hour, front_min, behind_hour, behind_min;
	char config_content[MAX_SIZE] = { 0 };		//���潫Ҫд�������ļ�������
	char path[MAX_PATH];				        //����timec���ذ�.exe��·��
	char temp_time_period[15], time_period[15]; //���������е�ʱ���
	char run_time[5], break_time[5];	        //���������е�ʱ�����Ϣʱ��
	char date[100] = { 0 };						//�������ÿ���������е�����
	char *lpSubKey = { "Software\\Microsoft\\Windows\\CurrentVersion\\Run" }; //ϵͳ������·��
	char *timec_path = { "C:\\Program Files\\timec\\timec���ذ�.exe" };		  //ע���������·���Լ����Ƶ�Ŀ���ļ���·��
	char *buffer = (char *)malloc(1024);	    //����һ�黺��

	HKEY hkey;
	FILE *fpread, *fpwrite;
	errno_t err;


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


	printf("\n\n���������ÿ���������е����ڣ����磺05��\n��ܰ��ʾ�����Ҫ��������������Ӣ�Ķ��ŷָ��������磺1,5,10,25,30��\n����000�������������ڡ�����");
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
			Flush();
		}
		Flush();
		if (i > 999)
		{
			printf("\n���ֵ���ܳ���999�����������룺");
			continue;
		}
		sprintf_s(break_time, 5, "%d", i);
			break;
	}

	system("cls && mode con cols=47 lines=15");
	printf("��ǰ���õĲ���Ϊ��\n\n");

	if(strcmp(temp_time_period,"00:00-00:00")==0)
		printf("����ÿ�������е�ʱ��Σ� %s������������ʱ��Σ�\n", temp_time_period);

	else if(strcmp(temp_time_period, "00:00 - 00:00") == 0)
		printf("����ÿ�������е�ʱ��Σ� %s������������ʱ��Σ�\n", temp_time_period);

	else
		printf("����ÿ�������е�ʱ��Σ� %s", temp_time_period);

	if(strcmp(date,"000")==0)
		printf("\n����ÿ���������е����ڣ� %s���������������ڣ�", date);
	else
	printf("\n����ÿ���������е����ڣ� %s", date);

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

	
	sprintf_s(config_content, MAX_PATH, "run time = %s ;               ���������е�ʱ�䡣\nbreak time = %s ;             ������Ϣ��ʱ�䡣\ntime period = %s ;   ���������е�ʱ��Ρ�\ndate = %s ;", run_time, break_time, temp_time_period, date);
	strcat_s(config_content, MAX_SIZE, "                   ���������е����ڡ�\n\nע�⣺��run time���͡�break time������ֵ�ĵ�λ��Ϊ�֣�ֵ��Χ5~999��\n��time period����ֵ�ĸ�ʽΪ��xx:xx-xx:xx�����벻Ҫ�������ı�㣨�磺����������\n�����뱣��ԭ��ʽ��������ֳ������������У���ɾ�����ļ���");
	
	
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


	system("cls && title ������������������˳�ɱ�� &&mode con cols=72 lines=25");
	printf("\n\n\n\n\nע�⣺������������������⵽ɱ�����أ��������������������\n\n\t                ��������޷�����������\n\n\n");
	Sleep(3000);

	GetModuleFileName(NULL, path, 100); //����windows api��á�timec���ذ�.exe����·��


	if (((err = fopen_s(&fpwrite, timec_path, "wb")) != 0) || ((err = fopen_s(&fpread, path, "rb")) != 0))
	{
		remove(folder_path);
		perror("\n�����ļ�ʧ��");
		system("pause");
		exit(EXIT_FAILURE);
	}

	//�����ļ�
	while (fread(buffer, sizeof(char), 1024, fpread) > 0)
		fwrite(buffer, sizeof(char), 1024, fpwrite);

	free(buffer);  //�ͷ��ڴ�
	fclose(fpread);//�ر��ļ�
	fclose(fpwrite);
	buffer = NULL;//�ͷ��ڴ����bufferָ��NULL


	//���������ļ�
	if ((err = fopen_s(&fpwrite, time_dat_path, "w")) != 0)
	{
		remove(folder_path);
		perror("\n���������ļ�ʧ��");
		system("pause");
		exit(EXIT_FAILURE);
	}
	fclose(fpwrite); //�����ļ�����Ϊ��

	//��ע������������
	if (RegOpenKeyEx(HKEY_CURRENT_USER, lpSubKey, 0, KEY_ALL_ACCESS, &hkey) == ERROR_SUCCESS)
	{  //���һ����Key,������ֵ
		RegSetValueEx(hkey, "timecyincang", 0, REG_SZ, (BYTE *)timec_path, strlen(timec_path));
		RegCloseKey(hkey);//�ر�ע���

		i = MessageBox(NULL, TEXT("���óɹ�����������Ч���Ƿ�����������\nע�⣺1������ȡ��������������timec�Զ����ɾ����\n            2������ǰ�뱣����������ϣ�"), TEXT("���óɹ���"), MB_YESNO | MB_ICONQUESTION);
		if (i == IDYES)
			system("shutdown -r -t 00");
		else if (i == IDNO)
			exit(EXIT_SUCCESS);
	}
	else
	{
		printf("\n\n���������ʧ�ܣ�");
		MessageBox(NULL, TEXT("����ʧ�ܣ����Թ���ԱȨ�����б�������ԣ�"), TEXT("����ʧ�ܣ�"), MB_OK | MB_ICONERROR);
		remove(folder_path);
		exit(EXIT_SUCCESS);
	}
	exit(EXIT_SUCCESS);
}


/*****************************************************************************
				��ǰʱ�����ػ�ʱ�仹��3��ʱ����ػ���ʾ
*****************************************************************************/
void Shutdown_Tip(const long sec)
{
	long second = sec - 180;

	if (second < 180)
		exit(EXIT_SUCCESS);

	second *= 1000;
	Sleep(second);
	MessageBox(NULL, TEXT("����3���Ӿ�Ҫ�ػ��ˣ�������׼����"), TEXT("����"), MB_OK | MB_ICONWARNING);
	system("shutdown -a && shutdown -s -t 180");
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
void Fgets_n(char *str)
{
	char *find;
	if ((find = strchr(str, '\n')))
		*find = '\0';
}


/*****************************************************************************
								��ʼ������
*****************************************************************************/
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