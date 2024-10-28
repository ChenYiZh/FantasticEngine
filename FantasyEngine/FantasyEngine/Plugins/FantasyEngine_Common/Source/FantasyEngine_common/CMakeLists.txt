cmake_minimum_required(VERSION 3.24)
#项目名称
project(FantasyEngine_common)
#设置C++ Standard的版本
set(CMAKE_CXX_STANDARD 11)

#输出位置
set(LIBRARY_OUTPUT_PATH ${PROJECT_OUTPUT_DIR})

#查找所有cpp文件
file(GLOB_RECURSE PROJECT_SOURCES src/*.cpp)
#将所有cpp文件都放进来
add_library(${PROJECT_NAME} SHARED ${PROJECT_SOURCES})
#设置头文件目录
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)