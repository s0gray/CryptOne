

#include "Logger.h"
#include "CryptOne.h"

#include "Utils.h"

#ifndef WIN32
#include <string.h>
#endif

#define APP_VERSION     "1.0.2"
#define DEFAULT_ENCRPYTED_FILENAME      "crypt-one-data.tar.gz.enc"
#define DEFAULT_COMPRESSED_FILENAME     "crypt-one-data.tar.gz"

CryptOne cryptOne;

int decryptAndDecompress(const std::string &inputFile) {
    std::string compressedFile =  DEFAULT_COMPRESSED_FILENAME;

    ErrCode ret = cryptOne.decryptFileWithPassKey(inputFile,
        KEY_FILENAME,
        compressedFile);

    if (ret != eOk) {
        LOGE("Could not decrypt file [%s]", inputFile.c_str());
        return 1;
    }
    LOGI("Decompressing file [%s]", compressedFile.c_str());
    CryptOne::exec(("tar xf " + compressedFile).c_str());

    CryptOne::exec(("del " + compressedFile).c_str());
    return 0;
}

int up(int cloudIndex, const std::string& encryptedFile) {
//   const int cloudIndex = atoi(cloudIndexParam);
    std::string cloudFolder = cryptOne.getCloudFolder(cloudIndex);
    if (cloudFolder.empty()) {
        LOGE("Could not find configured cloud folder in config file with index [%d]", cloudIndex);
        return 1;
    }

    // copy
    LOGI("Uploading file [%s] to cloud #%d", encryptedFile.c_str(), cloudIndex);
    ErrCode r = Utils::copyFileA(encryptedFile, cloudFolder + "//" + encryptedFile);
    if (r != eOk) {
        LOGE("Failed to upload encrypted file to cloud");
        return 1;
    }
    return 0;
}

