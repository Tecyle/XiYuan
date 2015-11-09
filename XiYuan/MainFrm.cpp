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

// MainFrm.cpp : CMainFrame ���ʵ��
//

#include "stdafx.h"
#include "XiYuan.h"
#include "XiYuanView.h"
#include "MainFrm.h"
#include "Qrcode.h"
#include "DownMgrDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_WINDOW_MANAGER, &CMainFrame::OnWindowManager)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
	ON_COMMAND(ID_VIEW_CAPTION_BAR, &CMainFrame::OnViewCaptionBar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_CAPTION_BAR, &CMainFrame::OnUpdateViewCaptionBar)
	ON_COMMAND(ID_TOOLS_OPTIONS, &CMainFrame::OnOptions)
	ON_MESSAGE(WM_MOVIE_DOWNLOAD_PROGRESS, &CMainFrame::OnMovieDownloadProgress)
	ON_MESSAGE(WM_MOVIE_DOWNLOAD_START, &CMainFrame::OnMovieDownloadStart)
	ON_MESSAGE(WM_MOVIE_DOWNLOAD_END, &CMainFrame::OnMovieDownloadEnd)
	ON_COMMAND(ID_BUTTON_HOMEPAGE, &CMainFrame::OnButtonHomepage)
	ON_COMMAND(ID_BUTTON_MOVIE, &CMainFrame::OnButtonMovie)
	ON_COMMAND(ID_BUTTON_ANIMATION, &CMainFrame::OnButtonAnimation)
	ON_COMMAND(ID_BUTTON_TELEPLAY, &CMainFrame::OnButtonTeleplay)
	ON_COMMAND(ID_BUTTON_SCIENCE, &CMainFrame::OnButtonScience)
	ON_COMMAND(ID_BUTTON_VARIETY, &CMainFrame::OnButtonVariety)
	ON_COMMAND(ID_BUTTON_SPORT, &CMainFrame::OnButtonSport)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SEARCH, &CMainFrame::OnUpdateEditSearch)
	ON_COMMAND(ID_BUTTON_SEARCH, &CMainFrame::OnButtonSearch)
	ON_COMMAND(ID_EDIT_SEARCH, &CMainFrame::OnEditSearch)
	ON_COMMAND(ID_BUTTON_QRCODE, &CMainFrame::OnButtonQrcode)
	ON_COMMAND(ID_BUTTON10, &CMainFrame::OnButton10)
END_MESSAGE_MAP()

// CMainFrame ����/����

CMainFrame::CMainFrame()
{
	// TODO:  �ڴ���ӳ�Ա��ʼ������
	m_search = "";
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_WINDOWS_7);
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;

	CMDITabInfo mdiTabParams;
	mdiTabParams.m_style = CMFCTabCtrl::STYLE_3D_ONENOTE; // ����������ʽ...
	mdiTabParams.m_bActiveTabCloseButton = TRUE;      // ����Ϊ FALSE �Ὣ�رհ�ť������ѡ�������Ҳ�
	mdiTabParams.m_bTabIcons = TRUE;    // ����Ϊ TRUE ���� MDI ѡ��������ĵ�ͼ��
	mdiTabParams.m_bAutoColor = TRUE;    // ����Ϊ FALSE ������ MDI ѡ����Զ���ɫ
	mdiTabParams.m_bDocumentMenu = TRUE; // ��ѡ�������ұ�Ե�����ĵ��˵�
	EnableMDITabbedGroups(TRUE, mdiTabParams);

	m_wndRibbonBar.Create(this);
	m_wndRibbonBar.LoadFromResource(IDR_RIBBON);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("δ�ܴ���״̬��\n");
		return -1;      // δ�ܴ���
	}

	CString strTitlePane1;
	CString strTitlePane2;
	bNameValid = strTitlePane1.LoadString(IDS_STATUS_PANE1);
	ASSERT(bNameValid);
	bNameValid = strTitlePane2.LoadString(IDS_STATUS_PANE2);
	ASSERT(bNameValid);
	m_wndStatusBar.AddElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE1, strTitlePane1, TRUE), strTitlePane1);
	m_wndStatusBar.AddExtendedElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE2, strTitlePane2, TRUE), strTitlePane2);

	// ���� Visual Studio 2005 ��ʽͣ��������Ϊ
	CDockingManager::SetDockingMode(DT_SMART);
	// ���� Visual Studio 2005 ��ʽͣ�������Զ�������Ϊ
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// ����������: 
	if (!CreateCaptionBar())
	{
		TRACE0("δ�ܴ���������\n");
		return -1;      // δ�ܴ���
	}

	// ���ز˵���ͼ��(�����κα�׼��������): 
	CMFCToolBar::AddToolBarForImageCollection(IDR_MENU_IMAGES, theApp.m_bHiColorIcons ? IDB_MENU_IMAGES_24 : 0);

	// ����ͣ������
	if (!CreateDockingWindows())
	{
		TRACE0("δ�ܴ���ͣ������\n");
		return -1;
	}

	m_wndFileView.EnableDocking(CBRS_ALIGN_ANY);
	m_wndClassView.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndFileView);
	CDockablePane* pTabbedBar = NULL;
	m_wndClassView.AttachToTabWnd(&m_wndFileView, DM_SHOW, TRUE, &pTabbedBar);

	// ���ڳ־�ֵ�����Ӿ�����������ʽ
	OnApplicationLook(theApp.m_nAppLook);

	// ������ǿ�Ĵ��ڹ���Ի���
	EnableWindowsDialog(ID_WINDOW_MANAGER, ID_WINDOW_MANAGER, TRUE);

	// ���ĵ�����Ӧ�ó��������ڴ��ڱ������ϵ�˳����н�������
	// ���Ľ��������Ŀ����ԣ���Ϊ��ʾ���ĵ�����������ͼ��
	ModifyStyle(0, FWS_PREFIXTITLE);

	// ע��������Ϣ�ص�
	theDownloadManager.AddProgressCallback(this);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO:  �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		 | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_MAXIMIZE | WS_SYSMENU;

	return TRUE;
}

