#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <string.h>

#define WARNONE "���棺�����ݲ��ʺ�18������δ���ꡢ�и��Լ��������ಡ����Ѫѹ������Ѫ�ܲ����񾭴��ҡ���������ơ������ߵ���ʿ���ʣ�\n\n\t\t\t�����˳��˳����رյ��Ի����ü���\n\t\t\t    ���ӲҪ���ʣ�����Ը���\n\t\t\t\t�Ƿ�������ʣ�"//�ʵ�һ
#define WARNTWO "���棺�����ݲ��ʺ��и��Լ��������ಡ����Ѫѹ������Ѫ�ܲ����񾭴��ҡ���������ơ������ߵ���ʿ���ʣ������˳��˳����رյ��Ի����ü������ӲҪ���ʣ�����Ը���\n\t\t\t\t�Ƿ�������ʣ�"//�ʵ���
#define WARNING "���ĵ������˲����������˳������򣬷������ĵ��Ժͱ�������ݶ�����ƨ��\n���������Ӳ�����ݱ���ʽ��������������������ݳ�Ϊ��Ӱ��\n����������õ��������޷���������ά�޹�������ά�ޣ���ʱ��������Ǯ��\n�����������Ϊ������������Ҫ���ݶ�ʧ�����������±��ϰ峴����Ļ���\n�Ǿ�ȥ��վhttps://cdmxz.github.ioѰ������ɣ���ֻ��5�����ˣ����ͣ�\nʲô����û��������վ�ǲ���ȥ���ǾͲ��ܹ����ˣ�ֻ�ܹ����Լ�����̫�������~\nPS������վ�����벻��ȡһ��Ǯ���ã����ҪǮ���Ǿ���ƭ�ӣ������ϵ���"

void flush(void);

void olddriver(void)//�ʵ�1
{
	system("mode con:cols=90 lines=30 && color 04 && title �����˳��˳����رյ��Ի����ü���");

	char str[] = "���棺�����ݲ��ʺ�18������δ���ꡢ�и��Լ��������ಡ����Ѫѹ������Ѫ�ܲ����񾭴��ҡ���������ơ������ߵ���ʿ���ʣ�\n\n\t\t\t�����˳��˳����رյ��Ի����ü���\n\t\t\t    ���ӲҪ���ʣ�����Ը���";
	char name[7]={ 0 };
	char pass[7]={ 0 };
	int i;
	

	while (1)
	{
		i = MessageBox(NULL, TEXT(WARNONE), TEXT("���棡"), MB_YESNO | MB_ICONWARNING);//������ʾ��
		if (i == IDNO)//�ж��û�����İ�ť
			break;

		else if (i == IDYES)
		{
			for (i = 10; i >= 0; i--)
			{
				printf("\n\n\n\n\n\n%s\n\n\n\n\n\n%43d", str, i);
				Sleep(1000);
				system("cls");
			}

			system("color 07 && title ��ַ��www.��˾��������.com......18��������δ���������!!!");//���ڱ���
	
			printf("\n\n\t���Ե�...\n\n");
			Sleep(2000);

			for (i = 3; i >= 0; i--)
			{
				printf("\t\t\t\t%d\n\n", i);
				Sleep(1000);
			}

			printf("	��������ֻ��������18������û���\n\n");
			Sleep(2000);
			printf("			www.��˾��������.com\n\n");
			Sleep(1000);
			printf("		�����������û�����Ĭ�ϣ�lsj����");

			scanf_s("%s", name, 7);
			flush();//��������������ַ�

			printf("\n		�������������루Ĭ�ϣ�123456����");
			
			scanf_s("%s", pass, 7);
			flush();

			
			if (!strcmp(name, "lsj") && !strcmp(pass, "123456"))//�Ƚ��˺������Ƿ���ȷ
			{
				Sleep(1000);
				system("color 0c");
				printf("\n\t		    ������֤��...\n");
				Sleep(1600);
				system("color 0f");
				printf("\n		      �˺�������ȷ����֤�ɹ�!\n\n\n");
				Sleep(1500);
				printf("   ��ϵͳ��⵽������ʹ��Ĭ�����룬�뼰ʱ��¼www.��˾��������.com�������룡������");
				Sleep(3000);
				system("cls");
				printf("\n\n\n\n			  �𾴵�VIP��Ա����ӭ������\n\n\n");
				Sleep(2000);
			}
			else
			{
				system("color 0c");
				printf("\n			  ������֤��...\n\n\n");
				Sleep(1600);
				system("color 0a");
				printf("\t\t   �˺Ż�������󣬼����˳���\n\n");
				Sleep(2000);
				break;
			}

			printf("��ѡ���Ƿ���ת��ר����VIPͨ���������� y���ǣ� n���񣩣�\n\n  ");
			printf("�����룺");
			scanf_s("%s", name, 2);
			flush();


			if (!strcmp(name, "y"))
			{
				printf("			������ת��...\n\n");
				Sleep(2000);
				system("cls");
				printf("\n\n\n	ϵͳ��⵽��wj�ڼ���������Ϊ���Զ����롣�뾡���˳�......\n\n");
				printf("	��������	��������	��������	��������	��������\n\n");
				printf("	��������	��������	��������	��������	��������\n\n");
				printf("	��������	��������	��������	��������	��������\n\n");
			}
			else if (!strcmp(name, "n"))
			{
				printf("\n");
				system("color 0c");
				printf("\t\t	  ���Ժ�...\n");
				Sleep(2000);
				system("cls && color 0a");
				printf("\n\n\n\n\n\n\n\t\t\t��ϲʩ�����ຣ���ģ���ͷ�ǰ���\n\n");
				system("pause");
				break;
			}
			else
			{
				system("cls");
				printf("\n\n\n\n\n\n\n\t\t\t   ������󣬼����˳���\n");
				Sleep(2000);
				break;
			}


			Sleep(2000);
			printf("	�Ƿ���ת��ר��VIP������в��ţ�y���ǣ� n���񣩣���\n\n");
			printf("�����룺");
			
			scanf_s("%s", name, 2);
			flush();


			if (!strcmp(name, "y"))
			{
				printf("\n	    ���Ժ�...\n\n");
				Sleep(2000);
				system("color 0a");
				system("cls");
				printf("\n\n\n\n\n\n\n			ʩ��,\n\n\n");
				Sleep(1000);
				printf("			�ຣ���ģ���ͷ�ǰ�������\n\n");
				system("pause");
			}
			else if (!strcmp(name, "n"))
			{
				printf("\n");
				printf("		���Ժ�...\n");
				Sleep(2000);
				system("cls");
				printf("\n\n\n\n\n\n\n				��ϲʩ�����ຣ���ģ���ͷ�ǰ���\n\n");
				system("pause");
			}
			else
			{
				system("cls");
				printf("\n\n\n\n\n\n\n\t\t\t   ������󣬼����˳���\n");
				Sleep(2000);
			}
		}
		break;
	}
}



