/*+===================================================================
  File:      injector.h

  Summary:   This file contains the prototypes of the exported  
			 routines of the DC Injector DLL.

  Functions: RemoteWrite,	RemoteExecute, 
			 RemoteCallA/W,	RemoteCallExA/W,
			 RemoteInject,	RemoteEject

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

#ifndef DCINJ_EXPORTS 
	#define DCAPI __declspec(dllimport)
#endif

#define DCCALL APIENTRY

#ifdef __cplusplus
extern "C" {
#endif

/*F+F+++F+++F+++F+++F+++F+++F+++F+++F+++F+++F+++F+++F+++F+++F+++F+++F
  Function: RemoteCallA

  Summary:  Calls a function exported from a library loaded in the 
			process.

  Args:     HANDLE hProcess
              A handle to the process in which the function is to be 
			  called. The handle must have the PROCESS_CREATE_THREAD,
			  PROCESS_QUERY_INFORMATION, PROCESS_VM_OPERATION, 
			  PROCESS_VM_WRITE, and PROCESS_VM_READ access rights, 
			  and may fail without these rights on certain platforms.
            HMODULE hModule
              A handle to the loaded library module.
            LPCSTR lpszProcedure
              The function name, or the function's ordinal value. 
			  If this parameter is an ordinal value, it must be
			  in the low-order word; the high-order word must be zero.
            LPDWORD lpdwRetVal
              Pointer to a DWORD value that will receive the return
			  value of the called function.

  Returns:  BOOL
              If the function succeeds, the return value is nonzero.

  Remarks:	Use next signature for the function called with 
			RemoteCallA.

			DWORD WINAPI CALL_ROUTINE(void);
F---F---F---F---F---F---F---F---F---F---F---F---F---F---F---F---F-F*/
DCAPI BOOL
DCCALL
RemoteCallA (
	__in HANDLE hProcess,
	__in HMODULE hModule,
	__in LPCSTR lpszProcedure,
	__out LPDWORD lpdwRetVal
	);

/*F+F+++F+++F+++F+++F+++F+++F+++F+++F+++F+++F+++F+++F+++F+++F+++F+++F
  Function: RemoteCallW

  Summary:  Calls a function exported from a library loaded in the 
			process.

  Args:     HANDLE hProcess
              A handle to the process in which the function is to be 
			  called. The handle must have the PROCESS_CREATE_THREAD,
			  PROCESS_QUERY_INFORMATION, PROCESS_VM_OPERATION, 
			  PROCESS_VM_WRITE, and PROCESS_VM_READ access rights, 
			  and may fail without these rights on certain platforms.
            HMODULE hModule
              A handle to the loaded library module.
            LPCSTR lpszProcedure
              The function name, or the function's ordinal value. 
			  If this parameter is an ordinal value, it must be
			  in the low-order word; the high-order word must be zero.
            LPDWORD lpdwRetVal
              Pointer to a DWORD value that will receive the return
			  value of the called function.

  Returns:  BOOL
              If the function succeeds, the return value is nonzero.

  Remarks:	Use next signature for the function called with 
			RemoteCallW.

			DWORD WINAPI CALL_ROUTINE(void);
F---F---F---F---F---F---F---F---F---F---F---F---F---F---F---F---F-F*/
DCAPI BOOL
DCCALL
RemoteCallW (
	__in HANDLE hProcess,
	__in HMODULE hModule,
	__in LPCWSTR lpszProcedure,
	__out LPDWORD lpdwRetVal
	);

#ifdef UNICODE
	#define RemoteCall RemoteCallW
#else
	#define RemoteCall RemoteCallA
#endif

/*F+F+++F+++F+++F+++F+++F+++F+++F+++F+++F+++F+++F+++F+++F+++F+++F+++F
  Function: RemoteCallExA

  Summary:  Calls a function exported from a library loaded in the 
			process.

  Args:     HANDLE hProcess
              A handle to the process in which the function is to be 
			  called. The handle must have the PROCESS_CREATE_THREAD,
			  PROCESS_QUERY_INFORMATION, PROCESS_VM_OPERATION, 
			  PROCESS_VM_WRITE, and PROCESS_VM_READ access rights, 
			  and may fail without these rights on certain platforms.
            HMODULE hModule
              A handle to the loaded library module.
            LPCSTR lpszProcedure
              The function name, or the function's ordinal value. 
			  If this parameter is an ordinal value, it must be
			  in the low-order word; the high-order word must be zero.
            LPVOID lpArgument
              Pointer to a buffer that will be copied to the address
			  space of the process and be passed as a pointer to the 
			  function as an argument.
            SIZE_T cbArgument
              The size of the argument buffer to be copied.
            LPDWORD lpdwRetVal
              Pointer to a DWORD value that will receive the return
			  value of the called function.

  Returns:  BOOL
              If the function succeeds, the return value is nonzero.

  Remarks:	Use next signature for the function called with 
			RemoteCallExA.

			DWORD WINAPI CALL_ROUTINE(LPVOID lpArgument);
F---F---F---F---F---F---F---F---F---F---F---F---F---F---F---F---F-F*/
DCAPI BOOL
DCCALL
RemoteCallExA (
	__in HANDLE hProcess,
	__in HMODULE hModule,
	__in LPCSTR lpszProcedure,
	__inout LPVOID lpArgument, 
	__in SIZE_T cbArgument,
	__out LPDWORD lpdwRetVal
	);

