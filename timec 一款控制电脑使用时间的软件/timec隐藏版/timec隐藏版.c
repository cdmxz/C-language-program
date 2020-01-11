#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <direct.h>
#include <io.h>

#define SIZE 50

int  GetTime(void);        //�Ƚϵ�ǰʱ��͵�����������ʱ��
int  WriteTime(void);      //д����Թػ�ʱ����´�������ʱ��
void ShutDown(void);	   //ִ�йػ�����
int  Read_File(int *RunTime, int *BreakTime, char *str);//�������ļ���ȡ���������е�ʱ�����Ϣʱ��
void Shutdown_Tip(long sec);//������ػ�ʱ�仹��3����ʱ����ػ���ʾ
void FirstStart(void);	   //��һ�������Զ������
void Flush(void);		   //���������
void Fgets_n(char *str);   //���fgets��ȡ��'\n'
void Initialize(char *temp_time_period, char *time_period);//��ʼ������

														   /*���磺��������40���ӣ�Ҫ��Ϣ10���Ӻ���ܼ������С�
���統ǰʱ��Ϊ2019��11��23��11:20�����趨��������40���Ӿ�Ҫ��Ϣ10���ӡ�
Ҳ����˵Ҫ�ڵ�ǰʱ��Ļ����ϼ���50���Ӻ�������´򿪵��ԡ�
��Ϊ��������40���Ӻ�ػ�����ʱ��ʱ����2019��11��23��12:00���ټ���10������Ϣʱ�䣬
������Ҫ��2019��11��23��12:10������ٴδ򿪵��Է������ʾ�ػ���*/

//�����ļ�·��
char time_dat_path[] = { "C:\\Program Files\\timec\\time.dat" };
char config_file_path[] = { "C:\\Users\\Timec_config.txt" };
char folder_path[] = { "C:\\Program Files\\timec" };
char str[15];//����
int x;//����

int main(void)
{		
	//���س������д���
	HWND hwnd;
	hwnd = FindWindow("ConsoleWindowClass", NULL);
	if (hwnd)
		ShowWindow(hwnd, SW_HIDE);

	//�ж������ļ���time.dat���Ƿ����
	if (_access(time_dat_path, 0))
	{//�������򴴽�Ŀ¼���õ�һ����������
		_mkdir(folder_path);//����Ŀ¼
		SetFileAttributes(folder_path, FILE_ATTRIBUTE_HIDDEN);//����windows api����Ŀ¼
		FirstStart();
	}
	
	GetTime();
	WriteTime();
	ShutDown();
	return 0;
}


