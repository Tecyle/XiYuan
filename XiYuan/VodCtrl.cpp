// VodCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "XiYuan.h"
#include "VodCtrl.h"
#include "ToastDlg.h"
#include "MainFrm.h"


// CVodCtrl

IMPLEMENT_DYNAMIC(CVodCtrl, CCmdTarget)


CVodCtrl::CVodCtrl()
{
	EnableAutomation();
	userName = L"tecyle";
	passWord = L"17090115";
	cacheDirectory = L"F:\\Movies";
}

CVodCtrl::~CVodCtrl()
{
}


void CVodCtrl::OnFinalRelease()
{
	// 释放了对自动化对象的最后一个引用后，将调用
	// OnFinalRelease。  基类将自动
	// 删除该对象。  在调用该基类之前，请添加您的
	// 对象所需的附加清理代码。

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CVodCtrl, CCmdTarget)
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CVodCtrl, CCmdTarget)
	DISP_FUNCTION(CVodCtrl, "VodDown2Play", VodDown2Play, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CVodCtrl, "VodDownloadAll", VodDownloadAll, VT_EMPTY, VTS_BSTR)
END_DISPATCH_MAP()

// 注意: 我们添加 IID_IVodCtrl 支持
//  以支持来自 VBA 的类型安全绑定。  此 IID 必须同附加到 .IDL 文件中的
//  调度接口的 GUID 匹配。

// {5277186D-01B5-4226-880A-169C24433906}
static const IID IID_IVodCtrl =
{ 0x5277186D, 0x1B5, 0x4226, { 0x88, 0xA, 0x16, 0x9C, 0x24, 0x43, 0x39, 0x6 } };

BEGIN_INTERFACE_MAP(CVodCtrl, CCmdTarget)
	INTERFACE_PART(CVodCtrl, IID_IVodCtrl, Dispatch)
END_INTERFACE_MAP()

BOOL CVodCtrl::CheckAndReplaceMovieHref(IHTMLDocument2 * doc)
{
	HRESULT hr;
	_variant_t url;
	hr = doc->get_URL(&url.bstrVal);
	if (FAILED(hr))
		return FALSE;
	CString strUrl = url.bstrVal;
	CString movieId;
	// 判断是不是播放列表页面
	if (strUrl.Find(L"http://vod.cau.edu.cn/index.php?mod=movie", 0) == -1)
		return FALSE;
	// 获取影片编号
	movieId = strUrl.Right(strUrl.GetLength() - strUrl.ReverseFind(L'=') - 1);
	// 抓取有效链接
	IHTMLElementCollection * pCollection = NULL;
	hr = doc->get_links(&pCollection);
	ASSERT(pCollection);
	if (FAILED(hr))
		return FALSE;
	long resNum;
	hr = pCollection->get_length(&resNum);
	if (FAILED(hr))
	{
		pCollection->Release();
		return FALSE;
	}
	for (long i = 0; i < resNum; i++)
	{
		IDispatch * pdisp = NULL;
		IHTMLAnchorElement * pAnchor = NULL;
		hr = pCollection->item((_variant_t)i, (_variant_t)i, &pdisp);
		if (FAILED(hr))
		{
			continue;
		}
		ASSERT(pdisp);
		hr = pdisp->QueryInterface(IID_IHTMLAnchorElement, (void **)&pAnchor);
		if (FAILED(hr))
		{
			pdisp->Release();
			continue;
		}
		// 通过检查href来判断是否满足要求
		hr = pAnchor->get_href(&url.bstrVal);
		if (FAILED(hr))
		{
			pdisp->Release();
			pAnchor->Release();
			continue;
		}
		strUrl = url.bstrVal;
		// 分集下载链接
		if (strUrl.Find(L"javascript:play('/index.php?mod=content&action=play&urlid=", 0) != -1)
		{
			// 替换目标
			CString urlid = strUrl.Mid(strUrl.ReverseFind(L'=') + 1, strUrl.ReverseFind(L'\'') - strUrl.ReverseFind(L'=') - 1);
			urlid = L"javascript:window.external.VodDown2Play(\'" + urlid + L"\')";
			BSTR s = urlid.AllocSysString();
			hr = pAnchor->put_href(s);
			SysFreeString(s);
			if (FAILED(hr))
			{
				TRACE1("写入 href : %s 时失败！\n", urlid);
				continue;
			}
		}
		// 全部下载链接
		if (strUrl.Find(L"http://pt.cau.edu.cn/browse.php", 0) != -1)
		{
			IHTMLElement * element;
			hr = pdisp->QueryInterface(IID_IHTMLElement, (void **)&element);
			if (SUCCEEDED(hr))
			{
				_variant_t bstrbuf;
				CString downLinkName;
				element->get_innerText(&bstrbuf.bstrVal);
				downLinkName = bstrbuf.bstrVal;
				if (downLinkName.Find(L"PT下载") != -1)
				{
					element->put_innerText(L"惜缘，一口气帮我全下载下来吧，拜托了~ ╮(╯▽╰)╭");
					downLinkName = L"javascript:window.external.VodDownloadAll(\'" + movieId + L"\')";
					BSTR s = downLinkName.AllocSysString();
					pAnchor->put_href(s);
					pAnchor->put_target(L"");
					SysFreeString(s);
				}
				element->Release();
			}
		}
		pdisp->Release();
		pAnchor->Release();
	}
	return TRUE;
}

