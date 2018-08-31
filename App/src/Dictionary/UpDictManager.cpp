#include "UpDictManager.h"
#include <regex>
#include<Windows.h>
#include<WinNls.h>
#include "UpDict.h"
#include "..\Duilib\Duilib\UIlib.h"
#include "UpStringUtility.h"
#include "Platform.h"


UpDictManager* UpDictManager::m_pDictManager = NULL;

UpDictManager::UpDictManager(void):
m_pDict(NULL)
{
	LANGID lid = GetSystemDefaultLangID();
	if (lid == 0X0804)
	{
		Init(Chinese);
	}
	else
	{
		Init(English);
	}
}

UpDictManager::~UpDictManager(void)
{
	UnInit();
}

UpDictManager* UpDictManager::GetInstance()
{
	if (NULL == m_pDictManager)
	{
		m_pDictManager = new UpDictManager();
	}

	return m_pDictManager;
}

void UpDictManager::DeleteInstance()
{
	if (m_pDictManager)
	{
		delete m_pDictManager;
		m_pDictManager = NULL;
	}		
}

void UpDictManager::Reload(Language lang)
{
	ASSERT(NULL != m_pDict);
	if (m_pDict)
	{
		m_pDict->Clear();
		Init(lang);
	}
}

std::wstring UpDictManager::FindString(std::wstring key)
{
	UpDict* pDict = UpDictManager::GetInstance()->m_pDict;
	if (pDict)
	{
		return pDict->FindString(key);
	}
	return _T("");
}

bool UpDictManager::Init(Language lang)
{
	std::wstring strName = _T("en.xml");
	if (lang == English)
	{
		strName = _T("en.xml");
	}
	else if (lang == Chinese)
	{
		strName = _T("zh.xml");
	}

	if (!m_pDict) m_pDict = new UpDict();
	return m_pDict->LoadDict(strName);
}

void UpDictManager::UnInit()
{
	if (m_pDict) delete m_pDict;
}

std::wstring UpDictManager::Format(const std::wstring format, std::vector<std::wstring> args)
{
	const std::tr1::wregex pattern(L"%\\d+\\$s");
	std::wstring str =format;

	const std::wsregex_token_iterator end;
	for (std::wsregex_token_iterator it(format.begin(),format.end(), pattern); it != end ; ++it)
	{
		std::wstring matchStr = *it;
		const std::tr1::wregex pattern_num(L"\\d+");
		std::wsmatch numResult;
		bool match = std::regex_search(matchStr, numResult, pattern_num);
		if (match)
		{
			int index = _wtoi(numResult.str().c_str()) - 1;

			if (args.size() > static_cast<unsigned int>(index))
			{
				std::wstring argStr = args.at(index);
				int pos = str.find(matchStr.c_str());
				if (pos >= 0)
				{
					str.replace(pos, matchStr.length(), argStr.c_str());
					std::wcout<<str<<std::endl;
				}
			}
		}
	}

	return str;
}