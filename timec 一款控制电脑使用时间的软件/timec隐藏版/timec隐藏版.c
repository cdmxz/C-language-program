#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <direct.h>
#include <io.h>

#define SIZE 50

int  GetTime(void);        //比较当前时间和电脑能启动的时间
int  WriteTime(void);      //写入电脑关机时间和下次能启动时间
void ShutDown(void);	   //执行关机命令
int  Read_File(int *RunTime, int *BreakTime, char *str);//从配置文件读取电脑能运行的时间和休息时间
void Shutdown_Tip(long sec);//当距离关机时间还有3分钟时输出关机提示
void FirstStart(void);	   //第一次启动自定义参数
void Flush(void);		   //清除缓冲区
void Fgets_n(char *str);   //清除fgets读取的'\n'
void Initialize(char *temp_time_period, char *time_period);//初始化数组

														   /*例如：电脑运行40分钟，要休息10分钟后才能继续运行。
假如当前时间为2019年11月23日11:20，我设定电脑运行40分钟就要休息10分钟。
也就是说要在当前时间的基础上加上50分钟后才能重新打开电脑。
因为电脑运行40分钟后关机，此时的时间是2019年11月23日12:00，再加上10分钟休息时间，
所以我要在2019年11月23日12:10后才能再次打开电脑否则就提示关机。*/

//数据文件路径
char time_dat_path[] = { "C:\\Program Files\\timec\\time.dat" };
char config_file_path[] = { "C:\\Users\\Timec_config.txt" };
char folder_path[] = { "C:\\Program Files\\timec" };
char str[15];//无用
int x;//无用

int main(void)
{		
	//隐藏程序运行窗口
	HWND hwnd;
	hwnd = FindWindow("ConsoleWindowClass", NULL);
	if (hwnd)
		ShowWindow(hwnd, SW_HIDE);

	//判断数据文件“time.dat”是否存在
	if (_access(time_dat_path, 0))
	{//不存在则创建目录调用第一次启动设置
		_mkdir(folder_path);//创建目录
		SetFileAttributes(folder_path, FILE_ATTRIBUTE_HIDDEN);//调用windows api隐藏目录
		FirstStart();
	}
	
	GetTime();
	WriteTime();
	ShutDown();
	return 0;
}


