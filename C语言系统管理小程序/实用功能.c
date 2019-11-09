#include <stdio.h>
#include <Windows.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <io.h>

#define PAI 3.14
#define WORDSIZE 105
#define MAXPATH 415  

//�����ļ�����
void encfile(void);    //�����ļ�����
void init(char filename[]);//��ʼ������Ϊ'\0'

//������
void cal(void);	    //������
void oper(void);    //��������
void square(void);  //��������
void bin(void);     //����ת��
void ascii(void);   //asciiת��
void circular(void);//Բ�������
void cube(void);    //���������
void cuboid(void);  //���������


//���������
void flush(void);   
//����������
int addstart(void);
//ɾ��������
int delstart(void);


int num;//����ȫ�ֱ���


//ʵ�ù���
void function(void)
{
	while (1)
	{
		system("mode con:cols=45 lines=20 && title ʵ�ù��� && cls");

		printf("\n\n\n\t      1���๦�ܼ�����\n\n");
		printf("\t      2�������ļ�����\n\n");
		printf("\t      3��������������\n\n");
		printf("\t      4��ɾ�����������\n\n");
		printf("\t      5������������\n\n\n");
		printf("    �����룺");

		while (scanf_s("%d", &num) != 1)
		{
			printf("\n������������ԣ�");
			flush();
		}
		flush();

		if (num == 1)
			cal();
		
		else if (num == 2)
			encfile();

		else if (num == 3)
			addstart();

		else if(num==4)
			delstart();

		else if (num == 5)
			break;

		else
		{
			printf("\n\t\t�������");
			Sleep(1000);
		}
	}
}


