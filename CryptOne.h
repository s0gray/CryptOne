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


#define KEY_FILENAMEW    L"key001.ekey"
//#define KEY_FILENAME    "key001.ekey"

#define COMPRESSED_FILEW    L"crypt-one-data.tar.gz"


class CryptOne
{
	public:
		CryptOne();
		virtual ~CryptOne();

		ErrCode		loadEncryptedKeyFromFile(const std::wstring& fileName, std::string& key);
		ErrCode		generateKeyWithPass(const std::wstring& fileName);
		ErrCode		generateKey(const wchar_t* outputFileName);
		ErrCode		encryptFileWithPassKey(const std::wstring& inputFile, const std::wstring& keyFile, const std::wstring& outputFileName);
		ErrCode		encryptFile(const wchar_t* inputFile, const wchar_t* keyFile, const wchar_t* outputFileName);
		ErrCode		decryptFileWithPassKey(const std::wstring& inputFile, const std::wstring& keyFile, const std::wstring& outputFileName);
		ErrCode		decryptFile(const wchar_t* inputFile, const wchar_t* keyFile, const wchar_t* outputFileName);
		ErrCode		initialize();

		const std::wstring& getKeyFolder() const { return mKeyFolder; }
		const std::wstring& getCloudFolder() const { return mCloudFolder; }

#ifdef WIN32
		static CStringA exec(const wchar_t* cmd);
#else
		static std::string exec(const char* cmd);
#endif
			
protected:
		ErrCode loadConfig();
		ErrCode loadConfig(wchar_t* folder);

private:
		SodiumCryptUnit cryptUnit;
		//OpenSslUnit cryptUnit;

		ConfigFile configFile;

		std::wstring mKeyFolder;
		std::wstring mCloudFolder;
};