/*F+F+++F+++F+++F+++F+++F+++F+++F+++F+++F+++F+++F+++F+++F+++F+++F+++F
  Function: RemoteCallExW

  Summary:  Calls a function exported from a library loaded in the 
			process.

  Args:     HANDLE hProcess
              A handle to the process in which the function is to be 
			  called. The handle must have the PROCESS_CREATE_THREAD,
			  PROCESS_QUERY_INFORMATION, PROCESS_VM_OPERATION, 
			  PROCESS_VM_WRITE, and PROCESS_VM_READ access rights, 
			  and may fail without these rights on certain platforms.
            HMODULE hModule
              A handle to the loaded library module.
            LPCSTR lpszProcedure
              The function name, or the function's ordinal value. 
			  If this parameter is an ordinal value, it must be
			  in the low-order word; the high-order word must be zero.
            LPVOID lpArgument
              Pointer to a buffer that will be copied to the address
			  space of the process and be passed as a pointer to the 
			  function as an argument.
            SIZE_T cbArgument
              The size of the argument buffer to be copied.
            LPDWORD lpdwRetVal
              Pointer to a DWORD value that will receive the return
			  value of the called function.

  Returns:  BOOL
              If the function succeeds, the return value is nonzero.

  Remarks:	Use next signature for the function called with 
			RemoteCallExW.

			DWORD WINAPI CALL_ROUTINE(LPVOID lpArgument);
F---F---F---F---F---F---F---F---F---F---F---F---F---F---F---F---F-F*/
DCAPI BOOL
DCCALL
RemoteCallExW (
	__in HANDLE hProcess,
	__in HMODULE hModule,
	__in LPCWSTR lpszProcedure,
	__inout LPVOID lpArgument, 
	__in SIZE_T cbArgument,
	__out LPDWORD lpdwRetVal
	);

#ifdef UNICODE
	#define RemoteCallEx RemoteCallExW
#else
	#define RemoteCallEx RemoteCallExA
#endif

/*F+F+++F+++F+++F+++F+++F+++F+++F+++F+++F+++F+++F+++F+++F+++F+++F+++F
  Function: RemoteEject

  Summary:  Frees the library by calling FreeLibrary in the process.

  Args:     HANDLE hProcess
              A handle to the process in which the library is to be 
			  freed. The handle must have the PROCESS_CREATE_THREAD,
			  PROCESS_QUERY_INFORMATION, PROCESS_VM_OPERATION, 
			  PROCESS_VM_WRITE, and PROCESS_VM_READ access rights, 
			  and may fail without these rights on certain platforms.
            HMODULE hModule
              A handle to the loaded library module.

  Returns:  BOOL
              If the function succeeds, the return value is nonzero.
F---F---F---F---F---F---F---F---F---F---F---F---F---F---F---F---F-F*/
DCAPI BOOL 
DCCALL 
RemoteEject(
	__in HANDLE hProcess,
	__in HMODULE hModule 
	);	

