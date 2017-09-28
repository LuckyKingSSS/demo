{*******************************************************}
{       Wondershare专业版多媒体任务管理单元             }
{       创建人:　 凌曙光　　 　　　　　　　   　　　　  }
{       创建日期: 2009/04/30                            }
{       修订人:   凌曙光                                }
{       修订日期: 2009/06/03                            }
{*******************************************************}

unit MediaTaskPro;

interface

uses Windows, SysUtils, Classes, Contnrs, WideStrings, Graphics, FSClasses,
  MediaCommon, MediaDecode, MediaEncodeMgr, MediaEncode, MediaProcess, MediaPlay,
  DVDInfo, MediaDibImage, WSTextArt, WSSubPic;

type
  TMediaTask = class;
  TMediaTaskMgr = class;
  TMediaTaskList = class;

  TMediaTaskType = (mttVideo, mttAudio, mttDVD, mttMergeSrc);

  TVideoDecodeException = class(Exception);
  TMediaTaskException = class(Exception);
  TMediaTaskMediaSourceClass = class of TMediaTaskMediaSource;

  TMediaTaskMediaSource = class
  protected
    FTask: TMediaTask;               // 负责编码参数的获取
    FLoaded: Boolean;                // 文件加载的标识位
    FMediaType: TMediaTaskType;      // 媒体类型(Video, DVD)
    FFileName    : UnicodeString;    // 源文件名
    FMediaLength : Double;           // 文件结束时间
    FClipStart  : Double;            // 剪辑开始时间（只有DVD Chapter不为零）
    FClipEnd  : Double;              // 剪辑结束时间
    FBitrate: Integer;               // 文件比特率
    FVideoBitrate: Integer;          // 文件视频的比特率
    FAudioBitrate: Integer;          // 文件音频的比特率
    FVideoInfo   : TWSVideoInfo;     // 视频的信息
    FAudioInfo   : TWSAudioInfo;     // 音频的信息
    FHasVideoInfo: Boolean;          // 是否含有视频标志
    FHasAudioInfo: Boolean;          // 是否含有音频标志
    FHasSubTitle: Boolean;           // 是否存在字幕（内部，外置）
    FSubTitles  : TWideStrings;      // 字幕列表
    FAudioTracks: TWideStrings;      // 音轨列表
    FDecModel   : DEC_MODEL;         // 文件解码方式
    FVDecParam  : TDecParam;         // 视频解码参数
    FADecParam  : TDecParam;         // 音频解码参数
    FMediaDecoder: TMediaDecoder;        // DVD解码器，引用计数自动释放，不要试图Free它
    procedure OnDVDDecoderRelease(Sender: TObject);
    function GetClipLength: Double;
    function GetMediaCantSeek: Boolean;
    function GetMediaUnknownLength: Boolean;
    function GetSubTitleIds(Index: Integer): Integer;
    function GetADecParam: PDecParam;
    function GetVDecParam: PDecParam;
    procedure SetADecParam(Value: PDecParam);
    procedure SetVDecParam(Value: PDecParam);
    function GetAudioTrackIdx: Integer;
    function GetSubTitleId: Integer;
    function GetSubTitleIdx: Integer;
    procedure SetAudioTrackIdx(Value: Integer);
    procedure SetSubTitleId(Value: Integer);
    procedure SetSubTitleIdx(Value: Integer);
    function GetAudioTrackId: Integer;
    procedure SetAudioTrackId(Value: Integer);
    function LoadAudioTrackList(pProgram: IProgramInfo): Integer;
    function LoadSubTitleList(pProgram: IProgramInfo): Integer;
    function LoadFromMediaInfo(pMediaInfo: IMediaInfo; AProgramIdx: Integer;
      ANeedRelease: Boolean=True): Boolean;
  public
    constructor Create(); 
    destructor Destroy; override;

    procedure Assign(Source: TMediaTaskMediaSource); virtual;
    function Clone: TMediaTaskMediaSource;

    function GetVideoDecoder(var AFileDecoder: TObject; Reason: TMediaDecodeReason): IVideoDecoder; // 获取视频解码器
    function GetAudioDecoder(var AFileDecoder: TObject; Reason: TMediaDecodeReason): IAudioDecoder; // 获取音频解码器

    function GetDecoder(var AFileDecoder: TObject; Reason: TMediaDecodeReason;
      out AVideoDec: IVideoDecoder; out AAudioDec: IAudioDecoder): Boolean; virtual; // 获取音视频解码器
    procedure SetPlayer(APlayer: TMediaPlayer; Reason: TMediaDecodeReason=mdrForPlay); virtual;
    // 媒体信息
    property Task: TMediaTask read FTask;
    property MediaType: TMediaTaskType read FMediaType;
    property FileName: UnicodeString read FFileName;
    property Loaded: Boolean read FLoaded;
    property MediaLength: Double read FMediaLength;
    property ClipStart: Double read FClipStart write FClipStart;
    property ClipEnd: Double read FClipEnd write FClipEnd;
    property ClipLength: Double read GetClipLength;
    property Bitrate: Integer read FBitrate;
    property VideoBitrate: Integer read FVideoBitrate;
    property AudioBitrate: Integer read FAudioBitrate;
    property VideoInfo: TWSVideoInfo read FVideoInfo;
    property AudioInfo: TWSAudioInfo read FAudioInfo;
    property HasVideoInfo: Boolean read FHasVideoInfo;
    property HasAudioInfo: Boolean read FHasAudioInfo;
    property MediaCantSeek: Boolean read GetMediaCantSeek;
    property MediaUnknownLength: Boolean read GetMediaUnknownLength;
    property HasSubTitle: Boolean read FHasSubTitle;                   // 是否含有字幕（内置-DVD/外置-MKV文件）
    property AudioTracks: TWideStrings read FAudioTracks;              // 音轨列表
    property SubTitles: TWideStrings read FSubTitles;                  // 外挂字幕
    property SubTitleIds[Index: Integer]: Integer read GetSubTitleIds; // 从索引取得字幕Id
    property AudioTrackIdx: Integer read GetAudioTrackIdx write SetAudioTrackIdx; // 音轨索引
    property SubTitleIdx: Integer read GetSubTitleIdx write SetSubTitleIdx;       // 字幕索引
    property SubTitleId: Integer read GetSubTitleId write SetSubTitleId;          // 字幕流Id
    property AudioTrackId: Integer read GetAudioTrackId write SetAudioTrackId;    // 音轨流Id
    // 解码参数
    property DecModel: DEC_MODEL read FDecModel write FDecModel;
    property VideoDecParam: PDecParam read GetVDecParam write SetVDecParam;
    property AudioDecParam: PDecParam read GetADecParam write SetADecParam;
  end;

  TMediaTaskVideoSource = class(TMediaTaskMediaSource)
  private
    function LoadAV: Boolean;
  public
    constructor Create(const AFileName: UnicodeString; ADecModel: DEC_MODEL); overload;
    constructor Create(const AFileName: UnicodeString; ADecModel: DEC_MODEL;
      pMediaInfo: IMediaInfo; AProgramIdx: Integer); overload;
  end;

  TMediaTaskDVDSource = class(TMediaTaskMediaSource)
  private
    FTitleIndex: Integer;            // 文件中Title的序列
    FChapterIndex: Integer;          // 文件中Chapter的序列
    FChapterCount: Integer;          // IFO包含的Chapter个数
    function LoadDVD(ADVDInfo: TDVDIFOInfo=nil): Boolean;
  public
    constructor Create(const AFileName: UnicodeString; ADecModel: DEC_MODEL;
      ATitleIndex, AChapterIndex: Integer; ADVDInfo: TDVDIFOInfo=nil);
    destructor Destroy; override;

    procedure Assign(Source: TMediaTaskMediaSource); override;

    class function IsDVDIFOFile(const AFileName: UnicodeString): Boolean;
    class function IsDVDISOFile(const AFileName: UnicodeString): Boolean;
    // DVD 媒体信息
    property TitleIndex: Integer read FTitleIndex;
    property ChapterIndex: Integer read FChapterIndex;
    property ChapterCount: Integer read FChapterCount;
  end;

  TMediaTaskEncodeParams = class
  private
    FOwner: TMediaTask;
    FEncParam   : TEncodeParam;
    function GetIsLosslessCopy: Boolean;
    function GetIsOutputAudio: Boolean;
    function GetEncParam: PEncodeParam;
    procedure SetEncParam(const Value: PEncodeParam);
  public
    constructor Create(AOwner: TMediaTask);

    procedure Assign(Source: TMediaTaskEncodeParams); virtual;

    function InitDefaultEncoders: Boolean;

    property Owner: TMediaTask read FOwner;
    // 编码参数
    property EncParam: PEncodeParam read GetEncParam write SetEncParam; // 编码参数
    property IsOutputAudio: Boolean read GetIsOutputAudio;    // 输出格式是否音频格式
    property IsLosslessCopy: Boolean read GetIsLosslessCopy;  // 是否DVD无损拷贝
  end;

  TWatermarkType = (wtImage, wtText);

  TFPoint = record
    X: Single;
    Y: Single;
  end;

  TFSize = record
    cx: Single;
    cy: Single;
  end;

  TMediaTaskEditParams = class
  private
    FOwner: TMediaTask;
    FResizeStyleModified: Boolean;
    FDeInterlacing: DEINTERLACE_STYLE;
    FContrast: Integer;
    FFilterId: UINT;
    FSaturation: Integer;
    FBrightness: Integer;
    FHorzFlip: Boolean;
    FVertFlip: Boolean;
    FCropRect   : TRect;
    FVolume: Integer;
    FMute: Boolean;
    FWaterMarkImage: TDibImage;
    FWaterMark: Boolean;
    FWaterMarkPos: TFPoint;
    FWaterMarkSize: TFSize;
    FWaterMarkType: TWatermarkType;
    FWaterMarkTrans: Integer;
    FWaterMarkText: UnicodeString;
    FWaterMarkFile: UnicodeString;
    FWaterMarkFont: TFont;
    FSubTitle: Boolean;
    FSubTitlePos: TFPoint;
    FSubTitleTrans: Integer;
    FSubTitleUserStyle: Boolean;
    FSubTitleAngle: Integer;
    FSubTitleFont: TFont;
    FRotation: Double;
    FSubTitleFile: UnicodeString;
    FResizeStyle: IMAGE_RESIZE_METHOD;
    function InnerGetWaterMarkImage(AScale: Double): TDibImage;
    function GetFlipFlag: UINT;
    function GetWaterMarkImage: TDibImage;
    procedure SetFlipFlag(Value: UINT);
    procedure SetSubTitleFont(Value: TFont);
    procedure SetWaterMark(Value: Boolean);
    procedure SetWaterMarkFont(Value: TFont);
    procedure SetWaterMarkImage(Value: TDibImage);
    function GetSubTitlePos: TPoint;
    function GetWaterMarkPos: TPoint;
    function GetWaterMarkSize: TSize;
    procedure SetSubTitlePos(const Value: TPoint);
    procedure SetWaterMarkPos(const Value: TPoint);
    procedure SetWaterMarkSize(const Value: TSize);
    function RelativeToAbsolutePos(const Value: TFPoint): TPoint;
    function RelativeToAbsoluteSize(const Value: TFSize): TSize;
    function AbsoluteToRelativePos(const Value: TPoint): TFPoint;
    function AbsoluteToRelativeSize(const Value: TSize): TFSize;
    function GetWaterMarkRect: TFRect;
    procedure SetWaterMarkRect(const Value: TFRect);
  public
    constructor Create(AOwner: TMediaTask);
    destructor Destroy; override;

    procedure Assign(Source: TMediaTaskEditParams); virtual;

    function GetUserWaterMarkParam: TUserWatermarkParam; // 获取用户水印参数记录
    function GetUserSubTitleParams: TUserSubPicRec; // 获取用户外挂字幕参数记录

    function SetVPUserWaterMark(AVideoProcessor: TVideoMediaProcessor; AToEncode: Boolean=False): Boolean;
    function SetUserSubTitle(AVideoProcessor: TVideoMediaProcessor; AToEncode: Boolean=False): Boolean;
    function SetVideoProcessor(AVideoProcessor: TVideoMediaProcessor; AToEncode: Boolean=False): Boolean;
    function SetAudioProcessor(AAudioProcessor: TAudioMediaProcessor): Boolean;
    function SetVideoPreviewProcessor(AVideoProcessor: TVideoMediaProcessor): Boolean;

    property Owner: TMediaTask read FOwner;
    // 编辑效果
    property ResizeStyleModified: Boolean read FResizeStyleModified write FResizeStyleModified;// 缩放是否被设置
    property ResizeStyle: IMAGE_RESIZE_METHOD read FResizeStyle write FResizeStyle;  // 缩放处理方式
    property CropRect: TRect read FCropRect write FCropRect;                         // 视频裁剪区域
    property Volume: Integer read FVolume write FVolume;                             // 音量
    property Mute: Boolean read FMute write FMute;                                   // 静音
    property Brightness: Integer read FBrightness write FBrightness;                 // 亮度
    property Contrast: Integer read FContrast write FContrast;                       // 对比度
    property Saturation: Integer read FSaturation write FSaturation;                 // 饱和度
    property FilterID: UINT read FFilterId write FFilterID;                          // 滤镜ID
    property FlipFlag: UINT read GetFlipFlag write SetFlipFlag;                      // 翻转标志(一般不直接操作这个值,而是用下面两个比较直观的属性)
    property HorzFlip: Boolean read FHorzFlip write FHorzFlip;                       // 横向翻转
    property VertFlip: Boolean read FVertFlip write FVertFlip;                       // 竖向翻转
    property DeInterlacing: DEINTERLACE_STYLE read FDeInterlacing write FDeInterlacing; // 反隔行扫描处理方式
    property Rotation: Double read FRotation write FRotation;           // V3.3新增功能，旋转
    // 水印
    property WaterMark: Boolean read FWaterMark write SetWaterMark; // 是否允许用户水印
    property WaterMarkImage: TDibImage read GetWaterMarkImage write SetWaterMarkImage;      // 用户水印图像，文字也转为图像
    property WaterMarkType: TWatermarkType read FWaterMarkType write FWaterMarkType; // 水印类型
    property WaterMarkFile: UnicodeString read FWaterMarkFile write FWaterMarkFile;  // 水印图片文件
    property WaterMarkText: UnicodeString read FWaterMarkText write FWaterMarkText;  // 水印文字
    property WaterMarkFont: TFont read FWaterMarkFont write SetWaterMarkFont;        // 水印字体
    property WaterMarkTrans: Integer read FWaterMarkTrans write FWaterMarkTrans;     // 水印透明度
    property WaterMarkPos: TPoint read GetWaterMarkPos write SetWaterMarkPos;        // 水印位置
    property WaterMarkSize: TSize read GetWaterMarkSize write SetWaterMarkSize;      // 水印大小
    property WaterMarkRect: TFRect read GetWaterMarkRect write SetWaterMarkRect;     // 新增：水印相对区域(最大坐标=1)
    // 外挂字幕
    property SubTitle: Boolean read FSubTitle write FSubTitle;                       // 是否允许用户外挂字幕
    property SubTitleUserStyle: Boolean read FSubTitleUserStyle write FSubTitleUserStyle;   // 是否允许用户自定义外挂字幕样式
    property SubTitleFont: TFont read FSubTitleFont write SetSubTitleFont;           // 字幕字体
    property SubTitleTrans: Integer read FSubTitleTrans write FSubTitleTrans;        // 字幕透明度
    property SubTitleAngle: Integer read FSubTitleAngle write FSubTitleAngle;        // 字幕旋转角度，未使用
    property SubTitlePos: TPoint read GetSubTitlePos write SetSubTitlePos;           // 字幕位置
    property SubTitleFile: UnicodeString read FSubTitleFile write FSubTitleFile;     // 外挂字幕文件 srt等
    property SubTitlePosF: TFPoint read FSubTitlePos write FSubTitlePos;             // 新增: 字幕相对位置
  end;

  TMediaTaskKind = (mtkMain, mtkClip, mtkMultiProfile, mtkMerge);

  TOnNewTaskEvent = procedure (ANewTask: TMediaTask) of object;

  TMediaMultiProfileTask = class;

  TMediaTaskClass = class of TMediaTask;

  // 任务抽象类,不能直接创建这个类实例，只能创建子类
  TMediaTask = class
  private
    FOwner: TMediaTaskList;
    FId: Integer;
    FOrderId: Integer;
    FTaskKind: TMediaTaskKind;
    FTitle: string;
    FOutputSubDir: UnicodeString;
    FOutFileName: UnicodeString;
    FSkipEncode: Boolean;
    FFormatId: Integer;
    FFormatExt: string;
    FEncodeParams: TMediaTaskEncodeParams;
    FChildMultiProfileTasks: TMediaTaskList;
    function GetOutputResizeParam: RESIZE_PARAM;
    function GetOutputVideoSize: TSize;
    function GetPreviewResizeParam: RESIZE_PARAM;
    function GetPreviewVideoSize: TSize;
    function GetCanEdit: Boolean;
    function GetFullOutFileName: UnicodeString;
    function GetOutputLength: Double;
    function GetOutputStart: Double;
    function GetMediaLength: Double;
    procedure OnMPListNotify(Ptr: Pointer; Action: TListNotification);
    procedure AddChildProfileTask(ATask: TMediaMultiProfileTask); overload;
    procedure RemoveChildProfileTask(ATask: TMediaMultiProfileTask);
  protected
    FChildTaskList: TMediaTaskList;
    function GetMediaSource: TMediaTaskMediaSource; virtual; abstract;
    function GetEncodeParams: TMediaTaskEncodeParams; virtual;
    function GetEditParams: TMediaTaskEditParams; virtual; abstract;
  public
    constructor CreateNew(AOwner: TMediaTaskList); virtual;
    constructor Create(AOwner: TMediaTaskList; const ATitle: string);
    destructor Destroy; override;

    procedure Remove;
    procedure Assign(Source: TMediaTask; ADeep: Boolean); virtual;
    function Clone: TMediaTask;
    function AddChildProfileTask: TMediaMultiProfileTask; overload;
    function InitEncoder: TMediaEncoder; virtual;      // 初始化编码器
    function GetChildTasks: TMediaTaskList; virtual;   // 获取所有子任务列表，返回对象需要调用者释放

    class function FromFile(AOwner: TMediaTaskList; const AFileName: UnicodeString;
      ADecModel: DEC_MODEL; AOnNewTask: TOnNewTaskEvent): Integer;  // 视频中每个视频流调用一次回调,返回值是新建立的任务数量
    class function CalcOutputFileSize(const AEncParam: TEncodeParam; ALength: Double): UInt64; overload;
    function CalcOutputFileSize(): UInt64; overload;
    // 任务属性
    property Owner: TMediaTaskList read FOwner;
    property Id: Integer read FId;
    property OrderId: Integer read FOrderId write FOrderId;  // 顺序ID，由用户使用
    property Title: string read FTitle write FTitle;         // 任务标题,一般是文件名,DVD章节标题由外部传入
    property OutputSubDir: UnicodeString read FOutputSubDir write FOutputSubDir;
    property OutFileName: UnicodeString read FOutFileName write FOutFileName; // 输出文件名
    property FullOutFileName: UnicodeString read GetFullOutFileName;
    property SkipEncode: Boolean read FSkipEncode write FSkipEncode;
    property TaskKind: TMediaTaskKind read FTaskKind;
    property FormatId: Integer read FFormatId write FFormatId;
    property FormatExt: string read FFormatExt write FFormatExt;
    // 常用的媒体剪辑信息
    property MediaLength: Double read GetMediaLength;
    property OutputStart: Double read GetOutputStart;
    property OutputLength: Double read GetOutputLength;

    // 媒体信息
    property MediaSource: TMediaTaskMediaSource read GetMediaSource;
    // 编码参数
    property EncodeParams: TMediaTaskEncodeParams read GetEncodeParams;
    // 编辑效果
    property EditParams: TMediaTaskEditParams read GetEditParams;
    // 多格式子项列表
    property ChildMultiProfileTasks: TMediaTaskList read FChildMultiProfileTasks;

    property CanEdit: Boolean read GetCanEdit;   // 是否允许编辑效果
    // 输出视频大小（从EncParam中取得）
    property OutputVideoSize: TSize read GetOutputVideoSize;
    // 输出预览视频大小（保证预览效果,不缩小）
    property PreviewVideoSize: TSize read GetPreviewVideoSize;
    // 快速获取输出缩放参数
    property OutputResizeParam: RESIZE_PARAM read GetOutputResizeParam;
    // 快速获取预览缩放参数
    property PreviewResizeParam: RESIZE_PARAM read GetPreviewResizeParam;
  end;

  TMediaClipTask = class;
  TMediaClipTaskList = class;

  // 拥有自己的所有属性的主任务基类（原子任务，不可再分）
  TMediaBaseTask = class(TMediaTask)
  private
    FMediaSource: TMediaTaskMediaSource;
    FEditParams: TMediaTaskEditParams;
    FVideoSlideProcess: TVideoMediaProcessor;
    FAudioSlideProcess: TAudioMediaProcessor;
    FChildClipTasks: TMediaClipTaskList;
    FMaxClipId: Integer;
    FFileDecoder: TObject;    
    function GetAutoClipId: Integer;
    procedure OnClipListNotify(Ptr: Pointer; Action: TListNotification);
    procedure AddChildClipTask(ATask: TMediaClipTask); overload;
    procedure RemoveChildClipTask(ATask: TMediaClipTask);
  protected
    procedure FreeSlideProcess;
    function AddToVideoSlide(VidSlide: TSlideDecoderEx; AStartTime: Double): Boolean; virtual;
    function AddToAudioSlide(AudSlide: TSlideDecoderEx; AStartTime: Double): Boolean; virtual;
    function VideoSlideClipEvent(Sender: TSlideDecoderEx; nOptFlag: UINT): HVIDEODECODER; virtual;
    function AudioSlideClipEvent(Sender: TSlideDecoderEx; nOptFlag: UINT): HAUDIODECODER; virtual;
    function GetMediaSource: TMediaTaskMediaSource; override;
    function GetEncodeParams: TMediaTaskEncodeParams; override;
    function GetEditParams: TMediaTaskEditParams; override;
  public
    constructor CreateNew(AOwner: TMediaTaskList); override;
    constructor Create(AOwner: TMediaTaskList; const ATitle: string;
      AMediaSource: TMediaTaskMediaSource);
    destructor Destroy; override;
    procedure AfterConstruction; override;
    procedure Assign(Source: TMediaTask; ADeep: Boolean); override;
    function GetChildTasks: TMediaTaskList; override;

    function AddChildClipTask: TMediaClipTask; overload;
    // Clip 剪切子项列表
    property ChildClipTasks: TMediaClipTaskList read FChildClipTasks;
  end;

  // 多段裁剪子任务（原子任务，不可再分，有自己的所有参数)
  TMediaClipTask = class(TMediaBaseTask)
  private
    FParentTask: TMediaBaseTask;
    FClipId: Integer;
  protected
    function GetEditParams: TMediaTaskEditParams; override;  // 子任务继承父任务的编辑效果
  public
    constructor Create(AParentTask: TMediaBaseTask; AOwner: TMediaTaskList; const ATitle: string);
    destructor Destroy; override;
    procedure AfterConstruction; override;
    procedure Assign(Source: TMediaTask; ADeep: Boolean); override;

    property ParentTask: TMediaBaseTask read FParentTask;
    property ClipId: Integer read FClipId;
  end;

  // 多格式输出，本身没有任务属性，效果参数，只有编码格式属性
  TMediaMultiProfileTask = class(TMediaTask)
  private
    FParentTask: TMediaTask;
  protected
    function GetMediaSource: TMediaTaskMediaSource; override;
    function GetEditParams: TMediaTaskEditParams; override;
  public
    constructor Create(AParentTask: TMediaTask; AOwner: TMediaTaskList; const ATitle: string);
    destructor Destroy; override;
    procedure AfterConstruction; override;

    property ParentTask: TMediaTask read FParentTask;
  end;

  TMediaMergeTask = class;

  // 合并任务媒体信息（只有部分参数有意义）
  // 注意: 只能合并基本任务(TMediaBaseTask)
  TMediaTaskMergeSource = class(TMediaTaskMediaSource)
  protected
    FOwnerTask: TMediaMergeTask;  // 这个负责通知的取消，FTask负责参数的获取
    FTaskList: TMediaTaskList;
    procedure OnRemoveTask(Sender: TMediaTask);
    procedure AddTasksToVideoSlide(VidSlide: TSlideDecoderEx);
    procedure AddTasksToAudioSlide(AudSlide: TSlideDecoderEx);
  public
    constructor Create(AOwner: TMediaMergeTask);
    destructor Destroy; override;
    procedure AfterConstruction; override;
    procedure Assign(Source: TMediaTaskMediaSource); override;
    function GetDecoder(var AFileDecoder: TObject; Reason: TMediaDecodeReason;
      out AVideoDec: IVideoDecoder; out AAudioDec: IAudioDecoder): Boolean; override; // 获取音视频解码器
    function UpdateInfoFromTaskList: Boolean;

    procedure AddSubTask(ATask: TMediaBaseTask);
    procedure RemoveSubTask(ATask: TMediaBaseTask);

    property TaskList: TMediaTaskList read FTaskList;
  end;

  // 合并任务（虚拟任务, 本身无任务属性，不能编辑效果，只有编码格式属性）
  TMediaMergeTask = class(TMediaTask)
  private
    FMediaSource: TMediaTaskMergeSource;
  protected
    function GetMediaSource: TMediaTaskMediaSource; override;
    function GetEditParams: TMediaTaskEditParams; override;
  public
    constructor CreateNew(AOwner: TMediaTaskList); override;
    destructor Destroy; override;
    procedure AfterConstruction; override;
    procedure Assign(Source: TMediaTask; ADeep: Boolean); override;
    function GetEncodeParamsFromFirstTask: Boolean;

    function InitEncoder: TMediaEncoder; override;

    property MediaSource: TMediaTaskMergeSource read FMediaSource;
  end;

  TMediaTaskListEnumerator = class
  private
    FIndex: Integer;
    FList: TMediaTaskList;
  public
    constructor Create(AList: TMediaTaskList);
    function GetCurrent: TMediaTask;
    function MoveNext: Boolean;
    property Current: TMediaTask read GetCurrent;
  end;

  TMediaListSortCompare = function(Item1, Item2: Pointer): Integer;

  TTaskNotifyEvent = procedure (Sender: TMediaTask) of object;

  TTaskListAssignOp = (tlaAdd, tlaCopy, tlaClone);

  TMediaTaskList = class
  private
    FOwner: TMediaTaskMgr;
    FList: TFSObjectList;
    FOnListNotify: TListNotifyEvent;
    function GetCount: Integer;
    function GetItems(Index: Integer): TMediaTask;
    procedure DoListNotify(Ptr: Pointer; Action: TListNotification);
  protected
    class var FAutoId: Integer;
    class function GetAutoId: Integer;
  public
    constructor Create(AOwner: TMediaTaskMgr; AOwnedObject: Boolean);
    destructor Destroy; override;

    procedure Assign(Source: TMediaTaskList; AOperator: TTaskListAssignOp);
    function Clone: TMediaTaskList;

    function Add(Item: TMediaTask): Integer; overload;
    procedure Insert(Index: Integer; Item: TMediaTask);
    function Extract(Item: TMediaTask): TMediaTask;
    function IndexOf(Id: Integer): Integer; overload;
    function IndexOf(Item: TMediaTask): Integer; overload;
    function Find(Id: Integer): TMediaTask; overload;
    function Find(const AFileName: UnicodeString): TMediaTask; overload;
    function Find(ASkipItem: TMediaTask; const AFileName: UnicodeString): TMediaTask; overload;
    function FindRepeat(ASkipItem: TMediaTask; const AFileName: UnicodeString): Integer; overload;
    function FindOutNoExt(ASkipItem: TMediaTask; const AFileName: UnicodeString): Integer; overload;
    function CheckRepeatAndRenameOutFile(ASkipItem: TMediaTask; const AFileName, AExt: UnicodeString): UnicodeString;
    procedure Delete(Index: Integer);
    procedure Clear;
    procedure Move(CurIndex, NewIndex: Integer);
    function Remove(Item: TMediaTask): Integer;
    function GetEnumerator: TMediaTaskListEnumerator;
    function CheckExists(const AFileName: UnicodeString): Boolean;
    function First: TMediaTask;
    function Last: TMediaTask;

    procedure SortByOrderId;

    property Owner: TMediaTaskMgr read FOwner;
    property Count: Integer read GetCount;
    property Items[Index: Integer]: TMediaTask read GetItems; default;
    property OnListNotify: TListNotifyEvent read FOnListNotify write FOnListNotify;
  end;

  TMediaClipTaskListEnumerator = class
  private
    FIndex: Integer;
    FList: TMediaClipTaskList;
  public
    constructor Create(AList: TMediaClipTaskList);
    function GetCurrent: TMediaClipTask;
    function MoveNext: Boolean;
    property Current: TMediaClipTask read GetCurrent;
  end;

  TMediaClipTaskList = class(TMediaTaskList)
  private
    FBaseTask: TMediaBaseTask;
    function GetItems(Index: Integer): TMediaClipTask;
  public
    constructor Create(AOwner: TMediaTaskMgr; ABaseTask: TMediaBaseTask);
    procedure Merge(Source: TMediaClipTaskList; ADeep: Boolean);  // 合并两个剪辑列表

    function Extract(Item: TMediaClipTask): TMediaClipTask;
    function IndexOfByClipId(ClipId: Integer): Integer; overload;
    function FindByClipId(ClipId: Integer): TMediaClipTask; overload;
    function GetEnumerator: TMediaClipTaskListEnumerator;

    property Items[Index: Integer]: TMediaClipTask read GetItems; default;
  end;

  TCustomProfileSettings = class
  private
    FFormatId: Integer;
    FVideoWidth: Integer;
    FVideoFrameRate: Double;
    FAudioEncoder: DWORD;
    FVideoEncoder: DWORD;
    FAudioBitrate: Integer;
    FVideoBitrate: Integer;
    FVideoHeight: Integer;
    FAudioSampleRate: Integer;
    FAudioChannels: Integer;
  public
    procedure AssignTo(ATask: TMediaTask);
    procedure AssignFrom(ATask: TMediaTask); overload;
    procedure AssignFrom(Source: TCustomProfileSettings); overload;
    procedure LoadConfig;
    procedure SaveConfig;

    property FormatId: Integer read FFormatId write FFormatId;
    property VideoEncoder: DWORD read FVideoEncoder write FVideoEncoder;
    property VideoWidth: Integer read FVideoWidth write FVideoWidth;
    property VideoHeight: Integer read FVideoHeight write FVideoHeight;
    property VideoFrameRate: Double read FVideoFrameRate write FVideoFrameRate;
    property VideoBitrate: Integer read FVideoBitrate write FVideoBitrate;

    property AudioEncoder: DWORD read FAudioEncoder write FAudioEncoder;
    property AudioSampleRate: Integer read FAudioSampleRate write FAudioSampleRate;
    property AudioChannels: Integer read FAudioChannels write FAudioChannels;
    property AudioBitrate: Integer read FAudioBitrate write FAudioBitrate;
  end;

  TMediaEncodeTaskList = class;

  TMediaEncodeTaskStatus = (mtsWaiting, mtsEncoding, mtsErrorAbort, mtsFinished);

  TMediaEncodeTask = class
  private
    FOwner: TMediaEncodeTaskList;
    FTask: TMediaTask;
  public
    constructor Create(AOwner: TMediaEncodeTaskList; ATask: TMediaTask);
    destructor Destroy; override;

    procedure Remove;

    function Start(hMsgWnd: HWnd=0): Boolean; virtual; abstract;
    function Stop: Boolean; virtual; abstract;
    function Pause: Boolean; virtual; abstract;
    function Resume: Boolean; virtual; abstract;

    property Owner: TMediaEncodeTaskList read FOwner;
    property Task: TMediaTask read FTask;
  end;

  TMediaTaskEncodeProgressEvent = function (Sender: TMediaEncodeTask; EncodingState: TMediaEncodingState;
    AProgress, AProgressMax: Integer; AProgressInfo: PMediaProgressInfo): Boolean of object;

  TMediaEncodeTaskListEnumerator = class
  private
    FIndex: Integer;
    FList: TMediaEncodeTaskList;
  public
    constructor Create(AList: TMediaEncodeTaskList);
    function GetCurrent: TMediaEncodeTask;
    function MoveNext: Boolean;
    property Current: TMediaEncodeTask read GetCurrent;
  end;

  TMediaEncodeTaskList = class
  private
    FOwner: TMediaTaskMgr;
    FList: TObjectList;
    function GetCount: Integer;
    function GetItems(Index: Integer): TMediaEncodeTask;
    function GetTotalOutputLength: Double;
  public
    constructor Create(AOwner: TMediaTaskMgr);
    destructor Destroy; override;

    function Add(Item: TMediaEncodeTask): Integer; overload;
    procedure Delete(Index: Integer);
    function Remove(Item: TMediaEncodeTask): Integer;
    function Extract(Item: TMediaEncodeTask): TMediaEncodeTask;
    function IndexOf(Item: TMediaEncodeTask): Integer;
    procedure Clear;
    function GetEnumerator: TMediaEncodeTaskListEnumerator;

    property Owner: TMediaTaskMgr read FOwner;
    property Count: Integer read GetCount;
    property Items[Index: Integer]: TMediaEncodeTask read GetItems; default;
    property TotalOutputLength: Double read GetTotalOutputLength;         // 所有文件输出长度
  end;

  TMediaSerialEncodeTaskList = class;

  // 普通转换任务
  TMediaNormalEncodeTask = class(TMediaEncodeTask)
  private
    FEncoder: TMediaEncoder;
    // 转换状态
    FEncodingState: TMediaEncodingState;
    FProgress, FProgressMax: Integer;
    FProgressInfo: PMediaProgressInfo;
    FEncodeProgressResult: BOOL;
    procedure DoSyncEncodeProgress;
    procedure DoSyncEncodeFinish;
    procedure FreeEncoder;
    function DoEncodeProgress(Sender: TMediaEncoder; EncodingState: TMediaEncodingState;
      AProgress, AProgressMax: Integer; AProgressInfo: PMediaProgressInfo): Boolean;
    function GetOwner: TMediaSerialEncodeTaskList;
  public
    constructor Create(AOwner: TMediaSerialEncodeTaskList; ATask: TMediaTask);
    destructor Destroy; override;

    function Start(hMsgWnd: HWnd): Boolean; override;
    function Pause: Boolean; override;
    function Resume: Boolean; override;
    function Stop: Boolean; override;

    property Owner: TMediaSerialEncodeTaskList read GetOwner;
    // 编码器, 开始转换时才有效
    property Encoder: TMediaEncoder read FEncoder;
  end;

  // 多任务按顺序转换（同一时间只能有一个转换任务进行）
  TMediaSerialEncodeTaskList = class(TMediaEncodeTaskList)
  private
    FEncodingTaskOrd: Integer;                          // 正在转换的任务索引
    FCurrentTask: TMediaNormalEncodeTask;               // 当前任务
    FEncodedTaskLength: Double;                         // 当前任务已转换长度
    FEncodedLength: Double;                             // 所有任务已转换长度
    FOnEncodeProgress: TMediaTaskEncodeProgressEvent;   // 转换回调
    function GetEncodedLength: Double;
    function GetCurrentEncoder: TMediaEncoder;
    function GetCurrentPaused: Boolean;
    function GetCurrentRunning: Boolean;
    function GetCurrentActive: Boolean;
    function GetActive: Boolean;
    procedure AddTasksToEncodeList(ATaskList: TMediaTaskList);
    function DoEncodeProgress(Sender: TMediaEncodeTask; EncodingState: TMediaEncodingState;
      AProgress, AProgressMax: Integer; AProgressInfo: PMediaProgressInfo): Boolean;
  public
    constructor Create(AOwner: TMediaTaskMgr);
    destructor Destroy; override;

    function EncodeNext(hMsgWnd: HWnd=0): Boolean;
    function Stop: Boolean;
    function Pause: Boolean;
    function Resume: Boolean;

    property Active: Boolean read GetActive;                              // 还有转换任务
    property CurrentTask: TMediaNormalEncodeTask read FCurrentTask;       // 正在转换的任务
    property CurrentEncoder: TMediaEncoder read GetCurrentEncoder;        // 当前任务的编码器
    property CurrentActive: Boolean read GetCurrentActive;                // 当前任务是否在转换或是暂停状态
    property CurrentPaused: Boolean read GetCurrentPaused;                // 判断是否被暂停
    property CurrentRunning: Boolean read GetCurrentRunning;              // 判断是否正在转换
    property EncodingTaskOrd: Integer read FEncodingTaskOrd;              // 正在转换的任务序号
    property EncodedLength: Double read GetEncodedLength;                 // 当前任务已经转换的长度
    // 转换进度回调(已切换回主线程，线程安全)
    property OnEncodeProgress: TMediaTaskEncodeProgressEvent read FOnEncodeProgress write FOnEncodeProgress; // 转换进度
  end;

  TMediaTaskMgr = class
  private
    FList: TMediaTaskList;
    FCustomProfile: TCustomProfileSettings;
    FOutputDir: UnicodeString;
    FWatermark: TWatermarkParam;
    FRegistered: Boolean;
    FMaxAudioLen: Double;
    FNotifyList: TFSEventList;
    function GetTotalOutputFileSize: UInt64;
    function GetIsAllOutputAudio: Boolean;
    function GetWatermark: PWatermarkParam;
    procedure SetWatermark(Value: PWatermarkParam);
  protected
    function AddNotify(const AEvent: TTaskNotifyEvent): Integer;
    function RemoveNotify(const AEvent: TTaskNotifyEvent): Integer;
    procedure DoRemoveTask(AItem: TMediaTask);
  public
    constructor Create();
    destructor Destroy; override;

    function CreateEncodeTaskList: TMediaSerialEncodeTaskList;          // 创建转换任务列表，需要返回列表自己释放, 创建任务列表后,请不要对任务列表进行任何增删,否则会出错
    procedure ResetAllSkip;                                             // 去除所有跳过转换标志

    property Tasks: TMediaTaskList read FList;                          // 任务列表
    property CustomProfile: TCustomProfileSettings read FCustomProfile; // 自定义Profile设置
    property Registered: Boolean read FRegistered write FRegistered;    // 是否已经注册
    property Watermark: PWatermarkParam read GetWatermark write SetWatermark; // 注册水印
    property IsAllOutputAudio: Boolean read GetIsAllOutputAudio;        // 是否所有文件都输出音频
    property MaxAudioLen: Double read FMaxAudioLen write FMaxAudioLen;  // 未注册生成音频文件长度
    property TotalOutputFileSize: UInt64 read GetTotalOutputFileSize;   // 所有文件输出大小
    property OutputDir: UnicodeString read FOutputDir write FOutputDir; // 输出目录
  end;

