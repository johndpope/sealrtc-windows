#include <windows.h>
#include <algorithm>
#include <functional>
#include "UpStringUtility.h"
#include "Platform.h"

#pragma comment(lib,"Rpcrt4.lib")

#define MIDSTR 8

UpStringUtility::UpStringUtility()
{
}

UpStringUtility::~UpStringUtility()
{
}

char* UpStringUtility::WcharToChar(const wchar_t* wp)
{
	int len= WideCharToMultiByte(CP_ACP,0,wp,wcslen(wp),NULL,0,NULL,NULL);
	char* m_char=new char[len+1];
	WideCharToMultiByte(CP_ACP,0,wp,wcslen(wp),m_char,len,NULL,NULL);
	m_char[len]='\0';
	return m_char;
}

wchar_t* UpStringUtility::CharToWchar(const char* c)
{
	int len = MultiByteToWideChar(CP_ACP,0,c,strlen(c),NULL,0);
	wchar_t* m_wchar=new wchar_t[len+1];
	MultiByteToWideChar(CP_ACP,0,c,strlen(c),m_wchar,len);
	m_wchar[len]='\0';
	return m_wchar;
}

std::wstring UpStringUtility::UTF8ToUnicode(const std::string& s)
{
	if( s.length() ==0 )
	{
		return L"";
	}
	std::wstring str;
	int len = MultiByteToWideChar(CP_UTF8,0,s.c_str(),s.length(),NULL,0);
	wchar_t* m_wchar=new wchar_t[len+1];
	MultiByteToWideChar(CP_UTF8,0,s.c_str(),s.length(),m_wchar,len);
	m_wchar[len]='\0';
	str.append(m_wchar, len);
	delete []m_wchar;
	m_wchar = NULL;
	return str;
}

std::string UpStringUtility::UnicodeToUTF8(const std::wstring& ws)
{
	std::string str;
	int len= WideCharToMultiByte(CP_UTF8,0,ws.c_str(),ws.length(),NULL,0,NULL,NULL);
	char* m_char=new char[len+1];
	WideCharToMultiByte(CP_UTF8,0,ws.c_str(),ws.length(),m_char,len,NULL,NULL);
	m_char[len]='\0';
	str.append(m_char,len);
	delete []m_char;
	m_char = NULL;
	return str;
}

char* UpStringUtility::StringToChar(const std::string& s)
{
	return const_cast<char*>(s.c_str());
}

char* UpStringUtility::WstringToChar(const std::wstring &ws)
{
	const wchar_t* wp=ws.c_str();
	return WcharToChar(wp);
}

wchar_t* UpStringUtility::WstringToWchar(const std::wstring &ws)
{
	return const_cast<wchar_t*>(ws.c_str());
}

wchar_t* UpStringUtility::StringToWchar(const std::string& s)
{
	const char* p=s.c_str();
	return CharToWchar(p);
}

std::string UpStringUtility::WstringToString(const std::wstring &ws)
{
	std::string s;
	char* p=WstringToChar(ws);
	s.append(p);
	delete []p;
	p = NULL;
	return s;
}

std::wstring UpStringUtility::StringToWstring(const std::string &s)
{
	std::wstring ws;
	wchar_t* wp=StringToWchar(s);
	ws.append(wp);
	delete []wp;
	wp = NULL;
	return ws;
}

std::string UpStringUtility::convertToHexString(const std::string buf)
{
	char *outstr = convertToHex(buf);
	std::string hexString(outstr);
	delete []outstr;
	outstr = NULL;

	return hexString;
}

char* UpStringUtility::convertToHex(const std::string buf)
{
	int h = 0, l = 0;
	int len = buf.length();	
	char* outStr = (char*)malloc(len*2+1);
	memset(outStr, 0, len*2+1);
	for (int i = 0; i < len; i++)
	{
		h = buf.c_str()[i] & 0xf0;
		h >>= 4;
		l = buf.c_str()[i] & 0x0f;
		outStr[i * 2] = ( h >= 0x0 && h <= 0x9 ) ? ( h + 0x30 ) : ( h + 0x57 );
		outStr[i * 2 + 1] = ( l >= 0x0 && l <= 0x9 ) ? ( l + 0x30 ) : ( l + 0x57 );
	}
	return outStr;
}

