#pragma once

#ifndef _H_INLEMONTAGE_H_
#define _H_INLEMONTAGE_H_

#include "NLEType.h"
#include <Guiddef.h>
#include <objbase.h>

#include "INLETimelineBase.h"

// {3FEB3BF6-573E-4FFC-A36A-1F3E769EFF47}
DEFINE_GUID(CLSID_NLEMontage, 0x3feb3bf6, 0x573e, 0x4ffc, 0xa3, 0x6a, 0x1f, 0x3e, 0x76, 0x9e, 0xff, 0x47);

// {2163F6CB-AFFB-4739-B45C-01C0B2F4C508}
DEFINE_GUID(IID_INLEMontage, 0x2163f6cb, 0xaffb, 0x4739, 0xb4, 0x5c, 0x1, 0xc0, 0xb2, 0xf4, 0xc5, 0x8);

/**
*@brief 蒙太奇控制接口。
*/
class INLEMontage :public IUnknown
{
public:
	/**
	*@brief 加载/保存蒙太奇模板
	*@param filePath 模板文件路径。加载时，NULL - 重置蒙太奇为空框架。
	*@param montageName 蒙太奇名称
	*@param	reload 是否重新加载模板文件。默认false，不重新加载
	*@return 标准返回
	*@note		
	*/
	virtual HRESULT LoadTemplate(WCHAR* filePath, WCHAR* montageName, bool reload = false) = 0;
	virtual HRESULT SaveTemplate(WCHAR* filePath, WCHAR* montageName) = 0;

	/**
	*@brief 获取蒙太奇场景数量
	*@param sceneCnt 场景数量
	*@return 标准返回
	*@note		
	*/
	virtual HRESULT GetSceneCount(int& sceneCnt) = 0;

	/**
	*@brief 设置/获取蒙太奇场景的源片段
	*@param sceneIdx 场景序号
	*@param scene 场景源片段
	*@return 标准返回
	*@note		
	*/
	virtual HRESULT SetSceneSource(int sceneIdx, INLETimelineBase* scene) = 0;
	virtual HRESULT GetSceneSource(int sceneIdx, INLETimelineBase** scene) = 0;

	/**
	*@brief 设置/获取蒙太奇场景的边框参数
	*@param thickness 边框宽度
	*@param clr 边框颜色
	*@return 标准返回
	*@note		
	*/
	virtual HRESULT SetBorder(int thickness, NLEColor clr) = 0;
	virtual HRESULT GetBorder(int& thickness, NLEColor& clr) = 0;

	/**
	*@brief 设置/获取蒙太奇被选中的场景{被选中的场景，边框会被反色}
	*@param pt 鼠标点的相对位置
	*@param sceneIdx 场景序号
	*@return 标准返回
	*@note		
	*/
	virtual HRESULT SetSelectSceneByPoint(NLEPointF *pt) = 0;
	virtual HRESULT SetSelectScene(int sceneIdx) = 0;
	virtual HRESULT GetSelectScene(int &sceneIdx) = 0;
};

#endif //_H_INLEMONTAGE_H_


