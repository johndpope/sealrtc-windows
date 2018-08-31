#pragma once

#include <iostream>
#include <xstring>
#include <list>


struct list;

class CinBuffer;
class UpStringUtility
{
public:
	UpStringUtility();
	virtual~ UpStringUtility();

	static char* WcharToChar(const wchar_t* wp);
	static char* StringToChar(const std::string& s);
	static char* WstringToChar(const std::wstring& ws);
	static wchar_t* CharToWchar(const char* c);
	static wchar_t* WstringToWchar(const std::wstring& ws);
	static wchar_t* StringToWchar(const std::string& s);
	static std::wstring StringToWstring(const std::string& s);
	static std::string WstringToString(const std::wstring& ws);
	static std::wstring UTF8ToUnicode(const std::string& s);
	static std::string UnicodeToUTF8(const std::wstring& ws);
	static std::string convertToHexString(const std::string buf);
	static char* convertToHex(const std::string buf);
	static int HexToChar(const char* aHex, char* out);
	static __int64 BufToInt64(std::string buf);
	static std::string Int64ToBuf(__int64 i64);
	static std::string NewUUIDString();
	static std::wstring NewUUIDWString();
	static std::string GetGUID();
	static std::wstring GetGUIDW();
	static std::string GetGUIDBuf16();
	static std::wstring LeftTrim(const std::wstring& s);
	static std::wstring RightTrim(const std::wstring& s);
	static std::wstring StringTrim(const std::wstring& s);
	static std::list<std::string >* Split(std::string& buf, const unsigned char splitFlag);
	static std::wstring Join(std::list<std::wstring> strings, std::wstring flag);
	static std::wstring UpperCase(const std::wstring& val);
	static std::wstring LowerCase(const std::wstring& val);
	static std::string UpperCase(const std::string& val);
	static std::string LowerCase(const std::string& val);
	static void ParseUdpString(const std::string & serverStr, std::string & strIp, int &port);
	static bool FindNoCase(std::wstring str, std::wstring substr);
};