int UpStringUtility::HexToChar(const char* aHex, char* out)
{
	int len = strlen((const char*)aHex);
	int j = 0;
	int h = 0;
	int l = 0;

	for(int i=0; i< len; i+=2)
	{
		h = ( aHex[i] >= 0x30 && aHex[i] <= 0x39 ) ? ( aHex[i] - 0x30 ) : ( aHex[i] >= 0x41 && aHex[i] <= 0x46 ) ? ( aHex[i] - 0x37 ) : ( aHex[i] >= 0x61 && aHex[i] <= 0x66 ) ? ( aHex[i] - 0x61 + 0x0a ) : -1;
		l = ( aHex[i + 1] >= 0x30 && aHex[i + 1] <= 0x39 ) ? ( aHex[i + 1] - 0x30 ) : ( aHex[i + 1] >= 0x41 && aHex[i + 1] <= 0x46 ) ? ( aHex[i + 1] - 0x37 ) : ( aHex[i + 1] >= 0x61 && aHex[i + 1] <= 0x66 ) ? ( aHex[i + 1] - 0x61 + 0x0a ) : -1;

		out[j] = ( h << 4 ) + l;
		j++;
	}
	return len/2;
}

__int64 UpStringUtility::BufToInt64(std::string buf)
{
	char length = buf.length();
	if(length == 0)
	{
		return 0;
	}
	__int64 ret = 0;
	unsigned char *p = (unsigned char*)buf.c_str();
	for(char i = 0; i < length; i ++)
	{
		ret |= ((__int64)*(p++))<< 8 * i;
	}
	return ret;
}

std::string UpStringUtility::Int64ToBuf(__int64 i64)
{
	char leadingZeroBytes = 8;
	for(char i = 8; i >1 ; i--)
	{
		if(((i64 >> 8 * (i - 1)) & 0xFF) != 0)
		{
			leadingZeroBytes = 8-i;
			break;
		}
	}
	if(leadingZeroBytes == 8)
	{
		leadingZeroBytes = 7;
	}
	unsigned short len = 8 - leadingZeroBytes;

	std::string buf;
	for(char i = 0 ; i < len; i++)
	{
		unsigned char p[2] = {0};
		p[0] = (i64 >> 8*i) & 0xff;
		p[1] = '\0';
		buf.append((const char*)p);
	}
	return buf;
}

std::string UpStringUtility::NewUUIDString()
{
	std::string uuidstring = WstringToString(NewUUIDWString());
	return uuidstring;
}

std::wstring UpStringUtility::NewUUIDWString()
{
	UUID guid;
	UuidCreate(&guid);
	RPC_WSTR rpc_ws;
	UuidToString(&guid,&rpc_ws);
	return (LPTSTR)rpc_ws;
}

std::string UpStringUtility::GetGUID()
{
	std::string buf;
	GUID guid;
	::CoInitialize(NULL);
	if (S_OK == ::CoCreateGuid(&guid))
	{
		char str[64] = {0};
		sprintf_s(str, 64, "%08X%04X%04X%02X%02X%02X%02X%02X%02X%02X%02X"
			, guid.Data1
			, guid.Data2
			, guid.Data3
			, guid.Data4[0], guid.Data4[1]
		, guid.Data4[2], guid.Data4[3]
		, guid.Data4[4], guid.Data4[5]
		, guid.Data4[6], guid.Data4[7]);

		buf.append(str, 32);

	}
	::CoUninitialize();
	return buf;
}

std::wstring UpStringUtility::GetGUIDW()
{
	return StringToWstring(GetGUID());
}

std::string UpStringUtility::GetGUIDBuf16()
{
	std::string buf;
	GUID guid;
	if (S_OK == ::CoCreateGuid(&guid))
	{
		char str[64] = {0};
		sprintf_s(str, 64, "%08X%04X%04X%02X%02X%02X%02X%02X%02X%02X%02X"
			, guid.Data1
			, guid.Data2
			, guid.Data3
			, guid.Data4[0], guid.Data4[1]
		, guid.Data4[2], guid.Data4[3]
		, guid.Data4[4], guid.Data4[5]
		, guid.Data4[6], guid.Data4[7]);

		char guidstr[32] = {0};
		UpStringUtility::HexToChar(str, guidstr);
		buf.append(guidstr, 16);
	}
	return buf;
}

