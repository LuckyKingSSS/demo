///////////////////////////////////////////////////////////////////////////
// Crypt.h
//==========================================================================
//==========================================================================

#pragma once

#include "base64.h"

__inline const char* WSEncodeStr(const char* src, char* dst)
{
	int len = strlen(src);
	char* tmp = new char[len + 1];
	
	for (int i = 0; i < len; ++i)
	{
		char c = src[i];
		c = ~c;
		tmp[i] = c - 1;
	}
	
	int n = Base64Encode(dst, (const unsigned char *)tmp, len);
	dst[n] = '\0';
	
	delete [] tmp;
	
	return dst;
}

__inline const char* WSDecodeStr(const char* src, char* dst)
{
	int len = strlen(src);
	int n = Base64Decode((unsigned char *)dst, src, len);
	
	for (int i = 0; i < n; ++i)
	{
		dst[i] = (char)(~(dst[i] + 1));
	}
	dst[n] = '\0';
	
	return dst;
}


