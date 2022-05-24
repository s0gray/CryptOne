
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

#ifdef USE_OPENSSL

#include "Typedefs.h"
#include "CryptoGate.h"

typedef struct x509_st X509;
typedef struct evp_pkey_st EVP_PKEY;
typedef struct bio_st BIO;
typedef struct dh_st DH;
typedef struct bignum_st BIGNUM;
typedef struct asn1_string_st ASN1_TIME;


class OpenSslCryptoGate : public CryptoGate {
public:
	OpenSslCryptoGate();
	virtual ~OpenSslCryptoGate();

	virtual RetCode init(const byte* seedData, size_t dataLen);
	virtual void encryptAesBlock(const byte *plain,  byte *encrypted, const byte *key, size_t keyLen);
	virtual void decryptAesBlock(const byte *encrypted, byte *plain, const byte *key, size_t keyLen);
	virtual void generateRnd(byte *buf, int len) const;

	RetCode generateRnd(byte *buf, int len);
	virtual void hashDataSHA256(const byte *in, size_t inLen, byte *hash);
	EVP_PKEY* openPrivateKey(const char* privatekey_pem, const char* password) const;

private:
	int 	get_written_BIO_data(BIO* wbio, char** data);
	int 	write_private_key(EVP_PKEY* pkey, const char* passwd, char** priv_pem_OUT);
 
	 static int password_callback(char* buf, int buf_size, int x /*not used*/, void* password);
	 BIO* set_read_BIO_from_buf(const char* buf, int len) const;

	 BIGNUM *p, *g, *q;
	 int bits;
	 int bytes;
};

#endif