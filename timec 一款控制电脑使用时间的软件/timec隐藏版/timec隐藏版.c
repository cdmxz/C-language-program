#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>
#include <direct.h>
#include <io.h>
#include <locale.h>

#define SIZE 100
#define MAX_SIZE 600
#define TIME_PERIOD_SIZE 15

const char APP_NAME[] = "timec";
const char RUN_TIME_NAME[] = "���Կ����������е�ʱ��";
const char BREAK_TIME_NAME[] = "���Թػ���ǿ����Ϣ��ʱ��";
const char TIME_PERIOD_NAME[] = "����ÿ�������е�ʱ���";
const char DATE_NAME[] = "����ÿ���������е�����";
const char SHUTDOWN_TIME_NAME[] = "��������Ϣʱ��������ʱ�ػ�ʱ��";
const char ESTIMATED_TIME_NAME[] = "Ԥ�ƹػ�ʱ��";
const char REG_PARAMETER[] = "reg";

// �жϵ����ڵ�ǰʱ���Ƿ�������
int  can_start(void);          
// ��ȡ���Թػ�ʱ��
long long get_shutdown_time(char* shutdown_time, size_t size, long long add_sec);
// ��str1����ָ���ַ��ָ�����ָ���ÿ���ַ�����str2�Ƚ��Ƿ���ͬ
int find_str(char* str1, const char ch, char* str2);
// ��������
int input_number(char* dest_str, size_t dest_str_size, int max_value);
// �ػ�
void shut_down(time_t sec, char* tip);
// ��ȡch��str���±�
int get_index(char* str, char ch);
// ɾ��str�е�ָ���ַ�
void del_char(char* str, char ch);
// ������ػ�ʱ�仹��3����ʱ����ػ���ʾ
void shutdown_tip(const long sec);
// ��һ������ʱ���ò���
void first_start(char* cmd);	
// ���������
void Flush(void);		  
// ���fgets��ȡ��'\n'
void fgets_n(char* str);   
// ���ش���
void hide_window(int mode);
// ��ʼ���������ļ��ȣ�·��
void init_path(char* cmd, size_t size);
// �ָ��ַ������ַ���ֻ����һ���ָ����1,2��
void split_str(char* str, const char ch, char* out_str1, size_t out_str1_size, char* out_str2, size_t out_str2_size);
// �жϰ�ch�ָ���ַ�����1,2,3����ÿ�������Ƿ���ȷ������0С��32��
int judge_date(const char* date_str, size_t date_str_size, char sep);
// ���������
void add_start(void);
// �������ļ�
void read_file(int* shutdown_sec, int* run_time, int* break_time, char* time_period, size_t time_period_size, char* date, size_t date_size, time_t* estimated_time);
// д�����ļ�
void write_file(char* cmd, char* run_time, char* break_time, char* time_period, char* date, char* shutdown_time);
// д��Ԥ�ƹػ�ʱ�䣨��ǰʱ��+�����е�ʱ�䣩
void write_estimated_time(time_t estimated_time);
// ���ֽ��ַ�ת���ֽ��ַ�
wchar_t* str_to_wcs(char* source_str);
/***************************************************************************************************************
*    ��ǰʱ��Ϊ2019��11��23��11:20�����趨��������40���Ӿ�Ҫ��Ϣ10���Ӻ�������У���������ֻ����ÿ���10:00-   *
*    13:00���С�                                                                                               *
*    Ҳ����˵Ҫ�ڵ�ǰʱ��Ļ����ϼ���50���ӣ�40���Ӻ�ػ�+10������Ϣ����������´򿪵��ԡ�				 	   *
*    ��Ϊ��������40���Ӻ�ػ������Ե��Թػ���ʱ����2019��11��23��12:00��						               *
*    �ټ���10������Ϣʱ�䣬������Ҫ��2019��11��23��12:10������ٴδ򿪵��ԣ��������ʾ�ػ���				   *
*    �������10:00-13:00���ʱ�����򿪵��ԣ�Ҳ����ʾ�ػ���												   *
****************************************************************************************************************/


long long lastSleepTime = 0;
char config_file_path[MAX_PATH];//�����ļ�·��
char folder_path[MAX_PATH];     //�ļ���·��
char dest_path[MAX_PATH];      //ע���������·���Լ����Ƶ�Ŀ���ļ���·��
char start_parameter[MAX_PATH];// ��������

