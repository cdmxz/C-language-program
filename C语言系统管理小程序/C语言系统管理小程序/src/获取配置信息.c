#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <Iphlpapi.h>

#include "str_util.h"

#pragma comment(lib,"Iphlpapi.lib")

#define MAX_SIZE 1024*4

// 获取系统安装时间
BOOL GetSysInstallTime(char* timeStr, size_t strSize);
// 获取主板信息
void GetMainboardInfo(char* info, size_t strSize);
// 获取操作系统版本
BOOL GetOsVersion(char* osVer, size_t strSize);
// 获取网卡名称
void GetInterFaceInfo(char* info, size_t strSize);
// 获取物理内存
long long GetPhysicalMemory(void);
// 获取CPU信息
void GetCpuInfo(char* cpuInfo, size_t strSize);
// 获取硬盘信息
void GetDiskInfo(char* driveInfo, size_t strSize);
// 获取显卡信息
void GetDisplayCardInfo(char* cardInfo, size_t strSize);
// 获取错误信息
void GetErrorMessage(DWORD errCode, char* msg, size_t strSize);
// 打印错误信息
void PrintErrMsg(char* str);
// 判断系统是否为64位
BOOL Is64BitSystem(void);
// 获取网卡类型
void GetInterFaceType(UINT type, char* type_str, size_t strSize);
// 获取网卡mac
void GetAdaptersMac(PIP_ADAPTER_INFO pip_info, char* str, size_t strSize);
// 获取用户名
void GetComputerUserName(char* name, size_t strSize);

void print_config(void)
{
	FILE* fp = NULL;
	char fileName[] = { "配置信息.txt" };
	char osVer[MAX_PATH], time[MAX_PATH], cpuInfo[MAX_PATH], cardInfo[MAX_PATH], userName[MAX_PATH];
	char disk[MAX_SIZE], interFace[MAX_SIZE], boardInfo[MAX_SIZE];
	// 获取分辨率
	int w = GetSystemMetrics(SM_CXSCREEN);
	int h = GetSystemMetrics(SM_CYSCREEN);
	long long memorySize = GetPhysicalMemory();
	GetOsVersion(osVer, sizeof(osVer));
	GetSysInstallTime(time, sizeof(time));
	GetCpuInfo(cpuInfo, sizeof(cpuInfo));
	GetMainboardInfo(boardInfo, sizeof(boardInfo));
	GetDisplayCardInfo(cardInfo, sizeof(cardInfo));
	GetDiskInfo(disk, sizeof(disk));
	GetInterFaceInfo(interFace, sizeof(interFace));
	GetComputerUserName(userName, sizeof(userName));

	if (fopen_s(&fp, fileName, "w") != 0)
	{
		perror("创建文件错误");
		system("pause");
		return;
	}
	fprintf(fp, "CPU： %s\n", cpuInfo);
	fprintf(fp, "主板：%s\n", boardInfo);
	fprintf(fp, "内存：%.0lfMB\n", memorySize / (1024.0 * 1024.0));
	fprintf(fp, "显卡：%s\n", cardInfo);
	fprintf(fp, "硬盘：\n    %s\r", disk);
	fprintf(fp, "网卡：\n    %s\r", interFace);
	fprintf(fp, "分辨率：%dx%d\n", w, h);
	fprintf(fp, "系统：%s\n", osVer);
	fprintf(fp, "用户名：%s\n", userName);
	fprintf(fp, "系统安装时间：%s\n", time);
	fclose(fp);
	system(fileName);
}

// 获取系统安装时间
BOOL GetSysInstallTime(char* timeStr, size_t strSize)
{
	time_t installDate = 0;
	DWORD size = sizeof(installDate);
	LSTATUS status;
	char* subKey = "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion";
	struct tm tmTime;
	HKEY hkey = NULL;
	ZeroMemory(timeStr, strSize);
	status = RegOpenKeyExA(HKEY_LOCAL_MACHINE, subKey, 0, KEY_QUERY_VALUE | KEY_WOW64_64KEY, &hkey);
	if (status != ERROR_SUCCESS)
	{
		PrintErrMsg("获取系统安装时间失败");
		return FALSE;
	}
	status = RegQueryValueExA(hkey, "InstallDate", 0, NULL, &(BYTE)installDate, &size);
	RegCloseKey(hkey);
	if (status != ERROR_SUCCESS)
	{
		PrintErrMsg("获取系统安装时间失败");
		return FALSE;
	}
	localtime_s(&tmTime, &installDate);
	strftime(timeStr, strSize, "%Y/%m/%d %H:%M:%S", &tmTime);
	return TRUE;
}

