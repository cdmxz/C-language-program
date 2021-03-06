//项目创建时间：2019年1月29日  目前最后修改时间：2020年03月17日

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
#pragma comment(lib, "urlmon.lib")
#pragma comment(lib, "wininet.lib")

void  shut(void);       //关机
void  reboot(void);     //重启
int   system_config(void);  //查看电脑配置
int   system_function(void);//系统管理
void  hosts(void);      //修改hosts
void  flush(void);      //清除缓冲区
void  function(void);   //实用功能
void  delfile(int mode);    //删除文件
int   autoshut(void);   //彩蛋
void  get_startup_directory(void);//获取启动目录


#define SIZE 100
#define DEL_FILE 1

int main(void)
{
    //#ifndef _WIN64//当前编译位32位时才会编译#ifndef到#endif中的代码
    //	char path[MAX_PATH];
    //	char url[] = { "http://cdmxz.cf/x64.exe" };
    //	DeleteUrlCacheEntry(url);//清除下载缓存
    //
    //	GetCurrentDirectoryA(MAX_PATH, path);//获取程序目录
    //	strcat_s(path, MAX_PATH, "\\C语言系统管理小程序x64.exe");
    //
    //	SYSTEM_INFO si;
    //	GetNativeSystemInfo(&si);//使用wimapi获取系统是32位还是64位
    //	if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 || si.wProcessorArchitecture != PROCESSOR_ARCHITECTURE_IA64)
    //	{//64位
    //		if (!_access(path, 0))//判断64位程序是否存在
    //			system(path);
    //
    //		else if (URLDownloadToFile(NULL, url, path, 0, NULL) == S_OK)//不存在就下载文件
    //			system(path);
    //	}
    //#endif

    while (1)
    {
        int num;
        system("mode con:cols=57 lines=25 && color 0F && title C语言系统管理小程序 请以管理员权限打开本软件");//标题
        delfile(1);//删除文件

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
            shut();
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
            printf("\n\n\n\n\n注意：\n请确保此计算机已关闭休眠功能，否则会当成启动休眠功能\n\n");
            printf("（开启或关闭休眠功能可在“9、系统管理”找到）\n");
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
            system_config();
            break;	//查看电脑配置
        case 9:
            system_function();
            break;	//系统管理
        case 10:
            system("start https://cdmxz.github.io");
            break;
        case 11: {
            TCHAR path[MAX_PATH];
            GetCurrentDirectory(MAX_PATH, path);//获取程序所在目录
            wcscat_s(path, MAX_PATH, L"\\readme.txt");
            if (_waccess(path, 0))//判断“readme.txt”文件是否在本程序目录下
                system("start https://github.com/cdmxz/C-language-program/blob/master/C%E8%AF%AD%E8%A8%80%E7%B3%BB%E7%BB%9F%E7%AE%A1%E7%90%86%E5%B0%8F%E7%A8%8B%E5%BA%8F/readme.txt");//如果不存在则打开网站
            else
                system("notepad readme.txt");//存在则直接打开
            break;
        }
        default:
            printf("\n\n\t     输入错误!\n\n");
            Sleep(1000);
            break; //while处理字符错误，switch处理数字错误
        }//switch

    }//while循环使代码执行后返回开头
    return 0;
}



