#pragma once


#include "typedefs.h"
#include <map>

class Utils
{
public:
	static std::string	IntToHexString(int value);

#ifdef WIN32
	static ErrCode		loadFileW(const std::wstring& fileName, std::string& result);
	static ErrCode		writeFileW(const std::wstring& fileName, const std::string& body) noexcept;
#endif
	static ErrCode		loadFileA(const std::string& fileName, std::string& result);
	static ErrCode		writeFileA(const std::string& fileName, const std::string& body) noexcept;

	static std::wstring s2ws(const std::string& value);
	static std::string	charToString(char ch);
	static std::string	charToHex(char ch);
	static std::string	bin2hex(const byte* buf, size_t len);
	static std::string	bin2hex(const std::string& data);
	static std::string	vectorToString(const std::vector<unsigned char>& data);
	static std::string	ws2s(const std::wstring& ws);
	static std::string	format(const char* fmt, ...);
	static std::string	format_arg_list(const char* fmt, va_list args);

#ifdef WIN32
	static void			getEnvTempPath(std::wstring& value);
#endif

	static bool		getLine(char** begin, const char* end, std::string& rLine);
	static bool		parseIniFile(const byte* data, size_t len, std::map<std::string, std::string>& map);

#ifdef WIN32
	static bool		LoadIniFile(const std::wstring& fileName, std::map<std::string, std::string>& map);
#endif
	static bool		LoadIniFile(const std::string& fileName, std::map<std::string, std::string>& map);

	static bool			parseIniFileLine(const std::string& str, std::string& key, std::string& value);

	static std::string	trim(const std::string& str);
	static std::string	hex2bin(const std::string& str);
	static std::string	 removeChar(const std::string& str1, char c);
	static byte*		hex2bin2(const std::string& str, size_t& len);
	static byte			parseChar_(char p);
	static ErrCode		copyFileW(const std::wstring& from, const std::wstring& to);
	static ErrCode		copyFileA(const std::string& from, const std::string& to);
	static ErrCode		getAvailableDrives(std::vector<std::string>& result);
	static ErrCode		getRemovablesDrives(std::vector<std::string>& result);
	static ErrCode		getKeyFolder(std::string& folder);

private:
#ifdef WIN32
	static byte* loadFileW(const wchar_t* fileName, size_t& len);
#endif
	static byte* loadFileA(const char* fileName, size_t& len);


};

