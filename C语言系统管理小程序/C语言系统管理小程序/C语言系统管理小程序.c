//文件创建时间：2019年1月29日  最后修改时间：2023年4月5日

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <direct.h>//为_getcwd提供声明
#include <io.h>
#include <UrlMon.h>
#include <shellapi.h>
#include <shlobj.h>
#include <process.h>
#include <wininet.h>
#include <stdbool.h>
#include "common.h" 
#pragma comment(lib, "urlmon.lib")
#pragma comment(lib, "wininet.lib")
// 关机
void shut_down(void);
void shutdown_hour(void);
void shutdown_custom_sec(void);
void shutdown_set_custom_task(void);
void shutdown_del_custom_task(void);
void excute_shutdown(int sec);
// 重启
void reboot(void);
void reboot_hour(void);
void reboot_custom_sec(void);
void reboot_set_custom_task(void);
void reboot_del_custom_task(void);
void excute_reboot(int sec);
// 查询
void lookup_current_task(void);
// 系统管理
int  system_function(void);
void create_file(void);
void create_file_custom_size(void);
void kill_process(void);
void open_readme(void);
// 修改hosts
void hosts(void);
// 清除缓冲区
void flush(void);
// 实用功能
void function(void);
// 删除文件
void delfile(int mode);
// 彩蛋
int  autoshut(void);
void print_and_exit(const char* const str);
// 获取启动目录
void open_startup_directory(void);
void print_config(void);
// 删除末尾的换行符
void del_last_n(char* str);

void print_msg(const char* str);

#define DEL_FILE_TEMP 1 // 删除程序创建的临时文件
#define DEL_FILE_SELF 2 // 删除程序自己
#define OPEN_HOSTS 1	 // 打开hosts
#define RECOVERY_HOSTS 2 // 恢复hosts

int main(void)
{
	while (true)
	{
		int num;
		// 标题
		system("mode con:cols=57 lines=25 && color 0F && title C语言系统管理小程序 请以管理员权限打开本软件");
		delfile(DEL_FILE_TEMP);// 删除文件

		printf("\n        ------欢迎使用C语言系统管理小程序------\n");
		printf("\t\t      1、关机\n");
		printf("\t\t      2、重启\n");
		printf("\t\t      3、休眠\n");
		printf("\t\t      4、睡眠\n");
		printf("\t\t      5、取消（关机、重启）\n");
		printf("\t\t      6、注销\n");
		printf("\t\t      7、实用功能\n");
		printf("\t\t      8、查看电脑配置\n");
		printf("\t\t      9、系统管理\n");
		printf("\t\t     10、下载源码\n");
		printf("\t\t     11、查看使用说明\n");
		printf("\t\t      0、退出\n\n");
		printf("     请输入序号：");
		while (scanf_s("%d", &num) != 1)//利用scanf_s的返回值判断输入的内容是否为字符
		{
			flush();//清空缓冲区残余字符
			printf("\n输入错误，请重新输入：");
		}
		flush();
		switch (num)
		{
		case 0:
			return 0; //退出
		case 1:
			shut_down();
			break;//关机
		case 2:
			reboot();
			break;//重启
		case 3:
			system("shutdown -h");
			Sleep(2000);
			return 0;//休眠
		case 4:
		{
			system("cls && color 04");
			printf("\n\n\n\n\n注意：\n请确保此计算机已关闭休眠功能，否则会当成启动休眠功能\n\n（开启或关闭休眠功能可在“9、系统管理”找到）\n");
			Sleep(2000);
			system("rundll32.exe powrprof.dll,SetSuspendState 0,1,0");//执行睡眠命令 
			return 0; //睡眠
		}
		case 5:
			system("shutdown -a");
			break;    //取消关机、重启
		case 6:
			system("shutdown -l");
			return 0; //注销
		case 7:
			function();
			break;	//实用功能
		case 8:
			print_config();
			break;	//查看电脑配置
		case 9:
			system_function();
			break;	//系统管理
		case 10:
			system("start https://cdmxz.github.io");
			break;
		case 11: open_readme();
			break;
		default:
			print_msg("\n\n\t     输入错误!\n\n");
			break; //while处理字符错误，switch处理数字错误
		}//switch

	}//while循环使代码执行后返回开头
	return 0;
}

