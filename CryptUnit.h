
#ifndef CRYPTUNIT_H_
#define CRYPTUNIT_H_

#include "typedefs.h" 


class CryptUnit
{
public:
	// RND methods
	virtual ErrCode		getRandom(unsigned char* randomBuffer, size_t randomLen) = 0;
	virtual ErrCode		verifyKeyPair(unsigned char* pubKey, size_t pubLen, unsigned char* privKey, size_t privLen) = 0;

	virtual size_t		getSHA256HashLength() const = 0;
	virtual ErrCode		hashDataSHA256(const unsigned char *data, size_t dataLen, unsigned char *hashData)= 0;

	// next is implemented with the help of generateRnd    
	virtual ErrCode deriveSessionSecretKey(const Blob& secKey, const Blob& derivationData, Blob& sessionSecKey);

	virtual ErrCode generateKeyPair(byte *privateKey, byte *publicKey) = 0;
	virtual ErrCode deriveSharedSecretKey(const unsigned char *senderPrivKey, const unsigned char *senderPubKey, const unsigned char *receiverPubKey, unsigned char *sharedSecret) = 0;

	virtual size_t getPublicKeyLength()= 0;
	virtual size_t getPrivateKeyLength() = 0;
	virtual size_t getDecryptedMessageLengthSymmetric(size_t cipherLen) const = 0;
	virtual size_t getEncryptedMessageLength(size_t messageLen) const = 0;
	virtual ErrCode decryptDataSymmetric(BinaryData &plainData, const BinaryData &cipherData, const BinaryData& nonce, const BinaryData &secretKey, size_t plainDataLen) = 0;
	virtual size_t getSecretKeyLength() = 0;

	virtual ErrCode encryptDataSymmetric(const unsigned char *plainData, size_t plainDataLen, unsigned char *cipherData, size_t &cipherDataSize, unsigned char* nonce, size_t &nonceSize, const unsigned char *secretKey) = 0;
	virtual ErrCode decryptDataSymmetric(unsigned char *plainData, const unsigned char *cipherData, size_t cipherDataLen, const unsigned char* nonce, const unsigned char *secretKey)=0;

	virtual ErrCode deriveSessionAuthKey(const Blob& authKey, const Blob& derivationData, Blob& sessionAuthKey) =0;
	virtual ErrCode xorData(const std::string& data1, const std::string& data2, size_t size, std::string &result);

	virtual ErrCode generateSecretKey(unsigned char* secretKey) = 0;

};

#endif // CRYPTUNIT_H_ 
