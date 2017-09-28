#include "stdafx.h"
#include "TVTrackView.h"
#include "TimelineManager.h"
#include "TVDraw.h"
#include "inc_CommonLib/TimelineRuler.h"
#include "TVControl.h"
#include "inc_FSCommonlib/FSNLETimeline.h"
#include "TimelineViewPrivate.h"
#include "inc_FSCommonlib/gathread.h"
#include "AudioWaveMgr.h"
#include "SelectionWidget.h"
extern QPoint RealPoint(QPoint p);
TVTrackView::TVTrackView(TimelineRuler *pRuler, QWidget *parent)
	: FSBaseWidget(parent)
{
	setObjectName("TimelineView_TrackView");
	m_bNormalDrag = true;
	m_bPressed = false;
	m_bEmptyPress = false;
	m_bDoHitSelect = false;
	m_bRectagleSelete = false;
	m_bTimelineDraging = false;
	m_bRightButtonPressed = false;
	m_bSetCursorHandCursor = false;
	m_bKeySpacePressed = false;
	m_pRuler = pRuler;
	m_pTimelineViewPrivate = qobject_cast<TimelineViewPrivate*>(parent);
	Q_ASSERT(m_pTimelineViewPrivate);
	m_pLine = new QWidget(this);
	m_pLine->setAttribute(Qt::WA_TransparentForMouseEvents);
	m_pLine->setStyleSheet("background-color:#27313B;");
	m_pTrackBuilder = TrackBuilder::GetInstance();
	m_pTrackBuilder->SetTrackView(this);
	m_pTrackBuilder->SetRuler(m_pRuler);
	m_pTimelineMgr = TimelineManager::GetInstance();
	m_pMenuContextMgr = new MenuContextMgr(this,this);
	qApp->installEventFilter(this);
	setAcceptDrops(true);
	m_pSelectionWidget = new SelectionWidget(this);
	TVDragControl::GetInstance()->Init(this);
	TVMouseEffectDragCtrl::GetInstance()->Init(this);
	
	connect(m_pTimelineMgr, &TimelineManager::signalTrackRebuild, this, &TVTrackView::slotRebuild);
	connect(m_pTimelineMgr, &TimelineManager::signalTrackUpdate, this, [&]{
		AudioWaveMgr::GetInstance()->SetNeedReflesh(true);
		slotUpdate();
	});
	connect(TimelineViewManager::GetInstance(), &TimelineViewManager::sigCBUpdateView, this, &TVTrackView::slotUpdate);

	connect(m_pRuler, &TimelineRuler::StartFramesChanged, TimelineViewManager::GetInstance(), &TimelineViewManager::sigStartSeek);
	connect(m_pRuler, &TimelineRuler::EndFramesChanged, this, [&]{
		TimelineViewManager::GetInstance()->sigEndSeek();
		m_pTimelineViewPrivate->EnableToolBar();
	});
	connect(m_pRuler, &TimelineRuler::sigSingleClickStartFramesChanged, this, [&]{
		//TimelineViewManager::GetInstance()->sigStartSeek();
	});
	connect(m_pRuler, &TimelineRuler::sigSingleClickEndFrameChanged, this, [&]{
		TimelineViewManager::GetInstance()->sigEndSeek();
		m_pTimelineViewPrivate->EnableToolBar();
	});
	connect(m_pRuler, &TimelineRuler::FramesChanged, this, &TVTrackView::OnCurrentFrameChanged); 


	auto scSelectAll = new QShortcut(QKeySequence::SelectAll, this);
	connect(scSelectAll, &QShortcut::activated, this, [&]{m_pTimelineMgr->SelectAllEditableClips(); });

	connect(&m_RightButtonTimer, &QTimer::timeout, this, &TVTrackView::OnRightButtonTimerOut);
	connect(&m_KeySpaceTimer, &QTimer::timeout, this, &TVTrackView::OnKeySpaceTimerOut);
	
	QTimer::singleShot(0, [&]{
		slotRebuild();
	});
}


TVTrackView::~TVTrackView()
{

}

void TVTrackView::Resize()
{
	resizeEvent(nullptr);
}

TimelineRuler* TVTrackView::Ruler()
{
	return m_pRuler;
}

void TVTrackView::ResizeGeometry(int l, int t, int w, int h)
{
	setGeometry(l, t, w, h);
	resizeEvent(nullptr);
}

int TVTrackView::Height()
{
	return m_pTrackBuilder->Height() + TRACK_WIDGET_HEIGHT;
}

bool TVTrackView::IsKeySpacePressed()
{
	return m_bKeySpacePressed;
}

bool TVTrackView::IsDragScroll()
{
	return m_bSetCursorHandCursor;
}

bool TVTrackView::CanDrag(QPoint pt)
{
	return TRACK_WIDGET_HEADER_WIDTH < pt.x();
}

void TVTrackView::dragEnterEvent(QDragEnterEvent *e)
{
	auto mimeData = e->mimeData();
	if (!mimeData->hasFormat("MatterView_BaseItem") && 
		!mimeData->hasFormat("TimelineView_ClipBase") && 
		!mimeData->hasFormat("MatterView_MouseEffect"))
	{
		Q_ASSERT(false);
		e->ignore();
		return;
	}

	if (mimeData->hasFormat("MatterView_MouseEffect"))
	{
		m_bNormalDrag = false;
		TVMouseEffectDragCtrl::GetInstance()->DragEnter(e);
	}
	else
	{
		m_bNormalDrag = true;
		if (!TVDragControl::GetInstance()->IsDraging())
		{
			TVDragControl::GetInstance()->SetDraginTrackIndex(m_nPressTrackIndex);
			TVDragControl::GetInstance()->SetDragEnterPoint(m_ptPressed);
		}
		TVDragControl::GetInstance()->DragEnter(e);
	}	
}

void TVTrackView::dragMoveEvent(QDragMoveEvent *e)
{
	if (m_bNormalDrag)
	{

		TVDragControl::GetInstance()->DragMove(e->pos());
		e->accept();
	}
	else
	{
		TVMouseEffectDragCtrl::GetInstance()->DragMove(e);
	}
	
}

void TVTrackView::dragLeaveEvent(QDragLeaveEvent *e)
{
	if (m_bNormalDrag)
	{
		TVDragControl::GetInstance()->DragLeave(e);
	}
	else
	{
		TVMouseEffectDragCtrl::GetInstance()->DragLeave(e);
	}
}

void TVTrackView::dropEvent(QDropEvent *e)
{
	emit TimelineViewManager::GetInstance()->sigUnselectMatterView();
	if (m_bNormalDrag)
	{
		TVDragControl::GetInstance()->DropDown();
		e->accept();
		mouseReleaseEvent(nullptr);
	}
	else
	{
		TVMouseEffectDragCtrl::GetInstance()->DropDown(e);
	}
}

//void TVTrackView::contextMenuEvent(QContextMenuEvent *e)
//{
//	if (!CanDrag(e->pos())) return;
//	m_pMenuContextMgr->Popup(e->pos());
//}

