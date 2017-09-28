#include "stdafx.h"

#include "TimelineViewPrivate.h"
#include "inc_CommonLib/TimelineRuler.h"
#include "inc_CommonLib/NLEWraper.h"
#include "inc_CommonLib/PlayerMaterProvider.h"
#include "Interface/INLEDocumentSerialize.h"
#include "TVTrackView.h"
#include "TimelineManager.h"
#include "AudioWaveMgr.h"
#include "TimelineManager.h"
#include "inc_FSCommonlib/FSNLETimeline.h"
#include "inc_CommonLib/externalapi.h"
#include "inc_FSCommonlib/gathread.h"

TimelineViewPrivate::TimelineViewPrivate(QWidget *parent)
	: FSBaseWidget(parent)
	, m_bSeeking(false)
{
	setObjectName("TimelineViewPrivate");
	m_nRealTopPosition = 0;
	//setAcceptDrops(true);
	m_pTimelineMgr = TimelineManager::GetInstance();
	m_pTimelineViewManager = TimelineViewManager::GetInstance();
	((TimelineViewManager*)m_pTimelineViewManager)->SetTimelineViewPrivate(this);
	m_pToolBar = new TVToolBar(this);
	m_pToolBar->SetButtonEnabled(TVToolBar::ButtonId_Undo, false);
	m_pToolBar->SetButtonEnabled(TVToolBar::ButtonId_Redo, false);
	m_pToolBar->SetButtonEnabled(TVToolBar::ButtonId_Split, false);
	m_pRuler = new TimelineRuler(this);

	m_pRuler->SetFPS(25.0f);
	m_pRuler->SetMousePressEmitFramesChangedEnabled(false);
	m_pRuler->SetSingalClickStartEndFrameChangeEnabled(true);
	m_pRuler->SetDrawColor(QColor(127, 156, 175));
	RulerPointerShape shape;
	shape.aligntop = true;
	shape.height = 20;
	shape.width = 13;
	shape.color = QColor(239, 48, 98);
	shape.linewidth = 1;
	QVector<QPointF> pts;
	pts << QPointF(-6, 0) << QPointF(6, 0) << QPointF(6, 13) << QPointF(0, 20) << QPointF(-6, 13);
	shape.shape = QPolygonF(pts);
	shape.showpostext = true;
	shape.postextcolor = Qt::white;
	m_pRuler->SetPointerShape(shape);
	m_pTrackView = new TVTrackView(m_pRuler, this);
	m_pScroolBarH = new QScrollBar(Qt::Horizontal, this);
	m_pScroolBarV = new QScrollBar(Qt::Vertical, this);

	m_pRulerBkGround = new RulerBkGround(this);
	m_pRulerBkGround->setObjectName("TimelineView_RulerBackground_Widget");
	m_pRulerToolBarMask = new QWidget(this);
	m_pRulerToolBarMask->setObjectName("TimelineView_RulerToolBarMask_Widget");
	m_pToolBar->raise();
	m_pRulerBkGround->raise();
	m_pRuler->raise();
	ScrollMgr::GetInstance()->Init(this);
	TLToolTipMgr::GetInstance()->SetView(this);
	connect(m_pTimelineMgr, &TimelineManager::signalTrackUpdate, this, [&]{EnableToolBar(); });
	connect(m_pTimelineMgr, &TimelineManager::signalTrackRebuild, this, [&]{EnableToolBar(); });
	connect(m_pTimelineMgr, &TimelineManager::signalSelectionChanged, this, [&]{EnableToolBar(); });
	connect(m_pToolBar, &TVToolBar::sigScaleValueChanged, this, [&](int value){
		m_pRuler->SetZoom(value);
		HorizontalScrollBar();
		AudioWaveMgr::GetInstance()->SetNeedReflesh(true);
		TrackBuilder::GetInstance()->UpdateItems();
	});
	connect(m_pToolBar, &TVToolBar::sigStartValueChanged, this, [&]{
		m_pRuler->StartZoomAndScrollRuler();
	});

	connect(m_pToolBar, &TVToolBar::sigClicked, this, &TimelineViewPrivate::slotButtonClicked);
	connect(m_pRuler, &TimelineRuler::ZoomChanged, [&](int zoom)
	{
		m_pToolBar->SetScaleUnitValue(zoom);
		HorizontalScrollBar();
		AudioWaveMgr::GetInstance()->SetNeedReflesh(true);
		TrackBuilder::GetInstance()->UpdateItems();
	});

	m_pMenuTrackMark = new QMenu(this);
	m_pMenuTrackMark->setFixedWidth(180);
	m_pActAddTrackMark = new QAction(this);
	connect(m_pRuler, &TimelineRuler::sigRightClicked, [&](QPoint pt)
	{
		m_pMenuTrackMark->clear();
		m_pActAddTrackMark->setText(tr("Add Marker"));
		m_pActAddTrackMark->setShortcut(QKeySequence("M"));
		m_pMenuTrackMark->addAction(m_pActAddTrackMark);
		m_pMenuTrackMark->exec(m_pRuler->mapToGlobal(pt));
	});
	connect(m_pActAddTrackMark, &QAction::triggered, this, [&]{
		m_pTimelineMgr->GetMarkPointMgr()->Add(m_pTimelineMgr->GetCurrentFrames());
	});

	m_pScroolBarH->hide();
	m_pScroolBarH->setRange(0, 100);
	connect(m_pScroolBarH, &QSlider::valueChanged, this, [&](int value)
	{
		m_pRuler->SetOffsetFrames(value);
		AudioWaveMgr::GetInstance()->SetNeedReflesh(true);
		TrackBuilder::GetInstance()->UpdateItems();
	});

	m_pScroolBarV->hide();
	m_pScroolBarV->setRange(0, 0);
	connect(m_pScroolBarV, &QSlider::valueChanged, this, [&](int value)
	{
		m_pTrackView->move(QPoint(0, m_pRuler->geometry().bottom() - value));
		m_nRealTopPosition = m_pTrackView->pos().y();
	});

	connect(m_pTrackView, &TVTrackView::sigDurationChanged, this, [&](int duration){
		m_pRuler->SetTotalFrames(duration);
	});
	connect(m_pRuler, &TimelineRuler::StartFramesChanged, this, [&]{m_bSeeking = true; });
	connect(m_pRuler, &TimelineRuler::EndFramesChanged, this, [&]{m_bSeeking = false; });

	connect(TimelineViewManager::GetInstance(), &TimelineViewManager::sigCBCurrentFrameChanged, this, [&](int frame){

		if (m_bSeeking) return;
		if (frame == m_pRuler->CurrentFrames()) return;

		m_pRuler->SetCurrentFrames(frame);
		m_pTimelineMgr->SetCurrentFrames(frame);

		int pos = m_pRuler->Frames2PositionF(frame);
		int newFramePos = 0;
		if (pos > m_pRuler->geometry().right())
		{
			newFramePos = m_pRuler->Duration2FramesF((pos - m_pRuler->geometry().width() / 2));
			newFramePos = m_pScroolBarH->value() + newFramePos;
		}
		else if (pos < /*m_pRuler->geometry().left()*/0)
		{
			newFramePos = m_pRuler->Duration2FramesF(/*m_pRuler->geometry().left()*/0 - pos);
			newFramePos = m_pScroolBarH->value() - newFramePos;
		}
		if (newFramePos != 0)
		{
			m_pScroolBarH->setValue(newFramePos);
		}
		TrackBuilder::GetInstance()->UpdateItems(true);
		EnableToolBar();
	});
	connect(TimelineViewManager::GetInstance(), &TimelineViewManager::sigDurationChanged, this, [&](int d){HorizontalScrollBar(true); });
	connect(TVDragControl::GetInstance(), &TVDragControl::sigCompositeFinish, this, &TimelineViewPrivate::EnableToolBar);
	connect(m_pTimelineMgr->GetPlayerEditManager(), &IPlayerEditorManager::sigTimelineAdded, this, &TimelineViewPrivate::slotReprtGaInfo);

	m_pRuler->SetZoom(m_pToolBar->GetScaleUnitDuration() - m_pToolBar->GetScaleUnitValue());
	SerializeHelper::GetInstance();
	m_pRuler->setAttribute(Qt::WA_StyledBackground, false);

	m_pTLMarkLayer = new TLMarkLayer(m_pTrackView,this);
	m_pTLMarkLayer->raise();

	m_pTopMaskView = new TopMaskView(this);
	connect(TrackBuilder::GetInstance(), &TrackBuilder::Update, this, [&]{m_pTopMaskView->Update(); });
	m_pTopMaskView->raise();
	qApp->installEventFilter(this);
}