//�Ƚ�ʱ�� 
int GetTime(void)
{
	int i, front_hour, front_min, behind_hour, behind_min;
	long long ShutDownTime;
	char temp_time[SIZE];
	char command[100] = { "shutdown -s -t " };
	char last_time[SIZE];      //�����´������е�ʱ��������������ϴιرպ��ʱ�������Ϣʱ�䣩
	char time_now[SIZE];       //���浱ǰʱ��
	char shut_down_time[SIZE]; //������Թػ�ʱ��
	char tip[100] = { 0 };	   //��������ǰ����ʱ����ػ���ʾ
	char lpSubKey[] = { "SYSTEM\\CurrentControlSet\\Control\\Windows" };
	char time_period[15] = { 0 };
	char time_period_tip[100] = { "���ڡ�" };

	FILE *fp;
	errno_t err;
	HKEY hKey;
	FILETIME file_time;
	DWORD dwsize;
	SYSTEMTIME system_time;
	ULARGE_INTEGER ui;
	time_t current_time, shutdown_time;

	//��ȡ1970-01-01����ǰ������
	struct tm  tm_current_time;
	time(&current_time);
	localtime_s(&tm_current_time, &current_time);//����ȡ������ת��Ϊ����ʱ��

	//��ȡ���������е�ʱ���
	Read_File(&x, &x, time_period);

	//�жϵ�ǰʱ���Ƿ���ʱ�����
	if (strcmp(time_period, "00:00-00:00") != 0)//�ж��Ƿ���ʱ�������
	{
		strcat_s(time_period_tip, 100, time_period);
		strcat_s(time_period_tip, 100, "��ʱ��������б����ԣ�\n�����Խ���10���ڹػ���");

		for (i = 0; i < 15; i++)
			if (time_period[i] == '-' || time_period[i] == ':')
				time_period[i] = ' ';
		//���������ʱ���д������
		sscanf_s(time_period, "%d %d %d %d", &front_hour, &front_min, &behind_hour, &behind_min);
		
		//�жϵ�ǰʱ���Ƿ���ʱ�����
		if ((tm_current_time.tm_hour > front_hour) && (tm_current_time.tm_hour < behind_hour))
			;
		else if ((tm_current_time.tm_hour == front_hour) || (tm_current_time.tm_hour == behind_hour))
		{
			if ((tm_current_time.tm_min >= front_min) && (tm_current_time.tm_min < behind_min))
				;
			else
			{
				system("shutdown -s -t 10");
				MessageBox(NULL, (time_period_tip), TEXT("���棺"), MB_OK | MB_ICONWARNING);
			}
		}
		else
		{
			system("shutdown -s -t 10");
			MessageBox(NULL, (time_period_tip), TEXT("���棺"), MB_OK | MB_ICONWARNING);
		}
	}
	
	
	//�򿪡�time.dat��,���ж��ļ��Ƿ�������
	if ((err = fopen_s(&fp, time_dat_path, "r")) != 0)
		return 1;//�����ʧ�ܾͷ���������ִ�йػ�����
	
	if (_filelength(_fileno(fp)) == 0)//�ж��ļ��Ƿ�Ϊ��
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
	
	//��������е������ַ�
	strcpy_s(shut_down_time, strlen(shut_down_time) + 1, shut_down_time + 22);
	strcpy_s(last_time, strlen(last_time) + 1, last_time + 28);
	
	//����ǰʱ�������д�뵽����
	sprintf_s(time_now, SIZE, "%lld", (long long)current_time);

	//�жϵ�ǰʱ���Ƿ�С�ڵ��Թػ�ʱ��
	if (strcmp(shut_down_time, time_now) > 0)
	{	
		//��ȡ�����ϴιػ�ʱ��
		if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpSubKey, 0, KEY_READ, &hKey) != ERROR_SUCCESS)
			ShutDown();

		if (RegQueryValueEx(hKey, TEXT("ShutdownTime"), NULL, NULL, NULL, &dwsize) == ERROR_SUCCESS && dwsize == sizeof(file_time))
		{
			RegQueryValueEx(hKey, TEXT("ShutdownTime"), NULL, NULL, (LPBYTE)&file_time, &dwsize);

			//FileTimeToSystemTime��FILETIMEʱ��ת��ΪSYSTEMTIMEʱ��
			FileTimeToSystemTime(&file_time, &system_time);
			ui.LowPart = file_time.dwLowDateTime;
			ui.HighPart = file_time.dwHighDateTime;
			shutdown_time = (LONGLONG)(ui.QuadPart - 116444736000000000) / 10000000;
		}
		RegCloseKey(hKey);

		sscanf_s(shut_down_time, "%lld", &ShutDownTime);//����������д������
		
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
		system(command);//ִ�йػ�����
		Shutdown_Tip((long)(ShutDownTime - current_time));
		//exit(EXIT_SUCCESS);
	}
	//�����жϵ�ǰʱ���Ƿ�С�ڵ�����������ʱ��
	else if (strcmp(last_time, time_now) > 0)
	{
		//�����ǰʱ��С�ڵ�����������ʱ���ִ��10��ػ����������ػ���ʾ
		system("shutdown -s -t 10");
		//�����ػ���ʾ
		MessageBox(NULL, (tip), TEXT("���棡"), MB_OK | MB_ICONWARNING);
		exit(EXIT_SUCCESS);
	}
	return 0;
}