void TVTrackView::mousePressEvent(QMouseEvent *e)
{
	if (e->button() == Qt::MidButton)
	{
		m_pTimelineViewPrivate->Zoom2FitTimeline();
		return;
	}


	bool bRightButton = (e->button() == Qt::RightButton);
	if (bRightButton)
	{
		m_RightButtonDragElapseTimer.restart();
		m_RightButtonTimer.start(30);
		m_bRightButtonPressed = true;
		m_ptRightPressed = QCursor::pos();
		m_nHscrollBarValue = m_pTimelineViewPrivate->GetHScrollBar()->value();
		m_nVscrollBarValue = m_pTimelineViewPrivate->GetVScrollBar()->value();
		FSBaseWidget::mousePressEvent(e);
		return;
	}
	if (m_bKeySpacePressed)
	{
		m_ptPressed = QCursor::pos();
		m_nHscrollBarValue = m_pTimelineViewPrivate->GetHScrollBar()->value();
		m_nVscrollBarValue = m_pTimelineViewPrivate->GetVScrollBar()->value();
		FSBaseWidget::mousePressEvent(e);
		return;
	}
	

	m_bPressed = true;
	m_ptPressed = e->pos();
	bool bCtrl = GetKeyState(VK_CONTROL) & 0x8000;
	bool bShift = GetKeyState(VK_SHIFT) & 0x8000;
	TrackWidget *child = qobject_cast<TrackWidget*>(childAt(m_ptPressed));
	
	if (!child)
	{
		m_bEmptyPress = true;
		m_nPressTrackIndex = -1;
		if (bShift)
			m_pTimelineMgr->ShiftSelect(-1, 0);
		else if (bCtrl)
			m_pTimelineMgr->CtrlSelect(-1, 0);
		else
			m_pTimelineMgr->SingleSelect(-1, 0);
	}
	else
	{
		auto index = child->GetTrackData()->GetIndex();
		int nframe = m_pRuler->Position2FramesF(m_ptPressed.x() - TRACK_WIDGET_HEADER_WIDTH);
		m_nPressTrackIndex = index;

		auto hitClip = child->GetTrackData()->GetClip(nframe);
		if (!hitClip)
			m_bEmptyPress = true;

		if (m_bEmptyPress || (hitClip && !hitClip->GetSelected()))
		{
			m_bDoHitSelect = true;
			if (bShift)
				m_pTimelineMgr->ShiftSelect(index, nframe);
			else if (bCtrl)
				m_pTimelineMgr->CtrlSelect(index, nframe);
			else
				m_pTimelineMgr->SingleSelect(index, nframe);
		}
	}
	FSBaseWidget::mousePressEvent(e);
}

void TVTrackView::mouseMoveEvent(QMouseEvent *e)
{
	if (m_bRightButtonPressed)
	{
		auto hScrollBar = m_pTimelineViewPrivate->GetHScrollBar();
		auto vScrollBar = m_pTimelineViewPrivate->GetVScrollBar();
		if (((m_ptRightPressed - e->pos()).manhattanLength() > qApp->startDragDistance())
			&& (hScrollBar->isVisible()|| vScrollBar->isVisible()))
		{
			auto xD = QCursor::pos().x() - m_ptRightPressed.x();
			auto yD = QCursor::pos().y() - m_ptRightPressed.y();
			int xOffset = xD > 0 ? 0- Ruler()->Duration2FramesF(qAbs(xD)) :  Ruler()->Duration2FramesF(qAbs(xD));
			int yOffset = 0 - yD;
			hScrollBar->setValue(qBound(0, m_nHscrollBarValue + xOffset, hScrollBar->maximum()));
			vScrollBar->setValue(qBound(0, m_nVscrollBarValue + yOffset, vScrollBar->maximum()));
		}
		FSBaseWidget::mouseMoveEvent(e);
		return;
	}
	if (m_bKeySpacePressed)
	{
		auto hScrollBar = m_pTimelineViewPrivate->GetHScrollBar();
		auto vScrollBar = m_pTimelineViewPrivate->GetVScrollBar();
		if (((m_ptPressed - e->pos()).manhattanLength() > qApp->startDragDistance())
			&& (hScrollBar->isVisible() || vScrollBar->isVisible()))
		{
			auto xD = QCursor::pos().x() - m_ptPressed.x();
			auto yD = QCursor::pos().y() - m_ptPressed.y();
			int xOffset = xD > 0 ? 0 - Ruler()->Duration2FramesF(qAbs(xD)) : Ruler()->Duration2FramesF(qAbs(xD));
			int yOffset = 0 - yD;
			hScrollBar->setValue(qBound(0, m_nHscrollBarValue + xOffset, hScrollBar->maximum()));
			vScrollBar->setValue(qBound(0, m_nVscrollBarValue + yOffset, vScrollBar->maximum()));
		}
		FSBaseWidget::mouseMoveEvent(e);
		return;
	}


	TrackWidget *child = qobject_cast<TrackWidget*>(childAt(e->pos()));
	if (m_bEmptyPress)
	{
		int nTrackFrom = m_nPressTrackIndex;
		int nFrameFrom = m_pRuler->Position2FramesF(m_ptPressed.x() - TRACK_WIDGET_HEADER_WIDTH);
		int nTrackTo = child ? child->GetTrackData()->GetIndex() : -1;
		int nFrameTo = m_pRuler->Position2FramesF(e->pos().x() - TRACK_WIDGET_HEADER_WIDTH);

		if (nTrackFrom < 0 && nTrackTo < 0 && e->pos().y() < height())
		{
			m_pSelectionWidget->Show(QRect(m_ptPressed, e->pos()), true);
			return;
		}
			
		if (nTrackFrom < 0)
			nTrackFrom = m_pTimelineMgr->GetTrackCount() - 1;
		if (nTrackTo < 0)
			nTrackTo = e->pos().y() > height() ? 0 : m_pTimelineMgr->GetTrackCount() - 1;
		nFrameTo = qMax(nFrameTo, 0);

		if (!m_bRectagleSelete)
		{
			m_bRectagleSelete = true;
			m_pTimelineMgr->BeginRectangleSelect();
		}
		m_pTimelineMgr->RectangleSelect(qMin(nTrackFrom, nTrackTo), qMax(nTrackFrom, nTrackTo), qMin(nFrameFrom, nFrameTo), qMax(nFrameFrom, nFrameTo));
		m_pSelectionWidget->Show(QRect(m_ptPressed, e->pos()), true);
	}

	if (m_bPressed && !m_bEmptyPress  && (m_ptPressed - e->pos()).manhattanLength() > qApp->startDragDistance())
	{
		bool bSelected = m_pTimelineMgr->GetSelected().size() > 0;
		if (child && bSelected)
		{
			m_bTimelineDraging = true;
			auto mimeData = new QMimeData;
			mimeData->setData("TimelineView_ClipBase", "");
			QDrag drag(this);
			drag.setMimeData(mimeData);
			drag.exec();
		}
	}
	FSBaseWidget::mouseMoveEvent(e);
}