void open_readme(void)
{
	TCHAR path[MAX_PATH];
	GetCurrentDirectory(arr_count(path), path);//获取程序所在目录
	wcscat_s(path, arr_count(path), L"\\readme.txt");
	if (_waccess(path, 0))//判断“readme.txt”文件是否在本程序目录下
		system("start https://github.com/cdmxz/C-language-program/blob/master/C%E8%AF%AD%E8%A8%80%E7%B3%BB%E7%BB%9F%E7%AE%A1%E7%90%86%E5%B0%8F%E7%A8%8B%E5%BA%8F/readme.txt");//如果不存在则打开网站
	else
		system("notepad readme.txt");//存在则直接打开
}

// 系统管理
int system_function(void)
{
	while (true)
	{
		int num;
		system("cls && color 71 && mode con:cols=63 lines=28");
		system("title 系统管理 PS：输入7返回");

		printf("系统管理：\n");
		printf(" 1、开启休眠        2、关闭休眠\n");
		printf(" 3、打开控制面板    4、打开策略组编辑器  5、打开本地用户和组\n");
		printf(" 6、打开设备管理器  8、查看系统版本      9、打开注册表编辑器\n");
		printf("10、打开IP地址侦测 11、打开计算机管理   12、打开Directx诊断工具\n");
		printf("13、打开组件服务   14、打开本地服务     15、清除DNS缓存\n\n");

		printf("磁盘管理：\n");
		printf("16、启动磁盘清理   17、启动磁盘管理     18、启动磁盘检查\n\n");

		printf("系统程序：\n");
		printf("19、重启资源管理器 20、启动任务管理器   21、启动性能监视器\n");
		printf("22、启动远程桌面   23、启动记事本       24、启动画图板\n");
		printf("25、启动写字板     26、启动Media Player 27、启动命令提示符\n");
		printf("28、启动计算器     29、启动放大镜       30、启动木马捆绑工具\n\n");

		printf("IP地址：\n");
		printf("31、查看IP地址     32、关闭IPv6临时地址 33、开启IPv6临时地址\n\n");

		printf("其他：\n");
		printf("34、修改hosts	   35、结束程序进程     36、打开彩蛋\n");
		printf("37、创建新文件     38、创建指定大小文件 39、查看系统信息\n");
		printf("40、删除自身       41、打开启动目录     42、电脑死机\n");
		printf("43、启用自带键盘   44、禁用自带键盘\n\n");
		printf("请输入序号：");

		while (scanf_s("%d", &num) != 1)//处理字符输入
		{
			flush();//清空缓冲区残余字符
			printf("\n输入错误，请重新输入：\n");
		}
		flush();

		switch (num)
		{
		case 1:
			system("powercfg -h on");
			system("pause");
			break;
		case 2:
			system("powercfg -h off");
			system("pause");
			break;
		case 3:
			system("control");
			break;
		case 4:
			system("gpedit.msc");
			break;
		case 5:
			system("lusrmgr.msc");
			break;
		case 6:
			system("devmgmt.msc");
			break;
		case 7:return 0;
		case 8:
			system("winver");
			break;
		case 9:
			system("regedit");
			break;
		case 10:
			system("cls && Nslookup");
			break;
		case 11:
			system("compmgmt.msc");
			break;
		case 12:
			system("dxdiag");
			break;
		case 13:
			system("dcomcnfg");
			break;
		case 14:
			system("services.msc");
			break;
		case 15:
			system("ipconfig /flushdns");
			break;
		case 16:
			system("cleanmgr");
			break;
		case 17:
			system("diskmgmt.msc");
			break;
		case 18:
			system("cls && mode con:cols=70 lines=40 && chkdsk.exe");
			system("pause");
			break;
		case 19:
			system("start %SystemRoot%\\System32\\tskill.exe explorer");
			system("pause");
			break;
		case 20:
			system("taskmgr");
			break;
		case 21:
			system("perfmon.msc");
			break;
		case 22:
			system("mstsc");
			break;
		case 23:
			system("notepad");
			break;
		case 24:
			system("mspaint");
			break;
		case 25:
			system("write");
			break;
		case 26:
			system("dvdplay");
			break;
		case 27:
			system("cmd ");
			break;
		case 28:
			system("calc");
			break;
		case 29:
			system("magnify");
			break;
		case 30:
			system("IExpress");
			break;
		case 31:
			system("cls && ipconfig /all > 本机IP信息.txt && 本机IP信息.txt");
			break;
		case 32:
			printf("\n\t   ");
			system("netsh interface ipv6 set privacy state=disable");
			break;
		case 33:
			printf("\n\t   ");
			system("netsh interface ipv6 set privacy state=enabled");
			break;
		case 34:
			hosts();
			break;
		case 35:
			kill_process();
			break;
		case 36:
			autoshut();
			break;
		case 37:
			create_file();
			break;
		case 38:
			create_file_custom_size();
			break;
		case 39:
			system("systeminfo >> 系统信息.txt && 系统信息.txt");
			break;
		case 40:
			delfile(DEL_FILE_SELF);
			break;
		case 41:
			open_startup_directory();
			break;
		case 42:
			while (true)
				malloc(10485760);
			break;
		case 43:
			system("sc config i8042prt start= auto");
			print_msg("\n\t重启后生效");
			break;
		case 44:
			system("sc config i8042prt start= disabled");
			print_msg("\n\t重启后生效");
			break;
		default:
			print_msg("\n\n\t输入错误!\n\n");
			break;
		}
	}//while

}