implementation

uses WideStrUtils, WideFileUtils, SysConfigs, MultiLang;

{ TMediaTask }

class function TMediaTask.CalcOutputFileSize(
  const AEncParam: TEncodeParam; ALength: Double): UInt64;
begin
  MediaEncodeAPI.Init;
  Result:=MediaEncodeAPI.WEGetEstimateDiskSpaceEx(AEncParam, ALength);// 精确估算
end;

procedure TMediaTask.AddChildProfileTask(ATask: TMediaMultiProfileTask);
begin
  FChildMultiProfileTasks.Add(ATask);
end;

function TMediaTask.AddChildProfileTask: TMediaMultiProfileTask;
begin
  Result:=TMediaMultiProfileTask.Create(Self, FChildMultiProfileTasks, '');
  Result.Assign(Self, False);
  Result.FTaskKind:=mtkMultiProfile;
end;

procedure TMediaTask.Assign(Source: TMediaTask; ADeep: Boolean);
begin
  if Source=nil then Exit;
// Id 不能复制，用于标识全局唯一
//  Self.FTaskKind:=Source.FTaskKind;
  Self.FOrderId:=Source.FOrderId;
  Self.FTitle:=Source.FTitle;
  Self.FOutputSubDir:=Source.FOutputSubDir;
  Self.FOutFileName:=Source.FOutFileName;
  Self.FSkipEncode:=Source.FSkipEncode;
  Self.FFormatId:=Source.FFormatId;
  Self.FFormatExt:=Source.FFormatExt;
  Self.FEncodeParams.Assign(Source.FEncodeParams);
  if ADeep then
  begin
    Self.FChildMultiProfileTasks.Assign(Source.FChildMultiProfileTasks, tlaClone);
  end;
