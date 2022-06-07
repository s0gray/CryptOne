
# CryptOne

Simple command line tool for folder encryption/decryption and uploading/downloading to cloud.
Generation of private key on external storage (USB stick)
Supported OSes: Windows and Linux, 
Supported Clouds: Apple iCloud, Google Drive, Microsoft One Drive


# Quick start

1. Create 'Crypt1.ini' file (see example below) in some folder
2. Create subfolder which you want to put encrypted to clouds
3. Insert USB stick
4. Generate key:  _Crypt1 generate-key_
5. Encrypt folder to file: _Crypt1 encrypt subfolder_
6. Upload encrypted archive to cloud #0  _Crypt1 up 0_
7. Upload encrypted archive to cloud #1 _Crypt1 up 1_
8. Later download archive from file from cloud #0 by _Crypt1 down 0_
9. Decrypt file (having USB stick in) _Crypt1 decrypt_


## Example config file ##

```text

# folders mapped to cloud storage
# to upload encrypted file

# iCloud folder
cloudFolder1=~\iCloudDrive

# Google Drive folder
cloudFolder2=G:\My Drive

# Microsoft One Driver folder
cloudFolder3=~\OneDrive

# folder with private key
# normally should be USB folder
# if not set - automatically find removable drive
#keyFolder=d:\
```
## Windows ##

C++

Visual Studio 2022 (x64 Debug/Release)

Uses libsodium


Can generate raw key, password protected key ( XOR with SHA256(password) )

Can encrypt / decrypt files with plain or encrypted keys.

Upload / download encrypted package to cloud.



## Linux ##

$ mkdir build

$ sudo apt-get install libncurses5-dev libsodium-dev


