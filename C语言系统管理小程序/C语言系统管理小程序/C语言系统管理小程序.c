//�ļ�����ʱ�䣺2019��1��29��  ����޸�ʱ�䣺2023��4��5��

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <direct.h>//Ϊ_getcwd�ṩ����
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
// �ػ�
void shut_down(void);
void shutdown_hour(void);
void shutdown_custom_sec(void);
void shutdown_set_custom_task(void);
void shutdown_del_custom_task(void);
void excute_shutdown(int sec);
// ����
void reboot(void);
void reboot_hour(void);
void reboot_custom_sec(void);
void reboot_set_custom_task(void);
void reboot_del_custom_task(void);
void excute_reboot(int sec);
// ��ѯ
void lookup_current_task(void);
// ϵͳ����
int  system_function(void);
void create_file(void);
void create_file_custom_size(void);
void kill_process(void);
void open_readme(void);
// �޸�hosts
void hosts(void);
// ���������
void flush(void);
// ʵ�ù���
void function(void);
// ɾ���ļ�
void delfile(int mode);
// �ʵ�
int  autoshut(void);
void print_and_exit(const char* const str);
// ��ȡ����Ŀ¼
void open_startup_directory(void);
void print_config(void);
// ɾ��ĩβ�Ļ��з�
void del_last_n(char* str);

void print_msg(const char* str);

#define DEL_FILE_TEMP 1 // ɾ�����򴴽�����ʱ�ļ�
#define DEL_FILE_SELF 2 // ɾ�������Լ�
#define OPEN_HOSTS 1	 // ��hosts
#define RECOVERY_HOSTS 2 // �ָ�hosts

int main(void)
{
	while (true)
	{
		int num;
		// ����
		system("mode con:cols=57 lines=25 && color 0F && title C����ϵͳ����С���� ���Թ���ԱȨ�޴򿪱����");
		delfile(DEL_FILE_TEMP);// ɾ���ļ�

		printf("\n        ------��ӭʹ��C����ϵͳ����С����------\n");
		printf("\t\t      1���ػ�\n");
		printf("\t\t      2������\n");
		printf("\t\t      3������\n");
		printf("\t\t      4��˯��\n");
		printf("\t\t      5��ȡ�����ػ���������\n");
		printf("\t\t      6��ע��\n");
		printf("\t\t      7��ʵ�ù���\n");
		printf("\t\t      8���鿴��������\n");
		printf("\t\t      9��ϵͳ����\n");
		printf("\t\t     10������Դ��\n");
		printf("\t\t     11���鿴ʹ��˵��\n");
		printf("\t\t      0���˳�\n\n");
		printf("     ��������ţ�");
		while (scanf_s("%d", &num) != 1)//����scanf_s�ķ���ֵ�ж�����������Ƿ�Ϊ�ַ�
		{
			flush();//��ջ����������ַ�
			printf("\n����������������룺");
		}
		flush();
		switch (num)
		{
		case 0:
			return 0; //�˳�
		case 1:
			shut_down();
			break;//�ػ�
		case 2:
			reboot();
			break;//����
		case 3:
			system("shutdown -h");
			Sleep(2000);
			return 0;//����
		case 4:
		{
			system("cls && color 04");
			printf("\n\n\n\n\nע�⣺\n��ȷ���˼�����ѹر����߹��ܣ�����ᵱ���������߹���\n\n��������ر����߹��ܿ��ڡ�9��ϵͳ�����ҵ���\n");
			Sleep(2000);
			system("rundll32.exe powrprof.dll,SetSuspendState 0,1,0");//ִ��˯������ 
			return 0; //˯��
		}
		case 5:
			system("shutdown -a");
			break;    //ȡ���ػ�������
		case 6:
			system("shutdown -l");
			return 0; //ע��
		case 7:
			function();
			break;	//ʵ�ù���
		case 8:
			print_config();
			break;	//�鿴��������
		case 9:
			system_function();
			break;	//ϵͳ����
		case 10:
			system("start https://cdmxz.github.io");
			break;
		case 11: open_readme();
			break;
		default:
			print_msg("\n\n\t     �������!\n\n");
			break; //while�����ַ�����switch�������ִ���
		}//switch

	}//whileѭ��ʹ����ִ�к󷵻ؿ�ͷ
	return 0;
}