end;

function TMediaTask.CalcOutputFileSize: UInt64;
var
  pEncParam: TEncodeParam;
begin
  pEncParam:=EncodeParams.FEncParam;
  pEncParam.bVideoDisable:=not MediaSource.HasVideoInfo;
  pEncParam.bAudioDisable:=not MediaSource.HasAudioInfo;
  Result:=CalcOutputFileSize(pEncParam, OutputLength);
end;

function TMediaTask.Clone: TMediaTask;
begin
  Result:=TMediaTaskClass(Self.ClassType).CreateNew(FOwner);
  Result.Assign(Self, True);
end;

function TMediaTask.GetCanEdit: Boolean;
begin
  Result:=Assigned(EditParams);
end;

function TMediaTask.GetChildTasks: TMediaTaskList;
begin
  if FChildTaskList=nil then
    FChildTaskList:=TMediaTaskList.Create(FOwner.Owner, False)
  else
    FChildTaskList.Clear;
  FChildTaskList.Assign(FChildMultiProfileTasks, tlaAdd);
  Result:=FChildTaskList;
end;

function TMediaTask.GetOutputLength: Double;
begin
  Result:=MediaSource.ClipLength;
end;

function TMediaTask.GetOutputStart: Double;
begin
  Result:=MediaSource.ClipStart;
end;

function TMediaTask.GetEncodeParams: TMediaTaskEncodeParams;
begin
  Result:=FEncodeParams;
  FEncodeParams.InitDefaultEncoders;
end;

function TMediaTask.GetFullOutFileName: UnicodeString;
begin
  if FOwner.Owner.OutputDir<>'' then
    Result:=WideIncludeTrailingBackslash(FOwner.Owner.OutputDir)+FOutFileName
  else
    Result:=FOutFileName;
end;

function TMediaTask.GetMediaLength: Double;
begin
  Result:=MediaSource.MediaLength;
end;

function TMediaTask.GetOutputResizeParam: RESIZE_PARAM;
begin
  with OutputVideoSize do
  begin
    Result.width:=cx;
    Result.height:=cy;
    Result.uInterpolation:=EncodeParams.FEncParam.Video.nInterpolation;
    Result.uStyle:=EditParams.ResizeStyle;  //保持原始比例
  end;
end;

function TMediaTask.GetOutputVideoSize: TSize;
begin
  with EncodeParams.FEncParam.Video do
  begin
    Result.cx:=nWidth;
    Result.cy:=nHeight;
  end;
  if (Result.cx=0) or (Result.cy=0) then // 保证输出视频大小有效（=源视频大小）
  begin
    Result.cx:=MediaSource.VideoInfo.Width;
    Result.cy:=MediaSource.VideoInfo.Height;
  end;
end;

function TMediaTask.GetPreviewResizeParam: RESIZE_PARAM;
begin
  with PreviewVideoSize do
  begin
    Result.width:=cx;
    Result.height:=cy;
    Result.uInterpolation:=EncodeParams.FEncParam.Video.nInterpolation;
    Result.uStyle:=EditParams.ResizeStyle;  //保持原始比例
  end;
end;

function TMediaTask.GetPreviewVideoSize: TSize;
begin
  with EncodeParams, MediaSource do
  begin
    if (FEncParam.Video.nWidth=0) and (FEncParam.Video.nHeight=0) then// 保证输出视频大小有效（=源视频大小）
    with FEncParam.Video do
    begin
      Result.cx:=VideoInfo.Width;
      Result.cy:=VideoInfo.Height;
    end else if (FEncParam.Video.nWidth>=VideoInfo.Width) and (FEncParam.Video.nHeight>=VideoInfo.Height) then
    with FEncParam.Video do
    begin
      Result.cx:=nWidth;
      Result.cy:=nHeight;
    end else
    begin
      if (VideoInfo.Width>=VideoInfo.Height) then
      begin
        Result.cx:=VideoInfo.Width;
        Result.cy:=Round(VideoInfo.Width*(FEncParam.Video.nHeight/FEncParam.Video.nWidth));
      end else
      begin
        Result.cy:=VideoInfo.Height;
        Result.cx:=Round(VideoInfo.Height*(FEncParam.Video.nWidth/FEncParam.Video.nHeight));
      end;
    end;
  end;
end;

function TMediaTask.InitEncoder: TMediaEncoder;
var
  bLosslessCopy: Boolean;
  ResizeParam: RESIZE_PARAM;
  pVideoSlideProcess: TVideoMediaProcessor;
  pAudioSlideProcess: TAudioMediaProcessor;
  dNewFrameRate: Double;
  bEvenFlag: Boolean;
  pMediaSource: TMediaTaskMediaSource;
//
  pEncParam: TEncodeParam;
  pEditParams: TMediaTaskEditParams;
  pVideoDec: IVideoDecoder;
  pAudioDec: IAudioDecoder;
begin
  FEncodeParams.InitDefaultEncoders;
  pMediaSource:=MediaSource;
  pEditParams:=EditParams;
  bLosslessCopy:=FEncodeParams.IsLosslessCopy;
  pMediaSource.FVDecParam.bDirectCopy:=bLosslessCopy;
  pMediaSource.FADecParam.bDirectCopy:=bLosslessCopy;
  pMediaSource.GetDecoder(TObject(nil^), mdrForEncode, pVideoDec, pAudioDec);
  if bLosslessCopy then // 无损拷贝
  begin
    Result:=TMediaEncoder.Create(pVideoDec, pAudioDec);
  end else
  begin                 // 普通加效果转换
    FillChar(ResizeParam, SizeOf(ResizeParam), 0);
    with OutputVideoSize do
    begin
      ResizeParam.width:=cx;
      ResizeParam.height:=cy;
    end;
    begin
      ResizeParam.uInterpolation:=FEncodeParams.EncParam.Video.nInterpolation;
      ResizeParam.uStyle:=pEditParams.ResizeStyle;  // 使用编辑参数中的缩放参数
    end;
    if (FEncodeParams.EncParam.dwFourCC=MakeFourCC('flv ')) or
       (FEncodeParams.EncParam.dwFourCC=MakeFourCC('avm2')) then
    begin
      with FEncodeParams.EncParam.Video do
      begin
        if dFrameRate=0 then
          dNewFrameRate:=pMediaSource.VideoInfo.frame_rate
        else
          dNewFrameRate:=dFrameRate;
      end;
      bEvenFlag:=True;      // 只有FLV文件才使用使用帧率转换和时间戳校正功能
    end else
    begin
      dNewFrameRate:=-1;    // 一般不使用时间戳校正功能
      bEvenFlag:=False;     //
    end;
    try
      pVideoSlideProcess:=TVideoMediaProcessor.Create(pVideoDec, @ResizeParam, dNewFrameRate);
    except
      pVideoSlideProcess:=nil;
      if Assigned(Classes.ApplicationHandleException) then
        Classes.ApplicationHandleException(Self);
    end;
    try
      with FEncodeParams.EncParam.Audio do
      begin
        pAudioSlideProcess:=TAudioMediaProcessor.Create(pAudioDec,
          nSampleRate, nChannels, nBitsPerSample, bEvenFlag);
      end;
    except
      pAudioSlideProcess:=nil;
      if Assigned(Classes.ApplicationHandleException) then
        Classes.ApplicationHandleException(Self);
    end;
    Result:=TMediaProcessEncoder.Create(pVideoSlideProcess, pAudioSlideProcess);
    if Assigned(pEditParams) then
    begin
      pEditParams.SetVideoProcessor(pVideoSlideProcess, True);
      pEditParams.SetAudioProcessor(pAudioSlideProcess);
    end;
  end;
  Result.SetOutputFile(FullOutFileName);
  if Assigned(Result) and (Result.Status=esHaveFile) then
  begin
    MediaEncodeMgrAPI.FCInitPlugins;
    pEncParam:=FEncodeParams.EncParam^;
//    pEncParam.Video.nResizeStyle:=pEditParams.ResizeStyle; // 使用任务编辑参数中的缩放参数
    if Assigned(pMediaSource) then
    begin
      with pEncParam.Video do
      begin
        if dFrameRate=0 then
          dFrameRate:=pMediaSource.VideoInfo.frame_rate;
        if (nWidth=0) or (nHeight=0) then // 支持保持源文件分辨率功能
        begin
          nWidth:=pMediaSource.VideoInfo.Width;
          nHeight:=pMediaSource.VideoInfo.Height;
        end;
      end;
    end;
    Result.EncodeParam:=@pEncParam;
  end;
end;

procedure TMediaTask.OnMPListNotify(Ptr: Pointer; Action: TListNotification);
begin
  case Action of
    lnAdded: TMediaMultiProfileTask(Ptr).FParentTask:=Self;
  end;
end;

constructor TMediaTask.Create(AOwner: TMediaTaskList; const ATitle: string);
begin
  CreateNew(AOwner);
  FTitle:=ATitle;
end;

constructor TMediaTask.CreateNew(AOwner: TMediaTaskList);
begin
  inherited Create;
  FOwner:=AOwner;
  FId:=AOwner.GetAutoId;
  FEncodeParams:=TMediaTaskEncodeParams.Create(Self);
  FChildMultiProfileTasks:=TMediaTaskList.Create(AOwner.Owner, True);
  FChildMultiProfileTasks.OnListNotify:=OnMPListNotify;
end;

destructor TMediaTask.Destroy;
begin
  FChildTaskList.Free;
  FChildMultiProfileTasks.Free;
  FEncodeParams.Free;
  inherited;
end;

class function TMediaTask.FromFile(AOwner: TMediaTaskList; const AFileName: UnicodeString;
  ADecModel: DEC_MODEL; AOnNewTask: TOnNewTaskEvent): Integer;
var
  k, i, j: Integer;
  pDVDInfo: TDVDIFOInfo;
  szTitleIdx, szTitle, szOutFile: string;
  pNewSource: TMediaTaskMediaSource;
  pNewTask: TMediaBaseTask;
  pMediaInfo: IMediaInfo;
  pProgramInfos: IProgramInfos;
  pProgramInfo: IProgramInfo;
