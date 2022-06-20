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

        public CryptoOne()
        {
            crypto.selfTest();

        }

        /**
         * Compress, encrypt, upload
         */

        public void push(MonitoredFolder monitoredFolder, CloudFolder cloudFolder, string keyFile, string pass)
        {
            Log.Line("push ["+ monitoredFolder.path + "] ["+cloudFolder.folder+ "] keyFile = [" + keyFile+"]");

            long startTime = DateTimeOffset.Now.ToUnixTimeMilliseconds();
            string tgzFile = Tools.CreateTGZ(monitoredFolder.path, monitoredFolder.getArchiveFileName(), Path.GetTempPath());
            long endTime = DateTimeOffset.Now.ToUnixTimeMilliseconds();

            Log.Line("Created TGZ: [" + tgzFile + "] for " + (endTime - startTime)/1000 + "s");

            // encrypt
            string encryptedTgz = encryptFileWithPassKey(tgzFile, keyFile, pass, tgzFile + ".enc");
            Log.Line("Created encrypted TGZ: " + encryptedTgz);

            string targetFilename = Path.GetFileName(encryptedTgz);
            Log.Line("targetFilename = " + targetFilename);
            // upload
            if (encryptedTgz != null && encryptedTgz.Length > 0)
            {
                string fullTarget = cloudFolder.fullPath + targetFilename;
                try
                {                    
                    Tools.copyFile(encryptedTgz, fullTarget);
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
