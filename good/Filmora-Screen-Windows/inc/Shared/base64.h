////////////////////////////////////////////////////////////
// base64.h

// 参考文章：http://www.cstc.net.cn/docs/docs.php?id=202

//************************************************************************/
//    base64编码表
// 
//		0 A 17 R 34 i 51 z 
//		1 B 18 S 35 j 52 0 
//		2 C 19 T 36 k 53 1 
//		3 D 20 U 37 l 54 2 
//		4 E 21 V 38 m 55 3 
//		5 F 22 W 39 n 56 4 
//		6 G 23 X 40 o 57 5 
//		7 H 24 Y 41 p 58 6 
//		8 I 25 Z 42 q 59 7 
//		9 J 26 a 43 r 60 8 
//		10 K 27 b 44 s 61 9 
//		11 L 28 c 45 t 62 + 
//		12 M 29 d 46 u 63 / 
//		13 N 30 e 47 v (pad) = 
//		14 O 31 f 48 w 
//		15 P 32 g 49 x 
//		16 Q 33 h 50 y 
//
// base64编码步骤：
// 
//		原文：
//
//		你好
//		C4 E3 BA C3
//		11000100 11100011 10111010 11000011
//		00110001 00001110 00001110 00111010
//		49       14							（十进制）
//		x        O        O        6		字符
//		01111000 01001111 01001111 00110110
//		78									（十六进制）
//		xOO6
//
//		解码：
//		xOO6
//		78 4f 4f 36
//		01111000 01001111 01001111 00110110
//		49       14 
//		00110001 00001110 00001110 00111010   31 0e 0e 3a
//
//		11000100 11100011 10111010
//		C4       E3       BA
//

#ifndef _BASE64_INCLUDE__H__
#define _BASE64_INCLUDE__H__

#include "bytewap.h"

// 编码后的长度一般比原文多占用1/3的存储空间，请保证base64code有足够的空间
inline int Base64Encode(char * base64code, const unsigned char * src, int src_len); 
inline int Base64Decode(unsigned char * buf, const char * base64code, int src_len = -1);


const char g_szBase64Table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

#define GetB64Char(index) g_szBase64Table[index]

// 从双字中取单字节
#define B0(a) (a & 0xFF)
#define B1(a) (a >> 8 & 0xFF)
#define B2(a) (a >> 16 & 0xFF)
#define B3(a) (a >> 24 & 0xFF)

// 编码后的长度一般比原文多占用1/3的存储空间，请保证base64code有足够的空间
inline int Base64Encode(char * base64code, const unsigned char * src, int src_len) 
{   
	int len = 0;
	const unsigned char* psrc = src;
	char * p64 = base64code;
	int i = 0;
	for (i = 0; i < src_len - 3; i += 3)
	{
		unsigned long ulTmp = *(unsigned long*)psrc;
		_B2N_32( ulTmp );
				
		register int b0 = GetB64Char((B0(ulTmp) >> 2) & 0x3F); 
		register int b1 = GetB64Char((B0(ulTmp) << 6 >> 2 | B1(ulTmp) >> 4) & 0x3F); 
		register int b2 = GetB64Char((B1(ulTmp) << 4 >> 2 | B2(ulTmp) >> 6) & 0x3F); 
		register int b3 = GetB64Char((B2(ulTmp) << 2 >> 2) & 0x3F); 
		
		//*((unsigned long*)p64) = b0 | b1 << 8 | b2 << 16 | b3 << 24;
		p64[0] = b0;
		p64[1] = b1;
		p64[2] = b2;
		p64[3] = b3;		
		len += 4;
		p64  += 4; 
		psrc += 3;
	}
	
	// 处理最后余下的不足3字节的饿数据
	if (i < src_len)
	{
		int rest = src_len - i;
		unsigned long ulTmp = 0;
		for (int j = 0; j < rest; ++j)
		{
			*(((unsigned char*)&ulTmp) + j) = *psrc++;
		}
		_B2N_32( ulTmp );
				
		p64[0] = GetB64Char((B0(ulTmp) >> 2) & 0x3F); 
		p64[1] = GetB64Char((B0(ulTmp) << 6 >> 2 | B1(ulTmp) >> 4) & 0x3F); 
		p64[2] = rest > 1 ? GetB64Char((B1(ulTmp) << 4 >> 2 | B2(ulTmp) >> 6) & 0x3F) : '='; 
		p64[3] = rest > 2 ? GetB64Char((B2(ulTmp) << 2 >> 2) & 0x3F) : '='; 
		p64 += 4; 
		len += 4;
	}
	
	*p64 = '\0'; 
	
	return len;
}

