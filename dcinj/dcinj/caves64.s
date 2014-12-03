COMMENT ~=============================================================
  File:      caves64.s

  Summary:   The 64 bit code cave procedures that are injected.

  Functions: RemoteInjectProc, RemoteEjectProc,
             RemoteCallProc, RemoteCallExProc             

  Origin:    This file is part of DC Injector.

----------------------------------------------------------------------
  Info on x64

http://msdn.microsoft.com/en-us/library/7kcdt6fy.aspx

http://www.codejury.com/a-walk-in-x64-land/

http://forum.nasm.us/index.php?topic=1758.0

http://stackoverflow.com/questions/12083810/x64-calling-convention-sta
                                                        ck-and-varargs
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

.Code

COMMENT ~*F+F+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  Function: RemoteCallProc

  Summary:  See injector.h
-----------------------------------------------------------------F-F*~
RemoteCallProc Proc Public
	; Save pointer to structure argument in Rcx in shadow stack space
	Mov [Rsp + 8], Rcx

	; create shadow stack space
	Sub Rsp, 28H
	
	; Rdx must be a string pointer to name of the function
	Mov Rdx, Rcx	

	; Rcx must be a handle of the library with the function
	Mov Rcx, (PCALLDATA[Rdx]).hModLibrary

	; call GetProcAddress to get the address
	; of the function in the library
	Call QWord Ptr (PCALLDATA[Rdx]).lpfnGetProcAddress

	; restore Rcx, a pointer to CALLDATA, from shadow space
	Mov Rcx, [Rsp + 30H]

	; move the result of GetProcAddress to the lpfnProcResult field
	; so the callee has a result whether retrieving the address
	; of the function succeeded or not
	Mov QWord Ptr (PCALLDATA[Rcx]).lpfnProcResult, Rax

	; is the function address GetProcAddress returns NULL?
	Test Rax, Rax
	Jz @F

	; call the function in the library
	Call QWord Ptr Rax
@@:
	; adjust stack
	Add Rsp, 28H

	; return
	Ret

	; Signature used to calculate length 
    DD PROC_SIGNATURE
RemoteCallProc EndP

COMMENT ~*F+F+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  Function: RemoteCallExProc

  See:		injector.h
-----------------------------------------------------------------F-F*~
RemoteCallProcEx Proc Public
	; Save pointer to structure argument in Rcx in shadow stack space
	Mov [Rsp + 8], Rcx

	; Create shadow stack space
	Sub Rsp, 28H

	; Rdx must be a string pointer to name of the function
	Mov Rdx, Rcx
	
	; Rcx must be a handle of the library with the function
	Mov Rcx, (PCALLDATA[Rdx]).hModLibrary
		
	; call GetProcAddress to get the address
	; of the function in the library
	Call QWord Ptr (PCALLDATA[Rdx]).lpfnGetProcAddress
		
	; restore Rcx, a pointer to CALLDATA, from shadow space
	Mov Rcx, [Rsp + 30H]

	; move the result of GetProcAddress to the lpfnProcResult field
	; so the callee has a result whether retrieving the address
	; of the function succeeded or not
	Mov QWord Ptr (PCALLDATA[Rcx]).lpfnProcResult, Rax

	; is the function address GetProcAddress returned NULL?
	Test Rax, Rax
	Jz @F

	; adjust Rcx to point behind CALLDATA to the user data
	Add Rcx, SizeOf CALLDATA

	; call Rax
	Call QWord Ptr Rax
@@:
	; adjust stack
	Add Rsp, 28H

	; return
	Ret

	; Signature used to calculate length 
    DD PROC_SIGNATURE
RemoteCallProcEx EndP

COMMENT ~*F+F+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  Function: RemoteEjectProc

  See:		injector.h
-----------------------------------------------------------------F-F*~
RemoteEjectProc Proc Public
	; Create shadow stack space
	Sub Rsp, 28H
	
	Mov Rax, Rcx
	Mov Rcx, (PEJECTDATA[Rax]).hModLibrary
	Call QWord Ptr (PEJECTDATA[Rax]).lpfnFreeLibrary
	Add Rsp, 28H
	Ret
	; Signature used to calculate length 
    DD PROC_SIGNATURE
RemoteEjectProc EndP

COMMENT ~*F+F+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  Function: RemoteInjectProc

  See:		injector.h
-----------------------------------------------------------------F-F*~
RemoteInjectProc Proc Public
	; Save the pointer to a INJECTDATA structure
	; that is in Rcx into the shadow stack space.
	Mov [Rsp + 8], Rcx

	; Adjust stack.
	Sub Rsp, 28H

	; Call LoadLibrary.
	Call QWord Ptr (PINJECTDATA[Rcx]).lpfnLoadLibrary

	; Restore the pointer to INJECTDATA structure.
	Mov Rcx, [Rsp + 30H]

	; store result of loadlibrary in Rax
	Mov QWord Ptr (PINJECTDATA[Rcx]).hModResult, Rax

	; Maybe this code is unnecessary but if a library is loaded at 
	; an address of which and Eax, the lower DWORD of Rax, is zero
	; it might cause a bug. Could not find info on what is the 
	; maximum base address on 64 bit so doing this check
	; for certainty.
	Test Rax, Rax
	Jz @F
	Mov Eax, 1
@@:

	; Restore stack.
	Add Rsp, 28H

	; Exit.
	Ret

	; Signature used to calculate length.
    DD PROC_SIGNATURE
RemoteInjectProc EndP

;~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

End

;~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
