#pragma once
#include <utility>
#include <Windows.h>
#include <vector>


std::pair<DWORD, std::vector<char>> getRawCode(const char* code);