//�����ļ�����
void encfile(void)
{
	int num, ch;
	char filename[MAXPATH] = { 0 };   //��������ܻ�������ļ�����
	FILE *fpRead, *fpWrite;
	errno_t err;


	while (1)
	{
		init(filename);//��ÿһ��ѭ��ǰ��ʼ������Ԫ��

		system("title ��ӭʹ�ü��ܻ���ܳ��� && cls && mode con cols=50 lines=20");
		printf("\n��ֹ������Ҫ�ļ�����ֹ�����磡��ֹ��Υ��������£�\n");
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
			printf("4����ֹ������Ҫ�ļ�����ֹ�����磡��ֹ��Υ��������£�\n");
			printf("5������000������һ���档\n\n\n\n");
			printf("������������ļ������ƣ����磺C:\\1.txt��1.txt����");

			gets_s(filename, (MAXPATH - 5));//����������ļ�������
			//printf("���鳤�ȣ�%lu", strlen(filename));
			if (filename[0] == 34)//֧���ļ���ק����
			{
				filename[strlen(filename) - 1] = 0;
				strcpy_s(filename, MAXPATH, filename + 1);//�����ק����������˫����
			}

			if (!strcmp(filename, "000"))   //����000����������
				continue;

			if ((err = fopen_s(&fpRead, filename, "rb")) != 0)//�Զ��ķ�ʽ���ļ�
			{
				perror("Error:");
				system("pause");
				continue;
			}

			strcat_s(filename, MAXPATH, ".fh");//�����ܺ���ļ���Ӻ�׺��

			if ((err = fopen_s(&fpWrite, filename, "wb")) != 0)//��д�ķ�ʽ���ļ�
			{
				perror("Error:");
				system("pause");
				exit(1);
			}

			printf("\n\n\t\t\t���Ե�...");

			while ((ch = fgetc(fpRead)) != EOF)  //���ļ��ж�ȡ���ݵ�ch��EOF���ļ�������־��
			{
				ch = ch + ' ' - 9 + '9';//�����ļ�����
				fputc(ch, fpWrite);//������ܺ�����ݵ���һ���ļ�
			}


			if (fclose(fpRead) == EOF)//�ر��ļ�
			{
				MessageBox(NULL, TEXT("�رմ������ļ�ʧ�ܣ�\n�����Ƿ�����������ռ�ã�"), TEXT("����"), MB_OK | MB_ICONERROR);
				exit(1);
			}
			if (fclose(fpWrite) == EOF)//�ر��ļ�
			{
				MessageBox(NULL, TEXT("�ر��Ѽ����ļ�ʧ�ܣ�\n�����Ƿ�����������ռ�ã�"), TEXT("����"), MB_OK | MB_ICONERROR);
				exit(1);
			}

			system("cls");
			printf("\n\n\n�ѽ��ܣ�%s\n\n\t\t    ", filename);//����ļ���
			system("pause");
		}


		else if (num == 2)//�����ļ�����
		{
			unsigned len, j;


			system("title �����ļ����� && mode con cols=60 lines=20 && cls");

			printf("ע�⣺\n");
			printf("1�������ܵ��ļ����ļ����ƣ�����·������㡢��׺���ȣ�����205�����ڣ�����205�ֻᱨ�����ˡ�\n");
			printf("2�����ܺ���ļ���������ļ����뱣��ԭ��׺����.fh���������磺�ѡ�1.txt.fh�� ����Ϊ ��2.txt.fh������\n");
			printf("3����ֹ������Ҫ�ļ�����ֹ�����磡��ֹ��Υ��������£�\n");
			printf("4������000������һ���档\n\n\n\n");
			printf("������������ļ������ƣ����磺C:\\1.txt��1.txt.fh����");

			gets_s(filename, MAXPATH); //����������ļ�������
			if (filename[0] == 34)//֧���ļ���ק����
			{
				filename[strlen(filename) - 1] = 0;
				strcpy_s(filename, MAXPATH, filename + 1);//�����ק����������˫����
			}

			if (!strcmp(filename, "000"))//����000����������
				continue;

			if ((err = fopen_s(&fpRead, filename, "rb")) != 0) //�Զ��ķ�ʽ���ļ�
			{
				MessageBox(NULL, TEXT("���ļ�ʧ�ܣ�\n��������ܵ��ļ��Ƿ�ͱ�������ͬһĿ¼��\n�����ļ������Ƿ���ȷ��"), TEXT("����"), MB_OK | MB_ICONERROR);
				continue;
			}


			len = strlen(filename) - 1;//��ȡ���鳤��
			for (j = len; j > (len - 3); j--)//ɾ����.fh����׺��
				filename[j] = '\0';


			if (_access(filename, 0) != -1)//�ж���Ҫ���ܵ��ļ��Ƿ��ڵ�ǰ·���д���
			{
				printf("\n%s���ڵ�ǰ·���д��ڣ��Ƿ񸲸Ǵ��ļ���y/n����", filename);
				scanf_s("%c", &(char)num, 1);
				rewind(stdin);

				if (num == 121 || num == 89)
					;
				else if (num == 110 || num == 78)
					continue;
				else
				{
					printf("\n\t\t\t�������");
					Sleep(1000);
					continue;
				}
			}

			if ((err = fopen_s(&fpWrite, filename, "wb")) != 0)//��д�ķ�ʽ���ļ�
			{
				perror("Error:");
				system("pause");
				exit(1);
			}

			printf("\n\n\t\t\t���Ե�...");

			while ((ch = fgetc(fpRead)) != EOF)  //���ļ��ж�ȡ���ݵ�ch��EOF���ļ�������־��
			{
				ch = ch - ' ' + 9 - '9';//�����ļ�����
				fputc(ch, fpWrite);//������ܺ�����ݵ���һ���ļ�
			}


			if (fclose(fpRead) == EOF)//�ر��ļ�
			{
				MessageBox(NULL, TEXT("�رմ������ļ�ʧ�ܣ�\n�����Ƿ�����������ռ�ã�"), TEXT("����"), MB_OK | MB_ICONERROR);
				exit(1);
			}
			if (fclose(fpWrite) == EOF)//�ر��ļ�
			{
				MessageBox(NULL, TEXT("�ر��ѽ����ļ�ʧ�ܣ�\n�����Ƿ�����������ռ�ã�"), TEXT("����"), MB_OK | MB_ICONERROR);
				exit(1);
			}


			system("cls");
			printf("\n\n\n�Ѽ��ܣ�%s\n\n\t\t    ", filename);//����ļ���
			system("pause");
		}

		else if (num == 3)//��������
		{
			unsigned  i;		//strlen����һ���޷�������
			char word[WORDSIZE];

			system("title �������� && mode con cols=50 lines=20 && cls");

			printf("\n\n������Ҫ���ܵ����ģ�50�����ڣ������ո���ȣ���\n");
			gets_s(word, WORDSIZE);

			for (i = 0; i < strlen(word); i++)//�����ַ���
				word[i] += 1;

			printf("\n\n���ܺ�������ǣ�%s\n\n", word);
			system("pause");
		}

		else if (num == 4)//��������
		{
			unsigned  i;		//strlen����һ���޷�������
			char word[WORDSIZE];

			system("title �������� && mode con cols=50 lines=20 && cls");

			printf("\n\n������Ҫ���ܵ����ģ�50�����ڣ������ո���ȣ���\n");
			gets_s(word, WORDSIZE);

			for (i = 0; i < strlen(word); i++)//�����ַ���
				word[i] -= 1;

			printf("\n\n���ܺ�������ǣ�%s\n\n", word);
			system("pause");
		}


		else if (num == 5)//ʹ��˵��
			MessageBox(NULL, TEXT("ʹ�÷�����\n1.�ļ��������뷽ʽ��\n1��ֱ���������ļ���ק�����򴰿ڼ���\n2���������ļ����ƴ�����ֱ�������ļ�·�����磺C://1.txt��\n3���������ļ����ƴ�����ֱ�������ļ����ƣ��磺1.txt��\nע�⣺ʹ�õ�3�ַ�ʽ��Ҫ�ѱ�����ʹ�����/�������ļ��ŵ�ͬһ�ļ����ڡ�\n\n2.������������֧�������ļ���ʽ����ʵ�ʻ�����֤��\n\nע���������ϸ�Ķ��������ݣ���\n1.���ܻ�����ļ�ֻ����ļ����ݽ��м��ܻ���ܣ�\n������ļ����ƽ��м��ܻ���ܡ�\n\n2.���ܺ���ļ���������ļ����뱣��ԭ��׺����.fh����\n�����磺�ѡ�1.txt.fh�� ����Ϊ ��2.txt.fh������\n\n3.���ܺ���ļ��벻Ҫ���������ݣ�������ܺ��ļ����ָܻ�ԭ����\n\n4.�������ļ����ƣ�����·������㡢��׺���ȣ�����200�����ڣ�\n�������ļ����ƣ�����·������㡢��׺���ȣ�����205�����ڡ�\n\n5.�����ֻ�ʺ����֣�\n��ֹʹ�ñ����������Ҫ�ļ�����ֹʹ�ñ������Υ��������£�\n��ֹʹ�ñ����������磡\n��ʹ�ñ������ɵ��κκ��������ԭ���߳е���\nһ��ʹ�ñ��������Ϊͬ������\n���������и��£�ˡ������֪ͨ��"), TEXT("ʹ��˵��"), MB_OK);

		else
		{
			printf("\n\t�������");
			Sleep(1000);
		}

	}
}


