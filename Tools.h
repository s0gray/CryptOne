#pragma once

#include "typedefs.h"


/**
*	Class with static utility methods
*/
class Tools {
public:
	static std::string		h2b(const std::string& str);
	static std::string		b2h(const std::string& data);

	static std::string		getPathSeparator();
	static std::string		getDeleteFileCommand();

	static byte				parseHexChar(char p);
	static std::string		trim(const std::string& str);

	static std::string		cleanChar(const std::string& str1, char c);
	static std::wstring		s2ws(const std::string& value);


	template<typename ... Args>
	static std::string		format(const std::string& format, Args ... args);

};

