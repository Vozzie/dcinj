/*+===================================================================
  File:      injector.h

  Summary:   This file contains the type definitions and prototypes 
			 of private and assembly routines for the DC Injector DLL.

  Functions: GetProcLength, WStringToAString,
			 RemoteInjectProc, RemoteEjectProc,
			 RemoteCallProc, RemoteCallExProc.

  Types:     PFNLoadLibrary, PFNFreeLibrary, PFNGetProcAddress,
			 CALLDATA, EJECTDATA, INJECTDATA.

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
#pragma once

#include <Windows.h>

 
#ifdef DCINJ_EXPORTS
	#define DCAPI __declspec(dllexport)
#endif

#include "dcinjapi.h"

// Max length for procedure name (should be way more)
#define MAX_PROC_NAME 512
// Signature that's in the assembled code's epilogue
#define PROC_SIGNATURE 0xCCCCCCCC
// Kernel32 string used here & there
#define KERNEL32_DLL TEXT("kernel32.dll")

/*~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-*/

// types, source "injector.c"

// Function pointer for FreeLibrary
typedef BOOL (WINAPI * PFNFreeLibrary)(HMODULE);
// Function pointer for GetProcAddress
typedef FARPROC (WINAPI * PFNGetProcAddress)(HMODULE, LPCSTR);
// Function pointer for LoadLibraryW
typedef HMODULE (WINAPI * PFNLoadLibraryW)(LPWSTR);

#include <PshPack1.h>

typedef struct 
{
	// IN: Name of procedure to call.
	CHAR szProcName[MAX_PROC_NAME];
	// IN: Handle to the library that was loaded in the process.
	HMODULE hModLibrary ;
	// IN: Address of the GetProcAddress procedure.
	PFNGetProcAddress lpfnGetProcAddress;
	// OUT: The address of the called procedure on success, 
	//		NULL on failure.
	FARPROC lpfnProcResult;
} 
CALLDATA , *PCALLDATA ;

typedef struct 
{
	// IN: Handle to the library that was loaded in the process.
	HMODULE hModLibrary;
	// IN: Address of the FreeLibrary procedure.
	PFNFreeLibrary lpfnFreeLibrary;
}
EJECTDATA, *PEJECTDATA ;

typedef struct 
{
	// IN: Path of the library to inject.
	WCHAR szDllName[MAX_PATH];
	// IN: Address of the LoadLibraryW procedure.
	PFNLoadLibraryW lpfnLoadLibrary;
	// OUT: Handle of loaded library, NULL on failure.
	HMODULE hModResult;
}
INJECTDATA, *PINJECTDATA ;

#include <PopPack.h>