string CVodCtrl::FetchMovieURL(CString urlid)
{
	CHttpConnection * pHttp;
	CHttpFile * pFile;
	string res;
	pHttp = session.GetHttpConnection(L"vod.cau.edu.cn");
	CString playCmd = L"index.php?mod=content&action=play&urlid=";
	playCmd += urlid;
	pFile = pHttp->OpenRequest(CHttpConnection::HTTP_VERB_GET, playCmd);
	pFile->AddRequestHeaders(L"Accept: image/gif, image/x-xbitmap, image/jpeg, image/pjpeg, application/vnd.ms-powerpoint, application/vnd.ms-excel, application/msword, */*");
	pFile->AddRequestHeaders(L"User-Agent: Mozilla/4.0 (compatible; MSIE 5.01; Windows NT 5.0)");
	pFile->AddRequestHeaders(L"Content-Type: application/x-www-form-urlencoded");
	pFile->SendRequest();
	// 判断请求是否成功
	DWORD respCode = 0;
	pFile->QueryInfoStatusCode(respCode);
	if (respCode < 200 || respCode > 299)
	{
		pFile->Close();
		pHttp->Close();
		return res;
	}
	// 解析请求内容
	BYTE buffer[1024];
	UINT len = 0;
	string strBuffer;
	while (len = pFile->Read(buffer, 1023))
	{
		buffer[len] = 0;
		strBuffer += (char *)buffer;
	}
	if (strBuffer.find("您还没有登陆!请登陆后点播") != string::npos)
	{
		if (!LoginMovieSite(strBuffer, pHttp))
		{
			pFile->Close();
			pHttp->Close();
			return res;
		}
	}
	// 现在，strBuffer里面就包含了影片地址
	res = strBuffer.substr(
		strBuffer.find("<param name=\"url\"") + strlen("<param name=\"url\""),
		strBuffer.find("<param name=\"uimode\"") - strBuffer.find("<param name=\"url\"") - strlen("<param name=\"url\"")
		);
	res = res.substr(
		res.find("value=\"") + strlen("value=\""),
		res.find("\">") - res.find("value=\"") - strlen("value=\"")
		);
	return res;
}

