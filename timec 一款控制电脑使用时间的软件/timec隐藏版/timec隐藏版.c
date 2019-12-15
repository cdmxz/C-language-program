#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <direct.h>
#include <io.h>

#define SIZE 15

int  GetTime(void);      //比较当前时间和电脑能启动的时间
int  WriteTime(void);    //写入电脑关机时间和下次能启动时间
void ShutDown(void);	 //执行关机命令
int  Read_File(int *OffTime, int *BreakTime); //从配置文件读取电脑能运行的时间和休息时间
void FirstStart(void);	 //第一次启动自定义参数
void Flush(void);		 //清除缓冲区
void fgets_n(char *str); //清除fgets读取的'\n'

/*例如：电脑运行40分钟，要休息10分钟后才能继续运行。
假如当前时间为2019年11月23日11:20，我设定电脑运行40分钟就要休息10分钟。
也就是说要在当前时间的基础上加上50分钟后才能重新打开电脑。
因为电脑运行40分钟后关机，此时的时间是2019年11月23日12:00，再加上10分钟休息时间，
所以我要在2019年11月23日12:10后才能再次打开电脑否则就提示关机。*/

//数据文件路径
char time_dat_path[] = { "C:\\Program Files\\timec\\time.dat" };
char config_file_path[] = { "C:\\Users\\Timec_config.txt" };
char folder_path[] = { "C:\\Program Files\\timec" };

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
	
	/*system("shutdown -s -t 2400");//执行40分钟关机命令
	Sleep(2220000);//等待37分后提示用户还有3分钟就要关机
	MessageBox(NULL, TEXT("您的电脑还有3分钟就要关机了，\n请做好关机准备，如：保存好重要资料、退出游戏等。"), TEXT("温馨提示"), MB_OK | MB_ICONWARNING);
	Sleep(240000);//再等待4分后提示用户1分钟就要关机
	MessageBox(NULL, TEXT("您的电脑还有1分钟就要关机了，\n请做好关机准备，如：保存好重要资料、退出游戏等。"), TEXT("温馨提示"), MB_OK | MB_ICONWARNING);
	Sleep(50000);//再等待50秒后提示用户马上就要关机
	system("shutdown -s -t 10");
	MessageBox(NULL, TEXT("您的电脑还有10秒就要关机了。\n请站起来活动一下、放松一下眼睛、喝口水。\n等10分钟后再打开电脑吧！"), TEXT("温馨提示"), MB_OK);*/
	return 0;
}


