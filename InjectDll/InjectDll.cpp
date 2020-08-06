// InjectDll.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <Windows.h>

#define GAME_CLASS_NAME "Warspear"
#define DLL_PATH "E:\\shgithub\\cpp\\hackWarspear\\hackWarspear\\Debug\\hackWarspear.dll"

void injectDll()
{
    DWORD pid = 0;
    DWORD byWriteSize = 0;
    HANDLE hProcess = NULL;
    LPDWORD addressDW = NULL;
    HANDLE threadHandle = NULL;

    HWND warHandle = FindWindowA(GAME_CLASS_NAME, NULL);
    if (warHandle == 0)
        return;

    printf("find handle\n");
    GetWindowThreadProcessId(warHandle, &pid);
    if (pid == 0)
        return;

    hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (hProcess == NULL)
        return;

    addressDW = (LPDWORD)VirtualAllocEx(hProcess, NULL, 256, MEM_COMMIT, PAGE_READWRITE);
    if (addressDW == NULL)
        return;

    WriteProcessMemory(hProcess, addressDW, DLL_PATH, strlen(DLL_PATH) + 1, &byWriteSize);
    if (byWriteSize < strlen(DLL_PATH))
    {
        printf("write memmory failed!");
        return;
    }

    threadHandle = CreateRemoteThread(hProcess, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibraryA, addressDW, NULL, NULL);
    WaitForSingleObject(threadHandle, 0xFFFFFFFF);
    CloseHandle(threadHandle);
    VirtualFreeEx(hProcess, addressDW, 256, MEM_COMMIT);
    CloseHandle(hProcess);
}

int main()
{
    std::cout << "Hello World!\n";
    injectDll();
    //getchar();
    return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