void open_readme(void)
{
	TCHAR path[MAX_PATH];
	GetCurrentDirectory(arr_count(path), path);//��ȡ��������Ŀ¼
	wcscat_s(path, arr_count(path), L"\\readme.txt");
	if (_waccess(path, 0))//�жϡ�readme.txt���ļ��Ƿ��ڱ�����Ŀ¼��
		system("start https://github.com/cdmxz/C-language-program/blob/master/C%E8%AF%AD%E8%A8%80%E7%B3%BB%E7%BB%9F%E7%AE%A1%E7%90%86%E5%B0%8F%E7%A8%8B%E5%BA%8F/readme.txt");//��������������վ
	else
		system("notepad readme.txt");//������ֱ�Ӵ�
}

// ϵͳ����
int system_function(void)
{
	while (true)
	{
		int num;
		system("cls && color 71 && mode con:cols=63 lines=28");
		system("title ϵͳ���� PS������7����");

		printf("ϵͳ����\n");
		printf(" 1����������        2���ر�����\n");
		printf(" 3���򿪿������    4���򿪲�����༭��  5���򿪱����û�����\n");
		printf(" 6�����豸������  8���鿴ϵͳ�汾      9����ע���༭��\n");
		printf("10����IP��ַ��� 11���򿪼��������   12����Directx��Ϲ���\n");
		printf("13�����������   14���򿪱��ط���     15�����DNS����\n\n");

		printf("���̹���\n");
		printf("16��������������   17���������̹���     18���������̼��\n\n");

		printf("ϵͳ����\n");
		printf("19��������Դ������ 20���������������   21���������ܼ�����\n");
		printf("22������Զ������   23���������±�       24��������ͼ��\n");
		printf("25������д�ְ�     26������Media Player 27������������ʾ��\n");
		printf("28������������     29�������Ŵ�       30������ľ�����󹤾�\n\n");

		printf("IP��ַ��\n");
		printf("31���鿴IP��ַ     32���ر�IPv6��ʱ��ַ 33������IPv6��ʱ��ַ\n\n");

		printf("������\n");
		printf("34���޸�hosts	   35�������������     36���򿪲ʵ�\n");
		printf("37���������ļ�     38������ָ����С�ļ� 39���鿴ϵͳ��Ϣ\n");
		printf("40��ɾ������       41��������Ŀ¼     42����������\n");
		printf("43�������Դ�����   44�������Դ�����\n\n");
		printf("��������ţ�");

		while (scanf_s("%d", &num) != 1)//�����ַ�����
		{
			flush();//��ջ����������ַ�
			printf("\n����������������룺\n");
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
			system("cls && ipconfig /all > ����IP��Ϣ.txt && ����IP��Ϣ.txt");
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
			system("systeminfo >> ϵͳ��Ϣ.txt && ϵͳ��Ϣ.txt");
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
			print_msg("\n\t��������Ч");
			break;
		case 44:
			system("sc config i8042prt start= disabled");
			print_msg("\n\t��������Ч");
			break;
		default:
			print_msg("\n\n\t�������!\n\n");
			break;
		}
	}//while

}

