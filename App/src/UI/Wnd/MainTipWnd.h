#ifndef MAIN_TIP_WND_H
#define MAIN_TIP_WND_H

class MainTipWnd : public CBaseWnd
{
public:
	MainTipWnd(void);
	virtual ~MainTipWnd(void);

    DECL_NOTIFY_MSG()
    DECL_APPWND_CONTROL()

    virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	virtual void OnFinalMessage(HWND hWnd);	
	virtual LPCTSTR GetWindowClassName() const;
	virtual void PreCreateWindowInit(const CStdStringPtrMap& aInitData);
	
public:
	void OnWinInit(TNotifyUI& msg);
	void OnClickTipClose(TNotifyUI &msg);
	void OnClickTipYes(TNotifyUI &msg);
	void OnClickTipNo(TNotifyUI &msg);

	void ShowTip(std::wstring strText, std::wstring strLeftText = L"ÊÇ", std::wstring strRightText = L"·ñ");
};

#endif