//��ʼ������
void init(char filename[])
{
	int i;
	for (i = 0; i < MAXPATH; i++)
		filename[i] = 0;
}


//������
void cal(void)
{

	while (1)
	{
		system("mode con lines=20 cols=60 && cls && title �๦�ܼ�����");//���ñ���

		printf("\t\t----��ӭʹ�ö๦�ܼ�����----\n\n");
		printf("    \t\t       0��������һ����\n");
		printf("    \t\t       1����ͨ����\n");
		printf("    \t\t       2����������\n");
		printf("    \t\t       3������ת��\n");
		printf("    \t\t       4��ASCII��ת��\n");
		printf("    \t\t       5��Բ����/Բ����\n");
		printf("    \t\t       6��������/�����\n");
		printf("    \t\t       7��������/�����\n\n\n");
		printf("\t�����룺");
		while (scanf_s("%d", &num) != 1)
		{
			flush();
			printf("\n����������������룺");
		}
		flush();

		switch (num)
		{
		case 0: break;
		case 1:oper(); break;    //�������
		case 2:square(); break;  //��������
		case 3:bin(); break;     //����ת��
		case 4:ascii(); break;   //ASCII��ת��ת��
		case 5:circular(); break;//Բ���塢Բ����
		case 6:cube(); break;    //�����塢�μ���
		case 7:cuboid(); break;  //�����塢�μ���
		default:printf("\n\t�������"); Sleep(1000); break;
		}
		break;
	}

}


//��������
void oper(void)
{
	double a, b;
	double d = 0.0;
	char c;

	while (1)
	{
		system("cls && title ��������");

		printf("\t\t      ����0*0����\n\n");
		printf("�����硰3*4.12����5.88+2.1������������ʽ��");
		while (scanf_s("%lf %c %lf", &a, &c, 1, &b) != 3)
		{
			flush();
			printf("\n����������������룺");
		}
		flush();

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
		while (scanf_s("%lf", &x) != 1)
		{
			flush();
			printf("\n����������������룺");
		}
		flush();

		if (x == 0)//����0����
			break;

		printf("\n\t\t %.3lf�������ڣ�%.3lf\n\n", x, sqrt(x));
		system("pause");
	}
}