#ifdef __cplusplus
extern "C" {
#endif

/*~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-*/

// private functions, source "injector.c"

/*F+F+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  Function: GetProcLength

  Summary:  Calculate the length of the code to be injected.

  Args:     PBYTE pbProcedure
              The address of the code to be injected.

  Returns:  SIZE_T
              The length of the code on success, zero on failure.
-----------------------------------------------------------------F-F*/
SIZE_T GetProcLength(PBYTE pbProcedure);

/*F+F+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  Function: WStringToAString

  Summary:  Converts an ANSI string to a UNICODE string..

  Args:     LPCWSTR lpszSource
              The string to be converted.

  Returns:  CHAR *
              A pointer to an UNICODE string on success, 
			  NULL on failure.
-----------------------------------------------------------------F-F*/
CHAR * WStringToAString(LPCWSTR lpszSource);

/*~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-*/

// injected procedures, sources "caves32.s" and "caves64.s"

/*F+F+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  Function: RemoteCallProc

  Summary:  Procedure that is written to and executed in a remote 
			process. On execution the function first tries to get the
			address by calling GetProcAddress. On GetProcAddress 
			success	the procedure will set the lpfnProcResult field of
			the CALLDATA structure and then call the procedure. On 
			GetProcAddress failure the lpfnProcResult field will be 
			set	to NULL and the procedure will return zero. On success
			the lpfnProcResult will be set to the address of the 
			called function and the procedure will return the return 
			value of the called function.

  Args:     PCALLDATA
              szProcName must contain the name of the function
			  that needs to be called and hModule the handle of the
			  library in wich to call the function. lpfnGetProcAddress
			  should be the address of GetProcAddress. On success
			  the lpfnProcResult field will be set to the address of
			  the function in the library, on failure lpfnProcResult
			  will be NULL.

  Returns:  DWORD
              Returns the value of the called function.
-----------------------------------------------------------------F-F*/
DWORD 
WINAPI 
RemoteCallProc (
	PCALLDATA
	);

/*F+F+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  Function: RemoteCallExProc

  Summary:  Procedure that is written to and executed in a remote 
			process. On execution the function first tries to get the
			address by calling GetProcAddress. On GetProcAddress 
			success	the procedure will set the lpfnProcResult field of
			the CALLDATA structure and then call the procedure passing
			a pointer to data written after the CALLDATA structure. On
			GetProcAddress failure the lpfnProcResult field will be 
			set	to NULL and the procedure will return zero. On success
			the lpfnProcResult will be set to the address of the 
			called function and the procedure will return the return 
			value of the called function. 

  Args:     PCALLDATA
              szProcName must contain the name of the function
			  that needs to be called and hModule the handle of the
			  library in wich to call the function. A pointer to the
			  data after CALLDATA is passed as argument to the called
			  function. lpfnGetProcAddress should be the address of
			  GetProcAddress. On success the lpfnProcResult field will be 
			  set to the address of the function in the library, 
			  on failure the lpfnProcResult will be NULL. 

  Returns:  DWORD
              Returns the value of the called function.
-----------------------------------------------------------------F-F*/
DWORD
WINAPI
RemoteCallProcEx (
	PCALLDATA
	);

/*F+F+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  Function: RemoteEjectProc

 Obsolete:	There is no need to use a code cave to call
			FreeLibrary in the remote process. See "RemoteEject".

 Summary:	Procedure that is written to and executed in a remote 
			process. On execution it frees a library by calling 
			FreeLibrary in the remote process.

  Args:     PEJECTDATA
              Pointer to a EJECTDATA structure that contains the
			  required data to free the library.

  Returns:  BOOL
              TRUE on success, FALSE on failure.
-----------------------------------------------------------------F-F*/
DWORD
WINAPI
RemoteEjectProc (
	PEJECTDATA
	);

/*F+F+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  Function: RemoteInjectProc

  Summary:  Procedure that is written to and executed in a specified
			process. On execution the procedure loads a library by 
			calling LoadLibraryW in the remote process. The procedure
			then copies the handle of the loaded library into the 
			hModResult field of the INJECTDATA structure. 
			Note: LoadLibraryW is used instead of LoadLibraryA 
			because a path could contain UNICODE characters.

  Args:     PINJECTDATA
              Pointer to a INJECTDATA structure that contains the
			  required data to load the library. After loading the 
			  handle is copied into the hModResult field of the
			  INJECTDATA structure.

  Returns:  BOOL
              FALSE on failure, TRUE on success.

  Remarks:  To load a library in a remote process one can just 
			write the library name into	the remote process and then
			call CreateRemoteThread passing the address of 
			LoadLibrary as thread procedure	and the library name 
			string as argument. This "doesn't work" on 64 bit if the
			module handle is needed as result.
			If one would use GetThreadExitCode to get this value,
			GetThreadExitCode returns a DWORD and library handle is
			a HMODULE. 
-----------------------------------------------------------------F-F*/
DWORD
WINAPI
RemoteInjectProc ( 
	PINJECTDATA
	);

/*~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-*/

#ifdef __cplusplus
} // extern "C"
#endif

/*~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-*/

// Memory allocation macros

#define AllocMemory(size_in_bytes) \
	HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, (size_in_bytes))

#define FreeMemory(memory) \
	HeapFree(GetProcessHeap(), 0, (memory))

/*~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-*/

