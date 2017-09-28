/*
 *  ServerSocket.h
 *  TestSocket
 *
 *  Created by jyy on 5/27/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */
#pragma once
#ifndef SERVERSOCKET_H
#define SERVERSOCKET_H
#include <Carbon/Carbon.h>
#include "Linux32_Typedef.h"
#include "BaseThread.h"
#include "SingleLock.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>  
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>
#include "CarbonEvent.h"
#include <list>
using namespace std;

#ifndef MSGPROTOCOL  //定义数据格式协议
#define MSGPROTOCOL
enum CMDValue
{
	PAUSE,
	RESUME,
	STOP,
	ABORT,
	GETGPROGRESS,
	//GETSTATE,
	SUCESSFUL,    //child process finish trans
	FAILTRANS
};
#endif

#ifndef PROCESSSTATE
#define PROCESSSTATE
enum ChildProcessState
{
	CREATE,
	RUNING
};
#endif

#ifndef PROCESSSOCKETINFO
#define PROCESSSOCKETINFO
	struct stuProcessSocketInfo
	{
		int nChildProcessID;
		int nSocketID;
		char *pCmdLine;
		char *pExecPath;
		ChildProcessState TransState;
	};
#endif

#ifndef SERVERPORT
#define SERVERPORT 1024 * 2 + 1000//端口号
#endif


#define WM_TRANSWAIT	(0X400 + 1024)
#define WM_TRANSSTRAT	(0X400 + 1025)
#define WM_ABORT		(0X400 + 1026)
#define WM_FINISH		(0X400 + 1027)

class ServerSocket : public BaseThread
{
public:
	ServerSocket(void)
	{
		m_pMsg = NULL;
		m_ServerSocketID = -1;
		m_bClose = TRUE;
		m_hWnd = NULL;	
	}
	
	virtual ~ServerSocket()
	{
		Close();
	}
	
public:
	BOOL  Open( HWND hWnd = NULL)
	{
		//if (NULL == hWnd)
		//return FALSE;
		
		Close();
		
		//创建服务端
		m_ServerSocketID = socket(AF_INET, SOCK_STREAM, 0);
		
		int on = 1;
		int ret = setsockopt(m_ServerSocketID, SOL_SOCKET, SO_REUSEPORT, &on, sizeof(on));
		if (ret < 0)
			printf("setsockopt %d", errno);
		
		struct sockaddr_in serAddr;
		memset(&serAddr, 0, sizeof(serAddr));
		
		serAddr.sin_port = htons(SERVERPORT);
		serAddr.sin_family = AF_INET;
		serAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
		ret = bind(m_ServerSocketID, (sockaddr *) &serAddr, sizeof(serAddr));
		if (ret < 0)
		{
			printf("bind fail %d\r\n", errno);
			return FALSE;
		}
		ret = listen(m_ServerSocketID, 5);
		if (ret < 0)
		{
			printf("linsten fail %d\r\n", errno);
			return FALSE;
		}
		
		m_pMsg = new BYTE[256];
		m_nProcessID = 0;
		m_bClose = FALSE;
		m_hWnd = hWnd;
		
		Start();
		
		return TRUE;
	}
	
	void  Close(void)
	{
		int nCMD = STOP;
		
		Abort();	
		CSingleLock sl_seek(&m_csSeek);
		
		list<stuProcessSocketInfo>::iterator iter;
		for (iter = listChildProcess.begin(); iter != listChildProcess.end(); iter++)
		{
			
			if (!SendMsg(iter->nChildProcessID, nCMD, 1, 0))
			{
				CloseChildSock(iter->nChildProcessID, TRUE, FALSE);
			}
			//close(iter->nSocketID);
		}
		
		do
		{
			for (iter = listChildProcess.begin(); iter != listChildProcess.end(); iter++)
			{
				BOOL bTransRet;
				int nRet = GetMsg(iter->nChildProcessID, 0, 100);
				if (SUCESSFUL == nRet)
					bTransRet = TRUE;
				else
					bTransRet = FALSE;
				
				CloseChildSock(iter->nChildProcessID, TRUE, bTransRet);
			}
			Sleep(1);
		} while(!listChildProcess.empty());
		
		
		if (m_ServerSocketID > -1)
		{
			close(m_ServerSocketID);
			m_ServerSocketID = -1;
		}
		
		if (m_pMsg)
		{
			delete[] m_pMsg;
			m_pMsg = NULL;
		}
		
		listChildProcess.clear();
		
		m_bClose = TRUE;
		m_hWnd = NULL;	
	}
	
