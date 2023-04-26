#include <stdio.h>
#include <direct.h>
#include <string.h>
#include <stdlib.h>
#include <io.h>
#include <windows.h>
#include <mmsystem.h>
#include <conio.h>
#include "common.h"
#include "str_util.h"
#pragma comment(lib,"winmm.lib")

#define IS_MP3_FILE 0
#define IS_FOLDER_OR_LIST 1

int OpenMusic(const TCHAR* musicName, const char* defaultErr);
//�ж����ļ�����Ŀ¼
int Check_extension(TCHAR* T_filename, TCHAR* T_musiclist);
//�������б����������ļ���
int OpenMusicList(const TCHAR* MusicList, TCHAR* MusicName, int* line, int mode);
//��ȡ����̨���λ��
void Refresh(COORD* pos);

//����MP3
int play_mp3(void)
{
	system("cls && mode con cols=110 lines=10 && title ����MP3");
	TCHAR T_filename[MAX_PATH];//�ļ���
	TCHAR T_open[MAX_PATH] = { 0 };    //���ļ�
	TCHAR T_temp[MAX_PATH] = { 0 };
	TCHAR T_musiclist[MAX_PATH];//�����б�·��

	char mode = 'u';
	char C_fileName[MAX_PATH];
	long Lcurrent, Llength, Lvolume = 600;
	int  n, ch, j = 0, i = 0, line = 0, numberOfCycles = 0;
	COORD pos1, pos2;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);// ��ȡ������


	printf("�������ļ����ļ��л��ļ��б�·���� ");
	fgets(C_fileName, MAX_PATH, stdin); // ������������
	rewind(stdin);					// ���������

	char_to_wchar(C_fileName, T_filename, MAX_PATH);
	del_last_newline_w(T_filename);			// ���fgets��ȡ��'\n'
	if (T_filename[0] == 34)		// ֧���ļ���ק����
	{
		T_filename[wcslen(T_filename) - 1] = 0;
		wcscpy_s(T_filename, wcslen(T_filename) + 1, T_filename + 1);// �����ק����������˫����
	}

	GetCurrentDirectory(MAX_PATH, T_musiclist);     // ��ȡ��ǰ�������ڵ�Ŀ¼   
	wcscat_s(T_musiclist, MAX_PATH, L"\\music.txt");// ��ȡ�������б��ļ�·��


	n = Check_extension(T_filename, T_musiclist);// �ж��Ƿ�ΪĿ¼���б�.MP3�ļ�

	while (1)
	{
		if (mode == 'u')
		{
			mciSendString(TEXT("close music"), 0, 0, 0);//�رղ���

			if (n != IS_FOLDER_OR_LIST)
			{
				if (OpenMusic(T_filename, NULL) == 1)// ���ļ�
				{
					system("pause");
					return 1;
				}
				mciSendString(T_open, 0, 0, 0);// ���´򿪵�ǰ�ļ�
			}
			else
			{
				j = 1;
				while (1)
				{
					if (j >= 3)
						return -1;
					j++;

					if (OpenMusicList(T_musiclist, T_filename, &line, 1))
					{
						printf("\n�ѵ������һ�׸裡");
						Sleep(1000);
						line = 0; // ��ͷ����
						continue;
					}

					if (OpenMusic(T_filename, "\n���ڴ���һ��...") == 1)// ���ļ�
					{
						Sleep(1000);
						continue;
					}
					break;
				}
			}
		}
		else if (mode == 't')
		{
			mciSendString(TEXT("close music"), 0, 0, 0);//�رղ���
			mciSendString(T_open, 0, 0, 0);//���´򿪵�ǰ�ļ�
			mciSendString(TEXT("play music"), 0, 0, 0);
		}
		else if (mode == 'y')
			;


		//��ȡ�����ļ�����
		mciSendString(TEXT("status music length"), T_temp, MAX_PATH, 0);
		Llength = wcstol(T_temp, NULL, 10);//ʹ�ÿ��ַ��汾
		i = Lcurrent = 0;//���õ�ǰ���ŵ�λ�� 

		wsprintf(T_temp, L"setaudio music volume to %d", Lvolume);
		mciSendString(T_temp, 0, 0, 0);//����������С
		//mciSendString(TEXT("setaudio music volume to 600"), 0, 0, 0);//�趨Ĭ������Ϊ600   
		mciSendString(TEXT("play music"), 0, 0, 0);//����

		system("mode con cols=120 lines=10");
		system("cls");
		printf("\n����:\n");
		printf("1.��ͣ����   2.��������   4.���Ż���   5.���ſ��  \n");
		printf("6.��������   7.������С   8.ֹͣ����  ->.����΢��� <-.����΢����\n");
		printf("+.����΢���� -.����΢��С 9.������һ�� 0.������һ��  R.ѭ������  \n");
		printf("J.����       K.��������   T.����ѭ��   U.���򲥷�    E.�˳�\n\n");

		for (i = (Lcurrent / 1000), numberOfCycles = 1; i <= (Llength / 1000); numberOfCycles++)
		{
			wchar_to_char(T_filename, C_fileName, arr_count(C_fileName));
			printf("���ڲ��ţ�%s", C_fileName);

			if (numberOfCycles == 10)
			{
				i++;
				numberOfCycles = 1;
			}
			Refresh(&pos1);
			printf("\n%02d:%02d/%02d:%02d    ", i / 60, i % 60, Llength / 1000 / 60, Llength / 1000 % 60);
			Refresh(&pos2);

			memset(T_temp, 0, MAX_PATH);

			if (_kbhit())
			{
				if ((ch = _getch()) == 0xE0) //�жϸ�λ�Ƿ����
					ch = _getch();//��λ���˵���Ƿ���������ٶ�ȡһ��


				if (ch == '4' || ch == 75)//��������
				{
					mciSendString(TEXT("status music position"), T_temp, MAX_PATH, 0);//��ȡ��ǰ���ŵ�λ��
					Lcurrent = wcstol(T_temp, NULL, 10);

					if (ch == '4')
						Lcurrent -= 10000;
					else
						Lcurrent -= 1000;

					if (Lcurrent < 0)//�ڵ�ǰ��ֵ�Ļ����ϼ�10000��������С��0����˵���ʼ״̬
						Lcurrent = 0;

					i = Lcurrent / 1000;

					wsprintf(T_temp, L"seek music to %ld", Lcurrent);
					mciSendString(T_temp, 0, 0, 0);//�޸Ĳ��ŵ�λ��
					mciSendString(TEXT("play music"), 0, 0, 0);
					printf("����...");
				}
				else if (ch == '5' || ch == 77)//������Ҽ�
				{
					mciSendString(TEXT("status music position"), T_temp, MAX_PATH, 0);//��ȡ��ǰ���ŵ�λ��
					Lcurrent = wcstol(T_temp, NULL, 10);

					if (ch == '5')
						Lcurrent += 10000;
					else
						Lcurrent += 1000;

					i = Lcurrent / 1000;
					wsprintf(T_temp, L"seek music to %ld", Lcurrent);//�ڵ�ǰ��ֵ�Ļ����ϼ�10000
					mciSendString(T_temp, 0, 0, 0);//�޸Ĳ��ŵ�λ��
					mciSendString(TEXT("play music"), 0, 0, 0);
					printf("���...");
				}
				else if (ch == '6' || ch == '+')//��������
				{
					if (ch == '6')
						Lvolume += 100;//ÿ������100
					else
						Lvolume += 10;//ÿ������10

					if (Lvolume > 1000)
					{
						printf("�ѵ������������");
						Lvolume = 1000;
						wsprintf(T_temp, L"setaudio music volume to %d", Lvolume);
						mciSendString(T_temp, 0, 0, 0);//����������С
						continue;
					}

					wsprintf(T_temp, L"setaudio music volume to %d", Lvolume);
					mciSendString(T_temp, 0, 0, 0);//����������С
					printf("��ǰ����Ϊ��%.1lf", (double)Lvolume / 100);
				}
				else if (ch == '7' || ch == '-')//��С����
				{
					if (ch == '7')
						Lvolume -= 100;//ÿ�μ�С100
					else
						Lvolume -= 10;//ÿ�μ�С10

					if (Lvolume < 0)
					{
						printf("�ѵ�����С������");
						Lvolume = 0;
					}

					wsprintf(T_temp, L"setaudio music volume to %d", Lvolume);
					mciSendString(T_temp, 0, 0, 0);//����������С
					printf("��ǰ����Ϊ��%.1lf", (double)Lvolume / 100);
				}


				switch (ch)
				{
				case '1'://��ͣ����
				{
					mciSendString(TEXT("pause music"), 0, 0, 0);
					printf("��ͣ...");
					while (1)//����ѭ����������
					{
						if ((ch = _getch()) == '2')
						{
							mciSendString(TEXT("play music"), 0, 0, 0);
							break;
						}
					}

					break;
				}
				case '2'://��������
				{
					mciSendString(TEXT("play music"), 0, 0, 0);
					printf("��������...");
					break;
				}
				case '8'://ֹͣ
				{
					mciSendString(TEXT("close music"), 0, 0, 0);
					break;
				}
				case '9'://������һ��
				{
					mciSendString(TEXT("close music"), 0, 0, 0);//�رղ���

					while (n == IS_FOLDER_OR_LIST)// n==1 �Ż�ִ��
					{
						if (OpenMusicList(T_musiclist, T_filename, &line, 1) == IS_FOLDER_OR_LIST)
						{
							printf("\n�ѵ������һ�׸裡");
							line = 0;
							continue;
						}

						if (OpenMusic(T_filename, "\n���ڴ���һ��...") == 1)// ���ļ�
						{
							Sleep(1000);
							continue;
						}
						else
							break;
					}

					if (n != IS_FOLDER_OR_LIST)
					{
						printf("\nֻ��һ�׸裡");
						mciSendString(T_open, 0, 0, 0);//���´򿪵�ǰ�ļ�
					}

					i = 0;//���ò��Ž���
					mciSendString(T_temp, 0, 0, 0);//����������С
					mciSendString(TEXT("play music"), 0, 0, 0);
					break;
				}
				case '0'://������һ��
				{
					if (line <= 0)
					{
						printf("\n�ѵ����һ�׸裡");
						i = line = 0;
						system("cls");
						continue;
					}

					mciSendString(TEXT("close music"), 0, 0, 0);//�رղ���

					while (n == IS_FOLDER_OR_LIST)// n==1 �Ż�ִ��
					{
						OpenMusicList(T_musiclist, T_filename, &line, 2);

						if (OpenMusic(T_filename, "\n���ڴ���һ��...") == 1)// ���ļ�
						{
							Sleep(1000);
							continue;
						}
						else
							break;
					}
					if (n != IS_FOLDER_OR_LIST)
					{
						printf("\nֻ��һ�׸裡");
						mciSendString(T_open, 0, 0, 0);
					}

					mciSendString(T_temp, 0, 0, 0);//����������С
					mciSendString(TEXT("play music"), 0, 0, 0);
					i = 0;//���ò��Ž���

					break;
				}
				case 'r':
				{
					mode = 'u';
					printf("ѭ������...");
					break;
				}
				case 't'://����ѭ��
				{
					mode = 't';
					printf("����ѭ��...");
					break;
				}
				case 'j'://�ر�����
				{
					mciSendString(TEXT("Set music audio all off"), 0, 0, 0);
					printf("����...");
					break;
				}
				case 'k'://��������
				{
					mciSendString(TEXT("Set music audio all on"), 0, 0, 0);
					printf("��������...");
					break;
				}
				case 'e'://�˳�
				{
					mciSendString(TEXT("close music"), 0, 0, 0);//�رղ���
					printf("�˳�...");
					exit(EXIT_SUCCESS);
				}
				break;
				}//switch()

			}//if()

			Sleep(100);

			//����ˢ�¿���̨
			SetConsoleCursorPosition(hConsole, pos2);//���ù��λ��
			printf("\r                                                                         \r");
			SetConsoleCursorPosition(hConsole, pos1);//���ù��λ��
			printf("\r                                                                         \r");

			mciSendString(TEXT("status music length"), T_temp, MAX_PATH, 0);//��ȡ�����ļ�����
			Llength = wcstol(T_temp, NULL, 10);//ʹ�ÿ��ַ��汾
		}
	}
	system("pause");
	return 0;
}


