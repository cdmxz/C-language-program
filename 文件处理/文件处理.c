#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <string.h>
#include <io.h>

long long get_file_size(const char* fileName);    //获取文件大小
void to_lower_or_upper(char* fileName, int mode); //将大或小写字母转换为小或大写字母
void read_file(const char* fileName, char* buffer, size_t fileSize, int mode, int* totalChar, int* line);//读取文件内容
int  utf_to_gbk(char* utf_str, char* gbk_str);//把utf-8编码转为gbk编码
int  is_chinese_char(char ch);					 //判断中文字符
void output_tip(int argc, const char* parameter);	 //输出提示
long long get_memory(void);            //获取当前可用内存大小
void print_file_size(double fileSize);	// 输出文件大小
void del_line_number(char* str);       // 删除行号
void my_strcpy_s(char* dest, size_t size, const char* source);
void trim(char* str);

#define IS_CHINESE_CHAR 1 // 是中文字符
#define TO_LOWER 0        // 大写字母转小写
#define TO_UPPER 1        // 小写字母转大写
#define TO_GBK_CODE 2     // 转为GBK编码
#define IS_NUMBER 1

int main(int argc, char* argv[])
{
    char fileName[MAX_PATH], destFileName[MAX_PATH];
    char parameter[5] = { 0 }, character[MAX_PATH];
    long long fileSize = 0, totalSize = 0, i = 0, currentMemory;
    int ch, mode = 1, line = 0, totalChar = 0;
    FILE* fpread, * fpwrite;
    size_t len;

    if (argc == 1)
    {
        printf("\n编译日期：2020-08-21\n");
        printf("用法：文件处理.exe <命令> <文件路径1> [ <文件路径2>可选 ] ... \n\n");
        printf("说明：                                      命令：\n");
        printf(
            "1、 打印文件内容到控制台                             -p    或 -p -u\n"
            "2、 逆序打印文件内容到控制台                         -p -r 或 -p -r -u\n"
            "3、 复制文件                                         -c	\n"
            "4、 复制文件（大内存模式）                           -c -b \n"
            "5、 计算文件大小                                     -s    \n"
            "6、 只打印文本文件中给定字符的一行                   -p -e 或 -p -e -u\n"
            "7、 把文本中的所有字母转换为大写                     -u    \n"
            "8、 把文本中的所有字母转换为小写                     -l    \n"
            "9、 计算文本文件的行数，在每一行前添加行号           -l -a \n"
            "10、计算文本文件的行数，先删除每一行前的行号再添加   -l -a -d\n"
            "11、计算文本文件的行数，并删除每一行前的行号         -l -d\n"
            "12、打印两个文本文件不同的一行和行号                 -p -l 或 -p -l -u\n"
            "13、把一个文本文件的内容逆序输出到另一个文本文件     -r -t\n"
            "14、把一个文本文件的内容添加到另一个文本文件的结尾处 -a -e\n\n");
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

    // 输出提示
    output_tip(argc, parameter);


    //打印文件内容到控制台
    if ((strcmp(parameter, "p") == 0 || strcmp(parameter, "pu") == 0))
    {
        if (strcmp(parameter, "pu") == 0)
        {
            my_strcpy_s(fileName, MAX_PATH, argv[3]);
            mode = TO_GBK_CODE;
        }
        else
        {
            my_strcpy_s(fileName, MAX_PATH, argv[2]);
        }


        fileSize = get_file_size(fileName);//获取文件大小
        char* buffer = (char*)calloc((size_t)fileSize + 1, sizeof(char));//分配缓存
        read_file(fileName, buffer, (size_t)fileSize + 1, mode, &totalChar, &line);//读取文件内容
        puts(buffer);
        free(buffer);//释放内存
        buffer = NULL;

        printf("\n总计：%d行%d字符。\n", line + 1, totalChar);
        printf("如果乱码，请把文件另存为ANSI格式或在“-p”参数后面添加“-u”重试。\n");
    }


    //逆序打印文件内容到控制台
    else if ((strcmp(parameter, "pr") == 0 || strcmp(parameter, "pru") == 0))
    {
        if (strcmp(parameter, "pru") == 0)//判断输入的参数
        {
            my_strcpy_s(fileName, MAX_PATH, argv[4]);
            mode = TO_GBK_CODE;
        }
        else
        {
            my_strcpy_s(fileName, MAX_PATH, argv[3]);
        }


        fileSize = get_file_size(fileName);
        char* buffer = calloc((size_t)fileSize + 1, sizeof(char));
        read_file(fileName, buffer, (size_t)fileSize + 1, mode, &totalChar, &line);

        i = strlen(buffer) - 1;
        //因为要逆序打印到控制台，相当于把字符串倒过来，末尾的'\0'在字符串开头。所以i要-1，丢掉'\0'。
        for (; i >= 0; i--)
        {
            if (is_chinese_char(buffer[i]) == IS_CHINESE_CHAR)//判断是中文还是英文
            {
                printf("%c%c", buffer[i - 1], buffer[i]);//中文同时输出两个字符
                i--;
            }
            else
                printf("%c", buffer[i]);
        }
        free(buffer);
        buffer = NULL;

        printf("\n总计：%d行%d字符。\n", line + 1, totalChar);
        printf("如果乱码，请把文件另存为ANSI格式或在“-p”参数后面添加“-u”重试。\n");
    }


    //复制文件
    else if (strcmp(parameter, "c") == 0 || strcmp(parameter, "cb") == 0)
    {
        i = 1;
        size_t buffSize;
        char c;

        if (strcmp(parameter, "cb") == 0)
        {	// 大内存模式

            my_strcpy_s(fileName, MAX_PATH, argv[3]);
            my_strcpy_s(destFileName, MAX_PATH, argv[4]);
            currentMemory = get_memory();
        }
        else
        {
            my_strcpy_s(fileName, MAX_PATH, argv[2]);
            my_strcpy_s(destFileName, MAX_PATH, argv[3]);
            currentMemory = 1048576;
        }

        fileSize = get_file_size(fileName);//获取文件大小
        if (strcmp(parameter, "cb") == 0)
            buffSize = (size_t)fileSize + 1;
        else
            buffSize = (size_t)currentMemory + 1;

        if (fopen_s(&fpread, fileName, "rb") != 0)//以读的方式打开文件
        {
            perror("\n打开源文件失败");
            exit(EXIT_FAILURE);
        }

        // 目标文件已存在
        while (_access(destFileName, 0) == 0)
        {
            printf("\n目标文件已存在，是否覆盖？（y/n）");
            rewind(stdin);
            scanf_s("%c", &c, 1);
            if (c == 'y' || c == 'Y')
                break;
            else if (c == 'n' || c == 'N')
                exit(EXIT_FAILURE);
        }

        if (fopen_s(&fpwrite, destFileName, "wb") != 0)//以写的方式打开文件
        {
            perror("\n打开目标文件失败");
            exit(EXIT_FAILURE);
        }

        while (i++ && currentMemory != 1048576)	// 如果是大内存模式（currentMemory != 1048576 为true）
        {
            buffSize /= (size_t)i;//求出复制文件时malloc分配的内存空间大小

            //将当前文件大小除以 i，并判断是否大于当前可用内存，加500MB（524288000 byte）是为了给系统留500MB内存空间
            if (((long long)buffSize + 524288000LL) <= currentMemory && buffSize < MAXUINT32 && buffSize <= 1073741824)
                //fileSize <= 4294967295 是为了确保因类型不同造成错误。fileSize是long long类型，calloc和fread需要size_t(unsigned int)型数据，4294967295是unsigned int表示的最大范围
                break;
        }

        char* buffer = (char*)calloc(buffSize, sizeof(char));
        if (buffer == NULL)//如果空间不足
        {
            printf("\n内存空间不足！");
            exit(EXIT_FAILURE);
        }

        printf("\n正在复制：%s\n", fileName);

        while ((len = fread(buffer, sizeof(char), buffSize, fpread)) > 0)//复制文件
        {
            totalSize += len;
            double currentProgress = (double)totalSize / (double)fileSize;   // 当前进度
            for (int j = 1; j <= (int)(currentProgress * 100 / 2); j++)      // 根据进度打印“>”
                printf(">");
            printf("%.2lf%%\r", currentProgress * 100);

            if (fwrite(buffer, sizeof(char), len, fpwrite) < len)
            {
                printf("\n目标磁盘空间不足！");
                exit(EXIT_FAILURE);
            }
        }

        fclose(fpread);
        fclose(fpwrite);
        free(buffer);
        buffer = NULL;

        printf("\n复制成功！");
        print_file_size((double)totalSize);
    }


    //计算文件大小
    else if (strcmp(parameter, "s") == 0)
    {
        print_file_size((double)get_file_size(argv[2]));
    }


    //打印给定字符的一行
    else if ((strcmp(parameter, "pe") == 0 || strcmp(parameter, "peu") == 0))
    {
        //判断命令参数
        if (strcmp(parameter, "peu") == 0)
        {
            my_strcpy_s(fileName, MAX_PATH, argv[4]);
            my_strcpy_s(character, MAX_PATH, argv[5]);
        }
        else
        {
            my_strcpy_s(fileName, MAX_PATH, argv[3]);
            my_strcpy_s(character, MAX_PATH, argv[4]);
        }

        //计算文件大小
        fileSize = get_file_size(fileName);
        //分配缓存
        char* buffer = (char*)calloc((size_t)fileSize + 1, sizeof(char));

        if (fopen_s(&fpread, fileName, "r") != 0)
        {
            perror("\n打开文件失败");
            exit(EXIT_FAILURE);
        }

        while (!feof(fpread))
        {
            memset(buffer, 0, (size_t)fileSize);
            fgets(buffer, (int)fileSize, fpread);//从文件读取一行

            if (strcmp(parameter, "peu") == 0)
            {
                size_t gbkSize = utf_to_gbk(buffer, NULL);
                char* gbk = (char*)calloc(gbkSize, sizeof(char));
                utf_to_gbk(buffer, gbk);//将utf-8转换为gbk编码
                if (strstr(gbk, character) != NULL)//查找字符
                    printf("%s", gbk);//打印

                free(gbk);
                gbk = NULL;
            }
            else
            {
                if (strstr(buffer, character) != NULL)//查找字符
                    printf("%s", buffer);//打印
            }
        }

        fclose(fpread);
        free(buffer);
        buffer = NULL;
        printf("\n\n如果乱码，请把文件另存为ANSI格式或在“-p”参数后面添加“-u”重试。\n");
    }


    //把文本中的所有字母转换为大写
    else if (strcmp(parameter, "u") == 0)
    {
        to_lower_or_upper(argv[2], TO_UPPER);
    }


    //把文本中的所有字母转换为小写
    else if (strcmp(parameter, "l") == 0)
    {
        to_lower_or_upper(argv[2], TO_LOWER);
    }


    //计算文本文件的行数并在每一行前添加行号
    else if (strcmp(parameter, "la") == 0 || strcmp(parameter, "lad") == 0 || strcmp(parameter, "ld") == 0)
    {
        line = 0;

        if (strcmp(parameter, "lad") == 0)
            my_strcpy_s(fileName, MAX_PATH, argv[4]);
        else
            my_strcpy_s(fileName, MAX_PATH, argv[3]);

        fileSize = get_file_size(fileName);
        char* buffer = (char*)malloc((size_t)fileSize + 1);
        char temp_file[MAX_PATH];
        sprintf_s(temp_file, MAX_PATH, "%s.tmp", fileName);

        if (fopen_s(&fpread, fileName, "r") != 0)
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
            fgets(buffer, (int)fileSize, fpread);//从文件读取一行
            if (strcmp(parameter, "lad") == 0)// 删除每一行前的行号，再重新添加
            {
                del_line_number(buffer);
                fprintf(fpwrite, "%02d  %s", ++line, buffer);// 添加行号并输出到临时文件
            }
            else if (strcmp(parameter, "ld") == 0)
            {
                del_line_number(buffer);
                fprintf(fpwrite, "%s", buffer);// 不添加行号
            }
            else
                fprintf(fpwrite, "%02d  %s", ++line, buffer);// 添加行号并输出到临时文件
        }

        fclose(fpread);
        fclose(fpwrite);

        if (remove(fileName) || rename(temp_file, fileName))//删除源文件并把临时文件重命名为源文件
        {
            printf("\n失败！\n");
            remove(temp_file);
            exit(EXIT_FAILURE);
        }
        printf("\n总计：%d行\n", line);
    }


    //把两个文本文件不同的一行打印并输出是哪一行
    else if ((strcmp(parameter, "pl") == 0 || strcmp(parameter, "plu") == 0) && argv[3] != NULL)
    {
        char file1[MAX_PATH], file2[MAX_PATH];
        FILE* fp1, * fp2;

        if (strcmp(parameter, "plu") == 0)
        {
            my_strcpy_s(file1, MAX_PATH, argv[4]);
            my_strcpy_s(file2, MAX_PATH, argv[5]);
        }
        else
        {
            my_strcpy_s(file1, MAX_PATH, argv[3]);
            my_strcpy_s(file2, MAX_PATH, argv[4]);
        }

        //获取文件大小
        long long file1_size = get_file_size(file1);
        long long file2_size = get_file_size(file2);
        //分配缓存
        char* buffer1 = (char*)calloc((size_t)file1_size + 1, sizeof(char));
        char* buffer2 = (char*)calloc((size_t)file2_size + 1, sizeof(char));

        if (fopen_s(&fp1, file1, "r") != 0)
        {
            perror("\n打开文件1失败");
            exit(EXIT_FAILURE);
        }
        if (fopen_s(&fp2, file2, "r") != 0)
        {
            perror("\n打开文件2失败");
            exit(EXIT_FAILURE);
        }

        if (!fp1 || !fp2 || !buffer1 || !buffer2)
        {
            printf("\n错误！");
            exit(EXIT_FAILURE);
        }

        while (!feof(fp1) || !feof(fp2))
        {
            memset(buffer1, 0, (size_t)file1_size);
            memset(buffer2, 0, (size_t)file2_size);
            fgets(buffer1, (int)file1_size, fp1);//读取一行
            fgets(buffer2, (int)file2_size, fp2);

            if (strcmp(parameter, "plu") == 0)//判断是否需要转换编码
            {
                size_t gbk1Size = utf_to_gbk(buffer1, NULL);//将utf-8转为gbk
                size_t gbk2Size = utf_to_gbk(buffer2, NULL);
                char* gbk1 = (char*)calloc(gbk1Size, sizeof(char));
                char* gbk2 = (char*)calloc(gbk2Size, sizeof(char));
                if (gbk1 == NULL || gbk2 == NULL)
                {
                    printf("\n错误！");
                    exit(EXIT_FAILURE);
                }
                utf_to_gbk(buffer1, gbk1);//将utf-8转为gbk		
                utf_to_gbk(buffer2, gbk2);

                if (strcmp(gbk1, gbk2) != 0)//比较两个文件的同一行是否一致
                {
                    if (strchr(gbk1, '\n') == 0)//判断字符串是否有换行符
                    {
                        printf("\n第%d行：\n文件一：%s\n文件二：%s", line + 1, gbk1, gbk2);//没有换行符就添加换行符打印
                        line++;
                        continue;
                    }
                    printf("\n第%d行：\n文件一：%s文件二：%s", line + 1, gbk1, gbk2);//有换行符就不添加换行符打印
                }
                line++;

                free(gbk1);
                gbk1 = NULL;
                free(gbk2);
                gbk2 = NULL;
            }
            else
            {
                if (strcmp(buffer1, buffer2) != 0)//比较两个文件的同一行是否一致
                {
                    if (strchr(buffer1, '\n') == 0)//判断字符串是否有换行符
                    {
                        printf("\n第%d行：\n文件一：%s\n文件二：%s", line + 1, buffer1, buffer2);//没有换行符就添加换行符打印
                        line++;
                        continue;
                    }
                    printf("\n第%d行：\n文件一：%s文件二：%s", line + 1, buffer1, buffer2);//有换行符就不添加换行符打印
                }
                line++;
            }
        }
        printf("\n\n如果乱码，请把文件另存为ANSI格式或在“-p”参数后面添加“-u”重试。\n");

        free(buffer1);
        buffer1 = NULL;
        free(buffer2);
        buffer2 = NULL;
        fclose(fp1);
        fclose(fp2);
    }


    //把一个文本文件倒序输出到另一个文本文件
    else if ((strcmp(parameter, "rt") == 0 || strcmp(parameter, "rtu") == 0) && argv[3] != NULL)
    {
        char s_file[MAX_PATH], d_file[MAX_PATH];

        if (strcmp(parameter, "rtu") == 0)
        {
            my_strcpy_s(s_file, MAX_PATH, argv[4]);
            my_strcpy_s(d_file, MAX_PATH, argv[5]);
            mode = TO_GBK_CODE;// 转为GBK编码
        }
        else
        {
            my_strcpy_s(s_file, MAX_PATH, argv[3]);
            my_strcpy_s(d_file, MAX_PATH, argv[4]);
        }

        fileSize = get_file_size(s_file);
        char* buffer = (char*)calloc((size_t)fileSize + 1, sizeof(char));
        if (!buffer)
        {
            printf("\n内存不足！");
            exit(EXIT_FAILURE);
        }

        read_file(s_file, buffer, (size_t)fileSize + 1, mode, &totalChar, &line);//读取文件1的内容

        if (fopen_s(&fpwrite, d_file, "w") != 0)//打开文件2
        {
            perror("打开文件2失败");
            exit(EXIT_FAILURE);
        }
        if (fpwrite == NULL || buffer == NULL)
        {
            perror("失败");
            exit(EXIT_FAILURE);
        }

        for (i = strlen(buffer) - 1; i >= 0; i--)//i要-1，丢掉'\0'
        {
            if (is_chinese_char(buffer[i]) == IS_CHINESE_CHAR)//判断是中文还是英文
            {
                fprintf(fpwrite, "%c%c", buffer[i - 1], buffer[i]);//中文同时输出两个字符，避免乱码
                i--;
            }
            else
                fprintf(fpwrite, "%c", buffer[i]);
        }
        fclose(fpwrite);
        free(buffer);
        buffer = NULL;

        puts("完成！");
        printf("\n\n如果乱码，请把文件另存为ANSI格式或在“-p”参数后面添加“-u”重试。\n");
    }


    //把一个文本文件的内容添加到另一个文本文件的结尾处
    else if (strcmp(parameter, "ae") == 0 && argv[3] != NULL)
    {
        if (fopen_s(&fpread, argv[3], "r") != 0)
        {
            perror("打开源文件失败");
            exit(EXIT_FAILURE);
        }

        if (fopen_s(&fpwrite, argv[4], "a") != 0)
        {
            perror("打开目标文件失败");
            exit(EXIT_FAILURE);
        }
        if (fpread == NULL || fpwrite == NULL)
        {
            printf("失败");
            exit(EXIT_FAILURE);
        }

        while ((ch = fgetc(fpread)) != EOF)
            fputc(ch, fpwrite);

        fclose(fpread);
        fclose(fpwrite);
        puts("完成！");
        printf("\n\n如果乱码，请查看两个文件编码是否一致。\n");
    }


    else
        printf("\n没有这个命令！\n\n");
    //system("pause");
    return 0;
}



