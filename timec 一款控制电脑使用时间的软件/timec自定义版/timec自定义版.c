#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <direct.h>
#include <io.h>

#define MAXPATH 1024
#define SIZE 6

void pass(void);     //��֤����
void shut(void);     //�Զ���ػ�
void hwn(void);      //���ش���
void countDown(void);//������ʾ��
int hyke(void);      //ע���
void flush(void);    //���������
//�����ļ�
int copyfile(char *fileread, char *filewrite);
void passwd(void);

int main(void)
{
	int time;

	while (1)
	{
		system("color 07 && title timec�Զ���� && mode con cols=50 lines=20");//������ɫ ���� ���ڴ�С
		
		printf("\t   ***��ӭʹ��timec�Զ����***\n");
		printf("\t\t 0��ȡ���ػ�\n");
		printf("\t\t 1��10���Ӻ�ػ�\n");
		printf("\t\t 2��30���Ӻ�ػ�\n");
		printf("\t\t 3��50���Ӻ�ػ�\n");
		printf("\t\t 4��60���Ӻ�ػ�\n");
		printf("\t\t 5��80���Ӻ�ػ�\n");
		printf("\t\t 6��100���Ӻ�ػ�\n");
		printf("\t\t 7���Զ���ػ�ʱ��\n");
		printf("\t\t 8���鿴ʹ��˵��\n");
		printf("\t\t 9�����������\n");
		printf("\t\t 10���˳�����\n\n");
		//printf("��ܰ��ʾ��������ź��밴�س���\n");
		printf("\n\t��������ţ�");

		while (scanf_s("%d", &time) != 1)//�����ַ�����
		{
			printf("\n������������ԣ�");
			flush();//��ջ����������ַ�
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



void pass(void)//������֤
{
	system("title ���������룬������1234�޸ġ� && cls");

	int i;
	FILE *fpwrite, *fpread;
	errno_t err;
	char pass[] = { "123456" };//����Ĭ������
	char newpass[7] = { 0 };//����������
	char tmpPass[7] = { 0 };//��ʱ��������
	char change[] = { "1234" };


	printf("       ��ܰ��ʾ�����������룬������1234�޸�\n\n\n\n\n\n");
	printf("��Ҫȡ���ػ������������룺");
	
	scanf_s("%s",tmpPass,7);//���뵱ǰ����
	flush();


	if (!strcmp(tmpPass, change))//�������������бȽ�
	{
		system("title Ĭ��������ڽ����ļ��в��� && cls");
		
		printf("       ��ܰ��ʾ��Ĭ��������ڽ����ļ��в���\n\n\n\n\n\n");
		printf("������Ĭ�����루6λ������");
		
		scanf_s("%s", &tmpPass, 7);//����Ĭ������
		flush();
		
		
		if (!strcmp(tmpPass, pass))//�������������Ĭ��������бȽ�
		{
			while (1)
			{
				printf("\n\n�����������루6λ������");
				
				scanf_s("%s", &newpass, 7);//����������
				flush();
				
				
				if (strlen(newpass) < SIZE)//�ж������볤��
				{
					printf("\n\n���������̫����̫�̣���Ҫ6λ���֣�");
					flush();//���������
				}
				else
					break;
			}
			
			if ((err = fopen_s(&fpwrite,"passwd.dat", "w")) != 0)//�򿪼�¼������ļ�,�����ж��ļ��Ƿ�������
			{
				MessageBox(NULL, TEXT("�����ļ���������\n���Թ���ԱȨ���������ԣ�"), TEXT("error!"), MB_OK | MB_ICONERROR);//������ʾ��
				exit(1);
			}
			
			for (i = 0; i <= 6;i++)  //��������д���ļ�
			{
				newpass[i] = newpass[i] - 't' + 'i' - 'm' / 'e';//�����ļ�����
				fputc(newpass[i], fpwrite);//������ܺ�����ݵ��ļ�
			}

			if (fclose(fpwrite) == EOF)//�ر��ļ�
			{
				MessageBox(NULL, TEXT("�ļ�д�����\n�����Ƿ�����������ռ�ã�\n�������ԣ�"), TEXT("error!"), MB_OK | MB_ICONERROR);
				exit(1);
			}
		}
		else
		{
			printf("\n\n\t\t�������");
			Sleep(1000);
		}

	}
	else//ȡ���ػ�
	{
		while (1)
		{
			if ((err = fopen_s(&fpread, "passwd.dat", "r")) != 0)//�򿪼�¼������ļ�,�����ж��ļ��Ƿ�������
			{
				MessageBox(NULL, TEXT("�����ļ���ȡ����\n�������Ŀ¼�Ƿ��С�passwd.dat���ļ������û�У����޸����롣"), TEXT("error!"), MB_OK | MB_ICONERROR);//������ʾ��
				break;
			}


			for (i = 0; i <= 6; i++)  //���ļ��ж�ȡ���ݵ�ch��EOF���ļ�������־��
			{
				newpass[i] = fgetc(fpread);//���ļ���ȡ����
				newpass[i] = newpass[i] + 't' - 'i' + 'm' * 'e';//�����ļ����ݲ��浽����
			}

			if (!strcmp(tmpPass, newpass))//������������������е�������бȽ�
				system("shutdown -a");
			else
			{
				printf("\n\n\t\t�������");
				Sleep(1000);
			}


			fclose(fpread); //�ر��ļ�
			
			break;
		}//while
	}//else
}



void hwn(void)//���ش���
{
	HWND hwnd;
	hwnd = FindWindow("ConsoleWindowClass", NULL);
	if (hwnd)
		ShowWindow(hwnd, SW_HIDE);
}


void countDown(void)
{
	MessageBox(NULL, TEXT("���ĵ��Ի���10���Ӿ�Ҫ�ػ���\n�����ùػ�׼����"), TEXT("��ܰ��ʾ"), MB_OK | MB_ICONWARNING);
}


//�Զ���ػ�
void shut(void)
{
	char shut[25] = { "shutdown -s -t " };
	char tmp[7] = { 0 };
	system("cls");
	printf("\nע�⣺Ŀǰ�Զ���ʱ��ػ����ܲ����ڹػ�ǰ������ʾ��,");
	printf("��ע��鿴ϵͳ��ʾ���������ʱ�䲻�ܳ���99999�롣\n\n\n\n\n");
	printf("��ܰ��ʾ��ֻ���������ּ��ɣ��������뵥λ\n");
	printf("\t  ������������ػ�����λ���룩��");
	
	scanf_s("%s", tmp ,7);
	flush();

	strcat_s(shut,sizeof(shut), tmp);//�ϲ�����
	
	system(shut);//ִ�йػ�����
	Sleep(2000);
	exit(0);
}


//���ע���
int hyke(void)
{
	system("cls");

	char *szSubKey = { "Software\\Microsoft\\Windows\\CurrentVersion\\Run" }; //ϵͳ������·��
	char *del = { "Software\\Microsoft\\Windows\\CurrentVersion\\Run\\timecyincang" };

	char folderName[] = { "C:\\Program Files\\timec" };//�����ļ���·��

	char copy_Y[] = { "\\timec�Զ����.exe" };
	char copy_Z[] = { "\\timec�Զ����.exe" };

	char buffer[MAXPATH];//���浱ǰ����·��
	
	char *timec_Y = { "C:\\Program Files\\timec���ذ�.exe" };  //ע���������·��
	char *timec_Z = { "C:\\Program Files\\timec�Զ����.exe" };//ע���������·��

	char path_Y[] = { "C:\\Program Files\\timec\\timec���ذ�.exe" };//timec���ذ� ���Ƶ�·��
	char path_Z[] = { "C:\\Program Files\\timec\\timec�Զ����.exe" };//timec�Զ���� ���Ƶ�·��

	_getcwd(buffer, MAXPATH);//��ȡ��ǰ����ľ���·��

	HKEY hKey;
	int i;


	printf("\n\t����3�鿴ʹ�ð���������4ɾ��������\n\n");
	printf("������Ҫ����������İ汾��\n\n");
	printf("\t��1.timec���ذ� 2.timec�Զ���棩\n\n\n\n");
	printf("    ��������ţ�");
	
	while (scanf_s("%d", &i) != 1)//�����ַ�����
	{
		printf("\n������������ԣ�");
		flush();
	}
	flush();


	switch (i)
	{
	case 1:
	{
		system("color 04 && cls");

		printf("\n\n\n\nע�⣺���⵽ɱ�����أ��������������������\n\n��������޷�����������\n\n\n");
		Sleep(3000);
		
		if (_access(folderName, 00) == -1)//�ж��ļ����Ƿ����
		if (_mkdir(folderName) == -1)			//�������򴴽��ļ���
		{
			MessageBox(NULL, TEXT("����Ŀ¼ʧ�ܣ����Թ���ԱȨ�����ԣ�"), TEXT("Error"), MB_OK | MB_ICONERROR);
			exit(1);
		}

		strcat_s(buffer, sizeof(buffer), copy_Y);//����ǰ��·��
		copyfile(buffer, path_Y);//�����ļ�

		//��ע��������� 
		if (RegOpenKeyEx(HKEY_CURRENT_USER, szSubKey, 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)
		{  //���һ����Key,������ֵ
			RegSetValueEx(hKey, "timecyincang", 0, REG_SZ, (BYTE *)timec_Y, strlen(timec_Y));
			RegCloseKey(hKey);//�ر�ע���
			
			printf("\t\t    ��ӳɹ���\n\n\t          ");
			system("pause");
			return 0;
		}
		else
		{
			MessageBox(NULL, TEXT("���ʧ�ܣ�"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
			return -1;
		}
		break;
	}

	case 2:
	{

		//�˴����ϴ�Դ���������ͬ���ʲ�дע�͡���ʵ��������~
		system("color 04 && cls");
		
		printf("\n\n\n\nע�⣺���⵽ɱ�����أ��������������������\n\n��������޷�����������\n\n\n");
		Sleep(3000);

		if (_access(folderName, 00) == -1)//�ж��ļ����Ƿ����
			if (_mkdir(folderName) == -1)			//�������򴴽��ļ���
			{
				MessageBox(NULL, TEXT("����Ŀ¼ʧ�ܣ����Թ���ԱȨ�����ԣ�"), TEXT("Error"), MB_OK | MB_ICONERROR);
				exit(1);
			}


		strcat_s(buffer, sizeof(buffer), copy_Z);//����ǰ��·��
		copyfile(buffer, path_Z);//�����ļ�

		if (RegOpenKeyEx(HKEY_CURRENT_USER, szSubKey, 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)
		{
			RegSetValueEx(hKey, "timeczidingyi", 0, REG_SZ, (BYTE *)timec_Z, strlen(timec_Z));
			RegCloseKey(hKey);
			
			printf("\t\t    ��ӳɹ���\n\n\t          ");
			system("pause");
			return 0;
		}
		else
		{
			MessageBox(NULL, TEXT("���ʧ�ܣ�"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
			return -1;
		}
		break;
	}

	case 3:
		MessageBox(NULL, TEXT("1�����ÿ����Զ�����ǰ���Թ���ԱȨ�����б����\n��ֹȨ�޲������ʧ�ܡ�\n2�����ÿ����Զ�����ǰ��ȷ����timec���ذ桱��\n��timec�Զ���桱��ͬһĿ¼��\n3�����ÿ����Զ�����ǰ��ȷ����timec���ذ桱��\n��timec�Զ���桱��ԭ�ļ���δ���޸ġ�\n4�������������ʧЧ����鿴��C:\\Program Files\\time��Ŀ¼����û��\n��timec�Զ���� ����timec���ذ桱������\n���߼���������Ȩ���Ƿ�ɱ������ˡ�\n5���ڵ�ǰҳ������4ɾ����������\n6��6��ʹ�ñ�����������κκ��������ԭ���߳е�\n��һ��ʹ�ñ��������Ϊͬ������\nע�������������κθ��£�ˡ������֪ͨ��"), TEXT("ʹ�ð���"), MB_OK);
		break;

	case 4://���ļ���ɾ���ļ�
	{
		passwd();//��֤����
		system("cls");
		printf("\n\n\n\nע�⣺���⵽ɱ�����أ��������������������\n\n��������޷�����������\n\n\n");
		Sleep(3000);
		system("cls");

		//ɾ���ļ�
		if (remove("C:\\Program Files\\time\\timec���ذ�.exe"))
			printf("\n\nɾ��timec���ذ�ʧ�ܣ�\n\n��δ���timec���ذ��������ɲ���ᡣ\n\n");
		else
			printf("\n\nɾ��timec���ذ�ɹ���\n\n\n");

		if (remove("C:\\Program Files\\timec\\timec�Զ����.exe"))
			printf("\n\nɾ��timec�Զ����ʧ�ܣ�\n��δ���timec�Զ�����������ɲ���ᡣ\n\n");
		else
			printf("\n\nɾ��timec�Զ����ɹ���\n\n\n");

		system("rd /s /q C:\\\"Program Files\"\\timec");//ɾ��Ŀ¼
		system("pause");
		
		//ɾ��ע���
		system("reg delete \"HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run\"  /v timecyincang /f");
		system("reg delete \"HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run\"  /v timeczidingyi /f");
		
		break;
	}
	default:printf("\n\t\t �������"); Sleep(1000); break;
	}
	return 0;
}


//���������
void flush(void)
{
	int ch;
	while ((ch = getchar()) != '\n'&&ch != EOF)
		;
}


//�����ļ�
int copyfile(char *fileread, char *filewrite)
{
	FILE *fpread;   //ָ����ǰ���ļ�·��
	FILE *fpwrite;  //ָ���ƺ���ļ�·��
	errno_t err;

	int   bufflen = 1024 * 4;  //����������
	char* buffer = (char*)malloc(bufflen); //���ٻ���
	int   readbyte;  //��ȡ���ֽ���
	

	if ((err = fopen_s(&fpread, fileread, "rb")) != 0 )//�Զ����Ʒ�ʽ���ļ�
	{
		printf("�޷����ļ���\n");
		system("pause");
		return 1;
	}
	if ((err = fopen_s(&fpwrite, filewrite, "wb")) != 0)
	{
		printf("�޷������ļ������Թ���ԱȨ�����ԣ�");
		system("pause");
		return 1;
	}

	//���ϴ�fileread��ȡ���ݣ����ڻ�����
	while ((readbyte = fread(buffer, 1, bufflen, fpread)) > 0)
		fwrite(buffer, readbyte, 1, fpwrite);	//��������������д��fileWrite
	

	free(buffer);//�ͷŸ�ָ�����������ڴ�ռ�
	fclose(fpread);//�ر��ļ�
	fclose(fpwrite);
	return 0;
}


void passwd(void)//ɾ����������֤����
{
	FILE *fpread;
	errno_t err;

	int i;
	char newPass[7] = { 0 };//����������
	char tmpPass[7] = { 0 };//��ʱ��������

	while (1)
	{
		system("cls && title ɾ������������Ҫ������ȷ������");

		printf("\n\t\t  ����1234�˳���\n         ���������룬��������������0���ġ�");
		printf("\n\n\nɾ������������Ҫ�������롣\n\n��������ȷ�����룺");
		scanf_s("%s", tmpPass, 7);
		flush();

		if (!strcmp(tmpPass, "1234"))//����1234�˳�
			exit(0);

		if ((err = fopen_s(&fpread, "passwd.dat", "r")) != 0)//�򿪼�¼������ļ�,�����ж��ļ��Ƿ�������
		{
			MessageBox(NULL, TEXT("�����ļ���ȡ����\n�������Ŀ¼�Ƿ��С�passwd.dat���ļ������û�У����޸����롣"), TEXT("error!"), MB_OK | MB_ICONERROR);//������ʾ��
			break;
		}

		for (i = 0; i <= 6; i++)  //���ļ��ж�ȡ���ݵ�ch��EOF���ļ�������־��
		{
			newPass[i] = fgetc(fpread);//���ļ���ȡ����
			newPass[i] = newPass[i] + 't' - 'i' + 'm' * 'e';//�����ļ����ݲ��浽����
		}

		if (!strcmp(tmpPass, newPass))//������������������е�������бȽ�
			break;
		else
		{
			printf("\n\n\t\t�������");
			Sleep(1000);
		}

		fclose(fpread); //�ر��ļ�
		system("cls");
	}

}