BOOL CVodCtrl::LoginMovieSite(string & strBuffer, CHttpConnection * pHttp)
{
	CHttpFile * pFile;
	// 提取登陆地址
	string newAddr = strBuffer.substr(
		strBuffer.find("location.href=\'") + strlen("location.href=\'"),
		strBuffer.find("\'</script>") - strBuffer.find("location.href=\'") - strlen("location.href=\'")
		);
	pFile = pHttp->OpenRequest(CHttpConnection::HTTP_VERB_GET, AnsiToUnicode(newAddr));
	pFile->AddRequestHeaders(L"Accept: image/gif, image/x-xbitmap, image/jpeg, image/pjpeg, application/vnd.ms-powerpoint, application/vnd.ms-excel, application/msword, */*");
	pFile->AddRequestHeaders(L"User-Agent: Mozilla/4.0 (compatible; MSIE 5.01; Windows NT 5.0)");
	pFile->AddRequestHeaders(L"Content-Type: application/x-www-form-urlencoded");
	pFile->SendRequest();
	BYTE buffer[1024];
	int len = 0;
	strBuffer = "";
	while (len = pFile->Read(buffer, 1023))
	{
		buffer[len] = 0;
		strBuffer += (LPSTR)buffer;
	}
	pFile->Close();
	// 需要POST提交登录数据
	string postData = "username=";
	postData += UnicodeToAnsi(userName);
	postData += "&password=";
	postData += UnicodeToAnsi(passWord);
	pFile = pHttp->OpenRequest(CHttpConnection::HTTP_VERB_POST, L"/index.php?mod=member&action=login");
	pFile->AddRequestHeaders(L"Accept: image/gif, image/x-xbitmap, image/jpeg, image/pjpeg, application/vnd.ms-powerpoint, application/vnd.ms-excel, application/msword, */*");
	pFile->AddRequestHeaders(L"User-Agent: Mozilla/4.0 (compatible; MSIE 5.01; Windows NT 5.0)");
	pFile->AddRequestHeaders(L"Content-Type: application/x-www-form-urlencoded");
	pFile->SendRequest(NULL, 0, (LPVOID)postData.c_str(), postData.size());
	strBuffer = "";
	while (len = pFile->Read(buffer, 1023))
	{
		buffer[len] = 0;
		strBuffer += (LPSTR)buffer;
	}
	// TODO : 此处添加登录失败判断逻辑
	if (strBuffer.find("用户名或者密码不正确!") != string::npos)
	{
		pFile->Close();
		return FALSE;
	}
	pFile->Close();
	return TRUE;
}

BOOL CVodCtrl::CreateMovieDirectory(CString fileName)
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

UINT DownloadMovieThread(LPVOID lpParams)
{
	DWORD tId = GetCurrentThreadId();
	CVodCtrl * pVodCtrl = (CVodCtrl *)lpParams;
	CString url = pVodCtrl->downloadTaskList[tId].url;
	CString downPath = url;
	// 获取保存位置
	downPath.Replace(L"http://", L"");
	downPath = downPath.Right(downPath.GetLength() - downPath.Find(L'/'));
	downPath.Replace(L"/", L"\\");
	downPath = pVodCtrl->cacheDirectory + downPath;
	pVodCtrl->CreateMovieDirectory(downPath);
	pVodCtrl->downloadTaskList[tId].flagPlayed = FALSE;
	pVodCtrl->downloadTaskList[tId].localName = downPath;
	pVodCtrl->downloadTaskList[tId].pVodCtrl = pVodCtrl;
	// 判断该视频文件是否曾经缓存过
	if (PathFileExists(downPath))
	{
		// 已经缓存过就直接播放就好了
		pVodCtrl->OnDownloadCompleted(tId);
		return 0;
	}
	// 开始下载
	((CMainFrame *)AfxGetMainWnd())->PostMessage(WM_MOVIE_DOWNLOAD_START);
	HRESULT hr = URLDownloadToFile(NULL, url, downPath, 0, &pVodCtrl->downloadTaskList[tId]);
	((CMainFrame *)AfxGetMainWnd())->PostMessage(WM_MOVIE_DOWNLOAD_END);
	if (FAILED(hr))
	{
		pVodCtrl->OnDownloadError(tId, hr);
		return hr;
	}
	else{
		pVodCtrl->OnDownloadCompleted(tId);
	}
	return 0;
}

HRESULT CVodCtrl::DownloadMovie(CString url)
{
	// 开启下载线程
	CDownloadProgressCtrl progressCtrl;
	progressCtrl.url = url;
	CWinThread * hThread = AfxBeginThread(DownloadMovieThread, this, 0, 0U, CREATE_SUSPENDED);
	progressCtrl.threadId = hThread->m_nThreadID;
	downloadTaskList[hThread->m_nThreadID] = progressCtrl;
	hThread->ResumeThread();
	return S_OK;
}

void CVodCtrl::OnDownloadError(DWORD tId, HRESULT hr)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void CVodCtrl::OnDownloadCompleted(DWORD tId)
{
	// 获取播放指令并播放
	CString sName = L"\"" + downloadTaskList[tId].localName + L"\"";
	ShellExecute(NULL, L"Open", sName, NULL, NULL, SW_SHOW);
	// 移除线程内容
	downloadTaskList.erase(tId);
}

void CVodCtrl::OnReadyToPlay(DWORD threadId, CString localName)
{
	// 获取播放指令并播放
	// ShellExecute(NULL, L"open", localName, NULL, NULL, SW_SHOW);
	DWORD errcode = GetLastError();
}

