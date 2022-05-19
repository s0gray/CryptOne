#pragma once

#include <string>
#include <vector>
#include <map>


enum ErrCode {
	eOk = 0,
	eFatal = 1,
	eBadArg = 2,
	eUnknownErr,
	eBadLen,
	eBadPubKey,
	eBadFile,
	eNotImpl,
	eNotFound
};


typedef unsigned char byte;

typedef std::vector<unsigned char> bytes;
typedef std::map<std::string, std::string>	MapStrStr;
#define Blob bytes


struct CryptHeader
{
	size_t plainDataSize;
	byte nonce[24];
};

#define ASSERTME(x)  if(x!=eOk){return x;}
#define SAFE_FREE(x) if(x!=nullptr){free(x);x=nullptr;}


#ifdef WIN32
#define _INT64 __int64
#else
#define _INT64 __int64_t
#endif

#ifdef WIN32
#define GETCH _getch
#else
#define GETCH getch
#endif


