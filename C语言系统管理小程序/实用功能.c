#include <stdio.h>
#include <Windows.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


#define PAI 3.14
#define ENCSIZE 163
#define DECSIZE 170
#define TXTSIZE 105
#define MAXPATH 1024  

//加密文件内容
void EncFile(void);    //加密文件内容
void init(char  encry[], char  encname[], char  decry[]);//初始化数组为'\0'
void Text(char TXT[]);//清除数组里的'\n'
void flush(void);//清除缓冲区

//计算器
void  cal(void);	//计算器
void oper(void);    //四则运算
void square(void);  //开方计算
void bin(void);     //进制转换
void ascii(void);   //ascii转换
void circular(void);//圆柱体计算
void cube(void);    //正方体计算
void cuboid(void);  //长方体计算
void cst(void);     //三角函数
void flush(void);   //清除缓冲区

//加入自启动
int addStart(void);
//删除自启动
void delStart(void);


int num;//定义全局变量


//实用功能
void function(void)
{
	while (1)
	{
		system("mode con:cols=45 lines=20 && title 实用工具 && cls");

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
			EncFile();

		else if (num == 3)
			addStart();

		else if(num==4)
			delStart();

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
void EncFile(void)
{
	int ch, i;

	while (1)
	{
		system("title 欢迎使用加密解密程序 && mode con cols=44 lines=18 && cls");
		printf("\n\n\t     0.返回主界面\n\n");
		printf("\t     1.加密或解密文件内容\n\n");
		printf("\t     2.加密明文或解密密文\n\n");
		printf("\t     3.查看使用说明\n\n\n");
		printf(" 请输入序号：");
		while (scanf_s("%d", &num) != 1)//处理字符输入
		{
			flush();
			printf("\n输入错误！请重新输入：");
		}
		flush();
			
		if (num == 0)
			break;

//加密或解密文件内容
		else if (num == 1)
		{
			char encry[ENCSIZE];   //储存待加密文件名称
			char encname[DECSIZE]; //储存待加密文件加密后的名称
			char decry[DECSIZE];   //储存待解密文件名称
			char enc[] = "已加密 ";
			char dec[] = "已解密 ";
			FILE *fpread, *fpwrite;
			errno_t err;
	

			while (1)
			{
				init(encry, encname, decry);//在每一次循环前初始化数组元素

				system("title 加密或解密文件内容 && cls && mode con cols=50 lines=18");
				printf("\n禁止加密重要文件！禁止恶作剧！禁止干违法犯罪的事！\n");
				printf("\n\n\t         1、加密文件内容\n\n");
				printf("\t         2、解密文件内容\n\n");
				printf("\t         3、返回主界面\n\n");
				printf("\t  请输入：");
				while (scanf_s("%d", &num) != 1)
				{
					flush();
					printf("\n输入错误！请重新输入：");
				}
				flush();
	

				if (num == 1)  //加密文件内容
				{
					system("title 加密文件内容 && mode con cols=60 lines=20 && cls");
					printf("注意：\n1、加密后的文件不能更改原来的内容，否则解密后不能恢复原样！\n");
					printf("2、加密后的文件如需更改文件名请保留原格式。（列如：把“已加密 1.txt“ 改名为 “已加密 2.txt”）。\n");
					printf("3、待加密的文件需要与本程序在同一目录下，并且文件名（包括后缀名和空格等）在80字以内，大于80字会报错并闪退。\n");
					printf("4、禁止加密重要文件！禁止恶作剧！禁止干违法犯罪的事！\n");
					printf("5、输入000返回主界面。\n\n\n\n");
					printf("请输入待加密文件的名称（列如：1.txt）：");

					fgets(encry, ENCSIZE, stdin);//输入待加密文件的名称
					flush();//清除缓冲区


					for (i = 0; i <= ENCSIZE; i++) //去掉数组里的换行符
						if (encry[i] == '\n')
							encry[i] = '\0';

					if (!strcmp(encry, "000"))   //输入000返回主界面
						continue;

					if ((err = fopen_s(&fpread, encry, "rb")) != 0)//以读取的方式打开一个二进制文件
					{
						MessageBox(NULL, TEXT("打开待加密文件失败！\n请检查待加密的文件是否和本程序在同一目录，\n或检查文件名称是否正确！"), TEXT("错误！"), MB_OK | MB_ICONERROR);
						continue;
					}

					strcat_s(encname, sizeof(encname), enc);//合并文件名称
					strcat_s(encname, sizeof(encname), encry);

					if ((err = fopen_s(&fpwrite, encname, "wb")) != 0)//以写入的方式打开一个二进制文件
					{
						perror("Error:");
						system("pause");
						//MessageBox(NULL, TEXT("创建文件失败！\n请以管理员权限运行本软件！"), TEXT("错误！"), MB_OK | MB_ICONERROR);
						exit(1);
					}

					while ((ch = fgetc(fpread)) != EOF)  //从文件中读取内容到ch。EOF是文件结束标志。
					{
						ch = ch + ' ' - 9 + '9';//加密文件内容
						fputc(ch, fpwrite);//输出加密后的内容到另一个文件
					}


					if (fclose(fpread) == EOF)//关闭文件
					{
						MessageBox(NULL, TEXT("关闭待加密文件失败！\n请检查是否有其它程序占用！"), TEXT("错误！"), MB_OK | MB_ICONERROR);
						exit(1);
					}
					if (fclose(fpwrite) == EOF)//关闭文件
					{
						MessageBox(NULL, TEXT("关闭已加密文件失败！\n请检查是否有其它程序占用！"), TEXT("错误！"), MB_OK | MB_ICONERROR);
						exit(1);
					}

					system("cls");
					printf("\n\n\n");
					fputs(encname, stdout);//输出文件名
					putchar('\n');
					system("pause");
				}


				else if (num == 2)//解密文件内容
				{
					system("title 解密文件内容 && mode con cols=60 lines=20 && cls");
					
					printf("注意：\n");
					printf("1、待解密的文件需要与本程序在同一目录下，并且文件名（包括后缀名和空格等）在85字以内，大于85字会报错并闪退。\n");
					printf("2、加密后的文件如需更改文件名请保留原格式。（列如：把“已加密 1.txt“ 改名为 “已加密 2.txt”）。\n");
					printf("3、禁止加密重要文件！禁止恶作剧！禁止干违法犯罪的事！\n");
					printf("4、输入000返回主界面。\n\n\n\n");
					printf("请输入待解密文件的名称（列如：1.txt）：");

					fgets(decry, DECSIZE, stdin); //输入待解密文件的名称
					flush();//清除缓冲区

					for (i = 0; i <= 170; i++)//去掉数组里的换行符
						if (decry[i] == '\n')
							decry[i] = '\0';

					if (!strcmp(decry, "000"))//输入000返回主界面
						continue;

					if ((err = fopen_s(&fpread, decry, "rb")) != 0) //以读取的方式打开一个二进制文件
					{
						MessageBox(NULL, TEXT("打开文件失败！\n请检查待加密的文件是否和本程序在同一目录，\n或检查文件名称是否正确！"), TEXT("错误！"), MB_OK | MB_ICONERROR);
						continue;
					}

					for (i = 0; i <= 5; i++)//for循环替换文件名前三个字符，即把文件名称的已加密替换成已解密。
						decry[i] = dec[i];

					if ((err = fopen_s(&fpwrite, decry, "wb")) != 0)//以写入的方式打开一个二进制文件
					{
						perror("Error:");
						system("pause");
						//MessageBox(NULL, TEXT("创建文件失败！\n请以管理员权限运行本软件！"), TEXT("错误！"), MB_OK | MB_ICONERROR);
						exit(1);
					}

					while ((ch = fgetc(fpread)) != EOF)  //从文件中读取内容到ch。EOF是文件结束标志。
					{
						ch = ch - ' ' + 9 - '9';//解密文件内容
						fputc(ch, fpwrite);//输出解密后的内容到另一个文件
					}


					if (fclose(fpread) == EOF)//关闭文件
					{
						MessageBox(NULL, TEXT("关闭待解密文件失败！\n请检查是否有其它程序占用！"), TEXT("错误！"), MB_OK | MB_ICONERROR);
						exit(1);
					}
					if (fclose(fpwrite) == EOF)//关闭文件
					{
						MessageBox(NULL, TEXT("关闭已解密文件失败！\n请检查是否有其它程序占用！"), TEXT("错误！"), MB_OK | MB_ICONERROR);
						exit(1);
					}


					system("cls");
					printf("\n\n\n");
					fputs(decry, stdout);//输出文件名
					putchar('\n');
					system("pause");
				}

				else if (num == 3)
				break;

				else
				{
					printf("\n\t\t输入错误！");
					Sleep(1000);
				}
			}
		}
//加密明文或解密密文
		else if (num == 2)
		{
			unsigned  i;		//strlen返回一个无符号整型
			char TXT[TXTSIZE];

			while (1)
			{
				system("title 加密明文或解密密文 && cls && mode con cols=50 lines=18");
				printf("\n\n\n\t\t  1、加密明文\n\n");
				printf("\t\t  2、解密密文\n\n");
				printf("\t\t  3、返回主界面\n\n");
				printf("  请输入序号：");	
				while (scanf_s("%d", &num) != 1)
				{
					flush();
					printf("\n输入错误！请重新输入：");
				}
				flush();


				if (num == 1)//加密明文
				{
					system("title 加密明文 && mode con cols=50 lines=20 && cls");
					printf("\n\n请输入要加密的明文（50字以内，包括空格标点等）：\n");
					fgets(TXT, TXTSIZE, stdin);
					flush();
					

					Text(TXT); //清除数组里的'\n'
					for (i = 0; i < strlen(TXT); i++)
						TXT[i] += 1;
									
					printf("\n\n加密后的密文是：%s\n\n", TXT);
					system("pause");
				}

				else if (num == 2)//解密密文
				{
					system("title 解密密文 && mode con cols=50 lines=20 && cls");
					printf("\n\n请输入要解密的密文（50字以内，包括空格标点等）：\n");
					fgets(TXT, TXTSIZE, stdin);
					flush();
					

					Text(TXT); //清除数组里的'\n'
					for (i = 0; i < strlen(TXT); i++)
						TXT[i] -= 1;
									
					printf("\n\n解密后的明文是：%s\n\n", TXT);
					system("pause");
				}

				else if (num == 3)//返回主界面
					break;

				else
				{
					printf("\n\n\t输入错误！\n");
					Sleep(1000);
				}
			}
		}
//使用说明
		else if (num == 3)
			MessageBox(NULL, TEXT("请仔细阅读以下内容：\n1.加密或解密文件只会对文件内容进行加密或解密，\n不会对文件名称进行加密或解密。\n\n2.加密文件内容或解密文件内容前请确保待加密或待解密的文件\n没有被其它程序占用并且和本程序在同一目录。\n\n3.加密后的文件请不要更改其内容，否则解密后文件不能恢复原样！\n\n4.待加密文件名称（包括后缀名和空格等）请在80字以内，\n待解密文件名称（包括后缀名和空格等）请在85字以内。\n\n5.禁止使用本软件加密重要文件！禁止使用本软件干违法犯罪的事！\n禁止使用本软件搞恶作剧！\n您使用本软件造成的任何后果均不由原作者承担！\n一经使用本软件，视为同意此条款！\n本条款若有更新，恕不另行通知！"), TEXT("使用说明"), MB_OK);


		else
		{
			printf("\n\t输入错误！");
			Sleep(1000);
		}
	}
}


//初始化数组
void init(char encry[], char encname[], char decry[])
{
	int i;

	for (i = 0; i < ENCSIZE; i++)
		encry[i] = 0;

	for (i = 0; i < DECSIZE; i++)
	{
		encname[i] = 0;
		decry[i] = 0;
	}
}


//清除数组里的'\n'
void Text(char TXT[])
{
	int i;
	for (i = 0; i < TXTSIZE; i++)
		if (TXT[i] == '\n')
			TXT[i] = '\0';
}


//计算器
void cal(void)
{

	while (1)
	{
		system("mode con lines=20 cols=60 && cls && title 多功能计算器");//设置标题

		printf("\t\t----欢迎使用多功能计算器----\n\n");
		printf("    \t\t       0、返回主界面\n");
		printf("    \t\t       1、普通计算\n");
		printf("    \t\t       2、开方计算\n");
		printf("    \t\t       3、进制转换\n");
		printf("    \t\t       4、ASCII码转换\n");
		printf("    \t\t       5、圆柱体/圆计算\n");
		printf("    \t\t       6、正方形/体计算\n");
		printf("    \t\t       7、长方形/体计算\n");
		printf("    \t\t       8、三角函数计算\n\n\n");
		printf("\t请输入：");
		while (scanf_s("%d", &num) != 1)
		{
			flush();
			printf("\n输入错误！请重新输入：");
		}
		flush();

		switch (num)
		{
		case 0: break;
		case 1:oper(); break;    //四则计算
		case 2:square(); break;  //开方计算
		case 3:bin(); break;     //进制转换
		case 4:ascii(); break;   //ASCII码转换转换
		case 5:circular(); break;//圆柱体、圆计算
		case 6:cube(); break;    //正方体、形计算
		case 7:cuboid(); break;  //长方体、形计算
		case 8:cst(); break;	 //三角函数计算
		default:printf("\n\t输入错误！"); Sleep(1000); break;
		}
		break;
	}

}


//四则运算
void oper(void)
{
	double a, b;
	double d = 0.0;
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


//进制转换
void bin(void)
{
	int Bin;

	while (1)
	{
		system("title 进制转换 && cls");
		printf("\n\n\t\t    1、十进制转二进制\n\n");
		printf("\t\t    2、二进制转十进制\n\n");
		printf("\t\t    3、返回主界面\n\n\n");
		printf("\t  请输入：");
		while (scanf_s("%d", &num) != 1)
		{
			printf("\n输入错误，请重试：");
			flush();
		}
		flush();

		if (num == 1)
		{
			int Dec = 0;
			int count[100];
			int i = 0;

			system("title 十进制转二进制 && cls");

			printf("\t\t\t输入0返回\n\n\n");
			printf("请输入一个十进制数：");
			while (scanf_s("%d", &Dec) != 1)//处理错误输入
			{
				printf("\n输入错误,请重试：");
				flush();
			}
			flush();

			if (Dec == 0)
				break;

			num = Dec;

			do   //转成十进制
			{
				Bin = Dec % 2;  //求每一次的余数
				Dec = Dec / 2;
				i++;
				count[i] = Bin;
			} while (Dec != 0);

			printf("\n\n十进制 %d 转换成二进制的值是：", num);
			for (; i > 0; i--)   //倒序输出 
				printf("%d", count[i]);

			printf("\n\n");
		}

		else if (num == 2)
		{
			long long dec, tmp, binry;
			double count = 0;
			int total = 0;

			system("title 二进制转十进制 && cls");

			printf("\t\t\t输入3返回\n\n\n");
			printf("\n请输入一个二进制数：");
			while (scanf_s("%lld", &dec) != 1)//处理错误输入
			{
				printf("\n输入错误,请重试：");
				flush();
			}
			flush();

			tmp = dec;
			if (dec == 3)//输入3返回
				break;

			while (dec)//转成二进制
			{
				binry = dec - (long long int)(dec / 10) * 10;
				count += binry * pow(2, total);
				total++;
				dec = (long long int)(dec / 10);
			}
			printf("\n\n二进制 %lld 转换为十进制的值是：%.0lf\n\n", tmp, count); //打印
		}

		else if (num == 3)
			break;

		else
		{
			printf("\n\t输入错误！");
			Sleep(1000);
		}
		system("pause");
	}
}


//ascii码转换
void ascii(void)
{
	int ascii;
	char ch;

	while (1)
	{
		system("cls && title ASCII码转换");
		printf("\n\n\n\t\t       0、返回主界面\n\n");
		printf("\t\t       1、ASCII码转字符\n\n");
		printf("\t\t       2、字符转ASCII码\n\n\n");
		printf("\t  请输入：");
		while (scanf_s("%d", &num) != 1)//处理字符输入
		{
			flush();	//清空缓冲区
			printf("\n输入错误！请重新输入：");
		}
		flush();


		if (num == 0)
			break;

		if (num == 1)
		{
			system("cls");
			printf("\n\n\n");
			printf("（注意：1-31和127都是控制字符）\n\n");
			printf("请输入需要转换成字符的ASCII码：");
			while (scanf_s("%d", &ascii) != 1)
			{
				printf("\n输入错误！请重新输入：");
				flush();	//清空缓冲区
			}
			flush();


			if (ascii >= 128)
			{
				printf("\n\n输入最大不能大于127");
				Sleep(1000);
			}
			if (ascii <= -1)
			{
				printf("\n\n输入最小不能小于0");
				Sleep(1000);
			}

			printf("\n\n\n\n      ASCII码 %d 对应的字符为：%c\n\n", ascii, ascii);
			printf("（如显示“□”则为控制字符）\n\n");
		}

		else if (num == 2)
		{
			system("cls");
			printf("\n\n输入需要转换成ASCII码的字符：");
			scanf_s("%c", &ch, 1);
			flush();

			printf("\n\n\n\n     字符 %c 对应的ASCII码为：%d\n\n", ch, ch);
		}
		else
		{
			printf("\n\n\t\t输入错误！");
			Sleep(2000);
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
		printf("\n\n请输入圆柱体的底面半径\n或圆的半径（不知道输入1）：");
		while (scanf_s("%lf", &r) != 1)
		{
			printf("\n输入错误，请重试：");
			flush();
		}
		flush();

		if (r == 0.0)//输入0返回
			break;

		printf("\n请输入圆柱体的高（不知道输入1）：");
		while (scanf_s("%lf", &h) != 1)
		{
			printf("\n输入错误，请重试：");
			flush();
		}
		flush();

		printf("\n\n圆柱体的底面积为：%.3lf\n\n", PAI*r*r);
		printf("圆柱体的底面周长为：%.3lf\n\n", 2 * PAI*r);
		printf("圆柱体的侧面积为：%.3lf\n\n", 2 * PAI*r*h);
		printf("圆柱体的表面积为：%.3lf\n\n", 2 * PAI*r*r + 2 * PAI*r*h);
		printf("圆柱体的体积为：%.3lf\n\n", PAI*r*r*h);
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
		printf("\n\n请输入正方形的边长\n或正方体的棱长：");
		while (scanf_s("%lf", &edge) != 1)//处理错误输入
		{
			printf("\n输入错误，请重试：");
			flush();
		}
		flush();

		if (edge == 0.0)
			break;

		printf("\n正方体的体积为：%.3lf\n\n", edge * edge * edge);
		printf("正方体的表面积为：%.3lf\n\n", edge * edge * 6);
		printf("正方形的面积为：%.3lf\n\n", edge * edge);
		printf("正方形的周长为：%.3lf\n\n", edge * 4);
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
		printf("\n\n请输入高度（不知道输入1）：");
		while (scanf_s("%lf", &h) != 1)
		{
			printf("\n输入错误，请重试：");
			flush();
		}
		flush();

		if (h == 0.0)
			break;

		printf("\n请输入长度（不知道输入1）：");
		while (scanf_s("%lf", &l) != 1)
		{
			printf("\n输入错误，请重试：");
			flush();
		}
		flush();

		printf("\n请输入宽度（不知道输入1）：");
		while (scanf_s("%lf", &w) != 1)
		{
			printf("\n输入错误，请重试：");
			flush();
		}
		flush();

		printf("\n\n长方体的体积是：%.3lf\n\n", h*l*w);
		printf("长方形的表面积是：%.3lf\n\n", (l*w + l * h + w * h) * 2);
		printf("长方形的面积是：%.3lf\n\n", l*w);
		printf("长方形的周长是：%.3lf\n\n", (l*w + l * w) * 2);
		system("pause");
	}
}


//三角函数
void cst(void)
{
	//int cst;
	

	while (1)
	{
		printf("\n\n\t\t本功能暂未完善\n\n");
		system("pause");
		break;

		/*
		system("cls && title 三角函数 && mode con cols=30 lines=15");
		printf("\n\n\t   0、返回\n");
		printf("\t   1、sin\n");
		printf("\t   2、cos\n");
		printf("\t   3、tan\n");
		printf("    请输入：");
		while (scanf_s("%d", &cst) != 1)
		{
			printf("\n输入错误！请重新输入：");
			flush();
		}
		flush();

		if (cst == 0)
			break;

		else if (cst == 1)
		{
			system("cls");
			printf("\n请输入：");
			while (scanf_s("%lf", &a) != 1)
			{
				printf("\n输入错误！请重新输入：");
				flush();
			}
			flush();
			//角度转化为弧度
			printf("%.4lf", sin(a * PAI / 180));
			system("pause");
		}

		else if (cst == 2)
			system("cls");
		else if (cst == 3)
			system("cls");

		else
		{
			printf("\n\t输入错误！");
			Sleep(1000);
		}*/
	}
}


//添加程序自启动
int addStart(void)
{
	system("title 添加软件自启动 && cls");

	int i, j;
	HKEY hKey;
	char *regpPath = { "Software\\Microsoft\\Windows\\CurrentVersion\\Run" }; //注册表启动项路径
	char path[MAXPATH] = { 0 };//软件路径
	char name[16] = { 0 };//注册表子项名称


	printf("\n请输入需要添加自启动的软件的路径\n（例如：H:\\test\\test.exe）\n ：");
	fgets(path, 1024, stdin);//输入路径
	flush();

	printf("\n\n请输入名称\n（可随便取，但不能是汉字并且在15字内）\n ：");
	fgets(name, 16, stdin);//输入名称
	flush();

	for (i = 0; i < MAXPATH; i++)//去除数组里的'\n'
	{
		if (path[i] == '\n')
			path[i] = '\0';
	}
	for(j = 0; j < 16; j++)
		if (name[j] == '\n')
			name[j] = '\0';


	//打开注册表启动项 
	if (RegOpenKeyEx(HKEY_CURRENT_USER, regpPath, 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)
	{  //添加一个子Key,并设置值
		RegSetValueEx(hKey, name, 0, REG_SZ, (BYTE *)path, strlen(path));
		RegCloseKey(hKey);//关闭注册表

		printf("\n\t         添加成功！\n注意：请不要删除已添加的程序文件，否则自启动会失效！\n\n              ");
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
void delStart(void)
{
	system("title 删除程序自启动 && cls");


	char path[110] = { "reg delete \"HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run\"  /v " };
	char name[16] = { 0 };
	int i;


	printf("\n请输入名称（15字内）：");
	fgets(name, sizeof(name), stdin);
	flush();
	
	for (i = 0; i < 16; i++)
		if (name[i] == '\n')
			name[i] = '\0';

	
	strcat_s(path, sizeof(path), name);

	puts(path);
	system(path);

	printf("\n\n\n");
	system("pause");
}