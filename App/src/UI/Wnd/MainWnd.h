#ifndef MAINWND_H
#define MAINWND_H
#include "StdAfx.h"
#include <list>
#include <map>
#include "PeerConnectionHelper.h"
#include "UpTimer.h"
#include "..\jsoncpp\include\json\json.h"

class MainWnd : public CBaseWnd, public TimerObserver
{
private:
	struct StreamInfo
	{
		StreamInfo() :m_iMediaType(0), m_bSimulcast(false)
		{
			m_iState[0] = 1;
			m_iState[1] = 1;
		}
		std::string m_strUserId;
		std::string m_strTag;
		std::string m_strStreamId;
		int m_iMediaType; // 1Audio 2Video 3AV
		int m_iState[2]; // m_iState[0] Audio m_iState[1] Video
		bool m_bSimulcast;
	};
	struct UserInfo
	{
		std::string m_strUserName;
		long long m_iJoinTime;
		int m_iJoinMode;
	};

public:
	MainWnd(void);
	virtual ~MainWnd(void);

    DECL_NOTIFY_MSG()
    DECL_APPWND_CONTROL()

    virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual void OnFinalMessage(HWND hWnd);	
	virtual LPCTSTR GetWindowClassName() const;
	virtual void PreCreateWindowInit(const CStdStringPtrMap& aInitData);

private:
	void InitCtrl();
	void InitText();
	virtual void Timeout(UpTimer* timer);

public:
	void OnWinInit(TNotifyUI& msg);
	void OnClickShowInfoBtn(TNotifyUI& msg);
	void OnClickCloseBtn(TNotifyUI &msg);
	void OnClickMicBtn(TNotifyUI &msg);
	void OnClickScreenShareBtn(TNotifyUI &msg);
	void OnClickCameraBtn(TNotifyUI &msg);
	void OnClickSpeakerBtn(TNotifyUI &msg);
	void OnClickHangeupBtn(TNotifyUI &msg);
	void OnTabOptionChanged(TNotifyUI &msg);
	void OnExchangeBtn(TNotifyUI &msg);
	void OnClickUpAndDownBtn(TNotifyUI &msg);

private:
	void AddOrUpdateVideoCtrl(std::string strStreams);
	void AddOrUpdateVideoCtrl(StreamInfo* pStreamInfo);
	void AddOrUpdateUserItem(std::string strUserId);
	void AddOrUpdateUserInfoFromGet(std::string strUserInfo);
	void AddOrUpdateUserInfoFromNotify(std::string strUserInfo);
	void RemoveVideoCtrl(std::string strStreams);
	void RemoveVideoCtrl(StreamInfo* pStreamInfo);
	void RemoveVideoCtrlAndUserInfo(std::string strUserId);
	void RenderVideoData(std::string strStreamId, BITMAPINFO* pBitmapInfo, unsigned char* pData);
	std::string CreateLocalStreamJson(int iPublishStreamType);
	std::string CreateSubscribeStreamJson();
	void SetLocalUserInfo();
	void SetConnectionTime(unsigned __int64 iTime);
	void AddMessageTipToMessageList(std::wstring wstrText);
	std::wstring CreateNotificationText(std::wstring wstrName, std::wstring wstrText);
	static std::string GetUserIdFromStreamId(const std::string &stream_id);

private:
	std::map<std::string, UserInfo*> m_mapUsersInfo;
	std::list<StreamInfo*> m_listStreamsInfo;
	bool m_bQuiteApp;
	bool m_bFirstJionChannel;
	bool m_bMuteMic;
	bool m_bMuteCamera;
	CVideoUI* m_pLocalVideoCtrl;
	CHorizontalLayoutUI* m_pRemoteVideoLayout;
	CListUI* m_pUserList;
	unsigned long long m_iConnectionSecond;
	UpTimer m_clConnectionTimer;
};
#endif