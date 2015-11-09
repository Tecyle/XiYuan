// Qrcode.cpp: implementation of the Qrcode class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Qrcode.h"
#include <math.h>
#include "afx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
#define QRCODE_DATA_PATH  "qrcode_data"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Qrcode::Qrcode()
{	
	qrcodeErrorCorrect = 'M';
	qrcodeEncodeMode = 'B';
	qrcodeVersion = 0;
	qrcodeStructureappendN = 0;
	qrcodeStructureappendM = 0;
	qrcodeStructureappendParity = 0;
	qrcodeStructureappendOriginaldata = "";	
}

Qrcode::~Qrcode()
{
	
}

void Qrcode::setQrcodeErrorCorrect(char c)
{
	qrcodeErrorCorrect = c;
}

char Qrcode::getQrcodeErrorCorrect()
{
	return qrcodeErrorCorrect;
}

int Qrcode::getQrcodeVersion()
{
	return qrcodeVersion;
}

void Qrcode::setQrcodeVersion(int i)
{
	if(i >= 0 && i <= 40)
		qrcodeVersion = i;
}

void Qrcode::setQrcodeEncodeMode(char c)
{
	qrcodeEncodeMode = c;
}

char Qrcode::getQrcodeEncodeMode()
{
	return qrcodeEncodeMode;
}

void Qrcode::setStructureappend(int i, int j, int k)
{
	if(j > 1 && j <= 16 && i > 0 && i <= 16 && k >= 0 && k <= 255)
	{
		qrcodeStructureappendM = i;
		qrcodeStructureappendN = j;
		qrcodeStructureappendParity = k;
	}
}

//int Qrcode::calStructureappendParity(char abyte0[],int i)
int Qrcode::calStructureappendParity(char* abyte0,int i)
{
	int j = 0;
	int k = 0;
	//int i = abyte0.length;
	if(i > 1)
	{
		k = 0;
		for(; j < i; j++)
			k ^= abyte0[j] & 255;
		
	} else
	{
		k = -1;
	}
	return k;
}

