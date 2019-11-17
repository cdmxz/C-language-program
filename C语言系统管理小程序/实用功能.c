﻿#include <stdio.h>
#include <Windows.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <io.h>

#define PAI 3.14
#define WORDSIZE 105
#define MAXPATH 415  

//加密文件内容
void encfile(void);    //加密文件内容
void init(char *filename);//初始化数组为'\0'

//计算器
int  cal(void);	     //计算器
void oper(void);     //四则运算
void square(void);   //开方计算
void bin(void);      //二进制转十进制
void dec(void);		 //十进制转二进制
void ascii(void);    //ascii码转字符
void character(void);//字符转ascii码
void circular(void); //圆柱体计算
void cube(void);     //正方体计算
void cuboid(void);   //长方体计算


//清除缓冲区
void flush(void);   
//加入自启动
int addstart(void);
//删除自启动
int delstart(void);


int num;//定义全局变量


//实用功能
void function(void)
{
	while (1)
	{
		system("mode con:cols=45 lines=20 && title 实用功能 && cls");

		printf("\n\n\n\t      1、多功能计算器\n\n");
		printf("\t      2、加密文件内容\n\n");
		printf("\t      3、添加软件自启动\n\n");
		printf("\t      4、删除软件自启动\n\n");
		printf("\t      5、返回主界面\n\n\n");
		printf("    请输入：");

		while (scanf_s("%d", &num) != 1)
		{
			printf("\n输入错误，请重试：");
			flush();
		}
		flush();

		if (num == 1)
			cal();
		
		else if (num == 2)
			encfile();

		else if (num == 3)
			addstart();

		else if(num==4)
			delstart();

		else if (num == 5)
			break;

		else
		{
			printf("\n\t\t输入错误！");
			Sleep(1000);
		}
	}
}


