using Org.BouncyCastle.Asn1.Pkcs;
using Org.BouncyCastle.Crypto;
using Org.BouncyCastle.Crypto.Digests;
using Org.BouncyCastle.Crypto.Engines;
using Org.BouncyCastle.Crypto.Macs;
using Org.BouncyCastle.Crypto.Modes;
using Org.BouncyCastle.Crypto.Paddings;
using Org.BouncyCastle.Crypto.Parameters;
using Org.BouncyCastle.Pkcs;
using Org.BouncyCastle.Security;
using Org.BouncyCastle.X509;
using System.Security.Cryptography;
using System.Text;


namespace CryptOneService
{
    public class Crypto
    {
        AesEngine engine = new AesEngine();
        byte[] iv = new byte[16];

        public Crypto()
        {
            for (int i = 0; i < 16; i++) iv[i] = 0;
        }
        public static string hashSHA256hex(string str)
        {
            byte[] hash = hashSHA256(str);
            return Tools.bytesToHex(hash);
        }


        public static byte[] hashSHA256(string str)
        {
            return hashSHA256bytes(Encoding.ASCII.GetBytes(str));
        }
        public static byte[] hashSHA256bytes(byte[] data)
        {
            SHA256 mySHA256 = SHA256.Create();
            byte[] hash = mySHA256.ComputeHash(data);
            return hash;
        }

        public byte[] loadEncryptedKey(string keyFile, string pass)
        {
            byte[] encryptedKeyData = File.ReadAllBytes(keyFile);
            if (encryptedKeyData == null) {
                Log.Line("Can not load file "+keyFile);
                return null; 
            }
//            Log.Data("Loaded encrypted key data", encryptedKeyData);

            byte[] salt = new byte[32];
            Array.Copy(encryptedKeyData, salt, 32);
//            Log.Data("Loaded salt", salt);

            byte[] xored = new byte[32];
            Array.Copy(encryptedKeyData, 32, xored, 0, 32);
//            Log.Data("Loaded xored key", salt);

            byte[] material = Tools.concat(salt, Encoding.ASCII.GetBytes(pass));
//            Log.Data("material", material);

            byte[] hashed = Crypto.hashSHA256bytes(material);
//            Log.Data("hashed", hashed);

            byte[] decrypted = Crypto.xorArray(xored, hashed, xored.Length);
//            Log.Data("decrypted", decrypted);

            return decrypted;
        }

        public static byte[] hashSHA1(string str)
        {
            SHA1 mySHA1 = SHA1.Create();
            byte[] hash = mySHA1.ComputeHash(Encoding.ASCII.GetBytes(str));
            return hash;
        }

        internal void selfTest()
        {
            byte[] key = generateSecureRandomBytes(32);
            byte[] msg = generateSecureRandomBytes(32);

            byte[] arr1 = encryptAES256(key, msg);
            byte[] arr2 = decryptAES256(key, arr1);

          //  Debug.WriteLine("msg = " + bytesToHex(msg));
          //  Debug.WriteLine("arr1 = " + bytesToHex(arr1));
          //  Debug.WriteLine("arr2 = " + bytesToHex(arr2));
        }

        public static string hashSHA1hex(string str)
        {
            byte[] hash = hashSHA1(str);
            return Tools.bytesToHex(hash);
        }

        /// <summary>
        /// Generates secure random byte array of set length
        /// </summary>
        /// <param name="len">length of return byte array</param>
        /// <returns>secure random byte array of set length</returns>
        public static byte[] generateSecureRandomBytes(int len)
        {
            var rand = new SecureRandom();
            var bytes = new byte[len];
            rand.NextBytes(bytes);
            return bytes;
        }

        public static byte[] xorArray(byte[] arr1, byte[] arr2, int size)
        {
            if (arr1 == null || arr2 == null)
                return null;

            if (arr1.Length == 0 || arr2.Length == 0)
                return null;

            if (arr1.Length < size || arr2.Length < size)
                return null;

            byte[] tmp = new byte[size];

            for(int i=0; i<size; i++)
            {
                tmp[i] = (byte)(arr1[i] ^ arr2[i]);
            }
            return tmp;
        }

