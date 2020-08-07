#include "HookGameMainThread.h"
#include "StructGame.h"
#include "debug.h"

HHOOK g_hhkGame;
const DWORD g_myMsgCode = RegisterWindowMessageA("myMsgCode");

enum MsgAction
{
    ATTACK = 0
};

void test()
{
    EntityMgr em;
    em.getData();
    em.test();
}

LRESULT CALLBACK GameWndProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    CWPSTRUCT* lpArg = (CWPSTRUCT*)lParam;
    if (nCode == HC_ACTION)
    {
        if (lpArg->hwnd == getGameWndHandle() && lpArg->message == g_myMsgCode)
        {
            switch (lpArg->wParam)
            {
            case MsgAction::ATTACK:
                test();
                break;
            default:
                break;
            }
            return 1;
        }
    }

    return CallNextHookEx(g_hhkGame, nCode, wParam, lParam);
}

DWORD HookMainThread()
{
    HWND hGame = getGameWndHandle();
    DWORD ndThreadId = GetWindowThreadProcessId(hGame, NULL);
    if (ndThreadId == NULL)
    {
        return 1;
    }

    g_hhkGame = SetWindowsHookExA(WH_CALLWNDPROC, GameWndProc, NULL, ndThreadId);
    return 1;
}

DWORD UnHookMainThread()
{
    UnhookWindowsHookEx(g_hhkGame);
    return 1;
}

DWORD MsgAttack(char* szpName)
{
    SendMessageA(getGameWndHandle(), g_myMsgCode, MsgAction::ATTACK, (LPARAM)szpName);
    return 1;
}