//加密文件内容
void encfile(void)
{
	int num, ch;
	char filename[MAXPATH] = { 0 };   //储存待加密或待解密文件名称
	FILE *fpRead, *fpWrite;
	errno_t err;


	while (1)
	{
		init(filename);//在每一次循环前初始化数组元素

		system("title 欢迎使用加密或解密程序 && cls && mode con cols=50 lines=20");
		printf("\n禁止加密重要文件！禁止恶作剧！禁止干违法犯罪的事！\n");
		printf("\n\n\t         0、返回上一界面\n\n");
		printf("\t         1、加密文件内容\n\n");
		printf("\t         2、解密文件内容\n\n");
		printf("\t         3、加密明文\n\n");
		printf("\t         4、解密密文\n\n");
		printf("\t         5、查看使用说明\n\n");
		printf("\t  请输入：");
		while (scanf_s("%d", &num) != 1)
		{
			rewind(stdin);
			printf("\n输入错误！请重新输入：");
		}
		rewind(stdin);

		if (num == 0)
			break;

		else if (num == 1)  //加密文件内容
		{
			system("title 加密文件内容 && mode con cols=60 lines=20 && cls");

			printf("注意：\n1、加密后的文件不能更改原来的内容，否则解密后不能恢复原样！\n");
			printf("2、待加密的文件的文件名称（包括路径、标点、后缀名等）请在200字以内，大于200字会报错并闪退。\n");
			printf("3、加密后的文件如需更改文件名请保留原后缀名“.fh”。（列如：把“1.txt.fh“ 改名为 “2.txt.fh”）。\n");
			printf("4、禁止加密重要文件！禁止恶作剧！禁止干违法犯罪的事！\n");
			printf("5、输入000返回上一界面。\n\n\n\n");
			printf("请输入待加密文件的名称（列如：C:\\1.txt或1.txt）：");

			gets_s(filename, (MAXPATH - 5));//输入待加密文件的名称
			//printf("数组长度：%lu", strlen(filename));
			if (filename[0] == 34)//支持文件拖拽输入
			{
				filename[strlen(filename) - 1] = 0;
				strcpy_s(filename, MAXPATH, filename + 1);//清楚拖拽输入多出来的双引号
			}

			if (!strcmp(filename, "000"))   //输入000返回主界面
				continue;

			if ((err = fopen_s(&fpRead, filename, "rb")) != 0)//以读的方式打开文件
			{
				perror("Error");
				system("pause");
				continue;
			}

			strcat_s(filename, MAXPATH, ".fh");//给加密后的文件添加后缀名

			if ((err = fopen_s(&fpWrite, filename, "wb")) != 0)//以写的方式打开文件
			{
				perror("Error:");
				system("pause");
				exit(1);
			}

			printf("\n\n\t\t\t请稍等...");

			while ((ch = fgetc(fpRead)) != EOF)  //从文件中读取内容到ch。EOF是文件结束标志。
			{
				ch = ch + ' ' - 9 + '9';//加密文件内容
				fputc(ch, fpWrite);//输出加密后的内容到另一个文件
			}


			if (fclose(fpRead) == EOF)//关闭文件
			{
				MessageBox(NULL, TEXT("关闭待加密文件失败！\n请检查是否有其它程序占用！"), TEXT("错误！"), MB_OK | MB_ICONERROR);
				exit(1);
			}
			if (fclose(fpWrite) == EOF)//关闭文件
			{
				MessageBox(NULL, TEXT("关闭已加密文件失败！\n请检查是否有其它程序占用！"), TEXT("错误！"), MB_OK | MB_ICONERROR);
				exit(1);
			}

			system("cls");
			printf("\n\n\n已加密：%s\n\n\t\t    ", filename);//输出文件名
			system("pause");
		}


		else if (num == 2)//解密文件内容
		{
			unsigned len, j;
			char ch;

			system("title 解密文件内容 && mode con cols=60 lines=20 && cls");

			printf("注意：\n");
			printf("1、待解密的文件的文件名称（包括路径、标点、后缀名等）请在205字以内，大于205字会报错并闪退。\n");
			printf("2、加密后的文件如需更改文件名请保留原后缀名“.fh”。（列如：把“1.txt.fh“ 改名为 “2.txt.fh”）。\n");
			printf("3、禁止加密重要文件！禁止恶作剧！禁止干违法犯罪的事！\n");
			printf("4、输入000返回上一界面。\n\n\n\n");
			printf("请输入待解密文件的名称（列如：C:\\1.txt或1.txt.fh）：");

			gets_s(filename, MAXPATH); //输入待解密文件的名称
			if (filename[0] == 34)//支持文件拖拽输入
			{
				filename[strlen(filename) - 1] = 0;
				strcpy_s(filename, MAXPATH, filename + 1);//清楚拖拽输入多出来的双引号
			}

			if (!strcmp(filename, "000"))//输入000返回主界面
				continue;

			if ((err = fopen_s(&fpRead, filename, "rb")) != 0) //以读的方式打开文件
			{
				MessageBox(NULL, TEXT("打开文件失败！\n请检查待加密的文件是否和本程序在同一目录，\n或检查文件名称是否正确！"), TEXT("错误！"), MB_OK | MB_ICONERROR);
				continue;
			}


			len = strlen(filename) - 1;//获取数组长度
			for (j = len; j > (len - 3); j--)//删除“.fh”后缀名
				filename[j] = '\0';


			if (_access(filename, 0) != -1)//判断需要解密的文件是否在当前路径中存在
			{
				printf("\n%s已在当前路径中存在，是否覆盖此文件（y/n）？", filename);
				scanf_s("%c", &ch, 1);
				flush();

				if (ch == 121 || ch == 89)
					;
				else if (ch == 110 || ch == 78)
					continue;
				else
				{
					printf("\n\t\t\t输入错误！");
					Sleep(1000);
					continue;
				}
			}

			if ((err = fopen_s(&fpWrite, filename, "wb")) != 0)//以写的方式打开文件
			{
				perror("Error");
				system("pause");
				exit(1);
			}

			printf("\n\n\t\t\t请稍等...");

			while ((ch = fgetc(fpRead)) != EOF)  //从文件中读取内容到ch。EOF是文件结束标志。
			{
				ch = ch - ' ' + 9 - '9';//解密文件内容
				fputc(ch, fpWrite);//输出解密后的内容到另一个文件
			}


			if (fclose(fpRead) == EOF)//关闭文件
			{
				MessageBox(NULL, TEXT("关闭待解密文件失败！\n请检查是否有其它程序占用！"), TEXT("错误！"), MB_OK | MB_ICONERROR);
				exit(1);
			}
			if (fclose(fpWrite) == EOF)//关闭文件
			{
				MessageBox(NULL, TEXT("关闭已解密文件失败！\n请检查是否有其它程序占用！"), TEXT("错误！"), MB_OK | MB_ICONERROR);
				exit(1);
			}


			system("cls");
			printf("\n\n\n已加密：%s\n\n\t\t    ", filename);//输出文件名
			system("pause");
		}

		else if (num == 3)//加密明文
		{
			unsigned  i;		//strlen返回一个无符号整型
			char word[WORDSIZE];

			system("title 加密明文 && mode con cols=50 lines=20 && cls");

			printf("\n\n请输入要加密的明文（50字以内，包括空格标点等）：\n");
			gets_s(word, WORDSIZE);

			for (i = 0; i < strlen(word); i++)//加密字符串
				word[i] += 1;

			printf("\n\n加密后的密文是：%s\n\n", word);
			system("pause");
		}

		else if (num == 4)//解密密文
		{
			unsigned  i;		//strlen返回一个无符号整型
			char word[WORDSIZE];

			system("title 解密密文 && mode con cols=50 lines=20 && cls");

			printf("\n\n请输入要解密的密文（50字以内，包括空格标点等）：\n");
			gets_s(word, WORDSIZE);

			for (i = 0; i < strlen(word); i++)//解密字符串
				word[i] -= 1;

			printf("\n\n解密后的明文是：%s\n\n", word);
			system("pause");
		}


		else if (num == 5)//使用说明
			MessageBox(NULL, TEXT("使用方法：\n1.文件名称输入方式：\n1、直接用鼠标把文件拖拽到程序窗口即可\n2、在输入文件名称窗口中直接输入文件路径（如：C://1.txt）\n3、在输入文件名称窗口中直接输入文件名称（如：1.txt）\n注意：使用第3种方式需要把本程序和待加密/待解密文件放到同一文件夹内。\n\n2.本程序理论上支持所有文件格式，但实际还待验证。\n\n注意事项（请仔细阅读以下内容）：\n1.加密或解密文件只会对文件内容进行加密或解密，\n不会对文件名称进行加密或解密。\n\n2.加密后的文件如需更改文件名请保留原后缀名“.fh”。\n（列如：把“1.txt.fh“ 改名为 “2.txt.fh”）。\n\n3.加密后的文件请不要更改其内容，否则解密后文件不能恢复原样！\n\n4.待加密文件名称（包括路径、标点、后缀名等）请在200字以内，\n待解密文件名称（包括路径、标点、后缀名等）请在205字以内。\n\n5.本软件只适合娱乐，\n禁止使用本软件加密重要文件！禁止使用本软件干违法犯罪的事！\n禁止使用本软件搞恶作剧！\n您使用本软件造成的任何后果均不由原作者承担！\n一经使用本软件，视为同意此条款！\n本条款若有更新，恕不另行通知！"), TEXT("使用说明"), MB_OK);

		else
		{
			printf("\n\t输入错误！");
			Sleep(1000);
		}

	}
}


