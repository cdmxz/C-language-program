#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <time.h>
#include <io.h>
#include <direct.h>
#include <stdbool.h>
#include <TlHelp32.h>
#include <process.h>
#include <shlobj.h>
#pragma comment(lib, "Shell32.lib")

void SplitStr(const char* str, int* X, int* Y, char* RGB, int RGBSize);
void OpenDingDing(void);
//unsigned int __stdcall Start(PVOID p);
void WriteLog(const char* err);
void GetMousePos(char* const currRGB);
bool GetProcess(const char* pName);
void Add(void);
void Del(void);
void Create(void);
void Deln(char* str);

char runPath[MAX_PATH] = { 0 };//钉钉安装路径
char logPath[MAX_PATH] = { 0 };//日志文件路径
char RGBFilePath[MAX_PATH] = { 0 };//自定义RGB文件路径
char FileContent[MAX_PATH] = { 0 };//自定义RGB文件内容
char RGB[] =//自带RGB数据
{ "214,109 255,146,62\n"
"224,120 255,148,62\n"
"231,118 255,148,62\n"
"219,115 255,148,99\n"
"234,119 255,148,62\n"
"237,119 255,148,62\n"
"213,115 255,148,62\n"
"83,120 255,185,132\n"
"83,111 255,185,132\n"
"83,114 255,167,62\n"
"77,117 255,203,99\n" };

int main(int argc, char* argv[])
{
	////查找窗口句柄
	//HWND hwnd = FindWindowA(NULL, "钉钉");

	////激活显示钉钉窗口
	//SetForegroundWindow(hwnd);

	////获取钉钉窗口坐标
	//RECT re;
	//GetWindowRect(hwnd, &re);
	//
	//printf("x %ld,y %ld", re.left, re.top);
	//system("pause");
	//return 0;

	system("title 自动进入钉钉直播间");
	printf("命令：自动进入钉钉直播间.exe  <参数（可选）>\n"
		"参数：\n"
		"1、无参数 （启动钉钉进入直播间）\n"
		"2、xy    （显示鼠标坐标和颜色）\n"
		"3、add    （添加自启动）\n"
		"4、del    （删除自启动）\n"
		"5、cre    （创建自定义RGB文件模板）\n\n");


	char deskPath[MAX_PATH];
	SHGetSpecialFolderPathA(0, deskPath, CSIDL_DESKTOPDIRECTORY, 0);//获取桌面路径
	sprintf_s(logPath, MAX_PATH, "%s\\自动进入钉钉直播间.log", deskPath);//获取日志文件路径
	sprintf_s(RGBFilePath, MAX_PATH, "%s\\自定义RGB.txt", deskPath);//获取自定义颜色文件路径

	if (argc == 2)
	{
		if (strcmp(argv[1], "xy") == 0)
			GetMousePos(NULL);//显示鼠标坐标和颜色
		else if (strcmp(argv[1], "add") == 0)
			Add();//添加自启动
		else if (strcmp(argv[1], "del") == 0)
			Del();//删除自启动
		else if (strcmp(argv[1], "cre") == 0)
			Create();
	}


	//system("mode con cols=45 lines=15");//设置窗口大小


	OpenDingDing();//打开钉钉
	Sleep(3000);//等待钉钉启动
	HWND _hwnd = FindWindowA(NULL, "钉钉");//获取钉钉窗口句柄
	if (!_hwnd)
	{
		WriteLog("获取钉钉窗口句柄失败！");
		exit(EXIT_FAILURE);
	}


	printf("获取钉钉窗口句柄成功\n");

	//寻找钉钉进程
	for (int i = 1; i <= 10; i++)
	{
		if (GetProcess("DingTalk.exe") == true)
			break;

		Sleep(3000);//未找到进程就等待2秒重试

		if (i == 10)//如果重试20次都未找到
		{
			WriteLog("未找到钉钉进程");
			exit(EXIT_FAILURE);
		}
	}


	printf("找到钉钉进程\n");

	//将钉钉窗口移动到屏幕左上角并设置窗口高度和宽度
	MoveWindow(_hwnd, 0, 0, 930, 640, true);

	printf("移动钉钉窗口到左上角\n");

	FILE* fp;
	unsigned i = 0;
	int ch, x, y, color = 0, j = 0;
	char currentRGB[20] = { 0 }, fileRGB[20] = { 0 }, tmp[25] = { 0 };

	if (!_access(RGBFilePath, 0))//判断文件是否存在
	{

		printf("找到自定义RGB文件\n");

		if (fopen_s(&fp, RGBFilePath, "r") == 0)//判断是否正确打开文件
		{
			if (_filelength(_fileno(fp)))//文件不为空
			{
				//读取文件内容
				for (i = 0; i < strlen(FileContent) + 1; i++)
				{
					if ((ch = fgetc(fp)) != EOF)
						FileContent[i] = ch;
				}
				fclose(fp);//关闭文件

				if (FileContent[strlen(FileContent)] != '\n')
					FileContent[strlen(FileContent)] = '\n';
				Deln(FileContent);//删除数组前面的\n
			}
			else
				fclose(fp);
		}
	}
	else
		printf("未找到自定义RGB文件\n");

	printf("自定义RGB文件路径： %s\n", RGBFilePath);
	printf("钉钉路径%s\n", runPath);
	printf("日志文件路径 %s\n", logPath);
	printf("自带RGB数据%s\n", RGB);


	if (FileContent[0] != '\0')//如果存在自定义文件
	{
		for (i = 0, j = 0; i < strlen(FileContent) + 1; i++)
		{
			if (FileContent[i] == '\n')
			{
				Deln(tmp);
				if (tmp[0] == '\0')
				{
					j = 0;
					continue;
				}

				SplitStr(tmp, &x, &y, fileRGB, sizeof(fileRGB));

				SetCursorPos(x, y);//设置鼠标位置
				GetMousePos(currentRGB);//获取当前鼠标位置的RGB
				printf("当前位置：%d,%d RGB：%s\n", x, y, currentRGB);


				if (strcmp(currentRGB, fileRGB) == 0)//如果鼠标当前坐标的RGB与文件上的RGB相同
					break;

				memset(tmp, 0, 20);//数组清零
				j = 0;
				Sleep(3000);//未匹配到正确RGB到则等待3秒再次匹配
			}

			if (i == strlen(FileContent))
			{
				i = 0;
				j = 0;
			}
			tmp[j++] = FileContent[i];
		}
	}


	for (i = 0; FileContent[0] == '\0' && color != 1 && i < strlen(RGB) + 1; i++)
	{
		if (RGB[i] == '\n')
		{
			Deln(tmp);
			if (tmp[0] == '\0')
			{
				j = 0;
				continue;
			}

			SplitStr(tmp, &x, &y, fileRGB, sizeof(fileRGB));//分割字符串	

			SetCursorPos(x, y);//设置鼠标位置
			GetMousePos(currentRGB);//获取当前鼠标位置的RGB
			printf("当前位置：%d,%d RGB：%s\n", x, y, currentRGB);

			if (strcmp(fileRGB, currentRGB) == 0)//如果鼠标当前坐标的RGB与文件上的RGB相同
				break;

			memset(tmp, 0, 20);
			j = 0;
			Sleep(3000);
		}

		if (i == strlen(RGB))
		{
			i = 0;
			j = 0;
		}
		tmp[j++] = RGB[i];
	}

	//模拟鼠标左键按下弹起
	mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);

	printf("完成\n");
	Sleep(10000);
	//	system("pause");
	return 0;
}