	int	  GetChildProgress(int nChildProcessID)
	{
		int ret = -1;
		int nCMD = GETGPROGRESS;
		
		if (!m_bClose)
		{	
			CSingleLock sl_seek(&m_csSeek);
			
			if (SendMsg(nChildProcessID, nCMD, 1, 0))
			{
				Sleep(1);
				ret = GetMsg(nChildProcessID, 1, 0);
			}
		}
		
		//printf(" GetChildProgress(int nChildProcessID) %d\r\n", ret);
		return ret;	
	}
	
	int   CreateChildProcess(const char *pCommandLine, const int nCMDLen, const char *pExcutePath, const int nPathLen, int nProcessID)  //返回进程的ID
	{
		if (!m_bClose)
		{
			CSingleLock sl_seek(&m_csSeek);
			
			if (pCommandLine && pExcutePath && (nCMDLen > 0) && (nPathLen > 0))
			{	
				list<stuProcessSocketInfo>::iterator iter;
				for (iter = listChildProcess.begin(); iter != listChildProcess.end(); iter++)
				{
					if (iter->nChildProcessID == nProcessID)
					{
						return -1;
					}
				}
				
				m_nProcessID = nProcessID;
				stuProcessSocketInfo chartSocketInfo;
				chartSocketInfo.nChildProcessID = m_nProcessID;
				chartSocketInfo.nSocketID = -1;
				
				chartSocketInfo.pCmdLine = new char [nCMDLen];
				memcpy(chartSocketInfo.pCmdLine, pCommandLine, nCMDLen);
				chartSocketInfo.pCmdLine[nCMDLen] = '\0';
				
				chartSocketInfo.pExecPath = new char[nPathLen];
				memcpy(chartSocketInfo.pExecPath, pExcutePath, nPathLen);
				chartSocketInfo.pExecPath[nPathLen] = '\0';
				
				chartSocketInfo.TransState = CREATE;
				listChildProcess.push_back(chartSocketInfo);
				
				if (listChildProcess.size() == 1)
				{
					StartTrans(m_nProcessID);
				}
				else
				{
					PostMessage(m_hWnd, WM_TRANSWAIT, m_nProcessID, 0);
				}
				
				return m_nProcessID;
			}
			else
			{
				return -1;
			}
		}
		else
		{
			return -1;
		}	
	}

	void  PauseChildProcess(int nChildProcessID)
	{
		int nCMD = PAUSE;
		
		CSingleLock sl_seek(&m_csSeek);

		SendMsg (nChildProcessID, nCMD, 1, 0);
		Sleep(1);
	}
	
	void  ResumeChildProcess(int nChildProcessID)
	{
		int nCMD = RESUME;

		CSingleLock sl_seek(&m_csSeek);
		
		SendMsg(nChildProcessID, nCMD, 1, 0);
		Sleep(1);
	}
	
	//void  StopChildProcess(int nChildProcessID);
	void  AbortChildProcess(int nChildProcessID)
	{
		int nCMD = ABORT;
		CSingleLock sl_seek(&m_csSeek);
		
		if (SendMsg (nChildProcessID, nCMD, 1, 0))
		{
			Sleep(1);
			do
			{
				BOOL bTransRet = FALSE;
				int nRet = GetMsg(nChildProcessID, 0, 100);
				if (-1 == nRet)
					continue;
					
				if (SUCESSFUL == nRet)
					bTransRet = TRUE;
				else
					bTransRet = FALSE;
						
				CloseChildSock(nChildProcessID, TRUE, bTransRet);
					
				list<stuProcessSocketInfo>::iterator iterNewTrans = listChildProcess.begin();
				if (iterNewTrans != listChildProcess.end())
				{
					//printf("StartTrans(iterNewTrans->nChildProcessID)\r\n");
					StartTrans(iterNewTrans->nChildProcessID);
				}
					
				break;
				
				Sleep(1);
			} while(1);
			
		}
		else
		{
			CloseChildSock(nChildProcessID, TRUE, FALSE);
		}
	}
	
