


#include "ConfigFile.h"
#include "Utils.h"
#include "Logger.h"
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
*	@return 0 if success
*/
int ConfigFile::load() {
	const bool ok = IniFileTools::loadIniFile(mIniFileName, mData);
	if (!ok) {
		return 1;
	}
	return 0;
}

int ConfigFile::load(const char *folder) {
	const bool ok = IniFileTools::loadIniFile(folder + Tools::getPathSeparator() + mIniFileName, mData);
	if (!ok) {
		return 1;
	}
	return 0;
}

/**
*	Store config to file
*/
int ConfigFile::store() {
	std::string content = "";
	MapStrStr::iterator it = mData.begin();
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
ErrCode ConfigFile::getValue(const std::string& key, std::string& value) {
	const MapStrStr::iterator it = mData.find(key);
	if (it == mData.end())
		return eFatal;

	value = it->second;
	return eOk;
}

ErrCode ConfigFile::getValueW(const std::string& key, std::wstring& value) {
	const MapStrStr::iterator it = mData.find(key);
	if (it == mData.end())
		return eFatal;

	std::string valueA = it->second;
	value = Utils::s2ws(valueA);
	return eOk;
}

ErrCode ConfigFile::getCloudFolders(std::vector<std::string> &folders) {
	for (MapStrStr::iterator iter = mData.begin(); iter != mData.end(); ++iter) {
		std::string key = iter->first;
		size_t off = key.find("cloudFolder");
		if ( off != std::string::npos) {
			folders.push_back( iter->second);
		}
	}
	return eOk;
}