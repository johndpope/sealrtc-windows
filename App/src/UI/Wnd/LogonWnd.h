#ifndef LOGONWND_H
#define LOGONWND_H
#include <string>
#include "UpHttp.h"
#include "UpTimer.h"


struct ServerConfig
{
	std::wstring strName;
	std::string strCmp;
	std::string strCmpTls;
	std::string strSniffer;
	std::string strSnifferTls;
	std::string strToken;
	std::string strJKS;
	std::string strDER;
	std::string strCRT;
	std::string strAppKey;
	//
	std::string strNav;
};

enum NetWorkType
{
	NETWORKTYPE_QUIC = 0,
	NETWORKTYPE_TCP = 1
};

struct ServerConfigItem
{
	std::wstring strName;
	std::map<NetWorkType, ServerConfig*> mapConfig;
};

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
	void OnClickJoinChannelBtn(TNotifyUI &msg);
	void OnClickNetworkListItem(TNotifyUI &msg);
	void OnVideoCheckBoxChanged(TNotifyUI &msg);
	void OnObserverCheckBoxChanged(TNotifyUI &msg);
	void OnClickSettingBtn(TNotifyUI &msg);

	void OnClickSettingOKBtn(TNotifyUI &msg);
	void OnClickSettingEmptyBtn(TNotifyUI &msg);
	void OnClickSettingCancelBtn(TNotifyUI &msg);
	void OnClickShowSettingLayout(TNotifyUI &msg);

private:
	void JoinChannel();
	void SetJoinChannelEnabled(bool bEnable);
	void SetNetworkSelectLayoutVisible(bool bVisible);
	bool GetToken(std::wstring strNetworkName);
	bool SetToken(std::string strToken);
	bool GetCrt(std::wstring strNetworkName);
	void GetConfigList();
	bool InitRtcEngine(std::string strCrt);
	void InitServerConfig(std::string strJson);
	void ClearServerConfig();
	ServerConfig* GetServerConfig(std::wstring strName);
	void StopInit();
	int CheckName(CDuiString strName);
	std::wstring PaserName(std::string strJson);

	void ParseJsonStr(std::string strJson);

	void InitText();
private:

	bool m_bFromMain;
	bool m_bGetTokenSuccess;
	bool m_bGetCrtSuccess;
	bool m_bGetConfigListSuccess;
	UpHttp m_clGetToken;
	UpHttp m_clGetServerConfig;
	UpHttp m_clGetCrt;
	UpTimer m_clGetConfigListTimer;
	UpTimer m_clGetTokenTimer;
	UpTimer m_clGetCrtTimer;
	std::wstring m_strNetworkName;
	CDuiString m_strChannelId;

	bool m_bQuic;
	std::vector<ServerConfigItem *> m_vectorServerConfigList;
};

class HttpRequestGetToken : public IUpHttpRequest
{
public:
	void OnHttpRequestSuccess(const char* headers, int headerLen, const char* body, int bodyLen);
	void OnHttpRequestFaild(int error);
};

class HttpRequestGetServerConfig : public IUpHttpRequest
{
public:
	void OnHttpRequestSuccess(const char* headers, int headerLen, const char* body, int bodyLen);
	void OnHttpRequestFaild(int error);
};

class HttpRequestGetCrt : public IUpHttpRequest
{
public:
	void OnHttpRequestSuccess(const char* headers, int headerLen, const char* body, int bodyLen);
	void OnHttpRequestFaild(int error);
};


#endif