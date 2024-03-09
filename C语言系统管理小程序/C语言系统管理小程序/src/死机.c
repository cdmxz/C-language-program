#undef UNICODE
#include <windows.h>
#include <tlhelp32.h> 
#include <tchar.h>
#include <stdio.h>

BOOL Kill_process(DWORD processId)
{
	HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, processId);
	if (hProcess == NULL)
		return FALSE;
	if (!TerminateProcess(hProcess, 0))
		return FALSE;
	return TRUE;
}

void kill_all_process()
{
	PROCESSENTRY32 pe32 = { 0 };
	pe32.dwSize = sizeof(pe32);
	// 获取当前进程id
	DWORD cur_process_id = GetCurrentProcessId();

	// 给系统内的所有进程拍一个快照
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		printf("CreateToolhelp32Snapshot调用失败！ \n");
		return;
	}

	// 遍历进程快照
	BOOL bMore = Process32First(hProcessSnap, &pe32);
	while (bMore)
	{
		if (cur_process_id == pe32.th32ProcessID)
		{
			continue;
		}
		printf("进程名: %s\n", pe32.szExeFile);
		printf("进程id：%u \n\n", pe32.th32ProcessID);
		// 杀死该进程
		Kill_process(pe32.th32ProcessID);
		bMore = Process32Next(hProcessSnap, &pe32);
	}

	CloseHandle(hProcessSnap);
	getchar();
}