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

#include <chrono>

#ifndef WIN32
#include <string.h>
#endif

#define APP_VERSION     "1.0.6"

#define DEFAULT_ENCRPYTED_FILENAME      "crypt-one-data.tar.gz.enc"
#define DEFAULT_COMPRESSED_FILENAME     "crypt-one-data.tar.gz"

CryptOne cryptOne;

RetCode compressFolder(const char *folder, const std::string& compressedFile) {
    if (!FileTools::isFolderPresent(folder)) {
        LOGGER("Folder [%s] not found", folder);
        return eNotFound;
    }
    if (!cryptOne.isExternalStoragePresent()) {
        LOGGER("This operation requires external storage, plug in USB stick..");
        return eFatal;
    }

    LOGGER("Compressing folder [%s] to file [%s]..", folder, compressedFile.c_str());
    auto start_time = std::chrono::high_resolution_clock::now();

    std::string res = CryptOne::exec(("tar -czf " + compressedFile + std::string(" ") + folder).c_str());
    auto end_time = std::chrono::high_resolution_clock::now();
    auto time = end_time - start_time;
    // encrypt
    LOGGER("Compressed for %ld ms", time / std::chrono::milliseconds(1));

    return eOk;
}

int decryptAndDecompress(const std::string &inputFile) {
    std::string compressedFile =  DEFAULT_COMPRESSED_FILENAME;

    RetCode ret = cryptOne.decryptFileWithPassKey(inputFile,
        KEY_FILENAME,
        compressedFile);

    if (ret != eOk) {
        LOGGER("Could not decrypt file [%s]" , inputFile.c_str());
        return 1;
    }
    auto start_time = std::chrono::high_resolution_clock::now();

    LOGGER("Decompressing file [%s]", compressedFile.c_str());
    CryptOne::exec(("tar xf " + compressedFile).c_str());
    auto end_time = std::chrono::high_resolution_clock::now();
    auto time = end_time - start_time;
    LOGGER("Decompressed for %ld ms", time / std::chrono::milliseconds(1));

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

    /// START  [ GENERATE - KEY ]
    if (!strcmp(argv[1], "generate-key")) {
        LOGGER("Generate key and store to USB stick");

        if (!cryptOne.isExternalStoragePresent()) {
            LOGGER("This operation requires external storage, plug in USB stick..");
            return 1;
        }
        return cryptOne.generateKeyWithPass(KEY_FILENAME);
    }
    /// END  [ GENERATE - KEY ]

    /// START  [ PUSH ]
    if (!strcmp(argv[1], "push")) {
        LOGGER("Compress, encrypt and upload folder to cloud");
        if (argc < 4) {
            LOGGER("Usage: CryptOne push <folder> <cloud-id>");
            return 1;
        }
        const int cloudIndex = atoi(argv[3]);
        std::string cloudFolder = cryptOne.getCloudFolder(cloudIndex);
        if (cloudFolder.empty()) {
            LOGGER("Could not find configured cloud folder in config file with index [%s]" , argv[3]);
            return 1;
        }
        std::string compressedFile = COMPRESSED_FILE;
        RetCode ret = compressFolder(argv[2], compressedFile);
        if (ret != eOk) {
            LOGGER("Compressing folder [%s] failed", argv[2]);
            return 1;
        }
      
        LOGGER("Encrypting file [%s]..", compressedFile.c_str());
        RetCode r = cryptOne.encryptFileWithPassKey(compressedFile, KEY_FILENAME, DEFAULT_ENCRPYTED_FILENAME);
        if (r != eOk) {
            LOGGER("Failed to encrypt compressed folder");
            return 1;
        }
        std::string encryptedFile = compressedFile + ".enc";        
        return up(cloudIndex, encryptedFile);
    }
    /// END  [ PUSH ]

    /// START  [ UP ]
    if (!strcmp(argv[1], "up")) {
        LOGGER("Upload encrypted file to cloud");
        if (argc < 3) {
            LOGGER("Usage: CryptOne up <cloud-id> [file]");
            return 1;
        }

        return up(atoi(argv[2]), (argc>3) ? argv[3] : DEFAULT_ENCRPYTED_FILENAME);
    }
    /// END  [ UP ]

    /// START  [ PULL ]
    if (!strcmp(argv[1], "pull")) {
        LOGGER("Download, decrypt and decompress folder from cloud");
    
        if (argc < 3) {
            LOGGER("Usage: CryptOne pull <cloud-id> [file]");
            return 1;
        }

        if (!cryptOne.isExternalStoragePresent()) {
            LOGGER("This operation requires external storage, plug in USB stick..");
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
    /// END  [ PULL ]

    /// START  [ DOWN ]
    if (!strcmp(argv[1], "down")) {
        LOGGER("Download file from cloud");

        if (argc < 3) {
            LOGGER("Usage: CryptOne down <cloud-id> [file]");
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
    /// END [ DOWN ]

    /// START  [ DECRYPT ]
    if (!strcmp(argv[1], "decrypt")) {
        LOGGER("Decrypt file from cloud");
        std::string inputFile = (argc > 2) ? argv[2] : DEFAULT_ENCRPYTED_FILENAME;
        return decryptAndDecompress(inputFile);       
    }
    /// END  [ DECRYPT ]

    /// START  [ ENCRYPT ]
    if (!strcmp(argv[1], "encrypt")) {
        LOGGER("Compress and encrypt folder to file using key on external storage");

        if (argc < 3) {
            LOGGER("Usage: CryptOne encrypt <folder> [<output file>]");
            return 1;
        }

        std::string compressedFile = COMPRESSED_FILE;
        RetCode ret = compressFolder(argv[2], compressedFile);
        if (ret != eOk) {
            LOGGER("Compressing folder [%s] failed", argv[2]);
            return 1;
        }

        LOGGER("Encrypting file [%s]..", compressedFile.c_str());
        ret = cryptOne.encryptFileWithPassKey(compressedFile,
            KEY_FILENAME,
            argc>3 ? argv[3] : DEFAULT_ENCRPYTED_FILENAME);

        // now delete temporary compressed unencrypred file
        std::string res = CryptOne::exec(("del " + compressedFile).c_str());
        if (ret != eOk) {
            LOGGER("Failed to encrypt compressed folder");
            return 1;
        }
        return 0;
    }
    /// END  [ ENCRYPT ]

    return 0;
}

