#include "Log.h"

#include "Tools.h"

#include <iostream>
#include <format>

Log::Log() {
}

Log::~Log() {
}


void Log::logs(const char* func, const std::string& format) {
	std::cout << "[" << func << "] " << format << std::endl;
}


