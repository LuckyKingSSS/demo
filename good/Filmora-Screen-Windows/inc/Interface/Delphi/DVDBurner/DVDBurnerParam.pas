unit
  DVDBurnerParam;

interface

uses
  Windows, Messages;

type
  BURNER_ID =
  (
	  USR_STAR_BURN =	0,
	  USR_NERO_BURN
  );

const
   WM_PROGRESS_PERCNET	= WM_USER + 1800;	// 进度消息
   WM_DVD_BURNER			  = WM_USER + 1701;	// DVDBurner消息

{****************************************************************
    uMsg	  : WM_DVD_BURNER;
    wParam	: DVDBURNER_EVENT
    lParam	: BOOL bSuccessful 是否成功
****************************************************************}
type
  DVDBURNER_EVENT =
  (
	DBE_GETDRIVER_END,			// 取刻录机信息结束
	DBE_PREPAREBURN_END,        // 准备刻录结束
	DBE_GETNEROVERSION_END,		// 取Nero版本结束
	DBE_CREATEISO_END,			// 生成ISO文件结束
	DEE_BURNDVD_END,			// 刻录DVD结束
	DBE_ISOTODVD_END,           // ISO刻录为DVD结束
	DBE_ERASE_END,              // 擦除结束
	DBE_SPACE_NOT_ENOUGH,       // 光盘容量不足，请提示换光盘
	DBE_GETDISKINFO_END,		// 取光盘信息结束
	DBE_ERROR                   // 发生错误
  );

type
  TDiskInfo = record
    m_DiskType: DWORD;			        //  碟片类型
    m_szDiskType: array[0..63] of AnsiChar;	//	碟片类型名称
    m_bIsEmpty: BOOL;			        //	是否为空碟
    m_bIsErasable:BOOL;		            //	是否可以擦除
    m_bWritable: BOOL;		            //	是否可以再写
    m_ullFreeSpace : int64;		            //	碟片上的空间
    //unsigned __int64	m_ullTotalFileSize;	//	刻录这个文件所需的空间
  end;
  pCDiskInfo = ^TDiskInfo;

implementation

end.
