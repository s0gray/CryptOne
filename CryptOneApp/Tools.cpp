
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
#include <algorithm>
#include <codecvt>

#ifdef WIN32
	#include <Shlobj.h>
#else
	#include <locale>
	#include <codecvt>
#endif

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


// trim from start (in place)
void Tools::ltrim(std::string& s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
		return !std::isspace(ch);
		}));
}

// trim from end (in place)
void Tools::rtrim(std::string& s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
		return !std::isspace(ch);
		}).base(), s.end());
}

// trim from both ends (in place)
void Tools::trim(std::string& s) {
	ltrim(s);
	rtrim(s);
}

std::wstring Tools::s2ws(const std::string& value) {
	return std::wstring(value.begin(), value.end());
}

// replace '~' -> username
std::string Tools::decodeFolder(const std::string &folder) {
	size_t off = folder.find('~');
	if (off == std::string::npos)
		return folder;

	std::string result = folder;
#ifdef WIN32
	WCHAR path[MAX_PATH];
	if (!SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_PROFILE, NULL, 0, path))) {
		return folder;
	}
	std::string homeDir = Tools::wideToChar(path); 
	result = homeDir;
	result += folder.substr(off + 1, folder.size() - off - 1);	
#endif

	return result;
}

std::string Tools::wideToChar(const std::wstring& wide) {
	if (wide.empty()) return std::string();

#ifdef WIN32
	int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wide[0], (int)wide.size(), NULL, 0, NULL, NULL);
	std::string strTo(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, &wide[0], (int)wide.size(), &strTo[0], size_needed, NULL, NULL);
	return strTo;
#else
	std::wstring string_to_convert = wide;

	//setup converter
	using convert_type = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_type, wchar_t> converter;

	//use converter (.to_bytes: wstr->str, .from_bytes: str->wstr)
	std::string converted_str = converter.to_bytes(string_to_convert);
	return converted_str;
#endif
}