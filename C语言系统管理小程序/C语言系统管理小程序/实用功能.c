#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <io.h>
#include <process.h>
#include <limits.h>
#include "common.h"
#include "str_util.h"

#define NO_OVER_WRITE 110 
#define NO_MEMORY -11

#ifdef _WIN64 // 64λ    
#define MAX_BUFFER_SIZE 4294967296LL // �����ļ����ʹ���ڴ�4GB
#define RESERVED_MEMORY 2696937472LL // ϵͳ�����ڴ�2.5GB
#else         // 32λ
#define MAX_BUFFER_SIZE 1073741824 // �����ļ����ʹ���ڴ�1GB
#define RESERVED_MEMORY 524288000  // ϵͳ�����ڴ�500MB
#endif 

// ���ܻ����
void encrypt_or_decrypt(void);
// ���ܻ�����ļ�����
int enc_dec_file(char* source, char* dest);
// ����������
void xor_data(char* data);
// ��·��������ͬ�ļ�����ʾ�Ƿ񸲸�
int over_write(char* sourceFileName, char* destFileName);
// ��ӡ����
void print_progress(double curSize, double totalSize);

int  cal(void);	     // ������
void oper(void);     // ��������
void square(void);   // ��������
void bin(void);      // ������תʮ����
void dec(void);		 // ʮ����ת������
void ascii(void);    // ascii��ת�ַ�
void character(void);// �ַ�תascii��
void circular(void); // Բ�������
void cube(void);     // ���������
void cuboid(void);   // ���������

//����������
int addstart(void);
//ɾ��������
int delstart(void);
//����ʱ
void countdown(void);
// ����mp3
int play_mp3(void);


// ʵ�ù���
void function(void)
{
	int num;

	while (1)
	{
		system("mode con:cols=45 lines=20 && title ʵ�ù��� && cls");

		printf("\n\n\t      1���๦�ܼ�����\n\n");
		printf("\t      2�������ļ�����\n\n");
		printf("\t      3��������������\n\n");
		printf("\t      4��ɾ�����������\n\n");
		printf("\t      5������ʱ\n\n");
		printf("\t      6������MP3\n\n");
		printf("\t      7������������\n\n");
		printf("    �����룺");

		while (scanf_s("%d", &num) != 1)
		{
			printf("\n������������ԣ�");
			rewind(stdin);
		}
		rewind(stdin);

		if (num == 1)
			cal();

		else if (num == 2)//�����ļ�����
			encrypt_or_decrypt();

		else if (num == 3)
			addstart();

		else if (num == 4)
			delstart();

		else if (num == 5)
			countdown();

		else if (num == 6)
			play_mp3();

		else if (num == 7)
			break;

		else
		{
			printf("\n\t\t�������");
			Sleep(1000);
		}
	}
}


