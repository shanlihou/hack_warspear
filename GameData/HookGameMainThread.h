#pragma once
#include <Windows.h>

DWORD HookMainThread();
DWORD UnHookMainThread();
DWORD MsgAttack(char* szpName);
DWORD MsgSearch(char* szpName);
DWORD MsgPick(char* szpName);