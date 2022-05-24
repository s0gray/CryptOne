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
RetCode IniFileTools::parseIniFile(const std::string& data, std::map<std::string, std::string>& map) {
	if (data.empty())
		return eBadArg;

	char* buff = (char*)&data.c_str()[0];
	char* buffEnd = buff + data.size();

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


RetCode IniFileTools::loadIniFile(const std::string& fileName, std::map<std::string, std::string>& map) {
	std::string data;
	ASSERTME( FileTools::loadFileA(fileName, data) );

	RetCode ret = parseIniFile(data, map);
	return ret;
}