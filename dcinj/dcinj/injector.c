/*+===================================================================
  File:      injector.c

  Summary:   This file contains the implementation for all the 
			 routines of the DC Injector DLL.

  Functions: RemoteWrite,	RemoteExecute, 
			 RemoteCallA/W,	RemoteCallExA/W,
			 RemoteInject,	RemoteEject, 
			 GetProcLength,	AStringToWString.			 

  Origin:    This file is part of DC Injector.

----------------------------------------------------------------------
  "The MIT License" Copyright (c) 2014 http://bitbucket.org/dogecrew

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files 
(the "Software"), to deal in the Software without restriction, 
including without limitation the rights to use, copy, modify, merge,
publish, distribute, sublicense, and/or sell copies of the Software,
and to permit persons to whom the Software is furnished to do so, 
subject to the following conditions:

The above copyright notice and this permission notice shall be 
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY 
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, 
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
===================================================================+*/

#include "injector.h"

#ifdef __cplusplus
extern "C" {
#endif

/*F+F+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  Function: GetProcLength

  Summary:  See injector.h
-----------------------------------------------------------------F-F*/
SIZE_T GetProcLength(PBYTE pbProcedure)
{
	SIZE_T cbLength;
	PBYTE p;
	int i;
	cbLength = 0;
	__try
	{
		p = pbProcedure;
		for(i  = 0; i < 1024; i++, p++)
		{
			if( *((LPDWORD)p) == PROC_SIGNATURE)
			{
				cbLength = p - pbProcedure;
				break;
			}
		}
	}
	__finally {	}
	return cbLength;
}

/*F+F+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  Function: RemoteCallA

  Summary:  See dcinjapi.h
-----------------------------------------------------------------F-F*/
DCAPI BOOL DCCALL RemoteCallA(	HANDLE hProcess,
								HMODULE hModule,
								LPCSTR lpszProcedure,
								LPDWORD lpdwRetVal )
{
	SIZE_T cbRemoteProc;
	CALLDATA callData;
	size_t ccProcedure;
	BOOL bResult;

	bResult = FALSE;

	// Checking the passed parameters,...
	if(hProcess == NULL 
	|| hModule == NULL 
	|| lpszProcedure == NULL
	|| lpdwRetVal == NULL
	|| !(ccProcedure = lstrlenA(lpszProcedure))
	|| ccProcedure >= MAX_PROC_NAME
	){
		return FALSE;
	}

	// Calculate length of code to inject
	if(!(cbRemoteProc = GetProcLength((PBYTE)RemoteCallProc)))
		return FALSE;

	// Zero the data to pass
	RtlZeroMemory(&callData, sizeof(CALLDATA));

	// Copy the name of the procedure to call
	lstrcpyA(callData.szProcName, lpszProcedure);
	
	// Copy the handle of the injected module
	callData.hModLibrary = hModule;

	// Get the address of GetProcAddress
	if(!(callData.lpfnGetProcAddress = (PFNGetProcAddress)GetProcAddress(GetModuleHandle(KERNEL32_DLL), "GetProcAddress")))
	{
		return FALSE;
	}

	// Execute
	if(RemoteExecute(hProcess, &callData, sizeof(CALLDATA), (LPTHREAD_START_ROUTINE)RemoteCallProc, cbRemoteProc, RE_READ_ARGUMENT_AFTER, lpdwRetVal)
	&& *lpdwRetVal != 0
	&& callData.lpfnProcResult != NULL
	){
		bResult = TRUE;
	}

	return bResult;
}

/*F+F+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  Function: RemoteCallW

  Summary:  See dcinjapi.h
-----------------------------------------------------------------F-F*/
DCAPI BOOL DCCALL RemoteCallW(	HANDLE hProcess,
								HMODULE hModule,
								LPCWSTR lpszProcedure,
								LPDWORD lpdwRetVal )
{
	CHAR * pszProcedure;
	BOOL bResult;

	// convert the procedure name to an ansi string
	// (GetProcAddress only exists in ansi)		
	if(NULL != ( pszProcedure = WStringToAString(lpszProcedure) ) )
	{
		// forward the call to the ansi version 
		bResult = RemoteCallA(hProcess, hModule, pszProcedure, lpdwRetVal);

		// free the ansi string
		FreeMemory(pszProcedure);

		return bResult;
	} 
	else
	{
		return FALSE;
	}
}

/*F+F+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  Function: RemoteCallExA

  Summary:  See dcinjapi.h
-----------------------------------------------------------------F-F*/
DCAPI BOOL DCCALL RemoteCallExA(HANDLE hProcess, 
								HMODULE hModule,
								LPCSTR lpszProcedure, 
								LPVOID lpArgument, 
								SIZE_T cbArgument,
								LPDWORD lpdwRetVal )
{
	SIZE_T cbRemoteProc,
			cbCallData;
	PCALLDATA pCallData;
	int ccProcedure;
	BOOL bResult;
	
	bResult = FALSE;

	// Checking the passed parameters,...
	if(hProcess == NULL 
	|| hModule == NULL 
	|| lpszProcedure == NULL
	|| lpArgument == NULL
	|| cbArgument == 0
	|| lpdwRetVal == NULL
	|| !(ccProcedure = lstrlenA(lpszProcedure))
	|| ccProcedure >= MAX_PROC_NAME
	){
		return FALSE;
	}

	// Calculate the length of the data to inject
	cbCallData = sizeof(CALLDATA) + cbArgument;

	// Calculate length of code to inject
	if(!(cbRemoteProc = GetProcLength((PBYTE)(RemoteCallProcEx))))
	{
		return FALSE;
	}

	// Allocate memory for the CALLDATA structure and the argument
	if(NULL == ( pCallData = (PCALLDATA)AllocMemory(cbCallData) ) )
	{
		return FALSE;
	}

	// Zero memory the data to pass
	RtlZeroMemory(pCallData, cbCallData);

	// Copy the name of the procedure to call
	lstrcpyA(pCallData->szProcName, lpszProcedure);

	// Copy the handle of the injected module
	pCallData->hModLibrary = hModule;

	// Copy the argument data after the structure
	RtlCopyMemory( (pCallData + 1), lpArgument, cbArgument);

	// Get the address of GetProcAddress
	if( NULL != ( pCallData->lpfnGetProcAddress = (PFNGetProcAddress)GetProcAddress(GetModuleHandle(KERNEL32_DLL), "GetProcAddress") ) )
	{
		// Execute and check the result
		if(RemoteExecute(hProcess, pCallData, cbCallData, (LPTHREAD_START_ROUTINE)RemoteCallProcEx, cbRemoteProc, RE_READ_ARGUMENT_AFTER, lpdwRetVal)
		&& *lpdwRetVal != 0
		&& pCallData->lpfnProcResult != NULL
		){
			// Copy the data after the structure back into lpArgument
			RtlCopyMemory( lpArgument, (pCallData + 1), cbArgument);

			bResult = TRUE;
		}
	}
	
	FreeMemory(pCallData);

	return bResult;
}

/*F+F+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  Function: RemoteCallExW

  Summary:  See dcinjapi.h
-----------------------------------------------------------------F-F*/
DCAPI BOOL DCCALL RemoteCallExW(HANDLE hProcess, 
								HMODULE hModule,
								LPCWSTR lpszProcedure, 
								LPVOID lpArgument, 
								SIZE_T cbArgument,
								LPDWORD lpdwRetVal )
{
	CHAR * pszProcedure;
	BOOL bResult;

	// convert the procedure name to an ansi string
	// (GetProcAddress only exists in ansi)
	if(NULL != (pszProcedure = WStringToAString(lpszProcedure) ) )
	{
		// forward the call to the ansi version 
		bResult = RemoteCallExA(hProcess, hModule, pszProcedure, lpArgument, cbArgument, lpdwRetVal);

		// free the ansi string
		FreeMemory(pszProcedure);

		return bResult;
	}
	else
	{
		return FALSE;
	}
}

/*F+F+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  Function: RemoteEject

  Summary:  See dcinjapi.h
-----------------------------------------------------------------F-F*/
DCAPI BOOL DCCALL RemoteEject(HANDLE hProcess,
								HMODULE hModule )
{
	FARPROC farProc;
	DWORD dwThreadId;
	HANDLE hThread;
	DWORD dwExitCode;
	
	// Get the address of FreeLibrary
	if(NULL == (farProc = GetProcAddress(GetModuleHandle(KERNEL32_DLL), "FreeLibrary")))
	{
		return FALSE;
	}

	// Call freelibrary in remote process
	//hThread = CreateRemoteThread(hProcess, NULL, 4096*16, (LPTHREAD_START_ROUTINE)farProc, (LPVOID) hModule, 0, &dwThreadId);
	hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)farProc, (LPVOID) hModule, 0, &dwThreadId);

	// Wait for freelibrary to finish, dll to unload
	if(WAIT_OBJECT_0 !=	WaitForSingleObject(hThread, INFINITE))
	{
		return FALSE;
	}

	// Get the exit code
	if(!GetExitCodeThread(hThread, &dwExitCode))
	{
		return FALSE;
	}
	
	return dwExitCode == 0 ? FALSE : TRUE;
}
	/* Old code
	
	EJECTDATA ejectData;
	SIZE_T cbRemoteProc;
	DWORD dwExitCode;

	if(hProcess == NULL || hModule == NULL)
	{
		return FALSE;
	}
	
	// Calculate length of code to inject
	if(0 == (cbRemoteProc = GetProcLength((PBYTE)RemoteEjectProc)))
	{
		return FALSE;
	}
	
	// Zero memory for the data to pass
	RtlZeroMemory(&ejectData, sizeof(EJECTDATA));

	// Copy the handle of the injected module
	ejectData.hInstance = hModule;

	// Get the address of FreeLibrary
	if(!(ejectData.lpfnFreeLibrary = (PFNFreeLibrary)GetProcAddress(GetModuleHandle(KERNEL32_DLL), "FreeLibrary")))
	{
		
		return FALSE;
	}

		// Execute
	if(!RemoteExecute(hProcess, &ejectData, sizeof(EJECTDATA), (LPTHREAD_START_ROUTINE)RemoteEjectProc, cbRemoteProc, TRUE, &dwExitCode))
	{
		return FALSE;
	}*/

	// if ( dwExitCode != ...) return FALSE;