//获取文件大小
long long get_file_size(const char* fileName)
{
    long long fileSize;
    FILE* fpread;

    if (fopen_s(&fpread, fileName, "rb") != 0)//打开文件
    {
        perror("\n打开文件失败");
        exit(EXIT_FAILURE);
    }

    if (fpread == NULL)
        return -1;

    fseek(fpread, 0, SEEK_END);    //将文件位置指针置于文件结尾
    fileSize = _ftelli64(fpread);  //算出结尾与开头的字节偏移量

    fclose(fpread);
    return fileSize;
}



//将大或小写字母转换为小或大写字母
void to_lower_or_upper(char* fileName, int mode)
{
    FILE* fpread, * fpwrite;
    int ch, i = 0;
    char temp_file[MAX_PATH] = { 0 };


    strcpy_s(temp_file, MAX_PATH, fileName);
    strcat_s(temp_file, MAX_PATH, ".temp");


    if (fopen_s(&fpread, fileName, "r") != 0)//打开文件
    {
        perror("\n打开文件失败");
        exit(EXIT_FAILURE);
    }
    if (fopen_s(&fpwrite, temp_file, "w") != 0)
    {
        perror("\n创建临时文件失败");
        exit(EXIT_FAILURE);
    }

    while ((ch = fgetc(fpread)) != EOF)//读取文件内容并判断是否为大写或小写字母
    {
        if (mode == TO_UPPER)//判断模式
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

    if (mode == TO_UPPER)
        printf("\n总计：共转换%d个小写字符。\n", i);
    else
        printf("\n总计：共转换%d个大写字符。\n", i);


}



//读取文件内容
void read_file(const char* fileName, char* buffer, size_t fileSize, int mode, int* totalChar, int* line)
{
    int i = 0, C_char = 0, E_char = 0;
    char ch;
    FILE* fpread;
    *line = 0;

    if (fileName == NULL)
    {
        perror("\n文件路径无效");
        exit(EXIT_FAILURE);
    }

    if (fopen_s(&fpread, fileName, "r") != 0)
    {
        perror("\n打开文件失败");
        exit(EXIT_FAILURE);
    }

    char* buff = (char*)calloc(fileSize, sizeof(char));
    if (fpread == NULL || buff == NULL)
        return;

    do
    {
        if ((ch = (char)fgetc(fpread)) == EOF)
            continue;

        if (ch == '\n' && line != NULL)//统计行数
            (*line)++;

        buff[i++] = ch;

        if (ch == '\n')
            continue;
        else if (is_chinese_char(ch) == IS_CHINESE_CHAR) //判断是否为中文
            C_char++;			     //是中文
        else if (is_chinese_char(ch) == 0)//不是中文
            E_char++;
    } while (ch != EOF);

    if (mode == TO_GBK_CODE)//判断是将文件内容转为gbk编码还是不转
    {
        utf_to_gbk(buff, buffer);
        *totalChar = E_char + (C_char / 3);//统计总字数，utf-8编码汉字占三个字节
    }
    else
    {
        my_strcpy_s(buffer, fileSize, buff);
        *totalChar = E_char + (C_char / 2);//统计总字数，gbk编码汉字占两个字节
    }

    fclose(fpread);
    free(buff);
    buff = NULL;

    //return i;//返回读取的字符数
}


//判断中文字符
int is_chinese_char(char ch)
{
    return (ch >> 8) == 0 ? 0 : IS_CHINESE_CHAR;//将ch右移8位并判断,是中文返回 1
}


//UTF-8编码转GBK
int utf_to_gbk(char* utf_str, char* gbk_str)
{
    int len = MultiByteToWideChar(CP_UTF8, 0, utf_str, -1, NULL, 0);       //求需求的宽字符数大小
    wchar_t* UC_str = (wchar_t*)malloc(len * sizeof(wchar_t));
    if (!UC_str)
        return 1;
    len = MultiByteToWideChar(CP_UTF8, 0, utf_str, -1, UC_str, len);        //将utf-8编码转换成unicode编码
    len = WideCharToMultiByte(CP_ACP, 0, UC_str, -1, NULL, 0, NULL, 0);	    //求转换需要的字节数
    if (gbk_str != NULL)// 如果gbk_str为空，返回需要的缓冲区大小
        len = WideCharToMultiByte(CP_ACP, 0, UC_str, -1, gbk_str, len, NULL, 0);//将unicode编码转换成gbk编码

    free(UC_str);//释放内存
    return len;
}


//获取当前可用内存大小
long long get_memory(void)
{
    MEMORYSTATUSEX memoryStatus;
    memoryStatus.dwLength = sizeof(memoryStatus);

    if (!GlobalMemoryStatusEx(&memoryStatus))//获取当前可用的物理内存
    {	// 获取当前可用内存失败
        return 1048576;//1MB
    }
    return memoryStatus.ullAvailPhys;
}


// 输出文件大小
void print_file_size(double fileSize)
{
    if (fileSize < 1024.0)
        printf("\n大小：%.0lf字节\n", fileSize);
    else if (fileSize < 1048576.0)
        printf("\n大小：%.2lfKB（%.0lf字节）\n", fileSize / 1024.0, fileSize);
    else if (fileSize < 1073741824.0)
        printf("\n大小：%.2lfMB（%.0lf字节）\n", fileSize / 1048576.0, fileSize);
    else
        printf("\n大小：%.2lfGB（%.0lf字节）\n", fileSize / 1073741824.0, fileSize);
}


// 删除行号
void del_line_number(char* str)
{
    char* p = str;
    char* tmp = NULL;
    int num, retVal;
    size_t i, j;

    tmp = calloc(strlen(str) + 1, sizeof(char));
    if (!tmp)
        return;
    trim(str);
    for (i = 0; *p != '\0' && *p != ' '; tmp++, p++, i++)
        *tmp = *p;
    // 不是数字
    retVal = sscanf_s(tmp - i, "%d", &num);
    free(tmp - i);
    if (retVal == EOF || retVal == 0)
        return;

    // 覆盖前面的数字和空格
    for (j = 0, i += 1; i < strlen(str) + 1; i++, j++)
        str[j] = str[i];
}

// 删除字符串开头的空白
void trim(char* str)
{
    while (str[0] == ' ')
    {
        for (size_t j = 0; j < strlen(str); j++)
        {
            str[j] = str[j + 1];
        }
    }
}

//输出提示
void output_tip(int argc, const char* parameter)
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

        else if (strcmp(parameter, "l") == 0)
            printf("格式：文件处理.exe -w <文件路径> \n例如：文件处理.exe -l C:\\test.txt\n\n");
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

        else if (strcmp(parameter, "ld") == 0)
            printf("格式：文件处理.exe -l -d <文件路径>\n例如：文件处理.exe -l -a C:\\test.txt\n\n");

        else if (strcmp(parameter, "pl") == 0)
            printf("格式：文件处理.exe -p -l <文件路径1> <文件路径2>\n例如：文件处理.exe -p -l C:\\test.txt H:\\text.txt\n\n");

        else if (strcmp(parameter, "rt") == 0)
            printf("格式：文件处理.exe -r -t <文件路径1> <文件路径2>\n例如：文件处理.exe -r -t C:\\test.txt H:\\text.txt\n\n");

        else if (strcmp(parameter, "ae") == 0)
            printf("格式：文件处理.exe -a -e <文件路径1> <文件路径2>\n例如：文件处理.exe -a -e C:\\test.txt  H:\\text.txt\n\n");

        else if (strcmp(parameter, "cb") == 0)
            printf("格式：文件处理.exe -c -b <源文件路径> <目标文件路径>\n例如：文件处理.exe -c -b C:\\test.txt H:\\test.txt\n\n");
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

        else if (strcmp(parameter, "lad") == 0)
            printf("格式：文件处理.exe -l -a -d <文件路径>\n例如：文件处理.exe -l -a -d C:\\test.txt\n\n");
    }
}


void my_strcpy_s(char* dest, size_t size, const char* source)
{
    if (source == NULL)
    {
        exit(EXIT_FAILURE);
    }
    strcpy_s(dest, size, source);
}