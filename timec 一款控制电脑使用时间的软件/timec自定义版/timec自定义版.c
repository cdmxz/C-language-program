#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <direct.h>
#include <io.h>

#define MAXPATH 1024
#define SIZE 6

void pass(void);     //验证密码
void shut(void);     //自定义关机
void hwn(void);      //隐藏窗口
void countDown(void);//弹出提示框
int hyke(void);      //注册表
void flush(void);    //清除缓冲区
//复制文件
int copyfile(char *fileread, char *filewrite);
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
		case 8:system("start readme.txt"); break;
		case 9:hyke(); break;
		case 10:exit(0); break;
		}
	}

	return 0;
}



void pass(void)//密码验证
{
	system("title 如忘记密码，请输入1234修改。 && cls");

	int i;
	FILE *fpwrite, *fpread;
	errno_t err;
	char pass[] = { "123456" };//储存默认密码
	char newpass[7] = { 0 };//储存新密码
	char tmpPass[7] = { 0 };//临时储存密码
	char change[] = { "1234" };


	printf("       温馨提示：如忘记密码，请输入1234修改\n\n\n\n\n\n");
	printf("如要取消关机，请输入密码：");
	
	scanf_s("%s",tmpPass,7);//输入当前密码
	flush();


	if (!strcmp(tmpPass, change))//将输入的密码进行比较
	{
		system("title 默认密码可在介绍文件中查找 && cls");
		
		printf("       温馨提示：默认密码可在介绍文件中查找\n\n\n\n\n\n");
		printf("请输入默认密码（6位数）：");
		
		scanf_s("%s", &tmpPass, 7);//输入默认密码
		flush();
		
		
		if (!strcmp(tmpPass, pass))//将输入的密码与默认密码进行比较
		{
			while (1)
			{
				printf("\n\n请输入新密码（6位数）：");
				
				scanf_s("%s", &newpass, 7);//输入新密码
				flush();
				
				
				if (strlen(newpass) < SIZE)//判断新密码长度
				{
					printf("\n\n输入的密码太长或太短（需要6位数字）");
					flush();//清除缓冲区
				}
				else
					break;
			}
			
			if ((err = fopen_s(&fpwrite,"passwd.dat", "w")) != 0)//打开记录密码的文件,并且判断文件是否正常打开
			{
				MessageBox(NULL, TEXT("密码文件创建错误！\n请以管理员权限运行重试！"), TEXT("error!"), MB_OK | MB_ICONERROR);//弹出提示框
				exit(1);
			}
			
			for (i = 0; i <= 6;i++)  //把新密码写入文件
			{
				newpass[i] = newpass[i] - 't' + 'i' - 'm' / 'e';//加密文件内容
				fputc(newpass[i], fpwrite);//输出加密后的内容到文件
			}

			if (fclose(fpwrite) == EOF)//关闭文件
			{
				MessageBox(NULL, TEXT("文件写入错误！\n请检查是否有其它程序占用！\n或者重试！"), TEXT("error!"), MB_OK | MB_ICONERROR);
				exit(1);
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
		while (1)
		{
			if ((err = fopen_s(&fpread, "passwd.dat", "r")) != 0)//打开记录密码的文件,并且判断文件是否正常打开
			{
				MessageBox(NULL, TEXT("密码文件读取错误！\n请检查程序本目录是否有“passwd.dat”文件。如果没有，请修改密码。"), TEXT("error!"), MB_OK | MB_ICONERROR);//弹出提示框
				break;
			}


			for (i = 0; i <= 6; i++)  //从文件中读取内容到ch。EOF是文件结束标志。
			{
				newpass[i] = fgetc(fpread);//从文件读取密码
				newpass[i] = newpass[i] + 't' - 'i' + 'm' * 'e';//解密文件内容并存到数组
			}

			if (!strcmp(tmpPass, newpass))//将输入的密码与数组中的密码进行比较
				system("shutdown -a");
			else
			{
				printf("\n\n\t\t密码错误！");
				Sleep(1000);
			}


			fclose(fpread); //关闭文件
			
			break;
		}//while
	}//else
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

	strcat_s(shut,sizeof(shut), tmp);//合并数组
	
	system(shut);//执行关机命令
	Sleep(2000);
	exit(0);
}


//添加注册表
int hyke(void)
{
	system("cls");

	char *szSubKey = { "Software\\Microsoft\\Windows\\CurrentVersion\\Run" }; //系统启动项路径
	char *del = { "Software\\Microsoft\\Windows\\CurrentVersion\\Run\\timecyincang" };

	char folderName[] = { "C:\\Program Files\\timec" };//创建文件夹路径

	char copy_Y[] = { "\\timec自定义版.exe" };
	char copy_Z[] = { "\\timec自定义版.exe" };

	char buffer[MAXPATH];//储存当前程序路径
	
	char *timec_Y = { "C:\\Program Files\\timec隐藏版.exe" };  //注册表自启动路径
	char *timec_Z = { "C:\\Program Files\\timec自定义版.exe" };//注册表自启动路径

	char path_Y[] = { "C:\\Program Files\\timec\\timec隐藏版.exe" };//timec隐藏版 复制的路径
	char path_Z[] = { "C:\\Program Files\\timec\\timec自定义版.exe" };//timec自定义版 复制的路径

	_getcwd(buffer, MAXPATH);//获取当前程序的绝对路径

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
		system("color 04 && cls");

		printf("\n\n\n\n注意：若遭到杀软拦截，请点击允许并加入白名单。\n\n否则软件无法正常启动！\n\n\n");
		Sleep(3000);
		
		if (_access(folderName, 00) == -1)//判断文件夹是否存在
		if (_mkdir(folderName) == -1)			//不存在则创建文件夹
		{
			MessageBox(NULL, TEXT("创建目录失败！请以管理员权限重试！"), TEXT("Error"), MB_OK | MB_ICONERROR);
			exit(1);
		}

		strcat_s(buffer, sizeof(buffer), copy_Y);//复制前的路径
		copyfile(buffer, path_Y);//复制文件

		//打开注册表启动项 
		if (RegOpenKeyEx(HKEY_CURRENT_USER, szSubKey, 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)
		{  //添加一个子Key,并设置值
			RegSetValueEx(hKey, "timecyincang", 0, REG_SZ, (BYTE *)timec_Y, strlen(timec_Y));
			RegCloseKey(hKey);//关闭注册表
			
			printf("\t\t    添加成功！\n\n\t          ");
			system("pause");
			return 0;
		}
		else
		{
			MessageBox(NULL, TEXT("添加失败！"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
			return -1;
		}
		break;
	}

	case 2:
	{

		//此处和上处源代码基本相同，故不写注释。其实还是懒啦~
		system("color 04 && cls");
		
		printf("\n\n\n\n注意：若遭到杀软拦截，请点击允许并加入白名单，\n\n否则软件无法正常启动！\n\n\n");
		Sleep(3000);

		if (_access(folderName, 00) == -1)//判断文件夹是否存在
			if (_mkdir(folderName) == -1)			//不存在则创建文件夹
			{
				MessageBox(NULL, TEXT("创建目录失败！请以管理员权限重试！"), TEXT("Error"), MB_OK | MB_ICONERROR);
				exit(1);
			}


		strcat_s(buffer, sizeof(buffer), copy_Z);//复制前的路径
		copyfile(buffer, path_Z);//复制文件

		if (RegOpenKeyEx(HKEY_CURRENT_USER, szSubKey, 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)
		{
			RegSetValueEx(hKey, "timeczidingyi", 0, REG_SZ, (BYTE *)timec_Z, strlen(timec_Z));
			RegCloseKey(hKey);
			
			printf("\t\t    添加成功！\n\n\t          ");
			system("pause");
			return 0;
		}
		else
		{
			MessageBox(NULL, TEXT("添加失败！"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
			return -1;
		}
		break;
	}

	case 3:
		MessageBox(NULL, TEXT("1、设置开机自动运行前请以管理员权限运行本软件\n防止权限不够造成失败。\n2、设置开机自动运行前请确保“timec隐藏版”和\n“timec自定义版”在同一目录。\n3、设置开机自动运行前请确保“timec隐藏版”和\n“timec自定义版”的原文件名未被修改。\n4、如果开机自启失效，请查看“C:\\Program Files\\time”目录下有没有\n“timec自定义版 ”或“timec隐藏版”这个软件\n或者检查软件自启权限是否被杀软禁掉了。\n5、在当前页面输入4删除自启动。\n6、6、使用本软件产生的任何后果均不由原作者承担\n。一经使用本软件，视为同意此条款。\n注：本条款若有任何更新，恕不另行通知！"), TEXT("使用帮助"), MB_OK);
		break;

	case 4://从文件夹删除文件
	{
		passwd();//验证密码
		system("cls");
		printf("\n\n\n\n注意：若遭到杀软拦截，请点击允许并加入白名单，\n\n否则软件无法正常启动！\n\n\n");
		Sleep(3000);
		system("cls");

		//删除文件
		if (remove("C:\\Program Files\\time\\timec隐藏版.exe"))
			printf("\n\n删除timec隐藏版失败！\n\n如未添加timec隐藏版自启动可不理会。\n\n");
		else
			printf("\n\n删除timec隐藏版成功！\n\n\n");

		if (remove("C:\\Program Files\\timec\\timec自定义版.exe"))
			printf("\n\n删除timec自定义版失败！\n如未添加timec自定义版自启动可不理会。\n\n");
		else
			printf("\n\n删除timec自定义版成功！\n\n\n");

		system("rd /s /q C:\\\"Program Files\"\\timec");//删除目录
		system("pause");
		
		//删除注册表
		system("reg delete \"HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run\"  /v timecyincang /f");
		system("reg delete \"HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run\"  /v timeczidingyi /f");
		
		break;
	}
	default:printf("\n\t\t 输入错误！"); Sleep(1000); break;
	}
	return 0;
}


//清除缓冲区
void flush(void)
{
	int ch;
	while ((ch = getchar()) != '\n'&&ch != EOF)
		;
}


//复制文件
int copyfile(char *fileread, char *filewrite)
{
	FILE *fpread;   //指向复制前的文件路径
	FILE *fpwrite;  //指向复制后的文件路径
	errno_t err;

	int   bufflen = 1024 * 4;  //缓冲区长度
	char* buffer = (char*)malloc(bufflen); //开辟缓存
	int   readbyte;  //读取的字节数
	

	if ((err = fopen_s(&fpread, fileread, "rb")) != 0 )//以二进制方式打开文件
	{
		printf("无法打开文件！\n");
		system("pause");
		return 1;
	}
	if ((err = fopen_s(&fpwrite, filewrite, "wb")) != 0)
	{
		printf("无法复制文件！请以管理员权限重试！");
		system("pause");
		return 1;
	}

	//不断从fileread读取内容，放在缓冲区
	while ((readbyte = fread(buffer, 1, bufflen, fpread)) > 0)
		fwrite(buffer, readbyte, 1, fpwrite);	//将缓冲区的内容写入fileWrite
	

	free(buffer);//释放给指针变量分配的内存空间
	fclose(fpread);//关闭文件
	fclose(fpwrite);
	return 0;
}


void passwd(void)//删除自启动验证密码
{
	FILE *fpread;
	errno_t err;

	int i;
	char newPass[7] = { 0 };//储存新密码
	char tmpPass[7] = { 0 };//临时储存密码

	while (1)
	{
		system("cls && title 删除自启动项需要输入正确的密码");

		printf("\n\t\t  输入1234退出。\n         如忘记密码，请在主界面输入0更改。");
		printf("\n\n\n删除自启动项需要输入密码。\n\n请输入正确的密码：");
		scanf_s("%s", tmpPass, 7);
		flush();

		if (!strcmp(tmpPass, "1234"))//输入1234退出
			exit(0);

		if ((err = fopen_s(&fpread, "passwd.dat", "r")) != 0)//打开记录密码的文件,并且判断文件是否正常打开
		{
			MessageBox(NULL, TEXT("密码文件读取错误！\n请检查程序本目录是否有“passwd.dat”文件。如果没有，请修改密码。"), TEXT("error!"), MB_OK | MB_ICONERROR);//弹出提示框
			break;
		}

		for (i = 0; i <= 6; i++)  //从文件中读取内容到ch。EOF是文件结束标志。
		{
			newPass[i] = fgetc(fpread);//从文件读取密码
			newPass[i] = newPass[i] + 't' - 'i' + 'm' * 'e';//解密文件内容并存到数组
		}

		if (!strcmp(tmpPass, newPass))//将输入的密码与数组中的密码进行比较
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
