#pragma once
#include "resource.h"

// 显示消息队列
typedef struct tagToastMsgParams
{
	CString msg;
	ULONG duringTime;
	BOOL canBeClosed;
}	ToastMsgParams;

// CToastDlg 对话框

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
	CToastDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CToastDlg();
	/**
	* 从资源中加载任意格式的图片
	*/
	BOOL ImageFromIDResource(UINT resourceID, LPCTSTR imgType, Image * &pImg);
	/**
	* 更新不规则窗体显示
	*/
	BOOL UpdateDisplay(Image * image, int Transparent = 100);

	void AddToastMsg(LPCTSTR msg, ULONG duringTime, BOOL canClose);

// 对话框数据
	enum { IDD = IDD_TOAST_BOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
