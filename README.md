### 功能
1. 动态更新DDNS , cpp版本
2. 第一次启动后,自动产生配置文件模板

### 必须安装为服务,后台运行
```
D:\aliddns>aliddns_cpp.exe
aliddns v2.8 build 20240313 by tick_guo
ipv6_support:yes https_support:yes服务进程无法连接到服务控制器上。

程序没有在服务模式下运行

Help:
aliddns_cpp.exe install          install the program as a service AliddnsAutoUpdate
aliddns_cpp.exe delete           delete the service: AliddnsAutoUpdate
aliddns_cpp.exe start            start the service: AliddnsAutoUpdate
```
### 编译软件
1. vs2022

编译结果
``` 
 Directory of D:\a\tick-aliddns-cpp\tick-aliddns-cpp\x64\Release

10/02/2026  04:18 AM    <DIR>          .
10/02/2026  04:17 AM    <DIR>          ..
10/02/2026  04:18 AM        53,293,090 AliddnsLib.lib
10/02/2026  04:18 AM         2,494,464 AliddnsLib.pdb
10/02/2026  04:18 AM           840,192 aliddns_cpp.exe
10/02/2026  04:18 AM         4,886,528 aliddns_cpp.pdb
               4 File(s)     61,514,274 bytes
```