//系统管理
int  system_function(void)
{
    while (1)
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
        printf("40、删除自身       41、打开启动目录\n\n");
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
            system("pause");
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
            system("tskill explorer");
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
            system("pause");
            break;
        case 32:
            printf("\n\t   ");
            system("netsh interface ipv6 set privacy state=disable");
            system("pause");
            break;
        case 33:
            printf("\n\t   ");
            system("netsh interface ipv6 set privacy state=enabled");
            system("pause");
            break;
        case 34:
            hosts();
            break;
        case 35: {
            char name[SIZE];
            char cmd[SIZE] = { "taskkill /f /t /im " };
            system("cls && tasklist >> 进程信息.txt && 进程信息.txt");
            printf("请输入要结束的进程名称（例如：1.exe）：");
            fgets(name, SIZE, stdin);
            if (name[strlen(name) - 1] == '\n')
                name[strlen(name) - 1] = '\0';

            strcat_s(cmd, SIZE, name);
            system(cmd);
            system("pause");
            break; }
        case 36:
            autoshut();
            break;
        case 37: {
            char fileName[MAX_PATH], cmd[MAX_PATH], content[MAX_PATH];
            system("cls && color 07");

            printf("\n\n请输入文件名：");
            fgets(fileName, MAX_PATH, stdin);
            fileName[strlen(fileName) - 1] = '\0';

            printf("\n\n请输入内容（如果要创建空文件请直接回车）：");
            fgets(content, MAX_PATH, stdin);
            content[strlen(content) - 1] = '\0';

            if (content[0] == '\0')
                sprintf_s(cmd, MAX_PATH, "type nul>%s", fileName);
            else
                sprintf_s(cmd, MAX_PATH, "echo %s>%s", content, fileName);

            printf("\n\n");
            system(cmd);
            system("pause");
            printf("\n\n");
            break;  }
        case 38: {
            char fileName[MAX_PATH], cmd[MAX_PATH];
            long long size;
            system("cls && color 07");

            printf("\n温馨提示：此项功能可以用来测试磁盘空间大小");
            printf("\n\n\n请输入文件名：");
            fgets(fileName, MAX_PATH, stdin);
            fileName[strlen(fileName) - 1] = '\0';//去除fgets读取的换行符

            printf("\n\n请输入要创建的文件大小（单位：MB）：");
            scanf_s("%lld", &size);
            flush();

            sprintf_s(cmd, MAX_PATH, "fsutil file createnew %s %lld", fileName, size * 1048576);

            printf("\n\n");
            system(cmd);
            printf("\n\n");
            system("pause");
            break;  }
        case 39:
            system("systeminfo >> 系统信息.txt && 系统信息.txt");
            break; //将诊断信息导出到当前程序所在的目录并打开系统信息.txt
        case 40:
            delfile(2);
            break;
        case 41:
            get_startup_directory();
            break;
        default:
            printf("\n\n\t输入错误!\n\n");
            Sleep(1000);
            break;
        }
    }//while

}



//修改hosts
void hosts(void)
{
    int num;
    FILE* fpwrite;
    const char host[] = { "# Copyright (c) 1993-2009 Microsoft Corp.\n"
"#\n"
"# This is a sample HOSTS file used by Microsoft TCP/IP for Windows.\n"
"#\n"
"# This file contains the mappings of IP addresses to host names. Each\n"
"# entry should be kept on an individual line. The IP address should\n"
"# be placed in the first column followed by the corresponding host name.\n"
"# The IP address and the host name should be separated by at least one\n"
"# space.\n"
"#\n"
"# Additionally, comments (such as these) may be inserted on individual\n"
"# lines or following the machine name denoted by a '#' symbol.\n"
"#\n"
"# For example:\n"
"#\n"
"#      102.54.94.97     rhino.acme.com          # source server\n"
"#       38.25.63.10     x.acme.com              # x client host\n"
"\n"
"# localhost name resolution is handled within DNS itself.\n"
"#	127.0.0.1       localhost\n"
"#	::1             localhost\n" };

    while (1)
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


        if (num == 1)//打开hosts
        {
            MessageBox(NULL, L"注意：编辑完hosts文件后请保存。", L"注意：", MB_OK | MB_ICONWARNING);
            system("notepad %windir%\\System32\\drivers\\etc\\hosts");//打开hosts
        }
        else if (num == 2)//恢复hosts
        {
            if (fopen_s(&fpwrite, "C:\\Windows\\System32\\drivers\\etc\\hosts", "w") != 0)//打开hosts
            {
                MessageBox(NULL, L"创建hosts文件失败！请以管理员权限运行本软件！", L"Error", MB_OK | MB_ICONERROR);
                exit(EXIT_FAILURE);
            }

            if (fpwrite)
            {
                if (fputs(host, fpwrite) == EOF)
                {
                    printf("\n恢复默认hosts失败！请以管理员权限打开本软件重试！\n\n");
                    system("pause");
                    exit(EXIT_FAILURE);
                }
                printf("\n\t\t恢复默认hosts成功！\n\n");
                fclose(fpwrite);
            }
            system("pause");
        }
        else if (num == 3)
            break;
        else
        {
            printf("\n\t输入错误！");
            Sleep(1000);
        }
    }
}



