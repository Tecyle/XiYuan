// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。  
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。  
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问  
// http://go.microsoft.com/fwlink/?LinkId=238214。
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// XiYuanDoc.cpp : CXiYuanDoc 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "XiYuan.h"
#endif

#include "XiYuanDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CXiYuanDoc

IMPLEMENT_DYNCREATE(CXiYuanDoc, CDocument)

BEGIN_MESSAGE_MAP(CXiYuanDoc, CDocument)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CXiYuanDoc, CDocument)
END_DISPATCH_MAP()

// 注意: 我们添加了对 IID_IXiYuan 的支持
//  以支持来自 VBA 的类型安全绑定。  此 IID 必须同附加到 .IDL 文件中的
//  调度接口的 GUID 匹配。

// {10A5A587-D169-48EB-AE84-A7762B0C3CDA}
static const IID IID_IXiYuan =
{ 0x10A5A587, 0xD169, 0x48EB, { 0xAE, 0x84, 0xA7, 0x76, 0x2B, 0xC, 0x3C, 0xDA } };

BEGIN_INTERFACE_MAP(CXiYuanDoc, CDocument)
	INTERFACE_PART(CXiYuanDoc, IID_IXiYuan, Dispatch)
END_INTERFACE_MAP()


// CXiYuanDoc 构造/析构

CXiYuanDoc::CXiYuanDoc()
{
	// TODO:  在此添加一次性构造代码

	EnableAutomation();

	AfxOleLockApp();
}

CXiYuanDoc::~CXiYuanDoc()
{
	AfxOleUnlockApp();
}

BOOL CXiYuanDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO:  在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CXiYuanDoc 序列化

void CXiYuanDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO:  在此添加存储代码
	}
	else
	{
		// TODO:  在此添加加载代码
	}
}

#ifdef SHARED_HANDLERS

// 缩略图的支持
void CXiYuanDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 修改此代码以绘制文档数据
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 搜索处理程序的支持
void CXiYuanDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 从文档数据设置搜索内容。
	// 内容部分应由“;”分隔

	// 例如:     strSearchContent = _T("point;rectangle;circle;ole object;")；
	SetSearchContent(strSearchContent);
}

void CXiYuanDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CXiYuanDoc 诊断

#ifdef _DEBUG
void CXiYuanDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CXiYuanDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CXiYuanDoc 命令
