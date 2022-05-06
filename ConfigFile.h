
#pragma once

#include "typedefs.h"



class ConfigFile
{
	public:
		ConfigFile();
		~ConfigFile();

		int Load();
		int Store();

		std::wstring getIniFileName() const {
			return mIniFileName;
		}

protected:
	int GetValue(const std::string& key, std::string& value);


private:
		MapStrStr mData;
		std::wstring mIniFileName;
};