BOOL CMainFrame::CreateDockingWindows()
{
	BOOL bNameValid;

	// ��������ͼ
	CString strClassView;
	bNameValid = strClassView.LoadString(IDS_CLASS_VIEW);
	ASSERT(bNameValid);
	if (!m_wndClassView.Create(strClassView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_CLASSVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("δ�ܴ���������ͼ������\n");
		return FALSE; // δ�ܴ���
	}

	// �����ļ���ͼ
	CString strFileView;
	bNameValid = strFileView.LoadString(IDS_FILE_VIEW);
	ASSERT(bNameValid);
	if (!m_wndFileView.Create(strFileView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_FILEVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT| CBRS_FLOAT_MULTI))
	{
		TRACE0("δ�ܴ������ļ���ͼ������\n");
		return FALSE; // δ�ܴ���
	}

	SetDockingWindowIcons(theApp.m_bHiColorIcons);
	return TRUE;
}

void CMainFrame::SetDockingWindowIcons(BOOL bHiColorIcons)
{
	HICON hFileViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_FILE_VIEW_HC : IDI_FILE_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndFileView.SetIcon(hFileViewIcon, FALSE);

	HICON hClassViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_CLASS_VIEW_HC : IDI_CLASS_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndClassView.SetIcon(hClassViewIcon, FALSE);

	UpdateMDITabbedBarsIcons();
}

BOOL CMainFrame::CreateCaptionBar()
{
	if (!m_wndCaptionBar.Create(WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, this, ID_VIEW_CAPTION_BAR, -1, TRUE))
	{
		TRACE0("δ�ܴ���������\n");
		return FALSE;
	}

	BOOL bNameValid;

	CString strTemp, strTemp2;
	bNameValid = strTemp.LoadString(IDS_CAPTION_BUTTON);
	ASSERT(bNameValid);
	m_wndCaptionBar.SetButton(strTemp, ID_TOOLS_OPTIONS, CMFCCaptionBar::ALIGN_LEFT, FALSE);
	bNameValid = strTemp.LoadString(IDS_CAPTION_BUTTON_TIP);
	ASSERT(bNameValid);
	m_wndCaptionBar.SetButtonToolTip(strTemp);

	bNameValid = strTemp.LoadString(IDS_CAPTION_TEXT);
	ASSERT(bNameValid);
	m_wndCaptionBar.SetText(strTemp, CMFCCaptionBar::ALIGN_LEFT);

	m_wndCaptionBar.SetBitmap(IDB_INFO, RGB(255, 255, 255), FALSE, CMFCCaptionBar::ALIGN_LEFT);
	bNameValid = strTemp.LoadString(IDS_CAPTION_IMAGE_TIP);
	ASSERT(bNameValid);
	bNameValid = strTemp2.LoadString(IDS_CAPTION_IMAGE_TEXT);
	ASSERT(bNameValid);
	m_wndCaptionBar.SetImageToolTip(strTemp, strTemp2);

	return TRUE;
}

// CMainFrame ���

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame ��Ϣ�������

void CMainFrame::OnWindowManager()
{
	ShowWindowsDialog();
}

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(TRUE);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
	}

	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}