//���ܻ����
void encrypt_or_decrypt(void)
{
	char filename[MAX_PATH];      // ��������ܻ�������ļ�����
	char dest_filename[MAX_PATH]; // ������ܺ����ܺ��ļ�����
	unsigned len, i, j;
	int res, err, num;
	FILE* fpwrite;

	while (1)
	{
		memset(filename, 0, MAX_PATH);//��������
		memset(dest_filename, 0, MAX_PATH);//��������

		system("title ��ӭʹ�ü��ܻ���ܳ��� && cls && mode con cols=50 lines=20");
		printf("\n��ֹ������Ҫ�ļ�����ֹ�����磡\n");
		printf("\n\n\t         0��������һ����\n\n");
		printf("\t         1�������ļ�����\n\n");
		printf("\t         2�������ļ�����\n\n");
		printf("\t         3����������\n\n");
		printf("\t         4����������\n\n");
		printf("\t         5���鿴ʹ��˵��\n\n");
		printf("\t  �����룺");
		while (scanf_s("%d", &num) != 1)
		{
			rewind(stdin);
			printf("\n����������������룺");
		}
		rewind(stdin);

		if (num == 0)
			break;
		else if (num == 1)  //�����ļ�����
		{
			system("title �����ļ����� && mode con cols=60 lines=20 && cls");

			printf("ע�⣺\n1�����ܺ���ļ����ܸ���ԭ�������ݣ�������ܺ��ָܻ�ԭ����\n");
			printf("2�������ܵ��ļ����ļ����ƣ�����·������㡢��׺���ȣ�����200�����ڣ�����200�ֻᱨ�����ˡ�\n");
			printf("3�����ܺ���ļ���������ļ����뱣��ԭ��׺����.fh���������磺�ѡ�1.txt.fh�� ����Ϊ ��2.txt.fh������\n");
			printf("4����ֹ������Ҫ�ļ�����ֹ�����磡\n");
			printf("5������000������һ���档\n\n\n\n");
			printf("������������ļ������ƣ����磺C:\\1.txt��1.txt����");

			fgets(filename, (MAX_PATH - 5), stdin);//����������ļ�������
			del_last_newline(filename);

			if (filename[0] == 34)//֧���ļ���ק����
			{
				filename[strlen(filename) - 1] = 0;
				strcpy_s(filename, strlen(filename) + 1, filename + 1);//�����ק����������˫����
			}

			if (!strcmp(filename, "000"))   //����000����������
				continue;

			strcpy_s(dest_filename, MAX_PATH, filename);
			strcat_s(dest_filename, MAX_PATH, ".fh");// �����ܺ���ļ���Ӻ�׺��

			int retVal = over_write(filename, dest_filename);
			if (retVal == NO_MEMORY)
				return;

			printf("\n���ڼ��ܣ�%s�����Ե�...\n", filename);// ����ļ���
			res = enc_dec_file(filename, dest_filename);
			if (res == 0)
			{
				system("cls");
				printf("\n\n\n���� %s�ɹ���\n\n\t\t    ", filename);// ����ļ���
			}
			else
				putchar('\n');

			system("pause");
			continue;
		}

		else if (num == 2)//�����ļ�����
		{
			system("title �����ļ����� && mode con cols=60 lines=20 && cls");

			printf("ע�⣺\n");
			printf("1�������ܵ��ļ����ļ����ƣ�����·������㡢��׺���ȣ�����205�����ڣ�����205�ֻᱨ�����ˡ�\n");
			printf("2�����ܺ���ļ���������ļ����뱣��ԭ��׺����.fh���������磺�ѡ�1.txt.fh�� ����Ϊ ��2.txt.fh������\n");
			printf("3����ֹ������Ҫ�ļ�����ֹ�����磡\n");
			printf("4������000������һ���档\n\n\n\n");
			printf("������������ļ������ƣ����磺C:\\1.txt��1.txt.fh����");

			fgets(filename, MAX_PATH, stdin); //����������ļ�������
			del_last_newline(filename);

			if (filename[0] == 34)//֧���ļ���ק����
			{
				filename[strlen(filename) - 1] = 0;
				strcpy_s(filename, strlen(filename) + 1, filename + 1);//�����ק����������˫����
			}

			if (!strcmp(filename, "000"))//����000����������
				continue;

			strcpy_s(dest_filename, MAX_PATH, filename);

			err = 0;
			len = (unsigned)strlen(dest_filename) - 1;     //��ȡ���鳤��
			for (j = len; j > (len - 3); j--)//ɾ����.fh����׺��
			{
				if (dest_filename[j] == 'h' || dest_filename[j] == 'f' || dest_filename[j] == '.')
					dest_filename[j] = '\0';
				else
					err = 1;
			}
			if (err == 1)
			{
				printf("\n�������ļ���չ���ǡ�.fh�����ļ���\n");
				system("pause");
				continue;
			}

			int retVal = over_write(filename, dest_filename);// �ж�Ŀ���ļ��Ƿ���ڣ����Ҵ���ʱ�Ƿ񸲸�
			if (retVal == NO_MEMORY)
				return;

			printf("\n���ڽ��ܣ�%s�����Ե�...", filename);//����ļ���

			res = enc_dec_file(filename, dest_filename);
			if (res == 0)
			{
				system("cls");
				printf("\n\n\n���� %s�ɹ���\n\n\t\t    ", filename);//����ļ���
			}
			else
				putchar('\n');

			system("pause");
			continue;
		}
		else if (num == 3)//��������
		{
			system("title �������� && mode con cols=50 lines=20 && cls");

			printf("\n\n������Ҫ���ܵ����ģ�50�����ڣ������ո���ȣ���\n");
			fgets(filename, MAX_PATH, stdin);
			del_last_newline(filename);

			j = (unsigned)strlen(filename);
			for (i = 0; i <= j; i++)//�����ַ���
				filename[i] = ~filename[i];

			//д���ļ�
			if ((err = fopen_s(&fpwrite, "���ܺ������.txt", "w")) != 0)
			{
				//i = MessageBox(NULL, L"�����ļ�ʧ�ܣ�\n�Ƿ�ֱ�����루���ܻᵼ����ʾ����ȫ����", L"Error", MB_YESNO | MB_ICONERROR);
				//if (i == IDNO)
				//    continue;
				//else if (i == IDYES)
				printf("\n\n���ܺ�������ǣ�%s\n\n", filename);
				continue;
			}

			if (fpwrite)
			{
				fputs(filename, fpwrite);//�����ܺ������д���ļ�
				fclose(fpwrite);
			}

			printf("\n\n���ܺ�������ǣ�%s\n\n", filename);
			printf("\n\n��򿪱�����Ŀ¼�еġ����ܺ������.txt���鿴\n\n");
			//  system("start ���ܺ������.txt");
			system("pause");
		}
		else if (num == 4)//��������
		{
			system("title �������� && mode con cols=50 lines=20 && cls");

			printf("\n\n������Ҫ���ܵ����ģ�50�����ڣ������ո���ȣ���\n");
			fgets(filename, MAX_PATH, stdin);
			del_last_newline(filename);


			j = (unsigned)strlen(filename);
			for (i = 0; i <= j; i++)//�����ַ���
				filename[i] = ~filename[i];

			//д���ļ�
			if (fopen_s(&fpwrite, "���ܺ������.txt", "w") != 0)
			{
				/* i = MessageBox(NULL, L"�����ļ�ʧ�ܣ�\n�Ƿ�ֱ����ʾ�����ܻᵼ����ʾ����ȫ����", L"Error", MB_YESNO | MB_ICONERROR);
				 if (i == IDNO)
					 continue;
				 else if (i == IDYES)*/
				printf("\n\n���ܺ�������ǣ�%s\n\n", filename);
				continue;
			}

			if (fpwrite)
			{
				fputs(filename, fpwrite);//�����ܺ������д���ļ�
				fclose(fpwrite);

			}
			printf("\n\n���ܺ�������ǣ�%s\n\n", filename);
			printf("\n\n��򿪱�����Ŀ¼�еġ����ܺ������.txt���鿴\n\n");
			//  system("start ���ܺ������.txt");
			system("pause");
		}
		else if (num == 5)//ʹ��˵��
		{
			if ((err = fopen_s(&fpwrite, "ʹ��˵��.txt", "w")) != 0)
			{
				perror("������ʹ��˵��.txt��ʧ��");
				putchar('\n');
				system("pause");
				continue;
			}
			if (fpwrite)
			{
				fputs("ʹ�÷�����\n"
					"1.�ļ��������뷽ʽ��\n"
					"1��ֱ���������ļ���ק�����򴰿ڼ���\n"
					"2���������ļ����ƴ�����ֱ�������ļ�·�����磺C://1.txt��\n"
					"3���������ļ����ƴ�����ֱ�������ļ����ƣ��磺1.txt��\n"
					"4��ע�⣺ʹ�õ�3�ַ�ʽ��Ҫ�ѱ�����ʹ�����/�������ļ��ŵ�ͬһ�ļ����ڡ�\n\n"
					"2.������������֧�������ļ���ʽ����ʵ�ʻ�����֤��\n\n"
					"3.ע���������ϸ�Ķ��������ݣ���\n"
					"1�����ܻ�����ļ�ֻ����ļ����ݽ��м��ܻ���ܣ�������ļ����ƽ��м��ܻ���ܡ�\n"
					"2�����ܺ���ļ���������ļ����뱣��ԭ��׺����.fh���������磺�ѡ�1.txt.fh�� ����Ϊ ��2.txt.fh������\n"
					"3�����ܺ���ļ��벻Ҫ���������ݣ�������ܺ��ļ����ָܻ�ԭ����\n"
					"4���������ļ����ƣ�����·������㡢��׺���ȣ�����200�����ڣ��������ļ����ƣ�����·������㡢��׺���ȣ�����205�����ڡ�\n"
					"5�������ֻ�ʺ����֡�\n"
					"6��ע�⣺��ֹʹ�ñ����������Ҫ�ļ�����ֹʹ�ñ������Υ��������£���ֹʹ�ñ����������磡��ʹ�ñ������ɵ��κκ��������ԭ���߳е���", fpwrite);
				fclose(fpwrite);
			}
			system("start ʹ��˵��.txt");
		}
		else
		{
			printf("\n\t�������");
			Sleep(1000);
		}

	}
}

