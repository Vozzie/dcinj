#pragma once

namespace DCInjDn
{
    /// <summary>
    /// Provides methods to load, unload types and call methods in another process.
    /// </summary>
	public ref class Injector sealed
	{
	private:
		
		HANDLE m_hProcess;
		HMODULE m_hModule;
		bool m_opened;
		Injector(HANDLE hProcess, HMODULE hModule, bool opened);

	public:
				
        /// <summary>
        /// Injects the loader into a process.
        /// </summary>
        /// <param name="process">Handle to the process to open. If open is true, 
		/// the process will be opened with PROCESS_CREATE_THREAD, PROCESS_QUERY_INFORMATION 
		/// PROCESS_VM_OPERATION, PROCESS_VM_READ, PROCESS_VM_WRITE access rights. When open
		/// is false the process should have these rights.
		/// </param>
        /// <param name="open">When true the method will open the process with the correct 
		///  rights, otherwise it will use the process handle.
		/// </param>
        /// <returns></returns>
		static Injector^ Create(IntPtr process, bool open)
		{
			wchar_t * pTypeName = NULL;
			HMODULE hModule = NULL;
			HANDLE hProcess = NULL;
			try
			{
				if(open)
				{
					 hProcess = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE, FALSE, GetProcessId((HANDLE)process));
				}
				else
				{
					hProcess = (HANDLE)process;
				}
				if(NULL != hProcess)
				{
					if(NULL != (pTypeName = (wchar_t*)(void*)Marshal::StringToHGlobalUni(Assembly::GetExecutingAssembly()->Location)))
					{
						if(NULL != (hModule = RemoteInjectW((HANDLE)process, pTypeName)))
						{
							return gcnew Injector(hProcess, hModule, open);
						}
					}
					if(open) CloseHandle(hProcess);
				}
			}
			catch(Exception^ex)
			{
				if ( hModule != NULL ) RemoteEject(hProcess, hModule);
				if ( hProcess != NULL && open ) CloseHandle(hProcess);
				Debug::WriteLine(ex->ToString());
			}
			finally
			{
				if(pTypeName != NULL) Marshal::FreeHGlobal(IntPtr((void*)pTypeName));
			}
			return nullptr;
		}
		
        /// <summary>
        /// 
        /// </summary>
        /// <param name="process"></param>
        /// <returns></returns>
		static Injector^ Create(IntPtr process)
		{
			return Create(process, true);
		}
		
        /// <summary>
        /// 
        /// </summary>
        /// <param name="process"></param>
        /// <returns></returns>
		static Injector^ Create(Process^ process) 
		{
			return Create(process->Handle);
		}
				
        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
		void Close();

		/// <summary>
        /// 
        /// </summary>
        /// <param name="typeName"></param>
        /// <param name="buffer"></param>
        /// <returns></returns>
		bool Command(String^ typeName, array<unsigned char>^ buffer);
				
        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
		Int32 Count();
				
        /// <summary>
        /// 
        /// </summary>
        /// <param name="typeName"></param>
        /// <returns></returns>
		bool Eject(String^ typeName);
				
		/// <summary>
        /// 
        /// </summary>
        /// <param name="assemblyFile"></param>
        /// <param name="typeName"></param>
        /// <returns></returns>
		bool Inject(String^ assemblyFile, String^ typeName);
		
		/// <summary>
        /// 
        /// </summary>
        /// <param name="index"></param>
        /// <returns></returns>
		String^ Item(Int32 index);
        
	}; // class
}
