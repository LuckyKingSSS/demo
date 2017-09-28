#include "stdafx.h"
#include "timelineviewruler.h"

#include "inc_CommonLib/TimelineRuler.h"
#include "inc_CommonLib/externalapi.h"
#include "inc_CommonLib/StringOperation.h"
//////////////////////////////////////////////////////////////////////////
TimelineRuler::TimelineRuler(QWidget *parent)
:QWidget(parent)
{
	setAttribute(Qt::WA_MouseTracking);
	
	m_p = new TimelineViewRuler(this);
	connect(m_p, &TimelineViewRuler::sigSingleClickStartFramesChanged, this, &TimelineRuler::sigSingleClickStartFramesChanged);
	connect(m_p, &TimelineViewRuler::sigSingleClickEndFrameChanged, this, &TimelineRuler::sigSingleClickEndFrameChanged);
	connect(m_p, &TimelineViewRuler::StartFramesChanged, this, &TimelineRuler::StartFramesChanged);
	connect(m_p, &TimelineViewRuler::EndFramesChanged, this, &TimelineRuler::EndFramesChanged);
	connect(m_p, &TimelineViewRuler::FramesChanged, this, &TimelineRuler::FramesChanged);
	connect(m_p, &TimelineViewRuler::StartZoomChanged, this, &TimelineRuler::StartZoomChanged);
	connect(m_p, &TimelineViewRuler::EndZoomChanged, this, &TimelineRuler::EndZoomChanged);
	connect(m_p, &TimelineViewRuler::ZoomChanged, this, &TimelineRuler::ZoomChanged);
	connect(m_p, &TimelineViewRuler::OffsetFramesChanged, this, &TimelineRuler::OffsetFramesChanged);

	QVBoxLayout *layout = new QVBoxLayout;
	layout->setContentsMargins(0, 0, 0, 0);
	layout->addWidget(m_p);
	setLayout(layout);
}
void TimelineRuler::setObjectName(QString name)
{
	m_p->setObjectName(name);
}
void TimelineRuler::SetCurrentFrames(int frames)
{
	m_p->SetCurrentFrames(frames);
}

int TimelineRuler::CurrentFrames()
{
	return m_p->CurrentFrames();
}

void TimelineRuler::SetOffsetFrames(int frames)
{
	m_p->SetOffsetFrames(frames);
}

int TimelineRuler::OffsetFrames()
{
	return m_p->OffsetFrames();
}

void TimelineRuler::SetOffsetPosition(int position)
{
	m_p->SetOffsetPosition(position);
}

//int TimelineRuler::OffsetPosition()
//{
//	return m_p->OffsetPosition();
//
//}
void TimelineRuler::SetTotalFrames(int frames)
{
	return m_p->SetTotalFrames(frames);
}

int TimelineRuler::TotaleFrames()
{
	return m_p->TotaleFrames();
}

int TimelineRuler::AvailableTotaleFrames()
{
	return m_p->AvailableTotaleFrames();
}

void TimelineRuler::SetZoom(float zoom)
{
	m_p->SetZoom(zoom);
}


void TimelineRuler::StartZoomAndScrollRuler()
{
	m_p->StartZoomAndScrollRuler();

}

int TimelineRuler::GetZoomAndScrollRulerOffsetPosition()
{
	return m_p->GetZoomAndScrollRulerOffsetPosition();
}

float TimelineRuler::FindZoom(int frames, int position)
{
    return m_p->FindZoom(frames, position);

}
float TimelineRuler::Zoom()
{
	return m_p->Zoom();
}

float TimelineRuler::MaxZoom()
{
	return m_p->MaxZoom();
}
void TimelineRuler::SetMaxZoom(float zoom)
{
	m_p->SetMaxZoom(zoom);
}
void TimelineRuler::SetFPS(float fps)
{
	m_p->SetFPS(fps);
}

float TimelineRuler::FPS()
{
	return m_p->FPS();
}

int TimelineRuler::Height()
{
	return m_p->Height();
}


float TimelineRuler::Position2FramesF(int pos)
{
	return m_p->GetRulerConvertor()->Position2FramesF(pos);
}

float TimelineRuler::Frames2PositionF(int frames)
{
	return m_p->GetRulerConvertor()->Frames2PositionF(frames);
}

float TimelineRuler::Frames2DurationF(int frames)
{
	return m_p->GetRulerConvertor()->Frames2DurationF(frames);
}

float TimelineRuler::Duration2FramesF(int duration)
{
	return m_p->GetRulerConvertor()->Duration2FramesF(duration);
}


int TimelineRuler::ZoomFromWidthAndTotaleFramesF(int rulerWidth, int totalFrames)
{
	return m_p->GetRulerConvertor()->ZoomFromWidthAndTotaleFramesF(rulerWidth, totalFrames);
}

void TimelineRuler::RebuildRulerConvertor(QList<std::tuple<float, int, int>> zoomPixelUnit)
{
	return m_p->GetRulerConvertor()->RebuildRulerConvertor(zoomPixelUnit);

}
void TimelineRuler::SetCursorEnable(bool enable)
{
	m_p->SetCursorEnable(enable);
}


