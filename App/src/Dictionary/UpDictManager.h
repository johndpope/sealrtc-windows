#pragma once
#include <string>
#include "DictDefine.h"
#include <vector>

#define UPSTR(s) UpDictManager::FindString(L#s)

class UpDict;
class UpDictManager
{
public:
	UpDictManager(void);
	~UpDictManager(void);

	static std::wstring FindString(std::wstring key);
	static void DeleteInstance();
	static std::wstring UpDictManager::Format(const std::wstring format, std::vector<std::wstring> args);

	static UpDictManager* GetInstance();
	void Reload(Language lang);
private:
	bool Init(Language lang);
	void UnInit();

private:
	static UpDictManager *m_pDictManager;
	UpDict *m_pDict;
};