/*
 * This file is part of the CryptOne distribution (https://github.com/s0gray/CryptOne).
 * Copyright (c) 2022 Sergey Salata.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "CryptOne.h"

#include "Log.h"
#include "FileTools.h"

#include <iostream>

#ifdef WIN32
    #include <conio.h>
#else
    #include <curses.h>
    #include <string.h>
    #include <unistd.h>
#endif

#define NONCE_SIZE      24


CryptOne::CryptOne() {    
}

CryptOne::~CryptOne() {
}

RetCode CryptOne::initialize() {
    ASSERTME(cryptoGate.selfTest() );

    RetCode ret = loadConfig(); // not mandatory
    RetCode ret1 = eFatal;
    if (ret == eOk) {
        LOGGER("Loaded config file");
        ret1 = configFile.getValue(KEY_FOLDER_KEY, mKeyFolder);
        configFile.getCloudFolders(mCloudFolders);

        for(size_t i = 0; i< mCloudFolders.size(); i++) {
           LOGGER("Configured cloud folder: [%u] [%s]", i, mCloudFolders.at(i).c_str() ); //it->c_str());
        }
    }
    else {
        LOGGER("Not loaded config file");
    }

    if (ret1 != eOk) {
        // keyFolder was not found in config file or config file was not loaded at all
        RetCode ret2 = FileTools::getKeyFolder(mKeyFolder);
        if (ret2 == eNotFound) {
            LOGGER("No removable drives found..");
            return ret2;
        }
    }
    LOGGER("Using keyFolder: [%s]" , mKeyFolder.c_str());
    return eOk;
}

RetCode CryptOne::loadConfig() {
    return (configFile.load() == 0) ? eOk : eFatal;
}

RetCode CryptOne::loadConfig(const char* folder) {
    return (configFile.load(folder) == 0) ? eOk : eFatal;
}

// load, ask password, decrypt
RetCode CryptOne::loadEncryptedKeyFromFile(const std::string& fileName, std::string& key) {
    std::string fileData;
    RetCode ret = FileTools::loadFileA(fileName, fileData);
    if (ret != eOk) {
        LOGGER("Can not load encrypted key from file [%s]", fileName.c_str());
        return ret;
    }
    std::string salt;
    std::string password;
    std::string secretEncrypted;

    salt = fileData.substr(0, 32);
    secretEncrypted = fileData.substr(32, 32);

    password = enterPassword("Please enter password for encryption of key: ");
    std::string hashed;

    ret = cryptoGate.hashDataSHA256(salt + password, hashed);
    if (ret != eOk) {
        LOGGER("Can not Hash password");
        return ret;
    }
    std::string xored;
    ret = cryptoGate.xorData(secretEncrypted, hashed, secretEncrypted.size(), xored);
    if (ret != eOk) {
        LOGGER("Can not Xor password");
        return ret;
    }
    key = xored;
    return eOk;
}

#ifndef WIN32
std::string CryptOne::exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}
#else
//
// Execute a command and get the results. (Only standard output)
//
/*
CStringA CryptOne::exec(const wchar_t* cmd )
{
    CStringA strResult;
    HANDLE hPipeRead, hPipeWrite;

    SECURITY_ATTRIBUTES saAttr = { sizeof(SECURITY_ATTRIBUTES) };
    saAttr.bInheritHandle = TRUE; // Pipe handles are inherited by child process.
    saAttr.lpSecurityDescriptor = NULL;

    // Create a pipe to get results from child's stdout.
    if (!CreatePipe(&hPipeRead, &hPipeWrite, &saAttr, 0))
        return strResult;

    STARTUPINFOW si = { sizeof(STARTUPINFOW) };
    si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
    si.hStdOutput = hPipeWrite;
    si.hStdError = hPipeWrite;
    si.wShowWindow = SW_HIDE; // Prevents cmd window from flashing.
                              // Requires STARTF_USESHOWWINDOW in dwFlags.

    PROCESS_INFORMATION pi = { 0 };

    BOOL fSuccess = CreateProcessW(NULL, (LPWSTR)cmd, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
    if (!fSuccess)
    {
        CloseHandle(hPipeWrite);
        CloseHandle(hPipeRead);
        return strResult;
    }

    bool bProcessEnded = false;
    for (; !bProcessEnded;)
    {
        // Give some timeslice (50 ms), so we won't waste 100% CPU.
        bProcessEnded = WaitForSingleObject(pi.hProcess, 50) == WAIT_OBJECT_0;

        // Even if process exited - we continue reading, if
        // there is some data available over pipe.
        for (;;)
        {
            char buf[1024];
            DWORD dwRead = 0;
            DWORD dwAvail = 0;

            if (!::PeekNamedPipe(hPipeRead, NULL, 0, NULL, &dwAvail, NULL))
                break;

            if (!dwAvail) // No data available, return
                break;

            if (!::ReadFile(hPipeRead, buf, min(sizeof(buf) - 1, dwAvail), &dwRead, NULL) || !dwRead)
                // Error, the child process might ended
                break;

            buf[dwRead] = 0;
            strResult += buf;
        }
    } //for

    CloseHandle(hPipeWrite);
    CloseHandle(hPipeRead);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return strResult;
} //ExecCmd
*/
std::string CryptOne::exec(const char* cmd)
{
   // LOGGER("exec [%s]", cmd);

    CStringA strResult;
    HANDLE hPipeRead, hPipeWrite;

    SECURITY_ATTRIBUTES saAttr = { sizeof(SECURITY_ATTRIBUTES) };
    saAttr.bInheritHandle = TRUE; // Pipe handles are inherited by child process.
    saAttr.lpSecurityDescriptor = NULL;

    // Create a pipe to get results from child's stdout.
    if (!CreatePipe(&hPipeRead, &hPipeWrite, &saAttr, 0))
        return std::string( strResult, strResult.GetLength());

    STARTUPINFOA si = { sizeof(STARTUPINFOW) };
    si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
    si.hStdOutput = hPipeWrite;
    si.hStdError = hPipeWrite;
    si.wShowWindow = SW_HIDE; // Prevents cmd window from flashing.
                              // Requires STARTF_USESHOWWINDOW in dwFlags.

    PROCESS_INFORMATION pi = { 0 };
    BOOL fSuccess = CreateProcessA(NULL, (LPSTR)cmd, NULL, NULL, TRUE, CREATE_NEW_CONSOLE,
        NULL, NULL, &si, &pi);

    if (!fSuccess) {
        CloseHandle(hPipeWrite);
        CloseHandle(hPipeRead);
        return std::string(strResult, strResult.GetLength());
    }

    bool bProcessEnded = false;
    for (; !bProcessEnded;)
    {
        // Give some timeslice (50 ms), so we won't waste 100% CPU.
        bProcessEnded = WaitForSingleObject(pi.hProcess, 50) == WAIT_OBJECT_0;

        // Even if process exited - we continue reading, if
        // there is some data available over pipe.
        for (;;) {
            char buf[1024];
            DWORD dwRead = 0;
            DWORD dwAvail = 0;

            if (!::PeekNamedPipe(hPipeRead, NULL, 0, NULL, &dwAvail, NULL))
                break;

            if (!dwAvail) // No data available, return
                break;

            if (!::ReadFile(hPipeRead, buf, min(sizeof(buf) - 1, dwAvail), &dwRead, NULL) || !dwRead)
                // Error, the child process might ended
                break;

            buf[dwRead] = 0;
            strResult += buf;
        }
    } //for

    CloseHandle(hPipeWrite);
    CloseHandle(hPipeRead);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return std::string(strResult, strResult.GetLength());
} //ExecCmd
#endif


