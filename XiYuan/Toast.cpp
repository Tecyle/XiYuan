#include "stdafx.h"
#include "Toast.h"


CToast::CToast()
{
	pToastDlg = NULL;
}


CToast::~CToast()
{
}

void CToast::ShowToast(LPCTSTR msg, ULONG autoCloseTime /*= 0*/)
{
	if (pToastDlg == NULL)
	{
		pToastDlg = new CToastDlg();
		pToastDlg->Create(IDD_TOAST_BOX);
	}
	pToastDlg->AddToastMsg(msg, autoCloseTime, TRUE);
	pToastDlg->ShowToastMessage();
}
