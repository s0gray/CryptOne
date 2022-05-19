#pragma once


#include "typedefs.h"
#include <map>

class Utils
{
public:

	static std::wstring s2ws(const std::string& value);
	static std::string	format(const char* fmt, ...);
	static std::string	format_arg_list(const char* fmt, va_list args);




};

