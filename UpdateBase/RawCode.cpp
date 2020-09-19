#include <regex>
#include <iostream>
#include "RawCode.h"
using std::regex;

char getHexFromChr(char chr)
{
    if (chr >= '0' && chr <= '9') {
        return chr - '0';
    }
    else {
        return chr - 'A' + 10;
    }
}

char getHexFromStr(const char* hex) {
    return getHexFromChr(hex[0]) * 16 + getHexFromChr(hex[1]);
}

std::pair<DWORD, std::vector<char>> getRawCode(const char *code) {

    regex reg("(\\w+) *[\\|\\\\/] *[\\.\\>$] *((?:[\\dABCDEF]+ )+)");
    //regex reg("(\\w+) *\\| *\\. *([\\dABCDEF]+ )+");
    std::string content(code);
    std::smatch m;
    std::vector<char> retRawCode;
    DWORD startPos;
    char oneHex[2];
    char idx = 0;
    auto pos = content.cbegin();
    auto end = content.cend();
    for (; std::regex_search(pos, end, m, reg); pos = m.suffix().first)
    {
        //std::cout << m.str(2) << std::endl;
        if (pos == content.cbegin()) {
            sscanf_s(m.str(1).c_str(), "%x", &startPos);
        }
        for (auto& chr : m.str(2)) {
            if (chr == ' ') {
                continue;
            }

            oneHex[idx] = chr;
            if (idx == 1) {
                retRawCode.push_back(getHexFromStr(oneHex));
            }
            idx ^= 1;
        }
    }

    return std::make_pair(startPos, std::move(retRawCode));
}