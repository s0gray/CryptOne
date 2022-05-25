
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

#include "ConfigFile.h"
#include "Log.h"
#include "Tools.h"
#include "IniFileTools.h"
#include "FileTools.h"


#define INI_CONFIG_FILE		"crypt1.ini"


ConfigFile::ConfigFile() {
	std::string configFolder = ".";

	mIniFileName = configFolder;
	mIniFileName += Tools::getPathSeparator() + INI_CONFIG_FILE;
}

ConfigFile::~ConfigFile() {
}

/**
*	Load config file to map
*/
RetCode ConfigFile::load() { // inline
	return IniFileTools::loadIniFile(mIniFileName, mData);
}

RetCode ConfigFile::load(const char *folder) {
	return IniFileTools::loadIniFile(folder + Tools::getPathSeparator() + mIniFileName, mData);
}

/**
*	Store config to file
*/
int ConfigFile::store() {
	std::string content = "";
	MapStringString::iterator it = mData.begin();
	while (it != mData.end()) {
		content += it->first + "=" + it->second + "\n";
		it++;
	}
	return FileTools::writeFileA(mIniFileName, content);
}


/**
*	Get value by key from map
*	if not found return not 0
*	@return 0 if found
*/
RetCode ConfigFile::getValue(const std::string& key, std::string& value) {
	const MapStringString::iterator it = mData.find(key);
	if (it == mData.end())
		return eFatal;

	value = it->second;
	return eOk;
}

RetCode ConfigFile::getValueW(const std::string& key, std::wstring& value) {
	const MapStringString::iterator it = mData.find(key);
	if (it == mData.end())
		return eFatal;

	std::string valueA = it->second;
	value = Tools::s2ws(valueA);
	return eOk;
}

RetCode ConfigFile::getCloudFolders(std::vector<std::string> &folders) {
	for (MapStringString::iterator iter = mData.begin(); iter != mData.end(); ++iter) {
		std::string key = iter->first;
		size_t off = key.find("cloudFolder");
		if ( off != std::string::npos) {
			folders.push_back( iter->second);
		}
	}
	return eOk;
}