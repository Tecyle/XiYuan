// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// (��Fluent UI��)����ʾ�������ο���
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���  
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�  
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ�������  
// http://go.microsoft.com/fwlink/?LinkId=238214��
//
// ��Ȩ����(C) Microsoft Corporation
// ��������Ȩ����

// XiYuanView.h : CXiYuanView ��Ľӿ�
//

#pragma once
#include "VodCtrl.h"
#include "XiYuanDoc.h"

class CXiYuanView : public CHtmlView
{
protected: // �������л�����
	CXiYuanView();
	DECLARE_DYNCREATE(CXiYuanView)

// ����
public:
	CXiYuanDoc* GetDocument() const;
	CVodCtrl mVodCtrl;

// ����
public:

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual HRESULT OnGetExternal(LPDISPATCH *lppDispatch);
protected:
	virtual void OnInitialUpdate(); // ������һ�ε���

// ʵ��
public:
	virtual ~CXiYuanView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
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

#ifndef _DEBUG  // XiYuanView.cpp �еĵ��԰汾
inline CXiYuanDoc* CXiYuanView::GetDocument() const
   { return reinterpret_cast<CXiYuanDoc*>(m_pDocument); }
#endif

