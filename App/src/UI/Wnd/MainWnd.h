#ifndef MAINWND_H
#define MAINWND_H
#include "StdAfx.h"
#include <list>
#include "PeerConnectionHelper.h"
#include "RongRTCEngineVideoRender.h"
#include "UpTimer.h"

class VideoRender : public IVideoRender
{

public:

	VideoRender(CVideoUI* pVideoCtrl, std::string strStreamId, std::wstring wstrShortName, int iRgb, int iMediaType, int iRoleType) :
		m_pVideoCtrl(pVideoCtrl),
		m_strStreamId(strStreamId),
		m_bOutputFrame(true),
		m_wstrShortName(wstrShortName),
		m_iRgb(iRgb),
		m_iMediaType(iMediaType),
		m_iRoleType(iRoleType)
	{

	}
	VideoRender() {}

	~VideoRender()
	{
		m_pVideoCtrl = NULL;
	}

	void OnFrame(BITMAPINFO* pBitmapInfo, unsigned char* pImage)
	{
		if (m_bOutputFrame)
		{
			m_clockFrame.Lock();
			if (m_pVideoCtrl && pBitmapInfo && pImage)
			{
				m_pVideoCtrl->SetBitmap(pBitmapInfo, pImage);
			}
			m_clockFrame.Unlock();
		}
	}

	void SetBitmap(BITMAPINFO* pBitmapInfo, unsigned char* pImage)
	{
		m_clockFrame.Lock();
		m_pVideoCtrl->SetBitmap(pBitmapInfo, pImage);
		m_clockFrame.Unlock();
	}
	void GetBitmap(BITMAPINFO* pBitmapInfo, unsigned char* pImage)
	{
		m_clockFrame.Lock();
		m_pVideoCtrl->GetBitmap(&pBitmapInfo, &pImage);
		m_clockFrame.Unlock();
	}

	CVideoUI* GetVideoUI() { return m_pVideoCtrl; }
	void SetVideoUI(CVideoUI* pVideoCtrl)
	{ 
		m_clockFrame.Lock();
		m_pVideoCtrl = pVideoCtrl; 
		m_clockFrame.Unlock();
	}
	std::string GetStreamId() { return m_strStreamId; }
	void SetOutputFrame(bool bOutputFrame) { m_bOutputFrame = bOutputFrame; }
	void SetShortName(std::wstring wstrShortName) { m_wstrShortName = wstrShortName; }
	std::wstring GetShortName() { return m_wstrShortName; }
	void SetRgb(int iRgb) { m_iRgb = iRgb; }
	int GetRgb() { return m_iRgb; }
	void SetMediaType(int iMediaType) { m_iMediaType = iMediaType; }
	int GetMediaType() { return m_iMediaType; }
	void SetRoleType(int iRoleType) { m_iRoleType = iRoleType; }
	int GetRoleType() { return m_iRoleType; }

	void ExchangeVideo(VideoRender* pVideoRender)
	{
		m_clockFrame.Lock();
		CVideoUI* pVideoUI = pVideoRender->GetVideoUI();
		pVideoRender->SetVideoUI(m_pVideoCtrl);
		m_pVideoCtrl = pVideoUI;
		m_clockFrame.Unlock();
	}
private:
	CVideoUI* m_pVideoCtrl;
	std::string m_strStreamId;
	bool m_bOutputFrame;
	UpLock m_clockFrame;
	std::wstring m_wstrShortName;
	int m_iRgb;
	int m_iMediaType;
	int m_iRoleType;
};

class MainWnd : public CBaseWnd, public TimerObserver
{
public:
	struct structTipInfo
	{
		// 被动  1，观察者收到主持人邀请观察者发言，2，主持人收到观察者主动要求发言（呼叫画面），3，用户收到主持人邀请打开用户音频，4，用户收到主持人邀请打开用户视频
		//主动 11，观察者请求发言（不弹框），12，用户获取主持人权限，13，主持人邀请打开音频（不弹框），14，主持人邀请打开视频（不弹框），15，主持人邀请关闭音频，16，主持人邀请关闭视频
		//17，主持人邀请观察者发言（不弹框），18，主持人请求降级正常用户为观察者，19，主持人踢人，20，获取邀请链接，21，保存白板提示，22，观察者打开白板，无白板提醒，23，音视频设备异常
		//24，关闭摄像头情况下，打开屏幕共享，25，屏幕共享情况下，关闭摄像头，26，切换屏幕共享和摄像头太频繁，27，观察者请求发言，主持人忙，请稍后再拨
		int m_iDoType;	
		std::string m_strResponsedUserId;
		int m_iHostControlMediaType; // 1是Video，2是Audio
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

public:
	void OnWinInit(TNotifyUI& msg);
	void OnChangeVideoSizeBtn(TNotifyUI& msg);
	void OnClickCloseBtn(TNotifyUI &msg);
	void OnClickHangeupBtn(TNotifyUI &msg);
	void OnClickCameraBtn(TNotifyUI &msg);
	void OnClickMicBtn(TNotifyUI &msg);
	void OnClickLoudSpeakerBtn(TNotifyUI &msg);
	void OnClickWBBtn(TNotifyUI &msg);
	void OnClickInviteBtn(TNotifyUI &msg);
	void OnClickScreenShareBtn(TNotifyUI& msg);
	void OnGetHostPower(TNotifyUI& msg);
	void OnExchangeVideoBtn(TNotifyUI &msg);
	void OnClickUpAndDownBtn(TNotifyUI &msg);
	void OnTabOptionChanged(TNotifyUI &msg);
	void OnEnterMemberListItem(TNotifyUI &msg);
	void OnLeaveMemberListItem(TNotifyUI &msg);
	void OnClickAudioBtnOnMemberList(TNotifyUI &msg);
	void OnClickVideoBtnOnMemberList(TNotifyUI &msg);
	void OnClickUpDownLevelBtnOnMemberList(TNotifyUI &msg);
	void OnClickDeleteMemberBtnOnMemberList(TNotifyUI &msg);
	void OnClickAgreeCall(TNotifyUI &msg);
	void OnClickRefuseCall(TNotifyUI &msg);
	void OnClickRequestSpeak(TNotifyUI &msg);
	void OnClickFullScreenOrRestore(TNotifyUI &msg);
	void OnClickShowOrHideRightFloatLayout(TNotifyUI &msg);
	void OnClickMoreBtn(TNotifyUI &msg);
	void OnClickHDSmoothSwitchBtn(TNotifyUI &msg);

