#include <stdio.h>
#include <direct.h>
#include <string.h>
#include <wchar.h>
#include <stdlib.h>
#include <io.h>
#include <windows.h>
#include <mmsystem.h>
#include <conio.h>
#include <assert.h>
#include <stdbool.h>
#include "common.h"
#include "str_util.h"
#pragma comment(lib,"winmm.lib")

#define IS_MP3_FILE 1
#define IS_FOLDER 2
#define IS_LIST 2
#define IS_FOLDER_OR_LIST 2
#define IS_ERR 0

int OpenMusic(const TCHAR* musicName, const char* defaultErr);
//判断是文件还是目录
int CheckExtension(TCHAR* T_filename);
//打开音乐列表，查找音乐文件名
int OpenMusicList(const TCHAR* MusicList, TCHAR* MusicName, int* line, int mode);
//获取控制台光标位置
void Refresh(COORD* pos);
// 将文件夹里的mp3文件导出到txt文件
void ExportMusicList(TCHAR* path, TCHAR* T_musiclist);

void ReadPath(TCHAR* T_filename, int len)
{
	char C_fileName[MAX_PATH];
	assert((T_filename, "T_filename为空指针"));
	printf("请输入文件或文件夹或文件列表路径： ");
	fgets(C_fileName, MAX_PATH, stdin); // 输入音乐名称
	rewind(stdin);					// 清除缓冲区
	char_to_wchar(C_fileName, T_filename, len);
	del_last_newline_w(T_filename);			// 清除fgets读取的'\n'
	if (T_filename[0] == 34)		// 支持文件拖拽输入
	{
		T_filename[wcslen(T_filename) - 1] = 0;
		wcscpy_s(T_filename, wcslen(T_filename) + 1, T_filename + 1);// 清除拖拽输入多出来的双引号
	}
}

void showTips()
{
	system("cls");
	printf("\n按键:\n");
	printf("1.暂停播放   2.继续播放   4.播放回退   5.播放快进  \n");
	printf("6.音量增大   7.音量减小   8.停止播放  ->.播放微快进 <-.播放微回退\n");
	printf("+.音量微增大 -.音量微减小 9.播放下一首 0.播放上一首  R.循环播放  \n");
	printf("J.静音       K.开启声音   T.单曲循环   U.逆序播放    E.退出\n\n");
}

