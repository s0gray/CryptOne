#include "Tools.h"

#include <memory>

/**
*	convert binary data in 'std::string' to Hex format in 'std::string'
*/
std::string Tools::b2h(const std::string& data) {
	if (data.empty())
		return "";

	std::string str = "";

	for (unsigned int i = 0; i < data.size(); i++)
		str += Tools::format("%02x", data.at(i));

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

std::string Tools::h2b(const std::string& str0) {
	std::string	 str2 = Tools::cleanChar(str0, ' ');
	std::string	 str = Tools::cleanChar(str2, '\t');

	size_t len = str.length() / 2;

	byte* buf = (byte*)calloc(len, 1);
	if (!buf)
		return "";

	size_t ptr = 0;
	for (size_t pos = 0; pos < str.length(); pos += 2) {
		byte b = (Tools::parseHexChar(str[pos]) << 4) + Tools::parseHexChar(str[pos + 1]);
		buf[ptr++] = b;
	}

	std::string result((const char*)buf, len);
	free(buf);
	return result;
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

//#include <string>
//#include <stdexcept>

template<typename ... Args>
std::string Tools::format(const std::string& format, Args ... args)
{
	size_t size = snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
	if (size <= 0) { /*throw std::runtime_error("Error during formatting.");*/ return ""; }
	std::unique_ptr<char[]> buf(new char[size]);
	snprintf(buf.get(), size, format.c_str(), args ...);
	return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
}

std::wstring Tools::s2ws(const std::string& value) {
	return std::wstring(value.begin(), value.end());
}