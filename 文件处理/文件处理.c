#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <string.h>

#define IS_UNICODE_TEXT 1

int IsUnicodeText(char* fileName, size_t size);
long Get_File_Size(const char* fileName);     //获取文件大小
void Letter(char* fileName, int mode);        //将大小写字母转换为小大写字母
int  Reverse_output(const char* FileName, char* buffer, long file_size, int mode, int* total, int* line);//读取文件内容
void utf_to_gbk(char* utf_str, char* gbk_str);//把utf-8编码转为gbk编码
int  Judging_Zh(char ch);					  //判断中文字符
void Tip(int argc, const char* str);		  //输出提示
long long CetCurrentMemory(void);             //获取当前可用内存大小


FILE* fpread, * fpwrite;
errno_t err;


int main(int argc, char* argv[])
{
	char filename[MAX_PATH];
	char parameter[5] = { 0 };
	long file_size;
	int ch, mode = 1, i = 0, line, total;



	if (argc == 1)
	{
		printf("\n编译日期：2020-08-01\n");
		printf("用法：文件处理.exe <命令> <文件路径1> [ <文件路径2>可选 ] ... \n\n");
		printf("命令：\n");
		printf(
			"1、 打印文件内容到控制台                             -p    或 -p -u\n"
			"2、 逆序打印文件内容到控制台                         -p -r 或 -p -r -u\n"
			"3、 复制文件                                         -c	  \n"
			"4、 计算文件大小                                     -s    \n"
			"5、 只打印文本文件中给定字符的一行                   -p -e 或 -p -e -u\n"
			"6、 把文本中的所有字母转换为大写                     -u    \n"
			"7、 把文本中的所有字母转换为小写                     -w    \n"
			"8、 计算文本文件的行数并在每一行前添加行号           -l -a \n"
			"9、 把两个文本文件不同的一行打印并输出是哪一行       -p -l 或 -p -l -u\n"
			"10、把一个文本文件的内容逆序输出到另一个文本文件     -t -r\n"
			"11、把一个文本文件的内容添加到另一个文本文件的结尾处 -a -e\n\n");
		return 0;
	}


	//将参数去掉“-”再写入数组
	if (*argv[1] == '-')//判断第一个参数
	{
		parameter[0] = *++argv[1];

		if (argc > 2)//判断是否有第二个参数
		{
			if (*argv[2] == '-')//判断第二个参数
			{
				parameter[1] = *++argv[2];

				if (argv[3] != NULL)//判断是否有第三个参数
				{
					if (*argv[3] == '-')//判断第三个参数
						parameter[2] = *++argv[3];
				}
			}
		}
	}

	Tip(argc, parameter);


	//打印文件内容到控制台
	if ((strcmp(parameter, "p") == 0 || strcmp(parameter, "pu") == 0))
	{
		if (strcmp(parameter, "pu") == 0)//判断输入的参数
		{
			if (argv[3] == NULL)
				exit(EXIT_FAILURE);
			strcpy_s(filename, MAX_PATH, argv[3]);
			mode = 2;
		}
		else
			strcpy_s(filename, MAX_PATH, argv[2]);

		file_size = Get_File_Size(filename);//获取文件大小
		char* buffer = (char*)calloc(file_size + 1, sizeof(char));//分配缓存

		Reverse_output(filename, buffer, file_size + 1, mode, &total, &line);//读取文件内容

		if (strcmp(parameter, "pu") == 0)
			puts(buffer);
		else
			puts(buffer);


		free(buffer);//释放内存
		buffer = NULL;

		printf("\n总计：%d行%d字。\n", line + 1, total);
		printf("\n\n如果乱码，请把文件另存为ANSI格式或在“-p”参数后面添加“-u”重试。\n");
	}



	//逆序打印文件内容到控制台
	if ((strcmp(parameter, "pr") == 0 || strcmp(parameter, "pru") == 0) && argv[3] != NULL)
	{
		if (strcmp(parameter, "pru") == 0)//判断输入的参数
		{
			if (argv[4] == NULL)
				exit(EXIT_FAILURE);
			strcpy_s(filename, MAX_PATH, argv[4]);
			mode = 2;
		}
		else
			strcpy_s(filename, MAX_PATH, argv[3]);

		long file_size = Get_File_Size(filename);//获取文件大小
		char* buffer = calloc((file_size + 1), sizeof(char));//分配缓存

		Reverse_output(filename, buffer, file_size + 1, mode, &total, &line);


		//因为要逆序打印到控制台，相当于把字符串倒过来，末尾的'\0'在字符串开头。所以i要-1，丢掉'\0'。
		for (i = strlen(buffer) - 1; i >= 0; i--)
		{
			if (buffer[i] < 0)//利用中文编码是负数特点，判断是中文还是英文
			{
				printf("%c%c", buffer[i - 1], buffer[i]);//中文同时输出两个字符
				i--;
			}
			else
				printf("%c", buffer[i]);
		}
		printf("\n总计：%d行%d字。\n", line + 1, total);
		printf("\n\n如果乱码，请把文件另存为ANSI格式或在“-p”参数后面添加“-u”重试。\n");
	}



	//复制文件
	if (strcmp(parameter, "c") == 0)
	{
		size_t len;
		long long currentMemory, S, FileSize;//获取当前可用物理内存大小
		int i = 2;
		MEMORYSTATUSEX memoryStatus;
		memoryStatus.dwLength = sizeof(memoryStatus);


		if (!GlobalMemoryStatusEx(&memoryStatus))//获取当前可用的物理内存
		{
			i = MessageBox(NULL, TEXT("获取当前可用内存失败，是否继续复制（可能导致电脑死机、蓝屏）？"), TEXT("错误"), MB_YESNO | MB_ICONERROR);
			if (i == IDNO)
				exit(EXIT_FAILURE);
		}
		if (i == IDYES)
			currentMemory = 10485760;//10MB
		else
			currentMemory = memoryStatus.ullAvailPhys;


		if ((err = fopen_s(&fpread, argv[2], "rb")) != 0)//以读的方式打开文件
		{
			perror("\n打开源文件失败");
			exit(EXIT_FAILURE);
		}

		fseek(fpread, 0, SEEK_END);//文件指针置于文件末尾
		S = FileSize = _ftelli64(fpread);//获取文件大小
		rewind(fpread);//文件指针置于文件开头

		if ((err = fopen_s(&fpwrite, argv[3], "wb")) != 0)//以写的方式打开文件
		{
			perror("\n打开目标文件失败");
			exit(EXIT_FAILURE);
		}

		printf("\n正在复制：%s ...", argv[2]);

		for (; i <= 100; i++)
		{
			if (currentMemory == 10485760)
				break;

			//将当前文件大小除以 i，并判断是否大于当前可用内存，加500MB（524288000 byte）是为了给系统留500MB内存空间	
			if ((FileSize / i + 524288000) <= currentMemory && FileSize < 4294967295 && FileSize <= 1073741824)//fileSize <= 4294967295 是为了确保因类型不同造成错误。fileSize是long long类型，calloc和fread需要size_t(unsigned int)型数据，4294967295是unsigned int表示的最大范围		
				break;

			if (i == 100)
			{
				printf("\n内存空间不足，请关闭一些应用程序重试！");
				exit(EXIT_FAILURE);
			}
			FileSize /= i;//求出复制文件时malloc分配的内存空间大小
		}


		char* buffer = (char*)calloc((size_t)(FileSize + 1), sizeof(char));
		if (buffer == NULL)//如果空间不足
		{
			printf("\n内存空间不足，请关闭一些应用程序重试！");
			exit(EXIT_FAILURE);
		}

		while ((len = fread(buffer, sizeof(char), (size_t)FileSize, fpread)) > 0)//复制文件
		{
			if (fwrite(buffer, sizeof(char), len, fpwrite) < len)
			{
				printf("\n目标磁盘空间不足，请释放空间后重试！");
				exit(EXIT_FAILURE);
			}
		}

		fclose(fpread);
		fclose(fpwrite);
		free(buffer);
		buffer = NULL;

		printf("\n复制成功！");

		if (S < 1024)
			printf("\n大小：%lld字节\n", S);
		else if (S < 1048576)
			printf("\n大小：%.2lfKB（%lld字节）\n", (double)S / 1024, S);
		else if (S < 1073741824)
			printf("\n大小：%.2lfMB（%lld字节）\n", (double)S / 1048576, S);
		else
			printf("\n大小：%.2lfGB（%lld字节）\n", (double)S / 1073741824, S);
	}



	//计算文件大小
	if (strcmp(parameter, "s") == 0)
	{
		FILE* fp;

		if (fopen_s(&fp, argv[2], "rb") != 0)
		{
			MessageBoxW(NULL, TEXT("打开文件错误！"), TEXT("错误"), MB_OK | MB_ICONERROR);
			exit(EXIT_FAILURE);
		}
		fseek(fp, 0, SEEK_END);
		long long FileSize = _ftelli64(fp);
		fclose(fp);

		if (FileSize < 1024)
			printf("\n大小：%lld字节\n", FileSize);
		else if (FileSize < 1048576)
			printf("\n大小：%.2lfKB（%lld字节）\n", (double)FileSize / 1024, FileSize);
		else if (FileSize < 1073741824)
			printf("\n大小：%.2lfMB（%lld字节）\n", (double)FileSize / 1048576, FileSize);
		else
			printf("\n大小：%.2lfGB（%lld字节）\n", (double)FileSize / 1073741824, FileSize);
	}



	//打印给定字符的一行
	if ((strcmp(parameter, "pe") == 0 || strcmp(parameter, "peu") == 0) && argv[3] != NULL)
	{
		//判断命令参数
		if (strcmp(parameter, "peu") == 0)
		{
			if (argv[4] == NULL)
				exit(EXIT_FAILURE);
			strcpy_s(filename, MAX_PATH, argv[4]);
		}
		else
			strcpy_s(filename, MAX_PATH, argv[3]);

		//计算文件大小
		file_size = Get_File_Size(filename);
		//分配缓存
		char* buffer = (char*)calloc(file_size + 1, sizeof(char));
		char* gbk = (char*)calloc(file_size + 1, sizeof(char));


		if ((err = fopen_s(&fpread, filename, "r")) != 0)
		{
			perror("\n打开文件失败");
			exit(EXIT_FAILURE);
		}

		while (!feof(fpread))
		{
			fgets(buffer, file_size + 1, fpread);//从文件读取一行

			if (strcmp(parameter, "peu") == 0)
			{
				utf_to_gbk(buffer, gbk);//将utf-8转换为gbk编码
				if (strstr(gbk, argv[5]) != NULL)//查找字符
					printf("%s", gbk);//打印
			}
			else
			{
				if (strstr(buffer, argv[4]) != NULL)//查找字符
					printf("%s", buffer);//打印
			}
		}

		fclose(fpread);
		free(buffer);
		printf("\n\n如果乱码，请把文件另存为ANSI格式或在“-p”参数后面添加“-u”重试。\n");
	}



	//把文本中的所有字母转换为大写
	if (strcmp(parameter, "u") == 0)
	{
		Letter(argv[2], 1);
	}



	//把文本中的所有字母转换为小写
	if (strcmp(parameter, "w") == 0)
	{
		Letter(argv[2], 2);
	}



	//计算文本文件的行数并在每一行前添加行号
	if (strcmp(parameter, "la") == 0 && argv[3] != NULL)
	{
		file_size = Get_File_Size(argv[3]);
		char temp_file[MAX_PATH] = { 0 };
		char* buffer = (char*)malloc(file_size + 1);
		line = 1;

		strcpy_s(temp_file, MAX_PATH, argv[3]);
		strcat_s(temp_file, MAX_PATH, ".temp");

		if (fopen_s(&fpread, argv[3], "r") != 0)
		{
			perror("\n打开文件失败");
			exit(EXIT_FAILURE);
		}
		if (fopen_s(&fpwrite, temp_file, "w") != 0)
		{
			perror("\n创建临时文件失败");
			exit(EXIT_FAILURE);
		}

		while (!feof(fpread))
		{
			fgets(buffer, file_size, fpread);//从文件读取一行
			fprintf(fpwrite, "%d  %s", line++, buffer);//添加行号后输出到文件
		}

		fclose(fpread);
		fclose(fpwrite);

		if (remove(argv[3]) || rename(temp_file, argv[3]))//删除源文件并把临时文件重命名为源文件
		{
			printf("\n失败！\n");
			remove(temp_file);
			exit(EXIT_FAILURE);
		}
		printf("\n总计：%d行\n", line - 1);
	}



	//把两个文本文件不同的一行打印并输出是哪一行
	if ((strcmp(parameter, "pl") == 0 || strcmp(parameter, "plu") == 0) && argv[3] != NULL)
	{
		char s_file[MAX_PATH], d_file[MAX_PATH];

		if (strcmp(parameter, "plu") == 0)
		{
			if (argv[4] == NULL)
				exit(EXIT_FAILURE);
			strcpy_s(s_file, MAX_PATH, argv[4]);
			strcpy_s(d_file, MAX_PATH, argv[5]);
		}
		else
		{
			strcpy_s(s_file, MAX_PATH, argv[3]);
			strcpy_s(d_file, MAX_PATH, argv[4]);
		}

		FILE* fp;
		//获取文件大小
		long s_file_size = Get_File_Size(s_file);
		long d_file_size = Get_File_Size(d_file);
		//分配缓存
		char* S_buffer = (char*)calloc(s_file_size + 1, sizeof(char));
		char* D_buffer = (char*)calloc(d_file_size + 1, sizeof(char));
		char* S_gbk = (char*)calloc(s_file_size + 1, sizeof(char));
		char* D_gbk = (char*)calloc(d_file_size + 1, sizeof(char));

		if (fopen_s(&fpread, s_file, "r") != 0)
		{
			perror("\n打开文件1失败");
			exit(EXIT_FAILURE);
		}
		if (fopen_s(&fp, d_file, "r") != 0)
		{
			perror("\n打开文件2失败");
			exit(EXIT_FAILURE);
		}

		while (!feof(fpread) && !feof(fp))
		{
			fgets(S_buffer, s_file_size, fpread);//读取一行
			fgets(D_buffer, d_file_size, fp);

			if (strcmp(parameter, "plu") == 0)//判断是否需要转换编码
			{
				utf_to_gbk(S_buffer, S_gbk);//将utf-8转为gbk
				utf_to_gbk(D_buffer, D_gbk);

				if (strcmp(S_gbk, D_gbk) != 0)//比较两个文件的同一行是否一致
				{
					if (strchr(S_gbk, '\n') == 0)//判断字符串是否有换行符
					{
						printf("\n第%d行：\n文件一：%s\n文件二：%s", line + 1, S_gbk, D_gbk);//没有换行符就添加换行符打印
						line++;
						continue;
					}
					printf("\n第%d行：\n文件一：%s文件二：%s", line + 1, S_gbk, D_gbk);//有换行符就不添加换行符打印
				}
				line++;
			}
			else
			{
				if (strcmp(S_buffer, D_buffer) != 0)//比较两个文件的同一行是否一致
				{
					if (strchr(S_buffer, '\n') == 0)//判断字符串是否有换行符
					{
						printf("\n第%d行：\n文件一：%s\n文件二：%s", line + 1, S_buffer, D_buffer);//没有换行符就添加换行符打印
						line++;
						continue;
					}
					printf("\n第%d行：\n文件一：%s文件二：%s", line + 1, S_buffer, D_buffer);//有换行符就不添加换行符打印
				}
				line++;
			}
		}
		printf("\n\n如果乱码，请把文件另存为ANSI格式或在“-p”参数后面添加“-u”重试。\n");

		free(S_buffer);
		free(D_buffer);
		free(S_gbk);
		free(D_gbk);
	}



	//把一个文本文件倒序输出到另一个文本文件
	if ((strcmp(parameter, "rt") == 0 || strcmp(parameter, "rtu") == 0) && argv[3] != NULL)
	{
		char s_file[MAX_PATH], d_file[MAX_PATH];

		if (strcmp(parameter, "rtu") == 0)
		{
			if (argv[4] == NULL)
				exit(EXIT_FAILURE);
			strcpy_s(s_file, MAX_PATH, argv[4]);
			strcpy_s(d_file, MAX_PATH, argv[5]);
			mode = 2;
		}
		else
		{
			strcpy_s(s_file, MAX_PATH, argv[3]);
			strcpy_s(d_file, MAX_PATH, argv[4]);
		}

		file_size = Get_File_Size(s_file);
		char* buffer = (char*)calloc(file_size + 1, sizeof(char));

		Reverse_output(s_file, buffer, file_size + 1, mode, &total, &line);//读取文件1的内容

		if ((err = fopen_s(&fpwrite, d_file, "w")) != 0)//打开文件2
		{
			perror("打开文件2失败");
			exit(EXIT_FAILURE);
		}

		for (i = strlen(buffer) - 1; i >= 0; i--)//因为要逆序打印到控制台，所以i要-1，丢掉'\0'
		{
			if (buffer[i] < 0)//利用汉字编码为负数的特点，判断是中文还是英文
			{
				fprintf(fpwrite, "%c%c", buffer[i - 1], buffer[i]);//中文同时输出两个字符，避免乱码
				i--;
			}
			else
				fprintf(fpwrite, "%c", buffer[i]);
		}
		fclose(fpwrite);
		puts("完成！");
		printf("\n\n如果乱码，请把文件另存为ANSI格式或在“-p”参数后面添加“-u”重试。\n");
	}



	//把一个文本文件的内容添加到另一个文本文件的结尾处
	if (strcmp(parameter, "ae") == 0 && argv[3] != NULL)
	{
		if ((err = fopen_s(&fpwrite, argv[4], "a")) != 0)
		{
			perror("打开源文件失败");
			exit(EXIT_FAILURE);
		}

		while ((ch = fgetc(fpread)) != EOF)
			fputc(ch, fpwrite);

		fclose(fpread);
		fclose(fpwrite);
		puts("完成！");
		printf("\n\n如果乱码，请查看两个文件编码是否一致。\n");
	}

	system("pause");
	return 0;
}



