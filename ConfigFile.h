
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

	ErrCode		getValue(const std::string& key, std::string& value);
	ErrCode		getValueW(const std::string& key, std::wstring& value);

private:
		MapStrStr mData;
		std::wstring mIniFileName;
};