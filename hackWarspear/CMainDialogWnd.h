#pragma once
#include <afxdialogex.h>

// CMainDialogWnd 对话框

class CMainDialogWnd : public CDialogEx
{
	DECLARE_DYNAMIC(CMainDialogWnd)

public:
	CMainDialogWnd(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CMainDialogWnd();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedButtonTest();
    afx_msg void OnBnClickedButtonHook();
    afx_msg void OnBnClickedButtonUnhook();
    afx_msg void OnBnClickedButtonAttack();
};
