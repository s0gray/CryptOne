#ifndef LOGGER_H
#define LOGGER_H


#include <stdarg.h>
#include <string>
typedef unsigned char byte;

/**
 * Logger output mask
 */
#define LO_CONSOLE		( 1 )
#define LO_FILE			( 2 )
#define LO_DEBUGGER		( 8 )


/**
 * Macros for global logger
 */
#define LOG_INIT			Logger::logger.Init

#define LOG(...)			Logger::logger.Log(__FUNCTION__, __VA_ARGS__)
#define LOG_DATA(...)		Logger::logger.Data(__FUNCTION__, __VA_ARGS__)

#define LOGE(...)		LOG(Log::eErr, __VA_ARGS__)
#define LOGI(...)		LOG(Log::eInfo, __VA_ARGS__)
#define LOGS(...)		LOG(Log::eStream, __VA_ARGS__)
#define LOGSD(...)		LOG(Log::eStreamDbg, __VA_ARGS__)

namespace Log 
{
	enum 
	{
		eErr = 1,
		eInfo,
		eStream,
		eStreamDbg
	};
};

/**
 * class Logger - to log in file
 */
class Logger
{
public:
// Global logger
	static Logger	logger;

// Constructor/Destructor
	Logger(); 
	virtual ~Logger();

// Implementation
	void	Init(int level, const std::wstring& filename);
	void	Log(const std::string &funcName, int level, const char* format, ...);
	void	Data(const std::string &funcName, int level,  const char *title, const char* buf, size_t len);
	void	Data(const std::string& funcName, int level, const char* title, const byte* buf, size_t len);

	void	SetLogLevel(int level);
	void	SetMode(int mode);

	const std::wstring& GetFileName() const;
	void	SetFileName(const std::wstring& fileName);

protected:
	void	LogArgs(int level,  const std::string &funcName, const char* format, va_list args);
	static std::string format(const char* fmt, ...);
	static std::string format_arg_list(const char* fmt, va_list args);

private:
// Members
	int			mMode;
	int			mLevel;
	std::wstring	mFilepath;	/// real full file name
};

#endif // LOGGER_H
