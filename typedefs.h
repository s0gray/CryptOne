#pragma once

#include <string>
#include <vector>


enum ErrCode {
	eOk = 0,
	eFatal = 1,
	eBadArg = 2,
	eUnknownErr,
	eBadLen,
	eBadPubKey,
	eBadFile
};

typedef unsigned short	uint16;

typedef std::string BinaryData;
typedef unsigned char byte;

typedef std::vector<unsigned char> bytes;

#define Blob bytes


struct CryptHeader
{
	size_t plainDataSize;
	byte nonce[24];
};