// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。  
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。  
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问  
// http://go.microsoft.com/fwlink/?LinkId=238214。
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// XiYuanView.h : CXiYuanView 类的接口
//

#pragma once
#include "VodCtrl.h"
#include "XiYuanDoc.h"

class CXiYuanView : public CHtmlView
{
protected: // 仅从序列化创建
	CXiYuanView();
	DECLARE_DYNCREATE(CXiYuanView)

// 特性
public:
	CXiYuanDoc* GetDocument() const;
	CVodCtrl mVodCtrl;

// 操作
public:

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual HRESULT OnGetExternal(LPDISPATCH *lppDispatch);
protected:
	virtual void OnInitialUpdate(); // 构造后第一次调用

// 实现
public:
	virtual ~CXiYuanView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnNavigateComplete2(LPCTSTR strURL);
	virtual void OnDocumentComplete(LPCTSTR lpszURL);
	virtual void OnNewWindow2(LPDISPATCH* ppDisp, BOOL* Cancel);
	virtual void OnBeforeNavigate2(LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName, CByteArray& baPostedData, LPCTSTR lpszHeaders, BOOL* pbCancel);
//	virtual void OnProgressChange(long nProgress, long nProgressMax);
	virtual void OnDownloadComplete();
};

#ifndef _DEBUG  // XiYuanView.cpp 中的调试版本
inline CXiYuanDoc* CXiYuanView::GetDocument() const
   { return reinterpret_cast<CXiYuanDoc*>(m_pDocument); }
#endif

