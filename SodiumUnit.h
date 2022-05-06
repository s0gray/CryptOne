
#pragma once

#include "CryptUnit.h"


class SodiumCryptUnit : public CryptUnit
{
public:

	SodiumCryptUnit();
	virtual ~SodiumCryptUnit();

	/// returns length of public key
	size_t getPublicKeyLength();

	/// returns length of private key
	size_t getPrivateKeyLength();

	/// returns length of secret key
	size_t getSecretKeyLength();

	/// returns used nonce length for asymmetric data encryption
	size_t getNonceLength();

	/// returns used nonce length for symmetric data encryption
	size_t getNonceLengthSymmetric();

	/// returns used mac length for asymmetric and symmetric data encryption
	size_t getMacLength();

	/// returns used mac length for symmetric data encryption
	size_t getMacLengthSymmetric();


	/// wipes memory/data on given address and with given length
	ErrCode wipeData(unsigned char* data, size_t dataLen);

	/// generates random data of given length
	ErrCode getRandom(unsigned char* randomBuffer, size_t randomLen);

	/// generates key pair
	ErrCode generateKeyPair(unsigned char *pubKey, unsigned char *privKey);

    /// verifies key pair
	ErrCode verifyKeyPair(unsigned char* pubKey, size_t pubLen, unsigned char* privKey, size_t privLen);

	/// generates shared secret key
	ErrCode generateSecretKey(unsigned char *secretKey);

	/// derives shared secret key (ECDH acc. to RFC 7748, chapter 6.1.  Curve25519)
	ErrCode deriveSharedSecretKey(const unsigned char *senderPrivKey, const unsigned char *senderPubKey, const unsigned char *receiverPubKey, unsigned char *sharedSecret);

	/// returns encrypted message length with given plain message length
	virtual size_t getEncryptedMessageLength(size_t messageLen) const;

    /// returns symmetric encrypted message length with given plain message length
	size_t getEncryptedMessageLengthSymmetric(size_t messageLen) const;

	/// returns encrypted message length with given plain message length
	size_t getDecryptedMessageLength(size_t cipherLen) const;

    /// returns encrypted message length with given plain message length
	virtual size_t getDecryptedMessageLengthSymmetric(size_t cipherLen) const;


	/// encrypts plain data asymteric
	ErrCode encryptData(const unsigned char *plainData, size_t plainDataLen, unsigned char *cipherData,
				    unsigned char* nonce, const unsigned char *pubKeyReceiver, const unsigned char *secKeySender);

	/// decrypts ciphered message asymteric
	ErrCode decryptData(unsigned char *plainData, const unsigned char *cipherData, size_t cipherDataLen,
				    const unsigned char* nonce, const unsigned char *pubKeySender, const unsigned char *privKeyReceiver);

	/// encrypts plain data
	virtual ErrCode encryptDataSymmetric(const unsigned char *plainData, size_t plainDataLen, unsigned char *cipherData, size_t &cipherDataSize, unsigned char* nonce, size_t &nonceSize, const unsigned char *secretKey);

	/// decrypts ciphered message
	virtual ErrCode decryptDataSymmetric(unsigned char *plainData, const unsigned char *cipherData, size_t cipherDataLen, const unsigned char* nonce, const unsigned char *secretKey);

	/// encrypts data using derived (from password) secret key
	ErrCode encryptWithPassword(const char *password, size_t passwordLen, const unsigned char *plainData,
		size_t plainDataLen, unsigned char *cipherData, unsigned char* nonce);

	/// decrypts encrypted data using derived (from password) secret key
	ErrCode decryptWithPassword(const char *password, size_t passwordLen, unsigned char *plainData,
		const unsigned char *cipherData, size_t cipherDataLen, const unsigned char* nonce);

	/// derive secret key from password
	ErrCode deriveSecretKeyFromPassword(const char *password, size_t passwordLen, unsigned char *secretKey);

	/// secure memory compare
	int secureMemCompare(const unsigned char* dataA, const unsigned char* dataB, size_t dataLen);

	ErrCode deriveSessionAuthKey(const Blob& authKey, const Blob& derivationData, Blob& sessionAuthKey);
	virtual size_t getSHA256HashLength() const;
	virtual ErrCode hashDataSHA256(const unsigned char *data, size_t dataLen, unsigned char *hashData);
	virtual ErrCode decryptDataSymmetric(BinaryData &plainData, const BinaryData &cipherData, const BinaryData& nonce, const BinaryData &secretKey, size_t plainDataLen);

private:
	size_t getOneTimeAuthKeyLength() const;

};

