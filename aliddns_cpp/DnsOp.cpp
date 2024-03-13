//#include "httplib.h"
#include <iostream>
#include "DnsOp.h"
#include <Windows.h>
#include <synchapi.h>
#include <mutex>
#include "LogHelper.h"
#include "curl/curl.h"
#include <sstream>
#include <algorithm>

#include <iostream>
#include <alibabacloud/core/AlibabaCloud.h>
#include <alibabacloud/alidns/AlidnsClient.h>
#include "CommonUtil.h"

using namespace std;

using namespace AlibabaCloud;
using namespace AlibabaCloud::Alidns;

//
static string const ChromeUserAgent = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/116.0.0.0 Safari/537.36 Edg/116.0.1938.69";

class curl_memory {
public:
	curl_memory() {
		response = NULL;
		size = 0;
	};
	~curl_memory() {
		if (response) {
			free(response);
			response = NULL;
		}
		size = 0;
	};
	char* response;
	size_t size;
};

static size_t curl_callback(void* data, size_t size, size_t nmemb, void* clientp)
{
	size_t realsize = size * nmemb;
	curl_memory* mem = (curl_memory*)clientp;

	char* ptr = (char*)realloc(mem->response, mem->size + realsize + 1);
	if (ptr == NULL)
		return 0;  /* out of memory! */

	mem->response = ptr;
	memcpy(&(mem->response[mem->size]), data, realsize);
	mem->size += realsize;
	mem->response[mem->size] = 0;

	return realsize;
}
/*
* 快速,不需要服务器返回ip
可以快速获取到连接IP,但是并不是服务器感知到的公网IP, 只是本机局域网IP
本机有公网ipv6就比较准确有效
本机是局域网ipv4,则获取的是本机局域网ipv4
*/
string getPublicIpFaster(string& url, IpType type) {
	auto time = get_boot_millisecond();
	string ret_ip;
	string ip_type_string = "auto";

	//可以获取到连接IP,但是并不是服务器感知到的公网IP, 只是本机局域网IP
	CURL* curl = curl_easy_init();
	char* ip = NULL;;

	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_USERAGENT, ChromeUserAgent.c_str());
	curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);//仅连接,不传输数据
	//双栈域名时,强制使用ip4或者ip6连接, 有效
	if (type == IpType::IP_V4) {
		curl_easy_setopt(curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4);
		ip_type_string = "v4";
	}
	else if (type == IpType::IP_V6) {
		curl_easy_setopt(curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V6);
		ip_type_string = "v6";
	}//else auto

	/* Perform the transfer */
	auto res = curl_easy_perform(curl);//debug
	/* Check for errors */
	if ((res == CURLE_OK) &&
		!curl_easy_getinfo(curl, CURLINFO_LOCAL_IP, &ip) && ip) {
		log_debug("Local IP%s: %s, 耗时:%lldms", ip_type_string.c_str(), ip, get_boot_millisecond() - time);
		ret_ip = string(ip);
	}
	else {
		log_info("失败:%s 原因:[err%d]%s , 耗时:%lldms", url.c_str(), res, curl_easy_strerror(res), get_boot_millisecond() - time);
	}
	/* always cleanup */
	curl_easy_cleanup(curl);

	return ret_ip;
}

/*
准确,但是较慢, 需要服务器返回ip
*/
string getPublicIpLegacy(string& url, IpType type) {
	string ip;
	curl_memory chunk;
	auto time = get_boot_millisecond();
	CURL* curl = curl_easy_init();
	//curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_USERAGENT, ChromeUserAgent.c_str());
	/* send all data to this function  */
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_callback);
	/* we pass our 'chunk' struct to the callback function */
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&chunk);
	//每次操作总时间不超过10s
	curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 10L * 1000L);

	if (type == IpType::IP_V4) {
		curl_easy_setopt(curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4);
	}
	else if (type == IpType::IP_V6) {
		curl_easy_setopt(curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V6);
	}//else auto

	/* Perform the transfer */
	auto res = curl_easy_perform(curl);
	time = get_boot_millisecond() - time;
	/* Check for errors */
	if (res != CURLE_OK) {
		log_info("失败:%s 原因:[err%d]%s , 耗时:%lldms", url.c_str(), res, curl_easy_strerror(res), time);
	}
	else {
		if (time > 3000) {
			log_info("警告 成功:%s, 但耗时较长:%lldms", url.c_str(), time);
		}
		else {
			log_debug("成功:%s, 耗时:%lldms", url.c_str(), time);
		}
	}
	/* always cleanup */
	curl_easy_cleanup(curl);
	if (chunk.response) {
		ip = string(chunk.response);
		ip = string_remove_blank(ip);
	}
	log_debug("获得ip:%s", ip.c_str());
	return ip;
}