int main(int argc, char* argv[])
{
    LOG_INIT(2, "");
    LOGI("CryptOne v%s", APP_VERSION);

    ErrCode ret = cryptOne.initialize();
    if (ret != eOk) {
        LOGE("CryptOne initialize FAILED : %u", ret);
        return 1;
    }

    if (argc < 2) {
 /*       LOGE("Usage: [e|d|g|k] <input file> [<key file>] [output file]");
        LOGI("   e - encrypt file with raw key");
        LOGI("   d - decrypt file with raw key");
        LOGI("   g - generate raw key");
        LOGI("   k - generate password protected key");
        LOGI("   x - encrypt file with password protected key");
        LOGI("   l - decrypt file with password protected key");*/

        LOGE("Usage: [generate-key | push | pull | decrypt | encrypt] <folder/file>");
        LOGI("   generate-key - generate password encrpyted key and store on USB stick");

        LOGI("   push - encrypt and upload folder to cloud");
        LOGI("   pull - download and decrypt data from cloud");
        LOGI("   decrypt - decrypt data from cloud (file to file)");
        LOGI("   encrypt - encrypt folder for cloud (folder to file)");
        LOGI("   down - get encrypted data from cloud");
        LOGI("   up - upload encrypted data to cloud");
        return 1;
    }

    if (!strcmp(argv[1], "generate-key")) {
        LOGI("Generate key and store to USB");

        return cryptOne.generateKeyWithPass(KEY_FILENAME);
    }

    if (!strcmp(argv[1], "push")) {
        LOGI("Upload folder to cloud");
        if (argc < 4) {
            LOGI("Usage: Crypt1 push <folder> <cloud-id>");
            return 1;
        }
        const int cloudIndex = atoi(argv[3]);
        std::string cloudFolder = cryptOne.getCloudFolder(cloudIndex);
        if (cloudFolder.empty()) {
            LOGE("Could not find configured cloud folder in config file with index [%s]", argv[3]);
            return 1;
        }
        std::string compressedFile = COMPRESSED_FILE;

        LOGI("Compressing folder [%s] to file [%s]..", argv[2], compressedFile.c_str());
        std::string res = CryptOne::exec( ("tar -czf " + compressedFile + std::string(" ") + argv[2]).c_str());

        LOGI("Encrypting file [%s]..", compressedFile.c_str());
        ErrCode r = cryptOne.encryptFileWithPassKey(compressedFile, KEY_FILENAME, DEFAULT_ENCRPYTED_FILENAME);
        if (r != eOk) {
            LOGE("Failed to encrypt compressed folder");
            return 1;
        }
        std::string encryptedFile = compressedFile + ".enc";        
        return up(cloudIndex, encryptedFile);
    }

    if (!strcmp(argv[1], "up")) {
        LOGI("Upload encrypted file to cloud");
        if (argc < 3) {
            LOGI("Usage: Crypt1 up <cloud-id> [file]");
            return 1;
        }

        return up(atoi(argv[2]), (argc>3) ? argv[3] : DEFAULT_ENCRPYTED_FILENAME);
    }

    if (!strcmp(argv[1], "pull")) {
        LOGI("Download folder from cloud");

     
        if (argc < 3) {
             LOGI("Usage: Crypt1 pull <cloud-id> [file]");
             return 1;
        }
        const int cloudIndex = atoi(argv[2]);
        std::string cloudFolder = cryptOne.getCloudFolder(cloudIndex);
        if (cloudFolder.empty()) {
            LOGE("Could not find configured cloud folder in config file with index [%s]", argv[2]);
            return 1;
        }

       // std::string compressedFile = COMPRESSED_FILE;
        std::string encryptedFile = (argc < 4) ? DEFAULT_ENCRPYTED_FILENAME : argv[3];

        std::string sourceFile = cloudFolder + "//" + encryptedFile;
           ;
       LOGI("Downloading file [%s] from cloud ", sourceFile.c_str());
       ret = Utils::copyFileA(sourceFile, encryptedFile);
       if (ret != eOk) {
           LOGE("Can not download file from cloud #%d", cloudIndex);
           return 1;
       }

       return decryptAndDecompress(encryptedFile);
       /*
       LOGI("Decrypting file from cloud #%d", cloudIndex);
       ret = cryptOne.decryptFileWithPassKey(encryptedFile, 
             KEY_FILENAME, compressedFile);

       if (ret != eOk) {
           LOGE("Could not decrypt downloaded file [%ws]", encryptedFile.c_str());
           return 1;
       }
       LOGI("Decompressing file [%s]", compressedFile.c_str());
       CryptOne::exec(("tar xf " + compressedFile).c_str());
       return 0;*/
    }
    

    if (!strcmp(argv[1], "down")) {
        LOGI("Download file from cloud");

        if (argc < 3) {
            LOGI("Usage: Crypt1 down <cloud-id> [file]");
            return 1;
        }

        const int cloudIndex = atoi(argv[2]);
        std::string cloudFolder = cryptOne.getCloudFolder(cloudIndex);
        if (cloudFolder.empty()) {
            LOGE("Could not find configured cloud folder in config file with index [%s]", argv[2]);
            return 1;
        }

        std::string encryptedFile = (argc<4) ? DEFAULT_ENCRPYTED_FILENAME : argv[3];
        std::string sourceFile = cryptOne.getCloudFolder(cloudIndex) + "//" + encryptedFile;
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
        std::string inputFile = (argc > 2) ? argv[2] : DEFAULT_ENCRPYTED_FILENAME;
        return decryptAndDecompress(inputFile);       
    }

    if (!strcmp(argv[1], "encrypt")) {
        LOGI("Encrypt file for cloud");

        if (argc < 3) {
            LOGI("Usage: Crypt1 encrypt <folder> [<output file>]");
            return 1;
        }

        std::string compressedFile = COMPRESSED_FILE;
        LOGI("Compressing folder [%s] to file [%s]..", argv[2], compressedFile.c_str());
        std::string res = CryptOne::exec(("tar -czf " + compressedFile + std::string(" ") + argv[2]).c_str());
        // encrypt

        LOGI("Encrypting file [%s]..", compressedFile.c_str());
        ret = cryptOne.encryptFileWithPassKey(compressedFile,
            KEY_FILENAME,
            argc>3 ? argv[3] : DEFAULT_ENCRPYTED_FILENAME);

        // now delete temporary compressed unencrypred file
        res = CryptOne::exec(("del " + compressedFile).c_str());
        LOGI("[%s]", res.c_str());

        if (ret != eOk) {
            LOGE("Failed to encrypt compressed folder");
            return 1;
        }
        return 0;
    }

  /*  switch (argv[1][0])
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
    }*/
}