        public byte[] encryptAES256(byte[] key, byte[] inputBytes)
        {
            return encryptAES256(iv,  key,inputBytes);
        }

        public byte[] encryptAES256(byte[] iv, byte[] key, byte[] inputBytes)
        {
            CbcBlockCipher blockCipher = new CbcBlockCipher(engine); //CBC
            PaddedBufferedBlockCipher cipher = new PaddedBufferedBlockCipher(blockCipher); //Default scheme is PKCS5/PKCS7
            KeyParameter keyParam = new KeyParameter(key); //Convert.FromBase64String(keyString));
            ParametersWithIV keyParamWithIV = new ParametersWithIV(keyParam, iv, 0, 16);

            // Encrypt
            cipher.Init(true, keyParamWithIV);
            byte[] outputBytes = new byte[cipher.GetOutputSize(inputBytes.Length)];
            int length = cipher.ProcessBytes(inputBytes, outputBytes, 0);
            cipher.DoFinal(outputBytes, length); //Do the final block
            string encryptedInput = Convert.ToBase64String(outputBytes);

        //    Debug.WriteLine("Encrypted string: {0}", encryptedInput);
            return outputBytes;
        }

        public byte[] decryptAES256(byte[] key, byte[] inputBytes)
        {
            return decryptAES256(iv, key, inputBytes);
        }

        public byte[] decryptAES256(byte[] iv, byte[]key, byte[] inputBytes)
        {
            CbcBlockCipher blockCipher = new CbcBlockCipher(engine); //CBC
            PaddedBufferedBlockCipher cipher = new PaddedBufferedBlockCipher(blockCipher); //Default scheme is PKCS5/PKCS7
            KeyParameter keyParam = new KeyParameter(key);// Convert.FromBase64String(keyString));
            ParametersWithIV keyParamWithIV = new ParametersWithIV(keyParam, iv, 0, 16);

            //Decrypt            
            cipher.Init(false, keyParamWithIV);
            byte[] comparisonBytes = new byte[cipher.GetOutputSize(inputBytes.Length)];
            int length = cipher.ProcessBytes(inputBytes, comparisonBytes, 0);
            cipher.DoFinal(comparisonBytes, length); //Do the final block

        //    Debug.WriteLine("Decrypted string: {0}", Encoding.UTF8.GetString(comparisonBytes)); //Should be abc
            return comparisonBytes;
        }
        private void CreatePfxFile(X509Certificate certificate, AsymmetricKeyParameter privateKey)
        {
            // create certificate entry
            var certEntry = new X509CertificateEntry(certificate);
            string friendlyName = certificate.SubjectDN.ToString();

            // get bytes of private key.
            PrivateKeyInfo keyInfo = PrivateKeyInfoFactory.CreatePrivateKeyInfo(privateKey);
            byte[] keyBytes = keyInfo.ToAsn1Object().GetEncoded();

            var builder = new Pkcs12StoreBuilder();
            builder.SetUseDerEncoding(true);
            var store = builder.Build();

            // create store entry
            // store.SetKeyEntry(Core.Constants.PrivateKeyAlias, new AsymmetricKeyEntry(privateKey), new X509CertificateEntry[] { certEntry });

            byte[] pfxBytes = null;
            var password = Guid.NewGuid().ToString("N");
            using (MemoryStream stream = new MemoryStream())
            {
                store.Save(stream, password.ToCharArray(), new SecureRandom());
                pfxBytes = stream.ToArray();
            }

            var result = Pkcs12Utilities.ConvertToDefiniteLength(pfxBytes);
            // this.StoreCertificate(Convert.ToBase64String(result));
        }
        public byte[] HmacSha256(string text, string key)
        {
            var hmac = new HMac(new Sha256Digest());
            hmac.Init(new KeyParameter(Encoding.UTF8.GetBytes(key)));
            byte[] result = new byte[hmac.GetMacSize()];
            byte[] bytes = Encoding.UTF8.GetBytes(text);

            hmac.BlockUpdate(bytes, 0, bytes.Length);
            hmac.DoFinal(result, 0);

            return result;
        }
    }


}
