// CMainDialogWnd.cpp: 实现文件
//

#include "pch.h"
#include "hackWarspear.h"
#include "CMainDialogWnd.h"
#include "afxdialogex.h"
#include "StructGame.h"

// CMainDialogWnd 对话框

IMPLEMENT_DYNAMIC(CMainDialogWnd, CDialogEx)

CMainDialogWnd::CMainDialogWnd(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

CMainDialogWnd::~CMainDialogWnd()
{
}

void CMainDialogWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMainDialogWnd, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_TEST, &CMainDialogWnd::OnBnClickedButtonTest)
END_MESSAGE_MAP()


// CMainDialogWnd 消息处理程序


void CMainDialogWnd::OnBnClickedButtonTest()
{
	// TODO: 在此添加控件通知处理程序代码
	EntityMgr em;
	em.getData();
	em.test();
	TRACE("GameDebug");
}
