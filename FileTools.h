#pragma once

#include "typedefs.h"

class FileTools
{
public:
	static ErrCode		loadFileA(const std::string& fileName, std::string& result);
	static ErrCode		writeFileA(const std::string& fileName, const std::string& body) noexcept;


	static ErrCode		copyFileW(const std::wstring& from, const std::wstring& to);
	static ErrCode		copyFileA(const std::string& from, const std::string& to);

	static ErrCode		getAvailableDrives(std::vector<std::string>& result);
	static ErrCode		getRemovablesDrives(std::vector<std::string>& result);
	static ErrCode		getKeyFolder(std::string& folder);

#ifdef WIN32
	static byte* loadFileW(const wchar_t* fileName, size_t& len);
#endif
	static byte* loadFileA(const char* fileName, size_t& len);

#ifdef WIN32
	static ErrCode		loadFileW(const std::wstring& fileName, std::string& result);
	static ErrCode		writeFileW(const std::wstring& fileName, const std::string& body) noexcept;
#endif
};