string getPublicIp(string& url, IpType type) {
	if (type == IpType::IP_V6) {
		return getPublicIpFaster(url, type);
	}
	else {
		return getPublicIpLegacy(url, type);
	}
}

void DnsOp::thread_function()
{
	string ipv4, ipv6;
	int ret = 0;
	long long index = 0;
	int updateCahche = 1;
	int timeInterval = dnsConfig.timeInterval;
	log_info("开始持续检测IP变化,检测时间间隔:%ds", timeInterval);
	while (!mStop) {
		index++;
		log_debug("检测次数:%d", index);
		//降低HTTP API的调用, 直接用本地缓存判断改变
		if (updateCahche) {
			log_info("更新缓存");
			ret = getRecords();
			if (ret != 0) {
				unique_lock<mutex> lock_on(mMutex);
				if (mStop) {
					break;
				}
				mConditon.wait_for(lock_on, std::chrono::seconds(timeInterval));
				continue;
			}
			updateCahche = 0;
		}

		if (dnsConfig.Ipv4Flag) {
			ipv4 = getPublicIp(dnsConfig.Ip4Url, IpType::IP_V4);
			if (ipv4.empty()) {
				log_info("ipv4获取失败");
			}
			else {
				//todo update
				//log_info("ipv4改变:%s", ipv4.c_str());
				string RecordId;
				auto ret = getRecordIdByPR(dnsConfig.NameIpv4, ipv4, true, RecordId);
				if (ret == IpStatus::IP_NOT_EXIST) {
					addDns(true, ipv4);
					updateCahche++;
				}
				else if (ret == IpStatus::IP_IS_CHANGE) {
					updateDns(true, ipv4, RecordId);
					updateCahche++;
				}
				else if (ret == IpStatus::IP_IS_SAME) {

				}
				else {
					log_info("未知返回值%d", ret);
				}
			}
		}
		if (dnsConfig.Ipv6Flag) {
			ipv6 = getPublicIp(dnsConfig.Ip6Url, IpType::IP_V6);
			if (ipv6.empty()) {
				log_info("ipv6获取失败");
			}
			else {
				//todo update
				//log_info("ipv6改变:%s", ipv6.c_str());
				string RecordId;
				auto ret = getRecordIdByPR(dnsConfig.NameIpv6, ipv6, false, RecordId);
				if (ret == IpStatus::IP_NOT_EXIST) {
					addDns(false, ipv6);
					updateCahche++;
				}
				else if (ret == IpStatus::IP_IS_CHANGE) {
					updateDns(false, ipv6, RecordId);
					updateCahche++;
				}
				else if (ret == IpStatus::IP_IS_SAME) {

				}
				else {
					log_info("未知返回值%d", ret);
				}
			}
		}

		{
			unique_lock<mutex> lock_on(mMutex);
			if (mStop) {
				break;
			}
			mConditon.wait_for(lock_on, std::chrono::seconds(timeInterval));
		}

	}
	log_info("已退出检测");
}

DnsOp::DnsOp()
{
	mStop = 0;

	LoadConfig loadconfig;
	dnsConfig = loadconfig.dnsConfig;

	/* 配置实例 */
	ClientConfiguration configuration("cn-hangzhou");
	g_client.reset(new AlidnsClient(dnsConfig.AccessKeyId, dnsConfig.AccessSecret, configuration));
	pThread.reset(new thread(&DnsOp::thread_function, this));
}

DnsOp::~DnsOp()
{
	//log_info("完成析构");
}

void DnsOp::debug()
{
	log_info("Enter debug");
	log_info("debug ip auto: %s", getPublicIp(dnsConfig.Ip4Url, IpType::IP_AUTO).c_str());
	log_info("debug ipv4: %s", getPublicIp(dnsConfig.Ip4Url, IpType::IP_V4).c_str());
	log_info("debug ipv6: %s", getPublicIp(dnsConfig.Ip6Url, IpType::IP_V6).c_str());
	log_info("Exit debug");
	return;
}