//获取文件大小
long Get_File_Size(const char* fileName)
{
	long file_size;

	if ((err = fopen_s(&fpread, fileName, "rb")) != 0)//打开文件
	{
		perror("\n打开文件失败");
		exit(EXIT_FAILURE);
	}

	fseek(fpread, 0, SEEK_END); //将文件位置指针置于文件结尾 
	file_size = ftell(fpread);  //算出结尾与开头的字节偏移量

	fclose(fpread);
	return file_size;
}



//将大小写字母转换为小大写字母
void Letter(char* fileName, int mode)
{
	int ch, i = 0;
	char temp_file[MAX_PATH] = { 0 };


	strcpy_s(temp_file, MAX_PATH, fileName);
	strcat_s(temp_file, MAX_PATH, ".temp");


	if ((err = fopen_s(&fpread, fileName, "r")) != 0)//打开文件
	{
		perror("\n打开文件失败");
		exit(EXIT_FAILURE);
	}
	if ((err = fopen_s(&fpwrite, temp_file, "w")) != 0)
	{
		perror("\n创建临时文件失败");
		exit(EXIT_FAILURE);
	}

	while ((ch = fgetc(fpread)) != EOF)//读取文件内容并判断是否为大写或小写字母
	{
		if (mode == 1)//判断模式
		{
			if ((ch >= 'a' && ch <= 'z'))
			{
				ch -= 32;//将小写字母转换为大写
				i++;
			}
		}
		else
		{
			if (ch >= 'A' && ch <= 'Z')
			{
				ch += 32;//将大写字母转为小写字母
				i++;
			}
		}

		fputc(ch, fpwrite);//输出到文件
	}

	fclose(fpread);
	fclose(fpwrite);

	if (remove(fileName) || rename(temp_file, fileName))//删除源文件并把临时文件更名为源文件
	{
		printf("\n转换失败！\n");
		remove(temp_file);
		exit(EXIT_FAILURE);
	}

	if (mode == 1)
		printf("\n总计：共转换%d个小写字符。\n", i);
	else
		printf("\n总计：共转换%d个大写字符。\n", i);

}



