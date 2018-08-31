#pragma once

#include <Windows.h>

#define LOCK()  \
      UpLock up_lock_xxx(this) 


class UpLock
{
public:
	UpLock(void);
	UpLock(UpLock* upLock);
	virtual ~UpLock(void);

public:
	void Lock();
	void Unlock();
private:
	CRITICAL_SECTION m_cs;
	UpLock* m_tmp;
};