/*д����Թرպ���Ϣ��ʱ�䣬�������´�������ʱ�䡣
���磺����������40������Ϣ10���ӣ�
Ҳ����˵�����´���������ʱ��Ҫ�ڵ�ǰʱ���ϼ���40��������ʱ���10������Ϣʱ��*/
int WriteTime(void)
{
	FILE *fp;
	errno_t err;
	int min, hour, day, mon, year, run_time, break_time;
	long sec = 0;
	Read_File(&run_time, &break_time, str);

	//���ļ�,�����ж��ļ��Ƿ�������
	if ((err = fopen_s(&fp, time_dat_path, "w")) != 0)
		return 1;


	time_t last_time;
	struct tm p;
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
		while (day > 31)
		{
			day -= 31;
			mon++;
		}

	if (mon == 4 || mon == 6 || mon == 9 || mon == 11)//�жϵ�ǰ�·��Ƿ�ΪС��
		while (day > 30)
		{
			day -= 30;
			mon++;
		}

	if (mon == 2)
		if (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0))//�жϵ�ǰ����Ƿ�Ϊ����
		{
			//printf("������\n");
			while (day > 29)
			{
				day -= 29;//����2����29��
				mon++;
			}
		}
		else while (day > 28)
		{
			day -= 28;//ƽ��2����28��
			mon++;
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
	fprintf(fp, "��ǰΪ��Ϣʱ��,�´δ˵�����������ʱ��Ϊ��%02d��%02d��%02d��%02dʱ%02d�ֺ�    ���Լ�����10���ڹػ���", year, mon, day, hour, min);
	fclose(fp);
	return 0;
}


//ִ�йػ�����
void ShutDown(void)
{
	int  run_time;
	char command[22] = { "shutdown -s -t " };
	char temp[7];

	//�������ļ���ȡ���������е�ʱ��
	Read_File(&run_time, &x, str);
	run_time *= 60;//�ѷ�ת��Ϊ��

	sprintf_s(temp, 7, "%d", run_time);
	strcat_s(command, 22, temp);
	system(command);
	Shutdown_Tip(run_time);
}


//�������ļ���ȡ���������е�ʱ���Լ���Ϣʱ��
int Read_File(int *RunTime, int *BreakTime, char *str)
{
	int ch, i, j = 0;
	int run_time = 40;	//Ĭ��������ʱ�䣨��λ���֣�
	int break_time = 10;//Ĭ����Ϣʱ�䣨��λ���֣�
	char config[15] = { 0 };
	char time_period[] = { "00:00-00:00" };//Ĭ�������е�ʱ���
	FILE *fp;
	errno_t err;


	*RunTime = run_time;
	*BreakTime = break_time;
	for (i = 0; i < 12; i++)
		str[i] = time_period[i];

	//�������ļ���ȡ����
	if ((err = fopen_s(&fp, config_file_path, "r")) != 0)//�ж��ļ��Ƿ�������
		return 1;//��ʧ�ܾͷ���Ĭ��ʱ��


	if ((ch = fgetc(fp)) == EOF)  //�ж��ļ��Ƿ�Ϊ��
	{
		MessageBox(NULL, TEXT("����"), TEXT("��timec���ذ桱�����ļ�����\n���ڡ�timec�Զ���桱�������������ļ���"), MB_OK | MB_ICONERROR);
		return 1;
	}
	do
	{
		ch = fgetc(fp);
		if (ch == '=')        //�ж�ch�Ƿ�Ϊ��=��
			while ((ch = fgetc(fp)) != ';')
			{
				if (ch == ' ')//�ж�ch�Ƿ�Ϊ�ո�
				{
					i = 0;
					continue;//������ȡ�Ŀո�
				}
				config[i] = ch;
				i++;
			}
		if (ch == '\n')       //�ж��Ƿ������һ��
		{
			if (j == 0)       //�ж��Ƿ�����ڶ���
			{
				sscanf_s(config, "%d", &run_time); //�ѵ�һ�е����ݱ��浽run_time����һ�е������ǵ��������е�ʱ��
				for (i = 0; i < 12; i++)
					config[i] = '\0';//��ʼ������
			}
			else if (j == 1)
			{
				sscanf_s(config, "%d", &break_time); //�ѵڶ��е����ݱ��浽break_time���ڶ��е������ǵ��Թػ�����Ϣ��ʱ��
				for (i = 0; i < 12; i++)
					config[i] = '\0';//��ʼ������
			}
			j++;
			i = 0;
		}
	} while (ch != EOF);

	if(config[2]!=':'||config[5]!='-'||config[8]!=':')
	{
		MessageBox(NULL, TEXT("����"), TEXT("��timec���ذ桱�����ļ�����\n���ڡ�timec�Զ���桱�������������ļ���"), MB_OK | MB_ICONERROR);
		return 1;
	}
	//���ش������ļ���ȡ��������
	*RunTime = run_time;
	*BreakTime = break_time;
	for (i = 0; i < 12; i++)
		str[i] = config[i];
	return 0;
}


