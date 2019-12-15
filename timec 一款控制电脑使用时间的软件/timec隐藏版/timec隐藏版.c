#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <direct.h>
#include <io.h>

#define SIZE 15

int  GetTime(void);      //�Ƚϵ�ǰʱ��͵�����������ʱ��
int  WriteTime(void);    //д����Թػ�ʱ����´�������ʱ��
void ShutDown(void);	 //ִ�йػ�����
int  Read_File(int *OffTime, int *BreakTime); //�������ļ���ȡ���������е�ʱ�����Ϣʱ��
void FirstStart(void);	 //��һ�������Զ������
void Flush(void);		 //���������
void fgets_n(char *str); //���fgets��ȡ��'\n'

/*���磺��������40���ӣ�Ҫ��Ϣ10���Ӻ���ܼ������С�
���統ǰʱ��Ϊ2019��11��23��11:20�����趨��������40���Ӿ�Ҫ��Ϣ10���ӡ�
Ҳ����˵Ҫ�ڵ�ǰʱ��Ļ����ϼ���50���Ӻ�������´򿪵��ԡ�
��Ϊ��������40���Ӻ�ػ�����ʱ��ʱ����2019��11��23��12:00���ټ���10������Ϣʱ�䣬
������Ҫ��2019��11��23��12:10������ٴδ򿪵��Է������ʾ�ػ���*/

//�����ļ�·��
char time_dat_path[] = { "C:\\Program Files\\timec\\time.dat" };
char config_file_path[] = { "C:\\Users\\Timec_config.txt" };
char folder_path[] = { "C:\\Program Files\\timec" };

int main(void)
{	
	//���س������д���
	HWND hwnd;
	hwnd = FindWindow("ConsoleWindowClass", NULL);
	if (hwnd)
		ShowWindow(hwnd, SW_HIDE);

	//�ж������ļ���time.dat���Ƿ����
	if (_access(time_dat_path, 0))
	{//�������򴴽�Ŀ¼���õ�һ����������
		_mkdir(folder_path);//����Ŀ¼
		SetFileAttributes(folder_path, FILE_ATTRIBUTE_HIDDEN);//����windows api����Ŀ¼
		FirstStart();
	}
	
	GetTime();
	WriteTime();
	ShutDown();
	
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
	long temp;
	int year, mon, day, hour, min, i = 0, ch;
	char command[20] = { "shutdown -s -t " }, temp_time[SIZE], YEAR[SIZE] = { 0 }, MON[SIZE] = { 0 }, DAY[SIZE] = { 0 }, HOUR[SIZE] = { 0 }, MIN[SIZE] = { 0 };
	char time_pre[SIZE];//��������ϴιر�ʱ��ĺ�10���ӣ�Ĭ�ϵ�������40������Ϣ10���ӵ�����£�
	char time_now[SIZE];//���浱ǰʱ��
	char time_off[SIZE];//������Թػ�ʱ��
	char tip[100];//��������ǰ����ʱ����ػ���ʾ
	FILE *fp;
	errno_t err;

	//��ȡ��ǰʱ��
	struct tm timep, timenow, timeoff;
	time_t p, now, off;
	time(&p);
	localtime_s(&timep, &p);

	//д�뵱ǰʱ�䵽�ַ�����time_now��
	sprintf_s(time_now, SIZE, "%02d%02d%02d%02d%02d", timep.tm_year + 1900, timep.tm_mon + 1, timep.tm_mday, timep.tm_hour, timep.tm_min);

	//�򿪡�time.dat��,���ж��ļ��Ƿ�������
	if ((err = fopen_s(&fp, time_dat_path, "r")) != 0)
		return 1;//�����ʧ�ܾͷ���������ִ��40���ӹػ�����
	if ((ch = fgetc(fp)) == EOF)
	{
		fclose(fp);
		return 1;
	}
	while (i++ < 4)
	{
		if (i == 1)
			//�ӡ�time.dat����ȡ�����ϴιرպ�xx����ʱ�䣬xx����ָ������Ϣʱ��         
			fgets(time_pre, SIZE, fp);
		else if(i==2)
			//�ӡ�time.dat����ȡ�ڶ��е�����Ҳ���ǹػ���ʾ������
			fgets(tip, 100, fp);  
		else if(i==3)
			//�ӡ�time.dat����ȡ�����ϴιر�ʱ��      
			fgets(time_off, SIZE, fp);
	}
	fclose(fp); 
	//���fgets��ȡ�Ļ��з�
	fgets_n(time_pre);
	fgets_n(tip);

	//�жϵ�ǰʱ���Ƿ�С�ڵ��Թػ�ʱ��
	if (strcmp(time_off, time_now) > 0)
	{//���С�ھ������ǰʱ�����ػ�ʱ�仹�ж��ٷ�
		for (i = 0; i <= 3; i++)
			YEAR[i] = time_off[i];
		for (i = 4; i <= 5; i++)
			MON[i-4] = time_off[i];
		for (i = 6; i <= 7; i++)
			DAY[i-6] = time_off[i];
		for (i = 8; i <= 9; i++)
			HOUR[i-8] = time_off[i];
		for (i = 10; i <= 11; i++)
			MIN[i-10] = time_off[i];
		sscanf_s(YEAR, "%d", &year);
		sscanf_s(MON, "%d", &mon);
		sscanf_s(DAY, "%d", &day);
		sscanf_s(HOUR, "%d", &hour);
		sscanf_s(MIN, "%d", &min);
		timeoff.tm_year = year - 1900;
		timeoff.tm_mon = mon - 1;
		timeoff.tm_mday = day;
		timeoff.tm_hour = hour - 1;
		timeoff.tm_min = min - 1;
		for (i = 0; i <= 3; i++)
			YEAR[i] = time_now[i];
		for (i = 4; i <= 5; i++)
			MON[i - 4] = time_now[i];
		for (i = 6; i <= 7; i++)
			DAY[i - 6] = time_now[i];
		for (i = 8; i <= 9; i++)
			HOUR[i - 8] = time_now[i];
		for (i = 10; i <= 11; i++)
			MIN[i - 10] = time_now[i];
		sscanf_s(YEAR, "%d", &year);
		sscanf_s(MON, "%d", &mon);
		sscanf_s(DAY, "%d", &day);
		sscanf_s(HOUR, "%d", &hour);
		sscanf_s(MIN, "%d", &min);
		timenow.tm_year = year - 1900;
		timenow.tm_mon = mon - 1;
		timenow.tm_mday = day;
		timenow.tm_hour = hour - 1;
		timenow.tm_min = min - 1;
		//�����ǰʱ�����ػ�ʱ�仹�ж�����
		now = mktime(&timenow);
		off = mktime(&timeoff);
		temp = (long)(off - now);
		sprintf_s(temp_time, SIZE, "%ld", temp);
		strcat_s(command, 20, temp_time);
		system(command);//ִ�йػ�����
		exit(EXIT_SUCCESS);
	}
	//�����жϵ�ǰʱ���Ƿ�С�ڵ�����������ʱ��
	else if (strcmp(time_pre, time_now) > 0)
	{
		//�����ǰʱ��С�ڵ�����������ʱ���ִ��10��ػ����������ػ���ʾ
		system("shutdown -s -t 10");
		//�����ػ���ʾ
		MessageBox(NULL, (tip), TEXT("���棡"), MB_OK | MB_ICONWARNING);
		exit(EXIT_SUCCESS);
	}
	return 0;
}


