//���س��򴰿�
#pragma comment (lib,"Urlmon.lib")
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )

#include <stdio.h>
#include <windows.h>
#include <time.h>
#define SIZE 13

int GetTime(void);//�Ƚϵ�ǰʱ��ͳ����´�������ʱ��
void LastTime(void);//д������ϴ����е�ʱ�䣨Ҳ���ǳ����´�������ʱ�䣩

int main(void)
{
	GetTime();
	LastTime();

	system("shutdown -s -t 2400");//ִ��40���ӹػ�����
	Sleep(2220000);//�ȴ�37�ֺ���ʾ�û�����3���Ӿ�Ҫ�ػ�
	MessageBox(NULL, TEXT("���ĵ��Ի���3���Ӿ�Ҫ�ػ��ˣ�\n�����ùػ�׼�����磺�������Ҫ���ϡ��˳���Ϸ�ȡ�"), TEXT("��ܰ��ʾ"), MB_OK | MB_ICONWARNING);
	Sleep(240000);//�ٵȴ�4�ֺ���ʾ�û�1���Ӿ�Ҫ�ػ�
	MessageBox(NULL, TEXT("���ĵ��Ի���1���Ӿ�Ҫ�ػ��ˣ�\n�����ùػ�׼�����磺�������Ҫ���ϡ��˳���Ϸ�ȡ�"), TEXT("��ܰ��ʾ"), MB_OK | MB_ICONWARNING);
	Sleep(50000);//�ٵȴ�50�����ʾ�û����Ͼ�Ҫ�ػ�
	//system("shutdown -s -t 10");
	MessageBox(NULL, TEXT("���ĵ��Ի���10���Ҫ�ػ��ˡ�\n��վ�����һ�¡�����һ���۾����ȿ�ˮ��\n��10���Ӻ��ٴ򿪵��԰ɣ�"), TEXT("��ܰ��ʾ"), MB_OK);
	return 0;
}


//�Ƚ�ʱ�� 
int GetTime(void)
{
	char time_pre[SIZE] = { 0 };//��������ϴιر�ʱ��ĺ�10����
	char time_now[SIZE] = { 0 };//���浱ǰʱ��
	
	FILE *fp;
	errno_t err;

	//�򿪼�¼�����ϴιر�ʱ����ļ�,�����ж��ļ��Ƿ�������
	if ((err = fopen_s(&fp, "time.dat", "r")) != 0) 
	{
		//�������Ŀ¼û��time.dat�ļ������´���
		if (err = fopen_s(&fp, "time.dat", "w") != 0)
			return 0;//�������ʧ�ܾͷ���������ִ��40���ӹػ�����
		fclose(fp); 
		return 0;
	}

	
	//��ȡ��ǰʱ��
	struct tm t;
	time_t now;
	time(&now);
	localtime_s(&t, &now);

	//д�뵱ǰ������ʱ�ֵ��ַ���
	sprintf_s(time_now, SIZE, "%02d%02d%02d%02d%02d", t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min); 
	printf("�´δ˵�����������ʱ�䣺%s", time_now);

	//���ļ���ȡ�����ϴιر�ʱ�䵽time_pre����
	fgets(time_pre, sizeof(time_now), fp);

	//�ر��ļ�
	fclose(fp); 
	
	//�жϵ�ǰʱ���Ƿ�С��Ԥ��ʱ��
	if (strcmp(time_pre, time_now) > 0)
	{
		system("shutdown -s -t 30");
		MessageBox(NULL, TEXT("��ǰΪ��Ϣʱ�䡣\n���Լ�����30���ڹػ���"), TEXT("���棡"), MB_OK | MB_ICONWARNING);//������ʾ��
		exit(0);
	}
	return 0;
}

//д�����رպ�10�ֵ�ʱ�䣬Ҳ�����ڵ�ǰʱ��Ļ����ϼ�50����
void LastTime(void)
{
	while (1)
	{
		FILE *fpwrite;
		errno_t err;
		int min, hour, day, mon, year;


		//���ļ�,�����ж��ļ��Ƿ�������
		if ((err = fopen_s(&fpwrite, "time.dat", "w")) != 0)
			break;
		
		//��ȡ�����´ιر�ʱ��
		time_t pre_time;						
		struct tm t;
		time(&pre_time);
		localtime_s(&t, &pre_time);
		min = t.tm_min + 50;//�ڵ�ǰ���ӵĻ����ϼ�50���ӣ���40���Ӻ�رյ���+10������Ϣʱ�䣩
		hour = t.tm_hour;
		day = t.tm_mday;
		mon = t.tm_mon + 1;
		year = t.tm_year + 1900;

		if (min >= 60)
		{
			min -= 60;
			hour++;
		}
		if (hour >= 24)
		{
			hour -= 24;
			day++;
		}
		if (mon == 1 || mon == 3 || mon == 5 || mon == 7 || mon == 8 || mon == 10 || mon == 12)//�жϵ�ǰ�·��Ƿ�Ϊ����
		{
			if (day > 31)
			{
				day -= 31;
				mon++;
			}
		}
		else if (mon == 4 || mon == 6 || mon == 9 || mon == 11)//�жϵ�ǰ�·��Ƿ�ΪС��
		{
			if (day > 30)
				day -= 30;
			mon++;
		}
		else if (mon == 2)
		{
			if (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0))//�жϵ�ǰ����Ƿ�Ϊ����
				day -= 29;//����2����29��
			else
				day -= 28;//ƽ��2����28��
			mon++;
		}
		if (mon > 12)//�жϵ�ǰ�·��Ƿ񳬹�12��
		{
			mon -= 12;
			year++;
		}
		//���ļ�д������´�������ʱ��
		fprintf(fpwrite, "%02d%02d%02d%02d%02d", year, mon, day, hour, min);   //д��������Сʱ����
		fclose(fpwrite);     //�ر��ļ�
		break;
	}
}
