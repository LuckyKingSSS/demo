{*******************************************************}
{       Wondershare视频播放窗口VCL组件                  }
{       创建人:　 凌曙光　　 　　　　　　　   　　　　  }
{       创建日期: 2008/10/20                            }
{       修订人:   凌曙光                                }
{       修订日期:                                       }
{*******************************************************}

unit MediaPlayView;

interface

uses Windows, Messages, SysUtils, Classes, Contnrs, Controls, Forms, Graphics,
  MediaCommon, MediaDecode, MediaProcess, MediaPlay;

type
  TMPVCropControl = class;
  TMediaPlayerView = class;

  TMPVFrameLineKind = (clmLeft, clmTop, clmRight, clmBottom);

  TMPVFrameLine = class(TCustomControl)
  private
    FLineKind: TMPVFrameLineKind;
    procedure WMSysCommand(var Message: TWMSysCommand); message WM_SYSCOMMAND;
    procedure WMEraseBkgnd(var Message: TWMEraseBkgnd); message WM_ERASEBKGND;
  protected
    procedure Paint; override;
    procedure CreateParams(var Params: TCreateParams); override;
  public
    constructor Create(AOwner: TComponent; ACropControl: TMPVCropControl;
      ALineKind: TMPVFrameLineKind); reintroduce;

    property LineKind: TMPVFrameLineKind read FLineKind;
  end;

  TMPVSizingKind = (smTopLeft, smTop, smTopRight, smLeft, smRight,
    smBottomLeft, smBottom, smBottomRight);

  TMPVResizeHandler = class(TCustomControl)
  private
    FSizingKind: TMPVSizingKind;
    FCropControl: TMPVCropControl;
    FMouseIsDown: Boolean;
    FMouseInDown: Boolean;
    procedure SetMouseIsDown(const Value: Boolean);
    procedure WMSysCommand(var Message: TWMSysCommand); message WM_SYSCOMMAND;
    procedure WMSetCursor(var Message: TWMSetCursor); message WM_SETCURSOR;
    procedure WMEraseBkgnd(var Message: TWMEraseBkgnd); message WM_ERASEBKGND;
  protected
    procedure Paint; override;
    procedure CreateParams(var Params: TCreateParams); override;
    procedure MouseDown(Button: TMouseButton; Shift: TShiftState; X, Y: Integer); override;
    procedure MouseMove(Shift: TShiftState; X, Y: Integer); override;
    procedure MouseUp(Button: TMouseButton; Shift: TShiftState; X, Y: Integer); override;
  public
    constructor Create(AOwner: TComponent; ACropControl: TMPVCropControl;
      ASizingKind: TMPVSizingKind); reintroduce;

    function ClientToParent(const pt: TPoint): TPoint;

    property SizingKind: TMPVSizingKind read FSizingKind;
    property CropControl: TMPVCropControl read FCropControl;
    property MouseIsDown: Boolean read FMouseInDown write SetMouseIsDown;
  end;

  TResizeHandlerArray = array[TMPVSizingKind] of TMPVResizeHandler;
  TMPVFrameLineArray = array[TMPVFrameLineKind] of TMPVFrameLine;

  TCropResizeType = (crtMove, crtResize);
  TCropResizeTypes = set of TCropResizeType;

  TCropRestrictType = (carNone, carVideoArea, carClientArea);

  TMPVCropControl = class
  private
    FResizeHandlers: TResizeHandlerArray;
    FFrameLines: TMPVFrameLineArray;
    FOwner: TMediaPlayerView;
    FVisible: Boolean;
    FBoundsRect: TRect;
    FMouseInClient: Boolean;
    FMouseInDown: Boolean;
    FLastMousePt: TPoint;
    FOriginalBounds: TRect;
    FDraging: Boolean;
    FCroping: Boolean;
    FSizeOrigin: TPoint;
    FCropRect: TRect;
    FMinHeight: Integer;
    FMinWidth: Integer;
    FSizeable: Boolean;
    FAreaRestrict: TCropRestrictType;
    FKeepRatio: Boolean;
    procedure BoundsChanged;
    function GetFrameLineRect(ALineKind: TMPVFrameLineKind): TRect;
    function GetResizingHandlerRect(SizingKind: TMPVSizingKind): TRect;
    procedure CheckUpdateHandleBounds; overload;
    procedure CheckUpdateHandleBounds(ARect: TRect; AResizeType: TCropResizeTypes=[crtResize, crtMove]); overload;
    procedure UpdateHandlersBounds;
    function PointInCropControl(X, Y: Integer): Boolean;
    procedure SetVisible(Value: Boolean);
    procedure SetBoundsRect(const Value: TRect);
    function GetClientRect: TRect;
    function GetHeight: Integer;
    function GetWidth: Integer;
    procedure SetHeight(Value: Integer);
    procedure SetWidth(Value: Integer);
    procedure SetMouseInClient(Value: Boolean);
    procedure SetMouseIsDown(Value: Boolean);
    function GetLeft: Integer;
    function GetTop: Integer;
    procedure SetLeft(Value: Integer);
    procedure SetTop(Value: Integer);
    procedure SetCropRect(const Value: TRect);
    procedure SetSizeable(Value: Boolean);
    procedure SetVideoAreaRestrict(Value: Boolean);
    procedure SetAreaRestrict(Value: TCropRestrictType);
    function GetVideoAreaRestrict: Boolean;
  protected
    procedure MouseDown(Button: TMouseButton; Shift: TShiftState; X, Y: Integer);
    procedure MouseMove(Shift: TShiftState; X, Y: Integer);
    procedure MouseUp(Button: TMouseButton; Shift: TShiftState; X, Y: Integer);
    procedure UpdateBoundsFromCropRect;

    property MouseInClient: Boolean read FMouseInClient write SetMouseInClient;
    property MouseIsDown: Boolean read FMouseInDown write SetMouseIsDown;
  public
    constructor Create(AOwner: TMediaPlayerView);
    destructor Destroy; override;
    procedure ClientToParent(var ARect: TRect);
    procedure ParentToClient(var ARect: TRect);
    function VideoToWindow(const Value: TRect): TRect;
    function WindowToVideo(const Value: TRect): TRect;
    procedure SetBounds(ALeft, ATop, AWidth, AHeight: Integer);
    procedure ResizeControl(SizingKind: TMPVSizingKind; X, Y: Integer);
    procedure MoveWith(dx, dy: Integer);
    procedure ResizeWith(dx, dy: Integer);
    procedure VideoCropChange;

    property Owner: TMediaPlayerView read FOwner;
    property Visible: Boolean read FVisible write SetVisible;
    property BoundsRect: TRect read FBoundsRect write SetBoundsRect;
    property CropRect: TRect read FCropRect write SetCropRect;
    property ClientRect: TRect read GetClientRect;
    property Left: Integer read GetLeft write SetLeft;
    property Top: Integer read GetTop write SetTop;
    property Width: Integer read GetWidth write SetWidth;
    property Height: Integer read GetHeight write SetHeight;
    property MinWidth: Integer read FMinWidth write FMinWidth;
    property MinHeight: Integer read FMinHeight write FMinHeight;
    property Sizeable: Boolean read FSizeable write SetSizeable;
    property KeepRatio: Boolean read FKeepRatio write FKeepRatio;
    property VideoAreaRestrict: Boolean read GetVideoAreaRestrict write SetVideoAreaRestrict;
    property AreaRestrict: TCropRestrictType read FAreaRestrict write SetAreaRestrict;
  end;

  TMediaPlayerView = class(TWinControl)
  private
    FPlayer, FOldPlayer: TMediaPlayer;
    FId, FOldId: IDVPVIEW;
    FViewMode: Integer;
    FOutVideoSize: TSize;
    FFullScreen: Boolean;
    FSaveWndStyle: Cardinal;
    FCropControl: TMPVCropControl;
    FOnCropAreaChange: TNotifyEvent;
    FFullScreenWindow: HWnd;
    FFullErasebkgnd: Boolean;
    procedure DoCropAreaChange;
    procedure InitCropControl;
    procedure InitFullScreenWindow;
    procedure CleanupFullScreenWindow;
    procedure FullScreenWndProc(var Message: TMessage);
    procedure SetCropRect(const pCropRect: TRect);
    function GetCropRect: TRect;
    function GetVideoOutputRect: TRect;
    procedure SetFullScreen(Value: Boolean);
    procedure SetPlayer(Value: TMediaPlayer);
    function GetShowCropControl: Boolean;
    procedure SetShowCropControl(Value: Boolean);
    function GetCropControl: TMPVCropControl;
    function GetVideoInfo: TWSVideoInfo;
    procedure SetViewMode(Value: Integer);
    procedure SetOutVideoSize(const Value: TSize);
    function GetKeepRatio: Boolean;
    procedure SetKeepRatio(Value: Boolean);
    procedure CNKeyDown(var Message: TWMKeyDown); message CN_KEYDOWN;
    procedure CNKeyUp(var Message: TWMKeyUp); message CN_KEYUP;
    procedure CNChar(var Message: TWMChar); message CN_CHAR;
    procedure CMRecreateWnd(var Message: TMessage); message CM_RECREATEWND;
    procedure WMSetCursor(var Message: TWMSetCursor); message WM_SETCURSOR;
    procedure WMEraseBkgnd(var Message: TWMEraseBkgnd); message WM_ERASEBKGND;
    procedure WMDestroy(var Message: TWMDestroy); message WM_DESTROY;
    procedure WMNCDestroy(var Message: TWMNCDestroy); message WM_NCDESTROY;
  protected
    procedure CreateParams(var Params: TCreateParams); override;
    procedure CreateWindowHandle(const Params: TCreateParams); override;
    procedure DestroyWindowHandle; override;
    procedure MouseDown(Button: TMouseButton; Shift: TShiftState; X, Y: Integer); override;
    procedure MouseMove(Shift: TShiftState; X, Y: Integer); override;
    procedure MouseUp(Button: TMouseButton; Shift: TShiftState; X, Y: Integer); override;
    procedure KeyDown(var Key: Word; Shift: TShiftState); override;
    procedure Resize; override;
  public
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;

    function GetVideoProcessHandle: HVIDEODECODER;
    function GetVideoProcessor: TVideoMediaProcessor;
    function GetDisplayResizeMode: IMAGE_RESIZE_METHOD;
    function SetDisplayResizeMode(irm: IMAGE_RESIZE_METHOD): BOOL;
    procedure SetCropEditStyle(uCropStyle: UINT = 1);
    procedure VideoCropChange;
    procedure RecreateWnd;

    property Id: IDVPVIEW read FId;
    property CropRect: TRect read GetCropRect write SetCropRect;
    property VideoOutputRect: TRect read GetVideoOutputRect;
    property FullScreen: Boolean read FFullScreen write SetFullScreen;
    property Player: TMediaPlayer read FPlayer write SetPlayer;   // 必须设置播放器才能工作
    property CropControl: TMPVCropControl read GetCropControl;
    property ShowCropControl: Boolean read GetShowCropControl write SetShowCropControl default False;
    property KeepRatio: Boolean read GetKeepRatio write SetKeepRatio;
    property ViewMode: Integer read FViewMode write SetViewMode default 0; // 只能在SetPlayer
    property OutVideoSize: TSize read FOutVideoSize write SetOutVideoSize;   // 前设置
    property VideoInfo: TWSVideoInfo read GetVideoInfo;
  published
    property OnCropAreaChange: TNotifyEvent read FOnCropAreaChange write FOnCropAreaChange;

    property AutoSize;
    property Caption;
    property Color;
    property Font;
    property ParentColor;
    property ParentFont;
    property ParentShowHint;
    property PopupMenu;
    property Text;
    property Visible;
    property Enabled;
    property Align;
    property Anchors;
    property Constraints;
    property AlignWithMargins;
    property Margins;
    property Padding;
    property Left;
    property Top;
    property Width;
    property Height;
    property Cursor;
    property Hint;
    property DoubleBuffered default False;
    property UseDockManager default True;
    property DockSite;
    property DragCursor;
    property DragKind;
    property DragMode;
    property TabOrder;
    property TabStop;
    property ShowHint;

    property OnCanResize;
    property OnClick;
    property OnConstrainedResize;
    property OnContextPopup;
    property OnDockDrop;
    property OnDockOver;
    property OnDblClick;
    property OnDragDrop;
    property OnDragOver;
    property OnEndDock;
    property OnEndDrag;
    property OnMouseActivate;
    property OnMouseDown;
    property OnMouseEnter;
    property OnMouseLeave;
    property OnMouseMove;
    property OnMouseUp;
    property OnMouseWheel;
    property OnMouseWheelDown;
    property OnMouseWheelUp;
    property OnResize;
    property OnAlignInsertBefore;
    property OnAlignPosition;
    property OnEnter;
    property OnExit;
    property OnGetSiteInfo;
    property OnKeyDown;
    property OnKeyPress;
    property OnKeyUp;
    property OnStartDock;
    property OnStartDrag;
    property OnUnDock;
  end;