//����ת��
void bin(void)
{
	int Bin;

	while (1)
	{
		system("title ����ת�� && cls");
		printf("\n\n\t\t    1��ʮ����ת������\n\n");
		printf("\t\t    2��������תʮ����\n\n");
		printf("\t\t    3������������\n\n\n");
		printf("\t  �����룺");
		while (scanf_s("%d", &num) != 1)
		{
			printf("\n������������ԣ�");
			flush();
		}
		flush();

		if (num == 1)
		{
			int Dec = 0;
			int count[100];
			int i = 0;

			system("title ʮ����ת������ && cls");

			printf("\t\t\t����0����\n\n\n");
			printf("������һ��ʮ��������");
			while (scanf_s("%d", &Dec) != 1)//�����������
			{
				printf("\n�������,�����ԣ�");
				flush();
			}
			flush();

			if (Dec == 0)
				break;

			num = Dec;

			do   //ת��ʮ����
			{
				Bin = Dec % 2;  //��ÿһ�ε�����
				Dec = Dec / 2;
				i++;
				count[i] = Bin;
			} while (Dec != 0);

			printf("\n\nʮ���� %d ת���ɶ����Ƶ�ֵ�ǣ�", num);
			for (; i > 0; i--)   //������� 
				printf("%d", count[i]);

			printf("\n\n");
		}

		else if (num == 2)
		{
			long long dec, tmp, binry;
			double count = 0;
			int total = 0;

			system("title ������תʮ���� && cls");

			printf("\t\t\t����3����\n\n\n");
			printf("\n������һ������������");
			while (scanf_s("%lld", &dec) != 1)//�����������
			{
				printf("\n�������,�����ԣ�");
				flush();
			}
			flush();

			tmp = dec;
			if (dec == 3)//����3����
				break;

			while (dec)//ת�ɶ�����
			{
				binry = dec - (long long int)(dec / 10) * 10;
				count += binry * pow(2, total);
				total++;
				dec = (long long int)(dec / 10);
			}
			printf("\n\n������ %lld ת��Ϊʮ���Ƶ�ֵ�ǣ�%.0lf\n\n", tmp, count); //���
		}

		else if (num == 3)
			break;

		else
		{
			printf("\n\t�������");
			Sleep(1000);
		}
		system("pause");
	}
}


//ascii��ת��
void ascii(void)
{
	int ascii;
	char ch;

	while (1)
	{
		system("cls && title ASCII��ת��");
		printf("\n\n\n\t\t       0������������\n\n");
		printf("\t\t       1��ASCII��ת�ַ�\n\n");
		printf("\t\t       2���ַ�תASCII��\n\n\n");
		printf("\t  �����룺");
		while (scanf_s("%d", &num) != 1)//�����ַ�����
		{
			flush();	//��ջ�����
			printf("\n����������������룺");
		}
		flush();


		if (num == 0)
			break;

		if (num == 1)
		{
			system("cls");
			printf("\n\n\n");
			printf("��ע�⣺1-31��127���ǿ����ַ���\n\n");
			printf("��������Ҫת�����ַ���ASCII�룺");
			while (scanf_s("%d", &ascii) != 1)
			{
				printf("\n����������������룺");
				flush();	//��ջ�����
			}
			flush();


			if (ascii > 127)
			{
				printf("\n\n��������ܴ���127");
				Sleep(1000);
			}
			if (ascii < 0)
			{
				printf("\n\n������С����С��0");
				Sleep(1000);
			}

			printf("\n\n\n\n      ASCII�� %d ��Ӧ���ַ�Ϊ��%c\n\n", ascii, ascii);
			printf("������ʾ��������Ϊ�����ַ���\n\n");
		}

		else if (num == 2)
		{
			system("cls");
			printf("\n\n������Ҫת����ASCII����ַ���");
			scanf_s("%c", &ch, 1);
			flush();

			printf("\n\n\n\n     �ַ� %c ��Ӧ��ASCII��Ϊ��%d\n\n", ch, ch);
		}
		else
		{
			printf("\n\n\t\t�������");
			Sleep(2000);
		}
		system("pause");
	}
}


