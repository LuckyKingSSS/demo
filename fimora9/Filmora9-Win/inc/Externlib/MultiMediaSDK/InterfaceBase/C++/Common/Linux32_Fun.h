// Linux32_Typedef.h
//

#pragma once
#include <sys/types.h>
#include <sys/stat.h>
#include "strconv.h"
#ifndef WIN32
#include <fcntl.h>
#include <unistd.h>
#define _stat	stat
#define _atoi64	atoll

struct __stat64
{
	__int64 st_size;
};

static __inline int _wstat(const wchar_t *path, struct stat *sb)
{
	USES_CONVERSION;
	return stat(W2A(path), sb);
}

static __inline int _wstat64(const wchar_t *path, struct __stat64 *buffer)
{
	USES_CONVERSION;
	int fd = open(W2A(path), O_RDONLY);
	if (fd > 0)
	{
		buffer->st_size = lseek(fd, 0, SEEK_END);
		close(fd);
		return (buffer->st_size >= 0 ? 0 : -1);
	}
	return -1;
}

static __inline FILE *_wfopen(const wchar_t *pPath, const wchar_t *pMode)
{
	USES_CONVERSION;
	char *pCMode = new char[wcslen(pMode) + 10];
	strcpy(pCMode, W2A(pMode));
	FILE *pFile = fopen(W2A(pPath), pCMode);
	delete[] pCMode;
	
	return pFile;
}

static __inline int _wmkdir(const wchar_t *dirname)
{
	USES_CONVERSION;
	char *pcPath = new char[wcslen(dirname) + 10];
	strcpy(pcPath, W2A(dirname));
	int nRet = mkdir(pcPath, 0666);
	if (!nRet)
	{
		nRet = chmod(pcPath, 0777);
	}
	delete[] pcPath;
	pcPath = NULL;
	return nRet;
};

static __inline int _wrmdir(const wchar_t *dirname)
{
	USES_CONVERSION;
	return rmdir(W2A(dirname));
}

static __inline int _wremove(const wchar_t *path)
{
	USES_CONVERSION;
	return remove(W2A(path));
}

static __inline void wsitoa(int i,char*string,int index = 10)
{
	int power,j;
	j=i;
	for(power=1;j>=10;j/=10)
		power*=10;
	for(;power>0;power/=10)
	{
		*string++='0'+i/power;
		i%=power;
	}
	*string='\0';
}

#ifndef WIN32
#define _fseeki64 fseeko
#define _ftelli64 ftello
#define DeleteFile _wremove
#define itoa  wsitoa
#endif

#endif



