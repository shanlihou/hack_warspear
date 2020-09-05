#include "kmp.h";
using std::vector;

vector<int> getNext(const vector<char> &input)
{
    int len = input.size();               // 字符串长度
    vector<int> next(len, 0);            // 保存结果，next[0]=0
    for (int i = 1; i < len; i++)
    {
        int k = next[i - 1];             // k 表示需要比较的位置，初始值为 next[i - 1]
        while (k > 0 && input[i] != input[k]) // 比较，若不相等则继续分割，直到相等或为0(即不含相同部分)
            k = next[k - 1];
        if (input[i] == input[k])             // 若相等，则 next[i] = k + 1，否则为0，其中 k 为索引
            k++;
        next[i] = k;                     // 更新 next[i]
    }
    return next;
}

vector<DWORD> search(ProcStream &ps, const vector<char> &hex)
{
    vector<int> next = getNext(hex);           // 获得 str2 的 next 数组
    vector<DWORD> retVec;
    int k = 0;                                  // 记录当前已匹配 str2 的索引
    while (!ps.isEnd) {
        while (k > 0 && ps.readChr() != hex[k])      // str1的第i个与str2的第k个字符进行比较，若不同，则k=next[k-1]，直到k为0或相等为止
            k = next[k - 1];

        if (ps.readChr() == hex[k])                  // 若相等，更新k
            k++;

        if (k == hex.size())              // 若找到完全匹配
        {
            retVec.push_back(ps.getIndex() - k + 1);
            k = next[k - 1];                    // 进行下一轮匹配，此处根据需要可去掉
        }
        ++ps;
    }
    return retVec;
}