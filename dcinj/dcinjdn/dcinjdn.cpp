
#include "dcinjdn.h"

DWORD WINAPI TypeCommand(PTYPECOMMANDDATA pCommandData){
	try
	{
		array<unsigned char>^ buffer = pCommandData->cbData ? gcnew array<unsigned char>(pCommandData->cbData) : nullptr;
		IntPtr pBuffer = IntPtr((void*)(pCommandData + 1));
		if(pCommandData->cbData) Marshal::Copy(IntPtr(pBuffer), buffer, 0, pCommandData->cbData);
		String^ typeName = gcnew String(pCommandData->szTypeName);
		TypeLoader::Instance->Command(typeName, buffer);
		if(pCommandData->cbData) Marshal::Copy(buffer, 0, pBuffer, buffer->Length);
		return 1;
	}
	catch(Exception^ ex)
	{
		Debug::WriteLine(ex->ToString());
	}
	return 0;
}

DWORD WINAPI TypeCount(void)
{
	try
	{
		return TypeLoader::Instance->Count();
	}
	catch(Exception^ ex)
	{
		Debug::WriteLine(ex->ToString());
	}
	return -1;
}

DWORD WINAPI TypeGetItem(PTYPEGETITEM pGetItem)
{
	try
	{
		String^ typeName = TypeLoader::Instance->Item((Int32)pGetItem->dwIndex);
		if(typeName == nullptr) 
		{
			pGetItem->szTypeName[0] = 0;
			return 0;
		}
		wchar_t * pTypeName = (wchar_t*)(void*)Marshal::StringToHGlobalUni(typeName);
		try{
			wcscpy_s(pGetItem->szTypeName, MAX_PATH, pTypeName);
		}finally{
			Marshal::FreeHGlobal(IntPtr((void*)pTypeName));
		}
		return 1;
	}
	catch(Exception^ ex)
	{
		Debug::WriteLine(ex->ToString());
	}
	return 0;
}

DWORD WINAPI TypeLoad(PTYPEINJECTDATA pInjectData)
{
	try
	{
		String^ assemblyFile = gcnew String(pInjectData->szAssemblyFile);
		String^ typeName = gcnew String(pInjectData->szTypeName);
		return TypeLoader::Instance->Load(assemblyFile, typeName) ? 1 : 0;
	}
	catch(Exception^ ex)
	{
		Debug::WriteLine(ex->ToString());
	}
	return 0;
}

DWORD WINAPI TypeUnload(PTYPEEJECTDATA pEjectData)
{
	try
	{
		String^ typeName = gcnew String(pEjectData->szTypeName);
		return TypeLoader::Instance->Unload(typeName) ? 1 : 0;
	}
	catch(Exception^ ex)
	{
		Debug::WriteLine(ex->ToString());
	}
	return 0;
}
