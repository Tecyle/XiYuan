// Qrcode.h: interface for the Qrcode class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QRCODE_H__C3190B1C_AC08_431D_9AA8_95D50D1F09E0__INCLUDED_)
#define AFX_QRCODE_H__C3190B1C_AC08_431D_9AA8_95D50D1F09E0__INCLUDED_
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//typedef bool near           *Pbool;
typedef char near           *Pbyte;


class Qrcode  :CDialog
{
public:
	Qrcode();
	virtual ~Qrcode();

private:
    char qrcodeErrorCorrect;
    char qrcodeEncodeMode;
    int qrcodeVersion;
    int qrcodeStructureappendN;
    int qrcodeStructureappendM;
    int qrcodeStructureappendParity;
    CString qrcodeStructureappendOriginaldata;

public:
	void	setQrcodeErrorCorrect(char c);
	char	getQrcodeErrorCorrect();

	void	setQrcodeVersion(int i);
	int		getQrcodeVersion();
	
	void	setQrcodeEncodeMode(char c);
	char	getQrcodeEncodeMode();

	void	setStructureappend(int i, int j, int k);
	void	TestMsg(char* myStr,int length);
	
	bool** calQrcode(char* abyte0,int iLen0,int& iLenRet);

	int calStructureappendParity(char* abyte0,int i);


private: //私有方法
	 char* divideDataBy8Bits(int* ai, char* abyte0, int i,int iLen,int& iLRet);//,,,返回长度
	 char* calculateRSECC(char* abyte0, char byte0, char* abyte1, int i, int j,
		int iLen0,int iLen1);	
	 char selectMask(char** abyte0, int i,int iLen0);
	 char* calculateByteArrayBits(char* abyte0, char* abyte1, CString s,
		int iLen0,int iLen1,int& iLRet);
};

#endif // !defined(AFX_QRCODE_H__C3190B1C_AC08_431D_9AA8_95D50D1F09E0__INCLUDED_)
