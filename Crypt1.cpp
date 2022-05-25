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

#include "Tools.h"
#include "FileTools.h"
#include "Log.h"

#ifndef WIN32
#include <string.h>
#endif

#define APP_VERSION     "1.0.4"

#define DEFAULT_ENCRPYTED_FILENAME      "crypt-one-data.tar.gz.enc"
#define DEFAULT_COMPRESSED_FILENAME     "crypt-one-data.tar.gz"

CryptOne cryptOne;


int decryptAndDecompress(const std::string &inputFile) {
    std::string compressedFile =  DEFAULT_COMPRESSED_FILENAME;

    RetCode ret = cryptOne.decryptFileWithPassKey(inputFile,
        KEY_FILENAME,
        compressedFile);

    if (ret != eOk) {
        LOGGER("Could not decrypt file [%s]" , inputFile.c_str());
        return 1;
    }
    LOGGER("Decompressing file [%s]", compressedFile.c_str());
    CryptOne::exec(("tar xf " + compressedFile).c_str());
    CryptOne::exec((Tools::getDeleteFileCommand() + " " + compressedFile).c_str());
    return 0;
}

int up(int cloudIndex, const std::string& encryptedFile) {
    std::string cloudFolder = cryptOne.getCloudFolder(cloudIndex);
    if (cloudFolder.empty()) {
        LOGGER("Could not find configured cloud folder in config file with index [%d]" , cloudIndex);
        return 1;
    }

    // copy
    LOGGER("Uploading file [%s] to cloud #%d", encryptedFile.c_str(), cloudIndex);
    RetCode r = FileTools::copyFileA(encryptedFile, cloudFolder + Tools::getPathSeparator() + encryptedFile);
    if (r != eOk) {
        LOGGER("Failed to upload encrypted file to cloud");
        return 1;
    }
    return 0;
}