unsigned __int64 get_current_available_memory(void)
{
	unsigned __int64 currentMemory;
	MEMORYSTATUSEX memoryStatus;
	memset(&memoryStatus, 0, sizeof(memoryStatus));
	memoryStatus.dwLength = sizeof(memoryStatus);
	if (!GlobalMemoryStatusEx(&memoryStatus))//��ȡ��ǰ���õ������ڴ�
		currentMemory = 10485760ULL;// 10MB
	else
		currentMemory = memoryStatus.ullAvailPhys;
	return currentMemory;
}

long long get_file_size(const char* file)
{
	FILE* fpRead;
	long long fileSize;
	if (fopen_s(&fpRead, file, "rb") != 0)
	{
		perror("��Դ�ļ�����");
		return -1;
	}
	fseek(fpRead, 0, SEEK_END);//�ļ�ָ�������ļ�ĩβ
	fileSize = _ftelli64(fpRead);//��ȡ�ļ���С
	return fileSize;
}

//���ܻ�����ļ�����
int enc_dec_file(char* source, char* dest)
{
	FILE* fpRead, * fpWrite;
	size_t len;
	size_t curSize = 0;
	size_t bufferCount;
	char bar[SIZE] = { 0 };
	long long currentMemory;
	long long fileSize, totalSize;
	double per = 0.0, max = 100.0;
	char* buffer = NULL;
	// char buffer[10] = {0};
	currentMemory = get_current_available_memory();
	fileSize = get_file_size(source);
	totalSize = fileSize;
	if (fopen_s(&fpRead, source, "rb") != 0)
	{
		perror("��Դ�ļ�����");
		return -1;
	}
	if (fopen_s(&fpWrite, dest, "wb") != 0 || !fpWrite)
	{
		perror("��Ŀ���ļ�����");
		return -1;
	}
	// ��������ļ�ʱmalloc���������ڴ�ռ��С
	for (int i = 2; i <= 100; i++)
	{
		if (currentMemory == 10485760)// 10MB
			break;
		// ����ǰ�ļ���С���� i�ټ��ϱ����ڴ棬���ж��Ƿ���ڵ�ǰ�����ڴ档
		if ((fileSize + RESERVED_MEMORY) <= currentMemory && fileSize < MAX_BUFFER_SIZE)// 32λ�¸����ļ�ʱ����ڴ�Ϊ1GB��64λΪ4GB
			break;
		fileSize /= i;
	}
	bufferCount = (size_t)fileSize;
	buffer = (char*)calloc(bufferCount + 1, sizeof(char));
	if (buffer == NULL)// ����ռ䲻��
	{
		perror("�ڴ治��");
		return -1;
	}

	while ((len = fread(buffer, sizeof(char), bufferCount, fpRead)) > 0)//�����ļ�
	{
		xor_data(buffer);// �Զ�ȡ���ڴ�����ݽ���������
		if (fwrite(buffer, sizeof(char), len, fpWrite) < len)
		{
			perror("д��Ŀ�����ʧ��");
			return -1;
		}
		// ��ӡ����
		curSize += len;
		print_progress((double)curSize, (double)totalSize);
	}
	fclose(fpRead);
	fclose(fpWrite);
	free(buffer);
	buffer = NULL;
	return 0;
}

