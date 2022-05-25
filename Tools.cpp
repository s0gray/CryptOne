
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

#include "Tools.h"

#include <memory>

/**
*	convert binary data in 'std::string' to Hex format in 'std::string'
*/
std::string Tools::b2h(const std::string& data) {
	if (data.empty())
		return "";

	std::string str = "";
	for (size_t i = 0; i < data.size(); i++)
		str += __format("%02x", data.at(i));

	return str;
}

std::string Tools::getPathSeparator() {
#ifdef WIN32
	return "\\";
#else
	return "/";
#endif
}

byte Tools::parseHexChar(char p) {
	if (p >= '0' && p <= '9')
		return (byte)(p - '0');

	if (p >= 'a' && p <= 'f')
		return 10 + (byte)(p - 'a');

	if (p >= 'A' && p <= 'F')
		return 10 + (byte)(p - 'A');

	return (byte)255;
}

/**
*	Hex to binary
*/
std::string Tools::h2b(const std::string& str0) {
	std::string	 str2 = Tools::cleanChar(str0, ' ');
	std::string	 str = Tools::cleanChar(str2, '\t');

	size_t len = str.length() / 2;
	std::unique_ptr<char[]> buf(new char[len]);
	if (!buf)
		return "";

	size_t ptr = 0;
	for (size_t pos = 0; pos < str.length(); pos += 2) 
		buf[ptr++] = (byte)(Tools::parseHexChar(str[pos]) << 4) + Tools::parseHexChar(str[pos + 1]);
	
	return std::string(buf.get(), len);
}

/**
*	Remove character 'c' from string 'str'
*/
std::string	 Tools::cleanChar(const std::string& str, char c) {
	std::string result = "";
	for (size_t i = 0; i < str.size(); i++) {
		if (str[i] != c)
			result += str[i];
	}
	return result;
}


std::string Tools::getDeleteFileCommand() {
#ifdef WIN32
	return "del";
#else
	return "rm";
#endif
}


std::string Tools::trim(const std::string& _str) {
	std::string str = _str;
	int i, j, start, end;

	//ltrim
	for (i = 0; (str[i] != 0 && str[i] <= 32); )
		i++;
	start = i;

	//rtrim
	for (i = 0, j = 0; str[i] != 0; i++)
		j = ((str[i] <= 32) ? j + 1 : 0);
	end = i - j;
	str = str.substr(start, end - start);
	return str;
}

std::wstring Tools::s2ws(const std::string& value) {
	return std::wstring(value.begin(), value.end());
}