begin
  Result:=0;
  if not VADecoderAPI.DecMgrInit then Exit;
  if TMediaTaskDVDSource.IsDVDIFOFile(AFileName) then
  begin
    pDVDInfo:=TDVDIFOInfo.Create(AFileName);
    if Assigned(pDVDInfo.Handle) then
    begin
      szTitleIdx:=Copy(WideExtractFileName(AFileName), 5, 2);
      for i := 0 to pDVDInfo.GetTitleCount - 1 do // 枚举标题
      if pDVDInfo.TitleAvailable(i) then
      begin
        szTitle:=format('Title_%s_%.2d', [szTitleIdx, i+1]);
        szOutFile:=WideFormat('Title_%s%.2d', [szTitleIdx, i+1]);
        pNewSource:=TMediaTaskDVDSource.Create(AFileName, ADecModel, i, 0, pDVDInfo);
        pNewTask:=TMediaBaseTask.Create(AOwner, szTitle, pNewSource);
        pNewTask.OutFileName:=szOutFile;
        if Assigned(AOnNewTask) then AOnNewTask(pNewTask);
        Inc(Result);
        for j := 0 to pDVDInfo.GetChapterCount(i) - 1 do // 枚举章节
        begin
          szTitle:=format('Chapter_%.2d', [j+1]);
          szOutFile:=WideFormat('Chapter_%s%.2d%.2d', [szTitleIdx, i+1, j+1]);
          pNewSource:=TMediaTaskDVDSource.Create(AFileName, ADecModel, i, j, pDVDInfo);
          pNewTask:=TMediaBaseTask.Create(AOwner, szTitle, pNewSource);
          pNewTask.OutFileName:=szOutFile;
          if Assigned(AOnNewTask) then AOnNewTask(pNewTask);
          Inc(Result);
        end;
      end;
    end;
  end else if TMediaTaskDVDSource.IsDVDISOFile(AFileName) then
  begin
    for k:=1 to TDVDIFOInfo.GetISOIFOCount(AFileName) do
    begin
      pDVDInfo:=TDVDIFOInfo.Create(AFileName+format(':%d', [k]));
      if Assigned(pDVDInfo.Handle) then
      begin
        szTitleIdx:=format('%.2d', [k]);
        for i := 0 to pDVDInfo.GetTitleCount - 1 do // 枚举标题
        if pDVDInfo.TitleAvailable(i) then
        begin
          szTitle:=format('Title_%s_%.2d', [szTitleIdx, i+1]);
          szOutFile:=szTitle;
          pNewSource:=TMediaTaskDVDSource.Create(AFileName, ADecModel, i, 0, pDVDInfo);
          pNewTask:=TMediaBaseTask.Create(AOwner, szTitle, pNewSource);
          pNewTask.OutFileName:=szOutFile;
          if Assigned(AOnNewTask) then AOnNewTask(pNewTask);
          Inc(Result);
          for j := 0 to pDVDInfo.GetChapterCount(i) - 1 do // 枚举章节
          begin
            szTitle:=format('Chapter_%.2d', [j+1]);
            szOutFile:=format('Chapter_%s%.2d%.2d', [szTitleIdx, i+1, j+1]);
            pNewSource:=TMediaTaskDVDSource.Create(AFileName, ADecModel, i, j, pDVDInfo);
            pNewTask:=TMediaBaseTask.Create(AOwner, szTitle, pNewSource);
            pNewTask.OutFileName:=szOutFile;
            if Assigned(AOnNewTask) then AOnNewTask(pNewTask);
            Inc(Result);
          end;
        end;
      end;
    end;
  end else
  begin
    pMediaInfo := nil;
    try
      try
        pMediaInfo:=VADecoderAPI.WSGetMediaFileInfo(AFileName, ADecModel);
        if Assigned(pMediaInfo) then
        begin
          pProgramInfos := nil;
          pMediaInfo.GetIPrograms(pProgramInfos);
          if Assigned(pProgramInfos) and (pProgramInfos.GetCount > 1) then
          begin
            for I := 0 to pProgramInfos.GetCount - 1 do
            begin
              szTitle:=format('Program_%d', [i]);
              szTitle:=ExtractFileName(AFileName);
              pNewTask:=TMediaBaseTask.Create(AOwner, szTitle, pNewSource);
              pNewTask.OutFileName:=szOutFile;
              if Assigned(AOnNewTask) then AOnNewTask(pNewTask);
              Inc(Result);
            end;  
          end
          else if Assigned(pProgramInfos) then
          begin
            pNewSource := TMediaTaskVideoSource.Create(AFileName, ADecModel, pMediaInfo, 0);
            szTitle := ExtractFileName(AFileName);
            pNewTask := TMediaBaseTask.Create(AOwner, szTitle, pNewSource);
            pNewTask.OutFileName := szOutFile;
            if Assigned(AOnNewTask) then AOnNewTask(pNewTask);
            Inc(Result);
          end;
          if Assigned(pProgramInfos) then
            pProgramInfos._Release;
          pProgramInfos := nil;
          pMediaInfo._Release;
          pMediaInfo := nil;
        end;
      except
        pMediaInfo:=nil;
        if Assigned(ApplicationHandleException) then
          Classes.ApplicationHandleException(nil);
      end;
    finally
      if Assigned(pMediaInfo) then
        VADecoderAPI.WSReleaseMediaFileInfo(pMediaInfo);
    end;
  end;
end;

procedure TMediaTask.Remove;
begin
  FOwner.Owner.DoRemoveTask(Self);
  Owner.Remove(Self);
end;

procedure TMediaTask.RemoveChildProfileTask(ATask: TMediaMultiProfileTask);
begin
  FChildMultiProfileTasks.Remove(ATask);
end;

{ TMediaTaskMediaSource }

function TMediaTaskMediaSource.GetMediaCantSeek: Boolean;
begin
  Result:=(HasVideoInfo and (VideoInfo.dwCapsFlag and MCAPS_CANT_SEEK<>0))
            or (HasAudioInfo and (AudioInfo.dwCapsFlag and MCAPS_CANT_SEEK<>0));
end;

function TMediaTaskMediaSource.GetMediaUnknownLength: Boolean;
begin
  Result:=(HasVideoInfo and (VideoInfo.dwCapsFlag and MCAPS_UNKNOWN_LENGTH<>0))
            or (HasAudioInfo and (AudioInfo.dwCapsFlag and MCAPS_UNKNOWN_LENGTH<>0));
end;

function TMediaTaskMediaSource.GetSubTitleIds(Index: Integer): Integer;
begin
  Result:=Integer(FSubTitles.Objects[Index]);
end;

procedure TMediaTaskMediaSource.Assign(Source: TMediaTaskMediaSource); 
begin
  if Source=nil then Exit;
  FLoaded:=Source.FLoaded;
  FMediaType:=Source.FMediaType;
  FFileName:=Source.FFileName;
  FMediaLength:=Source.FMediaLength;
  FClipStart:=Source.FClipStart;
  FClipEnd:=Source.FClipEnd;
  FBitrate:=Source.FBitrate;
  FVideoBitrate:=Source.FVideoBitrate;
  FAudioBitrate:=Source.FAudioBitrate;
  FVideoInfo:=Source.FVideoInfo;
  FAudioInfo:=Source.FAudioInfo;
  FHasVideoInfo:=Source.FHasVideoInfo;
  FHasAudioInfo:=Source.FHasAudioInfo;
  FHasSubTitle:=Source.FHasSubTitle;
  FSubTitles.Assign(Source.FSubTitles);
  FAudioTracks.Assign(Source.FAudioTracks);
  FDecModel:=Source.FDecModel;
  FVDecParam:=Source.FVDecParam;
  FADecParam:=Source.FADecParam;
end;

function TMediaTaskMediaSource.Clone: TMediaTaskMediaSource;
begin
  Result:=TMediaTaskMediaSourceClass(Self.ClassType).Create();
  Result.Assign(Self);
end;

constructor TMediaTaskMediaSource.Create();
begin
  inherited Create;
  FAudioTracks:=TWideStringList.Create;
  FSubTitles:=TWideStringList.Create;
end;

destructor TMediaTaskMediaSource.Destroy;
begin
  FAudioTracks.Free;
  FSubTitles.Free;
  inherited;
end;

function TMediaTaskMediaSource.GetADecParam: PDecParam;
begin
  Result:=@FADecParam;
end;

function TMediaTaskMediaSource.GetAudioDecoder(
  var AFileDecoder: TObject;
  Reason: TMediaDecodeReason): IAudioDecoder;
var
  pVideoDecoder: IVideoDecoder;
begin
  GetDecoder(AFileDecoder, Reason, pVideoDecoder, Result);
end;

function TMediaTaskMediaSource.GetAudioTrackId: Integer;
begin
  Result:=FADecParam.nTrack;
end;

function TMediaTaskMediaSource.GetAudioTrackIdx: Integer;
begin
  Result:=FAudioTracks.IndexOfObject(TObject(FADecParam.nTrack));
end;

function TMediaTaskMediaSource.GetClipLength: Double;
begin
  if FClipEnd>0 then
    Result:=FClipEnd-FClipStart
  else
    Result:=FMediaLength-FClipStart;
end;

function TMediaTaskMediaSource.GetSubTitleId: Integer;
begin
  Result:=FVDecParam.nSubTrack;
end;

function TMediaTaskMediaSource.GetSubTitleIdx: Integer;
begin
  Result:=FSubTitles.IndexOfObject(TObject(FVDecParam.nSubTrack));
end;

function TMediaTaskMediaSource.GetVDecParam: PDecParam;
begin
  Result:=@FVDecParam;
end;

function TMediaTaskMediaSource.GetVideoDecoder(var AFileDecoder: TObject;
  Reason: TMediaDecodeReason): IVideoDecoder;
var
  pAudioDecoder: IAudioDecoder;
begin
  GetDecoder(AFileDecoder, Reason, Result, pAudioDecoder);
end;

function TMediaTaskMediaSource.GetDecoder(var AFileDecoder: TObject; Reason: TMediaDecodeReason;
  out AVideoDec: IVideoDecoder; out AAudioDec: IAudioDecoder): Boolean;
var
  pMediaDecoder: TMediaDecoder;
begin
  if AFileDecoder = nil then
  begin
    pMediaDecoder := TMediaDecoder.Create(FileName, FVDecParam, FADecParam);
    if Assigned(@AFileDecoder) then
      AFileDecoder := pMediaDecoder;
  end else
    pMediaDecoder := AFileDecoder as TMediaDecoder;
  pMediaDecoder.DecodeReason := Reason;
  if Assigned(pMediaDecoder) and (pMediaDecoder.Handle = nil) then
  begin
    FreeAndNil(pMediaDecoder);
    if Assigned(@AFileDecoder) then
      AFileDecoder := nil;
  end;
  if Assigned(pMediaDecoder) then
  begin
    AVideoDec := pMediaDecoder;
    AAudioDec := pMediaDecoder;
    Result := True;
    Exit;
  end;
end;

function TMediaTaskMediaSource.LoadSubTitleList(pProgram: IProgramInfo): Integer;
var
  j: Integer;
  WS: UnicodeString;
  pSubPics: ISubPicStreamInfos;
  pSubPic: ISubPicStreamInfo;
  I: Integer;
begin
  Result:=0;
  FSubTitles.Clear;
  if Assigned(pProgram) then
  begin
    pSubPics := nil;
    pSubPic := nil;
    pProgram.GetSubPicStreamInfos(pSubPics);
    if Assigned(pSubPics) then
    begin
      for I := 0 to pSubPics.GetCount - 1 do
      begin
        pSubPic := nil;
        pSubPics.GetItem(I, pSubPic);
        if Assigned(pSubPic) then
        begin
          if pSubPic.GetID <> 0 then
          begin
            pSubPic.GetDescription(WS);
            FSubTitles.AddObject(WS, TObject(pSubPic.GetID));
          end;  
          pSubPic._Release;
        end;  
      end;  
      pSubPics._Release;
    end;  
  end;
  FHasSubTitle:=Result>0;
end;

procedure TMediaTaskMediaSource.OnDVDDecoderRelease(Sender: TObject);
begin
  if Sender = FMediaDecoder then
    FMediaDecoder:=nil;
end;

function TMediaTaskMediaSource.LoadAudioTrackList(
  pProgram: IProgramInfo): Integer;
var
  j: Integer;
  WS: UnicodeString;
  pAudioInfo: IStreamInfo;
  pAudioInfos: IStreamInfos;
  I: Integer;
begin
  Result:=0;
  FAudioTracks.Clear;
  pAudioInfos := nil;
  if Assigned(pProgram) then
  begin
    pProgram.GetAudioStreamInfos(pAudioInfos);
    if Assigned(pAudioInfos) then
    begin
      for I := 0 to pAudioInfos.GetCount - 1 do
      begin
        pAudioInfo := nil;
        pAudioInfos.GetItem(I, pAudioInfo);
        if Assigned(pAudioInfo) then
        begin
          if pAudioInfo.GetID <> 0 then
          begin
            pAudioInfo.GetDescription(WS);
            FAudioTracks.AddObject(WS, TObject(pAudioInfo.GetID));
          end;  
          pAudioInfo._Release;
        end;  
      end;  
      pAudioInfos._Release;
    end;  
  end;
end;

function TMediaTaskMediaSource.LoadFromMediaInfo(pMediaInfo: IMediaInfo;
  AProgramIdx: Integer; ANeedRelease: Boolean): Boolean;
var
  pProgInfos: IProgramInfos;
  pProgInfo: IProgramInfo;
  pVidStreamInfos: IStreamInfos;
  pAudStreamInfos: IStreamInfos;
  pVidStreamInfo: IStreamInfo;
  pAudStreamInfo: IStreamInfo;
begin
  Result:=False;
  pProgInfos := nil;
  pProgInfo := nil;
  pVidStreamInfos := nil;
  pAudStreamInfos := nil;
  pVidStreamInfo := nil;
  pAudStreamInfo := nil;
  if Assigned(pMediaInfo) then
  begin
    pMediaInfo.GetIPrograms(pProgInfos);
    if not Assigned(pProgInfos) then
      Exit;
  end;
  try
    if (AProgramIdx >= 0) and (AProgramIdx < pProgInfos.GetCount) then
    begin
      pProgInfos.GetItem(AProgramIdx, pProgInfo);
      if Assigned(pProgInfo) then
      begin
        Result := True;
        FBitrate := pProgInfo.GetBitrate;
        FMediaLength := pProgInfo.GetMediaLength;
        
        pProgInfo.GetVideoStreamInfos(pVidStreamInfos);
        FillChar(FVideoInfo, SizeOf(FVideoInfo), 0);
        if Assigned(pVidStreamInfos) then
        begin
          FHasVideoInfo := pVidStreamInfos.GetCount > 0;
          if FHasVideoInfo then
            pVidStreamInfos.GetItem(0, pVidStreamInfo);
          if FHasVideoInfo and Assigned(pVidStreamInfo) then
          begin
            if Assigned(pVidStreamInfo.GetVideoInfo) then
              FVideoInfo := pVidStreamInfo.GetVideoInfo^;
              FillChar(FVideoInfo, SizeOf(FVideoInfo), 0);
            pVidStreamInfo._Release;
            pVidStreamInfo := nil;
          end;  
          pVidStreamInfos._Release;
          pVidStreamInfos := nil;
        end;

        pProgInfo.GetAudioStreamInfos(pAudStreamInfos);
        FillChar(FAudioInfo, SizeOf(FAudioInfo), 0);
        if Assigned(pAudStreamInfos) then
        begin
          FHasAudioInfo := pAudStreamInfos.GetCount > 0;
          if FHasAudioInfo then
            pAudStreamInfos.GetItem(0, pAudStreamInfo);
          if FHasAudioInfo and Assigned(pAudStreamInfo) then
          begin
            if Assigned(pAudStreamInfo.GetAudioInfo) then
              FAudioInfo := pAudStreamInfo.GetAudioInfo^;
            pAudStreamInfo._Release;
            pAudStreamInfo := nil;
          end;  
          pAudStreamInfos._Release;
          pAudStreamInfos := nil;
        end; 
        
        //　设置好解码参数（视频）
        FVDecParam.uProgramID := pProgInfo.GetID;
        // 音频
        FADecParam.uProgramID := pProgInfo.GetID;
        // 加载字幕列表
        LoadSubTitleList(pProgInfo);
        // 加载音轨列表
        LoadAudioTrackList(pProgInfo);
        FLoaded:=True;
        Result:=True;
      end;
      
      if FMediaLength <= 0 then
        FMediaLength := pMediaInfo.GetMediaLength;
      FClipEnd := FMediaLength;
    end;
  finally
    if Assigned(pVidStreamInfo) then
      pVidStreamInfo._Release;
    if Assigned(pAudStreamInfo) then
      pAudStreamInfo._Release;
    if Assigned(pVidStreamInfos) then
      pVidStreamInfos._Release;
    if Assigned(pAudStreamInfos) then
      pAudStreamInfos._Release;
    if Assigned(pProgInfo) then
      pProgInfo._Release;
    if Assigned(pProgInfos) then
      pProgInfos._Release;
    if ANeedRelease then
      VADecoderAPI.WSReleaseMediaFileInfo(pMediaInfo);
  end;
end;

procedure TMediaTaskMediaSource.SetADecParam(Value: PDecParam);
begin
  if Assigned(Value) then
    FADecParam:=Value^;
end;

procedure TMediaTaskMediaSource.SetAudioTrackId(Value: Integer);
begin
  if Value<>FADecParam.nTrack then
  begin
    FADecParam.nTrack:=Value;
    if Assigned(FMediaDecoder) then
      FMediaDecoder.ChangeAudioTrack(Value);
  end;
end;

procedure TMediaTaskMediaSource.SetAudioTrackIdx(Value: Integer);
begin
  if (Value>=0) and (Value<FAudioTracks.Count) then
  begin
    AudioTrackId:=Integer(FAudioTracks.Objects[Value]);
  end else
    AudioTrackId:=0;
end;

procedure TMediaTaskMediaSource.SetPlayer(APlayer: TMediaPlayer; Reason: TMediaDecodeReason);
var
  pVidDec: IVideoDecoder;
  pAudDec: IAudioDecoder;
  pFileDecoder: TObject;
begin
  pFileDecoder:=nil;
  if GetDecoder(pFileDecoder, Reason, pVidDec, pAudDec) then
  begin
    if TObject(pFileDecoder) is TMediaDecoder then
    begin
      FMediaDecoder := TMediaDecoder(pFileDecoder);
      FMediaDecoder.OnRelease := Self.OnDVDDecoderRelease;
    end;
    APlayer.SetDecoder(pVidDec, pAudDec);
    APlayer.SetPlayTime(ClipStart, ClipLength);
  end;
end;

procedure TMediaTaskMediaSource.SetSubTitleId(Value: Integer);
begin
  if Value<>FVDecParam.nSubTrack then
  begin
    FVDecParam.nSubTrack:=Value;
    if Assigned(FMediaDecoder) then
      FMediaDecoder.ChangeSubtitle(Value);
  end;
end;

procedure TMediaTaskMediaSource.SetSubTitleIdx(Value: Integer);
begin
  if (Value>=0) and (Value<FSubTitles.Count) then
  begin
    SubTitleId:=Integer(FSubTitles.Objects[Value]);
  end else
    SubTitleId:=0;
end;

procedure TMediaTaskMediaSource.SetVDecParam(Value: PDecParam);
begin
  if Assigned(Value) then
    FVDecParam:=Value^;
end;

{ TMediaTaskVideoSource }

constructor TMediaTaskVideoSource.Create(const AFileName: UnicodeString;
  ADecModel: DEC_MODEL);
begin
  inherited Create();
  FMediaType:=mttVideo;
  FFileName:=AFileName;
  FDecModel:=ADecModel;
  LoadAV;
end;

constructor TMediaTaskVideoSource.Create(const AFileName: UnicodeString; ADecModel: DEC_MODEL;
  pMediaInfo: IMediaInfo; AProgramIdx: Integer);
begin
  inherited Create;
  FMediaType:=mttVideo;
  FFileName:=AFileName;
  FDecModel:=ADecModel;
  LoadFromMediaInfo(pMediaInfo, AProgramIdx, False);
end;

function TMediaTaskVideoSource.LoadAV: Boolean;
var
  pMediaInfo: IMediaInfo;
