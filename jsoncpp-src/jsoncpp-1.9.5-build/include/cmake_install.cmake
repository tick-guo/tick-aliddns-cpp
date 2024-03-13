# Install script for directory: E:/vs_project/aliddns_cpp_all/jsoncpp-src/jsoncpp-1.9.5/include

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "E:/vs_project/aliddns_cpp_all/jsoncpp-src/jsoncpp-1.9.5-install")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/json" TYPE FILE FILES
    "E:/vs_project/aliddns_cpp_all/jsoncpp-src/jsoncpp-1.9.5/include/json/allocator.h"
    "E:/vs_project/aliddns_cpp_all/jsoncpp-src/jsoncpp-1.9.5/include/json/assertions.h"
    "E:/vs_project/aliddns_cpp_all/jsoncpp-src/jsoncpp-1.9.5/include/json/config.h"
    "E:/vs_project/aliddns_cpp_all/jsoncpp-src/jsoncpp-1.9.5/include/json/forwards.h"
    "E:/vs_project/aliddns_cpp_all/jsoncpp-src/jsoncpp-1.9.5/include/json/json.h"
    "E:/vs_project/aliddns_cpp_all/jsoncpp-src/jsoncpp-1.9.5/include/json/json_features.h"
    "E:/vs_project/aliddns_cpp_all/jsoncpp-src/jsoncpp-1.9.5/include/json/reader.h"
    "E:/vs_project/aliddns_cpp_all/jsoncpp-src/jsoncpp-1.9.5/include/json/value.h"
    "E:/vs_project/aliddns_cpp_all/jsoncpp-src/jsoncpp-1.9.5/include/json/version.h"
    "E:/vs_project/aliddns_cpp_all/jsoncpp-src/jsoncpp-1.9.5/include/json/writer.h"
    )
endif()

