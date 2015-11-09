// DownMgrDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "XiYuan.h"
#include "DownMgrDlg.h"
#include "afxdialogex.h"


// CDownMgrDlg �Ի���

IMPLEMENT_DYNAMIC(CDownMgrDlg, CDialogEx)

CDownMgrDlg::CDownMgrDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDownMgrDlg::IDD, pParent)
{
	blinkCnt = 0;
	pDownProgress = NULL;
}

CDownMgrDlg::~CDownMgrDlg()
{
}

void CDownMgrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDownMgrDlg, CDialogEx)
	ON_WM_TIMER()
	ON_WM_CLOSE()
//	ON_WM_PAINT()
END_MESSAGE_MAP()


// CDownMgrDlg ��Ϣ�������


BOOL CDownMgrDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

// 	CListCtrl * pList = (CListCtrl *)GetDlgItem(IDC_LIST1);
// 	ASSERT(pList);
// 	// ����������
// 	CRect rtEmpty;
// 	rtEmpty.SetRectEmpty();
// 	pDownProgress = new CProgressCtrl();
// 	pDownProgress->Create(WS_CHILD, rtEmpty, pList, 1);
// 	// ���������б�
// 	pList->SetView(LVS_REPORT);
// 	DWORD dwStyle = pList->GetExtendedStyle();
// 	dwStyle |= LVS_EX_FULLROWSELECT;			//ѡ��ĳ��ʹ���и�����ֻ������report����listctrl��  
// 	dwStyle |= LVS_EX_GRIDLINES;				//�����ߣ�ֻ������report����listctrl��
// 	dwStyle |= LVCFMT_IMAGE;
// 	pList->SetExtendedStyle(dwStyle);
// 	// ����б�ͷ
// 	pList->InsertColumn(0, L"��������", LVCFMT_LEFT, 180);
// 	pList->InsertColumn(1, L"�ļ���С", LVCFMT_LEFT, 80);
// 	pList->InsertColumn(2, L"����״̬", LVCFMT_LEFT, 150);
// 	pList->InsertColumn(3, L"���ʱ��", LVCFMT_LEFT, 150);
// 	pList->InsertColumn(4, L"����", LVCFMT_LEFT, 120);

	pList = new CSingleDownListCtrl();
	pList->Create(WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | WS_HSCROLL, CRect(10, 40, 645, 470), this, IDC_LIST1);

	pList->SetTextBkColor(CLR_NONE);
	pList->SetBkImage(LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_XIYUAN_LIST_BK1)));
	pList->UpdateDownList();

	theDownloadManager.AddProgressCallback(pList);
	//SetTimer(1, 250, NULL);
	//pList->SetBkImage(L"E:\\Workspace\\C++ MFC Workspace\\XiYuan\\XiYuan\\res\\xiyuan_char1.png");

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}

void CDownMgrDlg::UpdateDownList()
{

}

void CDownMgrDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1)
	{
		// �����ñ���ͼ�е�����գ��
		blinkCnt++;
		if (blinkCnt % 8 == 0)
		{
			pList->SetBkImage(LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_XIYUAN_LIST_BK2)));
		}
		if (blinkCnt % 9 == 0)
		{
			pList->SetBkImage(LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_XIYUAN_LIST_BK1)));
			blinkCnt = 0;
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CDownMgrDlg::OnClose()
{
	KillTimer(1);
	theDownloadManager.RemoveProgressCallback(pList);
	pList->DestroyWindow();
	delete pList;

	CDialogEx::OnClose();
}


//void CDownMgrDlg::OnPaint()
//{
//	if (!pDownProgress)
//		return;
//	CPaintDC dc(this); // device context for painting
//	assert(pList);
//	if (pList->GetItemCount() == 0)
//	{
//		pDownProgress->ShowWindow(SW_HIDE);
//		return;
//	}
//	assert(pDownProgress);
//	// 	if (pList->GetTopIndex() > 0)
//	// 	{
//	// 		pDownProgress->ShowWindow(SW_HIDE);
//	// 		return;
//	// 	}
//	// 	if (theDownloadManager.downList.front().status != DOWNLOAD_DOWNING)
//	// 	{
//	// 		pDownProgress->ShowWindow(SW_HIDE);
//	// 		return;
//	// 	}
//	// ���ϻ�������
//	CRect rtHead, rtItem;
//	pList->GetHeaderCtrl()->GetItemRect(2, rtHead);
//	pList->GetSubItemRect(0, 2, LVIR_LABEL, rtItem);
//	pDownProgress->SetWindowPos(pList, rtItem.left, rtItem.top, rtItem.Width(), rtItem.Height(), SWP_NOZORDER);
//	pDownProgress->ShowWindow(SW_SHOW);
//
//	CDialogEx::OnPaint();
//}