int main(int argc, char* argv[])
{
	int ret_val;
	char cmd[MAX_PATH];
	init_path(cmd, sizeof(cmd));
	// �������д���
	hide_window(HIDE_WINDOW);

	if (argc == 2)
	{
		strcpy_s(start_parameter, sizeof(start_parameter), argv[1]);
		if (strcmp(argv[1], "-reset") == 0)//�������������ļ�
			first_start(cmd);
	}
	else if (_access(config_file_path, 0)) // �ж������ļ��Ƿ����                                   
		first_start(cmd);                // ����������õ�һ����������

	ret_val = can_start();
	if (ret_val == 0)
		shut_down(-1, NULL);
	return 0;
}

// ���س�������д���
void hide_window(int mode)
{
	HWND hwnd = FindWindow(TEXT("ConsoleWindowClass"), NULL);
	if (hwnd)
		ShowWindow(hwnd, mode);
}

// ��ʼ���������ļ��ȣ�·��
void init_path(char* cmd, size_t size)
{
	char system_data[MAX_PATH]; //����ϵͳ���̷�
	if (GetEnvironmentVariableA("ALLUSERSPROFILE", system_data, MAX_PATH) == 0) //��ȡ"C:\ProgramData"·��
		strcpy_s(system_data, MAX_PATH, "C:\\ProgramData");                             //��ȡʧ����Ĭ��ΪC:
	sprintf_s(folder_path, MAX_PATH, "%s\\timec", system_data); //�ļ���·��
	sprintf_s(config_file_path, MAX_PATH, "%s\\Timec_config.txt", folder_path);//�����ļ�·��
	sprintf_s(dest_path, MAX_PATH, "%s\\timec���ذ�.exe", folder_path);       //ע���������·���Լ����Ƶ�Ŀ���ļ���·��
	sprintf_s(cmd, size, "attrib +s +h \"%s\" ", folder_path);            //��Ŀ¼���ϵͳ��������
}