void TimelineRuler::SetDragable(bool enable)
{
	m_p->SetDragable(enable);

}

void TimelineRuler::SetFramesRound(bool round)
{
	m_p->SetFramesRound(round);
}

bool TimelineRuler::GetFramesRound()
{
	return m_p->GetFramesRound();
}


void TimelineRuler::SetMouseReleaseEmitFramesChangedEnabled(bool enable)
{
	m_p->SetMouseReleaseEmitFramesChangedEnabled(enable);
}

void TimelineRuler::SetMousePressEmitFramesChangedEnabled(bool enable)
{
	m_p->SetMousePressEmitFramesChangedEnabled(enable);
}

void TimelineRuler::SetSingalClickStartEndFrameChangeEnabled(bool enable)
{
	m_p->SetSingalClickStartEndFrameChangeEnabled(enable);
}

void TimelineRuler::SetPointerShape(RulerPointerShape shape)
{
	m_p->SetPointerShape(shape);
}

void TimelineRuler::SetDrawColor(QColor clr)
{
	m_p->SetDrawColor(clr);
}



//////////////////////////////////////////////////////////////////////////

TimelineViewRuler::TimelineViewRuler(QWidget *parent)
	: QWidget(parent)
	, m_pressDown(false)
	, m_hintRulerHandle(false)
	, m_mouseMoveZoom(false)

	, m_cursorEnable(true)
	, m_dragable(true)
	//, m_curFramePositionWhenDragRuler(0)
	, m_framesRound(false)
	, m_mouseReleaseFramesChangedEnabled(true)
	, m_mousePressFramesChangedEnabled(true)
	, m_bSignalClickStartEndFrameChangedEnabled(false)
{

	setObjectName("TimelineView_TimelineViewRuler");
	setAttribute(Qt::WA_StyledBackground);
	setMouseTracking(true);
	m_draw = new TimelineViewRulerDrawMgr;

	m_rulerConvertor = new RulerConvertor(m_draw);

    m_dragAdjustMgr = new RulerDragAdjustManager(this);
	//setStyleSheet(GetStyles());
}

TimelineViewRuler::~TimelineViewRuler()
{
	delete m_draw;
	delete m_rulerConvertor;
}

void TimelineViewRuler::SetCurrentFrames(int frames)
{
	Draw()->SetCurrentFrames(frames);
	update();

}

int TimelineViewRuler::CurrentFrames()
{
	return Draw()->CurrentFrames();
}

void TimelineViewRuler::SetOffsetFrames(int frames)
{
	Draw()->SetOffsetFrames(frames);
	update();

	emit OffsetFramesChanged(frames);
}

int TimelineViewRuler::OffsetFrames()
{
	return Draw()->OffsetFrames();
}

void TimelineViewRuler::SetOffsetPosition(int position)
{
	int frames = Draw()->Duration2FramesF(position);
	int posTemp = Draw()->Frames2DurationF(frames);
	int offsetFrames = position - posTemp;
	Draw()->SetOffsetFrames(frames);
	Draw()->SetOffsetPosition(offsetFrames);
}

int TimelineViewRuler::OffsetPosition()
{
	return Draw()->OffsetPosition();
}

void TimelineViewRuler::SetTotalFrames(int frames)
{
	Draw()->SetTotalFrames(frames);
}

int TimelineViewRuler::TotaleFrames()
{
	return Draw()->TotaleFrames();
}

int TimelineViewRuler::AvailableTotaleFrames()
{
	return Draw()->AvailableTotaleFrames();
}

void TimelineViewRuler::SetZoom(float zoom)
{
	m_draw->SetZoom(zoom);
	update();

}

float TimelineViewRuler::Zoom()
{
	return m_draw->Zoom();
}

void TimelineViewRuler::StartZoomAndScrollRuler()
{
	//m_curFramePositionWhenDragRuler = (Draw()->Frames2PositionF(Draw()->CurrentFrames()));
    m_dragAdjustMgr->SetFramePositionWhenDragRuler(Draw()->Frames2PositionF(Draw()->CurrentFrames()));

}


int TimelineViewRuler::GetZoomAndScrollRulerOffsetPosition()
{

    return m_dragAdjustMgr->GetZoomAndScrollRulerOffsetPosition(Draw());
}

float TimelineViewRuler::FindZoom(int frame, int pos)
{
    double ruler;

    ruler = 1.0 * pos / (1.0 * frame / 5);
    if (ruler > 150)
        return 0;
    if ((ruler >= 30) && (ruler <= 150))
        return (150.0f - ruler);
    ruler = pos / (1.0 * frame / (5 * 5));
    if ((ruler >= 30) && (ruler < 150))
        return (270.0f - ruler);
    ruler = pos / (1.0 * frame / (5 * 5 * 5));
    if ((ruler >= 30) && (ruler < 150))
        return (390.0f - ruler);
    ruler = pos / (1.0 * frame / (5 * 5 * 5 * 6));
    if ((ruler >= 30) && (ruler < 180))
        return (540.0f - ruler);
    ruler = pos / (1.0 * frame / (5 * 5 * 5 * 6 * 5));
    if ((ruler >= 30) && (ruler < 150))
        return (660.0f - ruler);
    ruler = pos / (1.0 * frame / (5 * 5 * 5 * 6 * 5 * 6));
    if ((ruler >= 30) && (ruler < 180))
        return (810.0f - ruler);
    if (ruler < 30 && ruler >= 0)
        return 780.0f;
    return Zoom();
}