//初始化数组
void init(char *filename)
{
	int i;
	for (i = 0; i < MAXPATH; i++)
		filename[i] = 0;
}


//计算器
int cal(void)
{
	while (1)
	{
		system("mode con lines=20 cols=60 && cls && title 多功能计算器");//设置标题

		printf("\t\t----欢迎使用多功能计算器----\n\n");
		printf("    \t\t       0、返回上一界面\n");
		printf("    \t\t       1、普通计算\n");
		printf("    \t\t       2、开方计算\n");
		printf("    \t\t       3、十进制转二进制\n");
		printf("    \t\t       4、二进制转十进制\n");
		printf("    \t\t       5、ASCII码转字符\n");
		printf("    \t\t       6、字符转ASCII码\n");
		printf("    \t\t       7、圆柱体/圆计算\n");
		printf("    \t\t       8、正方形/体计算\n");
		printf("    \t\t       9、长方形/体计算\n\n\n");
		printf("\t请输入：");
		while (scanf_s("%d", &num) != 1)
		{
			flush();
			printf("\n输入错误！请重新输入：");
		}
		flush();
		

		switch (num)
		{
		case 0:             return 0;//返回上一界面
		case 1:oper();      break;   //四则计算
		case 2:square();    break;   //开方计算
		case 3:dec();       break;   //十进制转二进制
		case 4:bin();       break;   //二进制转十进制
		case 5:ascii();     break;   //ASCII码转字符
		case 6:character(); break;   //字符转ASCII码
		case 7:circular();  break;   //圆柱体、圆计算
		case 8:cube();      break;   //正方体、形计算
		case 9:cuboid();    break;   //长方体、形计算
		default:printf("\n\t输入错误！"); Sleep(1000); break;
		}
	
	}
	return 0;
}