// 创建文件
void create_file(void)
{
	char fileName[MAX_PATH], cmd[MAX_PATH], content[MAX_PATH];
	system("cls && color 07");
	printf("\n\n请输入文件名：");
	fgets(fileName, arr_count(fileName), stdin);
	del_last_n(fileName);
	printf("\n\n请输入内容（如果要创建空文件请直接回车）：");
	fgets(content, arr_count(content), stdin);
	del_last_n(content);
	if (content[0] == '\0')
		sprintf_s(cmd, arr_count(cmd), "type nul>%s", fileName);
	else
		sprintf_s(cmd, arr_count(cmd), "echo %s>%s", content, fileName);
	printf("\n\n");
	system(cmd);
	system("pause");
	printf("\n\n");
}

// 创建指定大小文件
void create_file_custom_size(void)
{
	char fileName[MAX_PATH], cmd[MAX_PATH];
	long long size;
	system("cls && color 07");
	printf("\n温馨提示：此项功能可以用来测试磁盘空间大小");
	printf("\n\n\n请输入文件名：");
	fgets(fileName, arr_count(fileName), stdin);
	del_last_n(fileName);//去除fgets读取的换行符
	printf("\n\n请输入要创建的文件大小（单位：MB）：");
	scanf_s("%lld", &size);
	flush();
	sprintf_s(cmd, arr_count(cmd), "fsutil file createnew %s %lld", fileName, size * 1048576);
	printf("\n\n");
	system(cmd);
	printf("\n\n");
	system("pause");
}

// 杀死进程
void kill_process(void)
{
	char name[SIZE];
	char cmd[SIZE] = { "taskkill /f /t /im " };
	system("cls");
	//system("cls && tasklist >> 进程信息.txt && 进程信息.txt");
	printf("请输入要结束的进程名称（例如：1.exe）：");
	fgets(name, arr_count(name), stdin);
	del_last_n(name);
	strcat_s(cmd, arr_count(cmd), name);
	system(cmd);
	system("pause");
}

// 删除末尾的换行符
void del_last_n(char* str)
{
	while (*str != '\0')
	{
		str++;
	}
	str--;
	if (*str == '\n')
		*str = '\0';
}

