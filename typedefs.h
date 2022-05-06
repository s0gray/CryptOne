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
	eNotImpl
};

typedef unsigned short	uint16;

typedef std::string BinaryData;
typedef unsigned char byte;

typedef std::vector<unsigned char> bytes;
typedef std::map<std::string, std::string>	MapStrStr;
#define Blob bytes


struct CryptHeader
{
	size_t plainDataSize;
	byte nonce[24];
};