CString CVodCtrl::GetMediaPlayerLocation()
{
	return L"D:\\Program Files (x86)\\Thunder Network\\Xmp\\Program\\XMP.exe";
}

CString CVodCtrl::GetMediaPlayerParams(CString localName)
{
	CString params = L" /play \"%L\"";
	params.Replace(L"%L", localName);
	return params;
}

void CVodCtrl::VodDown2Play(BSTR urlid)
{
	string movieAddr = FetchMovieURL(urlid);
	if (movieAddr.length() == 0)
	{
		MessageBox(NULL, L"播放影片失败，获取影片地址时遇到错误！", NULL, MB_ICONERROR);
		return;
	}
	theToast.ShowToast(L"惜缘正在拼尽全力为您加载电影哦~加载完成之后就会自动打开播放啦~\r\n~(≧▽≦)/~ ", 5000);
	DownloadMovie(AnsiToUnicode(movieAddr));
}

void CVodCtrl::VodDownloadAll(BSTR movieId)
{
	theToast.ShowToast(L"下载过程马上就要开始啦~", 2000);
	theToast.ShowToast(L"此去经年，待到俗世纷扰如烟飘散，只留得故人孑影，怅对一湖天光。西风渐冷，又是谁，把流年暗中偷换。容颜未改,青衫如故。碧湖波光流动,瞬间折射出盈盈浅笑中的情思百转，如缠绕三生的因缘。");
}


// CVodCtrl 消息处理程序

HRESULT STDMETHODCALLTYPE CDownloadProgressCtrl::OnStartBinding(DWORD dwReserved, __RPC__in_opt IBinding *pib)
{
	UNREFERENCED_PARAMETER(dwReserved);
	UNREFERENCED_PARAMETER(pib);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CDownloadProgressCtrl::GetPriority(__RPC__out LONG *pnPriority)
{
	UNREFERENCED_PARAMETER(pnPriority);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CDownloadProgressCtrl::OnLowResource(DWORD reserved)
{
	UNREFERENCED_PARAMETER(reserved);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CDownloadProgressCtrl::OnProgress(ULONG ulProgress, ULONG ulProgressMax, ULONG ulStatusCode, __RPC__in_opt LPCWSTR szStatusText)
{
	if (ulProgressMax == 0)
		return S_OK;
	// 当下载进度达到20%的时候可以播放了
	if (lastProgress != (int)((double)ulProgress * 100.0 / (double)ulProgressMax))
	{
		lastProgress = (int)((double)ulProgress * 100.0 / (double)ulProgressMax);
		((CMainFrame *)AfxGetMainWnd())->PostMessage(WM_MOVIE_DOWNLOAD_PROGRESS, threadId, lastProgress);
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CDownloadProgressCtrl::OnStopBinding(HRESULT hresult, __RPC__in_opt LPCWSTR szError)
{
	UNREFERENCED_PARAMETER(hresult);
	UNREFERENCED_PARAMETER(szError);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CDownloadProgressCtrl::GetBindInfo(DWORD *grfBINDF, BINDINFO *pbindinfo)
{
	UNREFERENCED_PARAMETER(grfBINDF);
	UNREFERENCED_PARAMETER(pbindinfo);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CDownloadProgressCtrl::OnDataAvailable(DWORD grfBSCF, DWORD dwSize, FORMATETC *pformatetc, STGMEDIUM *pstgmed)
{
	UNREFERENCED_PARAMETER(grfBSCF);
	UNREFERENCED_PARAMETER(dwSize);
	UNREFERENCED_PARAMETER(pformatetc);
	UNREFERENCED_PARAMETER(pstgmed);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CDownloadProgressCtrl::OnObjectAvailable(__RPC__in REFIID riid, __RPC__in_opt IUnknown *punk)
{
	UNREFERENCED_PARAMETER(riid);
	UNREFERENCED_PARAMETER(punk);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CDownloadProgressCtrl::QueryInterface(REFIID riid, void **ppvObject)
{
	UNREFERENCED_PARAMETER(riid);
	UNREFERENCED_PARAMETER(ppvObject);
	return E_NOTIMPL;
}

ULONG STDMETHODCALLTYPE CDownloadProgressCtrl::AddRef(void)
{
	return 0;
}

ULONG STDMETHODCALLTYPE CDownloadProgressCtrl::Release(void)
{
	return 0;
}
