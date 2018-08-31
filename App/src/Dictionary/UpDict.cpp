#include "UpDict.h"
#include "..\Duilib\Duilib\UIlib.h"
#include "Platform.h"
#include "UpStringUtility.h"

UpDict::UpDict(void)
{
}

UpDict::~UpDict(void)
{
}

std::wstring UpDict::FindString(std::wstring key)
{
	std::wstring lowerKey = UpStringUtility::LowerCase(key);
	std::map<std::wstring, std::wstring>::const_iterator map_it = m_StringMap.find(lowerKey);
	if (map_it != m_StringMap.end()) return map_it->second;

	CPlatform::Print(_T("value not found [key %s]"), key.c_str());
	return L"";
}

bool UpDict::LoadDict(std::wstring filename)
{
	std::wstring path = CPlatform::GetAppPath() + _T("setting\\") +filename;
	DuiLib::CMarkup xml;
	bool ret = xml.LoadFromFilePath(path.c_str());
	if (!ret) return false;

	DuiLib::CMarkupNode root = xml.GetRoot();
	if (root.HasChildren())
	{
		DuiLib::CMarkupNode string = root.GetChild(_T("string"));
		std::wstring id = UpStringUtility::StringTrim(string.GetAttributeValue(_T("id")));
		id = UpStringUtility::LowerCase(id);
		std::wstring value = UpStringUtility::StringTrim(string.GetValue());
		m_StringMap.insert(make_pair(id,value));

		while (string.HasSiblings())
		{
			string = string.GetSibling();
			std::wstring id = UpStringUtility::StringTrim(string.GetAttributeValue(_T("id")));
			id = UpStringUtility::LowerCase(id);
			std::wstring value = UpStringUtility::StringTrim(string.GetValue());
			m_StringMap.insert(make_pair(id,value));
		}
	}

	return true;
}

void UpDict::Clear()
{
	m_StringMap.clear();
}