//bool** Qrcode::calQrcode(char abyte0[],int iLen0,int& iLenRet)
bool** Qrcode::calQrcode(char* abyte0,int iLen0,int& iLenRet)
{
	int j = 0;
	int n1;
	int i = iLen0;
	//int ai[] = new int[i + 32];
	int* ai = new int[i + 32];
	//char abyte1[] = new char[i + 32];
	char* abyte1 = new char[i + 32];
	memset(abyte1,0,i+32);
	if(i <= 0)
	{		
		bool** aflag=new bool*[1];
		aflag[0]=new bool[1];
		aflag[0][0]=false;
		iLenRet=1;
		return aflag;
	}
	if(qrcodeStructureappendN > 1)
	{
		ai[0] = 3;
		abyte1[0] = 4;
		ai[1] = qrcodeStructureappendM - 1;
		abyte1[1] = 4;
		ai[2] = qrcodeStructureappendN - 1;
		abyte1[2] = 4;
		ai[3] = qrcodeStructureappendParity;
		abyte1[3] = 8;
		j = 4;
	}
	abyte1[j] = 4;
	//int* ai1=new int[41];
	int ai1[41];
	int k;
	switch(qrcodeEncodeMode)
	{
	case 65: // 'A'
			 /*ai1 = (new int[] {
			 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
			 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 
			 2, 2, 2, 2, 2, 2, 2, 4, 4, 4, 
			 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 
			 4
	});*/
		{
			for(int ii=0;ii<41;ii++)
			{
				if(ii<=9)ai1[ii]=0;
				else if(ii<=26)ai1[ii]=2;
				else ai1[ii]=4;
			}
			
			ai[j] = 2;
			j++;
			ai[j] = i;
			abyte1[j] = 9;
			k = j;
			j++;
			for(int l = 0; l < i; l++)
			{
				char c = (char)abyte0[l];
				char byte0 = 0;
				if(c >= '0' && c < ':')
					byte0 = (char)(c - 48);
				else
					if(c >= 'A' && c < '[')
					{
						byte0 = (char)(c - 55);
					} else
					{
						if(c == ' ')
							byte0 = 36;
						if(c == '$')
							byte0 = 37;
						if(c == '%')
							byte0 = 38;
						if(c == '*')
							byte0 = 39;
						if(c == '+')
							byte0 = 40;
						if(c == '-')
							byte0 = 41;
						if(c == '.')
							byte0 = 42;
						if(c == '/')
							byte0 = 43;
						if(c == ':')
							byte0 = 44;
					}
					if(l % 2 == 0)
					{
						ai[j] = byte0;
						abyte1[j] = 6;
					} else
					{
						ai[j] = ai[j] * 45 + byte0;
						abyte1[j] = 11;
						if(l < i - 1)
							j++;
					}
			}			
			j++;			
		}		
		break;		
	case 78: // 'N'
			 /*ai1 = (new int[] {
			 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
			 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 
			 2, 2, 2, 2, 2, 2, 2, 4, 4, 4, 
			 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 
			 4
	});*/
		{
			for(int ii=0;ii<41;ii++)
			{
				if(ii<=9)ai1[ii]=0;
				else if(ii<=26)ai1[ii]=2;
				else ai1[ii]=4;
			}
			ai[j] = 1;
			j++;
			ai[j] = i;
			abyte1[j] = 10;
			k = j;
			j++;
			for(int i1 = 0; i1 < i; i1++)
				if(i1 % 3 == 0)
				{
					ai[j] = abyte0[i1] - 48;
					abyte1[j] = 4;
				} else
				{
					ai[j] = ai[j] * 10 + (abyte0[i1] - 48);
					if(i1 % 3 == 1)
					{
						abyte1[j] = 7;
					} else
					{
						abyte1[j] = 10;
						if(i1 < i - 1)
							j++;
					}
				}
				
				j++;
				
		}		
		break;			
	default:
	/*ai1 = (new int[] {
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
                8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 
                8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 
                8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 
                8
		});*/		
		for(int ii=0;ii<41;ii++)
		{
			if(ii<=9)ai1[ii]=0;			
			else ai1[ii]=8;
		}
		ai[j] = 4;
		j++;
		ai[j] = i;
		abyte1[j] = 8;
		k = j;
		j++;
		for(int j1 = 0; j1 < i; j1++)
		{
			ai[j1 + j] = abyte0[j1] & 255;
			abyte1[j1 + j] = 8;
		}
		
		j += i;
		break;
        }
        int k1 = 0;
        for(int l1 = 0; l1 < j; l1++)
            k1 += abyte1[l1];
		
        char byte1;
        switch(qrcodeErrorCorrect)
        {
        case 76: // 'L'
            byte1 = 1;
            break;
			
        case 81: // 'Q'
            byte1 = 3;
            break;
			
        case 72: // 'H'
            byte1 = 2;
            break;
			
        default:
            byte1 = 0;
            break;
        }

        //int ai2[4][41] = {
		int ai2[4][41] = {
            {
					0, 128, 224, 352, 512, 688, 864, 992, 1232, 1456, 
					1728, 2032, 2320, 2672, 2920, 3320, 3624, 4056, 4504, 5016, 
					5352, 5712, 6256, 6880, 7312, 8000, 8496, 9024, 9544, 10136, 
					10984, 11640, 12328, 13048, 13800, 14496, 15312, 15936, 16816, 17728, 
					18672
            }, 
			{
					0, 152, 272, 440, 640, 864, 1088, 1248, 1552, 1856, 
					2192, 2592, 2960, 3424, 3688, 4184, 4712, 5176, 5768, 6360, 
					6888, 7456, 8048, 8752, 9392, 10208, 10960, 11744, 12248, 13048, 
					13880, 14744, 15640, 16568, 17528, 18448, 19472, 20528, 21616, 22496, 
					23648
				}, 
				{
						0, 72, 128, 208, 288, 368, 480, 528, 688, 800, 
							976, 1120, 1264, 1440, 1576, 1784, 2024, 2264, 2504, 2728, 
							3080, 3248, 3536, 3712, 4112, 4304, 4768, 5024, 5288, 5608, 
							5960, 6344, 6760, 7208, 7688, 7888, 8432, 8768, 9136, 9776, 
							10208
					},
					{
							0, 104, 176, 272, 384, 496, 608, 704, 880, 1056, 
								1232, 1440, 1648, 1952, 2088, 2360, 2600, 2936, 3176, 3560, 
								3880, 4096, 4544, 4912, 5312, 5744, 6032, 6464, 6968, 7288, 
								7880, 8264, 8920, 9368, 9848, 10288, 10832, 11408, 12016, 12656, 
								13328
						}
        };
        int i2 = 0;
        if(qrcodeVersion == 0)
        {
            qrcodeVersion = 1;
            for(int j2 = 1; j2 <= 40; j2++)
            {
                if(ai2[byte1][j2] >= k1 + ai1[qrcodeVersion])
                {
                    i2 = ai2[byte1][j2];
                    break;
                }
                qrcodeVersion++;
            }
			
        } else
        {
            i2 = ai2[byte1][qrcodeVersion];
        }
        k1 += ai1[qrcodeVersion];
        abyte1[k] += ai1[qrcodeVersion];
        int ai3[] = {
            0, 26, 44, 70, 100, 134, 172, 196, 242, 292, 
				346, 404, 466, 532, 581, 655, 733, 815, 901, 991, 
				1085, 1156, 1258, 1364, 1474, 1588, 1706, 1828, 1921, 2051, 
				2185, 2323, 2465, 2611, 2761, 2876, 3034, 3196, 3362, 3532, 
				3706
        };
        int k2 = ai3[qrcodeVersion];
        int l2 = 17 + (qrcodeVersion << 2);
        int ai4[] = {
            0, 0, 7, 7, 7, 7, 7, 0, 0, 0, 
				0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 
				3, 4, 4, 4, 4, 4, 4, 4, 3, 3, 
				3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 
				0
        };
        int i3 = ai4[qrcodeVersion] + (k2 << 3);
        char* abyte2 = new char[i3];
        char* abyte3 = new char[i3];
        char* abyte4 = new char[i3];		
		char abyte5[15];
		char abyte6[15];
		char abyte7[1];
		char abyte8[128];
		
		memset(abyte2,0,i3);
		memset(abyte3,0,i3);
		memset(abyte4,0,i3);
		memset(abyte5,0,15);
		memset(abyte6,0,15);
		memset(abyte7,0,1);
		memset(abyte8,0,128);
		
        try
        {
            //CString s = "qrcode_data/qrv" + Integer.toString(qrcodeVersion) + "_" + Integer.toString(byte1) + ".dat";
			CString s;
			s.Format(L"qrcode_data/qrv%d_%d.dat",qrcodeVersion,byte1);			
			
			//CFile f(s, CFile::modeCreate | CFile::modeWrite );	
			CFile f(s, CFile::modeRead);
			try
			{				
				f.Read(abyte2, i3);
				f.Read(abyte3, i3);
				f.Read(abyte4, i3);
				f.Read(abyte5, 15);
				f.Read(abyte6, 15);
				f.Read(abyte7, 1);
				f.Read(abyte8, 128);
				f.Close();
			
			}
			catch(...)
			{			
				f.Close();
			}
			
			//写临时文件
            /*InputStream inputstream = (com.swetake.util.Qrcode.class).getResourceAsStream(s);
            BufferedInputStream bufferedinputstream = new BufferedInputStream(inputstream);
            bufferedinputstream.read(abyte2);
            bufferedinputstream.read(abyte3);
            bufferedinputstream.read(abyte4);
            bufferedinputstream.read(abyte5);
            bufferedinputstream.read(abyte6);
            bufferedinputstream.read(abyte7);
            bufferedinputstream.read(abyte8);
            bufferedinputstream.close();
            inputstream.close();*/
        }
        catch(...)
        {
			//AfxMessageBox(exception.)
            //exception.printStackTrace();
        }
        int j3 = 1;
        for(char byte2 = 1; byte2 < 128; byte2++)
        {
            if(abyte8[byte2] != 0)
                continue;
            j3 = byte2;
            break;
        }
		
        char* abyte9 = new char[j3+1];
		memset(abyte9,0,j3);
		memcpy(abyte9,abyte8,j3);
		abyte9[j3]=0;
        //System.arraycopy(abyte8, 0, abyte9, 0, j3);
        char abyte10[15] = {
			0, 1, 2, 3, 4, 5, 7, 8, 8, 8, 
				8, 8, 8, 8, 8
        };
        char abyte11[15] = {
			8, 8, 8, 8, 8, 8, 8, 8, 7, 5, 
				4, 3, 2, 1, 0
        };
        int k3 = i2 >> 3;
        int l3 = 4 * qrcodeVersion + 17;
        int i4 = l3 * l3;
        char* abyte12 = new char[i4 + l3];
		memset(abyte12,0,i4 + l3);
        try
        {
            //CString s1 = "qrcode_data/qrvfr" + Integer.toString(qrcodeVersion) + ".dat";
			CString s1;
			s1.Format(L"qrcode_data/qrvfr%d.dat",qrcodeVersion);
			//CFile f1(s1, CFile::modeCreate | CFile::modeWrite );	
			CFile f1(s1, CFile::modeRead);
			try
			{				
				f1.Read(abyte12, i4 + l3);
				f1.Close();
			}
			catch(...)
			{			
				f1.Close();
			}          
        }
        catch(...)
        {
			// exception1.printStackTrace();
        }
        if(k1 <= i2 - 4)
        {
            ai[j] = 0;
            abyte1[j] = 4;
        } else
			if(k1 < i2)
			{
				ai[j] = 0;
				abyte1[j] = (char)(i2 - k1);
			} else
				if(k1 > i2)
					TRACE("overflow");
				//AfxMessageBox()
				//	System.out.println("overflow");
		int iLret1=0;
	  char* abyte13 = divideDataBy8Bits(ai, abyte1, k3,i + 32,iLret1);
		

		char* abyte14 = calculateRSECC(abyte13, abyte7[0], abyte9, k3, k2,iLret1,j3);		
	
		char** abyte15=new char*[l3];
		for( n1 = 0; n1 <l3; n1++)
		{
			//abyte15[n1] = new char[l3];
			abyte15[n1] = new char[l3];		
		}
		
		for(int j4 = 0; j4 < l3; j4++)
		{
			for(int k4 = 0; k4 < l3; k4++)
				abyte15[k4][j4] = 0;			
		}		

		for(int l4 = 0; l4 < k2; l4++)
		{
			char byte3 = abyte14[l4];
			for(int j5 = 7; j5 >= 0; j5--)
			{
				int l5 = l4 * 8 + j5;
				//if((abyte2[l5]==0)&&(abyte3[l5]==34)) AfxMessageBox("1");
				abyte15[abyte2[l5] & 255][abyte3[l5] & 255] = (char)(255 * (byte3 & 1) ^ abyte4[l5]);
				//byte3 = (char)((byte3 & 255) >>> 1);
				byte3 = (char)((byte3 & 255) >> 1);
				//byte3 = (char)((unsigned char)((byte3 & 255) >> 1));
			}					
		}

		
	
		/*for( j4 = 0; j4 < l3; j4++)
		{
			TestMsg(abyte15[j4],l3);			
		}*/
		
		for(int i5 = ai4[qrcodeVersion]; i5 > 0; i5--)
		{
			int k5 = (i5 + k2 * 8) - 1;
			abyte15[abyte2[k5] & 255][abyte3[k5] & 255] = (char)(255 ^ abyte4[k5]);
		}
		
	
		char byte4 = selectMask((char**)abyte15, ai4[qrcodeVersion] + k2 * 8,l3);
		char byte5 = (char)(1 << byte4);
		char byte6 = (char)(byte1 << 3 | byte4);
		CString as[32] = {
			L"101010000010010", L"101000100100101", L"101111001111100", L"101101101001011", L"100010111111001", L"100000011001110", L"100111110010111", L"100101010100000", L"111011111000100", L"111001011110011",
			L"111110110101010", L"111100010011101", L"110011000101111", L"110001100011000", L"110110001000001", L"110100101110110", L"001011010001001", L"001001110111110", L"001110011100111", L"001100111010000",
			L"000011101100010", L"000001001010101", L"000110100001100", L"000100000111011", L"011010101011111", L"011000001101000", L"011111100110001", L"011101000000110", L"010010010110100", L"010000110000011",
			L"010111011011010", L"010101111101101"
		};
		for(int i6 = 0; i6 < 15; i6++)
		{			
			char byte7=as[byte6][i6]-48;			
			abyte15[abyte10[i6] & 255][abyte11[i6] & 255] = (char)(byte7 * 255);
			abyte15[abyte5[i6] & 255][abyte6[i6] & 255] = (char)(byte7 * 255);			
		}

		//boolean [][]aflag1 = new boolean[l3][l3];
		bool** aflag1 = new bool*[l3];	//\\\\\\\\\\\			
		for( n1 = 0; n1 <l3; n1++)
		{
			aflag1[n1] = new bool[l3];
		}
		int j6 = 0;
		for(int k6 = 0; k6 < l3; k6++)
		{
			for(int l6 = 0; l6 < l3; l6++)
			{
				if((abyte15[l6][k6] & byte5) != 0 || abyte12[j6] == 49)
					aflag1[l6][k6] = true;
				else
					aflag1[l6][k6] = false;
				j6++;
			}
			
			j6++;
		}
		//delete[] aflag1;
		iLenRet=l3;
		/************************************************************************/
		/*				删除new 变量											 */
		/************************************************************************/
		delete []ai;
		delete []abyte1;
		delete []abyte2;
		delete []abyte3;
		delete []abyte4;
		delete []abyte9;
		delete []abyte12;
		delete []abyte13;
		delete []abyte14;				
		
		for( n1 = 0; n1 <l3; n1++)
		{
			delete []abyte15[n1];
		}
		delete[] abyte15;
		return aflag1;
 }
	
	//char* Qrcode::divideDataBy8Bits(int ai[], char abyte0[], 
