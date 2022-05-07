

#include "Logger.h"
#include "CryptOne.h"

#include "Utils.h"


int wmain(int argc, wchar_t* argv[])
{
    LOG_INIT(2, L"");
    LOGI("CryptOne v1.0.0");

    CryptOne cryptOne;

    ErrCode ret = cryptOne.initialize();
    if (ret != eOk) {
        LOGE("CryptOne initialize FAILED : %u", ret);
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
        LOGI("   down - get data from cloud");
        LOGI("   up - upload data to cloud");
        return 1;
    }

    if (!wcscmp(argv[1], L"generate-key")) {
        LOGI("Generate key and store to USB");

        return cryptOne.generateKeyWithPass(KEY_FILENAMEW);
    }

    if (!wcscmp(argv[1], L"upload")) {
        LOGI("Upload folder to cloud");
        if (argc < 3) {
            LOGI("Usage: Crypt1 upload <folder>");
            return 1;
        }

        std::wstring compressedFile = COMPRESSED_FILEW;

        LOGI("Compressing folder [%ws] to file [%ws]..", argv[2], compressedFile.c_str());
        CStringA res = CryptOne::exec( (L"tar -czf " + compressedFile + std::wstring(L" ") + argv[2]).c_str());
        // encrypt

        LOGI("Encrypting file [%ws]..", compressedFile.c_str());

        ErrCode r = cryptOne.encryptFileWithPassKey(compressedFile, KEY_FILENAMEW, L"crypt-one-data.tar.gz.enc");
        if (r != eOk) {
            LOGE("Failed to encrypt compressed folder");
            return 1;
        }

        std::wstring encryptedFile = compressedFile + L".enc";
        // copy
        LOGI("Uploading file [%ws] to cloud", encryptedFile.c_str());
        r = Utils::copyFileW(encryptedFile, cryptOne.getCloudFolder() + L"//crypt-one-data.tar.gz.enc");
        if (r != 0) {
            LOGE("Failed to upload encrypted file to cloud");
            return 1;
        }
        return 0;
    }

    if (!wcscmp(argv[1], L"up")) {
        LOGI("Upload encrypted file to cloud");
        if (argc < 3) {
            LOGI("Usage: Crypt1 up <file>");
            return 1;
        }

        std::wstring encryptedFile = argv[2];
        // copy
        LOGI("Uploading file [%ws] to cloud", encryptedFile.c_str());
        ErrCode r = Utils::copyFileW(encryptedFile, cryptOne.getCloudFolder() + L"//crypt-one-data.tar.gz.enc");
        if (r != eOk) {
            LOGE("Failed to upload encrypted file to cloud");
            return 1;
        }
        return 0;
    }

    if (!wcscmp(argv[1], L"download")) {
        LOGI("Download folder from cloud");

        std::wstring compressedFile = COMPRESSED_FILEW;
        std::wstring encryptedFile = compressedFile + L".enc";

       std::wstring sourceFile = cryptOne.getCloudFolder() + L"//" + encryptedFile;
       LOGI("Downloading file [%ws] from cloud ", sourceFile.c_str());
       ret = Utils::copyFileW(sourceFile, encryptedFile);
       if (ret != eOk) {
           LOGE("Can not download file from cloud");
           return 1;
       }
       LOGI("Decrypting file from cloud");
       ret = cryptOne.decryptFileWithPassKey(encryptedFile, 
             KEY_FILENAMEW, compressedFile);

       if (ret != eOk) {
           LOGE("Could not decrypt downloaded file [%ws]", encryptedFile.c_str());
           return 1;
       }
       LOGI("Decompressing file [%ws]", compressedFile.c_str());
       CryptOne::exec((L"tar xf " + compressedFile).c_str());
       return 0;
    }

    if (!wcscmp(argv[1], L"down")) {
        LOGI("Download file from cloud");

        std::wstring compressedFile = COMPRESSED_FILEW;
        std::wstring encryptedFile = compressedFile + L".enc";


        std::wstring sourceFile = cryptOne.getCloudFolder() + L"//" + encryptedFile;
        LOGI("Downloading file [%ws] from cloud ", sourceFile.c_str());
        ret = Utils::copyFileW(sourceFile, encryptedFile);
        if (ret != eOk) {
            LOGE("Can not download file from cloud");
            return 1;
        }       
        return 0;
    }

    if (!wcscmp(argv[1], L"decrypt")) {
        LOGI("Decrypt file from cloud");

        if (argc < 3) {
            LOGI("Usage: Crypt1 decrypt <file>");
            return 1;
        }

        std::wstring compressedFile = COMPRESSED_FILEW;
        ret = cryptOne.decryptFileWithPassKey(argv[2],
             KEY_FILENAMEW, compressedFile);

        if (ret != eOk) {
            LOGE("Could not decrypt file [%ws]", compressedFile.c_str());
            return 1;
        }
        LOGI("Decompressing file [%ws]", compressedFile.c_str());
        CryptOne::exec((L"tar xf " + compressedFile).c_str());
        return 0;
    }

    if (!wcscmp(argv[1], L"encrypt")) {
        LOGI("Encrypt file for cloud");

        if (argc < 3) {
            LOGI("Usage: Crypt1 encrypt <folder>");
            return 1;
        }


        std::wstring compressedFile = COMPRESSED_FILEW;
        LOGI("Compressing folder [%ws] to file [%ws]..", argv[2], compressedFile.c_str());
        CStringA res = CryptOne::exec((L"tar -czf " + compressedFile + std::wstring(L" ") + argv[2]).c_str());
        // encrypt

        LOGI("Encrypting file [%ws]..", compressedFile.c_str());
        ret = cryptOne.encryptFileWithPassKey(compressedFile,
            KEY_FILENAMEW,
            L"crypt-one-data.tar.gz.enc");

        if (ret != eOk) {
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
            return cryptOne.encryptFile(argv[2], argv[3], (argc == 5) ? argv[4] : (argv[2] + std::wstring(L".enc")).c_str());
        }
        case L'x':
        {
            LOGI("encrypt file with password protected key");
            if (argc < 4) {
                LOGE("Usage: e <input file> <key file> [output file]");
                return 1;
            }
            return cryptOne.encryptFileWithPassKey(argv[2], argv[3], (argc == 5) ? argv[4] : (argv[2] + std::wstring(L".enc")).c_str());
        }
        case L'd':
        {
            LOGI("decrypt file");
            return cryptOne.decryptFile(argv[2], argv[3], (argc == 5) ? argv[4] : (argv[2] + std::wstring(L".dec")).c_str());
        }
        case L'l':
        {
            LOGI("decrypt file with password protected key");
            return cryptOne.decryptFileWithPassKey(argv[2], argv[3], (argc == 5) ? argv[4] : (argv[2] + std::wstring(L".dec")).c_str());
        }
        case L'g':
        {
            LOGI("generate key and store raw key to file");
            return cryptOne.generateKey(argv[2]);
        }
        case L'k':
        {
            LOGI("generate key and store password protected key to file");
            return cryptOne.generateKeyWithPass(argv[2]);
        }
        default:
                LOGE("unrecognized mode '%c'", argv[1][0]);
                return 1;
    }
}

