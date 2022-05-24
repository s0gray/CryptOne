/*
 * This file is part of the CryptOne distribution (https://github.com/s0gray/CryptOne).
 * Copyright (c) 2022 Sergey Salata.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "IniFileTools.h"

#include "Log.h"
#include "FileTools.h"
#include "Tools.h"

/**
*	Parse ini file in format <key>=<value> in every line
*/
RetCode IniFileTools::parseIniFile(const byte* data, size_t len, std::map<std::string, std::string>& map) {
	if (!data || len == 0)
		return eBadArg;

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
			ASSERTME( parseIniFileLine(line, key, value) );

			if (!key.empty())
				map[key] = value;

			lineNumber++;
		}
	} while (bContinue);

	return eOk;
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

*/
RetCode IniFileTools::parseIniFileLine(const std::string& str, std::string& key, std::string& value) {
	key = "";
	value = "";

	if (str.empty())
		return eOk; 

	if (str[0] == '#') // comment
		return eOk; // true;

	std::string::size_type pos = str.find_first_of("=");
	if (pos == std::string::npos)
		return eFatal;

	std::string _key = str.substr(0, pos);
	std::string _value = str.substr(pos + 1);

	key = Tools::trim(_key);
	value = Tools::trim(_value);
	return eOk;
}

#ifdef WIN32
bool IniFileTools::loadIniFile(const std::wstring& fileName, std::map<std::string, std::string>& map) {
	size_t len = 0;
	byte* data = FileTools::loadFileW(fileName.c_str(), len);
	if (len == 0 || !data)
		return false;

	bool ret = parseIniFile(data, len, map);

	free(data);
	return ret;
}
#endif

RetCode IniFileTools::loadIniFile(const std::string& fileName, std::map<std::string, std::string>& map) {
//	LOGI("Going to load file [%s]", fileName.c_str());
	size_t len = 0;
	byte* data = FileTools::loadFileA(fileName.c_str(), len);
	if (len == 0 || !data)
		return eBadFile;

	RetCode ret = parseIniFile(data, len, map);
	free(data);
	return ret;
}