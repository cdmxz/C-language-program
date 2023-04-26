#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <string.h>
#include <io.h>

long long get_file_size(const char* fileName);    //��ȡ�ļ���С
void to_lower_or_upper(char* fileName, int mode); //�����Сд��ĸת��ΪС���д��ĸ
void read_file(const char* fileName, char* buffer, size_t fileSize, int mode, int* totalChar, int* line);//��ȡ�ļ�����
int  utf_to_gbk(char* utf_str, char* gbk_str);//��utf-8����תΪgbk����
int  is_chinese_char(char ch);					 //�ж������ַ�
void output_tip(int argc, const char* parameter);	 //�����ʾ
long long get_memory(void);            //��ȡ��ǰ�����ڴ��С
void print_file_size(double fileSize);	// ����ļ���С
void del_line_number(char* str);       // ɾ���к�
void my_strcpy_s(char* dest, size_t size, const char* source);
void trim(char* str);

#define IS_CHINESE_CHAR 1 // �������ַ�
#define TO_LOWER 0        // ��д��ĸתСд
#define TO_UPPER 1        // Сд��ĸת��д
#define TO_GBK_CODE 2     // תΪGBK����
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
        printf("\n�������ڣ�2020-08-21\n");
        printf("�÷����ļ�����.exe <����> <�ļ�·��1> [ <�ļ�·��2>��ѡ ] ... \n\n");
        printf("˵����                                      ���\n");
        printf(
            "1�� ��ӡ�ļ����ݵ�����̨                             -p    �� -p -u\n"
            "2�� �����ӡ�ļ����ݵ�����̨                         -p -r �� -p -r -u\n"
            "3�� �����ļ�                                         -c	\n"
            "4�� �����ļ������ڴ�ģʽ��                           -c -b \n"
            "5�� �����ļ���С                                     -s    \n"
            "6�� ֻ��ӡ�ı��ļ��и����ַ���һ��                   -p -e �� -p -e -u\n"
            "7�� ���ı��е�������ĸת��Ϊ��д                     -u    \n"
            "8�� ���ı��е�������ĸת��ΪСд                     -l    \n"
            "9�� �����ı��ļ�����������ÿһ��ǰ����к�           -l -a \n"
            "10�������ı��ļ�����������ɾ��ÿһ��ǰ���к������   -l -a -d\n"
            "11�������ı��ļ�����������ɾ��ÿһ��ǰ���к�         -l -d\n"
            "12����ӡ�����ı��ļ���ͬ��һ�к��к�                 -p -l �� -p -l -u\n"
            "13����һ���ı��ļ������������������һ���ı��ļ�     -r -t\n"
            "14����һ���ı��ļ���������ӵ���һ���ı��ļ��Ľ�β�� -a -e\n\n");
        return 0;
    }


    //������ȥ����-����д������
    if (*argv[1] == '-')//�жϵ�һ������
    {
        parameter[0] = *++argv[1];

        if (argc > 2)//�ж��Ƿ��еڶ�������
        {
            if (*argv[2] == '-')//�жϵڶ�������
            {
                parameter[1] = *++argv[2];

                if (argv[3] != NULL)//�ж��Ƿ��е���������
                {
                    if (*argv[3] == '-')//�жϵ���������
                        parameter[2] = *++argv[3];
                }
            }
        }
    }

    // �����ʾ
    output_tip(argc, parameter);


    //��ӡ�ļ����ݵ�����̨
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


        fileSize = get_file_size(fileName);//��ȡ�ļ���С
        char* buffer = (char*)calloc((size_t)fileSize + 1, sizeof(char));//���仺��
        read_file(fileName, buffer, (size_t)fileSize + 1, mode, &totalChar, &line);//��ȡ�ļ�����
        puts(buffer);
        free(buffer);//�ͷ��ڴ�
        buffer = NULL;

        printf("\n�ܼƣ�%d��%d�ַ���\n", line + 1, totalChar);
        printf("������룬����ļ����ΪANSI��ʽ���ڡ�-p������������ӡ�-u�����ԡ�\n");
    }


    //�����ӡ�ļ����ݵ�����̨
    else if ((strcmp(parameter, "pr") == 0 || strcmp(parameter, "pru") == 0))
    {
        if (strcmp(parameter, "pru") == 0)//�ж�����Ĳ���
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
        //��ΪҪ�����ӡ������̨���൱�ڰ��ַ�����������ĩβ��'\0'���ַ�����ͷ������iҪ-1������'\0'��
        for (; i >= 0; i--)
        {
            if (is_chinese_char(buffer[i]) == IS_CHINESE_CHAR)//�ж������Ļ���Ӣ��
            {
                printf("%c%c", buffer[i - 1], buffer[i]);//����ͬʱ��������ַ�
                i--;
            }
            else
                printf("%c", buffer[i]);
        }
        free(buffer);
        buffer = NULL;

        printf("\n�ܼƣ�%d��%d�ַ���\n", line + 1, totalChar);
        printf("������룬����ļ����ΪANSI��ʽ���ڡ�-p������������ӡ�-u�����ԡ�\n");
    }


    //�����ļ�
    else if (strcmp(parameter, "c") == 0 || strcmp(parameter, "cb") == 0)
    {
        i = 1;
        size_t buffSize;
        char c;

        if (strcmp(parameter, "cb") == 0)
        {	// ���ڴ�ģʽ

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

        fileSize = get_file_size(fileName);//��ȡ�ļ���С
        if (strcmp(parameter, "cb") == 0)
            buffSize = (size_t)fileSize + 1;
        else
            buffSize = (size_t)currentMemory + 1;

        if (fopen_s(&fpread, fileName, "rb") != 0)//�Զ��ķ�ʽ���ļ�
        {
            perror("\n��Դ�ļ�ʧ��");
            exit(EXIT_FAILURE);
        }

        // Ŀ���ļ��Ѵ���
        while (_access(destFileName, 0) == 0)
        {
            printf("\nĿ���ļ��Ѵ��ڣ��Ƿ񸲸ǣ���y/n��");
            rewind(stdin);
            scanf_s("%c", &c, 1);
            if (c == 'y' || c == 'Y')
                break;
            else if (c == 'n' || c == 'N')
                exit(EXIT_FAILURE);
        }

        if (fopen_s(&fpwrite, destFileName, "wb") != 0)//��д�ķ�ʽ���ļ�
        {
            perror("\n��Ŀ���ļ�ʧ��");
            exit(EXIT_FAILURE);
        }

        while (i++ && currentMemory != 1048576)	// ����Ǵ��ڴ�ģʽ��currentMemory != 1048576 Ϊtrue��
        {
            buffSize /= (size_t)i;//��������ļ�ʱmalloc������ڴ�ռ��С

            //����ǰ�ļ���С���� i�����ж��Ƿ���ڵ�ǰ�����ڴ棬��500MB��524288000 byte����Ϊ�˸�ϵͳ��500MB�ڴ�ռ�
            if (((long long)buffSize + 524288000LL) <= currentMemory && buffSize < MAXUINT32 && buffSize <= 1073741824)
                //fileSize <= 4294967295 ��Ϊ��ȷ�������Ͳ�ͬ��ɴ���fileSize��long long���ͣ�calloc��fread��Ҫsize_t(unsigned int)�����ݣ�4294967295��unsigned int��ʾ�����Χ
                break;
        }

        char* buffer = (char*)calloc(buffSize, sizeof(char));
        if (buffer == NULL)//����ռ䲻��
        {
            printf("\n�ڴ�ռ䲻�㣡");
            exit(EXIT_FAILURE);
        }

        printf("\n���ڸ��ƣ�%s\n", fileName);

        while ((len = fread(buffer, sizeof(char), buffSize, fpread)) > 0)//�����ļ�
        {
            totalSize += len;
            double currentProgress = (double)totalSize / (double)fileSize;   // ��ǰ����
            for (int j = 1; j <= (int)(currentProgress * 100 / 2); j++)      // ���ݽ��ȴ�ӡ��>��
                printf(">");
            printf("%.2lf%%\r", currentProgress * 100);

            if (fwrite(buffer, sizeof(char), len, fpwrite) < len)
            {
                printf("\nĿ����̿ռ䲻�㣡");
                exit(EXIT_FAILURE);
            }
        }

        fclose(fpread);
        fclose(fpwrite);
        free(buffer);
        buffer = NULL;

        printf("\n���Ƴɹ���");
        print_file_size((double)totalSize);
    }


    //�����ļ���С
    else if (strcmp(parameter, "s") == 0)
    {
        print_file_size((double)get_file_size(argv[2]));
    }


    //��ӡ�����ַ���һ��
    else if ((strcmp(parameter, "pe") == 0 || strcmp(parameter, "peu") == 0))
    {
        //�ж��������
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

        //�����ļ���С
        fileSize = get_file_size(fileName);
        //���仺��
        char* buffer = (char*)calloc((size_t)fileSize + 1, sizeof(char));

        if (fopen_s(&fpread, fileName, "r") != 0)
        {
            perror("\n���ļ�ʧ��");
            exit(EXIT_FAILURE);
        }

        while (!feof(fpread))
        {
            memset(buffer, 0, (size_t)fileSize);
            fgets(buffer, (int)fileSize, fpread);//���ļ���ȡһ��

            if (strcmp(parameter, "peu") == 0)
            {
                size_t gbkSize = utf_to_gbk(buffer, NULL);
                char* gbk = (char*)calloc(gbkSize, sizeof(char));
                utf_to_gbk(buffer, gbk);//��utf-8ת��Ϊgbk����
                if (strstr(gbk, character) != NULL)//�����ַ�
                    printf("%s", gbk);//��ӡ

                free(gbk);
                gbk = NULL;
            }
            else
            {
                if (strstr(buffer, character) != NULL)//�����ַ�
                    printf("%s", buffer);//��ӡ
            }
        }

        fclose(fpread);
        free(buffer);
        buffer = NULL;
        printf("\n\n������룬����ļ����ΪANSI��ʽ���ڡ�-p������������ӡ�-u�����ԡ�\n");
    }


    //���ı��е�������ĸת��Ϊ��д
    else if (strcmp(parameter, "u") == 0)
    {
        to_lower_or_upper(argv[2], TO_UPPER);
    }


    //���ı��е�������ĸת��ΪСд
    else if (strcmp(parameter, "l") == 0)
    {
        to_lower_or_upper(argv[2], TO_LOWER);
    }


    //�����ı��ļ�����������ÿһ��ǰ����к�
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
            perror("\n���ļ�ʧ��");
            exit(EXIT_FAILURE);
        }
        if (fopen_s(&fpwrite, temp_file, "w") != 0)
        {
            perror("\n������ʱ�ļ�ʧ��");
            exit(EXIT_FAILURE);
        }

        while (!feof(fpread))
        {
            fgets(buffer, (int)fileSize, fpread);//���ļ���ȡһ��
            if (strcmp(parameter, "lad") == 0)// ɾ��ÿһ��ǰ���кţ����������
            {
                del_line_number(buffer);
                fprintf(fpwrite, "%02d  %s", ++line, buffer);// ����кŲ��������ʱ�ļ�
            }
            else if (strcmp(parameter, "ld") == 0)
            {
                del_line_number(buffer);
                fprintf(fpwrite, "%s", buffer);// ������к�
            }
            else
                fprintf(fpwrite, "%02d  %s", ++line, buffer);// ����кŲ��������ʱ�ļ�
        }

        fclose(fpread);
        fclose(fpwrite);

        if (remove(fileName) || rename(temp_file, fileName))//ɾ��Դ�ļ�������ʱ�ļ�������ΪԴ�ļ�
        {
            printf("\nʧ�ܣ�\n");
            remove(temp_file);
            exit(EXIT_FAILURE);
        }
        printf("\n�ܼƣ�%d��\n", line);
    }


    //�������ı��ļ���ͬ��һ�д�ӡ���������һ��
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

        //��ȡ�ļ���С
        long long file1_size = get_file_size(file1);
        long long file2_size = get_file_size(file2);
        //���仺��
        char* buffer1 = (char*)calloc((size_t)file1_size + 1, sizeof(char));
        char* buffer2 = (char*)calloc((size_t)file2_size + 1, sizeof(char));

        if (fopen_s(&fp1, file1, "r") != 0)
        {
            perror("\n���ļ�1ʧ��");
            exit(EXIT_FAILURE);
        }
        if (fopen_s(&fp2, file2, "r") != 0)
        {
            perror("\n���ļ�2ʧ��");
            exit(EXIT_FAILURE);
        }

        if (!fp1 || !fp2 || !buffer1 || !buffer2)
        {
            printf("\n����");
            exit(EXIT_FAILURE);
        }

        while (!feof(fp1) || !feof(fp2))
        {
            memset(buffer1, 0, (size_t)file1_size);
            memset(buffer2, 0, (size_t)file2_size);
            fgets(buffer1, (int)file1_size, fp1);//��ȡһ��
            fgets(buffer2, (int)file2_size, fp2);

            if (strcmp(parameter, "plu") == 0)//�ж��Ƿ���Ҫת������
            {
                size_t gbk1Size = utf_to_gbk(buffer1, NULL);//��utf-8תΪgbk
                size_t gbk2Size = utf_to_gbk(buffer2, NULL);
                char* gbk1 = (char*)calloc(gbk1Size, sizeof(char));
                char* gbk2 = (char*)calloc(gbk2Size, sizeof(char));
                if (gbk1 == NULL || gbk2 == NULL)
                {
                    printf("\n����");
                    exit(EXIT_FAILURE);
                }
                utf_to_gbk(buffer1, gbk1);//��utf-8תΪgbk		
                utf_to_gbk(buffer2, gbk2);

                if (strcmp(gbk1, gbk2) != 0)//�Ƚ������ļ���ͬһ���Ƿ�һ��
                {
                    if (strchr(gbk1, '\n') == 0)//�ж��ַ����Ƿ��л��з�
                    {
                        printf("\n��%d�У�\n�ļ�һ��%s\n�ļ�����%s", line + 1, gbk1, gbk2);//û�л��з�����ӻ��з���ӡ
                        line++;
                        continue;
                    }
                    printf("\n��%d�У�\n�ļ�һ��%s�ļ�����%s", line + 1, gbk1, gbk2);//�л��з��Ͳ���ӻ��з���ӡ
                }
                line++;

                free(gbk1);
                gbk1 = NULL;
                free(gbk2);
                gbk2 = NULL;
            }
            else
            {
                if (strcmp(buffer1, buffer2) != 0)//�Ƚ������ļ���ͬһ���Ƿ�һ��
                {
                    if (strchr(buffer1, '\n') == 0)//�ж��ַ����Ƿ��л��з�
                    {
                        printf("\n��%d�У�\n�ļ�һ��%s\n�ļ�����%s", line + 1, buffer1, buffer2);//û�л��з�����ӻ��з���ӡ
                        line++;
                        continue;
                    }
                    printf("\n��%d�У�\n�ļ�һ��%s�ļ�����%s", line + 1, buffer1, buffer2);//�л��з��Ͳ���ӻ��з���ӡ
                }
                line++;
            }
        }
        printf("\n\n������룬����ļ����ΪANSI��ʽ���ڡ�-p������������ӡ�-u�����ԡ�\n");

        free(buffer1);
        buffer1 = NULL;
        free(buffer2);
        buffer2 = NULL;
        fclose(fp1);
        fclose(fp2);
    }


    //��һ���ı��ļ������������һ���ı��ļ�
    else if ((strcmp(parameter, "rt") == 0 || strcmp(parameter, "rtu") == 0) && argv[3] != NULL)
    {
        char s_file[MAX_PATH], d_file[MAX_PATH];

        if (strcmp(parameter, "rtu") == 0)
        {
            my_strcpy_s(s_file, MAX_PATH, argv[4]);
            my_strcpy_s(d_file, MAX_PATH, argv[5]);
            mode = TO_GBK_CODE;// תΪGBK����
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
            printf("\n�ڴ治�㣡");
            exit(EXIT_FAILURE);
        }

        read_file(s_file, buffer, (size_t)fileSize + 1, mode, &totalChar, &line);//��ȡ�ļ�1������

        if (fopen_s(&fpwrite, d_file, "w") != 0)//���ļ�2
        {
            perror("���ļ�2ʧ��");
            exit(EXIT_FAILURE);
        }
        if (fpwrite == NULL || buffer == NULL)
        {
            perror("ʧ��");
            exit(EXIT_FAILURE);
        }

        for (i = strlen(buffer) - 1; i >= 0; i--)//iҪ-1������'\0'
        {
            if (is_chinese_char(buffer[i]) == IS_CHINESE_CHAR)//�ж������Ļ���Ӣ��
            {
                fprintf(fpwrite, "%c%c", buffer[i - 1], buffer[i]);//����ͬʱ��������ַ�����������
                i--;
            }
            else
                fprintf(fpwrite, "%c", buffer[i]);
        }
        fclose(fpwrite);
        free(buffer);
        buffer = NULL;

        puts("��ɣ�");
        printf("\n\n������룬����ļ����ΪANSI��ʽ���ڡ�-p������������ӡ�-u�����ԡ�\n");
    }


    //��һ���ı��ļ���������ӵ���һ���ı��ļ��Ľ�β��
    else if (strcmp(parameter, "ae") == 0 && argv[3] != NULL)
    {
        if (fopen_s(&fpread, argv[3], "r") != 0)
        {
            perror("��Դ�ļ�ʧ��");
            exit(EXIT_FAILURE);
        }

        if (fopen_s(&fpwrite, argv[4], "a") != 0)
        {
            perror("��Ŀ���ļ�ʧ��");
            exit(EXIT_FAILURE);
        }
        if (fpread == NULL || fpwrite == NULL)
        {
            printf("ʧ��");
            exit(EXIT_FAILURE);
        }

        while ((ch = fgetc(fpread)) != EOF)
            fputc(ch, fpwrite);

        fclose(fpread);
        fclose(fpwrite);
        puts("��ɣ�");
        printf("\n\n������룬��鿴�����ļ������Ƿ�һ�¡�\n");
    }


    else
        printf("\nû��������\n\n");
    //system("pause");
    return 0;
}