float TimelineViewRuler::MaxZoom()
{
	return m_draw->MaxZoom();
}

void TimelineViewRuler::SetMaxZoom(float zoom)
{
	return m_draw->SetMaxZoom(zoom);
}

void TimelineViewRuler::SetFPS(float fps)
{
	Draw()->SetFPS(fps);
}

float TimelineViewRuler::FPS()
{
	return Draw()->FPS();
}

int TimelineViewRuler::ScaleUnit()
{
	return Draw()->ScaleUnit();
}

int TimelineViewRuler::Height()
{
	return 30;
}

void TimelineViewRuler::SetCursorEnable(bool enable)
{
	m_cursorEnable = enable;
}

void TimelineViewRuler::SetDragable(bool enable)
{
	m_dragable = enable;
}

void TimelineViewRuler::SetFramesRound(bool round)
{
	m_framesRound = round;
}

bool TimelineViewRuler::GetFramesRound()
{
	return m_framesRound;
}

void TimelineViewRuler::SetMouseReleaseEmitFramesChangedEnabled(bool enable)
{
	m_mouseReleaseFramesChangedEnabled = enable;
}
void TimelineViewRuler::SetMousePressEmitFramesChangedEnabled(bool enable)
{
	m_mousePressFramesChangedEnabled = enable;
}

RulerConvertor* TimelineViewRuler::GetRulerConvertor()
{
	return m_rulerConvertor;
}

void TimelineViewRuler::StartRulerZoomChanged()
{
	//m_curFramePositionWhenDragRuler = Draw()->Frames2PositionF(Draw()->CurrentFrames());
    m_dragAdjustMgr->SetFramePositionWhenDragRuler(Draw()->Frames2PositionF(Draw()->CurrentFrames()));

}




void TimelineViewRuler::SetSingalClickStartEndFrameChangeEnabled(bool enable)
{
	m_bSignalClickStartEndFrameChangedEnabled = enable;
}

void TimelineViewRuler::SetPointerShape(RulerPointerShape shape)
{
	m_draw->SetPointerShape(shape);
}

void TimelineViewRuler::SetDrawColor(QColor clr)
{
	m_draw->SetDrawColor(clr);
}

void TimelineViewRuler::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	Draw()->Draw(&painter, rect(), fontMetrics());

}


void TimelineViewRuler::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_pressDown = true;
		m_pressPoint = event->pos();
		auto curFramePositionWhenDragRuler = Draw()->Frames2PositionF(Draw()->CurrentFrames());
        m_dragAdjustMgr->AdjustFramePositionWhenDragRuler(curFramePositionWhenDragRuler, Draw());

		auto findZoomPressFrame = Draw()->Position2FramesF(event->pos().x());
        m_dragAdjustMgr->AdjustFindZoomPressFrames(findZoomPressFrame, CurrentFrames(), event->x());

        m_hintRulerHandle = Draw()->HintRulerHandle(event->pos());
		if (m_hintRulerHandle)
			emit StartFramesChanged();
		
		if (m_mousePressFramesChangedEnabled)
			RulerHandlePositionChanged(event->pos());

	}
	
	//QWidget::mousePressEvent(event);
}

void TimelineViewRuler::mouseMoveEvent(QMouseEvent *event)
{
	if (m_pressDown)
	{
		if (m_hintRulerHandle)
		{
			RulerHandlePositionChanged(event->pos());
		}
		else if (m_dragable)
		{
			m_mouseMoveZoom = true;
			if (m_bStartZoomChanged)
			{
				m_bStartZoomChanged = false;
				emit StartZoomChanged();
			}

            int curPosition = 0;
            int findZoomPressFrames = 0;
            m_dragAdjustMgr->GetFindZoomPressFrameCurrentPosition(event->x(), findZoomPressFrames, curPosition);
            int zoom = FindZoom(qMax(0, findZoomPressFrames - OffsetFrames()), curPosition);
            SetZoom(zoom);
            emit ZoomChanged(zoom);
		}
	}

	if (m_cursorEnable)
	{
		bool hintRulerHandle = Draw()->HintRulerHandle(event->pos());
		if (hintRulerHandle || m_hintRulerHandle)
			setCursor(Qt::ArrowCursor);
		else
			setCursor(Qt::SizeHorCursor);
	}


	//QWidget::mouseMoveEvent(event);
}

