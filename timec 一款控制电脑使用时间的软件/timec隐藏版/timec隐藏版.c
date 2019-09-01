//隐藏程序窗口
#pragma comment (lib,"Urlmon.lib")
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )

#include <stdio.h>
#include <windows.h>
#include <time.h>
#define SIZE 13
int GetTime(void);//比较当前时间和程序下次能运行时间
void LastTime(void);//写入程序上次运行的时间

int main(void)
{
	system("title timec隐藏版");

	while (1)
	{
		GetTime();//比较当前时间和程序上次关闭后15分的时间
		LastTime();//写入程序下次关闭后15分的时间（也就是程序下次能运行时间）

		system("shutdown -s -t 3600");//执行1小时关机命令
		Sleep(3000000);//等待50分后提示用户10分钟就要关机
		MessageBox(NULL, TEXT("您的电脑还有10分钟就要关机了，\n请做好关机准备如：保存好重要资料、退出游戏等。"), TEXT("温馨提示"), MB_OK | MB_ICONWARNING);
		Sleep(420000);//再等待7分后提示用户3分钟就要关机
		MessageBox(NULL, TEXT("您的电脑还有3分钟就要关机了，\n请做好关机准备，如：保存好重要资料、退出游戏等。"), TEXT("温馨提示"), MB_OK | MB_ICONWARNING);
		Sleep(410000);//再等待6分50秒后提示用户马上就要关机
		MessageBox(NULL, TEXT("您的电脑还有10秒就要关机了。\n请站起来活动一下、放松一下眼睛、喝口水。\n等16分钟后再打开电脑吧！"), TEXT("温馨提示"), MB_OK | MB_ICONWARNING);
	}
	return 0;
}

int GetTime(void)//比较时间 
{
	int year, mon, day, hour, min;
	char time_pre[SIZE] = { 0 };//储存程序上次关闭时间后15分
	char time_now[SIZE] = { 0 };//储存当前时间
	//char defau[SIZE] = "201908041811";//时间默认值
	FILE *fpread_pre, *fpwrite, *fpread_now, *fp;
	errno_t err;

	if ((err = fopen_s(&fpread_pre, "time.dat", "r")) != 0) //以只读形式打开记录程序上次关闭时间的文件,并且判断文件是否正常打开
	{
		if (err = fopen_s(&fp, "time.dat", "w") != 0)//如果程序目录没有time.dat文件就重新创建
			return 0;//如果创建失败就返回主函数执行1小时关机命令
		//fputs(defau, fp);//向文件写入默认日期
		fclose(fp); //关闭文件
		return 0;
	}
	
	if ((err = fopen_s(&fpwrite, "nowtime.dat", "w")) != 0) //以只写形式打开记录当前时间的文件,并且判断文件是否正常打开
		return 0;//如果文件创建错误，就返回主函数

	//写入当前时间到文件
	struct tm t;
	time_t now;
	time(&now);
	localtime_s(&t, &now);
	fprintf(fpwrite, "%02d%02d%02d%02d%02d", t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min); //写入当前年月日时分到文件
	fclose(fpwrite); //关闭文件

	if ((err = fopen_s(&fpread_now, "nowtime.dat", "r")) != 0) //重新以只读形式打开记录当前时间的文件,并且判断文件是否正常打开
		return 0;//如果文件打开失败，就返回主函数

	fgets(time_pre, sizeof(time_now), fpread_pre);//从文件读取程序上次关闭时间到time_pre数组
	fgets(time_now, sizeof(time_now), fpread_now);//从文件读取当前时间到time_now数组
	fclose(fpread_now); //关闭文件
	fclose(fpread_pre); //关闭文件
	
	remove("nowtime.dat");//删除文件

	//将当前时间与程序下次能运行时间进行对比
	for (year = 0; year <= 3; year++)   //对比年份
	{
		if (time_pre[year] != time_now[year])
			return 0;
		//printf("%c %c\n", time_pre[year], time_now[year]);
	}
	for (mon = 4; mon <= 5; mon++)	//对比月份
	{
		if (time_pre[mon] != time_now[mon])
			return 0;
		//printf("%c %c\n", time_pre[mon], time_now[mon]);
	}
	for (day = 6; day <= 7; day++)	//对比天数 
	{
		if (time_pre[day] != time_now[day])
			return 0;
		//printf("%c %c\n", time_pre[day], time_now[day]);
	}
	for (hour = 8; hour <= 9; hour++)//对比小时 
	{
		if (time_pre[hour] < time_now[hour])
			return 0;
		//printf("%c %c\n", time_pre[hour], time_now[hour]);
	}
	for (min = 10; min <= 11; min++) //对比分钟 
	{
		if (time_pre[min] < time_now[min])
			return 0;
		//printf("%c %c\n", time_pre[min], time_now[min]);
	}
	MessageBox(NULL, TEXT("当前为休息时间。\n电脑即将在1分内关机！"), TEXT("警告！"), MB_OK | MB_ICONWARNING);//弹出提示框
	system("shutdown -s -t 60");
	return 0;
}

void LastTime(void)//写入程序关闭后15分的时间，也就是在当前时间的基础上加1小时15分钟并写入到文件
{
	while (1)
	{
		FILE *fpwrite;
		errno_t err;
		int min, hour, day, mon, year;

		if ((err = fopen_s(&fpwrite, "time.dat", "w")) != 0)//打开文件,并且判断文件是否正常打开
			break;
		time_t pre_time;						//获取程序下次关闭时间
		struct tm t;
		time(&pre_time);
		localtime_s(&t, &pre_time);
		min = t.tm_min + 15;//在当前分钟的基础上加15分钟（即关闭电脑后，15分钟之内不能打开电脑）
		hour = t.tm_hour + 1;//在当前小时的基础上加1小时（因为已设定1小时后关机，也就是在1小时后关闭本程序）
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