//播放MP3
int play_mp3(void)
{
	system("cls && mode con cols=110 lines=10 && title 播放MP3");
	//system("chcp 65001");
	TCHAR T_filename[MAX_PATH];//文件名
	TCHAR T_open[MAX_PATH] = { 0 };    //打开文件
	TCHAR T_temp[MAX_PATH] = { 0 };
	TCHAR T_musiclist[MAX_PATH];//音乐列表路径
	char C_fileName[MAX_PATH];

	char mode = 'u';

	long Lcurrent, Llength, Lvolume = 600;
	int  n, ch, j = 0, i = 0, line = 0, numberOfCycles = 0;
	COORD pos1, pos2;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);// 获取输出句柄

	// 输入音乐名称
	ReadPath(T_filename, MAX_PATH);
	GetCurrentDirectory(MAX_PATH, T_musiclist);     // 获取当前程序所在的目录   
	wcscat_s(T_musiclist, MAX_PATH, L"\\music.txt");// 获取的音乐列表文件路径
	// 判断是否为目录、列表、.MP3文件
	n = CheckExtension(T_filename);
	if (n == IS_FOLDER_OR_LIST)
		ExportMusicList(T_filename, T_musiclist);

	while (1)
	{
		if (mode == 'u')
		{
			mciSendString(TEXT("close music"), 0, 0, 0);//关闭播放

			if (n != IS_FOLDER_OR_LIST)
			{
				if (OpenMusic(T_filename, NULL) == 1)// 打开文件
				{
					system("pause");
					return 1;
				}
				mciSendString(T_open, 0, 0, 0);// 重新打开当前文件
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
						printf("\n已到达最后一首歌！ ");
						Sleep(1000);
						line = 0; // 从头播放
						continue;
					}

					if (OpenMusic(T_filename, "\n正在打开下一首...") == 1)// 打开文件
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
			mciSendString(TEXT("close music"), 0, 0, 0);//关闭播放
			mciSendString(T_open, 0, 0, 0);//重新打开当前文件
			mciSendString(TEXT("play music"), 0, 0, 0);
		}
		else if (mode == 'y')
			;


		//获取音乐文件长度
		mciSendString(TEXT("status music length"), T_temp, MAX_PATH, 0);
		Llength = wcstol(T_temp, NULL, 10);//使用宽字符版本
		i = Lcurrent = 0;//重置当前播放的位置 

		wsprintf(T_temp, L"setaudio music volume to %d", Lvolume);
		mciSendString(T_temp, 0, 0, 0);//调节音量大小
		//mciSendString(TEXT("setaudio music volume to 600"), 0, 0, 0);//设定默认音量为600   
		mciSendString(TEXT("play music"), 0, 0, 0);//播放

		system("mode con cols=120 lines=10");
		showTips();

		for (i = (Lcurrent / 1000), numberOfCycles = 1; i <= (Llength / 1000); numberOfCycles++)
		{
			showTips();
			wchar_to_char(T_filename, C_fileName, arr_count(C_fileName));
			printf("正在播放：%s", C_fileName);

			if (numberOfCycles == 10)
			{
				i++;
				numberOfCycles = 1;
			}
			Refresh(&pos1);
			printf("\n%02d:%02d/%02ld:%02ld    ", i / 60, i % 60, Llength / 1000 / 60, Llength / 1000 % 60);
			Refresh(&pos2);

			memset(T_temp, 0, MAX_PATH);

			if (_kbhit())
			{
				if ((ch = _getch()) == 0xE0) //判断高位是否相等
					ch = _getch();//高位相等说明是方向键，就再读取一次


				if (ch == '4' || ch == 75)//方向键左键
				{
					mciSendString(TEXT("status music position"), T_temp, MAX_PATH, 0);//获取当前播放的位置
					Lcurrent = wcstol(T_temp, NULL, 10);

					if (ch == '4')
						Lcurrent -= 10000;
					else
						Lcurrent -= 1000;

					if (Lcurrent < 0)//在当前数值的基础上减10000，如果结果小于0则快退到开始状态
						Lcurrent = 0;

					i = Lcurrent / 1000;

					wsprintf(T_temp, L"seek music to %ld", Lcurrent);
					mciSendString(T_temp, 0, 0, 0);//修改播放的位置
					mciSendString(TEXT("play music"), 0, 0, 0);
					printf("快退...");
				}
				else if (ch == '5' || ch == 77)//方向键右键
				{
					mciSendString(TEXT("status music position"), T_temp, MAX_PATH, 0);//获取当前播放的位置
					Lcurrent = wcstol(T_temp, NULL, 10);

					if (ch == '5')
						Lcurrent += 10000;
					else
						Lcurrent += 1000;

					i = Lcurrent / 1000;
					wsprintf(T_temp, L"seek music to %ld", Lcurrent);//在当前数值的基础上加10000
					mciSendString(T_temp, 0, 0, 0);//修改播放的位置
					mciSendString(TEXT("play music"), 0, 0, 0);
					printf("快进...");
				}
				else if (ch == '6' || ch == '+')//增加音量
				{
					if (ch == '6')
						Lvolume += 100;//每次增加100
					else
						Lvolume += 10;//每次增加10

					if (Lvolume > 1000)
					{
						printf("已到达最大音量！");
						Lvolume = 1000;
						wsprintf(T_temp, L"setaudio music volume to %d", Lvolume);
						mciSendString(T_temp, 0, 0, 0);//调节音量大小
						continue;
					}

					wsprintf(T_temp, L"setaudio music volume to %d", Lvolume);
					mciSendString(T_temp, 0, 0, 0);//调节音量大小
					printf("当前音量为：%.1lf", (double)Lvolume / 100);
				}
				else if (ch == '7' || ch == '-')//减小音量
				{
					if (ch == '7')
						Lvolume -= 100;//每次减小100
					else
						Lvolume -= 10;//每次减小10

					if (Lvolume < 0)
					{
						printf("已到达最小音量！");
						Lvolume = 0;
					}

					wsprintf(T_temp, L"setaudio music volume to %d", Lvolume);
					mciSendString(T_temp, 0, 0, 0);//调节音量大小
					printf("当前音量为：%.1lf", (double)Lvolume / 100);
				}


				switch (ch)
				{
				case '1'://暂停播放
				{
					mciSendString(TEXT("pause music"), 0, 0, 0);
					printf("暂停...");
					while (1)//不断循环接收输入
					{
						if ((ch = _getch()) == '2')
						{
							mciSendString(TEXT("play music"), 0, 0, 0);
							break;
						}
					}

					break;
				}
				case '2'://继续播放
				{
					mciSendString(TEXT("play music"), 0, 0, 0);
					printf("继续播放...");
					break;
				}
				case '8'://停止
				{
					mciSendString(TEXT("close music"), 0, 0, 0);
					break;
				}
				case '9'://播放下一首
				{
					mciSendString(TEXT("close music"), 0, 0, 0);//关闭播放

					while (n == IS_FOLDER_OR_LIST)
					{
						if (OpenMusicList(T_musiclist, T_filename, &line, 1) == IS_FOLDER_OR_LIST)
						{
							printf("已到达最后一首歌！ ");
							line = 0;
							continue;
						}

						if (OpenMusic(T_filename, "\n正在打开下一首...") == 1)// 打开文件
						{
							Sleep(1000);
							continue;
						}
						else
							break;
					}

					if (n != IS_FOLDER_OR_LIST)
					{
						printf("只有一首歌！");
						mciSendString(T_open, 0, 0, 0);//重新打开当前文件
					}

					i = 0;//重置播放进度
					mciSendString(T_temp, 0, 0, 0);//调节音量大小
					mciSendString(TEXT("play music"), 0, 0, 0);
					break;
				}
				case '0'://播放上一首
				{
					if (line <= 0)
					{
						printf("已到达第一首歌！");
						i = line = 0;
						system("cls");
						continue;
					}

					mciSendString(TEXT("close music"), 0, 0, 0);//关闭播放

					while (n == IS_FOLDER_OR_LIST)// n==1 才会执行
					{
						OpenMusicList(T_musiclist, T_filename, &line, 2);

						if (OpenMusic(T_filename, "\n正在打开下一首...") == 1)// 打开文件
						{
							Sleep(1000);
							continue;
						}
						else
							break;
					}
					if (n != IS_FOLDER_OR_LIST)
					{
						printf("只有一首歌！");
						mciSendString(T_open, 0, 0, 0);
					}

					mciSendString(T_temp, 0, 0, 0);//调节音量大小
					mciSendString(TEXT("play music"), 0, 0, 0);
					i = 0;//重置播放进度

					break;
				}
				case 'r':
				{
					mode = 'u';
					printf("循环播放...");
					break;
				}
				case 't'://单曲循环
				{
					mode = 't';
					printf("单曲循环...");
					break;
				}
				case 'j'://关闭声音
				{
					mciSendString(TEXT("Set music audio all off"), 0, 0, 0);
					printf("静音...");
					break;
				}
				case 'k'://开启声音
				{
					mciSendString(TEXT("Set music audio all on"), 0, 0, 0);
					printf("开启声音...");
					break;
				}
				case 'e'://退出
				{
					mciSendString(TEXT("close music"), 0, 0, 0);//关闭播放
					printf("退出...");
					return 0;
				}
				break;
				}//switch()

			}

			Sleep(100);

			//覆盖刷新控制台
			SetConsoleCursorPosition(hConsole, pos2);//设置光标位置
			printf("\r                                                                         \r");
			SetConsoleCursorPosition(hConsole, pos1);//设置光标位置
			printf("\r                                                                         \r");
			//获取音乐文件长度
			mciSendString(TEXT("status music length"), T_temp, MAX_PATH, 0);
			Llength = wcstol(T_temp, NULL, 10);// 使用宽字符版本
		}
	}
	system("pause");
	return 0;
}