void print_progress(double curSize, double totalSize)
{
	char bars[SIZE] = { 0 };
	double per = (curSize / totalSize) * 100;
	int charNum = (int)((per / 2) + 0.5);// �۰��ӡ�����ַ�
	charNum = charNum > 50 ? 50 : charNum;
	for (int i = 0; i < charNum; i++)
		bars[i] = '>';
	printf("\r[%-50s] %.1lf%%", bars, per);
}


void xor_data(char* data)
{
	while (*data)
	{
		*data = ~*data;
		data++;
	}
}

// ��·��������ͬ�ļ�����ʾ�Ƿ񸲸�
int over_write(char* sourceFileName, char* destFileName)
{
	int i = 1;
	char ch;
	if (_access(destFileName, 0) == -1)// �ж���Ҫ���ܻ���ܵ��ļ��Ƿ��ڵ�ǰ·���д���
		return 0;

	while (1)
	{
		system("cls");
		printf("\n\n%s���ڵ�ǰ·���д��ڣ��Ƿ񸲸Ǵ��ļ���y/n����", destFileName);
		rewind(stdin);
		scanf_s("%c", &ch, 1);

		if (ch == 'y' || ch == 'Y')
		{
			return 0;
		}
		else if (ch == 'n' || ch == 'N')
		{
			char ext[10];
			_splitpath_s(destFileName, NULL, 0, NULL, 0, NULL, 0, ext, sizeof(ext));

			size_t size = (strlen(destFileName) + 1 + sizeof(ext)) * sizeof(char);
			char* buff = (char*)malloc(size);
			if (buff == NULL)
			{
				MessageBox(NULL, L"�ڴ治�㣡", L"Error", MB_OK | MB_ICONERROR);
				return NO_MEMORY;
			}
			for (; i < 10000; i++)
			{
				memset(buff, 0, size);
				sprintf_s(buff, size, "%s(%d)%s", sourceFileName, i, ext);
				if (_access(buff, 0))
					break;
			}

			strcpy_s(destFileName, size, buff);
			free(buff);
			return 0;
		}
		else
		{
			printf("\n\t\t\t�������");
			Sleep(1000);
			continue;
		}
	}
	return 0;
}

//������
int cal(void)
{
	int num;

	while (1)
	{
		system("mode con lines=20 cols=60 && cls && title �๦�ܼ�����");//���ñ���

		printf("\t\t----��ӭʹ�ö๦�ܼ�����----\n\n");
		printf("    \t\t       0��������һ����\n");
		printf("    \t\t       1����ͨ����\n");
		printf("    \t\t       2����������\n");
		printf("    \t\t       3��ʮ����ת������\n");
		printf("    \t\t       4��������תʮ����\n");
		printf("    \t\t       5��ASCII��ת�ַ�\n");
		printf("    \t\t       6���ַ�תASCII��\n");
		printf("    \t\t       7��Բ����/Բ����\n");
		printf("    \t\t       8��������/�����\n");
		printf("    \t\t       9��������/�����\n\n\n");
		printf("\t�����룺");
		while (scanf_s("%d", &num) != 1)
		{
			rewind(stdin);
			printf("\n����������������룺");
		}
		rewind(stdin);


		switch (num)
		{
		case 0:             return 0;//������һ����
		case 1:oper();      break;   //�������
		case 2:square();    break;   //��������
		case 3:dec();       break;   //ʮ����ת������
		case 4:bin();       break;   //������תʮ����
		case 5:ascii();     break;   //ASCII��ת�ַ�
		case 6:character(); break;   //�ַ�תASCII��
		case 7:circular();  break;   //Բ���塢Բ����
		case 8:cube();      break;   //�����塢�μ���
		case 9:cuboid();    break;   //�����塢�μ���
		default:printf("\n\t�������"); Sleep(1000); break;
		}

	}
	return 0;
}


