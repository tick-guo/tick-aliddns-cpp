@echo on
pushd %~dp0

:: 环境vs2022
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"

:: 有效static
cmake  -S curl-8.2.1 -B curl-8.2.1-build -DBUILD_SHARED_LIBS=OFF  -DCMAKE_INSTALL_PREFIX=curl-8.2.1-install -DCURL_USE_SCHANNEL=ON 

::以下ssl模块不支持,需要额外库
rem -DCURL_USE_NSS=OFF  
rem -DCURL_USE_WOLFSSL=OFF 
rem -DCURL_USE_BEARSSL=OFF
rem -DCURL_USE_MBEDTLS=OFF
rem -DCURL_USE_OPENSSL=OFF
rem -DCURL_USE_SECTRANSP=OFF

devenv curl-8.2.1-build/CURL.sln  /rebuild "debug|x64" /project INSTALL
devenv curl-8.2.1-build/CURL.sln  /rebuild "release|x64" /project INSTALL


echo 编译完成
pause

