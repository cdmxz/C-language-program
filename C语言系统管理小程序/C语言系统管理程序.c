//����ʱ�䣺2019��1��29  Ŀǰ����޸�ʱ�䣺2019��8��26��
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <direct.h>//Ϊ_getcwd�ṩ����


int   autoshut(void);   //�ʵ�
void  shut(void);       //�ػ�
void  reboot(void);     //����
void  hosts(void);      //�޸�hosts
int   sysconfig(void);  //�鿴��������
void  flush(void);      //���������
void  function(void);   //ʵ�ù���
void  delfile(void);    //ɾ���ļ�


//����ȫ�ֱ���
int num;
FILE *fpRead, *fpWrite; 
errno_t err;



int main(void)
{
	while (1)
	{
		system("mode con:cols=57 lines=25 && color 0F && title C����ϵͳ����С���� ���Թ���ԱȨ�޴򿪱����");//����
		delfile();//ɾ���ļ�

		printf("\n        ------��ӭʹ��C����ϵͳ����С����------\n");
		printf("\t\t      1���ػ�\n");
		printf("\t\t      2������\n");
		printf("\t\t      3������\n");
		printf("\t\t      4��˯��\n");
		printf("\t\t      5��ȡ�����ػ���������\n");
		printf("\t\t      6��ע��\n");
		printf("\t\t      7��ʵ�ù���\n");
		printf("\t\t      8���鿴��������\n");
		printf("\t\t      9��ϵͳ����\n");
		printf("\t\t     10������Դ��\n");
		printf("\t\t      0���˳�\n\n");
		printf("     ��������ţ�");
		while (scanf_s("%d", &num) != 1)//����scanf_s�ķ���ֵ�ж�����������Ƿ�Ϊ�ַ�
		{
			flush();//��ջ����������ַ�
			printf("\n����������������룺");
		}
		flush();


		switch (num)
		{
		case 0:exit(0); //�˳�
		case 1:shut(); break;//�ػ�
		case 2:reboot(); break;//����
		case 3:system("shutdown -h"); Sleep(2000); exit(0);//����

		case 4: //˯��
		{
			system("cls && color 04");
		
			printf("\n\n\n\n\nע�⣺\n��ȷ���˼�����ѹر����߹��ܣ�����ᵱ���������߹���\n\n");
			printf("��������ر����߹��ܿ��ڡ�9�������رչ��ܡ��ҵ���\n");
			Sleep(2000);
			system("rundll32.exe powrprof.dll,SetSuspendState 0,1,0");//ִ��˯������ 
			return 0;
		}

		case 5:system("shutdown -a"); break; //ȡ��
		case 6:system("shutdown -l"); exit(0); //ע��
		case 7:function();  break;//ʵ�ù���
		case 8:sysconfig(); break;//�鿴��������

		case 9://ϵͳ����
		{
			system("cls && color 71 && mode con:cols=63 lines=28");
			system("title ϵͳ���� PS������7���أ�����37�鿴ʹ��˵��");

			printf("ϵͳ����\n");
			printf(" 1����������        2���ر�����         38���鿴ϵͳ��Ϣ\n");
			printf(" 3���򿪿������    4���򿪲�����༭��  5���򿪱����û�����\n");
			printf(" 6�����豸������  8���鿴ϵͳ�汾      9����ע���༭��\n");
			printf("10����IP��ַ��� 11���򿪼��������   12����Directx��Ϲ���\n");
			printf("13�����������   14���򿪱��ط���     15�����ַ��༭\n\n");

			printf("���̹���\n");
			printf("16��������������   17���������̹���     18���������̼��\n\n");

			printf("ϵͳ����\n");
			printf("19��������Դ������ 20���������������   21���������ܼ�����\n");
			printf("22������Զ������   23���������±�       24��������ͼ��\n");
			printf("25������д�ְ�     26������Media Player 27�������ַ�ӳ���\n");
			printf("28������������     29�������Ŵ�       30������ľ�����󹤾�\n\n");
			
			printf("IP��ַ��\n");
			printf("31���鿴IP��ַ     32���ر�IPv6��ʱ��ַ 33������IPv6��ʱ��ַ\n\n");
		
			printf("������\n");
			printf("34���޸�hosts	   35�������������        36���򿪲ʵ�\n\n");
			printf("��������ţ�");
			while (scanf_s("%d", &num) != 1)//�����ַ�����
			{
				flush();//��ջ����������ַ�
				printf("\n����������������룺\n");
			}
			flush();


			switch (num)
			{
			case 1:system("powercfg -h on"); system("pause"); break;
			case 2:system("powercfg -h off"); system("pause"); break;
			case 3:system("control");       break;
			case 4:system("gpedit.msc");    break;
			case 5:system("lusrmgr.msc");   break;
			case 6:system("devmgmt.msc");   break;
			case 7:						    break;
			case 8:system("winver");        break;
			case 9:system("regedit");       break;
			case 10:system("cls && Nslookup"); break;
			case 11:system("compmgmt.msc"); break;
			case 12:system("dxdiag");       break;
			case 13:system("dcomcnfg");     break;
			case 14:system("services.msc"); break;
			case 15:system("eudcedit");     break;
			case 16:system("cleanmgr");     break;
			case 17:system("diskmgmt.msc"); break;
			case 18:system("cls && mode con:cols=70 lines=40 && chkdsk.exe");  system("pause"); break;
			case 19:system("explorer");    break;
			case 20:system("taskmgr");     break;
			case 21:system("perfmon.msc"); break;
			case 22:system("mstsc");       break;
			case 23:system("notepad");     break;
			case 24:system("mspaint");     break;
			case 25:system("write");       break;
			case 26:system("dvdplay");     break;
			case 27:system("charmap");     break;
			case 28:system("calc");        break;
			case 29:system("magnify");     break;
			case 30:system("IExpress");    break;
			case 31:system("cls && ipconfig /all > ����IP��Ϣ.txt && ����IP��Ϣ.txt"); system("pause"); break;
			case 32:printf("\n\t   "); system("netsh interface ipv6 set privacy state=disable"); system("pause"); break;
			case 33:printf("\n\t   "); system("netsh interface ipv6 set privacy state=enabled"); system("pause"); break;
			case 34:hosts();               break;
			case 35:
			{
				char name[50];
				char process[70] = { "taskkill /f /t /im " };
				system("cls && tasklist >> ������Ϣ.txt && ������Ϣ.txt");
				remove("������Ϣ.txt");
				printf("������Ҫ�����Ľ������ƣ����磺1.exe����");
				gets_s(name, 50);
				strcat_s(process, sizeof(process), name);
				system(process);
				system("pause");
				break;
			}
			case 36:autoshut();            break;
			case 37:system("start readme.txt");  break;
			case 38://�鿴ϵͳ��Ϣ
			system("systeminfo >> ϵͳ��Ϣ.txt && ϵͳ��Ϣ.txt"); system("pause"); break; //�������Ϣ��������ǰ�������ڵ�Ŀ¼����ϵͳ��Ϣ.txt
			
			default:printf("\n\n\t�������!\n\n"); Sleep(1000); break;
			}
			break;
		}//case 9		
		
		case 10:system("start https://cdmxz.github.io"); break;
		
		default:printf("\n\n\t     �������!\n\n"); Sleep(1000); break; //while�����ַ�����switch�������ִ���
		}//switch
	
}//whileѭ��ʹ����ִ�к󷵻ؿ�ͷ
		return 0;
}



