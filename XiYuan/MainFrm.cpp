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

// MainFrm.cpp : CMainFrame 类的实现
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

// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
	// TODO:  在此添加成员初始化代码
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
	mdiTabParams.m_style = CMFCTabCtrl::STYLE_3D_ONENOTE; // 其他可用样式...
	mdiTabParams.m_bActiveTabCloseButton = TRUE;      // 设置为 FALSE 会将关闭按钮放置在选项卡区域的右侧
	mdiTabParams.m_bTabIcons = TRUE;    // 设置为 TRUE 将在 MDI 选项卡上启用文档图标
	mdiTabParams.m_bAutoColor = TRUE;    // 设置为 FALSE 将禁用 MDI 选项卡的自动着色
	mdiTabParams.m_bDocumentMenu = TRUE; // 在选项卡区域的右边缘启用文档菜单
	EnableMDITabbedGroups(TRUE, mdiTabParams);

	m_wndRibbonBar.Create(this);
	m_wndRibbonBar.LoadFromResource(IDR_RIBBON);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}

	CString strTitlePane1;
	CString strTitlePane2;
	bNameValid = strTitlePane1.LoadString(IDS_STATUS_PANE1);
	ASSERT(bNameValid);
	bNameValid = strTitlePane2.LoadString(IDS_STATUS_PANE2);
	ASSERT(bNameValid);
	m_wndStatusBar.AddElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE1, strTitlePane1, TRUE), strTitlePane1);
	m_wndStatusBar.AddExtendedElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE2, strTitlePane2, TRUE), strTitlePane2);

	// 启用 Visual Studio 2005 样式停靠窗口行为
	CDockingManager::SetDockingMode(DT_SMART);
	// 启用 Visual Studio 2005 样式停靠窗口自动隐藏行为
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// 创建标题栏: 
	if (!CreateCaptionBar())
	{
		TRACE0("未能创建标题栏\n");
		return -1;      // 未能创建
	}

	// 加载菜单项图像(不在任何标准工具栏上): 
	CMFCToolBar::AddToolBarForImageCollection(IDR_MENU_IMAGES, theApp.m_bHiColorIcons ? IDB_MENU_IMAGES_24 : 0);

	// 创建停靠窗口
	if (!CreateDockingWindows())
	{
		TRACE0("未能创建停靠窗口\n");
		return -1;
	}

	m_wndFileView.EnableDocking(CBRS_ALIGN_ANY);
	m_wndClassView.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndFileView);
	CDockablePane* pTabbedBar = NULL;
	m_wndClassView.AttachToTabWnd(&m_wndFileView, DM_SHOW, TRUE, &pTabbedBar);

	// 基于持久值设置视觉管理器和样式
	OnApplicationLook(theApp.m_nAppLook);

	// 启用增强的窗口管理对话框
	EnableWindowsDialog(ID_WINDOW_MANAGER, ID_WINDOW_MANAGER, TRUE);

	// 将文档名和应用程序名称在窗口标题栏上的顺序进行交换。这
	// 将改进任务栏的可用性，因为显示的文档名带有缩略图。
	ModifyStyle(0, FWS_PREFIXTITLE);

	// 注册下载消息回调
	theDownloadManager.AddProgressCallback(this);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO:  在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		 | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_MAXIMIZE | WS_SYSMENU;

	return TRUE;
}

BOOL CMainFrame::CreateDockingWindows()
{
	BOOL bNameValid;

	// 创建类视图
	CString strClassView;
	bNameValid = strClassView.LoadString(IDS_CLASS_VIEW);
	ASSERT(bNameValid);
	if (!m_wndClassView.Create(strClassView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_CLASSVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("未能创建“类视图”窗口\n");
		return FALSE; // 未能创建
	}

	// 创建文件视图
	CString strFileView;
	bNameValid = strFileView.LoadString(IDS_FILE_VIEW);
	ASSERT(bNameValid);
	if (!m_wndFileView.Create(strFileView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_FILEVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT| CBRS_FLOAT_MULTI))
	{
		TRACE0("未能创建“文件视图”窗口\n");
		return FALSE; // 未能创建
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
		TRACE0("未能创建标题栏\n");
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

// CMainFrame 诊断

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


// CMainFrame 消息处理程序

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
	// TODO:  在此添加命令处理程序代码
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
	//打开爱思主页新选项卡
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
	// TODO:  在此添加命令处理程序代码
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
	// TODO:  在此添加命令处理程序代码
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
	// TODO:  在此添加命令处理程序代码
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
	// TODO:  在此添加命令处理程序代码
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
	// TODO:  在此添加命令处理程序代码
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
	// TODO:  在此添加命令处理程序代码
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
	// TODO:  在此添加命令更新用户界面处理程序代码

}




void CMainFrame::OnButtonSearch()
{
	// TODO:  在此添加命令处理程序代码
	
	CString path;
	path=L"http://vod.cau.edu.cn/index.php?type=title&keyword="+m_search+L"&mod=content&action=search";
	// TODO:  在此添加命令处理程序代码
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
	// TODO:  在此添加命令处理程序代码
	
	//获取edit控件指针
	CMFCRibbonEdit *pEditTest=DYNAMIC_DOWNCAST(CMFCRibbonEdit, m_wndRibbonBar.FindByID(ID_EDIT_SEARCH));
	m_search = pEditTest->GetEditText();
	
	
}




bool SaveBmp(char*bmpName, unsigned char*imgBuf, int width, int height, int biBitCount, RGBQUAD *pColorTable)
{
	//如果位图数据指针为0，则没有数据传入，函数返回

	if (!imgBuf)
		return false;

	//颜色表大小，以字节为单位，灰度图像颜色表为1024字节，彩色图像颜色表大小为0

	int colorTablesize = 0;

	if (biBitCount == 8)
		colorTablesize = 1024;

	//待存储图像数据每行字节数为4的倍数

	int lineByte = (width * biBitCount / 8 + 3) / 4 * 4;

	//以二进制写的方式打开文件

	FILE *fp = fopen(bmpName, "wb");

	if (fp == 0)
		return false;

	//申请位图文件头结构变量，填写文件头信息

	BITMAPFILEHEADER fileHead;

	fileHead.bfType = 0x4D42;//bmp类型

	//bfSize是图像文件4个组成部分之和

	fileHead.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + colorTablesize + lineByte*height;

	fileHead.bfReserved1 = 0;

	fileHead.bfReserved2 = 0;

	//bfOffBits是图像文件前3个部分所需空间之和

	fileHead.bfOffBits = 54 + colorTablesize;

	//写文件头进文件

	fwrite(&fileHead, sizeof(BITMAPFILEHEADER), 1, fp);

	//申请位图信息头结构变量，填写信息头信息

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

	//写位图信息头进内存

	fwrite(&head, sizeof(BITMAPINFOHEADER), 1, fp);

	//如果灰度图像，有颜色表，写入文件 

	if (biBitCount == 8)
		fwrite(pColorTable, sizeof(RGBQUAD), 256, fp);

	//写位图数据进文件

	fwrite(imgBuf, height*lineByte, 1, fp);

	//关闭文件

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

	int iLRet = 0;//返回长度

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
	// TODO:  在此添加命令处理程序代码
	//CString path = L"http://www.baidu.com";
	CString path = L"调拨阿勒，我们可以用二维码聊天了";
	

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
	theToast.ShowToast(L"噶哦~好像下载过程中发生错误了诶~下载失败了~");
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
