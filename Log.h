#pragma once

#include "Typedefs.h"

#define LOGGER(...)		Log::logs(__func__, __VA_ARGS__)
//#define LOGE(x) {}
//#define LOGI(x) {}
//#define LOG_INIT(x) {}

class Log {
public:
	Log();
	~Log();

	static void logs(const char* func, const std::string& msg);


};

