
;~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

; A Windows constant used for the length of the DLL path.
MAX_PATH Equ 260

; The maximum name of the procedure name.
MAX_PROC_NAME Equ 512

; Signature used by GetProcLength to 
; calculate the length of a procedure
PROC_SIGNATURE Equ 0CCCCCCCCh

;~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

IFDEF RAX

	HMODULE TypeDef QWORD
	FARPROC TypeDef QWORD

ELSE

	HMODULE TypeDef DWORD
	FARPROC TypeDef DWORD

ENDIF

;~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

; Structure that is written to a remote process its memory.
; The structure contains the name of the procedure that needs to be 
; called in the injected dll, the instance handle of the injected dll
; and the address of the GetProcAddress function.

CALLDATA Struct
	szProcName DB MAX_PROC_NAME Dup(?)
	hModLibrary HMODULE ?
	lpfnGetProcAddress FARPROC ?
	lpfnProcResult FARPROC ?
CALLDATA EndS

PCALLDATA TextEqu <CALLDATA Ptr>

;~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

; Structure that is written to a remote process its memory.
; The structure instance handle of the injected dll and also the 
; address of the FreeLibrary function.

EJECTDATA Struct
	hModLibrary HMODULE ?
	lpfnFreeLibrary FARPROC ?
EJECTDATA EndS

PEJECTDATA TextEqu <EJECTDATA Ptr>

;~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

; Structure that is written to a remote process its memory.
; The structure contains the path of the dll to inject and also the 
; address of the LoadLibrary function.

INJECTDATA Struct
	szDllName DW MAX_PATH Dup(?)
	lpfnLoadLibrary FARPROC ?
	hModResult HMODULE ?
INJECTDATA EndS

PINJECTDATA TextEqu <INJECTDATA Ptr>

;~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

RESOLVEDATA Struct
	szProcName DB MAX_PROC_NAME Dup(?)
	hModLibrary HMODULE ?
	lpfnGetProcAddress FARPROC ?
	lpfnProcResult FARPROC ?
RESOLVEDATA EndS

PRESOLVEDATA TextEqu <RESOLVEDATA Ptr>

;~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