implementation

uses RTLConsts;

const
  SizeRectHeight = 7;
  SizeRectWidth = 7;

{ TMPVFrameLine }

constructor TMPVFrameLine.Create(AOwner: TComponent;
  ACropControl: TMPVCropControl; ALineKind: TMPVFrameLineKind);
begin
  inherited Create(AOwner);
  ControlStyle:=ControlStyle+[csNoDesignVisible, csNoStdEvents]-[csOpaque, csClickEvents, csDoubleClicks];
  FLineKind:=ALineKind;
  Color:=clWhite;
end;

procedure TMPVFrameLine.CreateParams(var Params: TCreateParams);
begin
  inherited;
//  with Params do
//  begin
//    with WindowClass do
//      style:=style or CS_PARENTDC;
//  end;
end;

procedure TMPVFrameLine.Paint;
begin
  Canvas.Pen.Style:=psDot;
  Canvas.Pen.Color:=clBlack;
  Canvas.Pen.Width:=1;
  Canvas.Pen.Mode:=pmCopy;
  case FLineKind of
    clmLeft, clmRight:
      begin
        Canvas.MoveTo(0, 0);
        Canvas.LineTo(0, Height-1);
      end;
    clmTop, clmBottom:
      begin
        Canvas.MoveTo(0, 0);
        Canvas.LineTo(Width, 0);
      end;
  end;
end;

procedure TMPVFrameLine.WMEraseBkgnd(var Message: TWMEraseBkgnd);
begin
  Message.Result:=1;
end;

