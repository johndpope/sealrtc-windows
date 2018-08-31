#pragma once
#include <map>
#include "UpLock.h"
#include "Base.h"

class UpTimer;
class TimerObserver
{
public:
	virtual void Timeout(UpTimer* timer) = 0;
};


class UpTimer: public CBase
{
public:
	UpTimer(void);
	~UpTimer(void);
public:
	void Start(unsigned int  elapsetime, TimerObserver* observer);
	void Stop();
	void Timeout();

	void SetData(void* data){ this->data = data;}
	void* GetData(){return data;} 
	bool GetIsWork(){ return m_bWork; }
public:
	static void TriggerTimeOut(__int64 idEvent);

private:
	static std::map<__int64, UpTimer*> m_workmap;
	static UpLock m_lock;

	unsigned int m_hEvent;
	TimerObserver* m_observer;
	void* data;
	bool m_bWork;
};