void TimelineViewRuler::mouseReleaseEvent(QMouseEvent *event)
{
	if (m_pressDown)
	{
		if (m_hintRulerHandle)
		{
			// 时间尺拖拽
			emit EndFramesChanged();
		}
		else
		{
			if (!m_mouseMoveZoom)
			{
				if (/*m_hintRulerHandle ||*/ m_mouseReleaseFramesChangedEnabled)
				{
					// 非拖拽时间尺
					if (m_bSignalClickStartEndFrameChangedEnabled) emit sigSingleClickStartFramesChanged();
					RulerHandlePositionChanged(event->pos());
					if (m_bSignalClickStartEndFrameChangedEnabled) emit sigSingleClickEndFrameChanged();
				}
			}
			else
			{
				emit EndZoomChanged();
			}

		}
		m_pressDown = false;
		m_mouseMoveZoom = false;
		m_hintRulerHandle = false;

		m_bStartZoomChanged = true;
	}

	//QWidget::mouseReleaseEvent(event);
}

void TimelineViewRuler::resizeEvent(QResizeEvent *event)
{
	m_draw->SetDrawRect(rect());
}

void TimelineViewRuler::RulerHandlePositionChanged(QPoint p)
{
	int frames = qMax<int>(0, Draw()->Position2FramesF(p.x()) + (GetFramesRound() ? 0.5f : 0.0f));
	//int frames = Draw()->Position2FramesF(qBound(0, p.x(), width() + Draw()->OffsetPosition())) + (GetFramesRound() ? 0.5f : 0.0f);
	//if (Draw()->TotaleFrames() >= 0)
	//	frames = qMin(Draw()->TotaleFrames(), frames);
	Draw()->SetCurrentFrames(frames);
	update();
	emit FramesChanged(frames);
}

TimelineViewRulerDraw* TimelineViewRuler::Draw()
{
	return m_draw->Draw();
}


//////////////////////////////////////////////////////////////////////////
TimelineViewRulerDrawMgr::TimelineViewRulerDrawMgr()
: m_curDraw(nullptr)
{

	/*
	缩放档	尺度（单位：像素）	一大刻度刻度倍数（一刻度现在规定为5帧）
	0~120	150~30	1
	121~240	149~30	1x5
	241~360	149~30	1x5x5
	361~510	179~30	1x5x5x6
	511~630	149~30	1x5x5x6x5
	631~780	179~30	1x5x5x6x5x6

	*/

	//CreateDraw(0, 120, 150, 1);
	//CreateDraw(121, 240, 149, 5);
	//CreateDraw(241, 360, 149, 5 * 5);
	//CreateDraw(361, 510, 179, 5 * 5 * 6);
	//CreateDraw(511, 630, 149, 5 * 5 * 6 * 5);
	//CreateDraw(631, m_maxZoom, 179, 5 * 5 * 6 * 5 * 6);

    QList<std::tuple<float, int, int>> converParam;

	converParam.push_back(std::make_tuple(120, 150, 1));
	converParam.push_back(std::make_tuple(119, 149, 5));
	converParam.push_back(std::make_tuple(119, 149, 5 * 5));
	converParam.push_back(std::make_tuple(149, 179, 5 * 5 * 6));
	converParam.push_back(std::make_tuple(119, 149, 5 * 5 * 6 * 5));
	converParam.push_back(std::make_tuple(149, 179, 5 * 5 * 6 * 5 * 6));


	RebuildRulerConvertor(converParam);
	m_maxZoom = 780;

	m_curDraw = CurrentDrawFromZoom(435);
}

TimelineViewRulerDrawMgr::~TimelineViewRulerDrawMgr()
{
	Clear();
}
TimelineViewRulerDraw* TimelineViewRulerDrawMgr::CreateDraw(float minZoom, float maxZoom, int rulerUnit, int scaleUnit)
{
	auto draw = new TimelineViewRulerDraw(rulerUnit, scaleUnit);
	draw->SetZoomRange(minZoom, maxZoom);
	m_draws.push_back(draw);
	return draw;
}


void TimelineViewRulerDrawMgr::Clear()
{
	qDeleteAll(m_draws);
	m_draws.clear();
	m_curDraw = nullptr;
	m_maxZoom = 0;
}

void TimelineViewRulerDrawMgr::SetZoom(float zoom)
{
	m_curDraw = CurrentDrawFromZoom(zoom);
	m_curDraw->SetZoom(zoom);
}

float TimelineViewRulerDrawMgr::Zoom()
{
	return m_curDraw->Zoom();
}

float TimelineViewRulerDrawMgr::MaxZoom()
{
	return m_maxZoom;
}

void TimelineViewRulerDrawMgr::SetMaxZoom(float zoom)
{
	m_maxZoom = zoom;
}

