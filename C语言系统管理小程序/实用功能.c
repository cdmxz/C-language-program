#include <stdio.h>
#include <Windows.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


#define PAI 3.14
#define ENCSIZE 163
#define DECSIZE 170
#define TXTSIZE 105
#define MAXPATH 1024  

//�����ļ�����
void EncFile(void);    //�����ļ�����
void init(char  encry[], char  encname[], char  decry[]);//��ʼ������Ϊ'\0'
void Text(char TXT[]);//����������'\n'
void flush(void);//���������

//������
void  cal(void);	//������
void oper(void);    //��������
void square(void);  //��������
void bin(void);     //����ת��
void ascii(void);   //asciiת��
void circular(void);//Բ�������
void cube(void);    //���������
void cuboid(void);  //���������
void cst(void);     //���Ǻ���
void flush(void);   //���������

//����������
int addStart(void);
//ɾ��������
void delStart(void);


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
			EncFile();

		else if (num == 3)
			addStart();

		else if(num==4)
			delStart();

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
void EncFile(void)
{
	int ch, i;

	while (1)
	{
		system("title ��ӭʹ�ü��ܽ��ܳ��� && mode con cols=44 lines=18 && cls");
		printf("\n\n\t     0.����������\n\n");
		printf("\t     1.���ܻ�����ļ�����\n\n");
		printf("\t     2.�������Ļ��������\n\n");
		printf("\t     3.�鿴ʹ��˵��\n\n\n");
		printf(" ��������ţ�");
		while (scanf_s("%d", &num) != 1)//�����ַ�����
		{
			flush();
			printf("\n����������������룺");
		}
		flush();
			
		if (num == 0)
			break;

//���ܻ�����ļ�����
		else if (num == 1)
		{
			char encry[ENCSIZE];   //����������ļ�����
			char encname[DECSIZE]; //����������ļ����ܺ������
			char decry[DECSIZE];   //����������ļ�����
			char enc[] = "�Ѽ��� ";
			char dec[] = "�ѽ��� ";
			FILE *fpread, *fpwrite;
			errno_t err;
	

			while (1)
			{
				init(encry, encname, decry);//��ÿһ��ѭ��ǰ��ʼ������Ԫ��

				system("title ���ܻ�����ļ����� && cls && mode con cols=50 lines=18");
				printf("\n��ֹ������Ҫ�ļ�����ֹ�����磡��ֹ��Υ��������£�\n");
				printf("\n\n\t         1�������ļ�����\n\n");
				printf("\t         2�������ļ�����\n\n");
				printf("\t         3������������\n\n");
				printf("\t  �����룺");
				while (scanf_s("%d", &num) != 1)
				{
					flush();
					printf("\n����������������룺");
				}
				flush();
	

				if (num == 1)  //�����ļ�����
				{
					system("title �����ļ����� && mode con cols=60 lines=20 && cls");
					printf("ע�⣺\n1�����ܺ���ļ����ܸ���ԭ�������ݣ�������ܺ��ָܻ�ԭ����\n");
					printf("2�����ܺ���ļ���������ļ����뱣��ԭ��ʽ�������磺�ѡ��Ѽ��� 1.txt�� ����Ϊ ���Ѽ��� 2.txt������\n");
					printf("3�������ܵ��ļ���Ҫ�뱾������ͬһĿ¼�£������ļ�����������׺���Ϳո�ȣ���80�����ڣ�����80�ֻᱨ�����ˡ�\n");
					printf("4����ֹ������Ҫ�ļ�����ֹ�����磡��ֹ��Υ��������£�\n");
					printf("5������000���������档\n\n\n\n");
					printf("������������ļ������ƣ����磺1.txt����");

					fgets(encry, ENCSIZE, stdin);//����������ļ�������
					flush();//���������


					for (i = 0; i <= ENCSIZE; i++) //ȥ��������Ļ��з�
						if (encry[i] == '\n')
							encry[i] = '\0';

					if (!strcmp(encry, "000"))   //����000����������
						continue;

					if ((err = fopen_s(&fpread, encry, "rb")) != 0)//�Զ�ȡ�ķ�ʽ��һ���������ļ�
					{
						MessageBox(NULL, TEXT("�򿪴������ļ�ʧ�ܣ�\n��������ܵ��ļ��Ƿ�ͱ�������ͬһĿ¼��\n�����ļ������Ƿ���ȷ��"), TEXT("����"), MB_OK | MB_ICONERROR);
						continue;
					}

					strcat_s(encname, sizeof(encname), enc);//�ϲ��ļ�����
					strcat_s(encname, sizeof(encname), encry);

					if ((err = fopen_s(&fpwrite, encname, "wb")) != 0)//��д��ķ�ʽ��һ���������ļ�
					{
						perror("Error:");
						system("pause");
						//MessageBox(NULL, TEXT("�����ļ�ʧ�ܣ�\n���Թ���ԱȨ�����б������"), TEXT("����"), MB_OK | MB_ICONERROR);
						exit(1);
					}

					while ((ch = fgetc(fpread)) != EOF)  //���ļ��ж�ȡ���ݵ�ch��EOF���ļ�������־��
					{
						ch = ch + ' ' - 9 + '9';//�����ļ�����
						fputc(ch, fpwrite);//������ܺ�����ݵ���һ���ļ�
					}


					if (fclose(fpread) == EOF)//�ر��ļ�
					{
						MessageBox(NULL, TEXT("�رմ������ļ�ʧ�ܣ�\n�����Ƿ�����������ռ�ã�"), TEXT("����"), MB_OK | MB_ICONERROR);
						exit(1);
					}
					if (fclose(fpwrite) == EOF)//�ر��ļ�
					{
						MessageBox(NULL, TEXT("�ر��Ѽ����ļ�ʧ�ܣ�\n�����Ƿ�����������ռ�ã�"), TEXT("����"), MB_OK | MB_ICONERROR);
						exit(1);
					}

					system("cls");
					printf("\n\n\n");
					fputs(encname, stdout);//����ļ���
					putchar('\n');
					system("pause");
				}


				else if (num == 2)//�����ļ�����
				{
					system("title �����ļ����� && mode con cols=60 lines=20 && cls");
					
					printf("ע�⣺\n");
					printf("1�������ܵ��ļ���Ҫ�뱾������ͬһĿ¼�£������ļ�����������׺���Ϳո�ȣ���85�����ڣ�����85�ֻᱨ�����ˡ�\n");
					printf("2�����ܺ���ļ���������ļ����뱣��ԭ��ʽ�������磺�ѡ��Ѽ��� 1.txt�� ����Ϊ ���Ѽ��� 2.txt������\n");
					printf("3����ֹ������Ҫ�ļ�����ֹ�����磡��ֹ��Υ��������£�\n");
					printf("4������000���������档\n\n\n\n");
					printf("������������ļ������ƣ����磺1.txt����");

					fgets(decry, DECSIZE, stdin); //����������ļ�������
					flush();//���������

					for (i = 0; i <= 170; i++)//ȥ��������Ļ��з�
						if (decry[i] == '\n')
							decry[i] = '\0';

					if (!strcmp(decry, "000"))//����000����������
						continue;

					if ((err = fopen_s(&fpread, decry, "rb")) != 0) //�Զ�ȡ�ķ�ʽ��һ���������ļ�
					{
						MessageBox(NULL, TEXT("���ļ�ʧ�ܣ�\n��������ܵ��ļ��Ƿ�ͱ�������ͬһĿ¼��\n�����ļ������Ƿ���ȷ��"), TEXT("����"), MB_OK | MB_ICONERROR);
						continue;
					}

					for (i = 0; i <= 5; i++)//forѭ���滻�ļ���ǰ�����ַ��������ļ����Ƶ��Ѽ����滻���ѽ��ܡ�
						decry[i] = dec[i];

					if ((err = fopen_s(&fpwrite, decry, "wb")) != 0)//��д��ķ�ʽ��һ���������ļ�
					{
						perror("Error:");
						system("pause");
						//MessageBox(NULL, TEXT("�����ļ�ʧ�ܣ�\n���Թ���ԱȨ�����б������"), TEXT("����"), MB_OK | MB_ICONERROR);
						exit(1);
					}

					while ((ch = fgetc(fpread)) != EOF)  //���ļ��ж�ȡ���ݵ�ch��EOF���ļ�������־��
					{
						ch = ch - ' ' + 9 - '9';//�����ļ�����
						fputc(ch, fpwrite);//������ܺ�����ݵ���һ���ļ�
					}


					if (fclose(fpread) == EOF)//�ر��ļ�
					{
						MessageBox(NULL, TEXT("�رմ������ļ�ʧ�ܣ�\n�����Ƿ�����������ռ�ã�"), TEXT("����"), MB_OK | MB_ICONERROR);
						exit(1);
					}
					if (fclose(fpwrite) == EOF)//�ر��ļ�
					{
						MessageBox(NULL, TEXT("�ر��ѽ����ļ�ʧ�ܣ�\n�����Ƿ�����������ռ�ã�"), TEXT("����"), MB_OK | MB_ICONERROR);
						exit(1);
					}


					system("cls");
					printf("\n\n\n");
					fputs(decry, stdout);//����ļ���
					putchar('\n');
					system("pause");
				}

				else if (num == 3)
				break;

				else
				{
					printf("\n\t\t�������");
					Sleep(1000);
				}
			}
		}
//�������Ļ��������
		else if (num == 2)
		{
			unsigned  i;		//strlen����һ���޷�������
			char TXT[TXTSIZE];

			while (1)
			{
				system("title �������Ļ�������� && cls && mode con cols=50 lines=18");
				printf("\n\n\n\t\t  1����������\n\n");
				printf("\t\t  2����������\n\n");
				printf("\t\t  3������������\n\n");
				printf("  ��������ţ�");	
				while (scanf_s("%d", &num) != 1)
				{
					flush();
					printf("\n����������������룺");
				}
				flush();


				if (num == 1)//��������
				{
					system("title �������� && mode con cols=50 lines=20 && cls");
					printf("\n\n������Ҫ���ܵ����ģ�50�����ڣ������ո���ȣ���\n");
					fgets(TXT, TXTSIZE, stdin);
					flush();
					

					Text(TXT); //����������'\n'
					for (i = 0; i < strlen(TXT); i++)
						TXT[i] += 1;
									
					printf("\n\n���ܺ�������ǣ�%s\n\n", TXT);
					system("pause");
				}

				else if (num == 2)//��������
				{
					system("title �������� && mode con cols=50 lines=20 && cls");
					printf("\n\n������Ҫ���ܵ����ģ�50�����ڣ������ո���ȣ���\n");
					fgets(TXT, TXTSIZE, stdin);
					flush();
					

					Text(TXT); //����������'\n'
					for (i = 0; i < strlen(TXT); i++)
						TXT[i] -= 1;
									
					printf("\n\n���ܺ�������ǣ�%s\n\n", TXT);
					system("pause");
				}

				else if (num == 3)//����������
					break;

				else
				{
					printf("\n\n\t�������\n");
					Sleep(1000);
				}
			}
		}
//ʹ��˵��
		else if (num == 3)
			MessageBox(NULL, TEXT("����ϸ�Ķ��������ݣ�\n1.���ܻ�����ļ�ֻ����ļ����ݽ��м��ܻ���ܣ�\n������ļ����ƽ��м��ܻ���ܡ�\n\n2.�����ļ����ݻ�����ļ�����ǰ��ȷ�������ܻ�����ܵ��ļ�\nû�б���������ռ�ò��Һͱ�������ͬһĿ¼��\n\n3.���ܺ���ļ��벻Ҫ���������ݣ�������ܺ��ļ����ָܻ�ԭ����\n\n4.�������ļ����ƣ�������׺���Ϳո�ȣ�����80�����ڣ�\n�������ļ����ƣ�������׺���Ϳո�ȣ�����85�����ڡ�\n\n5.��ֹʹ�ñ����������Ҫ�ļ�����ֹʹ�ñ������Υ��������£�\n��ֹʹ�ñ����������磡\n��ʹ�ñ������ɵ��κκ��������ԭ���߳е���\nһ��ʹ�ñ��������Ϊͬ������\n���������и��£�ˡ������֪ͨ��"), TEXT("ʹ��˵��"), MB_OK);


		else
		{
			printf("\n\t�������");
			Sleep(1000);
		}
	}
}