//��������
void oper(void)
{
	double a, b, d = 0.0;
	char c;

	while (1)
	{
		system("cls && title ��������");

		printf("\t\t      ����0*0����\n\n");
		printf("�����硰3*4.12����5.88+2.1������������ʽ��");
		while (scanf_s("%lf %c %lf", &a, &c, 1, &b) != 3)
		{
			rewind(stdin);
			printf("\n����������������룺");
		}
		rewind(stdin);

		if (a == 0 && c == '*' && b == 0)//����0*0����
			break;

		switch (c)
		{
		case'+':d = a + b; break;
		case'-':d = a - b; break;
		case'*':d = a * b; break;
		case'/':d = (a == 0) ? 0 : (a / b); break;
		case'%':d = (int)a % (int)b; break;
		}

		printf("\n\n\n\t\t%.3lf %c %.3lf = %.3lf\n\n\n", a, c, b, d);
		system("pause");
	}
}


//��������
void square(void)
{
	double x;

	while (1)
	{
		system("cls && title ��������");

		printf("\n��������Ҫ��������(����0���أ���");
		while (scanf_s("%lf", &x) != 1 || x < 0)
		{
			rewind(stdin);
			printf("\n����������������룺");
		}
		rewind(stdin);

		if (x == 0)//����0����
			break;

		printf("\n\t\t %.3lf�������ڣ�%.3lf\n\n", x, sqrt(x));
		system("pause");
	}
}


//ʮ����ת������
void dec(void)
{
	long long temp, dec;
	int count[MAX_PATH] = { 0 };
	int bin, i = 0;

	while (1)
	{
		system("title ʮ����ת������ && cls");

		printf("\n\n\n������һ��ʮ������(����0���أ���");
		while (scanf_s("%lld", &dec) != 1)//�����������
		{
			printf("\n�������,�����ԣ�");
			rewind(stdin);
		}
		rewind(stdin);

		if (dec == 0)
			break;

		temp = dec;
		do   //ת��ʮ����
		{
			bin = dec % 2;  //��ÿһ�ε�����
			dec = dec / 2;
			i++;
			count[i] = bin;
		} while (dec != 0);

		printf("\n\nʮ���� %lld ת���ɶ����Ƶ�ֵ�ǣ�", temp);
		for (; i > 0; i--)   //������� 
			printf("%d", count[i]);

		printf("\n\n");
		system("pause");
	}
}


//������תʮ����
void bin(void)
{
	while (1)
	{
		long long bin, dec = 0;

		system("title ������תʮ���� && cls");

		printf("\n\n\n������һ����������(����0���أ���");

		while ((bin = getchar()) != '\n')
			dec = dec * 2 + (bin - '0');

		if (dec == 0)
			return;

		printf("\n\nת��Ϊʮ���Ƶ�ֵ�ǣ�%lld\n\n", dec); //���
		system("pause");
	}
}


//ascii��ת�ַ�
void ascii(void)
{
	int ascii;

	while (1)
	{
		system("cls && title ASCII��ת�ַ�");

		printf("\n\n\n");
		printf("��ע�⣺1-31��127���ǿ����ַ���\n\n");
		printf("��������Ҫת�����ַ���ASCII��(����0���أ���");
		while (scanf_s("%d", &ascii) != 1)
		{
			printf("\n����������������룺");
			rewind(stdin);	//��ջ�����
		}
		rewind(stdin);

		if (ascii == 0)
			break;

		printf("\n\n\n\n      ASCII�� %d ��Ӧ���ַ�Ϊ��%c\n\n", ascii, ascii);
		printf("������ʾ��������Ϊ�����ַ���\n\n");
		system("pause");
	}
}


//�ַ�תascii��
void character(void)
{
	int i = 0;
	char c;

	while (1)
	{
		system("cls && title �ַ�תASCII��");
		printf("\n\n������Ҫת����ASCII����ַ���");
		scanf_s("%c", &c, 1);
		rewind(stdin);

		printf("\n\n\n\n     �ַ� %c ��Ӧ��ASCII��Ϊ��%d\n\n", c, c);
		i++;

		if (i == 3)
		{
			printf("\n\n\t       �Ƿ񷵻���һ���棿��Y/N��");
			scanf_s("%c", &c, 1);
			rewind(stdin);
			if (c == 'y' || c == 'Y')
				break;
			else if (c == 'n' || c == 'N')
			{
				i = 0;
				continue;
			}
			else
			{
				i = 0;
				printf("\n\n\t\t\t�������");
				Sleep(1000);
				continue;
			}
		}
		system("pause");
	}
}


