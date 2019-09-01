#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>
#include <time.h>
#include <conio.h>

int  GetTime(int n);

int main(void)
{
	char Pass[7] = "115-56";
	char TmpPass[7] = { 0 };
	int i, ch;

	system("title 本电脑需要输入正确的密码才能正常使用 && color 06 && mode con cols=91 lines=30 && shutdown -s -t 300");//设置窗口标题、字体颜色、窗口大小

	printf("\n\n\n\n\n\n\n\t\t         您需要输入正确的密码才能正常使用本电脑。\n\n\t\t PS：本电脑将在5分钟内关机，请按任意键进入输入密码界面。\n\n\n\n\n\n\t\t\t           ");
	system("pause");
	system("cls");


	printf("\n\n\n\n\n\t       温馨提示：您有60秒的时间输入正确的密码，并且您只有3次机会。\n");
	printf("         一旦超过60秒还未输入正确的密码或者输入3次错误的密码，电脑就会自动关机。\n\n\n\n\n");
	printf("注意：如果您急需使用本电脑可以联系我的QQ：2641015842 （备注：电脑密码） \n\t\t\t\t    微信：18318085358（备注：电脑密码） \n\t\t\t\t    电话：18318085358（发短信给我）");
	printf("\n\t\t\t\t    网站：http://fuhohua.web3v.com、https://cdmxz.github.io");
	printf("\n\n\n\n\t\t\t       按任意键打开网站获取密码\n\n\n\n\t\t\t    ");
	_getch();
	system("start http://fuhohua.web3v.com");
	printf("   按任意键进入输入密码界面");
	_getch();
	system("cls  && shutdown -a && shutdown -s -t 60");


	for (i = 3; i >= 1; i--)
	{
		printf("\n\t\t\t请在60秒内输入密码，否则电脑将会关机");
		printf("\n\n请输入密码（你还有%d次机会）：", i);
		scanf_s("%s", TmpPass, 7);
		while ((ch = getchar()) != '\n' && ch != EOF)//吸收多余字符和换行符
			;

		if (!strcmp(Pass, TmpPass))//对比密码是否正确
		{
			GetTime(0);
			system("shutdown -a");
			system("cls && color 02 && shutdown -s -t 2400");//清屏、设置字体颜色、设置40分后关机
			printf("\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t恭喜您，密码正确，您可以使用本电脑40分钟。\n\n\n\n\n\t\t\t\t   ");
			system("pause");
			//隐藏窗口
			HWND hwnd;
			hwnd = FindWindow("ConsoleWindowClass", NULL);
			if (hwnd)
				ShowWindow(hwnd, SW_HIDE);
			
			//倒计时
			Sleep(2100000);
			MessageBox(NULL, TEXT("本电脑还有5分钟就要关机了，请做好准备！"), TEXT("温馨提示"), MB_OK | MB_ICONWARNING);
			Sleep(240000);
			MessageBox(NULL, TEXT("本电脑还有1分钟就要关机了，请做好准备！"), TEXT("温馨提示"), MB_OK | MB_ICONWARNING);
			return 0;
		}
		else
		{
			GetTime(1);
			printf("\n\n\t\t\t密码错误！");
			Sleep(1000);
		}
		system("cls");
	}

	system("shutdown -a && shutdown -s -t 5");
	MessageBox(NULL, TEXT("密码连续三次输入错误，本电脑即将关机。"), TEXT("警告"), MB_OK | MB_ICONWARNING);
	return 0;
}


int GetTime(int n)//获取当前时间
{
	errno_t err;
	FILE *fpwrite;

	if ((err = fopen_s(&fpwrite, "log.dat", "a+")) != 0) //以只写形式打开记录当前时间的文件,并且判断文件是否正常打开
		return 1;//如果文件创建错误，就返回主函数

	//写入当前时间到文件
	struct tm t;
	time_t now;
	time(&now);
	localtime_s(&t, &now);
	
	//写入日志
	if (n == 0)
		fprintf(fpwrite, "\n\n开机时间：%d年%02d月%02d日%02d点%02d分%02d秒。\n结果：密码输入正确！", t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec); //写入当前年月日时分到文件
	else if (n == 1)
		fprintf(fpwrite, "\n\n\n开机时间：%d年%02d月%02d日%02d点%02d分%02d秒。\n结果：密码输入错误！", t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec); //写入当前年月日时分到文件

	fclose(fpwrite); //关闭文件
	
	return 0;
}