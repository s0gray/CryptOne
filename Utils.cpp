#include "Utils.h"

#include <sstream>
#include <fstream>
#include <stdarg.h>
#include <iostream>

#include "Logger.h"


#ifdef WIN32
	#include <Windows.h>
	#include <conio.h>
#else
	#include <curses.h>
	#include <string.h>
#endif



std::string Utils::format_arg_list(const char* fmt, va_list args) {
	if (!fmt) return "";
	int   result = -1, length = 256;
	char* buffer = 0;
	while (result == -1) {
		if (buffer) delete[] buffer;
		buffer = new char[length + 1];
		memset(buffer, 0, length + 1);
		result = vsnprintf(buffer, length, fmt, args);
		length *= 2;
	}
	std::string s(buffer);
	delete[] buffer;
	return s;
}

std::string Utils::format(const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);
	std::string s = format_arg_list(fmt, args);
	va_end(args);
	return s;
}

std::wstring Utils::s2ws(const std::string& value) 
{
	std::wstring w(value.begin(), value.end());
	return w;
}