void CMainFrame::OnViewCaptionBar()
{
	m_wndCaptionBar.ShowWindow(m_wndCaptionBar.IsVisible() ? SW_HIDE : SW_SHOW);
	RecalcLayout(FALSE);
}

void CMainFrame::OnUpdateViewCaptionBar(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndCaptionBar.IsVisible());
}

void CMainFrame::OnOptions()
{
	CMFCRibbonCustomizeDialog *pOptionsDlg = new CMFCRibbonCustomizeDialog(this, &m_wndRibbonBar);
	ASSERT(pOptionsDlg != NULL);

	pOptionsDlg->DoModal();
	delete pOptionsDlg;
}

LRESULT CMainFrame::OnMovieDownloadProgress(WPARAM wParam, LPARAM lParam)
{
	m_wndStatusBar.SetDownloadProgress(wParam);
	return 0;
}

LRESULT CMainFrame::OnMovieDownloadStart(WPARAM wParam, LPARAM lParam)
{
	DownloadTaskParams * params = (DownloadTaskParams *)lParam;
	ASSERT(params);
	m_wndStatusBar.StartDownloadProgress(params->dstFileName);
	return 0;
}

LRESULT CMainFrame::OnMovieDownloadEnd(WPARAM wParam, LPARAM lParam)
{
	DownloadTaskParams * params = (DownloadTaskParams *)lParam;
	ASSERT(params);
	m_wndStatusBar.EndDownloadProgress();
	return 0;
}



void CMainFrame::OnButtonHomepage()
{
	// TODO:  �ڴ���������������
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

	pView->Navigate2(_T("http://vod.cau.edu.cn/"), NULL, NULL);
	
	//CXiYuanView *pView = (CXiYuanView*)this->GetActiveView();
	////assert(pView);
	
	//LPDISPATCH *ppDisp=NULL ;
	//pView->SetRegisterAsBrowser(TRUE);
	//*ppDisp = pView->GetApplication();
	//BOOL cancel = false;
	//pView->OnNewWindow2(ppDisp,&cancel);
	//�򿪰�˼��ҳ��ѡ�
	// Get a pointer to the application object.
	//CWinApp* pApp = AfxGetApp();
	//
	//// Get the correct document template.
	//POSITION pos = pApp->GetFirstDocTemplatePosition();
	//CDocTemplate* pDocTemplate = pApp->GetNextDocTemplate(pos);
	//// Create a new frame.
	//CFrameWnd* pFrame = pDocTemplate->CreateNewFrame(
	//	pDocTemplate->CreateNewDocument(),
	//	(CFrameWnd*)AfxGetMainWnd());
	//// Activate the frame.
	//pDocTemplate->InitialUpdateFrame(pFrame, NULL);
	//CXiYuanView* pView = (CXiYuanView*)pFrame->GetActiveView();
	//// Pass pointer of WebBrowser object.
	//pView->SetRegisterAsBrowser(TRUE);
	//LPDISPATCH ppDisp = pView->GetApplication();
	//
	////assert(ppDisp);
	//BOOL Cancel = TRUE;
	//pView->OnNewWindow2(&ppDisp,& Cancel);
	
	
}


void CMainFrame::OnButtonMovie()
{
	// TODO:  �ڴ���������������
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

	pView->Navigate2(_T("http://vod.cau.edu.cn/index.php?mod=category&action=view&id=2"), NULL, NULL);
}


void CMainFrame::OnButtonAnimation()
{
	// TODO:  �ڴ���������������
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

	pView->Navigate2(_T("http://vod.cau.edu.cn/index.php?mod=category&action=view&id=72"), NULL, NULL);
}


void CMainFrame::OnButtonTeleplay()
{
	// TODO:  �ڴ���������������
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

	pView->Navigate2(_T("http://vod.cau.edu.cn/index.php?mod=category&action=view&id=12"), NULL, NULL);
}


void CMainFrame::OnButtonScience()
{
	// TODO:  �ڴ���������������
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

	pView->Navigate2(_T("http://vod.cau.edu.cn/index.php?mod=category&action=view&id=127"), NULL, NULL);
}


