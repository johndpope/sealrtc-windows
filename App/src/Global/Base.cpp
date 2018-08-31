#include "Base.h"
#include <malloc.h>
#include <memory>

CBase::CBase(void)
{
}

CBase::~CBase(void)
{
}

void *CBase::operator new[ ](size_t size)
{
	void *p = malloc(size);
	memset(p, '\0', size);
	return (p);	
}

void CBase::operator delete[ ](void *p)
{
	free(p);
}

void* CBase::operator new(size_t size)
{
	void *p = malloc(size); 
	memset(p, '\0', size);
	return (p);
}

void CBase::operator delete(void *p)
{
	free(p);
} 