void TVTrackView::mouseReleaseEvent(QMouseEvent *e)
{
	bool bRightButton = e ? (e->button() == Qt::RightButton) : false;
	if (bRightButton)
	{
		m_bRightButtonPressed = false;
		m_ptRightPressed = QPoint(INT_MAX, INT_MAX);
		if (m_bSetCursorHandCursor)
		{
			SetHandCursor(false);
			//m_bSetCursorHandCursor = false;
			//m_pTimelineViewPrivate->GetTopMaskView()->setCursor(Qt::ArrowCursor);
			//qApp->restoreOverrideCursor();
		}
		FSBaseWidget::mouseReleaseEvent(e);
		if (m_RightButtonDragElapseTimer.elapsed() < 300)
		{
			QPoint pt = e->pos();
			TrackWidget *child = qobject_cast<TrackWidget*>(childAt(pt));
			if (!child)
			{
				m_pTimelineMgr->SingleSelect(-1, 0);
			}
			else
			{
				if (!child->GetTrackData()->IsEditable())
				{
					m_pTimelineMgr->SingleSelect(-1, 0);
				}
				else
				{
					auto clip = child->IsHitClip(pt);
					if (!clip)
						m_pTimelineMgr->SingleSelect(-1, 0);
					else
					{
						if (!clip->GetSelected())
							m_pTimelineMgr->SingleSelect(clip->GetTimelineTrack()->GetIndex(), clip->GetStartAtTrack());
					}
				}
			}
			QTimer::singleShot(0, [this,pt]{OnMenuPopup(pt); });
		}
		m_RightButtonDragElapseTimer.invalidate();
		m_RightButtonTimer.stop();
		return;
	}

	if (m_bKeySpacePressed)
	{
		m_ptPressed = QPoint(INT_MAX, INT_MAX);
		FSBaseWidget::mouseReleaseEvent(e);
		return;
	}

	if (!m_bDoHitSelect && !m_bRectagleSelete && !m_bTimelineDraging)
	{
		bool bCtrl = GetKeyState(VK_CONTROL) & 0x8000;
		bool bShift = GetKeyState(VK_SHIFT) & 0x8000;
		TrackWidget *child = qobject_cast<TrackWidget*>(childAt(m_ptPressed));
		if (child)
		{
			auto index = child->GetTrackData()->GetIndex();
			int nframe = m_pRuler->Position2FramesF(m_ptPressed.x() - TRACK_WIDGET_HEADER_WIDTH);
			if (bShift)
				m_pTimelineMgr->ShiftSelect(index, nframe);
			else if (bCtrl)
				m_pTimelineMgr->CtrlSelect(index, nframe);
			else
				m_pTimelineMgr->SingleSelect(index, nframe);
		}
	}
	if (m_bRectagleSelete)
		m_pTimelineMgr->EndRectangleSelect();

	m_bPressed = false;
	m_ptPressed = QPoint(INT_MAX,INT_MAX);
	m_bEmptyPress = false;
	m_bDoHitSelect = false;
	m_bRectagleSelete = false;
	m_bTimelineDraging = false;
	m_pSelectionWidget->Show(QRect(), false);
	if(e)FSBaseWidget::mouseReleaseEvent(e);
}

void TVTrackView::resizeEvent(QResizeEvent *e)
{
	TrackWidgets trackWidgets = m_pTrackBuilder->GetTrackWidgets();
	int t = 0;
	int size = trackWidgets.size();
	for (int i = 0; i < size; ++i)
	{
		if (i == 0)
			t = height() - trackWidgets[i]->Height();
		else
			t = trackWidgets[i - 1]->geometry().top() - trackWidgets[i]->Height();
		trackWidgets[i]->setGeometry(0, t, width(), trackWidgets[i]->Height());

		qDebug() << "TrackWidget_bottom:" << mapToGlobal(trackWidgets[i]->geometry().bottomRight());
	}
	m_pLine->setGeometry(TRACK_WIDGET_HEADER_WIDTH - 1, 0, 1, height());

	m_pSelectionWidget->raise();
	m_pSelectionWidget->setGeometry(rect());
}

bool TVTrackView::eventFilter(QObject *obj, QEvent *e)
{
	if (e->type() == QEvent::KeyPress)
	{
		QKeyEvent *keyEvent = (QKeyEvent *)e;
		if (keyEvent->key() == Qt::Key_Space 
			&& !keyEvent->isAutoRepeat() 
			&& TVDragControl::GetInstance()->GetViewportRect().contains(QCursor::pos()))
		{
			if (TVTrimControl::GetInstance()->IsTriming() ||
				TVDragControl::GetInstance()->IsDraging() ||
				m_bRectagleSelete)
				return false;

			if (!m_bKeySpacePressed)
			{
				m_bKeySpacePressed = true;
				m_KeySpaceTimer.start(30);
				m_KeySpaceElapseTimer.restart();
			}
		}
		else if (keyEvent->key() == Qt::Key_Control && !keyEvent->isAutoRepeat())
		{
			auto widgets = TrackBuilder::GetInstance()->GetTrackWidgets();
			for (auto w : widgets)
			{
				w->SetKeyControlDown(true);
			}
		}
	}
	else if (e->type() == QEvent::KeyRelease)
	{
		QKeyEvent *keyEvent = (QKeyEvent *)e;
		if (keyEvent->key() == Qt::Key_Space 
			&& !keyEvent->isAutoRepeat())
		{
			m_bKeySpacePressed = false;
			m_KeySpaceTimer.stop();
			m_KeySpaceElapseTimer.invalidate();
			if (m_bSetCursorHandCursor)
			{
				SetHandCursor(false);
				//m_bSetCursorHandCursor = false;
				//qDebug() << "QEvent::KeyRelease, start timer restoreOverrideCursor";
				//qApp->restoreOverrideCursor();
				//m_pTimelineViewPrivate->GetTopMaskView()->setCursor(Qt::ArrowCursor);
			}
		}
		else if (keyEvent->key() == Qt::Key_Control && !keyEvent->isAutoRepeat())
		{
			auto widgets = TrackBuilder::GetInstance()->GetTrackWidgets();
			for (auto w : widgets)
			{
				w->SetKeyControlDown(false);
			}
		}
	}
	return false;
}

void TVTrackView::OnMenuPopup(QPoint pt)
{
	m_pMenuContextMgr->Popup(pt);
}

void TVTrackView::slotRebuild()
{
	m_pTrackBuilder->Rebuild();
	m_pTimelineViewPrivate->Resize();
	CheckDuration();
}

void TVTrackView::slotUpdate()
{
	m_pTrackBuilder->UpdateItems();
	CheckDuration();
}

void TVTrackView::OnCurrentFrameChanged(int frame)
{
	if (frame == m_pTimelineMgr->GetCurrentFrames()) return;
	m_pTimelineMgr->SetCurrentFrames(frame);
	TrackBuilder::GetInstance()->UpdateItems();
	emit TimelineViewManager::GetInstance()->sigSeek(frame); 
	
}