/*F+F+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  Function: RemoteExecute

  Summary:  See dcinjapi.h
-----------------------------------------------------------------F-F*/
DCAPI BOOL DCCALL RemoteExecute(HANDLE hProcess, 
								LPVOID lpArgument, 
								SIZE_T cbArgument,
								LPTHREAD_START_ROUTINE lpCode,
								SIZE_T cbCode,
								DWORD dwFlags,
								LPDWORD lpdwExitCode )
{
	LPVOID	lpRemoteData, 
			lpRemoteCode;
	SIZE_T cbBytesRead;
	HANDLE hThread;
	DWORD dwThreadId;
	BOOL bResult;

	lpRemoteData = NULL;
	lpRemoteCode = NULL;
	hThread = NULL;
	bResult = FALSE;
	
	// Check the arguments
	if(lpCode == NULL || cbCode == 0) 
		return FALSE;

	// Check the arguments
	if(lpArgument != NULL && cbArgument != 0)
	{
		// Write the argument to the remote process 
		if(NULL == (lpRemoteData = RemoteWrite(hProcess, lpArgument, cbArgument)))
		{
			return FALSE;
		}
	}
	else if(cbArgument != 0)
	{
		return FALSE;
	}

	// Write the code to the remote process
	if(NULL == (lpRemoteCode = RemoteWrite(hProcess, lpCode, cbCode)))
	{
		goto cleanup;
	}

	// Now spawn the thread
	if(NULL == (hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)lpRemoteCode, lpRemoteData, 0, &dwThreadId)))
	{
		goto cleanup;
	}

	// Wait for the remote thread,...
	if(WaitForSingleObject(hThread, INFINITE) != WAIT_OBJECT_0)
	{
		goto cleanup;
	}

	// Get the exit code and read the result
	if(!GetExitCodeThread(hThread, lpdwExitCode)
	|| *lpdwExitCode == 0)
	{
		goto cleanup;
	}
	
	// If an argument was passed, read it back from remote into this process
	if(lpArgument && (RE_READ_ARGUMENT_AFTER & dwFlags) == RE_READ_ARGUMENT_AFTER)
	{
		if (!ReadProcessMemory(hProcess, lpRemoteData, lpArgument, cbArgument, &cbBytesRead)
		|| cbBytesRead != cbArgument)
		{
			goto cleanup;
		}
	}

	bResult = TRUE;

