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

// MainFrm.h : CMainFrame ��Ľӿ�
//

#pragma once
#include "FileView.h"
#include "ClassView.h"
#include "XiYuanStatusBar.h"
#include "DownloadManager.h"

class CMainFrame : public CMDIFrameWndEx, public IDownloadProgress
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ʵ��
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:  // �ؼ���Ƕ���Ա
	CMFCRibbonBar     m_wndRibbonBar;
	CMFCRibbonApplicationButton m_MainButton;
	CMFCToolBarImages m_PanelImages;
	CXiYuanStatusBar  m_wndStatusBar;
	CFileView         m_wndFileView;
	CClassView        m_wndClassView;
	CMFCCaptionBar    m_wndCaptionBar;

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowManager();
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg void OnViewCaptionBar();
	afx_msg void OnUpdateViewCaptionBar(CCmdUI* pCmdUI);
	afx_msg void OnOptions();
	LRESULT OnMovieDownloadProgress(WPARAM wParam, LPARAM lParam);
	LRESULT OnMovieDownloadStart(WPARAM wParam, LPARAM lParam);
	LRESULT OnMovieDownloadEnd(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);
	BOOL CreateCaptionBar();
public:
	afx_msg void OnButtonHomepage();
	afx_msg void OnButtonMovie();
	afx_msg void OnButtonAnimation();
	afx_msg void OnButtonTeleplay();
	afx_msg void OnButtonScience();
	afx_msg void OnButtonVariety();
	afx_msg void OnButtonSport();
	afx_msg void OnUpdateEditSearch(CCmdUI *pCmdUI);
	
	
	CString m_search;
	int lastProgress;
	afx_msg void OnButtonSearch();
	afx_msg void OnEditSearch();
	afx_msg void OnButtonQrcode();

	// IDownloadProgress
public:
	virtual void OnDownloadStart(DownloadTaskParams * params);
	virtual void OnDownloadProgress(DownloadTaskParams * params);
	virtual void OnDownloadError(DownloadTaskParams * params, HRESULT hr);
	virtual void OnDownloadCompleted(DownloadTaskParams * params);

public:
	
	afx_msg void OnButton10();
};
BYTE* GenerateBarCode(const char *_pStrInfo, int *_pWidth, int *_pHeight);
bool SaveBmp(char*bmpName, unsigned char*imgBuf, int width, int height, int biBitCount, RGBQUAD *pColorTable);

