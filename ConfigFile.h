
#pragma once

#include "typedefs.h"

#define KEY_FOLDER_KEY		"keyFolder"
#define CLOUD_FOLDER_KEY	"cloudFolder"

class ConfigFile
{
	public:
		ConfigFile();
		~ConfigFile();

		int load();
		int store();
		int load(const char* folder);

		std::string getIniFileName() const {
			return mIniFileName;
		}
	ErrCode		getValue(const std::string& key, std::string& value);
	ErrCode		getValueW(const std::string& key, std::wstring& value);
	ErrCode		getCloudFolders(std::vector<std::string>& folders);

private:
		MapStrStr mData;
		std::string mIniFileName;
};