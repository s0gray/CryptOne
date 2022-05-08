#include "CryptOne.h"

#include "Logger.h"
#include "Utils.h"

#include <iostream>

#ifdef WIN32
    #include <conio.h>
#else
    #include <curses.h>
    #include <string.h>
#endif

#define NONCE_SIZE      24

#ifdef WIN32
#define GETCH _getch
#else
#define GETCH getch
#endif


CryptOne::CryptOne() {    
}

CryptOne::~CryptOne() {
}

ErrCode CryptOne::initialize() {
    ASSERTME( cryptUnit.selfTest() );
    ASSERTME( loadConfig() );
    ASSERTME( configFile.getValue(KEY_FOLDER_KEY, mKeyFolder) );
    ASSERTME( configFile.getValue(CLOUD_FOLDER_KEY, mCloudFolder ));

    return eOk;
}

ErrCode CryptOne::loadConfig() {
    return (configFile.Load() == 0) ? eOk : eFatal;
}

ErrCode CryptOne::loadConfig(const char* folder) {
    return (configFile.Load(folder) == 0) ? eOk : eFatal;
}

// load, ask password, decrypt
ErrCode CryptOne::loadEncryptedKeyFromFile(const std::string& fileName, std::string& key) {
    std::string secretEncrypted;
    ErrCode ret = Utils::loadFileA(fileName, secretEncrypted);
    if (ret != eOk) {
        LOGE("Can not load encrypted key from file [%ws], keyFile");
        return ret;
    }
    std::string password;

    std::cout << "Please enter password for encryption of key: ";
    password = enterPassword();
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

CStringA CryptOne::exec(const char* cmd)
{
    CStringA strResult;
    HANDLE hPipeRead, hPipeWrite;

    SECURITY_ATTRIBUTES saAttr = { sizeof(SECURITY_ATTRIBUTES) };
    saAttr.bInheritHandle = TRUE; // Pipe handles are inherited by child process.
    saAttr.lpSecurityDescriptor = NULL;

    // Create a pipe to get results from child's stdout.
    if (!CreatePipe(&hPipeRead, &hPipeWrite, &saAttr, 0))
        return strResult;

    STARTUPINFOA si = { sizeof(STARTUPINFOW) };
    si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
    si.hStdOutput = hPipeWrite;
    si.hStdError = hPipeWrite;
    si.wShowWindow = SW_HIDE; // Prevents cmd window from flashing.
                              // Requires STARTF_USESHOWWINDOW in dwFlags.

    PROCESS_INFORMATION pi = { 0 };

    BOOL fSuccess = CreateProcessA(NULL, (LPSTR)cmd, NULL, NULL, TRUE, CREATE_NEW_CONSOLE,
        NULL, NULL, &si, &pi);
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


ErrCode CryptOne::generateKeyWithPass(const std::string& fileName) {
    std::string outputFileName = getKeyFolder() + "//" +fileName;

    std::string secret;
    ErrCode ret = cryptUnit.generateSecretKey(secret);
    ASSERTME(ret);

    LOGI("generateSecretKey ret %u, len = %u", ret, cryptUnit.getSecretKeyLength());
    LOG_DATA(2, "secret", secret.c_str(), secret.size());

    std::string password;

    std::cout << "Please enter password for encryption of key: ";
    password = enterPassword();

    std::string hashed;
    ret = cryptUnit.hashDataSHA256(password, hashed);
    if (ret != eOk) {
        LOGE("Can not Hash password");
        return ret;
    }
    LOG_DATA(2, "password hash", hashed.c_str(), hashed.size());

    std::string xored;
    ret = cryptUnit.xorData(secret, hashed, secret.size(), xored);
    if (ret != eOk) {
        LOGE("Can not Xor password");
        return ret;
    }
    LOG_DATA(2, "xored key", xored.c_str(), xored.size());

    ret = Utils::writeFileA(outputFileName, xored);
    if (ret == eOk) {
        LOGI("Written %u bytes to [%ws]", cryptUnit.getSecretKeyLength(), outputFileName);
    }
    else {
        LOGE("Can not write data to [%ws]", outputFileName);
    }
    return ret;
}


ErrCode CryptOne::generateKey(const char* outputFileName) {
    std::string secret;
    ErrCode ret = cryptUnit.generateSecretKey(secret);

    LOGI("generateSecretKey ret %u, len = %u", ret, secret.size());
    ASSERTME(ret);

    LOG_DATA(2, "secret", secret.c_str(), secret.size());
    ret = Utils::writeFileA(outputFileName, secret);

    if (ret == eOk) {
        LOGI("Written %u bytes to [%ws]", cryptUnit.getSecretKeyLength(), outputFileName);
    }
    else {
        LOGE("Can not write data to [%ws]", outputFileName);
    }

    return ret;
}

ErrCode CryptOne::encryptFileWithPassKey(
                                        const std::string& inputFile, 
                                        const std::string& keyFileName, 
                                        const std::string& outputFileName) {
    std::string plain;

    ASSERTME( Utils::loadFileA(inputFile, plain));
    LOGI("Loaded %u bytes from [%s]", plain.size(), inputFile);

    std::string keyFile = mKeyFolder + std::string("//") + keyFileName;
    std::string plainKey;
    ASSERTME( loadEncryptedKeyFromFile(keyFile, plainKey));

    size_t encryptedSize = plain.size() + 64;
    std::string encrypted;

    CryptHeader header;
    header.plainDataSize = plain.size();
    std::string nonce;

    ASSERTME( cryptUnit.encryptDataSymmetric(plain, encrypted, nonce, plainKey) );
    memcpy(header.nonce, nonce.c_str(), nonce.size());

    std::string outFileData;

    outFileData.assign((const char*)&header, sizeof(header));
    outFileData += encrypted;

    ASSERTME( Utils::writeFileA(outputFileName, outFileData) );

    LOGI("Stored %u bytes to [%s]", outFileData.size(), outputFileName);
    return eOk;
}


ErrCode CryptOne::encryptFile(const char* inputFile, const char* keyFile, const char* outputFileName) {
    std::string data;
    ASSERTME( Utils::loadFileA(inputFile, data) );

    LOGI("Loaded %u bytes from [%s]", data.size(), inputFile);

    std::string secret;
    ASSERTME( Utils::loadFileA(keyFile, secret) );

    std::string encrypted, nonce;
    ASSERTME( cryptUnit.encryptDataSymmetric(data, secret, encrypted, nonce) );

    CryptHeader header;
    header.plainDataSize = data.size();
    memcpy(&header.nonce, nonce.c_str(), NONCE_SIZE);

    std::string outFileData;

    outFileData.assign((const char*)&header, sizeof(header));
    outFileData += encrypted;

    ASSERTME( Utils::writeFileA(outputFileName, outFileData));

    LOGI("Stored %u bytes to [%s]", outFileData.size(), outputFileName);
    return eOk;
}

ErrCode CryptOne::decryptFileWithPassKey(const std::string& inputFile, const std::string& keyFile, const std::string& outputFileName) {
    std::string data;
    ASSERTME( Utils::loadFileA(inputFile, data) );
    LOGI("Loaded %u bytes from [%s]", data.size(), inputFile);

    if (data.size() < sizeof(CryptHeader)) {
        LOGE("Too small input file : %u bytes", data.size());
        return eBadFile;
    }

    std::string plainKey;
    ASSERTME( loadEncryptedKeyFromFile(keyFile, plainKey) );

    CryptHeader* header = (CryptHeader*)data.c_str();
    const byte* encrypted = (const byte*)data.c_str() + sizeof(CryptHeader);
    size_t encryptedSize = data.size() - sizeof(CryptHeader);

    BinaryData decrypted;
    size_t decryptedSize = header->plainDataSize;

    ASSERTME( cryptUnit.decryptDataSymmetric(decrypted,
        std::string((const char*)encrypted, encryptedSize),
        std::string((const char*)header->nonce, NONCE_SIZE),
        plainKey,
        decryptedSize));

    ASSERTME( Utils::writeFileA(outputFileName, decrypted));

    return eOk;
}

ErrCode  CryptOne::decryptFile(const char* inputFile, const char* keyFile, const char* outputFileName) {
    std::string data;
    ASSERTME( Utils::loadFileA(inputFile, data) );

    LOGI("Loaded %u bytes from [%s]", data.size(), inputFile);

    if (data.size() < sizeof(CryptHeader)) {
        LOGE("Too small input file : %u bytes", data.size());
        return eBadFile;
    }

    std::string secret;
    ASSERTME( Utils::loadFileA(keyFile, secret) );

    CryptHeader* header = (CryptHeader*)data.c_str();
    const byte* encrypted = (const byte*)data.c_str() + sizeof(CryptHeader);
    size_t encryptedSize = data.size() - sizeof(CryptHeader);

    BinaryData decrypted;
    size_t decryptedSize = header->plainDataSize;

    ASSERTME(   cryptUnit.decryptDataSymmetric(decrypted,
                std::string((const char*)encrypted, encryptedSize),
                std::string((const char*)header->nonce, NONCE_SIZE),
                secret,
                decryptedSize
    ));

    ASSERTME( Utils::writeFileA(outputFileName, decrypted));

    return eOk;
}

std::string CryptOne::enterPassword() {
    std::string pass = "";
    char ch;
  //  std::cout << "Enter password: ";
    ch = GETCH();
    while (ch != 13) {//character 13 is enter
        pass.push_back(ch);
        std::cout << '*';
        ch = GETCH();
    }
    std::cout << "\n";
    return pass;
}