/*****************************************************************************
					�ж��ڵ�ǰʱ���ڵ����Ƿ�������
*****************************************************************************/
int can_start(void)
{
	int  shutdown_sec, break_time, run_time;
	char temp_time[SIZE] = { 0 }, front_time[TIME_PERIOD_SIZE], back_time[TIME_PERIOD_SIZE] = { 0 };
	char date[SIZE] = { 0 };	     //������������е�����
	char time_period[TIME_PERIOD_SIZE] = { 0 };//������������е�ʱ���
	char tip[SIZE] = { 0 };     //��������ǰ����ʱ����ػ���ʾ
	time_t current_time, shutdown_time, estimated_time;

	//��ȡ1970-01-01����ǰ������
	struct tm  tm_current_time;
	time(&current_time);
	localtime_s(&tm_current_time, &current_time);//����ȡ������ת��Ϊ����ʱ��

	read_file(&shutdown_sec, &run_time, &break_time, time_period, sizeof(time_period), date, sizeof(date), &estimated_time);//��ȡ���������е�ʱ��κ�����

	//�жϵ�ǰ�����Ƿ�������
	if (strcmp(date, "00") != 0)
	{
		sprintf_s(tip, sizeof(tip), "����ÿ���µġ�%s�������б����ԣ�\n�����Լ�����%d���ڹػ���", date, shutdown_sec);
		sprintf_s(temp_time, sizeof(temp_time), "%d", tm_current_time.tm_mday);
		if (find_str(date, ',', temp_time) == -1)// ����������в����ҵ���ǰ���ڣ���ػ�
			shut_down(shutdown_sec, tip);
	}

	//�жϵ�ǰʱ���Ƿ���ʱ�����
	if (strcmp(time_period, "00:00-00:00") != 0)//�ж��Ƿ���ʱ�������
	{
		sprintf_s(tip, SIZE, "����ÿ��ġ�%s�������б����ԣ�\n�����Լ�����%d���ڹػ���", time_period, shutdown_sec);
		// �����-���͡�:��
		//del_char(time_period, '-');
		del_char(time_period, ':');

		//index = get_index(time_period, ':');
		// �ָ��ǰʱ�̺ͺ�ʱ�̣��ָ�ǰ��time_period[TIME_PERIOD_SIZE]={"08:00-16:00"}���ָ��ǰʱ�̣�behind_time[TIME_PERIOD_SIZE]={"08:00"}����ʱ�̣�time_period[TIME_PERIOD_SIZE]={"16:00"}��
		split_str(time_period, '-', front_time, sizeof(front_time), back_time, sizeof(back_time));
		/*for (i = 0; i < strlen(time_period); i++, index++)
		{
			behind_time[i] = time_period[index];
			if (index == (strlen(time_period) - 1))
				time_period[index] = '\0';
		}*/

		sprintf_s(temp_time, TIME_PERIOD_SIZE, "%d%d", tm_current_time.tm_hour, tm_current_time.tm_min);//����ǰʱ��д������ 
		if (strcmp(temp_time, front_time) < 0 || strcmp(temp_time, back_time) > 0)     //�жϵ�ǰʱ���Ƿ��������е�ʱ�����
			shut_down(shutdown_sec, tip);
		memset(temp_time, 0, sizeof(temp_time));//�������
	}
	// ��ȡ�ϴιػ�ʱ��
	shutdown_time = get_shutdown_time(NULL, 0, 0);

	//�жϵ�ǰʱ���Ƿ�С��Ԥ�ƹػ�ʱ��
	if (current_time < estimated_time)
	{
		// ������������������ֶ����������
		if (strcmp(start_parameter, REG_PARAMETER) == 0)
		{
			// �����Ϊ���ɿ������عػ������������˵����ֶ��ػ���Ȼ��һ��ʱ����ֿ����Ļ���
			// ���жϵ�ǰʱ�� - �ػ�ʱ�� < ��Ϣʱ�䡣
			// ��������������Ͳ��Ϲػ����Ƕ�ʱ�䡣
			// ���磺���趨��Ϣʱ��break_timeΪ7���ӣ�Ԥ��12:00�ػ�������11:40�����˹ػ���Ȼ��11:45�ֿ��������棬
			// ��ô�м���Ϣ��5���ӣ�δ�����趨����Ϣʱ��break_time�����������ʵ�ʹػ�ʱ���ҪΪ12:05�֡�
			if (current_time - shutdown_time < (time_t)break_time * 60)
			{
				// ���Ϲػ����Ƕ�ʱ�䣬�������趨�ػ�
				write_estimated_time(estimated_time + (current_time - shutdown_time));// Ԥ�ƹػ�ʱ�� +����ǰʱ��-ʵ�ʹػ�ʱ�䣩
				shut_down(estimated_time + (current_time - shutdown_time) - current_time, NULL);// Ԥ�ƹػ�ʱ�� + ����ǰʱ�� - ʵ�ʹػ�ʱ�䣩- ʵ�ʹػ�ʱ��
			}
			else
				// ���磺���趨��Ϣʱ��break_timeΪ7���ӣ�Ԥ��12:00�ػ�������11:40�����˹ػ���Ȼ��11:50�ֿ��������棬
				// ��ô�м���Ϣ��10���ӣ��������趨����Ϣʱ��break_time������ô�趨run_time���Ӻ�ػ���
				return 0;
		}
		else
			shut_down(estimated_time - current_time, NULL);// Ԥ�ƹػ�ʱ�� + ����ǰʱ�� - ʵ�ʹػ�ʱ�䣩- ʵ�ʹػ�ʱ��
		//write_estimated_time(current_time + (estimated_time - shutdown_time));
		//shut_down(estimated_time - current_time, NULL);

		////�жϵ�ǰʱ��������ʱ���Ƿ����С��5����
		//if ((current_time - lastSleepTime) <= 300 && lastSleepTime != 0)
		//{	//�����ǰʱ��������ʱ�����С��5���Ӿ������ǰʱ������´ιػ�ʱ�仹�ж��ٷ�
		//	system("shutdown -a");
		//	sprintf_s(command, SIZE, "shutdown -f -s -t %lld", (ShutDownTime - current_time));//��ʱ���д������
		//	system(command); //ִ�йػ�����
		//	shutdown_tip((long)(ShutDownTime - current_time));
		//}
	}
	// ��ǰʱ�� ���� Ԥ�ƹػ�ʱ�� ����С�� Ԥ�ƹػ�ʱ�� + ��Ϣʱ��
	if (current_time >= estimated_time && current_time < (estimated_time + (time_t)break_time * 60))
	{
		shut_down(shutdown_sec, "��ǰΪ��Ϣʱ�䣬���Լ����ػ���");
	}
	write_estimated_time(current_time + (time_t)run_time * 60);
	return 0;
}

// �ָ��ַ������ַ���ֻ����һ���ָ����1,2��
void split_str(char* str, const char ch, char* out_str1, size_t out_str1_size, char* out_str2, size_t out_str2_size)
{
	if (!str || !ch || !out_str1 || !out_str2)
		return;

	memset(out_str1, 0, out_str1_size);
	memset(out_str2, 0, out_str2_size);

	char* p = str;
	for (; *p != ch; p++, out_str1++)
		*out_str1 = *p;
	str = p = strchr(str, ch) + 1;
	for (; *p != ch && *p != '\0'; p++, out_str2++)
		*out_str2 = *p;
}