begin
  Result:=False;
  if not VADecoderAPI.DecMgrInit then Exit;
  try
    pMediaInfo:=VADecoderAPI.WSGetMediaFileInfo(FileName, FDecModel);
  except
    raise TVideoDecodeException.CreateFmt('Fail to load file %s', [FileName]);
  end;
  Result:=LoadFromMediaInfo(pMediaInfo, 0, True);
end;

{ TMediaTaskDVDSource }

procedure TMediaTaskDVDSource.Assign(Source: TMediaTaskMediaSource);
begin
  if Source=nil then Exit;
  inherited;
  if Source is TMediaTaskDVDSource then
  begin
    FTitleIndex:=TMediaTaskDVDSource(Source).FTitleIndex;
    FChapterIndex:=TMediaTaskDVDSource(Source).FChapterIndex;
    FChapterCount:=TMediaTaskDVDSource(Source).FChapterCount;
  end;
end;

constructor TMediaTaskDVDSource.Create(const AFileName: UnicodeString;
  ADecModel: DEC_MODEL; ATitleIndex, AChapterIndex: Integer; ADVDInfo: TDVDIFOInfo=nil);
begin
  inherited Create();
  FMediaType:=mttDVD;
  FFileName:=AFileName;
  FDecModel:=ADecModel;
  FTitleIndex:=ATitleIndex;
  FChapterIndex:=AChapterIndex;
  LoadDVD(ADVDInfo);
end;

destructor TMediaTaskDVDSource.Destroy;
begin
  inherited;
end;

class function TMediaTaskDVDSource.IsDVDIFOFile(
  const AFileName: UnicodeString): Boolean;
begin
  Result:=SameText(WideExtractFileExt(AFileName), '.ifo');
end;

class function TMediaTaskDVDSource.IsDVDISOFile(
  const AFileName: UnicodeString): Boolean;
begin
  Result:=SameText(WideExtractFileExt(AFileName), '.iso');
end;

function TMediaTaskDVDSource.LoadDVD(ADVDInfo: TDVDIFOInfo): Boolean;
var
  pMediaInfo: IMediaInfo;
  pDVDInfo: TDVDIFOInfo;
begin
  Result:=FLoaded;
  if not FLoaded then
  begin
    if not VADecoderAPI.DecMgrInit then Exit;
    if ADVDInfo=nil then
      pDVDInfo:=TDVDIFOInfo.Create(FFileName)
    else
      pDVDInfo:=ADVDInfo;
    try
      pMediaInfo:=pDVDInfo.GetFileInfo();
      Result:=LoadFromMediaInfo(pMediaInfo, FTitleIndex, False);
      if not Result then
        raise TVideoDecodeException.CreateFmt('Fail to load file %s', [FileName]);
      pDVDInfo.GetTitleAudioTrackList(FTitleIndex, FAudioTracks);
      pDVDInfo.GetSubTitleList(FTitleIndex, FSubTitles);
      if FChapterIndex>=0 then
      begin
        FClipStart:=pDVDInfo.GetChapterBeginTime(FTitleIndex, FChapterIndex);
        FMediaLength:=pDVDInfo.GetChapterLength(FTitleIndex, FChapterIndex);
        FChapterCount:=0;
      end else
      begin
        FClipStart:=pDVDInfo.GetTitleBeginTime(FTitleIndex);
        FMediaLength:=pDVDInfo.GetTitleLength(FTitleIndex);
        FChapterCount:=pDVDInfo.GetChapterCount(FTitleIndex);
      end;
      FVDecParam.uProgramID:=pDVDInfo.GetTitleProgramId(pMediaInfo, FTitleIndex);
      FADecParam.uProgramID:=FVDecParam.uProgramID;
      FVDecParam.nSubTrack:=pDVDInfo.GetSubTitleTrackID(FTitleIndex, 0);
      FADecParam.nSubTrack:=FVDecParam.nSubTrack;
      FADecParam.nTrack:=pDVDInfo.GetAudioTrackID(FTitleIndex, 0);
      FClipEnd:=FClipStart+FMediaLength;
    finally
      if (ADVDInfo=nil) then
        pDVDInfo.Free;
    end;
    FLoaded:=True;
  end;
end;

{ TMediaTaskMergeSource }

procedure TMediaTaskMergeSource.AddSubTask(ATask: TMediaBaseTask);
begin
  FTaskList.Add(ATask);
  FMediaLength:=MediaLength+ATask.MediaSource.ClipLength;
end;

procedure TMediaTaskMergeSource.RemoveSubTask(ATask: TMediaBaseTask);
begin
  FTaskList.Remove(ATask);
  FMediaLength:=MediaLength-ATask.MediaSource.ClipLength;
end;

procedure TMediaTaskMergeSource.AddTasksToAudioSlide(AudSlide: TSlideDecoderEx);
var
  i: Integer;
  pTask: TMediaBaseTask;
  dStartTime: Double;
begin
  if Assigned(AudSlide) then
  begin
    dStartTime:=0;
    for i:=0 to FTaskList.Count - 1 do
    begin
      pTask:=TMediaBaseTask(FTaskList[i]);
      pTask.AddToAudioSlide(AudSlide, dStartTime);
      dStartTime:=dStartTime+pTask.OutputLength;
    end;
  end;
end;

procedure TMediaTaskMergeSource.AddTasksToVideoSlide(VidSlide: TSlideDecoderEx);
var
  i: Integer;
  pTask: TMediaBaseTask;
  dStartTime: Double;
begin
  if Assigned(VidSlide) then
  begin
    dStartTime:=0;
    for i:=0 to FTaskList.Count - 1 do
    begin
      pTask:=TMediaBaseTask(FTaskList[i]);
      pTask.AddToVideoSlide(VidSlide, dStartTime);
      dStartTime:=dStartTime+pTask.OutputLength;
    end;
  end;
end;

procedure TMediaTaskMergeSource.AfterConstruction;
begin
  inherited;
  FMediaType:=mttMergeSrc;
end;

procedure TMediaTaskMergeSource.Assign(Source: TMediaTaskMediaSource);
begin
  inherited;
  if Source is TMediaTaskMergeSource then
  begin
    FTaskList.Assign(TMediaTaskMergeSource(Source).TaskList, tlaCopy);
  end;
end;

constructor TMediaTaskMergeSource.Create(AOwner: TMediaMergeTask);
begin
  inherited Create;
  FTask:=AOwner;
  FOwnerTask:=AOwner;
  FTaskList:=TMediaTaskList.Create(nil, False); // 注意:不负责子任务的释放
  FOwnerTask.Owner.Owner.AddNotify(OnRemoveTask);
end;

destructor TMediaTaskMergeSource.Destroy;
begin
  FOwnerTask.Owner.Owner.RemoveNotify(OnRemoveTask);
  FTaskList.Free;
  inherited;
end;

function TMediaTaskMergeSource.GetDecoder(var AFileDecoder: TObject; Reason: TMediaDecodeReason;
  out AVideoDec: IVideoDecoder; out AAudioDec: IAudioDecoder): Boolean;
var
  AudSlideParam: TAudioSlideParam;
  VidSlideParam: TVideoSlideParam;
  bUseVideo: Boolean;
  bUseAudio: Boolean;
  pSlideDecoder: TSlideDecoderEx;
begin
  bUseVideo := False;
  bUseAudio := False;
  if not FTask.EncodeParams.EncParam.bAudioDisable then
  with FTask.EncodeParams.EncParam.Audio do
  begin
    if (dwFourCC<>0) or (Reason<>mdrForEncode) then
    begin
      FillChar(AudSlideParam, SizeOf(AudSlideParam), 0);
      AudSlideParam.nSampleRate:=nSampleRate;
      AudSlideParam.nChannels:=nChannels;
      AudSlideParam.nBitsPerSample:=nBitsPerSample;
      AudSlideParam.nInterpolation:=SIMPLE_LINEAR;
      bUseVideo := True;
    end;
  end;
  if not FTask.EncodeParams.EncParam.bVideoDisable then
  with FTask.EncodeParams.EncParam.Video do
  begin
    if (dwFourCC<>0) or (Reason<>mdrForEncode) then
    begin
      FillChar(VidSlideParam, SizeOf(VidSlideParam), 0);
      if (nWidth<>0) or (nHeight<>0) then // 如果输出分辨率有效,使用输出分辨率
      begin
        VidSlideParam.nWidth:=nWidth;
        VidSlideParam.nHeight:=nHeight;
      end else                            // 否则使用源文件分辨率
      begin
        VidSlideParam.nWidth:=VideoInfo.Width;
        VidSlideParam.nHeight:=VideoInfo.Width;
      end;
      if dFrameRate<>0 then               // 如果输出码率有效,则使用输出码率
        VidSlideParam.dFrameRate:=dFrameRate
      else                                // 否则使用源文件码率
        VidSlideParam.dFrameRate:=VideoInfo.frame_rate;
      VidSlideParam.dwCompression:=dwCompression;
      VidSlideParam.nBitsDepth:=nBitsDepth;
      bUseVideo := True;
    end;
  end;
  pSlideDecoder := TSlideDecoderEx.Create(VidSlideParam,  @TMediaBaseTask.AudioSlideClipEvent,
    AudSlideParam, @TMediaBaseTask.VideoSlideClipEvent);
  if bUseVideo then
  begin
    AVideoDec := pSlideDecoder;
    AddTasksToVideoSlide(pSlideDecoder);
  end;
  if bUseAudio then
  begin
    AAudioDec := pSlideDecoder;
    AddTasksToAudioSlide(pSlideDecoder);
  end;  
  Result:=Assigned(AVideoDec) or Assigned(AAudioDec);
end;

function TMediaTaskMergeSource.UpdateInfoFromTaskList: Boolean;
var
  pTask: TMediaTask;
  pSource: TMediaTaskMediaSource;
begin
  Result:=FTaskList.Count>0;
  FMediaLength:=0;
  FBitrate:=0;
  for pTask in FTaskList do
  begin
    pSource:=pTask.MediaSource;
    if pSource.Bitrate>FBitrate then
      FBitrate:=pSource.Bitrate;
    if pSource.HasVideoInfo then
    begin
      FHasVideoInfo:=pSource.HasVideoInfo;
      FVideoInfo:=pSource.FVideoInfo;
      FVideoBitrate:=pSource.VideoBitrate;
    end;
    if pSource.HasAudioInfo then
    begin
      FHasAudioInfo:=pSource.HasAudioInfo;
      FAudioInfo:=pSource.FAudioInfo;
      FAudioBitrate:=pSource.AudioBitrate;
    end;
    FMediaLength:=MediaLength+pTask.MediaSource.ClipLength;
  end;
end;

procedure TMediaTaskMergeSource.OnRemoveTask(Sender: TMediaTask);
begin
  FTaskList.Remove(Sender);
end;

{ TMediaTaskEncodeParams }

procedure TMediaTaskEncodeParams.Assign(Source: TMediaTaskEncodeParams);
begin
  if Source=nil then Exit;
  FEncParam:=Source.FEncParam;  
end;

constructor TMediaTaskEncodeParams.Create(AOwner: TMediaTask);
begin
  inherited Create;
  FOwner:=AOwner;
end;

function TMediaTaskEncodeParams.GetEncParam: PEncodeParam;
begin
  Result:=@FEncParam;
end;

function TMediaTaskEncodeParams.GetIsLosslessCopy: Boolean;
begin
  Result:=FEncParam.dwFourCC=MakeFourCC('COPY');
end;

function TMediaTaskEncodeParams.GetIsOutputAudio: Boolean;
begin
  with FEncParam do
  begin
    Result:=not bAudioDisable and not IsLosslessCopy and
      (bVideoDisable or (Video.nBitrate=0));     // 去掉了视频宽高的判断,但需要规范Format.dat文件,把音频格式里面的分辨率和视频码率字段去掉
  end;
end;

function TMediaTaskEncodeParams.InitDefaultEncoders: Boolean;
var
  i: Integer;
  pEnc: PPEncoderInfo;
  pEncInfo: PEncoderInfo;
  pFormatInfo: PFileFormatInfo;
begin
  Result:=False;
  if (FEncParam.Video.dwFourCC=0) or (FEncParam.Audio.dwFourCC=0) then
  begin
    MediaEncodeMgrAPI.FCInitPlugins;
    pFormatInfo:=MediaEncodeMgrAPI.FCGetFileFormatInfos;
    if Assigned(pFormatInfo) then
    begin
      while Assigned(pFormatInfo) and (pFormatInfo.dwFourCC<>0) do
      with pFormatInfo^ do
      begin
        if dwFourCC=EncParam.dwFourCC then
        begin
          pEnc:=ppVideoEncInfos;
          if FEncParam.Video.dwFourCC=0 then
          for i := 0 to Pred(nVideoEncs) do
          begin
            pEncInfo:=pEnc^;
            if pEncInfo.dwFourCC<>0 then
            begin
              FEncParam.Video.dwFourCC:=pEncInfo.dwFourCC;
              Break;
            end;
            Inc(pEnc);
          end;
          //
          pEnc:=ppAudioEncInfos;
          if FEncParam.Audio.dwFourCC=0 then
          for i := 0 to Pred(nAudioEncs) do
          begin
            pEncInfo:=pEnc^;
            if pEncInfo.dwFourCC<>0 then
            begin
              FEncParam.Audio.dwFourCC:=pEncInfo.dwFourCC;
              Break;
            end;
            Inc(pEnc);
          end;
          Result:=True;
          Break;
        end;
        Inc(pFormatInfo);
      end;
    end;
  end;
end;

procedure TMediaTaskEncodeParams.SetEncParam(const Value: PEncodeParam);
begin
  if Assigned(Value) then
    FEncParam:=Value^;
end;

{ TMediaTaskEditParams }

function TMediaTaskEditParams.AbsoluteToRelativePos(
  const Value: TPoint): TFPoint;
var
  rOutVideoSize: TSize;
begin
  rOutVideoSize:=Owner.GetOutputVideoSize;
  if (rOutVideoSize.cx<>0) and (rOutVideoSize.cy<>0) then
  begin
    Result.X:=Value.X/rOutVideoSize.cx;
    Result.Y:=Value.Y/rOutVideoSize.cy;
  end else
  begin
    Result.X:=0;
    Result.Y:=0;
  end;
end;

function TMediaTaskEditParams.AbsoluteToRelativeSize(
  const Value: TSize): TFSize;
var
  rOutVideoSize: TSize;
begin
  rOutVideoSize:=Owner.GetOutputVideoSize;
  if (rOutVideoSize.cx<>0) and (rOutVideoSize.cy<>0) then
  begin
    Result.cx:=Value.cx/rOutVideoSize.cx;
    Result.cy:=Value.cy/rOutVideoSize.cy;
  end else
  begin
    Result.cx:=0;
    Result.cy:=0;
  end;
end;

procedure TMediaTaskEditParams.Assign(Source: TMediaTaskEditParams);
begin
  if Source=nil then Exit;
  FResizeStyleModified:=Source.FResizeStyleModified;
  FResizeStyle:=Source.ResizeStyle;
  FDeInterlacing:=Source.FDeInterlacing;
  FContrast:=Source.FContrast;
  FFilterId:=Source.FFilterId;
  FSaturation:=Source.FSaturation;
  FBrightness:=Source.FBrightness;
  FHorzFlip:=Source.FHorzFlip;
  FVertFlip:=Source.FVertFlip;
  FCropRect:=Source.FCropRect;
  FVolume:=Source.FVolume;
  FMute:=Source.FMute;
  if Assigned(Source.FWaterMarkImage) then
    FWaterMarkImage:=Source.FWaterMarkImage.Clone
  else
    FWaterMarkImage:=nil;
  FWaterMark:=Source.FWaterMark;
  FWaterMarkPos:=Source.FWaterMarkPos;
  FWaterMarkType:=Source.FWaterMarkType;
  FWaterMarkSize:=Source.FWaterMarkSize;
  FWaterMarkTrans:=Source.FWaterMarkTrans;
  FWaterMarkText:=Source.FWaterMarkText;
  FWaterMarkFile:=Source.FWaterMarkFile;
  FWaterMarkFont.Assign(Source.FWaterMarkFont);
  FSubTitle:=Source.FSubTitle;
  FSubTitlePos:=Source.FSubTitlePos;
  FSubTitleTrans:=Source.FSubTitleTrans;
  FSubTitleUserStyle:=Source.FSubTitleUserStyle;
  FSubTitleAngle:=Source.FSubTitleAngle;
  FSubTitleFont.Assign(Source.FSubTitleFont);
end;

constructor TMediaTaskEditParams.Create(AOwner: TMediaTask);
begin
  inherited Create;
  FOwner:=AOwner;
  FVolume:=KEEP_ORIGINAL_VOLUME;        // 保持原音量
  FResizeStyle:=IRM_ORIGINAL_SCALE;     // 保持原尺寸
  FWaterMarkFont:=TFont.Create;
  FWaterMarkFont.Size:= 36;
  FWaterMarkFont.Color := clWhite;
  FSubTitleFont:=TFont.Create;
  FSubTitleFont.Size := 36;
  FSubTitleFont.Color := clWhite;
end;

destructor TMediaTaskEditParams.Destroy;
begin
  FWaterMarkImage.Free;
  FSubTitleFont.Free;
  FWaterMarkFont.Free;
  inherited;
end;

function TMediaTaskEditParams.GetFlipFlag: UINT;
begin
  Result:=0;
  if HorzFlip then
    Result:=Result or FLIP_HORIZONTAL;
  if VertFlip then
    Result:=Result or FLIP_VERTICAL;
end;

function TMediaTaskEditParams.GetSubTitlePos: TPoint;
begin
  Result:=RelativeToAbsolutePos(FSubTitlePos);
end;

function TMediaTaskEditParams.GetWaterMarkImage: TDibImage;
begin
  Result:=InnerGetWaterMarkImage(1);
end;

