#include <stdio.h>
#include <direct.h>
#include <string.h>
#include <stdlib.h>
#include <io.h>
#include <windows.h>
#include <mmsystem.h>
#include <conio.h>
#include <locale.h>
#pragma comment(lib,"winmm.lib")

#define SIZE 260
#define IS_MP3_FILE 0
#define IS_FOLDER_OR_LIST 1

int open_music(const wchar_t* musicName, const wchar_t* defaultErr);
//清除'\n'
void del_last_char(wchar_t* str, wchar_t ch);
void Fgetws_n(wchar_t* str);
//判断是文件还是目录
int is_file(wchar_t* fileName, wchar_t* musiclist);
//打开音乐列表，查找音乐文件名
int open_musiclist(const wchar_t* MusicList, wchar_t* MusicName, int* line, int mode);
//获取控制台光标位置
void refcur(COORD* pos);

// 获取程序所在的目录（不带“\\”）
void get_curdir(wchar_t* curdir, size_t size)
{
	wchar_t path[SIZE], drive[SIZE], dir[SIZE];
	size_t len;
	// 获取当前程序所在的路径
	GetModuleFileName(NULL, path, SIZE);
	// 分割路径
	_wsplitpath_s(path, drive, SIZE, dir, SIZE, NULL, 0, NULL, 0);
	len = wcslen(dir);
	// 清除路径末尾的“\\”
	if (dir[len] == '\\')
		dir[len] = '\0';
	swprintf_s(curdir, size, L"%s%s", drive, dir);
}

void del_last_char(wchar_t* str, wchar_t ch)
{
	size_t len = wcslen(str);
	if (str[len - 1] == ch)
		str[len - 1] = '\0';
}

void cutoff_str()
{

}

void reverse_play()
{

}

void pause_play()
{

}

void continue_play()
{

}



