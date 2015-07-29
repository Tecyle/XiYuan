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