#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <string.h>

#define WARNONE "警告：此内容不适合18岁以下未成年、孕妇以及患有心脏病、高血压、癫痫、脑血管病、神经错乱、哮喘、酗酒、吸毒者等人士访问！\n\n\t\t\t现在退出此程序或关闭电脑还来得及！\n\t\t\t    如果硬要访问，后果自负！\n\t\t\t\t是否继续访问？"//彩蛋一
#define WARNTWO "警告：此内容不适合孕妇以及患有心脏病、高血压、癫痫、脑血管病、神经错乱、哮喘、酗酒、吸毒者等人士访问！现在退出此程序或关闭电脑还来得及！如果硬要访问，后果自负！\n\t\t\t\t是否继续访问？"//彩蛋二
#define WARNING "您的电脑中了病毒。请勿退出本程序，否则您的电脑和宝贵的数据都会嗝屁。\n如果您不想硬盘数据被格式化，昔日辛苦得来的数据成为泡影。\n如果您不想让电脑蓝屏无法启动，找维修工人上门维修，费时费力还费钱。\n如果您不想因为电脑蓝屏或重要数据丢失而耽误工作导致被老板炒鱿鱼的话，\n那就去网站https://cdmxz.github.io寻找密码吧！您只有5分钟了，加油！\n什么？你没连网？网站登不上去？那就不能怪我了，只能怪您自己运气太差咯！哈哈~\nPS：本网站和密码不收取一分钱费用，如果要钱，那就是骗子，请勿上当！"

void flush(void);

void olddriver(void)//彩蛋1
{
	system("mode con:cols=90 lines=30 && color 04 && title 现在退出此程序或关闭电脑还来得及！");

	char str[] = "警告：此内容不适合18岁以下未成年、孕妇以及患有心脏病、高血压、癫痫、脑血管病、神经错乱、哮喘、酗酒、吸毒者等人士访问！\n\n\t\t\t现在退出此程序或关闭电脑还来得及！\n\t\t\t    如果硬要访问，后果自负！";
	char name[7]={ 0 };
	char pass[7]={ 0 };
	int i;
	

	while (1)
	{
		i = MessageBox(NULL, TEXT(WARNONE), TEXT("警告！"), MB_YESNO | MB_ICONWARNING);//弹出提示框
		if (i == IDNO)//判断用户点击的按钮
			break;

		else if (i == IDYES)
		{
			for (i = 10; i >= 0; i--)
			{
				printf("\n\n\n\n\n\n%s\n\n\n\n\n\n%43d", str, i);
				Sleep(1000);
				system("cls");
			}

			system("color 07 && title 网址：www.老司机成人网.com......18周岁以下未成年人勿进!!!");//窗口标题
	
			printf("\n\n\t请稍等...\n\n");
			Sleep(2000);

			for (i = 3; i >= 0; i--)
			{
				printf("\t\t\t\t%d\n\n", i);
				Sleep(1000);
			}

			printf("	以下内容只符合年满18周岁的用户！\n\n");
			Sleep(2000);
			printf("			www.老司机成人网.com\n\n");
			Sleep(1000);
			printf("		请输入您的用户名（默认：lsj）：");

			scanf_s("%s", name, 7);
			flush();//清除缓冲区残留字符

			printf("\n		请输入您的密码（默认：123456）：");
			
			scanf_s("%s", pass, 7);
			flush();

			
			if (!strcmp(name, "lsj") && !strcmp(pass, "123456"))//比较账号密码是否正确
			{
				Sleep(1000);
				system("color 0c");
				printf("\n\t		    正在验证中...\n");
				Sleep(1600);
				system("color 0f");
				printf("\n		      账号密码正确，验证成功!\n\n\n");
				Sleep(1500);
				printf("   （系统检测到您正在使用默认密码，请及时登录www.老司机成人网.com更改密码！！！）");
				Sleep(3000);
				system("cls");
				printf("\n\n\n\n			  尊敬的VIP会员，欢迎回来！\n\n\n");
				Sleep(2000);
			}
			else
			{
				system("color 0c");
				printf("\n			  正在验证中...\n\n\n");
				Sleep(1600);
				system("color 0a");
				printf("\t\t   账号或密码错误，即将退出！\n\n");
				Sleep(2000);
				break;
			}

			printf("请选择是否跳转到专属的VIP通道，请输入 y（是） n（否）？\n\n  ");
			printf("请输入：");
			scanf_s("%s", name, 2);
			flush();


			if (!strcmp(name, "y"))
			{
				printf("			正在跳转中...\n\n");
				Sleep(2000);
				system("cls");
				printf("\n\n\n	系统检测到有wj在监视您，已为您自动打码。请尽快退出......\n\n");
				printf("	。。。。	。。。。	。。。。	。。。。	。。。。\n\n");
				printf("	。。。。	。。。。	。。。。	。。。。	。。。。\n\n");
				printf("	。。。。	。。。。	。。。。	。。。。	。。。。\n\n");
			}
			else if (!strcmp(name, "n"))
			{
				printf("\n");
				system("color 0c");
				printf("\t\t	  请稍后...\n");
				Sleep(2000);
				system("cls && color 0a");
				printf("\n\n\n\n\n\n\n\t\t\t恭喜施主，苦海无涯，回头是岸！\n\n");
				system("pause");
				break;
			}
			else
			{
				system("cls");
				printf("\n\n\n\n\n\n\n\t\t\t   输入错误，即将退出！\n");
				Sleep(2000);
				break;
			}


			Sleep(2000);
			printf("	是否跳转到专属VIP房间进行播放（y（是） n（否）？）\n\n");
			printf("请输入：");
			
			scanf_s("%s", name, 2);
			flush();


			if (!strcmp(name, "y"))
			{
				printf("\n	    请稍后...\n\n");
				Sleep(2000);
				system("color 0a");
				system("cls");
				printf("\n\n\n\n\n\n\n			施主,\n\n\n");
				Sleep(1000);
				printf("			苦海无涯，回头是岸！！！\n\n");
				system("pause");
			}
			else if (!strcmp(name, "n"))
			{
				printf("\n");
				printf("		请稍后...\n");
				Sleep(2000);
				system("cls");
				printf("\n\n\n\n\n\n\n				恭喜施主，苦海无涯，回头是岸！\n\n");
				system("pause");
			}
			else
			{
				system("cls");
				printf("\n\n\n\n\n\n\n\t\t\t   输入错误，即将退出！\n");
				Sleep(2000);
			}
		}
		break;
	}
}



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
			
			system("shutdown -s -t 360");//关机命令
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
				flush();//清除缓冲区残留字符


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