function TMediaTaskEditParams.GetWaterMarkPos: TPoint;
begin
  Result:=RelativeToAbsolutePos(FWaterMarkPos);
end;

function TMediaTaskEditParams.GetWaterMarkRect: TFRect;
begin
  Result.left:=FWaterMarkPos.X;
  Result.top:=FWaterMarkPos.y;
  Result.right:=FWaterMarkPos.X+FWaterMarkSize.cx;
  Result.bottom:=FWaterMarkPos.Y+FWaterMarkSize.cy;
end;

function TMediaTaskEditParams.GetWaterMarkSize: TSize;
begin
  Result:=RelativeToAbsoluteSize(FWaterMarkSize);
end;

function TMediaTaskEditParams.InnerGetWaterMarkImage(AScale: Double): TDibImage;
var
  hImage: HDIBIMAGE;
  TextItem: TTextItem;
begin
  Result:=nil;
  if not WaterMark then Exit;
  Result:=FWaterMarkImage;
  if (Result=nil) then
  begin
    case WaterMarkType of
      wtImage:
        if WideFileExists(WaterMarkFile) then
          Result:=TDibImage.FromFile(WaterMarkFile); // 图片水印
      wtText:
        if WaterMarkText<>'' then
        begin
          FillChar(TextItem, SizeOf(TextItem), 0);
          WStrPLCopy(TextItem.strText, WaterMarkText, Length(TextItem.strText));
          with WaterMarkFont do
          begin
            WStrPLCopy(TextItem.strFontName, Name, Length(TextItem.strFontName));
            TextItem.crFont:=ColorToRGB(Color);
            TextItem.nFontSize:=Size;
            TextItem.uStyle:=VCLFontStyleToTextStyle(Style);
          end;
          hImage:=WSTextArtAPI.GenerateNormalTextEx(TextItem, 0, 0, nil);
          if Assigned(hImage) then
            Result:=TDibImage.Create(hImage);
        end;
    end;
    FWatermarkImage:=Result;
  end;
end;

function TMediaTaskEditParams.RelativeToAbsolutePos(
  const Value: TFPoint): TPoint;
var
  rOutVideoSize: TSize;
begin
  rOutVideoSize:=Owner.GetOutputVideoSize;
  Result.X:=Round(Value.X*rOutVideoSize.cx);
  Result.Y:=Round(Value.Y*rOutVideoSize.cy);
end;

function TMediaTaskEditParams.RelativeToAbsoluteSize(
  const Value: TFSize): TSize;
var
  rOutVideoSize: TSize;
begin
  rOutVideoSize:=Owner.GetOutputVideoSize;
  Result.cx:=Round(Value.cx*rOutVideoSize.cx);
  Result.cy:=Round(Value.cy*rOutVideoSize.cy);
end;

procedure TMediaTaskEditParams.SetFlipFlag(Value: UINT);
begin
  FHorzFlip:=Value and FLIP_HORIZONTAL<>0;
  FVertFlip:=Value and FLIP_VERTICAL<>0;
end;

procedure TMediaTaskEditParams.SetSubTitleFont(Value: TFont);
begin
  if Assigned(Value) then
    FSubTitleFont.Assign(Value);
end;

procedure TMediaTaskEditParams.SetSubTitlePos(const Value: TPoint);
begin
  FSubTitlePos:=AbsoluteToRelativePos(Value);
end;

function TMediaTaskEditParams.GetUserSubTitleParams: TUserSubPicRec;
var
  Pos: TPoint;
begin
  FillChar(Result, SizeOf(Result), 0);
  if SubTitle then
  begin
    Result.uSubPicID := Owner.MediaSource.SubTitleId;
    Result.Style.bUseDefaultStyle := not SubTitleUserStyle;
    if SubTitleUserStyle then
    begin
      Result.Style.nAngle := SubTitleAngle;
      Pos:=SubTitlePos;
      Result.Style.nLeftTopX := Pos.X;
      Result.Style.nLeftTopY := Pos.Y;
      with SubTitleFont do
      begin
        WStrPLCopy(Result.Style.szFontName, Name, Length(Result.Style.szFontName)-1);
        Result.Style.Color := ColorToRGB(Color);
        Result.Style.nFontSize := Size;
        Result.Style.uStyle := VCLFontStyleToTextStyle(Style);
      end;
    end;
    Result.nOpacity:=100-SubTitleTrans;
  end;
end;

function TMediaTaskEditParams.SetUserSubTitle(AVideoProcessor: TVideoMediaProcessor; AToEncode: Boolean): Boolean;
var
  pUserSubPic: TUserSubPicRec;
  pUserSubPicEx: TUserSubPicEx;
begin
  if Assigned(AVideoProcessor) then
  begin
    if SubTitle then
    begin
      pUserSubPic:=GetUserSubTitleParams();
      if (FSubTitleFile<>'') then
      begin
        pUserSubPicEx.pSubPicPath:=PWideChar(FSubTitleFile);
        pUserSubPicEx.Style:=pUserSubPic.Style;
        AVideoProcessor.SetSubPicEx(pUserSubPicEx, pUserSubPic.nOpacity);
      end else
        AVideoProcessor.SetUserSubPicture(@pUserSubPic, pUserSubPic.nOpacity);
    end else
      AVideoProcessor.SetUserSubPicture(nil);
    Result:=True;
  end else
    Result:=False;
end;

function TMediaTaskEditParams.SetVideoPreviewProcessor(
  AVideoProcessor: TVideoMediaProcessor): Boolean;
begin
  Result:=Assigned(AVideoProcessor);
  if Result then
  with AVideoProcessor do
  begin
    SetCropRectEx(Self.CropRect, ResizeStyle);
    SetBrightness(Self.Brightness);
    SetContrast(Self.Contrast);
    SetSaturation(Self.Saturation);
    SetExFilterFlag(Self.FilterID);
    SetFlipFlag(Self.FlipFlag);
    SetDeintelace(Self.FDeInterlacing);
    SetRotation(Round(Self.Rotation));
    // 外挂字幕（一般是MKV文件）
    SetUserSubTitle(AVideoProcessor);
//    // 用户水印
//    FreeAndNil(FWaterMarkImage);
//    pMarkImage:=InnerGetWatermarkImage(Owner.PreviewVideoSize.cx/Owner.OutputVideoSize.cx);
//    if Assigned(pMarkImage) then
//    begin
//      fRect:=CalcWatermarkPos;
//      SetUserWatermarkImage(pMarkImage.BitmapInfo, pMarkImage.Bits, @fRect, 100-WaterMarkTrans);
//    end else
//      SetUserWatermarkImage(nil, nil);
  end;
end;

function TMediaTaskEditParams.SetAudioProcessor(AAudioProcessor: TAudioMediaProcessor): Boolean;
begin
  Result:=Assigned(AAudioProcessor);
  if Result then
  begin
    if FMute then
      AAudioProcessor.SetVolum(0)
    else
      AAudioProcessor.SetVolum(FVolume);
  end;
end;

function TMediaTaskEditParams.SetVideoProcessor(
  AVideoProcessor: TVideoMediaProcessor; AToEncode: Boolean): Boolean;
begin
  Result:=Assigned(AVideoProcessor);
  if Result then
  with AVideoProcessor do
  begin
    SetCropRectEx(Self.CropRect, ResizeStyle);
    SetBrightness(Self.Brightness);
    SetContrast(Self.Contrast);
    SetSaturation(Self.Saturation);
    SetExFilterFlag(Self.FilterID);
    SetFlipFlag(Self.FlipFlag);
    SetDeintelace(Self.FDeInterlacing);
    SetRotation(Round(Self.Rotation));
    // 外挂字幕（一般是MKV文件）
    SetUserSubTitle(AVideoProcessor);
    // 用户水印
    SetVPUserWaterMark(AVideoProcessor, AToEncode);
  end;
end;

function TMediaTaskEditParams.GetUserWaterMarkParam: TUserWatermarkParam;
var
  pMarkImage: TDibImage;
begin
  pMarkImage:=GetWatermarkImage;
  FillChar(Result, SizeOf(Result), 0);
  if Assigned(pMarkImage) then
  begin
    Result.fRect:=WaterMarkRect;
    Result.lpbi:=pMarkImage.BitmapInfo;
    Result.lpBits:=pMarkImage.Bits;
    Result.nOpacity:=100-WaterMarkTrans;
  end;
end;

function TMediaTaskEditParams.SetVPUserWaterMark(
  AVideoProcessor: TVideoMediaProcessor; AToEncode: Boolean): Boolean;
var
  rWaterParam: TUserWatermarkParam;
begin
  Result:=Assigned(AVideoProcessor);
  if Result then
  with AVideoProcessor do
  begin
    rWaterParam:=GetUserWaterMarkParam();
    SetUserWatermarkImage(rWaterParam.lpbi,
      rWaterParam.lpBits, @rWaterParam.fRect, rWaterParam.nOpacity);
  end;
end;

procedure TMediaTaskEditParams.SetWaterMark(Value: Boolean);
begin
  if Value<>FWaterMark then
  begin
    FWaterMark := Value;
  end;
end;

procedure TMediaTaskEditParams.SetWaterMarkFont(Value: TFont);
begin
  if Assigned(Value) then
    FWaterMarkFont.Assign(Value);
end;

procedure TMediaTaskEditParams.SetWaterMarkImage(Value: TDibImage);
begin
  FreeAndNil(FWaterMarkImage);
  if Assigned(Value) then
    FWaterMarkImage:=Value.Clone;
end;

procedure TMediaTaskEditParams.SetWaterMarkPos(const Value: TPoint);
begin
  FWaterMarkPos:=AbsoluteToRelativePos(Value);
end;

procedure TMediaTaskEditParams.SetWaterMarkRect(const Value: TFRect);
begin
  FWaterMarkPos.X:=Value.left;
  FWaterMarkPos.y:=Value.top;
  FWaterMarkSize.cx:=Value.right-Value.left;
  FWaterMarkSize.cy:=Value.bottom-Value.top;
end;

procedure TMediaTaskEditParams.SetWaterMarkSize(const Value: TSize);
begin
  FWaterMarkSize:=AbsoluteToRelativeSize(Value);
end;

{ TMediaBaseTask }

procedure TMediaBaseTask.AddChildClipTask(ATask: TMediaClipTask);
begin
  FChildClipTasks.Add(ATask);
end;

procedure TMediaBaseTask.AfterConstruction;
begin
  inherited;
  FTaskKind:=mtkMain;
end;

procedure TMediaBaseTask.Assign(Source: TMediaTask; ADeep: Boolean);
begin
  inherited;
  FMaxClipId:=TMediaBaseTask(Source).FMaxClipId;
  if Assigned(FMediaSource) then
    FMediaSource.Assign(Source.MediaSource)
  else if Assigned(Source.MediaSource) then
  begin
    FMediaSource:=Source.MediaSource.Clone;
    FMediaSource.FTask:=Self;
  end;
  FEditParams.Assign(Source.EditParams);
  if ADeep and (Source is TMediaBaseTask) then
  begin
    FChildClipTasks.Assign(TMediaBaseTask(Source).FChildClipTasks, tlaClone);
  end;
end;

constructor TMediaBaseTask.Create(AOwner: TMediaTaskList; const ATitle: string;
  AMediaSource: TMediaTaskMediaSource);
begin
  inherited Create(AOwner, ATitle);
  FMediaSource:=AMediaSource;
  if Assigned(FMediaSource) then
  begin
    FMediaSource.FTask:=Self;
    OutFileName:=WideExtractFileName(AMediaSource.FileName);
  end;
end;

constructor TMediaBaseTask.CreateNew(AOwner: TMediaTaskList);
begin
  inherited CreateNew(AOwner);
  FEditParams:=TMediaTaskEditParams.Create(Self);
  FChildClipTasks:=TMediaClipTaskList.Create(AOwner.Owner, Self);
  FChildClipTasks.OnListNotify:=OnClipListNotify;
end;

function TMediaBaseTask.AddChildClipTask: TMediaClipTask;
var
  pLastTask: TMediaClipTask;
begin
  if FChildClipTasks.Count>0 then
    pLastTask:=TMediaClipTask(FChildClipTasks.Last)
  else
    pLastTask:=nil;
  Result:=TMediaClipTask.Create(Self, FChildClipTasks, '');
  Result.Assign(Self, False);
  Result.FTaskKind:=mtkClip;
  Result.Title:=MLText('ClipTaskTitle', 'Segment')+#32+IntToStr(FChildClipTasks.Count);
  if Assigned(pLastTask) then
  begin
    Result.MediaSource.ClipStart:=pLastTask.MediaSource.ClipEnd;
    Result.MediaSource.ClipEnd:=pLastTask.MediaSource.MediaLength;
  end;
end;

function TMediaBaseTask.AddToAudioSlide(AudSlide: TSlideDecoderEx; AStartTime: Double): Boolean;
var
  ClipTime: CLIP_TIME;
  pMediaSource: TMediaTaskMediaSource;
begin
  pMediaSource:=MediaSource;
  if Assigned(AudSlide) and Assigned(pMediaSource) then
  begin
    ClipTime.dStart:=AStartTime;
    ClipTime.dEnd:=AStartTime+pMediaSource.ClipLength;
    ClipTime.dTrimFrom:=pMediaSource.ClipStart;
    ClipTime.dTrimTo:=pMediaSource.ClipStart+pMediaSource.ClipLength;
    Result:=Assigned(AudSlide.AddAudioClip(pMediaSource.AudioInfo, ClipTime, Self));
  end else
    Result:=False;
end;

function TMediaBaseTask.AddToVideoSlide(VidSlide: TSlideDecoderEx; AStartTime: Double): Boolean;
var
  ClipTime: CLIP_TIME;
  pMediaSource: TMediaTaskMediaSource;
begin
  pMediaSource:=MediaSource;
  if Assigned(VidSlide) and Assigned(pMediaSource) then
  begin
    ClipTime.dStart:=AStartTime;
    ClipTime.dEnd:=AStartTime+pMediaSource.ClipLength;
    ClipTime.dTrimFrom:=pMediaSource.ClipStart;
    ClipTime.dTrimTo:=pMediaSource.ClipStart+pMediaSource.ClipLength;
    Result:=Assigned(VidSlide.AddVideoClip(pMediaSource.VideoInfo, ClipTime, Self));
  end else
    Result:=False;
end;

destructor TMediaBaseTask.Destroy;
begin
  FreeSlideProcess;
  FChildClipTasks.Free;
  FMediaSource.Free;
  FEditParams.Free;
  inherited;
end;

procedure TMediaBaseTask.FreeSlideProcess;
begin
  FFileDecoder:=nil;
  FVideoSlideProcess:=nil;
  FAudioSlideProcess:=nil;
end;

function TMediaBaseTask.GetAutoClipId: Integer;
begin
  if FMaxClipId<MaxInt then
    Inc(FMaxClipId)
  else
    FMaxClipId:=0;
  Result:=FMaxClipId;
end;

function TMediaBaseTask.GetChildTasks: TMediaTaskList;
begin
  Result:=inherited GetChildTasks;
  Result.Assign(FChildClipTasks, tlaAdd);
end;

function TMediaBaseTask.GetEditParams: TMediaTaskEditParams;
begin
  Result:=FEditParams;
end;

function TMediaBaseTask.GetEncodeParams: TMediaTaskEncodeParams;
begin
  Result:=FEncodeParams;
  FEncodeParams.InitDefaultEncoders;
end;

function TMediaBaseTask.GetMediaSource: TMediaTaskMediaSource;
begin
  Result:=FMediaSource;
end;

procedure TMediaBaseTask.OnClipListNotify(Ptr: Pointer;
  Action: TListNotification);
begin
  case Action of
    lnAdded: TMediaClipTask(Ptr).FParentTask:=Self;
  end;
end;

procedure TMediaBaseTask.RemoveChildClipTask(ATask: TMediaClipTask);
begin
  FChildClipTasks.Remove(ATask);
end;

function TMediaBaseTask.AudioSlideClipEvent(Sender: TSlideDecoderEx;
  nOptFlag: UINT): HAUDIODECODER;
var
  bClearIt: Boolean;
  pAudDecoder: IAudioDecoder;
begin
  Result:=nil;
  try
    case nOptFlag of
      SD_OPTFLAG_CLIP_START: // 准备音频剪辑解码器
        begin
          if FAudioSlideProcess=nil then
          begin
            pAudDecoder:=FMediaSource.GetAudioDecoder(FFileDecoder, Sender.DecodeReason);
            with Sender.AudioSlideParam do
            begin
              FAudioSlideProcess:=TAudioMediaProcessor.Create(pAudDecoder, nSampleRate, nChannels, nBitsPerSample, True);
            end;
            if Sender.DecodeReason in [mdrForPreview, mdrForEncode] then
            begin
              if Assigned(EditParams) then
                EditParams.SetAudioProcessor(FAudioSlideProcess);
            end;
          end;
          (FAudioSlideProcess as IInterface)._AddRef;
          Result:=FAudioSlideProcess.Handle;
        end;
      SD_OPTFLAG_CLIP_END:  // 释放音频剪辑解码器
        begin
          bClearIt:=FAudioSlideProcess.RefCount=1;
          (FAudioSlideProcess as IInterface)._Release;
          if bClearIt then
          begin
            FAudioSlideProcess:=nil;
            FFileDecoder:=nil;
          end;
        end;
    end;
  except
    if Assigned(ApplicationHandleException) then
      Classes.ApplicationHandleException(Self);
  end;
end;

function TMediaBaseTask.VideoSlideClipEvent(Sender: TSlideDecoderEx;
  nOptFlag: UINT): HVIDEODECODER;
var
  nClearIt: Boolean;
  pVidDecoder: IVideoDecoder;
  ResizeParam: RESIZE_PARAM;
