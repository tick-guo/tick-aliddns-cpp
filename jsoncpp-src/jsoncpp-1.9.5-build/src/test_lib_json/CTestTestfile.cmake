# CMake generated Testfile for 
# Source directory: E:/vs_project/aliddns_cpp_all/jsoncpp-src/jsoncpp-1.9.5/src/test_lib_json
# Build directory: E:/vs_project/aliddns_cpp_all/jsoncpp-src/jsoncpp-1.9.5-build/src/test_lib_json
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
if(CTEST_CONFIGURATION_TYPE MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test(jsoncpp_test "E:/vs_project/aliddns_cpp_all/jsoncpp-src/jsoncpp-1.9.5-build/bin/Debug/jsoncpp_test.exe")
  set_tests_properties(jsoncpp_test PROPERTIES  _BACKTRACE_TRIPLES "E:/vs_project/aliddns_cpp_all/jsoncpp-src/jsoncpp-1.9.5/src/test_lib_json/CMakeLists.txt;27;add_test;E:/vs_project/aliddns_cpp_all/jsoncpp-src/jsoncpp-1.9.5/src/test_lib_json/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test(jsoncpp_test "E:/vs_project/aliddns_cpp_all/jsoncpp-src/jsoncpp-1.9.5-build/bin/Release/jsoncpp_test.exe")
  set_tests_properties(jsoncpp_test PROPERTIES  _BACKTRACE_TRIPLES "E:/vs_project/aliddns_cpp_all/jsoncpp-src/jsoncpp-1.9.5/src/test_lib_json/CMakeLists.txt;27;add_test;E:/vs_project/aliddns_cpp_all/jsoncpp-src/jsoncpp-1.9.5/src/test_lib_json/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
  add_test(jsoncpp_test "E:/vs_project/aliddns_cpp_all/jsoncpp-src/jsoncpp-1.9.5-build/bin/MinSizeRel/jsoncpp_test.exe")
  set_tests_properties(jsoncpp_test PROPERTIES  _BACKTRACE_TRIPLES "E:/vs_project/aliddns_cpp_all/jsoncpp-src/jsoncpp-1.9.5/src/test_lib_json/CMakeLists.txt;27;add_test;E:/vs_project/aliddns_cpp_all/jsoncpp-src/jsoncpp-1.9.5/src/test_lib_json/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
  add_test(jsoncpp_test "E:/vs_project/aliddns_cpp_all/jsoncpp-src/jsoncpp-1.9.5-build/bin/RelWithDebInfo/jsoncpp_test.exe")
  set_tests_properties(jsoncpp_test PROPERTIES  _BACKTRACE_TRIPLES "E:/vs_project/aliddns_cpp_all/jsoncpp-src/jsoncpp-1.9.5/src/test_lib_json/CMakeLists.txt;27;add_test;E:/vs_project/aliddns_cpp_all/jsoncpp-src/jsoncpp-1.9.5/src/test_lib_json/CMakeLists.txt;0;")
else()
  add_test(jsoncpp_test NOT_AVAILABLE)
endif()
