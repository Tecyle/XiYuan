#include "stdafx.h"
#include "DownloadManager.h"

UINT DownloadMovieThread(LPVOID lpParams)
{
	CDownloadManager * downMgr = (CDownloadManager *)lpParams;
	ASSERT(downMgr);
	// ��ȡ������Ϣ
	DownloadTaskParams params = downMgr->downList.front();
	ENSURE(params.status == DOWNLOAD_DOWNING);
	// ֪ͨ���ع�������������ʼ
	downMgr->OnDownloadStart();
	if (downMgr->downProgress)
		delete downMgr->downProgress;
	downMgr->downProgress = new CDownloadStatusCtrl();
	downMgr->downProgress->pDownMgr = downMgr;
	HRESULT hr = URLDownloadToFile(NULL, params.srcUrl, params.dstPath, 0, downMgr->downProgress);
	if (SUCCEEDED(hr))
	{
		downMgr->OnDownloadCompleted();
	}
	else{
		downMgr->OnDownloadFailed(hr);
	}
	return 0;
}

CString GetDownloadStatusString(DownloadStatus status)
{
	switch (status)
	{
	case DOWNLOAD_COMPLETED:
		return L"�������";
	case DOWNLOAD_SUSPEND:
		return L"�ѹ���";
	case DOWNLOAD_DOWNING:
		return L"��������";
	case DOWNLOAD_WAITING:
		return L"�Ŷ���";
	case DOWNLOAD_ERROR:
		return L"����ʧ��";
	}
	return L"��...ϧԵ��֪���ù���ʱ���״̬��ʲô��...";
}

//////////////////////////////////////////////////////////////////////////

CDownloadManager::CDownloadManager()
{
	cacheDirectory = L"F:\\movies";
	downProgress = NULL;
	downThread = NULL;
}


CDownloadManager::~CDownloadManager()
{
}

BOOL CDownloadManager::CreateMovieDirectory(CString fileName)
{
	// ��֧�־���·��
	if (fileName.Find(L':') == -1)
		return FALSE;
	int iStart = 0;
	while ((iStart = fileName.Find(L'\\', iStart)) != -1)
	{
		iStart++;
		CString str = fileName.Left(iStart);
		if (!PathFileExists(str))
		{
			if (!CreateDirectory(str, NULL))
				return FALSE;
		}
	}
	return TRUE;
}

size_t CDownloadManager::AddDownloadTask(CString srcUrl, BOOL autoStart /*= TRUE*/)
{
	DownloadTaskParams params;
	params.srcUrl = srcUrl;
	// �����ļ��洢λ��
	CString downPath = srcUrl;
	downPath.Replace(L"http://", L"");
	downPath = downPath.Right(downPath.GetLength() - downPath.Find(L'/'));
	downPath.Replace(L"/", L"\\");
	downPath = cacheDirectory + downPath;
	if (PathFileExists(downPath))
	{
		// �����ļ��Ѿ����ڣ���ô����Ϊ��������Ѿ������ع��ˣ������ظ����
		return downList.size();
	}
	CreateMovieDirectory(downPath);
	params.dstPath = downPath;
	params.fileSize = 0;
	params.status = DOWNLOAD_WAITING;
	// ���£�downPath ��û����
	downPath.SetAt(downPath.ReverseFind(L'\\'), L'?');
	downPath = downPath.Right(downPath.GetLength() - downPath.ReverseFind(L'\\') - 1);
	downPath.Replace(L"?", L"\\");
	params.dstFileName = downPath;
	// �ռ������Ҫ��������Ϣ���������ض���
	downList.push_back(params);
	if (autoStart)
		StartDownloadMission();
	return downList.size();
}

BOOL CDownloadManager::CancelNowDownloadTask(void)
{
	if (downThread == NULL)
		return FALSE;
	// ǿ����ֹ��ǰ�߳�
	CString downPath = downList.front().dstPath;
	DWORD exitCode;
	GetExitCodeThread(downThread->m_hThread, &exitCode);
	TerminateThread(downThread->m_hThread, exitCode);
	WaitForSingleObject(downThread->m_hThread, INFINITE);
	downThread = NULL;
	// ɾ�����ص�һ��Ļ���
	if (PathFileExists(downPath))
	{
		return DeleteFile(downPath);
	}
	return TRUE;
}

