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

class IniFileTools {
public:
	static bool		getLine(char** begin, const char* end, std::string& rLine);
	static RetCode	parseIniFile(const std::string& data, std::map<std::string, std::string>& map);
	static RetCode	loadIniFile(const std::string& fileName, std::map<std::string, std::string>& map);
	static RetCode	parseIniFileLine(const std::string& str, std::string& key, std::string& value);
};

