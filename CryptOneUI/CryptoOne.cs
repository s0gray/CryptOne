using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CryptOneService
{
    public class CryptoOne
    {
        public Crypto crypto = new Crypto();
        public static string tempFolder = null;

        public static string DIR_HASH_KEY = "dirHash";
        public static string TGZ_HASH_KEY = "tgzHash";
        public static string ENC_HASH_KEY = "encHash";
        string pass = null;


        public CryptoOne()
        {
            crypto.selfTest();

        }

        /**
         * Compress, encrypt, upload
         */

        public void push(MonitoredFolder monitoredFolder, CloudFolder cloudFolder, string keyFile)
        {
            Log.Line("push ["+ monitoredFolder.path + "] ["+cloudFolder.folder+ "] keyFile = [" + keyFile+"]");

            string infoFilename = tempFolder + monitoredFolder.getInfoFileName();
            var existingInfoFile = monitoredFolder.loadInfoFile();
            if(existingInfoFile != null)
            {
                Log.Line("Loaded .info file " + infoFilename);
            } else
            {
                Log.Line(".info file not found at " + infoFilename);
            }

            string folderHash = Tools.calculateFolderHash(monitoredFolder.path);
            Log.Line("Current hash of ["+ monitoredFolder.path + "] is ["+folderHash+"]");
            bool reuseExistingArchive = false;
            bool reuseExistingEncryptedArchive = false;
            string encryptedTgz = "";
            string encTgzHash = null;

            if (existingInfoFile != null)
            {
                string oldFolderHash = existingInfoFile[DIR_HASH_KEY];
                Log.Line("Old folder hash is " + oldFolderHash);

                if(oldFolderHash != null && folderHash.Equals(oldFolderHash))
                {
                    Log.Line("Files are not changed in folder ["+ monitoredFolder.path + "] we can reuse archive");

                    string oldArchiveHash = existingInfoFile[TGZ_HASH_KEY];
                    Log.Line("Old archive hash is " + oldArchiveHash);

                    string exsistingArchiveHash = Tools.calculateFileHash(monitoredFolder.getFullArchiveFileName());
                    if(exsistingArchiveHash!=null && exsistingArchiveHash.Equals(oldArchiveHash))
                    {
                        Log.Line("Archive not changed");
                        reuseExistingArchive = true;

                        string oldEncrpytedArchiveHash = existingInfoFile[ENC_HASH_KEY];
                        Log.Line("Old encrypted archive hash is " + oldEncrpytedArchiveHash);

                        encTgzHash = Tools.calculateFileHash(monitoredFolder.getFullEncryptedArchiveFileName());
                        Log.Line("Existing encrypted archive hash is " + encTgzHash);

                        if (encTgzHash != null && encTgzHash.Equals(oldEncrpytedArchiveHash))
                        {
                            Log.Line("Encrypted archive not changed");
                            reuseExistingEncryptedArchive = true;

                            encryptedTgz = monitoredFolder.getFullEncryptedArchiveFileName();
                        }
                    }
                }
            }

            string tgzFile = monitoredFolder.getFullArchiveFileName();
            if (!reuseExistingArchive)
            {
                long startTime = DateTimeOffset.Now.ToUnixTimeMilliseconds();
                Tools.CreateTGZ(monitoredFolder.path, monitoredFolder.getArchiveFileNameWithoutExtension(), tempFolder);
                long endTime = DateTimeOffset.Now.ToUnixTimeMilliseconds();

                Log.Line("Created TGZ: [" + tgzFile + "] for " + (endTime - startTime) / 1000 + "s");
            }

            if (!reuseExistingEncryptedArchive)
            {
                // encrypt
                encryptedTgz = encryptFileWithPassKey(tgzFile, keyFile, getPass(), tgzFile + ".enc");
                Log.Line("Created encrypted TGZ: " + encryptedTgz);

                string tgzHash = Tools.calculateFileHash(tgzFile);
                Log.Line("Hash of TGZ is " + tgzHash);
                encTgzHash = Tools.calculateFileHash(encryptedTgz);
                Log.Line("Hash of encTGZ is " + encTgzHash);

                var info = new Dictionary<string, string>();
                info.Add(DIR_HASH_KEY, folderHash);
                info.Add(TGZ_HASH_KEY, tgzHash);
                info.Add(ENC_HASH_KEY, encTgzHash);

                Tools.createInfoFile(infoFilename, info);
            }

            string targetFilename = Path.GetFileName(encryptedTgz);
            Log.Line("targetFilename = " + targetFilename);

            // upload
            if (encryptedTgz != null && encryptedTgz.Length > 0)
            {
                string fullTarget = cloudFolder.fullPath + targetFilename;
                try
                {                    
                    Tools.smartCopyFile(encryptedTgz, encTgzHash, fullTarget);
                } catch(IOException e)
                {
                    Log.Line("Error during copy: " + e.ToString());
                }
            } else
            {
                Log.Line("encryptFileWithPassKey failed");
            }
        }

        /**
         * encrypt 'inputFile' to 'outputFile' using key in 'keyFile'
         */
        public string encryptFileWithPassKey(string inputFile, string keyFile, string pass, string outputFile)
        {
            Log.Line("encryptFileWithPassKey inputFile=" + inputFile + " "+keyFile+ " outputFile=" + outputFile);

            byte[] inputData = File.ReadAllBytes(inputFile);
            if(inputData==null)
            {
                Log.Line("File not loaded " + inputFile);

                return "";
            }
            Log.Line("Loaded "+ inputData.Length+" bytes from " + inputFile);

            byte[] key = crypto.loadEncryptedKey(keyFile, pass);
            if(key==null)
            {
                Log.Line("Can not load encrypted key from " + keyFile);
                return "";
            }
            Log.Line("Loaded key from " + keyFile);

            byte[] encryptedData = crypto.encryptAES256(key, inputData);
            if (encryptedData == null)
            {
                Log.Line("Can not encrypt data");
                return "";
            }
            Log.Line("Encrypted data size is " + encryptedData.Length + " bytes");

            File.WriteAllBytes(outputFile, encryptedData);
            return outputFile;
        }
        string getPass()
        {
            if (pass != null) return pass;
            pass = Tools.AskPassword();
            return pass;
        }
        public void generateEncryptedKeyFile(string drive, string password)
        {
            byte[] secret = Crypto.generateSecureRandomBytes(32);
            Log.Data("secret", secret);
            byte[] salt = Crypto.generateSecureRandomBytes(32);
            Log.Data("salt", salt);

            byte[] material = Tools.concat(salt, Encoding.ASCII.GetBytes(password));
            Log.Data("material", material);

            byte[] hashed = Crypto.hashSHA256bytes(material);
            Log.Data("hashed", hashed);

            byte[] encrypted = Crypto.xorArray(secret, hashed, secret.Length);
            Log.Data("encrypted", encrypted);

            byte[] result = Tools.concat(salt, encrypted);
            Log.Data("result", result);

            Tools.WriteFile(result, drive + Form1.KEY_FILENAME);
        }
    }
}