//获取电脑配置
int system_config(void)
{
    // 定义全局变量
    FILE* fpread, * fpwrite;
    int ch, i;
    TCHAR url[] = { L"https://gitee.com/fuhohua/C-language-program/raw/master/C%E8%AF%AD%E8%A8%80%E7%B3%BB%E7%BB%9F%E7%AE%A1%E7%90%86%E5%B0%8F%E7%A8%8B%E5%BA%8F/system.dll" };
    TCHAR path[MAX_PATH];

    system("cls && mode con:cols=90 lines=30");

    if (_access("获取硬件信息.exe", 0) != -1)
    {
        system("cls && mode con:cols=90 lines=35");
        system("获取硬件信息.exe");
        return 0;
    }
    else if (_access("system.dll", 0) == -1)
    {
        DeleteUrlCacheEntry(url);//清除下载缓存

        GetCurrentDirectory(MAX_PATH, path);
        wcscat_s(path, MAX_PATH, L"\\system.dll");

        i = MessageBox(NULL, L"未找到“system.dll”，是否下载？", L"警告", MB_YESNO | MB_ICONWARNING);
        if (i == IDNO)
            return 1;
        if (URLDownloadToFile(NULL, url, path, 0, NULL) != S_OK)//下载文件
        {
            MessageBox(NULL, L"下载失败，请重试！", L"错误", MB_OK | MB_ICONERROR);
            return 1;
        }
    }
    if (fopen_s(&fpread, "system.dll", "rb") != 0)// 打开DLL文件
    {
        MessageBox(NULL, L"文件打开错误！\n请检查本程序目录下是否有“system.dll”", L"Error", MB_OK | MB_ICONERROR);
        return 1;
    }
    if (fopen_s(&fpwrite, "获取硬件信息.bat", "wb") != 0)
    {
        MessageBox(NULL, L"数据文件创建错误！\n请以管理员权限运行本软件！", L"Error", MB_OK | MB_ICONERROR);
        return 1;
    }

    if (fpread && fpwrite)
    {
        //看到这应该明白了吧，所谓的DLL文件只是一个改了名字的加密文件，哈哈。
        while ((ch = fgetc(fpread)) != EOF)  //从文件中读取内容到ch。EOF是文件结束标志。
        {
            ch = ch - 'p' + 'z' - 'i' + 2021;//解密文件内容
            fputc(ch, fpwrite); //输出解密后的内容到另一个文件
        }
        fclose(fpwrite);   //关闭文件
        fclose(fpread);    //关闭文件
    }
    system("attrib +h +s 获取硬件信息.bat");// 设置文件属性为隐藏

    printf("\n\t   温馨提示：请以管理员权限运行本软件，如已使用管理员权限运行请忽略。\n\n\n\n");
    printf("\n\n\n\n\t\t正在获取电脑配置信息，约30秒-1分钟左右。请耐心等待...\n\n");
    Sleep(5000);

    system("cls && 获取硬件信息.bat");// 打开获取硬件信息
    return 0;
}