/*д����Թرպ���Ϣ��ʱ�䣬�������´�������ʱ�䡣
���磺����������40������Ϣ10���ӣ�
Ҳ����˵�����´���������ʱ��Ҫ�ڵ�ǰʱ���ϼ���40��������ʱ���10������Ϣʱ��*/
int WriteTime(void)
{
	FILE *fp;
	errno_t err;
	int min, hour, day, mon, year, run_time, break_time;
	int i = 0;
	Read_File(&run_time, &break_time);

	//���ļ�,�����ж��ļ��Ƿ�������
	if ((err = fopen_s(&fp, time_dat_path, "w")) != 0)
		return 1;

	//��ȡ��ǰʱ��
	time_t pre_time;
	struct tm p;
	time(&pre_time);
	localtime_s(&p, &pre_time);
	min = p.tm_min + run_time + break_time;//�ڵ�ǰ���ӵĻ����ϼ���xx���ӵ�����ʱ���xx������Ϣʱ��
	hour = p.tm_hour;
	day = p.tm_mday;
	mon = p.tm_mon + 1;
	year = p.tm_year + 1900;

	while (i++ < 3)
	{
		if (i == 2)
		{
			min = p.tm_min + run_time;
			hour = p.tm_hour;
			day = p.tm_mday;
			mon = p.tm_mon + 1;
			year = p.tm_year + 1900;
		}
		while (min >= 60)
		{
			min -= 60;
			hour++;
		}

		while (hour >= 24)
		{
			hour -= 24;
			day++;
		}
		//�ж��·�
		if (mon == 1 || mon == 3 || mon == 5 || mon == 7 || mon == 8 || mon == 10 || mon == 12)//�жϵ�ǰ�·��Ƿ�Ϊ����
			while (day > 31)
			{
				day -= 31;
				mon++;
			}

		if (mon == 4 || mon == 6 || mon == 9 || mon == 11)//�жϵ�ǰ�·��Ƿ�ΪС��
			while (day > 30)
			{
				day -= 30;
				mon++;
			}

		if (mon == 2)
			if (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0))//�жϵ�ǰ����Ƿ�Ϊ����
			{
				//printf("������\n");
				while (day > 29)
				{
					day -= 29;//����2����29��
					mon++;
				}
			}
			else while (day > 28)
			{
				day -= 28;//ƽ��2����28��
				mon++;
			}

		while (mon > 12) //�жϵ�ǰ�·��Ƿ񳬹�12��
		{
			mon -= 12;
			year++;
		}

		if (i == 1)
		{
			//���ļ�д������´�������ʱ��
			fprintf(fp, "%02d%02d%02d%02d%02d\n", year, mon, day, hour, min);
			//���ļ�д����ʾ��Ϣ
			fprintf(fp, "��ǰΪ��Ϣʱ��,�´δ˵�����������ʱ��Ϊ��%02d��%02d��%02d��%02dʱ%02d�ֺ�    ���Լ�����10���ڹػ���\n", year, mon, day, hour, min);
		}
		else if (i == 2)
			fprintf(fp, "%02d%02d%02d%02d%02d", year, mon, day, hour, min);
	}
	fclose(fp);
	return 0;
}


