
#ifndef CRYPTUNIT_H_
#define CRYPTUNIT_H_

#include "Typedefs.h" 


class CryptUnit
{
public:
	virtual size_t		getSHA256HashLength() const { return 32U; }
	virtual RetCode		hashDataSHA256(const std::string& data, std::string& hashData) = 0;

	virtual size_t		getSecretKeyLength() = 0;
	virtual RetCode		encryptDataSymmetric(const std::string& plainData, std::string& encrypted, std::string& nonce, const std::string& secretKey) = 0;
	virtual RetCode		decryptDataSymmetric(std::string& plainData, const std::string& cipherData, const std::string& nonce, const std::string& secretKey, size_t plainDataLen) = 0;

	virtual RetCode		xorData(const std::string& data1, const std::string& data2, size_t size, std::string &result);
	virtual RetCode		generateSecretKey(std::string& key) = 0;
	virtual size_t		getEncryptedMessageLength(size_t messageLen) const = 0;
	virtual RetCode		selfTest();
};

#endif // CRYPTUNIT_H_ 
