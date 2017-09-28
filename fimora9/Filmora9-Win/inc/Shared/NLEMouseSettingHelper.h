#ifndef NLE_MOUSE_SETTING_HELPER_H
#define NLE_MOUSE_SETTING_HELPER_H

#include "stdafx.h"

#include "NLEGraphics.h"

#include <string>
#include <vector>

class CNLESegmentMouseInfo
{
public:
	CNLESegmentMouseInfo(){}
	~CNLESegmentMouseInfo(){}

	//比例以100为基数，如0-100表示0%-100%
	int scaleFactor; //鼠标大小的缩放比例，0 - 500
	int opacity; //透明度， 0 - 100
	std::wstring highlightImgPath; //高亮图片路径


	NLERange range;
	std::wstring iconDirectory;
	std::wstring mouseImgPath;
	NLEPoint mouseImgHotspot; //热点相对于鼠标图像的左上角的坐标
	std::wstring clickedEffectImgPath;
	std::wstring doubleClickedEffectImgPath;
	std::wstring rightClickedEffectImgPath;
	std::wstring dragEffectImgPath;
};

class CNLEMouseSettingHelper
{
public:
	CNLEMouseSettingHelper();
	~CNLEMouseSettingHelper();

	/**
	@brief 将鼠标信息转换成一个字符串
	*/
	std::wstring GetPropertyValue();

	/**
	@brief 将设置的包含鼠标信息的字符串解析为鼠标信息结构
	*/
	HRESULT SetPropertyValue(std::wstring value);
	
	/**
	@brief 获取鼠标信息段的个数
	*/
	int SegmentCount();

	/**
	@brief 添加一个鼠标段的信息
	*/
	HRESULT AddSegmentMouseInfo(CNLESegmentMouseInfo info);
	
	/**
	@brief 获取指定的鼠标段的信息
	@param index 序号，值为[0, count-1]
	*/
	HRESULT GetSegmentMouseInfo(int index, CNLESegmentMouseInfo *pInfo);

	/**
	@brief 删除指定的鼠标段
	@param index 序号，值为[0, count-1]
	*/
	HRESULT RemoveSegmentMouseInfo(int index);

	/**
	@brief 清除所有的鼠标段信息
	*/
	HRESULT Clear();
	
private:
	std::vector<CNLESegmentMouseInfo> m_segmentMouseInfoVec;
};

#endif
