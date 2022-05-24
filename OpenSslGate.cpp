
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

#include "OpenSslGate.h"

#ifdef USE_OPENSSL

#include "Log.h"
#include <openssl/ssl.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <openssl/x509.h>
#include <openssl/conf.h>

#include "Tools.h"
#include <openssl/pem.h>
#include <openssl/conf.h>
#include <openssl/rsa.h>

#undef X509_NAME

#pragma warning(disable : 4996)

#include <openssl/bn.h>     // a BIGNUM is here
#include <openssl/aes.h>

#ifndef WIN32
	#include <sys/stat.h>
	#include <fcntl.h>
	#include <sys/types.h>
	#include <unistd.h>
#endif

#define SAFE_BN_FREE(x) if(x){BN_free(x);x=0;}

#define AES_KEY_LEN			256
#define RANDOM_POOL_SIZE	256   // to seed PRNG
#define KEY_BUF_SIZE		512

#define GOTO_ERR	 goto err;

OpenSslCryptoGate::OpenSslCryptoGate() {
	p = nullptr;
	g = nullptr;
	q = nullptr;
	ERR_load_crypto_strings();

#ifndef WIN32
	int randomData = open("/dev/urandom", O_RDONLY);
	byte myRandomPool[RANDOM_POOL_SIZE];
	read(randomData, myRandomPool, RANDOM_POOL_SIZE);
	close(randomData);
	init( myRandomPool, RANDOM_POOL_SIZE ); 
#else
	init( (const byte*)this, 4);
#endif
}

OpenSslCryptoGate::~OpenSslCryptoGate() {
	SAFE_BN_FREE(p);
	SAFE_BN_FREE(g);
	SAFE_BN_FREE(q);
	ERR_free_strings();
}

void OpenSslCryptoGate::encryptAesBlock(const byte *plain,  byte *encrypted, const byte *key, size_t keyLen) {
	AES_KEY lcKey;
	AES_set_encrypt_key( key, keyLen*8 /*AES_KEY_LEN*/, &lcKey );
	AES_encrypt( plain, encrypted, &lcKey  );
}

void OpenSslCryptoGate::decryptAesBlock(const byte *encrypted, byte *plain, const byte *key, size_t keyLen) {
	AES_KEY lcKey;
	AES_set_decrypt_key( key, keyLen*8 /*AES_KEY_LEN*/, &lcKey );
	AES_decrypt( encrypted, plain, &lcKey );
}

RetCode OpenSslCryptoGate::init( const byte* seedData, size_t dataLen ) {
	RAND_seed(seedData, dataLen);
	OpenSSL_add_all_algorithms();
	return eOk;
}

void OpenSslCryptoGate::generateRnd( byte *buf, int len ) const {
	RAND_bytes(buf, len);
}


///* returns length or -1 if error 
int OpenSslCryptoGate::get_written_BIO_data(BIO* wbio, char** data) {
	  int n;
	  char* p;
	  if (!data) 
		  GOTO_ERR("NULL arg");
	  *data = NULL;
	  BIO_flush(wbio);
	  n = BIO_get_mem_data(wbio,&p);

	  if (!((*data)=(char*)malloc(n+1))) 
		  GOTO_ERR("no memory?");
	  memcpy(*data, p, n);
	  (*data)[n] = '\0';
	  return n;
err:
	  return -1;
}

int OpenSslCryptoGate::write_private_key(EVP_PKEY* pkey, const char* passwd, char** priv_pem_OUT) {
  int len = -1;
  BIO* wbio=NULL;
  if (!passwd || !priv_pem_OUT || !pkey) 
	  GOTO_ERR("NULL arg(s)");
  *priv_pem_OUT = NULL;
  if (!(wbio = BIO_new(BIO_s_mem()))) 
	  GOTO_ERR("no memory?");

  if( !PEM_write_bio_PrivateKey(wbio, pkey, 0,0,0,0,0))
    GOTO_ERR("PEM_write_bio_PrivateKey (bad passwd, no memory?)");

  len = get_written_BIO_data(wbio, priv_pem_OUT);
err:
  if (wbio) BIO_free_all(wbio);
  return len;
}


int OpenSslCryptoGate::password_callback(char* buf, int buf_size,
		  int x /*not used*/, void* password)
{
  int n;
  if (!password) {
    strcpy(buf, "");
    return 0;
  }
  n = strlen((char*)password);
  if (n >= buf_size) n = buf_size-1;
  memcpy(buf, (char*)password, n);
  buf[n] = '\0';
  return n; 
}

EVP_PKEY* OpenSslCryptoGate::openPrivateKey(const char* privatekey_pem, const char* password) const {
  EVP_PKEY* pkey = NULL;
  BIO* rbio = NULL;

  if (!(rbio = set_read_BIO_from_buf(privatekey_pem, -1))) goto err;
  if (!(pkey = PEM_read_bio_PrivateKey(rbio,NULL, OpenSslCryptoGate::password_callback,
				     (void*)password)))
    GOTO_ERR("01 bad password or badly formatted private key pem file (PEM_read_bio_PrivateKey)");
  BIO_free(rbio);

  return pkey;
  
err:
  if (pkey) EVP_PKEY_free(pkey);
  if (rbio) BIO_free(rbio);

  return NULL;
}

// Initialize a memory BIO to have certain content 
BIO* OpenSslCryptoGate::set_read_BIO_from_buf(const char* buf, int len) const {
	  BIO* rbio;  
	  BUF_MEM* bm;
	  if( !buf ) 
		  return nullptr;
	  
	  if(len == -1) 
		  len = strlen(buf);

	  if (!(rbio = BIO_new(BIO_s_mem()))) 
		  return 0;
	  if (!(bm = BUF_MEM_new()))  
		  return 0;
	  if (!BUF_MEM_grow(bm, len)) 
		  return 0;
	  memcpy(bm->data, buf, len);
	  BIO_set_mem_buf(rbio, bm, 0 /*not used*/);

	  return rbio;
}


RetCode OpenSslCryptoGate::generateRnd( byte *buf, int len ) {
	RAND_bytes(buf, len);
	return eOk;
}

// input buffer		: const byte *in, , byte *hash
// input buffer size: size_t inLen
// output buffer	: byte *hash, must be preallocated SHA256_DIGEST_LENGTH bytes
void OpenSSLCryptUnit::hashDataSHA256(const byte *in, size_t inLen, byte *hash) {
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, in, inLen);
    SHA256_Final(hash, &sha256);

}

#endif