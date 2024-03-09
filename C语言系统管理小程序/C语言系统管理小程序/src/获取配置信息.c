#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <Iphlpapi.h>

#include "str_util.h"

#pragma comment(lib,"Iphlpapi.lib")

#define MAX_SIZE 1024*4

// ��ȡϵͳ��װʱ��
BOOL GetSysInstallTime(char* timeStr, size_t strSize);
// ��ȡ������Ϣ
void GetMainboardInfo(char* info, size_t strSize);
// ��ȡ����ϵͳ�汾
BOOL GetOsVersion(char* osVer, size_t strSize);
// ��ȡ��������
void GetInterFaceInfo(char* info, size_t strSize);
// ��ȡ�����ڴ�
long long GetPhysicalMemory(void);
// ��ȡCPU��Ϣ
void GetCpuInfo(char* cpuInfo, size_t strSize);
// ��ȡӲ����Ϣ
void GetDiskInfo(char* driveInfo, size_t strSize);
// ��ȡ�Կ���Ϣ
void GetDisplayCardInfo(char* cardInfo, size_t strSize);
// ��ȡ������Ϣ
void GetErrorMessage(DWORD errCode, char* msg, size_t strSize);
// ��ӡ������Ϣ
void PrintErrMsg(char* str);
// �ж�ϵͳ�Ƿ�Ϊ64λ
BOOL Is64BitSystem(void);
// ��ȡ��������
void GetInterFaceType(UINT type, char* type_str, size_t strSize);
// ��ȡ����mac
void GetAdaptersMac(PIP_ADAPTER_INFO pip_info, char* str, size_t strSize);
// ��ȡ�û���
void GetComputerUserName(char* name, size_t strSize);

void print_config(void)
{
	FILE* fp = NULL;
	char fileName[] = { "������Ϣ.txt" };
	char osVer[MAX_PATH], time[MAX_PATH], cpuInfo[MAX_PATH], cardInfo[MAX_PATH], userName[MAX_PATH];
	char disk[MAX_SIZE], interFace[MAX_SIZE], boardInfo[MAX_SIZE];
	// ��ȡ�ֱ���
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
		perror("�����ļ�����");
		system("pause");
		return;
	}
	fprintf(fp, "CPU�� %s\n", cpuInfo);
	fprintf(fp, "���壺%s\n", boardInfo);
	fprintf(fp, "�ڴ棺%.0lfMB\n", memorySize / (1024.0 * 1024.0));
	fprintf(fp, "�Կ���%s\n", cardInfo);
	fprintf(fp, "Ӳ�̣�\n    %s\r", disk);
	fprintf(fp, "������\n    %s\r", interFace);
	fprintf(fp, "�ֱ��ʣ�%dx%d\n", w, h);
	fprintf(fp, "ϵͳ��%s\n", osVer);
	fprintf(fp, "�û�����%s\n", userName);
	fprintf(fp, "ϵͳ��װʱ�䣺%s\n", time);
	fclose(fp);
	system(fileName);
}

// ��ȡϵͳ��װʱ��
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
		PrintErrMsg("��ȡϵͳ��װʱ��ʧ��");
		return FALSE;
	}
	status = RegQueryValueExA(hkey, "InstallDate", 0, NULL, &(BYTE)installDate, &size);
	RegCloseKey(hkey);
	if (status != ERROR_SUCCESS)
	{
		PrintErrMsg("��ȡϵͳ��װʱ��ʧ��");
		return FALSE;
	}
	localtime_s(&tmTime, &installDate);
	strftime(timeStr, strSize, "%Y/%m/%d %H:%M:%S", &tmTime);
	return TRUE;
}

// ��ȡ������Ϣ
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
	// sprintf_s(info, strSize, "Ʒ�ƣ�%s\n    �ͺţ�%s\n    BIOS���ڣ�%s\n    BIOS�汾��%s", brand, product, BIOSReleaseDate, BIOSVersion);
	sprintf_s(info, strSize, "%s", product);
}

// ��ȡ�û���
void GetComputerUserName(char* name, size_t strSize)
{
	ZeroMemory(name, strSize);
	char buff[MAX_PATH];
	int len = sizeof(buff);
	GetComputerNameA(buff, &len);
	strcpy_s(name, strSize, buff);
}

