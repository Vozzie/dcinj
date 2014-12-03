#pragma once


namespace DCInjDn
{
	ref class TypeLoader
	{
	private:
		TypeLoader(void);
		static TypeLoader^ m_instance = gcnew TypeLoader();
		static SortedDictionary<String^, IInjectable^>^ m_objects = gcnew SortedDictionary<String^, IInjectable^>();
	public:
		~TypeLoader(void);
		static property TypeLoader^ Instance
		{ 
			TypeLoader^ get()
			{ 
				return m_instance; 
			} 
		}
		bool Command(String^ typeName, array<unsigned char>^ buffer);
		Int32 Count();
		String^ Item(Int32 index);
		bool Load(String^ assemblyFile, String^ typeName);
		bool Unload(String^ typeName);
	};
} // namespace DCInjDn



