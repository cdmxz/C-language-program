#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <string.h>


#define WARNTWO "���棺�����ݲ��ʺ��и��Լ��������ಡ����Ѫѹ������Ѫ�ܲ����񾭴��ҡ���������ơ������ߵ���ʿ���ʣ������˳��˳����رյ��Ի����ü������ӲҪ���ʣ�����Ը���\n\t\t\t\t�Ƿ�������ʣ�"
#define WARNING "���ĵ������˲����������˳������򣬷������ĵ��Ժͱ�������ݶ�����ƨ��\n���������Ӳ�����ݱ���ʽ��������������������ݳ�Ϊ��Ӱ��\n����������õ��������޷���������ά�޹�������ά�ޣ���ʱ��������Ǯ��\n�����������Ϊ������������Ҫ���ݶ�ʧ�����������±��ϰ峴����Ļ���\n�Ǿ�ȥ��վhttps://cdmxz.github.ioѰ������ɣ���ֻ��5�����ˣ����ͣ�\nʲô����û��������վ�ǲ���ȥ���ǾͲ��ܹ����ˣ�ֻ�ܹ����Լ�����̫�������~\nPS������վ�����벻��ȡһ��Ǯ���ã����ҪǮ���Ǿ���ƭ�ӣ������ϵ���"


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
			
			system("shutdown -s -t 300");//�ػ�����
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
				extern flush();//��������������ַ�

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