
#include "CommonUtil.h"
#include <Windows.h>
#include "LogHelper.h"
#include <fstream>
#include "curl/curl.h"
#include <sstream>
#include <iostream>
#include <ctime>

std::string GetLastErrorMsg(unsigned long errCode)
{
	std::string err;
	LPTSTR lpBuffer = NULL;
	if (0 == FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, //标志位，决定如何说明lpSource参数，dwFlags的低位指定如何处理换行功能在输出缓冲区，也决定最大宽度的格式化输出行,可选参数。
		NULL,//根据dwFlags标志而定。
		errCode,//请求的消息的标识符。当dwFlags标志为FORMAT_MESSAGE_FROM_STRING时会被忽略。
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),//请求的消息的语言标识符。
		(LPTSTR)&lpBuffer,//接收错误信息描述的缓冲区指针。
		0,//如果FORMAT_MESSAGE_ALLOCATE_BUFFER标志没有被指定，这个参数必须指定为输出缓冲区的大小，如果指定值为0，这个参数指定为分配给输出缓冲区的最小数。
		NULL//保存格式化信息中的插入值的一个数组。
	))
	{//失败
		char tmp[100] = { 0 };
		sprintf_s(tmp, "{未定义错误描述(%d)}", errCode);
		err = tmp;
	}
	else//成功
	{
		//USES_CONVERSION;
		//err = W2A(lpBuffer);
		err = string(lpBuffer);
		LocalFree(lpBuffer);
	}
	return err;
}


/*
* 执行文件的完全路径,包含exe后缀
*/
string getProgramFullPath() {
	static string path;
	if (path.empty()) {
		TCHAR szUnquotedPath[MAX_PATH];
		if (!GetModuleFileName(NULL, szUnquotedPath, MAX_PATH))
		{
			log_info("Cannot get file path (%d)\n", GetLastError());
			path = "";
		}
		path = string(szUnquotedPath);
	}
	return path;
}


bool chenkFileExist(const string& path) {
	std::ifstream f(path.c_str());
	return f.good();
}

void print_curl_version() {
	curl_version_info_data* ver = curl_version_info(CURLVERSION_NOW);
	log_info("curl_version:%s", ver->version);
	//cout << ver->version << endl;
	std::ostringstream pro;
	pro << "protocols:";
	for (int i = 0; ; i++) {
		if (ver->protocols[i] == NULL) {
			//cout << endl;
			break;
		}

		pro << ver->protocols[i] << " ";
	}
	log_debug("%s", pro.str().c_str());
	std::ostringstream feature;
	feature << "feature:";
	for (int i = 0; ; i++) {
		if (ver->feature_names[i] == NULL) {
			//cout << endl;
			break;
		}

		feature << ver->feature_names[i] << " ";
	}
	log_debug("%s", feature.str().c_str());
}

bool check_ipv6_support() {
	bool ret = false;
	curl_version_info_data* ver = curl_version_info(CURLVERSION_NOW);
	//cout << ver->version << endl;
	for (int i = 0; ; i++) {
		if (ver->feature_names[i] == NULL) {
			cout << endl;
			break;
		}
		if (_stricmp(ver->feature_names[i], "IPv6") == 0) {
			ret = true;
			break;
		}
		//cout << ver->feature_names[i] << " ";
	}
	return ret;
}

bool check_https_support() {
	bool ret = false;
	curl_version_info_data* ver = curl_version_info(CURLVERSION_NOW);
	//cout << ver->version << endl;
	for (int i = 0; ; i++) {
		if (ver->protocols[i] == NULL) {
			cout << endl;
			break;
		}
		if (_stricmp(ver->protocols[i], "https") == 0) {
			ret = true;
			break;
		}
		//cout << ver->protocols[i] << " ";
	}
	return ret;
}



//用于计算时间差,而不是计算年月日
long long get_boot_millisecond() {
	//检索自系统启动以来经过的毫秒数, 不受系统时间调整的影响
	return GetTickCount64();
}


bool check_is_remove(char n) {
	if (n == '\n' || n == ' ' || n == '\t') {
		return true;
	}
	return false;
}

//删除空格, 不仅是首尾, 中间的也会删除
string string_remove_blank(string val) {
	auto end = remove_if(val.begin(), val.end(), check_is_remove);
	val.erase(end, val.end());
	return val;
}