void UpStringUtility::ParseUdpString(const std::string & serverStr, std::string & strIp, int &port)
{
	std::string::size_type pos = serverStr.find(':');
	if (pos < 1u)
	{
		return;
	}
	
	strIp = serverStr.substr(0u, pos);
	port = atoi(serverStr.substr(pos +1, serverStr.size()).c_str());
}

std::wstring UpStringUtility::LeftTrim(const std::wstring& s)
{
	std::wstring ss = s;
	ss.erase(ss.begin(), std::find_if(ss.begin(), ss.end(), std::not1(std::ptr_fun(iswspace))));
	return ss;
}

std::wstring UpStringUtility::RightTrim(const std::wstring& s)
{
	std::wstring ss = s;
	ss.erase(std::find_if(ss.rbegin(), ss.rend(), std::not1(std::ptr_fun(iswspace))).base(), ss.end());
	return ss; 
}

std::wstring UpStringUtility::StringTrim(const std::wstring& s)
{
	return LeftTrim(RightTrim(s)); 
}

std::list<std::string >* UpStringUtility::Split(std::string& buf,const unsigned char splitFlag)
{
	std::list<std::string >* list = new std::list<std::string >();
	std::string dest = buf;
	int index = dest.find_first_of(splitFlag);
	if( index == -1 )
	{
		list->push_back(buf);
	}
	else
	{
		while( index != -1 )
		{
			std::string val = dest.substr(0, index);
			list->push_back(val);
			dest = dest.substr(index + 1, dest.length() - val.length() - 1);
			index = dest.find_first_of(splitFlag);
		}
		if (dest.length() > 0)
		{
			list->push_back(dest);
		}
	}
	
	return list;
}

std::wstring UpStringUtility::Join(std::list<std::wstring> strings, std::wstring flag)
{
	std::wstring result;
	for (std::list<std::wstring>::iterator it = strings.begin(); it != strings.end(); ++it)
	{
		if (it != strings.begin())
		{
			result += flag;
		}
		result += (*it); 
	}

	return result;
}

std::wstring UpStringUtility::UpperCase(const std::wstring& val)
{
	std::wstring ret = L"";
	int len = val.length();
	for( int i = 0; i < len; i++ )
	{
		wchar_t c[2] = {0};
		c[0] = val.at(i);
		if( iswalpha(c[0]) !=0  )
		{
			wchar_t tmp[2] = {0};
			tmp[0] = towupper(c[0]);
			ret.append(tmp);
		}
		else
		{
			ret.append(c);
		}
	}
	return ret;
}

std::wstring UpStringUtility::LowerCase(const std::wstring& val)
{
	std::wstring ret;
	int len = val.length();
	for( int i = 0; i < len; i++ )
	{
		wchar_t c[2] = {0};
		c[0] = val.at(i);
		if( iswalpha(c[0]) !=0  )
		{
			wchar_t tmp[2] = {0};
			tmp[0] = towlower(c[0]);
			ret.append(tmp);
		}
		else
		{
			ret.append(c);
		}
	}
	return ret;
}

std::string UpStringUtility::UpperCase(const std::string& val)
{
	std::string ret;
	int len = val.length();
	for( int i = 0; i < len; i++ )
	{
		char c[2] = {0};
		c[0] = val.at(i);
		if( isalpha(c[0]) !=0  )
		{
			char tmp[2] = {0};
			tmp[0] = toupper(c[0]);
			ret.append(tmp);
		}
		else
		{
			ret.append(c);
		}
	}
	return ret;
}

std::string UpStringUtility::LowerCase(const std::string& val)
{
	std::string ret;
	int len = val.length();
	for( int i = 0; i < len; i++ )
	{
		char c[2] = {0};
		c[0] = val.at(i);
		if( isalpha(c[0]) !=0  )
		{
			char tmp[2] = {0};
			tmp[0] = tolower(c[0]);
			ret.append(tmp);
		}
		else
		{
			ret.append(c);
		}
	}
	return ret;
}

bool UpStringUtility::FindNoCase(std::wstring str, std::wstring substr)
{
	std::wstring strNoCase = UpStringUtility::LowerCase(str);
	std::wstring substrNoCase = UpStringUtility::LowerCase(substr);
	return ( strNoCase.find(substrNoCase) == std::wstring::npos) ? FALSE: TRUE;
}