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

#include <string>
#include <vector>
#include <map>


enum RetCode {
	eOk = 0,
	eFatal = 1,
	eBadArg = 2,
	eUnknownErr,
	eBadLen,
	eBadPubKey,
	eBadFile,
	eNotImpl,
	eNotFound
};


typedef unsigned char byte;
typedef std::map<std::string, std::string>	MapStringString;

struct CryptHeader {
	size_t	plainDataSize;
	byte	nonce[24];
};

#define ASSERTME(x)  if(x!=eOk){return x;}


#ifdef WIN32
	#define _INT64 __int64
#else
	#define _INT64 __int64_t
#endif

#ifdef WIN32
	#define GETCH _getch
#else
	#define GETCH getch
#endif