// 修改hosts
void hosts(void)
{
	int num;
	FILE* fpwrite;
	char path[] = "%windir%\\System32\\drivers\\etc\\hosts";
	char cmd[MAX_PATH];
	while (true)
	{
		system("cls && mode con:cols=60 lines=20 && title hosts修改 请以管理员权限打开此软件");
		printf("\n温馨提示：1.请以管理员权限打开此软件，避免权限不够而失败。\n");
		printf("          2.如果已经使用管理员权限打开本软件请忽略本提示。");
		printf("\n\n\n\t      1、用系统默认编辑器打开hosts文件\n\n");
		printf("\t      2、还原默认hosts文件\n\n");
		printf("\t      3、返回上一界面\n\n\n");
		printf(" 请输入序号：");
		while (scanf_s("%d", &num) != 1)
		{
			flush();//清空缓冲区残余字符
			printf("\n输入错误！请重新输入：");
		}
		flush();
		// 取消只读属性 
		sprintf_s(cmd, arr_count(cmd), "attrib -R -S \"%s\"", path);
		system(cmd);
		if (num == OPEN_HOSTS)// 打开hosts
		{
			sprintf_s(cmd, arr_count(cmd), "notepad \"%s\"", path);
			system(cmd);
		}
		else if (num == RECOVERY_HOSTS)// 恢复hosts
		{
			if (fopen_s(&fpwrite, path, "w") != 0 || !fpwrite)//打开hosts
			{
				print_and_exit("\n创建hosts文件失败！请以管理员权限运行本软件！\n\n");
				exit(EXIT_FAILURE);
			}
			if (fputs(HOSTS, fpwrite) == EOF)
			{
				print_and_exit("\n恢复默认hosts失败！请以管理员权限打开本软件重试！\n\n");
			}
			printf("\n\t\t恢复默认hosts成功！\n\n");
			fclose(fpwrite);
			system("pause");
		}
		else if (num == 3)
		{
			system("ipconfig /flushdns");
			break;
		}
		else
		{
			print_msg("\n\t输入错误！");
		}
	}
}

// 打印并退出程序
void print_and_exit(const char* const str)
{
	printf("\n创建hosts文件失败！请以管理员权限运行本软件！\n\n");
	system("pause");
	exit(EXIT_FAILURE);
}

//关机 
void shut_down(void)
{
	int min;
	system("mode con cols=55 lines=18 && color 06 && title 关机 PS：请以管理员权限运行本软件");
	printf("温馨提示：在当前页面输入7返回。  输入数字后请按回车。\n");
	printf("          如需设定小时请输入2。  XXX秒后关机请输入4。\n");
	printf("          自定义XX小时:XX分钟关机任务请输入6。\n");
	printf("          删除自定义XX小时:XX分钟关机请输入8。\n");
	printf("          查看当前任务请输入9。\n\n\n\n\n");
	printf("请输入（0、1、3、5、10、20、30、40、50、60）分后关机。\n\n请输入数字：");
	while (scanf_s("%d", &min) != 1)
	{
		flush();//清空缓冲区残余字符
		printf("\n输入错误，请重新输入：");
	}
	flush();
	switch (min)
	{
	case 0:
	case 1:
	case 3:
	case 5:
	case 10:
	case 20:
	case 30:
	case 40:
	case 50:
	case 60: {
		excute_shutdown(min * 60);
		break;
	}
	case 2:shutdown_hour(); break;
	case 4:shutdown_custom_sec(); break;
	case 6:shutdown_set_custom_task(); break;
	case 7: break;
	case 8:shutdown_del_custom_task(); break;
	case 9:lookup_current_task(); break;
	default:print_msg("\n\n\t 输入错误！"); break;
	}
}

void excute_shutdown(int sec)
{
	char cmd[SIZE];
	sprintf_s(cmd, arr_count(cmd), "shutdown -s -t %d", sec);
	system(cmd);
}

void shutdown_del_custom_task(void)
{
	system("cls && schtasks /delete /tn 关机 /F");
	printf("\n\n\n\n\n如果显示“拒绝访问”字样，就请以管理员权限运行此软件。\n\n");
	system("pause");
}

void shutdown_set_custom_task(void)
{
	char schtasks[SIZE] = { "schtasks /create /tn \"关机\" /tr \"shutdown /s /t 00\" /sc once /st " };
	char shut[SIZE] = { 0 };

	system("cls");
	printf("\n温馨提示：设置关机的时间请不要小于或等于当前时间。\n\n\n\n");
	printf("请输入多少点关机。（例如：12:00）：");

	scanf_s("%s", shut, arr_count(shut));
	flush();
	strcat_s(schtasks, arr_count(shut), shut);//合并数组
	system(schtasks);
	printf("\n\n如果显示“拒绝访问”字样，就请以管理员权限运行此软件。\n\n");
	system("pause");
}

void shutdown_custom_sec(void)
{
	int sec;
	char cmd[SIZE] = { 0 };

	system("cls");
	printf("\n温馨提示：输入数字后请按回车，最大时间不能超过999999秒。\n\n\n\n\n");
	printf("温馨提示：只需输入数字即可，不必输入单位\n");
	printf("请输入多少秒后关机（单位：秒）：");

	while (scanf_s("%d", &sec) != 1)
	{
		printf("\n输入错误，请重新输入：");
		flush();//清空缓冲区残余字符
	}
	flush();
	excute_shutdown(sec);
	Sleep(1000);
}

