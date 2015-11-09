#pragma once
#include "SingleDownListCtrl.h"


// CDownMgrDlg 对话框

class CDownMgrDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDownMgrDlg)
	int blinkCnt;
public:
	CDownMgrDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDownMgrDlg();

	CProgressCtrl * pDownProgress;
	CSingleDownListCtrl * pList;

// 对话框数据
	enum { IDD = IDD_DOWN_MANAGER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClose();
	void UpdateDownList();
//	afx_msg void OnPaint();
};
