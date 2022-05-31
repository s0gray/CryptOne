
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

#pragma once

#include "Typedefs.h"
#include <memory>

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
	static void ltrim(std::string& s);

	// trim from end (in place)
	static void rtrim(std::string& s);

	// trim from both ends (in place)
	static void trim(std::string& s);
};

template<typename ... Args>
std::string __format(const std::string& format, Args ... args) {
	size_t size = snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
	if (size <= 0) { return ""; }
	std::unique_ptr<char[]> buf(new char[size]);
	snprintf(buf.get(), size, format.c_str(), args ...);
	return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
}