BOOL CDownloadManager::CancelDownloadWaitingTask(int index)
{
	// ��ȡ�������index
	if (index < 0)
	{
		index = downList.size() + index;
	}
	// �ж��±�Խ��
	if (index < 1 || index > downList.size() - 1)
	{
		TRACE1("CancelDownloadWaitingTask �����±�Խ�磬����ֵ��%d\n", index);
		return FALSE;
	}
	// ���б���ɾ��λ�� index �ط�������
	downList.erase(downList.begin() + index);
	return TRUE;
}

BOOL CDownloadManager::SuspendDownloadTask(int index)
{
	// ��ȡ�������index
	if (index < 0)
	{
		index = downList.size() + index;
	}
	// �ж��±�Խ��
	if (index < 1 || index > downList.size() - 1)
	{
		TRACE1("SuspendDownloadTask �����±�Խ�磬����ֵ��%d\n", index);
		return FALSE;
	}
	// ���� index ������Ϊ����״̬�����ƶ�������ĩβ
	DownloadTaskParams params = downList.at(index);
	if (params.status != DOWNLOAD_WAITING)
	{
		TRACE1("���� %d λ�ô�����������Ϊ����״̬ʱ����������û�д��ڵȴ�����״̬��\n", index);
		return FALSE;
	}
	params.status = DOWNLOAD_SUSPEND;
	downList.erase(downList.begin() + index);
	downList.push_back(params);
	return TRUE;
}

BOOL CDownloadManager::MoveTaskForward(int index)
{
	// ��ȡ�������index
	if (index < 0)
	{
		index = downList.size() + index;
	}
	// �ж��±�Խ��
	if (index < 1 || index > downList.size() - 1)
	{
		TRACE1("SuspendDownloadTask �����±�Խ�磬����ֵ��%d\n", index);
		return FALSE;
	}
	// �ж��Ƿ�Ԫ���Ƿ��ǿɽ���״̬
	if (index == 1 && downList.front().status == DOWNLOAD_DOWNING)
	{
		TRACE0("��������������ʱ���޷����ڶ�λ������ǰ��\n");
		return FALSE;
	}
	// �������ǰ�Ƶ�ǰ�������ǣ�ǰ�������͵�ǰ�����״̬һ��
	if (downList.at(index).status != downList.at(index - 1).status)
	{
		TRACE1("Ҫ��ǰ�������ǰһ�������״̬��һ�£��޷���ǰ�����Ϊ��%d\n", index);
		return FALSE;
	}
	// ʵ��������ǰ�������ǣ���ɾ�� index������ index - 1 ǰ���� index
	DownloadTaskParams params = downList.at(index);
	downList.erase(downList.begin() + index);
	downList.insert(downList.begin() + index - 1, params);
	return TRUE;
}

BOOL CDownloadManager::MoveTaskBackward(int index)
{
	// ��ȡ�������index
	if (index < 0)
	{
		index = downList.size() + index;
	}
	// �ж��±�Խ��
	if (index < 1 || index > downList.size() - 1)
	{
		TRACE1("SuspendDownloadTask �����±�Խ�磬����ֵ��%d\n", index);
		return FALSE;
	}
	// �ж��Ƿ���Լ������Ʋ���
	if (index == downList.size() - 1)
	{
		TRACE0("ѡ��������λ���б�ĩβ���޷��������ơ�\n");
		return FALSE;
	}
	// ������Ժ��Ƶ�ǰ�������ǣ����������͵�ǰ�����״̬һ��
	if (downList.at(index).status != downList.at(index + 1).status)
	{
		TRACE1("Ҫ���Ƶ�����ͺ�һ�������״̬��һ�£��޷����ơ����Ϊ��%d\n", index);
		return FALSE;
	}
	// ʵ��������ƣ������ǣ��Ȳ��� index + 1����ɾ�� index
	DownloadTaskParams params = downList.at(index);
	downList.insert(downList.begin() + index + 1, params);
	downList.erase(downList.begin() + index);
	return TRUE;
}

