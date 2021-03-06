
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

#include "CryptoGate.h"

#include <string.h>
#include <memory>

#include "Log.h"
#include "Tools.h"

RetCode CryptoGate::xorData(const std::string& data1, const std::string& data2, size_t size, std::string& result) {
	if (data1.size() < size) {
		return eBadArg;
	}
	if (data2.size() < size) {
		return eBadArg;
	}
	result = "";
	
	std::unique_ptr<char[]> buf(new char[size]);
	if (!buf)
		return eFatal;

	const byte* ptr1 = (const byte*)data1.c_str();
	const byte* ptr2 = (const byte*)data2.c_str();

	for (size_t i = 0; i < size; i++) {
		buf[i] = ptr1[i] ^ ptr2[i];
	}

	result.assign(buf.get(), size);
	return eOk;
}

RetCode CryptoGate::selfTest() {

	// TEST 1. SHA256
	std::string input = "abc";
	std::string expectedHash = Tools::h2b("ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad");

	std::string hash;
	RetCode ret = this->hashDataSHA256(input, hash);
	ASSERTME(ret);

	if (hash.compare(expectedHash) != 0) {
//		LOGI("SHA256 self-test failed expected [%s] received [%s]", expectedHash.c_str(), Tools::b2h(hash).c_str());
		return eFatal;
	}

	// TEST 2. XOR
	std::string key1, key2;
	ret = generateSecretKey(key1);
	ASSERTME(ret);
	
	ret = generateSecretKey(key2);
	ASSERTME(ret);

	std::string xor1, xor2;
	ret = this->xorData(key1, key2, key1.size(), xor1);
	ASSERTME(ret);

	ret = this->xorData(xor1, key2, xor1.size(), xor2);
	ASSERTME(ret);

	if (key1.compare(xor2) != 0) {
	//	LOGI("XOR self-test failed expected [%s] received [%s]", Tools::b2h(xor1).c_str(), Tools::b2h(xor2).c_str());
		return eFatal;
	}

	// TEST 3. 
	std::string key, plain = "abcdefgh1234567890";
	ret = generateSecretKey(key);
	ASSERTME(ret);

	std::string encrypted, nonce, decrypted;
	ret = this->encryptDataSymmetric(plain, encrypted, nonce, key);
	ASSERTME(ret);

	ret = this->decryptDataSymmetric(decrypted, encrypted, nonce, key, plain.size());
	ASSERTME(ret);

	if (decrypted.compare(plain) != 0) {
	//	LOGGER("Symmetrical enc/dec self-test failed expected [%s] received [%s]", Tools::b2h(plain).c_str(), Tools::b2h(decrypted).c_str());
		return eFatal;
	}

	return eOk;
}