//比较时间 
int GetTime(void)
{
	long temp;
	int year, mon, day, hour, min, i = 0, ch;
	char command[20] = { "shutdown -s -t " }, temp_time[SIZE], YEAR[SIZE] = { 0 }, MON[SIZE] = { 0 }, DAY[SIZE] = { 0 }, HOUR[SIZE] = { 0 }, MIN[SIZE] = { 0 };
	char time_pre[SIZE];//储存程序上次关闭时间的后10分钟（默认电脑运行40分钟休息10分钟的情况下）
	char time_now[SIZE];//储存当前时间
	char time_off[SIZE];//储存电脑关机时间
	char tip[100];//当电脑提前启动时输出关机提示
	FILE *fp;
	errno_t err;

	//获取当前时间
	struct tm timep, timenow, timeoff;
	time_t p, now, off;
	time(&p);
	localtime_s(&timep, &p);

	//写入当前时间到字符串“time_now”
	sprintf_s(time_now, SIZE, "%02d%02d%02d%02d%02d", timep.tm_year + 1900, timep.tm_mon + 1, timep.tm_mday, timep.tm_hour, timep.tm_min);

	//打开“time.dat”,并判断文件是否正常打开
	if ((err = fopen_s(&fp, time_dat_path, "r")) != 0)
		return 1;//如果打开失败就返回主函数执行40分钟关机命令
	if ((ch = fgetc(fp)) == EOF)
	{
		fclose(fp);
		return 1;
	}
	while (i++ < 4)
	{
		if (i == 1)
			//从“time.dat”读取电脑上次关闭后xx分钟时间，xx分钟指的是休息时间         
			fgets(time_pre, SIZE, fp);
		else if(i==2)
			//从“time.dat”读取第二行的内容也就是关机提示的内容
			fgets(tip, 100, fp);  
		else if(i==3)
			//从“time.dat”读取电脑上次关闭时间      
			fgets(time_off, SIZE, fp);
	}
	fclose(fp); 
	//清除fgets读取的换行符
	fgets_n(time_pre);
	fgets_n(tip);

	//判断当前时间是否小于电脑关机时间
	if (strcmp(time_off, time_now) > 0)
	{//如果小于就算出当前时间距离关机时间还有多少分
		for (i = 0; i <= 3; i++)
			YEAR[i] = time_off[i];
		for (i = 4; i <= 5; i++)
			MON[i-4] = time_off[i];
		for (i = 6; i <= 7; i++)
			DAY[i-6] = time_off[i];
		for (i = 8; i <= 9; i++)
			HOUR[i-8] = time_off[i];
		for (i = 10; i <= 11; i++)
			MIN[i-10] = time_off[i];
		sscanf_s(YEAR, "%d", &year);
		sscanf_s(MON, "%d", &mon);
		sscanf_s(DAY, "%d", &day);
		sscanf_s(HOUR, "%d", &hour);
		sscanf_s(MIN, "%d", &min);
		timeoff.tm_year = year - 1900;
		timeoff.tm_mon = mon - 1;
		timeoff.tm_mday = day;
		timeoff.tm_hour = hour - 1;
		timeoff.tm_min = min - 1;
		for (i = 0; i <= 3; i++)
			YEAR[i] = time_now[i];
		for (i = 4; i <= 5; i++)
			MON[i - 4] = time_now[i];
		for (i = 6; i <= 7; i++)
			DAY[i - 6] = time_now[i];
		for (i = 8; i <= 9; i++)
			HOUR[i - 8] = time_now[i];
		for (i = 10; i <= 11; i++)
			MIN[i - 10] = time_now[i];
		sscanf_s(YEAR, "%d", &year);
		sscanf_s(MON, "%d", &mon);
		sscanf_s(DAY, "%d", &day);
		sscanf_s(HOUR, "%d", &hour);
		sscanf_s(MIN, "%d", &min);
		timenow.tm_year = year - 1900;
		timenow.tm_mon = mon - 1;
		timenow.tm_mday = day;
		timenow.tm_hour = hour - 1;
		timenow.tm_min = min - 1;
		//算出当前时间距离关机时间还有多少秒
		now = mktime(&timenow);
		off = mktime(&timeoff);
		temp = (long)(off - now);
		sprintf_s(temp_time, SIZE, "%ld", temp);
		strcat_s(command, 20, temp_time);
		system(command);//执行关机命令
		exit(EXIT_SUCCESS);
	}
	//否则判断当前时间是否小于电脑能启动的时间
	else if (strcmp(time_pre, time_now) > 0)
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
	int i = 0;
	Read_File(&run_time, &break_time);

	//打开文件,并且判断文件是否正常打开
	if ((err = fopen_s(&fp, time_dat_path, "w")) != 0)
		return 1;

	//获取当前时间
	time_t pre_time;
	struct tm p;
	time(&pre_time);
	localtime_s(&p, &pre_time);
	min = p.tm_min + run_time + break_time;//在当前分钟的基础上加上xx分钟的运行时间和xx分钟休息时间
	hour = p.tm_hour;
	day = p.tm_mday;
	mon = p.tm_mon + 1;
	year = p.tm_year + 1900;

	while (i++ < 3)
	{
		if (i == 2)
		{
			min = p.tm_min + run_time;
			hour = p.tm_hour;
			day = p.tm_mday;
			mon = p.tm_mon + 1;
			year = p.tm_year + 1900;
		}
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

		if (i == 1)
		{
			//向文件写入电脑下次能运行时间
			fprintf(fp, "%02d%02d%02d%02d%02d\n", year, mon, day, hour, min);
			//向文件写入提示信息
			fprintf(fp, "当前为休息时间,下次此电脑允许启动时间为：%02d年%02d月%02d日%02d时%02d分后。    电脑即将在10秒内关机！\n", year, mon, day, hour, min);
		}
		else if (i == 2)
			fprintf(fp, "%02d%02d%02d%02d%02d", year, mon, day, hour, min);
	}
	fclose(fp);
	return 0;
}


//执行关机命令
void ShutDown(void)
{
	int  off_time, tmp;
	char command[22] = { "shutdown -s -t " };
	char temp[7];

	//从配置文件读取关机时间
	Read_File(&off_time, &tmp);
	off_time *= 60;//把分转换为秒

	sprintf_s(temp, 7, "%d", off_time);
	strcat_s(command, 22, temp);
	system(command);
	system("pause");
}


