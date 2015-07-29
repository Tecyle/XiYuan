
#pragma once

class CVodCtrl;

// ���ؽ��Ȼص�
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

// CVodCtrl ����Ŀ��
// �̳� CCmdTarget ��Ϊ�����Զ�������� JavaScript �Ľ���
class CVodCtrl : public CCmdTarget
{
	DECLARE_DYNAMIC(CVodCtrl)

protected:
	CInternetSession session;			// HTTP �Ự
	CString userName;					// ��¼�˻���
	CString passWord;					// ��¼����
	CString cacheDirectory;				// ������Ƶ�Ļ���Ŀ¼��ĩβ���� '\\'
	map<DWORD, CDownloadProgressCtrl> downloadTaskList;		// �߳�������Դ�б�

public:
	CVodCtrl();
	virtual ~CVodCtrl();

	virtual void OnFinalRelease();

protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	// ���� urlid ץȡ��Ӱ�����ص�ַ���ڼ�᳢�Խ���һ���Զ���¼
	string FetchMovieURL(CString urlid);
	// ������ LoginMovieSite�������Զ���¼
	BOOL LoginMovieSite(string & strBuffer, CHttpConnection * pHttp);

public:

	friend UINT DownloadMovieThread(LPVOID lpParams);

	/**
	* ��鵱ǰҳ���Ƿ��ǵ�Ӱ�ּ�ѡ��ҳ�棬����ǣ������еķּ������滻�ɱ��ز��Ź���
	*/
	static BOOL CheckAndReplaceMovieHref(IHTMLDocument2 * doc);

	/**
	* �� url ����ָ�����Ƶ����ԭ����Ŀ¼�ṹ���ص�����Ŀ¼
	*/
	HRESULT DownloadMovie(CString url);

	/**
	* ���� fileName ��ָ�����ļ�����������Ӧ·�����ݹ鴴���������Ѵ��ڵ�Ŀ¼
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


