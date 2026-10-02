#include "LogHelper.h"
#include <time.h>


LogHelper::LogHelper()
{

}

LogHelper::~LogHelper()
{

	if (file.is_open()) {
		file.close();
	}
}

void LogHelper::log(const char* str)
{
	if (str == NULL) {
		//todo nothing
	}
	else if (file.is_open()) {
		file.write(str, strlen(str));
		//file.write("\n", strlen("\n"));
		file.flush();
	}
	else {
		//str已经包含换行符
		std::cout << str;
	}
}

void LogHelper::init(std::string& logpath)
{
	if (file.is_open()) {
		file.close();
	}
	file.open(logpath, std::ios::app);
}

void dns_log_print(const int loglevel, const char* fmt, ...) {
	if (loglevel <= gLogLevel) {
		int _Result;
		time_t t = time(NULL);
		struct tm local_time;
		localtime_s(&local_time, &t);
		char buffer[1024] = { 0 };
		int buf_offset = 0;
		snprintf(buffer, sizeof(buffer), "%04d-%02d-%02d %02d:%02d:%02d ", local_time.tm_year + 1900,
			local_time.tm_mon + 1, local_time.tm_mday, local_time.tm_hour, local_time.tm_min, local_time.tm_sec);
		buf_offset += 20;
		if (loglevel == LOG_LEVEL_INFO) {
			snprintf(buffer + buf_offset, sizeof(buffer) - buf_offset, "[info]");
			buf_offset += 6;
		}
		else if (loglevel == LOG_LEVEL_DEBUG) {
			snprintf(buffer + buf_offset, sizeof(buffer) - buf_offset, "[debug]");
			buf_offset += 7;
			//buf_offset = strlen(buffer);
		}
		else {
			snprintf(buffer + buf_offset, sizeof(buffer) - buf_offset, "[log%d]", loglevel);
			buf_offset = (int)strlen(buffer);
		}
		va_list _ArgList;
		__crt_va_start(_ArgList, fmt);
		_Result = vsnprintf(buffer + buf_offset, sizeof(buffer) - buf_offset, fmt, _ArgList);
		__crt_va_end(_ArgList);
		logger.log(buffer);
	}
	return;
}