//��ʼ������
void init(char encry[], char encname[], char decry[])
{
	int i;

	for (i = 0; i < ENCSIZE; i++)
		encry[i] = 0;

	for (i = 0; i < DECSIZE; i++)
	{
		encname[i] = 0;
		decry[i] = 0;
	}
}


//����������'\n'
void Text(char TXT[])
{
	int i;
	for (i = 0; i < TXTSIZE; i++)
		if (TXT[i] == '\n')
			TXT[i] = '\0';
}


//������
void cal(void)
{

	while (1)
	{
		system("mode con lines=20 cols=60 && cls && title �๦�ܼ�����");//���ñ���

		printf("\t\t----��ӭʹ�ö๦�ܼ�����----\n\n");
		printf("    \t\t       0������������\n");
		printf("    \t\t       1����ͨ����\n");
		printf("    \t\t       2����������\n");
		printf("    \t\t       3������ת��\n");
		printf("    \t\t       4��ASCII��ת��\n");
		printf("    \t\t       5��Բ����/Բ����\n");
		printf("    \t\t       6��������/�����\n");
		printf("    \t\t       7��������/�����\n");
		printf("    \t\t       8�����Ǻ�������\n\n\n");
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
		case 8:cst(); break;	 //���Ǻ�������
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
			printf("\n\n������ %lld ת��Ϊʮ���Ƶ�ֵ�ǣ�%.0lf\n\n", tmp, count); //��ӡ
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


			if (ascii >= 128)
			{
				printf("\n\n��������ܴ���127");
				Sleep(1000);
			}
			if (ascii <= -1)
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


//���Ǻ���
void cst(void)
{
	//int cst;
	

	while (1)
	{
		printf("\n\n\t\t��������δ����\n\n");
		system("pause");
		break;

		/*
		system("cls && title ���Ǻ��� && mode con cols=30 lines=15");
		printf("\n\n\t   0������\n");
		printf("\t   1��sin\n");
		printf("\t   2��cos\n");
		printf("\t   3��tan\n");
		printf("    �����룺");
		while (scanf_s("%d", &cst) != 1)
		{
			printf("\n����������������룺");
			flush();
		}
		flush();

		if (cst == 0)
			break;

		else if (cst == 1)
		{
			system("cls");
			printf("\n�����룺");
			while (scanf_s("%lf", &a) != 1)
			{
				printf("\n����������������룺");
				flush();
			}
			flush();
			//�Ƕ�ת��Ϊ����
			printf("%.4lf", sin(a * PAI / 180));
			system("pause");
		}

		else if (cst == 2)
			system("cls");
		else if (cst == 3)
			system("cls");

		else
		{
			printf("\n\t�������");
			Sleep(1000);
		}*/
	}
}


//��ӳ���������
int addStart(void)
{
	system("title ������������ && cls");

	int i, j;
	HKEY hKey;
	char *regpPath = { "Software\\Microsoft\\Windows\\CurrentVersion\\Run" }; //ע���������·��
	char path[MAXPATH] = { 0 };//���·��
	char name[16] = { 0 };//ע�����������


	printf("\n��������Ҫ����������������·��\n�����磺H:\\test\\test.exe��\n ��");
	fgets(path, 1024, stdin);//����·��
	flush();

	printf("\n\n����������\n�������ȡ���������Ǻ��ֲ�����15���ڣ�\n ��");
	fgets(name, 16, stdin);//��������
	flush();

	for (i = 0; i < MAXPATH; i++)//ȥ���������'\n'
	{
		if (path[i] == '\n')
			path[i] = '\0';
	}
	for(j = 0; j < 16; j++)
		if (name[j] == '\n')
			name[j] = '\0';


	//��ע��������� 
	if (RegOpenKeyEx(HKEY_CURRENT_USER, regpPath, 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)
	{  //���һ����Key,������ֵ
		RegSetValueEx(hKey, name, 0, REG_SZ, (BYTE *)path, strlen(path));
		RegCloseKey(hKey);//�ر�ע���

		printf("\n\t         ��ӳɹ���\nע�⣺�벻Ҫɾ������ӵĳ����ļ���������������ʧЧ��\n\n              ");
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
void delStart(void)
{
	system("title ɾ������������ && cls");


	char path[110] = { "reg delete \"HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run\"  /v " };
	char name[16] = { 0 };
	int i;


	printf("\n���������ƣ�15���ڣ���");
	fgets(name, sizeof(name), stdin);
	flush();
	
	for (i = 0; i < 16; i++)
		if (name[i] == '\n')
			name[i] = '\0';

	
	strcat_s(path, sizeof(path), name);

	puts(path);
	system(path);

	printf("\n\n\n");
	system("pause");
}