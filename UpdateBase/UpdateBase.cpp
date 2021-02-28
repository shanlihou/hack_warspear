// UpdateBase.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <Windows.h>
#include "RawCode.h"
#include "kmp.h"
#include "utils.h"
#include <Python.h>

#define GAME_CLASS_NAME "Warspear"
#define PAGE_SIZE 1024
#define DLL_PATH "\\Debug\\hackWarspear.dll"

static const char*g_testCode[] = { "00570FBD   . 8B07                     MOV EAX,DWORD PTR DS:[EDI]\
00570FBF   . 53                       PUSH EBX\
00570FC0   . 56                       PUSH ESI\
00570FC1.C747 10 02000000         MOV DWORD PTR DS : [EDI + 10] ,2\
00570FC8.FF50 14                  CALL DWORD PTR DS : [EAX + 14]",

"0073F211  |. 8A41 08        MOV AL,BYTE PTR DS:[ECX+8]\
0073F214 | . 8845 09        MOV BYTE PTR SS : [EBP + 9] ,AL\
0073F217 | . 8A41 0A        MOV AL,BYTE PTR DS : [ECX + A]\
0073F21A | . 8845 0A        MOV BYTE PTR SS : [EBP + A] ,AL\
0073F21D | . 8A41 0C        MOV AL,BYTE PTR DS : [ECX + C]\
0073F220 | . 8845 0B        MOV BYTE PTR SS : [EBP + B] ,AL"
};

static const char* g_suffixes[] = { "00570FD0   . FF77 14                  PUSH DWORD PTR DS:[EDI+14]               ; /Arg3\
00570FD3   . 8B70 10                  MOV ESI,DWORD PTR DS : [EAX + 10] ; |\
00570FD6.FF77 18                  PUSH DWORD PTR DS : [EDI + 18] ; | Arg2\
00570FD9   . 8B58 28                  MOV EBX,DWORD PTR DS : [EAX + 28] ; |\
00570FDC   . 57                       PUSH EDI; | Arg1\
00570FDD   . 8D8E E8020000            LEA ECX,DWORD PTR DS : [ESI + 2E8] ; |",
""
};

static DWORD g_readPos[] = {0x570fcc - 0x570fbd, 0};

void testHello()
{
    Py_Initialize();//调用Py_Initialize()进行初始化
    PyObject *pName = PyUnicode_FromString("test");
    PyObject *pModule = PyImport_ImportModule("ckz_test"); //载入plot3dfun的模块
    PyObject* pFunc = NULL;
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("print(sys.path)");
    PyRun_SimpleString("import ckz_test");
    PyRun_SimpleString("ckz_test.hello()");
    pFunc = PyObject_GetAttrString(pModule, "hello");//调用的函数名
    if (pFunc) {
        PyObject* tup = PyTuple_New(0);
        PyObject_Call(pFunc, tup, NULL);//调用函数,NULL表示参数为空
    }
    printf("im fine %p %p\n", pModule, pFunc);
    Py_Finalize();//调用Py_Finalize,和Py_Initialize相对应的.
}

int main()
{
    DWORD pid = 0;
    HWND warHandle = NULL;
    HANDLE hProcess = NULL;

    warHandle = FindWindowA(GAME_CLASS_NAME, NULL);
    if (warHandle == 0)
        return -1;
    testHello();
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
        auto retPair = getTargetVal(hProcess, g_testCode[i], g_readPos[i], g_suffixes[i]);
        printf("\nfinal:%x, %x\n", retPair.first, retPair.second);
    }
    CloseHandle(hProcess);
    return 0;
}
