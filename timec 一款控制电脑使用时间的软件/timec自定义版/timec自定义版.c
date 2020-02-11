#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <direct.h>
#include <io.h>

#define SIZE 6
#define MAX_SIZE 600

int  password(void);          //验证密码
void shut_down(void);         //自定义关机
void open_readme_txt(void);   //查看使用说明
void hwn(void);			      //隐藏窗口
void countdown(void);	      //弹出提示框
int  add_start(void);	      //添加自启动
void flush(void);		      //清除缓冲区
void fgets_n(char *str);      //清除fgets读取的'\n'
void Initialize(char *str1, char *str2, int n);//初始化数组
int  copyfile(char *fileread, char *filewrite);//复制文件
void del_start_password(void);//删除自启动验证密码

int main(void)
{
	int time;

	while (1)
	{
		system("color 07 && title timec自定义版 && mode con cols=50 lines=20");//设置颜色 标题 窗口大小

		printf("\t   ***欢迎使用timec自定义版***\n");
		printf("\t\t 0、取消关机\n");
		printf("\t\t 1、10分钟后关机\n");
		printf("\t\t 2、30分钟后关机\n");
		printf("\t\t 3、50分钟后关机\n");
		printf("\t\t 4、60分钟后关机\n");
		printf("\t\t 5、80分钟后关机\n");
		printf("\t\t 6、100分钟后关机\n");
		printf("\t\t 7、自定义关机时间\n");
		printf("\t\t 8、查看使用说明 \n");
		printf("\t\t 9、添加自启动\n");
		printf("\t\t 10、退出程序\n\n");
		//printf("温馨提示：输入序号后请按回车。\n");
		printf("\n\t请输入序号：");

		while (scanf_s("%d", &time) != 1)//处理字符输入
		{
			printf("\n输入错误！请重试：");
			flush();//清空缓冲区残余字符
		}
		flush();


		switch (time)
		{
		case 0:password(); break;
		case 1:system("shutdown -s -t 600"); hwn(); countdown(); break;
		case 2:system("shutdown -s -t 1800"); hwn(); Sleep(1200000); countdown(); break;
		case 3:system("shutdown -s -t 3000"); hwn(); Sleep(2400000); countdown(); break;
		case 4:system("shutdown -s -t 3600"); hwn(); Sleep(3000000); countdown(); break;
		case 5:system("shutdown -s -t 4800"); hwn(); Sleep(4200000); countdown(); break;
		case 6:system("shutdown -s -t 6000"); hwn(); Sleep(5400000); countdown(); break;
		case 7:shut_down(); break;
		case 8:open_readme_txt(); break;
		case 9:add_start(); break;
		case 10:exit(EXIT_SUCCESS); break;
		}
	}

	return 0;
}



