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
const char RUN_TIME_NAME[] = "电脑开机后能运行的时间";
const char BREAK_TIME_NAME[] = "电脑关机后强制休息的时间";
const char TIME_PERIOD_NAME[] = "电脑每天能运行的时间段";
const char DATE_NAME[] = "电脑每个月能运行的日期";
const char SHUTDOWN_TIME_NAME[] = "电脑在休息时间内启动时关机时间";
const char ESTIMATED_TIME_NAME[] = "预计关机时间";
const char REG_PARAMETER[] = "reg";

// 判断电脑在当前时刻是否能启动
int  can_start(void);          
// 获取电脑关机时间
long long get_shutdown_time(char* shutdown_time, size_t size, long long add_sec);
// 将str1按照指定字符分割，并将分割后的每个字符串和str2比较是否相同
int find_str(char* str1, const char ch, char* str2);
// 输入数字
int input_number(char* dest_str, size_t dest_str_size, int max_value);
// 关机
void shut_down(time_t sec, char* tip);
// 获取ch在str的下标
int get_index(char* str, char ch);
// 删除str中的指定字符
void del_char(char* str, char ch);
// 当距离关机时间还有3分钟时输出关机提示
void shutdown_tip(const long sec);
// 第一次启动时设置参数
void first_start(char* cmd);	
// 清除缓冲区
void Flush(void);		  
// 清除fgets读取的'\n'
void fgets_n(char* str);   
// 隐藏窗口
void hide_window(int mode);
// 初始化（配置文件等）路径
void init_path(char* cmd, size_t size);
// 分割字符串（字符串只能有一个分割符：1,2）
void split_str(char* str, const char ch, char* out_str1, size_t out_str1_size, char* out_str2, size_t out_str2_size);
// 判断按ch分割的字符串（1,2,3）的每个日期是否正确（大于0小于32）
int judge_date(const char* date_str, size_t date_str_size, char sep);
// 添加自启动
void add_start(void);
// 读配置文件
void read_file(int* shutdown_sec, int* run_time, int* break_time, char* time_period, size_t time_period_size, char* date, size_t date_size, time_t* estimated_time);
// 写配置文件
void write_file(char* cmd, char* run_time, char* break_time, char* time_period, char* date, char* shutdown_time);
// 写入预计关机时间（当前时间+能运行的时间）
void write_estimated_time(time_t estimated_time);
// 多字节字符转宽字节字符
wchar_t* str_to_wcs(char* source_str);
/***************************************************************************************************************
*    当前时间为2019年11月23日11:20，我设定电脑运行40分钟就要休息10分钟后才能运行，并且设置只能在每天的10:00-   *
*    13:00运行。                                                                                               *
*    也就是说要在当前时间的基础上加上50分钟（40分钟后关机+10分钟休息）后才能重新打开电脑。				 	   *
*    因为电脑运行40分钟后关机，所以电脑关机的时间是2019年11月23日12:00。						               *
*    再加上10分钟休息时间，所以我要在2019年11月23日12:10后才能再次打开电脑，否则就提示关机。				   *
*    如果我在10:00-13:00这个时间段外打开电脑，也会提示关机。												   *
****************************************************************************************************************/


long long lastSleepTime = 0;
char config_file_path[MAX_PATH];//配置文件路径
char folder_path[MAX_PATH];     //文件夹路径
char dest_path[MAX_PATH];      //注册表自启动路径以及复制的目标文件的路径
char start_parameter[MAX_PATH];// 启动参数

int main(int argc, char* argv[])
{
	int ret_val;
	char cmd[MAX_PATH];
	init_path(cmd, sizeof(cmd));
	// 隐藏运行窗口
	hide_window(HIDE_WINDOW);

	if (argc == 2)
	{
		strcpy_s(start_parameter, sizeof(start_parameter), argv[1]);
		if (strcmp(argv[1], "-reset") == 0)//重新设置配置文件
			first_start(cmd);
	}
	else if (_access(config_file_path, 0)) // 判断配置文件是否存在                                   
		first_start(cmd);                // 不存在则调用第一次启动设置

	ret_val = can_start();
	if (ret_val == 0)
		shut_down(-1, NULL);
	return 0;
}