	void ClickTipClose();
	void ClickTipYes();
	void ClickTipNo();
	VideoRender* GetVideoRender(std::string strStreamId);

private:
	void UserJoinChannel(std::string strStreamId, std::string strName, int iUserRoleType, int iMediaType);
	void DeleteVideoRender(std::string strStreamId, int iType = 0); // iType == 1降级，iType == 0退出房间
	void ClearVideoRender();
	void HandleObserverRequestSpeak(std::string strStreamId);
	void ObserverAgreeHostRequestSpeak();
	void NotifyObserverAgreeHostRequestSpeak(std::string strStreamId);
	void NotifyHostDoObserverRequestBecomeNormalUser(std::string strStreamId);
	void HostDoObserverRequestBecomeNormalUser(std::string strStreamId);
	void MemberGetHostPower(std::string strUserId, bool bSuccessed);
	void UpdateWBUrl();
	void DoUserJoinChannel(std::string strStreamId, std::wstring strFullName, int iMediaType, int iRoleType);
	void UpdateUserInfo(std::string strStreamId, int iMediaType, int iRoleType = -1);
	void SetVideoInvalid(std::string strStreamId);
	void OnHostRequestOpenMedia(std::string strHostId, int iMediaType);
	void InitCtrl();
	void InitText();
	virtual void Timeout(UpTimer* timer);
	void SetJoinChannelStatus(std::wstring strText);
	void SetSendRecvLosPacektPercent(int iSendLostPacketsPercent, int iReceiveLostPacketsPercent);
	void SetConnectionTime(unsigned __int64 iTime);
	void ShowConnectionInfo(bool bShow);
	void EnableOperationBtn(bool bRequestSpeakBtn, bool bGetInviteUrlBtn, bool bRequestHostBtn, bool bScreenShareBtn, bool bCameraBtn, bool bMicBtn, bool bHDSmoothBtn = true);
	std::wstring RemoveMemberFromMeetingList(std::string strStreamId);
	void ShowDoBtnOnMeetingList(CListContainerElementUI* pListItem, bool bShow);
	void ShowCallRequest(bool bShow, std::string strStreamId);
	void AddMessageTipToMessageList(std::wstring wstrText);
	std::wstring PaserTipInfoToText(structTipInfo* pStructTipInfo);
	void PaserTipInfoToDo(structTipInfo* pStructTipInfo, bool bAccept);
	void ShowTip(bool bShow, std::wstring strText, std::wstring strLeftText = L"是", std::wstring strRightText = L"否"); 
	bool TipInfoHaveExist(int iDoType);
	void SetFullScreenAndRestoreBtn(bool bToFullScreen);
	CDuiString CreateShortName(std::wstring strFullName);
	int CreateRandomRgb(int iRoleType);
	std::wstring CreateNotificationText(std::wstring wstrName, std::wstring wstrText);
	std::wstring GetUserNameFromMemberList(std::string strUserId);
	void GetShortNameAndRgb(std::string strUserId, std::wstring& wstrShortName, int& iRgb);
	void UpdateAudioLevel(std::string strJson);
	void UpdateMicAndCameraStatus(int iMediaType);

private:
	std::list<VideoRender*> m_lsVideoRender;
	std::list<VideoRender*> m_lsNeedDeleteVideoRender;
	bool m_bIsExistApp;
	CDuiString m_strChannelId;
	CDuiString m_strName;
	unsigned __int64 m_iConnectionSecond;
	UpTimer m_clConnectionTimer;
	std::list<structTipInfo*> m_listTipInfo;
	bool m_bHaveExistWB;
	CDuiString m_strWBUrl;
	UpTimer m_clCallRequestTimer;
	bool m_bCameraExist;
	bool m_bFirstJionChannel;
	RECT m_rcOldWnd;
	bool m_bFullScreen;
	int m_iNowResolutionType;
};
#endif