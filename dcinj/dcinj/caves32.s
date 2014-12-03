COMMENT ~=============================================================
  File:      caves32.s

  Summary:   The 32 bit code cave procedures that are injected.

  Functions: RemoteInjectProc, RemoteEjectProc,
             RemoteCallProc, RemoteCallExProc             

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
=====================================================================~

include masmdef.inc

;~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

; Processor, model and calling convention
.386
.Model Flat, C

;~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

.Code

;~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

; Disable prologue and epilogue
Option Prologue:None
Option Epilogue:None

COMMENT ~*F+F+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  Function: RemoteCallProc

  Summary:  See injector.h
-----------------------------------------------------------------F-F*~
RemoteCallProc@4 Proc Public ; pCallData:PCALLDATA

	; Pop the return address from the stack
	Pop Eax

	; Pop the structure address from the stack
	Pop Ecx

	; Push the return address back on the stack
	Push Eax

	; The struct address is also the address of the procedure name, 
	; push it as argument for GetProcAddress
	Push Ecx

	; Push the handle of the loaded library
	Push (PCALLDATA[Ecx]).hModLibrary

	; Call GetProcAddress
	Call (PCALLDATA[Ecx]).lpfnGetProcAddress

	Mov (PCALLDATA[Ebx]).lpfnProcResult , Eax

	; Exit if zero
	Or Eax, Eax
	Jz @F

	; Forward the call (not a call but jmp) and let the callee
	; return to the caller
	Jmp Eax

	; Exit (stack argument is removed and return address is on top)
@@:
	Ret
	
	; Signature used to calculate length 
    DD PROC_SIGNATURE

RemoteCallProc@4 EndP

COMMENT ~*F+F+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  Function: RemoteCallExProc

  See:		injector.h
-----------------------------------------------------------------F-F*~
RemoteCallProcEx@4 Proc Public ; pCallData:PCALLDATA
	; Preserve ebx
	Push Ebx

	; Move a pointer to the EJECTDATA structure to Ebx
	Mov Ebx, [Esp + 8]

	; The struct address is also the address of the procedure name, 
	; push it as argument for GetProcAddress
	Push Ebx

	; Push the handle of the loaded library
	Push (PCALLDATA[Ebx]).hModLibrary

	; Call GetProcAddress
	Call (PCALLDATA[Ebx]).lpfnGetProcAddress

	Mov (PCALLDATA[Ebx]).lpfnProcResult , Eax

	; Did we get an address?
	Or Eax, Eax
	Jz @F

	; Calculate address of the argument and push it on the stack
	Add Ebx, SizeOf CALLDATA
	Push Ebx

	; Call the procedure
	Call Eax

@@:
	; Restore Ebx
	Pop Ebx

	; Restore stack and return
	Ret 4

	; Signature used to calculate length 
    DD PROC_SIGNATURE
RemoteCallProcEx@4 EndP

COMMENT ~*F+F+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  Function: RemoteEjectProc

  Summary:  See injector.h
-----------------------------------------------------------------F-F*~
RemoteEjectProc@4 Proc Public ; pEjectData:PEJECTDATA
	; Move the address of the structore to Eax
	Mov Ecx, [Esp + 4]

	; Push the handle of the injected module on the stack
	Push (PEJECTDATA[Ecx]).hModLibrary

	; Call FreeLibrary
	Call (PEJECTDATA[Ecx]).lpfnFreeLibrary

	; Restore stack and return
	Ret 4
		
    ; Signature used to calculate length 
    DD PROC_SIGNATURE
RemoteEjectProc@4 EndP


COMMENT ~*F+F+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  Function: RemoteInjectProc

  Summary:  See injector.h
-----------------------------------------------------------------F-F*~
RemoteInjectProc@4 Proc Public ; pInjectData:PINJECTDATA
	; Preserve Ebx on the stack.
	Push Ebx

	; Move the address to the INJECTDATA structure argument to Ebx.
	Mov Ebx, [Esp + 8]

	; The struct address is also the address of the dll path string,
	; push it as argument for LoadLibrary.
	Push Ebx

	; Call LoadLibrary.
	Call (PINJECTDATA[Ebx]).lpfnLoadLibrary
		
	; Move the loaded library handle to Eax.
	Mov (PINJECTDATA[Ebx]).hModResult, Eax

	; Restore Ebx.
	Pop Ebx

	; Remove argument from stack and exit.
	Ret 4

	; Signature used to calculate length.
    DD PROC_SIGNATURE
RemoteInjectProc@4 EndP

;~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

; Restore the Prologue and Epilogue 

Option Prologue:PrologueDef
Option Epilogue:EpilogueDef

End

;~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
