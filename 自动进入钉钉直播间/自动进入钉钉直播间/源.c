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

char runPath[MAX_PATH] = { 0 };//������װ·��
char logPath[MAX_PATH] = { 0 };//��־�ļ�·��
char RGBFilePath[MAX_PATH] = { 0 };//�Զ���RGB�ļ�·��
char FileContent[MAX_PATH] = { 0 };//�Զ���RGB�ļ�����
char RGB[] =//�Դ�RGB����
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
	////���Ҵ��ھ��
	//HWND hwnd = FindWindowA(NULL, "����");

	////������ʾ��������
	//SetForegroundWindow(hwnd);

	////��ȡ������������
	//RECT re;
	//GetWindowRect(hwnd, &re);
	//
	//printf("x %ld,y %ld", re.left, re.top);
	//system("pause");
	//return 0;

	system("title �Զ����붤��ֱ����");
	printf("����Զ����붤��ֱ����.exe  <��������ѡ��>\n"
		"������\n"
		"1���޲��� ��������������ֱ���䣩\n"
		"2��xy    ����ʾ����������ɫ��\n"
		"3��add    �������������\n"
		"4��del    ��ɾ����������\n"
		"5��cre    �������Զ���RGB�ļ�ģ�壩\n\n");


	char deskPath[MAX_PATH];
	SHGetSpecialFolderPathA(0, deskPath, CSIDL_DESKTOPDIRECTORY, 0);//��ȡ����·��
	sprintf_s(logPath, MAX_PATH, "%s\\�Զ����붤��ֱ����.log", deskPath);//��ȡ��־�ļ�·��
	sprintf_s(RGBFilePath, MAX_PATH, "%s\\�Զ���RGB.txt", deskPath);//��ȡ�Զ�����ɫ�ļ�·��

	if (argc == 2)
	{
		if (strcmp(argv[1], "xy") == 0)
			GetMousePos(NULL);//��ʾ����������ɫ
		else if (strcmp(argv[1], "add") == 0)
			Add();//���������
		else if (strcmp(argv[1], "del") == 0)
			Del();//ɾ��������
		else if (strcmp(argv[1], "cre") == 0)
			Create();
	}


	//system("mode con cols=45 lines=15");//���ô��ڴ�С


	OpenDingDing();//�򿪶���
	Sleep(3000);//�ȴ���������
	HWND _hwnd = FindWindowA(NULL, "����");//��ȡ�������ھ��
	if (!_hwnd)
	{
		WriteLog("��ȡ�������ھ��ʧ�ܣ�");
		exit(EXIT_FAILURE);
	}


	printf("��ȡ�������ھ���ɹ�\n");

	//Ѱ�Ҷ�������
	for (int i = 1; i <= 10; i++)
	{
		if (GetProcess("DingTalk.exe") == true)
			break;

		Sleep(3000);//δ�ҵ����̾͵ȴ�2������

		if (i == 10)//�������20�ζ�δ�ҵ�
		{
			WriteLog("δ�ҵ���������");
			exit(EXIT_FAILURE);
		}
	}


	printf("�ҵ���������\n");

	//�����������ƶ�����Ļ���Ͻǲ����ô��ڸ߶ȺͿ��
	MoveWindow(_hwnd, 0, 0, 930, 640, true);

	printf("�ƶ��������ڵ����Ͻ�\n");

	FILE* fp;
	unsigned i = 0;
	int ch, x, y, color = 0, j = 0;
	char currentRGB[20] = { 0 }, fileRGB[20] = { 0 }, tmp[25] = { 0 };

	if (!_access(RGBFilePath, 0))//�ж��ļ��Ƿ����
	{

		printf("�ҵ��Զ���RGB�ļ�\n");

		if (fopen_s(&fp, RGBFilePath, "r") == 0)//�ж��Ƿ���ȷ���ļ�
		{
			if (_filelength(_fileno(fp)))//�ļ���Ϊ��
			{
				//��ȡ�ļ�����
				for (i = 0; i < strlen(FileContent) + 1; i++)
				{
					if ((ch = fgetc(fp)) != EOF)
						FileContent[i] = ch;
				}
				fclose(fp);//�ر��ļ�

				if (FileContent[strlen(FileContent)] != '\n')
					FileContent[strlen(FileContent)] = '\n';
				Deln(FileContent);//ɾ������ǰ���\n
			}
			else
				fclose(fp);
		}
	}
	else
		printf("δ�ҵ��Զ���RGB�ļ�\n");

	printf("�Զ���RGB�ļ�·���� %s\n", RGBFilePath);
	printf("����·��%s\n", runPath);
	printf("��־�ļ�·�� %s\n", logPath);
	printf("�Դ�RGB����%s\n", RGB);


	if (FileContent[0] != '\0')//��������Զ����ļ�
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

				SetCursorPos(x, y);//�������λ��
				GetMousePos(currentRGB);//��ȡ��ǰ���λ�õ�RGB
				printf("��ǰλ�ã�%d,%d RGB��%s\n", x, y, currentRGB);


				if (strcmp(currentRGB, fileRGB) == 0)//�����굱ǰ�����RGB���ļ��ϵ�RGB��ͬ
					break;

				memset(tmp, 0, 20);//��������
				j = 0;
				Sleep(3000);//δƥ�䵽��ȷRGB����ȴ�3���ٴ�ƥ��
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

			SplitStr(tmp, &x, &y, fileRGB, sizeof(fileRGB));//�ָ��ַ���	

			SetCursorPos(x, y);//�������λ��
			GetMousePos(currentRGB);//��ȡ��ǰ���λ�õ�RGB
			printf("��ǰλ�ã�%d,%d RGB��%s\n", x, y, currentRGB);

			if (strcmp(fileRGB, currentRGB) == 0)//�����굱ǰ�����RGB���ļ��ϵ�RGB��ͬ
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

	//ģ�����������µ���
	mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);

	printf("���\n");
	Sleep(10000);
	//	system("pause");
	return 0;
}

