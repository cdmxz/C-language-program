//隐藏程序窗口
#pragma comment (lib,"Urlmon.lib")
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )

#include <stdio.h>
#include <windows.h>
#include <time.h>
#define SIZE 13

int GetTime(void);//比较当前时间和程序下次能运行时间
void LastTime(void);//写入程序上次运行的时间（也就是程序下次能运行时间）

int main(void)
{
	GetTime();
	LastTime();

	system("shutdown -s -t 2400");//执行40分钟关机命令
	Sleep(2220000);//等待37分后提示用户还有3分钟就要关机
	MessageBox(NULL, TEXT("您的电脑还有3分钟就要关机了，\n请做好关机准备，如：保存好重要资料、退出游戏等。"), TEXT("温馨提示"), MB_OK | MB_ICONWARNING);
	Sleep(240000);//再等待4分后提示用户1分钟就要关机
	MessageBox(NULL, TEXT("您的电脑还有1分钟就要关机了，\n请做好关机准备，如：保存好重要资料、退出游戏等。"), TEXT("温馨提示"), MB_OK | MB_ICONWARNING);
	Sleep(50000);//再等待50秒后提示用户马上就要关机
	//system("shutdown -s -t 10");
	MessageBox(NULL, TEXT("您的电脑还有10秒就要关机了。\n请站起来活动一下、放松一下眼睛、喝口水。\n等10分钟后再打开电脑吧！"), TEXT("温馨提示"), MB_OK);
	return 0;
}


//比较时间 
int GetTime(void)
{
	char time_pre[SIZE] = { 0 };//储存程序上次关闭时间的后10分钟
	char time_now[SIZE] = { 0 };//储存当前时间
	
	FILE *fp;
	errno_t err;

	//打开记录程序上次关闭时间的文件,并且判断文件是否正常打开
	if ((err = fopen_s(&fp, "time.dat", "r")) != 0) 
	{
		//如果程序目录没有time.dat文件就重新创建
		if (err = fopen_s(&fp, "time.dat", "w") != 0)
			return 0;//如果创建失败就返回主函数执行40分钟关机命令
		fclose(fp); 
		return 0;
	}

	
	//获取当前时间
	struct tm t;
	time_t now;
	time(&now);
	localtime_s(&t, &now);

	//写入当前年月日时分到字符串
	sprintf_s(time_now, SIZE, "%02d%02d%02d%02d%02d", t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min); 
	printf("下次此电脑允许启动时间：%s", time_now);

	//从文件读取程序上次关闭时间到time_pre数组
	fgets(time_pre, sizeof(time_now), fp);

	//关闭文件
	fclose(fp); 
	
	//判断当前时间是否小于预定时间
	if (strcmp(time_pre, time_now) > 0)
	{
		system("shutdown -s -t 30");
		MessageBox(NULL, TEXT("当前为休息时间。\n电脑即将在30秒内关机！"), TEXT("警告！"), MB_OK | MB_ICONWARNING);//弹出提示框
		exit(0);
	}
	return 0;
}

//写入程序关闭后10分的时间，也就是在当前时间的基础上加50分钟
void LastTime(void)
{
	while (1)
	{
		FILE *fpwrite;
		errno_t err;
		int min, hour, day, mon, year;


		//打开文件,并且判断文件是否正常打开
		if ((err = fopen_s(&fpwrite, "time.dat", "w")) != 0)
			break;
		
		//获取程序下次关闭时间
		time_t pre_time;						
		struct tm t;
		time(&pre_time);
		localtime_s(&t, &pre_time);
		min = t.tm_min + 50;//在当前分钟的基础上加50分钟（即40分钟后关闭电脑+10分钟休息时间）
		hour = t.tm_hour;
		day = t.tm_mday;
		mon = t.tm_mon + 1;
		year = t.tm_year + 1900;

		if (min >= 60)
		{
			min -= 60;
			hour++;
		}
		if (hour >= 24)
		{
			hour -= 24;
			day++;
		}
		if (mon == 1 || mon == 3 || mon == 5 || mon == 7 || mon == 8 || mon == 10 || mon == 12)//判断当前月份是否为大月
		{
			if (day > 31)
			{
				day -= 31;
				mon++;
			}
		}
		else if (mon == 4 || mon == 6 || mon == 9 || mon == 11)//判断当前月份是否为小月
		{
			if (day > 30)
				day -= 30;
			mon++;
		}
		else if (mon == 2)
		{
			if (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0))//判断当前年份是否为闰年
				day -= 29;//闰年2月有29天
			else
				day -= 28;//平年2月有28天
			mon++;
		}
		if (mon > 12)//判断当前月份是否超过12月
		{
			mon -= 12;
			year++;
		}
		//向文件写入程序下次能运行时间
		fprintf(fpwrite, "%02d%02d%02d%02d%02d", year, mon, day, hour, min);   //写入年月日小时分钟
		fclose(fpwrite);     //关闭文件
		break;
	}
}