BOOL CDownloadManager::MoveTaskTop(int index)
{
	// ��ȡ�������index
	if (index < 0)
	{
		index = downList.size() + index;
	}
	// �ж��±�Խ��
	if (index < 1 || index > downList.size() - 1)
	{
		TRACE1("SuspendDownloadTask �����±�Խ�磬����ֵ��%d\n", index);
		return FALSE;
	}
	// �ö���Ϊͬ״̬�ö��������������ö����Ϊ��������Ķ���
	if (downList.at(index).status != downList.at(index - 1).status)
	{
		TRACE1("�޷�����ö��������������� %d �Ѿ�λ�ڶ��ˣ�\n", index);
		return FALSE;
	}
	// ��ǰ����ͬ״̬Ŀ��
	int iTop = index;
	while (iTop > 0)
	{
		if (downList.at(iTop - 1).status == downList.at(index).status)
		{
			MoveTaskForward(iTop--);
		}
		else{
			break;
		}
	}
	return TRUE;
}

BOOL CDownloadManager::MoveTaskBottom(int index)
{
	// ��ȡ�������index
	if (index < 0)
	{
		index = downList.size() + index;
	}
	// �ж��±�Խ��
	if (index < 1 || index > downList.size() - 1)
	{
		TRACE1("SuspendDownloadTask �����±�Խ�磬����ֵ��%d\n", index);
		return FALSE;
	}
	// �õ���Ϊͬ״̬�õף������������õ׽��Ϊ��������ĵ׶�
	if (index == downList.size() - 1)
	{
		TRACE0("��ǰѡ�����Ѿ�λ����׶ˡ�\n");
		return FALSE;
	}
	if (downList.at(index).status != downList.at(index + 1).status)
	{
		TRACE1("�޷�����ö��������������� %d �Ѿ�λ�ڶ��ˣ�\n", index);
		return FALSE;
	}
	// �������ͬ״̬Ŀ��
	int iBottom = index;
	while (iBottom < downList.size() - 1)
	{
		if (downList.at(iBottom + 1).status == downList.at(index).status)
		{
			MoveTaskBackward(iBottom++);
		}
		else{
			break;
		}
	}
	return TRUE;
}

BOOL CDownloadManager::StartDownloadMission()
{
	// �жϵ�ǰ�Ƿ���������
	if (downThread)
	{
		TRACE0("���Խ�������ʱ���֣��Ѿ��������������ˣ����ԾͲ������ˡ�\n");
		return FALSE;
	}
	// �жϵ�ǰ�Ƿ��������������
	if (!downList.size())
	{
		TRACE0("��Ȼû�ж������������ˣ����Ǻܲ����ġ�\n");
		return FALSE;
	}
	if (downList.front().status != DOWNLOAD_WAITING)
	{
		TRACE0("��Ȼû�ж������������ˣ����Ǻܲ����ġ�\n");
		return FALSE;
	}
	// �������״̬�������߳�����
	downList.front().status = DOWNLOAD_DOWNING;
	downThread = AfxBeginThread(DownloadMovieThread, this, 0, 0U, CREATE_SUSPENDED);
	downThread->ResumeThread();
	return TRUE;
}

void CDownloadManager::SaveDownloadList()
{
	setlocale(0, "chs");
	CStdioFile file;
	file.Open(L"downing.ini", CFile::modeWrite | CFile::modeCreate);
	for (size_t i = 0; i < downList.size(); i++)
	{
		file.WriteString(downList[i].toString());
	}
	file.Close();
	file.Open(L"downed.ini", CFile::modeWrite | CFile::modeCreate);
	for (size_t i = 0; i < finishList.size(); i++)
	{
		file.WriteString(finishList[i].toString());
	}
	file.Close();
}

