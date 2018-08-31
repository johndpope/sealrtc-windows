#include "StdAfx.h"
#include "Platform.h"
#include <shlobj.h>
#include <iphlpapi.h>
#include "UpStringUtility.h"

std::wstring CPlatform::m_AppPath = L"";

#ifdef _DEBUG 
void CPlatform::Print(const wchar_t* format, ...) 
{ 
	va_list args; 
	va_start(args, format); 
	wchar_t buf[256]; 
	_vstprintf_s(buf, format, args);
#ifdef PRINT_WITH_TIME
	::GetLocalTime(&g_st);
	wchar_t time[256]; 
	_stprintf_s(time, _T("[%d:%d:%d %03d] "), g_st.wHour, g_st.wMinute, g_st.wSecond, g_st.wMilliseconds);
	OutputDebugString(time); 
#endif
	OutputDebugString(buf); 
	OutputDebugString(L"\r\n"); 
	va_end(args); 
} 
#else 
void CPlatform::Print(const wchar_t* format, ...) 
{ 
//	va_list args; 
//	va_start(args, format); 
//	wchar_t buf[256]; 
//	_vstprintf_s(buf, format, args);
//#ifdef PRINT_WITH_TIME
//	::GetLocalTime(&g_st);
//	wchar_t time[256]; 
//	_stprintf_s(time, _T("%d:%d:%d %03d"), g_st.wHour, g_st.wMinute, g_st.wSecond, g_st.wMilliseconds);
//	OutputDebugString(time); 
//#endif
//	OutputDebugString(buf); 
//	OutputDebugString(L"\r\n"); 
//	va_end(args); 
} 
#endif

std::wstring CPlatform::GetAppPath()
{
	return m_AppPath;
}

std::wstring CPlatform::GetAppDataPath()
{
	std::wstring wstrResult = _T("");

	WCHAR szAppDataPath[MAX_PATH] = {0};
	CoInitialize(NULL);
	LPITEMIDLIST  ppidl;
	if (SHGetSpecialFolderLocation(NULL, CSIDL_APPDATA, &ppidl) == S_OK)
	{
		SHGetPathFromIDList(ppidl, szAppDataPath);
		CoTaskMemFree(ppidl);
		wstrResult.append(szAppDataPath);
		wstrResult.append(L"\\");
		wstrResult.append(APP_NAME);
		wstrResult.append(L"\\");
	}
	CoUninitialize();
	return wstrResult;
}

std::string CPlatform::GetCrashPath()
{
	std::string strResult = UpStringUtility::WstringToString(CPlatform::GetAppDataPath()) + "dumppath\\";
	if (!UpFile::IsDirExisted(strResult.c_str()))
	{
		UpFile::CreatePath(strResult.c_str());
	}
	return strResult;
}

void CPlatform::InitAppPath(HINSTANCE hInstance)
{
	TCHAR tszModule[MAX_PATH + 1] = { 0 };
	::GetModuleFileNameW(hInstance,  tszModule, MAX_PATH);
	m_AppPath = UpFile::GetPathW(tszModule);
	if( m_AppPath[m_AppPath.length() - 1] != '\\' )
	{
		m_AppPath.append(L"\\");
	}
}

std::string CPlatform::GetMac()
{
	PIP_ADAPTER_INFO pAdapterInfo;
	DWORD AdapterInfoSize;
	char szMac[32] = { 0 };
	DWORD Err;
	AdapterInfoSize = 0;

	Err = GetAdaptersInfo(NULL, &AdapterInfoSize);

	if ((Err != 0) && (Err != ERROR_BUFFER_OVERFLOW))
	{
		return szMac;
	}

	pAdapterInfo = (PIP_ADAPTER_INFO)GlobalAlloc(GPTR, AdapterInfoSize);
	if (pAdapterInfo == NULL)
	{
		return  szMac;
	}
	if (GetAdaptersInfo(pAdapterInfo, &AdapterInfoSize) != 0)
	{
		GlobalFree(pAdapterInfo);
		return  szMac;
	}
	sprintf(szMac, "%02X-%02X-%02X-%02X-%02X-%02X",
		pAdapterInfo->Address[0],
		pAdapterInfo->Address[1],
		pAdapterInfo->Address[2],
		pAdapterInfo->Address[3],
		pAdapterInfo->Address[4],
		pAdapterInfo->Address[5]);

	GlobalFree(pAdapterInfo);

	return szMac;
}