void CMainFrame::OnButtonVariety()
{
	// TODO:  �ڴ���������������
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

	pView->Navigate2(_T("http://vod.cau.edu.cn/index.php?mod=category&action=view&id=22"), NULL, NULL);
}


void CMainFrame::OnButtonSport()
{
	// TODO:  �ڴ���������������
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

	pView->Navigate2(_T("http://vod.cau.edu.cn/index.php?mod=category&action=view&id=106"), NULL, NULL);
}


void CMainFrame::OnUpdateEditSearch(CCmdUI *pCmdUI)
{
	// TODO:  �ڴ������������û����洦��������

}




void CMainFrame::OnButtonSearch()
{
	// TODO:  �ڴ���������������
	
	CString path;
	path=L"http://vod.cau.edu.cn/index.php?type=title&keyword="+m_search+L"&mod=content&action=search";
	// TODO:  �ڴ���������������
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
	pView->Navigate2(path, NULL, NULL);


}


void CMainFrame::OnEditSearch()
{
	// TODO:  �ڴ���������������
	
	//��ȡedit�ؼ�ָ��
	CMFCRibbonEdit *pEditTest=DYNAMIC_DOWNCAST(CMFCRibbonEdit, m_wndRibbonBar.FindByID(ID_EDIT_SEARCH));
	m_search = pEditTest->GetEditText();
	
	
}




bool SaveBmp(char*bmpName, unsigned char*imgBuf, int width, int height, int biBitCount, RGBQUAD *pColorTable)
{
	//���λͼ����ָ��Ϊ0����û�����ݴ��룬��������

	if (!imgBuf)
		return false;

	//��ɫ���С�����ֽ�Ϊ��λ���Ҷ�ͼ����ɫ��Ϊ1024�ֽڣ���ɫͼ����ɫ���СΪ0

	int colorTablesize = 0;

	if (biBitCount == 8)
		colorTablesize = 1024;

	//���洢ͼ������ÿ���ֽ���Ϊ4�ı���

	int lineByte = (width * biBitCount / 8 + 3) / 4 * 4;

	//�Զ�����д�ķ�ʽ���ļ�

	FILE *fp = fopen(bmpName, "wb");

	if (fp == 0)
		return false;

	//����λͼ�ļ�ͷ�ṹ��������д�ļ�ͷ��Ϣ

	BITMAPFILEHEADER fileHead;

	fileHead.bfType = 0x4D42;//bmp����

	//bfSize��ͼ���ļ�4����ɲ���֮��

	fileHead.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + colorTablesize + lineByte*height;

	fileHead.bfReserved1 = 0;

	fileHead.bfReserved2 = 0;

	//bfOffBits��ͼ���ļ�ǰ3����������ռ�֮��

	fileHead.bfOffBits = 54 + colorTablesize;

	//д�ļ�ͷ���ļ�

	fwrite(&fileHead, sizeof(BITMAPFILEHEADER), 1, fp);

	//����λͼ��Ϣͷ�ṹ��������д��Ϣͷ��Ϣ

	BITMAPINFOHEADER head;

	head.biBitCount = biBitCount;

	head.biClrImportant = 0;

	head.biClrUsed = 0;

	head.biCompression = 0;

	head.biHeight = height;

	head.biPlanes = 1;

	head.biSize = 40;

	head.biSizeImage = lineByte*height;

	head.biWidth = width;

	head.biXPelsPerMeter = 0;

	head.biYPelsPerMeter = 0;

	//дλͼ��Ϣͷ���ڴ�

	fwrite(&head, sizeof(BITMAPINFOHEADER), 1, fp);

	//����Ҷ�ͼ������ɫ��д���ļ� 

	if (biBitCount == 8)
		fwrite(pColorTable, sizeof(RGBQUAD), 256, fp);

	//дλͼ���ݽ��ļ�

	fwrite(imgBuf, height*lineByte, 1, fp);

	//�ر��ļ�

	fclose(fp);

	return true;

}
BYTE* GenerateBarCode(const char *_pStrInfo, int *_pWidth, int *_pHeight)
{
	if (NULL == _pStrInfo)
	{
		return NULL;
	}

	//Copy String
	int		iLen = 0;
	char	*_pStrCopy = NULL;

	iLen = strlen(_pStrInfo) + 1;
	if (iLen > 120)
	{
		return NULL;
	}

	if (NULL == (_pStrCopy = new char[iLen]))
	{
		return NULL;
	}
	strcpy(_pStrCopy, _pStrInfo);


	//Qrcode
	Qrcode _GenQrcode;
	_GenQrcode.setQrcodeErrorCorrect('M');
	_GenQrcode.setQrcodeEncodeMode('B');
	_GenQrcode.setQrcodeVersion(7);

	int iLRet = 0;//���س���

	bool** s = _GenQrcode.calQrcode(_pStrCopy, iLen, iLRet);


	int nStartX = 4;
	int nStartY = 4;
	int nStopX = 4;
	int nStopY = 4;
	int nCodeSize = 4;

	BYTE* pData = NULL;

	int SizeX = ((nStartX + iLRet * nCodeSize + nStopX) * 32 + 31) / 32;
	int SizeY = nStartY + iLRet * nCodeSize + nStopY;

	int iLRetX = (SizeX - nStartX - nStopX) / nCodeSize;
	int iLRetY = iLRet;

	unsigned int nLineByte = SizeX * 4;
	unsigned int nSize = nLineByte * SizeY;

	if (NULL == (pData = new BYTE[nSize]))
	{
		if (NULL != _pStrCopy)
		{
			delete[] _pStrCopy;
			_pStrCopy = NULL;
		}

		return NULL;
	}

	BYTE ColorWhite = 255;
	BYTE ColorBlack = 0;
	BYTE ColorAlpha = 255;
	BYTE ColorValue = ColorBlack;

	BYTE _R = 0;
	BYTE _G = 1;
	BYTE _B = 2;
	BYTE _A = 3;

	memset(pData, ColorWhite, nSize);

	int i = 0;
	int j = 0;
	unsigned int Cur = 0;
	int _nCodeSize = nCodeSize;
	bool bBlack = false;

	for (j = 0; j<iLRetY*nCodeSize; ++j)
	{
		for (i = 0; i<iLRetX; ++i)
		{
			bBlack = s[j / nCodeSize][i];
			ColorValue = bBlack ? ColorBlack : ColorWhite;

			Cur = (nStartY + j)*nLineByte + (nStartX + i*nCodeSize * 4);

			_nCodeSize = 0;
			while (_nCodeSize  < nCodeSize)
			{
				pData[Cur + _nCodeSize * nCodeSize + _R] = ColorValue;
				pData[Cur + _nCodeSize * nCodeSize + _G] = ColorValue;
				pData[Cur + _nCodeSize * nCodeSize + _B] = ColorValue;
				pData[Cur + _nCodeSize * nCodeSize + _A] = ColorAlpha;
				_nCodeSize++;
			}
		}
	}

	if (NULL != _pStrCopy)
	{
		delete[] _pStrCopy;
		_pStrCopy = NULL;
	}

	*_pWidth = SizeX;
	*_pHeight = SizeY;

	return pData;
}

