


#include "ConfigFile.h"
#include "Utils.h"


#define INI_CONFIG_FILE		L"crypt1.ini"


ConfigFile::ConfigFile()
{
	std::wstring configFolder = L"./";

	mIniFileName = configFolder;
	mIniFileName += std::wstring(L"\\") + INI_CONFIG_FILE;

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

int ConfigFile::Load(wchar_t *folder) {
	const bool ok = Utils::LoadIniFile(folder + std::wstring(L"//") + mIniFileName, mData);
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
	return Utils::writeFileW(mIniFileName, content);
}


/**
*	Get value by key from map
*	if not found return not 0
*	@return 0 if found
*/
int ConfigFile::GetValue(const std::string& key, std::string& value)
{
	const MapStrStr::iterator it = mData.find(key);
	if (it == mData.end())
		return 1;

	value = it->second;
	return 0;
}

int ConfigFile::GetValueW(const std::string& key, std::wstring& value)
{
	const MapStrStr::iterator it = mData.find(key);
	if (it == mData.end())
		return 1;

	std::string valueA = it->second;
	value = Utils::s2ws(valueA);
	return 0;
}