//从配置文件读取电脑能运行的时间以及休息时间
int Read_File(int *RunTime, int *BreakTime)
{
	int ch, i = 0, j = 0;
	int run_time = 40;	//默认能运行时间
	int break_time = 10;//默认休息时间
	char config[5];
	FILE *fp;
	errno_t err;

	//从配置文件读取内容
	if ((err = fopen_s(&fp, config_file_path, "r")) != 0)//判断文件是否正常打开
	{//打开失败就返回默认时间
		*RunTime = run_time;
		*BreakTime = break_time;
		return 1;
	}
	if ((ch = fgetc(fp)) != EOF)  //判断文件是否为空
		do
		{
			ch = fgetc(fp);
			if (ch == '=')        //判断ch是否为“=”
				while ((ch = fgetc(fp)) != ';')
				{
					if (ch == ' ')//丢弃读取的空格
						continue;
					config[i] = ch;
					i++;
				}
			if (ch == '\n')       //判断是否读到下一行
			{
				if (j == 0)       //判断是否读到第二行
				{
					sscanf_s(config, "%d", &run_time); //把第一行的内容保存到run_time，第一行的内容是电脑能运行的时间
					for (i = 0; i < 5; i++)
						config[i] = '\0';//初始化数组
				}
				j++;
				i = 0;
			}
		} while (ch != EOF);
		sscanf_s(config, "%d", &break_time); //把第二行的内容保存到break_time，第二行的内容是电脑关机后休息的时间
		fclose(fp);
		*RunTime = run_time;
		*BreakTime = break_time;
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

	int ch, i = 5;
	char content[200] = { "run_time = " };
	char path[MAX_PATH],run_time[5], break_time[5];
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
	system("cls");
	printf("\n温馨提示：如需重新设定时间，请编辑“C:\\Users\\Timec_config.txt”。");
	printf("\n\n\n请输入能运行的时间\n（也就是电脑运行XX分钟后强制关机。单位：分钟）：");
	scanf_s("%s", run_time, 5); 
	Flush();
	printf("\n\n\n请输入休息时间\n（也就是电脑强制关机后需要在XX分钟后才能使用。单位：分钟）：");
	scanf_s("%s", break_time, 5);
	Flush();
	
	//创建数据文件
	strcat_s(content, 200, run_time);
	strcat_s(content, 200, " ;     电脑能运行的时间。\nbreak_time = ");
	strcat_s(content, 200, break_time);
	strcat_s(content, 200, " ;  休息的时间。\n\n注意：两项值的单位都为分，值范围5~999。\n更改请保留原格式，如果出现程序不能正常运行，请删除此文件。");
	if ((err = fopen_s(&fpwrite, config_file_path, "w")) != 0)
	{
		remove(folder_path);
		perror("\n创建配置文件失败：");
		system("pause");
		exit(EXIT_FAILURE);
	}
	fputs(content, fpwrite);
	fclose(fpwrite);

	//复制文件
	system("cls && title 正在添加自启动，请退出杀软");
	printf("\n\n\n\n\n注意：正在添加自启动，若遭到杀软拦截，请点击允许并加入白名单。\n\n\t        否则软件无法正常启动！\n\n\n");
	Sleep(3000);

	GetModuleFileName(NULL, path, 100); //调用windows api获得“timec隐藏版.exe”的路径
	
	if ((err = fopen_s(&fpread, path, "rb")) != 0)
	{
		remove(folder_path);
		perror("复制文件失败：");
		system("pause");
		exit(EXIT_FAILURE);
	}

	if ((err = fopen_s(&fpwrite, timec_path, "wb")) != 0)
	{
		remove(folder_path);
		perror("复制文件失败：");
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
		perror("创建数据文件失败：");
		system("pause");
		exit(EXIT_FAILURE);
	}
	fclose(fpwrite);

	//打开注册表添加启动项 
	if (RegOpenKeyEx(HKEY_CURRENT_USER, SubKey, 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)
	{  //添加一个子Key,并设置值
		RegSetValueEx(hKey, "timecyincang", 0, REG_SZ, (BYTE *)timec_path, strlen(timec_path));
		RegCloseKey(hKey);//关闭注册表

		i = MessageBox(NULL, TEXT("添加自启动成功，重启后生效。\n如需取消，请在timec自定义版删除。\n是否现在重启？\n温馨提示：重启前请保存好数据资料！"), TEXT("添加自启动成功！"), MB_YESNO | MB_ICONQUESTION);
		if (i == IDYES)
			system("shutdown -r -t 00");
		else if (i == IDNO)
			exit(EXIT_SUCCESS);
	}
	else
	{
		MessageBox(NULL, TEXT("添加自启动失败！请以管理员权限运行本软件重试！"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
		remove(folder_path);
		exit(EXIT_SUCCESS);
	}
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
void fgets_n(char *str)
{
	char *find;
	if ((find = strchr(str, '\n')))
		*find = '\0';
}