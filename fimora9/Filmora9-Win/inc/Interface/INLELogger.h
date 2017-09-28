/**                                     
 * Copyright (c) 2016 Wondershare.inc   
 * All rights reserved.                 
 *                                      
 *@file  INLELogger.h
 *@brief                                
 *@version 1.0                          
 *@author  yaoyj                       
 *@date    3/24/2016 18:57
 */

#ifndef _H_INLELOG_H_
#define _H_INLELOG_H_


#include "NLEType.h"                                                          
#include <Guiddef.h>                                                            
#include <objbase.h>                                                            
#include <stdio.h>

/**
@brief 日志系统中可通过设置日志等级来过滤日志信息，所有高于或等于设定等级的日志才会输出，
系统的日志等级高低关系为： 
NLELogLevel_FATAL > NLELogLevel_ERROR > NLELogLevel_WARNING > NLELogLevel_NOTICE 
> NLELogLevel_INFO > NLELogLevel_TRACE

系统默认的等级为NLELogLevel_INFO.

注意：NLELogLevel_OFF和NLELogLevel_ALL只用于控制，不应该使用这两种级别输出日志。
设置level为 NLELogLevel_OFF，则关闭日志系统，不输出任何日志。
设置level为 NLELogLevel_ALL，则输出所有日志。
*/
typedef enum enumNLELogLevel
{
    NLELogLevel_OFF     = 0x70, ///不输出任何日志
    NLELogLevel_FATAL   = 0x60, ///只输出FATAL日志
    NLELogLevel_ERROR   = 0x50, ///输出FATAL, ERROR日志
    NLELogLevel_WARNING = 0x40, ///
    NLELogLevel_NOTICE  = 0x30,
    NLELogLevel_INFO    = 0x20,
    NLELogLevel_DEBUG   = 0x10,
    NLELogLevel_TRACE   = 0,
    NLELogLevel_ALL     = -1,
}NLELogLevel;


//{E3326A9A-937D-97AF-6171-4765F20A645E}
//DEFINE_GUID(CLSID_NLELogger, 0xE3326A9A, 0x937D, 0x97AF, 0x61, 0x71, 0x47, 0x65, 0xF2, 0x0A, 0x64, 0x5E);                   
//{B753DE3D-0DE8-8393-EAAD-D1179650F17E}
DEFINE_GUID(IID_INLELogger, 0xB753DE3D, 0x0DE8, 0x8393, 0xEA, 0xAD, 0xD1, 0x17, 0x96, 0x50, 0xF1, 0x7E);                         
/**
@brief 日志接口类，所有的日志系统操作都通过这个接口进行，在该文件的最后给出了用于简化日志操作的宏，建议尽可能
通过宏来输出日志
*/
class INLELogger :public IUnknown
{
public:
    /**
    @brief 初始化日志系统，设置日志输出的文件,该方法将创建滚动日志，系统会在日志文件达到指定的最大文件限制时自动重命名
    日志文件，然后创建新日志文件，日志文件数达到指定的最大文件数时则自动删除最早创建的一个日志文件。
    日志文件名形式为 ：
    xxx.log
    xxx_1.log
    xxx_2.log
    ...
    xxx_n.log
    @param pFileName 指定日志文件名，完整路径
    @param pMaxFileSize 最大文件大小，字节数，最小为2048,默认为5M
    @param pMaxFileCount 设置日志文件个数的上限，最小为2，默认为5
    */
    virtual int STDMETHODCALLTYPE Init(const wchar_t *pFileName, int nMaxFileSize = 5 * 1024 * 1024, int nMaxFileCount = 5) = 0;

    virtual int STDMETHODCALLTYPE Init(FILE *pFile = stdout) = 0;

    /**
    @brief 设置可输出的日志等级，在未设置状态下默认等级为 NLELogLevel_INFO，所有等级对应    
    */
    virtual HRESULT STDMETHODCALLTYPE SetLevel(NLELogLevel level) = 0;

    /**
    @brief 查询指定等级的日志是否可以生成
    @return 返回TRUE或FALSE
    */
    virtual BOOL STDMETHODCALLTYPE IsEnabled(NLELogLevel level) = 0;

    virtual int Log(NLELogLevel level, char *fmt, ...) = 0;

    virtual int Log(NLELogLevel level, wchar_t *fmt, ...) = 0;
        
};  




#define WIDEN2(x) L ## x
#define WIDEN(x) WIDEN2(x)
#define __WFILE__ WIDEN(__FILE__)
#define __WFUNCTION__ WIDEN(__FUNCTION__)


static inline wchar_t* LoggerGetFileName(wchar_t *path)
{
    wchar_t *str = wcsrchr(path, L'\\');
    if (NULL == str)
        str = wcsrchr(path, L'/');
    if (NULL == str)
        return path;

    return str + 1;
}