// ��ȡ����ϵͳ�汾
BOOL GetOsVersion(char* osVer, size_t strSize)
{
	DWORD bufSize = MAX_PATH;
	char productName[MAX_PATH], displayVersion[MAX_PATH];
	char bit[] = { "x86" };
	HKEY hkey;
	char* subKey = "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion";
	LSTATUS status;
	ZeroMemory(osVer, strSize);
	// ��ע���
	status = RegOpenKeyExA(HKEY_LOCAL_MACHINE, subKey, 0, KEY_QUERY_VALUE | KEY_WOW64_64KEY, &hkey);
	if (status != ERROR_SUCCESS)
	{
		PrintErrMsg("��ȡ����ϵͳ�汾ʧ��");
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

// �ж��Ƿ�Ϊ64λϵͳ
BOOL Is64BitSystem(void)
{
	SYSTEM_INFO sysInfo;
	GetNativeSystemInfo(&sysInfo);
	return(sysInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64);
}

// ��ȡCPU��Ϣ
void GetCpuInfo(char* cpuInfo, size_t strSize)
{
	char str[MAX_PATH];
	SYSTEM_INFO si;
	DWORD size = sizeof(str);
	ZeroMemory(&si, sizeof(SYSTEM_INFO));
	ZeroMemory(cpuInfo, strSize);
	RegGetValueA(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\SYSTEM\\CentralProcessor\\0", "ProcessorNameString", RRF_RT_ANY, NULL, (PVOID)str, &size);
	GetSystemInfo(&si);
	sprintf_s(cpuInfo, strSize, "%s %ld��", str, si.dwNumberOfProcessors);
}

// ��ȡ�����ڴ��С
long long GetPhysicalMemory(void)
{
	MEMORYSTATUSEX memory;
	memory.dwLength = sizeof(memory);
	GlobalMemoryStatusEx(&memory);
	return memory.ullTotalPhys;
}

// ��ȡ�Կ���Ϣ
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
				//  sprintf_s(cardInfo, strSize, "�ͺţ�%s\n    ������װ���ڣ�%s", data, time);
				CloseHandle(hKey);
				return;
			}
		}
	}
	PrintErrMsg("��ȡ�Կ���Ϣʧ��");
	CloseHandle(hKey);
}

// ��ȡӲ����Ϣ
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
		PrintErrMsg("��ȡӲ����Ϣʧ��");
		return;
	}
	while (*p)
	{ 
		switch (GetDriveTypeA(p))
		{
		case DRIVE_UNKNOWN:
			strcpy_s(drive_type, sizeof(drive_type), "δ֪�豸");
			break;
		case DRIVE_REMOVABLE:
			strcpy_s(drive_type, sizeof(drive_type), "���ƶ�����");
			break;
		case DRIVE_FIXED:
			strcpy_s(drive_type, sizeof(drive_type), "���ش���");
			break;
		case DRIVE_REMOTE:
			strcpy_s(drive_type, sizeof(drive_type), "�������");
			break;
		case DRIVE_CDROM:
			strcpy_s(drive_type, sizeof(drive_type), "����������");
			break;
		case DRIVE_RAMDISK:
			strcpy_s(drive_type, sizeof(drive_type), "����RAM����");
			break;
		default:
			strcpy_s(drive_type, sizeof(drive_type), "δ֪�豸");
			break;
		}
		result = GetDiskFreeSpaceExA(p, &FreeBytesAvailableToCaller, &TotalNumberOfBytes, &TotalNumberOfFreeBytes);
		if (result)
		{
			del_lastch(p, '\\');
			sprintf_s(temp, sizeof(temp), "%s%s  �ܼƣ�%0.2lfGB  ���ã�%0.2lfGB  ʣ�ࣺ%0.2lfGB\n    ",
				drive_type, p, TotalNumberOfBytes.QuadPart / 1024.0 / 1024.0 / 1024.0, FreeBytesAvailableToCaller.QuadPart / 1024.0 / 1024.0 / 1024.0, TotalNumberOfFreeBytes.QuadPart / 1024.0 / 1024.0 / 1024.0);
		}
		strcat_s(buff, sizeof(buff), temp);
		// ����ÿ���������ƺ��"\0"����
		p += strlen(p) + 2;// ӦΪdel_lastch������\\�ĳ�\0�����Լ���2
	}
	strcpy_s(driveInfo, strSize, buff);
}

// ��ȡ��������
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
		sprintf_s(temp, sizeof(temp), "���ƣ�%s\n    ������%s\n    ���ͣ�%s\n    IP��%s\n    MAC��%s\n    ", pip_info->AdapterName, pip_info->Description, type, pip_info->IpAddressList.IpAddress.String, mac);
		card_num++;
		strcat_s(buff, sizeof(buff), temp);
		pip_info = pip_info->Next;
	}
	sprintf_s(info, strSize, "������%d\n    %s", card_num, buff);
	free(pip_info);
}

// ��ȡ����mac
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

// ��ȡ��������
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
	printf("%s��%s\n", str, msg);
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