int TimelineViewRulerDrawMgr::ZoomFromWidthAndTotaleFramesF(int rulerWidth, int totalFrames)
{
	// 根据时间尺长度和总帧数，计算出最大zoom, 
	TimelineViewRulerDraw *temp = nullptr;
	for (int i = 0; i < m_draws.size(); ++i)
	{
		auto draw = m_draws[i];
		float zoomTemp = draw->ZoomFromWidthAndTotaleFramesF(rulerWidth, totalFrames);
		int zoom = zoomTemp +1.0f;

		auto tupleValue = draw->ZoomRange();
		if (zoom >= std::get<0>(tupleValue) && zoom <= std::get<1>(tupleValue))
		{
			// 改变scaleUnit，校正zoom的小数大于0.7即可进位，解决空隙过大或者出现滚动条问题，
			int maxRulerUnit = draw->RulerUnit();
			int scaleUnit = draw->ScaleUnit();
			for (int scaleTemp = scaleUnit; scaleTemp < 10000000; ++scaleTemp)
			{
				float newZoom = (maxRulerUnit - ((1.0 * (rulerWidth * 5) * scaleTemp) / totalFrames)) + std::get<0>(draw->ZoomRange());
				if (newZoom < 0)
				{
					break;
				}
				float temp = newZoom - (int)newZoom;
				// zoom精度大于0.7则可使用此newZoom
				if (temp > 0.7)
				{
					bool canAdjust = true;
					if (i > 0)
					{
						// ZOOM值小于前面draw的range，则不进行校正
						auto preDrawRange = m_draws[i - 1]->ZoomRange();
						if (std::get<1>(preDrawRange) >= newZoom)
						{
							canAdjust = false;

						}
					}

					if (canAdjust)
					{
						draw->SetScaleUnit(scaleTemp);
						zoom = newZoom + 1.0f;
					}
					break;
				}
			}

			return zoom;
		}
	}
	//Q_ASSERT(false);

	return 0;


	//TimelineViewRulerDraw *temp = nullptr;
	//for (auto draw : m_draws)
	//{
	//	float zoomTemp = draw->ZoomFromWidthAndTotaleFramesF(rulerWidth, totalFrames);
	//	int zoom = zoomTemp + 1.0f;
	//	
	//	auto tupleValue = draw->ZoomRange();
	//	if (zoom >= std::get<0>(tupleValue) && zoom <= std::get<1>(tupleValue))
	//	{
	//		return zoom;
	//	}
	//}
	////Q_ASSERT(false);

	//return 0;

}

void TimelineViewRulerDrawMgr::RebuildRulerConvertor(QList<std::tuple<float, int, int>> zoomAndPixels)
{
	Clear();
    float offsetZoom = 0;
	for (auto p : zoomAndPixels)
	{
        float zoom = std::get<0>(p);
		int pixel = std::get<1>(p);
		int scaleUnit = std::get<2>(p);
		auto draw = CreateDraw(offsetZoom, offsetZoom + zoom, pixel, scaleUnit);
		draw->SetDrawRect(m_rect);
		offsetZoom += zoom + 1;
	}
	SetZoom(offsetZoom - 1);
}

void TimelineViewRulerDrawMgr::SetDrawRect(QRect rect)
{
	m_rect = rect;
	m_curDraw->SetDrawRect(rect);
}

TimelineViewRulerDraw* TimelineViewRulerDrawMgr::CurrentDrawFromZoom(float zoom)
{

	TimelineViewRulerDraw * curDraw = nullptr;
	for (auto draw : m_draws)
	{
		auto tupleValue = draw->ZoomRange();
		if (zoom >= std::get<0>(tupleValue) && zoom <= std::get<1>(tupleValue))
		{
			curDraw = draw;
			break;
		}
	}
	Q_ASSERT(curDraw);
	if (m_curDraw)
		curDraw->Copy(m_curDraw);
	return curDraw;
}

TimelineViewRulerDraw* TimelineViewRulerDrawMgr::Draw()
{
	return m_curDraw;
}

void TimelineViewRulerDrawMgr::SetPointerShape(RulerPointerShape shape)
{
	for (auto draw : m_draws)
	{
		draw->SetPointerShape(shape);
	}
}

void TimelineViewRulerDrawMgr::SetDrawColor(QColor clr)
{
	for (auto draw : m_draws)
	{
		draw->SetDrawColor(clr);
	}
}

//////////////////////////////////////////////////////////////////////////
TimelineViewRulerDraw::TimelineViewRulerDraw(int rulerUnit, int scaleUnit)
: m_scaleUnit(scaleUnit)
, m_rulerUnit(rulerUnit)
, m_zoom(0.0f)
, m_fps(25.0f)
, m_oneBigScaleFrameCounts(5)
, m_curFrames(0)
, m_offsetFrames(0)
, m_framesTextUnit(1)
, m_totalFrames(0)
, m_pixelOffsetPosition(0)
, m_drawColor(0,0,0)
{
	DefaultPointerShape();
}


void TimelineViewRulerDraw::SetFPS(float fps)
{
	m_fps = fps;
}

float TimelineViewRulerDraw::FPS()
{
	return m_fps;
}

void TimelineViewRulerDraw::SetZoom(float zoom)
{
	m_zoom = zoom - std::get<0>(m_zoomRange);
	Q_ASSERT(m_zoom >= 0);


	QString text = gFrame2String(0, m_fps);
	QFontMetrics metrics = qApp->fontMetrics();
	int textWidth = metrics.width(text);
	int textHeight = 15;
	m_framesTextUnit = 1;
	int bigScaleWidthTemp = m_rulerUnit - m_zoom;;
	while (bigScaleWidthTemp >0 && bigScaleWidthTemp < textWidth * 1.5)
	{
		m_framesTextUnit *= 2;
		bigScaleWidthTemp *= 2;
	}

};