// 获取主板信息
void GetMainboardInfo(char* info, size_t strSize)
{
	ZeroMemory(info, strSize);
	char* subKey = "HARDWARE\\DESCRIPTION\\System\\BIOS";
	// char brand[MAX_PATH], product[MAX_PATH], BIOSReleaseDate[MAX_PATH], BIOSVersion[MAX_PATH];
	char product[MAX_PATH] = { 0 };
	DWORD size = sizeof(product);
	RegGetValueA(HKEY_LOCAL_MACHINE, subKey, "BaseBoardProduct", RRF_RT_REG_SZ, NULL, product, &size);
	/* RegGetValueA(HKEY_LOCAL_MACHINE, subKey, TEXT("BaseBoardManufacturer"), RRF_RT_REG_SZ, NULL, brand, &size);
	RegGetValueA(HKEY_LOCAL_MACHINE, subKey, TEXT("BIOSReleaseDate"), RRF_RT_REG_SZ, NULL, BIOSReleaseDate, &size);
	RegGetValueA(HKEY_LOCAL_MACHINE, subKey, TEXT("BIOSVersion"), RRF_RT_REG_SZ, NULL, BIOSVersion, &size);*/
	// sprintf_s(info, strSize, "品牌：%s\n    型号：%s\n    BIOS日期：%s\n    BIOS版本：%s", brand, product, BIOSReleaseDate, BIOSVersion);
	sprintf_s(info, strSize, "%s", product);
}

// 获取用户名
void GetComputerUserName(char* name, size_t strSize)
{
	ZeroMemory(name, strSize);
	char buff[MAX_PATH];
	int len = sizeof(buff);
	GetComputerNameA(buff, &len);
	strcpy_s(name, strSize, buff);
}

// 获取操作系统版本
BOOL GetOsVersion(char* osVer, size_t strSize)
{
	DWORD bufSize = MAX_PATH;
	char productName[MAX_PATH], displayVersion[MAX_PATH];
	char bit[] = { "x86" };
	HKEY hkey;
	char* subKey = "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion";
	LSTATUS status;
	ZeroMemory(osVer, strSize);
	// 打开注册表
	status = RegOpenKeyExA(HKEY_LOCAL_MACHINE, subKey, 0, KEY_QUERY_VALUE | KEY_WOW64_64KEY, &hkey);
	if (status != ERROR_SUCCESS)
	{
		PrintErrMsg("获取操作系统版本失败");
		return FALSE;
	}
	RegQueryValueExA(hkey, "ProductName", NULL, NULL, (LPBYTE)productName, &bufSize);
	RegQueryValueExA(hkey, "ReleaseId", NULL, NULL, (LPBYTE)displayVersion, &bufSize);
	RegCloseKey(hkey);
	if (Is64BitSystem())
		strcpy_s(bit, sizeof(bit), "x64");
	sprintf_s(osVer, strSize, "%s %s %s", productName, displayVersion, bit);
	return TRUE;
}

// 判断是否为64位系统
BOOL Is64BitSystem(void)
{
	SYSTEM_INFO sysInfo;
	GetNativeSystemInfo(&sysInfo);
	return(sysInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64);
}

