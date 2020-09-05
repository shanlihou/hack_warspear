#include "utils.h"
#include "RawCode.h"
#include "ProcStream.h"
#include "kmp.h"


DWORD readU32(HANDLE hProcess, DWORD pos)
{
    unsigned char targetBuf[4];
    SIZE_T readn;
    ReadProcessMemory(hProcess, (LPCVOID)pos, targetBuf, 4, &readn);
    DWORD targetVal = 0;
    for (int i = 3; i >= 0; i--) {
        targetVal <<= 8;
        targetVal += targetBuf[i];
    }

    return targetVal;
}

DWORD getTargetVal(HANDLE hProcess, const char *asmCode, DWORD oriTarget) 
{
    auto rawInfo = getRawCode(asmCode);
    ProcStream ps(hProcess);
    std::vector<DWORD> rets = search(ps, rawInfo.second);
    if (rets.size() > 1 || rets.size() < 1) {
        printf("search failed:%d\n", rets.size());
        return 0;
    }

    DWORD targetPos = rets[0] + oriTarget - rawInfo.first;
    return readU32(hProcess, targetPos);
}