procedure TMPVFrameLine.WMSysCommand(var Message: TWMSysCommand);
begin
  if not (Message.CmdType=SC_CLOSE) then
    inherited;
end;

{ TMPVResizeHandler }

function TMPVResizeHandler.ClientToParent(const pt: TPoint): TPoint;
begin
  Result:=pt;
  Inc(Result.X, Left);
  Inc(Result.Y, Top);
end;

constructor TMPVResizeHandler.Create(AOwner: TComponent;
  ACropControl: TMPVCropControl; ASizingKind: TMPVSizingKind);
begin
  inherited Create(AOwner);
  ControlStyle:=ControlStyle+[csNoDesignVisible]-[csDoubleClicks];
  FCropControl:=ACropControl;
  FSizingKind:=ASizingKind;
  Color:=clBlack;
end;

procedure TMPVResizeHandler.CreateParams(var Params: TCreateParams);
begin
  inherited;
//  with Params do
//  begin
//    with WindowClass do
//      style:=style or CS_PARENTDC;
//  end;
end;

procedure TMPVResizeHandler.MouseDown(Button: TMouseButton; Shift: TShiftState;
  X, Y: Integer);
begin
  inherited;
  FMouseIsDown:=True;
  GetCursorPos(FCropControl.FSizeOrigin);
  FCropControl.FOriginalBounds:=FCropControl.BoundsRect;
  FCropControl.FCroping:=True;
end;

procedure TMPVResizeHandler.MouseMove(Shift: TShiftState; X, Y: Integer);
var
  pt: TPoint;
begin
  inherited;
  if FMouseIsDown then
  begin
    GetCursorPos(pt);
    FCropControl.ResizeControl(SizingKind, pt.X, pt.Y);
  end;
end;

procedure TMPVResizeHandler.MouseUp(Button: TMouseButton; Shift: TShiftState; X,
  Y: Integer);
begin
  inherited;
  FMouseIsDown:=False;
  FCropControl.FCroping:=False;
  if Parent.CanFocus then Parent.SetFocus;
end;

procedure TMPVResizeHandler.Paint;
begin
  inherited;
  Canvas.Pen.Color:=clWhite;
  Canvas.Brush.Color:=Color;
  Canvas.Rectangle(ClientRect);
end;

procedure TMPVResizeHandler.SetMouseIsDown(const Value: Boolean);
begin
  FMouseInDown := Value;
end;

procedure TMPVResizeHandler.WMEraseBkgnd(var Message: TWMEraseBkgnd);
begin
  Message.Result:=1;
end;

procedure TMPVResizeHandler.WMSetCursor(var Message: TWMSetCursor);
const
  cCursors: array [TMPVSizingKind] of TCursor =
    (crSizeNWSE, crSizeNS, crSizeNESW, crSizeWE, crSizeWE,
     crSizeNESW, crSizeNS, crSizeNWSE);
var
  NewCursor: TCursor;
  P: TPoint;
begin
  with Message do
    if CursorWnd = Handle then
      case Smallint(HitTest) of
        HTCLIENT:
          begin
            GetCursorPos(P);
            NewCursor:=cCursors[SizingKind];
            if NewCursor <> crDefault then
            begin
              Windows.SetCursor(Screen.Cursors[NewCursor]);
              Result := 1;
              Exit;
            end;
          end;
      end;
  inherited;
end;

procedure TMPVResizeHandler.WMSysCommand(var Message: TWMSysCommand);
begin
  if not (Message.CmdType=SC_CLOSE) then
    inherited;
end;

{ TMPVCropControl }

procedure TMPVCropControl.BoundsChanged;
begin
  FCropRect:=WindowToVideo(FBoundsRect);
  FOwner.DoCropAreaChange;
end;

procedure TMPVCropControl.CheckUpdateHandleBounds(ARect: TRect; AResizeType: TCropResizeTypes);
var
  dx, dy: Integer;
  nWidth, nHeight: Integer;
  AVidRect: TRect;
begin
  dx:=0; dy:=0;
  case AreaRestrict of
    carVideoArea: AVidRect:=FOwner.VideoOutputRect
  else
    AVidRect:=FOwner.ClientRect;
  end;
  with AVidRect do
  begin
    nWidth:=Right-Left;
    nHeight:=Bottom-Top;
  end;
  with ARect do
  begin
    if Right-Left<16 then Right:=Left+16;
    if Bottom-Top<16 then Bottom:=Top+16;
    if FAreaRestrict=carNone then
    begin
      if crtMove in AResizeType then
      begin
        if Right<=AVidRect.Left then dx:=AVidRect.Left-Right;
        if Bottom<=AVidRect.Top then dy:=AVidRect.Top-Bottom;
        if Left>=AVidRect.Right then dx:=AVidRect.Right-Left;
        if Top>=AVidRect.Bottom then dy:=AVidRect.Bottom-Top;
        OffsetRect(ARect, dx, dy);
      end;
      if crtResize in AResizeType then
      begin
        if Right<AVidRect.Left then Right:=AVidRect.Left;
        if Bottom<AVidRect.Top then Bottom:=AVidRect.Top;
        if Left>=AVidRect.Right then Left:=AVidRect.Right;
        if Top>=AVidRect.Bottom then Top:=AVidRect.Bottom;
      end;
    end else
    begin
      if crtResize in AResizeType then
      begin
        if Left<AVidRect.Left then Left:=AVidRect.Left;
        if Top<AVidRect.Top then Top:=AVidRect.Top;
        if Right+1>AVidRect.Right then Right:=AVidRect.Right-1;
        if Bottom+1>AVidRect.Bottom then Bottom:=AVidRect.Bottom-1;
      end;
      if crtMove in AResizeType then
      begin
        if Right-Left>nWidth then Right:=Left+nWidth;
        if Bottom-Top>nHeight then Bottom:=Top+nHeight;
        if Left<AVidRect.Left then dx:=AVidRect.Left-Left;
        if Top<AVidRect.Top then dy:=AVidRect.Top-Top;
        if Right+1>AVidRect.Right then dx:=AVidRect.Right-Right-1;
        if Bottom+1>AVidRect.Bottom then dy:=AVidRect.Bottom-Bottom-1;
        OffsetRect(ARect, dx, dy);
      end;
    end;
  end;
  FBoundsRect:=ARect;
  UpdateHandlersBounds;
end;

procedure TMPVCropControl.CheckUpdateHandleBounds;
begin
  CheckUpdateHandleBounds(FBoundsRect, [crtResize, crtMove]);
end;

procedure TMPVCropControl.ClientToParent(var ARect: TRect);
begin
  with FBoundsRect do
    OffsetRect(ARect, Left, Top);
end;

constructor TMPVCropControl.Create(AOwner: TMediaPlayerView);
var
  nKind: TMPVSizingKind;
  nLine: TMPVFrameLineKind;
  pLine: TMPVFrameLine;
  pHandler: TMPVResizeHandler;