//比较时间 
int GetTime(void)
{
	int i, front_hour, front_min, behind_hour, behind_min;
	long long ShutDownTime;
	char temp_time[SIZE];
	char command[100] = { "shutdown -s -t " };
	char last_time[SIZE];      //电脑下次能运行的时间的秒数（电脑上次关闭后的时间加上休息时间）
	char time_now[SIZE];       //储存当前时间
	char shut_down_time[SIZE]; //储存电脑关机时间
	char tip[100] = { 0 };	   //当电脑提前启动时输出关机提示
	char lpSubKey[] = { "SYSTEM\\CurrentControlSet\\Control\\Windows" };
	char time_period[15] = { 0 };
	char time_period_tip[100] = { "请在“" };

	FILE *fp;
	errno_t err;
	HKEY hKey;
	FILETIME file_time;
	DWORD dwsize;
	SYSTEMTIME system_time;
	ULARGE_INTEGER ui;
	time_t current_time, shutdown_time;

	//获取1970-01-01到当前的秒数
	struct tm  tm_current_time;
	time(&current_time);
	localtime_s(&tm_current_time, &current_time);//将获取的秒数转换为本地时间

	//获取电脑能运行的时间段
	Read_File(&x, &x, time_period);

	//判断当前时间是否在时间段内
	if (strcmp(time_period, "00:00-00:00") != 0)//判断是否不限时间段启动
	{
		strcat_s(time_period_tip, 100, time_period);
		strcat_s(time_period_tip, 100, "”时间段内运行本电脑！\n本电脑将在10秒内关机！");

		for (i = 0; i < 15; i++)
			if (time_period[i] == '-' || time_period[i] == ':')
				time_period[i] = ' ';
		//将数组里的时间段写到变量
		sscanf_s(time_period, "%d %d %d %d", &front_hour, &front_min, &behind_hour, &behind_min);
		
		//判断当前时间是否在时间段内
		if ((tm_current_time.tm_hour > front_hour) && (tm_current_time.tm_hour < behind_hour))
			;
		else if ((tm_current_time.tm_hour == front_hour) || (tm_current_time.tm_hour == behind_hour))
		{
			if ((tm_current_time.tm_min >= front_min) && (tm_current_time.tm_min < behind_min))
				;
			else
			{
				system("shutdown -s -t 10");
				MessageBox(NULL, (time_period_tip), TEXT("警告："), MB_OK | MB_ICONWARNING);
			}
		}
		else
		{
			system("shutdown -s -t 10");
			MessageBox(NULL, (time_period_tip), TEXT("警告："), MB_OK | MB_ICONWARNING);
		}
	}
	
	
	//打开“time.dat”,并判断文件是否正常打开
	if ((err = fopen_s(&fp, time_dat_path, "r")) != 0)
		return 1;//如果打开失败就返回主函数执行关机命令
	
	if (_filelength(_fileno(fp)) == 0)//判断文件是否为空
	{
		fclose(fp);
		return 1;
	}
	//从“time.dat”读取内容
	for (i = 0; i < 4; i++)
	{
		if (i == 1)
			//从“time.dat”读取第一行，电脑关机的时间的秒数         
			fgets(shut_down_time, SIZE, fp);
		else if (i == 2)
			//“time.dat”读取第二行，电脑下次能运行的时间的秒数
			fgets(last_time, SIZE, fp);
		else if (i == 3)
			//从“time.dat”读取第三行，提示
			fgets(tip, 100, fp);
	}
	fclose(fp); 
	//清除fgets读取的换行符
	Fgets_n(shut_down_time);
	Fgets_n(last_time);
	
	//清除数组中的中文字符
	strcpy_s(shut_down_time, strlen(shut_down_time) + 1, shut_down_time + 22);
	strcpy_s(last_time, strlen(last_time) + 1, last_time + 28);
	
	//将当前时间的秒数写入到数组
	sprintf_s(time_now, SIZE, "%lld", (long long)current_time);

	//判断当前时间是否小于电脑关机时间
	if (strcmp(shut_down_time, time_now) > 0)
	{	
		//获取电脑上次关机时间
		if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpSubKey, 0, KEY_READ, &hKey) != ERROR_SUCCESS)
			ShutDown();

		if (RegQueryValueEx(hKey, TEXT("ShutdownTime"), NULL, NULL, NULL, &dwsize) == ERROR_SUCCESS && dwsize == sizeof(file_time))
		{
			RegQueryValueEx(hKey, TEXT("ShutdownTime"), NULL, NULL, (LPBYTE)&file_time, &dwsize);

			//FileTimeToSystemTime将FILETIME时间转换为SYSTEMTIME时间
			FileTimeToSystemTime(&file_time, &system_time);
			ui.LowPart = file_time.dwLowDateTime;
			ui.HighPart = file_time.dwHighDateTime;
			shutdown_time = (LONGLONG)(ui.QuadPart - 116444736000000000) / 10000000;
		}
		RegCloseKey(hKey);

		sscanf_s(shut_down_time, "%lld", &ShutDownTime);//将数组内容写到变量
		
		/*
		struct tm tm_Shut_Down_Time;
		localtime_s(&tm_Shut_Down_Time, &shutdown_time);
		printf("关机时间：%4d-%02d-%02d %02d:%02d:%02d\n", tm_Shut_Down_Time.tm_year + 1900, tm_Shut_Down_Time.tm_mon + 1, tm_Shut_Down_Time.tm_mday, tm_Shut_Down_Time.tm_hour, tm_Shut_Down_Time.tm_min, tm_Shut_Down_Time.tm_sec);
		*/
		if ((long long)(current_time - shutdown_time) < 0)//检查是否乱码
			ShutDown();
		if ((long long)(current_time - shutdown_time) > 300)//判断当前时间与关机时间是否相差大于5分钟
			return 0;//如果当前时间与关机时间相差大于5分钟就重新写入关机时间并执行关机

		//如果当前时间与关机时间相差小于5分钟就算出当前时间距离下次关机时间还有多少分
		sprintf_s(temp_time, SIZE, "%lld", (long long)(ShutDownTime - current_time));//把时间差写到数组
		strcat_s(command, 100, temp_time);
		system(command);//执行关机命令
		Shutdown_Tip((long)(ShutDownTime - current_time));
		//exit(EXIT_SUCCESS);
	}
	//否则判断当前时间是否小于电脑能启动的时间
	else if (strcmp(last_time, time_now) > 0)
	{
		//如果当前时间小于电脑能启动的时间就执行10秒关机命令并且输出关机提示
		system("shutdown -s -t 10");
		//弹出关机提示
		MessageBox(NULL, (tip), TEXT("警告！"), MB_OK | MB_ICONWARNING);
		exit(EXIT_SUCCESS);
	}
	return 0;
}


