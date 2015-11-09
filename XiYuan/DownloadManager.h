#pragma once

class CDownloadManager;

typedef enum tagDownloadStatus
{
	DOWNLOAD_COMPLETED,
	DOWNLOAD_SUSPEND,
	DOWNLOAD_DOWNING,
	DOWNLOAD_WAITING,
	DOWNLOAD_ERROR
}	DownloadStatus;

CString GetDownloadStatusString(DownloadStatus status);

typedef struct  tagDownloadTaskParams
{
	CString srcUrl;			///< 下载链接
	CString dstPath;		///< 下载目标位置
	ULONG fileSize;			///< 目标文件大小
	DownloadStatus status;	///< 下载状态
	ULONG fileDowned;		///< 已下载的字节数
	CString dstFileName;	///< 下载文件名字
	CString downTime;		///< 下载时间

	CString toString();
	BOOL fromString(LPCTSTR str);
}	DownloadTaskParams;

class CDownloadStatusCtrl : public IBindStatusCallback
{
	// IBindStatusCallback
public:
	virtual HRESULT STDMETHODCALLTYPE OnStartBinding(DWORD dwReserved, __RPC__in_opt IBinding *pib);
	virtual HRESULT STDMETHODCALLTYPE GetPriority(__RPC__out LONG *pnPriority);
	virtual HRESULT STDMETHODCALLTYPE OnLowResource(DWORD reserved);
	virtual HRESULT STDMETHODCALLTYPE OnProgress(ULONG ulProgress, ULONG ulProgressMax, ULONG ulStatusCode, __RPC__in_opt LPCWSTR szStatusText);
	virtual HRESULT STDMETHODCALLTYPE OnStopBinding(HRESULT hresult, __RPC__in_opt LPCWSTR szError);
	virtual HRESULT STDMETHODCALLTYPE GetBindInfo(DWORD *grfBINDF, BINDINFO *pbindinfo);
	virtual HRESULT STDMETHODCALLTYPE OnDataAvailable(DWORD grfBSCF, DWORD dwSize, FORMATETC *pformatetc, STGMEDIUM *pstgmed);
	virtual HRESULT STDMETHODCALLTYPE OnObjectAvailable(__RPC__in REFIID riid, __RPC__in_opt IUnknown *punk);
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppvObject);
	// IUnkown
public:
	virtual ULONG STDMETHODCALLTYPE AddRef(void);
	virtual ULONG STDMETHODCALLTYPE Release(void);
	// Properties
public:
	CDownloadManager * pDownMgr;
};

class IDownloadProgress
{
public:
	virtual void OnDownloadStart(DownloadTaskParams * params) = 0;
	virtual void OnDownloadProgress(DownloadTaskParams * params) = 0;
	virtual void OnDownloadError(DownloadTaskParams * params, HRESULT hr) = 0;
	virtual void OnDownloadCompleted(DownloadTaskParams * params) = 0;
};

class CDownloadManager
{
public:
	deque<DownloadTaskParams> downList;
	deque<DownloadTaskParams> finishList;

protected:
	CString cacheDirectory;
	CWinThread * downThread;
	CDownloadStatusCtrl * downProgress;
	vector<IDownloadProgress *> downProgressCallback;

public:
	CDownloadManager();
	~CDownloadManager();

	friend class CDownloadStatusCtrl;

	//************************************
	// Method:    AddDownloadTask
	// FullName:  CDownloadManager::AddDownloadTask
	// Access:    public 
	// Returns:   size_t 当前下载列表的任务数量
	// Qualifier: 根据下载地址添加一个下载任务，已经拥有缓存的任务将被自动忽略
	// Parameter: CString srcUrl 要下载的URL
	// Parameter: BOOL autoStart 添加完之后是否自动下载
	//************************************
	size_t AddDownloadTask(CString srcUrl, BOOL autoStart = TRUE);
	//************************************
	// Method:    CancelNowDownloadTask
	// FullName:  CDownloadManager::CancelNowDownloadTask
	// Access:    public 
	// Returns:   BOOL 返回取消是否成功，不成功一般是由于缓存删除失败或未进行下载引起的
	// Qualifier: 取消当前下载任务
	// Parameter: void
	//************************************
	BOOL CancelNowDownloadTask(void);
	//************************************
	// Method:    CancelDownloadWaitingTask
	// FullName:  CDownloadManager::CancelDownloadWaitingTask
	// Access:    public 
	// Returns:   BOOL 返回取消是否成功，失败一般是由于下标越界引起的
	// Qualifier: 取消还没有被下载的任务，该函数无法取消正在下载的任务
	// Parameter: int index 要取消任务的下标，正在下载任务的下标为 0，负数表示从末尾开始往前计数
	//************************************
	BOOL CancelDownloadWaitingTask(int index);
	//************************************
	// Method:    SuspendDownloadTask
	// FullName:  CDownloadManager::SuspendDownloadTask
	// Access:    public 
	// Returns:   BOOL 返回挂起任务是否成功，下标越界、任务状态不对都会导致返回失败
	// Qualifier: 挂起一个还没下载的任务，挂起意味着该任务不会被下载
	// Parameter: int index 要挂起任务的下标，正在下载任务的下标为 0，负数表示从末尾开始往前计数
	//************************************
	BOOL SuspendDownloadTask(int index);
	//************************************
	// Method:    MoveTaskForward
	// FullName:  CDownloadManager::MoveTaskForward
	// Access:    public 
	// Returns:   BOOL 返回移前操作是否成功执行，所有导致移前失败的操作都会返回FALSE
	// Qualifier: 将指定任务前移
	// Parameter: int index 要前移任务的下标，正在下载任务的下标为 0，负数表示从末尾开始往前计数
	//************************************
	BOOL MoveTaskForward(int index);
	//************************************
	// Method:    MoveTaskBackward
	// FullName:  CDownloadManager::MoveTaskBackward
	// Access:    public 
	// Returns:   BOOL 返回后移操作是否成功执行，所有导致后移失败的操作都会返回FALSE
	// Qualifier: 将制定任务后移
	// Parameter: int index 要后移的任务下标，正在下载任务的下标为 0，负数表示从末尾考试往前计数
	//************************************
	BOOL MoveTaskBackward(int index);
	//************************************
	// Method:    MoveTaskTop
	// FullName:  CDownloadManager::MoveTaskTop
	// Access:    public 
	// Returns:   BOOL 返回操作是否成功，个人觉得这个返回值意义不大
	// Qualifier: 将选定项在同类状态任务中置顶
	// Parameter: int index 下标，你懂的
	//************************************
	BOOL MoveTaskTop(int index);
	//************************************
	// Method:    MoveTaskBottom
	// FullName:  CDownloadManager::MoveTaskBottom
	// Access:    public 
	// Returns:   BOOL 返回操作是否成功，个人觉得这个返回值意义不大
	// Qualifier: 将选定项在同类状态任务中置底
	// Parameter: int index 下标，你懂的
	//************************************
	BOOL MoveTaskBottom(int index);

	BOOL StartDownloadMission();

	void SaveDownloadList();
	void LoadDownloadList();

	void OnDownloadStart();
	void OnDownloadCompleted();
	void OnDownloadFailed(HRESULT hr);
	void OnDownloadProgress();

	int AddProgressCallback(IDownloadProgress * callbackObj);
	int RemoveProgressCallback(IDownloadProgress * removeOne);

	/**
	* 根据 fileName 中指定的文件名来创建对应路径，递归创建，跳过已存在的目录
	*/
	BOOL CreateMovieDirectory(CString fileName);
	
	friend UINT DownloadMovieThread(LPVOID lpParams);
};

