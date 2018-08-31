#pragma once
#include<string>
#include<map>

class UpDict
{
public:
	UpDict(void);
	~UpDict(void);

	bool LoadDict(std::wstring filename);
	std::wstring FindString(std::wstring key);
	void Clear();
private:
	std::map<std::wstring, std::wstring> m_StringMap;	
};

