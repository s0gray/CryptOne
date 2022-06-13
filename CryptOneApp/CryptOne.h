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

#include "ConfigFile.h"
#include "SodiumGate.h"

#ifndef WIN32
	#include <cstdio>
	#include <memory>
	#include <stdexcept>
	#include <string>
	#include <array>
#else
	#include <windows.h>
	#include <atlstr.h>
#endif


#define KEY_FILENAME		"key0001.ekey"
#define COMPRESSED_FILE    "crypt-one-data.tar.gz"


class CryptOne
{
	public:
		CryptOne();
		virtual		~CryptOne();

		RetCode		loadEncryptedKeyFromFile(const std::string& fileName, std::string& key);
		RetCode		generateKeyWithPass(const std::string& fileName);
		RetCode		generateKey(const char* outputFileName);
		RetCode		encryptFileWithPassKey(const std::string& inputFile, const std::string& keyFile, const std::string& outputFileName);
		RetCode		encryptFile(const char* inputFile, const char* keyFile, const char* outputFileName);
		RetCode		decryptFileWithPassKey(const std::string& inputFile, const std::string& keyFile, const std::string& outputFileName);
		RetCode		decryptFile(const char* inputFile, const char* keyFile, const char* outputFileName);
		RetCode		initialize();

		const std::string& getKeyFolder() const { return mKeyFolder; }
		std::string getCloudFolder(int index);
		static std::string enterPassword(const char* promt);

		bool isExternalStoragePresent() const {
			return mExternalStoragePresent;
		}
#ifdef WIN32
		static std::string exec(const char* cmd);
#else
		static std::string exec(const char* cmd);
#endif
			
protected:
		RetCode loadConfig();
		RetCode loadConfig(const char* folder);

private:
		SodiumGate cryptoGate;
		ConfigFile configFile;
		std::string mKeyFolder;
		std::vector<std::string> mCloudFolders;
		bool		mExternalStoragePresent;


};

