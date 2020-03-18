#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <io.h>
#include <process.h>


#define PAI 3.14
#define SIZE 100

void Encrypt_or_decrypt(void);    //加密或解密
void enc_dec_file(char* source, char* dest);//加密或解密文件内容
int warning(void);                //当路径中有相同文件是提示是否覆盖


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


//加入自启动
int addstart(void);
//删除自启动
int delstart(void);
//去掉fgets的换行符
void fgets_n(char* n);


int  num, ch;//定义全局变量
unsigned  i;
char filename[MAX_PATH] = { 0 };   //储存待加密或待解密文件名称
char temp_filename[MAX_PATH];      //储存加密后或解密后文件名称
char c;
FILE* fpread, * fpwrite;
errno_t err;



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
			rewind(stdin);
		}
		rewind(stdin);

		if (num == 1)
			cal();

		else if (num == 2)//加密文件内容
			Encrypt_or_decrypt();

		else if (num == 3)
			addstart();

		else if (num == 4)
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


//加密或解密
void Encrypt_or_decrypt(void)
{
	unsigned len, j;

	while (1)
	{
		memset(filename, 0, MAX_PATH);//数组清零
		memset(temp_filename, 0, MAX_PATH);//数组清零

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

			fgets(filename, (MAX_PATH - 5), stdin);//输入待加密文件的名称
			fgets_n(filename);

			if (filename[0] == 34)//支持文件拖拽输入
			{
				filename[strlen(filename) - 1] = 0;
				strcpy_s(filename, strlen(filename) + 1, filename + 1);//清楚拖拽输入多出来的双引号
			}

			if (!strcmp(filename, "000"))   //输入000返回主界面
				continue;

			strcpy_s(temp_filename, MAX_PATH, filename);
			strcat_s(temp_filename, MAX_PATH, ".fh");//给加密后的文件添加后缀名
			if (warning() == 110)//判断文件是否存在
				continue;

			printf("\n正在加密：%s请稍等...", filename);//输出文件名

			enc_dec_file(filename, temp_filename);

			system("cls");
			printf("\n\n\n加密：%s成功！\n\n\t\t    ", filename);//输出文件名
			system("pause");
			continue;
		}

		else if (num == 2)//解密文件内容
		{
			system("title 解密文件内容 && mode con cols=60 lines=20 && cls");

			printf("注意：\n");
			printf("1、待解密的文件的文件名称（包括路径、标点、后缀名等）请在205字以内，大于205字会报错并闪退。\n");
			printf("2、加密后的文件如需更改文件名请保留原后缀名“.fh”。（列如：把“1.txt.fh“ 改名为 “2.txt.fh”）。\n");
			printf("3、禁止加密重要文件！禁止恶作剧！禁止干违法犯罪的事！\n");
			printf("4、输入000返回上一界面。\n\n\n\n");
			printf("请输入待解密文件的名称（列如：C:\\1.txt或1.txt.fh）：");

			fgets(filename, MAX_PATH, stdin); //输入待解密文件的名称
			fgets_n(filename);

			if (filename[0] == 34)//支持文件拖拽输入
			{
				filename[strlen(filename) - 1] = 0;
				strcpy_s(filename, strlen(filename) + 1, filename + 1);//清除拖拽输入多出来的双引号
			}
			puts(filename);

			if (!strcmp(filename, "000"))//输入000返回主界面
				continue;

			strcpy_s(temp_filename, MAX_PATH, filename);

			len = (unsigned)strlen(temp_filename) - 1;     //获取数组长度
			for (j = len; j > (len - 3); j--)//删除“.fh”后缀名
				if (temp_filename[j] == 'h' || temp_filename[j] == 'f' || temp_filename[j] == '.')
					temp_filename[j] = '\0';

			if (warning() == 110)//判断文件是否存在
				continue;

			printf("\n正在解密：%s，请稍等...", filename);//输出文件名

			enc_dec_file(filename, temp_filename);

			system("cls");
			printf("\n\n\n解密：%s成功！\n\n\t\t    ", filename);//输出文件名
			system("pause");
			continue;
		}

		else if (num == 3)//加密明文
		{
			system("title 加密明文 && mode con cols=50 lines=20 && cls");

			printf("\n\n请输入要加密的明文（50字以内，包括空格标点等）：\n");
			fgets(filename, MAX_PATH, stdin);
			fgets_n(filename);

			j = (unsigned)strlen(filename);
			for (i = 0; i <= j; i++)//加密字符串
				filename[i] += 115;

			//写入文件
			if ((err = fopen_s(&fpwrite, "加密后的密文.txt", "w")) != 0)
			{
				i = MessageBox(NULL, TEXT("创建文件失败！\n是否直接输入（可能会导致显示不完全）？"), TEXT("Error"), MB_YESNO | MB_ICONERROR);
				if (i == IDNO)
					continue;
				else if (i == IDYES)
					printf("\n\n加密后的密文是：%s\n\n", filename);
				continue;
			}

			if (fpwrite)
			{
				fputs(filename, fpwrite);//将加密后的内容写入文件
				fclose(fpwrite);
			}

			printf("\n\n请打开本程序目录中的“加密后的密文.txt”查看\n\n");
			system("start 加密后的密文.txt");
			system("pause");
		}
		else if (num == 4)//解密密文
		{
			system("title 解密密文 && mode con cols=50 lines=20 && cls");

			printf("\n\n请输入要解密的密文（50字以内，包括空格标点等）：\n");
			fgets(filename, MAX_PATH, stdin);
			fgets_n(filename);


			j = (unsigned)strlen(filename);
			for (i = 0; i <= j; i++)//加密字符串
				filename[i] -= 115;

			//写入文件
			if ((err = fopen_s(&fpwrite, "解密后的明文.txt", "w")) != 0)
			{
				i = MessageBox(NULL, TEXT("创建文件失败！\n是否直接显示（可能会导致显示不完全）？"), TEXT("Error"), MB_YESNO | MB_ICONERROR);
				if (i == IDNO)
					continue;
				else if (i == IDYES)
					printf("\n\n解密后的明文是：%s\n\n", filename);
				continue;
			}

			if (fpwrite)
			{
				fputs(filename, fpwrite);//将解密后的内容写入文件
				fclose(fpwrite);

			}
			printf("\n\n请打开本程序目录中的“解密后的明文.txt”查看\n\n");
			system("start 解密后的明文.txt");
			system("pause");
		}
		else if (num == 5)//使用说明
		{
			if ((err = fopen_s(&fpwrite, "使用说明.txt", "w")) != 0)
			{
				perror("创建“使用说明.txt”失败");
				putchar('\n');
				system("pause");
				continue;
			}

			if (fpwrite)
			{
				fputs("使用方法：\n"
					"1.文件名称输入方式：\n"
					"1、直接用鼠标把文件拖拽到程序窗口即可\n"
					"2、在输入文件名称窗口中直接输入文件路径（如：C://1.txt）\n"
					"3、在输入文件名称窗口中直接输入文件名称（如：1.txt）\n"
					"4、注意：使用第3种方式需要把本程序和待加密/待解密文件放到同一文件夹内。\n\n"
					"2.本程序理论上支持所有文件格式，但实际还待验证。\n\n"
					"3.注意事项（请仔细阅读以下内容）：\n"
					"1、加密或解密文件只会对文件内容进行加密或解密，不会对文件名称进行加密或解密。\n"
					"2、加密后的文件如需更改文件名请保留原后缀名“.fh”。（列如：把“1.txt.fh“ 改名为 “2.txt.fh”）。\n"
					"3、加密后的文件请不要更改其内容，否则解密后文件不能恢复原样！\n"
					"4、待加密文件名称（包括路径、标点、后缀名等）请在200字以内，待解密文件名称（包括路径、标点、后缀名等）请在205字以内。\n"
					"5、本软件只适合娱乐。\n"
					"6、注意：禁止使用本软件加密重要文件！禁止使用本软件干违法犯罪的事！禁止使用本软件搞恶作剧！您使用本软件造成的任何后果均不由原作者承担！", fpwrite);
				fclose(fpwrite);
			}
			system("start 使用说明.txt");
		}
		else
		{
			printf("\n\t输入错误！");
			Sleep(1000);
		}

	}
}


