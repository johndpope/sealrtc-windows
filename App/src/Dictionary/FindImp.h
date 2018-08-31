#pragma once
#include "UpDictManager.h"

class FindImp : public IFind
{
public:
	static FindImp* GetInstance()
	{
		if (NULL == instance)
		{
			instance = new FindImp();
		}

		return instance;
	}
	static void ClearInstance()
	{
		if (instance)
		{
			delete instance;
			instance = NULL;
		}
	}

	virtual CDuiString Find(CDuiString key)
	{
		std::wstring value = UpDictManager::FindString(key.GetData());
		return value.c_str();
	}

private:
	static FindImp* instance;
};

FindImp* FindImp::instance = NULL;
