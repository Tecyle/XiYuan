#pragma once
#include "resource.h"

// ��ʾ��Ϣ����
typedef struct tagToastMsgParams
{
	CString msg;
	ULONG duringTime;
	BOOL canBeClosed;
}	ToastMsgParams;

// CToastDlg �Ի���

class CToastDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CToastDlg)
protected:
	BLENDFUNCTION m_Blend;
	HDC m_hdcMemory;
	Image * pXiyuanChar1;
	Image * pXiyuanChar2;
	int m_blinkState;
	queue<ToastMsgParams> msgQueue;

public:
	CToastDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CToastDlg();
	/**
	* ����Դ�м��������ʽ��ͼƬ
	*/
	BOOL ImageFromIDResource(UINT resourceID, LPCTSTR imgType, Image * &pImg);
	/**
	* ���²���������ʾ
	*/
	BOOL UpdateDisplay(Image * image, int Transparent = 100);

	void AddToastMsg(LPCTSTR msg, ULONG duringTime, BOOL canClose);

// �Ի�������
	enum { IDD = IDD_TOAST_BOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void DrawPromptMsg(Graphics * g, LPCTSTR msg);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	void ShowToastMessage();
};
