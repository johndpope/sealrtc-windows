#ifndef WB_WND_H
#define WB_WND_H

class WbWnd : public CBaseWnd
{
public:
	WbWnd(void);
	virtual ~WbWnd(void);

    DECL_NOTIFY_MSG()
    DECL_APPWND_CONTROL()

    virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	virtual void OnFinalMessage(HWND hWnd);	
	virtual LPCTSTR GetWindowClassName() const;
	virtual void PreCreateWindowInit(const CStdStringPtrMap& aInitData);

public:
	void OnWinInit(TNotifyUI& msg);
	void UpdateUrl(CDuiString strUrl);

private:
	void InitControl();
	void InitText();
private:
	CDuiString m_strUrl;
};

#endif