//ɾ���ַ�����ͷ��'\n'
void Deln(char* str)
{
	if (str[0] == '\n')
	{
		unsigned i = 0;
		for (i = 0; i < strlen(str); i++)
			str[i] = str[i + 1];
	}
}

//�ָ��ַ���
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
			memset(tmp, 0, 10);//��������
			continue;
		}
		if (str[i] == ',' || str[i] == '��' || str[i] == ' ' || str[i] == '\n' || str[i] == '\0')
			continue;
		tmp[j++] = str[i];
	}


	*X = x;
	*Y = y;
	sprintf_s(RGB, RGBSize, "%d%d%d", r, g, b);
}

//�򿪶���
void OpenDingDing(void)
{
	//����ע���·��
	char* key = "SOFTWARE\\WOW6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\����";
	HANDLE _handle = NULL;
	char* find = NULL;//���Ҷ�����װ·�������һ����\����λ��
	HKEY hkey;
	DWORD dwSize = MAX_PATH;
	char path[MAX_PATH];

	//��ȡϵͳ��64λ����32λ
	SYSTEM_INFO si;
	GetNativeSystemInfo(&si);
	if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_INTEL)//32λ
		key = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\����";


	//��ע���
	if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, key, 0, REG_SZ, &hkey) == ERROR_SUCCESS)
	{
		//��ע����ȡ��ֵ
		if (RegQueryValueExA(hkey, "UninstallString", NULL, NULL, (LPBYTE)&path, &dwSize) == ERROR_SUCCESS)
		{
			//��ȡ����ж�س���·��������Ҫ��·���зָ����װ�ļ���
			if ((find = strrchr(path, '\\')) != NULL)
			{
				*(++find) = '\0';

				strcat_s(path, MAX_PATH, "DingtalkLauncher.exe");

				//�жϴ�ע����ж�ȡ�Ķ�����װ·���Ƿ����
				if (!_access(path, 0))
				{
					sprintf_s(runPath, MAX_PATH, "\"%s\"", path);
					system(runPath);//��������

					//_handle = (HANDLE)_beginthreadex(NULL, 0, Start, NULL, 0, NULL);//�����߳�
					//if (_handle)
					//	CloseHandle(_handle);//�ͷž��

					Sleep(3000);
					return;//����
				}
				else
					WriteLog("ע���Ѱ�ҵ��Ķ�����װ·����Ч��");
			}
			else
				WriteLog("ע����ȡ���Ķ�����װ·����Ч��");
		}
		else
			WriteLog("��ȡ��ֵ��UninstallString��ʧ�ܣ�");
	}
	else
		WriteLog("��ע���ʧ��");


	//������Ѱ�Ҷ�����ݷ�ʽ
	sprintf_s(runPath, MAX_PATH, "\"C:\\Users\\Public\\Desktop\\����.lnk\"");

	//�ж�·���Ƿ����
	if (_access(runPath, 0))
	{
		WriteLog("�޷���ȡ����Ķ�����ݷ�ʽ��");
		exit(EXIT_FAILURE);
	}

	//_handle = (HANDLE)_beginthreadex(NULL, 0, Start, NULL, 0, NULL);//�����߳�
	//if (_handle)
	//	CloseHandle(_handle);//�ͷž��
	system(runPath);//��������
}