//读取文件内容
int Reverse_output(const char* fileName, char* buffer, long file_size, int mode, int* total, int* line)
{
	int i = 0, L = 0, C_char = 0, E_char = 0;
	char ch;

	if (fopen_s(&fpread, fileName, "r") != 0)//打开文件
	{
		perror("\n打开文件失败");
		exit(EXIT_FAILURE);
	}

	char* buff = (char*)calloc(file_size, sizeof(char));
	
	if (IsUnicodeText(fileName, file_size) == IS_UNICODE_TEXT)
	{
		;
	}

	do
	{
		ch = fgetc(fpread);
		if (ch == -1)
			continue;

		if (ch == '\n')//统计行数
			L++;

		if (Judging_Zh(ch) == -1)    //判断是否为中文
			C_char++;			     //是中文	
		else if (Judging_Zh(ch) == 0)//不是中文
			E_char++;

		buff[i++] = ch;
	} while (ch != EOF);


	if (mode == 1)//判断是将文件内容转为gbk编码还是不转
	{
		strcpy_s(buffer, file_size, buff);
		*total = E_char + (C_char / 2);//统计总字数，gbk编码汉字占两个字节
	}
	else
	{
		utf_to_gbk(buff, buffer);
		*total = E_char + (C_char / 3);//统计总字数，utf-8编码汉字占三个字节
	}

	fclose(fpread);
	free(buff);

	*line = L;
	return i;//返回读取的字符数
}



