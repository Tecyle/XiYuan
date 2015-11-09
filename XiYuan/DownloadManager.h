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
	CString srcUrl;			///< ��������
	CString dstPath;		///< ����Ŀ��λ��
	ULONG fileSize;			///< Ŀ���ļ���С
	DownloadStatus status;	///< ����״̬
	ULONG fileDowned;		///< �����ص��ֽ���
	CString dstFileName;	///< �����ļ�����
	CString downTime;		///< ����ʱ��

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
	// Returns:   size_t ��ǰ�����б����������
	// Qualifier: �������ص�ַ���һ�����������Ѿ�ӵ�л�������񽫱��Զ�����
	// Parameter: CString srcUrl Ҫ���ص�URL
	// Parameter: BOOL autoStart �����֮���Ƿ��Զ�����
	//************************************
	size_t AddDownloadTask(CString srcUrl, BOOL autoStart = TRUE);
	//************************************
	// Method:    CancelNowDownloadTask
	// FullName:  CDownloadManager::CancelNowDownloadTask
	// Access:    public 
	// Returns:   BOOL ����ȡ���Ƿ�ɹ������ɹ�һ�������ڻ���ɾ��ʧ�ܻ�δ�������������
	// Qualifier: ȡ����ǰ��������
	// Parameter: void
	//************************************
	BOOL CancelNowDownloadTask(void);
	//************************************
	// Method:    CancelDownloadWaitingTask
	// FullName:  CDownloadManager::CancelDownloadWaitingTask
	// Access:    public 
	// Returns:   BOOL ����ȡ���Ƿ�ɹ���ʧ��һ���������±�Խ�������
	// Qualifier: ȡ����û�б����ص����񣬸ú����޷�ȡ���������ص�����
	// Parameter: int index Ҫȡ��������±꣬��������������±�Ϊ 0��������ʾ��ĩβ��ʼ��ǰ����
	//************************************
	BOOL CancelDownloadWaitingTask(int index);
	//************************************
	// Method:    SuspendDownloadTask
	// FullName:  CDownloadManager::SuspendDownloadTask
	// Access:    public 
	// Returns:   BOOL ���ع��������Ƿ�ɹ����±�Խ�硢����״̬���Զ��ᵼ�·���ʧ��
	// Qualifier: ����һ����û���ص����񣬹�����ζ�Ÿ����񲻻ᱻ����
	// Parameter: int index Ҫ����������±꣬��������������±�Ϊ 0��������ʾ��ĩβ��ʼ��ǰ����
	//************************************
	BOOL SuspendDownloadTask(int index);
	//************************************
	// Method:    MoveTaskForward
	// FullName:  CDownloadManager::MoveTaskForward
	// Access:    public 
	// Returns:   BOOL ������ǰ�����Ƿ�ɹ�ִ�У����е�����ǰʧ�ܵĲ������᷵��FALSE
	// Qualifier: ��ָ������ǰ��
	// Parameter: int index Ҫǰ��������±꣬��������������±�Ϊ 0��������ʾ��ĩβ��ʼ��ǰ����
	//************************************
	BOOL MoveTaskForward(int index);
	//************************************
	// Method:    MoveTaskBackward
	// FullName:  CDownloadManager::MoveTaskBackward
	// Access:    public 
	// Returns:   BOOL ���غ��Ʋ����Ƿ�ɹ�ִ�У����е��º���ʧ�ܵĲ������᷵��FALSE
	// Qualifier: ���ƶ��������
	// Parameter: int index Ҫ���Ƶ������±꣬��������������±�Ϊ 0��������ʾ��ĩβ������ǰ����
	//************************************
	BOOL MoveTaskBackward(int index);
	//************************************
	// Method:    MoveTaskTop
	// FullName:  CDownloadManager::MoveTaskTop
	// Access:    public 
	// Returns:   BOOL ���ز����Ƿ�ɹ������˾����������ֵ���岻��
	// Qualifier: ��ѡ������ͬ��״̬�������ö�
	// Parameter: int index �±꣬�㶮��
	//************************************
	BOOL MoveTaskTop(int index);
	//************************************
	// Method:    MoveTaskBottom
	// FullName:  CDownloadManager::MoveTaskBottom
	// Access:    public 
	// Returns:   BOOL ���ز����Ƿ�ɹ������˾����������ֵ���岻��
	// Qualifier: ��ѡ������ͬ��״̬�������õ�
	// Parameter: int index �±꣬�㶮��
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
	* ���� fileName ��ָ�����ļ�����������Ӧ·�����ݹ鴴���������Ѵ��ڵ�Ŀ¼
	*/
	BOOL CreateMovieDirectory(CString fileName);
	
	friend UINT DownloadMovieThread(LPVOID lpParams);
};

