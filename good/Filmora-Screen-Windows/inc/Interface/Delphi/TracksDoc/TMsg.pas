////////////////////////////////////////////////////////////////////////////
// TMsg.pas
//==========================================================================
// 功能    ： 定义Timeline向外部发送的消息 和Timeline接收的命令消息
// 创建时间： 2005-08-12
// 创建者  ： 杨长元
// 修订    ： 
//==========================================================================

unit
	TMsg;


interface

uses
    Windows;

// Timeline消息（命令）定义
const

//==========================================================================
// 主要Callback消息ID

// 文档Message
TCM_UPDATE							= 1;		// 时间线数据已经更改
TCM_CLIP_PARAM						= 2;		// Clip参数发生改变
TCM_ADD_CLIPS						= 3;		// 加入了新的Clip
TCM_DELETE_CLIPS					= 4;		// 有Clip被删除 wParam - 删除的元素个数 lParam为指向HCLIP的数组指针 可能包含转场Clip
TCM_CLIP_SELCET_CHANGE				= 5;		// Clip选择状态发生改变， wParam - 选中状态的Clip数量， lParam - 选择集是否连续
TCM_DELETE_ALL_CLIPS				= 6;		// 所有Clip被删除

TCM_CLIP_STATE						= 100;		// Clip状态发生改变 lParam - clip指针 HCLIP
TCM_SELECT_ALL						= 101;		// 选中了所有clip
TCM_SELECT_CLEAR_ALL				= 102;		// 清除所有clip的选中状态
TCM_MOVE_CLIPS						= 103;		// 有Clip被移动

TCM_DOC_MSG_MAX						= 299;		// 文档更改消息通知最大值 


//---------------------------------------------------------------------------
// 视图Message
TCM_TIMELINE_MOVING					= 302;		// 时间线移动消息（正在移动），lParam为指向double时间数据的指针
TCM_TIMELINE_MOVED					= 303;		// 时间线移动消息（移动完毕），lParam为指向double时间数据的指针
TCM_ZOOM_IN							= 304;		// 视图放大，wParam为0-9之间的整数，表示显示比例等级，一共10级；lParam为指向视图double起始时间数据的指针
TCM_ZOOM_OUT						= 305;		// 视图缩小，参数同TMI_ZOOM_IN
TCM_PAN								= 306;		// 视图平移，lParam为指向视图double起始时间数据的指针
										 
//---------------------------------------------------------------------------
// 其他Message							 
TCM_CLIPS_LOAD_PROGRESS				= 500;		// 正在加载照片文件 lParam - 进度百分比 (0, 100)
TCM_CLIP_CLICK						= 510;		// Clip被单击 lParam为被单击的HCLIP句柄
TCM_CLIP_DOUBLECLK					= 511;		// Clip被双击 lParam为被双击的HCLIP句柄
TCM_CONTEXT_MENU					= 512;		// 上下文菜单，RButtonClick  wParam - MAKEWPARAM(x, y) lParam - 为被点击的HCLIP句柄
TCM_OVER_CLIP_COUNT					= 520;		// Timeline中的非转场Clip数量已经达到最大值
//TCM_PRE_DELETE_CLIPS				= 521;		// Clip将被删除 lParam为指向用户数据的数组指针，内容为0则结束





//==========================================================================
// Timeline接收的主要命令消息ID