	//int   GetChildProcessSatate(const int nChildProcessID);  //获取子进程转换状态信息,返回-1，那么表示出错

private:
	BOOL SendMsg(const int nChildProcessID, const int nCMD, __darwin_time_t tv_sec, __darwin_suseconds_t tv_usec)
	{
		BOOL bRet = FALSE;
		
		if (!m_bClose)
		{
			list<stuProcessSocketInfo>::iterator iter;
			
			for (iter = listChildProcess.begin(); iter != listChildProcess.end(); iter++)
			{
				if ((iter->nChildProcessID == nChildProcessID) && (iter->TransState != CREATE))
				{
					if (IfWriteAvailable(iter->nSocketID, tv_sec, tv_usec))
					{
						memset(m_pMsg, '\0', sizeof(int));
						INT2LECHAR(nCMD, m_pMsg);
						write(iter->nSocketID, (const char *) m_pMsg, sizeof(int));
						bRet = TRUE;
						
						break;
					}
					/*
					 else  //该进程已经退出，不可写了
					 {
						 close(iter->nSocketID);
						 CloseChildSock(iter->nChildProcessID);
					 }
					 */
				}
			}
		}
		
		return bRet;
	}
	int  GetMsg(const int nChildProcessID, __darwin_time_t tv_sec, __darwin_suseconds_t	tv_usec)  // 可能陷入死循环,暂时不考虑，等测试的是否分析
	{
		int ret = -1;
		list<stuProcessSocketInfo>::iterator iter;
		
		if (!m_bClose)
		{
			for (iter = listChildProcess.begin(); iter != listChildProcess.end(); iter++)
			{
				if ((iter->nChildProcessID == nChildProcessID) && (iter->TransState != CREATE))
				{
					memset(m_pMsg, '\0', sizeof(int));
					
					if (IfReadAvailable(iter->nSocketID, tv_sec, tv_usec))
					{
						int nNum = read(iter->nSocketID, m_pMsg, sizeof(int));
						if (nNum > 0)
						{
							LECHAR2INT(m_pMsg, ret);
							break;
						}
					}
					break;
				}
			}
		}
		//printf ("read message is %d\r\n", ret);
		
		return ret;	
	}
	
	void CloseChildSock(const int	nChildProcessID, BOOL bUserAbort, BOOL bTransRet)
	{
		list<stuProcessSocketInfo>::iterator iter;

		for (iter = listChildProcess.begin(); iter != listChildProcess.end(); iter++)
		{
			if (nChildProcessID == iter->nChildProcessID)
			{
				//printf ("the child process %d is finish \r\n", nChildProcessID);
				//if (m_hWnd)
				int nMsgType;
				if (bUserAbort)
					nMsgType = WM_ABORT;
				else
					nMsgType = WM_FINISH;
				
				int nTransRet;
				if (bTransRet)
					nTransRet = 1;
				else
					nTransRet = 0;
							
				PostMessage(m_hWnd, nMsgType, nChildProcessID, nTransRet);
					
				if (iter->TransState != CREATE)	
					close(iter->nSocketID);
				
				delete[] iter->pCmdLine;
				iter->pCmdLine = NULL;
				delete[] iter->pExecPath;
				iter->pExecPath = NULL;
				listChildProcess.erase(iter);
				
				break;
			}
		}
	}
	
	BOOL IfReadAvailable(const int nChildSocket, int sec, int usec)  //判断是否可读
	{
		int iRet; 
		fd_set fdRead; 
		struct timeval tvLeft = {sec, usec}; 
		
		if (!m_bClose)
		{
			FD_ZERO(&fdRead); 
			FD_SET(nChildSocket, &fdRead); 
			iRet = select(nChildSocket + 1, &fdRead, NULL, NULL, &tvLeft);
			if(iRet == EBADF) 
			{
				return FALSE;
			}
			if (FD_ISSET(nChildSocket,&fdRead))//if可以读出...
			{
				return TRUE;
			}
			else
			{
				return FALSE;
			}
		}
		else
		{
			return FALSE;
		}	
	}
	
	BOOL IfWriteAvailable(const int nChildSocket,  __darwin_time_t tv_sec, __darwin_suseconds_t tv_usec)
	{
		int iRet; 
		fd_set fdWrite; 
		struct timeval tvLeft = {tv_sec, tv_usec};
		
		if (!m_bClose)
		{ 
			FD_ZERO(&fdWrite); 
			FD_SET(nChildSocket, &fdWrite); 
			iRet = select(nChildSocket + 1, NULL, &fdWrite, NULL, &tvLeft);
			if(iRet == EBADF) 
			{
				return FALSE;
			}
			if (FD_ISSET(nChildSocket,&fdWrite))//if可以写入...
			{
				return TRUE;
			}
			else
			{
				return FALSE;
			}
		}
		else
		{
			return FALSE;
		}	
	}
	
