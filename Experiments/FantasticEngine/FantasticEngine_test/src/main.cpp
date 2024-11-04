//
// Created by ChenYiZh on 2024/10/28.
//
#include <iostream>
#include <string>
#include "test.h"

int main() {
    int ret = fantasy_engine::Test::Add(5, 10);
    std::wcout << ret << std::endl;
    std::wcin.get();
    return 0;
}