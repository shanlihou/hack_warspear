#pragma once

#include <Windows.h>

#define START_ADDR 0x00400000
static DWORD END_ADDR = START_ADDR + 0x0063B000;
#define PAGE_SIZE 4096

class ProcStream
{
public:
    ProcStream(HANDLE hProcess);
    int operator++();
    char readChr();
    DWORD getIndex();

public:
    bool isEnd;
private:
    char _buffer[PAGE_SIZE + 1];
    HANDLE _process;
    DWORD _curAddr;
    int _curIndex;
};