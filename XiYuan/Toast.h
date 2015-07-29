#pragma once
#include "ToastDlg.h"

class CToast
{
protected:
	CToastDlg * pToastDlg;

public:
	CToast();
	~CToast();

	void ShowToast(LPCTSTR msg, ULONG autoCloseTime = 0);
};

