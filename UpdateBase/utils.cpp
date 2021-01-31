#include <iostream>
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

bool verifySuffix(HANDLE hProcess, DWORD pos, std::vector<char>& other) {
    char* buf = new char[other.size()];
    SIZE_T readn;
    ReadProcessMemory(hProcess, (LPCVOID)pos, buf, other.size(), &readn);
    for (int i = 0; i < other.size(); i++) {
        if (buf[i] != other[i]) {
            delete[]buf;
            return false;
        }
    }
    delete[]buf;
    return true;
}

std::pair<DWORD, DWORD> getTargetVal(HANDLE hProcess, const char *asmCode, DWORD oriTarget, const char* suffix)
{
    auto rawInfo = getRawCode(asmCode);
    auto _suffix = getRawCode(suffix);
    ProcStream ps(hProcess);
    std::vector<DWORD> rets = search(ps, rawInfo.second);
    if (rets.size() > 1) {
        printf("search beyond one:%d\n", rets.size()); 
        for (auto& i : rets) {
            DWORD _target = i + oriTarget;
            std::cout << std::hex << _target << "," << readU32(hProcess, _target) << std::endl;
            if (verifySuffix(hProcess, _target + 4, _suffix.second)) {
                return std::make_pair(_target, readU32(hProcess, _target));
            }
        }
        return std::make_pair(0, 0);
    }
    else if (rets.size() == 0) {
        printf("search failed:%d\n", rets.size());
        return std::make_pair(0, 0);
    }

    DWORD targetPos = rets[0] + oriTarget;
    return std::make_pair(targetPos, readU32(hProcess, targetPos));
}