// 隐藏程序的运行窗口
void hide_window(int mode)
{
	HWND hwnd = FindWindow(TEXT("ConsoleWindowClass"), NULL);
	if (hwnd)
		ShowWindow(hwnd, mode);
}

// 初始化（配置文件等）路径
void init_path(char* cmd, size_t size)
{
	char system_data[MAX_PATH]; //储存系统盘盘符
	if (GetEnvironmentVariableA("ALLUSERSPROFILE", system_data, MAX_PATH) == 0) //获取"C:\ProgramData"路径
		strcpy_s(system_data, MAX_PATH, "C:\\ProgramData");                             //获取失败则默认为C:
	sprintf_s(folder_path, MAX_PATH, "%s\\timec", system_data); //文件夹路径
	sprintf_s(config_file_path, MAX_PATH, "%s\\Timec_config.txt", folder_path);//配置文件路径
	sprintf_s(dest_path, MAX_PATH, "%s\\timec隐藏版.exe", folder_path);       //注册表自启动路径以及复制的目标文件的路径
	sprintf_s(cmd, size, "attrib +s +h \"%s\" ", folder_path);            //给目录添加系统隐藏属性
}


/*****************************************************************************
					判断在当前时间内电脑是否能启动
*****************************************************************************/
int can_start(void)
{
	int  shutdown_sec, break_time, run_time;
	char temp_time[SIZE] = { 0 }, front_time[TIME_PERIOD_SIZE], back_time[TIME_PERIOD_SIZE] = { 0 };
	char date[SIZE] = { 0 };	     //储存电脑能运行的日期
	char time_period[TIME_PERIOD_SIZE] = { 0 };//储存电脑能运行的时间段
	char tip[SIZE] = { 0 };     //当电脑提前启动时输出关机提示
	time_t current_time, shutdown_time, estimated_time;

	//获取1970-01-01到当前的秒数
	struct tm  tm_current_time;
	time(&current_time);
	localtime_s(&tm_current_time, &current_time);//将获取的秒数转换为本地时间

	read_file(&shutdown_sec, &run_time, &break_time, time_period, sizeof(time_period), date, sizeof(date), &estimated_time);//获取电脑能运行的时间段和日期

	//判断当前日期是否能运行
	if (strcmp(date, "00") != 0)
	{
		sprintf_s(tip, sizeof(tip), "请在每个月的“%s”号运行本电脑！\n本电脑即将在%d秒内关机！", date, shutdown_sec);
		sprintf_s(temp_time, sizeof(temp_time), "%d", tm_current_time.tm_mday);
		if (find_str(date, ',', temp_time) == -1)// 如果在数组中不能找到当前日期，则关机
			shut_down(shutdown_sec, tip);
	}

	//判断当前时间是否在时间段内
	if (strcmp(time_period, "00:00-00:00") != 0)//判断是否不限时间段启动
	{
		sprintf_s(tip, SIZE, "请在每天的“%s”内运行本电脑！\n本电脑即将在%d秒内关机！", time_period, shutdown_sec);
		// 清除“-”和“:”
		//del_char(time_period, '-');
		del_char(time_period, ':');

		//index = get_index(time_period, ':');
		// 分割成前时刻和后时刻（分割前：time_period[TIME_PERIOD_SIZE]={"08:00-16:00"}，分割后：前时刻：behind_time[TIME_PERIOD_SIZE]={"08:00"}、后时刻：time_period[TIME_PERIOD_SIZE]={"16:00"}）
		split_str(time_period, '-', front_time, sizeof(front_time), back_time, sizeof(back_time));
		/*for (i = 0; i < strlen(time_period); i++, index++)
		{
			behind_time[i] = time_period[index];
			if (index == (strlen(time_period) - 1))
				time_period[index] = '\0';
		}*/

		sprintf_s(temp_time, TIME_PERIOD_SIZE, "%d%d", tm_current_time.tm_hour, tm_current_time.tm_min);//将当前时间写到数组 
		if (strcmp(temp_time, front_time) < 0 || strcmp(temp_time, back_time) > 0)     //判断当前时刻是否在能运行的时间段内
			shut_down(shutdown_sec, tip);
		memset(temp_time, 0, sizeof(temp_time));//清空数组
	}
	// 获取上次关机时间
	shutdown_time = get_shutdown_time(NULL, 0, 0);

	//判断当前时间是否小于预计关机时间
	if (current_time < estimated_time)
	{
		// 如果是自启动（不是手动点击启动）
		if (strcmp(start_parameter, REG_PARAMETER) == 0)
		{
			// 如果因为不可抗拒因素关机，或着玩累了电脑手动关机，然后一段时间后又开机的话，
			// 就判断当前时间 - 关机时间 < 休息时间。
			// 如果条件成立，就补上关机的那段时间。
			// 比如：（设定休息时间break_time为7分钟）预计12:00关机，但我11:40玩累了关机，然后11:45又开机继续玩，
			// 那么中间休息了5分钟（未超过设定的休息时间break_time），开机后的实际关机时间就要为12:05分。
			if (current_time - shutdown_time < (time_t)break_time * 60)
			{
				// 补上关机的那段时间，并重新设定关机
				write_estimated_time(estimated_time + (current_time - shutdown_time));// 预计关机时间 +（当前时间-实际关机时间）
				shut_down(estimated_time + (current_time - shutdown_time) - current_time, NULL);// 预计关机时间 + （当前时间 - 实际关机时间）- 实际关机时间
			}
			else
				// 比如：（设定休息时间break_time为7分钟）预计12:00关机，但我11:40玩累了关机，然后11:50又开机继续玩，
				// 那么中间休息了10分钟（超过了设定的休息时间break_time），那么设定run_time分钟后关机。
				return 0;
		}
		else
			shut_down(estimated_time - current_time, NULL);// 预计关机时间 + （当前时间 - 实际关机时间）- 实际关机时间
		//write_estimated_time(current_time + (estimated_time - shutdown_time));
		//shut_down(estimated_time - current_time, NULL);

		////判断当前时间与休眠时间是否相差小于5分钟
		//if ((current_time - lastSleepTime) <= 300 && lastSleepTime != 0)
		//{	//如果当前时间与休眠时间相差小于5分钟就算出当前时间距离下次关机时间还有多少分
		//	system("shutdown -a");
		//	sprintf_s(command, SIZE, "shutdown -f -s -t %lld", (ShutDownTime - current_time));//把时间差写到数组
		//	system(command); //执行关机命令
		//	shutdown_tip((long)(ShutDownTime - current_time));
		//}
	}
	// 当前时间 大于 预计关机时间 并且小于 预计关机时间 + 休息时间
	if (current_time >= estimated_time && current_time < (estimated_time + (time_t)break_time * 60))
	{
		shut_down(shutdown_sec, "当前为休息时间，电脑即将关机！");
	}
	write_estimated_time(current_time + (time_t)run_time * 60);
	return 0;
}