void CMainFrame::OnButtonQrcode()
{
	// TODO:  �ڴ���������������
	//CString path = L"http://www.baidu.com";
	CString path = L"�������գ����ǿ����ö�ά��������";
	

	int W = 0;
	int H = 0;
	string s = UnicodeToAnsi(path);
	BYTE *lpByte = NULL;

	lpByte = GenerateBarCode(s.c_str(), &W, &H);

	if (NULL == lpByte)
	{
		return;
	}

	SaveBmp("GY.bmp", lpByte, W, H, 32, NULL);
	ShellExecute(NULL, L"Open", L"GY.bmp", NULL, NULL, SW_SHOW);
	
	delete[] lpByte;
	lpByte = NULL;
}

void CMainFrame::OnDownloadStart(DownloadTaskParams * params)
{
	PostMessage(WM_MOVIE_DOWNLOAD_START, 0U, (LPARAM)params);
	lastProgress = 0;
}

void CMainFrame::OnDownloadProgress(DownloadTaskParams * params)
{
	if (params->fileSize == 0)
		return;
	int progressPercent = (int)((double)params->fileDowned * 100.0 / (double)params->fileSize);
	if (progressPercent > lastProgress)
	{
		PostMessage(WM_MOVIE_DOWNLOAD_PROGRESS, progressPercent, 0L);
		lastProgress = progressPercent;
	}
}

void CMainFrame::OnDownloadError(DownloadTaskParams * params, HRESULT hr)
{
	theToast.ShowToast(L"��Ŷ~�������ع����з�����������~����ʧ����~");
}

void CMainFrame::OnDownloadCompleted(DownloadTaskParams * params)
{
	PostMessage(WM_MOVIE_DOWNLOAD_END, 0U, (LPARAM)params);
}


void CMainFrame::OnButton10()
{
	CDownMgrDlg dlg;
	dlg.DoModal();
}
