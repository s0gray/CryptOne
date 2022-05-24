
#pragma once

#include "CryptUnit.h"


class SodiumCryptUnit : public CryptUnit
{
public:
	SodiumCryptUnit();
	virtual ~SodiumCryptUnit();

	/// returns length of secret key
	size_t getSecretKeyLength();

	/// returns used nonce length for asymmetric data encryption
	size_t getNonceLength();

	/// returns used nonce length for symmetric data encryption
	size_t getNonceLengthSymmetric();

	/// wipes memory/data on given address and with given length
	RetCode wipeData(unsigned char* data, size_t dataLen);

	/// generates shared secret key
	RetCode generateSecretKey(std::string& secretKey);

	/// returns encrypted message length with given plain message length
	size_t getDecryptedMessageLength(size_t cipherLen) const;

	virtual size_t getDecryptedMessageLengthSymmetric(size_t cipherLen) const;

	/// encrypts plain data asymteric
	RetCode encryptData(const unsigned char *plainData, size_t plainDataLen, unsigned char *cipherData,
				    unsigned char* nonce, const unsigned char *pubKeyReceiver, const unsigned char *secKeySender);

	/// decrypts ciphered message asymteric
	RetCode decryptData(unsigned char *plainData, const unsigned char *cipherData, size_t cipherDataLen,
				    const unsigned char* nonce, const unsigned char *pubKeySender, const unsigned char *privKeyReceiver);

	/// encrypts plain data
	virtual RetCode encryptDataSymmetric(const std::string& plainData, std::string& encrypted, std::string& nonce, const std::string& secretKey);

	virtual RetCode generateRandomData(std::string& result, size_t size);

	/// returns encrypted message length with given plain message length
	virtual size_t getEncryptedMessageLength(size_t messageLen) const;

	/// secure memory compare
	int secureMemCompare(const unsigned char* dataA, const unsigned char* dataB, size_t dataLen);

	virtual size_t getSHA256HashLength() const;
	virtual RetCode hashDataSHA256(const std::string& data, std::string& hashData);
	virtual RetCode decryptDataSymmetric(std::string&plainData, const std::string&cipherData, const std::string& nonce, const std::string&secretKey, size_t plainDataLen);

private:
	size_t getOneTimeAuthKeyLength() const;

};