char* Qrcode::divideDataBy8Bits(int* ai, char* abyte0, 
		int i,int iLen,int& iLRet)//int i,int j)		
    {
        //int iLen = abyte0.length; iLenAi=ai..length
		int j=iLen;
        int l = 0;
        int i1 = 8;
        int j1 = 0;
        //if(j == ai.length);
        for(int i2 = 0; i2 < j; i2++)
            j1 += abyte0[i2];
		
        int k = (j1 - 1) / 8 + 1;
        char* abyte1 = new char[i];
		memset(abyte1,0,i);
        for(int j2 = 0; j2 < k; j2++)
            abyte1[j2] = 0;
		
        for(int k2 = 0; k2 < j; k2++)
        {
            int k1 = ai[k2];
            int l1 = abyte0[k2];
            bool flag = true;
            if(l1 == 0)
                break;
            while(flag) 
                if(i1 > l1)
                {
                    abyte1[l] = (char)(abyte1[l] << l1 | k1);
                    i1 -= l1;
                    flag = false;
                } else
                {
                    l1 -= i1;
                    abyte1[l] = (char)(abyte1[l] << i1 | k1 >> l1);
                    if(l1 == 0)
                    {
                        flag = false;
                    } else
                    {
                        k1 &= (1 << l1) - 1;
                        flag = true;
                    }
                    l++;
                    i1 = 8;
                }
        }
		
        if(i1 != 8)
            abyte1[l] = (char)(abyte1[l] << i1);
        else
            l--;
        if(l < i - 1)
        {
            for(bool flag1 = true; l < i - 1; flag1 = !flag1)
            {
                l++;
                if(flag1)
                    abyte1[l] = -20;
                else
                    abyte1[l] = 17;
            }
			
        }
		iLRet=i;
        return abyte1;
    }
	
    //private 
	//char* Qrcode::calculateRSECC(char abyte0[], char byte0, char abyte1[], int i, int j,