//��ȡ�ļ���С
long long get_file_size(const char* fileName)
{
    long long fileSize;
    FILE* fpread;

    if (fopen_s(&fpread, fileName, "rb") != 0)//���ļ�
    {
        perror("\n���ļ�ʧ��");
        exit(EXIT_FAILURE);
    }

    if (fpread == NULL)
        return -1;

    fseek(fpread, 0, SEEK_END);    //���ļ�λ��ָ�������ļ���β
    fileSize = _ftelli64(fpread);  //�����β�뿪ͷ���ֽ�ƫ����

    fclose(fpread);
    return fileSize;
}



//�����Сд��ĸת��ΪС���д��ĸ
void to_lower_or_upper(char* fileName, int mode)
{
    FILE* fpread, * fpwrite;
    int ch, i = 0;
    char temp_file[MAX_PATH] = { 0 };


    strcpy_s(temp_file, MAX_PATH, fileName);
    strcat_s(temp_file, MAX_PATH, ".temp");


    if (fopen_s(&fpread, fileName, "r") != 0)//���ļ�
    {
        perror("\n���ļ�ʧ��");
        exit(EXIT_FAILURE);
    }
    if (fopen_s(&fpwrite, temp_file, "w") != 0)
    {
        perror("\n������ʱ�ļ�ʧ��");
        exit(EXIT_FAILURE);
    }

    while ((ch = fgetc(fpread)) != EOF)//��ȡ�ļ����ݲ��ж��Ƿ�Ϊ��д��Сд��ĸ
    {
        if (mode == TO_UPPER)//�ж�ģʽ
        {
            if ((ch >= 'a' && ch <= 'z'))
            {
                ch -= 32;//��Сд��ĸת��Ϊ��д
                i++;
            }
        }
        else
        {
            if (ch >= 'A' && ch <= 'Z')
            {
                ch += 32;//����д��ĸתΪСд��ĸ
                i++;
            }
        }

        fputc(ch, fpwrite);//������ļ�
    }

    fclose(fpread);
    fclose(fpwrite);

    if (remove(fileName) || rename(temp_file, fileName))//ɾ��Դ�ļ�������ʱ�ļ�����ΪԴ�ļ�
    {
        printf("\nת��ʧ�ܣ�\n");
        remove(temp_file);
        exit(EXIT_FAILURE);
    }

    if (mode == TO_UPPER)
        printf("\n�ܼƣ���ת��%d��Сд�ַ���\n", i);
    else
        printf("\n�ܼƣ���ת��%d����д�ַ���\n", i);


}



