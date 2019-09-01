#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <string.h>
int main(void)
{
	int num, ch;
	FILE *fpread, *fpwrite;
	errno_t err;
	char time[51];                   //存储时间
	char content[301];               //存储写入文件的内容

	while (1)
	{
		system("cls");
		system("mode con:cols=60 lines=20"); //设置控制台窗口大小
		system("title 本程序可以帮您记录您的代办日程，但可能有点鸡肋...");
		printf("\n\t\t  0.退出代办日程程序\n\n");
		printf("\t\t  1.输入您的代办日程\n\n");
		printf("\t\t  2.查看您的代办日程\n\n");
		printf("\t\t  3.打开您的代办日程\n\n");
		printf("\t\t  4.删除所有代办日程\n\n\n");
		printf("(仅需输入前面的数字序号即可，输入后请按回车。)\n请输入序号：");
		while (scanf_s("%d", &num) != 1)       //处理字符输入
		{
			rewind(stdin);//清空缓冲区残余字符
			printf("\n输入错误！请重新输入：");
		}	
		rewind(stdin);//清空缓冲区残余字符
		if (num >= 0 && num <= 4)
		{
			if (num == 0)//退出
				break;
			if (num == 1)//输入
			{
				int number;

				while (1)
				{
					if (err = fopen_s(&fpwrite, "shuju.dat", "a") != 0)//打开文件（如果不存在,会自动创建），并判断文件是否正常打开
					{
						MessageBox(NULL, TEXT("文件读取或创建错误！\n请以管理员权限运行重试！"), TEXT("error!"), MB_OK | MB_ICONERROR);//弹出提示框
						exit(1);                                        //异常退出
					}
					system("cls");
					system("title 字数请在25字（包括符号、标点、空格）以内，否则会造成错误！");
					printf("温馨提示：\n1、输入完后请按回车。\n2、字数请在25字（包括符号、标点、空格）以内,否则会造成错误。\n\n\n\n");
					printf("请输入代办日程的时间\n（就是您要办这件事的日期）：");
					fgets(time, 51,stdin);                  //输入时间
					rewind(stdin);//清空缓冲区残余字符
					system("cls");
					system("title 字数请在150字（包括符号、标点、空格）以内，否则会造成错误！");
					printf("温馨提示：\n1、输入完后请按回车。\n2、字数请在150字（包括符号、标点、空格）以内,否则会造成错误。\n\n");
					printf("请输入代办日程的内容：\n");
					fgets(content,301,stdin);      //输入内容
					rewind(stdin);//清空缓冲区残余字符
					fprintf(fpwrite, "日程代办时间：%s", time);//把输入的内容写入文件
					fprintf(fpwrite, "日程代办内容：%s\n", content);
					fclose(fpwrite);//关闭文件
					printf("\n\t\t录入成功！\n\n");
					printf("是否继续录入（1.是 2.否）？\n请输入：");
					while (scanf_s("%d", &number) != 1)
					{
						rewind(stdin);//清空缓冲区残余字符
						printf("\n输入错误！请重新输入：");
					}
					rewind(stdin);
					if (number == 1)
						continue;
					else if (number == 2)
						break;
					else
					{
						printf("\n输入错误！没有这个选项！");
						Sleep(1000);
					}

				}
			}

			if (num == 2)//查看
			{
				system("mode con: cols=120 lines=40");
				if ((err = fopen_s(&fpread, "shuju.dat", "r")) != 0)//以只读形式打开文件
				{
					MessageBox(NULL, TEXT("数据文件读取错误！\n请检查本程序目录下是否有“shuju.dat”这个文件\n或检查数据文件是否被其它程序占用！"), TEXT("error!"), MB_OK | MB_ICONERROR);
					exit(1);                                   //程序异常退出
				}
				while ((ch = fgetc(fpread)) != EOF)            //读取文件内容到ch。EOF是文件结束标志。
					putchar(ch);                               //读取文件内容输出到控制台
				putchar('\n');
				system("pause");
				fclose(fpread);    //关闭文件
			}
			if (num == 3)//打开
			{
				if (rename("shuju.dat", "shuju.txt"))
				{
					MessageBox(NULL, TEXT("数据文件重命名失败！\n请检查是否有其它程序占用！\n或者重试！"), TEXT("error!"), MB_OK | MB_ICONERROR);
					continue;
				}
				system("shuju.txt");//打开文件
				system("pause");
				if (rename("shuju.txt", "shuju.dat"))
				{
					MessageBox(NULL, TEXT("数据文件重命名失败！\n请在本程序目录下把“shuju.txt”重命名为“shuju.dat”。"), TEXT("error!"), MB_OK | MB_ICONERROR);
					continue;
				}
			}

			if (num == 4)//删除
			{
				num = MessageBox(NULL, TEXT("此操作会删除您的所有代办日程，且删除后无法恢复！\n请务必在慎重考虑后删除！\n是否删除？"), TEXT("警告！"), MB_YESNO | MB_ICONWARNING);
				if (num == IDNO)                                   //利用返回值判断用户点击的按钮
					system("cls");
				if (num == IDYES)
					if(remove("shuju.dat"))
					MessageBox(NULL,TEXT("数据文件删除失败！\n请检查是否被其它程序占用\n或检查本程序目录下是否有“shuju.dat”这个文件！"),TEXT("error!"),MB_OK | MB_ICONERROR);
			}
		}
		else
		{
		  printf("\n\t输入错误！");
		  Sleep(1000);
	    }
    } 
} 