char* Qrcode::calculateRSECC(char* abyte0, char byte0, char* abyte1, int i, int j,
		int iLen0,int iLen1)
    {

	//iLen1 abyte1.length  ; iLen0 abyte0.length
        char** abyte2 = new char*[256];
		//char abyte2[][] = new char[256][byte0];
		int n1;
		for(n1=0;n1<256;n1++){
			abyte2[n1]=new char[byte0];
			memset(abyte2[n1],0,byte0);
		}
	
        try
        {            
			CString s; 
			s.Format(L"qrcode_data/rsc%d.dat", byte0);
			CFile f1(s, CFile::modeRead);
			try
			{			
				//for(int i2 = 0; i2 < 256; i2++)   bufferedinputstream.read(abyte2[i2]);
				for(int i2 = 0; i2 < 256; i2++)                
				f1.Read(abyte2[i2], byte0);
				f1.Close();
			}
			catch(...)
			{			
				f1.Close();
			}
        }
        catch(...)
        {
            //exception.printStackTrace();
        }
        bool flag = false;
        int i1 = 0;
        int k1 = 0;       
		
		char** abyte3 = new char*[iLen1];
        char* abyte4 = new char[j];
		memset(abyte4,0,j);		
		memcpy(abyte4, abyte0, iLen0);
       
		int ab3Length=0;
		for(int k = 0; k < iLen1; k++){
			ab3Length= (abyte1[k] & 255) - byte0;
            abyte3[k] = new char[(abyte1[k] & 255) - byte0];
			memset(abyte3[k],0,(abyte1[k] & 255) - byte0);
		}

       for(int l = 0; l < i; l++)
        {
         abyte3[k1][i1] = abyte0[l];
		 if(++i1 >= (abyte1[k1] & 255) - byte0)
            {
                i1 = 0;
                k1++;
            }
        }
	 /* for(int kkk=0;kkk<ab3Length;kkk++)
	  {
        FILE *fp=NULL; 
	fp=fopen("C:\\test.txt","at+");	
	fprintf(fp,"%d:  %d\r\n",kkk,abyte3[3][kkk]);
	fclose(fp);
	  }*/

		
        //for(int l1 = 0; l1 < abyte1.length; l1++)
		for(int l1 = 0; l1 < iLen1; l1++)
        {
            //char abyte5[] = (char[])abyte3[l1].clone();			
			int iLen5=(abyte1[l1] & 255) - byte0;
			char* abyte5=new char[iLen5];
			memset(abyte5,0,iLen5);
			for (n1=0;n1<iLen5;n1++)
			{
				abyte5[n1]=abyte3[l1][n1];
			}
			//char* abyte5 = abyte3[l1];
			
            int j2 = abyte1[l1] & 255;
            int k2 = j2 - byte0;
            for(int j1 = k2; j1 > 0; j1--)
            {
                char byte1 = abyte5[0];
                if(byte1 != 0)
                {
                    //char abyte6[] = new char[abyte5.length - 1];
					char* abyte6 = new char[iLen5];
					memcpy(abyte6,abyte5+1,iLen5-1);
					abyte6[iLen5-1]=0;
                    //System.arraycopy(abyte5, 1, abyte6, 0, iLen5 - 1);
                    //char abyte9[] = abyte2[byte1 & 255];
					char* abyte9 = abyte2[byte1 & 255];
					int iLret1;
					abyte5 = calculateByteArrayBits(abyte6, abyte9, L"xor", iLen5 - 1, byte0, iLret1);
					//abyte5[iLen5-1]=0;					
					delete [] abyte6;
                } else				
					if(byte0 < iLen5)
					{
						//char abyte7[] = new char[abyte5.length - 1];
						char* abyte7 = new char[iLen5 - 1];
						memcpy(abyte7,abyte5+1,iLen5-1);
						for(n1=0;n1<iLen5-1;n1++)
							abyte5[n1]=abyte7[n1];
						//abyte5 = abyte7;
						delete []abyte7;
					} else
					{						
						//char abyte8[] = new char[byte0];
						char* abyte8 = new char[byte0];
						memcpy(abyte8,abyte5+1,iLen5-1);						
					//	abyte8[byte0 - 1] = 0;	
						for(n1=0;n1<byte0;n1++)
							abyte5[n1]=abyte8[n1];
						//abyte5 = abyte8;
						delete []abyte8;
					}

            }
			memcpy(abyte4+iLen0+l1*byte0,abyte5,byte0);	
			delete []abyte5;
        }
	
		for(n1=0;n1<256;n1++){			
			delete []abyte2[n1];
		}
		delete []abyte2;
		

		for(n1=0;n1<iLen1;n1++){			
			delete []abyte3[n1];
		}
		delete []abyte3;

        return abyte4;
    }
	
    //private 
