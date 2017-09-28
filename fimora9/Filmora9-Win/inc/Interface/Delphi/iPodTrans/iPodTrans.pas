unit
	iPodTrans;

interface

uses
    Windows, Messages;

type

MusicItem = record 
	dwID        : DWORD ;                         //Music唯一标识
	dwFileSize  : DWORD;                          //Music文件的大小　　
	stTitle     : array[0..255] of widechar;      //Music标题，带扩展名
	stFileDir   : array[0..255] of widechar;      //Music文件的存放路径
end;
PMUSICITEM = ^MusicItem;

iPodDiskInfo = record
   TotalSize  : DWORD;
	 FreeSize   : DWORD;
end;
PDISKINFO = ^iPoddiskInfo;

VideoItem = record
  dwWidth         : DWORD      ;		//视频分辨率
  dwHeight        : DWORD      ;		//
  dwCompression   : DWORD      ;		//压缩格式FourCC，如'462H'为H.264压缩格式
  dwBitrate       : DWORD      ;		//视频码率，单位bps
  dwTotalFrames   : DWORD      ;		//总帧数
  dFrameRate      : Double     ;		//视频帧率，单位fps
  dwLength        : DWORD      ;		//视频长度，单位：毫秒
end;
PVideoItem = ^VideoItem;

AudioItem = record
  dwSampleRate    : DWORD      ;		//音频采样率，单位Hz
  dwChannels      : DWORD      ;		//声道数
  dwCompression   : DWORD      ;		//音频压缩格式FourCC，如' CAA'为AAC压缩格式
  bIsVBR          : BOOL       ;		//是否动态码率
  dwBitrate       : DWORD      ;		//音频码率，若为VBR则表示平均码率，单位bps
  dwMaxBitrate    : DWORD      ;		//最大码率，bIsVBR为FALSE时此值应忽略
  dwLength        : DWORD      ;		//音频长度

end;
PAudioItem = ^AudioItem;

MediaFileInfo = record
  dwFormat        : DWORD      ;		//文件格式FourCC，如' 4PM'表示文件格式为mp4
  dwFileSize      : DWORD      ;		//文件大小
  dwLength        : DWORD      ;		//文件长度（播放时间，单位：ms）
  dwBitrate       : DWORD      ;    //码率（音频与视频之和，单位bps）
  bHaveAudio      : BOOL       ;		//是否有音频
  bHaveVideo      : BOOL       ;		//是否有视频
  Audio           : AudioItem  ;
  Video           : VideoItem  ;
end;
PMediaFileInfo = ^MediaFileInfo;

// 对iPod进行文件传输的返回值
 ManageFailure =
  (
	SUCCESS = 0,							// 传输成功
	OPEN_MEDIA_FILE_FAILURE,	//打开媒体文件失败
	NOT_ENOUGH_DISK_SPACE, 		//磁盘空间不足
	COPY_MEAIA_FILE_FAILURE,	//复制失败	
	OPEM_IPOD_FAILURE,				//打开iPod失败
	WRITE_IPOD_FAILURE,				//写入iPod失败
	READ_DATA_FAILURE,				//读取数据文件失败
  CREATE_DB_FAILURE
  );

 const
    WM_COPY_PROGRESS = (WM_USER + 3002);        // 文件拷贝进度消息通知 wParam: 0   正在处理  lParam: 0 - 100
                                                //                      100 编码完成  lParam : 0 正常完成
                                                //                                             1 用户终止
                                                //                                             2 外部回掉终止
                                                //                                             3 遭遇错误


const
    WM_COPY_I2P_PROGRESS = (WM_USER + 3003);    // 文件拷贝进度消息通知 wParam: 0   正在处理  lParam: 0 - 100
                                                //                      100 编码完成  lParam : 0 正常完成
                                                //                                             1 用户终止
                                                //                                             2 外部回掉终止
                                                //
//传输文件至iPod中， 0为成功，非0为失败 
//int __stdcall WEWriteFileToIpod(const wchar_t* pFileName);	
//ManageFailure __stdcall WEWriteFileToIpodEx(const wchar_t* pFileName, const wchar_t* pTitle, int nTimeLength, 
								//const wchar_t* pDiskSymbol, MusicItem* pMusicItem, HWND hMsgReceiverWnd);		//参数DiskSymbol可为NULL，pParam暂未使用	
function WTWriteFileToIpodEx(const pFileName : PWideChar; const pTitle : PWideChar; nTimeLength : Integer;		 //参数DiskSymbol可为NULL，pParam暂未使用
                             const pDiskSymbol : PWideChar;  pMI : PMUSICITEM; hMsgReceiverWnd : HWND; piPodDiskInfo:PDISKINFO) : ManageFailure; stdcall;




//1.当返回值为-1时，获得信息失败
//pdisk_symbol为盘符，不指定时传空，
//2.pMusicInfo传空的时候，该函数返回值为数据库文件记录的音乐数目。
//3.pMusicInfo不为空的时候，传进的iMIContent为数组的大小，FileName为要查找的文件夹，函数返回值非-1的时候为pMusicInfo中实际信息的条数
//int __stdcall WTGetMusicInfo( wchar_t* pdisk_symbol, MusicItem* pMusicInfo, int ArraySize);
function WTGetMusicInfoEx(pDiskSymbol : PWideChar; pMI : PMUSICITEM; ArraySize: Integer; piPodDiskInfo:PDISKINFO ): Integer; stdcall;

//pdisk_symbol为NULL, MusicID为歌曲唯一标识，FileName为歌曲完整路径　
//ManageFailure __stdcall WTDeleteMusic(wchar_t* pdisk_symbol, DWORD MusicID, const wchar_t* FileName);
function WTDeleteMusicEx(pDiskSymbol : PWideChar; MusicID: DWORD; const FileName: PWideChar; piPodDiskInfo:PDISKINFO): ManageFailure; stdcall;


// 读取多媒体文件的播放时间，单位秒，目前只支持MP4, M4V, M4A和MP3格式，读取失败返回-1
// 函数中不对参数合法性进行检查，由调用者保证.
function  WTGetTimeLengthEx(const lpFile: PWideChar): integer; stdcall;


//BOOL WTCopyFileEx(LPCTSTR lpExistingFileName, LPCTSTR lpNewFileName, HWND hMsgReceiverWnd);
function WTCopyFileEx(lpExistingFileName, lpNewFileName : PWideChar;  hMsgReceiverWnd: HWND): Boolean; stdcall;

// 读取多媒体文件信息
//BOOL __stdcall WTGetFileInfo(const wchar_t* pFileName, MediaFileInfo* pInfo);
function WTGetFileInfo(pFileName : PWideChar; pInfo :  PMediaFileInfo):BOOL;stdcall;

//获取iPod的代数, 如果是最新的六代返回1, 小于六代为0, 读取iPod出错返回－1
//int __stdcall WTGetIpodGeneration(wchar_t* pdisk_symbol);
function WTGetIpodGeneration(): Integer; stdcall;

implementation

const
    DLLNAME = 'WS_iPodTrans.dll';

    function WTWriteFileToIpodEx			  ; external DLLNAME index 3;
    function WTGetMusicInfoEx           ; external DLLNAME index 1;
    function WTDeleteMusicEx            ; external DLLNAME index 2;
    function WTGetTimeLengthEx          ; external DLLNAME index 4;
    function WTCopyFileEx               ; external DLLNAME index 5;
    function WTGetFileInfo              ; external DLLNAME index 6;
    function WTGetIpodGeneration        ; external DLLNAME index 7;

end.
