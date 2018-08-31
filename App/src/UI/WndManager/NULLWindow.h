#pragma once
#include "StdAfx.h"
#include "MsgBroadcast.h"

class CNULLWindow: public CWindowWnd
{
public:

	CNULLWindow(void)
	{
	}

	virtual ~CNULLWindow(void)
	{
	}

	virtual LPCTSTR GetWindowClassName() const
	{
		return _T("NULL");
	}


	virtual UINT GetClassStyle() const
	{
		return CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
	}

	virtual LRESULT HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam ) 
	{
		 LRESULT ret= 0;
		 if( uMsg == WM_UP_BROADCAST && m_Callback != NULL )
		 {
			 m_Callback->HandleMessage((unsigned int)uMsg, (void*)wParam,  (void*)lParam);
		 }
		 else
		 {
			 ret = __super::HandleMessage( uMsg, wParam,  lParam);
		 }
		 return ret;
		 
	}

	void Destroy()
	{
		if( IsWindow(this->GetHWND()) )
		{
			DestroyWindow(this->GetHWND());
		}
	}

	void SetCallback(MsgCallBack* val) { m_Callback = val; }

	virtual void OnFinalMessage( HWND hWnd ) 
	{
	}

private:
	MsgCallBack* m_Callback;
};