void TVTrackView::CheckDuration()
{
	static int nPreDuration = m_pTrackBuilder->Duration();
	int curDuration = m_pTrackBuilder->Duration();
	if (curDuration != nPreDuration)
	{
		nPreDuration = curDuration;
		emit sigDurationChanged(curDuration);
		emit TimelineViewManager::GetInstance()->sigDurationChanged(curDuration);
	}
}

void TVTrackView::OnRightButtonTimerOut()
{
	if (m_RightButtonDragElapseTimer.elapsed() > 300 && m_bRightButtonPressed)
	{
		if (!m_bSetCursorHandCursor)
		{
			//m_bSetCursorHandCursor = true;
			SetHandCursor(true);
			//m_pTimelineViewPrivate->GetTopMaskView()->setCursor(Qt::ClosedHandCursor);
			//qApp->setOverrideCursor(Qt::ClosedHandCursor);
			m_RightButtonTimer.stop();
		}
	}
}

void TVTrackView::OnKeySpaceTimerOut()
{
	if (m_KeySpaceElapseTimer.elapsed() > 300 && m_bKeySpacePressed)
	{
		qDebug() << "OnKeySpaceTimerOut";
		if (!m_bSetCursorHandCursor)
		{
			//qDebug() << "setOverrideCursor";
			//m_bSetCursorHandCursor = true;
			SetHandCursor(true);
			//qApp->setOverrideCursor(Qt::ClosedHandCursor);
			//m_pTimelineViewPrivate->GetTopMaskView()->setCursor(Qt::ClosedHandCursor);
			m_KeySpaceTimer.stop();
		}
	}
}

void TVTrackView::SetHandCursor(bool bSet)
{
	if (m_bSetCursorHandCursor == bSet)
		return;
	m_bSetCursorHandCursor = bSet;
	for (auto w : m_pTrackBuilder->GetTrackWidgets())
		w->setCursor(bSet ? Qt::ClosedHandCursor : Qt::ArrowCursor);
	setCursor(bSet ? Qt::ClosedHandCursor : Qt::ArrowCursor);
}

//////////////////////////////////////////////////////////////////////////
TrackWidget::TrackWidget(TimelineTrack* pTrackData, QWidget *parent)
	:FSBaseWidget(parent)
{
	m_bKeyControlDown = false;
	m_pTrackData = pTrackData;
	m_btnHide = new BaseStateButton(0,"TimelineView_TrackWidget_HideStateButton",this);
	m_btnHide->AddButton(0, "TimelineView_TrackWidget_HideButton");
	m_btnHide->AddButton(1, "TimelineView_TrackWidget_ShowButton");
	connect(m_btnHide->GetButton(0), &BaseButton::clicked, this, &TrackWidget::OnHide);
	connect(m_btnHide->GetButton(1), &BaseButton::clicked, this, &TrackWidget::OnShow);

	m_btnFreeze = new BaseStateButton(1, "TimelineView_TrackWidget_FreezeStateButton", this);
	m_btnFreeze->AddButton(0, "TimelineView_TrackWidget_FreezeButton");
	m_btnFreeze->AddButton(1, "TimelineView_TrackWidget_UnFreezeButton");
	connect(m_btnFreeze->GetButton(0), &BaseButton::clicked, this, &TrackWidget::OnFreeze);
	connect(m_btnFreeze->GetButton(1), &BaseButton::clicked, this, &TrackWidget::OnUnFreeze);

	m_pDraw = new TVTrackWidgetDraw(this);
	m_pDraw->SetTrackWidget(this);
	setMouseTracking(true);

	m_pClipMarkerControl = new ClipMarkerControl(this, this);

	m_btnHide->SetCurrentButtonId(m_pTrackData->IsHiden() ? 1 : 0);
	m_btnFreeze->SetCurrentButtonId(m_pTrackData->IsFreeze() ? 1 : 0);

    RetranslateUi();
}

void TrackWidget::SetRuler(TimelineRuler *pRuler)
{
	m_pRuler = pRuler;
	m_pDraw->SetRuler(pRuler);
}

TimelineTrack* TrackWidget::GetTrackData()
{
	return m_pTrackData;
}

Clips TrackWidget::GetClips()
{
	return m_pTrackData->GetClips();
}

Clips TrackWidget::GetDragClips()
{
	Clips res;
	if (m_pTrackData)
	{
		Clips clips = m_pTrackData->GetClips();
		auto draginClips = TVDragControl::GetInstance()->GetDraginUIClips();

		if (draginClips.GetDragType() == UIDragClips::DragType_Timeline)
		{
			for (auto clipbase : draginClips.GetClipsList())
			{
				if (clips.contains(clipbase.clip))
					res.push_back(clipbase.clip);
			}
		}
	}
	return res;
}

Clips TrackWidget::GetUnDragClips()
{
	Clips clips;
	if (m_pTrackData)
	{
		clips = m_pTrackData->GetClips();
		auto draginClips = TVDragControl::GetInstance()->GetDraginUIClips();

		if (draginClips.GetDragType() == UIDragClips::DragType_Timeline)
		{
			for (auto clipbase : draginClips.GetClipsList())
			{
				clips.removeOne(clipbase.clip);
			}
		}
	}
	return clips;
}

int TrackWidget::Duration()
{
	return m_pTrackData->GetDuration();
}


TrackWidget * TrackWidget::PreTrackWidget()
{
	return TrackBuilder::GetInstance()->GetTrackWidget(m_pTrackData->GetIndex() - 1);
}

TrackWidget * TrackWidget::NextTrackWidget()
{
	return TrackBuilder::GetInstance()->GetTrackWidget(m_pTrackData->GetIndex() + 1);
}

int TrackWidget::Height()
{
	return TRACK_WIDGET_HEIGHT;
}

void TrackWidget::SetButtonHide(bool bHide)
{
	int id = bHide ? 1 : 0;
	if (m_btnHide->CurrentButtonId() == id)
		return;
	m_btnHide->SetCurrentButtonId(id);
	
}

void TrackWidget::SetButtonFreeze(bool bFreeze)
{
	int id = bFreeze ? 1 : 0;
	if (m_btnFreeze->CurrentButtonId() == id)
		return;
	m_btnFreeze->SetCurrentButtonId(id);
}

void TrackWidget::OnHide()
{
	TimelineManager::GetInstance()->HideTrack(m_pTrackData->GetIndex());
}

void TrackWidget::OnShow()
{
	TimelineManager::GetInstance()->ShowTrack(m_pTrackData->GetIndex());
}

void TrackWidget::OnFreeze()
{
	TimelineManager::GetInstance()->FreezeTrack(m_pTrackData->GetIndex());
}

void TrackWidget::OnUnFreeze()
{
	TimelineManager::GetInstance()->UnfreezeTrack(m_pTrackData->GetIndex());
}