/**
*   Encrypted key file format
*   1. generate salt
*   2. [salt + pass] -> hash
*   3. [salt] [key XOR hash]
* 
*   file : [salt] + [xored] 
*/
RetCode CryptOne::generateKeyWithPass(const std::string& fileName) {
    std::string outputFileName = getKeyFolder() + fileName;

  //  LOGGER("outputFileName [%s]", outputFileName.c_str());

    std::string secret;
    RetCode ret = cryptoGate.generateSecretKey(secret);
    ASSERTME(ret);

 //   LOGGER("generateSecretKey ret %u, len = %u", ret, cryptUnit.getSecretKeyLength());

    std::string password1;
    password1 = enterPassword("Please enter password for encryption of the key: ");

    std::string password2;
    password2 = enterPassword("For safety please re-enter password for encryption of the key: ");

    if (password1.compare(password2)!=0) {
        std::cout << "Passwords are different.. exiting" << std::endl;
        return eFatal;
    }

    std::string salt;
    ASSERTME(cryptoGate.generateSecretKey(salt) );
   // LOG_DATA(2, "salt", salt.c_str(), salt.size());

    std::string material = salt + password1;
    std::string hashed;
    ret = cryptoGate.hashDataSHA256(material, hashed);
    if (ret != eOk) {
        LOGGER("Can not Hash password");
        return ret;
    }
    std::string xored;
    ret = cryptoGate.xorData(secret, hashed, secret.size(), xored);
    if (ret != eOk) {
        LOGGER("Can not XOR password");
        return ret;
    }
   // LOG_DATA(2, "XORed key", xored.c_str(), xored.size());

    std::string output = salt + xored;
   // LOG_DATA(2, "output", output.c_str(), output.size());

    ret = FileTools::writeFileA(outputFileName, output);
    if (ret == eOk) {
        LOGGER("Written  bytes to []");// , cryptUnit.getSecretKeyLength(), outputFileName);
    }
    else {
        LOGGER("Can not write data to []");// , outputFileName);
    }
    return ret;
}


RetCode CryptOne::generateKey(const char* outputFileName) {
    std::string secret;
    ASSERTME(cryptoGate.generateSecretKey(secret));
    ASSERTME( FileTools::writeFileA(outputFileName, secret));

    return eOk;
}

