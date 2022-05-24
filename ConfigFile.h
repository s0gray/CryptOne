
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