// д��Ԥ�ƹػ�ʱ�䣨��ǰʱ��+�����е�ʱ�䣩
void write_estimated_time(time_t estimated_time)
{
	char time[SIZE];
	sprintf_s(time, sizeof(time), "%lld", estimated_time);
	WritePrivateProfileStringA(APP_NAME, ESTIMATED_TIME_NAME, time, config_file_path);
}

// ��str1����ָ���ַ��ָ�����ָ���ÿ���ַ�����str2�Ƚ��Ƿ���ͬ
int find_str(char* str1, const char ch, char* str2)
{
	char* p = NULL;
	char* con_text = NULL;
	// �ָ��ַ���
	p = (char*)strtok_s(str1, &ch, &con_text);
	while (p != NULL)
	{
		if (strcmp(p, str2) == 0)
			return 0;
		p = (char*)strtok_s(NULL, &ch, &con_text);
	}
	return -1;
}

// ��ȡ���Թػ�ʱ��
long long get_shutdown_time(char* shutdown_time, size_t size, long long add_sec)
{
	const wchar_t shutdown_key[] = { L"SYSTEM\\CurrentControlSet\\Control\\Windows" };//ϵͳ������·��
	HKEY hkey;
	FILETIME file_time;
	DWORD dwsize;
	SYSTEMTIME system_time;
	ULARGE_INTEGER ui;
	long long time = 0;

	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, shutdown_key, 0, KEY_READ, &hkey) != ERROR_SUCCESS)//��ȡ�����ϴιػ�ʱ��
		return -1;

	if (RegQueryValueEx(hkey, TEXT("ShutdownTime"), NULL, NULL, NULL, &dwsize) != ERROR_SUCCESS || sizeof(file_time) != dwsize)
		return -1;

	RegQueryValueEx(hkey, TEXT("ShutdownTime"), NULL, NULL, (LPBYTE)&file_time, &dwsize);

	FileTimeToSystemTime(&file_time, &system_time);
	ui.LowPart = file_time.dwLowDateTime;
	ui.HighPart = file_time.dwHighDateTime;
	time = (LONGLONG)(ui.QuadPart - 116444736000000000) / 10000000;
	RegCloseKey(hkey);

	time += (add_sec > 0 ? add_sec : 0);
	if (shutdown_time)
		sprintf_s(shutdown_time, size, "%lld", time);
	return time;
}

// ɾ��str�е�ָ���ַ�
void del_char(char* str, char ch)
{
	int i = 0, j;
	int len = (int)strlen(str);
	for (; i <= len; i++)
	{
		if (str[i] == ch)
		{
			for (j = i; j < len - 1; j++)
				str[j] = str[j + 1];
			str[j] = '\0';
		}
	}
}

// ��ȡch��str���±�
int get_index(char* str, char ch)
{
	char* p = str;
	for (; *p != '\0'; p++)
	{
		if (*p == ch)
			return (int)(p - str);
	}
	return -1;
}