// 分割字符串（字符串只能有一个分割符：1,2）
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

// 写入预计关机时间（当前时间+能运行的时间）
void write_estimated_time(time_t estimated_time)
{
	char time[SIZE];
	sprintf_s(time, sizeof(time), "%lld", estimated_time);
	WritePrivateProfileStringA(APP_NAME, ESTIMATED_TIME_NAME, time, config_file_path);
}

// 将str1按照指定字符分割，并将分割后的每个字符串和str2比较是否相同
int find_str(char* str1, const char ch, char* str2)
{
	char* p = NULL;
	char* con_text = NULL;
	// 分割字符串
	p = (char*)strtok_s(str1, &ch, &con_text);
	while (p != NULL)
	{
		if (strcmp(p, str2) == 0)
			return 0;
		p = (char*)strtok_s(NULL, &ch, &con_text);
	}
	return -1;
}

// 获取电脑关机时间
long long get_shutdown_time(char* shutdown_time, size_t size, long long add_sec)
{
	const wchar_t shutdown_key[] = { L"SYSTEM\\CurrentControlSet\\Control\\Windows" };//系统启动项路径
	HKEY hkey;
	FILETIME file_time;
	DWORD dwsize;
	SYSTEMTIME system_time;
	ULARGE_INTEGER ui;
	long long time = 0;

	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, shutdown_key, 0, KEY_READ, &hkey) != ERROR_SUCCESS)//获取电脑上次关机时间
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

// 删除str中的指定字符
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

// 获取ch在str的下标
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
				 从配置文件读取电脑能运行的时间以及休息时间
