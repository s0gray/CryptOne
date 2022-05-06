
//#include "OpenSslUnit.h"
#include "SodiumUnit.h"

#include "Logger.h"
#include "Utils.h"
#include <iostream>

#define NONCE_SIZE  24

SodiumCryptUnit cryptUnit;
//OpenSslUnit cryptUnit;


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

    if (argc < 3) {
        LOGE("Usage: [e|d|g|k] <input file> [<key file>] [output file]");
        LOGI("   e - encrypt file with raw key");
        LOGI("   d - decrypt file with raw key");
        LOGI("   g - generate raw key");
        LOGI("   k - generate password protected key");
        LOGI("   x - encrypt file with password protected key");
        LOGI("   l - decrypt file with password protected key");
        return 1;
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