/*****************************************************************************
				 �������ļ���ȡ���������е�ʱ���Լ���Ϣʱ��
*****************************************************************************/
void read_file(int* shutdown_sec, int* run_time, int* break_time, char* time_period, size_t time_period_size, char* date, size_t date_size, time_t* estimated_time)
{
	char temp[SIZE];
	/*strcat_s(content, MAX_SIZE, "\n\nע�⣺\n"
		"1�������Կ����������е�ʱ�䡱�͡����Թػ���ǿ����Ϣ��ʱ�䡱����ֵ�ĵ�λ��Ϊ�֣�ֵ��Χ5~999��\n"
		"2��������ÿ�������е�ʱ��Ρ���ֵ�ĸ�ʽΪ��xx:xx-xx:xx�����벻Ҫ�������ı�㣨�磺����������\n"
		"3��ÿһ��ĩβ�ķֺ���Ӣ�ķֺţ����������뱣��ԭ��ʽ��\n"
		"4��������ֳ������������У����ڡ�timec�Զ���桱���´��������ļ�����ӡ�-reset����������timec���ذ档");*/

	if (shutdown_sec)
	{// ��������Ϣʱ��������ʱ�ػ�ʱ��
		GetPrivateProfileStringA(APP_NAME, SHUTDOWN_TIME_NAME, "30", temp, sizeof(temp), config_file_path);
		sscanf_s(temp, "%d", shutdown_sec);
	}
	if (run_time)
	{// ���������е�ʱ��
		GetPrivateProfileStringA(APP_NAME, RUN_TIME_NAME, "40", temp, sizeof(temp), config_file_path);
		sscanf_s(temp, "%d", run_time);
	}
	if (break_time)
	{// ���Թػ���ǿ����Ϣ��ʱ��
		GetPrivateProfileStringA(APP_NAME, BREAK_TIME_NAME, "10", temp, sizeof(temp), config_file_path);
		sscanf_s(temp, "%d", break_time);
	}
	if (time_period)// ����ÿ�������е�ʱ���
		GetPrivateProfileStringA(APP_NAME, TIME_PERIOD_NAME, "00:00-00:00", time_period, (DWORD)time_period_size, config_file_path);
	if (date)// ����ÿ���������е�����
		GetPrivateProfileStringA(APP_NAME, DATE_NAME, "00", date, (DWORD)date_size, config_file_path);
	if (estimated_time)// Ԥ�ƹػ�ʱ�䣨��λ���룩
	{
		GetPrivateProfileStringA(APP_NAME, ESTIMATED_TIME_NAME, "-1", temp, sizeof(temp), config_file_path);
		sscanf_s(temp, "%lld", estimated_time);
	}
}