/*
__inline int GetB64Index(char ch)
{
	int index = -1;
	if (ch >= 'A' && ch <= 'Z')
	{
		index = ch - 'A';
	}
	else if (ch >= 'a' && ch <= 'z')
	{
		index = ch - 'a' + 26;
	}
	else if (ch >= '0' && ch <= '9')
	{
		index = ch - '0' + 52;
	}
	else if (ch == '+')
	{
		index = 62;
	}
	else if (ch == '/')
	{
		index = 63;
	}

	return index;
}
*/

const char g_Base64IndexTab[] =
{
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		62,        // '+'
		0, 0, 0,
		63,        // '/'
		52, 53, 54, 55, 56, 57, 58, 59, 60, 61,        // '0'-'9'
		0, 0, 0, 0, 0, 0, 0,
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
		13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,        // 'A'-'Z'
		0, 0, 0, 0, 0, 0,
		26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
		39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51,        // 'a'-'z'
};

#define GetB64Index(ch) g_Base64IndexTab[ch]

// 解码后的长度一般比原文少占用1/4的存储空间，请保证buf有足够的空间
inline int Base64Decode(unsigned char * buf, const char * base64code, int src_len) 
{   
	int s_len = (int)strlen(base64code);
	if (src_len == -1 || src_len > s_len)
		src_len = s_len;

	int len = 0;
	const char* psrc = base64code;
	unsigned char * pbuf = buf;
	int i = 0;
	for (i = 0; i < src_len - 4; i += 4)
	{
		unsigned long ulTmp = *(unsigned long*)psrc;
		_B2N_32( ulTmp );
		
		register int b0 = (GetB64Index((char)B0(ulTmp)) << 2 | GetB64Index((char)B1(ulTmp)) << 2 >> 6) & 0xFF;
		register int b1 = (GetB64Index((char)B1(ulTmp)) << 4 | GetB64Index((char)B2(ulTmp)) << 2 >> 4) & 0xFF;
		register int b2 = (GetB64Index((char)B2(ulTmp)) << 6 | GetB64Index((char)B3(ulTmp)) << 2 >> 2) & 0xFF;
		
		//*((unsigned long*)pbuf) = b0 | b1 << 8 | b2 << 16;
		pbuf[0] = b0;
		pbuf[1] = b1;
		pbuf[2] = b2;
		psrc  += 4; 
		pbuf += 3;
		len += 3;
	}

	// 处理最后余下的不足4字节的数据
	if (i < src_len)
	{
		int rest = src_len - i;
		unsigned long ulTmp = 0;
		for (int j = 0; j < rest; ++j)
		{
			*(((unsigned char*)&ulTmp) + j) = *psrc++;				
		}
		_B2N_32( ulTmp );
						
		register int b0 = (GetB64Index((char)B0(ulTmp)) << 2 | GetB64Index((char)B1(ulTmp)) << 2 >> 6) & 0xFF;
		*pbuf++ = b0;
		len  ++;

		if ('=' != B1(ulTmp) && '=' != B2(ulTmp))
		{
			register int b1 = (GetB64Index((char)B1(ulTmp)) << 4 | GetB64Index((char)B2(ulTmp)) << 2 >> 4) & 0xFF;
			*pbuf++ = b1;
			len  ++;
		}
		
		if ('=' != B2(ulTmp) && '=' != B3(ulTmp))
		{
			register int b2 = (GetB64Index((char)B2(ulTmp)) << 6 | GetB64Index((char)B3(ulTmp)) << 2 >> 2) & 0xFF;
			*pbuf++ = b2;
			len  ++;
		}

	}
		
	*pbuf = '\0'; 
	
	return len;
}

#endif // #ifndef _BASE64_INCLUDE__H__