void TimelineViewPrivate::HorizontalScrollBar(bool bDurationChange /*= false*/)
{
	int totalFrame = m_pRuler->AvailableTotaleFrames();
	int duration = TrackBuilder::GetInstance()->Duration();
	auto range = duration;
	m_pScroolBarH->setVisible(range != 0);

	m_pScroolBarH->setRange(0, range);
	m_pScroolBarH->setSingleStep(10);
	m_pScroolBarH->setPageStep(m_pRuler->AvailableTotaleFrames() - m_pRuler->OffsetFrames());

	if (!bDurationChange)
	{
		int offset = m_pRuler->GetZoomAndScrollRulerOffsetPosition();
		m_pScroolBarH->setValue(qMax(0.0f, m_pRuler->Duration2FramesF(offset)));
	}
}

void TimelineViewPrivate::OnAdjustVScroolBar(FSNLETimeline *addedTimeline)
{
	auto pos = (addedTimeline->GetLevel() + 1) * TRACK_WIDGET_HEIGHT;

	int visibleHeight = m_pScroolBarH->geometry().top() - m_pRuler->geometry().bottom();
	auto curValue = m_pScroolBarV->value();
	if ((pos - curValue) > visibleHeight)
	{
		m_pScroolBarV->setValue(curValue + (pos - curValue - visibleHeight));
	}
	else if ((pos - TRACK_WIDGET_HEIGHT - curValue) < 0)
	{
		m_pScroolBarV->setValue(curValue + (pos - TRACK_WIDGET_HEIGHT - curValue));
	}
}

void TimelineViewPrivate::OnAddMarker()
{
	auto curFrame = m_pTimelineMgr->GetCurrentFrames();
	auto selectClips = m_pTimelineMgr->GetSelected(curFrame);

	bool bAddCliped = false;
	if (selectClips.size() == 1)
	{
		auto tl = selectClips[0]->GetNLETimeline();
		if (tl->CanAddMarker())
		{
			bAddCliped = true;
			m_pTimelineMgr->GetClipMarkerMgr()->Add(tl, curFrame);
		}
	}
	if (!bAddCliped)
		m_pTimelineMgr->GetMarkPointMgr()->Add(curFrame);
}

void TimelineViewPrivate::EnableToolBar()
{
	QTimer::singleShot(0, [&]{
		m_pToolBar->SetButtonEnabled(TVToolBar::ButtonId_Undo, m_pTimelineMgr->CanUndo());
		m_pToolBar->SetButtonEnabled(TVToolBar::ButtonId_Redo, m_pTimelineMgr->CanRedo());
		m_pToolBar->SetButtonEnabled(TVToolBar::ButtonId_Split, m_pTimelineMgr->CanSplit());
	});

}

