#include <stdio.h>
#include <windows.h>
#include <direct.h>
#define MAXPATH 512

void copyfile(char *fileread, char *filewrite);
void flush(void);

int main(void)
{
	while(1)
	{
	system("title 添加自启动 && mode con cols=35 lines=10 && color 07"); 
	char *szSubKey = { "Software\\Microsoft\\Windows\\CurrentVersion\\Run" }; //系统启动项路径
	char *del = { "Software\\Microsoft\\Windows\\CurrentVersion\\Run\\电脑锁" };
	char folderName[] = { "C:\\Program Files\\电脑锁" };//创建文件夹的路径
	char buffer[MAXPATH];//储存当前程序路径
	
	//注册表自启动路径以及要复制到的路径
	char *pc_lock = { "C:\\Program Files\\电脑锁\\电脑锁.exe" };  
	
	HKEY hKey;
	int i;
	
	_getcwd(buffer, MAXPATH);//获取当前程序所在目录
		
	printf("\n\t  输入1添加自启动\n\n\t  输入2查看使用帮助\n\n\t  输入3删除自启动\n\n");
	printf("请输入序号：");
	while (scanf_s("%d", &i) != 1)//处理字符输入
	{
		printf("\n输入错误！请重试：");
		flush(); 
	}
	flush(); 
	
	
	if(i == 1)
	{
		system("color 04 && cls");
		printf("\n\n注意：若遭到杀软拦截，请点击允许并加入白名单。\n否则软件无法正常启动！\n\n");
		Sleep(3000);
		
		if (_access(folderName, 0))//判断文件夹是否存在
			if (_mkdir(folderName))	   //不存在则创建文件夹
			{
				MessageBox(NULL, TEXT("创建目录失败！请以管理员权限重试！"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
				exit(EXIT_FAILURE);
			}

		strcat_s(buffer, MAXPATH, "\\电脑锁.exe");//复制前的路径
		copyfile(buffer, pc_lock);//复制文件

		//打开注册表启动项 
		if (RegOpenKeyEx(HKEY_CURRENT_USER, szSubKey, 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)
		{  //添加一个子Key,并设置值
			RegSetValueEx(hKey, "电脑锁", 0, REG_SZ, (BYTE *)pc_lock, strlen(pc_lock));
			RegCloseKey(hKey);//关闭注册表
			printf("\t    添加成功！\n\n\t  ");
			system("pause"); 
		}
		else
		{
			printf("\t\t添加失败！\n\n\t");
			system("pause"); 
			return -1;
		}
	
	}

	else if(i == 2)
		MessageBox(NULL, TEXT("1、设置开机自动运行前请以管理员权限运行本软件防止权限不够造成失败。\n2、设置开机自动运行前请确保“加入自启动.exe”和“电脑锁.exe”在同一目录。\n3、如果开机自启失效，请在杀毒软件里加入自启动白名单并且重新设置自启动。"), TEXT("使用帮助"), MB_OK);

	else if(i == 3)//从文件夹删除文件
	{
		system("cls && title 删除自启动");
		printf("\n\n\n\n注意：若遭到杀软拦截，请点击允许并加入白名单！\n\n\n");
		Sleep(3000);
		system("cls && mode con cols=53 lines=20 && title 正在删除...");

		//结束进程
		printf("\n正在结束电脑锁的进程...\n");
		Sleep(2000);
		system("taskkill /f /t /im 电脑锁.exe");
		
		//删除文件
		printf("\n\n正在删除文件...");
		if (remove(pc_lock))
			printf("\n删除电脑锁.exe失败！\n\n");
		else
			printf("\n删除电脑锁.exe成功！\n\n");
		
		//删除目录
		printf("\n正在删除安装目录...\n");
		system("rd /s /q C:\\\"Program Files\"\\电脑锁");
		
		//删除注册表
		printf("\n\n正在删除电脑锁注册表启动项...\n");
		system("reg delete \"HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run\"  /v 电脑锁 /f");
		
		putchar('\n');
		system("pause");
	}
	else
	{
		printf("\n\t\t 输入错误！"); 
		Sleep(1000); 
	}
}
	
	return 0;
} 

//复制文件
void copyfile(char *fileread, char *filewrite)
{
	FILE *fpread;   //复制前文件的路径
	FILE *fpwrite;  //复制后文件的路径
	errno_t err;
	int ch; 
	
	
	if ((err = fopen_s(&fpread, fileread, "rb")) != 0 )//以二进制方式打开文件
	{
		MessageBox(NULL, TEXT("无法打开文件！\n请检查“电脑锁.exe”和“加入自启动.exe”是否在同一目录。\n"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
		exit(EXIT_FAILURE);
	}
	if ((err = fopen_s(&fpwrite, filewrite, "wb")) != 0)
	{
		MessageBox(NULL, TEXT("无法复制文件！请以管理员权限重试！\n"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
		exit(EXIT_FAILURE);
	}

	do
	{
		ch = fgetc(fpread);
		fputc(ch, fpwrite);
	} while (ch != EOF);

	fclose(fpread);//关闭文件
	fclose(fpwrite);
}


//清除缓冲区
void flush(void)
{
	int ch;
	while ((ch = getchar()) != '\n' && ch != EOF)
		;
}
