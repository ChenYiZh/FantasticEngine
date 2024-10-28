cmake_minimum_required(VERSION 3.24)
project(FantasyEngine)

set(CMAKE_CXX_STANDARD 11)

#set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(PROJECT_OUTPUT_DIR ${CMAKE_HOME_DIRECTORY}/bin)
#输出目录
Set(EXECUTABLE_OUTPUT_PATH ${PROJECT_OUTPUT_DIR})
#输出位置
set(LIBRARY_OUTPUT_PATH ${PROJECT_OUTPUT_DIR})

#改前缀
set(CMAKE_SHARED_LIBRARY_PREFIX "")

#头文件include只是为了没有红波浪
#include_directories("C:/Program Files (x86)/Microsoft Visual Studio/2017/Community/VC/Tools/MSVC/14.16.27023/include")
#设置头文件目录
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_subdirectory(FantasyEngine/Plugins/FantasyEngine_Common/Source/FantasyEngine_common)
#add_subdirectory(FantasyEngine_test)