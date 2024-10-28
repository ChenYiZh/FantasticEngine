//
// Created by ChenYiZh on 2024/10/27.
//
#define  DLLTEST_EXPORTS
#include "test.h"
#include <iostream>

namespace fantasy_engine {
    void test::hello() {
        std::cout << "Hello World"<<std::endl;
    }
} // fantasy_engine