int password(void)//密码验证
{
	system("title 如忘记密码，请输入1234修改。 && cls");

	int i;
	FILE *fpwrite, *fpread;
	errno_t err;
	char password[] = { "123456" };//储存默认密码
	char new_password[7] = { 0 };  //储存新密码
	char temp_password[7] = { 0 }; //临时储存密码


	printf("       温馨提示：如忘记密码，请输入1234修改\n\n\n\n\n\n");
	printf("如要取消关机，请输入密码：");

	scanf_s("%s", temp_password, 7);//输入当前密码
	flush();


	if (!strcmp(temp_password, "1234"))//将输入的密码进行比较
	{
		system("title 默认密码可在介绍文件中查找 && cls");

		printf("       温馨提示：默认密码可在介绍文件中查找\n\n\n\n\n\n");
		printf("请输入默认密码（6位数）：");

		scanf_s("%s", &temp_password, 7);//输入默认密码
		flush();


		if (!strcmp(temp_password, password))//将输入的密码与默认密码进行比较
		{
			while (1)
			{
				printf("\n\n请输入新密码（6位数）：");

				scanf_s("%s", &new_password, 7);//输入新密码
				flush();


				if (strlen(new_password) < SIZE)//判断新密码长度
					printf("\n\n输入的密码太长或太短（需要6位数字）。");
				else
					break;
			}

			if ((err = fopen_s(&fpwrite, "Password.dat", "w")) != 0)//打开记录密码的文件,并且判断文件是否正常打开
			{
				MessageBox(NULL, TEXT("密码文件创建错误！\n请以管理员权限运行重试！"), TEXT("ERROR"), MB_OK | MB_ICONERROR);//弹出提示框
				exit(EXIT_FAILURE);
			}

			for (i = 0; i <= 6; i++)  //把新密码写入文件
			{
				new_password[i] = new_password[i] - 't' + 'i' - 'm' / 'e';//加密文件内容
				fputc(new_password[i], fpwrite);//输出加密后的内容到文件
			}

			if (fclose(fpwrite) == EOF)//关闭文件
			{
				MessageBox(NULL, TEXT("文件写入错误！\n请检查是否有其它程序占用！\n或者重试！"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
				return 1;
			}
		}
		else
		{
			printf("\n\n\t\t密码错误！");
			Sleep(1000);
		}

	}
	else//取消关机
	{
		if ((err = fopen_s(&fpread, "Password.dat", "r")) != 0)//打开记录密码的文件,并且判断文件是否正常打开
		{
			MessageBox(NULL, TEXT("密码文件读取错误！\n请检查本程序目录是否有“Password.dat”这个文件。如果没有，请修改密码。"), TEXT("ERROR"), MB_OK | MB_ICONERROR);//弹出提示框
			return 1;
		}


		for (i = 0; i <= 6; i++)  //从文件中读取内容到ch。EOF是文件结束标志。
		{
			new_password[i] = fgetc(fpread);//从文件读取密码
			new_password[i] = new_password[i] + 't' - 'i' + 'm' * 'e';//解密文件内容并存到数组
		}

		if (!strcmp(temp_password, new_password))//将输入的密码与数组中的密码进行比较
			system("shutdown -a");
		else
		{
			printf("\n\n\t\t密码错误！");
			Sleep(1000);
		}
		fclose(fpread); //关闭文件

	}
	return 0;
}



//查看使用说明
void open_readme_txt(void)
{
	char path[MAX_PATH];

	_getcwd(path, MAX_PATH); //获取文件路径
	strcat_s(path, MAX_PATH, "\\readme.txt");
	if (_access(path, 0))//判断“readme.txt”在是否在本目录程序。不存在则打开网址，存在则直接打开文件
		system("start https://github.com/cdmxz/C-language-program/blob/master/timec%20%E4%B8%80%E6%AC%BE%E6%8E%A7%E5%88%B6%E7%94%B5%E8%84%91%E4%BD%BF%E7%94%A8%E6%97%B6%E9%97%B4%E7%9A%84%E8%BD%AF%E4%BB%B6/readme.txt");
	else
		system("start readme.txt");
}



void hwn(void)//隐藏窗口
{
	HWND hwnd;
	hwnd = FindWindow("ConsoleWindowClass", NULL);
	if (hwnd)
		ShowWindow(hwnd, SW_HIDE);
}


void countdown(void)
{
	MessageBox(NULL, TEXT("您的电脑还有10分钟就要关机了\n请做好关机准备！"), TEXT("温馨提示"), MB_OK | MB_ICONWARNING);
}



//自定义关机
void shut_down(void)
{
	char shut[25] = { "shutdown -s -t " };
	char temp[7] = { 0 };
	system("cls");
	printf("\n注意：目前自定义时间关机功能不会在关机前弹出提示框,");
	printf("请注意查看系统提示，并且最大时间不能超过99999秒。\n\n\n\n\n");
	printf("温馨提示：只需输入数字即可，不必输入单位\n");
	printf("\t  请输入多少秒后关机（单位：秒）：");

	scanf_s("%s", temp, 7);
	flush();

	strcat_s(shut, 25, temp);//合并数组

	system(shut);//执行关机命令
	Sleep(2000);
	exit(EXIT_SUCCESS);
}



//添加注册表
int add_start(void)
{
	system("cls && title 添加自启动 PS：输入7返回主界面");

	char *szSubKey = { "Software\\Microsoft\\Windows\\CurrentVersion\\Run" }; //系统启动项路径
	char folderName[MAX_PATH];//创建文件夹的路径
	char buffer[MAX_PATH];    //储存当前程序路径
	char timec_Y[MAX_PATH];   
	char timec_Z[MAX_PATH];
	char config_file_path[MAX_PATH];
	char folder_path[MAX_PATH];
	char cmd[MAX_PATH];
	char SystemRoot[SIZE];      //储存系统盘盘符
	char PROGRAMFILES[MAX_PATH];//储存程序默认安装目录

	HKEY hKey;
	int i;
	char ch;


	_getcwd(buffer, MAX_PATH);//获取当前程序路径

	if (GetEnvironmentVariable("SYSTEMDRIVE", SystemRoot, SIZE) == 0)//获取系统盘盘符
		strcpy_s(SystemRoot, SIZE, "C:");//获取失败则默认为C:

	if (GetEnvironmentVariable("PROGRAMFILES", folderName, MAX_PATH) == 0)//从环境变量获取程序默认安装目录
	{//获取失败
		sprintf_s(PROGRAMFILES, MAX_PATH, "%s\\Program Files (x86)", SystemRoot);
		if (_access(PROGRAMFILES, 0))//判断默认安装目录“Program Files (x86)”是否存在
			sprintf_s(folderName, MAX_PATH, "%s\\Program Files\\timec", SystemRoot);//不存在则使用“Program Files”目录
		else
			sprintf_s(folderName, MAX_PATH, "%s\\timec", PROGRAMFILES);//存在则使用“Program Files (x86)”目录
	}
	else//获取成功
		strcat_s(folderName, MAX_PATH, "\\timec");

	sprintf_s(timec_Y, MAX_PATH, "%s\\timec隐藏版.exe", folderName);    //注册表自启动路径以及复制的目标文件的路径
	sprintf_s(timec_Z, MAX_PATH, "%s\\timec自定义版.exe", folderName);  //注册表自启动路径以及复制的目标文件的路径
	sprintf_s(cmd, MAX_PATH, "attrib +s +h \"%s\" ", folderName);
	sprintf_s(config_file_path, MAX_PATH, "%s\\Timec_config.txt", SystemRoot);
	strcpy_s(folder_path, MAX_PATH, folderName);


	printf("注意：添加或删除自启动前请以管理员权限运行本软件。\n\n");
	printf("\t      1、添加timec隐藏版自启动\n\n");
	printf("\t      2、添加timec自定义版自启动\n\n");
	printf("\t      3、查看使用帮助\n\n");
	printf("\t      4、删除自启动\n\n");
	printf("\t      5、重新设置timec隐藏版配置文件\n\n");

	printf("    请输入序号：");

	while (scanf_s("%d", &i) != 1)//处理字符输入
	{
		printf("\n输入错误！请重试：");
		flush();
	}
	flush();


	switch (i)
	{
	case 1:
	{
		system("taskkill /f /t /im timec隐藏版.exe");//在复制文件前结束timec隐藏版.exe的进程，避免失败
		system("cls");

		printf("\n\n\n\n注意：若遭到杀软拦截，请点击允许并加入白名单。\n\n否则软件无法正常启动！\n\n\n");
		Sleep(3000);

		if (_access(folderName, 0))//判断文件夹是否存在
			if (_mkdir(folderName))	   //不存在则创建文件夹
			{
				MessageBox(NULL, TEXT("创建目录失败！请以管理员权限重试！"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
				exit(EXIT_FAILURE);
			}

		strcat_s(buffer, MAX_PATH, "\\timec隐藏版.exe");//复制前的路径
		copyfile(buffer, timec_Y);//复制文件

		system(cmd);//设置为隐藏文件

		//打开注册表启动项 
		if (RegOpenKeyEx(HKEY_CURRENT_USER, szSubKey, 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)
		{  //添加一个子Key,并设置值
			RegSetValueEx(hKey, "timec隐藏版", 0, REG_SZ, (BYTE *)timec_Y, strlen(timec_Y));
			RegCloseKey(hKey);//关闭注册表

			//printf("\t\t    添加成功！\n\n\t         ");
			printf("\t\t    添加成功！\n\n");
			printf("是否现在打开timec隐藏版（y/n）？\n");
			scanf_s("%c", &ch, 1);
			flush();
			if (ch == 'y' || ch == 'Y')
			{
				sprintf_s(buffer, MAX_PATH, " \"%s\" ", timec_Y);
				system(timec_Y);
			}
			system("pause");
			return 0;
		}
		else
		{
			MessageBox(NULL, TEXT("添加失败！"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
			remove(folderName);
			return 1;
		}
		break;
	}

	case 2:
	{
		//此处和上处源代码基本相同，故不写注释。其实还是懒啦~
		system("cls");

		printf("\n\n\n\n注意：若遭到杀软拦截，请点击允许并加入白名单，\n\n否则软件无法正常启动！\n\n\n");
		Sleep(3000);

		if (_access(folderName, 0))//判断文件夹是否存在
			if (_mkdir(folderName))			//不存在则创建文件夹
			{
				MessageBox(NULL, TEXT("创建目录失败！请以管理员权限重试！"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
				exit(EXIT_FAILURE);
			}

		system(cmd);//设置为隐藏文件

		strcat_s(buffer, MAX_PATH, "\\timec自定义版.exe");//复制前的路径
		copyfile(buffer, timec_Z);//复制文件

		if (RegOpenKeyEx(HKEY_CURRENT_USER, szSubKey, 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)
		{
			RegSetValueEx(hKey, "timec自定义版", 0, REG_SZ, (BYTE *)timec_Z, strlen(timec_Z));
			RegCloseKey(hKey);

			printf("\t\t    添加成功！\n\n\t         ");
			system("pause");
			return 0;
		}
		else
		{
			MessageBox(NULL, TEXT("添加失败！"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
			remove(folderName);
			return 1;
		}
		break;
	}

	case 3:
		MessageBox(NULL, TEXT("1、设置开机自动运行前请以管理员权限运行本软件\n防止权限不够造成失败。\n2、设置开机自动运行前请确保“timec隐藏版”和\n“timec自定义版”在同一目录。\n3、设置开机自动运行前请确保“timec隐藏版”和\n“timec自定义版”的原文件名未被修改。\n4、如果开机自启失效，请查看“C:\\Program Files\\time”目录下有没有\n“timec自定义版 ”或“timec隐藏版”这个软件\n或者检查软件自启权限是否被杀软禁掉了。\n5、在当前页面输入4删除自启动。\n6、开启自启动会对修改系统注册表，\n可能会造成电脑不稳定，请慎用！\n7、使用本软件产生的任何后果均不由原作者承担\n。一经使用本软件，视为同意此条款。\n注：本条款若有任何更新，恕不另行通知！"), TEXT("使用帮助"), MB_OK);
		break;

	case 4://从文件夹删除文件
	{
		del_start_password();//验证密码
		system("cls");
		printf("\n\n\n\n\n\n注意：1、请以管理员权限打开此软件！\n      2、若遭到杀软拦截，请点击允许并加入白名单！\n\n\n");
		Sleep(3000);                     
		system("cls && mode con cols=53 lines=30 && title 正在删除...");

		//结束进程
		printf("\n正在结束timec隐藏版的进程...\n");
		Sleep(2000);
		system("taskkill /f /t /im timec隐藏版.exe");

		//删除程序
		printf("\n\n正在删除文件...");
		if (remove(timec_Y))
			printf("\n删除timec隐藏版失败！\n如未添加timec隐藏版自启动可不理会。\n\n");
		else
			printf("\n删除timec隐藏版成功！\n\n");

		if (remove(timec_Z))
			printf("删除timec自定义版失败！\n如未添加timec自定义版自启动可不理会。\n\n");
		else
			printf("删除timec自定义版成功！\n\n");

		//删除配置文件
		printf("\n正在删除配置文件...\n");
		if (remove("C:\\Timec_config.txt"))
			printf("删除配置文件失败！\n请手动进入C盘根目录删除“Timec_config.txt”。\n\n");
		else
			printf("删除配置文件成功！\n\n");

		//删除目录
		printf("\n正在删除安装目录...\n");
		sprintf_s(cmd, MAX_PATH, "rd /s /q \"%s\"", folderName);
		system(cmd);

		//删除注册表
		printf("\n\n正在删除timec隐藏版注册表启动项...\n");
		system("reg delete \"HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run\"  /v timec隐藏版 /f");
		printf("\n正在删除timec自定义版注册表启动项...\n");
		system("reg delete \"HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run\"  /v timec自定义版 /f");

		putchar('\n');
		system("pause");
		break;
	}

	case 5://重新设置配置文件
	{

		system("title 重新设置timec隐藏版配置文件 && mode con cols=72 lines=17");
	
		char c;
		unsigned i, j = 0;
		int  front_hour, front_min, behind_hour, behind_min;
		char config_content[MAX_SIZE] = { 0 };				//储存将要写入配置文件的内容
		char temp_time_period[15] = { 0 }, time_period[15]; //储存能运行的时间段
		char run_time[5], break_time[5];					//储存能运行的时间和休息时间
		char date[100] = { 0 };								//储存电脑每个月能运行的日期

		FILE *fpwrite;
		errno_t err;



/***********************/
begin://goto语句标签
/***********************/

		system("cls && mode con cols=72 lines=25");

		printf("\n\n请输入电脑每天能运行的时间段（格式：xx:xx - xx:xx，请不要输入中文标点。\n温馨提示：输入00:00 - 00:00不限制运行时间段。）：");
		while (1)
		{
			Initialize(temp_time_period, time_period, 2);

			fgets(time_period, 15, stdin);
			rewind(stdin);
			fgets_n(time_period);	//清除fgets读取的'\n'

			for (i = 0; i < 15; i++)//把time_period的内容复制到temp_time_period
				temp_time_period[i] = time_period[i];

			//判断输入的格式是否正确
			if (strchr(date, '：') || strchr(date, '——'))
			{
				printf("\n请不要输入中文符号，请重新输入：");
				continue;
			}

			if (strlen(time_period) != 13 || time_period[6] != '-')//判断输入格式是xx:xx-xx:xx还是xx:xx - xx:xx
			{
				if (strlen(time_period) != 11 || time_period[5] != '-')
				{
					printf("\n格式错误（例如：08:00 - 14:00），请重试：");
					continue;
				}
			}

			for (i = 0; i < 15; i++)
			{
				if (time_period[i] == ':' || time_period[i] == '-')
					time_period[i] = ' ';
			}

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


		printf("\n\n请输入电脑每个月能运行的日期（例如：05。\n温馨提示：如果要输入多个日期请用英文逗号分隔开，例如：1,5,10,25,30。\n输入000不限制运行日期。）：");
		while (1)
		{
			Initialize(date, NULL, 1);
			fgets(date, 100, stdin);
			rewind(stdin);
			fgets_n(date);

			if (strchr(date, '，'))
			{
				printf("\n请不要输入中文符号，请重新输入：");
				continue;
			}
			if (strlen(date) <= 1)
			{
				printf("\n格式错误，请重新输入（例如：01,05,10）：");
				continue;
			}
			break;
		}


		printf("\n\n请输入电脑每次能运行的时间\n（也就是电脑每次运行XX分钟后强制关机。单位：分钟）：");
		while (1)
		{
			while (scanf_s("%d", &i) != 1)
			{
				printf("\n输入错误，请重试：");
				flush();
			}
			flush();
			if (i > 999)
			{
				printf("\n最大值不能超过999，请重新输入：");
				continue;
			}
			sprintf_s(run_time, 5, "%d", i);
			break;
		}


		printf("\n\n请输入电脑每次休息时间\n（也就是电脑每次强制关机后需要在XX分钟后才能使用。单位：分钟）：");
		while (1)
		{
			while (scanf_s("%d", &i) != 1)
			{
				printf("\n输入错误，请重试：");
				flush();
			}
			flush();
			if (i > 999)
			{
				printf("\n最大值不能超过999，请重新输入：");
				continue;
			}
			sprintf_s(break_time, 5, "%d", i);
			break;
		}

		printf("\n\n请输入当电脑在能运行的时间段外或在休息时间内启动时，电脑关机时间\n（单位：秒。最大值60秒）：");
		while (1)
		{
			while (scanf_s("%d", &i) != 1)
			{
				printf("\n输入错误，请重试：");
				flush();
			}
			flush();
			if (i > 60)
			{
				printf("\n最大值不能超过60秒，请重新输入：");
				continue;
			}
			break;
		}

		system("cls && mode con cols=47 lines=15");
		printf("当前设置的参数为：\n\n");

		if (strcmp(temp_time_period, "00:00-00:00") == 0)
			printf("电脑每天能运行的时间段： %s（不限制运行时间段）\n", temp_time_period);

		else if (strcmp(temp_time_period, "00:00 - 00:00") == 0)
			printf("电脑每天能运行的时间段： %s（不限制运行时间段）\n", temp_time_period);

		else
			printf("电脑每天能运行的时间段： %s", temp_time_period);

		if (strcmp(date, "00") == 0)
			printf("\n电脑每个月能运行的日期： %s（不限制运行日期）", date);
		else
			printf("\n电脑每个月能运行的日期： %s 号", date);

		printf("\n电脑每次能运行的时间：   %s 分", run_time);
		printf("\n电脑每次休息的时间：     %s 分", break_time);

		while (1)
		{
			printf("\n\n\t      是否确定？（Y/N）");
			printf("\n\t请输入：");
			scanf_s("%c", &c, 1);
			flush();

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

		sprintf_s(config_content, MAX_PATH, "电脑开机后能运行的时间 = %s ;\n"
			"电脑关机后强制休息的时间 = %s ;\n"
			"电脑每天能运行的时间段 = %s ;\n"
			"电脑每个月能运行的日期 = %s ;\n"
			"电脑在休息时间内启动时关机时间 = %d ;", run_time, break_time, temp_time_period, date, i);
		strcat_s(config_content, MAX_SIZE, "\n\n注意：\n"
			"1、“电脑开机后能运行的时间”和“电脑关机后强制休息的时间”两项值的单位都为分，值范围5~999。\n"
			"2、“电脑每天能运行的时间段”项值的格式为“xx:xx-xx:xx”，请不要输入中文标点（如：“：”）。\n"
			"3、每一项末尾的分号是英文分号，更改内容请保留原格式。\n"
			"4、如果出现程序不能正常运行，请在“timec自定义版”重新创建配置文件或添加“-reset”参数启动timec隐藏版。");

		//打开配置文件
		if ((err = fopen_s(&fpwrite, config_file_path, "w")) != 0)
		{
			remove(folder_path);
			perror("\n创建配置文件失败");
			system("pause");
			exit(EXIT_FAILURE);
		}
		fputs(config_content, fpwrite);//向配置文件输出内容
		fclose(fpwrite);
		
		printf("\n\t        设置成功！\n\t      ");
		system("pause");
		break;
	}

	case 7:break;

	default:printf("\n\t\t 输入错误！"); Sleep(1000); break;
	}
	return 0;
}


//清除缓冲区
void flush(void)
{
	int ch;
	while ((ch = getchar()) != '\n' && ch != EOF)
		;
}


//复制文件
int copyfile(char *fileread, char *filewrite)
{
	FILE *fpread, *fpwrite;
	errno_t err;
	int ch;


	if ((err = fopen_s(&fpread, fileread, "rb")) != 0)//以二进制方式打开文件
	{
		MessageBox(NULL, TEXT("无法复制文件！\n请检查“timec自定义版.exe”和“timec隐藏版.exe”是否在同一目录。\n"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
		exit(EXIT_FAILURE);
	}

	if ((err = fopen_s(&fpwrite, filewrite, "wb")) != 0)
	{
		//MessageBox(NULL, TEXT("无法复制文件！请以管理员权限重试！\n"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
		perror("Error");
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
	return 0;
}



void del_start_password(void)//删除自启动验证密码
{
	FILE *fpread;
	errno_t err;

	int i;
	char new_password[7] = { 0 };//储存新密码
	char temp_password[7] = { 0 };//临时储存密码

	while (1)
	{
		system("cls && title 删除自启动项需要输入正确的密码");

		printf("\n\t\t  输入1234退出。\n         如忘记密码，请在主界面输入0更改。");
		printf("\n\n\n删除自启动项需要输入密码。\n\n请输入正确的密码：");
		scanf_s("%s", temp_password, 7);
		flush();

		if (!strcmp(temp_password, "1234"))//输入1234退出
			exit(EXIT_SUCCESS);

		if ((err = fopen_s(&fpread, "Password.dat", "r")) != 0)//打开记录密码的文件,并且判断文件是否正常打开
		{
			MessageBox(NULL, TEXT("密码文件读取错误！\n请检查本程序目录是否有“Password.dat”这个文件。如果没有，请修改密码。"), TEXT("ERROR"), MB_OK | MB_ICONERROR);//弹出提示框
			exit(EXIT_SUCCESS);
		}

		for (i = 0; i <= 6; i++)  //从文件中读取内容到ch。EOF是文件结束标志。
		{
			new_password[i] = fgetc(fpread);//从文件读取密码
			new_password[i] = new_password[i] + 't' - 'i' + 'm' * 'e';//解密文件内容并存到数组
		}

		if (!strcmp(temp_password, new_password))//将输入的密码与数组中的密码进行比较
			break;
		else
		{
			printf("\n\n\t\t密码错误！");
			Sleep(1000);
		}

		fclose(fpread); //关闭文件
		system("cls");
	}

}


//清除fgets读取的'\n'
void fgets_n(char *str)
{
	char *find;
	if ((find = strchr(str, '\n')))
		*find = '\0';
}


//初始化数组
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