#define LOGGER_MACRO_BODY(logger, level, format, ...)                 \
    do                                                                \
    {                                                                  \
        INLELogger* pTempLogger = logger;                                   \
        if (NULL == pTempLogger)                                           \
        {                                                             \
		pTempLogger = NLEGetDefaultLogger();                   \
            if (NULL == pTempLogger)                                        \
            {                                                           \
                break;                                                  \
            }                                                           \
        }                                                             \
        if (!pTempLogger->IsEnabled(level))                                \
            break;                                                    \
        pTempLogger->Log(level, format L"  [%ls, %d]", ##__VA_ARGS__, \
			LoggerGetFileName(WIDEN(__FILE__)), __LINE__);         \
    }while (0);                                                                         
   

#define LOGGER_FATAL(logger, format, ...)   LOGGER_MACRO_BODY(logger, NLELogLevel_FATAL, format, ##__VA_ARGS__)
#define LOGGER_ERROR(logger, format, ...)   LOGGER_MACRO_BODY(logger, NLELogLevel_ERROR, format, ##__VA_ARGS__)
#define LOGGER_WARN(logger, format, ...)    LOGGER_MACRO_BODY(logger, NLELogLevel_WARNING, format, ##__VA_ARGS__)
#define LOGGER_NOTICE(logger, format, ...)  LOGGER_MACRO_BODY(logger, NLELogLevel_NOTICE, format, ##__VA_ARGS__)
#define LOGGER_INFO(logger, format, ...)    LOGGER_MACRO_BODY(logger, NLELogLevel_INFO, format, ##__VA_ARGS__)
#define LOGGER_DEBUG(logger, format, ...)   LOGGER_MACRO_BODY(logger, NLELogLevel_DEBUG, format, ##__VA_ARGS__)
#define LOGGER_TRACE(logger, format, ...)   LOGGER_MACRO_BODY(logger, NLELogLevel_TRACE, format, ##__VA_ARGS__)



#define LOGGER_FATAL_(format, ...)          LOGGER_FATAL(NULL, format, ##__VA_ARGS__)
#define LOGGER_ERROR_(format, ...)          LOGGER_ERROR(NULL, format, ##__VA_ARGS__)
#define LOGGER_WARN_(format, ...)           LOGGER_WARN(NULL, format, ##__VA_ARGS__)
#define LOGGER_NOTICE_(format, ...)         LOGGER_NOTICE(NULL, format, ##__VA_ARGS__)
#define LOGGER_INFO_(format, ...)           LOGGER_INFO(NULL, format, ##__VA_ARGS__)
#define LOGGER_DEBUG_(format, ...)          LOGGER_DEBUG(NULL, format, ##__VA_ARGS__)
#define LOGGER_TRACE_(format, ...)          LOGGER_TRACE(NULL, format, ##__VA_ARGS__)


#ifdef __cplusplus
extern "C" {
#endif

#ifdef NLELOGGER_EXPORTS
#define NLELOGGER_API __declspec( dllexport )
#else
#define NLELOGGER_API __declspec( dllimport )
#endif

#define EXTERN extern 
#define CALL_TYPE

	EXTERN NLELOGGER_API INLELogger* NLEGetDefaultLogger();

    EXTERN NLELOGGER_API FILE* CALL_TYPE GetStdout();

    EXTERN NLELOGGER_API void* CALL_TYPE CreateNLELogger();

    EXTERN NLELOGGER_API int CALL_TYPE NLELoggerInit(void *pLogger, const wchar_t *pFileName, int nMaxFileSize, int nMaxFileCount);

    EXTERN NLELOGGER_API int CALL_TYPE NLELoggerInit2(void *pLogger, FILE *pFile);

    /**
    @return 成功，返回S_OK； 失败，返回E_INVALIDARG  (注：#define E_INVALIDARG   _HRESULT_TYPEDEF_(0x80070057L) )
    */
    EXTERN NLELOGGER_API HRESULT CALL_TYPE NLELoggerSetLevel(void *pLogger, NLELogLevel level);

    EXTERN NLELOGGER_API BOOL CALL_TYPE NLELoggerIsEnabled(void *pLogger, NLELogLevel level);

    EXTERN NLELOGGER_API int CALL_TYPE NLELoggerLog(void *pLogger, NLELogLevel level, char *fmt, ...);

    EXTERN NLELOGGER_API int CALL_TYPE NLELoggerLog2(void *pLogger, NLELogLevel level, wchar_t *fmt, ...);
#ifdef __cplusplus
}
#endif


#endif //_H_INLELOG_H_
