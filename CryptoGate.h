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

#pragma once

#include "Typedefs.h" 


class CryptoGate {
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


