#pragma once

#include <string>
#include <iostream>
#include <map>
#include "json/json.h"

using namespace std;


class DnsConfig {
public:
	DnsConfig();
	//# 将accessKeyId改成自己的accessKeyId
	string AccessKeyId;
	//# 将accessSecret改成自己的accessSecret
	string AccessSecret;
	//# 是否开启ipv4 ddns解析,1为开启，0为关闭
	int Ipv4Flag;
	//# 是否开启ipv6 ddns解析,1为开启，0为关闭
	int	Ipv6Flag;
	//# 你的主域名
	string Domain;
	//# 要进行ipv4 ddns解析的子域名
	string	NameIpv4;
	//# 要进行ipv6 ddns解析的子域名
	string	NameIpv6;
	//# 日志同时输出到文件,1为开启，0为关闭
	//int	LogFileFlag;
	//# 探测本机公共ip的服务器,可以不配置,有内部默认值,默认服务器挂了,可以设置新的服务器
	string Ip4Url; //= https://api-ipv4.ip.sb/ip
	string	Ip6Url; //= https://api6.ipify.org
	int timeInterval;
	int LogLevel;

	Json::Value getDefaultJson();

private:
	Json::Value root;
};



class LoadConfig {
public:
	LoadConfig();

	void createDefaultJson();
	void readConfig();

	DnsConfig dnsConfig;
	string path;
};


int read_log_level_only();

