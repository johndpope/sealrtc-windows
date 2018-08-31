#pragma once
#include <string>
#include <Windows.h>
#include "Base.h"
#include <xstring>


class UpMutex: public CBase
{
public:
	UpMutex();
	UpMutex(std::string id);
	static bool CheckSameId(std::string id);
	virtual ~UpMutex(void);
private:
	HANDLE m_mutex;
};