*****************************************************************************/
void read_file(int* shutdown_sec, int* run_time, int* break_time, char* time_period, size_t time_period_size, char* date, size_t date_size, time_t* estimated_time)
{
	char temp[SIZE];
	/*strcat_s(content, MAX_SIZE, "\n\n注意：\n"
		"1、“电脑开机后能运行的时间”和“电脑关机后强制休息的时间”两项值的单位都为分，值范围5~999。\n"
		"2、“电脑每天能运行的时间段”项值的格式为“xx:xx-xx:xx”，请不要输入中文标点（如：“：”）。\n"
		"3、每一项末尾的分号是英文分号，更改内容请保留原格式。\n"
		"4、如果出现程序不能正常运行，请在“timec自定义版”重新创建配置文件或添加“-reset”参数启动timec隐藏版。");*/

	if (shutdown_sec)
	{// 电脑在休息时间内启动时关机时间
		GetPrivateProfileStringA(APP_NAME, SHUTDOWN_TIME_NAME, "30", temp, sizeof(temp), config_file_path);
		sscanf_s(temp, "%d", shutdown_sec);
	}
	if (run_time)
	{// 电脑能运行的时间
		GetPrivateProfileStringA(APP_NAME, RUN_TIME_NAME, "40", temp, sizeof(temp), config_file_path);
		sscanf_s(temp, "%d", run_time);
	}
	if (break_time)
	{// 电脑关机后强制休息的时间
		GetPrivateProfileStringA(APP_NAME, BREAK_TIME_NAME, "10", temp, sizeof(temp), config_file_path);
		sscanf_s(temp, "%d", break_time);
	}
	if (time_period)// 电脑每天能运行的时间段
		GetPrivateProfileStringA(APP_NAME, TIME_PERIOD_NAME, "00:00-00:00", time_period, (DWORD)time_period_size, config_file_path);
	if (date)// 电脑每个月能运行的日期
		GetPrivateProfileStringA(APP_NAME, DATE_NAME, "00", date, (DWORD)date_size, config_file_path);
	if (estimated_time)// 预计关机时间（单位：秒）
	{
		GetPrivateProfileStringA(APP_NAME, ESTIMATED_TIME_NAME, "-1", temp, sizeof(temp), config_file_path);
		sscanf_s(temp, "%lld", estimated_time);
	}
}