begin
  Result:=nil;
  try
    case nOptFlag of
      SD_OPTFLAG_CLIP_START: // 准备视频剪辑解码器
        begin
          if FVideoSlideProcess=nil then
          begin
            pVidDecoder:=FMediaSource.GetVideoDecoder(FFileDecoder, Sender.DecodeReason);
            with Sender.VideoSlideParam do
            begin
              ResizeParam.width:=nWidth;
              ResizeParam.height:=nHeight;
              ResizeParam.uInterpolation:=EncodeParams.EncParam.Video.nInterpolation;
              ResizeParam.uStyle:=EditParams.ResizeStyle;
            end;
            FVideoSlideProcess:=TVideoMediaProcessor.Create(pVidDecoder, @ResizeParam, 0);
            if Sender.DecodeReason in [mdrForPreview, mdrForEncode] then
            begin
              if Assigned(EditParams) then
                EditParams.SetVideoProcessor(FVideoSlideProcess, False);
            end;
          end;
          (FVideoSlideProcess as IInterface)._AddRef;
          Result:=FVideoSlideProcess.Handle;
        end;
      SD_OPTFLAG_CLIP_END:  // 释放视频剪辑解码器
        begin
          nClearIt := FVideoSlideProcess.RefCount = 1;
          (FVideoSlideProcess as IInterface)._Release;
          if nClearIt then
          begin
            FVideoSlideProcess:=nil;
            FFileDecoder:=nil;
          end;
        end;
    end;
  except
    if Assigned(ApplicationHandleException) then
      Classes.ApplicationHandleException(Self);
  end;
end;

{ TMediaClipTask; }

procedure TMediaClipTask.AfterConstruction;
begin
  inherited;
  FTaskKind:=mtkClip;
end;

procedure TMediaClipTask.Assign(Source: TMediaTask; ADeep: Boolean);
begin
  inherited;
  if Source is TMediaClipTask then
    FClipId:=TMediaClipTask(Source).ClipId;
end;

constructor TMediaClipTask.Create(AParentTask: TMediaBaseTask;
  AOwner: TMediaTaskList; const ATitle: string);
begin
  inherited Create(AOwner, ATitle, nil);
  FParentTask:=AParentTask;
  FMediaSource:=AParentTask.MediaSource.Clone;
  if Assigned(FParentTask) then
  begin
    FClipId:=FParentTask.GetAutoClipId;
    FParentTask.AddChildClipTask(Self);
  end;
end;

destructor TMediaClipTask.Destroy;
begin
  if Assigned(FParentTask) then
    FParentTask.RemoveChildClipTask(Self);
  inherited;
end;

function TMediaClipTask.GetEditParams: TMediaTaskEditParams;
begin
  Result:=FParentTask.EditParams;
end;

{ TMediaMultiProfileTask }

procedure TMediaMultiProfileTask.AfterConstruction;
begin
  inherited;
  FTaskKind:=mtkMultiProfile;
end;

constructor TMediaMultiProfileTask.Create(AParentTask: TMediaTask;
  AOwner: TMediaTaskList; const ATitle: string);
begin
  inherited Create(AOwner, ATitle);
  FParentTask:=AParentTask;
  FEncodeParams.Assign(AParentTask.EncodeParams);
  if Assigned(FParentTask) then
    FParentTask.AddChildProfileTask(Self);
end;

destructor TMediaMultiProfileTask.Destroy;
begin
  if Assigned(FParentTask) then
    FParentTask.RemoveChildProfileTask(Self);
  inherited;
end;

function TMediaMultiProfileTask.GetEditParams: TMediaTaskEditParams;
begin
  Result:=FParentTask.EditParams;
end;

function TMediaMultiProfileTask.GetMediaSource: TMediaTaskMediaSource;
begin
  Result:=FParentTask.MediaSource;
  Result.FTask:=Self;
end;

{ TMediaMergeTask }

procedure TMediaMergeTask.AfterConstruction;
begin
  inherited;
  FTaskKind:=mtkMerge;
end;

procedure TMediaMergeTask.Assign(Source: TMediaTask; ADeep: Boolean);
begin
  inherited;
  if Source is TMediaMergeTask then
  begin
    if Assigned(FMediaSource) then
      FMediaSource.Assign(TMediaMergeTask(Source).MediaSource)
    else if Assigned(TMediaMergeTask(Source).FMediaSource) then
      FMediaSource:=TMediaTaskMergeSource(TMediaMergeTask(Source).FMediaSource.Clone);
  end;
end;

constructor TMediaMergeTask.CreateNew(AOwner: TMediaTaskList);
begin
  inherited;
  FMediaSource:=TMediaTaskMergeSource.Create(Self);
end;

destructor TMediaMergeTask.Destroy;
begin
  FMediaSource.Free;
  inherited;
end;

function TMediaMergeTask.GetEditParams: TMediaTaskEditParams;
begin
  Result:=nil;
end;

function TMediaMergeTask.GetEncodeParamsFromFirstTask: Boolean;
var
  pFirstTask: TMediaBaseTask;
begin
  Result:=False;
  if FMediaSource.TaskList.Count>0 then
    pFirstTask:=TMediaBaseTask(FMediaSource.TaskList.First)
  else
    pFirstTask:=nil;
  if Assigned(pFirstTask) then
  begin
    FEncodeParams.Assign(pFirstTask.FEncodeParams);
    Result:=True;
  end;
end;

function TMediaMergeTask.GetMediaSource: TMediaTaskMediaSource;
begin
  Result:=FMediaSource;
  FMediaSource.UpdateInfoFromTaskList;
end;

function TMediaMergeTask.InitEncoder: TMediaEncoder;
var
  pVideoDec: IVideoDecoder;
  pAudioDec: IAudioDecoder;
//
  pEncParam: TEncodeParam;
  pMediaSource: TMediaTaskMediaSource;
begin
  FEncodeParams.InitDefaultEncoders;         // 初始化编码参数中的默认编码器
  pMediaSource:=MediaSource;
  pMediaSource.GetDecoder(TObject(nil^), mdrForEncode, pVideoDec, pAudioDec);
  Result:=TMediaProcessEncoder.Create(pVideoDec, pAudioDec);  // 这里主要是加版权水印，效果由每个子任务自己处理
  Result.SetOutputFile(FullOutFileName);   // 设置输出文件
  MediaEncodeMgrAPI.FCInitPlugins;
  pEncParam:=FEncodeParams.EncParam^;
  begin
    with pEncParam.Video do
    begin
      if dFrameRate=0 then
        dFrameRate:=pMediaSource.VideoInfo.frame_rate;
      if (nWidth=0) or (nHeight=0) then // 支持保持源文件分辨率功能
      begin
        nWidth:=pMediaSource.VideoInfo.Width;
        nHeight:=pMediaSource.VideoInfo.Height;
      end;
    end;
  end;
  Result.EncodeParam:=@pEncParam;
end;

{ TMediaTaskListEnumerator }

constructor TMediaTaskListEnumerator.Create(AList: TMediaTaskList);
begin
  inherited Create;
  FList:=AList;
  FIndex:=-1;
end;

function TMediaTaskListEnumerator.GetCurrent: TMediaTask;
begin
  Result:=FList[FIndex];
end;

function TMediaTaskListEnumerator.MoveNext: Boolean;
begin
  Result:=FIndex<FList.Count-1;
  if Result then
    Inc(FIndex);
end;

{ TMediaTaskList }

function TMediaTaskList.Add(Item: TMediaTask): Integer;
begin
  Result:=FList.Add(Item);
end;

procedure TMediaTaskList.Assign(Source: TMediaTaskList; AOperator: TTaskListAssignOp);
var
  pTask, pNewTask: TMediaTask;
begin
  if AOperator<>tlaAdd then
    Clear;
  for pTask in Source do
  begin
    case AOperator of
      tlaAdd,
      tlaCopy:
        Add(pTask);
      tlaClone:
        begin
          pNewTask:=pTask.Clone;
          pNewTask.FOwner:=Self;
          Add(pNewTask);
        end;
    end;
  end;
end;

function TMediaTaskList.CheckExists(const AFileName: UnicodeString): Boolean;
begin
  Result:=Assigned(Find(AFileName));
end;

function TMediaTaskList.CheckRepeatAndRenameOutFile(ASkipItem: TMediaTask;
  const AFileName, AExt: UnicodeString): UnicodeString;
var
  LastRepeatCount: Integer;
  nRepeatCount: Integer;
  szFile: UnicodeString;
begin
  szFile:=AFileName;
  LastRepeatCount:=0;
  repeat
    nRepeatCount:=FindRepeat(ASkipItem, szFile+AExt);
    if nRepeatCount>0 then
    begin
      if LastRepeatCount=0 then
        LastRepeatCount:=nRepeatCount
      else
        Inc(LastRepeatCount);
      szFile:=WideFormat('%s(%d)', [AFileName, LastRepeatCount]);
    end;
  until nRepeatCount=0;
  Result:=szFile+AExt;
end;

procedure TMediaTaskList.Clear;
begin
  FList.Clear;
end;

function TMediaTaskList.Clone: TMediaTaskList;
begin
  Result:=TMediaTaskList.Create(FOwner, True);
  Result.Assign(Self, tlaClone);
end;

constructor TMediaTaskList.Create(AOwner: TMediaTaskMgr; AOwnedObject: Boolean);
begin
  inherited Create();
  FOwner:=AOwner;
  FList:=TFSObjectList.Create(AOwnedObject);
  FList.OnNotify:=DoListNotify;
end;

procedure TMediaTaskList.Delete(Index: Integer);
begin
  FList.Delete(Index);
end;

destructor TMediaTaskList.Destroy;
begin
  FList.Free;
  inherited;
end;

procedure TMediaTaskList.DoListNotify(Ptr: Pointer; Action: TListNotification);
begin
  if Assigned(FOnListNotify) then FOnListNotify(Ptr, Action);
  case Action of
    lnAdded: if FList.OwnsObjects then TMediaTask(Ptr).FOwner:=Self;
  end;
end;

function TMediaTaskList.Extract(Item: TMediaTask): TMediaTask;
begin
  Result:=TMediaTask(FList.Extract(Item));
end;

function TMediaTaskList.Find(const AFileName: UnicodeString): TMediaTask;
var
  pItem: TMediaTask;
begin
  for pItem in Self do
  begin
    if (pItem.TaskKind=mtkMain) then
      if WideSameText(pItem.MediaSource.FileName, AFileName) then
      begin
        Result:=pItem;
        Exit;
      end;
  end;
  Result:=nil;
end;

function TMediaTaskList.Find(ASkipItem: TMediaTask;
  const AFileName: UnicodeString): TMediaTask;
var
  pItem: TMediaTask;
begin
  for pItem in Self do
  begin
    if (pItem<>ASkipItem) and (pItem.TaskKind=mtkMain)
      and WideSameText(pItem.MediaSource.FileName, AFileName) then
    begin
      Result:=pItem;
      Exit;
    end;
  end;
  Result:=nil;
end;

function TMediaTaskList.FindOutNoExt(ASkipItem: TMediaTask; const AFileName: UnicodeString): Integer;
var
  szFile: UnicodeString;
  pItem: TMediaTask;
begin
  Result:=0;
  szFile:=WideChangeFileExt(WideExtractFileName(AFileName), '');
  for pItem in Self do
  begin
    if (pItem<>ASkipItem) and
      WideSameText(WideChangeFileExt(WideExtractFileName(pItem.OutFileName), ''), szFile) then
    begin
      Inc(Result);
    end;
  end;
end;

function TMediaTaskList.FindRepeat(ASkipItem: TMediaTask;
  const AFileName: UnicodeString): Integer;
var
  pItem: TMediaTask;
begin
  Result:=0;
  for pItem in Self do
  begin
    if (pItem<>ASkipItem) and
      WideSameText(WideExtractFileName(pItem.OutFileName), AFileName) then
    begin
      Inc(Result);
    end;
  end;
end;

function TMediaTaskList.First: TMediaTask;
begin
  Result:=Items[0];
end;

function TMediaTaskList.Find(Id: Integer): TMediaTask;
var
  i: Integer;
  pItem: TMediaTask;
begin
  for i:=0 to FList.Count-1 do
  begin
    pItem:=TMediaTask(FList[i]);
    if pItem.Id=Id then
    begin
      Result:=pItem;
      Exit;
    end;
  end;
  Result:=nil;
end;

class function TMediaTaskList.GetAutoId: Integer;
begin
  if FAutoId<MaxInt then
    Inc(FAutoId)
  else
    FAutoId:=0;
  Result:=FAutoId;
end;

function TMediaTaskList.GetCount: Integer;
begin
  Result:=FList.Count;
end;

function TMediaTaskList.GetEnumerator: TMediaTaskListEnumerator;
begin
  Result:=TMediaTaskListEnumerator.Create(Self);
end;

function TMediaTaskList.GetItems(Index: Integer): TMediaTask;
begin
  Result:=TMediaTask(FList[Index]);
end;

function TMediaTaskList.IndexOf(Id: Integer): Integer;
var
  i: Integer;
  pItem: TMediaTask;
begin
  for i:=0 to FList.Count-1 do
  begin
    pItem:=TMediaTask(FList[i]);
    if pItem.Id=Id then
    begin
      Result:=i;
      Exit;
    end;
  end;
  Result:=-1;
end;

function TMediaTaskList.IndexOf(Item: TMediaTask): Integer;
begin
  Result:=FList.IndexOf(Item);
end;

procedure TMediaTaskList.Insert(Index: Integer; Item: TMediaTask);
begin
  FList.Insert(Index, Item);
end;

function TMediaTaskList.Last: TMediaTask;
begin
  Result:=Items[Count-1];
end;

procedure TMediaTaskList.Move(CurIndex, NewIndex: Integer);
begin
  FList.Move(CurIndex, NewIndex);
end;

function TMediaTaskList.Remove(Item: TMediaTask): Integer;
begin
  Result:=FList.Remove(Item);
end;

function MediaListSortByOrderIdCompare(Item1, Item2: Pointer): Integer;
begin
  Result:=TMediaTask(Item1).OrderId-TMediaTask(Item2).OrderId;
end;

procedure TMediaTaskList.SortByOrderId;
begin
  FList.Sort(MediaListSortByOrderIdCompare);
end;

{ TMediaClipTaskListEnumerator }

constructor TMediaClipTaskListEnumerator.Create(AList: TMediaClipTaskList);
begin
  inherited Create;
  FList:=AList;
  FIndex:=-1;
end;

function TMediaClipTaskListEnumerator.GetCurrent: TMediaClipTask;
begin
  Result:=TMediaClipTask(FList[FIndex]);
end;

function TMediaClipTaskListEnumerator.MoveNext: Boolean;
begin
  Result:=FIndex<FList.Count-1;
  if Result then
    Inc(FIndex);
end;

{ TMediaClipTaskList }

constructor TMediaClipTaskList.Create(AOwner: TMediaTaskMgr;
  ABaseTask: TMediaBaseTask);
begin
  inherited Create(AOwner, True);
  FBaseTask:=ABaseTask;
end;

function TMediaClipTaskList.Extract(Item: TMediaClipTask): TMediaClipTask;
begin
  Result:=TMediaClipTask(inherited Extract(Item));
end;

function TMediaClipTaskList.FindByClipId(ClipId: Integer): TMediaClipTask;
begin
  for Result in Self do
  begin
    if Result.ClipId=ClipId then Exit;
  end;
  Result:=nil;
end;

function TMediaClipTaskList.GetEnumerator: TMediaClipTaskListEnumerator;
begin
  Result:=TMediaClipTaskListEnumerator.Create(Self);
end;

function TMediaClipTaskList.GetItems(Index: Integer): TMediaClipTask;
begin
  Result:=TMediaClipTask(inherited Items[Index]);
end;

function TMediaClipTaskList.IndexOfByClipId(ClipId: Integer): Integer;
var
  pTask: TMediaClipTask;
begin
  for Result:=0 to Count - 1 do
  begin
    pTask:=Items[Result];
    if pTask.ClipId=ClipId then Exit;
  end;
  Result:=-1;
end;

procedure TMediaClipTaskList.Merge(Source: TMediaClipTaskList; ADeep: Boolean);
var
  i, idx: Integer;
  pTask, pDestTask: TMediaClipTask;
begin
  for i:=Count - 1 downto 0 do // 处理被删除的剪辑
  begin
    pTask:=Items[i];
    idx:=Source.IndexOfByClipId(pTask.ClipId);
    if idx=-1 then
      Delete(i);
  end;
  for pTask in Source do  // 处理保留的剪辑，新增的剪辑
  begin
    pDestTask:=FindByClipId(pTask.ClipId);
    if Assigned(pDestTask) then // 保留的
    begin
      pDestTask.Assign(pTask, ADeep);
    end else  // 新增的
    begin
      pDestTask:=TMediaClipTask.Create(FBaseTask, Self, '');
      pDestTask.Assign(pTask, ADeep);
    end;
  end;
end;

{ TCustomProfileSettings }

procedure TCustomProfileSettings.AssignFrom(ATask: TMediaTask);
begin
  with ATask.EncodeParams.EncParam^ do
  begin
    FFormatId:=ATask.FormatId;
    FVideoEncoder:=Video.dwFourCC;
    FVideoBitrate:=Video.nBitrate;
    FVideoFrameRate:=Video.dFrameRate;
    FVideoWidth:=Video.nWidth;
    FVideoHeight:=Video.nHeight;

    FAudioEncoder:=Audio.dwFourCC;
    FAudioBitrate:=Audio.nBitrate;
    FAudioSampleRate:=Audio.nSampleRate;
    FAudioChannels:=Audio.nChannels;
  end;
end;