begin
  inherited Create();
  FOwner:=AOwner;
  FMinWidth:=16;
  FMinHeight:=16;
  FSizeable:=True;
  FAreaRestrict:=carVideoArea;
  FBoundsRect:=Bounds(0, 0, FMinWidth, FMinWidth);
  FCropRect:=Bounds(0, 0, FMinHeight, FMinHeight);
  for nLine:=Low(TMPVFrameLineKind) to High(TMPVFrameLineKind) do // 先创建线条
  begin
    pLine:=TMPVFrameLine.Create(FOwner, Self, nLine);
    with pLine do
    begin
      pLine.Visible:=Self.Visible;
      pLine.Parent:=FOwner;
    end;
    FFrameLines[nLine]:=pLine;
  end;
  for nKind:=Low(TMPVSizingKind) to High(TMPVSizingKind) do
  begin
    pHandler:=TMPVResizeHandler.Create(FOwner, Self, nKind);
    with pHandler do
    begin
      Visible:=Self.Visible;
      Parent:=AOwner;
    end;
    FResizeHandlers[nKind]:=pHandler;
  end;
  UpdateHandlersBounds;
end;

destructor TMPVCropControl.Destroy;
var
  nKind: TMPVSizingKind;
  nLine: TMPVFrameLineKind;
begin
  for nLine:=Low(TMPVFrameLineKind) to High(TMPVFrameLineKind) do
    FFrameLines[nLine].Free;
  for nKind:=Low(TMPVSizingKind) to High(TMPVSizingKind) do
    FResizeHandlers[nKind].Free;
  inherited;
end;

function TMPVCropControl.GetClientRect: TRect;
begin
  Result:=FBoundsRect;
  with Result do
    OffsetRect(Result, -Left, -Top);
end;

function TMPVCropControl.GetFrameLineRect(ALineKind: TMPVFrameLineKind): TRect;
begin
  case ALineKind of
    clmLeft:
      begin
        Result:=FBoundsRect;
        Result.Right:=Result.Left+1;
      end;
    clmTop:
      begin
        Result:=FBoundsRect;
        Result.Bottom:=Result.Top+1;
      end;
    clmRight:
      begin
        Result:=FBoundsRect;
        Result.Left:=Result.Right;
        Inc(Result.Right);
      end;
    clmBottom:
      begin
        Result:=FBoundsRect;
        Result.Top:=Result.Bottom;
        Inc(Result.Bottom);
      end;
  else
    Result:=Bounds(0, 0, 0, 0);
  end;
end;

function TMPVCropControl.GetResizingHandlerRect(SizingKind: TMPVSizingKind): TRect;
var
  nSizeWidth, nSizeHeight: Integer;
begin
  nSizeWidth:=SizeRectWidth;
  nSizeHeight:=SizeRectHeight;
  case SizingKind of
    smTopLeft:
      Result := Bounds(0, 0, nSizeWidth, nSizeHeight);
    smTop:
      Result := Bounds(Width div 2, 0, nSizeWidth, nSizeHeight);
    smTopRight:
      Result := Bounds(Width, 0,
        nSizeWidth, nSizeHeight);
    smLeft:
      Result := Bounds(0, (Height div 2), nSizeWidth, nSizeHeight);
    smRight:
      Result := Bounds(Width,
        (Height div 2), nSizeWidth, nSizeHeight);
    smBottomLeft:
      Result := Bounds(0, Height,
        nSizeWidth, nSizeHeight);
    smBottom:
      Result := Bounds((Width div 2), Height,
        nSizeWidth, nSizeHeight);
    smBottomRight:
      Result := Bounds(Width, Height, nSizeWidth, nSizeHeight);
  end;
  OffsetRect(Result, -nSizeWidth div 2, -nSizeHeight div 2);
end;

function TMPVCropControl.GetHeight: Integer;
begin
  with FBoundsRect do
    Result:=Bottom-Top;
end;

function TMPVCropControl.GetLeft: Integer;
begin
  Result:=FBoundsRect.Left;
end;

function TMPVCropControl.GetTop: Integer;
begin
  Result:=FBoundsRect.Top;
end;

function TMPVCropControl.GetVideoAreaRestrict: Boolean;
begin
  Result:=FAreaRestrict=carVideoArea;
end;

function TMPVCropControl.GetWidth: Integer;
begin
  with FBoundsRect do
    Result:=Right-Left;
end;

procedure TMPVCropControl.MouseDown(Button: TMouseButton; Shift: TShiftState; X,
  Y: Integer);
begin
  if not (ssDouble in Shift) then
  begin
    MouseIsDown:=Visible and PointInCropControl(X, Y);
    if MouseIsDown then
    begin
      FLastMousePt:=Point(X, Y);
      FOriginalBounds:=BoundsRect;
    end;
  end;
end;

procedure TMPVCropControl.MouseMove(Shift: TShiftState; X, Y: Integer);
var
  ARect: TRect;
begin
  MouseInClient:=PointInCropControl(X, Y);
  if MouseIsDown then
  begin
    FDraging:=True;
    ARect:=FOriginalBounds;
    OffsetRect(ARect, X-FLastMousePt.X, Y-FLastMousePt.Y);
    CheckUpdateHandleBounds(ARect, [crtMove]);
    BoundsChanged;
  end;
end;

procedure TMPVCropControl.MouseUp(Button: TMouseButton; Shift: TShiftState; X,
  Y: Integer);
begin
  MouseIsDown:=False;
  if FDraging then
  begin
    FDraging:=False;
    CheckUpdateHandleBounds;
  end;
end;

procedure TMPVCropControl.MoveWith(dx, dy: Integer);
var
  ARect: TRect;
begin
  FDraging:=True;
  ARect:=FBoundsRect;
  OffsetRect(ARect, dx, dy);
  BoundsRect:=ARect;
  FDraging:=False;
end;

procedure TMPVCropControl.ParentToClient(var ARect: TRect);
begin
  with FBoundsRect do
    OffsetRect(ARect, -Left, -Top);
end;

function TMPVCropControl.PointInCropControl(X, Y: Integer): Boolean;
begin
  Result:=PtInRect(BoundsRect, Point(X, Y));
end;

procedure TMPVCropControl.ResizeWith(dx, dy: Integer);
var
  ARect: TRect;
begin
  FCroping:=True;
  ARect:=FBoundsRect;
  Inc(ARect.Right, dx);
  Inc(ARect.Bottom, dy);
  BoundsRect:=ARect;
  FCroping:=False;
end;

procedure TMPVCropControl.ResizeControl(SizingKind: TMPVSizingKind; X, Y: Integer);

  procedure CheckResizeXYRatio(var dx, dy: Integer; xdir, ydir: Integer);
  var
    nOldWidth, nOldHeight: Integer;
  begin
    with FOriginalBounds do
    begin
      nOldWidth:=Right-Left;
      nOldHeight:=Bottom-Top;
    end;
    if KeepRatio and (nOldWidth>0) and (nOldHeight>0) then
    begin
      if dx=0 then
        dx:=Round((dy*ydir)*(nOldWidth/nOldHeight)*xdir)
      else if dy=0 then
        dy:=Round((dx*xdir)/(nOldWidth/nOldHeight)*ydir)
      else if (dx*xdir/nOldWidth) > (dy*ydir/nOldHeight) then
      begin
        dy:=Round((dx*xdir)/(nOldWidth/nOldHeight)*ydir);
      end else
      begin
        dx:=Round((dy*ydir)*(nOldWidth/nOldHeight)*xdir);
      end;
    end;
  end;

var
  L, T, W, H, DeltaX, DeltaY: Integer;
  VideoRect: TRect;