//д����־
void WriteLog(const char* err)
{
	time_t t;
	FILE* fp;

	//��ȡ��ǰʱ��
	struct tm t_time;
	time(&t);
	localtime_s(&t_time, &t);

	//���ļ�
	if (fopen_s(&fp, logPath, "a+") != 0)
	{
		printf("����־�ļ�ʧ�ܣ�\n");
		perror("ԭ��");
		system("pause");
		exit(EXIT_FAILURE);
	}


	//���ļ�д�����ԭ���ʱ��
	fprintf_s(fp, "ԭ��%s\nʱ�䣺%02d-%02d-%02d %02d:%02d:%02d\n\n",
		err,
		t_time.tm_year + 1900, t_time.tm_mon + 1, t_time.tm_mday,
		t_time.tm_hour, t_time.tm_min, t_time.tm_sec);

	fclose(fp);//�ر��ļ�
	printf("ԭ��%s\nʱ�䣺%02d-%02d-%02d %02d:%02d:%02d\n",
		err,
		t_time.tm_year + 1900, t_time.tm_mon + 1, t_time.tm_mday,
		t_time.tm_hour, t_time.tm_min, t_time.tm_sec);
}


//��ȡ���ָ������͵�ǰ������ɫ
void GetMousePos(char* const RGB)
{
	POINT p;
	HDC hdc = GetDC(NULL);//��ȡ�����DC��ͼ�豸������
	COLORREF rgb;

	if (RGB != NULL)
	{
		GetCursorPos(&p);//��ȡ���ָ������
		rgb = GetPixel(hdc, p.x, p.y);//��ȡRGBֵ

		sprintf_s(RGB, 20, "%d%d%d", GetRValue(rgb), GetGValue(rgb), GetBValue(rgb));
		return;
	}


	OpenDingDing();//�򿪶���
	Sleep(3000);//�ȴ���������
	HWND _hwnd = FindWindowA(NULL, "����");//��ȡ�������ھ��
	if (!_hwnd)
	{
		WriteLog("��ȡ�������ھ��ʧ�ܣ�");
		exit(EXIT_FAILURE);
	}
	//Ѱ�Ҷ�������
	for (int i = 1; i <= 10; i++)
	{
		if (GetProcess("DingTalk.exe") == true)
			break;

		Sleep(3000);//δ�ҵ����̾͵ȴ�2������

		if (i == 10)//�������20�ζ�δ�ҵ�
		{
			WriteLog("δ�ҵ���������");
			exit(EXIT_FAILURE);
		}
	}

	ShowWindow(_hwnd, 3);
	//�����������ƶ�����Ļ���Ͻǲ����ô��ڸ߶ȺͿ��
	MoveWindow(_hwnd, 0, 0, 930, 640, true);

	while (1)
	{
		GetCursorPos(&p);//��ȡ���ָ������
		rgb = GetPixel(hdc, p.x, p.y);//��ȡRGBֵ

		printf("  \r��ǰ���λ�ã�X:%ld Y:%ld�� RGBֵ��%d,%d,%d", p.x, p.y, GetRValue(rgb), GetGValue(rgb), GetBValue(rgb));
		Sleep(300);
	}
}


