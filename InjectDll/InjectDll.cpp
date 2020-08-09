// InjectDll.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <Windows.h>
#include <direct.h>
#include <io.h>

#define GAME_CLASS_NAME "Warspear"
#define DLL_PATH "\\Debug\\hackWarspear.dll"
//#define DLL_PATH "F:\\shgithub\\cpp\\hack_warspear\\Debug\\hackWarspear.dll"

void injectDll()
{
    DWORD pid = 0;
    DWORD byWriteSize = 0;
    HANDLE hProcess = NULL;
    LPDWORD addressDW = NULL;
    HANDLE threadHandle = NULL;
    HWND warHandle = NULL;

    char dllPath[1024];
    char* buffer = _getcwd(NULL, 0);
    if (buffer == NULL)
    {
        printf("could get buffer\n");
        return;
    }
    char* pos = strrchr(buffer, '\\');
    strncpy_s(dllPath, buffer, pos - buffer);
    dllPath[pos - buffer] = 0;
    strcat_s(dllPath, DLL_PATH);
    printf("dll:%s\n", dllPath);
    free(buffer);

    warHandle = FindWindowA(GAME_CLASS_NAME, NULL);
    if (warHandle == 0)
        return;

    printf("find handle\n");
    GetWindowThreadProcessId(warHandle, &pid);
    if (pid == 0)
    {
        printf("get pid error\n");
        return;
    }

    hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (hProcess == NULL)
    {
        printf("get hProcess error\n");
        return;
    }

    addressDW = (LPDWORD)VirtualAllocEx(hProcess, NULL, 256, MEM_COMMIT, PAGE_READWRITE);
    if (addressDW == NULL)
    {
        printf("get hProcess error\n");
        return;
    }

    WriteProcessMemory(hProcess, addressDW, dllPath, strlen(dllPath) + 1, &byWriteSize);
    if (byWriteSize < strlen(dllPath))
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
