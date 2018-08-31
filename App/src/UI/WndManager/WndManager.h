#ifndef __WNDMANAGER_H__
#define __WNDMANAGER_H__

#pragma once
#include <map>
#include "Up_type.h"
#include "TransparentWnd.h"
#include "UpTimer.h"

namespace DuiLib 
{
	class CTransparentWnd;

	enum
	{
		ID_UNKNOWN				= 0,
		ID_WND_DIALOG			= 1,
		ID_WND_LOGON			= 2,
		ID_WND_MAIN				= 3,
		ID_WND_MAIN_TIP		= 4,
		ID_WND_STATE_INFO	= 5,
		ID_WND_WB					= 6,
		ID_END
	};

	const static int		HITTEST_SIZE = 5;
	static CUString	SkinFolder = _T("skin\\");

//////////////////////////////////////////////
//Cmd that passing from one wnd to another wnd
//////////////////////////////////////////////
	struct CmdParam
	{
		LPVOID	 pCmdParam;
		CUString strCmdParam;

		CmdParam(LPCTSTR pCmd = NULL,LPVOID pParam = NULL) :
		pCmdParam(pParam),
		strCmdParam(pCmd)
		{}

		CmdParam(const CmdParam& mp) : 
		pCmdParam(mp.pCmdParam),
		strCmdParam(mp.strCmdParam)
		{}
	};

	class IHandle
	{
	public:
		virtual void OnCloseAMenuHandle(const CmdParam& aCmdParam) = 0;
		virtual void OnMenuHandle(const CmdParam& aCmdParam) = 0;
		virtual void OnEmoticonsHandle(const CmdParam& aCmdParam) = 0;
		virtual void OnDynamicSubMenu(CContainerUI* parentElement) = 0;
	};
	
	class CBaseWnd : public WindowImplBase,public IHandle
	{
	public:
		CBaseWnd();
		virtual ~CBaseWnd();

		//WindowImplBase
		virtual void OnFinalMessage(HWND hWnd);
		virtual LPCTSTR GetWindowClassName( void ) const;
		virtual UINT GetClassStyle() const;
		

		virtual LRESULT HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam );
		virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
		virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		virtual LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		virtual LRESULT OnMoving(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		virtual LRESULT OnMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		virtual LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		virtual LRESULT OnExitSizing(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		virtual LRESULT OnClose( UINT , WPARAM , LPARAM , BOOL& bHandled );

		virtual void Notify(TNotifyUI& msg);
		virtual void OnWinInit(TNotifyUI& msg);

		//IHandle
		virtual void OnCloseAMenuHandle(const CmdParam& aCmdParam){};
		virtual void OnMenuHandle(const CmdParam& aCmdParam){};
		virtual void OnEmoticonsHandle(const CmdParam& aCmdParam){};
		virtual void OnDynamicSubMenu(CContainerUI* parentElement){};

		//CAppWnd
        virtual LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		virtual LRESULT OnMouseEnter(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		virtual LRESULT OnMouseLeave(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		virtual LRESULT OnHotKey(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

		//构造窗体传递参数
		virtual void PreCreateWindowInit(const CStdStringPtrMap& aInitData){};
		virtual void PostCreateWindowInit(const CStdStringPtrMap& aInitData){};

		virtual CControlUI* CreateControl(LPCTSTR pstrClass);
		virtual void SetWndIcon();
		
		HWND CreateWnd(HWND hwndParent, LPCTSTR pstrName, DWORD dwStyle, DWORD dwExStyle, const RECT rc, CUString maskFile = _T("common\\shade.png"), RECT* pRectMargin = NULL, RECT* pRect9GridMagrin = NULL);

		void SetWndCallbackNULL();

		static void	 SetSkinFolder(LPCTSTR pstrSkinFolder);
		CUString GetSkinFolder();
		void	 SetSkinFile(LPCTSTR pstrSkinFile) { m_strCurSkinFile = pstrSkinFile; }
		CUString GetSkinFile();
		virtual void SetSkinColor(){}

		virtual void OnLanguageChanged(){m_PaintManager.ReloadText();}

		void SetFixSize(BOOL bFixed) { m_bFixSize = bFixed; }
		BOOL GetFixSize() const { return m_bFixSize; }

		void SetWndID(UINT nWndID) { m_nWndID = nWndID; }
		UINT GetWndID() const { return m_nWndID; }

		void	SetWndToken(LPCTSTR pstrToken) { m_strWndToken = pstrToken; }
		LPCTSTR GetWndToken() const { return m_strWndToken; }

		void CenterWindow();
		void ShowMaskWindow(bool isShow);

		HWND GetParentHwnd() const { return m_parentHwnd; }
		
		void SetMaskWindow(LPCTSTR fileName);
		void Destroy();

		virtual void ShowWnd(bool bShow);

		bool ShowInTaskbar(bool bShow);

	public:
		virtual UINT ShowModal();
		bool IsModal() { return m_isModal; }
		bool IsOpenFileDialog() const { return m_isOpenFileDialog; }
		HWND GetTransparentWnd();

	public:
		CUString TrimTextByCtrlSize(CUString fulltext, int ctrlWidth, int ctrlHeight, int maxLength = 300);
		
	protected:
		CTransparentWnd* m_maskWnd;
		UINT	 m_nWndID;
		CUString m_strWndToken;
		CUString m_strCurSkinFile;
		CUString m_maskFile;
		BOOL	 m_bFixSize;
		HWND  m_parentHwnd;

	private:
		bool m_isModal;
		bool m_isOpenFileDialog;
	};

	typedef CBaseWnd* (CALLBACK* CAppWndCtor)();

	struct CAppWndFactory
	{
		CAppWndCtor	pCtor;
		CAppWndFactory(CAppWndCtor pTheCtor) :
		pCtor(pTheCtor)
		{}
	};

	typedef std::map<int, CAppWndFactory*> AllFactMap;

	class CWndManager
	{
	public:
		virtual ~CWndManager();
	protected:
		CWndManager(){};
		

	public:
		static CWndManager* GetWndManager();
		BOOL IsExsitWnd(int nWndID,LPCTSTR pstrWndToken);
		BOOL IsExsitFact(int nWndID);
		CBaseWnd* FindWnd(int nWndID,LPCTSTR pstrWndToken,BOOL bCreate = TRUE);
		CBaseWnd* GetWnd(int index);
		void RemoveWnd(int nWndID,LPCTSTR pstrWndToken);
		bool IsHasModal(HWND parentHwnd);
		void DestroyAllWnd();
		static void RegisterWnd(int nWndID,CAppWndFactory* pFact);
		int  GetCount() { return m_AllWnd.size(); }
		void CloseAllWindow(void);
		void CloseSubWindows(HWND parentHwnd);

	private:
		typedef std::map<CUString, CBaseWnd*> AllWndMap;
		AllWndMap			m_AllWnd;
		static AllFactMap	m_AllFact;
		static CWndManager* m_wm;
	};

}// namespace DuiLib

#endif // __WNDMANAGER_H__
