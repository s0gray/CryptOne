

#include <sodium.h> 

#include "SodiumUnit.h"
#include "Logger.h"

SodiumCryptUnit::SodiumCryptUnit()
{
}

SodiumCryptUnit::~SodiumCryptUnit()
{
}

size_t SodiumCryptUnit::getPublicKeyLength()
{
	return crypto_box_PUBLICKEYBYTES;
}

size_t SodiumCryptUnit::getPrivateKeyLength()
{
	return crypto_box_SECRETKEYBYTES;
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

ErrCode SodiumCryptUnit::getRandom(unsigned char* randomBuffer, size_t randomLen)
{
	if (!randomBuffer)
		return eBadArg;

	randombytes_buf(randomBuffer, randomLen);

	return eOk;
}

ErrCode SodiumCryptUnit::generateKeyPair(unsigned char *pubKey, unsigned char *privKey)
{
	if (!pubKey || !privKey)
		return eBadArg;

	int ret = crypto_box_keypair(pubKey, privKey);
    return (ret == 0) ? eOk : eUnknownErr;

	// derive public key from privatekey and compare with provided public key
	/*generateSecretKey(privKey);
	crypto_scalarmult_base(pubKey, privKey);*/

	return eOk;
}

ErrCode SodiumCryptUnit::verifyKeyPair(unsigned char* pubKey, size_t pubLen, unsigned char* privKey, size_t privLen)
{
    unsigned char pubKeyTemp[crypto_box_PUBLICKEYBYTES];

	if (!pubKey || !privKey)
		return eBadArg;

	if ((pubLen != getPublicKeyLength()) || (privLen != getPrivateKeyLength()))
		return eBadLen;

	// derive public key from private key and compare with provided public key
	crypto_scalarmult_base(pubKeyTemp, privKey);

	if (0 != secureMemCompare(pubKeyTemp, pubKey, pubLen)) {
		wipeData(pubKeyTemp, crypto_box_PUBLICKEYBYTES);
		return eBadPubKey;
	}

	wipeData(pubKeyTemp, crypto_box_PUBLICKEYBYTES);
	return eOk;
}

ErrCode SodiumCryptUnit::generateSecretKey(unsigned char *secretKey)
{
	if (!secretKey)
		return eBadArg;

	randombytes_buf(secretKey, getSecretKeyLength());

	return eOk;

}

size_t SodiumCryptUnit::getEncryptedMessageLength(size_t messageLen) const
{
	return (messageLen + crypto_box_MACBYTES);
}

size_t SodiumCryptUnit::getEncryptedMessageLengthSymmetric(size_t messageLen) const
{
    return (messageLen + crypto_secretbox_MACBYTES);
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

size_t SodiumCryptUnit::getMacLength()
{
	return crypto_box_MACBYTES;
}

size_t SodiumCryptUnit::getMacLengthSymmetric()
{
	return crypto_secretbox_MACBYTES;
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

ErrCode SodiumCryptUnit::encryptDataSymmetric(const unsigned char *plainData, size_t plainDataLen, unsigned char *cipherData, size_t &cipherDataSize,
	unsigned char* nonce, size_t &nonceSize, const unsigned char *secretKey)
{
	if (!plainData || !cipherData || !nonce || !secretKey)
		return eBadArg;

	// generate/use random bytes as nonce
	randombytes_buf(nonce, crypto_secretbox_NONCEBYTES);

	if (crypto_secretbox_easy(cipherData, plainData, plainDataLen, nonce, secretKey) != 0)
	{
		return eUnknownErr;
	}
	nonceSize = crypto_secretbox_NONCEBYTES;
	cipherDataSize = plainDataLen + crypto_secretbox_MACBYTES;

	LOG_DATA(3, "cipherData ", cipherData, cipherDataSize);
	LOG_DATA(3, "nonce ", nonce, this->getNonceLength());
	LOG_DATA(3, "secretKey ", secretKey, this->getSecretKeyLength());

	return eOk;
}

ErrCode SodiumCryptUnit::decryptDataSymmetric(unsigned char *plainData, const unsigned char *cipherData, size_t cipherDataLen,
	const unsigned char* nonce, const unsigned char *secretKey)
{
	if (!plainData || !cipherData || !nonce || !secretKey)
		return eBadArg;

	LOG_DATA(3, "cipherData ", cipherData, cipherDataLen);
	LOG_DATA(3, "nonce ", nonce, this->getNonceLength());
	LOG_DATA(3, "secretKey ", secretKey,this->getSecretKeyLength());

	if (crypto_secretbox_open_easy(plainData, cipherData, cipherDataLen, nonce, secretKey) != 0)
	{
		return eUnknownErr;
	}

	return eOk;
}

ErrCode SodiumCryptUnit::deriveSharedSecretKey(const unsigned char *senderPrivKey, const unsigned char *senderPubKey,
	const unsigned char *receiverPubKey, unsigned char *sharedSecret)
{
	if (!senderPrivKey || !senderPubKey || !receiverPubKey || !sharedSecret)
		return eBadArg;

	unsigned char tmpScalaramult[crypto_scalarmult_BYTES];
	crypto_generichash_state tmpHash;

	// ECDH acc.to RFC 7748, chapter 6.1.Curve25519
	// Alice and Bob can then use a key-derivationcfunction 
	// that includes K, K_A, and K_B to derive a symmetric key.

	crypto_scalarmult(tmpScalaramult, senderPrivKey, receiverPubKey);
	crypto_generichash_init(&tmpHash, NULL, 0U, crypto_generichash_BYTES);
	crypto_generichash_update(&tmpHash, tmpScalaramult, crypto_scalarmult_BYTES);

	if (crypto_generichash_final(&tmpHash, sharedSecret, crypto_secretbox_KEYBYTES) != 0)
	{
		return eUnknownErr;
	}

	return eOk;
}

ErrCode SodiumCryptUnit::deriveSecretKeyFromPassword(const char *password, size_t passwordLen, unsigned char *secretKey)
{
	if (!password || !secretKey)
		return eBadArg;

	unsigned char salt[crypto_pwhash_scryptsalsa208sha256_SALTBYTES];

	// in order to derive always the same secret key from same password we have to use identical/static salt
	// TODO: randomize salt with hidden string instead of using zeros
	//randombytes_buf(salt, sizeof salt);
	sodium_memzero(salt, sizeof salt);

	if (crypto_pwhash_scryptsalsa208sha256(secretKey, getPrivateKeyLength(), password, passwordLen, salt,
		 crypto_pwhash_scryptsalsa208sha256_OPSLIMIT_INTERACTIVE,
		 crypto_pwhash_scryptsalsa208sha256_MEMLIMIT_INTERACTIVE) != 0) {
		return eUnknownErr;
	}

	return eOk;
}


ErrCode SodiumCryptUnit::encryptWithPassword(const char *password, size_t passwordLen, const unsigned char *plainData,
	size_t plainDataLen, unsigned char *cipherData, unsigned char* nonce)
{
	if (!password || !plainData || !cipherData || !nonce)
		return eBadArg;

	unsigned char secretKey[crypto_secretbox_KEYBYTES];

	if (deriveSecretKeyFromPassword(password, passwordLen, secretKey) != 0){
		return eUnknownErr;
	}

	size_t cipherDataSize = 0;
	size_t nonceSize = 0;
	if (encryptDataSymmetric(plainData, plainDataLen, cipherData, cipherDataSize, nonce, nonceSize, secretKey) != 0) {
		return eUnknownErr;
	}

	wipeData(secretKey, sizeof(secretKey));

	return eOk;
}

ErrCode SodiumCryptUnit::decryptWithPassword(const char *password, size_t passwordLen, unsigned char *plainData,
	const unsigned char *cipherData, size_t cipherDataLen, const unsigned char* nonce)
{
	if (!password || !plainData || !cipherData || !nonce)
		return eBadArg;

	unsigned char secretKey[crypto_secretbox_KEYBYTES];

	if (deriveSecretKeyFromPassword(password, passwordLen, secretKey) != 0){
		return eUnknownErr;
	}

	if (decryptDataSymmetric(plainData, cipherData, cipherDataLen, nonce, secretKey) != 0) {
		return eUnknownErr;
	}

	wipeData(secretKey, sizeof(secretKey));

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

ErrCode SodiumCryptUnit::hashDataSHA256(const unsigned char *data, size_t dataLen, unsigned char *hashData)
{
	if (!data || !hashData)
		return eBadArg;

	if (crypto_hash_sha256(hashData, data, dataLen) != 0) {
		return eFatal;
	}

	return eOk;
}

ErrCode SodiumCryptUnit::deriveSessionAuthKey(const Blob& authKey, const Blob& derivationData, Blob& sessionAuthKey)
{
	LOG_DATA(3, "authKey", &authKey[0], authKey.size());
	LOG_DATA(3, "derivationData", &derivationData[0], derivationData.size());

	ErrCode ret = eOk;

	Blob tmpData1 = authKey;
	tmpData1.insert(tmpData1.end(), derivationData.begin(), derivationData.end());

	Blob tmpData2(getSHA256HashLength());

	// we repeat two sha256 hash rounds
	ret = hashDataSHA256(tmpData1.data(), tmpData1.size(), tmpData2.data());
	if (ret != eOk)
		return ret;
	
	ret = hashDataSHA256(tmpData2.data(), tmpData2.size(), tmpData2.data());
	if (ret != eOk)
		return ret;

	// take required length as session secret key
	sessionAuthKey.resize(getOneTimeAuthKeyLength());
	std::copy(tmpData2.begin(), tmpData2.begin() + getOneTimeAuthKeyLength(), sessionAuthKey.begin());
	
	return ret;
}

size_t SodiumCryptUnit::getOneTimeAuthKeyLength() const
{
	return crypto_onetimeauth_KEYBYTES;
}

ErrCode SodiumCryptUnit::decryptDataSymmetric(BinaryData &plainData, const BinaryData &cipherData, const BinaryData& nonce, const BinaryData &secretKey, size_t plainDataLen)
{
	LOGI(" plainDataLen = %u", plainDataLen);
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