//Բ���塢Բ
void circular(void)
{
	double r, h;

	while (1)
	{
		system("cls && title Բ����/Բ�ļ���");
		printf("\t\t �ڵ�һ������������0����");
		printf("\n\n������Բ����ĵ���뾶\n��Բ�İ뾶����֪������1����");
		while (scanf_s("%lf", &r) != 1)
		{
			printf("\n������������ԣ�");
			flush();
		}
		flush();

		if (r == 0.0)//����0����
			break;

		printf("\n������Բ����ĸߣ���֪������1����");
		while (scanf_s("%lf", &h) != 1)
		{
			printf("\n������������ԣ�");
			flush();
		}
		flush();

		printf("\n\nԲ����ĵ����Ϊ��%.3lf\n\n", PAI*r*r);
		printf("Բ����ĵ����ܳ�Ϊ��%.3lf\n\n", 2 * PAI*r);
		printf("Բ����Ĳ����Ϊ��%.3lf\n\n", 2 * PAI*r*h);
		printf("Բ����ı����Ϊ��%.3lf\n\n", 2 * PAI*r*r + 2 * PAI*r*h);
		printf("Բ��������Ϊ��%.3lf\n\n", PAI*r*r*h);
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
		printf("\n\n�����������εı߳�\n����������ⳤ��");
		while (scanf_s("%lf", &edge) != 1)//�����������
		{
			printf("\n������������ԣ�");
			flush();
		}
		flush();

		if (edge == 0.0)
			break;

		printf("\n����������Ϊ��%.3lf\n\n", edge * edge * edge);
		printf("������ı����Ϊ��%.3lf\n\n", edge * edge * 6);
		printf("�����ε����Ϊ��%.3lf\n\n", edge * edge);
		printf("�����ε��ܳ�Ϊ��%.3lf\n\n", edge * 4);
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
		printf("\n\n������߶ȣ���֪������1����");
		while (scanf_s("%lf", &h) != 1)
		{
			printf("\n������������ԣ�");
			flush();
		}
		flush();

		if (h == 0.0)
			break;

		printf("\n�����볤�ȣ���֪������1����");
		while (scanf_s("%lf", &l) != 1)
		{
			printf("\n������������ԣ�");
			flush();
		}
		flush();

		printf("\n�������ȣ���֪������1����");
		while (scanf_s("%lf", &w) != 1)
		{
			printf("\n������������ԣ�");
			flush();
		}
		flush();

		printf("\n\n�����������ǣ�%.3lf\n\n", h*l*w);
		printf("�����εı�����ǣ�%.3lf\n\n", (l*w + l * h + w * h) * 2);
		printf("�����ε�����ǣ�%.3lf\n\n", l*w);
		printf("�����ε��ܳ��ǣ�%.3lf\n\n", (l*w + l * w) * 2);
		system("pause");
	}
}

//��ӳ���������
int addstart(void)
{
	system("title ������������ && cls");

	HKEY hKey;
	char *regPath = { "Software\\Microsoft\\Windows\\CurrentVersion\\Run" }; //ע���������·��
	char path[MAXPATH] = { 0 };//��Ҫ����������������·��
	char name[31] = { 0 };//ע�����������

	printf("\t    ����000������һ����\n\n");
	printf("��������Ҫ����������������·��\n�����磺H:\\test\\test.exe��\n ��");
	gets_s(path, MAXPATH);//����·��

	if (!strcmp(path, "000"))//����000�˳�
		return 0;


	printf("\n\n��������ӵ�ע���ļ�ֵ������\n\n����Ҫ��16���ַ��ڡ�\n  ����ʹ���ַ������֡���������ո�\n  ������ʹ�á�\\����\n\n ��");
	gets_s(name, 31);//��������


	//��ע��������� 
	if (RegOpenKeyEx(HKEY_CURRENT_USER, regPath, 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)
	{  //���һ����Key,������ֵ
		RegSetValueEx(hKey, name, 0, REG_SZ, (BYTE *)path, strlen(path));
		RegCloseKey(hKey);//�ر�ע���

		printf("\n\t         ��ӳɹ���\nע�⣺�벻Ҫɾ������ӵĳ��򣬷�����������ʧЧ��\n\n              ");
		system("pause");
		return 0;
	}
	else
	{
		MessageBox(NULL, TEXT("���ʧ�ܣ�"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
		return -1;
	}
	return 0;
}



//ɾ������������
int delstart(void)
{
	system("title ɾ������������ && cls");
    char path[120] = { "reg delete \"HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run\"  /v  \"" };
	char name[31] = { 0 }; //�洢����
	const char Backslash[] = { "\"" };//���淴б��
	
	printf("\t    ����000������һ����\n\n\n");
	printf("�������ֵ�����ƣ�16���ַ��ڣ�\n��");
	gets_s(name, 31);

	if (!strcmp(name, "000"))
		return 0;
	
	strcat_s(path, sizeof(path), name);
	strcat_s(path, sizeof(path), Backslash);

	
	printf("\n\n");
	system(path);
	printf("\n\n");
	system("pause");
	return 0;
}