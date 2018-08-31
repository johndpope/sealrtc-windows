#pragma once

#include <vector>
#include <xstring>

class MsgItem
{
public:
	MsgItem(){}
	MsgItem(bool ok){ SetIsOK(ok);}
	MsgItem(__int64 id){IdArray.push_back(id);}
	MsgItem(std::string& buf){bufArray.push_back(buf);}
	MsgItem(void* val){pVal = val;}

public:
	inline __int64 GetId(int index) const;
	inline void PutId(__int64 val) { IdArray.push_back(val); }
	inline int GetIdSize(){return IdArray.size();}
	inline std::string GetBuf(int index) const;
	inline int GetBufSize(){return bufArray.size();}
	inline void PutBuf(const std::string& val) { bufArray.push_back(val); }
	inline void* GetPointer() const { return pVal; }
	inline void SetPointer(void* val) { pVal = val; }
	inline bool IsOK() const { return isOK; }
	inline void SetIsOK(bool val) { isOK = val; }
private:
	bool isOK;
	std::vector<__int64> IdArray;
	std::vector<std::string>  bufArray;
	void* pVal;
};

inline __int64 MsgItem::GetId( int index ) const
{
	if( (int)(IdArray.size()) > index )
	{
		return IdArray[index];
	}
	else
	{
		return -1;
	}
}

inline std::string MsgItem::GetBuf( int index ) const
{
	if( (int)(bufArray.size()) > index )
	{
		return bufArray[index];
	}
	else
	{
		return "";
	}
}

enum BroadcastType 
{
	BDT_CHANGE_SKIN_COLOR = 0,
	BDT_LANGUAGE_CHANGED = 1,
	BDT_JION_CAHNNEL_OK = 2,
	BDT_LEAVE_CAHNNEL_OK = 3,
	BDT_SEND_RECV_LOST_PERCENT_PERCENT = 4,
	BDT_GET_TOKEN_OK = 5,
	BDT_GET_SERVER_CONFIG = 6,
	BDT_GET_CRT_OK = 7,
	BDT_GET_WB_URL = 8,
	BDT_HOST_INVITE_OBSERVER_SPEAK = 9,
	BDT_HOST_TICK_OUT = 10,
	BDT_USER_SCREEN_SHARED = 11,
	BDT_GET_INVITE_URL = 12,
	BDT_QUERY_WB_URL = 13,
	BDT_CREATE_WB = 14,
	BDT_HOST_REQUSET_OPEN_MEDIA = 15,
	BDT_OBSERVER_REQUSET_SPEAK = 16,
	BDT_SEND_RECV_MEDIA_INFO = 17,
	BDT_REMOVE_VIDEO_RENDER = 18,
	BDT_GET_HOST_POWER_SUCCEED = 19,
	BDT_HOST_BUSY = 20,
	BDT_INIT_WB = 21,
	BDT_CLOSE_SCREEN_SHARED = 22,
	BDT_ON_FRAME = 23,
	BDT_USER_MEDIA_CHANGE = 24,
	BDT_START_CAMERA_OK = 25,
	BDT_REMOTE_VIDEO_OK = 26,
	BDT_OBSERVER_AGREE_HOST_REQUEST_SPEAK = 27,
	BDT_NOTIFY_OBSERVER_AGREE_HOST_REQUEST_SPEAK = 28,
	BDT_NOTIFY_HOST_DO_OBSERVER_REQUEST_BECOME_NORMAL_USER = 29,
	BDT_HOST_DO_OBSERVER_REQUEST_BECOME_NORMAL_USER = 30,
	BDT_USER_JOIN_CHANNEL = 31,
	BDT_USER_LEAVE_CHANNEL = 32,
	BDT_NORAML_USER_DO_HOST_REQUEST_DEGRADE_NORMAL_USER_TO_OBSERVER = 33,
	BDT_NOTIFY_NORAML_USER_DO_HOST_REQUEST_DEGRADE_NORMAL_USER_TO_OBSERVER = 34,
	BDT_STOP_CAMERA_OK = 35,
	BDT_NOTIFY_HOST_REQUEST_DEGRADE_NORMAL_USER_TO_OBSERVER = 36,
	BDT_NOTIFY_AUDIO_LEVEL = 37,
};
