
#pragma once

class CVodCtrl;

// 下载进度回调
class CDownloadProgressCtrl : public IBindStatusCallback
{
public:
	virtual HRESULT STDMETHODCALLTYPE OnStartBinding(DWORD dwReserved, __RPC__in_opt IBinding *pib);
	virtual HRESULT STDMETHODCALLTYPE GetPriority(__RPC__out LONG *pnPriority);
	virtual HRESULT STDMETHODCALLTYPE OnLowResource(DWORD reserved);
	virtual HRESULT STDMETHODCALLTYPE OnProgress(ULONG ulProgress, ULONG ulProgressMax, ULONG ulStatusCode, __RPC__in_opt LPCWSTR szStatusText);
	virtual HRESULT STDMETHODCALLTYPE OnStopBinding(HRESULT hresult, __RPC__in_opt LPCWSTR szError);
	virtual HRESULT STDMETHODCALLTYPE GetBindInfo(DWORD *grfBINDF, BINDINFO *pbindinfo);
	virtual HRESULT STDMETHODCALLTYPE OnDataAvailable(DWORD grfBSCF, DWORD dwSize, FORMATETC *pformatetc, STGMEDIUM *pstgmed);
	virtual HRESULT STDMETHODCALLTYPE OnObjectAvailable(__RPC__in REFIID riid, __RPC__in_opt IUnknown *punk);
public:
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppvObject);
	virtual ULONG STDMETHODCALLTYPE AddRef(void);
	virtual ULONG STDMETHODCALLTYPE Release(void);

public:
	CString url;
	DWORD threadId;
	BOOL flagPlayed;
	CString localName;
	CVodCtrl * pVodCtrl;
	int lastProgress;
};

// CVodCtrl 命令目标
// 继承 CCmdTarget 是为了其自动化完成与 JavaScript 的交互
class CVodCtrl : public CCmdTarget
{
	DECLARE_DYNAMIC(CVodCtrl)

protected:
	CInternetSession session;			// HTTP 会话
	CString userName;					// 登录账户名
	CString passWord;					// 登录密码
	CString cacheDirectory;				// 下载视频的缓冲目录，末尾不带 '\\'
	map<DWORD, CDownloadProgressCtrl> downloadTaskList;		// 线程下载资源列表

public:
	CVodCtrl();
	virtual ~CVodCtrl();

	virtual void OnFinalRelease();

protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	// 根据 urlid 抓取电影的下载地址，期间会尝试进行一次自动登录
	string FetchMovieURL(CString urlid);
	// 服务于 LoginMovieSite，进行自动登录
	BOOL LoginMovieSite(string & strBuffer, CHttpConnection * pHttp);

public:

	friend UINT DownloadMovieThread(LPVOID lpParams);

	/**
	* 检查当前页面是否是电影分集选择页面，如果是，将其中的分集链接替换成本地播放功能
	*/
	static BOOL CheckAndReplaceMovieHref(IHTMLDocument2 * doc);

	/**
	* 将 url 中所指向的视频按照原来的目录结构下载到缓冲目录
	*/
	HRESULT DownloadMovie(CString url);

	/**
	* 根据 fileName 中指定的文件名来创建对应路径，递归创建，跳过已存在的目录
	*/
	BOOL CreateMovieDirectory(CString fileName);
	void OnDownloadError(DWORD tId, HRESULT hr);
	void OnDownloadCompleted(DWORD tId);
	void OnReadyToPlay(DWORD threadId, CString localName);
	CString GetMediaPlayerLocation();
	CString GetMediaPlayerParams(CString localName);

	void VodDown2Play(BSTR urlid);
	void VodDownloadAll(BSTR movieId);
};


