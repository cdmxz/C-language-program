#include <stdio.h>
#include <windows.h>
#include <direct.h>
#define MAXPATH 1024

int main(void)
{
	char *szSubKey = { "Software\\Microsoft\\Windows\\CurrentVersion\\Run" }; //系统启动项路径
	char copy[MAXPATH] = { "copy \"" };//copy命令复制文件
	char buffer[MAXPATH];//储存当前程序路径
	char path[MAXPATH] = { "\\电脑锁.exe\" C:\\ProgramData /y" };//目标文件夹路径
	char *pclock = { "C:\\ProgramData\\电脑锁.exe" };//注册表设置自启动路径
	
	HKEY hKey;
	int i;
	system("title 添加自启动 && mode con cols=33 lines=10"); 
	printf("\n\t输入1添加自启动\n\t输入2查看使用帮助\n\t输入3删除自启动\n\n");
	printf("请输入序号：");
	while (scanf_s("%d", &i) != 1)//处理字符输入
	{
		printf("\n输入错误！请重试：");
		rewind(stdin);
	}
	rewind(stdin);
	
	switch (i)
	{
	case 1:
	{
		system("cls");
		system("color 04");
		printf("\n\n\n\n注意：若遭到杀软拦截，请点击允许并加入白名单。\n否则软件无法正常启动！\n\n\n");
		Sleep(3000);
		_getcwd(buffer, MAXPATH);//获取当前程序的绝对路径
		strcat_s(copy,sizeof(copy), buffer);//第一次合并数组
		strcat_s(copy, sizeof(copy), path);//第二次合并数组
		system(copy);//执行复制文件命令

		//打开注册表启动项 
		if (RegOpenKeyEx(HKEY_CURRENT_USER, szSubKey, 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)
		{  //添加一个子Key,并设置值
			RegSetValueEx(hKey, "电脑锁", 0, REG_SZ, (BYTE *)pclock, strlen(pclock));
			RegCloseKey(hKey);//关闭注册表
			printf("\t\t添加成功！\n\n\t");
			system("pause"); 
			return 0;
		}
		else
		{
			printf("\t\t添加失败！\n\n\t");
			system("pause"); 
			return -1;
		}
		break;
	}

	case 2:
		MessageBox(NULL, TEXT("1、设置开机自动运行前请以管理员权限运行本软件防止权限不够造成失败。\n2、设置开机自动运行前请确保“加入自启动.exe”和“电脑锁.exe”在同一目录。\n3、如果开机自启失效，请在杀毒软件里加入自启动白名单并且重新设置自启动。"), TEXT("使用帮助"), MB_OK);
		break;

	case 3://从文件夹删除文件
	{
		system("del \"C:\\ProgramData\\电脑锁.exe\"");
		system("pause");
		break;
	}
	default:printf("\n\t\t 输入错误！"); Sleep(1000); break;
	}
	return 0;
} 
