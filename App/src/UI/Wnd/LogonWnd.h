#ifndef LOGONWND_H
#define LOGONWND_H
#include <string>
#include "UpHttp.h"
#include "UpTimer.h"

class LogonWnd : public CBaseWnd, public TimerObserver
{
public:
	LogonWnd(void);
	virtual ~LogonWnd(void);

    DECL_NOTIFY_MSG()
    DECL_APPWND_CONTROL()

    virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual void OnFinalMessage(HWND hWnd);	
	virtual LPCTSTR GetWindowClassName() const;
	virtual void PreCreateWindowInit(const CStdStringPtrMap& aInitData);
	virtual void Timeout(UpTimer * timer);
public:
	void OnWinInit(TNotifyUI& msg);
	void OnClickCloseBtn(TNotifyUI &msg);
	void OnClickChannelEditTip(TNotifyUI& msg);
	void OnChannelEditKillFocus(TNotifyUI& msg);
	void OnClickNameEditTip(TNotifyUI& msg);
	void OnNameEditKillFocus(TNotifyUI& msg);
	void OnClickPhoneEditTip(TNotifyUI& msg);
	void OnPhoneEditKillFocus(TNotifyUI& msg);
	void OnClickVerifyCodeEditTip(TNotifyUI& msg);
	void OnVerifyCodeEditKillFocus(TNotifyUI& msg);
	void OnClickGetVerifyCodeBtn(TNotifyUI& msg);
	void OnClickJoinChannelBtn(TNotifyUI &msg);
	void OnVideoCheckBoxChanged(TNotifyUI &msg);
	void OnObserverCheckBoxChanged(TNotifyUI &msg);
	void OnClickSettingBtn(TNotifyUI &msg);

private:
	void DoClickJoinChannelBtn();
	void GetToken(std::string strPhone, std::string strVerifyCode);
	void GetCode(std::string strPhone);
	int CheckName(CDuiString strName);
	void SaveRoomInfo();
	void InitText();
	void InitCtrl();

private:
	bool m_bRelease;
	bool m_bFromMain;
	UpHttp m_clGetCode;
	UpTimer m_clGetCodeTimer;
	UpHttp m_clGetToken;
	int m_iGetCodeTime;
};

class HttpRequestGetCode : public IUpHttpRequest
{
public:
	void OnHttpRequestSuccess(const char* headers, int headerLen, const char* body, int bodyLen);
	void OnHttpRequestFaild(int error);
};

class HttpRequestGetToken : public IUpHttpRequest
{
public:
	void OnHttpRequestSuccess(const char* headers, int headerLen, const char* body, int bodyLen);
	void OnHttpRequestFaild(int error);
};

#endif