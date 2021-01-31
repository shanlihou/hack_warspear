#pragma once
#include <Windows.h>
#include <utility>
DWORD readU32(HANDLE hProcess, DWORD pos);
std::pair<DWORD, DWORD> getTargetVal(HANDLE hProcess, const char* asmCode, DWORD oriTarget, const char* suffix);
