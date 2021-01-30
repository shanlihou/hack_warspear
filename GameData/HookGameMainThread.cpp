#include "HookGameMainThread.h"
#include "StructGame.h"
#include "debug.h"

HHOOK g_hhkGame;
const DWORD g_myMsgCode = RegisterWindowMessageA("myMsgCode");

enum MsgAction
{
    ATTACK = 0,
    DEAD_SEARCH,
    PICK,
    TEST
};

void doAction(MsgAction act)
{
    EntityMgr em;
    em.getData();
    switch (act)
    {
    case MsgAction::ATTACK:
        em.attack();
        break;
    case MsgAction::DEAD_SEARCH:
        em.searchDead();
        break;
    case MsgAction::PICK:
        em.pick();
        break;
    case MsgAction::TEST:
        break;
    }
}

LRESULT CALLBACK GameWndProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    CWPSTRUCT* lpArg = (CWPSTRUCT*)lParam;
    if (nCode == HC_ACTION)
    {
        if (lpArg->hwnd == getGameWndHandle() && lpArg->message == g_myMsgCode)
        {
            doAction((MsgAction)lpArg->wParam);
            return 1;
        }
    }

    return CallNextHookEx(g_hhkGame, nCode, wParam, lParam);
}

DWORD HookMainThread()
{
    log_debug("hook ok\n");
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
    log_debug("unhook ok\n");
    UnhookWindowsHookEx(g_hhkGame);
    return 1;
}

DWORD MsgAttack(char* szpName)
{
    SendMessageA(getGameWndHandle(), g_myMsgCode, MsgAction::ATTACK, (LPARAM)szpName);
    return 1;
}

DWORD MsgSearch(char* szpName)
{
    SendMessageA(getGameWndHandle(), g_myMsgCode, MsgAction::DEAD_SEARCH, (LPARAM)szpName);
    return 1;
}

DWORD MsgPick(char* szpName)
{
    SendMessageA(getGameWndHandle(), g_myMsgCode, MsgAction::PICK, (LPARAM)szpName);
    return 1;
}

DWORD MsgTest(char* szpName)
{
    SendMessageA(getGameWndHandle(), g_myMsgCode, MsgAction::TEST, (LPARAM)szpName);
    return 1;
}