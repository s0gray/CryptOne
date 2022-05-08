#pragma once

#include "ConfigFile.h"
#include "SodiumUnit.h"

#ifndef WIN32
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#else
#include <windows.h>
#include <atlstr.h>
#endif


#define KEY_FILENAME    "key001.ekey"
//#define KEY_FILENAME    "key001.ekey"

#define COMPRESSED_FILE    "crypt-one-data.tar.gz"
//#define COMPRESSED_FILEW    L"crypt-one-data.tar.gz"

class CryptOne
{
	public:
		CryptOne();
		virtual ~CryptOne();

		ErrCode		loadEncryptedKeyFromFile(const std::string& fileName, std::string& key);
		ErrCode		generateKeyWithPass(const std::string& fileName);
		ErrCode		generateKey(const char* outputFileName);
		ErrCode		encryptFileWithPassKey(const std::string& inputFile, const std::string& keyFile, const std::string& outputFileName);
		ErrCode		encryptFile(const char* inputFile, const char* keyFile, const char* outputFileName);
		ErrCode		decryptFileWithPassKey(const std::string& inputFile, const std::string& keyFile, const std::string& outputFileName);
		ErrCode		decryptFile(const char* inputFile, const char* keyFile, const char* outputFileName);
		ErrCode		initialize();

		const std::string& getKeyFolder() const { return mKeyFolder; }
		const std::string& getCloudFolder() const { return mCloudFolder; }
		static std::string enterPassword();

#ifdef WIN32
//		static CStringA exec(const wchar_t* cmd);
		static std::string exec(const char* cmd);

#else
		static std::string exec(const char* cmd);
#endif
			
protected:
		ErrCode loadConfig();
		ErrCode loadConfig(const char* folder);

private:
		SodiumCryptUnit cryptUnit;
		//OpenSslUnit cryptUnit;

		ConfigFile configFile;

		std::string mKeyFolder;
		std::string mCloudFolder;
};