//ִ�йػ�����
void ShutDown(void)
{
	int  off_time, tmp;
	char command[22] = { "shutdown -s -t " };
	char temp[7];

	//�������ļ���ȡ�ػ�ʱ��
	Read_File(&off_time, &tmp);
	off_time *= 60;//�ѷ�ת��Ϊ��

	sprintf_s(temp, 7, "%d", off_time);
	strcat_s(command, 22, temp);
	system(command);
	system("pause");
}


//�������ļ���ȡ���������е�ʱ���Լ���Ϣʱ��
int Read_File(int *RunTime, int *BreakTime)
{
	int ch, i = 0, j = 0;
	int run_time = 40;	//Ĭ��������ʱ��
	int break_time = 10;//Ĭ����Ϣʱ��
	char config[5];
	FILE *fp;
	errno_t err;

	//�������ļ���ȡ����
	if ((err = fopen_s(&fp, config_file_path, "r")) != 0)//�ж��ļ��Ƿ�������
	{//��ʧ�ܾͷ���Ĭ��ʱ��
		*RunTime = run_time;
		*BreakTime = break_time;
		return 1;
	}
	if ((ch = fgetc(fp)) != EOF)  //�ж��ļ��Ƿ�Ϊ��
		do
		{
			ch = fgetc(fp);
			if (ch == '=')        //�ж�ch�Ƿ�Ϊ��=��
				while ((ch = fgetc(fp)) != ';')
				{
					if (ch == ' ')//������ȡ�Ŀո�
						continue;
					config[i] = ch;
					i++;
				}
			if (ch == '\n')       //�ж��Ƿ������һ��
			{
				if (j == 0)       //�ж��Ƿ�����ڶ���
				{
					sscanf_s(config, "%d", &run_time); //�ѵ�һ�е����ݱ��浽run_time����һ�е������ǵ��������е�ʱ��
					for (i = 0; i < 5; i++)
						config[i] = '\0';//��ʼ������
				}
				j++;
				i = 0;
			}
		} while (ch != EOF);
		sscanf_s(config, "%d", &break_time); //�ѵڶ��е����ݱ��浽break_time���ڶ��е������ǵ��Թػ�����Ϣ��ʱ��
		fclose(fp);
		*RunTime = run_time;
		*BreakTime = break_time;
		return 0;
}


