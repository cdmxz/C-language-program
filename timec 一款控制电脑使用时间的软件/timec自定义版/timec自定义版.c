#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <direct.h>
#include <io.h>

#define MAXPATH 512
#define SIZE 6

int  pass(void);     //验证密码
void shut(void);     //自定义关机
void open_readmetxt(void); //查看使用说明
void hwn(void);      //隐藏窗口
void countDown(void);//弹出提示框
int  hyke(void);     //注册表
void flush(void);    //清除缓冲区
//复制文件
int  copyfile(char *fileread, char *filewrite);
void passwd(void);

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
		printf("\t\t 8、查看使用说明\n");
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
		case 0:pass(); break;
		case 1:system("shutdown -s -t 600"); hwn(); countDown(); break;
		case 2:system("shutdown -s -t 1800"); hwn(); Sleep(1200000); countDown(); break;
		case 3:system("shutdown -s -t 3000"); hwn(); Sleep(2400000); countDown(); break;
		case 4:system("shutdown -s -t 3600"); hwn(); Sleep(3000000); countDown(); break;
		case 5:system("shutdown -s -t 4800"); hwn(); Sleep(4200000); countDown(); break;
		case 6:system("shutdown -s -t 6000"); hwn(); Sleep(5400000); countDown(); break;
		case 7:shut(); break;
		case 8:open_readmetxt(); break;
		case 9:hyke(); break;
		case 10:exit(EXIT_SUCCESS); break;
		}
	}

	return 0;
}



