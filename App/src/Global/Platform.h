#pragma once
#include <xstring>
#include <Windows.h>
#include <tchar.h>
#include<stdlib.h>
#include "UpFile.h"

class CLog;

#define UPLOG CLog log(__FUNCTION__);

class CPlatform
{
public:
	static void Print(const wchar_t* format, ...);
	static std::wstring GetAppPath();
	static std::wstring GetAppDataPath();
	static std::string GetCrashPath();
	static void InitAppPath(HINSTANCE hInstance);
	static std::string GetMac();
private:
	static std::wstring m_AppPath;
};