/*写入电脑关闭后休息的时间，即电脑下次能启动时间。
例如：电脑能运行40分钟休息10分钟，
也就是说电脑下次能启动的时间要在当前时间上加上40分能运行时间和10分钟休息时间*/
int WriteTime(void)
{
	FILE *fp;
	errno_t err;
	int min, hour, day, mon, year, run_time, break_time;
	long sec = 0;
	Read_File(&run_time, &break_time, str);

	//打开文件,并且判断文件是否正常打开
	if ((err = fopen_s(&fp, time_dat_path, "w")) != 0)
		return 1;


	time_t last_time;
	struct tm p;
	time(&last_time);//获取当前时间的秒数
	localtime_s(&p, &last_time);
	min = p.tm_min + run_time + break_time;//在当前分钟的基础上加上xx分钟的运行时间和xx分钟休息时间
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
	//判断月份
	if (mon == 1 || mon == 3 || mon == 5 || mon == 7 || mon == 8 || mon == 10 || mon == 12)//判断当前月份是否为大月
		while (day > 31)
		{
			day -= 31;
			mon++;
		}

	if (mon == 4 || mon == 6 || mon == 9 || mon == 11)//判断当前月份是否为小月
		while (day > 30)
		{
			day -= 30;
			mon++;
		}

	if (mon == 2)
		if (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0))//判断当前年份是否为闰年
		{
			//printf("是闰年\n");
			while (day > 29)
			{
				day -= 29;//闰年2月有29天
				mon++;
			}
		}
		else while (day > 28)
		{
			day -= 28;//平年2月有28天
			mon++;
		}

	while (mon > 12) //判断当前月份是否超过12月
	{
		mon -= 12;
		year++;
	}

	run_time *= 60;
	break_time *= 60;

	sec = sec + (long)last_time + (long)run_time;
	//向文件写入电脑下次能运行时间的秒数
	fprintf(fp, "电脑关机的时间的秒数：%ld\n", sec);
	sec += (long)break_time;
	fprintf(fp, "电脑下次能运行的时间的秒数：%ld\n", sec);
	//向文件写入提示信息
	fprintf(fp, "当前为休息时间,下次此电脑允许启动时间为：%02d年%02d月%02d日%02d时%02d分后。    电脑即将在10秒内关机！", year, mon, day, hour, min);
	fclose(fp);
	return 0;
}


//执行关机命令
void ShutDown(void)
{
	int  run_time;
	char command[22] = { "shutdown -s -t " };
	char temp[7];

	//从配置文件读取电脑能运行的时间
	Read_File(&run_time, &x, str);
	run_time *= 60;//把分转换为秒

	sprintf_s(temp, 7, "%d", run_time);
	strcat_s(command, 22, temp);
	system(command);
	Shutdown_Tip(run_time);
}


