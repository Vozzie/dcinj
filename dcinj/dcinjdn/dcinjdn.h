#pragma once

extern "C"
{
	#include <Windows.h>
	#include "..\dcinj\dcinjapi.h"
}

typedef struct _TYPEINJECTDATA
{
	wchar_t		szAssemblyFile[MAX_PATH];
	wchar_t		szTypeName[MAX_PATH];
}
TYPEINJECTDATA, *PTYPEINJECTDATA;

typedef struct _TYPECOMMANDDATA
{
	wchar_t		szTypeName[MAX_PATH];
	DWORD		cbData;
	//DWORD_PTR	lpNull; // Make sure there is size for a LPVOID pointer on the back
}
TYPECOMMANDDATA, *PTYPECOMMANDDATA;

typedef struct _TYPEEJECTDATA
{
	wchar_t		szTypeName[MAX_PATH];
}
TYPEEJECTDATA, *PTYPEEJECTDATA;

typedef struct _TYPEGETITEM
{
	DWORD		dwIndex;
	wchar_t		szTypeName[MAX_PATH];
}
TYPEGETITEM, *PTYPEGETITEM;

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Diagnostics;
using namespace System::Reflection;
using namespace System::Runtime::CompilerServices;
using namespace System::Runtime::InteropServices;
using namespace System::Runtime::Remoting;
using namespace System::Security::Permissions;
using namespace System::Text;

#if defined(_M_X64)
#define INTPTR(ptr,offset) IntPtr( (&(ptr))->ToInt64() + (Int64)(offset))
#else
#define INTPTR(ptr,offset) IntPtr( (&(ptr))->ToInt32() + (Int32)(offset))
#endif

#include "injector.h"
#include "iinjectable.h"
#include "typeloader.h"

using namespace DCInjDn;

#pragma comment(lib, "user32.lib")

#if defined(_M_X64)

#if defined(DEBUG) || defined(_DEBUG)
#pragma comment(lib, "..\\output\\x64\\Debug_Static\\dcinj.lib")
#else
#pragma comment(lib, "..\\output\\x64\\Release_Static\\dcinj.lib")
#endif

#else // if !defined(_M_X64)

#if defined(DEBUG) || defined(_DEBUG)
#pragma comment(lib, "..\\output\\Win32\\Debug_Static\\dcinj.lib")
#else
#pragma comment(lib, "..\\output\\Win32\\Release_Static\\dcinj.lib")
#endif

#endif // defined(_M_X64)