float TimelineViewRulerDraw::Zoom()
{
	return m_zoom + std::get<0>(m_zoomRange);
}

int TimelineViewRulerDraw::ScaleUnit()
{
	return m_scaleUnit;
}

void TimelineViewRulerDraw::SetScaleUnit(int scaleUnit)
{
	m_scaleUnit = scaleUnit;
}

int TimelineViewRulerDraw::RulerUnit()
{
	return m_rulerUnit;
}

void TimelineViewRulerDraw::SetZoomRange(float minv, float maxv)
{
	Q_ASSERT(minv >= 0);
	Q_ASSERT(maxv >= 0);
	m_zoomRange = std::make_tuple(minv, maxv);
};

std::tuple<float, float> TimelineViewRulerDraw::ZoomRange()
{
	return m_zoomRange;
}
void TimelineViewRulerDraw::Draw(QPainter *painter, QRect rect, QFontMetrics metrics)
{
	rect.adjust(0, 0, 0, -2);
	m_rect = rect;
	
	const int oneScaleHeight = 4;
	const int fiveScaleHeight = 10;
	
	int bigScaleWidth = m_rulerUnit - m_zoom;
	float left = 0;


	int bigOffsetFrames = m_offsetFrames / (m_oneBigScaleFrameCounts * m_scaleUnit);
	// 不够一大刻度的偏移量
	float offsetPosition = m_offsetFrames % (m_oneBigScaleFrameCounts * m_scaleUnit) * (1.0 * bigScaleWidth / m_oneBigScaleFrameCounts / m_scaleUnit);
	offsetPosition += m_pixelOffsetPosition;
	painter->save();
	painter->setPen(m_drawColor);
	QFont fnt = painter->font();
	fnt.setPixelSize(13);
	painter->setFont(fnt);
	for (int i = 0; i < rect.width() / bigScaleWidth + 2; ++i)
	{
		for (int j = 0; j < m_oneBigScaleFrameCounts; ++j)
		{
			// 一小刻度
			left = Frames2PositionForBigScale((i * m_oneBigScaleFrameCounts) * m_scaleUnit + j * m_scaleUnit);
			left -= offsetPosition;
			painter->drawLine(left, rect.height() - oneScaleHeight, left, rect.height());
			
		}


		left = Frames2PositionForBigScale((i * m_oneBigScaleFrameCounts) * m_scaleUnit);
		left -= offsetPosition;

		// 画时码
		if ((i + bigOffsetFrames) % m_framesTextUnit == 0)
		{
			QString text = gFrame2String((i * m_oneBigScaleFrameCounts + bigOffsetFrames * m_oneBigScaleFrameCounts) * m_scaleUnit, m_fps);
			int textWidth = StringOperation::GetTextTrueLenght(painter->font(), text);// metrics.width(text);
			int textHeight = 15;
			painter->drawText(QRectF(left + 2, rect.height() - fiveScaleHeight - textHeight, textWidth, textHeight), Qt::AlignVCenter | Qt::AlignLeft, text);
		}
		// 5帧一大刻度
		painter->drawLine(left, rect.height() - fiveScaleHeight, left, rect.height());
	}

	// 底线
	painter->drawLine(0, rect.height(), rect.width(), rect.height());
	painter->restore();
	// 标尺
	QPen pen = painter->pen();
	pen.setColor(m_pointerShape.color);
	pen.setWidth(1);
	painter->save();
	painter->setPen(pen);
	painter->setBrush(m_pointerShape.color);
	painter->setRenderHint(QPainter::Antialiasing);
	painter->drawPolygon(GetRulerHandle());
	painter->setRenderHint(QPainter::Antialiasing,false);
	if (m_pointerShape.aligntop)
	{
		pen = painter->pen();
		pen.setWidth(m_pointerShape.linewidth);
		painter->setPen(pen);
		float x = Frames2PositionF(m_curFrames);
		painter->drawLine(QPointF(x,m_pointerShape.height), QPointF(x,m_rect.height()));
	}
	if (m_pointerShape.showpostext)
	{
		pen = painter->pen();
		pen.setColor(m_pointerShape.postextcolor);
		painter->setPen(pen);
		fnt = painter->font();
		fnt.setPixelSize(13);
		painter->setFont(fnt);
		
		QString text = gFrame2String(m_curFrames, m_fps);
		float x = Frames2PositionF(m_curFrames) + (m_pointerShape.width / 2) + 3;
		float y = 15;
		painter->drawText(QPointF(x, y), text);
	}
	painter->restore();
}

void TimelineViewRulerDraw::SetDrawRect(QRect rect)
{
	m_rect = rect;
}



QRect TimelineViewRulerDraw::GetDrawRect()
{
	return m_rect;
}

void TimelineViewRulerDraw::SetPointerShape(RulerPointerShape shape)
{
	m_pointerShape = shape;
}

void TimelineViewRulerDraw::SetDrawColor(QColor clr)
{
	m_drawColor = clr;
}

void TimelineViewRulerDraw::SetCurrentFrames(int frames)
{
	m_curFrames = frames;
}

