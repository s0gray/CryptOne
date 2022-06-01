# CryptOne

Simple command line tool for folder encryption/decryption and uploading/downloading to cloud.
Generation of private key on external storage (USB stick)
Support Windows and Linux


** Windows **

C++

Visual Studio 2022 (x64 Debug/Release)

Uses libsodium


Can generate raw key, password protected key ( XOR with SHA256(password) )

Can encrypt / decrypt files with plain or encrypted keys.

Upload / download encrypted package to cloud.



** Linux **

$ mkdir build

$ sudo apt-get install libncurses5-dev libsodium-dev
