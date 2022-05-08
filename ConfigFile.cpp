


#include "ConfigFile.h"
#include "Utils.h"


#define INI_CONFIG_FILE		"crypt1.ini"


ConfigFile::ConfigFile()
{
	std::string configFolder = "./";

	mIniFileName = configFolder;
	mIniFileName += std::string("\\") + INI_CONFIG_FILE;

}

ConfigFile::~ConfigFile()
{
}

/**
*	Load config file to map
*	@return 0 if success
*/
int ConfigFile::Load() {
	const bool ok = Utils::LoadIniFile(mIniFileName, mData);
	if (!ok) {
		return 1;
	}
	return 0;
}

int ConfigFile::Load(const char *folder) {
	const bool ok = Utils::LoadIniFile(folder + std::string("//") + mIniFileName, mData);
	if (!ok) {
		return 1;
	}
	return 0;
}

/**
*	Store config to file
*/
int ConfigFile::Store() {
	std::string content = "";
	MapStrStr::iterator it = mData.begin();
	while (it != mData.end()) {
		content += it->first + "=" + it->second + "\n";
		it++;
	}
	return Utils::writeFileA(mIniFileName, content);
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
