/*
 * This file is part of the CryptOne distribution (https://github.com/s0gray/CryptOne).
 * Copyright (c) 2022 Sergey Salata.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <sodium.h> 

#include "SodiumGate.h"

#include <memory>

SodiumGate::SodiumGate() {
}

SodiumGate::~SodiumGate() {
}

size_t SodiumGate::getSecretKeyLength() {
	return crypto_secretbox_KEYBYTES;
}


RetCode SodiumGate::generateSecretKey(std::string &key) {
	return generateRandomData(key, getSecretKeyLength());
}

 RetCode SodiumGate::generateRandomData(std::string& result, size_t size) {

	 std::unique_ptr<char[]> privateKey(new char[size]);
	 randombytes_buf(privateKey.get(), size);
	 result.assign(privateKey.get(), size);
	 return eOk;
 }


size_t SodiumGate::getDecryptedMessageLengthSymmetric(size_t cipherLen) const {
    return (cipherLen - crypto_secretbox_MACBYTES);
}

RetCode SodiumGate::encryptDataSymmetric(	const std::string& plainData,	/// IN
												std::string& encryped,			/// OUT
												std::string& nonce,				/// OUT
												const std::string& secretKey)	/// IN
{
	// generate/use random bytes as nonce
	ASSERTME( generateRandomData(nonce, crypto_secretbox_NONCEBYTES) );

	size_t encryptedSize = this->getEncryptedMessageLength(plainData.size());
	std::unique_ptr<byte[]> cipherData(new byte[encryptedSize]);

	if (crypto_secretbox_easy(cipherData.get(), (const byte*)plainData.c_str(), plainData.size(),
			(const byte*)nonce.c_str(), (const byte*)secretKey.c_str()) != 0)	return eFatal;
	
	encryped.assign((const char*)cipherData.get(), encryptedSize);
	return eOk;
}


size_t SodiumGate::getEncryptedMessageLength(size_t messageLen) const {
	return (messageLen + crypto_box_MACBYTES);
}

RetCode SodiumGate::hashDataSHA256(const std::string& data, std::string& hashData) {
	byte hash[32];
	if (crypto_hash_sha256(hash, (const byte*)data.c_str(), data.size()) != 0) {
		return eFatal;
	}
	hashData.assign((const char*)hash, 32);
	return eOk;
}


RetCode SodiumGate::decryptDataSymmetric(std::string&plainData, const std::string&cipherData, const std::string& nonce, const std::string&secretKey, size_t plainDataLen) {
	size_t plainDataSize = getDecryptedMessageLengthSymmetric(cipherData.size());
	std::unique_ptr<byte[]> plainDataBuffer(new byte[plainDataSize]);

	if (crypto_secretbox_open_easy(plainDataBuffer.get(), (const byte*)cipherData.c_str(), cipherData.size(), (const byte*)nonce.c_str(), (const byte*)secretKey.c_str()) != 0) 
		return eUnknownErr;
	
	if (plainDataSize < plainDataLen) {
		return eFatal;
	}
	if (!plainDataBuffer) {
		return eFatal;
	}
	plainData.assign((const char*)plainDataBuffer.get(), plainDataLen);
	return eOk;
}