//��ȡ�ļ�����
void read_file(const char* fileName, char* buffer, size_t fileSize, int mode, int* totalChar, int* line)
{
    int i = 0, C_char = 0, E_char = 0;
    char ch;
    FILE* fpread;
    *line = 0;

    if (fileName == NULL)
    {
        perror("\n�ļ�·����Ч");
        exit(EXIT_FAILURE);
    }

    if (fopen_s(&fpread, fileName, "r") != 0)
    {
        perror("\n���ļ�ʧ��");
        exit(EXIT_FAILURE);
    }

    char* buff = (char*)calloc(fileSize, sizeof(char));
    if (fpread == NULL || buff == NULL)
        return;

    do
    {
        if ((ch = (char)fgetc(fpread)) == EOF)
            continue;

        if (ch == '\n' && line != NULL)//ͳ������
            (*line)++;

        buff[i++] = ch;

        if (ch == '\n')
            continue;
        else if (is_chinese_char(ch) == IS_CHINESE_CHAR) //�ж��Ƿ�Ϊ����
            C_char++;			     //������
        else if (is_chinese_char(ch) == 0)//��������
            E_char++;
    } while (ch != EOF);

    if (mode == TO_GBK_CODE)//�ж��ǽ��ļ�����תΪgbk���뻹�ǲ�ת
    {
        utf_to_gbk(buff, buffer);
        *totalChar = E_char + (C_char / 3);//ͳ����������utf-8���뺺��ռ�����ֽ�
    }
    else
    {
        my_strcpy_s(buffer, fileSize, buff);
        *totalChar = E_char + (C_char / 2);//ͳ����������gbk���뺺��ռ�����ֽ�
    }

    fclose(fpread);
    free(buff);
    buff = NULL;

    //return i;//���ض�ȡ���ַ���
}


