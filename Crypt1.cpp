
//#include "OpenSslUnit.h"
#include "SodiumUnit.h"

#include "Logger.h"
#include "Utils.h"
#include "ConfigFile.h"

#include <iostream>

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

#define NONCE_SIZE      24
#define KEY_FILENAMEW    L"key001.ekey"
#define KEY_FILENAME    "key001.ekey"

#define COMPRESSED_FILEW    L"crypt-one-data.tar.gz"

SodiumCryptUnit cryptUnit;
//OpenSslUnit cryptUnit;


ConfigFile configFile;

#ifndef WIN32
std::string exec(const char* cmd) {
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
CStringA exec(const wchar_t* cmd              // [in] command to execute
)
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
#endif

ErrCode loadConfig() {
    return (configFile.Load() == 0) ? eOk : eFatal;
}

ErrCode loadConfig(wchar_t *folder) {
    return (configFile.Load(folder) == 0) ? eOk : eFatal;
}

// load, ask password, decrypt
ErrCode loadEncryptedKeyFromFile(const wchar_t* fileName, std::string& key) {
    std::string secretEncrypted;
    ErrCode ret = Utils::loadFileW(fileName, secretEncrypted);
    if (ret != eOk) {
        LOGE("Can not load encrypted key from file [%ws], keyFile");
        return ret;
    }
    std::string password;

    std::cout << "Please enter password for encryption of key: ";
    std::cin >> password;

    std::string hashed;
    
    ret = cryptUnit.hashDataSHA256(password, hashed);
    if (ret != eOk) {
        LOGE("Can not Hash password");
        return ret;
    }
    LOG_DATA(2, "password hash", hashed.c_str(), hashed.size());
    std::string xored;
    ret = cryptUnit.xorData(secretEncrypted, hashed, secretEncrypted.size(), xored);
    if (ret != eOk) {
        LOGE("Can not Xor password");
        return ret;
    }
    LOG_DATA(2, "xored key (plain key)", xored.c_str(), xored.size());
    key = xored;
    return eOk;
}

ErrCode generateKeyWithPass(const wchar_t* outputFileName) {
    std::string secret;
    ErrCode ret = cryptUnit.generateSecretKey(secret);
    ASSERTME(ret);

    LOGI("generateSecretKey ret %u, len = %u", ret, cryptUnit.getSecretKeyLength());
    LOG_DATA(2, "secret", secret.c_str(), secret.size());

    std::string password;

    std::cout << "Please enter password for encryption of key: ";
    std::cin >> password;

    std::string hashed;    
    ret = cryptUnit.hashDataSHA256(password, hashed);
    if (ret != eOk) {
        LOGE("Can not Hash password");
        return ret;
    }
    LOG_DATA(2, "password hash", hashed.c_str(), hashed.size());

    std::string xored;
    ret = cryptUnit.xorData( secret, hashed, secret.size(), xored);
    if (ret != eOk) {
        LOGE("Can not Xor password");
        return ret;
    }
    LOG_DATA(2, "xored key", xored.c_str(), xored.size());

    ret = Utils::writeFileW(outputFileName, xored);
    if (ret == eOk) {
        LOGI("Written %u bytes to [%ws]", cryptUnit.getSecretKeyLength(), outputFileName);
    }
    else {
        LOGE("Can not write data to [%ws]", outputFileName);
    }
    return ret;
}

ErrCode generateKey(const wchar_t* outputFileName) {
    std::string secret;
    ErrCode ret = cryptUnit.generateSecretKey(secret);

    LOGI("generateSecretKey ret %u, len = %u", ret, secret.size());
    ASSERTME(ret);

    LOG_DATA(2, "secret", secret.c_str(), secret.size());
    ret = Utils::writeFileW(outputFileName, secret);

    if (ret == eOk) {
        LOGI("Written %u bytes to [%ws]", cryptUnit.getSecretKeyLength(), outputFileName);
    }
    else {
        LOGE("Can not write data to [%ws]", outputFileName);
    }

    return ret;
}

int encryptFileWithPassKey(const wchar_t* inputFile, const wchar_t* keyFile, const wchar_t* outputFileName) {
    std::string plain;
    ErrCode ret = Utils::loadFileW(inputFile, plain);
    if (ret != eOk) {
        LOGE("Can not read input file");
        return 1;
    }
    LOGI("Loaded %u bytes from [%ws]", plain.size(), inputFile);

    std::string plainKey;
    ret = loadEncryptedKeyFromFile(keyFile, plainKey);
    if (ret != eOk) {
        LOGE("Can not load/decrypt key");
        return 1;
    }

    size_t encryptedSize = plain.size() + 64;
    std::string encrypted;

    CryptHeader header;
    header.plainDataSize = plain.size();
    std::string nonce;
    ret = cryptUnit.encryptDataSymmetric(plain, encrypted, nonce, plainKey);
    LOGI("encryptDataSymmetric ret %u, encryptedSize = %u", ret, encryptedSize);
    memcpy(header.nonce, nonce.c_str(), nonce.size());

    std::string outFileData;

    outFileData.assign((const char*)&header, sizeof(header));
    outFileData += encrypted;

    ret = Utils::writeFileW(outputFileName, outFileData);

    if (ret != eOk) {
        LOGE("Can not write encrypted file [%ws]", outputFileName);
        return 4;
    }
    LOGI("Stored %u bytes to [%ws]", outFileData.size(), outputFileName);
    return 0;
}


int encryptFile(const wchar_t *inputFile, const wchar_t *keyFile, const wchar_t *outputFileName) {
    std::string data;
    ErrCode ret = Utils::loadFileW(inputFile, data);
    if (ret != eOk) {
        LOGE("Can not read input file");
        return 1;
    }
    LOGI("Loaded %u bytes from [%ws]", data.size(), inputFile);


    std::string secret;
    ret = Utils::loadFileW(keyFile, secret);
    if (ret != eOk) {
        LOGE("Can not load key from file [%ws], keyFile");
        return 2;
    }

    std::string encrypted, nonce;
    ret = cryptUnit.encryptDataSymmetric(data, secret, encrypted, nonce);

    if (ret!=eOk) {
        LOGE("Can not encrypt ");
        return 1;
    }

    CryptHeader header;
    header.plainDataSize = data.size();
    memcpy(&header.nonce, nonce.c_str(), NONCE_SIZE);

    std::string outFileData;

    outFileData.assign((const char*)&header, sizeof(header));
    outFileData += encrypted;

    ret = Utils::writeFileW(outputFileName, outFileData);

    if (ret != eOk) {
        LOGE("Can not write encrypted file [%ws]", outputFileName);
        return 4;
    }
    LOGI("Stored %u bytes to [%ws]", outFileData.size(), outputFileName);
    return 0;
}

int decryptFileWithPassKey(const wchar_t* inputFile, const wchar_t* keyFile, const wchar_t* outputFileName) {
    std::string data;
    ErrCode ret = Utils::loadFileW(inputFile, data);
    if (ret != eOk) {
        LOGE("Can not read input file");
        return 1;
    }
    LOGI("Loaded %u bytes from [%ws]", data.size(), inputFile);

    if (data.size() < sizeof(CryptHeader)) {
        LOGE("Too small input file : %u bytes", data.size());
        return 3;
    }

    std::string plainKey;
    ret = loadEncryptedKeyFromFile(keyFile, plainKey);
    if (ret != eOk) {
        LOGE("Can not load/decrypt key");
        return 1;
    }

    CryptHeader* header = (CryptHeader*)data.c_str();
    const byte* encrypted = (const byte*)data.c_str() + sizeof(CryptHeader);
    size_t encryptedSize = data.size() - sizeof(CryptHeader);

    BinaryData decrypted;
    size_t decryptedSize = header->plainDataSize;

    ret = cryptUnit.decryptDataSymmetric(decrypted, 
        std::string((const char*)encrypted, encryptedSize),
        std::string((const char*)header->nonce, NONCE_SIZE), 
        plainKey,
        decryptedSize);

    if (ret != eOk) {
        LOGE("Can not decrypt file [%ws]", inputFile);
        return 4;
    }

    ret = Utils::writeFileW(outputFileName, decrypted);
    if (ret != eOk) {
        LOGE("Can not write decrypted file to [%ws]", outputFileName);
        return 5;
    }
    return 0;
}

int decryptFile(const wchar_t* inputFile, const wchar_t* keyFile, const wchar_t* outputFileName) {
    std::string data;
    ErrCode ret = Utils::loadFileW(inputFile, data);
    if (ret != eOk) {
        LOGE("Can not read input file");
        return 1;
    }
    LOGI("Loaded %u bytes from [%ws]", data.size(), inputFile);

    if (data.size() < sizeof(CryptHeader)) {
        LOGE("Too small input file : %u bytes", data.size());
        return 3;
    }

    std::string secret;
    ret = Utils::loadFileW(keyFile, secret);
    if (ret != eOk) {
        LOGE("Can not load key from file [%ws], keyFile");
        return 2;
    }

    CryptHeader* header = (CryptHeader*)data.c_str();
    const byte *encrypted = (const byte*)data.c_str() + sizeof(CryptHeader);
    size_t encryptedSize = data.size() - sizeof(CryptHeader);

    BinaryData decrypted;
    size_t decryptedSize = header->plainDataSize;


    ret = cryptUnit.decryptDataSymmetric(decrypted, 
        std::string((const char*)encrypted, encryptedSize),
        std::string((const char*)header->nonce, NONCE_SIZE),
        secret,
        decryptedSize
        );

       if (ret != eOk) {
           LOGE("Can not decrypt file [%ws]", inputFile);
           return 4;
       }
   
   ret = Utils::writeFileW(outputFileName, decrypted);
   if (ret != eOk) {
       LOGE("Can not write decrypted file to [%ws]", outputFileName);
       return 5;
   }
   return 0;
}

int wmain(int argc, wchar_t* argv[])
{
    LOG_INIT(2, L"");
    LOGI("CryptOne v1.0.0");

    ErrCode ret = cryptUnit.selfTest();
    if (ret == eOk) {
        LOGI("CryptoUnit self test OK");
    }
    else {
        LOGE("CryptoUnit self test FAILED : %u", ret);
        return 1;
    }

    if (argc < 2) {
        LOGE("Usage: [e|d|g|k] <input file> [<key file>] [output file]");
        LOGI("   e - encrypt file with raw key");
        LOGI("   d - decrypt file with raw key");
        LOGI("   g - generate raw key");
        LOGI("   k - generate password protected key");
        LOGI("   x - encrypt file with password protected key");
        LOGI("   l - decrypt file with password protected key");

        LOGE("or Usage: [generate-key | upload | download | decrypt] <folder/file>");
        LOGI("   generate-key - generate passwrod encrpyted key and store on USB stick");
        LOGI("   upload - encrypt and upload folder to cloud");
        LOGI("   download - download and decrypt data from cloud");
        LOGI("   decrypt - decrypt data from cloud");
        LOGI("   encrypt - encrypt folder for cloud");
        return 1;
    }

    if (!wcscmp(argv[1], L"generate-key")) {
        LOGI("Generate key and store to USB");

        ErrCode ret = loadConfig();
        if (ret != eOk) {
            LOGE("Could not load config file");
            return 1;
        }
        std::string keyFolder;
        int r = configFile.GetValue(KEY_FOLDER_KEY, keyFolder);
        if (r != 0) {
            LOGE("Missing key '%s' in config file", KEY_FOLDER_KEY);
            return 1;
        }
        std::string keyFileName = KEY_FILENAME;

        return generateKeyWithPass( Utils::s2ws( keyFolder + "//" + keyFileName ).c_str());
    }
    if (!wcscmp(argv[1], L"upload")) {
        LOGI("Upload folder to cloud");
        if (argc < 3) {
            LOGI("Usage: Crypt1 upload <folder>");
            return 1;
        }

        ErrCode ret = loadConfig();
        if (ret != eOk) {
            LOGE("Could not load config file");
            return 1;
        }
        std::wstring cloudFolder, keyFolder;
        int r = configFile.GetValueW(CLOUD_FOLDER_KEY, cloudFolder);
        if (r != 0) {
            LOGE("Missing key '%s' in config file", CLOUD_FOLDER_KEY);
            return 1;
        }
        r = configFile.GetValueW(KEY_FOLDER_KEY, keyFolder);
        if (r != 0) {
            LOGE("Missing key '%s' in config file", KEY_FOLDER_KEY);
            return 1;
        }
        std::wstring compressedFile = COMPRESSED_FILEW;

        LOGI("Compressing folder [%ws] to file [%ws]..", argv[2], compressedFile.c_str());
        CStringA res = exec( (L"tar -czf " + compressedFile + std::wstring(L" ") + argv[2]).c_str());
        // encrypt

        LOGI("Encrypting file [%ws]..", compressedFile.c_str());

        r = encryptFileWithPassKey(compressedFile.c_str(),
           (keyFolder + std::wstring(L"//") +KEY_FILENAMEW).c_str(),
            L"crypt-one-data.tar.gz.enc");

        if (r != 0) {
            LOGE("Failed to encrypt compressed folder");
            return 1;
        }

        std::wstring encryptedFile = compressedFile + L".enc";
        // copy
        LOGI("Uploading file [%ws] to cloud", encryptedFile.c_str());
        r = Utils::copyFileW(encryptedFile, cloudFolder + L"//crypt-one-data.tar.gz.enc" );
        if (r != 0) {
            LOGE("Failed to upload encrypted file to cloud");
            return 1;
        }
        return 0;
    }
    if (!wcscmp(argv[1], L"download")) {
        LOGI("Download folder from cloud");

        std::wstring compressedFile = COMPRESSED_FILEW;
        std::wstring encryptedFile = compressedFile + L".enc";

       ErrCode ret = loadConfig();
       if (ret != eOk) {
           LOGE("Can not load config");
           return 1;
       }
       std::wstring cloudFolder, keyFolder;
       int r = configFile.GetValueW(CLOUD_FOLDER_KEY, cloudFolder);
       if (r != 0) {
           LOGE("Missing key '%s' in config", CLOUD_FOLDER_KEY);
           return 1;
       }
       r = configFile.GetValueW(KEY_FOLDER_KEY, keyFolder);
       if (r != 0) {
           LOGE("Missing key '%s' in config", KEY_FOLDER_KEY);
           return 1;
       }
       std::wstring sourceFile = cloudFolder + L"//" + encryptedFile;
       LOGI("Downloading file [%ws] from cloud ", sourceFile.c_str());
       ret = Utils::copyFileW(sourceFile, encryptedFile);
       if (ret != eOk) {
           LOGE("Can not download file from cloud");
           return 1;
       }
       LOGI("Decrypting file from cloud");
       r = decryptFileWithPassKey(encryptedFile.c_str(), 
            (keyFolder + std::wstring(L"//") + KEY_FILENAMEW).c_str(), compressedFile.c_str());

       if (r != 0) {
           LOGE("Could not decrypt downloaded file [%ws]", encryptedFile.c_str());
           return 1;
       }
       LOGI("Decompressing file [%ws]", compressedFile.c_str());
       exec((L"tar xf " + compressedFile).c_str());
       return 0;
    }
    if (!wcscmp(argv[1], L"decrypt")) {
        LOGI("Decrypt file from cloud");

        if (argc < 3) {
            LOGI("Usage: Crypt1 decrypt <file>");
            return 1;
        }
        std::wstring  keyFolder;
        int r = configFile.GetValueW(KEY_FOLDER_KEY, keyFolder);
        if (r != 0) {
            LOGE("Missing key '%s' in config", KEY_FOLDER_KEY);
            return 1;
        }
        std::wstring compressedFile = COMPRESSED_FILEW;
        r = decryptFileWithPassKey(argv[2],
            (keyFolder + std::wstring(L"//") + KEY_FILENAMEW).c_str(), compressedFile.c_str());

        if (r != 0) {
            LOGE("Could not decrypt file [%ws]", compressedFile.c_str());
            return 1;
        }
        LOGI("Decompressing file [%ws]", compressedFile.c_str());
        exec((L"tar xf " + compressedFile).c_str());
        return 0;
    }

    if (!wcscmp(argv[1], L"encrypt")) {
        LOGI("Encrypt file for cloud");

        if (argc < 3) {
            LOGI("Usage: Crypt1 encrypt <folder>");
            return 1;
        }
        std::wstring  keyFolder;
        int r = configFile.GetValueW(KEY_FOLDER_KEY, keyFolder);
        if (r != 0) {
            LOGE("Missing key '%s' in config", KEY_FOLDER_KEY);
            return 1;
        }

        std::wstring compressedFile = COMPRESSED_FILEW;

        LOGI("Compressing folder [%ws] to file [%ws]..", argv[2], compressedFile.c_str());
        CStringA res = exec((L"tar -czf " + compressedFile + std::wstring(L" ") + argv[2]).c_str());
        // encrypt

        LOGI("Encrypting file [%ws]..", compressedFile.c_str());

        r = encryptFileWithPassKey(compressedFile.c_str(),
            (keyFolder + std::wstring(L"//") + KEY_FILENAMEW).c_str(),
            L"crypt-one-data.tar.gz.enc");

        if (r != 0) {
            LOGE("Failed to encrypt compressed folder");
            return 1;
        }
        return 0;
    }

    switch (argv[1][0])
    {
        case L'e':
        {
            LOGI("encrypt file");
            if (argc < 4) {
                LOGE("Usage: e <input file> <key file> [output file]");
                return 1;
            }
            return encryptFile(argv[2], argv[3], (argc == 5) ? argv[4] : (argv[2] + std::wstring(L".enc")).c_str());
        }
        case L'x':
        {
            LOGI("encrypt file with password protected key");
            if (argc < 4) {
                LOGE("Usage: e <input file> <key file> [output file]");
                return 1;
            }
            return encryptFileWithPassKey(argv[2], argv[3], (argc == 5) ? argv[4] : (argv[2] + std::wstring(L".enc")).c_str());
        }
        case L'd':
        {
            LOGI("decrypt file");
            return decryptFile(argv[2], argv[3], (argc == 5) ? argv[4] : (argv[2] + std::wstring(L".dec")).c_str());
        }
        case L'l':
        {
            LOGI("decrypt file with password protected key");
            return decryptFileWithPassKey(argv[2], argv[3], (argc == 5) ? argv[4] : (argv[2] + std::wstring(L".dec")).c_str());
        }
        case L'g':
        {
            LOGI("generate key and store raw key to file");
            return generateKey(argv[2]);
        }
        case L'k':
        {
            LOGI("generate key and store password protected key to file");
            return generateKeyWithPass(argv[2]);
        }
        default:
                LOGE("unrecognized mode '%c'", argv[1][0]);
                return 1;
    }
}

