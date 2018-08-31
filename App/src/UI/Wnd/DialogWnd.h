#ifndef APP_DIALOGWND_H
#define APP_DIALOGWND_H

class CDialogWnd : public CBaseWnd
{
public:
	enum TDialogResult
	{
		EDialogClose = 0,
		EDialogLeft = 1,
		EDialogRigtht= 2,
		EDialogNotDo = 3,
	};

	enum TDialogFlag 
	{
		EDialogOneBtn = 0,
		EDialogTWoBtn = 1
	};

	virtual ~CDialogWnd(void);
	static TDialogResult ShowDialog(HWND hwnd, std::wstring titile, std::wstring leftBtn, std::wstring rightBtn, std::wstring info, TDialogFlag flag = EDialogTWoBtn);
	static TDialogResult ShowSingleDialog(HWND hwnd, std::wstring titile, std::wstring leftBtn, std::wstring rightBtn, std::wstring info, TDialogFlag flag = EDialogTWoBtn);
	static void ShowSingleWnd(HWND hwnd, std::wstring titile, std::wstring leftBtn, std::wstring rightBtn, std::wstring info, TDialogFlag flag = EDialogTWoBtn);

	static bool IsShowing(){return m_isShowing;}

	bool IsShow() const { return m_isShow; }
	void SetShow(bool val) { m_isShow = val; }
    DECL_NOTIFY_MSG()
    DECL_APPWND_CONTROL()
protected:
    virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void OnFinalMessage(HWND hWnd);	
	virtual LPCTSTR GetWindowClassName() const;

	void OnClickOK(TNotifyUI& msg);
	void OnClickCancel(TNotifyUI& msg);

private:
	void InitInfo(std::wstring& titile, std::wstring& leftBtn, std::wstring& rightBtn, std::wstring& info, TDialogFlag& flag );

	virtual void OnWinInit( TNotifyUI& msg );
	void SetSkinColor();
	CDialogWnd(void);

private:

	static TDialogResult m_result;
	static bool m_isShowing;
	std::wstring m_title;
	std::wstring m_left;
	std::wstring m_right;
	std::wstring m_info;
	TDialogFlag m_flag;
	bool m_isShow;

};
#endif