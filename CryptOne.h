#pragma once

#include "ConfigFile.h"
#include "SodiumGate.h"

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


#define KEY_FILENAME		"key0001.ekey"
//#define KEY_FILENAME    "key001.ekey"

#define COMPRESSED_FILE    "crypt-one-data.tar.gz"
//#define COMPRESSED_FILEW    L"crypt-one-data.tar.gz"

class CryptOne
{
	public:
		CryptOne();
		virtual ~CryptOne();

		RetCode		loadEncryptedKeyFromFile(const std::string& fileName, std::string& key);
		RetCode		generateKeyWithPass(const std::string& fileName);
		RetCode		generateKey(const char* outputFileName);
		RetCode		encryptFileWithPassKey(const std::string& inputFile, const std::string& keyFile, const std::string& outputFileName);
		RetCode		encryptFile(const char* inputFile, const char* keyFile, const char* outputFileName);
		RetCode		decryptFileWithPassKey(const std::string& inputFile, const std::string& keyFile, const std::string& outputFileName);
		RetCode		decryptFile(const char* inputFile, const char* keyFile, const char* outputFileName);
		RetCode		initialize();

		const std::string& getKeyFolder() const { return mKeyFolder; }
		std::string getCloudFolder(int index);
		static std::string enterPassword(const char* promt);

#ifdef WIN32
		static std::string exec(const char* cmd);
#else
		static std::string exec(const char* cmd);
#endif
			
protected:
		RetCode loadConfig();
		RetCode loadConfig(const char* folder);

private:
		SodiumGate cryptoGate;
		//OpenSslUnit cryptoGate;

		ConfigFile configFile;

		std::string mKeyFolder;
	//	std::string mCloudFolder;

		std::vector<std::string> mCloudFolders;
};

