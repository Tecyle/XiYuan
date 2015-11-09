// SingleDownListCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "XiYuan.h"
#include "SingleDownListCtrl.h"


// CSingleDownListCtrl

IMPLEMENT_DYNAMIC(CSingleDownListCtrl, CListCtrl)

CSingleDownListCtrl::CSingleDownListCtrl()
{

}

CSingleDownListCtrl::~CSingleDownListCtrl()
{
}


BEGIN_MESSAGE_MAP(CSingleDownListCtrl, CListCtrl)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_MESSAGE(WM_MOVIE_DOWNLOAD_START, &OnMovieDownStart)
	ON_MESSAGE(WM_MOVIE_DOWNLOAD_PROGRESS, &OnMovieDownProgress)
	ON_MESSAGE(WM_MOVIE_DOWNLOAD_END, &OnMovieDownEnd)
END_MESSAGE_MAP()



// CSingleDownListCtrl 消息处理程序




int CSingleDownListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	// 自动化创建需要的列头
	SetView(LVS_REPORT);
	DWORD dwStyle = GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;			//选中某行使整行高亮（只适用与report风格的listctrl）  
	dwStyle |= LVS_EX_GRIDLINES;				//网格线（只适用与report风格的listctrl）
	//dwStyle |= LVS_OWNERDRAWFIXED;
	dwStyle |= LVCFMT_IMAGE;
	SetExtendedStyle(dwStyle);
	// 添加列表头
	InsertColumn(0, L"下载任务", LVCFMT_LEFT, 180);
	InsertColumn(1, L"文件大小", LVCFMT_LEFT, 80);
	InsertColumn(2, L"下载状态", LVCFMT_LEFT, 150);
	InsertColumn(3, L"完成时间", LVCFMT_LEFT, 150);
	InsertColumn(4, L"操作", LVCFMT_LEFT, 120);

	pDownProgress = new CProgressCtrl();
	pDownProgress->Create(NULL, CRect(0, 0, 0, 0), this, 2);

	return 0;
}


// void CSingleDownListCtrl::DrawItem(LPDRAWITEMSTRUCT lpDIS)
// {
// 	CListCtrl::DrawItem(lpDIS);
// // 	int nItem = lpDIS->itemID;
// // 	if (nItem == -1)
// // 		return;
// // 	CRect rcCol = lpDIS->rcItem;
// // 	CString sText;
// // 
// // 	CDC* pDC = CDC::FromHandle(lpDIS->hDC);
// // 	int nOldDCMode = pDC->SaveDC();
// // 
// // 	LVITEM item;
// // 	item.iItem = nItem;
// // 	item.iSubItem = 0;
// // 	item.mask = LVIF_IMAGE | LVIF_STATE;
// // 	item.stateMask = 0XFFFF;
// // 	GetItem(&item);
// // 	BOOL bSelected = item.state & LVIS_SELECTED;
// // 
// // 	COLORREF color = ::GetSysColor(COLOR_WINDOW);
// // 	if (bSelected)
// // 	{
// // 		pDC->SetBkColor(::GetSysColor(COLOR_HIGHLIGHT));
// // 		pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
// // 		color = ::GetSysColor(COLOR_HIGHLIGHT);
// // 	}
// // 	else
// // 	{
// // 		pDC->SetBkColor(CLR_NONE);
// // 		pDC->SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
// // 	}
// // 
// // 	LV_COLUMN lvc;
// // 	lvc.mask = LVCF_FMT | LVCF_WIDTH;
// // 
// // 	rcCol.right = rcCol.left;
// // 	for (int nCol = 0; GetColumn(nCol, &lvc); nCol++)
// // 	{
// // 		rcCol.left = rcCol.right;
// // 		rcCol.right = rcCol.left + GetColumnWidth(nCol);
// // 		HPEN hOldPen = (HPEN)::SelectObject(lpDIS->hDC, ::CreatePen(PS_SOLID, 1, RGB(0xc0, 0xc0, 0xc0)));
// // 		HBRUSH hOldBrush = (HBRUSH)::SelectObject(lpDIS->hDC, ::CreateSolidBrush(color));
// // 		::Rectangle(lpDIS->hDC, rcCol.left - 1, rcCol.top - 1, rcCol.right, rcCol.bottom);
// // 		::DeleteObject(SelectObject(lpDIS->hDC, hOldBrush));
// // 		::DeleteObject(SelectObject(lpDIS->hDC, hOldPen));
// // 
// // 		sText = GetItemText(nItem, nCol);
// // 		pDC->DrawText(sText, -1, CRect::CRect(rcCol.left + 3, rcCol.top, rcCol.right, rcCol.bottom - 2), DT_LEFT);
// // 	}
// // 	pDC->RestoreDC(nOldDCMode);
// }


void CSingleDownListCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CWnd::DefWindowProc(WM_PAINT, WPARAM(dc.m_hDC), 0);
	// TODO:  在此处添加消息处理程序代码
	// 不为绘图消息调用 CListCtrl::OnPaint()
	if (!pDownProgress)
		return;
	if (GetItemCount() == 0)
	{
		pDownProgress->ShowWindow(SW_HIDE);
		return;
	}
	assert(pDownProgress);
	if (GetTopIndex() > 0)
	{
		pDownProgress->ShowWindow(SW_HIDE);
		return;
	}
	if (theDownloadManager.downList.size() == 0)
	{
		pDownProgress->ShowWindow(SW_HIDE);
		return;
	}
	if (theDownloadManager.downList.front().status != DOWNLOAD_DOWNING)
	{
		pDownProgress->ShowWindow(SW_HIDE);
		return;
	}
	// 符合绘制条件
	CRect rtHead, rtItem;
	GetHeaderCtrl()->GetItemRect(2, rtHead);
	GetSubItemRect(0, 2, LVIR_LABEL, rtItem);
	pDownProgress->SetWindowPos(this, rtItem.left, rtItem.top, rtItem.Width(), rtItem.Height(), SWP_NOZORDER);
	pDownProgress->ShowWindow(SW_SHOW);

}


BOOL CSingleDownListCtrl::OnEraseBkgnd(CDC* pDC)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	return CListCtrl::OnEraseBkgnd(pDC);
}

void CSingleDownListCtrl::UpdateDownList()
{
	DeleteAllItems();
	for (size_t i = 0; i < theDownloadManager.downList.size(); i++)
	{
		DownloadTaskParams params = theDownloadManager.downList.at(i);
		InsertItem(GetItemCount(), params.dstFileName);
		SetItemText(GetItemCount() - 1, 1, GetFileSizeString(params.fileSize));
		SetItemText(GetItemCount() - 1, 2, GetDownloadStatusString(params.status));
		SetItemText(GetItemCount() - 1, 3, params.downTime);
	}
	for (size_t i = 0; i < theDownloadManager.finishList.size(); i++)
	{
		DownloadTaskParams params = theDownloadManager.finishList.at(i);
		InsertItem(GetItemCount(), params.dstFileName);
		SetItemText(GetItemCount() - 1, 1, GetFileSizeString(params.fileSize));
		SetItemText(GetItemCount() - 1, 2, GetDownloadStatusString(params.status));
		SetItemText(GetItemCount() - 1, 3, params.downTime);
	}
}

void CSingleDownListCtrl::OnDownloadStart(DownloadTaskParams * params)
{
	if (!pDownProgress)
		return;

	PostMessage(WM_MOVIE_DOWNLOAD_START);
}

void CSingleDownListCtrl::OnDownloadProgress(DownloadTaskParams * params)
{
	if (!pDownProgress)
		return;

	if (params->fileSize == 0)
		return;
	int progressPercent = (int)((double)params->fileDowned * 100.0 / (double)params->fileSize);
	if (progressPercent != lastProgress)
	{
		PostMessage(WM_MOVIE_DOWNLOAD_PROGRESS, progressPercent, 0L);
		lastProgress = progressPercent;
	}
}

void CSingleDownListCtrl::OnDownloadError(DownloadTaskParams * params, HRESULT hr)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void CSingleDownListCtrl::OnDownloadCompleted(DownloadTaskParams * params)
{
	if (!pDownProgress)
		return;

	PostMessage(WM_MOVIE_DOWNLOAD_END);
}

LRESULT CSingleDownListCtrl::OnMovieDownStart(WPARAM wParam, LPARAM lParam)
{
	UpdateDownList();
	pDownProgress->SetPos(0);
	pDownProgress->SetRange(0, 100);
	RedrawWindow();
	return 0;
}

LRESULT CSingleDownListCtrl::OnMovieDownProgress(WPARAM wParam, LPARAM lParam)
{
	if (GetItemText(0, 1) == L"0 B" && theDownloadManager.downList.front().fileSize > 0)
	{
		SetItemText(0, 1, GetFileSizeString(theDownloadManager.downList.front().fileSize));
	}
	pDownProgress->SetPos(wParam);
	return 0;
}

LRESULT CSingleDownListCtrl::OnMovieDownEnd(WPARAM wParam, LPARAM lParam)
{
	UpdateDownList();
	RedrawWindow();
	return 0;
}
