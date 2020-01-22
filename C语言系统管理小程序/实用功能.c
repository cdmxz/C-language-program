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
void init(char *str); //��ʼ������Ϊ'\0'

//������
int  cal(void);	     //������
void oper(void);     //��������
void square(void);   //��������
void bin(void);      //������תʮ����
void dec(void);		 //ʮ����ת������
void ascii(void);    //ascii��ת�ַ�
void character(void);//�ַ�תascii��
void circular(void); //Բ�������
void cube(void);     //���������
void cuboid(void);   //���������

//���������
void flush(void);   
//����������
int addstart(void);
//ɾ��������
int delstart(void);
//ȥ��fgets�Ļ��з�
void fgets_n(char *n);

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
	int ch;
	char filename[MAXPATH] = { 0 };   //��������ܻ�������ļ�����
	FILE *fpread, *fpwrite;
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
			flush();
			printf("\n����������������룺");
		}
		flush();

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

			fgets(filename, (MAXPATH - 5), stdin);//����������ļ�������
			fgets_n(filename);

			if (filename[0] == 34)//֧���ļ���ק����
			{
				filename[strlen(filename) - 1] = 0;
				strcpy_s(filename, strlen(filename) + 1, filename + 1);//�����ק����������˫����
			}

			if (!strcmp(filename, "000"))   //����000����������
				continue;

			if ((err = fopen_s(&fpread, filename, "rb")) != 0)//�Զ��ķ�ʽ���ļ�
			{
				perror("\n���󣨿�������ʹ�÷���������룩");
				system("pause");
				continue;
			}

			strcat_s(filename, MAXPATH, ".fh");//�����ܺ���ļ���Ӻ�׺��

			if ((err = fopen_s(&fpwrite, filename, "wb")) != 0)//��д�ķ�ʽ���ļ�
			{
				perror("\n���󣨿�������ʹ�÷���������룩");
				system("pause");
				exit(EXIT_FAILURE);
			}

			printf("\n\n\t\t\t���Ե�...");

			while ((ch = fgetc(fpread)) != EOF)  //���ļ��ж�ȡ���ݵ�ch��EOF���ļ�������־��
				fputc(~ch, fpwrite);//�����ݽ���ȡ����������������ܺ�����ݵ���һ���ļ�


			fclose(fpread);
			fclose(fpwrite);

			system("cls");
			printf("\n\n\n�Ѽ��ܣ�%s\n\n\t\t    ", filename);//����ļ���
			system("pause");
		}


		else if (num == 2)//�����ļ�����
		{
			unsigned len, j;
			char ch;

			system("title �����ļ����� && mode con cols=60 lines=20 && cls");

			printf("ע�⣺\n");
			printf("1�������ܵ��ļ����ļ����ƣ�����·������㡢��׺���ȣ�����205�����ڣ�����205�ֻᱨ�����ˡ�\n");
			printf("2�����ܺ���ļ���������ļ����뱣��ԭ��׺����.fh���������磺�ѡ�1.txt.fh�� ����Ϊ ��2.txt.fh������\n");
			printf("3����ֹ������Ҫ�ļ�����ֹ�����磡��ֹ��Υ��������£�\n");
			printf("4������000������һ���档\n\n\n\n");
			printf("������������ļ������ƣ����磺C:\\1.txt��1.txt.fh����");

			fgets(filename, MAXPATH, stdin); //����������ļ�������
			fgets_n(filename);

			if (filename[0] == 34)//֧���ļ���ק����
			{
				filename[strlen(filename) - 1] = 0;
				strcpy_s(filename, strlen(filename) + 1, filename + 1);//�����ק����������˫����
			}
			puts(filename);

			if (!strcmp(filename, "000"))//����000����������
				continue;

			if ((err = fopen_s(&fpread, filename, "rb")) != 0) //�Զ��ķ�ʽ���ļ�
			{
				MessageBox(NULL, TEXT("���ļ�ʧ�ܣ�\n��������ܵ��ļ��Ƿ�ͱ�������ͬһĿ¼��\n�����ļ������Ƿ���ȷ��"), TEXT("Error"), MB_OK | MB_ICONERROR);
				continue;
			}


			len = strlen(filename) - 1;//��ȡ���鳤��
			for (j = len; j > (len - 3); j--)//ɾ����.fh����׺��
				if(filename[j] == 'h' || filename[j] == 'f' || filename[j] == '.')
				filename[j] = '\0';


			if (_access(filename, 0) != -1)//�ж���Ҫ���ܵ��ļ��Ƿ��ڵ�ǰ·���д���
			{
				printf("\n%s���ڵ�ǰ·���д��ڣ��Ƿ񸲸Ǵ��ļ���y/n����", filename);
				scanf_s("%c", &ch, 1);
				flush();

				if (ch == 121 || ch == 89)
					;
				else if (ch == 110 || ch == 78)
					continue;
				else
				{
					printf("\n\t\t\t�������");
					Sleep(1000);
					continue;
				}
			}

			if ((err = fopen_s(&fpwrite, filename, "wb")) != 0)//��д�ķ�ʽ���ļ�
			{
				perror("\n���󣨿�������ʹ�÷���������룩");
				system("pause");
				exit(EXIT_FAILURE);
			}

			printf("\n\n\t\t\t���Ե�...");

			while ((ch = fgetc(fpread)) != EOF)  //���ļ��ж�ȡ���ݵ�ch
				fputc(~ch, fpwrite);//�����ݽ���ȡ����������������ܺ�����ݵ���һ���ļ�


			fclose(fpread);//�ر��ļ�
			fclose(fpwrite);
			
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
			fgets(word, WORDSIZE, stdin);
			fgets_n(word);

			for (i = 0; i < strlen(word); i++)//�����ַ���
				word[i] += 115;
			
			//д���ļ�
			if ((err = fopen_s(&fpwrite, "���ܺ������.txt", "w")) != 0) 
			{
				i = MessageBox(NULL, TEXT("�����ļ�ʧ�ܣ�\n�Ƿ�ֱ�����루���ܻᵼ����ʾ����ȫ����"), TEXT("Error"), MB_YESNO | MB_ICONERROR);
				if(i == IDNO)
					continue;
				else if (i == IDYES)
					printf("\n\n���ܺ�������ǣ�%s\n\n", word);
				continue;
			}
			fputs(word, fpwrite);//�����ܺ������д���ļ�
			fclose(fpwrite);
			printf("\n\n��򿪱�����Ŀ¼�еġ����ܺ������.txt���鿴\n\n");
			system("start ���ܺ������.txt");
			system("pause");
		}

		else if (num == 4)//��������
		{
			unsigned  i;		//strlen����һ���޷�������
			char word[WORDSIZE];

			system("title �������� && mode con cols=50 lines=20 && cls");

			printf("\n\n������Ҫ���ܵ����ģ�50�����ڣ������ո���ȣ���\n");
			fgets(word, WORDSIZE, stdin);
			fgets_n(word);

			for (i = 0; i < strlen(word); i++)//�����ַ���
				word[i] -= 115;
			
			//д���ļ�
			if ((err = fopen_s(&fpwrite, "���ܺ������.txt", "w")) != 0) 
			{
				i = MessageBox(NULL, TEXT("�����ļ�ʧ�ܣ�\n�Ƿ�ֱ����ʾ�����ܻᵼ����ʾ����ȫ����"), TEXT("Error"), MB_YESNO | MB_ICONERROR);
				if (i == IDNO)
					continue;
				else if (i == IDYES)
					printf("\n\n���ܺ�������ǣ�%s\n\n", word);
				continue;
			}
			fputs(word, fpwrite);//�����ܺ������д���ļ�
			fclose(fpwrite);
			printf("\n\n��򿪱�����Ŀ¼�еġ����ܺ������.txt���鿴\n\n");
			system("start ���ܺ������.txt");
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
void init(char *str)
{
	while (*str != '\0')
		*str++ = '\0';
}


//������
int cal(void)
{
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
			flush();
			printf("\n����������������룺");
		}
		flush();
		

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
	double a, b, d;
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


//ʮ����ת������
void dec(void)
{
	while (1)
	{
		system("title ʮ����ת������ && cls");
		long long temp, dec = 0;
		int count[256] = { 0 };
		int bin, i = 0;

		printf("\t\t\t����0����\n\n\n");
		printf("������һ��ʮ��������");
		while (scanf_s("%lld", &dec) != 1)//�����������
		{
			printf("\n�������,�����ԣ�");
			flush();
		}
		flush();

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
	int i = 0;

	while (1)
	{
		int bin;
		long long dec = 0;
		char ch;

		system("title ������תʮ���� && cls");

		printf("\n\n\n������һ������������");
		while ((bin = getchar()) != '\n')
			dec = dec * 2 + (bin - '0');
		printf("\n\nת��Ϊʮ���Ƶ�ֵ�ǣ�%lld\n\n", dec); //���
		
		i++;

		if (i == 3)
		{
			printf("\n\n\t       �Ƿ񷵻���һ���棿��Y/N��");
			scanf_s("%c", &ch, 1);
			flush();
			if (ch == 121 || ch == 89)
				break;
			else if (ch == 110 || ch == 78)
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


//ascii��ת�ַ�
void ascii(void)
{
	int ascii, i = 0;
	char ch;

	while (1)
	{
		system("cls && title ASCII��ת�ַ�");

		printf("\n\n\n");
		printf("��ע�⣺1-31��127���ǿ����ַ���\n\n");
		printf("��������Ҫת�����ַ���ASCII�룺");
		while (scanf_s("%d", &ascii) != 1)
		{
			printf("\n����������������룺");
			flush();	//��ջ�����
		}
		flush();

		if (ascii == 0)
			break;

		printf("\n\n\n\n      ASCII�� %d ��Ӧ���ַ�Ϊ��%c\n\n", ascii, ascii);
		printf("������ʾ��������Ϊ�����ַ���\n\n");

		i++;
		if (i == 3)
		{
			printf("\n\n\t       �Ƿ񷵻���һ���棿��Y/N��");
			scanf_s("%c", &ch, 1);
			flush();
			if (ch == 121 || ch == 89)
				break;
			else if (ch == 110 || ch == 78)
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


//�ַ�תascii��
void character(void)
{
	char ch;
	int i = 0;

	while (1)
	{
		system("cls && title �ַ�תASCII��");
		printf("\n\n������Ҫת����ASCII����ַ���");
		scanf_s("%c", &ch, 1);
		flush();

		printf("\n\n\n\n     �ַ� %c ��Ӧ��ASCII��Ϊ��%d\n\n", ch, ch);
		i++;

		if (i == 3)
		{
			printf("\n\n\t       �Ƿ񷵻���һ���棿��Y/N��");
			scanf_s("%c", &ch, 1);
			flush();
			if (ch == 121 || ch == 89)
				break;
			else if (ch == 110 || ch == 78)
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
	double r, h;

	while (1)
	{
		system("cls && title Բ����/Բ�ļ���");
		printf("\t\t �ڵ�һ������������0����");
		printf("\n\n������Բ����ĵ���뾶\n��Բ�İ뾶����λ�����ף���");
		while (scanf_s("%lf", &r) != 1)
		{
			printf("\n������������ԣ�");
			flush();
		}
		flush();

		if (r == 0.0)//����0����
			break;

		printf("\n������Բ����ĸ�\n������Բ��������ܳ�������1����λ�����ף���");
		while (scanf_s("%lf", &h) != 1)
		{
			printf("\n������������ԣ�");
			flush();
		}
		flush();

		printf("\nԲ�����Ϊ��%.3lf ƽ������\n", PAI*r*r);
		printf("Բ���ܳ�Ϊ��%.3lf ����\n\n", 2 * PAI*r);
		printf("Բ����ĵ����Ϊ��  %.3lf ƽ������\n", PAI*r*r);
		printf("Բ����ĵ����ܳ�Ϊ��%.3lf ����\n", 2 * PAI*r);
		printf("Բ��������Ϊ��    %.3lf ��������\n", PAI*r*r*h);
		printf("Բ����Ĳ����Ϊ��  %.3lf ƽ������\n", 2 * PAI*r*h);
		printf("Բ����ı����Ϊ��  %.3lf ƽ������\n\n", 2 * PAI*r*r + 2 * PAI*r*h);
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
			flush();
		}
		flush();

		if (edge == 0.0)
			break;

		printf("\n\n\n�����ε����Ϊ��%.3lf ƽ������\n", edge * edge);
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
			flush();
		}
		flush();
		if (l == 0.0)
			break;

		printf("\n�����볤���λ򳤷���Ŀ���λ�����ף���");
		while (scanf_s("%lf", &w) != 1)
		{
			printf("\n������������ԣ�");
			flush();
		}
		flush();

		printf("\n�����볤����ĸ�\n��ֻ���㳤����������1����λ�����ף���");
		while (scanf_s("%lf", &h) != 1)
		{
			printf("\n������������ԣ�");
			flush();
		}
		flush();

		printf("\n\n�����ε�����ǣ�%.3lf ƽ������\n", l*w);
		printf("�����ε��ܳ��ǣ�%.3lf ����\n\n", (l*w + l * w) * 2);
		printf("�����������ǣ�  %.3lf ��������\n", h*l*w);
		printf("������ı�����ǣ�%.3lf ƽ������\n\n", (l*w + l * h + w * h) * 2);
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
	fgets(path, MAXPATH, stdin);//����·��
	fgets_n(path);

	if (!strcmp(path, "000"))//����000�˳�
		return 0;


	printf("\n\n��������ӵ�ע���ļ�ֵ������\n\n����Ҫ��15���ַ��ڡ�\n  ����ʹ���ַ������֡���������ո�\n  ������ʹ�á�\\����\n\n ��");
	fgets(name, 31, stdin);//��������
	fgets_n(name);

	//��ע��������� 
	if (RegOpenKeyEx(HKEY_CURRENT_USER, regPath, 0, KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS)
	{
		MessageBox(NULL, TEXT("���ʧ�ܣ�"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
		return -1;
	}
	//���һ����Key,������ֵ
	if (RegSetValueEx(hKey, name, 0, REG_SZ, (BYTE *)path, strlen(path)) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);//�ر�ע���
		MessageBox(NULL, TEXT("���ʧ�ܣ�"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
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
	char name[31] = { 0 }; //�洢����
	HKEY hKey;
	char *regPath = { "Software\\Microsoft\\Windows\\CurrentVersion\\Run" }; //ע���������·��

	printf("\t    ����000������һ����\n\n\n");
	printf("�������ֵ�����ƣ�15���ַ��ڣ���");
	fgets(name, 31, stdin);
	fgets_n(name);

	if (!strcmp(name, "000"))
		return 0;

	//��ע��������� 
	if (RegOpenKeyEx(HKEY_CURRENT_USER, regPath, 0, KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS || RegDeleteValue(hKey, name) != ERROR_SUCCESS)
	{
		MessageBox(NULL, TEXT("ɾ��ʧ�ܣ�\n���Թ���ԱȨ�����б�������ԣ�"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
		return -1;
	}
	printf("\n\n");
	printf("\t\tɾ���ɹ���");
	printf("\n\n\t      ");
	system("pause");
	return 0;
}


//ɾ��fgets��ȡ�Ļ��з�
void fgets_n(char *n)
{
	char *find;
	find = strchr(n, '\n');  //����'\n'
	if (find)                          
		*find = '\0';
}


//�ʵ�
int autoshut(void)
{
	system("title �����˳��˳����رյ��Ի����ü���");

	int i;
	char password[7] = { "115-56" };//����
	char temp[7];

	char wanging_zero[] = { "���棺�����ݲ��ʺ��и��Լ��������ಡ����Ѫѹ������Ѫ�ܲ����񾭴��ҡ���������ơ������ߵ���ʿ���ʣ�"
		"�����˳��˳����رյ��Ի����ü������ӲҪ���ʣ�����Ը���" };

	char warning_one[] = { "���棺�����ݲ��ʺ��и��Լ��������ಡ����Ѫѹ������Ѫ�ܲ����񾭴��ҡ���������ơ������ߵ���ʿ���ʣ�"
		"�����˳��˳����رյ��Ի����ü������ӲҪ���ʣ�����Ը���\n\t\t\t\t�Ƿ�������ʣ�" };

	char warning_two[] = { "���ĵ������˲����������˳������򣬷������ĵ��Ժͱ�������ݶ�����ƨ��\n���������Ӳ�����ݱ���ʽ��������������������ݳ�Ϊ��Ӱ��\n"
		"����������õ��������޷���������ά�޹�������ά�ޣ���ʱ��������Ǯ��\n�����������Ϊ������������Ҫ���ݶ�ʧ�����������±��ϰ峴����Ļ���\n"
		"�Ǿ�ȥ��վhttps://cdmxz.github.ioѰ������ɣ���ֻ��5�����ˣ����ͣ�\nʲô����û��������վ�ǲ���ȥ���ǾͲ��ܹ����ˣ�"
		"ֻ�ܹ����Լ�����̫�������~\nPS������վ�����벻��ȡһ��Ǯ���ã����ҪǮ���Ǿ���ƭ�ӣ������ϵ���" };

	i = MessageBox(NULL, (warning_one), TEXT("���棡"), MB_YESNO | MB_ICONWARNING);
	if (i == IDNO)
		return 0;

	else if (i == IDYES)
	{
		system("color 04");
		for (i = 10; i >= 0; i--)
		{
			system("cls");
			printf("\n\n\n\n\n\n\n\n%s", wanging_zero);
			printf("\n\n\n     ����%d�����룬�����˳��˳����رյ��Ի����ü���", i);
			Sleep(1000);
		}


		system("mode con:cols=70 lines=25 && title �����˳������򣬷������ĵ��Ժͱ�������ݶ�����ƨ��");//���ô��ڴ�С 

		system("shutdown -s -t 300");//�ػ�����
		MessageBox(NULL, (warning_two), TEXT("��ܰ��ʾ�������濴�ꡣ"), MB_OK | MB_ICONQUESTION);

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
			flush();//��������������ַ�

			if (!strcmp(temp, password))//��������бȽ�
			{
				system("color 0f && cls");
				printf("\n\n\n\n\n\n\n\n\n\t    ��ϲ����������ȷ�����ĵ��Ժͱ�������ݲ�����ƨ��\n\n");
				system("shutdown -a");
				system("pause");
				return 0;  //���� 
			}
			else
			{
				printf("\n\t\t\t\t\t�������");
				system("color 0c");  //����������ɫ
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


/*//���������
void flush(void)
{
	int ch;
	while ((ch = getchar()) != '\n' && ch != EOF)
		;
}*/