/*F+F+++F+++F+++F+++F+++F+++F+++F+++F+++F+++F+++F+++F+++F+++F+++F+++F
  Function: RemoteExecute

  Summary:  Creates a thread in the address space of a process passing
			a pointer to a data buffer as argument. The function then 
			waits for the thread to finish and gets the exit code 
			after.

  Args:     HANDLE hProcess
              A handle to the process in which the thread is to be 
			  created. The handle must have the PROCESS_CREATE_THREAD,
			  PROCESS_QUERY_INFORMATION, PROCESS_VM_OPERATION, 
			  PROCESS_VM_WRITE, and PROCESS_VM_READ access rights, 
			  and may fail without these rights on certain platforms.
            LPVOID lpArgument
              Pointer to a buffer that will be copied to the address
			  space of the process and be passed to the thread 
			  function as an argument.
            SIZE_T cbArgument
              The size of the argument buffer to be copied.
            LPTHREAD_START_ROUTINE lpCode
              Pointer to code with a LPTHREAD_START_ROUTINE signature
			  that will be copied to the address space of the process
			  and executed.
            SIZE_T cbCode
              Size of the code to be copied.
            DWORD dwFlags
              See RE_* flags.
            LPDWORD lpdwExitCode
              A pointer to a variable to receive the thread 
			  termination status. For more information, see the
			  Remarks section of GetExitCodeThread.

  Returns:  BOOL
              TRUE on success, FALSE on failure.
F---F---F---F---F---F---F---F---F---F---F---F---F---F---F---F---F-F*/
DCAPI BOOL 
DCCALL 
RemoteExecute(
	__in HANDLE hProcess, 
	__inout_opt LPVOID lpArgument, 
	__in_opt SIZE_T cbArgument,
	__in LPTHREAD_START_ROUTINE lpCode,
	__in SIZE_T cbCode,
	__in DWORD dwFlags,
	__out LPDWORD lpdwExitCode
	);

#define RE_READ_ARGUMENT_AFTER 0x00000001

/*F+F+++F+++F+++F+++F+++F+++F+++F+++F+++F+++F+++F+++F+++F+++F+++F+++F
  Function: RemoteInjectA

  Summary:  Loads a library in the the address space of a process by
			calling LoadLibraryW in the process.

  Args:     HANDLE hProcess
              Handle to the process in which the library should be 
			  loaded. The handle must have PROCESS_CREATE_THREAD,
			  PROCESS_QUERY_INFORMATION, PROCESS_VM_OPERATION,
			  PROCESS_VM_READ and PROCESS_VM_WRITE access rights,
			  and may fail without these rights on certain platforms.
            LPCSTR lpszDllName
              Pointer to a string buffer that contains the path
			  of the library.

  Returns:  HMODULE
              A handle to the loaded library on success,
			  NULL on failure.

  Remarks: Because paths and filenames can contain UNIOCDE
			characters LoadLibraryA first converts the library name
			to a UNICODE string and then forwards the call to 
			RemoteInjectW.
F---F---F---F---F---F---F---F---F---F---F---F---F---F---F---F---F-F*/
DCAPI HMODULE
DCCALL 
RemoteInjectA (
	__in HANDLE hProcess, 
	__in LPCSTR lpszDllName
	);

/*F+F+++F+++F+++F+++F+++F+++F+++F+++F+++F+++F+++F+++F+++F+++F+++F+++F
  Function: RemoteInjectW

  Summary:  Loads a library in the the address space of a process by
			calling LoadLibraryW in the process.
			
  Args:     HANDLE hProcess
              Handle to the process in which the library should be 
			  loaded. The handle must have PROCESS_CREATE_THREAD,
			  PROCESS_QUERY_INFORMATION, PROCESS_VM_OPERATION,
			  PROCESS_VM_READ and PROCESS_VM_WRITE access, and may
			  fail without these rights on certain platforms.
            LPCWSTR lpszDllName	
              Pointer to a string buffer that contains the path
			  of the library.
			  
  Returns:  HMODULE
              A handle to the loaded library on success,
			  NULL on failure.
F---F---F---F---F---F---F---F---F---F---F---F---F---F---F---F---F-F*/
DCAPI HMODULE
DCCALL 
RemoteInjectW (
	__in HANDLE hProcess, 
	__in LPCWSTR lpszDllName
	);

#ifdef UNICODE
	#define RemoteInject RemoteInjectW
#else
	#define RemoteInject RemoteInjectA
#endif

/*F+F+++F+++F+++F+++F+++F+++F+++F+++F+++F+++F+++F+++F+++F+++F+++F+++F
  Function: RemoteWrite

  Summary:  Allocates a buffer in the address space of a process 
			and copies data to that buffer. 

  Args:     HANDLE hProcess
              A handle to the process into which to write data. This
			  handle must have PROCESS_VM_WRITE and 
			  PROCESS_VM_OPERATION access, and may fail without these
			  rights on certain platforms.
            LPCVOID lpData
			  A pointer to a buffer that contains data to be written
			  into the process its address space.
            SIZE_T cbData
              The length of the data in bytes.

  Returns:  LPVOID
              NULL on failure, the base address of the data written
			  in the address space of the process. Use VirtualFreeEx
			  with the MEM_RELEASE flag to release this memory.
F---F---F---F---F---F---F---F---F---F---F---F---F---F---F---F---F-F*/
DCAPI LPVOID 
DCCALL 
RemoteWrite(	
	__in HANDLE hProcess, 
	__in LPCVOID lpData, 
	__in SIZE_T cbData 
	);

#ifdef __cplusplus
} // extern "C"
#endif