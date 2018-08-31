
#ifndef __APP_TYPE_H__
#define __APP_TYPE_H__

#pragma once

namespace DuiLib
{
	typedef CPoint				CUPoint;
	typedef CSize				CUSize;
	typedef CDuiRect			CURect;
	typedef CStdPtrArray		CUPtrArray;
	typedef CStdValArray		CUValArray;
	typedef CDuiString			CUString;
	typedef CStdStringPtrMap	CUStringPtrMap;
	typedef CVariant			CUVariant;
}

#define MAIN_UI_STYLE  WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_POPUP
#define CHILD_UI_STYLE  WS_VISIBLE | WS_CLIPSIBLINGS | WS_POPUP

#define DECL_APPWND_CONTROL() \
	static CBaseWnd* CALLBACK CreateImpl( ); \
	static void RegisterWnd();

#define IMPL_APPWND_CONTROL(Class,WndID) \
	CBaseWnd* CALLBACK Class::CreateImpl( ) \
{ \
	return new Class();  \
} \
	void Class::RegisterWnd() \
{ \
	CWndManager::RegisterWnd( WndID, new CAppWndFactory( Class::CreateImpl ) ); \
}

#define DECL_NOTIFY_MSG() \
	virtual void Notify(TNotifyUI& msg);

#define NOTIFY_BEGIN(theClass) \
	void theClass::Notify(TNotifyUI& msg) \
	{ \
		CUString sName = _T(""); \
		if(msg.pSender != NULL)  \
		{ sName = msg.pSender->GetName(); } \
		CUString sType = msg.sType; \
		if(FALSE) {} 

#define ON_NOTIFY_MSG(EventType, ControlName, Func) \
		else if(_tcsicmp(sType,EventType) == 0 && (_tcsicmp(ControlName, sName) == 0 || ControlName == _T(""))) \
		{ return Func(msg);} 

#define NOTIFY_END() \
		__super::Notify(msg); \
	}

#endif //__APP_TYPE_H__