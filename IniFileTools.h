#pragma once

#include "Typedefs.h"

class IniFileTools {
public:
	static bool		getLine(char** begin, const char* end, std::string& rLine);
	static bool		parseIniFile(const byte* data, size_t len, std::map<std::string, std::string>& map);

#ifdef WIN32
	static bool		loadIniFile(const std::wstring& fileName, std::map<std::string, std::string>& map);
#endif
	static bool		loadIniFile(const std::string& fileName, std::map<std::string, std::string>& map);
	static bool		parseIniFileLine(const std::string& str, std::string& key, std::string& value);

};

