#ifndef RONG_RTC_ENGINE_DEV
#define RONG_RTC_ENGINE_DEV
#include "RongRTCEngine.h"

class RongRTCEventListenerDev
{
public:
	/*
	����: ֪ͨ�û����ܺͷ���ͳ����Ϣ
	����: out_statistics_data_jsonΪͳ����Ϣjson��
	*/
	virtual void NotifyStatisticsData(const char* out_statistics_data_json) = 0;
};

class RONG_RTC_API RongRTCEngineDev
{
public:
	static int InitDev(RongRTCEventListenerDev* in_listener);
};

#endif