void TimelineViewPrivate::TransFromGaInfo()
{
	int iAudioClipCount = 0;
	int iVedioClipCount = 0;
	int iPictureClipCount = 0;
	int iAnnotationClipCount = 0;
	int allClipConunts = 0;
	allClipConunts = m_pTimelineMgr->GetClips().length();
	TimelineTracks allTracks = m_pTimelineMgr->GetTracks();
	QList<TimelineTrack*>::iterator iter = allTracks.begin();
	QList<Clip*>::iterator clipIter;
	for (; iter != allTracks.end(); ++iter)
	{
		//遍历每个轨道上的clip信息
		Clips clipsInfos = (*iter)->GetClips();
		clipIter = clipsInfos.begin();
		for (; clipIter != clipsInfos.end(); ++clipIter)
		{
			if ((*clipIter)->GetType() == TimelineType::TimelineType_Audio)
			{
				iAudioClipCount++;
				continue;
			}
			if ((*clipIter)->GetType() == TimelineType::TimelineType_Video
				|| (*clipIter)->GetType() == TimelineType_VideoAudio
				|| (*clipIter)->GetType() == TimelineType_Complex)
			{
				iVedioClipCount++;
				continue;
			}
			if ((*clipIter)->GetType() == TimelineType::TimelineType_Image)
			{
				iPictureClipCount++;
				continue;
			}
			if ((*clipIter)->GetType() == TimelineType::TimelineType_Annotaions_Caption)
			{
				iAnnotationClipCount++;
				continue;
			}
		}
	}
	//上传用户的数据
	//退出前所有的clip的数目
	GAThread::getInstance()->insertEventItem(Clip_Edit, "ClipCount", QString::number(allClipConunts));
	//退出前Audio clip的数目
	GAThread::getInstance()->insertEventItem(Clip_Edit, "AudioClipsCount", QString::number(iAudioClipCount));
	//退出前photo clip的数目
	GAThread::getInstance()->insertEventItem(Clip_Edit, "PhotoClipsCount", QString::number(iPictureClipCount));
	//退出前Vedio clip的数目
	GAThread::getInstance()->insertEventItem(Clip_Edit, "VideoClipsCount", QString::number(iVedioClipCount));
	//退出前Annotation clip的数目
	GAThread::getInstance()->insertEventItem(Clip_Edit, "AnnotationClipsCount", QString::number(iAnnotationClipCount));
}

void TimelineViewPrivate::Zoom2FitTimeline()
{
	auto durationFrame = TrackBuilder::GetInstance()->Duration();
	auto viewWidth = m_pTrackView->width() - TRACK_WIDGET_HEADER_WIDTH - 16;
	auto viewWidthFrames = m_pRuler->Duration2FramesF(viewWidth);

	auto findZoom = m_pRuler->FindZoom(durationFrame, viewWidth);
	m_pRuler->SetZoom(findZoom);
	m_pToolBar->SetScaleUnitValue(m_pRuler->Zoom());

	m_pScroolBarH->setValue(0);
	AudioWaveMgr::GetInstance()->SetNeedReflesh(true);
	TrackBuilder::GetInstance()->UpdateItems();
	
}

TopMaskView* TimelineViewPrivate::GetTopMaskView()
{
	return m_pTopMaskView;
}

IPlayerEditorManager* TimelineViewPrivate::GetPlayerEditManager()
{
	return TimelineManager::GetInstance()->GetPlayerEditManager();
}

ITimelineViewManager* TimelineViewPrivate::GetTimelineViewManager()
{
	return m_pTimelineViewManager;
}

ITitleBarManager* TimelineViewPrivate::GetTitleBarManager()
{
	return TimelineManager::GetInstance()->GetTitleBarManager();
}

void TimelineViewPrivate::Resize()
{
	resizeEvent(nullptr);
}

QScrollBar* TimelineViewPrivate::GetHScrollBar()
{
	return m_pScroolBarH;
}

QScrollBar* TimelineViewPrivate::GetVScrollBar()
{
	return m_pScroolBarV;
}

TVTrackView* TimelineViewPrivate::GetTrackView()
{
	return m_pTrackView;
}

void TimelineViewPrivate::EnableShortcuts(bool bEnable)
{
	m_pToolBar->EnableShortCuts(bEnable);
}

void TimelineViewPrivate::slotButtonClicked(int id)
{
	switch (id)
	{
	case TVToolBar::ButtonId_Undo:
		m_pTimelineMgr->Undo();
		break;
	case TVToolBar::ButtonId_Redo:
		m_pTimelineMgr->Redo();
		break;
	case TVToolBar::ButtonId_Split:
		m_pTimelineMgr->Split();
		TrackBuilder::GetInstance()->UpdateItems();
		//上传GA数据
		GAThread::getInstance()->insertEventItem(Clip_Edit, "Split", "");
		break;
	case TVToolBar::ButtonId_Mark:
		OnAddMarker();
		break;
	case TVToolBar::ButtonId_Zoom2FitTimeline:
		Zoom2FitTimeline();
		break;

	}
}

void TimelineViewPrivate::slotReprtGaInfo(FSNLETimeline* timeline)
{
	//上传GA数据
	if (timeline->GetType() == TimelineType_Annotaions_Caption)
	{
		//timeline
		QString strName = timeline->GetName();
		GAThread::getInstance()->insertEventItem(Annotation_Edit, "Type", strName);
	}
}