/*****************************************************************************
							   ��һ����������
******************************************************************************/
void first_start(char* cmd)
{
	//��ʾ�������д���
	hide_window(SW_SHOW);
	system("title ��һ������... && mode con cols=52 lines=17");
	char c;
	unsigned i, j = 0;
	int  front_hour, front_min, back_hour, back_min;
	char  time_period_backup[TIME_PERIOD_SIZE], time_period[TIME_PERIOD_SIZE]; //���������е�ʱ���
	char run_time[TIME_PERIOD_SIZE] = { 0 }, break_time[TIME_PERIOD_SIZE] = { 0 }, shutdown_sec[TIME_PERIOD_SIZE];	        //���������е�ʱ�����Ϣʱ��
	char date[SIZE] = { 0 };						//�������ÿ���������е�����


	/*for (i = 5; i > 0; i--)
	{
		printf("\t      timec���ذ� ��һ������...\n\n\n\n\n\n\t\t    ���ڳ�ʼ��...\n\n\n\n\n");
		printf("��ܰ��ʾ�����Թ���ԱȨ�����б���������˳�ɱ�������\n\n\n");
		printf("\t\t\t%d", i);
		Sleep(1000);
		system("cls");
	}*/

	/***********************/
begin://goto����ǩ
/***********************/

	system("cls && mode con cols=72 lines=25");

	printf("��ܰ��ʾ�����������趨���������ڡ�timec�Զ���桱����9������5�޸ġ�");
	printf("\n\n���������ÿ�������е�ʱ��Σ���ʽ��xx:xx - xx:xx���벻Ҫ�������ı�㡣\n��ܰ��ʾ������00:00 - 00:00����������ʱ��Ρ�����");
	while (1)
	{
		memset(time_period_backup, 0, sizeof(time_period_backup));
		memset(time_period, 0, sizeof(time_period));

		fgets(time_period, TIME_PERIOD_SIZE, stdin);
		rewind(stdin);
		fgets_n(time_period);	//���fgets��ȡ��'\n'

		del_char(time_period, ' ');
		//��time_period�����ݸ��Ƶ�time_period_backup
		strcpy_s(time_period_backup, sizeof(time_period_backup), time_period);

		//�ж�����ĸ�ʽ�Ƿ���ȷ
		if (strlen(time_period) != 11 || time_period[5] != '-')
		{
			printf("\n��ʽ�������磺08:00 - 14:00���������ԣ�");
			continue;
		}

		for (i = 0; i < TIME_PERIOD_SIZE; i++)
		{
			if (time_period[i] == ':' || time_period[i] == '-')
				time_period[i] = ' ';
		}

		sscanf_s(time_period, "%d %d %d %d", &front_hour, &front_min, &back_hour, &back_min);

		//�ж������ʱ���Ƿ�����
		if ((front_hour > 24 || front_hour < 0) || (back_hour > 24 || back_hour < 0) || (front_min > 60 || front_min < 0) || (back_min > 60 || back_min < 0))
		{
			printf("\n��ʽ�������磺08:00 - 14:00���������ԣ�");
			continue;
		}
		if (front_hour == 24 || back_hour == 24)//�ж�����ʱ��Сʱ�����Ƿ�Ϊ24
		{
			if (front_hour == 24 && back_hour == 24)//�ж�����ʱ��Сʱ�����Ƿ�Ϊ24
			{
				time_period_backup[1] = time_period_backup[0] = '0';
				if (time_period[8] == '2' && time_period[9] == '4')
					time_period_backup[9] = time_period_backup[8] = '0';
				else
					time_period_backup[7] = time_period_backup[6] = '0';
				printf("\n��ʽ����Ӧ����Ϊ��%s�������ԣ�", time_period_backup);
			}
			else if (front_hour == 24)//�ж�ǰʱ��Сʱ�����Ƿ�Ϊ24
			{
				time_period_backup[1] = time_period_backup[0] = '0';
				printf("\n��ʽ����Ӧ����Ϊ��%s�������ԣ�", time_period_backup);
			}
			else if (back_hour == 24)//�жϺ�ʱ��Сʱ�����Ƿ�Ϊ24
			{
				if (time_period[8] == '2' && time_period[9] == '4')
					time_period_backup[9] = time_period_backup[8] = '0';
				else
					time_period_backup[7] = time_period_backup[6] = '0';
				printf("\n��ʽ���󣬺�ʱ��Ӧ����Ϊ��%s�������ԣ�", time_period_backup);
			}
			continue;
		}

		//�ж������Ƿ�Ϊ00:00 - 00:00
		if (front_hour == 0 && back_hour == 0 && front_min == 0 && back_min == 0)
			break;

		//�жϺ�ʱ���Ƿ��ǰʱ�̴�
		if (front_hour > back_hour)
		{
			printf("\nǰʱ��Ҫ�Ⱥ�ʱ��С�������ԣ�");
			continue;
		}
		else if (front_hour == back_hour)
		{
			if (front_min > back_min)
			{
				printf("\nǰʱ��Ҫ�Ⱥ�ʱ��С�������ԣ�");
				continue;
			}
			else if (front_min == back_min)
			{
				printf("\nǰʱ��Ҫ�Ⱥ�ʱ��С���Ҳ�����ȡ������ԣ�");
				continue;
			}
		}

		//�ж�ǰʱ�����ʱ���Ƿ����С��5����
		front_hour = (front_hour * 3600) + (front_min * 60);
		back_hour = (back_hour * 3600) + (back_min * 60);
		if ((back_hour - front_hour) < 300)
		{
			printf("\nǰʱ�����ʱ�̱������5���ӡ������ԣ�");
			continue;
		}
		break;
	}


	printf("\n\n���������ÿ���������е����ڣ����磺05��\n��ܰ��ʾ�����Ҫ��������������Ӣ�Ķ��ŷָ��������磺1,5,10,25,30��\n����00�������������ڡ�����");
	while (1)
	{
		memset(date, 0, sizeof(date));
		fgets(date, sizeof(date), stdin);
		rewind(stdin);
		fgets_n(date);

		if (strlen(date) <= 1 || judge_date(date, sizeof(date), ','))
		{
			printf("\n��ʽ�������������루���磺01,05,10����");
			continue;
		}
		break;
	}

	printf("\n\n���������ÿ�������е�ʱ��\n��Ҳ���ǵ���ÿ������XX���Ӻ�ǿ�ƹػ�����λ�����ӣ���");
	input_number(run_time, sizeof(run_time), 999);

	printf("\n\n���������ÿ����Ϣʱ��\n��Ҳ���ǵ���ÿ��ǿ�ƹػ�����Ҫ��XX���Ӻ����ʹ�á���λ�����ӣ���");
	input_number(break_time, sizeof(break_time), 999);

	printf("\n\n�����뵱�����������е�ʱ����������Ϣʱ��������ʱ�����Թػ�ʱ��\n����λ���롣���ֵ60�룩��");
	input_number(shutdown_sec, sizeof(break_time), 60);

	system("cls && mode con cols=47 lines=15");
	printf("��ǰ���õĲ���Ϊ��\n\n");

	if (strcmp(time_period_backup, "00:00-00:00") == 0)
		printf("����ÿ�������е�ʱ��Σ� %s������������ʱ��Σ�\n", time_period_backup);

	else if (strcmp(time_period_backup, "00:00 - 00:00") == 0)
		printf("����ÿ�������е�ʱ��Σ� %s������������ʱ��Σ�\n", time_period_backup);

	else
		printf("����ÿ�������е�ʱ��Σ� %s", time_period_backup);

	if (strcmp(date, "00") == 0)
		printf("\n����ÿ���������е����ڣ� %s���������������ڣ�", date);
	else
		printf("\n����ÿ���������е����ڣ� %s ��", date);

	printf("\n����ÿ�������е�ʱ�䣺   %s ��", run_time);
	printf("\n����ÿ����Ϣ��ʱ�䣺     %s ��", break_time);
	printf("\n����Ϣʱ������ʱ�ػ�ʱ�䣺%s ��", shutdown_sec);

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
			memset(run_time, 0, sizeof(run_time));
			memset(break_time, 0, sizeof(break_time));
			goto begin;
		}
		else
			continue;
	}

	write_file(cmd, run_time, break_time, time_period_backup, date, shutdown_sec);

	system("cls && title ������������������˳�ɱ�� &&mode con cols=72 lines=25");
	printf("\n\n\n\n\nע�⣺������������������⵽ɱ�����أ��������������������\n\n\t                ��������޷�����������\n\n\n");
	Sleep(3000);
	add_start();
	exit(EXIT_SUCCESS);
}

