#pragma once
#include "SingleDownListCtrl.h"


// CDownMgrDlg �Ի���

class CDownMgrDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDownMgrDlg)
	int blinkCnt;
public:
	CDownMgrDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDownMgrDlg();

	CProgressCtrl * pDownProgress;
	CSingleDownListCtrl * pList;

// �Ի�������
	enum { IDD = IDD_DOWN_MANAGER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClose();
	void UpdateDownList();
//	afx_msg void OnPaint();
};