procedure TCustomProfileSettings.AssignFrom(Source: TCustomProfileSettings);
begin
  FFormatId:=Source.FormatId;
  FVideoEncoder:=Source.FVideoEncoder;
  FVideoBitrate:=Source.FVideoBitrate;
  FVideoFrameRate:=Source.FVideoFrameRate;
  FVideoWidth:=Source.FVideoWidth;
  FVideoHeight:=Source.FVideoHeight;

  FAudioEncoder:=Source.FAudioEncoder;
  FAudioBitrate:=Source.FAudioBitrate;
  FAudioSampleRate:=Source.FAudioSampleRate;
  FAudioChannels:=Source.FAudioChannels;
end;

procedure TCustomProfileSettings.AssignTo(ATask: TMediaTask);
begin
  if FFormatId=0 then Exit;

  ATask.FormatId:=FFormatId;
  with ATask.EncodeParams.EncParam^ do
  begin
    if FVideoEncoder<>0 then
      Video.dwFourCC:=FVideoEncoder;
    if FVideoBitrate<>0 then
      Video.nBitrate:=FVideoBitrate;
//  if FVideoFrameRate<>0 then     // =0时保持原来的帧率
      Video.dFrameRate:=FVideoFrameRate;
//    if FVideoWidth<>0 then     　// =0时保持原来的视频大小
      Video.nWidth:=FVideoWidth;
//    if FVideoHeight<>0 then      // =0时保持原来的视频大小
      Video.nHeight:=FVideoHeight;
    if FAudioEncoder<>0 then
      Audio.dwFourCC:=FAudioEncoder;
    if FAudioBitrate<>0 then
      Audio.nBitrate:=FAudioBitrate;
    if FAudioSampleRate<>0 then
      Audio.nSampleRate:=FAudioSampleRate;
    if FAudioChannels<>0 then
      Audio.nChannels:=FAudioChannels;
  end;
end;

procedure TCustomProfileSettings.LoadConfig;
begin
  gConfig.Section:='Output';
  FFormatId:=gConfig.ReadInteger('FormatId', 0);
  FVideoEncoder:=gConfig.ReadInteger('VideoFourCC', FVideoEncoder);
  FVideoBitrate:=gConfig.ReadInteger('VideoBitrate', FVideoBitrate);
  FVideoFrameRate:=gConfig.ReadFloat('VideoFrameRate', FVideoFrameRate);
  FVideoWidth:=gConfig.ReadInteger('VideoWidth', FVideoWidth);
  FVideoHeight:=gConfig.ReadInteger('VideoHeight', FVideoHeight);

  FAudioEncoder:=gConfig.ReadInteger('AudioFourCC', FAudioEncoder);
  FAudioBitrate:=gConfig.ReadInteger('AudioBitrate', FAudioBitrate);
  FAudioSampleRate:=gConfig.ReadInteger('AudioSampleRate', FAudioSampleRate);
  FAudioChannels:=gConfig.ReadInteger('Channels', FAudioChannels);
end;

procedure TCustomProfileSettings.SaveConfig;
begin
  gConfig.Section:='Output';
  gConfig.WriteInteger('FormatId', FFormatId);

  gConfig.WriteInteger('VideoFourCC', FVideoEncoder);
  gConfig.WriteInteger('VideoBitrate', FVideoBitrate);
  gConfig.WriteFloat('VideoFrameRate', FVideoFrameRate);
  gConfig.WriteInteger('VideoWidth', FVideoWidth);
  gConfig.WriteInteger('VideoHeight', FVideoHeight);

  gConfig.WriteInteger('AudioFourCC', FAudioEncoder);
  gConfig.WriteInteger('AudioBitrate', FAudioBitrate);
  gConfig.WriteInteger('AudioSampleRate', FAudioSampleRate);
  gConfig.WriteInteger('Channels', FAudioChannels);
end;

{ TMediaTaskMgr }

function TMediaTaskMgr.AddNotify(const AEvent: TTaskNotifyEvent): Integer;
begin
  Result:=FNotifyList.Add(TMethod(AEvent));
end;

constructor TMediaTaskMgr.Create;
begin
  inherited Create;
  FList:=TMediaTaskList.Create(Self, True);
  FCustomProfile:=TCustomProfileSettings.Create;
  FNotifyList:=TFSEventList.Create;
end;

destructor TMediaTaskMgr.Destroy;
begin
  FreeAndNil(FNotifyList);
  FCustomProfile.Free;
  FList.Free;
  inherited;
end;

procedure TMediaTaskMgr.DoRemoveTask(AItem: TMediaTask);
var
  i: Integer;
begin
  if Assigned(FNotifyList) then
  begin
    for i:=0 to FNotifyList.Count - 1 do
    begin
      TTaskNotifyEvent(FNotifyList[i])(AItem);
    end;
  end;
end;

function TMediaTaskMgr.GetIsAllOutputAudio: Boolean;

  function GetIsAllOutputAudio(ATask: TMediaTask): Boolean;
  var
    pChildTask: TMediaTask;
  begin
    Result:=not ATask.SkipEncode and ATask.EncodeParams.IsOutputAudio;
    if not Result then
      Exit;
    for pChildTask in ATask.GetChildTasks do
    begin
      Result:=GetIsAllOutputAudio(pChildTask);
      if not Result then Exit;
    end;
  end;

var
  pTask: TMediaTask;
begin
  for pTask in FList do
  begin
    Result:=GetIsAllOutputAudio(pTask);
    if not Result then Exit;
  end;
  Result:=True;
end;

function TMediaTaskMgr.GetTotalOutputFileSize: UInt64;

  function GetTotalOutputFileSize(ATask: TMediaTask): UInt64;
  var
    pChildTask: TMediaTask;
  begin
    Result:=0;
    if not ATask.SkipEncode then
      Inc(Result, ATask.CalcOutputFileSize);
    for pChildTask in ATask.GetChildTasks do
    begin
      Inc(Result, GetTotalOutputFileSize(pChildTask));
    end;
  end;
  
var
  pTask: TMediaTask;
begin
  Result:=0;
  for pTask in FList do
  begin
    Inc(Result, GetTotalOutputFileSize(pTask));
  end;
end;

function TMediaTaskMgr.GetWatermark: PWatermarkParam;
begin
  Result:=@FWatermark;
end;

function TMediaTaskMgr.RemoveNotify(const AEvent: TTaskNotifyEvent): Integer;
begin
  Result:=FNotifyList.Remove(TMethod(AEvent));
end;

procedure TMediaTaskMgr.ResetAllSkip;

  procedure ResetSkip(ATask: TMediaTask);
  var
    pChildTask: TMediaTask;
  begin
    ATask.SkipEncode:=False;
    for pChildTask in ATask.GetChildTasks do
    begin
      ResetSkip(pChildTask);
    end;
  end;

var
  pTask: TMediaTask;
begin
  for pTask in FList do
  begin
    ResetSkip(pTask);
  end;
end;

procedure TMediaTaskMgr.SetWatermark(Value: PWatermarkParam);
begin
  FWatermark:=Value^;
end;

function TMediaTaskMgr.CreateEncodeTaskList: TMediaSerialEncodeTaskList;
begin
  Result:=TMediaSerialEncodeTaskList.Create(Self);
end;

{ TMediaEncodeTask }

constructor TMediaEncodeTask.Create(AOwner: TMediaEncodeTaskList; ATask: TMediaTask);
begin
  inherited Create;
  FOwner:=AOwner;
  FTask:=ATask;
end;

procedure TMediaEncodeTask.Remove;
begin
  FOwner.Remove(Self);
end;

destructor TMediaEncodeTask.Destroy;
begin
  inherited;
end;

{ TMediaNormalEncodeTask }

function TMediaNormalEncodeTask.Pause: Boolean;
begin
  Result:=False;
  if Assigned(FEncoder) then
  begin
    FEncoder.Pause;
    Result:=True;
  end;
end;

function TMediaNormalEncodeTask.Resume: Boolean;
begin
  Result:=False;
  if Assigned(FEncoder) then
  begin
    FEncoder.Resume;
    Result:=True;
  end;
end;

function TMediaNormalEncodeTask.Start(hMsgWnd: HWnd): Boolean;
var
  dStart, dLength: Double;
begin
  Result:=False;
  FreeAndNil(FEncoder);
  FEncoder:=FTask.InitEncoder;
  if Assigned(FEncoder) then
  begin
    dStart:=FTask.OutputStart;
    dLength:=FTask.OutputLength;
    if not Owner.Owner.Registered then// 如果没有注册
    begin
      if FTask.FEncodeParams.IsOutputAudio then // 纯音频设置时长限制（一般３分钟，180秒）
      begin
        if dLength>Owner.Owner.MaxAudioLen then
          dLength:=Owner.Owner.MaxAudioLen;
      end;
      // 版权水印
      if FEncoder is TMediaProcessEncoder then
      begin
        with Owner.Owner.FWatermark do
          TMediaProcessEncoder(FEncoder).VideoProcessor.SetWatermarkSrcIDAlign(hModule, @szID, nAlign, nOpacity);
        TMediaProcessEncoder(FEncoder).VideoProcessor.DisableWatermark(False);
      end;
    end;
    FEncoder.OnEncodeProgress:=DoEncodeProgress;
    Result:=FEncoder.Start(dStart, dLength, hMsgWnd)=0;
  end;
end;

function TMediaNormalEncodeTask.Stop: Boolean;
begin
  Result:=False;
  if Assigned(FEncoder) then
  begin
    CheckSynchronize(0);
    FEncoder.Stop();
    FreeAndNil(FEncoder);
    Result:=True;
  end;
end;

procedure TMediaNormalEncodeTask.FreeEncoder;
begin
  if GetCurrentThreadId=MainThreadId then
    CheckSynchronize(0);
  FreeAndNil(FEncoder);
end;

function TMediaNormalEncodeTask.GetOwner: TMediaSerialEncodeTaskList;
begin
  Result:=FOwner as TMediaSerialEncodeTaskList;
end;

constructor TMediaNormalEncodeTask.Create(AOwner: TMediaSerialEncodeTaskList;
  ATask: TMediaTask);
begin
  inherited Create(AOwner, ATask);
end;

destructor TMediaNormalEncodeTask.Destroy;
begin

  inherited;
end;

function TMediaNormalEncodeTask.DoEncodeProgress(Sender: TMediaEncoder;
  EncodingState: TMediaEncodingState; AProgress, AProgressMax: Integer;
  AProgressInfo: PMediaProgressInfo): Boolean;
begin
  FEncodingState:=EncodingState;
  FProgress:=AProgress;
  FProgressMax:=AProgressMax;
  case EncodingState of
    mesFinished, mesUserAbort, mesCallBackAbort, mesError:
      begin
        FProgressInfo:=nil; // 非同步调用，不能保存指针
        TThread.Queue(nil, DoSyncEncodeFinish); // 加入到队列，返回, 等候处理
      end;
  else
    FProgressInfo:=AProgressInfo;
    TThread.Synchronize(nil, DoSyncEncodeProgress);
  end;
  Result:=FEncodeProgressResult;
end;

procedure TMediaNormalEncodeTask.DoSyncEncodeFinish;
begin
  FreeEncoder;
  DoSyncEncodeProgress;
end;

procedure TMediaNormalEncodeTask.DoSyncEncodeProgress;
begin
  if Assigned(FOwner) then
    FEncodeProgressResult:=Owner.DoEncodeProgress(Self, FEncodingState, FProgress, FProgressMax,
        FProgressInfo)
  else
    FEncodeProgressResult:=True;
end;

{ TMediaEncodeTaskListEnumerator }

constructor TMediaEncodeTaskListEnumerator.Create(AList: TMediaEncodeTaskList);
begin
  inherited Create;
  FList:=AList;
  FIndex:=-1;
end;

function TMediaEncodeTaskListEnumerator.GetCurrent: TMediaEncodeTask;
begin
  Result:=FList[FIndex];
end;

function TMediaEncodeTaskListEnumerator.MoveNext: Boolean;
begin
  Result:=FIndex<FList.Count - 1;
  if Result then
    Inc(FIndex);
end;

{ TMediaEncodeTaskList }

function TMediaEncodeTaskList.Add(Item: TMediaEncodeTask): Integer;
begin
  Result:=FList.Add(Item);
end;

procedure TMediaEncodeTaskList.Clear;
begin
  FList.Clear;
end;

constructor TMediaEncodeTaskList.Create(AOwner: TMediaTaskMgr);
begin
  inherited Create;
  FOwner:=AOwner;
  FList:=TObjectList.Create(True);
end;

procedure TMediaEncodeTaskList.Delete(Index: Integer);
begin
  FList.Delete(Index);
end;

destructor TMediaEncodeTaskList.Destroy;
begin
  inherited;
  FList.Free;
end;

function TMediaEncodeTaskList.Extract(Item: TMediaEncodeTask): TMediaEncodeTask;
begin
  Result:=TMediaEncodeTask(FList.Extract(Item));
end;

function TMediaEncodeTaskList.GetCount: Integer;
begin
  Result:=FList.Count;
end;

function TMediaEncodeTaskList.GetEnumerator: TMediaEncodeTaskListEnumerator;
begin
  Result:=TMediaEncodeTaskListEnumerator.Create(Self);
end;

function TMediaEncodeTaskList.GetItems(Index: Integer): TMediaEncodeTask;
begin
  Result:=TMediaEncodeTask(FList[Index]);
end;

function TMediaEncodeTaskList.IndexOf(Item: TMediaEncodeTask): Integer;
begin
  Result:=FList.IndexOf(Item);
end;

function TMediaEncodeTaskList.Remove(Item: TMediaEncodeTask): Integer;
begin
  Result:=FList.Remove(Item);
end;

function TMediaEncodeTaskList.GetTotalOutputLength: Double;
var
  pTask: TMediaEncodeTask;
begin
  Result:=0;
  for pTask in Self do
  begin
    Result:=Result+pTask.Task.OutputLength;
  end;
end;

{ TMediaSerialEncodeTaskList }

function TMediaSerialEncodeTaskList.DoEncodeProgress(Sender: TMediaEncodeTask;
  EncodingState: TMediaEncodingState; AProgress, AProgressMax: Integer;
  AProgressInfo: PMediaProgressInfo): Boolean;
begin
  case EncodingState of
    mesEncoding:
      begin
        FEncodedTaskLength:=AProgressInfo.dCurrTime;
      end;
    mesFinished:
      begin
        FEncodedTaskLength:=0;
        FEncodedLength:=FEncodedLength+Sender.FTask.OutputLength;
      end;
  end;
  if Assigned(FOnEncodeProgress) then
    Result:=FOnEncodeProgress(Sender, EncodingState, AProgress, AProgressMax,
      AProgressInfo)
  else
    Result:=True;
end;

function TMediaSerialEncodeTaskList.GetEncodedLength: Double;
begin
  Result:=FEncodedTaskLength+FEncodedLength;
end;

procedure TMediaSerialEncodeTaskList.AddTasksToEncodeList(ATaskList: TMediaTaskList);

  procedure AddTaskToEncodeList(ATask: TMediaTask);
  var
    pChildTask: TMediaTask;
    pEncodeTask: TMediaEncodeTask;
  begin
    if not ATask.SkipEncode then
    begin
      pEncodeTask:=TMediaNormalEncodeTask.Create(Self, ATask);
      Self.Add(pEncodeTask);
    end;
    for pChildTask in ATask.GetChildTasks do
    begin
      AddTaskToEncodeList(pChildTask);
    end;
  end;

var
  pTask: TMediaTask;
begin
  for pTask in ATaskList do
  begin
    AddTaskToEncodeList(pTask);
  end;
end;

constructor TMediaSerialEncodeTaskList.Create(AOwner: TMediaTaskMgr);
begin
  inherited Create(AOwner);
  AddTasksToEncodeList(AOwner.Tasks);
end;

destructor TMediaSerialEncodeTaskList.Destroy;
begin
  Stop;
  inherited;
end;

function TMediaSerialEncodeTaskList.EncodeNext(hMsgWnd: HWnd=0): Boolean;
var
  i: Integer;
  pTask: TMediaNormalEncodeTask;
begin
  for i:=FEncodingTaskOrd to FList.Count-1 do
  begin
    pTask:=TMediaNormalEncodeTask(Items[i]);
    Result:=pTask.Start(hMsgWnd);
    if Result then
    begin
      Inc(FEncodingTaskOrd);
      FCurrentTask:=pTask;
      Exit;
    end;
  end;
  FCurrentTask:=nil;
  Result:=True;
end;

function TMediaSerialEncodeTaskList.GetCurrentEncoder: TMediaEncoder;
begin
  if Assigned(FCurrentTask) then
    Result:=FCurrentTask.Encoder
  else
    Result:=nil;
end;

function TMediaSerialEncodeTaskList.GetCurrentActive: Boolean;
begin
  if Assigned(CurrentEncoder) then
    Result:=CurrentEncoder.Status in [esEncoding, esPause]
  else
    Result:=False;
end;

function TMediaSerialEncodeTaskList.GetCurrentPaused: Boolean;
begin
  if Assigned(CurrentEncoder) then
    Result:=CurrentEncoder.Status=esPause
  else
    Result:=False;
end;

function TMediaSerialEncodeTaskList.GetCurrentRunning: Boolean;
begin
  if Assigned(CurrentEncoder) then
    Result:=CurrentEncoder.Status=esEncoding
  else
    Result:=False;
end;

function TMediaSerialEncodeTaskList.GetActive: Boolean;
begin
  Result:=Assigned(FCurrentTask);
end;

function TMediaSerialEncodeTaskList.Pause: Boolean;
begin
  if Assigned(FCurrentTask) then
    Result:=FCurrentTask.Pause
  else
    Result:=False;
end;

function TMediaSerialEncodeTaskList.Resume: Boolean;
begin
  if Assigned(FCurrentTask) then
    Result:=FCurrentTask.Resume
  else
    Result:=False;
end;

function TMediaSerialEncodeTaskList.Stop: Boolean;
begin
  if Assigned(FCurrentTask) then
    Result:=FCurrentTask.Stop
  else
    Result:=False;
end;

end.