int pass(void)//密码验证
{
	system("title 如忘记密码，请输入1234修改。 && cls");

	int i;
	FILE *fpwrite, *fpread;
	errno_t err;
	char pass[] = { "123456" };//储存默认密码
	char newpass[7] = { 0 };//储存新密码
	char tmppass[7] = { 0 };//临时储存密码


	printf("       温馨提示：如忘记密码，请输入1234修改\n\n\n\n\n\n");
	printf("如要取消关机，请输入密码：");
	
	scanf_s("%s",tmppass,7);//输入当前密码
	flush();


	if (!strcmp(tmppass, "1234"))//将输入的密码进行比较
	{
		system("title 默认密码可在介绍文件中查找 && cls");
		
		printf("       温馨提示：默认密码可在介绍文件中查找\n\n\n\n\n\n");
		printf("请输入默认密码（6位数）：");
		
		scanf_s("%s", &tmppass, 7);//输入默认密码
		flush();
		
		
		if (!strcmp(tmppass, pass))//将输入的密码与默认密码进行比较
		{
			while (1)
			{
				printf("\n\n请输入新密码（6位数）：");
				
				scanf_s("%s", &newpass, 7);//输入新密码
				flush();
				
				
				if (strlen(newpass) < SIZE)//判断新密码长度
					printf("\n\n输入的密码太长或太短（需要6位数字）。");
				else
					break;
			}
			
			if ((err = fopen_s(&fpwrite,"passwd.dat", "w")) != 0)//打开记录密码的文件,并且判断文件是否正常打开
			{
				MessageBox(NULL, TEXT("密码文件创建错误！\n请以管理员权限运行重试！"), TEXT("ERROR"), MB_OK | MB_ICONERROR);//弹出提示框
				exit(EXIT_FAILURE);
			}
			
			for (i = 0; i <= 6;i++)  //把新密码写入文件
			{
				newpass[i] = newpass[i] - 't' + 'i' - 'm' / 'e';//加密文件内容
				fputc(newpass[i], fpwrite);//输出加密后的内容到文件
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

		if ((err = fopen_s(&fpread, "passwd.dat", "r")) != 0)//打开记录密码的文件,并且判断文件是否正常打开
		{
			MessageBox(NULL, TEXT("密码文件读取错误！\n请检查程序本目录是否有“passwd.dat”文件。如果没有，请修改密码。"), TEXT("ERROR"), MB_OK | MB_ICONERROR);//弹出提示框
			return 1;
		}


		for (i = 0; i <= 6; i++)  //从文件中读取内容到ch。EOF是文件结束标志。
		{
			newpass[i] = fgetc(fpread);//从文件读取密码
			newpass[i] = newpass[i] + 't' - 'i' + 'm' * 'e';//解密文件内容并存到数组
		}

		if (!strcmp(tmppass, newpass))//将输入的密码与数组中的密码进行比较
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
void open_readmetxt(void)
{
	char path[MAXPATH];

	_getcwd(path, MAXPATH);//获取文件路径
	strcat_s(path, MAXPATH, "\\readme.txt");
	if (_access(path, 0))//判断“readme.txt”是否存在。不存在则打开网址，存在则直接打开文件
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


void countDown(void)
{
	MessageBox(NULL, TEXT("您的电脑还有10分钟就要关机了\n请做好关机准备！"), TEXT("温馨提示"), MB_OK | MB_ICONWARNING);
}



//自定义关机
void shut(void)
{
	char shut[25] = { "shutdown -s -t " };
	char tmp[7] = { 0 };
	system("cls");
	printf("\n注意：目前自定义时间关机功能不会在关机前弹出提示框,");
	printf("请注意查看系统提示，并且最大时间不能超过99999秒。\n\n\n\n\n");
	printf("温馨提示：只需输入数字即可，不必输入单位\n");
	printf("\t  请输入多少秒后关机（单位：秒）：");
	
	scanf_s("%s", tmp ,7);
	flush();

	strcat_s(shut, 25, tmp);//合并数组
	
	system(shut);//执行关机命令
	Sleep(2000);
	exit(EXIT_SUCCESS);
}



//添加注册表
int hyke(void)
{
	system("cls && title 添加自启动 PS：输入7返回主界面");

	char *szSubKey = { "Software\\Microsoft\\Windows\\CurrentVersion\\Run" }; //系统启动项路径
	char *del = { "Software\\Microsoft\\Windows\\CurrentVersion\\Run\\timecyincang" };
	char folderName[] = { "C:\\Program Files\\timec" };//创建文件夹的路径
	char buffer[MAXPATH];//储存当前程序路径
	
	//注册表自启动路径以及要复制到的路径
	char *timec_Y = { "C:\\Program Files\\timec\\timec隐藏版.exe" };  
	char *timec_Z = { "C:\\Program Files\\timec\\timec自定义版.exe" };

	_getcwd(buffer, MAXPATH);//获取当前程序路径
	
	HKEY hKey;
	int i;


	printf("\n\t输入3查看使用帮助，输入4删除自启动\n\n");
	printf("请输入要添加自启动的版本：\n\n");
	printf("\t（1.timec隐藏版 2.timec自定义版）\n\n\n\n");
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
		system("color 04 && cls");

		printf("\n\n\n\n注意：若遭到杀软拦截，请点击允许并加入白名单。\n\n否则软件无法正常启动！\n\n\n");
		Sleep(3000);

		if (_access(folderName, 0))//判断文件夹是否存在
		if (_mkdir(folderName))	   //不存在则创建文件夹
		{
			MessageBox(NULL, TEXT("创建目录失败！请以管理员权限重试！"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
			exit(EXIT_FAILURE);
		}

		strcat_s(buffer, MAXPATH, "\\timec隐藏版.exe");//复制前的路径
		copyfile(buffer, timec_Y);//复制文件
		
		SetFileAttributes(timec_Y, FILE_ATTRIBUTE_HIDDEN);//设置为隐藏文件
		SetFileAttributes(folderName, FILE_ATTRIBUTE_HIDDEN);//设置为隐藏文件

		//打开注册表启动项 
		if (RegOpenKeyEx(HKEY_CURRENT_USER, szSubKey, 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)
		{  //添加一个子Key,并设置值
			RegSetValueEx(hKey, "timecyincang", 0, REG_SZ, (BYTE *)timec_Y, strlen(timec_Y));
			RegCloseKey(hKey);//关闭注册表
			
			printf("\t\t    添加成功！\n\n\t         ");
			system("pause");
			return 0; 
		}
		else
		{
			MessageBox(NULL, TEXT("添加失败！"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
			remove("C:\\Program Files\\timec\\timec隐藏版.exe");
			return 1;
		}
		break;
	}

	case 2:
	{

		//此处和上处源代码基本相同，故不写注释。其实还是懒啦~
		system("color 04 && cls");
		
		printf("\n\n\n\n注意：若遭到杀软拦截，请点击允许并加入白名单，\n\n否则软件无法正常启动！\n\n\n");
		Sleep(3000);

		if (_access(folderName, 0))//判断文件夹是否存在
			if (_mkdir(folderName))			//不存在则创建文件夹
			{
				MessageBox(NULL, TEXT("创建目录失败！请以管理员权限重试！"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
				exit(EXIT_FAILURE);
			}
		SetFileAttributes(timec_Z, FILE_ATTRIBUTE_HIDDEN);//设置为隐藏文件
		SetFileAttributes(folderName, FILE_ATTRIBUTE_HIDDEN);//设置为隐藏文件

		strcat_s(buffer, MAXPATH, "\\timec自定义版.exe");//复制前的路径
		copyfile(buffer, timec_Z);//复制文件

		if (RegOpenKeyEx(HKEY_CURRENT_USER, szSubKey, 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)
		{
			RegSetValueEx(hKey, "timeczidingyi", 0, REG_SZ, (BYTE *)timec_Z, strlen(timec_Z));
			RegCloseKey(hKey);
			
			printf("\t\t    添加成功！\n\n\t         ");
			system("pause");
			return 0;
		}
		else
		{
			MessageBox(NULL, TEXT("添加失败！"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
			remove("C:\\Program Files\\timec\\timec自定义版.exe");
			return 1;
		}
		break;
	}

	case 3:
		MessageBox(NULL, TEXT("1、设置开机自动运行前请以管理员权限运行本软件\n防止权限不够造成失败。\n2、设置开机自动运行前请确保“timec隐藏版”和\n“timec自定义版”在同一目录。\n3、设置开机自动运行前请确保“timec隐藏版”和\n“timec自定义版”的原文件名未被修改。\n4、如果开机自启失效，请查看“C:\\Program Files\\time”目录下有没有\n“timec自定义版 ”或“timec隐藏版”这个软件\n或者检查软件自启权限是否被杀软禁掉了。\n5、在当前页面输入4删除自启动。\n6、开启自启动会对修改系统注册表，\n可能会造成电脑不稳定，请慎用！\n7、使用本软件产生的任何后果均不由原作者承担\n。一经使用本软件，视为同意此条款。\n注：本条款若有任何更新，恕不另行通知！"), TEXT("使用帮助"), MB_OK);
		break;

	case 4://从文件夹删除文件
	{
		passwd();//验证密码
		system("cls");
		printf("\n\n\n\n\n\n注意：若遭到杀软拦截，请点击允许并加入白名单！\n\n\n");
		Sleep(3000);
		system("cls && mode con cols=53 lines=30 && title 正在删除...");

		//结束进程
		printf("\n正在结束timec隐藏版的进程...\n");
		Sleep(2000);
		system("taskkill /f /t /im timec隐藏版.exe");
		
		//删除程序
		printf("\n\n正在删除文件...");
		if (remove("C:\\Program Files\\timec\\timec隐藏版.exe"))
			printf("\n删除timec隐藏版失败！\n如未添加timec隐藏版自启动可不理会。\n\n");
		else
			printf("\n删除timec隐藏版成功！\n\n");

		if (remove("C:\\Program Files\\timec\\timec自定义版.exe"))
			printf("删除timec自定义版失败！\n如未添加timec自定义版自启动可不理会。\n\n");
		else
			printf("删除timec自定义版成功！\n\n");

		//删除配置文件
		printf("\n正在删除配置文件...\n");
		if (remove("C:\\Users\\Timec_config.txt"))
			printf("删除配置文件失败！\n请手动进入“C:\\Users”目录删除“Timec_config.txt”。\n\n");
		else
			printf("删除配置文件成功！\n\n");

		//删除目录
		printf("\n正在删除安装目录...\n");
		system("rd /s /q C:\\\"Program Files\"\\timec");
		
		//删除注册表
		printf("\n\n正在删除timec隐藏版注册表启动项...\n");
		system("reg delete \"HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run\"  /v timecyincang /f");
		printf("\n正在删除timec自定义版注册表启动项...\n");
		system("reg delete \"HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run\"  /v timeczidingyi /f");
		
		putchar('\n');
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
	
	
	if ((err = fopen_s(&fpread, fileread, "rb")) != 0 )//以二进制方式打开文件
	{
		MessageBox(NULL, TEXT("无法复制文件！\n请检查“timec自定义版.exe”和“timec隐藏版.exe”是否在同一目录。\n"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
		exit(EXIT_FAILURE);
	}

	if ((err = fopen_s(&fpwrite, filewrite, "wb")) != 0)
	{
		//MessageBox(NULL, TEXT("无法复制文件！请以管理员权限重试！\n"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
		perror("Error");
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


void passwd(void)//删除自启动验证密码
{
	FILE *fpread;
	errno_t err;

	int i;
	char newpass[7] = { 0 };//储存新密码
	char tmppass[7] = { 0 };//临时储存密码

	while (1)
	{
		system("cls && title 删除自启动项需要输入正确的密码");

		printf("\n\t\t  输入1234退出。\n         如忘记密码，请在主界面输入0更改。");
		printf("\n\n\n删除自启动项需要输入密码。\n\n请输入正确的密码：");
		scanf_s("%s", tmppass, 7);
		flush();

		if (!strcmp(tmppass, "1234"))//输入1234退出
			exit(EXIT_SUCCESS);

		if ((err = fopen_s(&fpread, "passwd.dat", "r")) != 0)//打开记录密码的文件,并且判断文件是否正常打开
		{
			MessageBox(NULL, TEXT("密码文件读取错误！\n请检查程序本目录是否有“passwd.dat”文件。如果没有，请修改密码。"), TEXT("ERROR"), MB_OK | MB_ICONERROR);//弹出提示框
			exit(EXIT_SUCCESS);
		}

		for (i = 0; i <= 6; i++)  //从文件中读取内容到ch。EOF是文件结束标志。
		{
			newpass[i] = fgetc(fpread);//从文件读取密码
			newpass[i] = newpass[i] + 't' - 'i' + 'm' * 'e';//解密文件内容并存到数组
		}

		if (!strcmp(tmppass, newpass))//将输入的密码与数组中的密码进行比较
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
