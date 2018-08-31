#include "UpTimer.h"
#include <Windows.h>

std::map<__int64, UpTimer*> UpTimer::m_workmap;
UpLock UpTimer::m_lock;

void CALLBACK timerProc(
	HWND hwnd,
	UINT uMsg,
	UINT_PTR idEvent,
	DWORD dwTime
	)
{
	UpTimer::TriggerTimeOut(idEvent);
}

UpTimer::UpTimer(void)
{
	m_hEvent = 0;
	m_observer = NULL;
	m_bWork = false;
}
UpTimer::~UpTimer(void)
{
	if( m_hEvent != 0 )
	{
		Stop();
	}
}

//The time-out value, in milliseconds
void UpTimer::Start(unsigned int  elapsetime, TimerObserver* observer)
{
	m_lock.Lock();
	m_hEvent  = ::SetTimer(NULL, m_hEvent, elapsetime, timerProc);
	if( m_hEvent != 0 )
	{
		m_workmap[m_hEvent] = this;
		m_observer = observer;
		m_bWork = true;
	}
	m_lock.Unlock();
}

void UpTimer::Stop()
{
	m_lock.Lock();
	if( m_hEvent != 0  )
	{
		KillTimer(NULL, m_hEvent);
		if( m_workmap.size() > 0 && (m_workmap.find(m_hEvent) != m_workmap.end()) )
		{
			 m_workmap.erase(m_hEvent);
		}

		m_observer = NULL;
		m_hEvent = 0;
		m_bWork = false;
	}
	m_lock.Unlock();

}

void UpTimer::Timeout()
{
	if( m_observer != NULL )
	{
		m_observer->Timeout(this);
	}
}

void UpTimer::TriggerTimeOut(__int64 idEvent)
{
	m_lock.Lock();
	
	UpTimer* timer = NULL;
	if ( m_workmap.size() > 0 )
	{
		std::map<__int64, UpTimer*>::const_iterator it = m_workmap.find(idEvent);		
		if((it != m_workmap.end()) )
		{
			timer = it->second;
		}
	}

	m_lock.Unlock();

	if (timer != NULL && timer->m_hEvent != 0 )
	{
		timer->Timeout();
	}
}
