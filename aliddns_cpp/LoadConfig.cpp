
#include "LoadConfig.h"
#include "LogHelper.h"
#include "CommonUtil.h"
#include "json/json.h"


string KEY_AccessKeyId = "AccessKeyId";
string KEY_AccessSecret = "AccessSecret";
string KEY_Ipv4Flag = "Ipv4Flag";
string KEY_Ipv6Flag = "Ipv6Flag";
string KEY_Domain = "Domain";
string KEY_NameIpv4 = "NameIpv4";
string KEY_NameIpv6 = "NameIpv6";
string KEY_Ip4Url = "Ip4Url";
string KEY_Ip6Url = "Ip6Url";
string KEY_TimeInterval = "TimeInterval";
string KEY_LogLevel = "LogLevel";

DnsConfig::DnsConfig()
{
	AccessKeyId = "AccessKeyId";
	AccessSecret = "AccessSecret";
	Ipv4Flag = 1;
	Ipv6Flag = 1;
	Domain = "baise.tk";
	NameIpv4 = "tb4";
	NameIpv6 = "tb6";
	Ip4Url = "http://4.ipw.cn";
	Ip6Url = "http://api6.ipify.org";
	timeInterval = 30;
	LogLevel = 0;

	//
	root[KEY_AccessKeyId] = this->AccessKeyId;
	root[KEY_AccessKeyId].setComment(string("//将accessKeyId改成自己的accessKeyId"), Json::CommentPlacement::commentAfterOnSameLine);
	root[KEY_AccessSecret] = this->AccessSecret;
	root[KEY_AccessSecret].setComment(string("//将accessSecret改成自己的accessSecret"), Json::CommentPlacement::commentAfterOnSameLine);
	root[KEY_Ipv4Flag] = this->Ipv4Flag;
	root[KEY_Ipv4Flag].setComment(string("//是否开启ipv4 ddns解析,1为开启，0为关闭"), Json::CommentPlacement::commentAfterOnSameLine);
	root[KEY_Ipv6Flag] = this->Ipv6Flag;
	root[KEY_Ipv6Flag].setComment(string("//是否开启ipv6 ddns解析,1为开启，0为关闭"), Json::CommentPlacement::commentAfterOnSameLine);
	root[KEY_Domain] = this->Domain;
	root[KEY_Domain].setComment(string("//你的主域名"), Json::CommentPlacement::commentAfterOnSameLine);
	root[KEY_NameIpv4] = this->NameIpv4;
	root[KEY_NameIpv4].setComment(string("//要进行ipv4 ddns解析的子域名"), Json::CommentPlacement::commentAfterOnSameLine);
	root[KEY_NameIpv6] = this->NameIpv6;
	root[KEY_NameIpv6].setComment(string("//要进行ipv6 ddns解析的子域名"), Json::CommentPlacement::commentAfterOnSameLine);
	root[KEY_Ip4Url] = this->Ip4Url;
	root[KEY_Ip4Url].setComment(string("//探测本机公共ipv4的服务器,可以不配置,有内部默认值,默认服务器挂了,可以设置新的服务器"), Json::CommentPlacement::commentAfterOnSameLine);
	root[KEY_Ip6Url] = this->Ip6Url;
	root[KEY_Ip6Url].setComment(string("//探测本机公共ipv6的服务器,可以不配置,有内部默认值,默认服务器挂了,可以设置新的服务器"), Json::CommentPlacement::commentAfterOnSameLine);
	root[KEY_TimeInterval] = this->timeInterval;
	root[KEY_TimeInterval].setComment(string("//检测ip变化的时间间隔,默认30秒"), Json::CommentPlacement::commentAfterOnSameLine);
	root[KEY_LogLevel] = this->LogLevel;
	root[KEY_LogLevel].setComment(string("//log级别, 默认0: 少量log, 1: 更详细的log"), Json::CommentPlacement::commentAfterOnSameLine);

}

Json::Value DnsConfig::getDefaultJson()
{
	return root;
}

LoadConfig::LoadConfig()
{
	path = getProgramFullPath() + ".json";
	readConfig();

}