void CDownloadManager::LoadDownloadList()
{
	setlocale(0, "chs");
	CStdioFile file;
	if (file.Open(L"downing.ini", CFile::modeRead))
	{
		CString sTp;
		CString content;
		while (file.ReadString(sTp))
		{
			if (sTp.GetLength())
			{
				if (sTp.Left(1) == L"[")
				{
					if (content.GetLength())
					{
						DownloadTaskParams params;
						if (params.fromString(content))
							downList.push_back(params);
					}
					content = sTp;
				}
				else{
					content += (content.GetLength() ? L"\n" : L"") + sTp;
				}
			}
		}
		// ������һ����¼
		DownloadTaskParams pas;
		if (pas.fromString(content))
			downList.push_back(pas);
		file.Close();
	}
	if (file.Open(L"downed.ini", CFile::modeRead))
	{
		CString sTp;
		CString content;
		while (file.ReadString(sTp))
		{
			if (sTp.GetLength())
			{
				if (sTp.Left(1) == L"[")
				{
					if (content.GetLength())
					{
						DownloadTaskParams params;
						if (params.fromString(content))
							finishList.push_back(params);
					}
					content = sTp;
				}
				else{
					content += (content.GetLength() ? L"\n" : L"") + sTp;
				}
			}
		}
		// ������һ����¼
		DownloadTaskParams pas;
		if (pas.fromString(content))
			finishList.push_back(pas);
		file.Close();
	}
	// �����쳣״̬�ж�
	if (downList.size())
	{
		if (downList.front().status == DOWNLOAD_DOWNING)
		{
			downList.front().status == DOWNLOAD_WAITING;
			if (PathFileExists(downList.front().dstPath))
			{
				// �˴�Ӧ����һ����ȫ��飬�鿴ɾ�����ļ����ͺ�λ��
				DeleteFile(downList.front().dstPath);
			}
		}
	}
}

void CDownloadManager::OnDownloadStart()
{
	ENSURE(downList.front().status == DOWNLOAD_DOWNING);
	if (downProgressCallback.size())
	{
		for (size_t i = 0; i < downProgressCallback.size(); i++)
		{
			downProgressCallback[i]->OnDownloadStart(&downList.front());
		}
	}
}

void CDownloadManager::OnDownloadCompleted()
{
	ENSURE(downList.front().status == DOWNLOAD_DOWNING);
	if (downProgressCallback.size())
	{
		for (size_t i = 0; i < downProgressCallback.size(); i++)
		{
			downProgressCallback[i]->OnDownloadCompleted(&downList.front());
		}
	}
	downThread = NULL;
	DownloadTaskParams params = downList.front();
	params.status = DOWNLOAD_COMPLETED;
	CTime st = CTime::GetCurrentTime();
	params.downTime = st.Format("%Y-%m-%d %H:%M:%S");
	downList.pop_front();
	finishList.push_front(params);
	if (downList.size())
		StartDownloadMission();
}

void CDownloadManager::OnDownloadFailed(HRESULT hr)
{
	ENSURE(downList.front().status == DOWNLOAD_DOWNING);
	if (downProgressCallback.size())
	{
		for (size_t i = 0; i < downProgressCallback.size(); i++)
		{
			downProgressCallback[i]->OnDownloadError(&downList.front(), hr);
		}
	}
	downThread = NULL;
	DownloadTaskParams params = downList.front();
	params.status = DOWNLOAD_ERROR;
	CTime st = CTime::GetCurrentTime();
	params.downTime = st.Format("%Y-%m-%d %H:%M:%S");
	downList.pop_front();
	finishList.push_front(params);
	if (downList.size())
		StartDownloadMission();
}

void CDownloadManager::OnDownloadProgress()
{
	ENSURE(downList.front().status == DOWNLOAD_DOWNING);
	if (downProgressCallback.size())
	{
		for (size_t i = 0; i < downProgressCallback.size(); i++)
		{
			downProgressCallback[i]->OnDownloadProgress(&downList.front());
		}
	}
}

int CDownloadManager::AddProgressCallback(IDownloadProgress * callbackObj)
{
	if (downProgressCallback.end() == find(downProgressCallback.begin(), downProgressCallback.end(), callbackObj))
	{
		downProgressCallback.push_back(callbackObj);
		return downProgressCallback.size();
	}
	return -1;
}

int CDownloadManager::RemoveProgressCallback(IDownloadProgress * removeOne)
{
	vector<IDownloadProgress *>::iterator it = find(downProgressCallback.begin(), downProgressCallback.end(), removeOne);
	if (it == downProgressCallback.end())
	{
		return -1;
	}
	downProgressCallback.erase(it);
	return 0;
}

//////////////////////////////////////////////////////////////////////////
 
