// UpdateBase.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <Windows.h>
#include "RawCode.h"
#include "kmp.h"
#include "utils.h"

#define GAME_CLASS_NAME "Warspear"
#define PAGE_SIZE 1024
#define DLL_PATH "\\Debug\\hackWarspear.dll"

static const char*g_testCode[] = { "0073F211  |. 8A41 08        MOV AL,BYTE PTR DS:[ECX+8]\
    0073F214 | . 8845 09        MOV BYTE PTR SS : [EBP + 9] , AL\
    0073F217 | . 8A41 0A        MOV AL, BYTE PTR DS : [ECX + A]\
    0073F21A | . 8845 0A        MOV BYTE PTR SS : [EBP + A] , AL\
    0073F21D | . 8A41 0C        MOV AL, BYTE PTR DS : [ECX + C]\
    0073F220 | . 8845 0B        MOV BYTE PTR SS : [EBP + B] , AL\
    0073F223 | . 8D45 09        LEA EAX, DWORD PTR SS : [EBP + 9]\
    0073F226 | . 50             PUSH EAX; / Arg4\
    0073F227 | . 8D47 08        LEA EAX, DWORD PTR DS : [EDI + 8] ; |\
    0073F22A | . 50             PUSH EAX; | Arg3\
    0073F22B | .FFB7 EC000000  PUSH DWORD PTR DS : [EDI + EC] ; | Arg2\
    0073F231 | . 8D8A E80C0000  LEA ECX, DWORD PTR DS : [EDX + CE8] ; |\
    0073F237 | . 6A 01          PUSH 1; | Arg1 = 00000001" };

static DWORD g_readPos[] = { 0x0073F23F };

int main()
{
    DWORD pid = 0;
    HWND warHandle = NULL;
    HANDLE hProcess = NULL;

    warHandle = FindWindowA(GAME_CLASS_NAME, NULL);
    if (warHandle == 0)
        return -1;

    printf("find handle\n");
    GetWindowThreadProcessId(warHandle, &pid);
    if (pid == 0)
    {
        printf("get pid error\n");
        return -1;
    }

    hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (hProcess == NULL)
    {
        printf("get hProcess error\n");
        return -1;
    }

    int length = (sizeof(g_testCode) / sizeof(char*));
    for (int i = 0; i < length; i ++) 
    {
        DWORD targetVal = getTargetVal(hProcess, g_testCode[i], g_readPos[i]);
        printf("final:%x", targetVal);
    }
    CloseHandle(hProcess);
    return 0;
}
