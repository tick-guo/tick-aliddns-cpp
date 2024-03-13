@echo on
pushd %~dp0

:: 环境vs2022
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"

:: 有效static
cmake  -S jsoncpp-1.9.5 -B jsoncpp-1.9.5-build  -DCMAKE_INSTALL_PREFIX=jsoncpp-1.9.5-install  -DBUILD_SHARED_LIBS=OFF -DBUILD_STATIC_LIBS=ON
 

devenv jsoncpp-1.9.5-build/jsoncpp.sln  /rebuild "debug|x64" /project INSTALL
devenv jsoncpp-1.9.5-build/jsoncpp.sln  /rebuild "release|x64" /project INSTALL


echo 编译完成
pause

