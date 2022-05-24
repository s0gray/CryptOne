
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


struct ClusterLicenseData;

class OpenSslCryptoGate : public CryptoGate
{
public:
	OpenSslCryptoGate();
	virtual ~OpenSslCryptoGate();

protected: 

	virtual RetCode init(const byte* seedData, size_t dataLen);

	virtual void encryptAesBlock(const byte *plain,  byte *encrypted, const byte *key, size_t keyLen);
	virtual void decryptAesBlock(const byte *encrypted, byte *plain, const byte *key, size_t keyLen);

	virtual void generateRnd(byte *buf, int len) const;
	virtual void addEnthropy( const byte *buf, int len);
	

	RetCode generateRnd(byte *buf, int len);

	virtual void hashDataSHA256(const byte *in, size_t inLen, byte *hash);
public:	


	EVP_PKEY* open_private_key(const char* privatekey_pem, const char* password) const;
	int decryptWithPublicKey(const byte* data, size_t len, const byte* key, size_t keyLen, byte *out);

private:
	int 	get_written_BIO_data(BIO* wbio, char** data);
	int 	write_private_key(EVP_PKEY* pkey, const char* passwd, char** priv_pem_OUT);
 
	 static int password_callback(char* buf, int buf_size, int x /*not used*/, void* password);
	 static char* asn1time_to_char(ASN1_TIME *time);
	 BIO* set_read_BIO_from_buf(const char* buf, int len) const;

	 BIGNUM *p, *g, *q;
	 int bits;
	 int bytes;

};

#endif