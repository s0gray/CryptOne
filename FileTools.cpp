
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

#include "FileTools.h"

#include <sstream>
#include <fstream>
#include <iostream>

#include "Log.h"

#ifdef WIN32
	#include <Windows.h>
	#include <conio.h>
#else
	#include <curses.h>
	#include <string.h>

	#include <sys/stat.h>
#endif


RetCode FileTools::loadFileA(const std::string& fileName, std::string& result) {
	size_t len = 0U;
	std::ifstream::pos_type size;
	std::ifstream file(fileName, std::ios::in | std::ios::binary | std::ios::ate);
	if (!file.is_open()) return eBadFile;

	size = file.tellg();
	len = (unsigned int)size;

	std::unique_ptr<char[]> memblock(new char[len]);
	file.seekg(0, std::ios::beg);
	file.read(memblock.get(), size);
	file.close();
	
	result.assign(memblock.get(), len);
	return eOk;
}


RetCode FileTools::writeFileA(const std::string& fileName, const std::string& body) noexcept {
	auto myfile = std::fstream(fileName, std::ios::out | std::ios::binary);
	myfile.write(body.c_str(), body.size());
	myfile.close();

	return eOk;
}


RetCode FileTools::copyFileA(const std::string& from, const std::string& to) {
	std::string data;
	ASSERTME( FileTools::loadFileA(from, data) );
	ASSERTME( FileTools::writeFileA(to, data) );
	return eOk;
}


RetCode FileTools::getAvailableDrives(std::vector<std::string>& result) {
#ifdef WIN32
	unsigned int  drives = GetLogicalDrives();
	for (unsigned int i = 0; i < 32U; i++) {
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
		//		LOGGER("Drive Type of [%s] is [%u]", it->c_str(), driveType);
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

bool FileTools::isFolderPresent(const char *folder) {
	if (!folder)
		return false;

	struct stat info;

	if (stat(folder, &info) != 0)
		return false;
	else if (info.st_mode & S_IFDIR)
		return true;
	else
		return false;
}