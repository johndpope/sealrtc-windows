#include "UpThreadHandler.h"
#include "UpStringUtility.h"

#define UP_THREAD_MSG WM_USER+10

#define MS_VC_EXCEPTION 0x406D1388

typedef struct tagTHREADNAME_INFO
{
	DWORD dwType; // Must be 0x1000.
	LPCSTR szName; // Pointer to name (in user addr space).
	DWORD dwThreadID; // Thread ID (-1=caller thread).
	DWORD dwFlags; // Reserved for future use, must be zero.
} THREADNAME_INFO;

void SetThreadName( DWORD dwThreadID, LPCSTR szThreadName)
{
	THREADNAME_INFO info;
	info.dwType = 0x1000;
	info.szName = szThreadName;
	info.dwThreadID = dwThreadID;
	info.dwFlags = 0;
	__try
	{
		RaiseException( MS_VC_EXCEPTION, 0, sizeof(info)/sizeof(DWORD), (DWORD*)&info );
	}
	__except(EXCEPTION_CONTINUE_EXECUTION)
	{
	}
}


DWORD WINAPI threadProc(LPVOID lpParameter)
{
	UpThreadHandler* handle = (UpThreadHandler*)lpParameter;
	MSG Msg;
	if( handle != NULL )
	{
		handle->SetStatus(true);
		handle->DealRemain();
	}
	while(GetMessage(&Msg,NULL,0,0))
	{
		switch (Msg.message)
		{
		    case UP_THREAD_MSG:
				{
					 if(  handle != NULL )
					 {
						 handle->DealRemain();
					 }
				}
				break;
			case WM_TIMER:
				{
					if( handle != NULL)
					{
						UpTimer::TriggerTimeOut(Msg.wParam);
					}
				}
				break;
			default:
				break;
		}
	}
	return 0;
}


UpThreadHandler::UpThreadHandler(void)
{
	m_timer = NULL;
	m_runable = NULL;
	m_thread = NULL;
	m_threadId  = 0;
	m_iswork= 0;
	m_elapsetime = 0;
}


UpThreadHandler::~UpThreadHandler(void)
{
	Stop();
}

void UpThreadHandler::Start(std::string name)
{
	Stop();
    StratThread(name);
}

void UpThreadHandler::Post(Runable* runable, unsigned int elapsetime )
{
	m_runable = runable;
	m_elapsetime = elapsetime;
	if( m_iswork )
	{
		PostThreadMessage(m_threadId, UP_THREAD_MSG, 0, 0);
	}
	
}

void UpThreadHandler::Stop()
{
	m_iswork  = false;
	m_runable = NULL;

	if( m_thread != NULL )
	{
		PostThreadMessage(m_threadId, WM_QUIT, 0,0);
		if( m_threadId != GetCurrentThreadId() && WAIT_OBJECT_0 != WaitForSingleObject(m_thread, 150))
		{
			TerminateThread(m_thread, 0);
		}
		CloseHandle(m_thread);
		m_thread = NULL;
	}

	if(  m_timer != NULL )
	{
		m_timer->Stop();
		delete m_timer;
		m_timer = NULL;
	}

}

void UpThreadHandler::StratThread(std::string name)
{
	m_thread = CreateThread(NULL, 0,threadProc , this, 0, &m_threadId);
#ifdef _DEBUG
	if( name.length() > 0 )
	{
		SetThreadName(m_threadId, name.c_str());
	}
#endif
	
}

void UpThreadHandler::Timeout(UpTimer* timer)
{
	if(  m_timer != NULL )
	{
		m_timer->Stop();
		delete m_timer;
		m_timer = NULL;
	}
	Threadwork();
}

void UpThreadHandler::Threadwork()
{
	if( m_runable != NULL )
	{
		m_runable->Run();
	}
}

void UpThreadHandler::DealRemain()
{
	if( m_elapsetime == 0 )
	{
		Threadwork();
	}
	else
	{
		if(  m_timer != NULL )
		{
			m_timer->Stop();
			delete m_timer;
			m_timer = NULL;
		}
		m_timer = new UpTimer();
		m_timer->Start(m_elapsetime, this);
	}
}

void UpThreadHandler::SetPriorityThread(TThreadPriority priority)
{
	if( m_thread == NULL ) return;
	int threadPriority = THREAD_PRIORITY_NORMAL;
	switch(priority)
	{
	case EThreadCritical:
		threadPriority  = THREAD_PRIORITY_TIME_CRITICAL;
		break;
	case EThreadHigh:
		threadPriority  = THREAD_PRIORITY_HIGHEST;
		break;
	case EThreadLow:
		threadPriority  = THREAD_PRIORITY_IDLE;
		break;
	case EThreadNormal:
		threadPriority  = THREAD_PRIORITY_NORMAL;
		break;

	default:
		break;
	}
	int ret = SetThreadPriority(m_thread, threadPriority);
}

