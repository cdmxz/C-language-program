#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <direct.h>
#include <io.h>

#define SIZE 13

int  GetTime(void);      //比较当前时间和电脑能启动的时间
void LastTime(void);     //写入电脑下次能启动时间
void ShutDown(void);	 //执行关机命令
int  read_file(void);	 //从配置文件读取电脑运行的时间和休息时间

/*例如：电脑运行40分钟，要休息10分钟后才能继续运行。
假如当前时间为2019年11月23日11:20，我设定电脑运行40分钟就要休息10分钟。
也就是说要在当前时间的基础上加上50分钟后才能重新打开电脑。
因为电脑运行40分钟后关机，此时的时间是2019年11月23日12:00，再加上10分钟休息时间，
所以我要在2019年11月23日12:10后才能再次打开电脑否则就提示关机。*/

//数据文件路径
char time_path[] = { "C:\\Program Files\\timec\\time.dat" };
char config_file[] = { "C:\\Users\\Timec_config.txt" };


int main(void)
{
	//隐藏程序运行窗口
	HWND hwnd;
	hwnd = FindWindow("ConsoleWindowClass", NULL);
	if (hwnd)
		ShowWindow(hwnd, SW_HIDE);

	char folder_name[] = { "C:\\Program Files\\timec" };
	//创建数据文件夹
	if (_access(folder_name, 0)) //判断文件夹是否存在
		_mkdir(folder_name);	 //不存在则创建

	GetTime();
	LastTime();
	ShutDown();

	//调用windows api隐藏文件
	SetFileAttributes(time_path, FILE_ATTRIBUTE_HIDDEN);
	SetFileAttributes(folder_name, FILE_ATTRIBUTE_HIDDEN);

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
	char time_pre[SIZE];//储存程序上次关闭时间的后10分钟（默认电脑运行40分钟休息10分钟的情况下）
	char time_now[SIZE];//储存当前时间
	char tip[100];//当电脑提前启动时输出关机提示
	FILE *fp;
	errno_t err;

	//获取当前时间
	struct tm *p;
	time_t now;
	time(&now);
	p = localtime(&now);

	//写入当前时间到字符串“time_now”
	sprintf_s(time_now, SIZE, "%02d%02d%02d%02d%02d", p->tm_year + 1900, p->tm_mon + 1, p->tm_mday, p->tm_hour, p->tm_min);
	
	//打开“time.dat”,并判断文件是否正常打开
	if ((err = fopen_s(&fp, time_path, "r")) != 0)
	{
		//如果程序目录没有“time.dat”这个文件就重新创建
		if (err = fopen_s(&fp, time_path, "w") != 0)
			return 1;//如果创建失败就返回主函数执行40分钟关机命令
		fclose(fp);
		return 0;
	}
	//从文件读取程序上次关闭时间到time_pre数组         
	fgets(time_pre, SIZE, fp);
	//关闭文件
	fclose(fp); 
	//判断当前时间是否小于电脑能启动的时间
	if (strcmp(time_pre, time_now) > 0)
	{
		//如果当前时间小于电脑能启动的时间就执行10秒关机命令并且输出关机提示
		system("shutdown -s -t 10");

		//打开文件，从文件读取关机提示的内容
		if (err = fopen_s(&fp, time_path, "r") != 0)
		{//如果打开文件失败就直接输出关机提示
			MessageBox(NULL, TEXT("当前为休息时间，电脑即将在10秒内关机！"), TEXT("警告！"), MB_OK | MB_ICONWARNING);//弹出关机提示
			exit(EXIT_FAILURE);
		}
		
		while (!feof(fp))
			fgets(tip, 100, fp);  //读取“time.dat”第二行的内容也就是关机提示的内容
		fclose(fp);

		//弹出关机提示
		MessageBox(NULL, (tip), TEXT("警告！"), MB_OK | MB_ICONWARNING);
		exit(EXIT_FAILURE);
	}
	return 0;
}


