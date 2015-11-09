// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// (��Fluent UI��)����ʾ�������ο���
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���  
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�  
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ�������  
// http://go.microsoft.com/fwlink/?LinkId=238214��
//
// ��Ȩ����(C) Microsoft Corporation
// ��������Ȩ����

// stdafx.cpp : ֻ������׼�����ļ���Դ�ļ�
// XiYuan.pch ����ΪԤ����ͷ
// stdafx.obj ������Ԥ����������Ϣ

#include "stdafx.h"

/**
* �����ֽ��ַ���ת���ɿ��ַ��ַ���
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
* �����ַ��ַ���ת���ɶ��ֽ��ַ���
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
	wchar_t * pwBuf = new wchar_t[nwLen + 1];//һ��Ҫ��1����Ȼ�����β��
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