// д�����ļ�
void write_file(char* cmd, char* run_time, char* break_time, char* time_period, char* date, char* shutdown_time)
{
	FILE* fp = NULL;
	char content[MAX_SIZE] = { 0 };		//���潫Ҫд�������ļ�������         
	//����Ŀ¼
	if (_access(folder_path, 0) == -1)//�ж�Ŀ¼�Ƿ����
	{
		if (_mkdir(folder_path) != 0)
		{
			MessageBox(NULL, TEXT("����Ŀ¼ʧ�ܣ����Թ���ԱȨ�����б������"), TEXT("ERROR"), MB_YESNO | MB_ICONERROR);
			exit(EXIT_FAILURE);
		}
	}
	system(cmd);//ִ��cmd��������Ŀ¼

	WritePrivateProfileStringA(APP_NAME, RUN_TIME_NAME, run_time, config_file_path);
	WritePrivateProfileStringA(APP_NAME, BREAK_TIME_NAME, break_time, config_file_path);
	WritePrivateProfileStringA(APP_NAME, TIME_PERIOD_NAME, time_period, config_file_path);
	WritePrivateProfileStringA(APP_NAME, DATE_NAME, date, config_file_path);
	WritePrivateProfileStringA(APP_NAME, SHUTDOWN_TIME_NAME, shutdown_time, config_file_path);

	//���������ļ�
	if (fopen_s(&fp, config_file_path, "a+") != 0)
	{
		perror("\n���������ļ�ʧ��");
		system("pause");
		exit(EXIT_FAILURE);
	}
	strcat_s(content, MAX_SIZE, "\n\nע�⣺\n"
		"1�������Կ����������е�ʱ�䡱�͡����Թػ���ǿ����Ϣ��ʱ�䡱����ֵ�ĵ�λ��Ϊ�֣�ֵ��Χ5~999��\n"
		"2��������ÿ�������е�ʱ��Ρ���ֵ�ĸ�ʽΪ��xx:xx-xx:xx�����벻Ҫ�������ı�㣨�磺����������\n"
		"3��ÿһ��ĩβ�ķֺ���Ӣ�ķֺţ����������뱣��ԭ��ʽ��\n"
		"4��������ֳ������������У����ڡ�timec�Զ���桱���´��������ļ�����ӡ�-reset����������timec���ذ档");
	if (fp)
	{
		fputs(content, fp);//�������ļ��������
		fclose(fp);
	}
}

// ���������
void add_start(void)
{
	char* key = { "Software\\Microsoft\\Windows\\CurrentVersion\\Run" }; //ϵͳ������·��
	HKEY hkey;
	int ret_val;
	char path[MAX_PATH];  //����timec���ذ�.exe��·��    

	//�����ļ�
	GetModuleFileNameA(NULL, path, MAX_PATH); //����windows api��á�timec���ذ�.exe����·��
	if (!CopyFileA(path, dest_path, FALSE))
	{
		MessageBox(NULL, TEXT("�����ļ�ʧ�ܣ����Թ���ԱȨ�����ԣ�"), TEXT("����"), MB_OK | MB_ICONERROR);
		exit(EXIT_FAILURE);
	}

	//��ע������������
	if (RegOpenKeyExA(HKEY_CURRENT_USER, key, 0, KEY_ALL_ACCESS, &hkey) != ERROR_SUCCESS)
	{
		MessageBox(NULL, TEXT("��ӵ�ע���ʧ�ܣ�"), TEXT("ʧ��"), MB_OK | MB_ICONERROR);
		exit(EXIT_FAILURE);
	}
	//���һ����Key,������ֵ
	sprintf_s(path, sizeof(path), "\"%s\" %s", dest_path, REG_PARAMETER);
	RegSetValueExA(hkey, "timec���ذ�", 0, REG_SZ, (BYTE*)path, MAX_PATH);
	RegCloseKey(hkey);//�ر�ע���

	ret_val = MessageBox(NULL, TEXT("����������ɹ�����������Ч���Ƿ�����������\nע�⣺1������ȡ��������������timec�Զ����ɾ����\n            2������ǰ�뱣����������ϣ�"), TEXT("�ɹ�"), MB_YESNO | MB_ICONQUESTION);
	if (ret_val == IDYES)
		system("shutdown -a && shutdown -r -t 00");
}