int IsUnicodeText(char* fileName, size_t size)
{
	FILE* fp = NULL;
	int retVal;

	if ((err = fopen_s(&fp, fileName, "r")) != 0) // 打开文件
	{
		perror("\n打开文件失败");
		exit(EXIT_FAILURE);
	}

	char* buff = (char*)calloc(size, sizeof(char));
	fread_s(buff, size * sizeof(char), sizeof(char), size, fp);
	retVal = IsTextUnicode(buff, size * sizeof(char), NULL);// 判断是否为unicode字符，是返回1
	
	free(buff);
	fclose(fp);

	return retVal;
}


//判断中文字符
int Judging_Zh(char ch)
{
	return (ch >> 8) == 0 ? 0 : -1;//将ch右移8位并判断,是中文返回-1
}



//UTF-8编码转GBK
void utf_to_gbk(char* utf_str, char* gbk_str)
{
	int Wlen = MultiByteToWideChar(CP_UTF8, 0, utf_str, -1, NULL, 0);       //求需求的宽字符数大小
	wchar_t* UC_str = (wchar_t*)malloc(Wlen * sizeof(wchar_t));

	Wlen = MultiByteToWideChar(CP_UTF8, 0, utf_str, -1, UC_str, Wlen);        //将utf-8编码转换成unicode编码
	Wlen = WideCharToMultiByte(CP_ACP, 0, UC_str, -1, NULL, 0, NULL, 0);	  //求转换需要的字节数
	Wlen = WideCharToMultiByte(CP_ACP, 0, UC_str, -1, gbk_str, Wlen, NULL, 0);//将unicode编码转换成gbk编码

	free(UC_str);//释放内存
}


