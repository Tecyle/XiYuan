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

// stdafx.cpp : 只包括标准包含文件的源文件
// XiYuan.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"

/**
* 将多字节字符串转换成宽字符字符串
*/
CString AnsiToUnicode(string str)
{
	size_t len;
	CString res;
	WCHAR * buffer = NULL;
	len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, buffer, 0);
	buffer = new WCHAR[len];
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, buffer, len);
	res = buffer;
	delete buffer;
	return res;
}

/**
* 将宽字符字符串转换成多字节字符串
*/
string UnicodeToAnsi(CString str)
{
	size_t len;
	string res;
	char * buffer = NULL;
	len = WideCharToMultiByte(CP_ACP, 0, str, -1, buffer, 0, NULL, NULL);
	buffer = new char[len];
	WideCharToMultiByte(CP_ACP, 0, str, -1, buffer, len, NULL, NULL);
	res = buffer;
	delete buffer;
	return res;
}
string string_To_UTF8(const std::string & str)
{
	int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
	wchar_t * pwBuf = new wchar_t[nwLen + 1];//一定要加1，不然会出现尾巴
	ZeroMemory(pwBuf, nwLen * 2 + 2);
	::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen);
	int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);
	char * pBuf = new char[nLen + 1];
	ZeroMemory(pBuf, nLen + 1);
	::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);
	std::string retStr(pBuf);
	delete[]pwBuf;
	delete[]pBuf;
	pwBuf = NULL;
	pBuf = NULL;
	return retStr;
}

CString GetFileSizeString(ULONG fSize)
{
	double tpSize = (double)fSize;
	int unitHit = 0;
	CString unit;
	while (tpSize > 1024.0 && unitHit < 3)
	{
		tpSize /= 1024.0;
		unitHit++;
	}
	switch (unitHit)
	{
	case 0:
		unit = L"B";
		break;
	case 1:
		unit = L"KB";
		break;
	case 2:
		unit = L"MB";
		break;
	case 3:
		unit = L"GB";
	default:
		break;
	}
	CString res;
	if (unitHit > 0)
		res.Format(L"%.2f %s", tpSize, unit);
	else
		res.Format(L"%d %s", fSize, unit);
	return res;
}