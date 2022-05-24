#include "FileTools.h"

#include <sstream>
#include <fstream>
#include <iostream>

#include "Logger.h"

#ifdef WIN32
	#include <Windows.h>
	#include <conio.h>
#else
	#include <curses.h>
	#include <string.h>
#endif


/**
*	Load file from disk
*/
#ifdef WIN32
RetCode FileTools::loadFileW(const std::wstring& fileName, std::string& result) {
	size_t len;
	byte* buffer = FileTools::loadFileW(fileName.c_str(), len);
	if (!buffer)
		return eBadFile;
	
	result.assign((const char*)buffer, len);
	free(buffer);
	return eOk;
}
#endif

RetCode FileTools::loadFileA(const std::string& fileName, std::string& result) {
	size_t len;
	byte* buffer = FileTools::loadFileA(fileName.c_str(), len);
	if (!buffer)
		return eBadFile;

	result.assign((const char*)buffer, len);
	free(buffer);
	return eOk;
}


#ifdef WIN32
byte* FileTools::loadFileW(const wchar_t* fileName, size_t& len) {
	std::ifstream::pos_type size;
	byte* memblock = nullptr;
	len = 0U;

	std::ifstream file(fileName, std::ios::in | std::ios::binary | std::ios::ate);
	if (file.is_open()) {
		size = file.tellg();
		len = (unsigned int)size;
		memblock = new byte[len];
		file.seekg(0, std::ios::beg);
		file.read((char*)memblock, size);
		file.close();
	}
	return memblock;
}
#endif

byte* FileTools::loadFileA(const char* fileName, size_t& len) {
	std::ifstream::pos_type size;
	byte* memblock = 0;
	len = 0;

	std::ifstream file(fileName, std::ios::in | std::ios::binary | std::ios::ate);
	if (file.is_open()) {
		size = file.tellg();
		len = (unsigned int)size;
		memblock = new byte[len];
		file.seekg(0, std::ios::beg);
		file.read((char*)memblock, size);
		file.close();
	}
	return memblock;
}


#ifdef WIN32
RetCode FileTools::writeFileW(const std::wstring& fileName, const std::string& body) noexcept {
	FILE* ptr = nullptr;
	errno_t ret = _wfopen_s(&ptr, fileName.c_str(), L"wb+");
	if (ret == 0 && ptr!=nullptr) {
		fwrite(body.c_str(), 1, body.size(), ptr);
		fclose(ptr);
	}
	else {
		return eBadFile;
	}
	return eOk;
}
#endif

RetCode FileTools::writeFileA(const std::string& fileName, const std::string& body) noexcept {
	FILE* ptr = fopen(fileName.c_str(), "wb+");
	if (!ptr) return eBadFile;

	fwrite(body.c_str(), 1, body.size(), ptr);
	fclose(ptr);

	return eOk;
}

#ifdef WIN32
RetCode FileTools::copyFileW(const std::wstring& from, const std::wstring& to) {
	std::string data;
	RetCode ret = FileTools::loadFileW(from, data);
	ASSERTME(ret);
	LOGI("Loaded %u bytes from [%ws]", data.size(), from.c_str());

	ret = FileTools::writeFileW(to, data);
	if (ret == eOk) {
		LOGI("Written %u bytes to [%ws]", data.size(), to.c_str());
	}
	return ret;
}
#endif

RetCode FileTools::copyFileA(const std::string& from, const std::string& to) {
	std::string data;
	RetCode ret = FileTools::loadFileA(from, data);
	ASSERTME(ret);
	LOGI("Loaded %u bytes from [%s]", data.size(), from.c_str());

	ret = FileTools::writeFileA(to, data);
	if (ret == eOk) {
		LOGI("Written %u bytes to [%s]", data.size(), to.c_str());
	}
	return ret;
}


RetCode FileTools::getAvailableDrives(std::vector<std::string>& result) {
#ifdef WIN32
	unsigned int  drives = GetLogicalDrives();
	for (unsigned int i = 0; i < 32; i++) {
		unsigned int res = drives & (1 << i);
		char letter = 'A' + i;
		bool present = (res != 0);

		if (present) {
			result.push_back(std::string("") + letter + std::string(":\\"));
		}
	}
#endif
	return eOk;
}


RetCode FileTools::getRemovablesDrives(std::vector<std::string>& result) {
#ifdef WIN32
	std::vector<std::string> drives;
	ASSERTME(FileTools::getAvailableDrives(drives));
	for (std::vector<std::string>::iterator it = drives.begin(); it != drives.end(); it++) {
		unsigned int driveType = GetDriveTypeA(it->c_str());
		//		LOGI("Drive Type of [%s] is [%u]", it->c_str(), driveType);
		if (driveType == DRIVE_REMOVABLE) {
			result.push_back(*it);
		}
	}
#endif
	return eOk;
}



/**
*	Key folder can be only on removable drive
*	1. Get list of removable drives
*	2. if none - return, if more than one - ask
*/
RetCode FileTools::getKeyFolder(std::string& folder) {
	std::vector<std::string> removeableDrives;
	ASSERTME(getRemovablesDrives(removeableDrives));

	if (removeableDrives.empty()) {
		folder = "";
		return eNotFound;
	}
	if (removeableDrives.size() == 1) {
		folder = removeableDrives.at(0);
		return eOk;
	}
	// few found - ask
	std::cout << "Few removable drives found. Please choose the one with key: " << std::endl;
	for (size_t i = 0; i < removeableDrives.size(); i++) {
		std::cout << "Press [" << i << "] to choose drive [" << removeableDrives.at(i) << "]" << std::endl;
	}
	char ch = GETCH();

	int choice = ch = '0';
	if (choice >= 0 && choice < removeableDrives.size()) {
		folder = removeableDrives.at(choice);
		return eOk;
	}
	std::cout << "Bad input" << std::endl;

	return eFatal;
}