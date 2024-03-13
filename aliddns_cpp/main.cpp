// aliddns_cpp.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <string>
#include <sstream>
#include "ServiceHelper.h"
#include "LogHelper.h"
#include "DnsOp.h"
#include "CommonUtil.h"
#include "curl/curl.h"

//libcurl need this
#pragma comment(lib, "ws2_32.lib")

//#include <atlconv.h>

using namespace std;

string gCmdName;


class LIB_AUTO_INIT {
public:
	LIB_AUTO_INIT() {
		curl_global_init(CURL_GLOBAL_ALL);

		/* 初始化 SDK */
		AlibabaCloud::InitializeSdk();

	}
	~LIB_AUTO_INIT() {
		/* 关闭 SDK */
		AlibabaCloud::ShutdownSdk();

		curl_global_cleanup();
		//log_info("析构全局对象");
	}
};

void DisplayHelp() {
	cout << "\nHelp:\n";
	cout << gCmdName << " install\t\t install the program as a service " << SVCNAME << endl;
	cout << gCmdName << " delete\t\t delete the service: " << SVCNAME << endl;
	cout << gCmdName << " start\t\t start the service: " << SVCNAME << endl;
}

string getCmdName(string path) {
	size_t offset = 0;
	offset = path.find_last_of('/');
	if (offset == std::string::npos) {
		offset = path.find_last_of('\\');
	}
	if (offset == string::npos) {
		return path;
	}
	else {
		string t = path.substr(offset + 1);
		return t;
	}
}

LogHelper logger;
int gLogLevel = LOG_LEVEL_INFO;
static LIB_AUTO_INIT curl_init_done;

/*
*
*
*/
static string VERSION_NOTICE = "aliddns v2.8 build 20240313 by tick_guo";

int main(int argc, char* argv[])
{
	int ret = 0;
	string logfile = getProgramFullPath() + ".log";
	logger.init(logfile);
	gLogLevel = read_log_level_only();
	//
	cout << VERSION_NOTICE << endl;
	log_info("%s", VERSION_NOTICE.c_str());

	gCmdName = getCmdName(argv[0]);
	ServiceHelper svcHelper;
	for (int i = 0; i < argc; i++) {
		//cout << "argc:" << i << " [" << argv[i] << "]" << endl;
	}

	print_curl_version();
	bool ipv6 = check_ipv6_support();
	bool https = check_https_support();
	log_info("ipv6_support:%s https_support:%s", ipv6 ? "yes" : "no", https ? "yes" : "no");
	printf("ipv6_support:%s https_support:%s", ipv6 ? "yes" : "no", https ? "yes" : "no");

	if (argc >= 2) {
		if (string(argv[1]) == "install") {
			cout << "install the servie\n";
			ret = svcHelper.InstallService();
			if (ret == 0) {
				//cout << "install success : " << SVCNAME << endl;
			}
			else {
				cerr << "install " << SVCNAME << " failed : " << GetLastErrorMsg(ret) << endl;
			}
			return ret;
		}
		else if (string(argv[1]) == "delete") {
			cout << "delete the servie\n";
			ret = svcHelper.stopService();
			if (ret) {
				cout << GetLastErrorMsg(ret) << endl;
			}
			ret = svcHelper.DeleteService();
			if (ret) {
				cout << GetLastErrorMsg(ret) << endl;
			}
			return ret;

		}
		else if (string(argv[1]) == "start") {
			cout << "start the servie\n";
			ret = svcHelper.startService();
			if (ret) {
				cout << GetLastErrorMsg(ret) << endl;
			}
			return ret;

		}

	}

	//这个函数会阻塞程序, 直到服务退出
	ret = svcHelper.ServiceRunAndWaitStop();
	if (ret == ERROR_FAILED_SERVICE_CONTROLLER_CONNECT) {
		cout << GetLastErrorMsg(ret) << endl;
		cout << "程序没有在服务模式下运行\n";
		LoadConfig createdefault;
		DisplayHelp();

#if _DEBUG//debug
		DnsOp dnsOp;
		dnsOp.debug();
		Sleep(150000);
		dnsOp.WakeAndStop();
#endif

	}
	else if (ret == ERROR_SERVICE_ALREADY_RUNNING) {
		cout << GetLastErrorMsg(ret) << endl;
	}
	log_debug("退出main");

	return ret;
}