int OpenMusic(const TCHAR* musicName, const char* defaultErr)
{
	TCHAR T_open[MAX_PATH]; // ���ļ�
	char name[MAX_PATH];

	wsprintf(T_open, L"open \"%s\" alias music", musicName);// ���ļ�

	if (mciSendString(T_open, 0, 0, 0) != 0) // ���ļ�
	{
		wchar_to_char(musicName, name, arr_count(name));
		printf("\n��%s����", name);

		perror("ԭ��");
		if (defaultErr != NULL)// �Ƿ��ӡָ���Ĵ�������
			printf("%s", defaultErr);
		return 1;
	}
	return 0;
}

// �ж����ļ�����Ŀ¼
int Check_extension(TCHAR* T_filename, TCHAR* T_musiclist)
{
	DWORD dwAttr;
	wchar_t cmd[MAX_PATH];
	wchar_t extension[5];

	_wsplitpath_s(T_filename, NULL, 0, NULL, 0, NULL, 0, extension, 5);// �ָ��ļ������õ��ļ���׺��

	if ((_waccess(T_filename, 0)) == -1)
	{
		printf("\n�ļ���Ŀ¼�����ڣ�\n");
		system("pause");
		exit(EXIT_FAILURE);
	}

	if (wcscmp(extension, L".mp3") != 0 && wcscmp(extension, L".MP3") != 0)// �жϲ��Ǻ�׺��Ϊ.mp3���ļ�
	{
		if (wcscmp(extension, L".txt") == 0 || wcscmp(extension, L".TXT") == 0)// �жϺ�׺���Ƿ�Ϊ.txt�������б�
		{
			return IS_FOLDER_OR_LIST;// ��Ŀ¼���б���1
		}
		else if ((dwAttr = GetFileAttributes(T_filename)) == INVALID_FILE_ATTRIBUTES)// �����ж��Ƿ�ΪĿ¼
		{
			printf("\n��ȡ�ļ���Ϣʧ�ܣ�\n");
			system("pause");
			exit(EXIT_FAILURE);
		}

		if (dwAttr & FILE_ATTRIBUTE_DIRECTORY)//�ж��Ƿ�ΪĿ¼
		{
			_wremove(T_musiclist);// ɾ��ԭ�����ļ��б�
			swprintf_s(cmd, MAX_PATH, L"dir /a-d /s /b /on \"%s\\*.mp3\" >> \"%s\"", T_filename, T_musiclist);
			_wsystem(cmd);	      // cmd���Ŀ¼�е�MP3�ļ����������ļ�
			return IS_FOLDER_OR_LIST;			  // ��Ŀ¼���б���1
		}
		else//�������Ŀ¼���б���׺��Ϊ,mp3���ļ�
		{
			printf("\n������Ŀ¼���б�ͺ�׺��Ϊ.mp3���ļ���\n");
			system("pause");
			exit(EXIT_FAILURE);
		}
	}
	return IS_MP3_FILE;// ��MP3�ļ�����0
}


