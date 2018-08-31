#pragma once
#include <string>

class UpFile
{
public:
	UpFile(const char* fileName, bool isOnlyReady = false);
	UpFile(const wchar_t* fileName, bool isOnlyReady = false);
	virtual ~UpFile(void);

public:
	
	__int64 FileSize();
	int Read(char* buf, int num);
	int Write(const char* buf, int num);
	bool Seek(int postion);

public:

	static std::string* ReadFileToString(std::wstring filepath);
	static void ReleaseFileString(std::string* fileString);

	static bool CreatePath(const char* filePath);
	static bool CreatePathW(const wchar_t* filePath);
	static bool IsDirExisted(const char* dirName);
	static bool IsDirExistedW(const wchar_t* dirName);

	static std::string GetPath(const char* fileName);
	static std::wstring GetPathW(const wchar_t* fileName);
	static bool IsFileExisted(const char* fileName);
	static bool IsFileExistedW(const wchar_t* fileName);
	static bool DeleteExistFile(const char* fileName);
	static bool DeleteExistFileW(const wchar_t* fileName);
	static void OpenURL(std::wstring url, bool use_ie = true);

private:
	void* file;
};