//�޸�hosts
void hosts(void)  
{
	int  ch;
	
	while (1)
	{
		system("cls && mode con:cols=60 lines=20 && title hosts�޸� ���Թ���ԱȨ�޴򿪴����");

		printf("\n��ܰ��ʾ��1.���Թ���ԱȨ�޴򿪴����������Ȩ�޲�����ʧ�ܡ�\n");
		printf("          2.����Ѿ�ʹ�ù���ԱȨ�޴򿪱��������Ա���ʾ��");
		printf("\n\n\n\t      1����ϵͳĬ�ϱ༭����hosts�ļ�\n\n");
		printf("\t      2����ԭĬ��hosts�ļ�\n\n");
		printf("\t      3���������������\n\n\n");
		printf(" ��������ţ�");
		while (scanf_s("%d", &num) != 1)
		{
			flush();//��ջ����������ַ�
			printf("\n����������������룺");
		}
		flush();


		if (num == 1)//��hosts
		{
			MessageBox(NULL, TEXT("ע�⣺�༭��hosts�ļ�������ݱ��������ʾ��\n���밴���������...������������ٹرձ�����\n����ֱ�ӹرձ����򣬷�������hosts�ļ��𻵣�"), TEXT("ע�⣺"), MB_OK | MB_ICONWARNING);
			if (rename("C:\\Windows\\System32\\drivers\\etc\\hosts", "C:\\Windows\\System32\\drivers\\etc\\hosts.txt") != 0)//������hosts
			{
				MessageBox(NULL, TEXT("hosts�ļ�������ʧ�ܣ����Թ���ԱȨ�����б����\n��ԭĬ��hosts�ļ���"), TEXT("error!"), MB_OK | MB_ICONERROR);
				continue;
			}

			system("start C:\\Windows\\System32\\drivers\\etc\\hosts.txt");//��hosts
			//printf("ע�⣺�༭��hosts�ļ�������ݱ��������ʾ��\n���밴���������...������������ٹرձ�����\n����ֱ�ӹرձ����򣬷�������hosts�ļ��𻵣�\n\n");
			printf("\n\n\t\t");
			system("pause");


			if (rename("C:\\Windows\\System32\\drivers\\etc\\hosts.txt", "C:\\Windows\\System32\\drivers\\etc\\hosts") != 0)
			{
				MessageBox(NULL, TEXT("hosts�ļ�������ʧ�ܣ�\n���ֶ�����C:\\Windows\\System32\\drivers\\etc\\\nĿ¼�°ѡ�hosts.txt��������λ��hosts����"), TEXT("Error!"), MB_OK | MB_ICONERROR);
				system("start C:\\Windows\\System32\\drivers\\etc");
			}
		}

		else if (num == 2)//�ָ�hosts
		{
			if (err = fopen_s(&fpRead, "hosts.dll", "rb") != 0)//��hosts.dll
			{
				MessageBox(NULL, TEXT("���ļ��������鱾����Ŀ¼���Ƿ��С�hosts.dll����"), TEXT("Error!"), MB_OK | MB_ICONERROR);
				continue;
			}
			if (err = fopen_s(&fpWrite, "C:\\Windows\\System32\\drivers\\etc\\hosts", "wb") != 0)//��hosts
			{
				MessageBox(NULL, TEXT("����hosts�ļ�ʧ�ܣ����Թ���ԱȨ�����б������"), TEXT("Error!"), MB_OK | MB_ICONERROR);
				exit(1);
			}

			while ((ch = fgetc(fpRead)) != EOF)//��ȡhosts.dll���������д�뵽hosts
			{
				ch = ch - 'h' + 's' - 'o';//����hosts.dll�ļ��������
				fputc(ch, fpWrite);//д�뵽hosts
			}

			if (fclose(fpWrite) != EOF || fclose(fpRead) != EOF)//�ر��ļ�
			{
				perror("Error:");
				system("pause");
			}
		}
		

		else if (num == 3)
			break;

		else
		{
			printf("\n\t�������");
			Sleep(1000);
		}
	}
}