HRESULT STDMETHODCALLTYPE CDownloadStatusCtrl::OnStartBinding(DWORD dwReserved, __RPC__in_opt IBinding *pib)
{
	UNREFERENCED_PARAMETER(dwReserved);
	UNREFERENCED_PARAMETER(pib);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CDownloadStatusCtrl::GetPriority(__RPC__out LONG *pnPriority)
{
	UNREFERENCED_PARAMETER(pnPriority);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CDownloadStatusCtrl::OnLowResource(DWORD reserved)
{
	UNREFERENCED_PARAMETER(reserved);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CDownloadStatusCtrl::OnProgress(ULONG ulProgress, ULONG ulProgressMax, ULONG ulStatusCode, __RPC__in_opt LPCWSTR szStatusText)
{
	ENSURE(pDownMgr->downList.front().status == DOWNLOAD_DOWNING);
	pDownMgr->downList.front().fileSize = ulProgressMax;
	pDownMgr->downList.front().fileDowned = ulProgress;
	pDownMgr->OnDownloadProgress();
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CDownloadStatusCtrl::OnStopBinding(HRESULT hresult, __RPC__in_opt LPCWSTR szError)
{
	UNREFERENCED_PARAMETER(hresult);
	UNREFERENCED_PARAMETER(szError);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CDownloadStatusCtrl::GetBindInfo(DWORD *grfBINDF, BINDINFO *pbindinfo)
{
	UNREFERENCED_PARAMETER(grfBINDF);
	UNREFERENCED_PARAMETER(pbindinfo);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CDownloadStatusCtrl::OnDataAvailable(DWORD grfBSCF, DWORD dwSize, FORMATETC *pformatetc, STGMEDIUM *pstgmed)
{
	UNREFERENCED_PARAMETER(grfBSCF);
	UNREFERENCED_PARAMETER(dwSize);
	UNREFERENCED_PARAMETER(pformatetc);
	UNREFERENCED_PARAMETER(pstgmed);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CDownloadStatusCtrl::OnObjectAvailable(__RPC__in REFIID riid, __RPC__in_opt IUnknown *punk)
{
	UNREFERENCED_PARAMETER(riid);
	UNREFERENCED_PARAMETER(punk);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CDownloadStatusCtrl::QueryInterface(REFIID riid, void **ppvObject)
{
	UNREFERENCED_PARAMETER(riid);
	UNREFERENCED_PARAMETER(ppvObject);
	return E_NOTIMPL;
}

ULONG STDMETHODCALLTYPE CDownloadStatusCtrl::AddRef(void)
{
	return 0;
}

ULONG STDMETHODCALLTYPE CDownloadStatusCtrl::Release(void)
{
	return 0;
}

CString tagDownloadTaskParams::toString()
{
	CString res = L"[XiYuanDownloadParams]\n";
	CString tp;
	res += L"url=" + srcUrl + L"\n";
	res += L"path=" + dstPath + L"\n";
	tp.Format(L"size=%d\n", fileSize);
	res += tp;
	tp.Format(L"status=%d\n", status);
	res += tp;
	res += L"title=" + dstFileName + L"\n";
	res += L"time=" + downTime + L"\n";
	return res;
}

BOOL tagDownloadTaskParams::fromString(LPCTSTR str)
{
	LPWSTR pszResult;
	LPWSTR s = _wcsdup(str);
	LPWSTR token;
	token = wcstok_s(s, L"\n", &pszResult);
	if (!token)
		return FALSE;
	if (wcscmp(token, L"[XiYuanDownloadParams]"))
	{
		free(s);
		return FALSE;
	}
	while (token = wcstok_s(NULL, L"\n", &pszResult))
	{
		CString dsToken = token;
		if (dsToken.Find(L"=") == -1)
			continue;
		CString key = dsToken.Left(dsToken.Find(L"="));
		CString value = dsToken.Right(dsToken.GetLength() - dsToken.Find(L"=") - 1);
		if (key == L"url")
			srcUrl = value;
		else if (key == L"path")
			dstPath = value;
		else if (key == L"size")
			fileSize = _wtoll(value);
		else if (key == L"status")
			status = (DownloadStatus)_wtoi(value);
		else if (key == L"title")
			dstFileName = value;
		else if (key == L"time")
			downTime = value;
	}
	free(s);
	return TRUE;
}