char* Qrcode::calculateByteArrayBits(char* abyte0, char* abyte1, CString s,
		int iLen0,int iLen1,int& iLRet)
    {
		char* abyte3;//
		char* abyte4;	
		int i,j,n1;
		if(iLen0 > iLen1)
        {            
			i=iLen0;			
			j=iLen1;			
			abyte3=new char[i+1];
			abyte4=new char[j+1];			
			memset(abyte3,0,i+1);
			for (n1=0;n1<i;n1++){
				abyte3[n1]=abyte0[n1];			
			}
			memset(abyte4,0,j+1);
			for (n1=0;n1<j;n1++)
			{
				abyte4[n1]=abyte1[n1];			
			}
        } else
        {            
			i=iLen1;		
			j=iLen0;	
			abyte3=new char[i+1];
			abyte4=new char[j+1];
			memset(abyte3,0,i+1);
			for (n1=0;n1<i;n1++)
			{				
				abyte3[n1]=abyte1[n1];			
			}
			memset(abyte4,0,j+1);
			for (n1=0;n1<j;n1++)
			{
				abyte4[n1]=abyte0[n1];			
			}
        }

		char* abyte2 = new char[i+1];
		memset(abyte2,0,i+1);
        for(int k = 0; k < i; k++){
            if(k < j)
            {
                if(s == "xor")
                    abyte2[k] = (char)(abyte3[k] ^ abyte4[k]);
                else
                    abyte2[k] = (char)(abyte3[k] | abyte4[k]);
            } else
            {
                abyte2[k] = abyte3[k];
            }			
			
		}
		delete []abyte3;
		delete []abyte4;		
		return abyte2;
	}
	

