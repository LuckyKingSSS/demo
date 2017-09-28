unit
	TDoc;

{$MINENUMSIZE 4}
    
    
interface

uses
    Windows, TClip;

type
    HTRACK = Pointer;
    HTLDOC = Pointer;
    PHCLIP = ^HCLIP;

// 轨道类型定义
type
TrackType = 
(
	TT_NONE,			// 未知类型
	TT_VIDEO,			// 视频轨道
	TT_AUDIO,			// 音频轨道
	TT_OVERLAP			// 叠加轨道	
);


//==========================================================================
// Timeline Track函数

//TrackType __stdcall TTGetType(HTRACK hTrack);
function TTGetType(hTrack: HTRACK) : TrackType; stdcall;

//double __stdcall TTGetLength(HTRACK hTrack); // 返回轨道有效长度
function TTGetLength(hTrack: HTRACK) : double; stdcall;


// 遍历轨道（除转场以外的）所有clip

//int __stdcall TTGetClipCount(HTRACK hTrack);
function TTGetClipCount(hTrack: HTRACK) : Integer; stdcall;

//HCLIP __stdcall TTGetClip(HTRACK hTrack, int nIndex);
function TTGetClip(hTrack: HTRACK; nIndex : Integer) : HCLIP; stdcall;

//int __stdcall TTGetTransClipCount(HTRACK hTrack);
function TTGetTransClipCount(hTrack: HTRACK) : Integer; stdcall;

//HTRANSCLIP __stdcall TTGetTransClip(HTRACK hTrack, int nIndex);
function TTGetTransClip(hTrack: HTRACK; nIndex : Integer) : HCLIP; stdcall;


//int __stdcall TTInsertClipIndex(HTRACK hTrack, HCLIP hClip, int nBeforeIndex = -1); // 将hClip插入索引为nBeforeIndex的Clip的前面，nBeforeIndex为-1表示插入到轨道尾部 返回新Clip的索引值 clip不能是转场clip
function TTInsertClipIndex(hTrack: HTRACK; hClip : HCLIP; nBeforeIndex : Integer = -1) : Integer; stdcall;

//int __stdcall TTInsertClip(HTRACK hTrack, HCLIP hClip, HCLIP hClipExist); //将hClip插入到hClipExist的前面 hClipExist为0表示插入到轨道尾部
function TTInsertClip(hTrack: HTRACK; hClip : HCLIP; hClipExist : HCLIP) : Integer; stdcall;

//BOOL __stdcall TTEraseClip(HTRACK hTrack, HCLIP hClip); // 删除hClip
function TTEraseClip(hTrack: HTRACK; hClip : HCLIP) : BOOL; stdcall;

//BOOL __stdcall TTEraseClipIndex(HTRACK hTrack, int nIndex); // 删除索引为nIndex的clip
function TTEraseClipIndex(hTrack: HTRACK; nIndex : Integer) : BOOL; stdcall;


//int __stdcall TTMoveClips(HTRACK hTrack, int nIndexExist, int nStartIndex, int nClipCount, BOOL bCopyFlag = FALSE); // 将从nStartIndex开始的nClipCount个clip移动到索引为nIndexExist的clip的前面
// 返回被移动的第一个clip的新索引值 TCGetIndex 函数可以取得clip在其所在轨道中的位置的索引号
function TTMoveClips(hTrack: HTRACK; nIndexExist : Integer; nStartIndex : Integer; nClipCount : Integer; bCopyFlag : BOOL = FALSE) : Integer; stdcall;


//extern "C" HCLIP __stdcall TTSetClipTailTransID(HTRACK hTrack, HCLIP hClip, const wchar_t* pTransID, double dLength = -1); // 设置尾部转场 返回转场Clip句柄 
function TTSetClipTailTransID(hTrack: HTRACK; hClip : HCLIP; const pTransID : PWideChar; dLength : double = -1) : HCLIP; stdcall;

//BOOL __stdcall TTSetClipTailTrans(HTRACK hTrack, HCLIP hClip, HTRANSCLIP hTransClip); // 设置尾部转场
function TTSetClipTailTrans(hTrack: HTRACK; hClip : HCLIP; hTransClip : HCLIP) : BOOL; stdcall;

