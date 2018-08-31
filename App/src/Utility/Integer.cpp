#include <map>
#include <utility>
#include "integer.h"

namespace up_utility
{

	std::wstring Integer::ToWString( int value )
	{
		wchar_t	strVal[100];
		_itow_s(value, strVal, 100, 10);
		return strVal;
	}

	std::string Integer::ToString( int value )
	{
		char strVal[100];
		_itoa_s(value, strVal, 100, 10);
		return strVal;
	}

	int Integer::ParseWString( std::wstring wstring )
	{
		return _wtoi(wstring.c_str());
	}

	int Integer::ParseString( std::string string )
	{
		return atoi(string.c_str());
	}

	std::wstring Integer::ToWString_64( __int64 value )
	{
		wchar_t	strVal[100];
		_i64tow_s(value, strVal, 100, 10);
		return strVal;
	}

	std::string Integer::ToString_64( __int64 value )
	{
		char strVal[100];
		_i64toa_s(value, strVal, 100, 10);
		return strVal;
	}

	__int64 Integer::ParseWString_64( std::wstring wstring )
	{
		return _wtoi64(wstring.c_str());
	}

	__int64 Integer::ParseString_64( std::string string )
	{
		return _atoi64(string.c_str());
	}

	bool Integer::UnitTest()
	{
		//////////////////////////////////////////////////////////////////////////
		std::map<int, std::string> in_out;
		in_out.insert(std::make_pair(0,"0"));
		in_out.insert(std::make_pair(123,"123"));
		in_out.insert(std::make_pair(65535,"65535"));

		for (std::map<int, std::string>::iterator it=in_out.begin(); it!=in_out.end(); it++)
		{
			if((*it).second != Integer::ToString((*it).first))
			{
				printf("Test Failed: Integer::ToString");

				return false;
			}

			if((*it).first != Integer::ParseString((*it).second))
			{
				printf("Test Failed: Integer::ParserString");

				return false;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		std::map<__int64, std::string> in_out64;
		in_out64.insert(std::make_pair(0,"0"));
		in_out64.insert(std::make_pair(123,"123"));
		in_out64.insert(std::make_pair(65535,"65535"));
		in_out64.insert(std::make_pair(11165535,"11165535"));

		for (std::map<__int64, std::string>::iterator it=in_out64.begin(); it!=in_out64.end(); it++)
		{
			if((*it).second != Integer::ToString_64((*it).first))
			{
				printf("Test Failed: Integer::ToString_64");

				return false;
			}

			if((*it).first != Integer::ParseString_64((*it).second))
			{
				printf("Test Failed: Integer::ParserString_64");

				return false;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		std::map<int, std::wstring> in_outw;
		in_outw.insert(std::make_pair(0,L"0"));
		in_outw.insert(std::make_pair(123,L"123"));
		in_outw.insert(std::make_pair(65535,L"65535"));

		for (std::map<int, std::wstring>::iterator it=in_outw.begin(); it!=in_outw.end(); it++)
		{
			if((*it).second != Integer::ToWString((*it).first))
			{
				printf("Test Failed: Integer::ToWString");

				return false;
			}

			if((*it).first != Integer::ParseWString((*it).second))
			{
				printf("Test Failed: Integer::ParserWString");

				return false;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		std::map<__int64, std::wstring> in_out64w;
		in_out64w.insert(std::make_pair(0,L"0"));
		in_out64w.insert(std::make_pair(123,L"123"));
		in_out64w.insert(std::make_pair(65535,L"65535"));
		in_out64w.insert(std::make_pair(11165535,L"11165535"));

		for (std::map<__int64, std::wstring>::iterator it=in_out64w.begin(); it!=in_out64w.end(); it++)
		{
			if((*it).second != Integer::ToWString_64((*it).first))
			{
				printf("Test Failed: Integer::ToWString_64");

				return false;
			}

			if((*it).first != Integer::ParseWString_64((*it).second))
			{
				printf("Test Failed: Integer::ParserWString_64");

				return false;
			}
		}

		return true;
	}

}
