#include <stdio.h>
#include <direct.h>
#include <string.h>
#include <stdlib.h>
#include <io.h>
#include <windows.h>
#include <mmsystem.h>
#include <conio.h>
#pragma comment(lib,"winmm.lib")

#define SIZE 260
#define IS_MP3_FILE 0
#define IS_FOLDER_OR_LIST 1

int OpenMusic(const TCHAR* musicName, const char* defaultErr);
//清除'\n'
void Fgetws_n(wchar_t* str);
//将char转为TCHAR
void CharToTCHAR(const char* C_str, TCHAR* T_str, int destSize);
//将TCHAR转char
char* TCHARToChar(const TCHAR* T_str);
//判断是文件还是目录
int Check_extension(TCHAR* T_filename, TCHAR* T_musiclist);
//打开音乐列表，查找音乐文件名
int OpenMusicList(const TCHAR* MusicList, TCHAR* MusicName, int* line, int mode);
//获取控制台光标位置
void Refresh(COORD* pos);

int main(int argc, char* argv[])
{
	TCHAR T_filename[SIZE];//文件名
	TCHAR T_open[SIZE];    //打开文件
	TCHAR T_temp[SIZE] = { 0 };
	TCHAR T_musiclist[SIZE];//音乐列表路径

	char mode = 'u';
	char C_fileName[SIZE];
	long Lcurrent, Llength, Lvolume = 600;
	int  n, ch, j = 0, i = 0, line = 0;
	COORD pos1, pos2;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);// 获取输出句柄

	if (argv[1] != NULL)
	{
		CharToTCHAR(argv[1], T_filename, SIZE);
	}
	else
	{
		printf("请输入文件或文件夹或文件列表路径： ");
		fgets(C_fileName, SIZE, stdin); // 输入音乐名称
		rewind(stdin);					// 清除缓冲区

		CharToTCHAR(C_fileName, T_filename, SIZE);
		Fgetws_n(T_filename);			// 清除fgets读取的'\n'
		if (T_filename[0] == 34)		// 支持文件拖拽输入
		{
			T_filename[wcslen(T_filename) - 1] = 0;
			wcscpy_s(T_filename, wcslen(T_filename) + 1, T_filename + 1);// 清除拖拽输入多出来的双引号
		}
	}


	GetCurrentDirectory(SIZE, T_musiclist);     // 获取当前程序所在的目录   
	wcscat_s(T_musiclist, SIZE, L"\\music.txt");// 获取的音乐列表文件路径


	n = Check_extension(T_filename, T_musiclist);// 判断是否为目录、列表、.MP3文件

	while (1)
	{
		if (mode == 'u')
		{
			mciSendString(TEXT("close music"), 0, 0, 0);//关闭播放

			if (n != IS_FOLDER_OR_LIST)
			{
				if (OpenMusic(T_filename, NULL) == 0)// 打开文件
				{
					system("pause");
					return 1;
				}
				mciSendString(T_open, 0, 0, 0);// 重新打开当前文件
			}
			else
			{
				while (1)
				{
					//int j = 0;
					if (j == 10)
						return 1;

					if (OpenMusicList(T_musiclist, T_filename, &line, 1))
					{
						printf("\n已达最后一首！");
						line = 0;
						j++;
						continue;
					}

					if (OpenMusic(T_filename, "\n正在打开下一首...") == 0)// 打开文件
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
		mciSendString(TEXT("status music length"), T_temp, SIZE, 0);
		Llength = wcstol(T_temp, NULL, 10);//使用宽字符版本
		i = Lcurrent = 0;//重置当前播放的位置                         
		mciSendString(TEXT("setaudio music volume to 600"), 0, 0, 0);//设定默认音量为600   
		mciSendString(TEXT("play music"), 0, 0, 0);//播放

		system("mode con cols=120 lines=10");
		system("cls");
		printf("\n按键:\n");
		printf("1.暂停播放   2.继续播放   4.播放回退   5.播放快进  \n");
		printf("6.音量增大   7.音量减小   8.停止播放  ->.播放微快进 <-.播放微回退\n");
		printf("+.音量微增大 -.音量微减小 9.播放下一首 0.播放上一首  R.循环播放  \n");
		printf("J.静音       K.开启声音   T.单曲循环   U.逆序播放    E.退出\n\n");

		for (i = (Lcurrent / 1000); i <= (Llength / 1000); i++)
		{
			char* tmp = TCHARToChar(T_filename);
			printf("正在播放：%s", tmp);
			free(tmp);

			Refresh(&pos1);
			printf("\n%02d:%02d/%02d:%02d    ", i / 60, i % 60, Llength / 1000 / 60, Llength / 1000 % 60);
			Refresh(&pos2);

			memset(T_temp, 0, SIZE);

			if (_kbhit())
			{
				if ((ch = _getch()) == 0xE0) //判断高位是否相等
					ch = _getch();//高位相等说明是方向键，就再读取一次


				if (ch == '4' || ch == 75)//方向键左键
				{
					mciSendString(TEXT("status music position"), T_temp, SIZE, 0);//获取当前播放的位置
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
					mciSendString(TEXT("status music position"), T_temp, SIZE, 0);//获取当前播放的位置
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

					while (n == 1)// n==1 才会执行
					{
						if (OpenMusicList(T_musiclist, T_filename, &line, 1) == IS_FOLDER_OR_LIST)
						{
							printf("\n已到达最后一首歌！");
							line = 0;
							continue;
						}

						if (OpenMusic(T_filename, "\n正在打开下一首...") == 0)// 打开文件
						{
							Sleep(1000);
							continue;
						}
						else
							break;
					}

					if (n != 1)
					{
						printf("\n只有一首歌！");
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
						printf("\n已到达第一首歌！");
						i = line = 0;
						system("cls");
						continue;
					}

					mciSendString(TEXT("close music"), 0, 0, 0);//关闭播放

					while (n == 1)// n==1 才会执行
					{
						OpenMusicList(T_musiclist, T_filename, &line, 2);

						if (OpenMusic(T_filename, "\n正在打开下一首...") == 0)// 打开文件
						{
							Sleep(1000);
							continue;
						}
						else
							break;
					}
					if (n != 1)
					{
						printf("\n只有一首歌！");
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
					exit(EXIT_SUCCESS);
				}
				break;
				}//switch()

			}//if()

			Sleep(1000);

			//覆盖刷新控制台
			SetConsoleCursorPosition(hConsole, pos2);//设置光标位置
			printf("\r                                                                         \r");
			SetConsoleCursorPosition(hConsole, pos1);//设置光标位置
			printf("\r                                                                         \r");

			mciSendString(TEXT("status music length"), T_temp, SIZE, 0);//获取音乐文件长度
			Llength = wcstol(T_temp, NULL, 10);//使用宽字符版本
		}
	}
	system("pause");
	return 0;
}


int OpenMusic(const TCHAR* musicName, const char* defaultErr)
{
	TCHAR T_open[SIZE]; // 打开文件
	char* name = NULL;

	wsprintf(T_open, L"open \"%s\" alias music", musicName);// 打开文件
	if (mciSendString(T_open, 0, 0, 0) != 0) // 打开文件
	{
		name = TCHARToChar(musicName);
		printf("\n打开%s错误！", name);
		free(name);

		perror("原因");
		if (defaultErr != NULL)// 是否打印指定的错误内容
			printf("%s", defaultErr);
		return 0;
	}
	return 1;
}


//将char转为TCHAR
void CharToTCHAR(const char* C_str, TCHAR* T_str, int destSize)
{
	MultiByteToWideChar(CP_ACP, 0, C_str, -1, T_str, destSize);// 将char转为TCHAR  
}


char* TCHARToChar(const TCHAR* T_str)
{
	CHAR* defaultChar = "*";
	int size = WideCharToMultiByte(CP_ACP, 0, T_str, -1, NULL, 0, NULL, NULL); // 先取得目标缓冲区需要的大小
	static char* buff = NULL;
	buff = (char*)malloc(size); // 开辟缓冲区
	WideCharToMultiByte(CP_ACP, 0, T_str, -1, buff, size, defaultChar, NULL);
	return buff;
}


// 判断是文件还是目录
int Check_extension(TCHAR* T_filename, TCHAR* T_musiclist)
{
	DWORD dwAttr;
	wchar_t cmd[SIZE];
	wchar_t extension[5];

	_wsplitpath_s(T_filename, NULL, 0, NULL, 0, NULL, 0, extension, 5);// 分割文件名，得到文件后缀名

	if ((_waccess(T_filename, 0)) == -1)
	{
		printf("\n文件或目录不存在！\n");
		system("pause");
		exit(EXIT_FAILURE);
	}

	if (wcscmp(extension, L".mp3") != 0 && wcscmp(extension, L".MP3") != 0)// 判断不是后缀名为.mp3的文件
	{
		if (wcscmp(extension, L".txt") == 0 || wcscmp(extension, L".TXT") == 0)// 判断后缀名是否为.txt（音乐列表）
		{
			return IS_FOLDER_OR_LIST;// 是目录或列表返回1
		}
		else if ((dwAttr = GetFileAttributes(T_filename)) == INVALID_FILE_ATTRIBUTES)// 否则判断是否为目录
		{
			printf("\n获取文件信息失败！\n");
			system("pause");
			exit(EXIT_FAILURE);
		}

		if (dwAttr & FILE_ATTRIBUTE_DIRECTORY)//判断是否为目录
		{
			_wremove(T_musiclist);// 删除原来的文件列表
			swprintf_s(cmd, SIZE, L"dir /a-d /s /b /on \"%s\\*.mp3\" >> \"%s\"", T_filename, T_musiclist);
			_wsystem(cmd);	      // cmd命令将目录中的MP3文件名导出到文件
			return IS_FOLDER_OR_LIST;			  // 是目录或列表返回1
		}
		else//如果不是目录、列表、后缀名为,mp3的文件
		{
			printf("\n必须是目录、列表和后缀名为.mp3的文件！\n");
			system("pause");
			exit(EXIT_FAILURE);
		}
	}
	return IS_MP3_FILE;// 是MP3文件返回0
}


//打开音乐列表，查找音乐文件名
int OpenMusicList(const TCHAR* MusicList, TCHAR* MusicName, int* line, int mode)
{
	FILE* fp;
	wchar_t FileName[SIZE] = { 1 };
	int i = 0;

	memset(MusicName, 0, SIZE);
	if (_wfopen_s(&fp, MusicList, L"r") != 0)// 是目录则创建音乐列表并打开
	{
		perror("\n打开音乐列表失败");
		system("pause");
		exit(EXIT_FAILURE);
	}

	if (mode == 2)// 逆序播放
		*line -= 1;

	while (FileName[0] != '\0' && i <= *line)// 从文件中读取第line行的文件名
	{
		memset(FileName, 0, SIZE);// 数组清零

		fgetws(FileName, SIZE, fp);// 从音乐列表中读取文件名
		Fgetws_n(FileName);
		i++;// 统计读取了多少首歌	
	}

	if (mode == 1)// 顺序播放
		*line += 1;
	if (FileName[0] == '\0')// 如果FileName[0] = '\0'，则说明已读到文件末尾
		return 1;

	fclose(fp);
	wcscpy_s(MusicName, SIZE, FileName);
	return 0;
}


void Fgetws_n(wchar_t* str)
{
	wchar_t* find;
	if ((find = wcschr(str, L'\n')))
		*find = '\0';
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