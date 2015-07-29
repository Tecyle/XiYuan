XAccessibleServer
// XiYuanStatusBar.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "XiYuan.h"
#include "XiYuanStatusBar.h"


// CXiYuanStatusBar

CXiYuanStatusBar::CXiYuanStatusBar()
{
}

CXiYuanStatusBar::~CXiYuanStatusBar()
{
}


// CXiYuanStatusBar ��Ա����
BEGIN_MESSAGE_MAP(CXiYuanStatusBar, CMFCRibbonStatusBar)
	ON_WM_CREATE()
	ON_WM_TIMER()
END_MESSAGE_MAP()


int CXiYuanStatusBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMFCRibbonStatusBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	AddElement(new CMFCRibbonStatusBarPane(1, L"����"), L"����");
	AddElement(new CMFCRibbonProgressBar(2, 130, 25), L"���ؽ���");
	AddSeparator();
	AddElement(new CMFCRibbonStatusBarPane(3, L"��������"), L"��ǰ�������ص�Ӱ�ļ�");
	AddElement(new CMFCRibbonProgressBar(4, 200, 25), L"���ؽ���");
	AddSeparator();

	GetElement(3)->SetVisible(FALSE);
	GetElement(4)->SetVisible(FALSE);

	return 0;
}

void CXiYuanStatusBar::SetNavigateStatus(LPCTSTR status)
{
	GetElement(0)->SetText(status);
	RecalcLayout();
	RedrawWindow();
}

void CXiYuanStatusBar::SetNavigateProgress(BOOL visible, BOOL marquee, int progress)
{
	CMFCRibbonProgressBar * pProgress = (CMFCRibbonProgressBar *)GetElement(1);
	pProgress->SetVisible(visible);
	pProgress->SetInfiniteMode(marquee);
	if(!marquee)
		pProgress->SetPos(progress);
	else
		SetTimer(1, 100, NULL);
	if (!visible)
		KillTimer(1);
	RecalcLayout();
	RedrawWindow();
}

void CXiYuanStatusBar::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1)
	{
		CMFCRibbonProgressBar * pProgress = (CMFCRibbonProgressBar *)GetElement(1);
		if (pProgress->GetPos() >= 100)
		{
			pProgress->SetPos(0);
		}
		else{
			pProgress->SetPos(pProgress->GetPos() + 10);
		}
	}

	CMFCRibbonStatusBar::OnTimer(nIDEvent);
}

void CXiYuanStatusBar::StartDownloadProgress()
{
	GetElement(3)->SetVisible(TRUE);
	CMFCRibbonProgressBar * pProgress = (CMFCRibbonProgressBar *)GetElement(4);
	pProgress->SetVisible(TRUE);
	pProgress->SetRange(0, 100);
	pProgress->SetInfiniteMode(FALSE);
	pProgress->SetPos(0);
	RecalcLayout();
	RedrawWindow();
}

void CXiYuanStatusBar::EndDownloadProgress()
{
	GetElement(3)->SetVisible(FALSE);
	GetElement(4)->SetVisible(FALSE);
	RecalcLayout();
	RedrawWindow();

}

void CXiYuanStatusBar::SetDownloadProgress(int progress)
{
	CMFCRibbonProgressBar * pProgress = (CMFCRibbonProgressBar *)GetElement(4);
	pProgress->SetPos(progress);
	RecalcLayout();
	RedrawWindow();
}
