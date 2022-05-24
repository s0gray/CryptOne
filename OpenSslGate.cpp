

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

#define GOTO_ERR goto err;

OpenSslCryptoGate::OpenSslCryptoGate()
{
	p = 0;
	g = 0;
	q = 0;
	ERR_load_crypto_strings();


#ifndef WIN32
	int randomData = open("/dev/urandom", O_RDONLY);
	byte myRandomPool[RANDOM_POOL_SIZE];
	read(randomData, myRandomPool, RANDOM_POOL_SIZE);
	// you now have a random integer!
	close(randomData);

	init( myRandomPool, RANDOM_POOL_SIZE ); //(const byte*)this, 4);
#else
	init( (const byte*)this, 4);
#endif
}

OpenSslCryptoGate::~OpenSslCryptoGate()
{
	SAFE_BN_FREE(p);
	SAFE_BN_FREE(g);
	SAFE_BN_FREE(q);
	ERR_free_strings();
}


void OpenSslCryptoGate::encryptAesBlock(const byte *plain,  byte *encrypted, const byte *key, size_t keyLen)
{
	AES_KEY lcKey;
	AES_set_encrypt_key( key, keyLen*8 /*AES_KEY_LEN*/, &lcKey );
	AES_encrypt( plain, encrypted, &lcKey  );
}

void OpenSslCryptoGate::decryptAesBlock(const byte *encrypted, byte *plain, const byte *key, size_t keyLen)
{
	AES_KEY lcKey;
	AES_set_decrypt_key( key, keyLen*8 /*AES_KEY_LEN*/, &lcKey );
	AES_decrypt( encrypted, plain, &lcKey );
}

RetCode OpenSslCryptoGate::init( const byte* seedData, size_t dataLen )
{
//	CRYPTO_malloc_init();
	RAND_seed(seedData, dataLen);
	OpenSSL_add_all_algorithms();
	return eOk;//CryptUnit::init(seedData, dataLen);
}

void OpenSslCryptoGate::generateRnd( byte *buf, int len ) const
{
	/*int ret =*/ RAND_bytes(buf, len);
}

void OpenSslCryptoGate::addEnthropy( const byte *buf, int len)
{
	RAND_seed(buf, len);
}




///* returns length or -1 if error 
int OpenSslCryptoGate::get_written_BIO_data(BIO* wbio, char** data)
{
	  int n;
	  char* p;
	  if (!data) 
		  GOTO_ERR("NULL arg");
	  *data = NULL;
	  BIO_flush(wbio);
	  n = BIO_get_mem_data(wbio,&p);
//	  LOG(Log::eInfo, "get_written_BIO_data: %x %d bytes", p, n);
	  if (!((*data)=(char*)malloc(n+1))) 
		  GOTO_ERR("no memory?");
	  memcpy(*data, p, n);
	  (*data)[n] = '\0';
	  return n;
err:
	  return -1;
}

int OpenSslCryptoGate::write_private_key(EVP_PKEY* pkey, const char* passwd, char** priv_pem_OUT)
{
  int len = -1;
  BIO* wbio=NULL;
  if (!passwd || !priv_pem_OUT || !pkey) 
	  GOTO_ERR("NULL arg(s)");
  *priv_pem_OUT = NULL;
  if (!(wbio = BIO_new(BIO_s_mem()))) 
	  GOTO_ERR("no memory?");

//  if( !PEM_write_bio_PrivateKey(wbio, pkey, EVP_des_ede3_cbc(),(byte*)passwd, strlen(passwd),	NULL,NULL))
  if( !PEM_write_bio_PrivateKey(wbio, pkey, 0,0,0,0,0))
    GOTO_ERR("PEM_write_bio_PrivateKey (bad passwd, no memory?)");

  len = get_written_BIO_data(wbio, priv_pem_OUT);
err:
  if (wbio) BIO_free_all(wbio);
  return len;
}



char* OpenSslCryptoGate::asn1time_to_char(ASN1_TIME* time)
{
	char *not1, *result;
	int n;
	BIO *out;

	out = BIO_new(BIO_s_mem());
	ASN1_TIME_print(out, time);
	n = BIO_get_mem_data(out, &not1);
	result = (char *) malloc (n+1);
	result [n] = '\0';
        memcpy (result, not1, n);

	BIO_free(out);
	return result;
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

EVP_PKEY* OpenSslCryptoGate::open_private_key(const char* privatekey_pem, const char* password) const
{
  EVP_PKEY* pkey = NULL;
  BIO* rbio = NULL;
 // LOG(Log::eStream, "open_private_key: %x %x", privatekey_pem, password);

  if (!(rbio = set_read_BIO_from_buf(privatekey_pem, -1))) goto err;
  if (!(pkey = PEM_read_bio_PrivateKey(rbio,NULL, OpenSslCryptoGate::password_callback,
				     (void*)password)))
    GOTO_ERR("01 bad password or badly formatted private key pem file (PEM_read_bio_PrivateKey)");
 // LOG(Log::eStream, "done");
  BIO_free(rbio);

  return pkey;
  
err:
  if (pkey) EVP_PKEY_free(pkey);
  if (rbio) BIO_free(rbio);
//  LOG(Log::eErr, "error");
  return NULL;
}

/* Initialize a memory BIO to have certain content */

BIO* OpenSslCryptoGate::set_read_BIO_from_buf(const char* buf, int len) const
{
	  BIO* rbio;  
	  BUF_MEM* bm;
	  if( !buf ) 
		  //GOTO_ERR("NULL file buffer");
		  return 0;
	  
	  if(len == -1) 
		  len = strlen(buf);
	 // LOG(Log::eInfo, "set_read_BIO_from_buf %x, len %d", buf, len);
	  if (!(rbio = BIO_new(BIO_s_mem()))) 
		  return 0;
	  if (!(bm = BUF_MEM_new()))  
		  return 0;
	  if (!BUF_MEM_grow(bm, len)) 
		  return 0;
	  memcpy(bm->data, buf, len);
	  BIO_set_mem_buf(rbio, bm, 0 /*not used*/);

	  return rbio;
	/*err:
	  return NULL;*/
}

//////
int OpenSslCryptoGate::decryptWithPublicKey(const byte* data, size_t len, const byte* key, size_t keyLen, byte *out)
{
	RSA* publicKey = d2i_RSA_PUBKEY (NULL, (const byte**)&key, keyLen);
    int result = RSA_public_decrypt(len,data,out,publicKey,RSA_PKCS1_PADDING);
     RSA_free(publicKey);
	return result;
}

RetCode OpenSslCryptoGate::generateRnd( byte *buf, int len )
{
	RAND_bytes(buf, len);
	return eOk;
}

// input buffer		: const byte *in, , byte *hash
// input buffer size: size_t inLen
// output buffer	: byte *hash, must be preallocated SHA256_DIGEST_LENGTH bytes
void OpenSSLCryptUnit::hashDataSHA256(const byte *in, size_t inLen, byte *hash)
{
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, in, inLen);
    SHA256_Final(hash, &sha256);

}

#endif