/*写入电脑关闭后10分的时间，也就是在当前时间的基础上加50分钟。 
即电脑下次能启动时间。（默认电脑运行40分钟休息10分钟的情况下）*/
void LastTime(void)
{
	while (1)
	{
		FILE *fp;
		errno_t err;
		int min, hour, day, mon, year;
		int off_time;

		off_time = read_file();

		//获取当前时间
		time_t pre_time;
		struct tm *p;
		time(&pre_time);
		p = localtime(&pre_time);
		min = p->tm_min + off_time;/*在当前分钟的基础上加50分钟（即40分钟后关闭电脑+10分钟休息时间）
		（默认电脑运行40分钟休息10分钟的情况下）*/
		hour = p->tm_hour;
		day = p->tm_mday;
		mon = p->tm_mon + 1;
		year = p->tm_year + 1900;

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
			{
				day -= 30;
				mon++;
			}
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

		//打开文件,并且判断文件是否正常打开
		if ((err = fopen_s(&fp, time_path, "w")) != 0)
			break;

		//向文件写入电脑下次能运行时间
		fprintf(fp, "%02d%02d%02d%02d%02d\n", year, mon, day, hour, min);
		//向文件写入提示信息
		fprintf(fp, "当前为休息时间,下次此电脑允许启动时间为：%02d年%02d月%02d日%02d时%02d分。    电脑即将在10秒内关机！", year, mon, day, hour, min);   //写入年月日小时分钟
		fclose(fp);

		printf("下次此电脑允许启动时间为：%02d年%02d月%02d日%02d时%02d分。\n", year, mon, day, hour, min);
		break;
	}
}


//执行关机命令
void ShutDown(void)
{
	int  ch, i = 0;
	int off_time = 40;
	char command[22] = { "shutdown -s -t " };
	char tmp[5];
	FILE *fp;
	errno_t err;

	//从配置文件读取内容
	if (!_access(config_file, 0)) //判断文件是否存在
		if (!(err = fopen_s(&fp, config_file, "r")))//判断文件是否正常打开
		{
			if ((ch = fgetc(fp)) != EOF)//判断文件是否为空
				fscanf_s(fp, "%*s %*s %d", &off_time);
			fclose(fp);
		}

	printf("将在%d分（", off_time);
	off_time *= 60;//把分转换为秒
	printf("%d秒）后关机。\n\n", off_time);
	sprintf_s(tmp, 5, "%d", off_time);
	strcat_s(command, 22, tmp);
	system(command);
	system("pause");
}


//获取电脑运行的时间以及休息时间
int read_file(void)
{
	int  ch, i = 0, j = 1;
	int off_time = 40;	//默认运行时间
	int break_time = 10;//默认休息时间
	char config[5] = { 0 };
	FILE *fp;
	errno_t err;

	//从配置文件读取内容
	if (!_access(config_file, 0)) //判断文件是否存在
	{
		if ((err = fopen_s(&fp, config_file, "r")) != 0)//判断文件是否正常打开
			return 	off_time += break_time;

		if ((ch = fgetc(fp)) != EOF)//判断文件是否为空
			do
			{
				ch = fgetc(fp);
				if (ch == '=')//判断ch是否为“=”
					while ((ch = fgetc(fp)) != ';')
					{
						if (ch == ' ')//丢弃读取的空格
							continue;
						config[i] = ch;
						i++;
					}
				if (ch == '\n')//判断是否读到第二行
				{
					i = 0;
					if (j < 2)
						sscanf_s(config, "%d", &off_time);//把第一行的内容保存到off_time，第一行的内容是电脑能运行的时间
					j++;
				}
			} while (ch != EOF);
			sscanf_s(config, "%d", &break_time);//把第二行的内容保存到break_time，第二行的内容是电脑关机后休息的时间
			fclose(fp);
	}
	else
	{
		if (!(err = fopen_s(&fp, config_file, "w")))
		{
			fprintf_s(fp, "off_time = 40 ;     电脑能运行的时间。\nbreak_time = 10 ;  休息的时间。\n\n注意：两项值的单位都为分，值范围5~999。\n更改请保留原格式，如果出现程序不能正常运行，请删除此文件。");
			fclose(fp);
		}
	}

	printf("能运行时间 = %d分, 休息时间 = %d分, ", off_time, break_time);
	off_time += break_time;
	printf("总计 = %d分。\n", off_time);

	return off_time;
}