/**
*   Entry Point
*/
int main(int argc, char* argv[])
{
    LOGGER( " --== CryptOne %s ==-- ", APP_VERSION);

    RetCode ret = cryptOne.initialize();
    if (ret != eOk) {
        LOGGER("CryptOne initialize FAILED");
        return 1;
    }

    if (argc < 2) {
 /*       LOGGER("Usage: [e|d|g|k] <input file> [<key file>] [output file]");
        LOGGER("   e - encrypt file with raw key");
        LOGGER("   d - decrypt file with raw key");
        LOGGER("   g - generate raw key");
        LOGGER("   k - generate password protected key");
        LOGGER("   x - encrypt file with password protected key");
        LOGGER("   l - decrypt file with password protected key");*/

        LOGGER("Usage: [generate-key | push | pull | decrypt | encrypt] <folder/file>");
        LOGGER("   generate-key - generate password encrpyted key and store on USB stick");

        LOGGER("   push - encrypt and upload folder to cloud");
        LOGGER("   pull - download and decrypt data from cloud");
        LOGGER("   decrypt - decrypt data from cloud (file to file)");
        LOGGER("   encrypt - encrypt folder for cloud (folder to file)");
        LOGGER("   down - get encrypted data from cloud");
        LOGGER("   up - upload encrypted data to cloud");
        return 1;
    }

    if (!strcmp(argv[1], "generate-key")) {
        LOGGER("Generate key and store to USB");

        return cryptOne.generateKeyWithPass(KEY_FILENAME);
    }

    if (!strcmp(argv[1], "push")) {
        LOGGER("Upload folder to cloud");
        if (argc < 4) {
            LOGGER("Usage: Crypt1 push <folder> <cloud-id>");
            return 1;
        }
        const int cloudIndex = atoi(argv[3]);
        std::string cloudFolder = cryptOne.getCloudFolder(cloudIndex);
        if (cloudFolder.empty()) {
            LOGGER("Could not find configured cloud folder in config file with index [%s]" , argv[3]);
            return 1;
        }
        std::string compressedFile = COMPRESSED_FILE;

        LOGGER("Compressing folder [%s] to file [%s]..", argv[2], compressedFile.c_str());
        std::string res = CryptOne::exec( ("tar -czf " + compressedFile + std::string(" ") + argv[2]).c_str());

        LOGGER("Encrypting file [%s]..", compressedFile.c_str());
        RetCode r = cryptOne.encryptFileWithPassKey(compressedFile, KEY_FILENAME, DEFAULT_ENCRPYTED_FILENAME);
        if (r != eOk) {
            LOGGER("Failed to encrypt compressed folder");
            return 1;
        }
        std::string encryptedFile = compressedFile + ".enc";        
        return up(cloudIndex, encryptedFile);
    }

    if (!strcmp(argv[1], "up")) {
        LOGGER("Upload encrypted file to cloud");
        if (argc < 3) {
            LOGGER("Usage: Crypt1 up <cloud-id> [file]");
            return 1;
        }

        return up(atoi(argv[2]), (argc>3) ? argv[3] : DEFAULT_ENCRPYTED_FILENAME);
    }

    if (!strcmp(argv[1], "pull")) {
        LOGGER("Download folder from cloud");
    
        if (argc < 3) {
            LOGGER("Usage: Crypt1 pull <cloud-id> [file]");
            return 1;
        }
        const int cloudIndex = atoi(argv[2]);
        std::string cloudFolder = cryptOne.getCloudFolder(cloudIndex);
        if (cloudFolder.empty()) {
            LOGGER("Could not find configured cloud folder in config file with index [%s]", argv[2]);
            return 1;
        }
        std::string encryptedFile = (argc < 4) ? DEFAULT_ENCRPYTED_FILENAME : argv[3];
        std::string sourceFile = cloudFolder + "//" + encryptedFile;
           
       LOGGER("Downloading file [%s] from cloud ", sourceFile.c_str());
       ret = FileTools::copyFileA(sourceFile, encryptedFile);
       if (ret != eOk) {
           LOGGER("Can not download file from cloud #%d", cloudIndex);
           return 1;
       }

       return decryptAndDecompress(encryptedFile);
    }
  

    if (!strcmp(argv[1], "down")) {
        LOGGER("Download file from cloud");

        if (argc < 3) {
            LOGGER("Usage: Crypt1 down <cloud-id> [file]");
            return 1;
        }

        const int cloudIndex = atoi(argv[2]);
        std::string cloudFolder = cryptOne.getCloudFolder(cloudIndex);
        if (cloudFolder.empty()) {
            LOGGER("Could not find configured cloud folder in config file with index %s" , argv[2]);
            return 1;
        }

        std::string encryptedFile = (argc<4) ? DEFAULT_ENCRPYTED_FILENAME : argv[3];
        std::string sourceFile = cryptOne.getCloudFolder(cloudIndex) + "//" + encryptedFile;
        LOGGER("Downloading file [%s] from cloud", sourceFile.c_str());
        ret = FileTools::copyFileA(sourceFile, encryptedFile);
        if (ret != eOk) {
            LOGGER("Can not download file from cloud");
            return 1;
        }       
        return 0;
    }

    if (!strcmp(argv[1], "decrypt")) {
        LOGGER("Decrypt file from cloud");
        std::string inputFile = (argc > 2) ? argv[2] : DEFAULT_ENCRPYTED_FILENAME;
        return decryptAndDecompress(inputFile);       
    }

    if (!strcmp(argv[1], "encrypt")) {
        LOGGER("Encrypt file for cloud");

        if (argc < 3) {
            LOGGER("Usage: Crypt1 encrypt <folder> [<output file>]");
            return 1;
        }

        std::string compressedFile = COMPRESSED_FILE;
        LOGGER("Compressing folder [%s] to file [%s]..", argv[2], compressedFile.c_str());
        std::string res = CryptOne::exec(("tar -czf " + compressedFile + std::string(" ") + argv[2]).c_str());
        // encrypt

        LOGGER("Encrypting file [%s]..", compressedFile.c_str());
        ret = cryptOne.encryptFileWithPassKey(compressedFile,
            KEY_FILENAME,
            argc>3 ? argv[3] : DEFAULT_ENCRPYTED_FILENAME);

        // now delete temporary compressed unencrypred file
        res = CryptOne::exec(("del " + compressedFile).c_str());
        //LOGGER("[%s]", res.c_str());

        if (ret != eOk) {
            LOGGER("Failed to encrypt compressed folder");
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
            LOGGER("encrypt file with password protected key");
            if (argc < 4) {
                LOGE("Usage: e <input file> <key file> [output file]");
                return 1;
            }
            return cryptOne.encryptFileWithPassKey(argv[2], argv[3], (argc == 5) ? argv[4] : (argv[2] + std::string(".enc")).c_str());
        }
        case L'd':
        {
            LOGGER("decrypt file");
            return cryptOne.decryptFile(argv[2], argv[3], (argc == 5) ? argv[4] : (argv[2] + std::string(".dec")).c_str());
        }
        case L'l':
        {
            LOGGER("decrypt file with password protected key");
            return cryptOne.decryptFileWithPassKey(argv[2], argv[3], (argc == 5) ? argv[4] : (argv[2] + std::string(".dec")).c_str());
        }
        case L'g':
        {
            LOGGER("generate key and store raw key to file");
            return cryptOne.generateKey(argv[2]);
        }
        case L'k':
        {
            LOGGER("generate key and store password protected key to file");
            return cryptOne.generateKeyWithPass(argv[2]);
        }
        default:
                LOGE("unrecognized mode '%c'", argv[1][0]);
                return 1;
    }*/
}

