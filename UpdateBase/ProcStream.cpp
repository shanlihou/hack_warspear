#include "ProcStream.h"


ProcStream::ProcStream(HANDLE hProcess) : _process(hProcess), _curAddr(START_ADDR), _curIndex(0), isEnd(false)
{
    SIZE_T readn = 0;
    ReadProcessMemory(_process, (LPCVOID)_curAddr, _buffer, PAGE_SIZE, &readn);

}
int ProcStream::operator++() 
{
    SIZE_T readn = 0;
    _curIndex++;
    if (_curIndex == PAGE_SIZE) {
        if (_curAddr >= END_ADDR - PAGE_SIZE) {
            _curIndex--;
            isEnd = true;
            return -1;
        }

        _curAddr += PAGE_SIZE;

        ReadProcessMemory(_process, (LPCVOID)_curAddr, _buffer, PAGE_SIZE, &readn);
        _curIndex = 0;
    }

    return 0;
}


char ProcStream::readChr() 
{
    return _buffer[_curIndex];
};


DWORD ProcStream::getIndex() 
{
    return _curAddr + _curIndex;
}