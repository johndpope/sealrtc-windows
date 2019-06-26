#ifndef PC_HELPER_H
#define PC_HELPER_H
#include <string>
#include <Windows.h>
#include "RongRTCEngine.h"
#include "RongRTCEngineDev.h"
class PeerConnectionHelper : public RongRTCEventListener, public RongRTCEventListenerDev, public RongRTCCallBackOne, public RongRTCCallBackTwo
{
public:
	static PeerConnectionHelper* GetInstance();

public:
	PeerConnectionHelper(void);
	~PeerConnectionHelper(void);

	virtual void NotifyDisconnectWithRoom();
	virtual void NotifyConnectionStatusChanged(RongRTCConnectionStatus out_status);
	virtual void NotifyReceivedMessage(const char* out_object_name, const char* out_content);
	virtual void NotifyRemoteUserJoinedRoom(const char* out_user_id);
	virtual void NotifyRemoteUserLeftRoom(const char* out_user_id);
	virtual void NotifyRemoteUserOffLine(const char* out_user_id);
	virtual void NotifyRemoteUserPublishedStreams(const char* out_streams_json);
	virtual void NotifyRemoteUserUnPublishedStreams(const char* out_streams_json);
	virtual void NotifyRemoteUserChangedStreamsState(const char* out_streams_json);
	virtual void NotifyRenderVideoData(const char* out_stream_id, RongRTCVideoFrame out_video_frame);
	virtual void NotifyPreprocessingDefaultVideoData(RongRTCVideoFrame out_video_frame);
	virtual void NotifyStatisticsData(const char* out_statistics_data_json);
	void OnCallBack(int code);
	void OnCallBack(int code, const char* ret);
	
public:

	static std::string m_strSDKVersion;
	static std::string m_strUserId;
	static std::string m_strUserName;
	static std::string m_strChannelId;
	static std::string m_strToken;
	static std::string m_strPhone;
	static int m_iResourceType;
	static int m_iResolutionType;
	static bool m_bOpenBeauty;
	static bool m_bOpenTinyStream;
	static std::string m_strDialogId;
	static bool m_bConnected;
	static HWND m_hMainWnd;
};

class GetRoomAttributeValueCallBack : public RongRTCCallBackTwo
{
public:
	static GetRoomAttributeValueCallBack* GetInstance();
	virtual void OnCallBack(int code, const char* ret);

private:
	static GetRoomAttributeValueCallBack* m_pGetRoomAttributeValueCallBack;
};

class JoinRoomCallBack : public RongRTCCallBackTwo
{
public:
	static JoinRoomCallBack* GetInstance();
	virtual void OnCallBack(int code, const char* ret);

private:
	static JoinRoomCallBack* m_pJoinRoomCallBack;
};

class LeaveRoomCallBack : public RongRTCCallBackOne
{
public:
	static LeaveRoomCallBack* GetInstance();
	virtual void OnCallBack(int code);

private:
	static LeaveRoomCallBack* m_pLeaveRoomCallBack;
};
#endif