//获取当前可用内存大小
long long CetCurrentMemory(void)
{
	MEMORYSTATUSEX memoryStatus;
	memoryStatus.dwLength = sizeof(memoryStatus);

	if (!GlobalMemoryStatusEx(&memoryStatus))//获取当前可用的物理内存
	{
		int i = MessageBox(NULL, TEXT("获取当前可用内存失败，是否继续复制（可能导致电脑死机、蓝屏）？"), TEXT("错误"), MB_YESNO | MB_ICONERROR);
		if (i == IDYES)
			return 104857600;//100MB
		else
			exit(EXIT_FAILURE);
	}
	return memoryStatus.ullAvailPhys;
}



//输出提示
void Tip(int argc, const char* parameter)
{
	if (argc == 2)
	{
		if (strcmp(parameter, "p") == 0)
			printf("格式：文件处理.exe -p <文件路径>\n例如：文件处理.exe -p C:\\test.txt\n\n");

		else if (strcmp(parameter, "c") == 0)
			printf("格式：文件处理.exe -c <源文件路径> <目标文件路径>\n例如：文件处理.exe -c C:\\test.txt H:\\test.txt\n\n");

		else if (strcmp(parameter, "s") == 0)
			printf("格式：文件处理.exe -s <文件路径> \n例如：文件处理.exe -s C:\\test.iso\n\n");

		else if (strcmp(parameter, "u") == 0)
			printf("格式：文件处理.exe -u <文件路径> \n例如：文件处理.exe -u C:\\test.txt\n\n");

		else if (strcmp(parameter, "w") == 0)
			printf("格式：文件处理.exe -w <文件路径> \n例如：文件处理.exe -w C:\\test.txt\n\n");

		else
			printf("\n没有这个命令！\n\n");
		exit(EXIT_SUCCESS);
	}
	else if (argc == 3)
	{
		if (strcmp(parameter, "pu") == 0)
			printf("格式：文件处理.exe -p -u <文件路径>\n例如：文件处理.exe -p -u C:\\test.txt\n\n");

		else if (strcmp(parameter, "pr") == 0)
			printf("格式：文件处理.exe -p  -r <文件路径>\n例如：文件处理.exe -p -r C:\\test.txt\n\n");

		else if (strcmp(parameter, "pe") == 0)
			printf("格式：文件处理.exe -p -e <文件路径> <字符>\n例如：文件处理.exe -p -e C:\\test.txt Hello\n\n");

		else if (strcmp(parameter, "la") == 0)
			printf("格式：文件处理.exe -l -a <文件路径>\n例如：文件处理.exe -l -a C:\\test.txt\n\n");

		else if (strcmp(parameter, "pl") == 0)
			printf("格式：文件处理.exe -p -l <文件路径1> <文件路径2>\n例如：文件处理.exe -p -l C:\\test.txt H:\\text.txt\n\n");

		else if (strcmp(parameter, "rt") == 0)
			printf("格式：文件处理.exe -r -t <文件路径1> <文件路径2>\n例如：文件处理.exe -r -t C:\\test.txt H:\\text.txt\n\n");

		else if (strcmp(parameter, "ae") == 0)
			printf("格式：文件处理.exe -a -e <文件路径1> <文件路径2>\n例如：文件处理.exe -a -e C:\\test.txt  H:\\text.txt\n\n");
	}
	else if (argc == 4)
	{
		if (strcmp(parameter, "pru") == 0)
			printf("格式：文件处理.exe -p  -r -u <文件路径>\n例如：文件处理.exe -p -r -u C:\\test.txt\n\n");

		else if (strcmp(parameter, "peu") == 0)
			printf("格式：文件处理.exe -p -e -u <文件路径> <字符>\n例如：文件处理.exe -p -e -u C:\\test.txt Hello\n\n");

		else if (strcmp(parameter, "plu") == 0)
			printf("格式：文件处理.exe -p -l -u <文件路径1> <文件路径2>\n例如：文件处理.exe -p -l -u C:\\test.txt H:\\text.txt\n\n");

		else if (strcmp(parameter, "rtu") == 0)
			printf("格式：文件处理.exe -r -t -u <文件路径1> <文件路径2>\n例如：文件处理.exe -r -t -u C:\\test.txt H:\\text.txt\n\n");
	}
}