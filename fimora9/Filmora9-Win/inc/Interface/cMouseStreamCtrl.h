

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Thu Mar 02 11:38:37 2017
 */
/* Compiler settings for cMouseStreamCtrl.idl:
    Oicf, W1, Zp8, env=Win64 (32b run), target_arch=AMD64 8.00.0603 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */

#include "stdint.h"
#include <Guiddef.h>
#include <objbase.h>

#ifndef __cMouseStreamCtrl_h__
#define __cMouseStreamCtrl_h__

// {06B6E0A8-C2B7-4C15-BBE8-4FF1F8883AED}
DEFINE_GUID(IID_IMouseStreamCtrl, 
0x6b6e0a8, 0xc2b7, 0x4c15, 0xbb, 0xe8, 0x4f, 0xf1, 0xf8, 0x88, 0x3a, 0xed);

#ifdef __cplusplus
extern "C"{
#endif 

enum MouseEvent
{
	MouseEvent_None					= 0x00000000,

	MouseEvent_L16_Mask				= 0x0000ffff,
	MouseEvent_L16_LeftClick		= 0x00000001,
	MouseEvent_L16_MiddleClick		= 0x00000002,
	MouseEvent_L16_RightClick		= 0x00000004,
	MouseEvent_L16_LeftDblClick		= 0x00000008,
	MouseEvent_L16_MiddleDblClick	= 0x00000010,
	MouseEvent_L16_RightDblClick	= 0x00000020,

	MouseEvent_H16_Mask				= 0xffff0000,
	MouseEvent_H16_Ctrl				= 0x00010000,
	MouseEvent_H16_Shift			= 0x00020000,
	MouseEvent_H16_Alt				= 0x00040000,
	MouseEvent_H16_Win				= 0x00080000,
	MouseEvent_H16_Command			= MouseEvent_H16_Win
};

#pragma pack(push)
#pragma pack(4)
typedef struct Mouse_Frame
    {
    int32_t x, y; // 鼠标轨迹位置（相对于捕获画面的位置）
    int32_t event; // 低16位： 对应于MouseEvent_L16_xyz；高16位：对应于MouseEvent_H16_xyz
    int32_t key; // 保留

    int32_t w, h; // 鼠标图标的尺寸
    int32_t hs_x, hs_y; // 热点的位置
    int32_t size; // icon_data的数据长度
    int8_t *icon_data; // RGBA数据
    } 	Mouse_Frame;
#pragma pack(pop)

#define MAX_MOUSE_KEYFRAME_INTERVALS    30
    
// 鼠标流的控制接口

#ifndef __IMouseStreamCtrl_INTERFACE_DEFINED__
#define __IMouseStreamCtrl_INTERFACE_DEFINED__


class IMouseStreamCtrl : public IUnknown
{
public:
	virtual HRESULT STDMETHODCALLTYPE PushFrame(
		/* [in] */ Mouse_Frame *frame) = 0;
};


#endif 	/* __IMouseStreamCtrl_INTERFACE_DEFINED__ */


#ifdef __cplusplus
}
#endif

#endif


