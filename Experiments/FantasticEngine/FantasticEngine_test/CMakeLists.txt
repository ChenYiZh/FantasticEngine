cmake_minimum_required(VERSION 3.24)
#项目名称
project(FantasticEngine_test)
#设置C++ Standard的版本
#set(CMAKE_CXX_STANDARD 11)

#输出位置
set(LIBRARY_OUTPUT_PATH ${PROJECT_OUTPUT_DIR})

#将所有cpp文件都放进来
add_executable(${PROJECT_NAME} src/main.cpp)
#设置头文件目录
target_include_directories(${PROJECT_NAME} PUBLIC FantasticEngine_common)
#动态库依赖
target_link_libraries(${PROJECT_NAME} FantasticEngine_common)