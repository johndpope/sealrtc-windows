#pragma once

#include "Broadcast_type.h"

class CNULLWindow;

class MsgCallBack
{
public:
	virtual ~MsgCallBack(){}
	virtual long HandleMessage( unsigned int uMsg, void* wParam, void* lParam ) = 0;
};


class MsgBroadcast: public MsgCallBack
{
public:
	static MsgBroadcast* GetMsgBroadcast();
	virtual ~MsgBroadcast(void);
	void Destroy();
	unsigned long GetMainThreadId();
protected:
	MsgBroadcast(void);

public:
	unsigned long GetHideWnd(void);

	long  SendMsg(BroadcastType type, MsgItem* item);
	void PostMsg( BroadcastType type, MsgItem* item );
	long Dispatch(void* wParam, void* lParam);

	virtual long HandleMessage( unsigned int uMsg, void* wParam, void* lParam );
	void SetEnable(bool enable);

private:
    CNULLWindow* m_wnd;
	static MsgBroadcast* instance;
	unsigned long m_threadId;
	bool       m_isWork;
};