// 打开音乐文件
int OpenMusic(const TCHAR* musicName, const char* defaultErr)
{
	TCHAR T_open[MAX_PATH]; // 打开文件
	char name[MAX_PATH];

	wsprintf(T_open, L"open \"%s\" alias music", musicName);// 打开文件

	if (mciSendString(T_open, 0, 0, 0) != 0) // 打开文件
	{
		wchar_to_char(musicName, name, arr_count(name));
		printf("\n打开%s错误！", name);

		perror("原因");
		if (defaultErr != NULL)// 是否打印指定的错误内容
			printf("%s", defaultErr);
		return true;
	}
	return false;
}

// 判断是否为目录、列表、.MP3文件
int CheckExtension(TCHAR* path)
{
	DWORD dwAttr;
	wchar_t extension[5];

	if ((_waccess(path, 0)) == -1)
	{
		printf("\n文件或目录不存在！\n");
		return IS_ERR;
	}

	// 分割文件名，得到文件后缀名
	_wsplitpath_s(path, NULL, 0, NULL, 0, NULL, 0, extension, 5);
	// 转小写
	_wcslwr_s(extension, wcslen(extension) + 1);
	// 判断后缀名是不是为.mp3的文件
	if (wcscmp(extension, L".mp3") == 0)
	{
		return IS_MP3_FILE;
	}
	// 判断后缀名是否为.txt（音乐列表）
	if (wcscmp(extension, L".txt") == 0)
	{
		return IS_FOLDER_OR_LIST;
	}
	// 判断路径 是否为目录
	if ((dwAttr = GetFileAttributes(path)) != INVALID_FILE_ATTRIBUTES)
	{
		if (dwAttr & FILE_ATTRIBUTE_DIRECTORY)
			return IS_FOLDER_OR_LIST;
	}
	else
	{
		printf("\n获取文件信息失败！\n");
		return IS_ERR;
	}

	//如果不是目录、列表、后缀名为,mp3的文件
	printf("\n必须是目录、列表和后缀名为.mp3的文件！\n");
	system("pause");
	return IS_ERR;
}

