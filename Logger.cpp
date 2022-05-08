
#include "Logger.h"

#include <stdio.h>
#include <time.h>
#include <stdarg.h>

#if defined(WIN32) || defined(_WIN64)
	#include <windows.h>
	#include <direct.h>
#else
	#include <unistd.h>
	#include <string.h>
#endif

#include <chrono>
#include <iostream>


#define MAX_LOG_LEN		1024
#define MAX_DATA_LEN	128

/**
 * Global logger instance
 */
Logger Logger::logger;

/**
*	Logger constructor
*/
Logger::Logger() : mMode(1), mLevel(0)
{
	mFilepathA = "";
	mFilepathW = L"";
}

/**
*	Logger destructor
*/
Logger::~Logger()
{
}

/**
*	Initiate file logger with level and fileName
*	@param	level		[IN]	level of logger
*	@param	fileName	[IN]	file name
*/
#ifdef WIN32
void Logger::Init(int level, const std::wstring& fileName)
{
	mLevel = level;

	if (!fileName.empty()) 
	{
		SetFileName(fileName);
		// check if file can be opened
		FILE* fp;
		errno_t ret = _wfopen_s(&fp, fileName.c_str(), L"w");
		if (ret==0 && fp!=nullptr) 
		{
			fclose(fp);
			mMode |= LO_FILE;			
		}
	}
}
#endif

/**
*	Initiate file logger with level and fileName
*	@param	level		[IN]	level of logger
*	@param	fileName	[IN]	file name
*/
void Logger::Init(int level, const std::string& fileName)
{
	mLevel = level;

	if (!fileName.empty())
	{
		SetFileName(fileName);
		// check if file can be opened
		FILE* fp;
		fp = fopen(fileName.c_str(), "w");
		if ( fp != nullptr)
		{
			fclose(fp);
			mMode |= LO_FILE;
		}
	}
}

/**
*	Set file name for logge
*/
void Logger::SetFileName(const std::wstring& fileName) 
{ 
#ifdef WIN32
	time_t t = time(NULL);
	struct tm tmp;
	localtime_s(&tmp, &t);
	wchar_t ts[200];
	wcsftime(ts, 200, L".%y%m%d-%H%M.txt", &tmp);
	mFilepathW = fileName + ts;
#endif
}

/**
*	Set file name for logge
*/
void Logger::SetFileName(const std::string& fileName)
{
	time_t t = time(NULL);

#ifdef WIN32
	struct tm tmp;
	localtime_s(&tmp, &t);
	char ts[200];
	strftime(ts, 200, ".%y%m%d-%H%M.txt", &tmp);
#else
	struct tm *tmp = localtime(&t);
	char ts[200];
	strftime(ts, 200, ".%y%m%d-%H%M.txt", tmp);
#endif
	mFilepathA = fileName + ts;
}


void Logger::LogArgs(int , const std::string& funcName, const char* format, va_list args)
{
	// check if any logging mode is enabled
	if (!((mMode & LO_CONSOLE) || (mMode & LO_DEBUGGER) || (mMode & LO_FILE)))
		return;

	char buf[ MAX_LOG_LEN + 1 ];
	vsnprintf( buf, MAX_LOG_LEN, format, args );

	buf[ MAX_LOG_LEN ] = '\x0';
	std::string str = "[";

	time_t t = time(NULL);
	_INT64 now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	_INT64 secs = (_INT64)(now / 1000);
	_INT64 now_ms = now - secs * 1000;
	char ts[201] = { 0 };

#ifdef WIN32
	struct tm tmp;
	localtime_s(&tmp, &t);
	strftime(ts, 200, "%Y-%m-%d %H:%M:%S", &tmp);

#else
	struct tmp* tmp = localtime(&t);
	strftime(ts, 200, "%Y-%m-%d %H:%M:%S", tmp);

#endif

	str += ts;
	str += Logger::format(".%03u", (unsigned int)now_ms);
	str += "]   ["+funcName+"] ";
	str += buf;
	str += "\n";

	if( mMode & LO_CONSOLE ) 
	{
#ifndef RUN_AS_SERVICE
		printf( "%s", str.c_str() );
#endif
	}
	if( mMode & LO_FILE ) 
	{
		FILE* fp = nullptr;

#ifdef WIN32
		if(!mFilepathW.empty())
			_wfopen_s(&fp, mFilepathW.c_str(), L"a");
		else
			fp = fopen(mFilepathA.c_str(), "a");

#else
		fp = fopen(mFilepathA.c_str(), "a");

#endif


		if( fp!=nullptr )
		{
			fputs( str.c_str(), fp );
			fflush(fp);
			fclose( fp );
		}
	}
	if( mMode & LO_DEBUGGER )
	{
#ifdef WIN32
		OutputDebugStringA( str.c_str() );
#else
#ifndef RUN_AS_SERVICE
		printf( "%s", str.c_str() );
#endif
#endif
	}
}

/**
* Log live in printf-format
*/
void Logger::Log(const std::string &funcName, int level, const char *format, ...)
{
	if( mLevel < level )
		return;
	va_list	args;
	va_start( args, format );
	LogArgs( level, funcName, format, args );
	va_end( args );
}

/**
*	Log data block
*/
void  Logger::Data(const std::string& funcName, int level, const char* title, const byte* buf, size_t len)
{
	Data(funcName, level, title, (const char*)buf, len);;
}

void Logger::Data(const std::string &funcName, int level, const char *title, const char *data, size_t len)
{
	if( mLevel < level )
		return;
	Log(funcName, level, title);

	char str[ 256 ];
	memset(str, 0, 256);
	int pos = 0;
	int bytesPerLine = 0;
	size_t sz = len;
#ifdef MAX_DATA_LEN
	if(sz>MAX_DATA_LEN) sz = MAX_DATA_LEN;
#endif
	for(size_t i=0; i<sz; i ++)
	{
		if(bytesPerLine > 0 && ( bytesPerLine % 16 ) == 0) 
		{
			Log( funcName, level, str );
			pos = bytesPerLine = 0;
		}
		int n = sprintf( &str[pos], "%02X ", (0xFF & data[i]) );
		pos += n;
		bytesPerLine ++;
	}
	if( pos )
		Log(funcName, level, str);
}


/**
* Format printf style string to std::string
*/
std::string Logger::format(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	std::string s = format_arg_list(fmt, args);
	va_end(args);
	return s;
}

/**
*	Format printf list of arguments to string
*/
std::string Logger::format_arg_list(const char* fmt, va_list args)
{
	if (!fmt) return "";
	int   result = -1;
	size_t length = 256;
	char* buffer = 0;
	while (result == -1)
	{
		if (buffer) delete[] buffer;
		buffer = new char[length + 1];
		memset(buffer, 0, length + 1);
		result = vsnprintf(buffer, length, fmt, args);
		length *= 2;
	}
	std::string s(buffer);
	delete[] buffer;
	return s;
}

const std::wstring& Logger::GetFileNameW() const
{ 
	return mFilepathW; 
}

const std::string& Logger::GetFileNameA() const
{
	return mFilepathA;
}

void	Logger::SetLogLevel(int level) { mLevel = level; }
void	Logger::SetMode(int mode) { mMode = mode; }