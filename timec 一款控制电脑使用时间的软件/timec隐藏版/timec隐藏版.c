#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <direct.h>
#include <io.h>

#define SIZE 13

int  GetTime(void);      //�Ƚϵ�ǰʱ��͵�����������ʱ��
void LastTime(void);     //д������´�������ʱ��
void ShutDown(void);	 //ִ�йػ�����
int  read_file(void);	 //�������ļ���ȡ�������е�ʱ�����Ϣʱ��

/*���磺��������40���ӣ�Ҫ��Ϣ10���Ӻ���ܼ������С�
���統ǰʱ��Ϊ2019��11��23��11:20�����趨��������40���Ӿ�Ҫ��Ϣ10���ӡ�
Ҳ����˵Ҫ�ڵ�ǰʱ��Ļ����ϼ���50���Ӻ�������´򿪵��ԡ�
��Ϊ��������40���Ӻ�ػ�����ʱ��ʱ����2019��11��23��12:00���ټ���10������Ϣʱ�䣬
������Ҫ��2019��11��23��12:10������ٴδ򿪵��Է������ʾ�ػ���*/

//�����ļ�·��
char time_path[] = { "C:\\Program Files\\timec\\time.dat" };
char config_file[] = { "C:\\Users\\Timec_config.txt" };


int main(void)
{
	//���س������д���
	HWND hwnd;
	hwnd = FindWindow("ConsoleWindowClass", NULL);
	if (hwnd)
		ShowWindow(hwnd, SW_HIDE);

	char folder_name[] = { "C:\\Program Files\\timec" };
	//���������ļ���
	if (_access(folder_name, 0)) //�ж��ļ����Ƿ����
		_mkdir(folder_name);	 //�������򴴽�

	GetTime();
	LastTime();
	ShutDown();

	//����windows api�����ļ�
	SetFileAttributes(time_path, FILE_ATTRIBUTE_HIDDEN);
	SetFileAttributes(folder_name, FILE_ATTRIBUTE_HIDDEN);

	/*system("shutdown -s -t 2400");//ִ��40���ӹػ�����
	Sleep(2220000);//�ȴ�37�ֺ���ʾ�û�����3���Ӿ�Ҫ�ػ�
	MessageBox(NULL, TEXT("���ĵ��Ի���3���Ӿ�Ҫ�ػ��ˣ�\n�����ùػ�׼�����磺�������Ҫ���ϡ��˳���Ϸ�ȡ�"), TEXT("��ܰ��ʾ"), MB_OK | MB_ICONWARNING);
	Sleep(240000);//�ٵȴ�4�ֺ���ʾ�û�1���Ӿ�Ҫ�ػ�
	MessageBox(NULL, TEXT("���ĵ��Ի���1���Ӿ�Ҫ�ػ��ˣ�\n�����ùػ�׼�����磺�������Ҫ���ϡ��˳���Ϸ�ȡ�"), TEXT("��ܰ��ʾ"), MB_OK | MB_ICONWARNING);
	Sleep(50000);//�ٵȴ�50�����ʾ�û����Ͼ�Ҫ�ػ�
	system("shutdown -s -t 10");
	MessageBox(NULL, TEXT("���ĵ��Ի���10���Ҫ�ػ��ˡ�\n��վ�����һ�¡�����һ���۾����ȿ�ˮ��\n��10���Ӻ��ٴ򿪵��԰ɣ�"), TEXT("��ܰ��ʾ"), MB_OK);*/
	return 0;
}


//�Ƚ�ʱ�� 
int GetTime(void)
{
	char time_pre[SIZE];//��������ϴιر�ʱ��ĺ�10���ӣ�Ĭ�ϵ�������40������Ϣ10���ӵ�����£�
	char time_now[SIZE];//���浱ǰʱ��
	char tip[100];//��������ǰ����ʱ����ػ���ʾ
	FILE *fp;
	errno_t err;

	//��ȡ��ǰʱ��
	struct tm *p;
	time_t now;
	time(&now);
	p = localtime(&now);

	//д�뵱ǰʱ�䵽�ַ�����time_now��
	sprintf_s(time_now, SIZE, "%02d%02d%02d%02d%02d", p->tm_year + 1900, p->tm_mon + 1, p->tm_mday, p->tm_hour, p->tm_min);
	
	//�򿪡�time.dat��,���ж��ļ��Ƿ�������
	if ((err = fopen_s(&fp, time_path, "r")) != 0)
	{
		//�������Ŀ¼û�С�time.dat������ļ������´���
		if (err = fopen_s(&fp, time_path, "w") != 0)
			return 1;//�������ʧ�ܾͷ���������ִ��40���ӹػ�����
		fclose(fp);
		return 0;
	}
	//���ļ���ȡ�����ϴιر�ʱ�䵽time_pre����         
	fgets(time_pre, SIZE, fp);
	//�ر��ļ�
	fclose(fp); 
	//�жϵ�ǰʱ���Ƿ�С�ڵ�����������ʱ��
	if (strcmp(time_pre, time_now) > 0)
	{
		//�����ǰʱ��С�ڵ�����������ʱ���ִ��10��ػ����������ػ���ʾ
		system("shutdown -s -t 10");

		//���ļ������ļ���ȡ�ػ���ʾ������
		if (err = fopen_s(&fp, time_path, "r") != 0)
		{//������ļ�ʧ�ܾ�ֱ������ػ���ʾ
			MessageBox(NULL, TEXT("��ǰΪ��Ϣʱ�䣬���Լ�����10���ڹػ���"), TEXT("���棡"), MB_OK | MB_ICONWARNING);//�����ػ���ʾ
			exit(EXIT_FAILURE);
		}
		
		while (!feof(fp))
			fgets(tip, 100, fp);  //��ȡ��time.dat���ڶ��е�����Ҳ���ǹػ���ʾ������
		fclose(fp);

		//�����ػ���ʾ
		MessageBox(NULL, (tip), TEXT("���棡"), MB_OK | MB_ICONWARNING);
		exit(EXIT_FAILURE);
	}
	return 0;
}