begin
  L := Left;
  T := Top;
  W := Width;
  H := Height;
  DeltaX := X - FSizeOrigin.X;
  DeltaY := Y - FSizeOrigin.Y;
  VideoRect:=FOwner.VideoOutputRect;
  case SizingKind of
    smTopLeft:
      begin
        if L+DeltaX<VideoRect.Left then DeltaX:=VideoRect.Left-L;
        if T+DeltaY<VideoRect.Top then DeltaY:=VideoRect.Top-T;
        CheckResizeXYRatio(DeltaX, DeltaY, -1, -1);
        if (L + DeltaX >= 0) and (W - DeltaX > MinWidth) then
        begin
          L := L + DeltaX;
          W := W - DeltaX;
        end;
        if (T + DeltaY >= 0) and (H - DeltaY > MinHeight) then
        begin
          T := T + DeltaY;
          H := H - DeltaY;
        end;
      end;
    smTop:
      begin
        DeltaX:=0;
        if T+DeltaY<VideoRect.Top then DeltaY:=VideoRect.Top-T;
        CheckResizeXYRatio(DeltaX, DeltaY, 1, -1);
        if (T + DeltaY >= 0) and (H - DeltaY > MinHeight) then
        begin
          T := T + DeltaY;
          H := H - DeltaY;
        end;
        W := W + DeltaX;
      end;
    smTopRight:
      begin
        if T+DeltaY<VideoRect.Top then DeltaY:=VideoRect.Top-T;
        if (L+W+DeltaX)>VideoRect.Right then
          DeltaX:=VideoRect.Right-W-L;
        CheckResizeXYRatio(DeltaX, DeltaY, 1, -1);
        W := W + DeltaX;
        if (T + DeltaY >= 0) and (H - DeltaY > MinHeight) then
        begin
          T := T + DeltaY;
          H := H - DeltaY;
        end;
      end;
    smLeft:
      begin
        DeltaY:=0;
        if L+DeltaX<VideoRect.Left then DeltaX:=VideoRect.Left-L;
        CheckResizeXYRatio(DeltaX, DeltaY, -1, 1);
        if (L + DeltaX >= 0) and (W - DeltaX > MinWidth) then
        begin
          L := L + DeltaX;
          W := W - DeltaX;
        end;
        H := H + DeltaY;
      end;
    smRight:
      begin
        DeltaY:=0;
        if (L+W+DeltaX)>VideoRect.Right then
          DeltaX:=VideoRect.Right-W-L;
        CheckResizeXYRatio(DeltaX, DeltaY, 1, 1);
        W := W + DeltaX;
        H := H + DeltaY;
      end;
    smBottomLeft:
      begin
        if L+DeltaX<VideoRect.Left then DeltaX:=VideoRect.Left-L;
        if (T+H+DeltaY)>VideoRect.Bottom then
          DeltaY:=VideoRect.Bottom-H-T;
        CheckResizeXYRatio(DeltaX, DeltaY, -1, 1);
        if (L + DeltaX >= 0) and (W - DeltaX > MinWidth) then
        begin
          L := L + DeltaX;
          W := W - DeltaX;
          H := H + DeltaY;
        end;
      end;
    smBottom:
      begin
        DeltaX:=0;
        if (T+H+DeltaY)>VideoRect.Bottom then
          DeltaY:=VideoRect.Bottom-H-T;
        CheckResizeXYRatio(DeltaX, DeltaY, 1, 1);
        W := W + DeltaX;
        H := H + DeltaY;
      end;
    smBottomRight:
      begin
        if (L+W+DeltaX)>VideoRect.Right then
          DeltaX:=VideoRect.Right-W-L;
        if (T+H+DeltaY)>VideoRect.Bottom then
          DeltaY:=VideoRect.Bottom-H-T;
        CheckResizeXYRatio(DeltaX, DeltaY, 1, 1);
        W := W + DeltaX;
        H := H + DeltaY;
      end;
  end;
  FSizeOrigin.X := X;
  FSizeOrigin.Y := Y;
  CheckUpdateHandleBounds(Bounds(L, T, W, H), [crtResize]);
  BoundsChanged;
end;

procedure TMPVCropControl.SetAreaRestrict(Value: TCropRestrictType);
begin
  if Value<>FAreaRestrict then
  begin
    FAreaRestrict := Value;
    if FAreaRestrict<>carNone then
      CheckUpdateHandleBounds;
  end;
end;

procedure TMPVCropControl.SetBounds(ALeft, ATop, AWidth, AHeight: Integer);
begin
  BoundsRect:=Bounds(ALeft, ATop, AWidth, AHeight);
end;

procedure TMPVCropControl.SetBoundsRect(const Value: TRect);
begin
  CheckUpdateHandleBounds(Value, [crtResize, crtMove]);
  BoundsChanged;
end;

procedure TMPVCropControl.SetCropRect(const Value: TRect);
begin
  if not FDraging and not FCroping then
  begin
    FCropRect:=Value;
    UpdateBoundsFromCropRect;
  end;
end;

procedure TMPVCropControl.SetHeight(Value: Integer);
begin
  if Value<>Height then
  begin
    with FBoundsRect do
      Bottom:=Top+Value;
    UpdateHandlersBounds;
  end;
end;

procedure TMPVCropControl.SetLeft(Value: Integer);
begin
  SetBounds(Value, Top, Width, Height);
end;

procedure TMPVCropControl.SetMouseInClient(Value: Boolean);
begin
  FMouseInClient := Value;
end;

procedure TMPVCropControl.SetMouseIsDown(Value: Boolean);
begin
  FMouseInDown := Value;
end;

procedure TMPVCropControl.SetSizeable(Value: Boolean);
var
  pHandler: TMPVResizeHandler;
begin
  if Value<>FSizeable then
  begin
    FSizeable := Value;
    for pHandler in FResizeHandlers do
      pHandler.Visible:=Value and Visible;
  end;
end;

procedure TMPVCropControl.SetTop(Value: Integer);
begin
  SetBounds(Left, Value, Width, Height);
end;

procedure TMPVCropControl.SetVideoAreaRestrict(Value: Boolean);
begin
  if Value<>GetVideoAreaRestrict then
  begin
    if Value then FAreaRestrict:=carVideoArea else FAreaRestrict:=carClientArea;
    if Value then
      CheckUpdateHandleBounds;
  end;
end;

procedure TMPVCropControl.SetVisible(Value: Boolean);
var
  pLine: TMPVFrameLine;
  bHandleVisible: Boolean;
  pHandler: TMPVResizeHandler;
begin
  if Value<>FVisible then
  begin
    FVisible := Value;
    for pLine in FFrameLines do
    begin
      pLine.Visible:=Value;
    end;
    bHandleVisible:=Value and FSizeable;
    for pHandler in FResizeHandlers do
    begin
      pHandler.Visible:=bHandleVisible;
    end;
  end;
end;

procedure TMPVCropControl.SetWidth(Value: Integer);
begin
  if Value<>Width then
  begin
    with FBoundsRect do
      Right:=Left+Value;
    UpdateHandlersBounds;
  end;
end;

procedure TMPVCropControl.VideoCropChange;
begin
  CheckUpdateHandleBounds();
  BoundsChanged;
end;

procedure TMPVCropControl.UpdateBoundsFromCropRect;
begin
  CheckUpdateHandleBounds(VideoToWindow(FCropRect));
end;

