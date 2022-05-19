#include "Tools.h"

#include "Utils.h"
/**
*	convert binary data in 'std::string' to Hex format in 'std::string'
*/
std::string Tools::b2h(const std::string& data) {
	if (data.empty())
		return "";

	std::string str = "";

	for (unsigned int i = 0; i < data.size(); i++)
		str += Utils::format("%02x", data.at(i));

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

	int len = str.length() / 2;

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


