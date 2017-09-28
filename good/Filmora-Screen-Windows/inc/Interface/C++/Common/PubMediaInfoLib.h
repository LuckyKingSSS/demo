#pragma once
enum SCAN_TYPE {
	SCAN_TYPE_PROGRESSIVE, ///< 逐行模式
	SCAN_TYPE_INTERLACED, ///< 交织模式
	SCAN_TYPE_UNKNOWN,	///< 模式未知
	SCAN_TYPE_RESERVED	///< 保留字段
};
enum ENCRYPTION_TYPE {
	ENCRYPTION_TYPE_ENCRYPTION,///< 视频文件加密
	ENCRYPTION_TYPE_UNENCRYPTION, ///< 视频文件没有加密		
};

#ifndef WIN32
#define  __stdcall 
#endif

struct MEDIA_INFO_LIB_ID3_INFO
{
	wchar_t szArtist[260];      // 艺术家
	wchar_t szTitle[260];       // 标题       
	wchar_t szTrackNumber[260]; // 音轨号
	wchar_t szAlbum[260];     	// 专辑    
	wchar_t szDate[260];    	//年代   
	wchar_t szGenre[260];	    // 风格  
	wchar_t szPublisher[260];   //发行商
};

struct Video_QuickTime_Mov_Sprite
{
	int  widthscale_a ;
	int  widthrotate_b ;
	int widthangle_u  ;
	int heightscale_c ;
	int heightrotate_d ;
	int	heightangle_v ;
	int positionleft_x ;
	int positiontop_y ;
	int divider_w ;

	int  nRotate;
};
struct MEDIA_INFO_LIB_VIDEO_INFO
{
	int		nVideoID;		// 视轨号	：	文件中编号
	int		nVideoDuration;	//					ms

	wchar_t	wcsFourCC[260];	// 视频编码器 
	int		nWidth;			// 分辨率宽度		pixel
	int		nHeight;		// 分辨率高度
	double  dFrameRate;		// 帧率
	int		nTotalFrames;   // 帧数 
	int		nBitrate;		// 码率				bps
	wchar_t wcsCodeInfo[260];//编码器描述信息

	SCAN_TYPE	scanType;	//扫描类型
	ENCRYPTION_TYPE encryType;	//加密
	
	int xratio;	// x方向比例 	
	int yratio;	// y方向比例

	double dMaxFrameRate;
	int	   nIsVBR;
	Video_QuickTime_Mov_Sprite vqmSprite;

	
};

struct MEDIA_INFO_LIB_AUDIO_INFO
{
	int		nAudioID;			// 音轨号 ： 文件中编号
	int		nAudioDuration;		//				ms

	wchar_t	wcsFourCC[260];	// 音频编码器
	int		nSampleRate;	// 采样率			Hz
	int		nChannels;		// 声道数量
	int		nBitrate;		// 码率				bps 
	wchar_t wcsCodeInfo[260];//编码器描述信息

	ENCRYPTION_TYPE audioEncryType;	//加密
};

struct MEDIA_INFO_PROGRAM_INFO
{
	int		nProgramID;		   // 默认0
	int		nProgramDuration;  //ms
	
	ENCRYPTION_TYPE programEncryType;	//加密

	MEDIA_INFO_LIB_AUDIO_INFO  mAudioProgrm;
	MEDIA_INFO_LIB_VIDEO_INFO  mpVideoProgrm;
};
struct  MEDIA_INFO_LIB_FILE_INFO
{
	wchar_t szDate[260];           
	wchar_t	wcsFileFormat[260];		//文件格式
	wchar_t wcsFileFormatInfo[260];	//文件容器信息
	double dFileSize;				//文件大小			byte
	int	nDuration;					//文件播放时长		(ms)
	int		nOverAllBitRate;		//文件总的比特率	bps
	int		nProgramNum;			//节目数
	MEDIA_INFO_LIB_ID3_INFO	 mFileID3Info; //文件ID3信息
	MEDIA_INFO_PROGRAM_INFO mpFilePrograms;//节目表
};

class CPubMediaInfoLib
{
public:
	CPubMediaInfoLib(void);

	virtual int SetVideoName(const wchar_t *pVideoFileName) = 0;

	virtual const MEDIA_INFO_LIB_FILE_INFO  Get_File_Info( void) = 0;
	
	virtual ~CPubMediaInfoLib(void);
};

#ifdef __cplusplus
extern "C"
{
#endif

#pragma pack(push)
#pragma pack(1)
	struct MEIDACREATETIME
	{
		int nYear;
		int nMonth;
		int nDay;
		int nHour;
		int nMinute ;
		int nSecond;

	};
#pragma pack(pop)

	CPubMediaInfoLib * CreatePubMediaInfoLib(void);

	// audio version
	//-1 ;err
	// 1; version 1; 
	//2 ; version 2;

	int  GetMp3Version(const wchar_t *pVideoFileName);

	bool GetImageSize(const wchar_t* image_file, int* width, int* height);

	bool GetCreateTimeInfo(MEIDACREATETIME &mtCreatTime,MEIDACREATETIME &mtModifyTime,const wchar_t *pVideoFileName);

	int __stdcall GetExifAngle(const wchar_t* pFileNanme);

#ifdef __cplusplus
}
#endif
