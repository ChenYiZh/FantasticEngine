cmake_minimum_required(VERSION 3.24)
project(FantasyEngine)

set(CMAKE_CXX_STANDARD 14)

#set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

#设置变量
if (CMAKE_SYSTEM_NAME MATCHES "Windows")
    set(PROJECT_OUTPUT_DIR ${CMAKE_HOME_DIRECTORY}/FantasyEngine.Unity/Assets/Plugins/FantasyEngine/Standalone)
elseif (CMAKE_SYSTEM_NAME MATCHES "Android")
    set(PROJECT_OUTPUT_DIR ${CMAKE_HOME_DIRECTORY}/FantasyEngine.Unity/Assets/Plugins/FantasyEngine/Android)
endif ()

set(UNREALENGINE_PLUGINS_DIR ${CMAKE_HOME_DIRECTORY}/FantasyEngine/Plugins)
#输出目录
Set(EXECUTABLE_OUTPUT_PATH ${PROJECT_OUTPUT_DIR})
#输出位置
set(LIBRARY_OUTPUT_PATH ${PROJECT_OUTPUT_DIR})

#add_compile_options(/source-charset:utf-8 /execution-charset:utd-8)

#改前缀
set(CMAKE_SHARED_LIBRARY_PREFIX "")

#头文件include只是为了没有红波浪
if (CMAKE_SYSTEM_NAME MATCHES "Windows")
    include_directories("C:/Program Files (x86)/Microsoft Visual Studio/2017/Community/VC/Tools/MSVC/14.16.27023/include")
endif ()
#设置头文件目录
if (CMAKE_SYSTEM_NAME MATCHES "Windows")
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/Windows)
elseif (CMAKE_SYSTEM_NAME MATCHES "Android")
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/Android)
endif ()
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_subdirectory(FantasyEngine_common)
add_subdirectory(FantasyEngine_test)