//删除字符串开头的'\n'
void Deln(char* str)
{
	if (str[0] == '\n')
	{
		unsigned i = 0;
		for (i = 0; i < strlen(str); i++)
			str[i] = str[i + 1];
	}
}

//分割字符串
void SplitStr(const char* str, int* X, int* Y, char* RGB, int RGBSize)
{
	int i = 0;
	int j = 0, num = 1, x = 0, y = 0, r = 0, g = 0, b = 0;
	char tmp[10] = { 0 };
	memset(RGB, 0, RGBSize);
	if (str[0] == '\0')
		return;

	for (; i < RGBSize; i++)
	{
		if (tmp[0] != '\0' && (str[i] == ',' || str[i] == ' ' || str[i] == '\n' || str[i] == '\0'))
		{
			if (num == 1)
				sscanf_s(tmp, "%d", &x);
			else if (num == 2)
				sscanf_s(tmp, "%d", &y);
			else if (num == 3)
				sscanf_s(tmp, "%d", &r);
			else if (num == 4)
				sscanf_s(tmp, "%d", &g);
			else if (num == 5)
				sscanf_s(tmp, "%d", &b);
			num++;
			j = 0;
			memset(tmp, 0, 10);//数组清零
			continue;
		}
		if (str[i] == ',' || str[i] == '，' || str[i] == ' ' || str[i] == '\n' || str[i] == '\0')
			continue;
		tmp[j++] = str[i];
	}


	*X = x;
	*Y = y;
	sprintf_s(RGB, RGBSize, "%d%d%d", r, g, b);
}

