
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

