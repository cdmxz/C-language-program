#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <string.h>
int main(void)
{
	int num, ch;
	FILE *fpread, *fpwrite;
	errno_t err;
	char time[51];                   //�洢ʱ��
	char content[301];               //�洢д���ļ�������

	while (1)
	{
		system("cls");
		system("mode con:cols=60 lines=20"); //���ÿ���̨���ڴ�С
		system("title ��������԰�����¼���Ĵ����ճ̣��������е㼦��...");
		printf("\n\t\t  0.�˳������ճ̳���\n\n");
		printf("\t\t  1.�������Ĵ����ճ�\n\n");
		printf("\t\t  2.�鿴���Ĵ����ճ�\n\n");
		printf("\t\t  3.�����Ĵ����ճ�\n\n");
		printf("\t\t  4.ɾ�����д����ճ�\n\n\n");
		printf("(��������ǰ���������ż��ɣ�������밴�س���)\n��������ţ�");
		while (scanf_s("%d", &num) != 1)       //�����ַ�����
		{
			rewind(stdin);//��ջ����������ַ�
			printf("\n����������������룺");
		}	
		rewind(stdin);//��ջ����������ַ�
		if (num >= 0 && num <= 4)
		{
			if (num == 0)//�˳�
				break;
			if (num == 1)//����
			{
				int number;

				while (1)
				{
					if (err = fopen_s(&fpwrite, "shuju.dat", "a") != 0)//���ļ������������,���Զ������������ж��ļ��Ƿ�������
					{
						MessageBox(NULL, TEXT("�ļ���ȡ�򴴽�����\n���Թ���ԱȨ���������ԣ�"), TEXT("error!"), MB_OK | MB_ICONERROR);//������ʾ��
						exit(1);                                        //�쳣�˳�
					}
					system("cls");
					system("title ��������25�֣��������š���㡢�ո����ڣ��������ɴ���");
					printf("��ܰ��ʾ��\n1����������밴�س���\n2����������25�֣��������š���㡢�ո�����,�������ɴ���\n\n\n\n");
					printf("����������ճ̵�ʱ��\n��������Ҫ������µ����ڣ���");
					fgets(time, 51,stdin);                  //����ʱ��
					rewind(stdin);//��ջ����������ַ�
					system("cls");
					system("title ��������150�֣��������š���㡢�ո����ڣ��������ɴ���");
					printf("��ܰ��ʾ��\n1����������밴�س���\n2����������150�֣��������š���㡢�ո�����,�������ɴ���\n\n");
					printf("����������ճ̵����ݣ�\n");
					fgets(content,301,stdin);      //��������
					rewind(stdin);//��ջ����������ַ�
					fprintf(fpwrite, "�ճ̴���ʱ�䣺%s", time);//�����������д���ļ�
					fprintf(fpwrite, "�ճ̴������ݣ�%s\n", content);
					fclose(fpwrite);//�ر��ļ�
					printf("\n\t\t¼��ɹ���\n\n");
					printf("�Ƿ����¼�루1.�� 2.�񣩣�\n�����룺");
					while (scanf_s("%d", &number) != 1)
					{
						rewind(stdin);//��ջ����������ַ�
						printf("\n����������������룺");
					}
					rewind(stdin);
					if (number == 1)
						continue;
					else if (number == 2)
						break;
					else
					{
						printf("\n�������û�����ѡ�");
						Sleep(1000);
					}

				}
			}

			if (num == 2)//�鿴
			{
				system("mode con: cols=120 lines=40");
				if ((err = fopen_s(&fpread, "shuju.dat", "r")) != 0)//��ֻ����ʽ���ļ�
				{
					MessageBox(NULL, TEXT("�����ļ���ȡ����\n���鱾����Ŀ¼���Ƿ��С�shuju.dat������ļ�\n���������ļ��Ƿ���������ռ�ã�"), TEXT("error!"), MB_OK | MB_ICONERROR);
					exit(1);                                   //�����쳣�˳�
				}
				while ((ch = fgetc(fpread)) != EOF)            //��ȡ�ļ����ݵ�ch��EOF���ļ�������־��
					putchar(ch);                               //��ȡ�ļ��������������̨
				putchar('\n');
				system("pause");
				fclose(fpread);    //�ر��ļ�
			}
			if (num == 3)//��
			{
				if (rename("shuju.dat", "shuju.txt"))
				{
					MessageBox(NULL, TEXT("�����ļ�������ʧ�ܣ�\n�����Ƿ�����������ռ�ã�\n�������ԣ�"), TEXT("error!"), MB_OK | MB_ICONERROR);
					continue;
				}
				system("shuju.txt");//���ļ�
				system("pause");
				if (rename("shuju.txt", "shuju.dat"))
				{
					MessageBox(NULL, TEXT("�����ļ�������ʧ�ܣ�\n���ڱ�����Ŀ¼�°ѡ�shuju.txt��������Ϊ��shuju.dat����"), TEXT("error!"), MB_OK | MB_ICONERROR);
					continue;
				}
			}

			if (num == 4)//ɾ��
			{
				num = MessageBox(NULL, TEXT("�˲�����ɾ���������д����ճ̣���ɾ�����޷��ָ���\n����������ؿ��Ǻ�ɾ����\n�Ƿ�ɾ����"), TEXT("���棡"), MB_YESNO | MB_ICONWARNING);
				if (num == IDNO)                                   //���÷���ֵ�ж��û�����İ�ť
					system("cls");
				if (num == IDYES)
					if(remove("shuju.dat"))
					MessageBox(NULL,TEXT("�����ļ�ɾ��ʧ�ܣ�\n�����Ƿ���������ռ��\n���鱾����Ŀ¼���Ƿ��С�shuju.dat������ļ���"),TEXT("error!"),MB_OK | MB_ICONERROR);
			}
		}
		else
		{
		  printf("\n\t�������");
		  Sleep(1000);
	    }
    } 
} 