//��ȡ��������
int sysconfig(void)
{
	
	int ch;

	system("cls && mode con:cols=90 lines=30");


	if ((err = fopen_s(&fpRead, "system.dll", "rb")) != 0)//��DLL�ļ�
	{
		MessageBox(NULL, TEXT("�ļ��򿪴���\n���鱾����Ŀ¼���Ƿ��С�system.dll��"), TEXT("error!"), MB_OK | MB_ICONERROR);
		return 1;
	}
	if ((err = fopen_s(&fpWrite, "��ȡӲ����Ϣ.bat", "wb")) != 0)
	{
		MessageBox(NULL, TEXT("�����ļ���������\n���Թ���ԱȨ�����б������"), TEXT("error!"), MB_OK | MB_ICONERROR);
		remove("��ȡӲ����Ϣ.bat");
		return 1;             
	}

	
	//������Ӧ�������˰ɣ���ν��DLL�ļ�ֻ��һ���������ֵļ����ļ���������
	while ((ch = fgetc(fpRead)) != EOF)  //���ļ��ж�ȡ���ݵ�ch��EOF���ļ�������־��
	{
		ch = ch - 'p' + 'z' - 'i' + 2021;//�����ļ�����
		fputc(ch, fpWrite); //������ܺ�����ݵ���һ���ļ�
	}

	
	if (fclose(fpWrite) == EOF)//�ر��ļ�
		remove("��ȡӲ����Ϣ.bat");

	fclose(fpRead);    //�ر��ļ�

	//����API�����á���ȡӲ����Ϣ��Ϊ�����ļ� 
	SetFileAttributes("��ȡӲ����Ϣ.bat", FILE_ATTRIBUTE_HIDDEN);


	printf("\n\t   ��ܰ��ʾ�����Թ���ԱȨ�����б����������ʹ�ù���ԱȨ����������ԡ�\n\n\n\n");
	//printf("ע�⣺���ڴ˳�����ʾ�����밴���������...��������������ٹرձ���������ֱ�ӹرձ�����\n");
	printf("\n\n\n\n\t\t���ڻ�ȡ����������Ϣ��Լ30��-1�������ҡ������ĵȴ�...\n\n");
	Sleep(5000);

	system("cls && ��ȡӲ����Ϣ.bat");//�򿪻�ȡӲ����Ϣ
	system("pause");
	remove("��ȡӲ����Ϣ.bat");//ɾ���ļ�
	return 0;
}


