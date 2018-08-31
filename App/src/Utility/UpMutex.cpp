#include "UpMutex.h"
#include "UpStringUtility.h"

const char* UPMUTEXT_EXT = "_app_";

UpMutex::UpMutex()
{
	std::string name;
	name.append(UPMUTEXT_EXT);
	m_mutex = ::CreateMutexA(NULL, FALSE, name.c_str());
	DWORD ret = ::WaitForSingleObject(m_mutex, INFINITE); 
}

UpMutex::UpMutex(std::string id)
{
	std::string  name;
	name.append(id);
	name.append(UPMUTEXT_EXT);
	m_mutex = ::CreateMutexA(NULL, FALSE, name.c_str());
	DWORD d = ::WaitForSingleObject(m_mutex, INFINITE); 
}



UpMutex::~UpMutex(void)
{
	::ReleaseMutex(m_mutex); 
	::CloseHandle(m_mutex);
}

bool UpMutex::CheckSameId( std::string id )
{
	bool ret = false;
	std::string  name;
	name.append(id);
	name.append(UPMUTEXT_EXT);
    HANDLE mutex = ::CreateMutexA(NULL, FALSE, name.c_str());
	if( GetLastError() == ERROR_ALREADY_EXISTS )
	{
		ret = true;
	}
	::CloseHandle(mutex);
	return ret;
	
}

