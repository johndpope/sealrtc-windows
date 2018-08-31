#include "StdAfx.h"
#include "minidump.h"
#include "PeerConnectionHelper.h"
#include "UpStringUtility.h"

const char* INFODUM = "%s-%s-%s-%04d%02d%02d%02d%02d%02d.dmp";

inline BOOL IsDataSectionNeeded(const WCHAR* pModuleName) 
{  
	if(pModuleName == 0)  
	{  
		return FALSE;  
	}  
	WCHAR szFileName[_MAX_FNAME] = L"";  
	_wsplitpath(pModuleName, NULL, NULL, szFileName, NULL);  
	if(wcsicmp(szFileName, L"ntdll") == 0)  
		return TRUE;  
	return FALSE;  
} 

inline BOOL CALLBACK MiniDumpCallback(PVOID   pParam,  const PMINIDUMP_CALLBACK_INPUT   pInput,  PMINIDUMP_CALLBACK_OUTPUT     pOutput)  
{  
	if(pInput == 0 || pOutput == 0)  
		return FALSE;  

	switch(pInput->CallbackType)  
	{  
	case ModuleCallback:  
		{
			if(pOutput->ModuleWriteFlags & ModuleWriteDataSeg)  
				if(!IsDataSectionNeeded(pInput->Module.FullPath))  
					pOutput->ModuleWriteFlags &= (~ModuleWriteDataSeg);  
		}
	case IncludeModuleCallback:  
	case IncludeThreadCallback:  
	case ThreadCallback:  
	case ThreadExCallback:  
		return TRUE;  
	default:
		break;
	}  
	return FALSE;  
}  

inline void CreateMiniDump(PEXCEPTION_POINTERS pep, LPCTSTR strFileName)  
{  
	HANDLE hFile = CreateFile(strFileName, GENERIC_READ | GENERIC_WRITE,  
		FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);  
	if((hFile != NULL) && (hFile != INVALID_HANDLE_VALUE))  
	{  
		MINIDUMP_EXCEPTION_INFORMATION mdei;  
		mdei.ThreadId           = GetCurrentThreadId();  
		mdei.ExceptionPointers  = pep;  
		mdei.ClientPointers     = NULL;  
		MINIDUMP_CALLBACK_INFORMATION mci;  
		mci.CallbackRoutine     = (MINIDUMP_CALLBACK_ROUTINE)MiniDumpCallback;  
		mci.CallbackParam       = 0;  

		::MiniDumpWriteDump(::GetCurrentProcess(), ::GetCurrentProcessId(), hFile, MiniDumpNormal, (pep != 0) ? &mdei : 0, NULL, &mci);  

		CloseHandle(hFile);  
	}  
}  

LONG __stdcall MyUnhandledExceptionFilter(PEXCEPTION_POINTERS pExceptionInfo)
{  
	char szBuffer[1024];
	SYSTEMTIME sys; 
	GetLocalTime( &sys ); 
	
	sprintf(szBuffer, INFODUM, APP_VERSION, const_cast<char*>(PeerConnectionHelper::m_strSDKVersion.c_str()), const_cast<char*>(PeerConnectionHelper::m_strUserId.c_str()), sys.wYear, sys.wMonth, sys.wDay,sys.wHour, sys.wMinute, sys.wSecond);

	std::string dumppath = CPlatform::GetCrashPath() + szBuffer;
	CreateMiniDump(pExceptionInfo,UpStringUtility::StringToWstring(dumppath).c_str());
	return EXCEPTION_EXECUTE_HANDLER;  
}
