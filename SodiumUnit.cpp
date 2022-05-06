

#include <sodium.h> 

#include "SodiumUnit.h"
#include "Logger.h"

SodiumCryptUnit::SodiumCryptUnit()
{
}

SodiumCryptUnit::~SodiumCryptUnit()
{
}

size_t SodiumCryptUnit::getSecretKeyLength()
{
	return crypto_secretbox_KEYBYTES;
}


ErrCode SodiumCryptUnit::wipeData(unsigned char* data, size_t dataLen)
{
	if (!data)
		return eBadArg;

	sodium_memzero((unsigned char*)data, dataLen);

	return eOk;
}

 ErrCode SodiumCryptUnit::generateSecretKey(std::string &key) {
	return generateRandomData(key, getSecretKeyLength());
}

 ErrCode SodiumCryptUnit::generateRandomData(std::string& result, size_t size) {
	 byte* secretKey = (byte*)malloc(size);
	 if (!secretKey)
		 return eFatal;

	 randombytes_buf(secretKey, size);

	 result.assign((const char*)secretKey, size);
	 free(secretKey);

	 return eOk;
 }

size_t SodiumCryptUnit::getDecryptedMessageLength(size_t cipherLen) const
{
	return (cipherLen - crypto_box_MACBYTES);
}

size_t SodiumCryptUnit::getDecryptedMessageLengthSymmetric(size_t cipherLen) const
{
    return (cipherLen - crypto_secretbox_MACBYTES);
}

size_t SodiumCryptUnit::getNonceLength()
{
	return crypto_box_NONCEBYTES;
}

size_t SodiumCryptUnit::getNonceLengthSymmetric()
{
	return crypto_secretbox_NONCEBYTES;
}


ErrCode SodiumCryptUnit::encryptData(const unsigned char *plainData, size_t plainDataLen, unsigned char *cipherData,
								    unsigned char* nonce, const unsigned char *pubKeyReceiver, const unsigned char *privKeySender)
{
	if (!plainData || !cipherData || !nonce || !pubKeyReceiver || !privKeySender)
		return eBadArg;

	// generate/use random bytes as nonce
	randombytes_buf(nonce, crypto_box_NONCEBYTES);

	if (crypto_box_easy(cipherData, plainData, plainDataLen, nonce, pubKeyReceiver, privKeySender) != 0)
	{
		return eUnknownErr;
	}

	return eOk;
}

ErrCode SodiumCryptUnit::decryptData(unsigned char *plainData, const unsigned char *cipherData, size_t cipherDataLen,
								    const unsigned char* nonce, const unsigned char *pubKeySender, const unsigned char *privKeyReceiver)
{
	if (!plainData || !cipherData || !nonce || !pubKeySender || !privKeyReceiver)
		return eBadArg;

	if (crypto_box_open_easy(plainData, cipherData, cipherDataLen, nonce, pubKeySender, privKeyReceiver) != 0)
	{
		return eUnknownErr;
	}

	return eOk;
}

ErrCode SodiumCryptUnit::encryptDataSymmetric(	const std::string& plainData, 
												std::string& encryped,
												std::string& nonce,
												const std::string& secretKey)
{
	// generate/use random bytes as nonce
	ASSERTME( generateRandomData(nonce, crypto_secretbox_NONCEBYTES) );

	size_t encryptedSize = this->getEncryptedMessageLength(plainData.size());
	byte* cipherData = (byte*)calloc(encryptedSize, 1);
	if (!cipherData)
		return eFatal;

	if (crypto_secretbox_easy(cipherData,(const byte*) plainData.c_str(), plainData.size(),
			(const byte*)nonce.c_str(), (const byte*)secretKey.c_str()) != 0)
	{
		free(cipherData);
		return eUnknownErr;
	}
	//nonceSize = crypto_secretbox_NONCEBYTES;
	//cipherDataSize = plainData.size() + crypto_secretbox_MACBYTES;
	encryped.assign((const char*)cipherData, encryptedSize);
	free(cipherData);

	//LOG_DATA(3, "cipherData ", cipherData, cipherDataSize);
	//LOG_DATA(3, "nonce ", nonce, this->getNonceLength());
	//LOG_DATA(3, "secretKey ", secretKey.c_str(), secretKey.size());

	return eOk;
}

int SodiumCryptUnit::secureMemCompare(const unsigned char* dataA, const unsigned char* dataB, size_t dataLen)
{
	if (!dataA || !dataB)
		return eBadArg;

	return sodium_memcmp(dataA, dataB, dataLen);
}

size_t SodiumCryptUnit::getSHA256HashLength() const
{
	return crypto_hash_sha256_BYTES;
}

size_t SodiumCryptUnit::getEncryptedMessageLength(size_t messageLen) const
{
	return (messageLen + crypto_box_MACBYTES);
}

ErrCode SodiumCryptUnit::hashDataSHA256(const std::string& data, std::string& hashData)
{
	byte hash[32];
	if (crypto_hash_sha256(hash, (const byte*)data.c_str(), data.size()) != 0) {
		return eFatal;
	}
	hashData.assign((const char*)hash, 32);
	return eOk;
}

size_t SodiumCryptUnit::getOneTimeAuthKeyLength() const
{
	return crypto_onetimeauth_KEYBYTES;
}

ErrCode SodiumCryptUnit::decryptDataSymmetric(std::string&plainData, const std::string&cipherData, const std::string& nonce, const std::string&secretKey, size_t plainDataLen)
{
//	LOGI(" plainDataLen = %u", plainDataLen);
	LOG_DATA(3, "cipherData ", (const byte*)cipherData.c_str(), cipherData.size());
	LOG_DATA(3, "nonce ", (const byte*)nonce.c_str(), this->getNonceLength());
	LOG_DATA(3, "secretKey ", (const byte*)secretKey.c_str(), this->getSecretKeyLength());

	size_t plainDataSize = getDecryptedMessageLengthSymmetric(cipherData.size());
	LOGI("plainDataSize = %u", plainDataSize);
	byte *plainDataBuffer = (byte*)calloc(plainDataSize, 1);
	if (crypto_secretbox_open_easy(plainDataBuffer, (const byte*)cipherData.c_str(), cipherData.size(), (const byte*)nonce.c_str(), (const byte*)secretKey.c_str()) != 0)
	{
		free(plainDataBuffer);
		return eUnknownErr;
	}
	if (plainDataSize < plainDataLen) {
		LOGE("plainDataSize [%u] < plainDataLen [%u]", plainDataSize, plainDataLen); // ?
		return eFatal;
	}
	if (!plainDataBuffer) {
		LOGE("plainDataBuffer is null");
		return eFatal;
	}
	plainData.assign((const char*)plainDataBuffer, plainDataLen);
	free(plainDataBuffer);
	return eOk;
}

