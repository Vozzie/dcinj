
#include "dcinjdn.h"

Injector::Injector(HANDLE hProcess, HMODULE hModule, bool opened)
{
	this->m_hProcess = hProcess;
	this->m_hModule = hModule;
	this->m_opened = opened;
}

void Injector::Close()
{
	RemoteEject(this->m_hProcess, this->m_hModule);
	if(this->m_opened) CloseHandle(this->m_hProcess);
}

bool Injector::Command(String^ typeName, array<unsigned char>^ buffer)
{
	// Allocate memory for the TYPECOMMANDDATA structure containing the 
	// typename and for the length of the liquidcommand buffer
	Int32 cbBuffer = (buffer == nullptr ? 0 : buffer->Length);
	Int32 cbData = sizeof(TYPECOMMANDDATA) + cbBuffer;
	IntPtr lpData = Marshal::AllocHGlobal(cbData);
	PTYPECOMMANDDATA pCmdData = (PTYPECOMMANDDATA)(void*)lpData;
	try
	{
		// Copy the typename to the structure
		array<unsigned char>^ bTypeName = Encoding::Unicode->GetBytes(typeName + "\0");
		Marshal::Copy(bTypeName, 0, lpData, bTypeName->Length);
		// Write the size of the buffer to the structure
		Marshal::WriteInt32(lpData, (Int32)((char*)&pCmdData->cbData - (char*)(void*)pCmdData), cbBuffer);
		// Copy the liquidCommand->buffer behind the structure
		if(cbBuffer > 0) Marshal::Copy(buffer, 0, INTPTR(lpData, sizeof(TYPECOMMANDDATA)), buffer->Length);
		// Call the injected TypeCommand procedure in the remote process
		DWORD dwRetVal;
		bool result =  (RemoteCallExA(this->m_hProcess, this->m_hModule, "TypeCommand", (void*)lpData, cbData, &dwRetVal) && dwRetVal != 0)
			? true 
			: false;
		if(result && cbBuffer > 0) 
		{
			Marshal::Copy(INTPTR(lpData, sizeof(TYPECOMMANDDATA)), buffer, 0, buffer->Length);
		}
		return result;
	}
	finally
	{
		Marshal::FreeHGlobal(lpData);		
	}
	return false;
}
		
int Injector::Count()
{
	DWORD dwRetVal;
	if((RemoteCallA(this->m_hProcess, this->m_hModule, "TypeCount",  &dwRetVal)))
		return (int)dwRetVal;
	return -1;
}

bool Injector::Eject(String^ typeName)
{
	IntPtr lpData = Marshal::AllocHGlobal(sizeof(TYPEEJECTDATA));
	try
	{
		// Copy the typename to the structure
		array<unsigned char>^ bTypeName = Encoding::Unicode->GetBytes(typeName + "\0");
		Marshal::Copy(bTypeName, 0, lpData, bTypeName->Length);
		DWORD dwRetVal;
		return (RemoteCallExA(this->m_hProcess, this->m_hModule, "TypeUnload", (void*)lpData, sizeof(TYPEEJECTDATA), &dwRetVal) && dwRetVal != 0)
			? true 
			: false;
	}
	finally
	{
		Marshal::FreeHGlobal(lpData);
	}
	return false;
}

bool Injector::Inject(String^ assemblyFile, String^ typeName)
{ 
	array<unsigned char>^ bAssemblyFile = Encoding::Unicode->GetBytes(assemblyFile + "\0");
	array<unsigned char>^ bTypeName = Encoding::Unicode->GetBytes(typeName + "\0");
	// Allocate memory for the TYPEINJECTDATA structure containing the 
	// typename and for the length of the liquidcommand buffer
	Int32 cbData = sizeof(TYPEINJECTDATA);
	IntPtr lpData = Marshal::AllocHGlobal(cbData);
	try
	{
		// Copy the filename to the structure
		Marshal::Copy(bAssemblyFile, 0, lpData, bAssemblyFile->Length);
		// Copy the typename to the structure
		Marshal::Copy(bTypeName, 0, INTPTR(lpData, (((char*)((PTYPEINJECTDATA)(void*)lpData)->szTypeName - (char*)((PTYPEINJECTDATA)(void*)lpData)))), bTypeName->Length);
		// Load the type in the process
		DWORD dwResult;
		return (RemoteCallExA(this->m_hProcess, this->m_hModule, "TypeLoad", (void*)lpData, cbData, &dwResult) && dwResult != 0)
			? true 
			: false;
	}
	finally
	{
		Marshal::FreeHGlobal(lpData);
	}
}

String^ Injector::Item(Int32 index)
{
	IntPtr lpData = Marshal::AllocHGlobal(sizeof(TYPEGETITEM));
	try
	{
		((PTYPEGETITEM)(void*)lpData)->dwIndex = index;
		DWORD dwRetVal;
		if( (RemoteCallExA(this->m_hProcess, this->m_hModule, "TypeGetItem", (void*)lpData, sizeof(TYPEGETITEM), &dwRetVal) && dwRetVal != 0))
		{
			PTYPEGETITEM pData = (PTYPEGETITEM)(void*)lpData;
			return gcnew String(((PTYPEGETITEM)(void*)lpData)->szTypeName);
		}
	}
	finally
	{
		Marshal::FreeHGlobal(lpData);
	}
	return nullptr;
}

