//���س��򴰿�
#pragma comment (lib,"Urlmon.lib")
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )

#include <stdio.h>
#include <windows.h>
#include <time.h>
#define SIZE 13
int GetTime(void);//�Ƚϵ�ǰʱ��ͳ����´�������ʱ��
void LastTime(void);//д������ϴ����е�ʱ��

int main(void)
{
	system("title timec���ذ�");

	while (1)
	{
		GetTime();//�Ƚϵ�ǰʱ��ͳ����ϴιرպ�15�ֵ�ʱ��
		LastTime();//д������´ιرպ�15�ֵ�ʱ�䣨Ҳ���ǳ����´�������ʱ�䣩

		system("shutdown -s -t 3600");//ִ��1Сʱ�ػ�����
		Sleep(3000000);//�ȴ�50�ֺ���ʾ�û�10���Ӿ�Ҫ�ػ�
		MessageBox(NULL, TEXT("���ĵ��Ի���10���Ӿ�Ҫ�ػ��ˣ�\n�����ùػ�׼���磺�������Ҫ���ϡ��˳���Ϸ�ȡ�"), TEXT("��ܰ��ʾ"), MB_OK | MB_ICONWARNING);
		Sleep(420000);//�ٵȴ�7�ֺ���ʾ�û�3���Ӿ�Ҫ�ػ�
		MessageBox(NULL, TEXT("���ĵ��Ի���3���Ӿ�Ҫ�ػ��ˣ�\n�����ùػ�׼�����磺�������Ҫ���ϡ��˳���Ϸ�ȡ�"), TEXT("��ܰ��ʾ"), MB_OK | MB_ICONWARNING);
		Sleep(410000);//�ٵȴ�6��50�����ʾ�û����Ͼ�Ҫ�ػ�
		MessageBox(NULL, TEXT("���ĵ��Ի���10���Ҫ�ػ��ˡ�\n��վ�����һ�¡�����һ���۾����ȿ�ˮ��\n��16���Ӻ��ٴ򿪵��԰ɣ�"), TEXT("��ܰ��ʾ"), MB_OK | MB_ICONWARNING);
	}
	return 0;
}

int GetTime(void)//�Ƚ�ʱ�� 
{
	int year, mon, day, hour, min;
	char time_pre[SIZE] = { 0 };//��������ϴιر�ʱ���15��
	char time_now[SIZE] = { 0 };//���浱ǰʱ��
	//char defau[SIZE] = "201908041811";//ʱ��Ĭ��ֵ
	FILE *fpread_pre, *fpwrite, *fpread_now, *fp;
	errno_t err;

	if ((err = fopen_s(&fpread_pre, "time.dat", "r")) != 0) //��ֻ����ʽ�򿪼�¼�����ϴιر�ʱ����ļ�,�����ж��ļ��Ƿ�������
	{
		if (err = fopen_s(&fp, "time.dat", "w") != 0)//�������Ŀ¼û��time.dat�ļ������´���
			return 0;//�������ʧ�ܾͷ���������ִ��1Сʱ�ػ�����
		//fputs(defau, fp);//���ļ�д��Ĭ������
		fclose(fp); //�ر��ļ�
		return 0;
	}
	
	if ((err = fopen_s(&fpwrite, "nowtime.dat", "w")) != 0) //��ֻд��ʽ�򿪼�¼��ǰʱ����ļ�,�����ж��ļ��Ƿ�������
		return 0;//����ļ��������󣬾ͷ���������

	//д�뵱ǰʱ�䵽�ļ�
	struct tm t;
	time_t now;
	time(&now);
	localtime_s(&t, &now);
	fprintf(fpwrite, "%02d%02d%02d%02d%02d", t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min); //д�뵱ǰ������ʱ�ֵ��ļ�
	fclose(fpwrite); //�ر��ļ�

	if ((err = fopen_s(&fpread_now, "nowtime.dat", "r")) != 0) //������ֻ����ʽ�򿪼�¼��ǰʱ����ļ�,�����ж��ļ��Ƿ�������
		return 0;//����ļ���ʧ�ܣ��ͷ���������

	fgets(time_pre, sizeof(time_now), fpread_pre);//���ļ���ȡ�����ϴιر�ʱ�䵽time_pre����
	fgets(time_now, sizeof(time_now), fpread_now);//���ļ���ȡ��ǰʱ�䵽time_now����
	fclose(fpread_now); //�ر��ļ�
	fclose(fpread_pre); //�ر��ļ�
	
	remove("nowtime.dat");//ɾ���ļ�

	//����ǰʱ��������´�������ʱ����жԱ�
	for (year = 0; year <= 3; year++)   //�Ա����
	{
		if (time_pre[year] != time_now[year])
			return 0;
		//printf("%c %c\n", time_pre[year], time_now[year]);
	}
	for (mon = 4; mon <= 5; mon++)	//�Ա��·�
	{
		if (time_pre[mon] != time_now[mon])
			return 0;
		//printf("%c %c\n", time_pre[mon], time_now[mon]);
	}
	for (day = 6; day <= 7; day++)	//�Ա����� 
	{
		if (time_pre[day] != time_now[day])
			return 0;
		//printf("%c %c\n", time_pre[day], time_now[day]);
	}
	for (hour = 8; hour <= 9; hour++)//�Ա�Сʱ 
	{
		if (time_pre[hour] < time_now[hour])
			return 0;
		//printf("%c %c\n", time_pre[hour], time_now[hour]);
	}
	for (min = 10; min <= 11; min++) //�Աȷ��� 
	{
		if (time_pre[min] < time_now[min])
			return 0;
		//printf("%c %c\n", time_pre[min], time_now[min]);
	}
	MessageBox(NULL, TEXT("��ǰΪ��Ϣʱ�䡣\n���Լ�����1���ڹػ���"), TEXT("���棡"), MB_OK | MB_ICONWARNING);//������ʾ��
	system("shutdown -s -t 60");
	return 0;
}

void LastTime(void)//д�����رպ�15�ֵ�ʱ�䣬Ҳ�����ڵ�ǰʱ��Ļ����ϼ�1Сʱ15���Ӳ�д�뵽�ļ�
{
	while (1)
	{
		FILE *fpwrite;
		errno_t err;
		int min, hour, day, mon, year;

		if ((err = fopen_s(&fpwrite, "time.dat", "w")) != 0)//���ļ�,�����ж��ļ��Ƿ�������
			break;
		time_t pre_time;						//��ȡ�����´ιر�ʱ��
		struct tm t;
		time(&pre_time);
		localtime_s(&t, &pre_time);
		min = t.tm_min + 15;//�ڵ�ǰ���ӵĻ����ϼ�15���ӣ����رյ��Ժ�15����֮�ڲ��ܴ򿪵��ԣ�
		hour = t.tm_hour + 1;//�ڵ�ǰСʱ�Ļ����ϼ�1Сʱ����Ϊ���趨1Сʱ��ػ���Ҳ������1Сʱ��رձ�����
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
