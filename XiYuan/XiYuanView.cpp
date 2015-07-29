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

// XiYuanView.cpp : CXiYuanView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
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
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CHtmlView::OnFilePrint)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CXiYuanView ����/����

CXiYuanView::CXiYuanView()
{
	// TODO:  �ڴ˴���ӹ������

}

CXiYuanView::~CXiYuanView()
{
}

BOOL CXiYuanView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:  �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CHtmlView::PreCreateWindow(cs);
}

void CXiYuanView::OnInitialUpdate()
{
	CHtmlView::OnInitialUpdate();

	Navigate2(_T("http://vod.cau.edu.cn/"),NULL,NULL);
}


// CXiYuanView ��ӡ


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


// CXiYuanView ���

#ifdef _DEBUG
void CXiYuanView::AssertValid() const
{
	CHtmlView::AssertValid();
}

void CXiYuanView::Dump(CDumpContext& dc) const
{
	CHtmlView::Dump(dc);
}

CXiYuanDoc* CXiYuanView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CXiYuanDoc)));
	return (CXiYuanDoc*)m_pDocument;
}
#endif //_DEBUG


// CXiYuanView ��Ϣ�������


void CXiYuanView::OnNavigateComplete2(LPCTSTR strURL)
{
	IHTMLDocument2 * pDoc = (IHTMLDocument2 *)GetHtmlDocument();
	BSTR title;
	pDoc->get_title(&title);
	GetDocument()->SetTitle(title);
	pDoc->Release();

	((CMainFrame *)AfxGetMainWnd())->m_wndStatusBar.SetNavigateStatus(L"���ڼ���ҳ��...");

	CHtmlView::OnNavigateComplete2(strURL);
}


void CXiYuanView::OnDocumentComplete(LPCTSTR lpszURL)
{
	IHTMLDocument2 * pDoc = (IHTMLDocument2 *)GetHtmlDocument();
	CVodCtrl::CheckAndReplaceMovieHref(pDoc);
	pDoc->Release();

	((CMainFrame *)AfxGetMainWnd())->m_wndStatusBar.SetNavigateStatus(L"����");
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
		((CMainFrame *)AfxGetMainWnd())->m_wndStatusBar.SetNavigateStatus(L"�������ӷ�����...");
		((CMainFrame *)AfxGetMainWnd())->m_wndStatusBar.SetNavigateProgress(TRUE, TRUE, 10);
		GetDocument()->SetTitle(L"ϧԵ����Ϊ������ҳ��...");
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
