#pragma once


// CSingleDownListCtrl

class CSingleDownListCtrl : public CListCtrl, public IDownloadProgress
{
	DECLARE_DYNAMIC(CSingleDownListCtrl)

public:
	CSingleDownListCtrl();
	virtual ~CSingleDownListCtrl();
	CProgressCtrl * pDownProgress;
	int lastProgress;
protected:
	DECLARE_MESSAGE_MAP()

	afx_msg LRESULT OnMovieDownStart(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMovieDownProgress(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMovieDownEnd(WPARAM wParam, LPARAM lParam);
public:
	void UpdateDownList();

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
/*	virtual void DrawItem(LPDRAWITEMSTRUCT / *lpDrawItemStruct* /);*/
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	virtual void OnDownloadStart(DownloadTaskParams * params);
	virtual void OnDownloadProgress(DownloadTaskParams * params);
	virtual void OnDownloadError(DownloadTaskParams * params, HRESULT hr);
	virtual void OnDownloadCompleted(DownloadTaskParams * params);

};