// 获取CPU信息
void GetCpuInfo(char* cpuInfo, size_t strSize)
{
	char str[MAX_PATH];
	SYSTEM_INFO si;
	DWORD size = sizeof(str);
	ZeroMemory(&si, sizeof(SYSTEM_INFO));
	ZeroMemory(cpuInfo, strSize);
	RegGetValueA(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\SYSTEM\\CentralProcessor\\0", "ProcessorNameString", RRF_RT_ANY, NULL, (PVOID)str, &size);
	GetSystemInfo(&si);
	sprintf_s(cpuInfo, strSize, "%s %ld核", str, si.dwNumberOfProcessors);
}

// 获取物理内存大小
long long GetPhysicalMemory(void)
{
	MEMORYSTATUSEX memory;
	memory.dwLength = sizeof(memory);
	GlobalMemoryStatusEx(&memory);
	return memory.ullTotalPhys;
}

// 获取显卡信息
void GetDisplayCardInfo(char* cardInfo, size_t strSize)
{
	char info[MAX_PATH], data[MAX_PATH] = { 0 };
	char* p = NULL;
	int info_size = sizeof(info);
	int data_size = sizeof(data);
	ZeroMemory(cardInfo, strSize);
	HKEY hKey;
	LSTATUS result = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "HARDWARE\\DEVICEMAP\\VIDEO", 0, KEY_QUERY_VALUE, &hKey);
	if (result == ERROR_SUCCESS)
	{
		result = RegQueryValueExA(hKey, "\\Device\\Video0", NULL, NULL, info, &info_size);
		p = strstr(info, "\\S");
		if (result == ERROR_SUCCESS && p)
		{
			result = RegGetValueA(HKEY_LOCAL_MACHINE, p + 1, "Device Description", RRF_RT_ANY, NULL, (PVOID)data, &data_size);
			if (result == ERROR_FILE_NOT_FOUND)
				result = RegGetValueA(HKEY_LOCAL_MACHINE, p + 1, "DriverDesc", RRF_RT_ANY, NULL, (PVOID)data, &data_size);
			if (result == ERROR_SUCCESS)
			{
				sprintf_s(cardInfo, strSize, "%s", data);
				//  sprintf_s(cardInfo, strSize, "型号：%s\n    驱动安装日期：%s", data, time);
				CloseHandle(hKey);
				return;
			}
		}
	}
	PrintErrMsg("获取显卡信息失败");
	CloseHandle(hKey);
}

// 获取硬盘信息
void GetDiskInfo(char* driveInfo, size_t strSize)
{
	char buff[MAX_SIZE] = { 0 }, drives[MAX_PATH], drive_type[MAX_PATH], temp[MAX_PATH] = { 0 };
	char* p = drives;
	BOOL result;
	ULARGE_INTEGER FreeBytesAvailableToCaller, TotalNumberOfBytes, TotalNumberOfFreeBytes;
	DWORD logic = GetLogicalDriveStringsA(sizeof(drives) - 1, drives);
	ZeroMemory(driveInfo, strSize);
	if (logic == 0)
	{
		PrintErrMsg("获取硬盘信息失败");
		return;
	}
	while (*p)
	{ 
		switch (GetDriveTypeA(p))
		{
		case DRIVE_UNKNOWN:
			strcpy_s(drive_type, sizeof(drive_type), "未知设备");
			break;
		case DRIVE_REMOVABLE:
			strcpy_s(drive_type, sizeof(drive_type), "可移动磁盘");
			break;
		case DRIVE_FIXED:
			strcpy_s(drive_type, sizeof(drive_type), "本地磁盘");
			break;
		case DRIVE_REMOTE:
			strcpy_s(drive_type, sizeof(drive_type), "网络磁盘");
			break;
		case DRIVE_CDROM:
			strcpy_s(drive_type, sizeof(drive_type), "光盘驱动器");
			break;
		case DRIVE_RAMDISK:
			strcpy_s(drive_type, sizeof(drive_type), "虚拟RAM磁盘");
			break;
		default:
			strcpy_s(drive_type, sizeof(drive_type), "未知设备");
			break;
		}
		result = GetDiskFreeSpaceExA(p, &FreeBytesAvailableToCaller, &TotalNumberOfBytes, &TotalNumberOfFreeBytes);
		if (result)
		{
			del_lastch(p, '\\');
			sprintf_s(temp, sizeof(temp), "%s%s  总计：%0.2lfGB  已用：%0.2lfGB  剩余：%0.2lfGB\n    ",
				drive_type, p, TotalNumberOfBytes.QuadPart / 1024.0 / 1024.0 / 1024.0, FreeBytesAvailableToCaller.QuadPart / 1024.0 / 1024.0 / 1024.0, TotalNumberOfFreeBytes.QuadPart / 1024.0 / 1024.0 / 1024.0);
		}
		strcat_s(buff, sizeof(buff), temp);
		// 跳过每个磁盘名称后的"\0"符号
		p += strlen(p) + 2;// 应为del_lastch函数把\\改成\0，所以加上2
	}
	strcpy_s(driveInfo, strSize, buff);
}