//extern "C" BOOL __stdcall TTAddFile(HTRACK hTrack, const wchar_t* pFileName, const wchar_t* pTransID = 0, double dLength = 0); // pTransID 转场ID
function TTAddFile(hTrack: HTRACK; const pFileName : PWideChar; const pTransID : PWideChar = nil; dLength : double = 0) : BOOL; stdcall;

//void __stdcall TTDeleteSelectedClips(HTRACK hTrack); // 删除选择集
procedure TTDeleteSelectedClips(hTrack: HTRACK); stdcall;

//void __stdcall TTDeleteAllClips(HTRACK hTrack); // 删除所有
procedure TTDeleteAllClips(hTrack: HTRACK); stdcall;

//void __stdcall TTAdjustClipPos(HTRACK hTrack);
procedure TTAdjustClipPos(hTrack: HTRACK); stdcall;

//BOOL __stdcall TTSetAutoAdjust(HTRACK hTrack, BOOL bFlag = TRUE); // 返回先前的值
function TTSetAutoAdjust(hTrack: HTRACK; bFlag : BOOL = TRUE) : BOOL; stdcall;

//void __stdcall TTSetVolume(HTRACK hTrack, int nVolume);
procedure TTSetVolume(hTrack: HTRACK; nVolume : Integer); stdcall;

//int __stdcall TTGetVolume(HTRACK hTrack);
function TTGetVolume(hTrack: HTRACK) : Integer; stdcall;



//==========================================================================
// 文档接口函数

//HTRACK __stdcall TDGetTrack(HTLDOC hDoc, TrackType uTrackType, int nIndex); // 获取uTrackType类型的索引为nIndex的轨道句柄
function TDGetTrack(hDoc: HTLDOC; uTrackType : TrackType; nIndex : Integer) : HTRACK; stdcall;

//int __stdcall TDGetTracksCount(HTLDOC hDoc); 
function TDGetTracksCount(hDoc: HTLDOC) : Integer; stdcall;

//int __stdcall TDGetTypeTrackCount(HTLDOC hDoc, TrackType uType);
function TDGetTypeTrackCount(hDoc: HTLDOC; uType : TrackType) : Integer; stdcall;

//double __stdcall TDGetLength(HTLDOC hDoc); // 返回文档有效长度
function TDGetLength(hDoc: HTLDOC) : double; stdcall;

//void __stdcall TDDeleteSelectedClips(HTLDOC hDoc);
procedure TDDeleteSelectedClips(hDoc: HTLDOC); stdcall;

//void __stdcall TDDeleteAllClips(HTLDOC hDoc);
procedure TDDeleteAllClips(hDoc: HTLDOC); stdcall;


//HCLIP __stdcall TDGetFocusClip(HTLDOC hDoc); // 取得焦点Clip
function TDGetFocusClip(hDoc: HTLDOC) : HCLIP; stdcall;

//void __stdcall TDSetFocusClip(HTLDOC hDoc, HCLIP hClip); // 设置焦点Clip
procedure TDSetFocusClip(hDoc: HTLDOC; hClip : HCLIP); stdcall;

//BOOL __stdcall TDSelectAddClip(HTLDOC hDoc, HCLIP hClip, BOOL bClearOld = FALSE); // 选中clip, bClearOld - 是否清除先前的选择集
function TDSelectAddClip(hDoc: HTLDOC; hClip : HCLIP; bClearOld : BOOL = FALSE) : BOOL; stdcall;

//BOOL __stdcall TDSelectEraseClip(HTLDOC hDoc, HCLIP hClip); // 从选择集中去除clip
function TDSelectEraseClip(hDoc: HTLDOC; hClip : HCLIP) : BOOL; stdcall;

//BOOL __stdcall TDSelectClear(HTLDOC hDoc); // 清除选择集
function TDSelectClear(hDoc: HTLDOC) : BOOL; stdcall;

//int __stdcall TDSelectAllClips(HTLDOC hDoc); // 选中所有的元素，返回选中的元素个数，不计转场
function TDSelectAllClips(hDoc: HTLDOC) : Integer; stdcall;