cleanup:

	if( lpRemoteData != NULL ) VirtualFreeEx(hProcess, lpRemoteData, 0, MEM_RELEASE);
	if( lpRemoteCode != NULL ) VirtualFreeEx(hProcess, lpRemoteCode, 0, MEM_RELEASE);
	if( hThread != NULL ) CloseHandle(hThread);

	return bResult;
}

/*F+F+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  Function: RemoteInjectA

  Summary:  See dcinjapi.h
-----------------------------------------------------------------F-F*/
DCAPI HMODULE DCCALL RemoteInjectA (HANDLE hProcess, 
									LPCSTR lpszDllName )
{
	HMODULE hModResult;
	WCHAR * pwszDllName;
	int ccLength, ccConverted;

	hModResult = NULL;

	// Checking the passed parameters,...
	if( lpszDllName == NULL
	||  !(ccLength = lstrlenA(lpszDllName))
	){
		return NULL;
	}
		
	// get required buffer size to convert to wide string
	if( 0 == ( ccConverted = MultiByteToWideChar(CP_ACP, MB_ERR_INVALID_CHARS, lpszDllName, ++ccLength, NULL, 0) ) )
	{
		return NULL;
	}
	
	// allocate buffer to convert to wide string
	if( NULL == ( pwszDllName = (WCHAR*)AllocMemory(ccConverted * sizeof(WCHAR)) ) )
	{
		return NULL;
	}

	// convert to wide string
	if( ccConverted == MultiByteToWideChar(CP_ACP, 0, lpszDllName, ccLength, pwszDllName, ccConverted) )
	{
		// forward the call to the wide function (paths can contain unicode chars)
		hModResult = RemoteInjectW(hProcess, pwszDllName);
	}

	FreeMemory(pwszDllName);

	return hModResult;
}