void TrackWidget::mousePressEvent(QMouseEvent *e)
{
	if (IsHitHeader(e->pos()))
		return;

	bool bRightButton = (e->button() == Qt::RightButton);
	if (bRightButton)
	{
		FSBaseWidget::mousePressEvent(e);
		return;
	}

	if (((TVTrackView*)parent())->IsKeySpacePressed())
	{
		FSBaseWidget::mousePressEvent(e);
		return;
	}

	if (!m_pTrackData->IsEditable())
	{
		if (!IsHitClip(e->pos()))
		{
			FSBaseWidget::mousePressEvent(e);
		}
		return;
	}

	if (IsKeyControlDown()/*GetKeyState(VK_CONTROL) & 0x8000*/)
	{
		ChangeSpeedControl::GetInstance()->StartChange(e->pos(), this);
	}
	else
	{
		TVTrimControl::GetInstance()->StartTrim(e->pos(), this);
	}
	
	if (!TVTrimControl::GetInstance()->IsTriming() && !ChangeSpeedControl::GetInstance()->IsChanging())
		FSBaseWidget::mousePressEvent(e);
}

void TrackWidget::mouseMoveEvent(QMouseEvent *e)
{
	if (TVTrimControl::GetInstance()->IsTriming())
		TVTrimControl::GetInstance()->MoveTrim(e->pos(), this);
	else if (ChangeSpeedControl::GetInstance()->IsChanging())
		ChangeSpeedControl::GetInstance()->MoveChange(e->pos(), this);
	else
	{
		IsKeyControlDown() ? ChangeSpeedControl::GetInstance()->MoveChange(e->pos(), this) : TVTrimControl::GetInstance()->MoveTrim(e->pos(), this);
	}

	if (!TVTrimControl::GetInstance()->IsTriming() && !ChangeSpeedControl::GetInstance()->IsChanging())
		FSBaseWidget::mouseMoveEvent(e);
}

void TrackWidget::mouseReleaseEvent(QMouseEvent *e)
{
	bool bRightButton = (e->button() == Qt::RightButton);
	if (bRightButton)
	{
		FSBaseWidget::mouseReleaseEvent(e);
		return;
	}
	if (((TVTrackView*)parent())->IsKeySpacePressed())
	{
		FSBaseWidget::mouseReleaseEvent(e);
		return;
	}

	if (ChangeSpeedControl::GetInstance()->IsChanging())
		ChangeSpeedControl::GetInstance()->EndChange(e->pos());
	else if (TVTrimControl::GetInstance()->IsTriming())
		TVTrimControl::GetInstance()->EndTrim(e->pos());
	FSBaseWidget::mouseReleaseEvent(e);
}

void TrackWidget::mouseDoubleClickEvent(QMouseEvent *e)
{
	if (!m_pTrackData->IsEditable())
		return;
	if (e->button() == Qt::LeftButton)
	{
		if (IsHitHeader(e->pos()))
		{
			TimelineManager::GetInstance()->SelectTrack(m_pTrackData->GetIndex());
		}
		else
		{
			auto clip = m_pTrackData->GetClip(m_pRuler->Position2FramesF(e->pos().x() - TRACK_WIDGET_HEADER_WIDTH));
			if (clip)
			{
				m_pRuler->SetCurrentFrames(clip->GetStartAtTrack());
				TimelineViewManager::GetInstance()->sigStartSeek();
				((TVTrackView*)parent())->OnCurrentFrameChanged(clip->GetStartAtTrack());
				TimelineViewManager::GetInstance()->sigEndSeek();
			}
		}
	}
	TrackBuilder::GetInstance()->UpdateItems();
}

void TrackWidget::paintEvent(QPaintEvent *e)
{
	QPainter p(this);
	m_pDraw->Draw(&p, rect());
}

void TrackWidget::resizeEvent(QResizeEvent *e)
{
	static const int w = 18;
	static const int h = 18;
	static const int padding = 5;
	
	int l = TRACK_WIDGET_HEADER_WIDTH - w - 5;
	int t = (Height() - w - h - padding) * 0.5;
	m_btnHide->setGeometry(l,t,w,h);

	t = m_btnHide->geometry().bottom() + padding;
	m_btnFreeze->setGeometry(l, t, w, h);
}

void TrackWidget::RetranslateUi()
{
    m_btnHide->setToolTip(tr("Toggle track output"));
    m_btnFreeze->setToolTip(tr("Toggle track lock"));

}

Clip* TrackWidget::IsHitClip(QPoint pt)
{
	auto realPt = RealPoint(pt);
	auto xFrame = m_pRuler->Position2FramesF(realPt.x());
	auto clips = m_pTrackData->GetClips();
	for (auto clip : clips)
	{
		QRect rcClip(clip->GetStartAtTrack(), 0, clip->GetLenAtTrack(), 2);
		if (rcClip.contains(QPoint(xFrame,1)))
		{
			return clip;
		}
	}
	return nullptr;
}

void TrackWidget::SetKeyControlDown(bool bDown)
{
	if (m_bKeyControlDown == bDown)
		return;
	m_bKeyControlDown = bDown;
	if (m_bKeyControlDown)
	{
		if (!TVTrimControl::GetInstance()->IsTriming() &&
			!ChangeSpeedControl::GetInstance()->IsChanging())
		{
			ChangeSpeedControl::GetInstance()->MoveChange(mapFromGlobal(QCursor::pos()), this);
		}
	}
	else
	{
		if (!TVTrimControl::GetInstance()->IsTriming() &&
			!ChangeSpeedControl::GetInstance()->IsChanging())
		{
			TVTrimControl::GetInstance()->MoveTrim(mapFromGlobal(QCursor::pos()), this);
		}
	}
}

bool TrackWidget::IsKeyControlDown()
{
	return m_bKeyControlDown;
}

QSize TrackWidget::GetMarkerSize()
{
	return m_pDraw->GetMarkerSize();
}

int TrackWidget::GetMarkerTop()
{
	return m_pDraw->GetMarkerTop();
}

bool TrackWidget::IsHitHeader(QPoint pt)
{
	if (pt.x() < TRACK_WIDGET_HEADER_WIDTH)
		return true;
	return false;
}