void Qrcode::TestMsg(char* myStr,int length) 
{ 
//int kkk = strlen(myStr);
for(int iii=0;iii<length;iii++)
{
	//FILE *fp=NULL; 
	//fp = fopen("C:\\test.txt", L"at+");
	//fprintf(fp,"%d:  %d\r\n",iii,myStr[iii]);
	//fclose(fp);
}
}

char Qrcode::selectMask(char** abyte0, int i,int iLen0)
{
	//iLen0 abyte0.GetLength();
	//int j = abyte0.GetLength();		
	int j = iLen0;
	int ai[] = {
		0, 0, 0, 0, 0, 0, 0, 0
	};


	int ai1[] = {
		0, 0, 0, 0, 0, 0, 0, 0
	};
	int ai2[] = {
		0, 0, 0, 0, 0, 0, 0, 0
	};
	int ai3[] = {
		0, 0, 0, 0, 0, 0, 0, 0
	};
	int k = 0;
	int l = 0;
	int ai4[] = {
		0, 0, 0, 0, 0, 0, 0, 0
	};
	
	for(int i1 = 0; i1 < j; i1++)
	{
		int ai5[] = {
			0, 0, 0, 0, 0, 0, 0, 0
		};
		int ai6[] = {
			0, 0, 0, 0, 0, 0, 0, 0
		};
		bool aflag[] = {
			false, false, false, false, false, false, false, false
		};
		bool aflag1[] = {
			false, false, false, false, false, false, false, false
		};
	
for(int l1 = 0; l1 < j; l1++)
{
if(l1 > 0 && i1 > 0)
{
	k = abyte0[l1][i1] & abyte0[l1 - 1][i1] & abyte0[l1][i1 - 1] & abyte0[l1 - 1][i1 - 1] & 255;
	l = abyte0[l1][i1] & 255 | abyte0[l1 - 1][i1] & 255 | abyte0[l1][i1 - 1] & 255 | abyte0[l1 - 1][i1 - 1] & 255;
}
		
      for(int j2 = 0; j2 < 8; j2++)
			{
				// ai5[j2] = (ai5[j2] & 63) << 1 | (abyte0[l1][i1] & 255) >>> j2 & 1;
				// ai6[j2] = (ai6[j2] & 63) << 1 | (abyte0[i1][l1] & 255) >>> j2 & 1;//高位自动补零
				
				int n=abyte0[l1][i1] & 255;
				int x=j2;				

				ai5[j2] = (ai5[j2] & 63) << 1 |(n>>x)&((int)pow((double)2,(double)(32-x))-1) & 1;
				ai6[j2] = (ai6[j2] & 63) << 1 |(n>>x)&((int)pow((double)2,(double)(32-x))-1) & 1;
				//ai5[j2] = (ai5[j2] & 63) << 1 |((abyte0[l1][i1] & 255))>> j2 & 1;
				//ai6[j2] = (ai6[j2] & 63) << 1 |((abyte0[i1][l1] & 255))>> j2 & 1;
				// ai5[j2] = (ai5[j2] & 63) << 1 | (abyte0[l1][i1] & 255) >> j2 & 1;
				// ai6[j2] = (ai6[j2] & 63) << 1 | (abyte0[i1][l1] & 255) >> j2 & 1;//高位自动补零
			   
				
				if((abyte0[l1][i1] & 1 << j2) != 0)
					ai4[j2]++;
				if(ai5[j2] == 93)
					ai2[j2] += 40;
				if(ai6[j2] == 93)
					ai2[j2] += 40;
				if(l1 > 0 && i1 > 0)
				{
					if((k & 1) != 0 || (l & 1) == 0)
						ai1[j2] += 3;
					k >>= 1;
					l >>= 1;
				}
				if((ai5[j2] & 31) == 0 || (ai5[j2] & 31) == 31)
				{
					if(l1 > 3)
						if(aflag[j2])
						{
							ai[j2]++;
						} else
						{
							ai[j2] += 3;
							aflag[j2] = true;
						}
				} else
				{
					aflag[j2] = false;
				}
				if((ai6[j2] & 31) == 0 || (ai6[j2] & 31) == 31)
				{
					if(l1 > 3)
						if(aflag1[j2])
						{
							ai[j2]++;
						} else
						{
							ai[j2] += 3;
							aflag1[j2] = true;
						}
				} else
				{
					aflag1[j2] = false;
				}
			}
			
		}
			
	}
	

	int j1 = 0;
	char byte0 = 0;
	int ai7[] = {
		90, 80, 70, 60, 50, 40, 30, 20, 10, 0, 
			0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 
			90
	};
	for(int k1 = 0; k1 < 8; k1++)
	{
		ai3[k1] = ai7[(20 * ai4[k1]) / i];
		int i2 = ai[k1] + ai1[k1] + ai2[k1] + ai3[k1];
		if(i2 < j1 || k1 == 0)
		{
			byte0 = (char)k1;
			j1 = i2;
		}
	}
	
	return byte0;
}
