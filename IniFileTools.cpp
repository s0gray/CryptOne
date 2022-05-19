#include "IniFileTools.h"

#include "Logger.h"
#include "FileTools.h"
#include "Tools.h"

// ret true if OK
bool IniFileTools::parseIniFile(const byte* data, size_t len, std::map<std::string, std::string>& map) {
	if (!data || len == 0)
		return false;

	char* buff = (char*)&data[0];
	char* buffEnd = buff + len;

	std::string line;
	int lineNumber = 1;

	bool bContinue = true;
	do {
		bContinue = getLine(&buff, buffEnd, line);
		if (bContinue || !line.empty()) {
			std::string key;
			std::string value;
			if (!parseIniFileLine(line, key, value)) {
				//LOG(Log::eStream, "config parse error: [%s]", line.c_str());
				return false;
			}
			if (!key.empty())
				map[key] = value;

			lineNumber++;
		}
	} while (bContinue);

	return true;
}

/**
	Read one line from config-file buffer.
	return 'true' if end not reached
*/
bool IniFileTools::getLine(char** begin, const char* end, std::string& rLine) {
	rLine.clear();

	char* lineStart = *begin;
	char* lineEnd = lineStart;

	// iterate over valid chars
	while (lineEnd < end && *lineEnd != '\r' && *lineEnd != '\n')
		lineEnd++;

	if (lineEnd != lineStart)
		rLine = std::string(lineStart, lineEnd);

	// skip [\r]\n at the end of line
	if (lineEnd < end && *lineEnd == '\r')
		lineEnd++;

	if (lineEnd < end && *lineEnd == '\n')
		lineEnd++;

	*begin = lineEnd;
	return (lineEnd != end);
}


/**
	 parse line [key=value]
	  # is comment
	 return true if OK, false if error

	 if OK: key = key , value = value
*/
bool IniFileTools::parseIniFileLine(const std::string& str, std::string& key, std::string& value)
{
	key = "";
	value = "";

	if (str.empty())
		return true; //true;

	if (str[0] == '#') // comment
		return true; // true;

	std::string::size_type pos = str.find_first_of("=");
	if (pos == std::string::npos)
		return false;

	std::string _key = str.substr(0, pos);
	std::string _value = str.substr(pos + 1);

	key = Tools::trim(_key);
	value = Tools::trim(_value);
	return true;
}

#ifdef WIN32
bool IniFileTools::loadIniFile(const std::wstring& fileName, std::map<std::string, std::string>& map) {
	size_t len = 0;
	byte* data = FileTools::loadFileW(fileName.c_str(), len);
	//LOG(Log::eStream, "Loaded %d bytes from %s", len, fileName);
	if (len == 0 || !data)
		return false;

	bool ret = parseIniFile(data, len, map);

	free(data);
	return ret;
}
#endif

bool IniFileTools::loadIniFile(const std::string& fileName, std::map<std::string, std::string>& map) {
	LOGI("Going to load file [%s]", fileName.c_str());
	size_t len = 0;
	byte* data = FileTools::loadFileA(fileName.c_str(), len);
	if (len == 0 || !data)
		return false;

	bool ret = parseIniFile(data, len, map);
	free(data);
	return ret;
}