	BOOL  StartTrans(int nChildProcessID)
	{
		do
		{
			if (m_bClose)
				break;
			
			list<stuProcessSocketInfo>::iterator iter;
			for (iter = listChildProcess.begin(); iter != listChildProcess.end(); iter++)
			{
				if (nChildProcessID == iter->nChildProcessID)
				{
					//printf("iter->TransState = %d\r\n", iter->TransState);
					if (iter->TransState != CREATE)	
						break;
					
					char *pCmdLine = new char [strlen(iter->pCmdLine) + strlen(iter->pExecPath) + 256];
					sprintf(pCmdLine, "exec \"%s\" %s\0", iter->pExecPath, iter->pCmdLine);
					_beginthreadex (NULL, 0, threadFun, pCmdLine, 0, 0);
					
					Sleep(1);	
					int chartSocket = -1;
					
					do
					{
						chartSocket = accept(m_ServerSocketID, NULL, NULL);	
						if (chartSocket > 0)
							break;
						else if (errno !=  EINTR)
							break;	
					} while (1);
					
					if (chartSocket > 0)
					{
						iter->TransState = RUNING;				
						iter->nSocketID = chartSocket;
						//if (m_hWnd)
						PostMessage(m_hWnd, WM_TRANSSTRAT, nChildProcessID, 0);
						//printf("........................WM_TRANSSTRAT \r\n");
						
						return TRUE;
					}
					else
					{
						//printf("accept(m_ServerSocketID, NULL, NULL); fail %d \r\n", errno);	
						break;
					}
				} //if (nChildProcessID == iter->nChildProcessID)
			} //for (iter = listChildProcess.begin()
	} while (0);
		
		//printf("CloseChildSock(nChildProcessID, FALSE, FALSE);\r\n");
		CloseChildSock(nChildProcessID, FALSE, FALSE);
		
		return FALSE;	
	}
	
	__inline void  INT2LECHAR(const int nValue, BYTE *pReturn);
	__inline void LECHAR2INT(const BYTE *pValue, int &nReturn);
	
	static void* __stdcall threadFun(void *pCmdLine)
	{	
		system((const char *) pCmdLine);
		
		delete[] pCmdLine;
		pCmdLine = 0;
	}	
	
protected:
	virtual void svc(void)
	{
		int iRet;
		
		for (;;)
		{
			Sleep(1);
			CSingleLock sl_seek(&m_csSeek);
			
			IFABORTEVENT_BREAK(0);
			
			list<stuProcessSocketInfo>::iterator iter;
			for (iter = listChildProcess.begin(); iter != listChildProcess.end(); iter++)
			{
				if ((iter->TransState != CREATE) && IfReadAvailable(iter->nSocketID, 0, 50))
				{
					memset(m_pMsg, 0, sizeof(int));
					int nNum = read(iter->nSocketID, m_pMsg, sizeof(int));
					if (nNum > 0)
					{
						int nMsg;
						LECHAR2INT(m_pMsg, nMsg);
						if ((SUCESSFUL == nMsg) || (FAILTRANS == nMsg))
						{
							if (SUCESSFUL == nMsg)
								CloseChildSock(iter->nChildProcessID, FALSE, TRUE);
							else
								CloseChildSock(iter->nChildProcessID, FALSE, FALSE);
							
							list<stuProcessSocketInfo>::iterator iterNewTrans = listChildProcess.begin();
							if (iterNewTrans != listChildProcess.end())
							{
								StartTrans(iterNewTrans->nChildProcessID);
							}
							
							break;
						}
					} //if (IfReadAvailable(i
			}
		}  //for (iter = listChildProcess.begin();
	}	
	}
	
private:
	int m_ServerSocketID;
	list<stuProcessSocketInfo> listChildProcess;
	BYTE *m_pMsg;
	CCritical		m_csSeek;
	int		m_nProcessID;
	BOOL	m_bClose;
	 HWND	m_hWnd;
};

__inline void  ServerSocket::INT2LECHAR(const int nValue, BYTE *pReturn)  //
{
	if (pReturn)
	{
		*pReturn = nValue >> 3;
		pReturn++;
		*pReturn = (nValue & 0x00ff0000) >> 2;
		pReturn++;
		*pReturn = (nValue & 0x0000ff00) >> 1;
		pReturn++;
		*pReturn  = nValue & 0x000000ff;
	}
}

__inline void ServerSocket::LECHAR2INT(const BYTE *pValue, int &nReturn)
{
	if (pValue)
	{
		nReturn = (((int) (*pValue)) << 3) | (((int) *(pValue + 1)) << 2) | (((int) *(pValue + 2)) << 1) | (int) *(pValue + 3);
	}
}
#endif