//关机（单位：小时）
void shutdown_hour(void)
{
	int num;
	double hour = 0.0;
	system("cls");
	printf("       温馨提示：1.输入0返回  2.输入序号后请按回车\n\n");
	printf("\t\t  1、1.5小时后关机\n");
	printf("\t\t  2、2  小时后关机\n");
	printf("\t\t  3、2.5小时后关机\n");
	printf("\t\t  4、3  小时后关机\n");
	printf("\t\t  5、3.5小时后关机\n");
	printf("\t\t  6、4  小时后关机\n");
	printf("\t\t  7、4.5小时后关机\n");
	printf("\t\t  8、5  小时后关机\n");
	printf("\t\t  9、5.5小时后关机\n");
	printf("请输入序号：");
	while (scanf_s("%d", &num) != 1)//处理字符输入
	{
		printf("\n输入错误，请重新输入：");
		flush();//清空缓冲区残余字符
	}
	flush();
	switch (num)
	{
	case 0:break;
	case 1:hour = 1.5; break;
	case 2:hour = 2; break;
	case 3:hour = 2.5; break;
	case 4:hour = 3; break;
	case 5:hour = 3.5; break;
	case 6:hour = 4; break;
	case 7:hour = 4.5; break;
	case 8:hour = 5; break;
	case 9:hour = 5.5; break;
	default:
		print_msg("\n\n\t 输入错误！");
		return;
	}
	excute_shutdown((int)(hour * 3600));
}

//重启
void reboot(void)
{
	int min;
	char cmd[SIZE];
	system("mode con cols=55 lines=18 && color 06 && cls && title 重启 PS：请以管理员权限运行本软件");
	printf("温馨提示：在当前页面输入7返回。  输入数字后请按回车。\n");
	printf("          如需小时时间请输入2。  XXX秒后重启请输入4。\n");
	printf("          自定义XX小时:XX分钟重启任务请输入6。\n");
	printf("          删除自定义XX小时:XX分钟重启请输入8。\n");
	printf("          查看当前任务请输入9。\n\n\n\n\n");
	printf("请输入（0、1、3、5、10、20、30、40、50、60）分后重启。\n\n请输入数字：");

	while (scanf_s("%d", &min) != 1)
	{
		printf("\n输入错误，请重新输入：");
		flush();//清空缓冲区残余字符
	}

	switch (min)
	{
	case 0:
	case 1:
	case 3:
	case 5:
	case 10:
	case 20:
	case 30:
	case 40:
	case 50:
	case 60:
		excute_reboot(min * 60);
		break;
	case 2:reboot_hour(); break;
	case 4:reboot_custom_sec(); break;
	case 6:reboot_set_custom_task(); break;
	case 7:reboot_del_custom_task(); break;
	case 8:lookup_current_task(); break;
	default:print_msg("\n\n\t 输入错误！"); break;
	}
}

void excute_reboot(int sec)
{
	char cmd[SIZE];
	sprintf_s(cmd, arr_count(cmd), "shutdown -r -t %d", sec);
	system(cmd);
}
void lookup_current_task(void)
{
	system("cls && schtasks > 当前任务.txt && 当前任务.txt");
	printf("\n\n\n文件已导出到当前程序目录\n\n");
	system("pause");
}

void reboot_del_custom_task(void)
{
	system("cls && schtasks /delete /tn 重启 /F");
	printf("\n\n\n\n\n如果显示“拒绝访问”字样，请以管理员权限运行此软件。\n\n");
	system("pause");
}

void reboot_set_custom_task(void)
{
	char schtasks[SIZE] = { "schtasks /create /tn \"重启\" /tr \"shutdown /r /t 00\" /sc once /st /0" };
	char cmd[SIZE] = { '\0' };

	system("cls");
	printf("\n温馨提示：设置重启的时间请不要小于或等于当前时间。\n\n\n\n");
	printf("请输入多少点重启。（例如：12:00）：");

	scanf_s("%s", cmd, arr_count(cmd));
	flush();
	strcat_s(schtasks, arr_count(cmd), cmd);
	system(schtasks);
	printf("\n\n如果显示“拒绝访问”字样，请以管理员权限运行此软件。\n\n");
	system("pause");
}

