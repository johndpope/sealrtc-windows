#ifndef __UP_INTEGER_H__
#define __UP_INTEGER_H__
#pragma once
#include <string>

namespace up_utility
{
	class Integer
	{
	public:
		Integer();
		virtual~ Integer();

	public:
		static std::wstring ToWString(int value);
		static std::string ToString(int value);
		static int ParseWString(std::wstring wstring);
		static int ParseString(std::string string);

		static std::wstring ToWString_64(__int64 value);
		static std::string ToString_64(__int64 value);
		static __int64 ParseWString_64(std::wstring wstring);
		static __int64 ParseString_64(std::string string);

	public:
		static bool UnitTest();
	};

}

#endif