procedure TMPVCropControl.UpdateHandlersBounds;
var
  ARect: TRect;
  pLine: TMPVFrameLine;
  pHandler: TMPVResizeHandler;
begin
  for pLine in FFrameLines do
  begin
    pLine.BoundsRect:=GetFrameLineRect(pLine.LineKind);
  end;
  for pHandler in FResizeHandlers do
  begin
    ARect:=GetResizingHandlerRect(pHandler.SizingKind);
    ClientToParent(ARect);
    pHandler.BoundsRect:=ARect;
  end;
  FOwner.Update;
end;

function TMPVCropControl.VideoToWindow(const Value: TRect): TRect;
var
  AVidRect, AClientRect: TRect;
  nVidWidth, nVidHeight: Integer;
  nWidth, nHeight: Integer;
  xratio, yratio: Double;
begin
  if (FOwner.FViewMode=OUTPUT_VIEW_MODE_ID) and (FOwner.FOutVideoSize.cx<>0) and (FOwner.FOutVideoSize.cy<>0) then // 输出视图
  begin
    nVidWidth:=FOwner.FOutVideoSize.cx;
    nVidHeight:=FOwner.FOutVideoSize.cy;
  end else if Assigned(FOwner.FPlayer) then
  with FOwner.VideoInfo do
  begin
    nVidWidth:=Width;
    nVidHeight:=Height;
  end else
  begin
    Result:=Value;
    Exit;
  end;
  AVidRect:=FOwner.VideoOutputRect;
  AClientRect:=FOwner.ClientRect;
  with AVidRect do
  begin
    nWidth:=Right-Left;
    nHeight:=Bottom-Top;
  end;
  xratio:=nWidth/(nVidWidth+0.000001);
  yratio:=nHeight/(nVidHeight+0.000001);
  Result.Left:=AVidRect.Left+Round(Value.Left*xratio);
  Result.Right:=Result.Left+Round((Value.Right-Value.Left)*xratio)-1;
  Result.Top:=AVidRect.Top+Round(Value.Top*yratio);
  Result.Bottom:=Result.Top+Round((Value.Bottom-Value.Top)*yratio)-1;
end;

function TMPVCropControl.WindowToVideo(const Value: TRect): TRect;
var
  AVidRect, AClientRect: TRect;
  nVidWidth, nVidHeight: Integer;
  nWidth, nHeight: Integer;
  xratio, yratio: Double;
begin
  if Assigned(FOwner.FPlayer) then
  with FOwner.VideoInfo do
  begin
    nVidWidth:=Width;
    nVidHeight:=Height;
  end else
  begin
    Result:=Value;
    Exit;
  end;
  AVidRect:=FOwner.VideoOutputRect;
  AClientRect:=FOwner.ClientRect;
  with AVidRect do
  begin
    nWidth:=Right-Left;
    nHeight:=Bottom-Top;
  end;
  xratio:=nVidWidth/(nWidth+0.000001);
  yratio:=nVidHeight/(nHeight+0.000001);
  Result.Left:=Round((Value.Left-AVidRect.Left)*xratio);
  Result.Right:=Round((Value.Right-AVidRect.Left+1)*xratio);
  Result.Top:=Round((Value.Top-AVidRect.Top)*yratio);
  Result.Bottom:=Round((Value.Bottom-AVidRect.Top+1)*yratio);
end;

{ TMediaPlayerView }

type
  PMoveChildWindowData = ^TMoveChildWindowData;
  TMoveChildWindowData = record
    OldParent, NewParent: HWND;
  end;

function MoveChildWindow(Window: HWND; Data: PMoveChildWindowData): BOOL; stdcall;
begin
  if Windows.GetParent(Window)=Data.OldParent then
  begin
    Windows.SetParent(Window, Data.NewParent);
    SetWindowPos(Window, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE or SWP_NOSIZE or SWP_NOREDRAW or SWP_NOACTIVATE);
  end;
  Result := True;
end;

procedure TMediaPlayerView.CleanupFullScreenWindow;
var
  hFSW: HWnd;
begin
  if FFullScreenWindow<>0 then
  begin
    hFSW:=FFullScreenWindow;
    FFullScreenWindow:=0;
    Classes.DeallocateHWnd(hFSW);
  end;
end;

procedure TMediaPlayerView.CMRecreateWnd(var Message: TMessage);
var
  WasFocused: Boolean;
  SaveDefWndProc: Pointer;
  SaveHandle: HWnd;
  LData: TMoveChildWindowData;
begin
  SaveHandle:=WindowHandle;
  if Showing then
  begin // 优化重建句柄过程，避免闪烁
    SaveDefWndProc:=DefWndProc;
    WasFocused := Focused;
    UpdateRecreatingFlag(True);
    try
      SetWindowLong(SaveHandle, GWL_WNDPROC, Integer(SaveDefWndProc)); // 防止控件响应
      Perform(WM_NCDESTROY, 0, 0);  // 手工重置句柄和显示状态
      HandleNeeded;
      LData.OldParent:=SaveHandle;
      LData.NewParent:=WindowHandle;
      EnumChildWindows(SaveHandle, @MoveChildWindow, Integer(@LData));
      UpdateControlState;
      if Assigned(FOldPlayer) and (FOldId<>0) then
        FOldPlayer.DestroyView(FOldId)
      else
        Windows.DestroyWindow(SaveHandle);
    finally
      UpdateRecreatingFlag(False);
    end;
    if WasFocused and (WindowHandle <> 0) then
      Windows.SetFocus(WindowHandle);
  end else
    inherited;
end;

procedure TMediaPlayerView.CNChar(var Message: TWMChar);
begin
  if FullScreen then
    Message.Result:=0
  else
    inherited;
end;

procedure TMediaPlayerView.CNKeyDown(var Message: TWMKeyDown);
var
  dx, dy: Integer;
  ShiftState: TShiftState;
begin
  ShiftState:=KeyDataToShiftState(Message.KeyData);
  if [ssCtrl] = ShiftState then
  begin
    dx:=0;
    dy:=0;
    case Message.CharCode of
      VK_LEFT: dx:=-1;
      VK_UP: dy:=-1;
      VK_RIGHT: dx:=1;
      VK_DOWN: dy:=1;
    end;
    if Assigned(FCropControl) then
      FCropControl.MoveWith(dx, dy);
    Exit;
  end else if [ssShift] = ShiftState then
  begin
    dx:=0;
    dy:=0;
    case Message.CharCode of
      VK_LEFT: dx:=-1;
      VK_UP: dy:=-1;
      VK_RIGHT: dx:=1;
      VK_DOWN: dy:=1;
    end;
    if Assigned(FCropControl) then
      FCropControl.ResizeWith(dx, dy);
    Exit;
  end;
  if FullScreen then
  begin
    Message.Result := 0;
    UpdateUIState(Message.CharCode);
  end else
    inherited;
end;

procedure TMediaPlayerView.CNKeyUp(var Message: TWMKeyUp);
begin
  if FullScreen then
    Message.Result := 0
  else
    inherited;
end;

constructor TMediaPlayerView.Create(AOwner: TComponent);
begin
  inherited Create(AOwner);
  ControlStyle:=ControlStyle+[csAcceptsControls];
  UseDockManager:=True;
end;

