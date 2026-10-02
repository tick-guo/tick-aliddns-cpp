#pragma once

#include <iostream>
#include <string>
#include <Windows.h>

#define SVCNAME "AliddnsAutoUpdate"

using namespace std;



class ServiceHelper {
public:
	ServiceHelper();
	~ServiceHelper();

public:
	/*
	* 把本程序安装为服务
	*/
	int InstallService();
	int DeleteService();
	int startService();
	int stopService();
	int ServiceRunAndWaitStop();

private:

};