RetCode CryptOne::encryptFileWithPassKey(
                                        const std::string& inputFile, 
                                        const std::string& keyFileName, 
                                        const std::string& outputFileName) {
    std::string plain;

    ASSERTME(FileTools::loadFileA(inputFile, plain));
    LOGGER("Loaded %u kBytes from [%s]", plain.size() / 1024, inputFile.c_str());

    std::string keyFile = mKeyFolder  + keyFileName;
    std::string plainKey;
    ASSERTME( loadEncryptedKeyFromFile(keyFile, plainKey));

    size_t encryptedSize = plain.size() + 64;
    std::string encrypted;

    CryptHeader header;
    header.plainDataSize = plain.size();
    std::string nonce;

    ASSERTME(cryptoGate.encryptDataSymmetric(plain, encrypted, nonce, plainKey) );
    memcpy(header.nonce, nonce.c_str(), nonce.size());

    std::string outFileData;
    outFileData.assign((const char*)&header, sizeof(header));
    outFileData += encrypted;

    ASSERTME(FileTools::writeFileA(outputFileName, outFileData) );
    LOGGER("Stored %u kBytes to [%s]", outFileData.size() / 1024, outputFileName.c_str());
    return eOk;
}


RetCode CryptOne::encryptFile(const char* inputFile, const char* keyFile, const char* outputFileName) {
    std::string data;
    ASSERTME(FileTools::loadFileA(inputFile, data) );
 //   LOGGER("Loaded %u bytes from [%s]", data.size(), inputFile);

    std::string secret;
    ASSERTME(FileTools::loadFileA(keyFile, secret) );

    std::string encrypted, nonce;
    ASSERTME(cryptoGate.encryptDataSymmetric(data, secret, encrypted, nonce) );

    CryptHeader header;
    header.plainDataSize = data.size();
    memcpy(&header.nonce, nonce.c_str(), NONCE_SIZE);

    std::string outFileData;
    outFileData.assign((const char*)&header, sizeof(header));
    outFileData += encrypted;

    ASSERTME(FileTools::writeFileA(outputFileName, outFileData));

 //   LOGGER("Stored %u bytes to [%s]", outFileData.size(), outputFileName);
    return eOk;
}

RetCode CryptOne::decryptFileWithPassKey(const std::string& inputFile,
                                const std::string& keyFile, 
                                const std::string& outputFileName) {

  //  LOGGER("[%s] [%s] [%s]", inputFile.c_str(), keyFile.c_str(), outputFileName.c_str());

    std::string data;
    ASSERTME(FileTools::loadFileA(inputFile, data) );
 //   LOGGER("Loaded %u bytes from [%s]", data.size(), inputFile.c_str());

    if (data.size() < sizeof(CryptHeader)) {
        LOGGER("Too small input file");// , data.size());
        return eBadFile;
    }
    std::string plainKey;
    ASSERTME( loadEncryptedKeyFromFile(mKeyFolder + keyFile, plainKey) );

    CryptHeader* header = (CryptHeader*)data.c_str();
    const byte* encrypted = (const byte*)data.c_str() + sizeof(CryptHeader);
    size_t encryptedSize = data.size() - sizeof(CryptHeader);

    std::string decrypted;
    size_t decryptedSize = header->plainDataSize;

    ASSERTME(cryptoGate.decryptDataSymmetric(decrypted,
        std::string((const char*)encrypted, encryptedSize),
        std::string((const char*)header->nonce, NONCE_SIZE),
        plainKey,
        decryptedSize));

    ASSERTME(FileTools::writeFileA(outputFileName, decrypted));

    return eOk;
}

RetCode  CryptOne::decryptFile(const char* inputFile, const char* keyFile, const char* outputFileName) {
    std::string data;
    ASSERTME(FileTools::loadFileA(inputFile, data) );

 //   LOGGER("Loaded %u bytes from [%s]", data.size(), inputFile);

    if (data.size() < sizeof(CryptHeader)) {
        LOGGER("Too small input file");
        return eBadFile;
    }

    std::string secret;
    ASSERTME(FileTools::loadFileA(keyFile, secret) );

    CryptHeader* header = (CryptHeader*)data.c_str();
    const byte* encrypted = (const byte*)data.c_str() + sizeof(CryptHeader);
    size_t encryptedSize = data.size() - sizeof(CryptHeader);

    std::string decrypted;
    size_t decryptedSize = header->plainDataSize;

    ASSERTME(cryptoGate.decryptDataSymmetric(decrypted,
                std::string((const char*)encrypted, encryptedSize),
                std::string((const char*)header->nonce, NONCE_SIZE),
                secret,
                decryptedSize
    ));

    ASSERTME(FileTools::writeFileA(outputFileName, decrypted));
    return eOk;
}

std::string CryptOne::enterPassword(const char* promt) {
#ifdef WIN32
    std::cout << promt;
    std::string pass = "";
    char ch;
    ch = GETCH();
    while (ch != 13) {//character 13 is enter
        pass.push_back(ch);
        std::cout << '*';
        ch = GETCH();
    }
    std::cout << "\n";
    return pass;
#else
    char* password = getpass(promt);
    return password;
#endif
}

 std::string CryptOne::getCloudFolder(int index) {     
     if (index<0 || index >= mCloudFolders.size())
         return "";

     return mCloudFolders.at(index);
}