//�ж϶��������Ƿ����
bool GetProcess(const char* name)
{
	char pName[MAX_PATH];
	strcpy_s(pName, MAX_PATH, name);
	CharLowerBuffA(pName, MAX_PATH);//�������������ĸת��ΪСд						
	PROCESSENTRY32 currentProcess;	//��ſ��ս�����Ϣ��һ���ṹ��					
	currentProcess.dwSize = sizeof(currentProcess);	//��ʹ������ṹ֮ǰ�����������Ĵ�С	
	HANDLE _handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);//��ϵͳ�ڵ����н�����һ������

	if (_handle == INVALID_HANDLE_VALUE)
	{
		WriteLog("��ȡϵͳ����ʧ��");
		return false;
	}


	bool bMore = Process32First(_handle, &currentProcess);	//��ȡ��һ��������Ϣ	
	while (bMore)
	{
		CharLowerBuffA(currentProcess.szExeFile, MAX_PATH);//��������ת��ΪСд
		if (strcmp(currentProcess.szExeFile, pName) == 0)//�Ƚ��Ƿ���ڴ˽���
		{
			CloseHandle(_handle);//�رվ��
			return true;
		}
		bMore = Process32Next(_handle, &currentProcess);//������һ��
	}
	CloseHandle(_handle);	//�رվ��
	return false;
}


//���������
void Add(void)
{
	char tmp[MAX_PATH], path[MAX_PATH];
	HKEY hkey;

	GetModuleFileNameA(NULL, tmp, MAX_PATH);//��ȡ��ǰexe·��
	sprintf_s(path, MAX_PATH, "\"%s\"", tmp);

	//��ע��������� 
	if (RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_ALL_ACCESS, &hkey) == ERROR_SUCCESS)
	{
		if (RegSetValueExA(hkey, "�Զ����붤��ֱ����", 0, REG_SZ, (BYTE*)path, MAX_PATH) == ERROR_SUCCESS)//д��ע���
		{
			RegCloseKey(hkey);//�ر�ע���
			printf("��ӳɹ�������ɾ�����ļ�\n ");
			system("pause");
			exit(EXIT_SUCCESS);
		}
	}

	printf("���ʧ�ܣ�\n���Թ���ԱȨ�����б������\n�Ѵ������ӵ�ɱ�����������");
	system("pause");
	exit(EXIT_FAILURE);
}


//ɾ��������
void Del(void)
{
	HKEY hkey;

	//��ע��������� 
	if (RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_ALL_ACCESS, &hkey) == ERROR_SUCCESS)
	{
		if (RegQueryValueExA(hkey, "�Զ����붤��ֱ����", NULL, NULL, NULL, NULL) == ERROR_FILE_NOT_FOUND)//�ж��������Ƿ����
		{
			printf("���������\n");
			system("pause");
			exit(EXIT_FAILURE);
		}
		if (RegDeleteValueA(hkey, "�Զ����붤��ֱ����") == ERROR_SUCCESS)//ɾ����ֵ
		{
			RegCloseKey(hkey);//�ر�ע���
			printf("ɾ���ɹ���\n ");
			system("pause");
			exit(EXIT_SUCCESS);
		}
	}

	printf("ɾ��ʧ�ܣ�\n���Թ���ԱȨ�����б������\n�Ѵ������ӵ�ɱ�����������");
	system("pause");
	exit(EXIT_FAILURE);
}


//����RGB�ļ�ģ��
void Create(void)
{
	FILE* fp;

	if (fopen_s(&fp, RGBFilePath, "w") != 0)
	{
		perror("�����ļ�ʧ��\nԭ��");
		system("pause");
		exit(EXIT_FAILURE);
	}

	if (fwrite(RGB, sizeof(char), strlen(RGB), fp) < strlen(RGB))
	{
		perror("д���ļ�ʧ��\nԭ��");
		system("pause");
		exit(EXIT_FAILURE);
	}

	fclose(fp);
	printf("����ģ���ļ��ɹ�����������鿴\n");
	system("pause");
	exit(EXIT_SUCCESS);
}


//unsigned int __stdcall Start(PVOID p)
//{
//	system(runPath);//�򿪶���
//	return 0;
//}
