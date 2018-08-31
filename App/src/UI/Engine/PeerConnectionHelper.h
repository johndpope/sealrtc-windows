#ifndef PC_HELPER_H
#define PC_HELPER_H
#include "RongRTCEngine.h"
#include <string>

class PeerConnectionHelper : public RongRTCEngineObserver
{
public:
	PeerConnectionHelper(void);
	~PeerConnectionHelper(void);

	static void CreatePeerConnectionHelper();
	static void DestoryPeerConnectionHelper();
	static PeerConnectionHelper* GetInstance();


	virtual void OnJoinChannelOK(UserRoleType enumUserRoleType);
	virtual void OnJoinChannelFailed(JoinChannelFailedType enumJoinChannelFailedType);
	virtual void OnLeaveChannelOK();
	virtual void OnDisconnectWhitChannel();
	virtual IVideoRender* OnStartLocalAudioVideoOK(UserMediaType enumUserMediaType);
	virtual void OnNotifyLocalAudioVideoStop();
	virtual IVideoRender* OnNotifyUserVideoCreated(const char* strUserID, UserMediaType enumUserMediaType = UserMediaType_None);
	virtual void OnNotifyUserJoinChannel(const char* strUserID, const char* strName, UserRoleType enumUserRoleType, UserMediaType enumUserMediaType = UserMediaType_None);
	virtual void OnNotifyUserLeaveChannel(const char* strUserID, bool bIsObserver = false);
	virtual void OnNotifyUserVideoDestroyed(const char* strUserID);
	virtual void OnUserMediaChanged(const char* strUserID, UserMediaType enumUserMediaType);
	virtual void OnUserOpenOrCloseScreenShare(const char* strUserID, bool bOpen);
	virtual void OnUserCreateWB(const char* strUserID);
	virtual void OnGetEWBServiceUrl(const char* strUrl);
	virtual void OnQueryEWBServiceUrl(const char* strUrl);
	virtual void OnGetSendRecvStatisticsData(unsigned __int64 iSendLostPacketPercent, unsigned __int64 iReceiveLostPacketPercent);
	virtual void OnNotifyUserAudioLevel(const char* strJson);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual void OnObserverRequestBecomeNormalUser(bool bSuccessed);
	virtual void OnNotifyObserverRequestBecomeNormalUser(const char* strUserID);
	virtual void OnHostDoObserverRequestBecomeNormalUser(const char* strUserId, bool bAccept, bool bSuccessed);
	virtual void OnNotifyHostDoObserverRequestBecomeNormalUser(const char* strUserID, OperationType enumEngineOperationType);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual void OnHostRequestUpgradeObserverToNormalUser(const char* strUserId, bool bSuccessed);
	virtual void OnNotifyHostRequestUpgradeObserverToNormalUser(const char* strHostID);
	virtual void OnObserverDoHostRequestUpgradeObserverToNormalUser(const char* strHostId, bool bAccpet, bool bSuccessed);
	virtual void OnNotifyObserverDoHostRequestUpgradeObserverToNormalUser(const char* strUserId, bool bAccpet);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual void OnHostRequestDegradeNormalUserToObserver(const char* strUserID, bool bSuccessed);
	virtual void OnNotifyHostRequestDegradeNormalUserToObserver(const char* strHostID);
	virtual void OnNoramlUserDoHostRequestDegradeNormalUserToObserver(const char* strHostID, bool bAccpet, bool bSuccessed);
	virtual void OnNotifyNoramlUserDoHostRequestDegradeNormalUserToObserver(const char* strUserId, bool bAccpet);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual void OnHostRequestControlUserDevice(const char* strUserId, ControlAction enumControlAction, ControlMediaType enumControlMediaType, bool bSuccessed);
	virtual void OnNotifyHostRequestControlUserDevice(const char* strHostId, ControlAction enumControlAction, ControlMediaType enumControlMediaType);
	virtual void OnUserDoHostRequestControlUserDevice(const char* strHostId, ControlAction enumControlAction, ControlMediaType enumControlMediaType, bool bAccept, bool bSuccessed, UserMediaType enumMediaType);
	virtual void OnNotifyUserDoHostRequestControlUserDevice(const char* strUserId, ControlAction enumControlAction, ControlMediaType enumControlMediaType, bool bAccpet, UserMediaType enumMediaType);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual void OnHostReomveMemberOrObserver(const char* strUserId, bool bSuccessed);
	virtual void OnNotifyHostReomveMemberOrObserver(const char* strUserID);
	virtual void OnGetHostPower(bool bSuccessed);
	virtual void OnNotifyGetHostPower(const char* strUserID);
	virtual void OnGetInviteUrl(const char* strUrl, bool bSuccessed);

public:
	static void UpdateMediaType(int iOperationMediaType, int iDoType);
private:

	static PeerConnectionHelper* m_pPeerConnectionHelper;

public:
	static std::string m_strUserId;
	static std::string m_strSDKVersion;
	static std::string m_strToken;
	static int m_iMediaType;
	static int m_iRoleType;
	static int m_iResolutionType;

	static bool m_bOpenObserver;
	static bool m_bOpenOnlyAudio;
	static bool m_bOpenBeauty;
	static bool m_bOpenTinyStream;
};
#endif