#ifndef RONG_RTC_ENGINE_DEV
#define RONG_RTC_ENGINE_DEV
#include "RongRTCEngine.h"

class RongRTCEventListenerDev
{
public:
	/*
	功能: 通知用户接受和发送统计信息
	参数: out_statistics_data_json为统计信息json串
	*/
	virtual void NotifyStatisticsData(const char* out_statistics_data_json) = 0;
};

class RONG_RTC_API RongRTCEngineDev
{
public:
	static int InitDev(RongRTCEventListenerDev* in_listener);
};

#endif