//Բ���塢Բ
void circular(void)
{
	double pi = 3.14, r, h;
	char tmp[20];

	system("cls && title Բ����/Բ�ļ���");
	do
	{
		printf("\n������Pi��ֵ���س�Ĭ��3.14����");
		fgets(tmp, sizeof(tmp), stdin);
		rewind(stdin);

		if (tmp[0] == '\n')	// ֱ�ӻس�Ĭ��pi=3.14
			break;

		del_last_newline(tmp);
	} while (sscanf_s(tmp, "%lf", &pi) == EOF);

	while (1)
	{
		system("cls && title Բ����/Բ�ļ���");
		printf("\t\t �ڵ�һ������������0����");
		printf("\n\n������Բ����ĵ���뾶\n��Բ�İ뾶����λ�����ף���");
		while (scanf_s("%lf", &r) != 1)
		{
			printf("\n������������ԣ�");
			rewind(stdin);
		}
		rewind(stdin);

		if (r == 0.0)//����0����
			break;

		printf("\n������Բ����ĸ�\n������Բ��������ܳ�������1����λ�����ף���");
		while (scanf_s("%lf", &h) != 1)
		{
			printf("\n������������ԣ�");
			rewind(stdin);
		}
		rewind(stdin);

		printf("\n\nԲ�����Ϊ��%.3lf ƽ������\n", pi * r * r);
		printf("Բ���ܳ�Ϊ��%.3lf ����\n\n", 2 * pi * r);
		printf("Բ����ĵ����Ϊ��  %.3lf ƽ������\n", pi * r * r);
		printf("Բ����ĵ����ܳ�Ϊ��%.3lf ����\n", 2 * pi * r);
		printf("Բ��������Ϊ��    %.3lf ��������\n", pi * r * r * h);
		printf("Բ����Ĳ����Ϊ��  %.3lf ƽ������\n", 2 * pi * r * h);
		printf("Բ����ı����Ϊ��  %.3lf ƽ������\n\n", 2 * pi * r * r + 2 * pi * r * h);
		system("pause");
	}

}


//�����Ρ�������
void cube(void)
{
	double edge;

	while (1)
	{
		system("cls && title ������/������ļ���");

		printf("\t\t �ڵ�һ������������0����");
		printf("\n\n�����������εı߳�\n����������ⳤ����λ�����ף���");
		while (scanf_s("%lf", &edge) != 1)//�����������
		{
			printf("\n������������ԣ�");
			rewind(stdin);
		}
		rewind(stdin);

		if (edge == 0.0)
			break;

		printf("\n\n�����ε����Ϊ��%.3lf ƽ������\n", edge * edge);
		printf("�����ε��ܳ�Ϊ��%.3lf ����\n\n", edge * 4);
		printf("����������Ϊ��  %.3lf ��������\n", edge * edge * edge);
		printf("������ı����Ϊ��%.3lf ƽ������\n\n", edge * edge * 6);
		system("pause");
	}
}


//�����Ρ�������
void cuboid(void)
{
	double h, l, w;

	while (1)
	{
		system("cls && title ������/������ļ���");

		printf("\t\t �ڵ�һ������������0����");
		printf("\n\n�����볤���λ򳤷���ĳ�����λ�����ף���");
		while (scanf_s("%lf", &l) != 1)
		{
			printf("\n������������ԣ�");
			rewind(stdin);
		}
		rewind(stdin);
		if (l == 0.0)
			break;

		printf("\n�����볤���λ򳤷���Ŀ���λ�����ף���");
		while (scanf_s("%lf", &w) != 1)
		{
			printf("\n������������ԣ�");
			rewind(stdin);
		}
		rewind(stdin);

		printf("\n�����볤����ĸ�\n��ֻ���㳤����������1����λ�����ף���");
		while (scanf_s("%lf", &h) != 1)
		{
			printf("\n������������ԣ�");
			rewind(stdin);
		}
		rewind(stdin);

		printf("\n\n�����ε�����ǣ�%.3lf ƽ������\n", l * w);
		printf("�����ε��ܳ��ǣ�%.3lf ����\n\n", (l * w + l * w) * 2);
		printf("�����������ǣ�  %.3lf ��������\n", h * l * w);
		printf("������ı�����ǣ�%.3lf ƽ������\n\n", (l * w + l * h + w * h) * 2);
		system("pause");
	}
}