//�ж������ַ�
int is_chinese_char(char ch)
{
    return (ch >> 8) == 0 ? 0 : IS_CHINESE_CHAR;//��ch����8λ���ж�,�����ķ��� 1
}


//UTF-8����תGBK
int utf_to_gbk(char* utf_str, char* gbk_str)
{
    int len = MultiByteToWideChar(CP_UTF8, 0, utf_str, -1, NULL, 0);       //������Ŀ��ַ�����С
    wchar_t* UC_str = (wchar_t*)malloc(len * sizeof(wchar_t));
    if (!UC_str)
        return 1;
    len = MultiByteToWideChar(CP_UTF8, 0, utf_str, -1, UC_str, len);        //��utf-8����ת����unicode����
    len = WideCharToMultiByte(CP_ACP, 0, UC_str, -1, NULL, 0, NULL, 0);	    //��ת����Ҫ���ֽ���
    if (gbk_str != NULL)// ���gbk_strΪ�գ�������Ҫ�Ļ�������С
        len = WideCharToMultiByte(CP_ACP, 0, UC_str, -1, gbk_str, len, NULL, 0);//��unicode����ת����gbk����

    free(UC_str);//�ͷ��ڴ�
    return len;
}


//��ȡ��ǰ�����ڴ��С
long long get_memory(void)
{
    MEMORYSTATUSEX memoryStatus;
    memoryStatus.dwLength = sizeof(memoryStatus);

    if (!GlobalMemoryStatusEx(&memoryStatus))//��ȡ��ǰ���õ������ڴ�
    {	// ��ȡ��ǰ�����ڴ�ʧ��
        return 1048576;//1MB
    }
    return memoryStatus.ullAvailPhys;
}