//从配置文件读取电脑能运行的时间以及休息时间
int Read_File(int *RunTime, int *BreakTime, char *str)
{
	int ch, i, j = 0;
	int run_time = 40;	//默认能运行时间（单位：分）
	int break_time = 10;//默认休息时间（单位：分）
	char config[15] = { 0 };
	char time_period[] = { "00:00-00:00" };//默认能运行的时间段
	FILE *fp;
	errno_t err;


	*RunTime = run_time;
	*BreakTime = break_time;
	for (i = 0; i < 12; i++)
		str[i] = time_period[i];

	//从配置文件读取内容
	if ((err = fopen_s(&fp, config_file_path, "r")) != 0)//判断文件是否正常打开
		return 1;//打开失败就返回默认时间


	if ((ch = fgetc(fp)) == EOF)  //判断文件是否为空
	{
		MessageBox(NULL, TEXT("错误"), TEXT("“timec隐藏版”配置文件错误！\n请在“timec自定义版”重新设置配置文件！"), MB_OK | MB_ICONERROR);
		return 1;
	}
	do
	{
		ch = fgetc(fp);
		if (ch == '=')        //判断ch是否为“=”
			while ((ch = fgetc(fp)) != ';')
			{
				if (ch == ' ')//判断ch是否为空格
				{
					i = 0;
					continue;//丢弃读取的空格
				}
				config[i] = ch;
				i++;
			}
		if (ch == '\n')       //判断是否读到下一行
		{
			if (j == 0)       //判断是否读到第二行
			{
				sscanf_s(config, "%d", &run_time); //把第一行的内容保存到run_time，第一行的内容是电脑能运行的时间
				for (i = 0; i < 12; i++)
					config[i] = '\0';//初始化数组
			}
			else if (j == 1)
			{
				sscanf_s(config, "%d", &break_time); //把第二行的内容保存到break_time，第二行的内容是电脑关机后休息的时间
				for (i = 0; i < 12; i++)
					config[i] = '\0';//初始化数组
			}
			j++;
			i = 0;
		}
	} while (ch != EOF);

	if(config[2]!=':'||config[5]!='-'||config[8]!=':')
	{
		MessageBox(NULL, TEXT("错误"), TEXT("“timec隐藏版”配置文件错误！\n请在“timec自定义版”重新设置配置文件！"), MB_OK | MB_ICONERROR);
		return 1;
	}
	//返回从配置文件读取到的内容
	*RunTime = run_time;
	*BreakTime = break_time;
	for (i = 0; i < 12; i++)
		str[i] = config[i];
	return 0;
}