//�ػ� 
void shut(void)
{
	int min, hour;

	system("mode con cols=55 lines=18 && color 06 && title �ػ� PS�����Թ���ԱȨ�����б����");

	printf("��ܰ��ʾ���ڵ�ǰҳ������7���ء�  �������ֺ��밴�س���\n");
	printf("          �������ʱ��������2��  XXX���ػ�������3��\n");
	printf("          �Զ���XXСʱ:XX���ӹػ�����������4��\n");
	printf("          ɾ���Զ���XXСʱ:XX���ӹػ�������6��\n");
	printf("          �鿴��ǰ����������8��\n\n\n\n\n");

	printf("�����루0��1��5��10��20��30��40��50��60���ֺ�ػ���\n\n���������֣�");
	while (scanf_s("%d", &min) != 1)
	{
		flush();//��ջ����������ַ�
		printf("\n����������������룺");
	}
	flush();


	switch (min)
	{
	case 0:system("shutdown -s -t 00"); break;
	case 1:system("shutdown -s -t 60"); break;
	case 2://�ػ�����λ��Сʱ�� 
	{
		system("cls");
		printf("       ��ܰ��ʾ��1.����0����  2.������ź��밴�س�\n\n");
		printf("\t\t  1��1.5Сʱ��ػ�\n");
		printf("\t\t  2��2  Сʱ��ػ�\n");
		printf("\t\t  3��2.5Сʱ��ػ�\n");
		printf("\t\t  4��3  Сʱ��ػ�\n");
		printf("\t\t  5��3.5Сʱ��ػ�\n");
		printf("\t\t  6��4  Сʱ��ػ�\n");
		printf("\t\t  7��4.5Сʱ��ػ�\n");
		printf("\t\t  8��5  Сʱ��ػ�\n");
		printf("\t\t  9��5.5Сʱ��ػ�\n");
		printf("��������ţ�");
		while (scanf_s("%d", &hour) != 1)//�����ַ�����
		{
			printf("\n����������������룺");
			flush();//��ջ����������ַ�
		}
		flush();

		switch (hour)
		{
		case 0:break;
		case 1:system("shutdown -s -t 5400"); break;
		case 2:system("shutdown -s -t 7200"); break;
		case 3:system("shutdown -s -t 9000"); break;
		case 4:system("shutdown -s -t 10800"); break;
		case 5:system("shutdown -s -t 12600"); break;
		case 6:system("shutdown -s -t 14400"); break;
		case 7:system("shutdown -s -t 16200"); break;
		case 8:system("shutdown -s -t 18000"); break;
		case 9:system("shutdown -s -t 19800"); break;
		default:printf("\n\n\t �������"); Sleep(1000); break;
		}
		break;
	}//�ػ�����λ��Сʱ��

	case 3://�Զ���ʱ��ػ�
	{
		char shut[22] = { "shutdown -s -t " };
		char tmp[7] = { 0 };

		system("cls");
		printf("\n��ܰ��ʾ���������ֺ��밴�س������ʱ�䲻�ܳ���999999�롣\n\n\n\n\n");
		printf("��ܰ��ʾ��ֻ���������ּ��ɣ��������뵥λ\n");
		printf("������������ػ�����λ���룩��");

		scanf_s("%s", tmp, 7);
		flush();//�ϲ�����

		strcat_s(shut, sizeof(shut), tmp);//�ϲ�����
		system(shut);//ִ�йػ�����

		Sleep(1000);
		break;
	}

	case 4:
	{
		char schtasks[76] = { "schtasks /create /tn \"�ػ�\" /tr \"shutdown /s /t 00\" /sc once /st " };
		char shut[6] = { 0 };

		system("cls");
		printf("\n��ܰ��ʾ�����ùػ���ʱ���벻ҪС�ڻ���ڵ�ǰʱ�䡣\n\n\n\n");
		printf("��������ٵ�ػ��������磺12:00����");

		scanf_s("%s", shut, 6);
		flush();

		strcat_s(schtasks, sizeof(schtasks), shut);//�ϲ�����
		system(schtasks);

		printf("\n\n�����ʾ���ܾ����ʡ������������Թ���ԱȨ�����д������\n\n");
		system("pause");
		break;
	}

	case 5:system("shutdown -s -t 300"); break;

	case 6:
	{
		system("cls && schtasks /delete /tn �ػ� /F");
		printf("\n\n\n\n\n�����ʾ���ܾ����ʡ������������Թ���ԱȨ�����д������\n\n");
		system("pause");
		break;
	}

	case 7:break;

	case 8:
	{
		system("cls && schtasks > ��ǰ����.txt && ��ǰ����.txt");
		printf("\n\n\n�ļ��ѵ�������ǰ����Ŀ¼\n\n");
		system("pause");
		break;
	}

	case 10:system("shutdown -s -t 600"); break;
	case 20:system("shutdown -s -t 1200"); break;
	case 30:system("shutdown -s -t 1800"); break;
	case 40:system("shutdown -s -t 2400"); break;
	case 50:system("shutdown -s -t 3000"); break;
	case 60:system("shutdown -s -t 3600"); break;
	default:printf("\n\n\t �������"); Sleep(1000); break;
	}
}