procedure TMediaPlayerView.CreateParams(var Params: TCreateParams);
begin
  inherited;
  with Params do
  begin
    WindowClass.style:=WindowClass.style or (CS_VREDRAW or CS_HREDRAW);
    if Assigned(FPlayer) then
      StrCat(WinClassName, 'X');
  end;
end;

procedure TMediaPlayerView.CreateWindowHandle(const Params: TCreateParams);
var
  ResizeParam: RESIZE_PARAM;
begin
  if Assigned(FPlayer) then
  with Params do
  begin
    Windows.UnregisterClass(WinClassName, WindowClass.hInstance);
    ResizeParam.width:=FOutVideoSize.cx;
    ResizeParam.height:=FOutVideoSize.cy;
    ResizeParam.uInterpolation:=IIN_AUTO;
    ResizeParam.uStyle:=IRM_ORIGINAL_SCALE;
    FId:=FPlayer.CreateViewEx(WndParent, Bounds(X, Y, Width, Height), @ResizeParam, FViewMode);
    WindowHandle:=FPlayer.PlayerWnd[FId];
    DefWndProc:=Pointer(SetWindowLong(WindowHandle, GWL_WNDPROC, Integer(WindowClass.lpfnWndProc)));
    SetWindowLong(WindowHandle, GWL_STYLE, Style);
    SetWindowLong(WindowHandle, GWL_EXSTYLE, ExStyle);
    SetClassLong(WindowHandle, GCL_STYLE, WindowClass.style);
    SetWindowText(WindowHandle, Caption);
    FFullErasebkgnd:=False;
  end else
    inherited;
end;

destructor TMediaPlayerView.Destroy;
begin
  CleanupFullScreenWindow;
  FCropControl.Free;
  inherited;
end;

procedure TMediaPlayerView.DestroyWindowHandle;
var
  pPlayer: TMediaPlayer;
  nViewId: Integer;
begin
  if (csRecreating in ControlState) and Assigned(FOldPlayer) then
  begin
    pPlayer:=FOldPlayer;
    nViewId:=FOldId;
  end else if Assigned(FPlayer) then
  begin
    pPlayer:=FPlayer;
    nViewId:=FId;
  end else
  begin
    pPlayer:=nil;
    nViewId:=0;
  end;
  if Assigned(pPlayer) and (nViewId<>0) then
  begin
    ControlState:=ControlState+[csDestroyingHandle];
    try
      pPlayer.DestroyView(nViewId);
    finally
      ControlState:=ControlState-[csDestroyingHandle];
    end;
    WindowHandle:=0;
  end else
    inherited;
end;

procedure TMediaPlayerView.DoCropAreaChange;
begin
  if Assigned(FOnCropAreaChange) then FOnCropAreaChange(Self);
  if Assigned(FPlayer) then
  begin
    if Assigned(FPlayer.OnCropAreaChange) then
      FPlayer.OnCropAreaChange(FPlayer);
    if FPlayer.CallBackWnd<>0 then
      SendMessage(FPlayer.CallBackWnd, UM_CROP_CHANGE, 0, 0);
  end;
end;

procedure TMediaPlayerView.FullScreenWndProc(var Message: TMessage);
begin
  with Message do
    Result := DefWindowProc(FFullScreenWindow, Msg, wParam, lParam);
end;

function TMediaPlayerView.GetCropControl: TMPVCropControl;
begin
  InitCropControl;
  Result:=FCropControl;
end;

function TMediaPlayerView.GetCropRect: TRect;
begin
  if ShowCropControl then
    Result:=FCropControl.CropRect
  else
    FPlayer.GetCropRect(FId, Result);
end;

function TMediaPlayerView.GetDisplayResizeMode: IMAGE_RESIZE_METHOD;
begin
  Result:=FPlayer.GetDisplayResizeMode(FId);
end;

function TMediaPlayerView.GetKeepRatio: Boolean;
begin
  if Assigned(FCropControl) then
    Result:=FCropControl.KeepRatio
  else
    Result:=False;
end;

function TMediaPlayerView.GetShowCropControl: Boolean;
begin
  Result:=Assigned(FCropControl) and FCropControl.Visible;
end;

function TMediaPlayerView.GetVideoProcessHandle: HVIDEODECODER;
begin
  Result:=FPlayer.GetVideoProcessHandle(FId);
end;

function TMediaPlayerView.GetVideoProcessor: TVideoMediaProcessor;
begin
  Result:=FPlayer.GetVideoProcessor(FId);
end;

function TMediaPlayerView.GetVideoInfo: TWSVideoInfo;
var
  pVideo: TVideoMediaProcessor;
begin
  pVideo:=GetVideoProcessor;
  try
    Result:=pVideo.VideoInfo^;
  finally
    pVideo.Free;
  end;
end;

function TMediaPlayerView.GetVideoOutputRect: TRect;
var
  dVideoScale, dWinScale: Double;
  nVidWidth, nVidHeight: Integer;
  nWidth, nHeight: Integer;
  n: Integer;
  nxratio, nyratio: Integer;
  nResizeMode: IMAGE_RESIZE_METHOD;
  AClientRect: TRect;
begin
  AClientRect:=ClientRect;
  with AClientRect do
  begin
    nWidth:=Right-Left;
    nHeight:=Bottom-Top;
  end;
  if (FViewMode=OUTPUT_VIEW_MODE_ID) and (FOutVideoSize.cx<>0) and (FOutVideoSize.cy<>0) then // 输出视图
  begin
    nVidWidth:=FOutVideoSize.cx;
    nVidHeight:=FOutVideoSize.cy;
    nxratio:=0;
    nyratio:=0;
  end else if Assigned(FPlayer) then
  with VideoInfo do
  begin
    nVidWidth:=Width;
    nVidHeight:=Height;
    nxratio:=xratio;
    nyratio:=yratio;
  end else
  begin
    Result:=AClientRect;
    Exit;
  end;
  dWinScale:=nWidth/(nHeight+0.000001);
  nResizeMode:=GetDisplayResizeMode; 

  case nResizeMode of
    IRM_16_9:
      dVideoScale := 16.0 / 9;
    IRM_4_3:
      dVideoScale := 4.0 / 3;
    IRM_FULLSCREEN:
      dVideoScale:=dWinScale;
//    IRM_LETTERBOX,
//    IRM_ORIGINAL_SCALE,
//    IRM_PAN_SCAN:
  else
    if (nyratio = 0) then
      dVideoScale := nVidWidth/(nVidHeight+0.000001)
    else
      dVideoScale := nxratio / (nyratio + 0.000001);
  end;
  if (dWinScale>=dVideoScale) or (nResizeMode=IRM_PAN_SCAN) then
  begin
    Result:=AClientRect;
    n:=Round(nHeight*dVideoScale);
    Inc(Result.Left, (nWidth-n) div 2);
    Result.Right:=Result.Left+n;
  end else
  begin
    Result:=AClientRect;
    n:=Round(nWidth/dVideoScale);
    Inc(Result.Top, (nHeight-n) div 2);
    Result.Bottom:=Result.Top+n;
  end;
end;

procedure TMediaPlayerView.InitCropControl;
begin
  if FCropControl=nil then
  begin
    FCropControl:=TMPVCropControl.Create(Self);
  end;
end;

procedure TMediaPlayerView.InitFullScreenWindow;
begin
  if FFullScreenWindow=0 then
  begin
    FFullScreenWindow:=Classes.AllocateHWnd(FullScreenWndProc);
    SetWindowText(FFullScreenWindow, PChar(Caption));
  end;