/*F+F+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  Function: RemoteInjectW

  Summary:  See dcinjapi.h
-----------------------------------------------------------------F-F*/
DCAPI HMODULE DCCALL RemoteInjectW ( HANDLE hProcess, 
									LPCWSTR lpszDllName	)
{
	SIZE_T cbRemoteProc;
	INJECTDATA injectData;
	size_t ccDllName;
	DWORD dwExitCode;

	// Checking the passed parameters,...
	if(hProcess == NULL 
	|| lpszDllName == NULL
	|| 0 == (ccDllName = lstrlenW(lpszDllName)) 
	|| ccDllName >= MAX_PATH
	){
		return NULL;
	}

	// Calculate length of code to inject
	if(0 == (cbRemoteProc = GetProcLength((PBYTE)RemoteInjectProc)))
	{
		return NULL;
	}

	// Zero  memory for the data to pass
	RtlZeroMemory(&injectData, sizeof(INJECTDATA));

	// Copy the name of the dll to load 
	lstrcpyW(injectData.szDllName, lpszDllName);
	
	// Get the address of LoadLibraryW
	if(NULL == (injectData.lpfnLoadLibrary = (PFNLoadLibraryW)GetProcAddress(GetModuleHandle(KERNEL32_DLL), "LoadLibraryW")))
		return NULL;

	// Execute
	if(RemoteExecute(hProcess, &injectData, sizeof(INJECTDATA), (LPTHREAD_START_ROUTINE)RemoteInjectProc, cbRemoteProc, RE_READ_ARGUMENT_AFTER, &dwExitCode)
	&& dwExitCode != 0
	){
		return injectData.hModResult;
	}
	else
	{
		return NULL;
	}
}

/*F+F+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  Function: RemoteWrite

  Summary:  See dcinjapi.h
-----------------------------------------------------------------F-F*/
DCAPI LPVOID DCCALL RemoteWrite(HANDLE hProcess, 
								LPCVOID lpData, 
								SIZE_T cbData )
{
	SIZE_T dwWritten;
	LPVOID	lpRemoteData;

	// Allocate memory in the remote process	
	if( NULL == ( lpRemoteData = VirtualAllocEx(hProcess, NULL, cbData, MEM_COMMIT, PAGE_EXECUTE_READWRITE) ) )
	{
		return NULL;
	}
	
	// Write date to memory in remote process
	dwWritten = 0;
	if(!WriteProcessMemory(hProcess, lpRemoteData, lpData, cbData, &dwWritten) 
	&& dwWritten >= cbData 
	){
		VirtualFreeEx(hProcess, lpRemoteData, 0, MEM_RELEASE);	
		return NULL;
	}

	return lpRemoteData;
}

/*F+F+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  Function: WStringToAString

  Summary:  See injector.h
-----------------------------------------------------------------F-F*/
CHAR * WStringToAString(LPCWSTR lpszSource)
{
	CHAR * lpszTarget;
	int ccLength, ccConverted; 

	// check arguments
	if(lpszSource == NULL 
	|| 0 == ( ccLength = lstrlenW(lpszSource) ) 
	){
		return NULL;
	}

	// get required buffer size to convert to ansi
	if( 0 == ( ccConverted = WideCharToMultiByte(CP_ACP, 0, lpszSource, ccLength, NULL, 0, NULL, NULL) ) )
	{
		return NULL;
	}

	// allocate memory for ansi string
	if( NULL == ( lpszTarget = (CHAR*)AllocMemory(ccConverted * sizeof(WCHAR)) ) )
	{
		return NULL;
	}

	// convert to ansi
	if( ccConverted != WideCharToMultiByte(CP_ACP, 0, lpszSource, ccLength, lpszTarget, ccConverted, NULL, NULL) )
	{
		FreeMemory(lpszTarget);
		return NULL;
	}

	return lpszTarget;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#ifdef __cplusplus
} // extern "C"
#endif

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
