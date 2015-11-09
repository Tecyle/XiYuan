// ToastDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "XiYuan.h"
#include "ToastDlg.h"
#include "afxdialogex.h"


// CToastDlg 对话框

IMPLEMENT_DYNAMIC(CToastDlg, CDialogEx)

CToastDlg::CToastDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CToastDlg::IDD, pParent)
{

}

CToastDlg::~CToastDlg()
{
}

void CToastDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CToastDlg, CDialogEx)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

BOOL CToastDlg::ImageFromIDResource(UINT resourceID, LPCTSTR imgType, Image * &pImg)
{
	HINSTANCE hInst = AfxGetResourceHandle();
	HRSRC hRsrc = FindResource(hInst, MAKEINTRESOURCE(resourceID), imgType);
	if (hRsrc)
	{
		DWORD len = SizeofResource(hInst, hRsrc);
		BYTE* lpRsrc = (BYTE*)LoadResource(hInst, hRsrc);
		if (lpRsrc)
		{
			HGLOBAL m_hMem = GlobalAlloc(GMEM_FIXED, len);
			BYTE * pmem = (BYTE*)GlobalLock(m_hMem);
			memcpy(pmem, lpRsrc, len);
			IStream * pstm;
			CreateStreamOnHGlobal(m_hMem, FALSE, &pstm);
			pImg = Image::FromStream(pstm);

			GlobalUnlock(m_hMem);
			pstm->Release();
			FreeResource(lpRsrc);
			return TRUE;
		}
	}
	return FALSE;
}

void CToastDlg::DrawPromptMsg(Graphics * g, LPCTSTR msg)
{
	SolidBrush brush(Color(0, 0, 255));
	FontFamily fontFamily(L"微软雅黑");
	Gdiplus::Font font(&fontFamily, 0.2f, FontStyleRegular, UnitInch);
	PointF pointF(50, 220);
	RectF layoutRect(30, 220, 700, 0);
	RectF boundRect;
	INT codePointsFitted = 0;
	INT linesFitted = 0;
	g->MeasureString(msg, -1, &font, layoutRect, NULL, &boundRect, &codePointsFitted, &linesFitted);
	g->DrawString(msg, -1, &font, boundRect, NULL, &brush);
}

BOOL CToastDlg::UpdateDisplay(Image * image, int Transparent)
{
	int imageWidth = image->GetWidth();
	int imageHeight = image->GetHeight();
	HDC hdcTemp = GetDC()->m_hDC;
	m_hdcMemory = CreateCompatibleDC(hdcTemp);
	HBITMAP hBitmap = CreateCompatibleBitmap(hdcTemp, imageWidth, imageHeight);
	SelectObject(m_hdcMemory, hBitmap);
	if (Transparent < 0 || Transparent > 100)
		Transparent = 100;
	m_Blend.SourceConstantAlpha = int(Transparent * 2.55);
	HDC hdcScreen = ::GetDC(m_hWnd);
	RECT rct;
	GetWindowRect(&rct);
	POINT ptWinPos = { rct.left, rct.top };
	Graphics graph(m_hdcMemory);
	Point points[] = {
		Point(0, 0),
		Point(imageWidth, 0),
		Point(0, imageHeight)
	};
	graph.DrawImage(image, points, 3);
	if (msgQueue.size() > 0)
		DrawPromptMsg(&graph, msgQueue.front().msg);
	SIZE sizeWindow = { imageWidth, imageHeight };
	POINT ptSrc = { 0, 0 };
	DWORD dwExStyle = GetWindowLong(m_hWnd, GWL_EXSTYLE);
	if ((dwExStyle & 0x80000) != 0x80000)
		SetWindowLong(m_hWnd, GWL_EXSTYLE, dwExStyle ^ 0x80000);
	BOOL bRet = FALSE;
	bRet = ::UpdateLayeredWindow(m_hWnd, hdcScreen, &ptWinPos, &sizeWindow, m_hdcMemory, &ptSrc, 0, &m_Blend, 2);
	graph.ReleaseHDC(m_hdcMemory);
	::ReleaseDC(m_hWnd, hdcScreen);
	hdcScreen = NULL;
	::ReleaseDC(m_hWnd, hdcTemp);
	hdcTemp = NULL;
	DeleteObject(hBitmap);
	DeleteDC(m_hdcMemory);
	m_hdcMemory = NULL;
	return bRet;
}


// CToastDlg 消息处理程序


BOOL CToastDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


int CToastDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_Blend.BlendOp = 0;
	m_Blend.BlendFlags = 0;
	m_Blend.AlphaFormat = 1;
	m_Blend.SourceConstantAlpha = 255;

	ImageFromIDResource(IDB_XIYUAN_BAR1, L"PNG", pXiyuanChar1);
	ImageFromIDResource(IDB_XIYUAN_BAR2, L"PNG", pXiyuanChar2);
	::SetWindowPos(m_hWnd, HWND_TOPMOST, 
		0, 
		0, 
		pXiyuanChar1->GetWidth(), 
		pXiyuanChar1->GetHeight(), 
		SWP_NOSIZE | SWP_NOMOVE);

	while (!msgQueue.empty())
	{
		msgQueue.pop();
	}
	return 0;
}


void CToastDlg::OnDestroy()
{
	if (pXiyuanChar1)
	{
		delete pXiyuanChar1;
		pXiyuanChar1 = NULL;
	}
	if (pXiyuanChar2)
	{
		delete pXiyuanChar2;
		pXiyuanChar2 = NULL;
	}
	KillTimer(1);
	CDialogEx::OnDestroy();

	// TODO:  在此处添加消息处理程序代码
}


void CToastDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1)
	{
		m_blinkState++;
		if (m_blinkState % 8 == 0)
		{
			UpdateDisplay(pXiyuanChar2);
		}
		if (m_blinkState % 9 == 0)
		{
			UpdateDisplay(pXiyuanChar1);
		}
		if (m_blinkState > 10)
			m_blinkState = 0;
	}
	if (nIDEvent == 2)
	{
		OnLButtonUp(0, CPoint(0, 0));
		KillTimer(2);
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CToastDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	UpdateDisplay(pXiyuanChar1);
	SetTimer(1, 100, NULL);

	RECT rc;
	SystemParametersInfo(SPI_GETWORKAREA, 0, (PVOID)&rc, 0);
	MoveWindow(rc.right - pXiyuanChar1->GetWidth(),
		rc.bottom - pXiyuanChar1->GetHeight(),
		pXiyuanChar1->GetWidth(),
		pXiyuanChar1->GetHeight());
}

void CToastDlg::AddToastMsg(LPCTSTR msg, ULONG duringTime, BOOL canClose)
{
	ToastMsgParams msgParams = { msg, duringTime, canClose };
	msgQueue.push(msgParams);
}


void CToastDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	KillTimer(2);
		msgQueue.pop();
	// 检查Toast消息队列，如果队列为空，则关闭窗口，否则显示下一条消息
	if (msgQueue.size())
	{
		if (msgQueue.front().duringTime > 0)
			SetTimer(2, msgQueue.front().duringTime, NULL);
		UpdateDisplay(pXiyuanChar1);
	}
	else{
		SendMessage(WM_CLOSE, 0, 0);
	}
	CDialogEx::OnLButtonUp(nFlags, point);
}

void CToastDlg::ShowToastMessage()
{
	if (msgQueue.size() == 1)
	{
		if (msgQueue.front().duringTime > 0)
			SetTimer(2, msgQueue.front().duringTime, NULL);
	}
	ShowWindow(SW_SHOW);
}