// �����ļ�
void create_file(void)
{
	char fileName[MAX_PATH], cmd[MAX_PATH], content[MAX_PATH];
	system("cls && color 07");
	printf("\n\n�������ļ�����");
	fgets(fileName, arr_count(fileName), stdin);
	del_last_n(fileName);
	printf("\n\n���������ݣ����Ҫ�������ļ���ֱ�ӻس�����");
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

// ����ָ����С�ļ�
void create_file_custom_size(void)
{
	char fileName[MAX_PATH], cmd[MAX_PATH];
	long long size;
	system("cls && color 07");
	printf("\n��ܰ��ʾ������ܿ����������Դ��̿ռ��С");
	printf("\n\n\n�������ļ�����");
	fgets(fileName, arr_count(fileName), stdin);
	del_last_n(fileName);//ȥ��fgets��ȡ�Ļ��з�
	printf("\n\n������Ҫ�������ļ���С����λ��MB����");
	scanf_s("%lld", &size);
	flush();
	sprintf_s(cmd, arr_count(cmd), "fsutil file createnew %s %lld", fileName, size * 1048576);
	printf("\n\n");
	system(cmd);
	printf("\n\n");
	system("pause");
}

// ɱ������
void kill_process(void)
{
	char name[SIZE];
	char cmd[SIZE] = { "taskkill /f /t /im " };
	system("cls");
	//system("cls && tasklist >> ������Ϣ.txt && ������Ϣ.txt");
	printf("������Ҫ�����Ľ������ƣ����磺1.exe����");
	fgets(name, arr_count(name), stdin);
	del_last_n(name);
	strcat_s(cmd, arr_count(cmd), name);
	system(cmd);
	system("pause");
}

// ɾ��ĩβ�Ļ��з�
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

// �޸�hosts
void hosts(void)
{
	int num;
	FILE* fpwrite;
	char path[] = "%windir%\\System32\\drivers\\etc\\hosts";
	char cmd[MAX_PATH];
	while (true)
	{
		system("cls && mode con:cols=60 lines=20 && title hosts�޸� ���Թ���ԱȨ�޴򿪴����");
		printf("\n��ܰ��ʾ��1.���Թ���ԱȨ�޴򿪴����������Ȩ�޲�����ʧ�ܡ�\n");
		printf("          2.����Ѿ�ʹ�ù���ԱȨ�޴򿪱��������Ա���ʾ��");
		printf("\n\n\n\t      1����ϵͳĬ�ϱ༭����hosts�ļ�\n\n");
		printf("\t      2����ԭĬ��hosts�ļ�\n\n");
		printf("\t      3��������һ����\n\n\n");
		printf(" ��������ţ�");
		while (scanf_s("%d", &num) != 1)
		{
			flush();//��ջ����������ַ�
			printf("\n����������������룺");
		}
		flush();
		// ȡ��ֻ������ 
		sprintf_s(cmd, arr_count(cmd), "attrib -R -S \"%s\"", path);
		system(cmd);
		if (num == OPEN_HOSTS)// ��hosts
		{
			sprintf_s(cmd, arr_count(cmd), "notepad \"%s\"", path);
			system(cmd);
		}
		else if (num == RECOVERY_HOSTS)// �ָ�hosts
		{
			if (fopen_s(&fpwrite, path, "w") != 0 || !fpwrite)//��hosts
			{
				print_and_exit("\n����hosts�ļ�ʧ�ܣ����Թ���ԱȨ�����б������\n\n");
				exit(EXIT_FAILURE);
			}
			if (fputs(HOSTS, fpwrite) == EOF)
			{
				print_and_exit("\n�ָ�Ĭ��hostsʧ�ܣ����Թ���ԱȨ�޴򿪱�������ԣ�\n\n");
			}
			printf("\n\t\t�ָ�Ĭ��hosts�ɹ���\n\n");
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
			print_msg("\n\t�������");
		}
	}
}

// ��ӡ���˳�����
void print_and_exit(const char* const str)
{
	printf("\n����hosts�ļ�ʧ�ܣ����Թ���ԱȨ�����б������\n\n");
	system("pause");
	exit(EXIT_FAILURE);
}

//�ػ� 
void shut_down(void)
{
	int min;
	system("mode con cols=55 lines=18 && color 06 && title �ػ� PS�����Թ���ԱȨ�����б����");
	printf("��ܰ��ʾ���ڵ�ǰҳ������7���ء�  �������ֺ��밴�س���\n");
	printf("          �����趨Сʱ������2��  XXX���ػ�������4��\n");
	printf("          �Զ���XXСʱ:XX���ӹػ�����������6��\n");
	printf("          ɾ���Զ���XXСʱ:XX���ӹػ�������8��\n");
	printf("          �鿴��ǰ����������9��\n\n\n\n\n");
	printf("�����루0��1��3��5��10��20��30��40��50��60���ֺ�ػ���\n\n���������֣�");
	while (scanf_s("%d", &min) != 1)
	{
		flush();//��ջ����������ַ�
		printf("\n����������������룺");
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
	default:print_msg("\n\n\t �������"); break;
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
	system("cls && schtasks /delete /tn �ػ� /F");
	printf("\n\n\n\n\n�����ʾ���ܾ����ʡ������������Թ���ԱȨ�����д������\n\n");
	system("pause");
}

void shutdown_set_custom_task(void)
{
	char schtasks[SIZE] = { "schtasks /create /tn \"�ػ�\" /tr \"shutdown /s /t 00\" /sc once /st " };
	char shut[SIZE] = { 0 };

	system("cls");
	printf("\n��ܰ��ʾ�����ùػ���ʱ���벻ҪС�ڻ���ڵ�ǰʱ�䡣\n\n\n\n");
	printf("��������ٵ�ػ��������磺12:00����");

	scanf_s("%s", shut, arr_count(shut));
	flush();
	strcat_s(schtasks, arr_count(shut), shut);//�ϲ�����
	system(schtasks);
	printf("\n\n�����ʾ���ܾ����ʡ������������Թ���ԱȨ�����д������\n\n");
	system("pause");
}

void shutdown_custom_sec(void)
{
	int sec;
	char cmd[SIZE] = { 0 };

	system("cls");
	printf("\n��ܰ��ʾ���������ֺ��밴�س������ʱ�䲻�ܳ���999999�롣\n\n\n\n\n");
	printf("��ܰ��ʾ��ֻ���������ּ��ɣ��������뵥λ\n");
	printf("������������ػ�����λ���룩��");

	while (scanf_s("%d", &sec) != 1)
	{
		printf("\n����������������룺");
		flush();//��ջ����������ַ�
	}
	flush();
	excute_shutdown(sec);
	Sleep(1000);
}

//�ػ�����λ��Сʱ��
void shutdown_hour(void)
{
	int num;
	double hour = 0.0;
	system("cls");
	printf("       ��ܰ��ʾ��1.����0����  2.������ź��밴�س�\n\n");
	printf("\t\t  1��1.5Сʱ��ػ�\n");
	printf("\t\t  2��2  Сʱ��ػ�\n");
	printf("\t\t  3��2.5Сʱ��ػ�\n");
	printf("\t\t  4��3  Сʱ��ػ�\n");
	printf("\t\t  5��3.5Сʱ��ػ�\n");
	printf("\t\t  6��4  Сʱ��ػ�\n");
	printf("\t\t  7��4.5Сʱ��ػ�\n");
	printf("\t\t  8��5  Сʱ��ػ�\n");
	printf("\t\t  9��5.5Сʱ��ػ�\n");
	printf("��������ţ�");
	while (scanf_s("%d", &num) != 1)//�����ַ�����
	{
		printf("\n����������������룺");
		flush();//��ջ����������ַ�
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
		print_msg("\n\n\t �������");
		return;
	}
	excute_shutdown((int)(hour * 3600));
}

//����
void reboot(void)
{
	int min;
	char cmd[SIZE];
	system("mode con cols=55 lines=18 && color 06 && cls && title ���� PS�����Թ���ԱȨ�����б����");
	printf("��ܰ��ʾ���ڵ�ǰҳ������7���ء�  �������ֺ��밴�س���\n");
	printf("          ����Сʱʱ��������2��  XXX�������������4��\n");
	printf("          �Զ���XXСʱ:XX������������������6��\n");
	printf("          ɾ���Զ���XXСʱ:XX��������������8��\n");
	printf("          �鿴��ǰ����������9��\n\n\n\n\n");
	printf("�����루0��1��3��5��10��20��30��40��50��60���ֺ�������\n\n���������֣�");

	while (scanf_s("%d", &min) != 1)
	{
		printf("\n����������������룺");
		flush();//��ջ����������ַ�
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
	default:print_msg("\n\n\t �������"); break;
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
	system("cls && schtasks > ��ǰ����.txt && ��ǰ����.txt");
	printf("\n\n\n�ļ��ѵ�������ǰ����Ŀ¼\n\n");
	system("pause");
}

void reboot_del_custom_task(void)
{
	system("cls && schtasks /delete /tn ���� /F");
	printf("\n\n\n\n\n�����ʾ���ܾ����ʡ����������Թ���ԱȨ�����д������\n\n");
	system("pause");
}

void reboot_set_custom_task(void)
{
	char schtasks[SIZE] = { "schtasks /create /tn \"����\" /tr \"shutdown /r /t 00\" /sc once /st /0" };
	char cmd[SIZE] = { '\0' };

	system("cls");
	printf("\n��ܰ��ʾ������������ʱ���벻ҪС�ڻ���ڵ�ǰʱ�䡣\n\n\n\n");
	printf("��������ٵ������������磺12:00����");

	scanf_s("%s", cmd, arr_count(cmd));
	flush();
	strcat_s(schtasks, arr_count(cmd), cmd);
	system(schtasks);
	printf("\n\n�����ʾ���ܾ����ʡ����������Թ���ԱȨ�����д������\n\n");
	system("pause");
}

void reboot_custom_sec(void)
{
	char cmd[SIZE];
	int sec;

	system("cls");
	printf("\n��ܰ��ʾ���������ֺ��밴�س������ʱ�䲻�ܳ���999999�롣\n\n\n\n\n");
	printf("��ܰ��ʾ��ֻ���������ּ��ɣ��������뵥λ\n");
	printf("��������������������λ���룩��");

	while (scanf_s("%d", &sec) != 1)
	{
		printf("\n����������������룺");
		flush();//��ջ����������ַ�
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
	printf("       ��ܰ��ʾ��1.����0����  2.������ź��밴�س�\n\n");
	printf("\t\t  1��1.5Сʱ������\n");
	printf("\t\t  2��2  Сʱ������\n");
	printf("\t\t  3��2.5Сʱ������\n");
	printf("\t\t  4��3  Сʱ������\n");
	printf("\t\t  5��3.5Сʱ������\n");
	printf("\t\t  6��4  Сʱ������\n");
	printf("\t\t  7��4.5Сʱ������\n");
	printf("\t\t  8��5  Сʱ������\n");
	printf("\t\t  9��5.5Сʱ������\n");
	printf("��������ţ�");
	while (scanf_s("%d", &num) != 1)//�����ַ�����
	{
		printf("\n����������������룺");
		flush();//��ջ����������ַ�
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
		print_msg("\n\n\t �������");
		return;
	}
	excute_reboot((int)(hour * 3600));
}

//���������
void flush(void)
{
	int ch;
	while ((ch = getchar()) != '\n' && ch != EOF)
		;
}

//ɾ���ļ�
void delfile(int mode)
{
	TCHAR cmd[MAX_PATH] = { 0 }, cmdpath[MAX_PATH], filepath[MAX_PATH];
	if (mode == DEL_FILE_TEMP)
	{
		remove("����IP��Ϣ.txt");
		remove("ϵͳ��Ϣ.txt");
		remove("��ȡӲ����Ϣ.bat");
		remove("������Ϣ.txt");
		remove("ʹ��˵��.txt");
		return;
	}
	// ɾ����������
	if ((GetModuleFileName(NULL, filepath, arr_count(filepath)) != 0)
		&& (GetEnvironmentVariable(L"COMSPEC", cmdpath, arr_count(cmdpath)) != 0))
	{
		swprintf_s(cmd, arr_count(cmd), L"\"%s\" /c del /q \"%s\" > 1.txt", cmdpath, filepath);// �������
		SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);				// ���������̵����ȼ�
		SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
		SHChangeNotify(SHCNE_DELETE, SHCNF_PATH, filepath, NULL);					// ֪ͨ��Դ�����,�������Ѿ���ɾ��
		ShellExecute(NULL, L"open", cmdpath, cmd, NULL, SW_HIDE);
		exit(EXIT_SUCCESS);
	}
	else
	{
		print_msg("ɾ��ʧ��!\n");
	}
}

// ������Ŀ¼
void open_startup_directory(void)
{
	TCHAR path[MAX_PATH], cmd[MAX_PATH] = { 0 };
	// ��ȡ�û��ļ���
	if (GetEnvironmentVariable(L"APPDATA", path, arr_count(path)) == 0)
	{
		// ��ȡʧ��
		print_msg("��ȡ�û��ļ���ʧ�ܣ�\n");
	}
	else
	{
		swprintf_s(cmd, arr_count(cmd), L"\"%s\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\"", path);
		ShellExecute(NULL, L"open", L"explorer.exe", cmd, NULL, SW_SHOWNORMAL);
	}
}

// ��ӡ��Ϣ
void print_msg(const char* str)
{
	printf("%s\n", str);
	Sleep(2000);
}