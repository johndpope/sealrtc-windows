#include "MsgBroadcast.h"
#include "TransparentWnd.h"
#include "NULLWindow.h"

MsgBroadcast* MsgBroadcast::instance = NULL;

MsgBroadcast::MsgBroadcast(void)
{
	m_threadId = GetCurrentThreadId();
	m_wnd = new CNULLWindow();
    m_wnd->Create(NULL,  NULL, WS_POPUP, WS_EX_TOOLWINDOW , CDuiRect(0, 0, 0 ,0));
	m_wnd->ShowWindow(false);
	m_wnd->SetCallback(this);
	m_isWork = true;
}

unsigned long MsgBroadcast::GetHideWnd(void)
{
    return (unsigned long)m_wnd->GetHWND();
}

MsgBroadcast::~MsgBroadcast(void)
{
	Destroy();
}

 long MsgBroadcast::SendMsg( BroadcastType type, MsgItem* item )
{
	long ret = 0;
	if( m_isWork )
	{
		ret = SendMessage(m_wnd->GetHWND(), WM_UP_BROADCAST, type, (long)item);
	}
	else
	{
		delete item;
	}
	return ret;
}

void MsgBroadcast::PostMsg( BroadcastType type, MsgItem* item )
{
	if( m_isWork )
	{
		PostMessage(m_wnd->GetHWND(), WM_UP_BROADCAST, type, (long)item);
	}
}


long MsgBroadcast::Dispatch(void* wParam, void* lParam)
{
	long ret = 0;
	CWndManager* wndManager = CWndManager::GetWndManager();
	int count = wndManager->GetCount();
	std::vector<HWND> hwndArray;
	for( int i = 0; i < count;  i++ )
	{
		 CBaseWnd* wnd = wndManager->GetWnd(i);
		 hwndArray.push_back(wnd->GetHWND());
	}
	std::vector<HWND>::iterator it = hwndArray.begin();
	for(; it != hwndArray.end(); it++)
	{
		if( ret == 0 )
		{
		    ret = SendMessage(*it,WM_UP_BROADCAST, (WPARAM)wParam, (LPARAM)lParam );
		}
		else
		{
			SendMessage(*it,WM_UP_BROADCAST, (WPARAM)wParam, (LPARAM)lParam );
		}
	}
	hwndArray.clear();
	delete (MsgItem*)lParam;
	return ret;
}

MsgBroadcast* MsgBroadcast::GetMsgBroadcast()
{
	if( instance == NULL )
	{
		instance = new MsgBroadcast();
	}
    return instance;	
}


long MsgBroadcast::HandleMessage( unsigned int uMsg, void* wParam, void* lParam )
{
	long ret = 0;
	if( uMsg == WM_UP_BROADCAST )
	{
		MsgBroadcast* msgBroadcast = MsgBroadcast::GetMsgBroadcast();
		ret = msgBroadcast->Dispatch(wParam, lParam);
	}
	return ret;
}

void MsgBroadcast::Destroy()
{
	if( m_wnd != NULL )
	{
		m_wnd->Destroy();
		delete m_wnd;
		m_wnd = NULL;
	}
}

unsigned long MsgBroadcast::GetMainThreadId()
{
	return m_threadId;
}


void MsgBroadcast::SetEnable( bool enable )
{
	m_isWork = enable;
}



