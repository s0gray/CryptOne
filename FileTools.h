#pragma once

#include "Typedefs.h"

class FileTools
{
public:
	static RetCode		loadFileA(const std::string& fileName, std::string& result);
	static RetCode		writeFileA(const std::string& fileName, const std::string& body) noexcept;


	static RetCode		copyFileW(const std::wstring& from, const std::wstring& to);
	static RetCode		copyFileA(const std::string& from, const std::string& to);

	static RetCode		getAvailableDrives(std::vector<std::string>& result);
	static RetCode		getRemovablesDrives(std::vector<std::string>& result);
	static RetCode		getKeyFolder(std::string& folder);

#ifdef WIN32
	static byte* loadFileW(const wchar_t* fileName, size_t& len);
#endif
	static byte* loadFileA(const char* fileName, size_t& len);

#ifdef WIN32
	static RetCode		loadFileW(const std::wstring& fileName, std::string& result);
	static RetCode		writeFileW(const std::wstring& fileName, const std::string& body) noexcept;
#endif
};