//TCC_DELETE_SELECTED_CLIPS			= 5001;	// 删除所有选中的Clips
//TCC_DELETE_ALL_CLIPS				= 5002;	// 删除所有Clips
TCC_SWITCH_VIEWMODE					= 5003;	// 
TCC_SET_VIEWMODE					= 5004;	// wParam: 0 - Timeline Mode  1 - Storyboard View Mode
TCC_GET_VIEWMODE					= 5005;	// return: 0 - Timeline Mode  1 - Storyboard View Mode
TCC_STORYBOARD_ONLY					= 5006;	// 仅使用故事板模式
//TCC_APPLY_EFFECT_TO_SELECTED		= 5007;	// 应用特效到选中的Clip  wParam - 特效长度（毫秒）  lParam - 特效ID（字符串指针）
TCC_ENSURE_VISIBLE_CLIP				= 5008;	// 让指定Clip处于可见状态 wParam - 照片元素索引号
TCC_ON_HOME							= 5009;	// 视图滚动到头部
TCC_ON_END							= 5010;	// 视图滚动到尾部
//TCC_CAN_APPLY_EFFECT_TO_SELECTED	= 5011;	// 是否可以应用特效到选择集 返回 0 - 不能， 1 - 能
//TCC_SELECTED_CLIP_COUNT				= 5012;	// 返回选中的Clip个数 包括媒体和转场Clip
//TCC_SELECTED_CAN_PREVIEW			= 5013;	// 返回 1 - 照片选择集连续可以预览  0 - 不可以预览				
//TCC_ADD_CLIPS						= 5014;	// 添加Clip wParam = MAKEWPARAM(nCount, nDefultLength)  lParam - const wchar_t*szFileNameArray[]
//TCC_APPLY_EFFECT_TO_ALL				= 5015;	// 应用特效到所有Clip  wParam - 特效长度（毫秒）  lParam - 特效ID（字符串指针）
//TCC_RANDOM_EFFECT_TO_ALL			= 5016;	// 应用随机特效到所有Clip  wParam - 特效长度（毫秒）
TCC_SET_TIME						= 5020;	// 设置时间线位置 lParam - double类型时间数据地址指针
										  
TCC_SHOW_TRANSCLIP					= 8000;	// wParam: 1 - 显示转场  0 - 隐藏转场
TCC_ENSURE_CLIP						= 8001;	// 滚动视图，使索引为wParam的Clip可见
//TCC_STORYBOARD_THUMBNAIL_SIZE		= 8002;	// 故事板缩略图尺寸控制 lParam
//TCC_STORYBOARD_TIMEMODE				= 8003;	// 故事板时间显示方式，0 - 秒 1 - 时：分：秒
										  
TCC_STORYBOARD_GETPARAM				= 8004;	// 设置取得故事板参数，lParam - 指向StoryboardOptions结构的指针
TCC_STORYBOARD_SETPARAM				= 8005;	// 设置故事板参数，lParam - 指向StoryboardOptions结构的指针
										  
// 故事板参数结构
type
	StoryboardOptions = record 

	nImageAreaWidth : Integer;	// 图像区宽度
	nTransAreaWidth : Integer;	// 转场区宽度

	nTransWidth : Integer;		// 转场缩略图宽度
	nTransHeight : Integer;		// 转场缩略图高度

	nImageBorderLeft : Integer;	// 图像左边框宽度
	nImageBorderTop : Integer;	// 图像上边框高度
	nImageBorderRight : Integer;// 图像右边框宽度
	nImageBorderBottom : Integer;// 图像下边框高度

	nBottomAreaHeight : Integer;// 底部预留高度

	nImageEditWidth : Integer;	// 图像编辑框长度
	nEditHeight : Integer;		// 编辑框高度

	bTransVisible : BOOL;		// 转场是否可见
	uImageLengthFlag : UINT;	// Image片段长度区 0 - 不可见  1 - 可见 不能编辑  3 - 可见 可以编辑 
	uTransLengthFlag : UINT;	// Trans片段长度区 0 - 不可见  1 - 可见 不能编辑  3 - 可见 可以编辑 	

	uTimeMode : UINT;			// 0 - 秒（只允许输入整数）   
								// 1 - 秒（允许输入小数）	（目前不支持）    
								// 2 - 时:分:秒:帧			（目前不支持）    
								// 3 - 时:分:秒.秒/100		（目前不支持）   
								
	nMaxClips : Integer;		// 最大clip数量 不计转场 0 为不限制
	uEditCtrlFlag : UINT;		// 0 - 可以接受外部拖入数据、clip可以交换顺序、可以删除
								// 1 - 不可以接受外部拖入数据、clip不可以交换顺序、不可以删除

end;


// ...

implementation

end.


