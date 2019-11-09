#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <string.h>


#define WARNTWO "警告：此内容不适合孕妇以及患有心脏病、高血压、癫痫、脑血管病、神经错乱、哮喘、酗酒、吸毒者等人士访问！现在退出此程序或关闭电脑还来得及！如果硬要访问，后果自负！\n\t\t\t\t是否继续访问？"
#define WARNING "您的电脑中了病毒。请勿退出本程序，否则您的电脑和宝贵的数据都会嗝屁。\n如果您不想硬盘数据被格式化，昔日辛苦得来的数据成为泡影。\n如果您不想让电脑蓝屏无法启动，找维修工人上门维修，费时费力还费钱。\n如果您不想因为电脑蓝屏或重要数据丢失而耽误工作导致被老板炒鱿鱼的话，\n那就去网站https://cdmxz.github.io寻找密码吧！您只有5分钟了，加油！\n什么？你没连网？网站登不上去？那就不能怪我了，只能怪您自己运气太差咯！哈哈~\nPS：本网站和密码不收取一分钱费用，如果要钱，那就是骗子，请勿上当！"


int autoshut(void)
{
	system("title 现在退出此程序或关闭电脑还来得及！");

	int i;
	char pass[7] = { "115-56" };//密码
	char tmp[7] = { 0 };
	char str[] = { "警告：此内容不适合孕妇以及患有心脏病、高血压、癫痫、脑血管病、神经错乱、哮喘、酗酒、吸毒者等人士访问！现在退出此程序或关闭电脑还来得及！如果硬要访问，后果自负！" };
	

		i = MessageBox(NULL, TEXT(WARNTWO), TEXT("警告！"), MB_YESNO | MB_ICONWARNING);
		if (i == IDNO)
			return 0;

		else if (i == IDYES)
		{
			system("color 04");
			for (i = 10; i >= 0; i--)
			{
				system("cls");
				printf("\n\n\n\n\n\n\n\n%s", str);
				printf("\n\n\n     将在%d秒后进入，现在退出此程序或关闭电脑还来得及！", i);
				Sleep(1000);
			}


			system("mode con:cols=70 lines=25 && title 请勿退出本程序，否则您的电脑和宝贵的数据都会嗝屁！");//设置窗口大小 
			
			system("shutdown -s -t 300");//关机命令
			MessageBox(NULL, TEXT(WARNING), TEXT("温馨提示：请认真看完。"), MB_OK | MB_ICONQUESTION);

			puts(WARNING);

			printf("\n\n\n\n\n\n您的电脑中了病毒（请勿退出本程序，否则您的电脑和宝贵的数据都会嗝屁！）。\n\n");
			printf("\t\t温馨提示：你有300秒的时间输入密码。\n\n\n\t\t\t");
			system("pause");
			system("cls");

			for (i = 5; i >= 0; i--) //for循环
			{
				system("color 07");
				printf("温馨提示：你只剩下%d次机会。\n", i);
				printf("请输入密码（输入密码后别忘了按回车）:");
				
				scanf_s("%s", &tmp, 7);
				extern flush();//清除缓冲区残留字符

				if (!strcmp(tmp, pass))//将密码进行比较
				{
					system("color 0f");
					system("cls");
					printf("\n\n\n\n\n\n\n\n\n\t    恭喜您，密码正确，您的电脑和宝贵的数据不会嗝屁。\n\n");
					system("shutdown -a");
					system("pause");
					return 0;  //结束 
				}
				else
				{
					printf("\n\t\t\t\t\t密码错误！");
					system("color 0c");  //设置字体颜色
					Sleep(1000);
					system("cls");
				}
			}
			
			system("color 0a && cls");
			printf("\n\n\n\n\n\n\n\n\n    密码连续输入错误5次,您的电脑将嗝屁！您的宝贵数据也将嗝屁！\n\n\n");
			system("pause");
		}
		return 0;
}