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

#include "CryptoGate.h"


class SodiumGate : public CryptoGate
{
public:
	SodiumGate();
	virtual ~SodiumGate();

	size_t			getSecretKeyLength();
	RetCode			generateSecretKey(std::string& secretKey);
	virtual size_t	getDecryptedMessageLengthSymmetric(size_t cipherLen) const;
	virtual RetCode encryptDataSymmetric(const std::string& plainData, std::string& encrypted, std::string& nonce, const std::string& secretKey);
	virtual RetCode generateRandomData(std::string& result, size_t size);
	virtual size_t	getEncryptedMessageLength(size_t messageLen) const;
	virtual RetCode	hashDataSHA256(const std::string& data, std::string& hashData);
	virtual RetCode	decryptDataSymmetric(std::string&plainData, const std::string&cipherData, const std::string& nonce, const std::string&secretKey, size_t plainDataLen);
};

