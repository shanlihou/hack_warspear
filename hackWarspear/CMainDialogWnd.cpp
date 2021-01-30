// CMainDialogWnd.cpp: 实现文件
//

#include "pch.h"
#include "hackWarspear.h"
#include "CMainDialogWnd.h"
#include "afxdialogex.h"
#include "StructGame.h"
#include "HookGameMainThread.h"

// CMainDialogWnd 对话框

IMPLEMENT_DYNAMIC(CMainDialogWnd, CDialogEx)

CMainDialogWnd::CMainDialogWnd(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{
	HookMainThread();
}

CMainDialogWnd::~CMainDialogWnd()
{
	UnHookMainThread();
}

void CMainDialogWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMainDialogWnd, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_TEST, &CMainDialogWnd::OnBnClickedButtonTest)
    ON_BN_CLICKED(IDC_BUTTON_HOOK, &CMainDialogWnd::OnBnClickedButtonHook)
	ON_BN_CLICKED(IDC_BUTTON_UNHOOK, &CMainDialogWnd::OnBnClickedButtonUnhook)
	ON_BN_CLICKED(IDC_BUTTON_ATTACK, &CMainDialogWnd::OnBnClickedButtonAttack)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH_DEAD, &CMainDialogWnd::OnBnClickedButtonSearchDead)
	ON_BN_CLICKED(IDC_PICK, &CMainDialogWnd::OnBnClickedPick)
END_MESSAGE_MAP()


// CMainDialogWnd 消息处理程序


void CMainDialogWnd::OnBnClickedButtonTest()
{
	// TODO: 在此添加控件通知处理程序代码
	MsgTest("GameDebug");
}


void CMainDialogWnd::OnBnClickedButtonHook()
{
    // TODO: 在此添加控件通知处理程序代码
}


void CMainDialogWnd::OnBnClickedButtonUnhook()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CMainDialogWnd::OnBnClickedButtonAttack()
{
	// TODO: 在此添加控件通知处理程序代码
	MsgAttack("hello");
}


void CMainDialogWnd::OnBnClickedButtonSearchDead()
{
	// TODO: 在此添加控件通知处理程序代码
	MsgSearch("hello");

}


void CMainDialogWnd::OnBnClickedPick()
{
	// TODO: 在此添加控件通知处理程序代码
	MsgPick("hello");
}