//打开钉钉
void OpenDingDing(void)
{
	//钉钉注册表路径
	char* key = "SOFTWARE\\WOW6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\钉钉";
	HANDLE _handle = NULL;
	char* find = NULL;//查找钉钉安装路径中最后一个‘\’的位置
	HKEY hkey;
	DWORD dwSize = MAX_PATH;
	char path[MAX_PATH];

	//获取系统是64位还是32位
	SYSTEM_INFO si;
	GetNativeSystemInfo(&si);
	if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_INTEL)//32位
		key = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\钉钉";


	//打开注册表
	if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, key, 0, REG_SZ, &hkey) == ERROR_SUCCESS)
	{
		//从注册表读取键值
		if (RegQueryValueExA(hkey, "UninstallString", NULL, NULL, (LPBYTE)&path, &dwSize) == ERROR_SUCCESS)
		{
			//获取的是卸载程序路径，所以要从路径中分割出安装文件夹
			if ((find = strrchr(path, '\\')) != NULL)
			{
				*(++find) = '\0';

				strcat_s(path, MAX_PATH, "DingtalkLauncher.exe");

				//判断从注册表中读取的钉钉安装路径是否存在
				if (!_access(path, 0))
				{
					sprintf_s(runPath, MAX_PATH, "\"%s\"", path);
					system(runPath);//启动钉钉

					//_handle = (HANDLE)_beginthreadex(NULL, 0, Start, NULL, 0, NULL);//创建线程
					//if (_handle)
					//	CloseHandle(_handle);//释放句柄

					Sleep(3000);
					return;//返回
				}
				else
					WriteLog("注册表寻找到的钉钉安装路径无效！");
			}
			else
				WriteLog("注册表读取到的钉钉安装路径无效！");
		}
		else
			WriteLog("读取键值“UninstallString”失败！");
	}
	else
		WriteLog("打开注册表失败");


	//从桌面寻找钉钉快捷方式
	sprintf_s(runPath, MAX_PATH, "\"C:\\Users\\Public\\Desktop\\钉钉.lnk\"");

	//判断路径是否存在
	if (_access(runPath, 0))
	{
		WriteLog("无法获取桌面的钉钉快捷方式！");
		exit(EXIT_FAILURE);
	}

	//_handle = (HANDLE)_beginthreadex(NULL, 0, Start, NULL, 0, NULL);//创建线程
	//if (_handle)
	//	CloseHandle(_handle);//释放句柄
	system(runPath);//启动钉钉
}


//写入日志
void WriteLog(const char* err)
{
	time_t t;
	FILE* fp;

	//获取当前时间
	struct tm t_time;
	time(&t);
	localtime_s(&t_time, &t);

	//打开文件
	if (fopen_s(&fp, logPath, "a+") != 0)
	{
		printf("打开日志文件失败！\n");
		perror("原因");
		system("pause");
		exit(EXIT_FAILURE);
	}


	//向文件写入错误原因和时间
	fprintf_s(fp, "原因：%s\n时间：%02d-%02d-%02d %02d:%02d:%02d\n\n",
		err,
		t_time.tm_year + 1900, t_time.tm_mon + 1, t_time.tm_mday,
		t_time.tm_hour, t_time.tm_min, t_time.tm_sec);

	fclose(fp);//关闭文件
	printf("原因：%s\n时间：%02d-%02d-%02d %02d:%02d:%02d\n",
		err,
		t_time.tm_year + 1900, t_time.tm_mon + 1, t_time.tm_mday,
		t_time.tm_hour, t_time.tm_min, t_time.tm_sec);
}


//获取鼠标指针坐标和当前坐标颜色
void GetMousePos(char* const RGB)
{
	POINT p;
	HDC hdc = GetDC(NULL);//获取桌面的DC绘图设备描述符
	COLORREF rgb;

	if (RGB != NULL)
	{
		GetCursorPos(&p);//获取鼠标指针坐标
		rgb = GetPixel(hdc, p.x, p.y);//获取RGB值

		sprintf_s(RGB, 20, "%d%d%d", GetRValue(rgb), GetGValue(rgb), GetBValue(rgb));
		return;
	}


	OpenDingDing();//打开钉钉
	Sleep(3000);//等待钉钉启动
	HWND _hwnd = FindWindowA(NULL, "钉钉");//获取钉钉窗口句柄
	if (!_hwnd)
	{
		WriteLog("获取钉钉窗口句柄失败！");
		exit(EXIT_FAILURE);
	}
	//寻找钉钉进程
	for (int i = 1; i <= 10; i++)
	{
		if (GetProcess("DingTalk.exe") == true)
			break;

		Sleep(3000);//未找到进程就等待2秒重试

		if (i == 10)//如果重试20次都未找到
		{
			WriteLog("未找到钉钉进程");
			exit(EXIT_FAILURE);
		}
	}

	ShowWindow(_hwnd, 3);
	//将钉钉窗口移动到屏幕左上角并设置窗口高度和宽度
	MoveWindow(_hwnd, 0, 0, 930, 640, true);

	while (1)
	{
		GetCursorPos(&p);//获取鼠标指针坐标
		rgb = GetPixel(hdc, p.x, p.y);//获取RGB值

		printf("  \r当前鼠标位置：X:%ld Y:%ld， RGB值：%d,%d,%d", p.x, p.y, GetRValue(rgb), GetGValue(rgb), GetBValue(rgb));
		Sleep(300);
	}
}


