#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>
#include <time.h>
#include <conio.h>

int  GetTime(int n);

int main(void)
{
	char Pass[7] = "115-56";
	char TmpPass[7] = { 0 };
	int i, ch;

	system("title ��������Ҫ������ȷ�������������ʹ�� && color 06 && mode con cols=91 lines=30 && shutdown -s -t 300");//���ô��ڱ��⡢������ɫ�����ڴ�С

	printf("\n\n\n\n\n\n\n\t\t         ����Ҫ������ȷ�������������ʹ�ñ����ԡ�\n\n\t\t PS�������Խ���5�����ڹػ����밴�������������������档\n\n\n\n\n\n\t\t\t           ");
	system("pause");
	system("cls");


	printf("\n\n\n\n\n\t       ��ܰ��ʾ������60���ʱ��������ȷ�����룬������ֻ��3�λ��ᡣ\n");
	printf("         һ������60�뻹δ������ȷ�������������3�δ�������룬���Ծͻ��Զ��ػ���\n\n\n\n\n");
	printf("ע�⣺���������ʹ�ñ����Կ�����ϵ�ҵ�QQ��2641015842 ����ע���������룩 \n\t\t\t\t    ΢�ţ�18318085358����ע���������룩 \n\t\t\t\t    �绰��18318085358�������Ÿ��ң�");
	printf("\n\t\t\t\t    ��վ��http://fuhohua.web3v.com��https://cdmxz.github.io");
	printf("\n\n\n\n\t\t\t       �����������վ��ȡ����\n\n\n\n\t\t\t    ");
	_getch();
	system("start http://fuhohua.web3v.com");
	printf("   ����������������������");
	_getch();
	system("cls  && shutdown -a && shutdown -s -t 60");


	for (i = 3; i >= 1; i--)
	{
		printf("\n\t\t\t����60�����������룬������Խ���ػ�");
		printf("\n\n���������루�㻹��%d�λ��ᣩ��", i);
		scanf_s("%s", TmpPass, 7);
		while ((ch = getchar()) != '\n' && ch != EOF)//���ն����ַ��ͻ��з�
			;

		if (!strcmp(Pass, TmpPass))//�Ա������Ƿ���ȷ
		{
			GetTime(0);
			system("shutdown -a");
			system("cls && color 02 && shutdown -s -t 2400");//����������������ɫ������40�ֺ�ػ�
			printf("\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t��ϲ����������ȷ��������ʹ�ñ�����40���ӡ�\n\n\n\n\n\t\t\t\t   ");
			system("pause");
			//���ش���
			HWND hwnd;
			hwnd = FindWindow("ConsoleWindowClass", NULL);
			if (hwnd)
				ShowWindow(hwnd, SW_HIDE);
			
			//����ʱ
			Sleep(2100000);
			MessageBox(NULL, TEXT("�����Ի���5���Ӿ�Ҫ�ػ��ˣ�������׼����"), TEXT("��ܰ��ʾ"), MB_OK | MB_ICONWARNING);
			Sleep(240000);
			MessageBox(NULL, TEXT("�����Ի���1���Ӿ�Ҫ�ػ��ˣ�������׼����"), TEXT("��ܰ��ʾ"), MB_OK | MB_ICONWARNING);
			return 0;
		}
		else
		{
			GetTime(1);
			printf("\n\n\t\t\t�������");
			Sleep(1000);
		}
		system("cls");
	}

	system("shutdown -a && shutdown -s -t 5");
	MessageBox(NULL, TEXT("������������������󣬱����Լ����ػ���"), TEXT("����"), MB_OK | MB_ICONWARNING);
	return 0;
}


int GetTime(int n)//��ȡ��ǰʱ��
{
	errno_t err;
	FILE *fpwrite;

	if ((err = fopen_s(&fpwrite, "log.dat", "a+")) != 0) //��ֻд��ʽ�򿪼�¼��ǰʱ����ļ�,�����ж��ļ��Ƿ�������
		return 1;//����ļ��������󣬾ͷ���������

	//д�뵱ǰʱ�䵽�ļ�
	struct tm t;
	time_t now;
	time(&now);
	localtime_s(&t, &now);
	
	//д����־
	if (n == 0)
		fprintf(fpwrite, "\n\n����ʱ�䣺%d��%02d��%02d��%02d��%02d��%02d�롣\n���������������ȷ��", t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec); //д�뵱ǰ������ʱ�ֵ��ļ�
	else if (n == 1)
		fprintf(fpwrite, "\n\n\n����ʱ�䣺%d��%02d��%02d��%02d��%02d��%02d�롣\n����������������", t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec); //д�뵱ǰ������ʱ�ֵ��ļ�

	fclose(fpwrite); //�ر��ļ�
	
	return 0;
}