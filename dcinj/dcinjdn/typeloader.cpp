
#include "dcinjdn.h"

using namespace DCInjDn;

TypeLoader::TypeLoader(void) { }
TypeLoader::~TypeLoader(void) { }

bool TypeLoader::Command(String^ typeName, array<unsigned char>^ buffer)
{
	try
	{
		if(m_objects->ContainsKey(typeName))
		{
			m_objects->default::get(typeName)->OnCommand(buffer);
			return true;
		}
	}
	catch(Exception^ex)
	{
		Debug::WriteLine(ex->ToString());
	}
	return false;
}

bool TypeLoader::Load(String^ assemblyFile, String^ typeName)
{
	try
	{
		if(!m_objects->ContainsKey(typeName))
		{
			ObjectHandle^ hObj = Activator::CreateInstanceFrom(assemblyFile, typeName);
			IInjectable^ ii = safe_cast<IInjectable^>(hObj->Unwrap());
			m_objects->Add(typeName, ii);
			ii->OnLoad();
			return true;
		}
	}
	catch(Exception^ex)
	{
		Debug::WriteLine(ex->ToString());
	}
	return false;
}

bool TypeLoader::Unload(String^ typeName)
{
	try
	{
		if(m_objects->ContainsKey(typeName))
		{
			m_objects->default::get(typeName)->OnUnload();
			m_objects->Remove(typeName);
			GC::Collect();
			return true;
		}
	}
	catch(Exception^ex)
	{
		Debug::WriteLine(ex->ToString());
	}
	return false;
}

Int32 TypeLoader::Count()
{
	return m_objects->Count;
}

String^ TypeLoader::Item(Int32 index)
{	
	Int32 count = 0;
	for each(String^ typeName in m_objects->Keys)
	{
		if(count++ == index) return typeName;
	}
	return nullptr;
}