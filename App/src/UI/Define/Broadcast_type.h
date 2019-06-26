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
	BDT_CHANGE_SKIN_COLOR,
	BDT_LANGUAGE_CHANGED,
	BDT_SEND_RECV_MEDIA_INFO,
	BDT_SEND_RECV_INFO,
	BDT_GET_CODE_OK,
	BDT_GET_CODE_FAILED,
	BDT_GET_TOKEN_OK,
	BDT_GET_TOKEN_FAILED,
	BDT_CONNECT_SIGNAL_SERVER,
	BDT_JOIN_ROOM_OK,
	BDT_LEAVE_ROOM_OK,
	BDT_REMOTE_USER_JOIN,
	BDT_REMOTE_USER_LEFT,
	BDT_REMOTE_STREAM_PUBLISH,
	BDT_REMOTE_STREAM_UNPUBLISH,
	BDT_REMOTE_STREAM_STATE_CHANGED,
	BDT_GET_ROOM_ATTRIBUTE_VALUE,
	BDT_NOTIFY_SET_ROOM_ATTRIBUTE_VALUE,
};