//////////////////////////////////////////////////////////////////////////
MenuContextMgr::MenuContextMgr(QObject *parent, TVTrackView* pWidget)
	:QObject(parent)
{
	m_nCurTrackIndex = 0;
	m_pTVTrackView = pWidget;
	m_pTimelineMgr = TimelineManager::GetInstance();

	m_Menu = new CusMenu(m_pTVTrackView);
	m_Menu->setFixedWidth(320);
	m_Menu->SetLeftPadding(20);
	m_pActCut = new QAction(m_pTVTrackView);
	m_pActCopy = new QAction(m_pTVTrackView);
	m_pActPaste = new QAction(m_pTVTrackView);
	m_pActDelete = new QAction(m_pTVTrackView);
	m_pActSplit = new QAction(m_pTVTrackView);
	m_pActMuteAudio = new QAction(m_pTVTrackView);
	m_pActMuteAudio->setCheckable(true);
	m_pActAudioDetach = new QAction(m_pTVTrackView);
	m_pActFreezeFrame = new QAction(m_pTVTrackView);
	m_pActAddMarker = new QAction(m_pTVTrackView);

	m_pActInserTrack = new QAction(m_pTVTrackView);
	m_pActRemoveEmptyTrack = new QAction(m_pTVTrackView);

	m_pActSelectGroup = new QAction(m_pTVTrackView);
	m_pColorMenuItem = new ColorMenuItem(m_pTVTrackView);
	auto colorList = TimelineManager::GetInstance()->GetClipColorList();
	for (auto clipClr : colorList)
	{
		static int index = 1;
		m_pColorMenuItem->AddColor(clipClr.clr);

		auto sctext = QString("Alt+%1").arg(index++);
		m_pColorMenuItem->SetToolTip(clipClr.clr, sctext);

		auto scSelectColor = new QShortcut(m_pTVTrackView);
		scSelectColor->setKey(QKeySequence(sctext));
		scSelectColor->setContext(Qt::ApplicationShortcut);
		QColor clr = clipClr.clr;
		connect(scSelectColor, &QShortcut::activated, this, [&,clr]{
			auto selected = TimelineManager::GetInstance()->GetSelected();
			for (auto clip : selected)
			{
				clip->GetNLETimeline()->SetBackgroundColor(clr, false);
			}
			TrackBuilder::GetInstance()->UpdateItems();
		});
	}

	auto scSelectGroup = new QShortcut(m_pTVTrackView);
	scSelectGroup->setKey(QKeySequence("Alt+`"));
	scSelectGroup->setContext(Qt::ApplicationShortcut);
	connect(scSelectGroup, &QShortcut::activated, this, [&]{
		auto selected = TimelineManager::GetInstance()->GetSelected();
		bool bCanChange = false;
		QColor clr;
		for (int i = 0; i < selected.size(); ++i)
		{
			if (i == 0)
			{
				bCanChange = true;
				clr = selected[i]->GetNLETimeline()->GetBackgroundColor();
			}
			else
			{
				if (clr != selected[i]->GetNLETimeline()->GetBackgroundColor())
				{
					bCanChange = false;
					break;
				}
			}
		}
		if(bCanChange) 
			TimelineManager::GetInstance()->SelectColorGroup(selected[0]);
	});

	m_pActVisible = new QAction(m_pTVTrackView);
	m_pActUnvisible = new QAction(m_pTVTrackView);
	m_pActLock = new QAction(m_pTVTrackView);
	m_pActUnlock = new QAction(m_pTVTrackView);
	ConnectSignals();
}

void MenuContextMgr::Popup(QPoint pos)
{
	m_Menu->clear();
	TrackWidget *child = qobject_cast<TrackWidget*>(m_pTVTrackView->childAt(pos));
	if (child && child->GetTrackData()->IsEditable())
	{
		m_nCurTrackIndex = child->GetTrackData()->GetIndex();
		int nPos = pos.x() - TRACK_WIDGET_HEADER_WIDTH;
		int nFrame = m_pTVTrackView->Ruler()->Position2FramesF(nPos);
		auto clip = child->GetTrackData()->GetClip(nFrame);
		if (clip)
		{
			m_Menu->setProperty("clip", qptrdiff(clip));
			m_Menu->addAction(m_pActCut);
			m_Menu->addAction(m_pActCopy);
			m_Menu->addAction(m_pActPaste);
			m_Menu->addAction(m_pActDelete);
			m_Menu->addSeparator();
			m_Menu->addAction(m_pActSplit);

			m_pActCut->setEnabled(m_pTimelineMgr->CanCut());
			m_pActCopy->setEnabled(m_pTimelineMgr->CanCopy());
			m_pActPaste->setEnabled(m_pTimelineMgr->CanPaste());
			m_pActSplit->setEnabled(m_pTimelineMgr->CanSplit());

			if (clip->GetNLETimeline()->GetType() == TimelineType_VideoAudio)
			{
				m_Menu->addSeparator();
				m_Menu->addAction(m_pActMuteAudio);
				m_Menu->addAction(m_pActAudioDetach);

				bool bCanMute, bHadMute;
				m_pTimelineMgr->CanMute(clip, bCanMute, bHadMute);
				m_pActMuteAudio->setEnabled(bCanMute);
				m_pActMuteAudio->setChecked(bCanMute && bHadMute);
				m_pActAudioDetach->setEnabled(m_pTimelineMgr->CanDetack(clip));
			}

			if (m_pTimelineMgr->CanFreezeFrame(clip))
			{
				m_pActFreezeFrame->setProperty("clip", qptrdiff(clip));
				m_Menu->addAction(m_pActFreezeFrame);
			}

			if (m_pTimelineMgr->GetClipMarkerMgr()->CanAddMarker(clip->GetNLETimeline(),m_pTimelineMgr->GetCurrentFrames()))
			{
				m_pActAddMarker->setProperty("clip", qptrdiff(clip));
				m_Menu->addAction(m_pActAddMarker);
			}

			m_Menu->addSeparator();
			m_pActSelectGroup->setProperty("clip", qptrdiff(clip));
			m_Menu->addAction(m_pActSelectGroup);
			m_pColorMenuItem->setProperty("clip", qptrdiff(clip));
			m_pColorMenuItem->SetSelectColor(clip->GetNLETimeline()->GetBackgroundColor());
			m_Menu->AddWidget(m_pColorMenuItem);
		}
		else
		{
			m_Menu->addAction(m_pActInserTrack);
			m_Menu->addAction(m_pActRemoveEmptyTrack);
			m_Menu->addAction(m_pActPaste);
			m_pActRemoveEmptyTrack->setEnabled(m_pTimelineMgr->HasEmptyTrack());
			m_pActPaste->setEnabled(m_pTimelineMgr->CanPaste());
			
			m_Menu->addSeparator();
			m_Menu->addAction(m_pActVisible);
			m_Menu->addAction(m_pActUnvisible);
			m_Menu->addAction(m_pActLock);
			m_Menu->addAction(m_pActUnlock);
			m_pActVisible->setEnabled(child->GetTrackData()->IsHiden());
			m_pActVisible->setProperty("trackID", child->GetTrackData()->GetIndex());
			m_pActUnvisible->setEnabled(!child->GetTrackData()->IsHiden());
			m_pActUnvisible->setProperty("trackID", child->GetTrackData()->GetIndex());
			m_pActLock->setEnabled(!child->GetTrackData()->IsFreeze());
			m_pActLock->setProperty("trackID", child->GetTrackData()->GetIndex());
			m_pActUnlock->setEnabled(child->GetTrackData()->IsFreeze());
			m_pActUnlock->setProperty("trackID", child->GetTrackData()->GetIndex());

		}
	}
	else if (child && !child->GetTrackData()->IsEditable())
	{
		m_Menu->addAction(m_pActRemoveEmptyTrack);
		m_Menu->addAction(m_pActPaste);
		m_pActRemoveEmptyTrack->setEnabled(m_pTimelineMgr->HasEmptyTrack());
		m_pActPaste->setEnabled(m_pTimelineMgr->CanPaste());
		m_Menu->addSeparator();
		m_Menu->addAction(m_pActVisible);
		m_Menu->addAction(m_pActUnvisible);
		m_Menu->addAction(m_pActLock);
		m_Menu->addAction(m_pActUnlock);
		m_pActVisible->setEnabled(child->GetTrackData()->IsHiden());
		m_pActVisible->setProperty("trackID", child->GetTrackData()->GetIndex());
		m_pActUnvisible->setEnabled(!child->GetTrackData()->IsHiden());
		m_pActUnvisible->setProperty("trackID", child->GetTrackData()->GetIndex());
		m_pActLock->setEnabled(!child->GetTrackData()->IsFreeze());
		m_pActLock->setProperty("trackID", child->GetTrackData()->GetIndex());
		m_pActUnlock->setEnabled(child->GetTrackData()->IsFreeze());
		m_pActUnlock->setProperty("trackID", child->GetTrackData()->GetIndex());
	}
	else
	{
		m_Menu->addAction(m_pActRemoveEmptyTrack);
		m_Menu->addAction(m_pActPaste);
		m_pActRemoveEmptyTrack->setEnabled(m_pTimelineMgr->HasEmptyTrack());
		m_pActPaste->setEnabled(m_pTimelineMgr->CanPaste());
	}
	RetranslateUi();
	m_Menu->exec(m_pTVTrackView->mapToGlobal(pos));
}

