
////////////////////////////////////////////////////////////////////////
// http://stackoverflow.com/questions/647310/c-sharp-to-c-cli-to-c-dll-system-io-filenotfoundexception/881110#881110

#include <Windows.h>


BOOL WINAPI DllMain(HINSTANCE hinstDLL, 
					DWORD fdwReason, 
					LPVOID lpvReserved )
{
	switch(fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hinstDLL);
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}