//加密或解密文件内容
void enc_dec_file(char* source, char* dest)
{
	FILE* fpRead, * fpWrite;
	size_t len;
	long long currentMemory, fileSize;//获取当前可用物理内存大小
	int i = 2;
	MEMORYSTATUSEX memoryStatus;
	memoryStatus.dwLength = sizeof(memoryStatus);


	if (!GlobalMemoryStatusEx(&memoryStatus))//获取当前可用的物理内存
	{
		i = MessageBox(NULL, TEXT("获取当前可用内存失败，是否继续复制（可能导致电脑死机、蓝屏）？"), TEXT("错误"), MB_YESNO | MB_ICONERROR);
		if (i == IDNO)
			exit(EXIT_FAILURE);
	}
	if (i == IDYES)
		currentMemory = 10485760;//10MB
	else
		currentMemory = memoryStatus.ullAvailPhys;


	if (fopen_s(&fpRead, source, "rb") != 0)
	{
		MessageBox(NULL, TEXT("打开源文件错误！"), TEXT("错误"), MB_OK | MB_ICONERROR);
		exit(EXIT_FAILURE);
	}


	fseek(fpRead, 0, SEEK_END);//文件指针置于文件末尾
	fileSize = _ftelli64(fpRead);//获取文件大小
	rewind(fpRead);//文件指针置于文件开头


	if (fopen_s(&fpWrite, dest, "wb") != 0)
	{
		MessageBox(NULL, TEXT("打开目标文件错误！"), TEXT("错误"), MB_OK | MB_ICONERROR);
		exit(EXIT_FAILURE);
	}


	for (; i <= 100; i++)
	{
		if (currentMemory == 10485760)
			break;

		//将当前文件大小除以 i，并判断是否大于当前可用内存，加500MB（524288000 byte）是为了给系统留500MB内存空间	
		if ((fileSize / i + 524288000) <= currentMemory && fileSize < 4294967295 && fileSize <= 1073741824)//fileSize <= 4294967295 是为了确保因类型不同造成错误。fileSize是long long类型，calloc和fread需要size_t(unsigned int)型数据，4294967295是unsigned int表示的最大范围		
			break;

		if (i == 100)
		{
			MessageBox(NULL, TEXT("内存空间不足，请关闭一些应用程序重试！"), TEXT("错误"), MB_OK | MB_ICONERROR);
			exit(EXIT_FAILURE);
		}
		fileSize /= i;//求出复制文件时malloc分配的内存空间大小
	}


	char* buffer = (char*)calloc((size_t)(fileSize + 1), sizeof(char));
	if (buffer == NULL)//如果空间不足
	{
		MessageBox(NULL, TEXT("内存空间不足，请关闭一些应用程序重试！"), TEXT("错误"), MB_OK | MB_ICONERROR);
		exit(EXIT_FAILURE);
	}


	if (fpWrite)
	{
		while ((len = fread(buffer, sizeof(char), (size_t)fileSize, fpRead)) > 0)//复制文件
		{
			for (i = 0; i <= fileSize; i++)
				buffer[i] = ~buffer[i];//对读取到内存的内容进行加密或解密


			if (fwrite(buffer, sizeof(char), strlen(buffer) + 1, fpWrite) < len)
			{
				MessageBox(NULL, TEXT("目标磁盘空间不足，请释放空间后重试！"), TEXT("错误"), MB_OK | MB_ICONERROR);
				exit(EXIT_FAILURE);
			}

		}

		fclose(fpRead);
		fclose(fpWrite);
	}
	free(buffer);
	buffer = NULL;
}


