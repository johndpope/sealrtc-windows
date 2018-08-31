#include "UpLock.h"
#include "Platform.h"

UpLock::UpLock(void)
{
	  InitializeCriticalSection(&m_cs);
	  m_tmp = NULL;
}

UpLock::UpLock(UpLock* upLock)
{	
	m_tmp = upLock;
    m_tmp->Lock();
}

UpLock::~UpLock(void)
{
	if( m_tmp )
	{
		m_tmp->Unlock();
	}
	else
	{
	     DeleteCriticalSection(&m_cs);
	}
}

void UpLock::Lock()
{
	//CPlatform::Print(L"UpLock::Lock = %d",this);
	EnterCriticalSection(&m_cs);
}

void UpLock::Unlock()
{
	//CPlatform::Print(L"UpLock::UnLock = %d",this);
	LeaveCriticalSection(&m_cs);
}
