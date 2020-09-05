#pragma once
#include <Windows.h>
DWORD readU32(HANDLE hProcess, DWORD pos);
DWORD getTargetVal(HANDLE hProcess, const char* asmCode, DWORD oriTarget);
