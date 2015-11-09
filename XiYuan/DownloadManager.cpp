#include "stdafx.h"
#include "DownloadManager.h"

UINT DownloadMovieThread(LPVOID lpParams)
{
	CDownloadManager * downMgr = (CDownloadManager *)lpParams;
	ASSERT(downMgr);
	// 获取下载信息
	DownloadTaskParams params = downMgr->downList.front();
	ENSURE(params.status == DOWNLOAD_DOWNING);
	// 通知下载管理器下载任务开始
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
		return L"下载完成";
	case DOWNLOAD_SUSPEND:
		return L"已挂起";
	case DOWNLOAD_DOWNING:
		return L"正在下载";
	case DOWNLOAD_WAITING:
		return L"排队中";
	case DOWNLOAD_ERROR:
		return L"下载失败";
	}
	return L"唔...惜缘不知道该管这时候的状态叫什么了...";
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
	// 仅支持绝对路径
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
	// 分析文件存储位置
	CString downPath = srcUrl;
	downPath.Replace(L"http://", L"");
	downPath = downPath.Right(downPath.GetLength() - downPath.Find(L'/'));
	downPath.Replace(L"/", L"\\");
	downPath = cacheDirectory + downPath;
	if (PathFileExists(downPath))
	{
		// 缓存文件已经存在，那么就认为这个任务已经被下载过了，不再重复添加
		return downList.size();
	}
	CreateMovieDirectory(downPath);
	params.dstPath = downPath;
	params.fileSize = 0;
	params.status = DOWNLOAD_WAITING;
	// 以下，downPath 就没用了
	downPath.SetAt(downPath.ReverseFind(L'\\'), L'?');
	downPath = downPath.Right(downPath.GetLength() - downPath.ReverseFind(L'\\') - 1);
	downPath.Replace(L"?", L"\\");
	params.dstFileName = downPath;
	// 收集完毕需要的下载信息，加入下载队列
	downList.push_back(params);
	if (autoStart)
		StartDownloadMission();
	return downList.size();
}

BOOL CDownloadManager::CancelNowDownloadTask(void)
{
	if (downThread == NULL)
		return FALSE;
	// 强行终止当前线程
	CString downPath = downList.front().dstPath;
	DWORD exitCode;
	GetExitCodeThread(downThread->m_hThread, &exitCode);
	TerminateThread(downThread->m_hThread, exitCode);
	WaitForSingleObject(downThread->m_hThread, INFINITE);
	downThread = NULL;
	// 删除下载到一半的缓存
	if (PathFileExists(downPath))
	{
		return DeleteFile(downPath);
	}
	return TRUE;
}

BOOL CDownloadManager::CancelDownloadWaitingTask(int index)
{
	// 获取修正后的index
	if (index < 0)
	{
		index = downList.size() + index;
	}
	// 判断下标越界
	if (index < 1 || index > downList.size() - 1)
	{
		TRACE1("CancelDownloadWaitingTask 发生下标越界，传入值：%d\n", index);
		return FALSE;
	}
	// 从列表中删除位置 index 地方的任务
	downList.erase(downList.begin() + index);
	return TRUE;
}

BOOL CDownloadManager::SuspendDownloadTask(int index)
{
	// 获取修正后的index
	if (index < 0)
	{
		index = downList.size() + index;
	}
	// 判断下标越界
	if (index < 1 || index > downList.size() - 1)
	{
		TRACE1("SuspendDownloadTask 发生下标越界，传入值：%d\n", index);
		return FALSE;
	}
	// 设置 index 项任务为挂起状态，并移动至队列末尾
	DownloadTaskParams params = downList.at(index);
	if (params.status != DOWNLOAD_WAITING)
	{
		TRACE1("设置 %d 位置处的下载任务为挂起状态时出错，该任务并没有处于等待下载状态。\n", index);
		return FALSE;
	}
	params.status = DOWNLOAD_SUSPEND;
	downList.erase(downList.begin() + index);
	downList.push_back(params);
	return TRUE;
}