//四则运算
void oper(void)
{
	double a, b, d;
	char c;

	while (1)
	{
		system("cls && title 四则运算");

		printf("\t\t      输入0*0返回\n\n");
		printf("输入如“3*4.12”或“5.88+2.1”的四则运算式：");
		while (scanf_s("%lf %c %lf", &a, &c, 1, &b) != 3)
		{
			flush();
			printf("\n输入错误！请重新输入：");
		}
		flush();

		if (a == 0 && c == '*' && b == 0)//输入0*0返回
			break;

		switch (c)
		{
		case'+':d = a + b; break;
		case'-':d = a - b; break;
		case'*':d = a * b; break;
		case'/':d = (a == 0) ? 0 : (a / b); break;
		case'%':d = (int)a % (int)b; break;
		}

		printf("\n\n\n\t\t%.3lf %c %.3lf = %.3lf\n\n\n", a, c, b, d);
		system("pause");
	}
}


//开方计算
void square(void)
{
	double x;

	while (1)
	{
		system("cls && title 开方计算");

		printf("\n请输入需要开方的数(输入0返回）：");
		while (scanf_s("%lf", &x) != 1)
		{
			flush();
			printf("\n输入错误！请重新输入：");
		}
		flush();

		if (x == 0)//输入0返回
			break;

		printf("\n\t\t %.3lf开方等于：%.3lf\n\n", x, sqrt(x));
		system("pause");
	}
}


//十进制转二进制
void dec(void)
{
	while (1)
	{
		system("title 十进制转二进制 && cls");
		long long temp, dec = 0;
		int count[256] = { 0 };
		int bin, i = 0;

		printf("\t\t\t输入0返回\n\n\n");
		printf("请输入一个十进制数：");
		while (scanf_s("%lld", &dec) != 1)//处理错误输入
		{
			printf("\n输入错误,请重试：");
			flush();
		}
		flush();

		if (dec == 0)
			break;

		temp = dec;

		do   //转成十进制
		{
			bin = dec % 2;  //求每一次的余数
			dec = dec / 2;
			i++;
			count[i] = bin;
		} while (dec != 0);

		printf("\n\n十进制 %lld 转换成二进制的值是：", temp);
		for (; i > 0; i--)   //倒序输出 
			printf("%d", count[i]);

		printf("\n\n");
		system("pause");
	}
}


//二进制转十进制
void bin(void)
{
	int i = 0;

	while (1)
	{
		int bin;
		long long dec = 0;
		char ch;

		system("title 二进制转十进制 && cls");

		printf("\n\n\n请输入一个二进制数：");
		while ((bin = getchar()) != '\n')
			dec = dec * 2 + (bin - '0');
		printf("\n\n转换为十进制的值是：%lld\n\n", dec); //输出
		
		i++;

		if (i == 3)
		{
			printf("\n\n\t       是否返回上一界面？（Y/N）");
			scanf_s("%c", &ch, 1);
			flush();
			if (ch == 121 || ch == 89)
				break;
			else if (ch == 110 || ch == 78)
			{
				i = 0;
				continue;
			}
			else
			{
				i = 0;
				printf("\n\n\t\t\t输入错误！");
				Sleep(1000);
				continue;
			}
		}
		system("pause");
	}
}


