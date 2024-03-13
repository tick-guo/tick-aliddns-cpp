// AliddnsLib.cpp : 定义静态库的函数。
//
#include <iostream>
#include <Windows.h>
#include <mutex>
#include <alibabacloud/core/AlibabaCloud.h>
#include <alibabacloud/alidns/AlidnsClient.h>

using namespace std;

using namespace AlibabaCloud;
using namespace AlibabaCloud::Alidns;


// TODO: 这是一个库函数示例
void fnAliddnsLib()
{
	AlibabaCloud::InitializeSdk();
	ClientConfiguration configuration("cn-hangzhou");
	shared_ptr<AlidnsClient> g_client(new AlidnsClient("dnsConfig.AccessKeyId", "dnsConfig.AccessSecret", configuration));
	//接口可用性检测, 源库接口太多, 去掉不要的
	{
		Model::DescribeDomainRecordsRequest request;
		auto result = g_client->describeDomainRecords(request);
		//std::vector<Model::DescribeDomainRecordsResult::Record> records = result.result().getDomainRecords();
		auto mRecords = result.result().getDomainRecords();
	}

	{
		Model::AddDomainRecordRequest request;
		auto res = g_client->addDomainRecord(request);
	}

	{
		Model::UpdateDomainRecordRequest request;
		auto res = g_client->updateDomainRecord(request);
	}

	AlibabaCloud::ShutdownSdk();

}

