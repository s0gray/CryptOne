#include "Utils.h"

#include <sstream>
#include <fstream>
#include <stdarg.h>

using namespace std;

std::string Utils::IntToHexString(int value) 
{
	std::stringstream stream;
	stream << std::hex << value;
	return std::string(stream.str());
}

/**
*	Load file from disk
*/
ErrCode Utils::loadFileW(const std::wstring& fileName, std::string& result)
{
	size_t len;
	byte* buffer = Utils::loadFileW(fileName.c_str(), len);
	if (!buffer) 
	{
		return eBadFile;
	}
	result.assign((const char*)buffer, len);
	free(buffer);
	return eOk;
}

byte* Utils::loadFileW(const wchar_t* fileName, size_t& len)
{
	ifstream::pos_type size;
	byte* memblock = 0;
	len = 0;

	ifstream file(fileName, ios::in | ios::binary | ios::ate);
	if (file.is_open())
	{
		size = file.tellg();
		len = (unsigned int)size;
		memblock = new byte[len];
		file.seekg(0, ios::beg);
		file.read((char*)memblock, size);
		file.close();
	}
	return memblock;
}


std::string Utils::vectorToString(const std::vector<unsigned char>& data) 
{
	std::string  str(data.begin(), data.end());
	return str;
}

std::string Utils::bin2hex(const std::string& data) 
{
	return Utils::bin2hex((const byte*)data.c_str(), data.size());
}

std::string Utils::bin2hex(const byte* buf, size_t len)
{
	std::string str;
	if (!buf || len == 0U)
		return str;
	for (unsigned int i = 0; i < len; i++) 
	{
		str += Utils::format("%02x", buf[i]);
	}
	return str;
}

std::string Utils::charToHex(char ch) 
{
	std::string str = charToString(ch);
	return Utils::bin2hex(str);
}

std::string Utils::charToString(char ch) 
{
	char tmp[1];
	tmp[0] = ch;
	std::string v(tmp, 1);
	return v;
}

std::string Utils::ws2s(const std::wstring& ws) 
{
	std::string s(ws.begin(), ws.end());
	return s;
}

std::string Utils::format_arg_list(const char* fmt, va_list args)
{
	if (!fmt) return "";
	int   result = -1, length = 256;
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

std::string Utils::format(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	std::string s = format_arg_list(fmt, args);
	va_end(args);
	return s;
}

ErrCode Utils::writeFileW(const std::wstring& fileName, const std::string& body) noexcept
{
	FILE* ptr;
	errno_t ret = _wfopen_s(&ptr, fileName.c_str(), L"wb+");
	if (ret == 0) {
		fwrite(body.c_str(), 1, body.size(), ptr);
		fclose(ptr);
	}
	else {
		return eBadFile;
	}
	return eOk;
}

std::wstring Utils::s2ws(const std::string& value) 
{
	std::wstring w(value.begin(), value.end());
	return w;
}

void Utils::getEnvTempPath(std::wstring& value) 
{
	wchar_t* env_var_buffer = nullptr;
	std::size_t size = 0;
	if (_wdupenv_s(&env_var_buffer, &size, L"TEMP") == 0 &&
		env_var_buffer != nullptr) 
	{
		value = static_cast<const wchar_t*>(env_var_buffer);
	}
}


__int64 Utils::strToInt64(const std::string& s)
{
	return _strtoi64(s.c_str(), NULL, 16);
}

// ret true if OK
bool Utils::parseIniFile(const byte* data, size_t len, std::map<std::string, std::string>& map)
{
	if (!data || len == 0)
		return false;

	char* buff = (char*)&data[0];
	char* buffEnd = buff + len;

	std::string line;
	int lineNumber = 1;

	bool bContinue = true;
	do
	{
		bContinue = getLine(&buff, buffEnd, line);
		if (bContinue || !line.empty())
		{
			std::string key;
			std::string value;
			if (!parseIniFileLine(line, key, value))
			{
				//LOG(Log::eStream, "config parse error: [%s]", line.c_str());
				return false;
			}
			if (!key.empty())
				map[key] = value;

			lineNumber++;
		}
	} while (bContinue);

	return true;
}

/**
	Read one line from config-file buffer.
	return 'true' if end not reached
*/
bool Utils::getLine(char** begin, const char* end, std::string& rLine) //const
{
	rLine.clear();

	char* lineStart = *begin;
	char* lineEnd = lineStart;

	// iterate over valid chars
	while (lineEnd < end && *lineEnd != '\r' && *lineEnd != '\n')
		lineEnd++;

	if (lineEnd != lineStart)
		rLine = std::string(lineStart, lineEnd);

	// skip [\r]\n at the end of line
	if (lineEnd < end && *lineEnd == '\r')
		lineEnd++;

	if (lineEnd < end && *lineEnd == '\n')
		lineEnd++;

	*begin = lineEnd;
	return (lineEnd != end);
}


/**
	 parse line [key=value]
	  # is comment
	 return true if OK, false if error

	 if OK: key = key , value = value
*/
bool Utils::parseIniFileLine(const std::string& str, std::string& key, std::string& value)
{
	key = "";
	value = "";

	if (str.empty())
		return true; //true;

	if (str[0] == '#') // comment
		return true; // true;

	std::string::size_type pos = str.find_first_of("=");
	if (pos == std::string::npos)
		return false;

	std::string _key = str.substr(0, pos);
	std::string _value = str.substr(pos + 1);

	key = Utils::trim(_key);
	value = Utils::trim(_value);
	return true;
}

string Utils::trim(const string& _str)
{
	std::string str = _str;
	int i, j, start, end;

	//ltrim
	for (i = 0; (str[i] != 0 && str[i] <= 32); )
		i++;
	start = i;

	//rtrim
	for (i = 0, j = 0; str[i] != 0; i++)
		j = ((str[i] <= 32) ? j + 1 : 0);
	end = i - j;
	str = str.substr(start, end - start);
	return str;
}

bool Utils::LoadIniFile(const std::wstring& fileName, std::map<std::string, std::string>& map)
{
	size_t len = 0;
	byte* data = Utils::loadFileW(fileName.c_str(), len);
	//LOG(Log::eStream, "Loaded %d bytes from %s", len, fileName);
	if (len == 0 || !data)
		return false;

	bool ret = parseIniFile(data, len, map);
	//LOG(Log::eStream, "config.parse ret %d", ret);

	free( data );
	return ret;
}
