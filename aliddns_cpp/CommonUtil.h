#pragma once

#include <iostream>
#include <string>

using namespace std;

std::string GetLastErrorMsg(unsigned long errCode);
string getProgramFullPath();
bool chenkFileExist(const string& path);
void print_curl_version();
bool check_ipv6_support();
bool check_https_support();
long long get_boot_millisecond();
string string_remove_blank(string val);