void MenuContextMgr::ConnectSignals()
{
	connect(m_pActCut, &QAction::triggered, this, [&]{
		m_pTimelineMgr->Cut();
	});
	connect(m_pActCopy, &QAction::triggered, this, [&]{
		m_pTimelineMgr->Copy();
	});
	connect(m_pActPaste, &QAction::triggered, this, [&]{
		m_pTimelineMgr->Paste();
	});
	connect(m_pActDelete, &QAction::triggered, this, [&]{
		m_pTimelineMgr->RemoveClips();
	});
	connect(m_pActSplit, &QAction::triggered, this, [&]{
		m_pTimelineMgr->Split();
	});
	connect(m_pActMuteAudio, &QAction::triggered, this, [&]{
		m_pTimelineMgr->Mute(m_pActMuteAudio->isChecked());
		//上传GA数据
		GAThread::getInstance()->insertEventItem(Clip_Edit,"Mute","");
	});
	connect(m_pActAudioDetach, &QAction::triggered, this, [&]{
		m_pTimelineMgr->Detach();
		//上传GA数据
		GAThread::getInstance()->insertEventItem(Clip_Edit, "Audio Detach", "");
	});
	connect(m_pActFreezeFrame, &QAction::triggered, this, [&]{
		auto clip = reinterpret_cast<Clip*>(m_Menu->property("clip").value<qptrdiff>());
		m_pTimelineMgr->FreezeFrame(clip);
	});

	connect(m_pActAddMarker, &QAction::triggered, this, [&]{
		auto clip = reinterpret_cast<Clip*>(m_pActAddMarker->property("clip").value<qptrdiff>());
		m_pTimelineMgr->GetClipMarkerMgr()->Add(clip->GetNLETimeline(), m_pTimelineMgr->GetCurrentFrames());
	});

	connect(m_pActInserTrack, &QAction::triggered, this, [&]{
		m_pTimelineMgr->InsertTrack(m_nCurTrackIndex);
	});
	connect(m_pActRemoveEmptyTrack, &QAction::triggered, this, [&]{
		m_pTimelineMgr->RemoveAllEmptyTrack();
	});

	connect(m_pActVisible, &QAction::triggered, this, [&]{
		m_pTimelineMgr->ShowTrack(m_pActVisible->property("trackID").toInt());
	});
	connect(m_pActUnvisible, &QAction::triggered, this, [&]{
		m_pTimelineMgr->HideTrack(m_pActVisible->property("trackID").toInt());
	});
	connect(m_pActLock, &QAction::triggered, this, [&]{
		m_pTimelineMgr->FreezeTrack(m_pActVisible->property("trackID").toInt());
	});
	connect(m_pActUnlock, &QAction::triggered, this, [&]{
		m_pTimelineMgr->UnfreezeTrack(m_pActVisible->property("trackID").toInt());
	});
	
	connect(m_pActSelectGroup, &QAction::triggered, this, [&]{
		auto clip = reinterpret_cast<Clip*>(m_pActSelectGroup->property("clip").value<qptrdiff>());
		TimelineManager::GetInstance()->SelectColorGroup(clip);
	});
	
	connect(m_pColorMenuItem, &ColorMenuItem::sigSelectedChanged, this, [&](QColor clr){
		auto clips = TimelineManager::GetInstance()->GetSelected();
		for (auto clip : clips)
			clip->GetNLETimeline()->SetBackgroundColor(clr, false);
		TrackBuilder::GetInstance()->UpdateItems();
	});
}

void MenuContextMgr::RetranslateUi()
{
	m_pActCut->setText(tr("Cut"));
	m_pActCut->setShortcut(QKeySequence::Cut);
	m_pActCopy->setText(tr("Copy"));
	m_pActCopy->setShortcut(QKeySequence::Copy);
	m_pActPaste->setText(tr("Paste"));
	m_pActPaste->setShortcut(QKeySequence::Paste);
	m_pActDelete->setText(tr("Delete"));
	m_pActDelete->setShortcut(QKeySequence::Delete);
	m_pActSplit->setText(tr("Split"));
	m_pActMuteAudio->setText(tr("Mute audio"));
	m_pActAudioDetach->setText(tr("Audio detach"));
	m_pActFreezeFrame->setText(tr("Freeze Frame"));
	m_pActAddMarker->setText(tr("Add Marker"));
	m_pActAddMarker->setShortcut(QKeySequence("M"));

	m_pActInserTrack->setText(tr("Insert track"));
	m_pActRemoveEmptyTrack->setText(tr("Remove all empty track"));

	m_pActVisible->setText(tr("Visible"));
	m_pActUnvisible->setText(tr("Unvisible"));
	m_pActLock->setText(tr("Lock"));
	m_pActUnlock->setText(tr("Unlock"));

	m_pActSelectGroup->setText(tr("Select Same Color Group"));
	m_pActSelectGroup->setShortcut(QKeySequence("Alt+~"));
}



//////////////////////////////////////////////////////////////////////////
TLToolTipMgr::TLToolTipMgr(QObject *parent /*= 0*/)
	:QObject(parent)
{
	m_bShowing = false;
	m_nTimerType = timer_none;
	m_clip = nullptr;
	qApp->installEventFilter(this);
	connect(&m_timer, &QTimer::timeout, this, &TLToolTipMgr::OnTimeOut);
}
TLToolTipMgr::~TLToolTipMgr()
{

}