void TimelineViewPrivate::resizeEvent(QResizeEvent *e)
{
	static const int scrollBarSpace = 8;
	static const int toolBarH = 28;
	static const int rulerH = 46;
	static const int trackHeader = TRACK_WIDGET_HEADER_WIDTH;
	static const int rightMargin = 6;

	m_pToolBar->setGeometry(0, 0, width() - rightMargin, toolBarH);

	int l = trackHeader;
	int t = m_pToolBar->geometry().bottom() + 1;
	m_pRuler->setGeometry(l, t, width() - trackHeader - rightMargin, rulerH);
	m_pRulerBkGround->setGeometry(0, m_pRuler->geometry().top(), width(), m_pRuler->geometry().height());

	m_pRulerToolBarMask->setGeometry(0, m_pToolBar->geometry().top(), m_pToolBar->width(), m_pToolBar->height() + m_pRuler->height());

	if (m_nRealTopPosition == 0)
		t = m_pRuler->geometry().bottom() + 1;
	else
		t = m_nRealTopPosition + 1;

	int viewportHeiht = height() - scrollBarSpace - m_pRuler->geometry().bottom();
	int h = qMax(m_pTrackView->Height(), viewportHeiht);

	auto newSV = m_pScroolBarV->value() + (h - m_pTrackView->height());


	m_pTrackView->ResizeGeometry(0, t, width() - scrollBarSpace, h - 1);

	l = trackHeader;
	t = height() - scrollBarSpace;
	m_pScroolBarH->setGeometry(l, t, width() - trackHeader - rightMargin, scrollBarSpace);

	l = width() - rightMargin;
	t = m_pRuler->geometry().bottom() + 1;
	h = height() - m_pRuler->geometry().bottom() - m_pScroolBarH->height();
	m_pScroolBarV->setGeometry(l, t, scrollBarSpace, h);

	int trackHeightTemp = viewportHeiht - m_pTrackView->Height();
	m_pScroolBarV->setSingleStep(10);
	m_pScroolBarV->setPageStep(m_pScroolBarV->height());
	m_pScroolBarV->setVisible(trackHeightTemp < 0);
	m_pScroolBarV->setRange(0, qMax(0, -trackHeightTemp));

	m_pScroolBarV->setValue(newSV);

	m_pTopMaskView->setGeometry(trackHeader,
		m_pRuler->geometry().bottom(),
		width() - trackHeader - m_pScroolBarV->geometry().width(),
		height() - m_pRuler->geometry().bottom() - m_pScroolBarH->geometry().height());

	m_pTLMarkLayer->setGeometry(m_pTopMaskView->geometry());


	QPoint topleft = m_pTopMaskView->geometry().topLeft();// -QPoint(trackHeader, 0);
	QPoint bottomright = m_pTopMaskView->geometry().bottomRight();
	topleft = mapToGlobal(topleft);
	bottomright = mapToGlobal(bottomright);

	TVDragControl::GetInstance()->SetViewportRect(QRect(topleft, bottomright));

	m_pRuler->SetZoom(m_pToolBar->GetScaleUnitDuration() - m_pToolBar->GetScaleUnitValue());
}

void TimelineViewPrivate::wheelEvent(QWheelEvent *e)
{
	int step = e->angleDelta().y() > 0 ? -TRACK_WIDGET_HEIGHT : TRACK_WIDGET_HEIGHT;
	m_pScroolBarV->setValue(qMax(0, m_pScroolBarV->value() + step));
}

void TimelineViewPrivate::mousePressEvent(QMouseEvent *e)
{
	emit m_pTimelineViewManager->sigUnselectMatterView();
	FSBaseWidget::mousePressEvent(e);
}

void TimelineViewPrivate::closeEvent(QCloseEvent *event)
{
	return FSBaseWidget::closeEvent(event);
}