//当路径中有相同文件是提示是否覆盖
int warning(void)
{
	if (_access(temp_filename, 0) != -1)//判断需要加密或解密的文件是否在当前路径中存在
	{
		while (1)
		{
			system("cls");
			printf("\n\n\n\n%s已在当前路径中存在，是否覆盖此文件（y/n）？", temp_filename);
			rewind(stdin);
			scanf_s("%c", &c, 1);

			if (c == 'y' || c == 'Y')
				return 121;

			else if (c == 'n' || c == 'N')
				return 110;

			else
			{
				printf("\n\t\t\t输入错误！");
				Sleep(1000);
				continue;
			}
		}
	}
	return 0;
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
			rewind(stdin);
			printf("\n输入错误！请重新输入：");
		}
		rewind(stdin);


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
	double a, b, d = 0.0;
	char c;

	while (1)
	{
		system("cls && title 四则运算");

		printf("\t\t      输入0*0返回\n\n");
		printf("输入如“3*4.12”或“5.88+2.1”的四则运算式：");
		while (scanf_s("%lf %c %lf", &a, &c, 1, &b) != 3)
		{
			rewind(stdin);
			printf("\n输入错误！请重新输入：");
		}
		rewind(stdin);

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
			rewind(stdin);
			printf("\n输入错误！请重新输入：");
		}
		rewind(stdin);

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
		int count[MAX_PATH] = { 0 };
		int bin;

		printf("\t\t\t输入0返回\n\n\n");
		printf("请输入一个十进制数：");
		while (scanf_s("%lld", &dec) != 1)//处理错误输入
		{
			printf("\n输入错误,请重试：");
			rewind(stdin);
		}
		rewind(stdin);

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
	i = 0;
	long long bin, dec = 0;

	while (1)
	{
		system("title 二进制转十进制 && cls");

		printf("\n\n\n请输入一个二进制数：");
		while ((bin = getchar()) != '\n')
			dec = dec * 2 + (bin - '0');
		printf("\n\n转换为十进制的值是：%lld\n\n", dec); //输出

		i++;

		if (i == 3)
		{
			printf("\n\n\t       是否返回上一界面？（Y/N）");
			scanf_s("%c", &c, 1);
			rewind(stdin);
			if (c == 121 || c == 89)
				break;
			else if (c == 110 || c == 78)
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
	i = 0;

	while (1)
	{
		system("cls && title ASCII码转字符");

		printf("\n\n\n");
		printf("（注意：1-31和127都是控制字符）\n\n");
		printf("请输入需要转换成字符的ASCII码：");
		while (scanf_s("%d", &ascii) != 1)
		{
			printf("\n输入错误！请重新输入：");
			rewind(stdin);	//清空缓冲区
		}
		rewind(stdin);

		if (ascii == 0)
			break;

		printf("\n\n\n\n      ASCII码 %d 对应的字符为：%c\n\n", ascii, ascii);
		printf("（如显示“□”则为控制字符）\n\n");

		i++;

		if (i == 3)
		{
			printf("\n\n\t       是否返回上一界面？（Y/N）");
			scanf_s("%c", &c, 1);
			rewind(stdin);
			if (c == 121 || c == 89)
				break;
			else if (c == 110 || c == 78)
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


//字符转ascii码
void character(void)
{
	i = 0;

	while (1)
	{
		system("cls && title 字符转ASCII码");
		printf("\n\n输入需要转换成ASCII码的字符：");
		scanf_s("%c", &c, 1);
		rewind(stdin);

		printf("\n\n\n\n     字符 %c 对应的ASCII码为：%d\n\n", c, c);
		i++;

		if (i == 3)
		{
			printf("\n\n\t       是否返回上一界面？（Y/N）");
			scanf_s("%c", &c, 1);
			rewind(stdin);
			if (c == 121 || c == 89)
				break;
			else if (c == 110 || c == 78)
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
			rewind(stdin);
		}
		rewind(stdin);

		if (r == 0.0)//输入0返回
			break;

		printf("\n请输入圆柱体的高\n（计算圆的面积或周长请输入1。单位：厘米）：");
		while (scanf_s("%lf", &h) != 1)
		{
			printf("\n输入错误，请重试：");
			rewind(stdin);
		}
		rewind(stdin);

		printf("\n圆的面积为：%.3lf 平方厘米\n", PAI * r * r);
		printf("圆的周长为：%.3lf 厘米\n\n", 2 * PAI * r);
		printf("圆柱体的底面积为：  %.3lf 平方厘米\n", PAI * r * r);
		printf("圆柱体的底面周长为：%.3lf 厘米\n", 2 * PAI * r);
		printf("圆柱体的体积为：    %.3lf 立方厘米\n", PAI * r * r * h);
		printf("圆柱体的侧面积为：  %.3lf 平方厘米\n", 2 * PAI * r * h);
		printf("圆柱体的表面积为：  %.3lf 平方厘米\n\n", 2 * PAI * r * r + 2 * PAI * r * h);
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
			rewind(stdin);
		}
		rewind(stdin);

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
			rewind(stdin);
		}
		rewind(stdin);
		if (l == 0.0)
			break;

		printf("\n请输入长方形或长方体的宽（单位：厘米）：");
		while (scanf_s("%lf", &w) != 1)
		{
			printf("\n输入错误，请重试：");
			rewind(stdin);
		}
		rewind(stdin);

		printf("\n请输入长方体的高\n（只计算长方形请输入1。单位：厘米）：");
		while (scanf_s("%lf", &h) != 1)
		{
			printf("\n输入错误，请重试：");
			rewind(stdin);
		}
		rewind(stdin);

		printf("\n\n长方形的面积是：%.3lf 平方厘米\n", l * w);
		printf("长方形的周长是：%.3lf 厘米\n\n", (l * w + l * w) * 2);
		printf("长方体的体积是：  %.3lf 立方厘米\n", h * l * w);
		printf("长方体的表面积是：%.3lf 平方厘米\n\n", (l * w + l * h + w * h) * 2);
		system("pause");
	}
}

//添加程序自启动
int addstart(void)
{
	system("title 添加软件自启动 && cls");

	HKEY hKey;
	char* regPath = { "Software\\Microsoft\\Windows\\CurrentVersion\\Run" }; //注册表启动项路径
	char path[MAX_PATH] = { 0 };//需要添加自启动的软件的路径
	char name[SIZE] = { 0 };//注册表子项名称

	printf("\t    输入000返回上一界面\n\n");
	printf("请输入需要添加自启动的软件的路径\n（例如：H:\\test\\test.exe）\n ：");
	fgets(path, MAX_PATH, stdin);//输入路径
	fgets_n(path);

	if (!strcmp(path, "000"))//输入000退出
		return 0;


	printf("\n\n请输入添加到注册表的键值项名称\n\n（需要在15个字符内。\n  可以使用字符、数字、代表符、空格，\n  但不能使用“\\”）\n\n ：");
	fgets(name, SIZE, stdin);//输入名称
	fgets_n(name);

	//打开注册表启动项 
	if (RegOpenKeyEx(HKEY_CURRENT_USER, regPath, 0, KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS)
	{
		MessageBox(NULL, TEXT("添加失败！"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
		return -1;
	}
	//添加一个子Key,并设置值
	if (RegSetValueEx(hKey, name, 0, REG_SZ, (BYTE*)path, MAX_PATH) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);//关闭注册表
		MessageBox(NULL, TEXT("添加失败！"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
		return -1;
	}
	RegCloseKey(hKey);//关闭注册表

	printf("\n\t         添加成功！\n注意：请不要删除已添加的程序，否则自启动会失效！\n\n              ");
	system("pause");


	return 0;
}



//删除程序自启动
int delstart(void)
{
	system("title 删除程序自启动 && cls");
	char name[SIZE] = { 0 }; //存储名称
	HKEY hKey;
	char* regPath = { "Software\\Microsoft\\Windows\\CurrentVersion\\Run" }; //注册表启动项路径

	printf("\t    输入000返回上一界面\n\n\n");
	printf("请输入键值项名称（15个字符内）：");
	fgets(name, 31, stdin);
	fgets_n(name);

	if (!strcmp(name, "000"))
		return 0;

	//打开注册表启动项 
	if (RegOpenKeyEx(HKEY_CURRENT_USER, regPath, 0, KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS || RegDeleteValue(hKey, name) != ERROR_SUCCESS)
	{
		MessageBox(NULL, TEXT("删除失败，\n请以管理员权限运行本软件重试！"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
		return -1;
	}
	printf("\n\n");
	printf("\t\t删除成功！");
	printf("\n\n\t      ");
	system("pause");
	return 0;
}


//删除fgets读取的换行符
void fgets_n(char* n)
{
	char* find;
	find = strchr(n, '\n');  //查找'\n'
	if (find)
		*find = '\0';
}


//彩蛋
int autoshut(void)
{
	system("title 现在退出此程序或关闭电脑还来得及！");

	int i;
	char passbuffer[SIZE] = { "115-56" };//密码
	char temp[SIZE] = { '\0' };
	char FileName[MAX_PATH];
	/*HKEY hkey;
	BYTE v = 1;*/

	GetCurrentDirectoryA(MAX_PATH, FileName);//获取程序所在目录
	strcat_s(FileName, MAX_PATH, "\\hacked.jpg");


	char wanging_zero[] = { "警告：此内容不适合孕妇以及患有心脏病、高血压、癫痫、脑血管病、神经错乱、哮喘、酗酒、吸毒者等人士访问！"
		"现在退出此程序或关闭电脑还来得及！如果硬要访问，后果自负！" };

	char warning_one[] = { "警告：此内容不适合孕妇以及患有心脏病、高血压、癫痫、脑血管病、神经错乱、哮喘、酗酒、吸毒者等人士访问！"
		"现在退出此程序或关闭电脑还来得及！如果硬要访问，后果自负！\n\t\t\t\t是否继续访问？" };

	char warning_two[] = { "您的电脑中了病毒。请勿退出本程序，否则您的电脑和宝贵的数据都会嗝屁。\n如果您不想硬盘数据被格式化，昔日辛苦得来的数据成为泡影。\n"
		"如果您不想让电脑蓝屏无法启动，找维修工人上门维修，费时费力还费钱。\n如果您不想因为电脑蓝屏或重要数据丢失而耽误工作导致被老板炒鱿鱼的话，\n"
		"那就去网站https://cdmxz.github.io寻找密码吧！您只有5分钟了，加油！\n什么？你没连网？网站登不上去？那就不能怪我了，"
		"只能怪您自己运气太差咯！哈哈~\nPS：本网站和密码不收取一分钱费用，如果要钱，那就是骗子，请勿上当！" };

	if ((i = MessageBox(NULL, (warning_one), TEXT("警告！"), MB_YESNO | MB_ICONWARNING)) == IDNO)
		return 0;

	else if (i == IDYES)
	{
		system("color 04");
		for (i = 0; i >= 0; i--)
		{
			system("cls");
			printf("\n\n\n\n\n\n\n\n%s", wanging_zero);
			printf("\n\n\n     将在%d秒后进入，现在退出此程序或关闭电脑还来得及！", i);
			Sleep(1000);
		}


		system("mode con:cols=70 lines=25 && title 请勿退出本程序，否则您的电脑和宝贵的数据都会嗝屁！");//设置窗口大小 

		//URLDownloadToFileA(NULL, "https://ss0.bdstatic.com/70cFuHSh_Q1YnxGkpoWK1HF6hhy/it/u=784112224,522461964&fm=26&gp=0.jpg", FileName, 0, NULL);//下载图片
		//RegCreateKeyA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System", &hkey);
		//RegSetValueExA(hkey, "Wallpaper", 0, REG_SZ, (BYTE*)FileName, sizeof(FileName));//修改桌面背景
		//RegSetValueExA(hkey, "WallpaperStyle", 0, REG_DWORD, &v, sizeof(DWORD));//修改桌面背景
		//RegCloseKey(hkey);

		system("shutdown -f -s -t  300");//关机命令
		MessageBox(NULL, (warning_two), TEXT("温馨提示：请认真看完。"), MB_OK | MB_ICONQUESTION);

		puts(warning_two);

		printf("\n\n\n\n\n\n您的电脑中了病毒（请勿退出本程序，否则您的电脑和宝贵的数据都会嗝屁！）。\n\n");
		printf("\t\t温馨提示：你有300秒的时间输入密码。\n\n\n\t\t\t");
		system("pause");
		system("cls");

		for (i = 5; i >= 0; i--) //for循环
		{
			system("color 07");
			printf("温馨提示：你只剩下%d次机会。\n", i);
			printf("请输入密码（输入密码后别忘了按回车）:");

			scanf_s("%s", &temp, 7);
			rewind(stdin);//清除缓冲区残留字符

			if (!strcmp(temp, passbuffer))//将密码进行比较
			{
				system("color 0f && cls");
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