//第一次启动设置
void FirstStart(void)
{
	//显示程序运行窗口
	HWND hwnd;
	hwnd = FindWindow("ConsoleWindowClass", NULL);
	if (hwnd)
		ShowWindow(hwnd, SW_SHOW);

	system("title 第一次启动... && mode con cols=52 lines=17");

	int ch, i = 5, front_hour, front_min, behind_hour, behind_min;
	char config_content[350] = { "run time = " }; //储存将要写入配置文件的内容
	char path[MAX_PATH];				          //储存timec隐藏版.exe的路径
	char temp_time_period[15], time_period[15];   //储存能运行的时间段
	char run_time[5], break_time[5];	          //储存能运行的时间和休息时间
	char *SubKey = { "Software\\Microsoft\\Windows\\CurrentVersion\\Run" }; //系统启动项路径
	char *timec_path = { "C:\\Program Files\\timec\\timec隐藏版.exe" };     //注册表自启动路径以及复制的目标文件的路径
	HKEY hKey;
	FILE *fpread, *fpwrite;
	errno_t err;

	for (; i > 0; i--)
	{
		printf("\t      timec隐藏版 第一次启动...\n\n\n\n\n\n\t\t    正在初始化...\n\n\n\n\n");
		printf("温馨提示：请以管理员权限运行本软件，并退出杀毒软件。\n\n\n");
		printf("\t\t\t%d", i);
		Sleep(1000);
		system("cls");
	}

	system("cls && mode con cols=72 lines=17");
	
	printf("温馨提示：如需重新设定时间，请编辑“C:\\Users\\Timec_config.txt”。");
	printf("\n\n请输入电脑能运行的时间段（格式：xx:xx - xx:xx，请不要输入中文标点。\n温馨提示：输入00:00 - 00:00不限制时间段运行。）：");
	while (1)
	{

		Initialize(temp_time_period, time_period);

		fgets(time_period, 15, stdin);
		rewind(stdin);
		Fgets_n(time_period);	//清除fgets读取的'\n'
		
		for (i = 0; i < 15; i++)//把time_period的内容复制到temp_time_period
			temp_time_period[i] = time_period[i];
		
		//判断输入的格式是否正确
		if (strlen(time_period) != 13 || time_period[6] != '-')//判断输入格式是xx:xx-xx:xx还是xx:xx - xx:xx
			if (strlen(time_period) != 11 || time_period[5] != '-')
			{
				printf("\n格式错误（例如：08:00 - 14:00），请重试：");
				continue;
			}
		
		for (i = 0; i < 15; i++)
			if (time_period[i] == ':' || time_period[i] == '-')
				time_period[i] = ' ';

		sscanf_s(time_period, "%d %d %d %d", &front_hour, &front_min, &behind_hour, &behind_min);
		//判断输入的时刻是否有误
		if ((front_hour > 24 || front_hour < 0) || (behind_hour > 24 || behind_hour < 0) || (front_min > 60 || front_min < 0) || (behind_min > 60 || behind_min < 0))
		{
			printf("\n格式错误（例如：08:00 - 14:00），请重试：");
			continue;
		}
		if (front_hour == 24 || behind_hour == 24)//判断两个时刻小时部分是否为24
		{
			if (front_hour == 24 && behind_hour == 24)//判断两个时刻小时部分是否都为24
			{
				temp_time_period[1] = temp_time_period[0] = '0';
				if (time_period[8] == '2' && time_period[9] == '4')
					temp_time_period[9] = temp_time_period[8] = '0';
				else
					temp_time_period[7] = temp_time_period[6] = '0';
				printf("\n格式错误，应输入为：%s。请重试：", temp_time_period);
			}
			else if (front_hour == 24)//判断前时刻小时部分是否为24
			{
				temp_time_period[1] = temp_time_period[0] = '0';
				printf("\n格式错误，应输入为：%s。请重试：", temp_time_period);
			}
			else if (behind_hour == 24)//判断后时刻小时部分是否为24
			{
				if (time_period[8] == '2' && time_period[9] == '4')
					temp_time_period[9] = temp_time_period[8] = '0';
				else
					temp_time_period[7] = temp_time_period[6] = '0';
				printf("\n格式错误，后时刻应输入为：%s。请重试：", temp_time_period);
			}
			continue;
		}
	
		//判断输入是否为00:00 - 00:00
		if (front_hour == 0 && behind_hour == 0 && front_min == 0 && behind_min == 0)
			break;

		//判断后时刻是否比前时刻大
		if (front_hour > behind_hour)
		{
			printf("\n前时刻要比后时刻小，请重试：");
			continue;
		}
		else if (front_hour == behind_hour)
		{
			if (front_min > behind_min)
			{
				printf("\n前时刻要比后时刻小。请重试：");
				continue;
			}
			else if (front_min == behind_min)
			{
				printf("\n前时刻要比后时刻小，且不能相等。请重试：");
				continue;
			}
		}
		
		//判断前时刻与后时刻是否相差小于5分钟
		front_hour = (front_hour * 3600) + (front_min * 60);
		behind_hour = (behind_hour * 3600) + (behind_min * 60);
		if ((behind_hour - front_hour) < 300)
		{
			printf("\n前时刻与后时刻必须相差5分钟。请重试：");
			continue;
		}
		break;
	}

	printf("\n\n请输入电脑能运行的时间\n（也就是电脑运行XX分钟后强制关机。单位：分钟）：");
	while (1)
	{
		while (scanf_s("%d", &i) != 1)
		{
			printf("\n输入错误，请重试：");
			Flush();
		}
		Flush();
		if (i > 999)
		{
			printf("\n最大值不能超过999，请重新输入：");
			continue;
		}
		sprintf_s(run_time, 5, "%d", i);
		break;
	}

	printf("\n\n请输入电脑休息时间\n（也就是电脑强制关机后需要在XX分钟后才能使用。单位：分钟）：");
	while (1)
	{
		while (scanf_s("%d", &i) != 1)
		{
			printf("\n输入错误，请重试：");
			Flush();
		}
		Flush();
		if (i > 999)
		{
			printf("\n最大值不能超过999，请重新输入：");
			continue;
		}
		sprintf_s(break_time, 5, "%d", i);
			break;
	}
	//创建数据文件
	strcat_s(config_content, 350, run_time);
	strcat_s(config_content, 350, " ;     电脑能运行的时间。\nbreak time = ");
	strcat_s(config_content, 350, break_time);
	strcat_s(config_content, 350, " ;   电脑休息的时间。\ntime period = ");
	strcat_s(config_content, 350, temp_time_period);
	strcat_s(config_content, 350, " ;   电脑能运行的时间段。\n\n注意：“run time”和“break time”两项值的单位都为分，值范围5~999。\n“time period”项值的格式为“xx:xx-xx:xx”，请不要输入中文标点（如：“：”）。\n更改请保留原格式，如果出现程序不能正常运行，请删除此文件。");
	if ((err = fopen_s(&fpwrite, config_file_path, "w")) != 0)
	{
		remove(folder_path);
		perror("\n创建配置文件失败");
		system("pause");
		exit(EXIT_FAILURE);
	}
	fputs(config_content, fpwrite);
	fclose(fpwrite);

	//复制文件
	system("cls && title 正在添加自启动，请退出杀软");
	printf("\n\n\n\n\n注意：正在添加自启动，若遭到杀软拦截，请点击允许并加入白名单。\n\n\t                否则软件无法正常启动！\n\n\n");
	Sleep(3000);

	GetModuleFileName(NULL, path, 100); //调用windows api获得“timec隐藏版.exe”的路径
	
	if ((err = fopen_s(&fpread, path, "rb")) != 0)
	{
		remove(folder_path);
		perror("\n复制文件失败");
		system("pause");
		exit(EXIT_FAILURE);
	}

	if ((err = fopen_s(&fpwrite, timec_path, "wb")) != 0)
	{
		remove(folder_path);
		perror("\n复制文件失败");
		system("pause");
		exit(EXIT_FAILURE);
	}

	do
	{
		ch = fgetc(fpread);
		fputc(ch, fpwrite);
	} while (ch != EOF);

	fclose(fpread);//关闭文件
	fclose(fpwrite);

	//创建数据文件
	if ((err = fopen_s(&fpwrite, time_dat_path, "w")) != 0)
	{
		remove(folder_path);
		perror("\n创建数据文件失败");
		system("pause");
		exit(EXIT_FAILURE);
	}
	fclose(fpwrite); //数据文件内容为空

	//打开注册表添加启动项 
	if (RegOpenKeyEx(HKEY_CURRENT_USER, SubKey, 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)
	{  //添加一个子Key,并设置值
		RegSetValueEx(hKey, "timecyincang", 0, REG_SZ, (BYTE *)timec_path, strlen(timec_path));
		RegCloseKey(hKey);//关闭注册表

		i = MessageBox(NULL, TEXT("设置成功！重启后生效，是否现在重启？\n注意：1、如需取消自启动，请在timec自定义版删除。\n\t2、重启前请保存好数据资料！"), TEXT("设置成功！"), MB_YESNO | MB_ICONQUESTION);
		if (i == IDYES)
			system("shutdown -r -t 00");
		else if (i == IDNO)
			exit(EXIT_SUCCESS);
	}
	else
	{
		printf("\n\n添加自启动失败！");
		MessageBox(NULL, TEXT("设置失败！请以管理员权限运行本软件重试！"), TEXT("设置失败！"), MB_OK | MB_ICONERROR);
		remove(folder_path);
		exit(EXIT_SUCCESS);
	}
	exit(EXIT_SUCCESS);
}

//当前时间距离关机时间还有3分时输出关机提示
void Shutdown_Tip(long sec)
{
	long second = sec - 180;
	
	if (second < 180)
		exit(EXIT_SUCCESS);
	
	second *= 1000;
	Sleep(second);
	MessageBox(NULL, TEXT("还有3分钟就要关机了，请做好准备！"), TEXT("警告"), MB_OK | MB_ICONWARNING);
	exit(EXIT_SUCCESS);
}


//清除缓冲区
void Flush(void)
{
	int ch;
	while ((ch = getchar()) != '\n' && ch != EOF)
		;
}


//清除fgets读取的'\n'
void Fgets_n(char *str)
{
	char *find;
	if ((find = strchr(str, '\n')))
		*find = '\0';
}


//初始化数组
void Initialize(char *temp_time_period, char *time_period)
{
	while (*temp_time_period != '\0')
		*time_period++ = *temp_time_period++ = '\0';
}