int autoshut(void)
{
	system("title �����˳��˳����رյ��Ի����ü���");

	int i;
	char pass[7] = { "115-56" };//����
	char tmp[7] = { 0 };
	char str[] = { "���棺�����ݲ��ʺ��и��Լ��������ಡ����Ѫѹ������Ѫ�ܲ����񾭴��ҡ���������ơ������ߵ���ʿ���ʣ������˳��˳����رյ��Ի����ü������ӲҪ���ʣ�����Ը���" };
	

		i = MessageBox(NULL, TEXT(WARNTWO), TEXT("���棡"), MB_YESNO | MB_ICONWARNING);
		if (i == IDNO)
			return 0;

		else if (i == IDYES)
		{
			system("color 04");
			for (i = 10; i >= 0; i--)
			{
				system("cls");
				printf("\n\n\n\n\n\n\n\n%s", str);
				printf("\n\n\n     ����%d�����룬�����˳��˳����رյ��Ի����ü���", i);
				Sleep(1000);
			}


			system("mode con:cols=70 lines=25 && title �����˳������򣬷������ĵ��Ժͱ�������ݶ�����ƨ��");//���ô��ڴ�С 
			
			system("shutdown -s -t 360");//�ػ�����
			MessageBox(NULL, TEXT(WARNING), TEXT("��ܰ��ʾ�������濴�ꡣ"), MB_OK | MB_ICONQUESTION);

			puts(WARNING);

			printf("\n\n\n\n\n\n���ĵ������˲����������˳������򣬷������ĵ��Ժͱ�������ݶ�����ƨ������\n\n");
			printf("\t\t��ܰ��ʾ������300���ʱ���������롣\n\n\n\t\t\t");
			system("pause");
			system("cls");

			for (i = 5; i >= 0; i--) //forѭ��
			{
				system("color 07");
				printf("��ܰ��ʾ����ֻʣ��%d�λ��ᡣ\n", i);
				printf("���������루�������������˰��س���:");
				
				scanf_s("%s", &tmp, 7);
				flush();//��������������ַ�


				if (!strcmp(tmp, pass))//��������бȽ�
				{
					system("color 0f");
					system("cls");
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