void reboot_custom_sec(void)
{
	char cmd[SIZE];
	int sec;

	system("cls");
	printf("\n温馨提示：输入数字后请按回车，最大时间不能超过999999秒。\n\n\n\n\n");
	printf("温馨提示：只需输入数字即可，不必输入单位\n");
	printf("请输入多少秒后重启（单位：秒）：");

	while (scanf_s("%d", &sec) != 1)
	{
		printf("\n输入错误，请重新输入：");
		flush();//清空缓冲区残余字符
	}
	flush();
	excute_reboot(sec);
	Sleep(1000);
}

void reboot_hour(void)
{
	int num;
	double hour = 0.0;
	system("cls");
	printf("       温馨提示：1.输入0返回  2.输入序号后请按回车\n\n");
	printf("\t\t  1、1.5小时后重启\n");
	printf("\t\t  2、2  小时后重启\n");
	printf("\t\t  3、2.5小时后重启\n");
	printf("\t\t  4、3  小时后重启\n");
	printf("\t\t  5、3.5小时后重启\n");
	printf("\t\t  6、4  小时后重启\n");
	printf("\t\t  7、4.5小时后重启\n");
	printf("\t\t  8、5  小时后重启\n");
	printf("\t\t  9、5.5小时后重启\n");
	printf("请输入序号：");
	while (scanf_s("%d", &num) != 1)//处理字符输入
	{
		printf("\n输入错误，请重新输入：");
		flush();//清空缓冲区残余字符
	}
	switch (num)
	{
	case 0:break;
	case 1:hour = 1.5; break;
	case 2:hour = 2; break;
	case 3:hour = 2.5; break;
	case 4:hour = 3; break;
	case 5:hour = 3.5; break;
	case 6:hour = 4; break;
	case 7:hour = 4.5; break;
	case 8:hour = 5; break;
	case 9:hour = 5.5; break;
	default:
		print_msg("\n\n\t 输入错误！");
		return;
	}
	excute_reboot((int)(hour * 3600));
}

//清除缓冲区
void flush(void)
{
	int ch;
	while ((ch = getchar()) != '\n' && ch != EOF)
		;
}

//删除文件
void delfile(int mode)
{
	TCHAR cmd[MAX_PATH] = { 0 }, cmdpath[MAX_PATH], filepath[MAX_PATH];
	if (mode == DEL_FILE_TEMP)
	{
		remove("本机IP信息.txt");
		remove("系统信息.txt");
		remove("获取硬件信息.bat");
		remove("进程信息.txt");
		remove("使用说明.txt");
		return;
	}
	// 删除程序自身
	if ((GetModuleFileName(NULL, filepath, arr_count(filepath)) != 0)
		&& (GetEnvironmentVariable(L"COMSPEC", cmdpath, arr_count(cmdpath)) != 0))
	{
		swprintf_s(cmd, arr_count(cmd), L"\"%s\" /c del /q \"%s\" > 1.txt", cmdpath, filepath);// 命令参数
		SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);				// 提高自身进程的优先级
		SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
		SHChangeNotify(SHCNE_DELETE, SHCNF_PATH, filepath, NULL);					// 通知资源浏览器,本程序已经被删除
		ShellExecute(NULL, L"open", cmdpath, cmd, NULL, SW_HIDE);
		exit(EXIT_SUCCESS);
	}
	else
	{
		print_msg("删除失败!\n");
	}
}

// 打开启动目录
void open_startup_directory(void)
{
	TCHAR path[MAX_PATH], cmd[MAX_PATH] = { 0 };
	// 获取用户文件夹
	if (GetEnvironmentVariable(L"APPDATA", path, arr_count(path)) == 0)
	{
		// 获取失败
		print_msg("获取用户文件夹失败！\n");
	}
	else
	{
		swprintf_s(cmd, arr_count(cmd), L"\"%s\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\"", path);
		ShellExecute(NULL, L"open", L"explorer.exe", cmd, NULL, SW_SHOWNORMAL);
	}
}

// 打印信息
void print_msg(const char* str)
{
	printf("%s\n", str);
	Sleep(2000);
}