//关机 
void shut(void)
{
    int min, hour;

    system("mode con cols=55 lines=18 && color 06 && title 关机 PS：请以管理员权限运行本软件");
    printf("温馨提示：在当前页面输入7返回。  输入数字后请按回车。\n");
    printf("          如需更长时间请输入2。  XXX秒后关机请输入4。\n");
    printf("          自定义XX小时:XX分钟关机任务请输入9。\n");
    printf("          删除自定义XX小时:XX分钟关机请输入6。\n");
    printf("          查看当前任务请输入8。\n\n\n\n\n");

    printf("请输入（0、1、3、5、10、20、30、40、50、60）分后关机。\n\n请输入数字：");
    while (scanf_s("%d", &min) != 1)
    {
        flush();//清空缓冲区残余字符
        printf("\n输入错误，请重新输入：");
    }
    flush();


    switch (min)
    {
    case 0:system("shutdown -s -t 00"); break;
    case 1:system("shutdown -s -t 60"); break;
    case 2://关机（单位：小时） 
    {
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
        while (scanf_s("%d", &hour) != 1)//处理字符输入
        {
            printf("\n输入错误，请重新输入：");
            flush();//清空缓冲区残余字符
        }
        flush();

        switch (hour)
        {
        case 0:break;
        case 1:system("shutdown -s -t 5400"); break;
        case 2:system("shutdown -s -t 7200"); break;
        case 3:system("shutdown -s -t 9000"); break;
        case 4:system("shutdown -s -t 10800"); break;
        case 5:system("shutdown -s -t 12600"); break;
        case 6:system("shutdown -s -t 14400"); break;
        case 7:system("shutdown -s -t 16200"); break;
        case 8:system("shutdown -s -t 18000"); break;
        case 9:system("shutdown -s -t 19800"); break;
        default:printf("\n\n\t 输入错误！"); Sleep(1000); break;
        }
        break;
    }//关机（单位：小时）

    case 3:system("shutdown -s -t 180"); break;

    case 4://自定义时间关机
    {
        char shut[SIZE] = { "shutdown -s -t " };
        char tmp[SIZE] = { 0 };

        system("cls");
        printf("\n温馨提示：输入数字后请按回车，最大时间不能超过999999秒。\n\n\n\n\n");
        printf("温馨提示：只需输入数字即可，不必输入单位\n");
        printf("请输入多少秒后关机（单位：秒）：");

        scanf_s("%s", tmp, SIZE);
        flush();
        strcat_s(shut, sizeof(shut), tmp);//合并数组
        system(shut);//执行关机命令
        Sleep(1000);
        break;
    }

    case 9:
    {
        char schtasks[SIZE] = { "schtasks /create /tn \"关机\" /tr \"shutdown /s /t 00\" /sc once /st " };
        char shut[SIZE] = { 0 };

        system("cls");
        printf("\n温馨提示：设置关机的时间请不要小于或等于当前时间。\n\n\n\n");
        printf("请输入多少点关机。（例如：12:00）：");

        scanf_s("%s", shut, 6);
        flush();
        strcat_s(schtasks, sizeof(schtasks), shut);//合并数组
        system(schtasks);
        printf("\n\n如果显示“拒绝访问”字样，就请以管理员权限运行此软件。\n\n");
        system("pause");
        break;
    }

    case 5:system("shutdown -s -t 300"); break;

    case 6:
    {
        system("cls && schtasks /delete /tn 关机 /F");
        printf("\n\n\n\n\n如果显示“拒绝访问”字样，就请以管理员权限运行此软件。\n\n");
        system("pause");
        break;
    }

    case 7:break;

    case 8:
    {
        system("cls && schtasks > 当前任务.txt && 当前任务.txt");
        printf("\n\n\n文件已导出到当前程序目录\n\n");
        system("pause");
        break;
    }

    case 10:system("shutdown -s -t 600"); break;
    case 20:system("shutdown -s -t 1200"); break;
    case 30:system("shutdown -s -t 1800"); break;
    case 40:system("shutdown -s -t 2400"); break;
    case 50:system("shutdown -s -t 3000"); break;
    case 60:system("shutdown -s -t 3600"); break;
    default:printf("\n\n\t 输入错误！"); Sleep(1000); break;
    }
}