int TimelineViewRulerDraw::Position2Frames(int pos)
{
	return (((pos * m_oneBigScaleFrameCounts) / (m_rulerUnit - m_zoom)) + m_offsetFrames / m_scaleUnit) * m_scaleUnit;
}

int TimelineViewRulerDraw::Frames2Position(int frames)
{
	return (frames / m_scaleUnit - m_offsetFrames / m_scaleUnit)  * 1.0 * (m_rulerUnit - m_zoom) / m_oneBigScaleFrameCounts;
}

int TimelineViewRulerDraw::Frames2PositionForBigScale(int frames)
{
	return (frames / m_scaleUnit) * 1.0 * (m_rulerUnit - m_zoom) / m_oneBigScaleFrameCounts;

}

float TimelineViewRulerDraw::Position2FramesF(int pos)
{
	pos += OffsetPosition();
	return ((1.0*(pos * m_oneBigScaleFrameCounts) / (m_rulerUnit - m_zoom)) + 1.0 * m_offsetFrames / m_scaleUnit) * m_scaleUnit ;
}

float TimelineViewRulerDraw::Frames2PositionF(int frames)
{
	return (1.0*frames / m_scaleUnit - 1.0*m_offsetFrames / m_scaleUnit)  * 1.0 * (m_rulerUnit - m_zoom) / m_oneBigScaleFrameCounts - OffsetPosition();
}

float TimelineViewRulerDraw::Frames2DurationF(int frames)
{
	return (1.0*frames / m_scaleUnit)  * 1.0 * (m_rulerUnit - m_zoom) / m_oneBigScaleFrameCounts;
}

float TimelineViewRulerDraw::Duration2FramesF(int duration)
{
	return (1.0 * (duration * m_oneBigScaleFrameCounts) / (m_rulerUnit - m_zoom)) * m_scaleUnit;
}


int TimelineViewRulerDraw::Duration2MaxFramesF(int duration)
{
	return (1.0 * (duration * m_oneBigScaleFrameCounts) / (m_rulerUnit - std::get<1>(m_zoomRange))) * m_scaleUnit;
}

float TimelineViewRulerDraw::ZoomFromWidthAndTotaleFramesF(int rulerWidth, int totalFrames)
{
	return (m_rulerUnit - ((1.0 * (rulerWidth * m_oneBigScaleFrameCounts) * m_scaleUnit) / totalFrames)) + std::get<0>(m_zoomRange);
}

int TimelineViewRulerDraw::OffsetFramesFromPosition(int curFrame, int position)
{
	// 当前帧减position长度表示的帧等于偏移帧
    return curFrame - ((1.0 * position * m_oneBigScaleFrameCounts / (m_rulerUnit - m_zoom)) *m_scaleUnit);
}

int TimelineViewRulerDraw::OffsetPosition()
{
	return m_pixelOffsetPosition;
}

void TimelineViewRulerDraw::Copy(TimelineViewRulerDraw *other)
{
	m_curFrames = other->CurrentFrames();
	m_totalFrames = other->TotaleFrames();
	m_offsetFrames = other->OffsetFrames();
	m_fps = other->FPS();
	m_framesTextUnit = other->m_framesTextUnit;
	m_rect = other->m_rect;
	m_pixelOffsetPosition = other->OffsetPosition();
}

int TimelineViewRulerDraw::CurrentFrames()
{
	return m_curFrames;
}

void TimelineViewRulerDraw::SetTotalFrames(int frames)
{
	m_totalFrames = frames;
}

int TimelineViewRulerDraw::TotaleFrames()
{
	return m_totalFrames;
}

int TimelineViewRulerDraw::AvailableTotaleFrames()
{
	return 1.0 * m_rect.width() / (m_rulerUnit - m_zoom) * m_oneBigScaleFrameCounts * m_scaleUnit + m_offsetFrames;
}

void TimelineViewRulerDraw::SetOffsetFrames(int frames)
{
	m_offsetFrames = frames;
}

int TimelineViewRulerDraw::OffsetFrames()
{
	return m_offsetFrames;
}

void TimelineViewRulerDraw::SetOffsetPosition(int position)
{
	m_pixelOffsetPosition = position;
}

bool TimelineViewRulerDraw::HintRulerHandle(QPoint p)
{
	return GetRulerHandle().containsPoint(p, Qt::OddEvenFill);
}

int TimelineViewRulerDraw::OffsetFramesFromZoomChange(int offsetPosition)
{
	int offsetFrame = ((1.0 * m_curFrames / m_scaleUnit) - ((1.0 * offsetPosition * m_oneBigScaleFrameCounts) / (m_rulerUnit - m_zoom))) * m_scaleUnit;
	return offsetFrame;
}

QPolygonF TimelineViewRulerDraw::GetRulerHandle()
{
	float left = Frames2PositionF(m_curFrames);
	float transX = left;
	float transY = m_pointerShape.aligntop ? 0 : (m_rect.height() - m_pointerShape.height);
	return m_pointerShape.shape.translated(transX, transY);
}