bool TimelineViewPrivate::eventFilter(QObject * obj, QEvent * e)
{
	if (e->type() == QEvent::MouseButtonPress)
	{
		QMouseEvent *mse = (QMouseEvent*)e;
		auto pos = mapFromGlobal(mse->globalPos());
		if (geometry().contains(pos))
		{
			emit m_pTimelineViewManager->sigFocusIn();
		}
	}
	else if (e->type() == QEvent::Drop)
	{
		QDropEvent *dpe = (QDropEvent*)e;
		auto pos = mapFromGlobal(dpe->pos());
		if (geometry().contains(pos))
		{
			emit m_pTimelineViewManager->sigFocusIn();
		}
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
TVToolBar::TVToolBar(QWidget *parent)
	: FSBaseWidget(parent)
{
	m_pSlider = new ScaleUnitSlider(this);



	auto scZoomin = new QShortcut(this);
	scZoomin->setKey(QKeySequence("Ctrl+="));
	scZoomin->setContext(Qt::ApplicationShortcut);
	auto scZoomout = new QShortcut(this);
	scZoomout->setKey(QKeySequence("Ctrl+-"));
	scZoomout->setContext(Qt::ApplicationShortcut);
	connect(scZoomin, &QShortcut::activated, this, [&]{m_pSlider->Add(); });
	connect(scZoomout, &QShortcut::activated, this, [&]{m_pSlider->Sub(); });

	InitButtons();
	connect(m_pSlider, &ScaleUnitSlider::valueChanged, this, [&](int value){ emit sigScaleValueChanged(m_pSlider->duration() - value); });
	connect(m_pSlider, &ScaleUnitSlider::startValueChanged, this, [&](){ emit sigStartValueChanged(); });
	m_pSlider->SetChunkHeight(10);
	m_pSlider->setObjectName("TVToolBar_m_pSlider");
	setObjectName("TVToolBar");
	RetranslateUi();
}

void TVToolBar::SetScaleUnitValue(int value)
{
	m_pSlider->setValue(m_pSlider->duration() - value);
}

int TVToolBar::GetScaleUnitValue()
{
	return m_pSlider->value();
}

int TVToolBar::GetScaleUnitDuration()
{
	return m_pSlider->duration();
}

void TVToolBar::SetButtonEnabled(ButtonId id, bool enable)
{
	auto btn = GetButton(id);
	if (btn)
	{
		btn->SetEnabled(enable);
	}
}

void TVToolBar::EnableShortCuts(bool bEnable)
{
	for (auto sc : m_lstShortcuts)
	{
		sc->setEnabled(bEnable);
	}
}

void TVToolBar::InitButtons()
{
	for (int i = ButtonId_BeginFlag + 1; i < ButtonId_EndFlag; ++i)
	{
		AddButton((ButtonId)i);
	}
}

QString TVToolBar::GetButtonTooltipFromButtonID(TVToolBar::ButtonId id)
{
    QString tooltip;
    switch (id)
    {
    case TVToolBar::ButtonId_Undo:
        tooltip = tr("Undo") + "(Ctrl+Z)";
        break;
    case TVToolBar::ButtonId_Redo:
        tooltip = tr("Redo") + "(Ctrl+Y)";
        break;
    case TVToolBar::ButtonId_Split:
        tooltip = tr("Split") + "(Ctrl+B)";
        break;
	case TVToolBar::ButtonId_Mark:
		tooltip = tr("Add Marker") + "(M)";
		break;
    case TVToolBar::ButtonId_Zoom2FitTimeline:
        tooltip = tr("Zoom to Fit Timeline") + "(Shift+Z)";
        break;
    }
    return tooltip;
}

void TVToolBar::AddButton(ButtonId id)
{
	QString objname;
	switch (id)
	{
	case TVToolBar::ButtonId_Undo:
		objname = "TimelineView_ToolBar_UndoButton";
		break;
	case TVToolBar::ButtonId_Redo:
		objname = "TimelineView_ToolBar_RedoButton";
		break;
	case TVToolBar::ButtonId_Split:
		objname = "TimelineView_ToolBar_SplitButton";
		break;
	case TVToolBar::ButtonId_Mark:
		objname = "TimelineView_ToolBar_MarkButton";
		break;
	case TVToolBar::ButtonId_Zoom2FitTimeline:
		objname = "TimelineView_ToolBar_Zoom2FitTimlineButton";
		break;
	}
    QString tooltip = GetButtonTooltipFromButtonID(id);
	BaseButton *pButton = new BaseButton(id, objname, this);
	pButton->setToolTip(tooltip);
	connect(pButton, &BaseButton::Clicked, this, [&](int id){
		emit sigClicked((ButtonId)id);
	});
	m_lstButtons.push_back(pButton);

	if (id == ButtonId_Split)
	{
		auto scSplit = new QShortcut(this);
		scSplit->setKey(QKeySequence("Ctrl+B"));
		scSplit->setContext(Qt::ApplicationShortcut);
		connect(scSplit, &QShortcut::activated, this, [&]{
			auto splitbtn = GetButton(ButtonId_Split);
			if (splitbtn->isEnabled())
				emit sigClicked(ButtonId_Split);
		});
	}
	else if (id == ButtonId_Mark)
	{
		auto scAddMarder = new QShortcut(this);
		scAddMarder->setKey(QKeySequence("M"));
		scAddMarder->setContext(Qt::ApplicationShortcut);
		connect(scAddMarder, &QShortcut::activated, this, [&]{
			auto addMarkerbtn = GetButton(ButtonId_Mark);
			if (addMarkerbtn->isEnabled())
				emit sigClicked(ButtonId_Mark);
		});
	}
	else if (id == ButtonId_Zoom2FitTimeline)
	{
		auto scZoom2Fit = new QShortcut(this);
		scZoom2Fit->setKey(QKeySequence("Shift+Z"));
		scZoom2Fit->setContext(Qt::ApplicationShortcut);
		connect(scZoom2Fit, &QShortcut::activated, this, [&]{
			auto zoom2FitBnt = GetButton(ButtonId_Zoom2FitTimeline);
			if (zoom2FitBnt->isEnabled())
				emit sigClicked(ButtonId_Zoom2FitTimeline);
		});
		m_lstShortcuts.push_back(scZoom2Fit);
	}
}

BaseButton* TVToolBar::GetButton(ButtonId id)
{
	for (auto btn : m_lstButtons)
	{
		if (btn->GetId() == id)
		{
			return btn;
		}
	}
	return nullptr;
}

void TVToolBar::resizeEvent(QResizeEvent *e)
{
	int h = height();
	int w = h;

	int l = 0;
	int t = 0;
	for (auto btn : m_lstButtons)
	{
		btn->setGeometry(l, t, w, h);
		l = btn->geometry().right() + 1;
	}
	w = 200;
	m_pSlider->setGeometry(l, t, w, h);
}

void TVToolBar::RetranslateUi()
{
    for (BaseButton *btn : m_lstButtons)
    {
        btn->setToolTip(GetButtonTooltipFromButtonID((ButtonId)btn->GetId()));
    }

    auto btnZoomin = m_pSlider->GetZoomInButton();
    auto btnZoomout = m_pSlider->GetZoomOutButton();
    btnZoomin->setToolTip(tr("Zoom in") + "(Ctrl+=)");
    btnZoomout->setToolTip(tr("Zoom out") + "(Ctrl+-)");
}

//////////////////////////////////////////////////////////////////////////
ScrollMgr::ScrollMgr(QObject *parent /*= 0*/)
	:QObject(parent)
{
	m_bAutoScrollOn = false;
	connect(&m_timer, &QTimer::timeout, this, &ScrollMgr::OnTimer);
}

ScrollMgr* ScrollMgr::GetInstance()
{
	static ScrollMgr ins;
	return &ins;
}

void ScrollMgr::Init(TimelineViewPrivate* view)
{
	m_TimelineViewPrivate = view;
}

void ScrollMgr::AutoScroll(bool bOn)
{
	if (m_bAutoScrollOn == bOn) return;
	m_bAutoScrollOn = bOn;
	bOn ? m_timer.start(30) : m_timer.stop();
}

void ScrollMgr::OnTimer()
{
	if (!m_bAutoScrollOn) return;

	auto rcViewport = TVDragControl::GetInstance()->GetViewportRect();
	auto hScrollBar = m_TimelineViewPrivate->GetHScrollBar();
	auto vScrollBar = m_TimelineViewPrivate->GetVScrollBar();

	QPoint cursorPos = QCursor::pos();
	if (cursorPos.x() < rcViewport.left())
	{
		cursorPos.setX(rcViewport.left() + 1);
		hScrollBar->setValue(qMax(0, hScrollBar->value() - 50));
	}
	else if (cursorPos.x() > rcViewport.right()- 24)
	{
		cursorPos.setX(rcViewport.right() - 1);
		hScrollBar->setValue(qMin(hScrollBar->maximum(), hScrollBar->value() + 50));
	}
		
	if (cursorPos.y() < rcViewport.top())
		cursorPos.setY(rcViewport.top() + 1);
	else if (cursorPos.y() > rcViewport.bottom())
		cursorPos.setY(rcViewport.bottom() - 1);

	auto pt = m_TimelineViewPrivate->GetTrackView()->mapFromGlobal(cursorPos);
	TVDragControl::GetInstance()->DragMove(pt);
	
}


//////////////////////////////////////////////////////////////////////////
TimelineViewManager::TimelineViewManager(QObject *parent /*= 0*/)
{
	m_pTimelineViewPrivate = nullptr;
	connect(TimelineManager::GetInstance(), &TimelineManager::sigDoCommand, this, [&]{
		emit sigContentChanged(true);
	});
}

TimelineViewManager* TimelineViewManager::GetInstance()
{
	static TimelineViewManager ins;
	return &ins;
}

void TimelineViewManager::SetTimelineViewPrivate(TimelineViewPrivate *pTimelineViewPrivate)
{
	m_pTimelineViewPrivate = pTimelineViewPrivate;
}

void TimelineViewManager::SetCurrentFrame(int frame)
{
	emit sigCBCurrentFrameChanged(frame);
}

int TimelineViewManager::CurrentFrame()
{
	return TimelineManager::GetInstance()->GetCurrentFrames();
}

FSNLETimeline* TimelineViewManager::GetRootTimeline()
{
	return TimelineManager::GetInstance()->GetRootTimeline();
}


void TimelineViewManager::FetchAudioWaveData(FSNLETimeline *audioTimeline)
{
	return AudioWaveMgr::GetInstance()->Add(audioTimeline);
}

void TimelineViewManager::UpdateView()
{
	emit sigCBUpdateView();
}

QString TimelineViewManager::GetProjectString()
{
	return SerializeHelper::GetInstance()->GetString();
}

void TimelineViewManager::LoadProjectString(const QString projString)
{
	SerializeHelper::GetInstance()->Load(projString);
}

void TimelineViewManager::Clear()
{
	SerializeHelper::GetInstance()->Clear();
}

void TimelineViewManager::EnableShortcuts(bool bEnable)
{
	if (m_pTimelineViewPrivate) m_pTimelineViewPrivate->EnableShortcuts(bEnable);
}

void TimelineViewManager::SetSpeedScale(FSNLETimeline* timeline, float scale)
{
	TimelineManager::GetInstance()->SetSpeedScale(timeline,scale);
}

void TimelineViewManager::DenoiseLevelChanged(FSNLETimeline *timeline)
{
	AudioWaveMgr::GetInstance()->Add(timeline);
}

void TimelineViewManager::SetMouseEffectDraging(bool bDraging)
{
	TVMouseEffectDragCtrl::GetInstance()->SetDragTip(bDraging);
}

//////////////////////////////////////////////////////////////////////////
SerializeHelper::SerializeHelper(QObject *parent /*= 0*/)
	:QObject(parent)
{
	m_pISerailizeEx = nullptr;
	m_pSerialize = NLEWraper::GetInstance().GetPlayerMaterProvider()->CreateDocumentSerialize();
	Q_ASSERT(m_pSerialize);
	m_pSerialize->QueryInterface(IID_INLEDocumentSerializeEx, (void**)&m_pISerailizeEx);
	Q_ASSERT(m_pISerailizeEx);
}

SerializeHelper* SerializeHelper::GetInstance()
{
	static SerializeHelper ins;
	return &ins;
}

void SerializeHelper::Load(const QString &str)
{
	if (str.trimmed().isEmpty()) return;
	INLETimelineBase *timeline;

	QString strTimeline;
	QString begin = "<TimelineMessage>";
	QString end = "</TimelineMessage>";
	int pos1 = str.indexOf(begin) + begin.length();
	int pos2 = str.indexOf(end);
	if (pos1 != -1 && pos2 != -1)
		strTimeline = str.mid(pos1, pos2 - pos1);

	if (strTimeline.trimmed().isEmpty())
		return;
	std::wstring wstr = strTimeline.toStdWString();
	m_pISerailizeEx->LoadEx(&timeline, wstr.c_str());
	Q_ASSERT(timeline);
	TimelineManager::GetInstance()->Load(timeline);

}

QString SerializeHelper::GetString()
{
	LPWSTR pStr = nullptr;
	TimelineManager::GetInstance()->SaveHidenFreezeTrack2RootTimeline();
	m_pISerailizeEx->SaveEx(TimelineManager::GetInstance()->GetRootTimeline()->GetSrc(), &pStr);
	QString res =QString("") +  "<TimelineMessage>\r\n" 
		+ QString::fromStdWString(pStr) 
		+ "</TimelineMessage>\r\n";
	return res;
}

void SerializeHelper::Clear()
{
	TimelineManager::GetInstance()->Reset();
}

//////////////////////////////////////////////////////////////////////////
RulerBkGround::RulerBkGround(QWidget *parent)
	:QWidget(parent)
{
	//setAttribute(Qt::WA_TranslucentBackground);
	setAttribute(Qt::WA_StyledBackground);
	//setStyleSheet("background-color:blue");
}

void RulerBkGround::paintEvent(QPaintEvent *e)
{
	QPainter painter(this);
	QPen pen;
	pen.setWidth(1);
	pen.setColor(QColor(39, 49, 60));
	painter.setPen(pen);
	painter.drawLine(0, height() - 1, TRACK_WIDGET_HEADER_WIDTH, height() - 1);
	painter.drawLine(TRACK_WIDGET_HEADER_WIDTH - 1, 0, TRACK_WIDGET_HEADER_WIDTH - 1, height());
}

//////////////////////////////////////////////////////////////////////////
TopMaskView::TopMaskView(QWidget *parent)
	:QWidget(parent)
{
	setAttribute(Qt::WA_TranslucentBackground);
	setAttribute(Qt::WA_TransparentForMouseEvents);

	m_pToolTipWidget = new TrimToolTip(this);
	m_pToolTipWidget->resize(140, 40);
	m_pToolTipWidget->setVisible(false);

	connect(TVTrimControl::GetInstance(), &TVTrimControl::sigBeginTrim, this, [&]{
		//上传GA数据
		GAThread::getInstance()->insertEventItem(Clip_Edit, "Trim", "");
	});
	connect(TVTrimControl::GetInstance(), &TVTrimControl::sigEndTrim, this, [&]{
		ShowToolTip(false);
	});
	connect(TVTrimControl::GetInstance(), &TVTrimControl::sigTriming, this, [&](QPoint pt,int startPos,int lenght){
		QString strDuration = gFrame2String(lenght);
		QString strStartTime = gFrame2String(startPos);
		QString tipText = " " + tr("Start") + ": " + strStartTime + "\r\n";
		tipText += " " + tr("Duration") + ": " + strDuration;

		ShowToolTip(true, tipText, pt);
	});

	connect(ChangeSpeedControl::GetInstance(), &ChangeSpeedControl::sigChanging, this, [&](QPoint pt,int lenght,int scale){
		QString strDuration = gFrame2String(lenght);
		QString strScale = QString("%1%").arg(scale);
		QString tipText = " " + tr("Speed") + ": " + strScale + "\r\n";
		tipText += " " + tr("Duration") + ": " + strDuration;

		ShowToolTip(true, tipText, pt);
	});
	connect(ChangeSpeedControl::GetInstance(), &ChangeSpeedControl::sigEndChange, this, [&]{
		ShowToolTip(false);
	});

	connect(TLToolTipMgr::GetInstance(), &TLToolTipMgr::sigShowTip, this, [&](bool bShow, int startPos, int lenght, QPoint globalPos){
		if (!TVTrimControl::GetInstance()->IsTriming() && !ChangeSpeedControl::GetInstance()->IsChanging())
		{
			QString strDuration = gFrame2String(lenght);
			QString strStartTime = gFrame2String(startPos);
			QString tipText = " " + tr("Start") + ": " + strStartTime + "\r\n";
			tipText += " " + tr("Duration") + ": " + strDuration;

			ShowToolTip(bShow, tipText, globalPos);
		}
	});
}

void TopMaskView::Update()
{
	auto ruler = TrackBuilder::GetInstance()->GetRuler();
	int nCurrentPos = ruler->Frames2PositionF(ruler->CurrentFrames());
	update();
}

void TopMaskView::paintEvent(QPaintEvent *e)
{
	QPainter painter(this);
	auto ruler = TrackBuilder::GetInstance()->GetRuler();

	//CurrentFrame
	QColor clrCurrentFrameLine(239, 48, 98);
	int nCurrentPos = ruler->Frames2PositionF(ruler->CurrentFrames());
	QPen cfpen(clrCurrentFrameLine);
	cfpen.setWidth(1);
	painter.setPen(cfpen);
	painter.drawLine(nCurrentPos, 0, nCurrentPos, height());

	auto adsorbPts = TVDragControl::GetInstance()->GetAdsorbList();
	if (adsorbPts.size() > 0 && TVDragControl::GetInstance()->IsDraging())
	{
		//QColor clrAsorptionLine(133, 133, 133);
		QPen pen;
		pen.setWidth(2);
		pen.setColor(Qt::yellow);
		painter.setPen(pen);
		for (auto adsorbPt : adsorbPts)
		{
			auto pos = ruler->Frames2PositionF(adsorbPt);
			painter.drawLine(pos, 0, pos, height());
		}
	}

	adsorbPts = TVTrimControl::GetInstance()->GetAdsorbList();
	if (adsorbPts.size() > 0 && TVTrimControl::GetInstance()->IsTriming())
	{
		//QColor clrAsorptionLine(133, 133, 133);
		QPen pen;
		pen.setWidth(2);
		pen.setColor(Qt::yellow);
		painter.setPen(pen);
		for (auto adsorbPt : adsorbPts)
		{
			auto pos = ruler->Frames2PositionF(adsorbPt);
			painter.drawLine(pos, 0, pos, height());
		}
	}

	auto adsorbpPt = ChangeSpeedControl::GetInstance()->GetAdsorbPos();
	if (adsorbpPt >= 0)
	{
		QPen pen;
		pen.setWidth(2);
		pen.setColor(Qt::yellow);
		painter.setPen(pen);
		auto pos = ruler->Frames2PositionF(adsorbpPt);
		painter.drawLine(pos, 0, pos, height());
		
	}
}

#include "inc_CommonLib/StringOperation.h"
void TopMaskView::ShowToolTip(bool bShow, QString tipText, QPoint globalPos /*= QPoint()*/)
{
	m_pToolTipWidget->setVisible(bShow);
	if (!bShow)
		return;
	
	//QString strDuration = gFrame2String(lenght);
	//QString strStartTime = gFrame2String(startPos);

	//QString tipText = " " + tr("Start") + ": " + strStartTime + "\r\n";
	//tipText += " " + tr("Duration") + ": " + strDuration;

	m_pToolTipWidget->SetToolTip(tipText);
	auto showPoint = mapFromGlobal(globalPos + QPoint(5, 12));
	showPoint.setY(qBound(0, showPoint.y(), height() - 40));
	m_pToolTipWidget->move(showPoint);
  
    int tipWidth = StringOperation::GetTextTrueLenght(m_pToolTipWidget->font(), tipText);
    m_pToolTipWidget->setFixedWidth(tipWidth * 2 / 3);
}


//////////////////////////////////////////////////////////////////////////
TrimToolTip::TrimToolTip(QWidget *parent)
	:QWidget(parent)
{
	setAttribute(Qt::WA_StyledBackground);
	setObjectName("FSTimelineView_TrimToolTip");

	m_pLbText = new QLabel(this);
	m_pLbText->setObjectName("FSTimelineView_TrimToolTip_TextLabel");
	m_pLbText->setAlignment(Qt::AlignVCenter);
}

void TrimToolTip::SetToolTip(const QString &text)
{
	m_pLbText->setText(text);
}

void TrimToolTip::resizeEvent(QResizeEvent *e)
{
	m_pLbText->setGeometry(rect());
}

//////////////////////////////////////////////////////////////////////////
TLMarkLayer::TLMarkLayer(TVTrackView* pTrackView, QWidget *parent)
	:FSBaseWidget(parent)
{
	m_pHitMark = nullptr;
	m_pTVTrackView = pTrackView;
	m_MarkWidgetW = 9;
	m_MarkWidgetH = 16;
	setAttribute(Qt::WA_TranslucentBackground);
	setAttribute(Qt::WA_TransparentForMouseEvents);

	m_pMenu = new CusMenu(this);
	m_pMenu->setFixedWidth(220);
	m_pMenu->SetLeftPadding(20);
	m_pActDelete = new QAction(this);
	m_pActDeleteAll = new QAction(this);
	m_pActColor = new ColorMenuItem(this);
	m_pActColor->AddColor(QColor(255, 255, 255));
	m_pActColor->AddColor(QColor(175, 139, 177));
	m_pActColor->AddColor(QColor(66, 141, 252));
	m_pActColor->AddColor(QColor(25, 244, 214));
	m_pActColor->AddColor(QColor(113, 134, 55));
	m_pActColor->AddColor(QColor(208, 161, 43));
	m_pActColor->AddColor(QColor(233, 111, 36));
	m_pActColor->SetSelectColor(QColor(255, 255, 255));
	

	connect(TimelineManager::GetInstance()->GetMarkPointMgr(), &MarkPointMgr::sigUpdate, this, [&]{update(); });
	connect(m_pActDelete, &QAction::triggered, this, [&]{
		TimelineManager::GetInstance()->GetMarkPointMgr()->Remove(m_pHitMark);
	});
	connect(m_pActDeleteAll, &QAction::triggered, this, [&]{
		TimelineManager::GetInstance()->GetMarkPointMgr()->RemoveAll();
	});
	connect(m_pActColor, &ColorMenuItem::sigSelectedChanged, this, [&](QColor clr){
		TimelineManager::GetInstance()->GetMarkPointMgr()->SetColor(m_pHitMark, clr);
	});
	qApp->installEventFilter(this);
	RetranslateUi();
}

void TLMarkLayer::RetranslateUi()
{
	m_pActDelete->setText(tr("Delete This Marker"));
	m_pActDelete->setShortcut(QKeySequence::Delete);
	m_pActDeleteAll->setText(tr("Delete All Markers"));
}

MarkPoint* TLMarkLayer::IsHitMark(QPoint globalPt)
{
	auto ruler = TrackBuilder::GetInstance()->GetRuler();
	auto markPoints = TimelineManager::GetInstance()->GetMarkPointMgr()->GetMarkPoints();
	for (auto mark : markPoints)
	{
		int pos = ruler->Frames2DurationF(mark->GetFrame()) - ruler->Frames2DurationF(ruler->OffsetFrames());
		QPoint topLeft = mapToGlobal(QPoint(pos - m_MarkWidgetW * 0.5, 0));
		QRect rc(topLeft.x(),topLeft.y(), m_MarkWidgetW, m_MarkWidgetH);
		if (rc.contains(globalPt))
		{
			return mark.get();
		}
	}
	return nullptr;
}

void TLMarkLayer::OnRightClick()
{
	m_pMenu->clear();
	m_pMenu->addAction(m_pActDelete);
	m_pMenu->addAction(m_pActDeleteAll);
	m_pMenu->AddWidget(m_pActColor);
	m_pActColor->SetSelectColor(m_pHitMark->GetColor());
	m_pMenu->exec(QCursor::pos());
}

void TLMarkLayer::paintEvent(QPaintEvent *e)
{
	auto ruler = TrackBuilder::GetInstance()->GetRuler();
	auto markPoints = TimelineManager::GetInstance()->GetMarkPointMgr()->GetMarkPoints();

	static int triH = 5;
	QPainter p(this);
	p.setRenderHint(QPainter::Antialiasing);
	for (auto mptr : markPoints)
	{
		MarkPoint* mark = mptr.get();
		if (mark == m_pHitMark)
			mark = &m_MovingMark;

		int pos = ruler->Frames2PositionF(mark->GetFrame());
		pos += 1;
		QVector<QPoint> pts;
		pts << QPoint(pos, 0) 
			<< QPoint(pos + m_MarkWidgetW * 0.5, triH) 
			<< QPoint(pos + m_MarkWidgetW * 0.5, m_MarkWidgetH)
			<< QPoint(pos - m_MarkWidgetW * 0.5, m_MarkWidgetH)
			<< QPoint(pos - m_MarkWidgetW * 0.5, triH);
		QPainterPath path;
		path.addPolygon(QPolygon(pts));
		p.fillPath(path, QBrush(mark->GetColor()));

		if (!mark->IsSelected())
			p.fillPath(path, QBrush(QColor(0, 0, 0, 80)));
	}
}

bool TLMarkLayer::eventFilter(QObject *obj, QEvent *e)
{
	if (e->type() == QEvent::MouseButtonPress && obj == m_pTVTrackView)
	{
		m_pHitMark = IsHitMark(QCursor::pos());
		if (m_pHitMark)
		{
			m_bLeftClicked = ((QMouseEvent*)e)->button() == Qt::LeftButton;
			TimelineManager::GetInstance()->GetMarkPointMgr()->SetSelected(m_pHitMark, true);
			TimelineManager::GetInstance()->UnselectAll();
			TimelineManager::GetInstance()->GetClipMarkerMgr()->UnselectAll();
			m_MovingMark = *m_pHitMark;
			return true;
		}
		else
		{
			TimelineManager::GetInstance()->GetMarkPointMgr()->UnselectAll();
		}
	}
	else if (e->type() == QEvent::MouseButtonRelease && obj == m_pTVTrackView)
	{
		if (m_pHitMark )
		{
			if (m_bLeftClicked)
			{
				if (m_pHitMark->GetFrame() != m_MovingMark.GetFrame())
					TimelineManager::GetInstance()->GetMarkPointMgr()->Move(m_pHitMark, m_MovingMark.GetFrame());
				TimelineManager::GetInstance()->SetCurrentFrames(m_pHitMark->GetFrame());
				TrackBuilder::GetInstance()->GetRuler()->SetCurrentFrames(m_pHitMark->GetFrame());

				TimelineViewManager::GetInstance()->sigStartSeek();
				emit TimelineViewManager::GetInstance()->sigSeek(m_pHitMark->GetFrame());
				TimelineViewManager::GetInstance()->sigEndSeek();
			}
				
			else
				OnRightClick();

			m_pHitMark = nullptr;
			TrackBuilder::GetInstance()->UpdateItems();
			return true;
		}
	}
	else if (e->type() == QEvent::MouseMove && obj == m_pTVTrackView)
	{
		if (m_pHitMark)
		{
			if (m_bLeftClicked)
			{
				auto ruler = TrackBuilder::GetInstance()->GetRuler();
				auto pos = mapFromGlobal(QCursor::pos());
				m_MovingMark.SetFrame(ruler->Position2FramesF(pos.x()));
				update();
			}
			return true;
		}
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