/*д����Թرպ�10�ֵ�ʱ�䣬Ҳ�����ڵ�ǰʱ��Ļ����ϼ�50���ӡ� 
�������´�������ʱ�䡣��Ĭ�ϵ�������40������Ϣ10���ӵ�����£�*/
void LastTime(void)
{
	while (1)
	{
		FILE *fp;
		errno_t err;
		int min, hour, day, mon, year;
		int off_time;

		off_time = read_file();

		//��ȡ��ǰʱ��
		time_t pre_time;
		struct tm *p;
		time(&pre_time);
		p = localtime(&pre_time);
		min = p->tm_min + off_time;/*�ڵ�ǰ���ӵĻ����ϼ�50���ӣ���40���Ӻ�رյ���+10������Ϣʱ�䣩
		��Ĭ�ϵ�������40������Ϣ10���ӵ�����£�*/
		hour = p->tm_hour;
		day = p->tm_mday;
		mon = p->tm_mon + 1;
		year = p->tm_year + 1900;

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
			{
				day -= 30;
				mon++;
			}
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

		//���ļ�,�����ж��ļ��Ƿ�������
		if ((err = fopen_s(&fp, time_path, "w")) != 0)
			break;

		//���ļ�д������´�������ʱ��
		fprintf(fp, "%02d%02d%02d%02d%02d\n", year, mon, day, hour, min);
		//���ļ�д����ʾ��Ϣ
		fprintf(fp, "��ǰΪ��Ϣʱ��,�´δ˵�����������ʱ��Ϊ��%02d��%02d��%02d��%02dʱ%02d�֡�    ���Լ�����10���ڹػ���", year, mon, day, hour, min);   //д��������Сʱ����
		fclose(fp);

		printf("�´δ˵�����������ʱ��Ϊ��%02d��%02d��%02d��%02dʱ%02d�֡�\n", year, mon, day, hour, min);
		break;
	}
}


//ִ�йػ�����
void ShutDown(void)
{
	int  ch, i = 0;
	int off_time = 40;
	char command[22] = { "shutdown -s -t " };
	char tmp[5];
	FILE *fp;
	errno_t err;

	//�������ļ���ȡ����
	if (!_access(config_file, 0)) //�ж��ļ��Ƿ����
		if (!(err = fopen_s(&fp, config_file, "r")))//�ж��ļ��Ƿ�������
		{
			if ((ch = fgetc(fp)) != EOF)//�ж��ļ��Ƿ�Ϊ��
				fscanf_s(fp, "%*s %*s %d", &off_time);
			fclose(fp);
		}

	printf("����%d�֣�", off_time);
	off_time *= 60;//�ѷ�ת��Ϊ��
	printf("%d�룩��ػ���\n\n", off_time);
	sprintf_s(tmp, 5, "%d", off_time);
	strcat_s(command, 22, tmp);
	system(command);
	system("pause");
}


//��ȡ�������е�ʱ���Լ���Ϣʱ��
int read_file(void)
{
	int  ch, i = 0, j = 1;
	int off_time = 40;	//Ĭ������ʱ��
	int break_time = 10;//Ĭ����Ϣʱ��
	char config[5] = { 0 };
	FILE *fp;
	errno_t err;

	//�������ļ���ȡ����
	if (!_access(config_file, 0)) //�ж��ļ��Ƿ����
	{
		if ((err = fopen_s(&fp, config_file, "r")) != 0)//�ж��ļ��Ƿ�������
			return 	off_time += break_time;

		if ((ch = fgetc(fp)) != EOF)//�ж��ļ��Ƿ�Ϊ��
			do
			{
				ch = fgetc(fp);
				if (ch == '=')//�ж�ch�Ƿ�Ϊ��=��
					while ((ch = fgetc(fp)) != ';')
					{
						if (ch == ' ')//������ȡ�Ŀո�
							continue;
						config[i] = ch;
						i++;
					}
				if (ch == '\n')//�ж��Ƿ�����ڶ���
				{
					i = 0;
					if (j < 2)
						sscanf_s(config, "%d", &off_time);//�ѵ�һ�е����ݱ��浽off_time����һ�е������ǵ��������е�ʱ��
					j++;
				}
			} while (ch != EOF);
			sscanf_s(config, "%d", &break_time);//�ѵڶ��е����ݱ��浽break_time���ڶ��е������ǵ��Թػ�����Ϣ��ʱ��
			fclose(fp);
	}
	else
	{
		if (!(err = fopen_s(&fp, config_file, "w")))
		{
			fprintf_s(fp, "off_time = 40 ;     ���������е�ʱ�䡣\nbreak_time = 10 ;  ��Ϣ��ʱ�䡣\n\nע�⣺����ֵ�ĵ�λ��Ϊ�֣�ֵ��Χ5~999��\n�����뱣��ԭ��ʽ��������ֳ������������У���ɾ�����ļ���");
			fclose(fp);
		}
	}

	printf("������ʱ�� = %d��, ��Ϣʱ�� = %d��, ", off_time, break_time);
	off_time += break_time;
	printf("�ܼ� = %d�֡�\n", off_time);

	return off_time;
}