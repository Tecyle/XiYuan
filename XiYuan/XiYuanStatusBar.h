#pragma once

// CXiYuanStatusBar ÃüÁîÄ¿±ê

class CXiYuanStatusBar : public CMFCRibbonStatusBar
{
public:
	CXiYuanStatusBar();
	virtual ~CXiYuanStatusBar();
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

public:
	void SetNavigateStatus(LPCTSTR status);
	void SetNavigateProgress(BOOL visible, BOOL marquee, int progress);
	void SetDownloadProgress(int progress);
	void StartDownloadProgress();
	void EndDownloadProgress();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};