//判断钉钉进程是否存在
bool GetProcess(const char* name)
{
	char pName[MAX_PATH];
	strcpy_s(pName, MAX_PATH, name);
	CharLowerBuffA(pName, MAX_PATH);//将名称里面的字母转换为小写						
	PROCESSENTRY32 currentProcess;	//存放快照进程信息的一个结构体					
	currentProcess.dwSize = sizeof(currentProcess);	//在使用这个结构之前，先设置它的大小	
	HANDLE _handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);//给系统内的所有进程拍一个快照

	if (_handle == INVALID_HANDLE_VALUE)
	{
		WriteLog("获取系统进程失败");
		return false;
	}


	bool bMore = Process32First(_handle, &currentProcess);	//获取第一个进程信息	
	while (bMore)
	{
		CharLowerBuffA(currentProcess.szExeFile, MAX_PATH);//将进程名转换为小写
		if (strcmp(currentProcess.szExeFile, pName) == 0)//比较是否存在此进程
		{
			CloseHandle(_handle);//关闭句柄
			return true;
		}
		bMore = Process32Next(_handle, &currentProcess);//遍历下一个
	}
	CloseHandle(_handle);	//关闭句柄
	return false;
}


//添加自启动
void Add(void)
{
	char tmp[MAX_PATH], path[MAX_PATH];
	HKEY hkey;

	GetModuleFileNameA(NULL, tmp, MAX_PATH);//获取当前exe路径
	sprintf_s(path, MAX_PATH, "\"%s\"", tmp);

	//打开注册表启动项 
	if (RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_ALL_ACCESS, &hkey) == ERROR_SUCCESS)
	{
		if (RegSetValueExA(hkey, "自动进入钉钉直播间", 0, REG_SZ, (BYTE*)path, MAX_PATH) == ERROR_SUCCESS)//写入注册表
		{
			RegCloseKey(hkey);//关闭注册表
			printf("添加成功！请勿删除本文件\n ");
			system("pause");
			exit(EXIT_SUCCESS);
		}
	}

	printf("添加失败！\n请以管理员权限运行本软件或\n把此软件添加到杀软白名单重试");
	system("pause");
	exit(EXIT_FAILURE);
}


//删除自启动
void Del(void)
{
	HKEY hkey;

	//打开注册表启动项 
	if (RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_ALL_ACCESS, &hkey) == ERROR_SUCCESS)
	{
		if (RegQueryValueExA(hkey, "自动进入钉钉直播间", NULL, NULL, NULL, NULL) == ERROR_FILE_NOT_FOUND)//判断启动项是否存在
		{
			printf("启动项不存在\n");
			system("pause");
			exit(EXIT_FAILURE);
		}
		if (RegDeleteValueA(hkey, "自动进入钉钉直播间") == ERROR_SUCCESS)//删除键值
		{
			RegCloseKey(hkey);//关闭注册表
			printf("删除成功！\n ");
			system("pause");
			exit(EXIT_SUCCESS);
		}
	}

	printf("删除失败！\n请以管理员权限运行本软件或\n把此软件添加到杀软白名单重试");
	system("pause");
	exit(EXIT_FAILURE);
}


//创建RGB文件模板
void Create(void)
{
	FILE* fp;

	if (fopen_s(&fp, RGBFilePath, "w") != 0)
	{
		perror("创建文件失败\n原因");
		system("pause");
		exit(EXIT_FAILURE);
	}

	if (fwrite(RGB, sizeof(char), strlen(RGB), fp) < strlen(RGB))
	{
		perror("写入文件失败\n原因");
		system("pause");
		exit(EXIT_FAILURE);
	}

	fclose(fp);
	printf("创建模板文件成功，请在桌面查看\n");
	system("pause");
	exit(EXIT_SUCCESS);
}


//unsigned int __stdcall Start(PVOID p)
//{
//	system(runPath);//打开钉钉
//	return 0;
//}
