

#include "Logger.h"
#include "CryptOne.h"

#include "Utils.h"


int main(int argc, char* argv[])
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

    if (!strcmp(argv[1], "generate-key")) {
        LOGI("Generate key and store to USB");

        return cryptOne.generateKeyWithPass(KEY_FILENAME);
    }

    if (!strcmp(argv[1], "upload")) {
        LOGI("Upload folder to cloud");
        if (argc < 3) {
            LOGI("Usage: Crypt1 upload <folder>");
            return 1;
        }

        std::string compressedFile = COMPRESSED_FILE;

        LOGI("Compressing folder [%s] to file [%s]..", argv[2], compressedFile.c_str());
        CStringA res = CryptOne::exec( ("tar -czf " + compressedFile + std::string(" ") + argv[2]).c_str());
        // encrypt

        LOGI("Encrypting file [%ws]..", compressedFile.c_str());

        ErrCode r = cryptOne.encryptFileWithPassKey(compressedFile, KEY_FILENAME, "crypt-one-data.tar.gz.enc");
        if (r != eOk) {
            LOGE("Failed to encrypt compressed folder");
            return 1;
        }

        std::string encryptedFile = compressedFile + ".enc";
        // copy
        LOGI("Uploading file [%ws] to cloud", encryptedFile.c_str());
        r = Utils::copyFileA(encryptedFile, cryptOne.getCloudFolder() + "//crypt-one-data.tar.gz.enc");
        if (r != 0) {
            LOGE("Failed to upload encrypted file to cloud");
            return 1;
        }
        return 0;
    }

    if (!strcmp(argv[1], "up")) {
        LOGI("Upload encrypted file to cloud");
        if (argc < 3) {
            LOGI("Usage: Crypt1 up <file>");
            return 1;
        }

        std::string encryptedFile = argv[2];
        // copy
        LOGI("Uploading file [%ws] to cloud", encryptedFile.c_str());
        ErrCode r = Utils::copyFileA(encryptedFile, cryptOne.getCloudFolder() + "//crypt-one-data.tar.gz.enc");
        if (r != eOk) {
            LOGE("Failed to upload encrypted file to cloud");
            return 1;
        }
        return 0;
    }

    if (!strcmp(argv[1], "download")) {
        LOGI("Download folder from cloud");

        std::string compressedFile = COMPRESSED_FILE;
        std::string encryptedFile = compressedFile + ".enc";

       std::string sourceFile = cryptOne.getCloudFolder() + "//" + encryptedFile;
       LOGI("Downloading file [%s] from cloud ", sourceFile.c_str());
       ret = Utils::copyFileA(sourceFile, encryptedFile);
       if (ret != eOk) {
           LOGE("Can not download file from cloud");
           return 1;
       }
       LOGI("Decrypting file from cloud");
       ret = cryptOne.decryptFileWithPassKey(encryptedFile, 
             KEY_FILENAME, compressedFile);

       if (ret != eOk) {
           LOGE("Could not decrypt downloaded file [%ws]", encryptedFile.c_str());
           return 1;
       }
       LOGI("Decompressing file [%s]", compressedFile.c_str());
       CryptOne::exec(("tar xf " + compressedFile).c_str());
       return 0;
    }

    if (!strcmp(argv[1], "down")) {
        LOGI("Download file from cloud");

        std::string compressedFile = COMPRESSED_FILE;
        std::string encryptedFile = compressedFile + ".enc";


        std::string sourceFile = cryptOne.getCloudFolder() + "//" + encryptedFile;
        LOGI("Downloading file [%s] from cloud ", sourceFile.c_str());
        ret = Utils::copyFileA(sourceFile, encryptedFile);
        if (ret != eOk) {
            LOGE("Can not download file from cloud");
            return 1;
        }       
        return 0;
    }

    if (!strcmp(argv[1], "decrypt")) {
        LOGI("Decrypt file from cloud");

        if (argc < 3) {
            LOGI("Usage: Crypt1 decrypt <file>");
            return 1;
        }

        std::string compressedFile = COMPRESSED_FILE;
        ret = cryptOne.decryptFileWithPassKey(argv[2],
             KEY_FILENAME, compressedFile);

        if (ret != eOk) {
            LOGE("Could not decrypt file [%ws]", compressedFile.c_str());
            return 1;
        }
        LOGI("Decompressing file [%s]", compressedFile.c_str());
        CryptOne::exec(("tar xf " + compressedFile).c_str());
        return 0;
    }

    if (!strcmp(argv[1], "encrypt")) {
        LOGI("Encrypt file for cloud");

        if (argc < 3) {
            LOGI("Usage: Crypt1 encrypt <folder>");
            return 1;
        }


        std::string compressedFile = COMPRESSED_FILE;
        LOGI("Compressing folder [ws] to file [%s]..", argv[2], compressedFile.c_str());
        CStringA res = CryptOne::exec(("tar -czf " + compressedFile + std::string(" ") + argv[2]).c_str());
        // encrypt

        LOGI("Encrypting file [%s]..", compressedFile.c_str());
        ret = cryptOne.encryptFileWithPassKey(compressedFile,
            KEY_FILENAME,
            "crypt-one-data.tar.gz.enc");

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
            return cryptOne.encryptFile(argv[2], argv[3], (argc == 5) ? argv[4] : (argv[2] + std::string(".enc")).c_str());
        }
        case L'x':
        {
            LOGI("encrypt file with password protected key");
            if (argc < 4) {
                LOGE("Usage: e <input file> <key file> [output file]");
                return 1;
            }
            return cryptOne.encryptFileWithPassKey(argv[2], argv[3], (argc == 5) ? argv[4] : (argv[2] + std::string(".enc")).c_str());
        }
        case L'd':
        {
            LOGI("decrypt file");
            return cryptOne.decryptFile(argv[2], argv[3], (argc == 5) ? argv[4] : (argv[2] + std::string(".dec")).c_str());
        }
        case L'l':
        {
            LOGI("decrypt file with password protected key");
            return cryptOne.decryptFileWithPassKey(argv[2], argv[3], (argc == 5) ? argv[4] : (argv[2] + std::string(".dec")).c_str());
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