// ����ļ���С
void print_file_size(double fileSize)
{
    if (fileSize < 1024.0)
        printf("\n��С��%.0lf�ֽ�\n", fileSize);
    else if (fileSize < 1048576.0)
        printf("\n��С��%.2lfKB��%.0lf�ֽڣ�\n", fileSize / 1024.0, fileSize);
    else if (fileSize < 1073741824.0)
        printf("\n��С��%.2lfMB��%.0lf�ֽڣ�\n", fileSize / 1048576.0, fileSize);
    else
        printf("\n��С��%.2lfGB��%.0lf�ֽڣ�\n", fileSize / 1073741824.0, fileSize);
}


// ɾ���к�
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
    // ��������
    retVal = sscanf_s(tmp - i, "%d", &num);
    free(tmp - i);
    if (retVal == EOF || retVal == 0)
        return;

    // ����ǰ������ֺͿո�
    for (j = 0, i += 1; i < strlen(str) + 1; i++, j++)
        str[j] = str[i];
}

// ɾ���ַ�����ͷ�Ŀհ�
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

//�����ʾ
void output_tip(int argc, const char* parameter)
{
    if (argc == 2)
    {
        if (strcmp(parameter, "p") == 0)
            printf("��ʽ���ļ�����.exe -p <�ļ�·��>\n���磺�ļ�����.exe -p C:\\test.txt\n\n");

        else if (strcmp(parameter, "c") == 0)
            printf("��ʽ���ļ�����.exe -c <Դ�ļ�·��> <Ŀ���ļ�·��>\n���磺�ļ�����.exe -c C:\\test.txt H:\\test.txt\n\n");

        else if (strcmp(parameter, "s") == 0)
            printf("��ʽ���ļ�����.exe -s <�ļ�·��> \n���磺�ļ�����.exe -s C:\\test.iso\n\n");

        else if (strcmp(parameter, "u") == 0)
            printf("��ʽ���ļ�����.exe -u <�ļ�·��> \n���磺�ļ�����.exe -u C:\\test.txt\n\n");

        else if (strcmp(parameter, "l") == 0)
            printf("��ʽ���ļ�����.exe -w <�ļ�·��> \n���磺�ļ�����.exe -l C:\\test.txt\n\n");
    }
    else if (argc == 3)
    {
        if (strcmp(parameter, "pu") == 0)
            printf("��ʽ���ļ�����.exe -p -u <�ļ�·��>\n���磺�ļ�����.exe -p -u C:\\test.txt\n\n");

        else if (strcmp(parameter, "pr") == 0)
            printf("��ʽ���ļ�����.exe -p  -r <�ļ�·��>\n���磺�ļ�����.exe -p -r C:\\test.txt\n\n");

        else if (strcmp(parameter, "pe") == 0)
            printf("��ʽ���ļ�����.exe -p -e <�ļ�·��> <�ַ�>\n���磺�ļ�����.exe -p -e C:\\test.txt Hello\n\n");

        else if (strcmp(parameter, "la") == 0)
            printf("��ʽ���ļ�����.exe -l -a <�ļ�·��>\n���磺�ļ�����.exe -l -a C:\\test.txt\n\n");

        else if (strcmp(parameter, "ld") == 0)
            printf("��ʽ���ļ�����.exe -l -d <�ļ�·��>\n���磺�ļ�����.exe -l -a C:\\test.txt\n\n");

        else if (strcmp(parameter, "pl") == 0)
            printf("��ʽ���ļ�����.exe -p -l <�ļ�·��1> <�ļ�·��2>\n���磺�ļ�����.exe -p -l C:\\test.txt H:\\text.txt\n\n");

        else if (strcmp(parameter, "rt") == 0)
            printf("��ʽ���ļ�����.exe -r -t <�ļ�·��1> <�ļ�·��2>\n���磺�ļ�����.exe -r -t C:\\test.txt H:\\text.txt\n\n");

        else if (strcmp(parameter, "ae") == 0)
            printf("��ʽ���ļ�����.exe -a -e <�ļ�·��1> <�ļ�·��2>\n���磺�ļ�����.exe -a -e C:\\test.txt  H:\\text.txt\n\n");

        else if (strcmp(parameter, "cb") == 0)
            printf("��ʽ���ļ�����.exe -c -b <Դ�ļ�·��> <Ŀ���ļ�·��>\n���磺�ļ�����.exe -c -b C:\\test.txt H:\\test.txt\n\n");
    }
    else if (argc == 4)
    {
        if (strcmp(parameter, "pru") == 0)
            printf("��ʽ���ļ�����.exe -p  -r -u <�ļ�·��>\n���磺�ļ�����.exe -p -r -u C:\\test.txt\n\n");

        else if (strcmp(parameter, "peu") == 0)
            printf("��ʽ���ļ�����.exe -p -e -u <�ļ�·��> <�ַ�>\n���磺�ļ�����.exe -p -e -u C:\\test.txt Hello\n\n");

        else if (strcmp(parameter, "plu") == 0)
            printf("��ʽ���ļ�����.exe -p -l -u <�ļ�·��1> <�ļ�·��2>\n���磺�ļ�����.exe -p -l -u C:\\test.txt H:\\text.txt\n\n");

        else if (strcmp(parameter, "rtu") == 0)
            printf("��ʽ���ļ�����.exe -r -t -u <�ļ�·��1> <�ļ�·��2>\n���磺�ļ�����.exe -r -t -u C:\\test.txt H:\\text.txt\n\n");

        else if (strcmp(parameter, "lad") == 0)
            printf("��ʽ���ļ�����.exe -l -a -d <�ļ�·��>\n���磺�ļ�����.exe -l -a -d C:\\test.txt\n\n");
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