int DnsOp::getRecords()
{
	Model::DescribeDomainRecordsRequest request;
	request.setDomainName(dnsConfig.Domain);
	request.setPageNumber(1);
	request.setPageSize(500);

	auto result = g_client->describeDomainRecords(request);
	if (!result.isSuccess()) {
		log_info("获取DNS解析列表失败: %s", result.error().errorCode().c_str());
		return -1;
	}

	//std::vector<Model::DescribeDomainRecordsResult::Record> records = result.result().getDomainRecords();
	mRecords = result.result().getDomainRecords();

	return 0;
}


/*
*
{"DomainName":"baise.tk","Line":"default","Locked":false,"RR":"tb","RecordId":"768988894054603776","Status":"ENABLE",
"TTL":600,"Type":"AAAA","Value":"2409:8a62:3e2:f2c0:799c:d0cf:6423:3
bc4","Weight":1}
*/
IpStatus DnsOp::getRecordIdByPR(string PR, string ip, bool isV4, string& RecordIdOut) {

	string aType = "AAAA";
	if (isV4) {
		aType = "A";
	}
	for (int i = 0; i < mRecords.size(); i++) {
		//fmt.Println("Index =", index, "Value =", value)
		if (mRecords[i].rR == PR && mRecords[i].type == aType)
		{
			RecordIdOut = mRecords[i].recordId;
			if (mRecords[i].value == ip) {
				log_debug("ip地址没变:%s", ip.c_str());
				return IpStatus::IP_IS_SAME;
			}
			else {
				//修改
				return IpStatus::IP_IS_CHANGE;
			}
		}
	}
	//添加
	return IpStatus::IP_NOT_EXIST;
}

int DnsOp::addDns(bool isV4, string ip) {
	string name = dnsConfig.NameIpv6;
	string aType = "AAAA";
	if (isV4) {
		name = dnsConfig.NameIpv4;
		aType = "A";
	}

	log_info("添加解析: %s.%s", name.c_str(), dnsConfig.Domain.c_str());

	Model::AddDomainRecordRequest request;
	request.setDomainName(dnsConfig.Domain);
	request.setRR(name);
	request.setType(aType);
	request.setValue(ip);

	auto res = g_client->addDomainRecord(request);
	if (res.isSuccess()) {
		log_info("添加解析成功:%s", ip.c_str());
		return 0;
	}
	else {
		log_info("添加解析失败:%s", res.error().errorCode().c_str());
		return -1;
	}
}

int DnsOp::updateDns(bool isV4, string ip, string RecordId) {
	string name = dnsConfig.NameIpv6;
	string aType = "AAAA";
	if (isV4) {
		name = dnsConfig.NameIpv4;
		aType = "A";
	}

	log_info("更新解析: %s.%s", name.c_str(), dnsConfig.Domain.c_str());
	Model::UpdateDomainRecordRequest request;
	request.setRecordId(RecordId);
	request.setRR(name);
	request.setType(aType);
	request.setValue(ip);

	auto res = g_client->updateDomainRecord(request);

	if (res.isSuccess()) {
		log_info("更新解析成功:%s", ip.c_str());
		return 0;
	}
	else {
		log_info("更新解析失败:%s", res.error().errorCode().c_str());
		return -1;
	}
}

void DnsOp::WakeAndStop()
{
	{
		unique_lock<mutex> lock_on(mMutex);
		mStop = 1;
		mConditon.notify_all();
		log_info("准备退出");
	}
	pThread.get()->join();
}


int main_sample(int argc, char** argv)
{
	/* 初始化 SDK */
	AlibabaCloud::InitializeSdk();

	/* 配置实例 */
	ClientConfiguration configuration("cn-hangzhou");
	AlidnsClient client("<your-access-key-id>", "<your-access-key-secret>", configuration);

	/* 创建API请求并设置参数 */
	Model::AddCustomLineRequest request;

	/* 该参数值为假设值，请您根据实际情况进行填写 */
	request.setLang("your_value");

	/* 该参数值为假设值，请您根据实际情况进行填写 */
	request.setDomainName("your_value");

	/* 该参数值为假设值，请您根据实际情况进行填写 */
	request.setLineName("your_value");


	auto outcome = client.addCustomLine(request);
	if (!outcome.isSuccess())
	{
		/* 异常处理 */
		std::cout << outcome.error().errorCode() << std::endl;
		AlibabaCloud::ShutdownSdk();
		return(-1);
	}

	//std::cout << "totalCount: " << outcome.result().getTotalCount() << std::endl;

	/* 打印您需要的返回值，此处打印的是此次请求的 RequestId */
	//std::cout << outcome.getRequestId() << std::endl;

	/* 关闭 SDK */
	AlibabaCloud::ShutdownSdk();
	return(0);
}

