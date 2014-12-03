
#include "test.h"

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	switch(fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		MessageBox(NULL, TEXT("Process attach"), TEXT("DLL Inject OK"), MB_ICONINFORMATION);
		break;
	case DLL_PROCESS_DETACH:
		MessageBox(NULL, TEXT("Process detach"), TEXT("DLL Eject OK"), MB_ICONINFORMATION);
		break;
	}
	return TRUE;
}

DWORD WINAPI CallMe(void)
{
	MessageBox(NULL, TEXT("CallMe"), TEXT("CallMe OK"), MB_ICONINFORMATION);
	return 0xBEEFBABE;
}

DWORD WINAPI CallMeEx(LPVOID lpVoid)
{
	PREMOTEDATA pRemoteData;
	pRemoteData = (PREMOTEDATA)lpVoid;
	MessageBox(NULL, pRemoteData->szString, TEXT("CallMeEx OK"), MB_ICONINFORMATION);
	lstrcpy(pRemoteData->szString, TEXT("Hello universe."));
	return 0xB00BFACE;
}