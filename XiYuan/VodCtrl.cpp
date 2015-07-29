// VodCtrl.cpp : ʵ���ļ�
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
	// �ͷ��˶��Զ�����������һ�����ú󣬽�����
	// OnFinalRelease��  ���ཫ�Զ�
	// ɾ���ö���  �ڵ��øû���֮ǰ�����������
	// ��������ĸ���������롣

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CVodCtrl, CCmdTarget)
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CVodCtrl, CCmdTarget)
	DISP_FUNCTION(CVodCtrl, "VodDown2Play", VodDown2Play, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CVodCtrl, "VodDownloadAll", VodDownloadAll, VT_EMPTY, VTS_BSTR)
END_DISPATCH_MAP()

// ע��: ������� IID_IVodCtrl ֧��
//  ��֧������ VBA �����Ͱ�ȫ�󶨡�  �� IID ����ͬ���ӵ� .IDL �ļ��е�
//  ���Ƚӿڵ� GUID ƥ�䡣

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
	// �ж��ǲ��ǲ����б�ҳ��
	if (strUrl.Find(L"http://vod.cau.edu.cn/index.php?mod=movie", 0) == -1)
		return FALSE;
	// ��ȡӰƬ���
	movieId = strUrl.Right(strUrl.GetLength() - strUrl.ReverseFind(L'=') - 1);
	// ץȡ��Ч����
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
		// ͨ�����href���ж��Ƿ�����Ҫ��
		hr = pAnchor->get_href(&url.bstrVal);
		if (FAILED(hr))
		{
			pdisp->Release();
			pAnchor->Release();
			continue;
		}
		strUrl = url.bstrVal;
		// �ּ���������
		if (strUrl.Find(L"javascript:play('/index.php?mod=content&action=play&urlid=", 0) != -1)
		{
			// �滻Ŀ��
			CString urlid = strUrl.Mid(strUrl.ReverseFind(L'=') + 1, strUrl.ReverseFind(L'\'') - strUrl.ReverseFind(L'=') - 1);
			urlid = L"javascript:window.external.VodDown2Play(\'" + urlid + L"\')";
			BSTR s = urlid.AllocSysString();
			hr = pAnchor->put_href(s);
			SysFreeString(s);
			if (FAILED(hr))
			{
				TRACE1("д�� href : %s ʱʧ�ܣ�\n", urlid);
				continue;
			}
		}
		// ȫ����������
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
				if (downLinkName.Find(L"PT����") != -1)
				{
					element->put_innerText(L"ϧԵ��һ��������ȫ���������ɣ�������~ �r(�s���t)�q");
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
	// �ж������Ƿ�ɹ�
	DWORD respCode = 0;
	pFile->QueryInfoStatusCode(respCode);
	if (respCode < 200 || respCode > 299)
	{
		pFile->Close();
		pHttp->Close();
		return res;
	}
	// ������������
	BYTE buffer[1024];
	UINT len = 0;
	string strBuffer;
	while (len = pFile->Read(buffer, 1023))
	{
		buffer[len] = 0;
		strBuffer += (char *)buffer;
	}
	if (strBuffer.find("����û�е�½!���½��㲥") != string::npos)
	{
		if (!LoginMovieSite(strBuffer, pHttp))
		{
			pFile->Close();
			pHttp->Close();
			return res;
		}
	}
	// ���ڣ�strBuffer����Ͱ�����ӰƬ��ַ
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
	// ��ȡ��½��ַ
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
	// ��ҪPOST�ύ��¼����
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
	// TODO : �˴���ӵ�¼ʧ���ж��߼�
	if (strBuffer.find("�û����������벻��ȷ!") != string::npos)
	{
		pFile->Close();
		return FALSE;
	}
	pFile->Close();
	return TRUE;
}

BOOL CVodCtrl::CreateMovieDirectory(CString fileName)
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

UINT DownloadMovieThread(LPVOID lpParams)
{
	DWORD tId = GetCurrentThreadId();
	CVodCtrl * pVodCtrl = (CVodCtrl *)lpParams;
	CString url = pVodCtrl->downloadTaskList[tId].url;
	CString downPath = url;
	// ��ȡ����λ��
	downPath.Replace(L"http://", L"");
	downPath = downPath.Right(downPath.GetLength() - downPath.Find(L'/'));
	downPath.Replace(L"/", L"\\");
	downPath = pVodCtrl->cacheDirectory + downPath;
	pVodCtrl->CreateMovieDirectory(downPath);
	pVodCtrl->downloadTaskList[tId].flagPlayed = FALSE;
	pVodCtrl->downloadTaskList[tId].localName = downPath;
	pVodCtrl->downloadTaskList[tId].pVodCtrl = pVodCtrl;
	// �жϸ���Ƶ�ļ��Ƿ����������
	if (PathFileExists(downPath))
	{
		// �Ѿ��������ֱ�Ӳ��žͺ���
		pVodCtrl->OnDownloadCompleted(tId);
		return 0;
	}
	// ��ʼ����
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
	// ���������߳�
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
	// ��ȡ����ָ�����
	CString sName = L"\"" + downloadTaskList[tId].localName + L"\"";
	ShellExecute(NULL, L"Open", sName, NULL, NULL, SW_SHOW);
	// �Ƴ��߳�����
	downloadTaskList.erase(tId);
}

void CVodCtrl::OnReadyToPlay(DWORD threadId, CString localName)
{
	// ��ȡ����ָ�����
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
		MessageBox(NULL, L"����ӰƬʧ�ܣ���ȡӰƬ��ַʱ��������", NULL, MB_ICONERROR);
		return;
	}
	theToast.ShowToast(L"ϧԵ����ƴ��ȫ��Ϊ�����ص�ӰŶ~�������֮��ͻ��Զ��򿪲�����~\r\n~(�R���Q)/~ ", 5000);
	DownloadMovie(AnsiToUnicode(movieAddr));
}

void CVodCtrl::VodDownloadAll(BSTR movieId)
{
	theToast.ShowToast(L"���ع������Ͼ�Ҫ��ʼ��~", 2000);
	theToast.ShowToast(L"��ȥ���꣬����������������Ʈɢ��ֻ���ù�����Ӱ�����һ����⡣���罥�䣬����˭�������갵��͵��������δ��,������ʡ��̺���������,˲�������ӯӯǳЦ�е���˼��ת���������������Ե��");
}


// CVodCtrl ��Ϣ�������

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
	// �����ؽ��ȴﵽ20%��ʱ����Բ�����
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
