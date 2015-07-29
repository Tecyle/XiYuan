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

// XiYuanView.cpp : CXiYuanView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "XiYuan.h"
#endif

#include "XiYuanDoc.h"
#include "XiYuanView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CXiYuanView

IMPLEMENT_DYNCREATE(CXiYuanView, CHtmlView)

BEGIN_MESSAGE_MAP(CXiYuanView, CHtmlView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CHtmlView::OnFilePrint)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CXiYuanView 构造/析构

CXiYuanView::CXiYuanView()
{
	// TODO:  在此处添加构造代码

}

CXiYuanView::~CXiYuanView()
{
}

BOOL CXiYuanView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:  在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CHtmlView::PreCreateWindow(cs);
}

void CXiYuanView::OnInitialUpdate()
{
	CHtmlView::OnInitialUpdate();

	Navigate2(_T("http://vod.cau.edu.cn/"),NULL,NULL);
}


// CXiYuanView 打印


void CXiYuanView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}


void CXiYuanView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CXiYuanView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CXiYuanView 诊断

#ifdef _DEBUG
void CXiYuanView::AssertValid() const
{
	CHtmlView::AssertValid();
}

void CXiYuanView::Dump(CDumpContext& dc) const
{
	CHtmlView::Dump(dc);
}

CXiYuanDoc* CXiYuanView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CXiYuanDoc)));
	return (CXiYuanDoc*)m_pDocument;
}
#endif //_DEBUG


// CXiYuanView 消息处理程序


void CXiYuanView::OnNavigateComplete2(LPCTSTR strURL)
{
	IHTMLDocument2 * pDoc = (IHTMLDocument2 *)GetHtmlDocument();
	BSTR title;
	pDoc->get_title(&title);
	GetDocument()->SetTitle(title);
	pDoc->Release();

	((CMainFrame *)AfxGetMainWnd())->m_wndStatusBar.SetNavigateStatus(L"正在加载页面...");

	CHtmlView::OnNavigateComplete2(strURL);
}


void CXiYuanView::OnDocumentComplete(LPCTSTR lpszURL)
{
	IHTMLDocument2 * pDoc = (IHTMLDocument2 *)GetHtmlDocument();
	CVodCtrl::CheckAndReplaceMovieHref(pDoc);
	pDoc->Release();

	((CMainFrame *)AfxGetMainWnd())->m_wndStatusBar.SetNavigateStatus(L"就绪");
	((CMainFrame *)AfxGetMainWnd())->m_wndStatusBar.SetNavigateProgress(FALSE, FALSE, 0);

	CHtmlView::OnDocumentComplete(lpszURL);
}

HRESULT CXiYuanView::OnGetExternal(LPDISPATCH *lppDispatch)
{
	*lppDispatch = mVodCtrl.GetIDispatch(TRUE);
	return S_OK;
}


void CXiYuanView::OnNewWindow2(LPDISPATCH* ppDisp, BOOL* Cancel)
{
	// Get a pointer to the application object.
	CWinApp* pApp = AfxGetApp();

	// Get the correct document template.
	POSITION pos = pApp->GetFirstDocTemplatePosition();
	CDocTemplate* pDocTemplate = pApp->GetNextDocTemplate(pos);

	// Create a new frame.
	CFrameWnd* pFrame = pDocTemplate->CreateNewFrame(
		pDocTemplate->CreateNewDocument(),
		(CFrameWnd*)AfxGetMainWnd());

	// Activate the frame.
	pDocTemplate->InitialUpdateFrame(pFrame, NULL);
	CXiYuanView* pView = (CXiYuanView*)pFrame->GetActiveView();

	// Pass pointer of WebBrowser object.
	pView->SetRegisterAsBrowser(TRUE);
	*ppDisp = pView->GetApplication();

	CHtmlView::OnNewWindow2(ppDisp, Cancel);
}


void CXiYuanView::OnBeforeNavigate2(LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName, CByteArray& baPostedData, LPCTSTR lpszHeaders, BOOL* pbCancel)
{
	CString sTarget = lpszURL;
	if (sTarget.Left(wcslen(L"javascript:")).CompareNoCase(L"javascript:"))
	{
		((CMainFrame *)AfxGetMainWnd())->m_wndStatusBar.SetNavigateStatus(L"正在连接服务器...");
		((CMainFrame *)AfxGetMainWnd())->m_wndStatusBar.SetNavigateProgress(TRUE, TRUE, 10);
		GetDocument()->SetTitle(L"惜缘正在为您加载页面...");
	}

	CHtmlView::OnBeforeNavigate2(lpszURL, nFlags, lpszTargetFrameName, baPostedData, lpszHeaders, pbCancel);
}


//void CXiYuanView::OnProgressChange(long nProgress, long nProgressMax)
//{
//	CHtmlView::OnProgressChange(nProgress, nProgressMax);
//}


void CXiYuanView::OnDownloadComplete()
{
	IHTMLDocument2 * pDoc = (IHTMLDocument2 *)GetHtmlDocument();
	BSTR title;
	if (!pDoc)
		return;
	pDoc->get_title(&title);
	GetDocument()->SetTitle(title);
	pDoc->Release();

	CHtmlView::OnDownloadComplete();
}