//����
void reboot(void)
{
	int min, hour;

	system("mode con cols=55 lines=18 && color 06 && cls && title ���� PS�����Թ���ԱȨ�����б����");

	printf("��ܰ��ʾ���ڵ�ǰҳ������7���ء�  �������ֺ��밴�س���\n");
	printf("          �������ʱ��������2��  XXX�������������3��\n");
	printf("          �Զ���XXСʱ:XX������������������4��\n");
	printf("          ɾ���Զ���XXСʱ:XX��������������6��\n");
	printf("          �鿴��ǰ����������8��\n\n\n\n\n");
	printf("�����루0��1��5��10��20��30��40��50��60���ֺ�������\n\n���������֣�");

	while (scanf_s("%d", &min) != 1)
	{
		printf("\n����������������룺");
		flush();//��ջ����������ַ�
	}

	switch (min)
	{
	case 0:system("shutdown -r -t 00"); break;
	case 1:system("shutdown -r -t 60"); break;
	case 2://��������λ��Сʱ�� 
	{
		system("cls");
		printf("       ��ܰ��ʾ��1.����0����  2.������ź��밴�س�\n\n");
		printf("\t\t  1��1.5Сʱ������\n");
		printf("\t\t  2��2  Сʱ������\n");
		printf("\t\t  3��2.5Сʱ������\n");
		printf("\t\t  4��3  Сʱ������\n");
		printf("\t\t  5��3.5Сʱ������\n");
		printf("\t\t  6��4  Сʱ������\n");
		printf("\t\t  7��4.5Сʱ������\n");
		printf("\t\t  8��5  Сʱ������\n");
		printf("\t\t  9��5.5Сʱ������\n");
		printf("��������ţ�");
		while (scanf_s("%d", &hour) != 1)//�����ַ�����
		{
			printf("\n����������������룺");
			flush();//��ջ����������ַ�
		}


		switch (hour)
		{
		case 0:break;
		case 1:system("shutdown -r -t 5400"); break;
		case 2:system("shutdown -r -t 7200"); break;
		case 3:system("shutdown -r -t 9000"); break;
		case 4:system("shutdown -r -t 10800"); break;
		case 5:system("shutdown -r -t 12600"); break;
		case 6:system("shutdown -r -t 14400"); break;
		case 7:system("shutdown -r -t 16200"); break;
		case 8:system("shutdown -r -t 18000"); break;
		case 9:system("shutdown -r -t 19800"); break;
		default:printf("\n\n\t �������"); Sleep(1000); break;
		}
		break;
	}//��������λ��Сʱ��

	case 3:
	{
		char reboot[22] = { "shutdown -r -t " };
		char tmp[7] = { 0 };

		system("cls");
		printf("\n��ܰ��ʾ���������ֺ��밴�س������ʱ�䲻�ܳ���999999�롣\n\n\n\n\n");
		printf("��ܰ��ʾ��ֻ���������ּ��ɣ��������뵥λ\n");
		printf("��������������������λ���룩��");

		scanf_s("%s", tmp, 7);
		flush();

		strcat_s(reboot, sizeof(reboot), tmp);//�ϲ�����
		system(reboot);//ִ����������

		Sleep(1000);
		break;
	}
	case 4:
	{
		char schtasks[76] = { "schtasks /create /tn \"����\" /tr \"shutdown /r /t 00\" /sc once /st /0" };
		char reboot[6] = { 0 };

		system("cls");
		printf("\n��ܰ��ʾ������������ʱ���벻ҪС�ڻ���ڵ�ǰʱ�䡣\n\n\n\n");
		printf("��������ٵ������������磺12:00����");

		scanf_s("%s", reboot, 6);
		flush();

		strcat_s(schtasks, sizeof(schtasks), reboot);
		system(schtasks);

		printf("\n\n�����ʾ���ܾ����ʡ����������Թ���ԱȨ�����д������\n\n");
		system("pause");
		break;
	}

	case 5:system("shutdown -r -t 300"); break;

	case 6:
	{
		system("cls && schtasks /delete /tn ���� /F");
		printf("\n\n\n\n\n�����ʾ���ܾ����ʡ����������Թ���ԱȨ�����д������\n\n");
		system("pause");
		break;
	}

	case 7:break;

	case 8:
	{
		system("cls && schtasks > ��ǰ����.txt && ��ǰ����.txt");
		printf("\n\n\n�ļ��ѵ�������ǰ����Ŀ¼\n\n");
		system("pause");
		break;
	}

	case 10:system("shutdown -r -t 600"); break;
	case 20:system("shutdown -r -t 1200"); break;
	case 30:system("shutdown -r -t 1800"); break;
	case 40:system("shutdown -r -t 2400"); break;
	case 50:system("shutdown -r -t 3000"); break;
	case 60:system("shutdown -r -t 3600"); break;
	default:printf("\n\n\t �������"); Sleep(1000); break;
	}
}


//���������
void flush(void)
{
	int ch;
	while ((ch = getchar()) != '\n' && ch != EOF)
		;
}


void  delfile(void)
{
	//ɾ���ļ�
	remove("����IP��Ϣ.txt");
	remove("ϵͳ��Ϣ.txt");
	remove("��ȡӲ����Ϣ.bat");
	remove("������Ϣ.txt");
}