#pragma once
#include <Windows.h>

DWORD HookMainThread();
DWORD UnHookMainThread();
DWORD MsgAttack(char* szpName);