void TLToolTipMgr::SetView(TimelineViewPrivate *view)
{
	m_pTimelineViewPrivate = view;
}

TLToolTipMgr* TLToolTipMgr::GetInstance()
{
	static TLToolTipMgr ins;
	return &ins;
}

void TLToolTipMgr::OnTimeOut()
{
	m_timer.stop();
	if (m_nTimerType == timer_prepare)
	{
		emit sigShowTip(true, m_clip->GetStartAtTrack(), m_clip->GetLenAtTrack(), m_ptCurrent);
		m_bShowing = true;
		m_timer.start(4000);
		m_nTimerType = timer_showing;
	}
	else if (m_nTimerType == timer_showing)
	{
		emit sigShowTip(false, 0, 0, QPoint());
		m_bShowing = false;
		m_timer.stop();
		m_nTimerType = timer_none;
		m_clip = nullptr;
	}
}

bool TLToolTipMgr::eventFilter(QObject *obj, QEvent *e)
{
	if (e->type() == QEvent::MouseMove)
	{
		bool bHitClip = false;
		m_ptCurrent = QCursor::pos();
		if (TVDragControl::GetInstance()->GetViewportRect().contains(m_ptCurrent))
		{
			auto trackView = m_pTimelineViewPrivate->GetTrackView();
			auto child = qobject_cast<TrackWidget*>(trackView->childAt(trackView->mapFromGlobal(m_ptCurrent)));
			if (child)
			{
				auto clip = child->IsHitClip(child->mapFromGlobal(m_ptCurrent));
				if (clip)
				{
					bHitClip = true;
					if (m_bShowing)
					{
						m_bShowing = false;
						emit sigShowTip(false, 0, 0, QPoint());
					}
					m_timer.start(700);
					m_nTimerType = timer_prepare;
					m_clip = clip;
				}
			}
		}
		if (!bHitClip)
		{
			if (m_bShowing)
			{
				m_bShowing = false;
				emit sigShowTip(false, 0, 0, QPoint());
			}
			if (m_timer.isActive())
			{
				m_timer.stop();
				m_nTimerType = timer_none;
				m_clip = nullptr;
			}
		}
	}
	else if (e->type() == QEvent::DragMove)
	{
		if (m_bShowing)
		{
			m_bShowing = false;
			emit sigShowTip(false, 0, 0, QPoint());
		}
		if (m_timer.isActive())
		{
			m_timer.stop();
			m_nTimerType = timer_none;
			m_clip = nullptr;
		}
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
ClipMarkerControl::ClipMarkerControl(QObject *parent, TrackWidget* pWidget)
	:QObject(parent)
	, m_pTrackWidget(pWidget)
{
	m_nHitMarker = -1;
	m_pHitClip = nullptr;
	m_pMenu = new QMenu(m_pTrackWidget);
	m_pMenu->setFixedWidth(220);
	m_pActDelete = new QAction(m_pTrackWidget);
	m_pActDeleteAll = new QAction(m_pTrackWidget);
	m_pMenu->addAction(m_pActDelete);
	m_pMenu->addAction(m_pActDeleteAll);
	connect(m_pActDelete, &QAction::triggered, this, [&]{
		TimelineManager::GetInstance()->GetClipMarkerMgr()->Remove(m_pHitClip->GetNLETimeline(), m_nHitMarker);
	});
	connect(m_pActDeleteAll, &QAction::triggered, this, [&]{
		TimelineManager::GetInstance()->GetClipMarkerMgr()->RemoveAll(m_pHitClip->GetNLETimeline());
	});
	RetranslateUi();
	m_pTrackWidget->installEventFilter(this);
}

int ClipMarkerControl::IsHitMarker(QPoint globlaPt)
{
	m_pHitClip = m_pTrackWidget->IsHitClip(m_pTrackWidget->mapFromGlobal(globlaPt));
	if (m_pHitClip)
	{
		auto markers = m_pHitClip->GetNLETimeline()->GetMarkers();
		auto ruler = TrackBuilder::GetInstance()->GetRuler();
		
		int markW = m_pTrackWidget->GetMarkerSize().width();
		int markH = m_pTrackWidget->GetMarkerSize().height();
		int markT = m_pTrackWidget->GetMarkerTop();
		for (auto mk : markers)
		{
			mk += (m_pHitClip->GetNLETimeline()->GetPosition() - m_pHitClip->GetNLETimeline()->GetTrimin());
			auto pos = ruler->Frames2PositionF(mk);

			QPoint localPt(pos - markW * 0.5 + TRACK_WIDGET_HEADER_WIDTH, markT);
			auto pt = m_pTrackWidget->mapToGlobal(localPt);
			if (QRect(pt.x(), pt.y(), markW, markH).contains(globlaPt))
			{
				return mk;
			}
		}
	}
	return -1;
}

void ClipMarkerControl::RetranslateUi()
{
	m_pActDelete->setText(tr("Delete This Marker"));
	m_pActDelete->setShortcut(QKeySequence::Delete);
	m_pActDeleteAll->setText(tr("Delete All Markers"));
}

bool ClipMarkerControl::eventFilter(QObject *obj, QEvent *e)
{
	if (e->type() == QEvent::MouseButtonPress)
	{
		if (m_pTrackWidget->IsHitHeader(m_pTrackWidget->mapFromGlobal(QCursor::pos())))
			return false;
		m_nHitMarker = IsHitMarker(QCursor::pos());
		if (m_nHitMarker != -1)
		{
			TimelineManager::GetInstance()->MultiSelect(QList<FSNLETimeline*>() << m_pHitClip->GetNLETimeline());
			TimelineManager::GetInstance()->GetClipMarkerMgr()->SetSelected(m_pHitClip->GetNLETimeline(), m_nHitMarker, true);
			TimelineManager::GetInstance()->GetMarkPointMgr()->UnselectAll();
			m_bLeftClick = ((QMouseEvent*)e)->button() == Qt::LeftButton;
			if (!m_bLeftClick)
				m_pMenu->exec(QCursor::pos());
			return true;
		}
	}
	else if (e->type() == QEvent::MouseButtonRelease)
	{
		if (m_nHitMarker != -1)
		{
			int tmep = m_nHitMarker;
			TimelineManager::GetInstance()->SetCurrentFrames(m_nHitMarker);
			TrackBuilder::GetInstance()->GetRuler()->SetCurrentFrames(m_nHitMarker);

			TimelineViewManager::GetInstance()->sigStartSeek();
			emit TimelineViewManager::GetInstance()->sigSeek(m_nHitMarker);
			TimelineViewManager::GetInstance()->sigEndSeek();

			m_pHitClip = nullptr;
			m_nHitMarker = -1;
			TrackBuilder::GetInstance()->UpdateItems();
			return true;
		}
	}
	else if (e->type() == QEvent::MouseMove)
	{
		if (m_nHitMarker != -1)
		{
			return true;
		}
	}
	else if (e->type() == QEvent::LanguageChange)
	{
		RetranslateUi();
	}
	return false;
}
