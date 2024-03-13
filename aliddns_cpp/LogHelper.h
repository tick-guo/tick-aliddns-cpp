#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <stdio.h>


class LogHelper {
public:
	LogHelper();
	~LogHelper();
	void log(const char* str);
	void init(std::string& logpath);

private:

	std::fstream file;

};

extern LogHelper logger;

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

	extern int gLogLevel;

#define LOG_LEVEL_INFO 0
#define LOG_LEVEL_DEBUG 1

#define  log_info(fmt, ...)  	dns_log_print(LOG_LEVEL_INFO, "[%s:%d]"##fmt"\n" ,__FUNCTION__,__LINE__,##__VA_ARGS__)
#define  log_debug(fmt, ...)  	dns_log_print(LOG_LEVEL_DEBUG, "[%s:%d]"##fmt"\n" ,__FUNCTION__,__LINE__,##__VA_ARGS__)

	void dns_log_print(const int loglevel, const char* fmt, ...);


#ifdef __cplusplus
}
#endif // __cplusplus