//ascii码转字符
void ascii(void)
{
	int ascii;

	while (1)
	{
		system("cls && title ASCII码转字符");

		printf("\n\n\n");
		printf("（注意：1-31和127都是控制字符）\n\n");
		printf("请输入需要转换成字符的ASCII码：");
		while (scanf_s("%d", &ascii) != 1)
		{
			printf("\n输入错误！请重新输入：");
			flush();	//清空缓冲区
		}
		flush();

		if (ascii == 0)
			break;
		if (ascii > 127)
		{
			printf("\n\n输入最大不能大于127");
			Sleep(1000);
		}
		if (ascii < 0)
		{
			printf("\n\n输入最小不能小于0");
			Sleep(1000);
		}

		printf("\n\n\n\n      ASCII码 %d 对应的字符为：%c\n\n", ascii, ascii);
		printf("（如显示“□”则为控制字符）\n\n");
		system("pause");
	}
}


//字符转ascii码
void character(void)
{
	char ch;
	int i = 0;

	while (1)
	{
		system("cls && title 字符转ASCII码");
		printf("\n\n输入需要转换成ASCII码的字符：");
		scanf_s("%c", &ch, 1);
		flush();

		printf("\n\n\n\n     字符 %c 对应的ASCII码为：%d\n\n", ch, ch);
		i++;

		if (i == 3)
		{
			printf("\n\n\t       是否返回上一界面？（Y/N）");
			scanf_s("%c", &ch, 1);
			flush();
			if (ch == 121 || ch == 89)
				break;
			else if (ch == 110 || ch == 78)
			{
				i = 0;
				continue;
			}
			else
			{
				i = 0;
				printf("\n\n\t\t\t输入错误！");
				Sleep(1000);
				continue;
			}
		}
		system("pause");
	}
}


//圆柱体、圆
void circular(void)
{
	double r, h;

	while (1)
	{
		system("cls && title 圆柱体/圆的计算");
		printf("\t\t 在第一项输入里输入0返回");
		printf("\n\n请输入圆柱体的底面半径\n或圆的半径（单位：厘米）：");
		while (scanf_s("%lf", &r) != 1)
		{
			printf("\n输入错误，请重试：");
			flush();
		}
		flush();

		if (r == 0.0)//输入0返回
			break;

		printf("\n请输入圆柱体的高\n（计算圆的面积或周长请输入1。单位：厘米）：");
		while (scanf_s("%lf", &h) != 1)
		{
			printf("\n输入错误，请重试：");
			flush();
		}
		flush();

		printf("\n圆的面积为：%.3lf 平方厘米\n", PAI*r*r);
		printf("圆的周长为：%.3lf 厘米\n\n", 2 * PAI*r);
		printf("圆柱体的底面积为：  %.3lf 平方厘米\n", PAI*r*r);
		printf("圆柱体的底面周长为：%.3lf 厘米\n", 2 * PAI*r);
		printf("圆柱体的体积为：    %.3lf 立方厘米\n", PAI*r*r*h);
		printf("圆柱体的侧面积为：  %.3lf 平方厘米\n", 2 * PAI*r*h);
		printf("圆柱体的表面积为：  %.3lf 平方厘米\n\n", 2 * PAI*r*r + 2 * PAI*r*h);
		system("pause");
	}

}