end;

procedure TMediaPlayerView.KeyDown(var Key: Word; Shift: TShiftState);
begin
  inherited;
  if (ssAlt in Shift) and (Key=VK_F4) then
    Key:=0;
end;

procedure TMediaPlayerView.MouseDown(Button: TMouseButton; Shift: TShiftState;
  X, Y: Integer);
begin
  inherited;
  if Canfocus then SetFocus;
  if Assigned(FCropControl) then
    FCropControl.MouseDown(Button, Shift, X, Y);
end;

procedure TMediaPlayerView.MouseMove(Shift: TShiftState; X, Y: Integer);
begin
  inherited;
  if Assigned(FCropControl) then
    FCropControl.MouseMove(Shift, X, Y);
end;

procedure TMediaPlayerView.MouseUp(Button: TMouseButton; Shift: TShiftState; X,
  Y: Integer);
begin
  inherited;
//  if PointInCropControl(X, Y) then
  if Assigned(FCropControl) then
    FCropControl.MouseUp(Button, Shift, X, Y);
end;

procedure TMediaPlayerView.RecreateWnd;
begin
  inherited RecreateWnd;
end;

procedure TMediaPlayerView.Resize;
begin
  inherited;
  if Assigned(FCropControl) and FCropControl.Visible then
    FCropControl.UpdateBoundsFromCropRect;
end;

procedure TMediaPlayerView.SetCropEditStyle(uCropStyle: UINT);
begin
  FPlayer.SetCropEditStyle(FId, uCropStyle);
end;

procedure TMediaPlayerView.SetCropRect(const pCropRect: TRect);
begin
  InitCropControl;
  FCropControl.CropRect:=pCropRect;
end;

function TMediaPlayerView.SetDisplayResizeMode(irm: IMAGE_RESIZE_METHOD): BOOL;
begin
  Result:=FPlayer.SetDisplayResizeMode(FId, irm);
  if Assigned(FCropControl) and FCropControl.Visible then
    FCropControl.UpdateBoundsFromCropRect;
end;

procedure TMediaPlayerView.SetFullScreen(Value: Boolean);
begin
  if Value<>FFullScreen then
  begin
    FFullScreen:=Value;
    if Value then
    begin
      InitFullScreenWindow;
      FSaveWndStyle:=GetWindowLong(FFullScreenWindow, GWL_STYLE);
//      FSaveWndExStyle:=GetWindowLong(FFullScreenWindow, GWL_EXSTYLE);
      SetWindowLong(FFullScreenWindow, GWL_STYLE, (FSaveWndStyle or WS_POPUP or WS_CLIPCHILDREN) and not (WS_CHILD or WS_BORDER)); // 修改窗口风格
//      SetWindowLong(FFullScreenWindow, GWL_EXSTYLE, FSaveWndExStyle and not (WS_EX_TOOLWINDOW)); // 修改窗口风格
      Windows.SetParent(WindowHandle, FFullScreenWindow);
      ShowWinNoAnimate(FFullScreenWindow, SW_SHOWMAXIMIZED);
      ShowWinNoAnimate(WindowHandle, SW_SHOWMAXIMIZED);
      if System.DebugHook=0 then
        SetWindowPos(FFullScreenWindow, HWND_TOPMOST, 0, 0, 0, 0,
          SWP_NOSIZE or SWP_NOMOVE or SWP_NOACTIVATE or SWP_NOOWNERZORDER);// 窗口置顶
      Windows.SetFocus(WindowHandle);
      UpdateBounds;
    end else
    begin
      ShowWinNoAnimate(FFullScreenWindow, SW_RESTORE);            // 恢复原大小
//      SetWindowLong(FFullScreenWindow, GWL_EXSTYLE, FSaveWndExStyle); // 恢复窗口风格
      SetWindowLong(FFullScreenWindow, GWL_STYLE, FSaveWndStyle); // 恢复客户区风格
      SetWindowPos(FFullScreenWindow, HWND_NOTOPMOST, 0, 0, 0, 0,
        SWP_NOSIZE or SWP_NOMOVE or SWP_NOACTIVATE);                // 取消窗口置顶
      Windows.SetParent(WindowHandle, GetParentHandle);        // 恢复父窗口
      ShowWindow(FFullScreenWindow, SW_HIDE);                  // 隐藏全屏窗口
      Windows.SetFocus(WindowHandle);
      UpdateBounds;
      CleanupFullScreenWindow;
    end;
  end;
end;

procedure TMediaPlayerView.SetKeepRatio(Value: Boolean);
begin
  if Assigned(FCropControl) then
    FCropControl.KeepRatio:=Value;
end;

procedure TMediaPlayerView.SetOutVideoSize(const Value: TSize);
begin
  if FId=0 then
  begin
    FOutVideoSize := Value;
  end else if (Value.cx<>FOutVideoSize.cx) or (Value.cy<>FOutVideoSize.cy) then
  begin
    FOutVideoSize := Value;
    RecreateWnd;
  end;
end;

procedure TMediaPlayerView.SetPlayer(Value: TMediaPlayer);
begin
  if Value<>FPlayer then
  begin
    if Assigned(Value) then
    begin
      FOldPlayer:=FPlayer;
      FOldId:=FId;
      FPlayer := Value;
      RecreateWnd;
    end else
    begin
      DestroyHandle;
      FPlayer := Value;
    end;
    FOldPlayer:=nil;
    FOldId:=0;
  end;
end;

procedure TMediaPlayerView.SetShowCropControl(Value: Boolean);
begin
  if Value then
    InitCropControl;
  if Assigned(FCropControl) then
  begin
    FCropControl.Visible:=Value;
  end;
end;

procedure TMediaPlayerView.SetViewMode(Value: Integer);
begin
  if (FId=0) then
    FViewMode := Value;
end;

procedure TMediaPlayerView.VideoCropChange;
begin
  if Assigned(FCropControl) then
    FCropControl.VideoCropChange;
end;

procedure TMediaPlayerView.WMDestroy(var Message: TWMDestroy);
begin
  FFullErasebkgnd:=True;
  if (csDestroying in ComponentState) then
    FPlayer:=nil;
  inherited;
  CleanupFullScreenWindow;
end;

procedure TMediaPlayerView.WMEraseBkgnd(var Message: TWMEraseBkgnd);
begin
  if not FFullErasebkgnd and Assigned(FPlayer) and (FId<>0) then
    DefaultHandler(Message)
  else
    inherited;
end;

procedure TMediaPlayerView.WMNCDestroy(var Message: TWMNCDestroy);
begin
  FFullErasebkgnd:=True;
  inherited;
  FId:=0;
end;

procedure TMediaPlayerView.WMSetCursor(var Message: TWMSetCursor);
var
  pt: TPoint;
begin
  if Assigned(FCropControl) and FCropControl.Visible then
  with Message do
    if CursorWnd = Handle then
      case Smallint(HitTest) of
        HTCLIENT:
          begin
            GetCursorPos(pt);
            pt:=ScreenToClient(pt);
            if FCropControl.PointInCropControl(pt.X, pt.Y) then
            begin
              Windows.SetCursor(Screen.Cursors[crSizeAll]);
              Result := 1;
              Exit;
            end;
          end;
      end;
  inherited;
end;

end.
