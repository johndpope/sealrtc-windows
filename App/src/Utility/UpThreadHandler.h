#pragma once

#include "UpTimer.h"

class __declspec(novtable) Runable
{
public:
	 virtual void Run() = 0;
};

class UpThreadHandler: public TimerObserver
{
public:
	UpThreadHandler(void);
	virtual ~UpThreadHandler(void);
public:
	void Start(std::string name);
	void Stop();
	void Post(Runable* runable, unsigned int elapsetime = 0);
	bool IsWork(){ return m_iswork; }
	void Threadwork();
	void DealRemain();
	virtual void Timeout(UpTimer* timer) ;
	void SetStatus(bool isWork){ m_iswork = isWork; }

public:
	enum TThreadPriority
	{
		EThreadLow,
		EThreadNormal,
		EThreadHigh,
		EThreadCritical,
	};
private:
	void StratThread(std::string name);
public:
	void SetPriorityThread(TThreadPriority priority);

private:
	void*   m_thread;
	DWORD   m_threadId;
	Runable*  m_runable;
	unsigned  m_elapsetime;
	UpTimer*  m_timer;
	bool	m_iswork;
};

