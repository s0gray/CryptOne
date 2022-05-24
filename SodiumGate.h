
#pragma once

#include "CryptoGate.h"


class SodiumGate : public CryptoGate
{
public:
	SodiumGate();
	virtual ~SodiumGate();

	/// returns length of secret key
	size_t getSecretKeyLength();

	/// generates shared secret key
	RetCode generateSecretKey(std::string& secretKey);

	virtual size_t getDecryptedMessageLengthSymmetric(size_t cipherLen) const;

	/// encrypts plain data
	virtual RetCode encryptDataSymmetric(const std::string& plainData, std::string& encrypted, std::string& nonce, const std::string& secretKey);

	virtual RetCode generateRandomData(std::string& result, size_t size);

	/// returns encrypted message length with given plain message length
	virtual size_t getEncryptedMessageLength(size_t messageLen) const;


	virtual RetCode hashDataSHA256(const std::string& data, std::string& hashData);
	virtual RetCode decryptDataSymmetric(std::string&plainData, const std::string&cipherData, const std::string& nonce, const std::string&secretKey, size_t plainDataLen);


};