//正方形、正方体
void cube(void)
{
	double edge;

	while (1)
	{
		system("cls && title 正方形/正方体的计算");

		printf("\t\t 在第一项输入里输入0返回");
		printf("\n\n请输入正方形的边长\n或正方体的棱长（单位：厘米）：");
		while (scanf_s("%lf", &edge) != 1)//处理错误输入
		{
			printf("\n输入错误，请重试：");
			flush();
		}
		flush();

		if (edge == 0.0)
			break;

		printf("\n\n\n正方形的面积为：%.3lf 平方厘米\n", edge * edge);
		printf("正方形的周长为：%.3lf 厘米\n\n", edge * 4);
		printf("正方体的体积为：  %.3lf 立方厘米\n", edge * edge * edge);
		printf("正方体的表面积为：%.3lf 平方厘米\n\n", edge * edge * 6);
		system("pause");
	}
}


//长方形、长方体
void cuboid(void)
{
	double h, l, w;

	while (1)
	{
		system("cls && title 长方形/长方体的计算");

		printf("\t\t 在第一项输入里输入0返回");
		printf("\n\n请输入长方形或长方体的长（单位：厘米）：");
		while (scanf_s("%lf", &l) != 1)
		{
			printf("\n输入错误，请重试：");
			flush();
		}
		flush();
		if (l == 0.0)
			break;

		printf("\n请输入长方形或长方体的宽（单位：厘米）：");
		while (scanf_s("%lf", &w) != 1)
		{
			printf("\n输入错误，请重试：");
			flush();
		}
		flush();

		printf("\n请输入长方体的高\n（只计算长方形请输入1。单位：厘米）：");
		while (scanf_s("%lf", &h) != 1)
		{
			printf("\n输入错误，请重试：");
			flush();
		}
		flush();

		printf("\n\n长方形的面积是：%.3lf 平方厘米\n", l*w);
		printf("长方形的周长是：%.3lf 厘米\n\n", (l*w + l * w) * 2);
		printf("长方体的体积是：  %.3lf 立方厘米\n", h*l*w);
		printf("长方体的表面积是：%.3lf 平方厘米\n\n", (l*w + l * h + w * h) * 2);
		system("pause");
	}
}

//添加程序自启动
int addstart(void)
{
	system("title 添加软件自启动 && cls");

	HKEY hKey;
	char *regPath = { "Software\\Microsoft\\Windows\\CurrentVersion\\Run" }; //注册表启动项路径
	char path[MAXPATH] = { 0 };//需要添加自启动的软件的路径
	char name[31] = { 0 };//注册表子项名称

	printf("\t    输入000返回上一界面\n\n");
	printf("请输入需要添加自启动的软件的路径\n（例如：H:\\test\\test.exe）\n ：");
	gets_s(path, MAXPATH);//输入路径

	if (!strcmp(path, "000"))//输入000退出
		return 0;


	printf("\n\n请输入添加到注册表的键值项名称\n\n（需要在16个字符内。\n  可以使用字符、数字、代表符、空格，\n  但不能使用“\\”）\n\n ：");
	gets_s(name, 31);//输入名称


	//打开注册表启动项 
	if (RegOpenKeyEx(HKEY_CURRENT_USER, regPath, 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)
	{  //添加一个子Key,并设置值
		RegSetValueEx(hKey, name, 0, REG_SZ, (BYTE *)path, strlen(path));
		RegCloseKey(hKey);//关闭注册表

		printf("\n\t         添加成功！\n注意：请不要删除已添加的程序，否则自启动会失效！\n\n              ");
		system("pause");
		return 0;
	}
	else
	{
		MessageBox(NULL, TEXT("添加失败！"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
		return -1;
	}
	return 0;
}



//删除程序自启动
int delstart(void)
{
	system("title 删除程序自启动 && cls");
    char path[120] = { "reg delete \"HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run\"  /v  \"" };
	char name[31] = { 0 }; //存储名称
	const char Backslash[] = { "\"" };//储存反斜杠
	
	printf("\t    输入000返回上一界面\n\n\n");
	printf("请输入键值项名称（16个字符内）\n：");
	gets_s(name, 31);

	if (!strcmp(name, "000"))
		return 0;
	
	strcat_s(path, sizeof(path), name);
	strcat_s(path, sizeof(path), Backslash);

	
	printf("\n\n");
	system(path);
	printf("\n\n");
	system("pause");
	return 0;
}