//��ӳ���������
int addstart(void)
{
	system("title ������������ && cls");

	HKEY hKey;
	wchar_t* regPath = { L"Software\\Microsoft\\Windows\\CurrentVersion\\Run" }; //ע���������·��
	wchar_t W_name[SIZE];
	wchar_t W_path[MAX_PATH];
	char path[MAX_PATH] = { 0 };//��Ҫ����������������·��
	char name[SIZE] = { 0 };//ע�����������

	printf("\t    ����000������һ����\n\n");
	printf("��������Ҫ����������������·��\n�����磺H:\\test\\test.exe��\n ��");
	fgets(path, MAX_PATH, stdin);//����·��
	del_last_newline(path);

	if (!strcmp(path, "000"))//����000�˳�
		return 0;

	printf("\n\n��������ӵ�ע���ļ�ֵ������\n\n����Ҫ��15���ַ��ڡ�\n  ����ʹ���ַ������֡���������ո�\n  ������ʹ�á�\\����\n\n ��");
	fgets(name, SIZE, stdin);//��������
	del_last_newline(name);

	char_to_wchar(name, W_name, arr_count(name));
	char_to_wchar(path, W_path, arr_count(path));

	//��ע��������� 
	if (RegOpenKeyExW(HKEY_CURRENT_USER, regPath, 0, KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS)
	{
		MessageBox(NULL, L"���ʧ�ܣ�", L"ERROR", MB_OK | MB_ICONERROR);
		return -1;
	}

	//���һ����Key,������ֵ
	if (RegSetKeyValue(hKey, NULL, W_name, REG_SZ, (LPCVOID)W_path, MAX_PATH) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);//�ر�ע���
		MessageBox(NULL, L"���ʧ�ܣ�", L"ERROR", MB_OK | MB_ICONERROR);
		return -1;
	}
	RegCloseKey(hKey);//�ر�ע���

	printf("\n\t         ��ӳɹ���\nע�⣺�벻Ҫɾ������ӵĳ��򣬷�����������ʧЧ��\n\n              ");
	system("pause");
	return 0;
}