//重启
void reboot(void)
{
    int min, hour;

    system("mode con cols=55 lines=18 && color 06 && cls && title 重启 PS：请以管理员权限运行本软件");
    printf("温馨提示：在当前页面输入7返回。  输入数字后请按回车。\n");
    printf("          如需更长时间请输入2。  XXX秒后重启请输入3。\n");
    printf("          自定义XX小时:XX分钟重启任务请输入4。\n");
    printf("          删除自定义XX小时:XX分钟重启请输入6。\n");
    printf("          查看当前任务请输入8。\n\n\n\n\n");
    printf("请输入（0、1、5、10、20、30、40、50、60）分后重启。\n\n请输入数字：");

    while (scanf_s("%d", &min) != 1)
    {
        printf("\n输入错误，请重新输入：");
        flush();//清空缓冲区残余字符
    }

    switch (min)
    {
    case 0:system("shutdown -r -t 00"); break;
    case 1:system("shutdown -r -t 60"); break;
    case 2://重启（单位：小时） 
    {
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
        while (scanf_s("%d", &hour) != 1)//处理字符输入
        {
            printf("\n输入错误，请重新输入：");
            flush();//清空缓冲区残余字符
        }


        switch (hour)
        {
        case 0:break;
        case 1:system("shutdown -r -t 5400"); break;
        case 2:system("shutdown -r -t 7200"); break;
        case 3:system("shutdown -r -t 9000"); break;
        case 4:system("shutdown -r -t 10800"); break;
        case 5:system("shutdown -r -t 12600"); break;
        case 6:system("shutdown -r -t 14400"); break;
        case 7:system("shutdown -r -t 16200"); break;
        case 8:system("shutdown -r -t 18000"); break;
        case 9:system("shutdown -r -t 19800"); break;
        default:printf("\n\n\t 输入错误！"); Sleep(1000); break;
        }
        break;
    }//重启（单位：小时）

    case 3:
    {
        char reboot[SIZE] = { "shutdown -r -t " };
        char tmp[SIZE] = { 0 };

        system("cls");
        printf("\n温馨提示：输入数字后请按回车，最大时间不能超过999999秒。\n\n\n\n\n");
        printf("温馨提示：只需输入数字即可，不必输入单位\n");
        printf("请输入多少秒后重启（单位：秒）：");

        scanf_s("%s", tmp, 7);
        flush();
        strcat_s(reboot, 22, tmp);//合并数组
        system(reboot);//执行重启命令
        Sleep(1000);
        break;
    }
    case 4:
    {
        char schtasks[SIZE] = { "schtasks /create /tn \"重启\" /tr \"shutdown /r /t 00\" /sc once /st /0" };
        char reboot[SIZE] = { 0 };

        system("cls");
        printf("\n温馨提示：设置重启的时间请不要小于或等于当前时间。\n\n\n\n");
        printf("请输入多少点重启。（例如：12:00）：");

        scanf_s("%s", reboot, 6);
        flush();
        strcat_s(schtasks, 76, reboot);
        system(schtasks);
        printf("\n\n如果显示“拒绝访问”字样，请以管理员权限运行此软件。\n\n");
        system("pause");
        break;
    }

    case 5:system("shutdown -r -t 300"); break;

    case 6:
    {
        system("cls && schtasks /delete /tn 重启 /F");
        printf("\n\n\n\n\n如果显示“拒绝访问”字样，请以管理员权限运行此软件。\n\n");
        system("pause");
        break;
    }

    case 7:break;

    case 8:
    {
        system("cls && schtasks > 当前任务.txt && 当前任务.txt");
        printf("\n\n\n文件已导出到当前程序目录\n\n");
        system("pause");
        break;
    }

    case 10:system("shutdown -r -t 600"); break;
    case 20:system("shutdown -r -t 1200"); break;
    case 30:system("shutdown -r -t 1800"); break;
    case 40:system("shutdown -r -t 2400"); break;
    case 50:system("shutdown -r -t 3000"); break;
    case 60:system("shutdown -r -t 3600"); break;
    default:printf("\n\n\t 输入错误！"); Sleep(1000); break;
    }
}


//清除缓冲区
void flush(void)
{
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF)
        ;
}


//删除文件
void  delfile(int mode)
{
    if (mode == DEL_FILE)
    {
        remove("本机IP信息.txt");
        remove("系统信息.txt");
        remove("获取硬件信息.bat");
        remove("进程信息.txt");
        remove("使用说明.txt");
    }
    else
    {
        char command[MAX_PATH] = { 0 }, cmdpath[MAX_PATH], filepath[MAX_PATH];

        if (!_access("system.dll", 0))
        {
            if (remove("system.dll"))
            {
                MessageBox(NULL, L"删除失败！", L"错误", MB_OK | MB_ICONERROR);
                exit(EXIT_FAILURE);
            }
        }

        // 获得程序自身路径和cmd路径，并删除system.dll
        if ((GetModuleFileNameA(NULL, filepath, MAX_PATH) != 0) && (GetEnvironmentVariableA("COMSPEC", cmdpath, MAX_PATH) != 0))
        {
            sprintf_s(command, sizeof(command), "\"%s\" /c del /q \"%s\" > 1.txt", cmdpath, filepath);// 命令参数
            SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);				// 提高自身进程的优先级
            SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
            SHChangeNotify(SHCNE_DELETE, SHCNF_PATH, filepath, NULL);					// 通知资源浏览器,本程序已经被删除
            ShellExecuteA(NULL, "open", cmdpath, command, NULL, SW_HIDE);
        }
        else
            MessageBox(NULL, L"删除失败！", L"错误", MB_OK | MB_ICONERROR);
        exit(EXIT_SUCCESS);
    }
}


void get_startup_directory(void)
{
    TCHAR path[MAX_PATH], cmd[MAX_PATH] = { 0 };

    if (GetEnvironmentVariable(L"APPDATA", path, MAX_PATH) == 0) //获取用户文件夹
        wcscpy_s(cmd, MAX_PATH, L"\"C:\\Users\\Administrator\\AppData\\Roaming\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\"");//获取失败则默认路径
    else
        swprintf_s(cmd, MAX_PATH, L"\"%s\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\"", path);
    ShellExecute(NULL, L"open", L"explorer.exe", cmd, NULL, SW_SHOWNORMAL);
}