/*****************************************************************************
							   第一次启动设置
******************************************************************************/
void first_start(char* cmd)
{
	//显示程序运行窗口
	hide_window(SW_SHOW);
	system("title 第一次启动... && mode con cols=52 lines=17");
	char c;
	unsigned i, j = 0;
	int  front_hour, front_min, back_hour, back_min;
	char  time_period_backup[TIME_PERIOD_SIZE], time_period[TIME_PERIOD_SIZE]; //储存能运行的时间段
	char run_time[TIME_PERIOD_SIZE] = { 0 }, break_time[TIME_PERIOD_SIZE] = { 0 }, shutdown_sec[TIME_PERIOD_SIZE];	        //储存能运行的时间和休息时间
	char date[SIZE] = { 0 };						//储存电脑每个月能运行的日期


	/*for (i = 5; i > 0; i--)
	{
		printf("\t      timec隐藏版 第一次启动...\n\n\n\n\n\n\t\t    正在初始化...\n\n\n\n\n");
		printf("温馨提示：请以管理员权限运行本软件，并退出杀毒软件。\n\n\n");
		printf("\t\t\t%d", i);
		Sleep(1000);
		system("cls");
	}*/

	/***********************/
begin://goto语句标签
/***********************/

	system("cls && mode con cols=72 lines=25");

	printf("温馨提示：如需重新设定参数，请在“timec自定义版”输入9再输入5修改。");
	printf("\n\n请输入电脑每天能运行的时间段（格式：xx:xx - xx:xx，请不要输入中文标点。\n温馨提示：输入00:00 - 00:00不限制运行时间段。）：");
	while (1)
	{
		memset(time_period_backup, 0, sizeof(time_period_backup));
		memset(time_period, 0, sizeof(time_period));

		fgets(time_period, TIME_PERIOD_SIZE, stdin);
		rewind(stdin);
		fgets_n(time_period);	//清除fgets读取的'\n'

		del_char(time_period, ' ');
		//把time_period的内容复制到time_period_backup
		strcpy_s(time_period_backup, sizeof(time_period_backup), time_period);

		//判断输入的格式是否正确
		if (strlen(time_period) != 11 || time_period[5] != '-')
		{
			printf("\n格式错误（例如：08:00 - 14:00），请重试：");
			continue;
		}

		for (i = 0; i < TIME_PERIOD_SIZE; i++)
		{
			if (time_period[i] == ':' || time_period[i] == '-')
				time_period[i] = ' ';
		}

		sscanf_s(time_period, "%d %d %d %d", &front_hour, &front_min, &back_hour, &back_min);

		//判断输入的时刻是否有误
		if ((front_hour > 24 || front_hour < 0) || (back_hour > 24 || back_hour < 0) || (front_min > 60 || front_min < 0) || (back_min > 60 || back_min < 0))
		{
			printf("\n格式错误（例如：08:00 - 14:00），请重试：");
			continue;
		}
		if (front_hour == 24 || back_hour == 24)//判断两个时刻小时部分是否为24
		{
			if (front_hour == 24 && back_hour == 24)//判断两个时刻小时部分是否都为24
			{
				time_period_backup[1] = time_period_backup[0] = '0';
				if (time_period[8] == '2' && time_period[9] == '4')
					time_period_backup[9] = time_period_backup[8] = '0';
				else
					time_period_backup[7] = time_period_backup[6] = '0';
				printf("\n格式错误，应输入为：%s。请重试：", time_period_backup);
			}
			else if (front_hour == 24)//判断前时刻小时部分是否为24
			{
				time_period_backup[1] = time_period_backup[0] = '0';
				printf("\n格式错误，应输入为：%s。请重试：", time_period_backup);
			}
			else if (back_hour == 24)//判断后时刻小时部分是否为24
			{
				if (time_period[8] == '2' && time_period[9] == '4')
					time_period_backup[9] = time_period_backup[8] = '0';
				else
					time_period_backup[7] = time_period_backup[6] = '0';
				printf("\n格式错误，后时刻应输入为：%s。请重试：", time_period_backup);
			}
			continue;
		}

		//判断输入是否为00:00 - 00:00
		if (front_hour == 0 && back_hour == 0 && front_min == 0 && back_min == 0)
			break;

		//判断后时刻是否比前时刻大
		if (front_hour > back_hour)
		{
			printf("\n前时刻要比后时刻小，请重试：");
			continue;
		}
		else if (front_hour == back_hour)
		{
			if (front_min > back_min)
			{
				printf("\n前时刻要比后时刻小。请重试：");
				continue;
			}
			else if (front_min == back_min)
			{
				printf("\n前时刻要比后时刻小，且不能相等。请重试：");
				continue;
			}
		}

		//判断前时刻与后时刻是否相差小于5分钟
		front_hour = (front_hour * 3600) + (front_min * 60);
		back_hour = (back_hour * 3600) + (back_min * 60);
		if ((back_hour - front_hour) < 300)
		{
			printf("\n前时刻与后时刻必须相差5分钟。请重试：");
			continue;
		}
		break;
	}


	printf("\n\n请输入电脑每个月能运行的日期（例如：05。\n温馨提示：如果要输入多个日期请用英文逗号分隔开，例如：1,5,10,25,30。\n输入00不限制运行日期。）：");
	while (1)
	{
		memset(date, 0, sizeof(date));
		fgets(date, sizeof(date), stdin);
		rewind(stdin);
		fgets_n(date);

		if (strlen(date) <= 1 || judge_date(date, sizeof(date), ','))
		{
			printf("\n格式错误，请重新输入（例如：01,05,10）：");
			continue;
		}
		break;
	}

	printf("\n\n请输入电脑每次能运行的时间\n（也就是电脑每次运行XX分钟后强制关机。单位：分钟）：");
	input_number(run_time, sizeof(run_time), 999);

	printf("\n\n请输入电脑每次休息时间\n（也就是电脑每次强制关机后需要在XX分钟后才能使用。单位：分钟）：");
	input_number(break_time, sizeof(break_time), 999);

	printf("\n\n请输入当电脑在能运行的时间段外或在休息时间内启动时，电脑关机时间\n（单位：秒。最大值60秒）：");
	input_number(shutdown_sec, sizeof(break_time), 60);

	system("cls && mode con cols=47 lines=15");
	printf("当前设置的参数为：\n\n");

	if (strcmp(time_period_backup, "00:00-00:00") == 0)
		printf("电脑每天能运行的时间段： %s（不限制运行时间段）\n", time_period_backup);

	else if (strcmp(time_period_backup, "00:00 - 00:00") == 0)
		printf("电脑每天能运行的时间段： %s（不限制运行时间段）\n", time_period_backup);

	else
		printf("电脑每天能运行的时间段： %s", time_period_backup);

	if (strcmp(date, "00") == 0)
		printf("\n电脑每个月能运行的日期： %s（不限制运行日期）", date);
	else
		printf("\n电脑每个月能运行的日期： %s 号", date);

	printf("\n电脑每次能运行的时间：   %s 分", run_time);
	printf("\n电脑每次休息的时间：     %s 分", break_time);
	printf("\n在休息时间启动时关机时间：%s 秒", shutdown_sec);

	while (1)
	{
		printf("\n\n\t      是否确定？（Y/N）");
		printf("\n\t请输入：");
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

	system("cls && title 正在添加自启动，请退出杀软 &&mode con cols=72 lines=25");
	printf("\n\n\n\n\n注意：正在添加自启动，若遭到杀软拦截，请点击允许并加入白名单。\n\n\t                否则软件无法正常启动！\n\n\n");
	Sleep(3000);
	add_start();
	exit(EXIT_SUCCESS);
}

// 写配置文件
void write_file(char* cmd, char* run_time, char* break_time, char* time_period, char* date, char* shutdown_time)
{
	FILE* fp = NULL;
	char content[MAX_SIZE] = { 0 };		//储存将要写入配置文件的内容         
	//创建目录
	if (_access(folder_path, 0) == -1)//判断目录是否存在
	{
		if (_mkdir(folder_path) != 0)
		{
			MessageBox(NULL, TEXT("创建目录失败，请以管理员权限运行本软件。"), TEXT("ERROR"), MB_YESNO | MB_ICONERROR);
			exit(EXIT_FAILURE);
		}
	}
	system(cmd);//执行cmd命令隐藏目录

	WritePrivateProfileStringA(APP_NAME, RUN_TIME_NAME, run_time, config_file_path);
	WritePrivateProfileStringA(APP_NAME, BREAK_TIME_NAME, break_time, config_file_path);
	WritePrivateProfileStringA(APP_NAME, TIME_PERIOD_NAME, time_period, config_file_path);
	WritePrivateProfileStringA(APP_NAME, DATE_NAME, date, config_file_path);
	WritePrivateProfileStringA(APP_NAME, SHUTDOWN_TIME_NAME, shutdown_time, config_file_path);

	//创建配置文件
	if (fopen_s(&fp, config_file_path, "a+") != 0)
	{
		perror("\n创建配置文件失败");
		system("pause");
		exit(EXIT_FAILURE);
	}
	strcat_s(content, MAX_SIZE, "\n\n注意：\n"
		"1、“电脑开机后能运行的时间”和“电脑关机后强制休息的时间”两项值的单位都为分，值范围5~999。\n"
		"2、“电脑每天能运行的时间段”项值的格式为“xx:xx-xx:xx”，请不要输入中文标点（如：“：”）。\n"
		"3、每一项末尾的分号是英文分号，更改内容请保留原格式。\n"
		"4、如果出现程序不能正常运行，请在“timec自定义版”重新创建配置文件或添加“-reset”参数启动timec隐藏版。");
	if (fp)
	{
		fputs(content, fp);//向配置文件输出参数
		fclose(fp);
	}
}

// 添加自启动
void add_start(void)
{
	char* key = { "Software\\Microsoft\\Windows\\CurrentVersion\\Run" }; //系统启动项路径
	HKEY hkey;
	int ret_val;
	char path[MAX_PATH];  //储存timec隐藏版.exe的路径    

	//复制文件
	GetModuleFileNameA(NULL, path, MAX_PATH); //调用windows api获得“timec隐藏版.exe”的路径
	if (!CopyFileA(path, dest_path, FALSE))
	{
		MessageBox(NULL, TEXT("复制文件失败，请以管理员权限重试！"), TEXT("错误"), MB_OK | MB_ICONERROR);
		exit(EXIT_FAILURE);
	}

	//打开注册表添加启动项
	if (RegOpenKeyExA(HKEY_CURRENT_USER, key, 0, KEY_ALL_ACCESS, &hkey) != ERROR_SUCCESS)
	{
		MessageBox(NULL, TEXT("添加到注册表失败！"), TEXT("失败"), MB_OK | MB_ICONERROR);
		exit(EXIT_FAILURE);
	}
	//添加一个子Key,并设置值
	sprintf_s(path, sizeof(path), "\"%s\" %s", dest_path, REG_PARAMETER);
	RegSetValueExA(hkey, "timec隐藏版", 0, REG_SZ, (BYTE*)path, MAX_PATH);
	RegCloseKey(hkey);//关闭注册表

	ret_val = MessageBox(NULL, TEXT("添加自启动成功，重启后生效。是否现在重启？\n注意：1、如需取消自启动，请在timec自定义版删除。\n            2、重启前请保存好数据资料！"), TEXT("成功"), MB_YESNO | MB_ICONQUESTION);
	if (ret_val == IDYES)
		system("shutdown -a && shutdown -r -t 00");
}

// 判断按ch分割的字符串（1,2,3）的每个日期是否正确（大于0小于32）
int judge_date(const char* date_str, size_t date_str_size, char sep)
{
	int date;
	char* p = NULL, * con_text = NULL;
	char* backup = calloc(date_str_size, sizeof(char));
	if (!backup)
		return 0;

	strcpy_s(backup, date_str_size, date_str);
	// 分割字符串
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

// 输入数字
int input_number(char* dest_str, size_t dest_str_size, int max_value)
{
	int num;
	while (1)
	{
		while (scanf_s("%d", &num) != 1)
		{
			printf("\n输入错误，请重试：");
			Flush();
		}
		Flush();
		if (num > max_value)
		{
			printf("\n最大值不能超过%d，请重新输入：", max_value);
			continue;
		}
		break;
	}
	if (dest_str && dest_str_size > 0)
		sprintf_s(dest_str, dest_str_size, "%d", num);
	return num;
}

/*****************************************************************************
							 执行关机命令
*****************************************************************************/
void shut_down(time_t sec, char* tip)
{
	int  run_time;
	char command[SIZE];
	TCHAR* buf = NULL;

	if (sec == -1)
	{
		//从配置文件读取电脑能运行的时间
		read_file(NULL, &run_time, NULL, NULL, 0, NULL, 0, NULL);
		run_time *= 60;//把分转换为秒
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
		// 将多字节字符转为宽字节字符
		buf = str_to_wcs(tip);
		MessageBox(NULL, buf, TEXT("警告"), MB_OK | MB_ICONWARNING);
		exit(EXIT_SUCCESS);
	}
}

// 多字节字符转宽字节字符
wchar_t* str_to_wcs(char* source_str)
{
	// 先求得需要的目标缓冲区的大小
	int size = MultiByteToWideChar(CP_ACP, 0, source_str, -1, NULL, 0);
	wchar_t* dest_str = calloc(size, sizeof(TCHAR));
	MultiByteToWideChar(CP_ACP, 0, source_str, -1, dest_str, size);
	return dest_str;
}



/*****************************************************************************
				当前时间距离关机时间还有3分时输出关机提示
*****************************************************************************/
void shutdown_tip(const long sec)
{
	long second = sec - 180;
	char tip[SIZE] = { 0 };

	if (second <= 180)
	{
		sprintf_s(tip, sizeof(tip), "shutdown -a && shutdown -f -s -t %d -c 还有 %d分 就要关机了，请做好准备！", second, second / 60);
		system(tip);
	}
	else
	{
		Sleep(second * 1000);
		system("shutdown -a && shutdown -f -s -t 180 -c 还有 3分 就要关机了，请做好准备！");
	}
	exit(EXIT_SUCCESS);
}


/*****************************************************************************
								清除缓冲区
*****************************************************************************/
void Flush(void)
{
	int ch;
	while ((ch = getchar()) != '\n' && ch != EOF)
		;
}


/*****************************************************************************
							清除fgets读取的'\n'
*****************************************************************************/
void fgets_n(char* str)
{
	char* find;
	if ((find = strchr(str, '\n')))
		*find = '\0';
}