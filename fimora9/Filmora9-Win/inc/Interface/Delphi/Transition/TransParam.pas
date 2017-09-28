unit TransParam;

  {$MINENUMSIZE 4}

interface

uses
  Windows;

type

  // YV12图像数据结构定义
  YV12Image = record
    y         : PByte;
    u         : PByte;
    v         : PByte;
    a         : PByte;        // Alpha 通道（不透明度）
    width     : Integer;
    height    : Integer;
    y_pitch   : Integer;
    u_pitch   : Integer;
    v_pitch   : Integer;
  end;
  PYV12Image = ^YV12Image;

  // 转场参数结构定义
  TTransParam = record
    szTransID : array [0..63] of Char;  // Transition ID
    fLength   : Single;                 // Transition 长度（时间单位）
    fPos      : Single;                 // Transition 当前位置

  	// 以下参数由不同的转场实现定义其意义
    nParam1   : Integer;
    nParam2   : Integer;
    nParam3   : Integer;
    nParam4   : Integer;
    
    fParam1   : Single;
    fParam2   : Single;
    fParam3   : Single;
    fParam4   : Single;
  end;
  PTTransParam = ^TTransParam;

type
  TRANSITION_API = procedure(imgOut : PYV12Image; const imgFrom : PYV12Image; const imgTo : PYV12Image; const trParam : PTTransParam); stdcall;

implementation

end.