// 获取网卡名称
void GetInterFaceInfo(char* info, size_t strSize)
{
	char buff[MAX_SIZE] = { 0 }, temp[MAX_SIZE], type[MAX_PATH], mac[MAX_PATH];
	int card_num = 0;
	int pip_size = sizeof(PIP_ADAPTER_INFO);
	ZeroMemory(info, strSize);
	GetAdaptersInfo(NULL, &pip_size);
	PIP_ADAPTER_INFO pip_info = (IP_ADAPTER_INFO*)malloc(pip_size);
	if (!pip_info)
		return;
	GetAdaptersInfo(pip_info, &pip_size);
	while (pip_info)
	{
		GetAdaptersMac(pip_info, mac, sizeof(mac));
		GetInterFaceType(pip_info->Type, type, sizeof(type));
		sprintf_s(temp, sizeof(temp), "名称：%s\n    描述：%s\n    类型：%s\n    IP：%s\n    MAC：%s\n    ", pip_info->AdapterName, pip_info->Description, type, pip_info->IpAddressList.IpAddress.String, mac);
		card_num++;
		strcat_s(buff, sizeof(buff), temp);
		pip_info = pip_info->Next;
	}
	sprintf_s(info, strSize, "总数：%d\n    %s", card_num, buff);
	free(pip_info);
}

// 获取网卡mac
void GetAdaptersMac(PIP_ADAPTER_INFO pip_info, char* str, size_t strSize)
{
	ZeroMemory(str, strSize);
	int len = pip_info->AddressLength;
	char* temp = (char*)calloc(len, sizeof(char));
	if (!temp)
		return;
	for (UINT i = 0; i < pip_info->AddressLength; i++)
	{
		if (i < pip_info->AddressLength - 1)
			sprintf_s(temp, len, "%02X-", pip_info->Address[i]);
		else
			sprintf_s(temp, len, "%02X", pip_info->Address[i]);
		strcat_s(str, strSize, temp);
	}
	free(temp);
}

// 获取网卡类型
void GetInterFaceType(UINT type, char* type_str, size_t strSize)
{
	ZeroMemory(type_str, strSize);
	switch (type)
	{
	case MIB_IF_TYPE_OTHER:
		strcpy_s(type_str, strSize, "OTHER");
		break;
	case MIB_IF_TYPE_ETHERNET:
		strcpy_s(type_str, strSize, "ETHERNET");
		break;
	case MIB_IF_TYPE_TOKENRING:
		strcpy_s(type_str, strSize, "TOKENRING");
		break;
	case MIB_IF_TYPE_FDDI:
		strcpy_s(type_str, strSize, "FDDI");
		break;
	case MIB_IF_TYPE_PPP:
		strcpy_s(type_str, strSize, "PPP");
		break;
	case MIB_IF_TYPE_LOOPBACK:
		strcpy_s(type_str, strSize, "LOOPBACK");
		break;
	case MIB_IF_TYPE_SLIP:
		strcpy_s(type_str, strSize, "SLIP");
		break;
	}
}

void PrintErrMsg(char* str)
{
	char msg[MAX_PATH];
	GetErrorMessage(GetLastError(), msg, sizeof(msg));
	printf("%s：%s\n", str, msg);
}

void GetErrorMessage(DWORD errCode, char* msg, size_t strSize)
{
	if (!msg || strSize < 1)
		return;
	ZeroMemory(msg, strSize);
	LPVOID lpBuf = NULL;
	DWORD bufSize = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		errCode,
		LANG_USER_DEFAULT,
		(LPSTR)&lpBuf,
		0,
		NULL);
	strncpy_s(msg, strSize, lpBuf, bufSize);
	LocalFree(lpBuf);
}