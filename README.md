
# CryptOne

Simple command line tool for folder encryption/decryption and uploading/downloading to cloud.
Generation of private key on external storage (USB stick)

Supported OS: Windows and Linux

Supported Cloud: Apple iCloud, Google Drive, Microsoft One Drive


# Quick start

1. Create 'CryptOne.ini' file (see example below) in some folder
2. Create subfolder which you want to put encrypted to clouds
3. Insert USB stick
4. Generate key:  _CryptOne generate-key_
5. Encrypt folder to file: _CryptOne encrypt subfolder_
6. Upload encrypted archive to cloud #0  _CryptOne up 0_
7. Upload encrypted archive to cloud #1 _CryptOne up 1_
8. Later download archive from file from cloud #0 by _CryptOne down 0_
9. Decrypt file (having USB stick in) _CryptOne decrypt_


## Example config file _CryptOne.ini_ ##

```text

# folders mapped to cloud storage
# to upload encrypted file

# iCloud folder
cloudFolder1=~\iCloudDrive

# Google Drive folder
cloudFolder2=G:\My Drive

# Microsoft OneDrive folder
cloudFolder3=~\OneDrive

# folder with private key
# normally should be USB folder
# if not set - automatically find removable drive
#keyFolder=d:\
```
## Windows ##

C++

Visual Studio 2022 (x64 Debug/Release)

Uses libsodium  ( https://github.com/jedisct1/libsodium )

Open CryptOne.sln and build


## Linux ##

$ mkdir build

$ sudo apt-get install libncurses5-dev libsodium-dev