//ɾ������������
int delstart(void)
{
	system("title ɾ������������ && cls");
	char name[SIZE];      // �洢����
	wchar_t w_name[SIZE]; // �洢����
	HKEY hKey;
	wchar_t* regPath = { L"Software\\Microsoft\\Windows\\CurrentVersion\\Run" }; //ע���������·��

	printf("\t    ����000������һ����\n\n\n");
	printf("�������ֵ�����ƣ�15���ַ��ڣ���");
	fgets(name, 31, stdin);
	del_last_newline(name);

	if (!strcmp(name, "000"))
		return 0;

	char_to_wchar(name, w_name, arr_count(w_name));
	// ��ע��������� 
	if (RegOpenKeyEx(HKEY_CURRENT_USER, regPath, 0, KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS)
	{
		MessageBox(NULL, L"ɾ��ʧ�ܣ�\n�����Ƿ��д���\n���Թ���ԱȨ�����б�������ԣ�", L"ERROR", MB_OK | MB_ICONERROR);
		return -1;
	}
	if (RegQueryValueEx(hKey, w_name, NULL, NULL, NULL, NULL) == ERROR_FILE_NOT_FOUND)
	{
		MessageBox(NULL, L"ע������ڴ��", L"ERROR", MB_OK | MB_ICONERROR);
		return -1;
	}
	if (RegDeleteValue(hKey, w_name) != ERROR_SUCCESS)
	{
		MessageBoxW(NULL, L"ɾ��ʧ�ܣ�\n�����Ƿ��д���\n���Թ���ԱȨ�����б�������ԣ�", L"ERROR", MB_OK | MB_ICONERROR);
		return -1;
	}
	RegCloseKey(hKey);

	printf("\n\n");
	printf("\t\tɾ���ɹ���");
	printf("\n\n\t      ");
	system("pause");
	return 0;
}

//����ʱ
void countdown(void)
{
	double min;
	system("mode con cols=40 lines=8");

	printf("��������ٷ�(����0���أ���");
	while (scanf_s("%lf", &min) != 1)
	{
		printf("����������������룺");
		rewind(stdin);
	}
	rewind(stdin);

	//���س������д���
	HWND hwnd = FindWindow(L"ConsoleWindowClass", NULL);
	if (hwnd)
		ShowWindow(hwnd, SW_HIDE);

	min *= 60;	 // �ֻ���
	min *= 1000; // �뻯���� 
	min -= 10000;
	Sleep((DWORD)min);
	MessageBox(NULL, TEXT("����ʱ����10���ӣ�"), TEXT("��ʾ"), MB_OK);

	if (hwnd)
		ShowWindow(hwnd, SW_SHOW);
}

//�ʵ�
int autoshut(void)
{
	system("title �����˳��˳����رյ��Ի����ü���");

	int i;
	char password[SIZE] = { "115-56" };//����
	char temp[SIZE] = { '\0' };
	wchar_t str[500];
	/*wchar_t fileName[MAX_PATH];
	HKEY hkey;
	BYTE v = 1;*/

	//GetCurrentDirectory(MAX_PATH, fileName);// ��ȡ��������Ŀ¼
	//wcscat_s(fileName, MAX_PATH, L"\\hacked.jpg");

	char wanging_zero[] = { "���棺�����ݲ��ʺ��и��Լ��������ಡ����Ѫѹ������Ѫ�ܲ����񾭴��ҡ���������ơ������ߵ���ʿ���ʣ�"
		"�����˳��˳����رյ��Ի����ü������ӲҪ���ʣ�����Ը���" };

	char warning_one[] = { "���棺�����ݲ��ʺ��и��Լ��������ಡ����Ѫѹ������Ѫ�ܲ����񾭴��ҡ���������ơ������ߵ���ʿ���ʣ�"
		"�����˳��˳����رյ��Ի����ü������ӲҪ���ʣ�����Ը���\n\t\t\t\t�Ƿ�������ʣ�" };

	char warning_two[] = { "���ĵ������˲����������˳������򣬷������ĵ��Ժͱ�������ݶ�����ƨ��\n���������Ӳ�����ݱ���ʽ��������������������ݳ�Ϊ��Ӱ��\n"
		"����������õ��������޷���������ά�޹�������ά�ޣ���ʱ��������Ǯ��\n�����������Ϊ������������Ҫ���ݶ�ʧ�����������±��ϰ峴����Ļ���\n"
		"�Ǿ�ȥ��վhttps://cdmxz.github.ioѰ������ɣ���ֻ��5�����ˣ����ͣ�\nʲô����û��������վ�ǲ���ȥ���ǾͲ��ܹ����ˣ�"
		"ֻ�ܹ����Լ�����̫�������~\nPS������վ�����벻��ȡһ��Ǯ���ã����ҪǮ���Ǿ���ƭ�ӣ������ϵ���" };

	char_to_wchar(warning_one, str, arr_count(str));
	i = MessageBox(NULL, str, L"���棡", MB_YESNO | MB_ICONWARNING);
	if (i == IDNO)
		return 0;
	else if (i == IDYES)
	{
		system("color 04");
		for (i = 0; i >= 0; i--)
		{
			system("cls");
			printf("\n\n\n\n\n\n\n\n%s", wanging_zero);
			printf("\n\n\n     ����%d�����룬�����˳��˳����رյ��Ի����ü���", i);
			Sleep(1000);
		}

		system("mode con:cols=70 lines=25 && title �����˳������򣬷������ĵ��Ժͱ�������ݶ�����ƨ��");// ���ô��ڴ�С 

		//URLDownloadToFileA(NULL, "https://ss0.bdstatic.com/70cFuHSh_Q1YnxGkpoWK1HF6hhy/it/u=784112224,522461964&fm=26&gp=0.jpg", fileName, 0, NULL);//����ͼƬ
		//RegCreateKeyA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System", &hkey);
		//RegSetValueExA(hkey, "Wallpaper", 0, REG_SZ, (BYTE*)fileName, sizeof(fileName));//�޸����汳��
		//RegSetValueExA(hkey, "WallpaperStyle", 0, REG_DWORD, &v, sizeof(DWORD));//�޸����汳��
		//RegCloseKey(hkey);

		system("shutdown -f -s -t  300");// �ػ�����
		char_to_wchar(warning_two, str, arr_count(str));
		MessageBox(NULL, str, L"��ܰ��ʾ�������濴�ꡣ", MB_OK | MB_ICONQUESTION);

		puts(warning_two);
		printf("\n\n\n\n\n\n���ĵ������˲����������˳������򣬷������ĵ��Ժͱ�������ݶ�����ƨ������\n\n");
		printf("\t\t��ܰ��ʾ������300���ʱ���������롣\n\n\n\t\t\t");
		system("pause");
		system("cls");

		for (i = 5; i >= 0; i--) //forѭ��
		{
			system("color 07");
			printf("��ܰ��ʾ����ֻʣ��%d�λ��ᡣ\n", i);
			printf("���������루�������������˰��س���:");

			scanf_s("%s", &temp, 7);
			rewind(stdin);// ��������������ַ�

			if (!strcmp(temp, password))// ��������бȽ�
			{
				system("color 0f && cls");
				printf("\n\n\n\n\n\n\n\n\n\t    ��ϲ����������ȷ�����ĵ��Ժͱ�������ݲ�����ƨ��\n\n");
				system("shutdown -a");
				system("pause");
				return 0;
			}
			else
			{
				printf("\n\t\t\t\t\t�������");
				system("color 0c");  // ����������ɫ
				Sleep(1000);
				system("cls");
			}
		}
		system("color 0a && cls");
		printf("\n\n\n\n\n\n\n\n\n    ���������������5��,���ĵ��Խ���ƨ�����ı�������Ҳ����ƨ��\n\n\n");
		system("pause");
	}
	return 0;
}

//int char_to_wchar(char* sourceStr, wchar_t* destStr)
//{
//    wchar_t* buff = NULL;
//    int len = MultiByteToWideChar(CP_ACP, 0, sourceStr, -1, NULL, 0);// ���Ŀ��������Ҫ�ĳ���
//    //if (wcslen(destStr) + 1 <= len)// ���Ŀ������С��ת������ĳ���
//    //	return;
//
//    buff = (wchar_t*)calloc(len, sizeof(wchar_t));
//    if (buff == NULL)
//        return -1;
//
//    MultiByteToWideChar(CP_ACP, 0, sourceStr, -1, buff, len);
//    wcscpy_s(destStr, len, buff);
//    free(buff);
//    return 0;
//}