int main(int argc, wchar_t* argv[])
{
	wchar_t fileName[SIZE];//文件名
	wchar_t open[SIZE];    //打开文件
	wchar_t temp[SIZE] = { 0 };
	wchar_t musiclist[SIZE];//音乐列表路径
	wchar_t mode = 'u'; // 播放模式
	long cur, len, vol = 600;
	int  n, ch, j = 0, i = 0, line = 0;
	COORD pos1, pos2;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);// 获取输出句柄

	setlocale(LC_ALL, "chs");
	if (!argv[1])
	{
		wprintf(L"请输入文件或文件夹或文件列表路径： ");
		fgetws(fileName, SIZE, stdin); // 输入音乐名称
		del_last_char(fileName, TEXT('\n'));// 清除fgets读取的'\n'
		if (fileName[0] == TEXT('\"'))		  // 支持文件拖拽输入
		{
			fileName[wcslen(fileName) - 1] = 0;
			wcscpy_s(fileName, SIZE, fileName + 1);// 清除拖拽输入多出来的双引号
		}
	}
	else
		wcscpy_s(fileName, SIZE, argv[1]);

	GetCurrentDirectory(SIZE, musiclist);     // 获取当前程序所在的目录   
	wcscat_s(musiclist, SIZE, L"\\music.txt");// 获取的音乐列表文件路径
	n = is_file(fileName, musiclist);// 判断是否为目录、列表、.MP3文件

	while (1)
	{
		if (mode == 'u')
		{
			mciSendString(TEXT("close music"), 0, 0, 0);//关闭播放

			if (n != IS_FOLDER_OR_LIST)
			{
				if (open_music(fileName, NULL) == 1)// 打开文件
				{
					system("pause");
					return 1;
				}
				mciSendString(open, 0, 0, 0);// 重新打开当前文件
			}
			else
			{
				j = 1;
				while (1)
				{
					if (j >= 3)
						return -1;
					j++;

					if (open_musiclist(musiclist, fileName, &line, 1))
					{
						wprintf(L"\n已到达最后一首歌！");
						line = 0; // 从头播放
						continue;
					}

					if (open_music(fileName, L"\n正在打开下一首...") == 1)// 打开文件
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
			mciSendString(open, 0, 0, 0);//重新打开当前文件
			mciSendString(TEXT("play music"), 0, 0, 0);
		}
		else if (mode == 'y')
			;


		//获取音乐文件长度
		mciSendString(TEXT("status music length"), temp, SIZE, 0);
		len = wcstol(temp, NULL, 10);//使用宽字符版本
		i = cur = 0;//重置当前播放的位置                         
		mciSendString(TEXT("setaudio music volume to 600"), 0, 0, 0);//设定默认音量为600   
		mciSendString(TEXT("play music"), 0, 0, 0);//播放

		system("mode con cols=120 lines=10");
		system("cls");
		wprintf(L"\n按键:\n");
		wprintf(L"1.暂停播放   2.继续播放   4.播放回退   5.播放快进  \n");
		wprintf(L"6.音量增大   7.音量减小   8.停止播放  ->.播放微快进 <-.播放微回退\n");
		wprintf(L"+.音量微增大 -.音量微减小 9.播放下一首 0.播放上一首  R.循环播放  \n");
		wprintf(L"J.静音       K.开启声音   T.单曲循环   U.逆序播放    E.退出\n\n");

		for (i = (cur / 1000); i <= (len / 1000); i++)
		{
			wprintf(L"正在播放：%s", fileName);// 截断字符串
			refcur(&pos1);
			wprintf(L"\n%02d:%02d/%02d:%02d    ", i / 60, i % 60, len / 1000 / 60, len / 1000 % 60);
			refcur(&pos2);

			memset(temp, 0, SIZE);

			if (_kbhit())
			{
				if ((ch = _getch()) == 0xE0) //判断高位是否相等
					ch = _getch();//高位相等说明是方向键，就再读取一次


				if (ch == '4' || ch == 75)//方向键左键
				{
					mciSendString(TEXT("status music position"), temp, SIZE, 0);//获取当前播放的位置
					cur = wcstol(temp, NULL, 10);

					if (ch == '4')
						cur -= 10000;
					else
						cur -= 1000;

					if (cur < 0)//在当前数值的基础上减10000，如果结果小于0则快退到开始状态
						cur = 0;

					i = cur / 1000;

					wsprintf(temp, L"seek music to %ld", cur);
					mciSendString(temp, 0, 0, 0);//修改播放的位置
					mciSendString(TEXT("play music"), 0, 0, 0);
					wprintf(L"快退...");
				}
				else if (ch == '5' || ch == 77)//方向键右键
				{
					mciSendString(TEXT("status music position"), temp, SIZE, 0);//获取当前播放的位置
					cur = wcstol(temp, NULL, 10);

					if (ch == '5')
						cur += 10000;
					else
						cur += 1000;

					i = cur / 1000;
					wsprintf(temp, L"seek music to %ld", cur);//在当前数值的基础上加10000
					mciSendString(temp, 0, 0, 0);//修改播放的位置
					mciSendString(TEXT("play music"), 0, 0, 0);
					wprintf(L"快进...");
				}
				else if (ch == '6' || ch == '+')//增加音量
				{
					if (ch == '6')
						vol += 100;//每次增加100
					else
						vol += 10;//每次增加10

					if (vol > 1000)
					{
						wprintf(L"已到达最大音量！");
						vol = 1000;
						wsprintf(temp, L"setaudio music volume to %d", vol);
						mciSendString(temp, 0, 0, 0);//调节音量大小
						continue;
					}

					wsprintf(temp, L"setaudio music volume to %d", vol);
					mciSendString(temp, 0, 0, 0);//调节音量大小
					wprintf(L"当前音量为：%.1lf", (double)vol / 100);
				}
				else if (ch == '7' || ch == '-')//减小音量
				{
					if (ch == '7')
						vol -= 100;//每次减小100
					else
						vol -= 10;//每次减小10

					if (vol < 0)
					{
						wprintf(L"已到达最小音量！");
						vol = 0;
					}

					wsprintf(temp, L"setaudio music volume to %d", vol);
					mciSendString(temp, 0, 0, 0);//调节音量大小
					wprintf(L"当前音量为：%.1lf", (double)vol / 100);
				}


				switch (ch)
				{
				case '1'://暂停播放
				{
					mciSendString(TEXT("pause music"), 0, 0, 0);
					wprintf(L"暂停...");
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
					wprintf(L"继续播放...");
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

					while (n == IS_FOLDER_OR_LIST)// n==1 才会执行
					{
						if (open_musiclist(musiclist, fileName, &line, 1) == IS_FOLDER_OR_LIST)
						{
							wprintf(L"\n已到达最后一首歌！");
							line = 0;
							continue;
						}

						if (open_music(fileName, L"\n正在打开下一首...") == 1)// 打开文件
						{
							Sleep(1000);
							continue;
						}
						else
							break;
					}

					if (n != IS_FOLDER_OR_LIST)
					{
						wprintf(L"\n只有一首歌！");
						mciSendString(open, 0, 0, 0);//重新打开当前文件
					}

					i = 0;//重置播放进度
					mciSendString(temp, 0, 0, 0);//调节音量大小
					mciSendString(TEXT("play music"), 0, 0, 0);
					break;
				}
				case '0'://播放上一首
				{
					if (line <= 0)
					{
						wprintf(L"\n已到达第一首歌！");
						i = line = 0;
						system("cls");
						continue;
					}

					mciSendString(TEXT("close music"), 0, 0, 0);//关闭播放

					while (n == IS_FOLDER_OR_LIST)// n==1 才会执行
					{
						open_musiclist(musiclist, fileName, &line, 2);

						if (open_music(fileName, L"\n正在打开下一首...") == 1)// 打开文件
						{
							Sleep(1000);
							continue;
						}
						else
							break;
					}
					if (n != IS_FOLDER_OR_LIST)
					{
						wprintf(L"\n只有一首歌！");
						mciSendString(open, 0, 0, 0);
					}

					mciSendString(temp, 0, 0, 0);//调节音量大小
					mciSendString(TEXT("play music"), 0, 0, 0);
					i = 0;//重置播放进度

					break;
				}
				case 'r':
				{
					mode = 'u';
					wprintf(L"循环播放...");
					break;
				}
				case 't'://单曲循环
				{
					mode = 't';
					wprintf(L"单曲循环...");
					break;
				}
				case 'j'://关闭声音
				{
					mciSendString(TEXT("Set music audio all off"), 0, 0, 0);
					wprintf(L"静音...");
					break;
				}
				case 'k'://开启声音
				{
					mciSendString(TEXT("Set music audio all on"), 0, 0, 0);
					wprintf(L"开启声音...");
					break;
				}
				case 'e'://退出
				{
					mciSendString(TEXT("close music"), 0, 0, 0);//关闭播放
					wprintf(L"退出...");
					exit(EXIT_SUCCESS);
				}
				break;
				}//switch()

			}//if()

			Sleep(1000);

			//覆盖刷新控制台
			SetConsoleCursorPosition(hConsole, pos2);//设置光标位置
			wprintf(L"\r                                                                         \r");
			SetConsoleCursorPosition(hConsole, pos1);//设置光标位置
			wprintf(L"\r                                                                         \r");

			mciSendString(TEXT("status music length"), temp, SIZE, 0);//获取音乐文件长度
			len = wcstol(temp, NULL, 10);//使用宽字符版本
		}
	}
	system("pause");
	return 0;
}


int open_music(const wchar_t* musicName, const wchar_t* defaultErr)
{
	wchar_t open[SIZE]; // 打开文件
	wchar_t* name = NULL;

	wsprintf(open, L"open \"%s\" alias music", musicName);// 打开文件
	if (mciSendString(open, 0, 0, 0) != 0) // 打开文件
	{
		wprintf(L"\n打开%s错误！", name);
		free(name);
		_wperror(L"原因：");
		if (defaultErr != NULL)// 是否打印指定的错误内容
			wprintf(L"%s", defaultErr);
		return 1;
	}
	return 0;
}

// 判断是文件还是目录
int is_file(wchar_t* fileName, wchar_t* musiclist)
{
	DWORD dwAttr;
	wchar_t cmd[SIZE];
	wchar_t extension[5];

	_wsplitpath_s(fileName, NULL, 0, NULL, 0, NULL, 0, extension, 5);// 分割文件名，得到文件后缀名

	if ((_waccess(fileName, 0)) == -1)
	{
		wprintf(L"\n文件或目录不存在！\n");
		system("pause");
		exit(EXIT_FAILURE);
	}

	if (wcscmp(extension, L".mp3") != 0 && wcscmp(extension, L".MP3") != 0)// 判断不是后缀名为.mp3的文件
	{
		if (wcscmp(extension, L".txt") == 0 || wcscmp(extension, L".TXT") == 0)// 判断后缀名是否为.txt（音乐列表）
		{
			return IS_FOLDER_OR_LIST;// 是目录或列表返回1
		}
		else if ((dwAttr = GetFileAttributes(fileName)) == INVALID_FILE_ATTRIBUTES)// 否则判断是否为目录
		{
			wprintf(L"\n获取文件信息失败！\n");
			system("pause");
			exit(EXIT_FAILURE);
		}

		if (dwAttr & FILE_ATTRIBUTE_DIRECTORY)//判断是否为目录
		{
			_wremove(musiclist);// 删除原来的文件列表
			swprintf_s(cmd, SIZE, L"dir /a-d /s /b /on \"%s\\*.mp3\" >> \"%s\"", fileName, musiclist);
			_wsystem(cmd);	      // cmd命令将目录中的MP3文件名导出到文件
			return IS_FOLDER_OR_LIST;			  // 是目录或列表返回1
		}
		else//如果不是目录、列表、后缀名为,mp3的文件
		{
			wprintf(L"\n必须是目录、列表和后缀名为.mp3的文件！\n");
			system("pause");
			exit(EXIT_FAILURE);
		}
	}
	return IS_MP3_FILE;// 是MP3文件返回0
}


//打开音乐列表，查找音乐文件名
int open_musiclist(const wchar_t* MusicList, wchar_t* MusicName, int* line, int mode)
{
	FILE* fp;
	wchar_t fileName[SIZE] = { 1 };
	int i = 0;

	memset(MusicName, 0, SIZE);
	if (_wfopen_s(&fp, MusicList, L"r") != 0)// 是目录则创建音乐列表并打开
	{
		_wperror(L"\n打开音乐列表失败\n原因：");
		system("pause");
		exit(EXIT_FAILURE);
	}
	if (!fp)
	{
		wprintf(L"\n打开音乐列表失败");
		system("pause");
		exit(EXIT_FAILURE);
	}
	if (mode == 2)// 逆序播放
		*line -= 1;

	while (fileName[0] != '\0' && i <= *line)// 从文件中读取第line行的文件名
	{
		memset(fileName, 0, SIZE);// 数组清零
		fgetws(fileName, SIZE, fp);// 从音乐列表中读取文件名
		Fgetws_n(fileName);
		i++;// 统计读取了多少首歌	
	}

	fclose(fp);
	if (fileName[0] == '\0')// 如果FileName[0] = '\0'，则说明已读到文件末尾
		return 1;
	if (mode == 1)// 顺序播放
		*line += 1;

	wcscpy_s(MusicName, SIZE, fileName);
	return 0;
}

void Fgetws_n(wchar_t* str)
{
	wchar_t* find;
	if ((find = wcschr(str, L'\n')))
		*find = '\0';
}

void refcur(COORD* pos)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);//获取输出句柄
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	if (GetConsoleScreenBufferInfo(hConsole, &csbi))//获取光标位置
	{
		(*pos).X = csbi.dwCursorPosition.X;
		(*pos).Y = csbi.dwCursorPosition.Y;
	}
}