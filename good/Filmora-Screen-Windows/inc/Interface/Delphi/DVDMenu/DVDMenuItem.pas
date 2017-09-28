unit
  DVDMenuItem;

{$MINENUMSIZE 4}

interface

uses
  Windows, Messages;

const

  MAX_FILE_NAME       = 128;
  MAX_FRAMES_IN_PAGE  = 32;

type

  DOUBLE_RECT = record
    dLeft       : Double;
    dTop        : Double;
    dRight      : Double;
    dBottom     : Double;
  end;
  PDOUBLE_RECT = ^DOUBLE_RECT;

  BUTTON_TYPE =
  (
	  BTN_PREV = 0,
	  BTN_PLAY,
	  BTN_NEXT
  );

  BUTTON_ITEM = record
  	btnType     : BUTTON_TYPE;
	  pos         : DOUBLE_RECT;
  end;
  PBUTTON_ITEM = ^BUTTON_ITEM;

  BUTTON_STATE = record
    bHavePrev   : BOOL;
    bHavePlay   : BOOL;
    bHaveNext   : BOOL;
  end;
  PBUTTON_STATE = ^BUTTON_STATE;

  FRAME_ITEM = record
    pos         : DOUBLE_RECT;
  end;
  PFRAME_ITEM = ^FRAME_ITEM;

const
  MAX_TEXT_LEN  = 256;

type
  TEXT_ITEM = record
    szContent   : array[0..MAX_TEXT_LEN - 1] of WideChar;    // 文本内容
    szFontName  : array[0..MAX_TEXT_LEN - 1] of WideChar;    // 字体名称
    nFontSize   : Integer;                                   // 字号
    uFontColor  : COLORREF;                                  // 颜色
    bBold       : BOOL;                                      // 是否粗体
    bItalic     : BOOL;                                      // 是否斜体
    bUnderline  : BOOL;                                      // 是否有下划线
    bStrikeout  : BOOL;                                      // 是否有删除线
  end;
  PTEXT_ITEM = ^TEXT_ITEM;

  TEXT_STYLE =
  (
    TS_BOLD = 0,
    TS_ITALIC,
    TS_UNDERLINE,
    TS_STRIKEOUT
  );

// 菜单管理器回调函数
// typedef BOOL (__stdcall * MENUMGR_CALLBACK)(void* pUserObj, UINT uMsg, WPARAM wParam, LPARAM lParam);
type
  MENUMGR_CALLBACK = function(pUserObj : Pointer; uMsg : UINT; wParam : WPARAM; lParam : LPARAM) : BOOL; stdcall;

const
  WM_MENUDESIGN   = WM_USER + 1501;	// MenuDesigner消息
  WM_MENUDATA	    = WM_USER + 1502;	// DVDMenuData消息
  WM_TITLELIST    = WM_USER + 1503; // DVDTitleList消息
  WM_MENUPREVIEW	= WM_USER + 1504;	// 菜单预览消息

  WM_OUTPUT_CTRL	= WM_USER + 1604;	// OutputCtrl消息

  WM_DVDAUTHOR_PROGRESS = WM_USER + 2101;	// DVDAuthor进度消息

{****************************************************************
	uMsg	: WM_MENUDESIGN;
	wParam	: MENUDESIGN_EVENT
	lParam	: MENUDESIGN_PAGEITEM
****************************************************************}
type
  MENUDESIGN_EVENT =
  (
  	MD_CHANGEITEM = 0,
    MD_DOUBLE_CLICK,
    MD_SET_FOCUS,
    MD_RIGHT_CLICK
  );

  MENUDESIGN_PAGEITEM =
  (
  	PAGE_TEMPLATE = 0,
	  PAGE_BACKGND,
	  PAGE_FRAME,
    PAGE_BUTTON_PREV,
    PAGE_BUTTON_PLAY,
    PAGE_BUTTON_NEXT,
    PAGE_TEXT
  );

{****************************************************************
	uMsg	: WM_TITLELIST;
	wParam	: TITLIST_EEVENT
	lParam	: wParam = TL_AMOUNT_CHANGE 时无意义
			  wParam = TL_THUMBNAIL_CHANGE时，lParam为Title编号
****************************************************************}
  TITLIST_EEVENT =
  (
	  TL_AMOUNT_CHANGE = 0,
    TL_THUMBNAIL_CHANGE,
    TL_TITLENAME_CHANGE
  );

{****************************************************************
	uMsg	: WM_MENUPREVIEW;	
	wParam	: MENUPREVIEW_EVENT
	lParam	: wParam = MPE_CLICK_FRAME:  点击的相框编号（总编号）
			      wParam = MPE_TURN_PAGE:	  跳转后菜单页编号
****************************************************************}
  MENUPREVIEW_EVENT =
  (
    MPE_CLICK_FRAME = 0,
    MPE_TURN_PAGE
  );

{****************************************************************
  uMsg	  : WM_OUTPUT_CTRL;
  wParam	: OUTPUTCTRL_EVENT
  lParam	: wParam = OCE_PROGRESS —— lParam为百分比
			      wParam = 其余 —— lParam为BOOL类型 bSuccessful
****************************************************************}
  OUTPUTCTRL_EVENT =
  (
    OCE_PROGRESS = 0,	// 进度消息
    OCE_SUB_END,	    // 子处理过程结束
    OCE_ALL_END,     	// 全部处理结束
  	OCE_MENUCONV_BEGIN,
	  OCE_CREATEDVD_BEGIN
  );

implementation

end.