void LoadConfig::createDefaultJson()
{
	DnsConfig default_value;
	Json::Value root = default_value.getDefaultJson();

	fstream file;
	file.open(path, std::ios::trunc | std::ios::out);
	if (!file.is_open()) {
		log_info("配置文件打开失败:%s", path.c_str());
		return;
	}

	Json::StyledStreamWriter jsWrite;
	jsWrite.write(file, root);

}

void LoadConfig::readConfig()
{
	Json::Reader reader;
	if (!chenkFileExist(path)) {
		log_info("配置文件不存在,创建默认配置文件");
		this->createDefaultJson();
	}

	fstream f(path);
	Json::Value root;
	reader.parse(f, root, true);

	int need_update = 0;//如果有缺少的元素,则需要更新文件
	DnsConfig default_value;
	Json::Value defultJson = default_value.getDefaultJson();
	auto members = defultJson.getMemberNames();
	for (int i = 0; i < members.size(); i++) {
		if (!root.isMember(members[i])) {
			need_update++;
			root[members[i]] = defultJson[members[i]];
			log_info("配置文件没有 %s = %s ,自动添加", members[i].c_str(), root[members[i]].asString().c_str());
		}
	}

	if (root.isMember(KEY_AccessKeyId))
	{
		dnsConfig.AccessKeyId = root[KEY_AccessKeyId].asString();
	}
	if (root.isMember(KEY_AccessSecret))
	{
		dnsConfig.AccessSecret = root[KEY_AccessSecret].asString();
	}
	if (root.isMember(KEY_Ipv4Flag) && root[KEY_Ipv4Flag].isInt())
	{
		dnsConfig.Ipv4Flag = root[KEY_Ipv4Flag].asInt();
	}
	if (root.isMember(KEY_Ipv6Flag) && root[KEY_Ipv6Flag].isInt())
	{
		dnsConfig.Ipv6Flag = root[KEY_Ipv6Flag].asInt();
	}
	if (root.isMember(KEY_Domain))
	{
		dnsConfig.Domain = root[KEY_Domain].asString();
	}
	if (root.isMember(KEY_NameIpv4))
	{
		dnsConfig.NameIpv4 = root[KEY_NameIpv4].asString();
	}
	if (root.isMember(KEY_NameIpv6))
	{
		dnsConfig.NameIpv6 = root[KEY_NameIpv6].asString();
	}
	if (root.isMember(KEY_Ip4Url))
	{
		dnsConfig.Ip4Url = root[KEY_Ip4Url].asString();
	}
	if (root.isMember(KEY_Ip6Url))
	{
		dnsConfig.Ip6Url = root[KEY_Ip6Url].asString();
	}
	if (root.isMember(KEY_TimeInterval) && root[KEY_TimeInterval].isInt())
	{
		dnsConfig.timeInterval = root[KEY_TimeInterval].asInt();
		if (dnsConfig.timeInterval <= 0) {
			dnsConfig.timeInterval = default_value.timeInterval;
			log_info("配置的时间间隔<=0, 使用默认值:%d", dnsConfig.timeInterval);
			root[KEY_TimeInterval] = defultJson[KEY_TimeInterval];;
			need_update++;
		}
	}
	if (root.isMember(KEY_LogLevel) && root[KEY_LogLevel].isInt())
	{
		dnsConfig.LogLevel = root[KEY_LogLevel].asInt();
		//延迟生效
		gLogLevel = dnsConfig.LogLevel;
	}

	if (need_update) {
		fstream file;
		file.open(path, std::ios::trunc | std::ios::out);
		log_info("部分配置项不存在,刷新配置文件");
		Json::StyledStreamWriter jsWrite;
		jsWrite.write(file, root);
	}

}

int read_log_level_only() {
	int level = LOG_LEVEL_INFO;
	string path = getProgramFullPath() + ".json";
	fstream f(path);
	Json::Value root;
	Json::Reader reader;

	if (f.good()) {
		reader.parse(f, root, true);
		if (root.isMember(KEY_LogLevel) && root[KEY_LogLevel].isInt())
		{
			level = root[KEY_LogLevel].asInt();
		}
	}
	return level;
}