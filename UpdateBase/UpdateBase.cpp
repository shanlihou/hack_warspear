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
    0073F237 | . 6A 01          PUSH 1; | Arg1 = 00000001",

    "0073F205  |> 8B96 8C030000  MOV EDX,DWORD PTR DS:[ESI+38C]\
0073F20B | . 8B8A 78110000  MOV ECX,DWORD PTR DS : [EDX + 1178]\
0073F211 | . 8A41 08        MOV AL,BYTE PTR DS : [ECX + 8]\
0073F214 | . 8845 09        MOV BYTE PTR SS : [EBP + 9] ,AL\
0073F217 | . 8A41 0A        MOV AL,BYTE PTR DS : [ECX + A]\
0073F21A | . 8845 0A        MOV BYTE PTR SS : [EBP + A] ,AL\
0073F21D | . 8A41 0C        MOV AL,BYTE PTR DS : [ECX + C]\
0073F220 | . 8845 0B        MOV BYTE PTR SS : [EBP + B] ,AL\
0073F223 | . 8D45 09        LEA EAX,DWORD PTR SS : [EBP + 9]\
0073F226 | . 50             PUSH EAX; / Arg4\
0073F227 | . 8D47 08        LEA EAX,DWORD PTR DS : [EDI + 8] ; |\
0073F22A | . 50             PUSH EAX; | Arg3\
0073F22B | .FFB7 EC000000  PUSH DWORD PTR DS : [EDI + EC] ; | Arg2\
0073F231 | . 8D8A E80C0000  LEA ECX,DWORD PTR DS : [EDX + CE8] ; |\
0073F237 | . 6A 01          PUSH 1; | Arg1 = 00000001",

    "00782430  /. 55             PUSH EBP\
00782431 | . 8BEC           MOV EBP,ESP\
00782433 | . 8B49 18        MOV ECX,DWORD PTR DS : [ECX + 18]\
00782436 | . 85C9           TEST ECX,ECX\
00782438 | . 74 06          JE SHORT warspear.00782440\
0078243A | . 8B01           MOV EAX,DWORD PTR DS : [ECX]\
0078243C | . 5D             POP EBP\
0078243D | .FF60 18        JMP DWORD PTR DS : [EAX + 18]\
00782440 | > 5D             POP EBP\
00782441  \.C2 0800        RETN 8",

    "0073F0C0  /$ 55             PUSH EBP\
0073F0C1 | . 8BEC           MOV EBP,ESP\
0073F0C3 | . 83EC 08        SUB ESP,8\
0073F0C6 | . 56             PUSH ESI\
0073F0C7 | . 8BF1           MOV ESI,ECX\
0073F0C9 | . 8B8E 8C030000  MOV ECX,DWORD PTR DS : [ESI + 38C]\
0073F0CF | . 81C1 E80C0000  ADD ECX,0CE8\
0073F0D5 | . 8B91 90040000  MOV EDX,DWORD PTR DS : [ECX + 490]\
0073F0DB | . 0FB642 08      MOVZX EAX,BYTE PTR DS : [EDX + 8]\
0073F0DF | . 8845 FD        MOV BYTE PTR SS : [EBP - 3] ,AL\
0073F0E2 | . 0FB642 0A      MOVZX EAX,BYTE PTR DS : [EDX + A]\
0073F0E6 | . 8845 FE        MOV BYTE PTR SS : [EBP - 2] ,AL\
0073F0E9 | . 0FB642 0C      MOVZX EAX,BYTE PTR DS : [EDX + C]\
0073F0ED | . 8845 FF        MOV BYTE PTR SS : [EBP - 1] ,AL\
0073F0F0 | . 8D45 FD        LEA EAX,DWORD PTR SS : [EBP - 3]\
0073F0F3 | . 50             PUSH EAX; / Arg3\
0073F0F4 | . 8B81 2C010000  MOV EAX,DWORD PTR DS : [ECX + 12C] ; |\
0073F0FA | . 83C0 08        ADD EAX,8; |\
0073F0FD | . 50             PUSH EAX; | Arg2\
0073F0FE | . 8B45 08        MOV EAX,DWORD PTR SS : [EBP + 8] ; |\
0073F101 | .FFB0 84000000  PUSH DWORD PTR DS : [EAX + 84] ; | Arg1"
};

static DWORD g_readPos[] = { 0x0073F23F, 0x0073F1E0 , 0x00782430, 0x0073F0C0 };

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
        auto retPair = getTargetVal(hProcess, g_testCode[i], g_readPos[i]);
        printf("\nfinal:%x, %x", retPair.first, retPair.second);
    }
    CloseHandle(hProcess);
    return 0;
}