//��һ����������
void FirstStart(void)
{
	//��ʾ�������д���
	HWND hwnd;
	hwnd = FindWindow("ConsoleWindowClass", NULL);
	if (hwnd)
		ShowWindow(hwnd, SW_SHOW);

	system("title ��һ������... && mode con cols=52 lines=17");

	int ch, i = 5, front_hour, front_min, behind_hour, behind_min;
	char config_content[350] = { "run time = " }; //���潫Ҫд�������ļ�������
	char path[MAX_PATH];				          //����timec���ذ�.exe��·��
	char temp_time_period[15], time_period[15];   //���������е�ʱ���
	char run_time[5], break_time[5];	          //���������е�ʱ�����Ϣʱ��
	char *SubKey = { "Software\\Microsoft\\Windows\\CurrentVersion\\Run" }; //ϵͳ������·��
	char *timec_path = { "C:\\Program Files\\timec\\timec���ذ�.exe" };     //ע���������·���Լ����Ƶ�Ŀ���ļ���·��
	HKEY hKey;
	FILE *fpread, *fpwrite;
	errno_t err;

	for (; i > 0; i--)
	{
		printf("\t      timec���ذ� ��һ������...\n\n\n\n\n\n\t\t    ���ڳ�ʼ��...\n\n\n\n\n");
		printf("��ܰ��ʾ�����Թ���ԱȨ�����б���������˳�ɱ�������\n\n\n");
		printf("\t\t\t%d", i);
		Sleep(1000);
		system("cls");
	}

	system("cls && mode con cols=72 lines=17");
	
	printf("��ܰ��ʾ�����������趨ʱ�䣬��༭��C:\\Users\\Timec_config.txt����");
	printf("\n\n��������������е�ʱ��Σ���ʽ��xx:xx - xx:xx���벻Ҫ�������ı�㡣\n��ܰ��ʾ������00:00 - 00:00������ʱ������С�����");
	while (1)
	{

		Initialize(temp_time_period, time_period);

		fgets(time_period, 15, stdin);
		rewind(stdin);
		Fgets_n(time_period);	//���fgets��ȡ��'\n'
		
		for (i = 0; i < 15; i++)//��time_period�����ݸ��Ƶ�temp_time_period
			temp_time_period[i] = time_period[i];
		
		//�ж�����ĸ�ʽ�Ƿ���ȷ
		if (strlen(time_period) != 13 || time_period[6] != '-')//�ж������ʽ��xx:xx-xx:xx����xx:xx - xx:xx
			if (strlen(time_period) != 11 || time_period[5] != '-')
			{
				printf("\n��ʽ�������磺08:00 - 14:00���������ԣ�");
				continue;
			}
		
		for (i = 0; i < 15; i++)
			if (time_period[i] == ':' || time_period[i] == '-')
				time_period[i] = ' ';

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

	printf("\n\n��������������е�ʱ��\n��Ҳ���ǵ�������XX���Ӻ�ǿ�ƹػ�����λ�����ӣ���");
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

	printf("\n\n�����������Ϣʱ��\n��Ҳ���ǵ���ǿ�ƹػ�����Ҫ��XX���Ӻ����ʹ�á���λ�����ӣ���");
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
	//���������ļ�
	strcat_s(config_content, 350, run_time);
	strcat_s(config_content, 350, " ;     ���������е�ʱ�䡣\nbreak time = ");
	strcat_s(config_content, 350, break_time);
	strcat_s(config_content, 350, " ;   ������Ϣ��ʱ�䡣\ntime period = ");
	strcat_s(config_content, 350, temp_time_period);
	strcat_s(config_content, 350, " ;   ���������е�ʱ��Ρ�\n\nע�⣺��run time���͡�break time������ֵ�ĵ�λ��Ϊ�֣�ֵ��Χ5~999��\n��time period����ֵ�ĸ�ʽΪ��xx:xx-xx:xx�����벻Ҫ�������ı�㣨�磺����������\n�����뱣��ԭ��ʽ��������ֳ������������У���ɾ�����ļ���");
	if ((err = fopen_s(&fpwrite, config_file_path, "w")) != 0)
	{
		remove(folder_path);
		perror("\n���������ļ�ʧ��");
		system("pause");
		exit(EXIT_FAILURE);
	}
	fputs(config_content, fpwrite);
	fclose(fpwrite);

	//�����ļ�
	system("cls && title ������������������˳�ɱ��");
	printf("\n\n\n\n\nע�⣺������������������⵽ɱ�����أ��������������������\n\n\t                ��������޷�����������\n\n\n");
	Sleep(3000);

	GetModuleFileName(NULL, path, 100); //����windows api��á�timec���ذ�.exe����·��
	
	if ((err = fopen_s(&fpread, path, "rb")) != 0)
	{
		remove(folder_path);
		perror("\n�����ļ�ʧ��");
		system("pause");
		exit(EXIT_FAILURE);
	}

	if ((err = fopen_s(&fpwrite, timec_path, "wb")) != 0)
	{
		remove(folder_path);
		perror("\n�����ļ�ʧ��");
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
	if (RegOpenKeyEx(HKEY_CURRENT_USER, SubKey, 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)
	{  //���һ����Key,������ֵ
		RegSetValueEx(hKey, "timecyincang", 0, REG_SZ, (BYTE *)timec_path, strlen(timec_path));
		RegCloseKey(hKey);//�ر�ע���

		i = MessageBox(NULL, TEXT("���óɹ�����������Ч���Ƿ�����������\nע�⣺1������ȡ��������������timec�Զ����ɾ����\n\t2������ǰ�뱣����������ϣ�"), TEXT("���óɹ���"), MB_YESNO | MB_ICONQUESTION);
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

//��ǰʱ�����ػ�ʱ�仹��3��ʱ����ػ���ʾ
void Shutdown_Tip(long sec)
{
	long second = sec - 180;
	
	if (second < 180)
		exit(EXIT_SUCCESS);
	
	second *= 1000;
	Sleep(second);
	MessageBox(NULL, TEXT("����3���Ӿ�Ҫ�ػ��ˣ�������׼����"), TEXT("����"), MB_OK | MB_ICONWARNING);
	exit(EXIT_SUCCESS);
}


//���������
void Flush(void)
{
	int ch;
	while ((ch = getchar()) != '\n' && ch != EOF)
		;
}


//���fgets��ȡ��'\n'
void Fgets_n(char *str)
{
	char *find;
	if ((find = strchr(str, '\n')))
		*find = '\0';
}


//��ʼ������
void Initialize(char *temp_time_period, char *time_period)
{
	while (*temp_time_period != '\0')
		*time_period++ = *temp_time_period++ = '\0';
}