// 取得已序选择集
//int __stdcall TDGetSelectedClips(HTLDOC hDoc, HCLIP* pClips, int nCount); // 取得普通Clip选择集 pClips - HCLIP数组指针 nCount - 缓冲区数量
// 返回填入缓冲区的clip数量 如果pClips为0，则直接返回当前选择集clip的数量
function TDGetSelectedClips(hDoc: HTLDOC; pClips : PHCLIP; nCount : Integer) : Integer; stdcall;

//int __stdcall TDGetSelectedTransClips(HTLDOC hDoc, HTRANSCLIP* pClips, int nCount); // 取得转场Clip选择集 参数意义同 TDGetSelectedClips
function TDGetSelectedTransClips(hDoc: HTLDOC; pClips : PHCLIP; nCount : Integer) : Integer; stdcall;

//BOOL __stdcall TDSelectedIsContinuous(HTLDOC hDoc); // 当前选择集是否连续 不计转场
function TDSelectedIsContinuous(hDoc: HTLDOC) : BOOL; stdcall;

//BOOL __stdcall TDSelectedIsContinuousWithTrans(HTLDOC hDoc); // 当前选择集（计转场）是否连续 必须选中的普通clip和转场Clip能构成一个连续的整体
function TDSelectedIsContinuousWithTrans(hDoc: HTLDOC) : BOOL; stdcall;



implementation

const
    DLLNAME = 'WS_Timeline.dll';

function TTGetType; external DLLNAME Name 'TTGetType';
function TTGetLength; external DLLNAME Name 'TTGetLength';
function TTGetClipCount; external DLLNAME Name 'TTGetClipCount';
function TTGetClip; external DLLNAME Name 'TTGetClip';
function TTGetTransClipCount; external DLLNAME Name 'TTGetTransClipCount';
function TTGetTransClip; external DLLNAME Name 'TTGetTransClip';
function TTInsertClipIndex; external DLLNAME Name 'TTInsertClipIndex';
function TTInsertClip; external DLLNAME Name 'TTInsertClip';
function TTEraseClip; external DLLNAME Name 'TTEraseClip';
function TTEraseClipIndex; external DLLNAME Name 'TTEraseClipIndex';
function TTMoveClips; external DLLNAME Name 'TTMoveClips';
function TTSetClipTailTransID; external DLLNAME Name 'TTSetClipTailTransID';
function TTSetClipTailTrans; external DLLNAME Name 'TTSetClipTailTrans';
function TTAddFile; external DLLNAME Name 'TTAddFile';
procedure TTDeleteSelectedClips; external DLLNAME Name 'TTDeleteSelectedClips';
procedure TTDeleteAllClips; external DLLNAME Name 'TTDeleteAllClips';
procedure TTAdjustClipPos; external DLLNAME Name 'TTAdjustClipPos';
function TTSetAutoAdjust; external DLLNAME Name 'TTSetAutoAdjust';
procedure TTSetVolume; external DLLNAME Name 'TTSetVolume';
function TTGetVolume; external DLLNAME Name 'TTGetVolume';

function TDGetTrack; external DLLNAME Name 'TDGetTrack';
function TDGetTracksCount; external DLLNAME Name 'TDGetTracksCount';
function TDGetTypeTrackCount; external DLLNAME Name 'TDGetTypeTrackCount';
function TDGetLength; external DLLNAME Name 'TDGetLength';
procedure TDDeleteSelectedClips; external DLLNAME Name 'TDDeleteSelectedClips';
procedure TDDeleteAllClips; external DLLNAME Name 'TDDeleteAllClips';
function TDGetFocusClip; external DLLNAME Name 'TDGetFocusClip';
procedure TDSetFocusClip; external DLLNAME Name 'TDSetFocusClip';
function TDSelectAddClip; external DLLNAME Name 'TDSelectAddClip';
function TDSelectEraseClip; external DLLNAME Name 'TDSelectEraseClip';
function TDSelectClear; external DLLNAME Name 'TDSelectClear';
function TDGetSelectedClips; external DLLNAME Name 'TDGetSelectedClips';
function TDGetSelectedTransClips; external DLLNAME Name 'TDGetSelectedTransClips';
function TDSelectedIsContinuous; external DLLNAME Name 'TDSelectedIsContinuous';
function TDSelectedIsContinuousWithTrans; external DLLNAME Name 'TDSelectedIsContinuousWithTrans';
function TDSelectAllClips; external DLLNAME Name 'TDSelectAllClips';



end.