//��һ����������
void FirstStart(void)
{
	//��ʾ�������д���
	HWND hwnd;
	hwnd = FindWindow("ConsoleWindowClass", NULL);
	if (hwnd)
		ShowWindow(hwnd, SW_SHOW);

	system("title ��һ������... && mode con cols=52 lines=17");

	int ch, i = 5;
	char content[200] = { "run_time = " };
	char path[MAX_PATH],run_time[5], break_time[5];
	char *SubKey = { "Software\\Microsoft\\Windows\\CurrentVersion\\Run" }; //ϵͳ������·��
	char *timec_path = { "C:\\Program Files\\timec\\timec���ذ�.exe" };     //ע���������·���Լ����Ƶ�Ŀ���ļ���·��
	HKEY hKey;
	FILE *fpread, *fpwrite;
	errno_t err;

	for (; i > 0; i--)
	{
		printf("\t      timec���ذ� ��һ������...\n\n\n\n\n\n\t\t    ���ڳ�ʼ��...\n\n\n\n\n");
		printf("��ܰ��ʾ�����Թ���ԱȨ�����б���������˳�ɱ�������\n\n\n");
		printf("\t\t\t%d", i);
		Sleep(1000);
		system("cls");
	}
	system("cls");
	printf("\n��ܰ��ʾ�����������趨ʱ�䣬��༭��C:\\Users\\Timec_config.txt����");
	printf("\n\n\n�����������е�ʱ��\n��Ҳ���ǵ�������XX���Ӻ�ǿ�ƹػ�����λ�����ӣ���");
	scanf_s("%s", run_time, 5); 
	Flush();
	printf("\n\n\n��������Ϣʱ��\n��Ҳ���ǵ���ǿ�ƹػ�����Ҫ��XX���Ӻ����ʹ�á���λ�����ӣ���");
	scanf_s("%s", break_time, 5);
	Flush();
	
	//���������ļ�
	strcat_s(content, 200, run_time);
	strcat_s(content, 200, " ;     ���������е�ʱ�䡣\nbreak_time = ");
	strcat_s(content, 200, break_time);
	strcat_s(content, 200, " ;  ��Ϣ��ʱ�䡣\n\nע�⣺����ֵ�ĵ�λ��Ϊ�֣�ֵ��Χ5~999��\n�����뱣��ԭ��ʽ��������ֳ������������У���ɾ�����ļ���");
	if ((err = fopen_s(&fpwrite, config_file_path, "w")) != 0)
	{
		remove(folder_path);
		perror("\n���������ļ�ʧ�ܣ�");
		system("pause");
		exit(EXIT_FAILURE);
	}
	fputs(content, fpwrite);
	fclose(fpwrite);

	//�����ļ�
	system("cls && title ������������������˳�ɱ��");
	printf("\n\n\n\n\nע�⣺������������������⵽ɱ�����أ��������������������\n\n\t        ��������޷�����������\n\n\n");
	Sleep(3000);

	GetModuleFileName(NULL, path, 100); //����windows api��á�timec���ذ�.exe����·��
	
	if ((err = fopen_s(&fpread, path, "rb")) != 0)
	{
		remove(folder_path);
		perror("�����ļ�ʧ�ܣ�");
		system("pause");
		exit(EXIT_FAILURE);
	}

	if ((err = fopen_s(&fpwrite, timec_path, "wb")) != 0)
	{
		remove(folder_path);
		perror("�����ļ�ʧ�ܣ�");
		system("pause");
		exit(EXIT_FAILURE);
	}

	do
	{
		ch = fgetc(fpread);
		fputc(ch, fpwrite);
	} while (ch != EOF);

	fclose(fpread);//�ر��ļ�
	fclose(fpwrite);

	//���������ļ�
	if ((err = fopen_s(&fpwrite, time_dat_path, "w")) != 0)
	{
		remove(folder_path);
		perror("���������ļ�ʧ�ܣ�");
		system("pause");
		exit(EXIT_FAILURE);
	}
	fclose(fpwrite);

	//��ע������������ 
	if (RegOpenKeyEx(HKEY_CURRENT_USER, SubKey, 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)
	{  //���һ����Key,������ֵ
		RegSetValueEx(hKey, "timecyincang", 0, REG_SZ, (BYTE *)timec_path, strlen(timec_path));
		RegCloseKey(hKey);//�ر�ע���

		i = MessageBox(NULL, TEXT("����������ɹ�����������Ч��\n����ȡ��������timec�Զ����ɾ����\n�Ƿ�����������\n��ܰ��ʾ������ǰ�뱣����������ϣ�"), TEXT("����������ɹ���"), MB_YESNO | MB_ICONQUESTION);
		if (i == IDYES)
			system("shutdown -r -t 00");
		else if (i == IDNO)
			exit(EXIT_SUCCESS);
	}
	else
	{
		MessageBox(NULL, TEXT("���������ʧ�ܣ����Թ���ԱȨ�����б�������ԣ�"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
		remove(folder_path);
		exit(EXIT_SUCCESS);
	}
	exit(EXIT_SUCCESS);
}


//���������
void Flush(void)
{
	int ch;
	while ((ch = getchar()) != '\n' && ch != EOF)
		;
}


//���fgets��ȡ��'\n'
void fgets_n(char *str)
{
	char *find;
	if ((find = strchr(str, '\n')))
		*find = '\0';
}