
#include "CryptUnit.h"

#include <string.h>
#include "Logger.h"

/**
*	Generates random UINT16 value
*	@return random
*/
uint16 CryptUnit::generateUint16()
{
	uint16 ret = 0;
	getRandom((byte*)&ret, 2);
	return ret;
}

/**
*	Generates random BYTE value
*	@return random
*/
byte CryptUnit::generateRndByte()
{
	byte ret = 0;
	getRandom(&ret, 1);
	return ret;
}

/**
* Derive session encryption key
*/
ErrCode CryptUnit::deriveSessionSecretKey(const Blob& secKey, const Blob& derivationData, Blob& sessionSecKey)
{
	ErrCode ret = eOk;

	Blob tmpData1 = secKey;
	tmpData1.insert(tmpData1.end(), derivationData.begin(), derivationData.end());

	Blob tmpData2(getSHA256HashLength());

	// we repeat two sha256 hash rounds
	ret = hashDataSHA256(tmpData1.data(), tmpData1.size(), tmpData2.data());
	if (ret != eOk)
		return ret;

	ret = hashDataSHA256(tmpData2.data(), tmpData2.size(), tmpData2.data());
	if (ret != eOk)
		return ret;

	// take required length as session secret key
	sessionSecKey.resize(getSecretKeyLength());
	std::copy(tmpData2.begin(), tmpData2.begin() + getSecretKeyLength(), sessionSecKey.begin());

	return ret;
}

ErrCode CryptUnit::xorData(const std::string& data1, const std::string& data2, size_t size, std::string& result)
{
	if (data1.size() < size) {
		return eBadArg;
	}
	if (data2.size() < size) {
		return eBadArg;
	}
	result = "";
	
	byte* buf = (byte*)calloc(size, 1);
	if (!buf)
		return eFatal;

	const byte* ptr1 = (const byte*)data1.c_str();
	const byte* ptr2 = (const byte*)data2.c_str();

	for (size_t i = 0; i < size; i++) {
		buf[i] = ptr1[i] ^ ptr2[i];
	}

	result.assign((const char*)buf, size);
	free(buf);

	return eOk;
}
