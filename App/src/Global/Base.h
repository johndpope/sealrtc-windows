#pragma once
class CBase
{
public:
	CBase(void);
	virtual ~CBase(void);

	void *operator new[ ](size_t size);
	void operator delete[ ](void *p);

	void* operator new(size_t size);
	void operator delete(void *p);
};