void TimelineViewRulerDraw::DefaultPointerShape()
{
	m_pointerShape.aligntop = false;
	m_pointerShape.height = 16;
	m_pointerShape.width = 8;
	m_pointerShape.color = Qt::black;
	m_pointerShape.linewidth = 2;
	QVector<QPointF> pts;
	pts << QPointF(-4, 0) << QPointF(4, 0) << QPointF(4, 16) << QPointF(-4, 16);
	m_pointerShape.shape = QPolygonF(pts);
	m_pointerShape.showpostext = false;
}

//////////////////////////////////////////////////////////////////////////
RulerConvertor::RulerConvertor(TimelineViewRulerDrawMgr *draw)
:m_draw(draw)
{

}

float RulerConvertor::Position2FramesF(int pos)
{
	return Draw()->Position2FramesF(pos);
}

float RulerConvertor::Frames2PositionF(int frames)
{
	return Draw()->Frames2PositionF(frames);
}

int RulerConvertor::Position2Frames(int pos)
{
	return Draw()->Position2Frames(pos);
}

int RulerConvertor::Frames2Position(int frames)
{
	return Draw()->Frames2Position(frames);
}

float RulerConvertor::Frames2DurationF(int frames)
{
	return Draw()->Frames2DurationF(frames);
}

float RulerConvertor::Duration2FramesF(int duration)
{
	return Draw()->Duration2FramesF(duration);
}

int RulerConvertor::ZoomFromWidthAndTotaleFramesF(int rulerWidth, int totalFrames)
{
	return m_draw->ZoomFromWidthAndTotaleFramesF(rulerWidth, totalFrames);
}

void RulerConvertor::RebuildRulerConvertor(QList<std::tuple<float, int, int>> zoomPixelUnit)
{
	m_draw->RebuildRulerConvertor(zoomPixelUnit);
}

TimelineViewRulerDraw* RulerConvertor::Draw()
{
	return m_draw->Draw();
}

//////////////////////////////////////////////////////////////////////////
RulerDragAdjustManager::RulerDragAdjustManager(QObject *parent)
:QObject(parent)
{
    m_virtualCurrentFrames = 0;
    m_WhenDragRulerOffset = 0;
}

int RulerDragAdjustManager::GetZoomAndScrollRulerOffsetPosition(TimelineViewRulerDraw *draw)
{
    if (!NeedScrollBar())
        return draw->OffsetFrames();

    // 根据当前位置计算出偏移量（帧）
    int offsetFrames = draw->OffsetFramesFromPosition(VirtualCurrentFrames(), m_curFramePositionWhenDragRuler);
    // 计算不满一帧的偏移量（像素）
    int newOffsetPosition = m_curFramePositionWhenDragRuler - draw->Frames2DurationF(draw->CurrentFrames() - offsetFrames) - draw->Frames2DurationF(VirtualOffset());
    // 两个相加得出最终偏移量（像素）
    return draw->Frames2DurationF(offsetFrames) + qAbs(newOffsetPosition);
}

void RulerDragAdjustManager::AdjustFramePositionWhenDragRuler(int position, TimelineViewRulerDraw *draw)
{
    m_virtualCurrentFrames = draw->CurrentFrames();
    m_WhenDragRulerOffset = 0;
    if (position < 0)
    {
        // 当前帧不在显示时间线上, 模拟当前帧显示在draw->OffsetFrames() + 10处
        m_virtualCurrentFrames = draw->OffsetFrames() + 10;
        position = draw->Frames2PositionF(m_virtualCurrentFrames);
        m_WhenDragRulerOffset = m_virtualCurrentFrames - draw->CurrentFrames();
    }
    m_curFramePositionWhenDragRuler = position;
}

void RulerDragAdjustManager::AdjustFindZoomPressFrames(int fileZoomFrames, int curFrames, int pressX)
{
    m_FindZoomPressFrames = fileZoomFrames;
    m_leftOffset = 0;
    bool m_FindZoomPressFrameOnCurrentFrameLeft = fileZoomFrames < curFrames;
    if (m_FindZoomPressFrameOnCurrentFrameLeft)
    {
        // 拖拽时间尺时，pressdown在当前帧左边，解决往右拖时间线zoom会调到最大问题
        m_FindZoomPressFrames = curFrames + (curFrames - fileZoomFrames);
        m_leftOffset = 2 * (m_curFramePositionWhenDragRuler - pressX);
    }
}

void RulerDragAdjustManager::GetFindZoomPressFrameCurrentPosition(int posX, int &findZoomPosition, int &offset)
{
    findZoomPosition = m_FindZoomPressFrames;
    offset = m_leftOffset + posX;
}



void RulerDragAdjustManager::SetFramePositionWhenDragRuler(int position)
{
    m_curFramePositionWhenDragRuler = position;
}

bool RulerDragAdjustManager::NeedScrollBar()
{
    return VirtualOffset() >= 0;
}

int RulerDragAdjustManager::VirtualCurrentFrames()
{
    return m_virtualCurrentFrames;
}

int RulerDragAdjustManager::VirtualOffset()
{
    return m_WhenDragRulerOffset;
}

