#include "stdafx.h"
#include "TVDraw.h"
#include "TimelineManager.h"
#include "TVTrackView.h"
#include "TVControl.h"
#include "inc_FSCommonlib/FSNLETimeline.h"
#include "AudioWaveMgr.h"
#include "inc_CommonLib/StringOperation.h"

TVTrackWidgetDraw::TVTrackWidgetDraw(QObject *parent)
	: QObject(parent)
{
	m_pTimelineMgr = TimelineManager::GetInstance();
}

void TVTrackWidgetDraw::SetTrackWidget(TrackWidget *pWidget)
{
	m_pTrackWidget = pWidget;
	m_pTrackData = pWidget->GetTrackData();
}

void TVTrackWidgetDraw::SetRuler(TimelineRuler *pRuler)
{
	m_pRuler = pRuler;
}

void TVTrackWidgetDraw::Draw(QPainter *painter, QRect rc)
{
	if (!TimelineManager::GetInstance()->GetTracks().contains(m_pTrackData))
	{
		return;
	}

	QColor clrHeader(49, 60, 72);
	QColor clrTrack(49, 60, 72);
	QColor clrLine(39, 49, 60);
	QColor clrText(85, 200, 193);

	painter->save();
	painter->translate(TRACK_WIDGET_HEADER_WIDTH, 0);

	//background
	QRect trackRect(0, 0, rc.width() - TRACK_WIDGET_HEADER_WIDTH, rc.height());
	painter->fillRect(trackRect, clrTrack);
	int t = 0;//trackRect.height();
	painter->setPen(clrLine);
	painter->drawLine(0, 0, trackRect.width(), 0);


	//clips
	int l, w, h;
	QColor clrClipBk(78, 104, 115);
	QColor clrSelectedBorder(97, 222, 208);
	QColor clrUnselectedBorder(0,0,0,120);
	Clips clips = m_pTrackWidget->GetClips();
	Clips dragClips = m_pTrackWidget->GetDragClips();

	for (auto clip : clips)
	{
		if (!clip) continue;
		Clip* c = clip;
		l = m_pRuler->Frames2PositionF(c->GetStartAtTrack());
		t = 0;
		w = m_pRuler->Frames2DurationF(c->GetLenAtTrack());
		w = qMax(2, w);
		h = rc.height() - 1;

		QRect rcClip(l, t, w, h);
		//painter->fillRect(rcClip.adjusted(-1,0,0,0), clrClipBk);

		QPen pen;
		pen.setWidth(1);
		pen.setColor(clrUnselectedBorder);
		painter->setPen(pen);
		painter->setBrush(c->GetNLETimeline()->GetBackgroundColor());
		painter->drawRect(rcClip.adjusted(-1, 0, 0, 0));


		auto timeline = c->GetNLETimeline();
		auto type = timeline->GetType();
		//audio wave
		if (type == TimelineType_Audio || type == TimelineType_VideoAudio)
		{
			//auto guid = timeline->IsRecordTimeline() ? timeline->GetRecordAncestralGuidString() : timeline->GetAncestralGuidString();

			auto leftFrame = m_pRuler->Position2FramesF(0);
			auto rightFrame = m_pRuler->Position2FramesF(m_pTrackWidget->width() - TRACK_WIDGET_HEADER_WIDTH);
			auto startFrame = c->GetStartAtTrack();
			auto endFrame = startFrame + c->GetLenAtTrack() - 1;

			if (startFrame < rightFrame && endFrame > leftFrame)
			{
				auto waveLeftPos = m_pRuler->Frames2PositionF(qMax((int)leftFrame, startFrame));
				auto waveRightPos = m_pRuler->Frames2PositionF(qMin((int)rightFrame, endFrame) + 1);
				auto pixelWidth = waveRightPos - waveLeftPos + 1;
				auto pixelHeight = rcClip.height() - 1;
				QRect rcWave((int)waveLeftPos, 0, pixelWidth, pixelHeight);
				auto startAtSrc = c->GetStartAtSrc();
				if (startFrame < leftFrame)
					startAtSrc += (leftFrame - startFrame);
				auto endAtSrc = startAtSrc + m_pRuler->Duration2FramesF(pixelWidth) - 1;

				QPixmap pixWave = AudioWaveMgr::GetInstance()->GetPixmap(timeline, pixelWidth, pixelHeight, startAtSrc, endAtSrc, 1);

				if (!pixWave.isNull())
				{
					painter->drawPixmap(rcWave, pixWave);
				}
					
			}
		}

		QRect rcTitle = QRect(rcClip.left() + 5, rcClip.top(), rcClip.width() - 5, rcClip.height());
		if (type == TimelineType_VideoAudio ||
			type == TimelineType_Video ||
			type == TimelineType_Image ||
			type == TimelineType_Annotaions_Caption ||
			type == TimelineType_Annotaions_Overlay ||
			type == TimelineType_Annotaions_Element)
		{
			//thumbnail
			QPixmap thumbnail = timeline->GetThumbnail();
			QRect rcThumbnail = QRect(rcClip.left() + 2, rcClip.top() + 2, CLIP_THUMBNAIL_WIDTH, rcClip.height() - 4);
			thumbnail = thumbnail.scaled(rcThumbnail.width(),
				rcThumbnail.height(),
				Qt::KeepAspectRatio,
				(type == TimelineType_Annotaions_Caption 
				|| type == TimelineType_Annotaions_Overlay 
				|| type == TimelineType_Annotaions_Element) ? Qt::SmoothTransformation : Qt::FastTransformation);

			QPixmap px(QSize(rcThumbnail.width(), rcThumbnail.height()));
			px.fill((type == TimelineType_Annotaions_Caption 
				|| type == TimelineType_Annotaions_Overlay 
				|| type == TimelineType_Annotaions_Element) ? Qt::transparent : Qt::black);
			QPainter pter(&px);
			int l = (px.width() - thumbnail.width()) / 2;
			int t = (px.height() - thumbnail.height()) / 2;
			pter.drawPixmap(QRect(l,t,thumbnail.width(),thumbnail.height()),thumbnail);

			int ThumbnailW = qMin(CLIP_THUMBNAIL_WIDTH, rcClip.width() - 2);
			QRect rcDrawThumbnail = QRect(rcClip.left() + 2, rcClip.top() + 2, ThumbnailW, rcClip.height() - 4);
			painter->drawPixmap(rcDrawThumbnail, px, QRect(0, 0, rcDrawThumbnail.width(), rcDrawThumbnail.height()));
			rcTitle = QRect(rcThumbnail.right() + 5, rcThumbnail.top(), rcClip.right() - (rcThumbnail.right() + 5), rcThumbnail.height());
		}

		//title
		//qDebug() << rcTitle << " isnull:" << rcTitle.isNull();
		if (rcTitle.width() > 0)
		{
			QTextOption op(Qt::AlignVCenter);
			painter->setPen(Qt::white);
			QFont fnt;
			fnt.setFamily("lato");
			fnt.setPixelSize(14);
			fnt.setBold(true);
			painter->setFont(fnt);
			QString name = StringOperation::GetElidedString(fnt, 200, c->GetNLETimeline()->GetDisplayName());

			auto timeline = c->GetNLETimeline();
			if (timeline->IsDesktopVideo() || timeline->IsGameVideo())
			{
				name += " (" + tr("Screen") + ")";
			}
			else if (timeline->IsCameraVideo())
			{
				name += " (" + tr("Camera") + ")";
			}
			else if (timeline->IsSystemAudio())
			{
				name += " (" + tr("Computer Audio") + ")";
			}
			else if (timeline->IsMicrophoneAudio())
			{
				name += " (" + tr("Microphone") + ")";
			}
			painter->drawText(rcTitle, Qt::AlignVCenter | Qt::TextSingleLine, name);
		}

		//marker
		painter->setRenderHint(QPainter::Antialiasing);
		for (auto mk : c->GetNLETimeline()->GetMarkers())
		{
			int mkTL = mk + (c->GetNLETimeline()->GetPosition() - c->GetNLETimeline()->GetTrimin());

			auto pos = m_pRuler->Frames2PositionF(mkTL);
			m_nMarkTop = rcClip.top() + 2;

			static int triH = 3;
			QVector<QPoint> pts;
			pts << QPoint(pos, m_nMarkTop)
				<< QPoint(pos + m_nMarkerW * 0.5, m_nMarkTop + triH)
				<< QPoint(pos + m_nMarkerW * 0.5, m_nMarkTop + m_nMarkerH)
				<< QPoint(pos - m_nMarkerW * 0.5, m_nMarkTop + m_nMarkerH)
				<< QPoint(pos - m_nMarkerW * 0.5, m_nMarkTop + triH);
			QPainterPath path;
			path.addPolygon(QPolygon(pts));
			QPainterPath pathTmp;
			pathTmp.addRect(rcClip.left(), rcClip.top(), rcClip.width(), rcClip.height());
			path = path.intersected(pathTmp);
			painter->fillPath(path, QColor(180, 180, 180));

			if (c->GetNLETimeline()->GetSelectedMarker() == mk)
				painter->fillPath(path, QColor(255, 255, 255));
		}
		painter->setRenderHint(QPainter::Antialiasing,false);
	}

	for (auto clip : clips)
	{
		//53, 60, 65
		if (!clip) continue;
		Clip* c = clip;
		if (!c->GetSelected()) continue;

		l = m_pRuler->Frames2PositionF(c->GetStartAtTrack());
		t = 0;
		w = m_pRuler->Frames2DurationF(c->GetLenAtTrack());
		w = qMax(2, w);
		h = rc.height() - 1;
		QRect rcClip(l, t, w, h);

		if (dragClips.contains(clip))
		{
			painter->fillRect(rcClip, QColor(0, 0, 0, 150));
			continue;
		}
		//selected border
		QPen pen;
		pen.setWidth(2);
		pen.setColor(clrSelectedBorder);
		painter->setPen(pen);
		painter->setBrush(Qt::NoBrush);
		painter->drawRect(rcClip.adjusted(0, 1,0, 0));
	}
	for (auto clip : clips)
	{
		//53, 60, 65
		if (!clip) continue;
		Clip* c = clip;
		l = m_pRuler->Frames2PositionF(c->GetStartAtTrack());
		t = 0;
		w = m_pRuler->Frames2DurationF(c->GetLenAtTrack());
		w = qMax(2, w);
		h = rc.height() - 1;
		QRect rcClip(l, t, w, h);

		//接受鼠标效果提示框
		if (TVMouseEffectDragCtrl::GetInstance()->IsTipAcceptable() && c->GetNLETimeline()->IsDesktopVideo())
		{
			auto targetClip = TVMouseEffectDragCtrl::GetInstance()->GetTargetClip();
			painter->setPen(QPen((targetClip == c ? QBrush(QColor(239, 48, 98)) : QBrush(Qt::yellow)), 1));
			painter->setBrush(Qt::NoBrush);
			painter->drawRect(rcClip.adjusted(1, 0, 0, 0));
		}

	}

	//drag clips
	QColor clrDragBorder(Qt::yellow);
	painter->setPen(clrDragBorder);
	painter->setBrush(Qt::NoBrush);
	auto dragingClips = TVDragControl::GetInstance()->GetDragingClips(m_pTrackData->GetIndex());
	for (auto clip : dragingClips)
	{
		l = m_pRuler->Frames2PositionF(clip.startPos);
		t = trackRect.y() + 1;
		w = m_pRuler->Frames2DurationF(clip.length);
		h = trackRect.height() - 2;
		QRect simulateRect(l, t, w, h);
		painter->drawRect(simulateRect);
	}


	//trim clip
	auto trimClips = TVTrimControl::GetInstance()->GetTrimingList(m_pTrackData->GetIndex());
	for (auto clip : trimClips)
	{
		l = m_pRuler->Frames2PositionF(clip.startPos);
		t = trackRect.y() + 1;
		w = m_pRuler->Frames2DurationF(clip.length);
		h = trackRect.height() - 2;
		QRect simulateRect(l, t, w, h);
		painter->drawRect(simulateRect);
	}

	//change speed
	auto changeClip = ChangeSpeedControl::GetInstance()->GetDragClip(m_pTrackData->GetIndex());
	if (!changeClip.IsNull())
	{
		l = m_pRuler->Frames2PositionF(changeClip.startPos);
		t = trackRect.y() + 1;
		w = m_pRuler->Frames2DurationF(changeClip.length);
		h = trackRect.height() - 2;
		QRect simulateRect(l, t, w, h);
		painter->drawRect(simulateRect);
	}

	painter->restore();

	
	//header
	painter->save();
	QRect headerRect(0, 0, TRACK_WIDGET_HEADER_WIDTH , rc.height());
	painter->fillRect(headerRect, clrHeader);

	//t = headerRect.height() - 1;
	painter->setPen(clrLine);
	painter->drawLine(0, 0, TRACK_WIDGET_HEADER_WIDTH-1, 0);

	painter->drawLine(TRACK_WIDGET_HEADER_WIDTH - 1, 0, TRACK_WIDGET_HEADER_WIDTH - 1, headerRect.height());

	int nTrackIndex = m_pTrackData->GetIndex();
	QString strHeader = tr("Track") + QString(" %1").arg(nTrackIndex + 1);
	painter->setPen(clrText);
	QFont fnt;
	fnt.setFamily("lato");
	fnt.setPixelSize(14);
	painter->setFont(fnt);
	headerRect = QRect(0, 0, TRACK_WIDGET_HEADER_WIDTH-24, rc.height());
	painter->drawText(headerRect, strHeader, QTextOption(Qt::AlignCenter));
	painter->restore();


	painter->save();
	painter->translate(TRACK_WIDGET_HEADER_WIDTH, 0);
	painter->setClipRect(rc);
	//hiden
	if (m_pTrackData->IsHiden())
	{
		painter->fillRect(rc, QColor(0, 0, 0, 125));
	}
	//freeze
	if (m_pTrackData->IsFreeze())
	{
		painter->setRenderHint(QPainter::Antialiasing,true);
		painter->setPen(QColor(127,156,175,120));
		int xDuraion = 60;
		for (int i = 10 - xDuraion; i < rc.width(); i += 10)
		{
			painter->drawLine(QPoint(i, rc.height()), QPoint(i + xDuraion, 0));
		}
		painter->setRenderHint(QPainter::Antialiasing,false);
	}
	painter->restore();
	m_pTrackWidget->SetButtonHide(m_pTrackData->IsHiden());
	m_pTrackWidget->SetButtonFreeze(m_pTrackData->IsFreeze());
}

QSize TVTrackWidgetDraw::GetMarkerSize()
{
	return QSize(m_nMarkerW, m_nMarkerH);
}

int TVTrackWidgetDraw::GetMarkerTop()
{
	return m_nMarkTop;
}