// 将文件夹里的mp3文件导出到txt文件
void ExportMusicList(TCHAR* path, TCHAR* T_musiclist)
{
	wchar_t cmd[MAX_PATH];
	_wremove(T_musiclist);// 删除原来的文件列表
	swprintf_s(cmd, MAX_PATH, L"dir /a-d /s /b /on \"%s\\*.mp3\" >> \"%s\"", path, T_musiclist);
	_wsystem(cmd);	      // cmd命令将目录中的MP3文件名导出到文件
}

//打开音乐列表，查找音乐文件名
int OpenMusicList(const TCHAR* MusicList, TCHAR* MusicName, int* line, int mode)
{
	FILE* fp;
	char C_FileName[MAX_PATH] = { 1 };
	int i = 0;

	memset(MusicName, 0, MAX_PATH);
	if (_wfopen_s(&fp, MusicList, L"r") != 0)// 是目录则创建音乐列表并打开
	{
		perror("\n打开音乐列表失败");
		system("pause");
		exit(EXIT_FAILURE);
	}

	if (fp == NULL)
		return 1;

	if (mode == 2)// 逆序播放
		*line -= 1;

	while (C_FileName[0] != '\0' && i <= *line)// 从文件中读取第line行的文件名
	{
		memset(C_FileName, 0, MAX_PATH);// 数组清零
		fgets(C_FileName, MAX_PATH, fp);// 从音乐列表中读取文件名
		del_last_newline(C_FileName);
		i++;// 统计读取了多少首歌	
	}

	fclose(fp);

	if (C_FileName[0] == '\0')// 如果FileName[0] = '\0'，则说明已读到文件末尾
		return 1;
	if (mode == 1)// 顺序播放
		*line += 1;

	char_to_wchar(C_FileName, MusicName, MAX_PATH);// 将多字节转为宽字节
	return 0;
}

void Refresh(COORD* pos)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);//获取输出句柄
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	if (GetConsoleScreenBufferInfo(hConsole, &csbi))//获取光标位置
	{
		(*pos).X = csbi.dwCursorPosition.X;
		(*pos).Y = csbi.dwCursorPosition.Y;
	}
}