// �жϰ�ch�ָ���ַ�����1,2,3����ÿ�������Ƿ���ȷ������0С��32��
int judge_date(const char* date_str, size_t date_str_size, char sep)
{
	int date;
	char* p = NULL, * con_text = NULL;
	char* backup = calloc(date_str_size, sizeof(char));
	if (!backup)
		return 0;

	strcpy_s(backup, date_str_size, date_str);
	// �ָ��ַ���
	p = (char*)strtok_s(backup, &sep, &con_text);
	while (p != NULL)
	{
		sscanf_s(p, "%d", &date);
		if (date < 1 || date>31)
		{
			free(backup);
			return -1;
		}
		p = (char*)strtok_s(NULL, &sep, &con_text);
	}
	free(backup);
	return 0;
}

// ��������
int input_number(char* dest_str, size_t dest_str_size, int max_value)
{
	int num;
	while (1)
	{
		while (scanf_s("%d", &num) != 1)
		{
			printf("\n������������ԣ�");
			Flush();
		}
		Flush();
		if (num > max_value)
		{
			printf("\n���ֵ���ܳ���%d�����������룺", max_value);
			continue;
		}
		break;
	}
	if (dest_str && dest_str_size > 0)
		sprintf_s(dest_str, dest_str_size, "%d", num);
	return num;
}

/*****************************************************************************
							 ִ�йػ�����
*****************************************************************************/
void shut_down(time_t sec, char* tip)
{
	int  run_time;
	char command[SIZE];
	TCHAR* buf = NULL;

	if (sec == -1)
	{
		//�������ļ���ȡ���������е�ʱ��
		read_file(NULL, &run_time, NULL, NULL, 0, NULL, 0, NULL);
		run_time *= 60;//�ѷ�ת��Ϊ��
	}
	else
		run_time = (int)sec;
	system("shutdown -a");
	sprintf_s(command, sizeof(command), "shutdown -f -s -t %d", run_time);
	system(command);
	if (!tip)
		shutdown_tip(run_time);
	else
	{
		// �����ֽ��ַ�תΪ���ֽ��ַ�
		buf = str_to_wcs(tip);
		MessageBox(NULL, buf, TEXT("����"), MB_OK | MB_ICONWARNING);
		exit(EXIT_SUCCESS);
	}
}

// ���ֽ��ַ�ת���ֽ��ַ�
wchar_t* str_to_wcs(char* source_str)
{
	// �������Ҫ��Ŀ�껺�����Ĵ�С
	int size = MultiByteToWideChar(CP_ACP, 0, source_str, -1, NULL, 0);
	wchar_t* dest_str = calloc(size, sizeof(TCHAR));
	MultiByteToWideChar(CP_ACP, 0, source_str, -1, dest_str, size);
	return dest_str;
}



/*****************************************************************************
				��ǰʱ�����ػ�ʱ�仹��3��ʱ����ػ���ʾ
*****************************************************************************/
void shutdown_tip(const long sec)
{
	long second = sec - 180;
	char tip[SIZE] = { 0 };

	if (second <= 180)
	{
		sprintf_s(tip, sizeof(tip), "shutdown -a && shutdown -f -s -t %d -c ���� %d�� ��Ҫ�ػ��ˣ�������׼����", second, second / 60);
		system(tip);
	}
	else
	{
		Sleep(second * 1000);
		system("shutdown -a && shutdown -f -s -t 180 -c ���� 3�� ��Ҫ�ػ��ˣ�������׼����");
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
void fgets_n(char* str)
{
	char* find;
	if ((find = strchr(str, '\n')))
		*find = '\0';
}