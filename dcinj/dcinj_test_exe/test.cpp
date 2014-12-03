
#include <Windows.h>
#include <tchar.h>
#include <stdio.h>

#include "..\dcinj\dcinjapi.h"
#include "..\dcinj_test_dll\test.h"

#if defined(_M_X64)

#if defined(DEBUG) || defined(_DEBUG)
#pragma comment(lib, "..\\output\\x64\\Debug\\dcinj.lib")
#else
#pragma comment(lib, "..\\output\\x64\\Release\\dcinj.lib")
#endif

#else // if !defined(_M_X64)

#if defined(DEBUG) || defined(_DEBUG)
#pragma comment(lib, "..\\output\\Win32\\Debug\\dcinj.lib")
#else
#pragma comment(lib, "..\\output\\Win32\\Release\\dcinj.lib")
#endif

#endif // defined(_M_X64)

int main(int argc, char * argv[])
{
	PROCESS_INFORMATION processInfo;
	STARTUPINFO startupInfo;
	TCHAR szFileName[MAX_PATH];
	HANDLE hProcess;
	HMODULE hModInjected;
	DWORD dwReturnValue;
	REMOTEDATA remoteData;
	

	
	printf("starting \"notepad.exe\"\n");
	RtlZeroMemory(&startupInfo, sizeof(STARTUPINFO));
	GetStartupInfo(&startupInfo);
	RtlZeroMemory(szFileName, MAX_PATH * sizeof(TCHAR));
	lstrcpy(szFileName, TEXT("NOTEPAD.EXE"));
	CreateProcess(NULL, szFileName, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &startupInfo, &processInfo);
	WaitForInputIdle(processInfo.hProcess, 5000);
	CloseHandle(processInfo.hThread);
	CloseHandle(processInfo.hProcess);
	printf("process id %d\n", processInfo.dwProcessId);

	printf("opening notepad process\n");
	hProcess = OpenProcess(PROCESS_TERMINATE | PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE, FALSE, processInfo.dwProcessId);
	printf("process handle 0x%X\n", (LONG_PTR)hProcess);
	
	GetModuleFileName(NULL, szFileName, MAX_PATH);
	*(_tcsrchr(szFileName, '\\') + 1) = 0;
	lstrcat(szFileName, TEXT("dcinj_test_dll.dll"));
	_tprintf(TEXT("injecting dll %s\n"), szFileName);
	hModInjected = RemoteInject(hProcess, szFileName);
	printf("dll handle 0x%X\n", (LONG_PTR)hModInjected);
	
	printf("calling \"CallMe\"\n");
	RemoteCall(hProcess, hModInjected, TEXT("CallMe"), &dwReturnValue);
	printf("CallMe returned 0x%X\n", dwReturnValue);

	lstrcpy(remoteData.szString, TEXT("Hello world."));
	_tprintf(TEXT("calling \"CallMeEx\" passing string \"%s\"\n"), remoteData.szString);
	RemoteCallEx(hProcess, hModInjected, TEXT("CallMeEx"), &remoteData, sizeof(REMOTEDATA), &dwReturnValue);
	_tprintf(TEXT("CallMeEx returned 0x%X and changed string into \"%s\"\n"), dwReturnValue, remoteData.szString);
		
	printf("Releasing dll\n");
	RemoteEject(hProcess, hModInjected);

	printf("Terminating process\n");
	TerminateProcess( hProcess, 0);

	return 0;
}