//�������б����������ļ���
int OpenMusicList(const TCHAR* MusicList, TCHAR* MusicName, int* line, int mode)
{
	FILE* fp;
	char C_FileName[MAX_PATH] = { 1 };
	int i = 0;

	memset(MusicName, 0, MAX_PATH);
	if (_wfopen_s(&fp, MusicList, L"r") != 0)// ��Ŀ¼�򴴽������б���
	{
		perror("\n�������б�ʧ��");
		system("pause");
		exit(EXIT_FAILURE);
	}

	if (fp == NULL)
		return 1;

	if (mode == 2)// ���򲥷�
		*line -= 1;

	while (C_FileName[0] != '\0' && i <= *line)// ���ļ��ж�ȡ��line�е��ļ���
	{
		memset(C_FileName, 0, MAX_PATH);// ��������
		fgets(C_FileName, MAX_PATH, fp);// �������б��ж�ȡ�ļ���
		del_last_newline(C_FileName);
		i++;// ͳ�ƶ�ȡ�˶����׸�	
	}

	fclose(fp);

	if (C_FileName[0] == '\0')// ���FileName[0] = '\0'����˵���Ѷ����ļ�ĩβ
		return 1;
	if (mode == 1)// ˳�򲥷�
		*line += 1;

	char_to_wchar(C_FileName, MusicName, MAX_PATH);// �����ֽ�תΪ���ֽ�
	return 0;
}

void Refresh(COORD* pos)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);//��ȡ������
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	if (GetConsoleScreenBufferInfo(hConsole, &csbi))//��ȡ���λ��
	{
		(*pos).X = csbi.dwCursorPosition.X;
		(*pos).Y = csbi.dwCursorPosition.Y;
	}
}