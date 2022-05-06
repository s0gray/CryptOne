
#pragma once

#include "typedefs.h"

#define KEY_FOLDER_KEY		"keyFolder"
#define CLOUD_FOLDER_KEY	"cloudFolder"

class ConfigFile
{
	public:
		ConfigFile();
		~ConfigFile();

		int Load();
		int Store();

		int Load(wchar_t* folder);


		std::wstring getIniFileName() const {
			return mIniFileName;
		}


	int		GetValue(const std::string& key, std::string& value);
	int		GetValueW(const std::string& key, std::wstring& value);


private:
		MapStrStr mData;
		std::wstring mIniFileName;
};