BOOL CDownloadManager::MoveTaskForward(int index)
{
	// 获取修正后的index
	if (index < 0)
	{
		index = downList.size() + index;
	}
	// 判断下标越界
	if (index < 1 || index > downList.size() - 1)
	{
		TRACE1("SuspendDownloadTask 发生下标越界，传入值：%d\n", index);
		return FALSE;
	}
	// 判断是否顶元素是否是可交换状态
	if (index == 1 && downList.front().status == DOWNLOAD_DOWNING)
	{
		TRACE0("当有任务在下载时，无法将第二位任务移前。\n");
		return FALSE;
	}
	// 任务可以前移的前提条件是，前面的任务和当前任务的状态一致
	if (downList.at(index).status != downList.at(index - 1).status)
	{
		TRACE1("要移前的任务和前一个任务的状态不一致，无法移前。序号为：%d\n", index);
		return FALSE;
	}
	// 实现任务移前，操作是：先删除 index，再在 index - 1 前插入 index
	DownloadTaskParams params = downList.at(index);
	downList.erase(downList.begin() + index);
	downList.insert(downList.begin() + index - 1, params);
	return TRUE;
}

BOOL CDownloadManager::MoveTaskBackward(int index)
{
	// 获取修正后的index
	if (index < 0)
	{
		index = downList.size() + index;
	}
	// 判断下标越界
	if (index < 1 || index > downList.size() - 1)
	{
		TRACE1("SuspendDownloadTask 发生下标越界，传入值：%d\n", index);
		return FALSE;
	}
	// 判断是否可以继续后移操作
	if (index == downList.size() - 1)
	{
		TRACE0("选定任务项位于列表末尾，无法继续后移。\n");
		return FALSE;
	}
	// 任务可以后移的前提条件是，后面的任务和当前任务的状态一致
	if (downList.at(index).status != downList.at(index + 1).status)
	{
		TRACE1("要后移的任务和后一个任务的状态不一致，无法后移。序号为：%d\n", index);
		return FALSE;
	}
	// 实现任务后移，操作是：先插入 index + 1，再删除 index
	DownloadTaskParams params = downList.at(index);
	downList.insert(downList.begin() + index + 1, params);
	downList.erase(downList.begin() + index);
	return TRUE;
}

BOOL CDownloadManager::MoveTaskTop(int index)
{
	// 获取修正后的index
	if (index < 0)
	{
		index = downList.size() + index;
	}
	// 判断下标越界
	if (index < 1 || index > downList.size() - 1)
	{
		TRACE1("SuspendDownloadTask 发生下标越界，传入值：%d\n", index);
		return FALSE;
	}
	// 置顶视为同状态置顶，即挂起任务置顶结果为挂起任务的顶端
	if (downList.at(index).status != downList.at(index - 1).status)
	{
		TRACE1("无法完成置顶操作，所请求项 %d 已经位于顶端？\n", index);
		return FALSE;
	}
	// 向前搜索同状态目标
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
	// 获取修正后的index
	if (index < 0)
	{
		index = downList.size() + index;
	}
	// 判断下标越界
	if (index < 1 || index > downList.size() - 1)
	{
		TRACE1("SuspendDownloadTask 发生下标越界，传入值：%d\n", index);
		return FALSE;
	}
	// 置底视为同状态置底，即挂起任务置底结果为挂起任务的底端
	if (index == downList.size() - 1)
	{
		TRACE0("当前选择项已经位于最底端。\n");
		return FALSE;
	}
	if (downList.at(index).status != downList.at(index + 1).status)
	{
		TRACE1("无法完成置顶操作，所请求项 %d 已经位于顶端？\n", index);
		return FALSE;
	}
	// 向后搜索同状态目标
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
	// 判断当前是否正在下载
	if (downThread)
	{
		TRACE0("尝试进行下载时发现，已经有任务在下载了，所以就不开心了。\n");
		return FALSE;
	}
	// 判断当前是否还有任务可以下载
	if (!downList.size())
	{
		TRACE0("居然没有东西可以下载了，还是很不开心。\n");
		return FALSE;
	}
	if (downList.front().status != DOWNLOAD_WAITING)
	{
		TRACE0("居然没有东西可以下载了，还是很不开心。\n");
		return FALSE;
	}
	// 标记下载状态并开启线程下载
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
		// 添加最后一条记录
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
		// 添加最后一条记录
		DownloadTaskParams pas;
		if (pas.fromString(content))
			finishList.push_back(pas);
		file.Close();
	}
	// 进行异常状态判断
	if (downList.size())
	{
		if (downList.front().status == DOWNLOAD_DOWNING)
		{
			downList.front().status == DOWNLOAD_WAITING;
			if (PathFileExists(downList.front().dstPath))
			{
				// 此处应该做一个安全检查，查看删除的文件类型和位置
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
