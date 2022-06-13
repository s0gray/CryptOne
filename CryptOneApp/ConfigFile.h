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

#define KEY_FOLDER_KEY		"keyFolder"
#define CLOUD_FOLDER_KEY	"cloudFolder"

class ConfigFile {
	public:
		ConfigFile();
		~ConfigFile();

		RetCode load();
		int store();
		RetCode load(const char* folder);

		std::string getIniFileName() const {
			return mIniFileName;
		}

		RetCode		getValue(const std::string& key, std::string& value);
		RetCode		getValueW(const std::string& key, std::wstring& value);
		RetCode		getCloudFolders(std::vector<std::string>& folders);

private:
		MapStringString mData;
		std::string mIniFileName;
};