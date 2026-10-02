#pragma once

#include <mutex>
#include <thread>
#include <condition_variable>
#include <iostream>
#include <alibabacloud/core/AlibabaCloud.h>
#include <alibabacloud/alidns/AlidnsClient.h>
#include "LoadConfig.h"

using namespace std;

using namespace AlibabaCloud;
using namespace AlibabaCloud::Alidns;

enum class IpStatus {
	IP_IS_SAME = 1,
	IP_NOT_EXIST,
	IP_IS_CHANGE,
};

enum class IpType {
	IP_AUTO = 1,
	IP_V4,
	IP_V6,
};

class DnsOp {
public:
	DnsOp();
	~DnsOp();
	void debug();
	void WakeAndStop();

private:
	void thread_function();
	int getRecords();
	IpStatus getRecordIdByPR(string PR, string ip, bool isV4, string& RecordIdOut);
	int addDns(bool isV4, string ip);
	int updateDns(bool isV4, string ip, string RecordId);

private:
	int mStop;
	shared_ptr<std::thread> pThread;
	mutex mMutex;
	condition_variable mConditon;
	std::vector<Model::DescribeDomainRecordsResult::Record> mRecords;
	DnsConfig dnsConfig;
	shared_ptr<AlidnsClient> g_client;
};
