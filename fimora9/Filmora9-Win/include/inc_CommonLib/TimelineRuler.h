/*
*Author:		zhy
*Date:			2016.06.13
*Description:	时间尺控件
*/

#pragma once


#include "inc_CommonLib/commonlib_global.h"

#include <QtWidgets/QWidget>
#include <QtCore/QList>

#include <tuple>

struct RulerPointerShape
{
	QPolygonF	shape;		//pointer的形状
	int			height;		//pointer高度
	int			width;		//pointer高度
	QColor		color;
	bool		aligntop;	//pointer放在尺子的上方或者下方
	int			linewidth;	//线宽度
	bool		showpostext; //是否显示当前时间点
	QColor		postextcolor;
};

class TimelineViewRuler;
class COMMONLIB_EXPORT TimelineRuler : public QWidget
{
	Q_OBJECT
public:
	TimelineRuler(QWidget *parent = nullptr);

	// 设置当前帧
	void SetCurrentFrames(int frames);
	// 获取当前帧
	int CurrentFrames();

	// 设置时间尺偏移帧
	void SetOffsetFrames(int frames);
	int OffsetFrames();

	// 设置时间尺偏移像素，主用于滚动条
	void SetOffsetPosition(int position);
	//int OffsetPosition();

	// 设置总长
	void SetTotalFrames(int frames);
	int TotaleFrames();

	// 当前时间尺长度显示的总帧数
	int AvailableTotaleFrames();

	// 设置时间尺zoom值
    void SetZoom(float zoom);
    float Zoom();

	void StartZoomAndScrollRuler();
	int GetZoomAndScrollRulerOffsetPosition();

    float FindZoom(int frames, int position);

	// 设置时间尺最大zoom
    void SetMaxZoom(float zoom);
    float MaxZoom();

	// 设置帧率
	void SetFPS(float fps);
	float FPS();

	// 时间尺预设高
	int Height();


	void setObjectName(QString name);

	// 位置点转化帧
	float Position2FramesF(int pos);
	// 像转化到位置
	float Frames2PositionF(int frames);
	// 帧数转化长度（像素）
	float Frames2DurationF(int frames);
	// 长度（像素）转化到帧数
	float Duration2FramesF(int duration);

	// 根据长度和总帧数返回对应的zoom,
	int ZoomFromWidthAndTotaleFramesF(int rulerWidth, int totalFrames);
	// 重建时间尺比例尺
	// 参数分别zoom长度、 zoom对应的像素长度、刻度数
    void RebuildRulerConvertor(QList<std::tuple<float, int, int>> zoomPixelUnit);

	// 是否开启默认的CursorShape
	void SetCursorEnable(bool enable);
	// 是否开启拖拽时间尺增加/减小zoom值
	void SetDragable(bool enable);

	void SetMouseReleaseEmitFramesChangedEnabled(bool enable);
	void SetMousePressEmitFramesChangedEnabled(bool enable);

	// 鼠标点击位置转化为帧时，是否进行四舍五入
	void SetFramesRound(bool round);
	bool GetFramesRound();

	void SetSingalClickStartEndFrameChangeEnabled(bool enable);

	//设置Pointer的形状
	void SetPointerShape(RulerPointerShape shape);

	//设置刻度和时码的颜色
	void SetDrawColor(QColor clr);

Q_SIGNALS:
	void sigSingleClickStartFramesChanged();
	void sigSingleClickEndFrameChanged();
	void StartFramesChanged();
	void EndFramesChanged();
	void FramesChanged(int frames);
	void StartZoomChanged();
	void EndZoomChanged();
	void ZoomChanged(int zoom);
	void OffsetFramesChanged(int frames);
private:
	TimelineViewRuler *m_p;
};


