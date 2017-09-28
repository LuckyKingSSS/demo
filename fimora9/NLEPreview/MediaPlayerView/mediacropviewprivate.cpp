#include "stdafx.h"
#include "mediacropviewprivate.h"
#include "cropviewtoolbar.h"
#include "mediaplayercontrolprivate.h"
#include "inc_MediaPlayerView/MediaPlayerView.h"
#include "inc_CommonLib/NLEBaseWidget.h"
#include "inc_CommonLib/NLEWraper.h"
#include "inc_CommonLib/PlayerMaterProvider.h"
#include "inc_CommonLib/NLETimeline.h"
#include "INLETimelineBase.h"
#include "cropmediaplayercontrol.h"
#include <QtWidgets/QPushButton>

#define DisplayViewW 460
#define DisplayViewH 240

//MediaCropView
MediaCropView::MediaCropView(NLETimeline *pDestTimeLine, QWidget *parent)
:MediaPlayerView(new MediaCropViewPrivate(pDestTimeLine), parent)
{
	auto p = dynamic_cast<MediaCropViewPrivate*>(m_p);
	Q_ASSERT(p);
	connect(p, &MediaCropViewPrivate::signalOK, this, &MediaCropView::signalOK);
	connect(p, &MediaCropViewPrivate::signalCancel, this, &MediaCropView::signalCancel);
}

//MediaCropViewPrivate
MediaCropViewPrivate::MediaCropViewPrivate(NLETimeline *pDestTimeLine, QWidget *parent)
: MediaPlayerViewPrivate(parent, new CropMediaPlayerControl)
{
	setMouseTracking(true);
	m_pDestTimeline = pDestTimeLine;

	m_pControl = dynamic_cast<CropMediaPlayerControl *>(m_mediaPlayerControl);
	Q_ASSERT(m_pControl);
	
	m_displayView->setStyleSheet("DisplayView{background-color:black;}");
	m_destView = new DestDisplayView(m_pControl, this);
	m_toolbar = new CropViewToolBar(m_pControl, this);
	m_EditVView = new CropEditVView(this);

	connect(m_toolbar, &CropViewToolBar::signalButtonClicked, this, &MediaCropViewPrivate::ButtonClicked);
	m_pControl->SetToolBar(m_toolbar);

	connect(m_displayView, &DisplayView::signalDisplayRect, this, [&](QRect rc){
		m_EditVView->SetFrameRect(rc);
		int l = rc.left();
		int t = rc.top();
		int nW = rc.width();
		int nH = rc.height();
		QRectF region = m_pDestTimeline->GetCropRegion();
		m_EditVView->SetAdjustRect(CropEditVView::BlockID_100, QRect(l + nW * region.left(),t + nH * region.top(),nW * region.width(),nH * region.height()));
	});

	QShortcut *sc = new QShortcut(QKeySequence(Qt::Key_Space), this);
	connect(sc, &QShortcut::activated, this, [&](){
		if (m_pControl->HaveSource())
		{
			if (m_pControl->IsPlaying())
				m_pControl->Pause();
			else
				m_pControl->Play();
		}
	});

	connect(m_EditVView, &CropEditVView::signalCropRectUpdate, this, [&](QRectF rc){
		m_pDestTimeline->SetCropRegion(rc);
	});
	//m_pDestTimeline->SetCropRegion(QRectF(0.2, 0.2, 0.5, 0.5));
}

MediaCropViewPrivate::~MediaCropViewPrivate()
{

}

void MediaCropViewPrivate::SetRatio(float ratio)
{
	MediaPlayerViewPrivate::SetRatio(ratio);
	m_destView->setRatio(ratio);
}

void MediaCropViewPrivate::ButtonClicked(int id)
{
	switch (id)
	{
	case CropViewToolBar::CropView_BtnID_RadioManually:
		m_EditVView->SetAspectRatio(AspectRatio_Manually);
		break;
	case CropViewToolBar::CropView_BtnID_Radio169:
		m_EditVView->SetAspectRatio(AspectRatio_16_9);
		break;
	case CropViewToolBar::CropView_BtnID_Radio43:
		m_EditVView->SetAspectRatio(AspectRatio_4_3);
		break;
	case CropViewToolBar::CropView_BtnID_Reset:
		m_EditVView->Reset();
		break;
	case CropViewToolBar::CropView_BtnID_OK:
		emit signalOK();
		break;
	case CropViewToolBar::CropView_BtnID_Cancel:
		emit signalCancel();
		break;

	default:
		;//Q_ASSERT(false);
	}
}

void MediaCropViewPrivate::resizeEvent(QResizeEvent *event)
{
	int nTotalW = rect().width();
	int nTotalH = rect().height();

	int nHSpacing = (nTotalW - 2 * DisplayViewW) / 3;
	int nVSpacing = 47;

	int top = nVSpacing;
	int left = nHSpacing;

	m_displayView->resize(DisplayViewW, DisplayViewH);
	m_displayView->move(left, top);

	left = nHSpacing * 2 + DisplayViewW;
	m_destView->resize(DisplayViewW, DisplayViewH);
	m_destView->move(left, top);

	left = 0;
	top = nVSpacing + DisplayViewH;
	int nToolbarW = nTotalW;
	int nToolbarH = nTotalH - top;
	m_toolbar->resize(nToolbarW, nToolbarH);
	m_toolbar->move(left, top);
}


void MediaCropViewPrivate::showEvent(QShowEvent *event)
{
	MediaPlayerViewPrivate::showEvent(event);
	m_pControl->Open(m_pDestTimeline);
}

void MediaCropViewPrivate::mouseMoveEvent(QMouseEvent *event)
{
	QPoint pt = event->localPos().toPoint();
	bool bInAdjustRect = m_EditVView->GetAdjustRect().contains(pt);
	bool bInBlocks = m_EditVView->PtInBlock(pt);
	if (bInAdjustRect && !bInBlocks)
	{
		//qDebug() << "QApplication::setOverrideCursor";
		//QApplication::setOverrideCursor(QCursor(Qt::SizeAllCursor));
	}
	else
	{
		//qDebug() << "QApplication::restoreOverrideCursor()";
		//QApplication::restoreOverrideCursor();
	}

	m_EditVView->MouseMove(pt);
	event->ignore();
}

void MediaCropViewPrivate::mousePressEvent(QMouseEvent *event)
{
	QPoint pt = event->localPos().toPoint();

	bool bInAdjustRect = m_EditVView->GetAdjustRect().contains(pt);
	bool bInBlocks = m_EditVView->PtInBlock(pt);

	if (bInAdjustRect && !bInBlocks)
	{
		m_EditVView->MousePress(CropEditVView::BlockID_100, pt);
	}
	event->ignore();
}

void MediaCropViewPrivate::mouseReleaseEvent(QMouseEvent *event)
{
	m_EditVView->MouseRelease(event->localPos().toPoint());
	event->ignore();
}

//DestDisplayView

DestDisplayView::DestDisplayView(IMediaPlayerControl *control, QWidget *parent)
:DisplayView(control,parent)
{
	setStyleSheet("DestDisplayView{background-color:black;}");
}

void DestDisplayView::SetPlayerViewHwnd(const RECT &r)
{
	CropMediaPlayerControl *pControl = dynamic_cast<CropMediaPlayerControl *>(m_control);
	Q_ASSERT(pControl);
	pControl->SetDestViewHwnd((HVIEW)m_display->winId(), r);
}

//CropEditVView:
CropEditVView::CropEditVView(QObject *parent)
:QObject(parent)
{
	m_AspectRatio = AspectRatio_Manually;
	m_bPressed = false;
	m_parentWidget = dynamic_cast<QWidget*>(parent);
	Q_ASSERT(m_parentWidget);
	Init();
}

void CropEditVView::SetFrameRect(const QRect &rect)
{
	m_rcFrame = rect;
	//SetAdjustRect(BlockID_100,rect);
}

void CropEditVView::SetAspectRatio(AspectRatio ratio)
{
	m_AspectRatio = ratio;
	if (m_AspectRatio == AspectRatio_16_9 || m_AspectRatio == AspectRatio_4_3)
	{
		static const int blockW = 8;
		static const int nDelta = 2 * blockW - 1;
		float fRatio = m_AspectRatio == AspectRatio_16_9 ? 16.0 / 9.0 : 4.0 / 3.0;
		int nMinH = nDelta + 1;
		int nMinW = nMinH * fRatio;

		int nCurW = m_rcAdjust.width();
		int nCurH = m_rcAdjust.height();
		if (nCurW < nMinW) 
		{
			m_rcAdjust.setSize(QSize(nMinW, m_rcAdjust.height()));
			if (m_rcAdjust.right() > m_rcFrame.right())
			{
				m_rcAdjust.moveLeft(m_rcAdjust.right() - m_rcFrame.right());
			}
		}

		float fCurRatio = (float)nCurW / (float)nCurH;
		//总是变高度
		if (fCurRatio > fRatio || fCurRatio < fRatio)
		{ 
			int nTop = m_rcAdjust.top();
			int nHeight = nCurW / fRatio;
			m_rcAdjust.setBottom(nTop + nHeight - 1);
			if (m_rcAdjust.bottom() > m_rcFrame.bottom())
			{
				int nBottom = m_rcFrame.bottom();
				m_rcAdjust.setBottom(nBottom);
				m_rcAdjust.setWidth(m_rcAdjust.height() * fRatio);
			}
		}
		UpdateUi();
		UpdateDestCropRect();
	}
}

void CropEditVView::ManuallyRestrict(BlockID nId, const QRect &rect)
{
	static const int blockW = 8;
	static const int nDelta = 2 * blockW - 1;
	QRect rc = rect;
	switch (nId)
	{
	case CropEditVView::BlockID_0:
		//left
		if (rc.left() + nDelta > rc.right()) rc.setLeft(rc.right() - nDelta);
		if (rc.left() < m_rcFrame.left()) rc.setLeft(m_rcFrame.left());
		//top
		if (rc.top() + nDelta > rc.bottom()) rc.setTop(rc.bottom() - nDelta);
		if (rc.top() < m_rcFrame.top()) rc.setTop(m_rcFrame.top());
		break;
	case CropEditVView::BlockID_1:
		//top
		if (rc.top() + nDelta > rc.bottom()) rc.setTop(rc.bottom() - nDelta);
		if (rc.top() < m_rcFrame.top()) rc.setTop(m_rcFrame.top());
		break;
	case CropEditVView::BlockID_2:
		//right
		if (rc.right() < rc.left() + nDelta) rc.setRight(rc.left() + nDelta);
		if (rc.right() > m_rcFrame.right()) rc.setRight(m_rcFrame.right());
		//top
		if (rc.top() + nDelta > rc.bottom()) rc.setTop(rc.bottom() - nDelta);
		if (rc.top() < m_rcFrame.top()) rc.setTop(m_rcFrame.top());
		break;
	case CropEditVView::BlockID_3:
		//left
		if (rc.left() + nDelta > rc.right()) rc.setLeft(rc.right() - nDelta);
		if (rc.left() < m_rcFrame.left()) rc.setLeft(m_rcFrame.left());
		break;
	case CropEditVView::BlockID_4:
		//right
		if (rc.right() < rc.left() + nDelta) rc.setRight(rc.left() + nDelta);
		if (rc.right() > m_rcFrame.right()) rc.setRight(m_rcFrame.right());
		break;
	case CropEditVView::BlockID_5:
		//left
		if (rc.left() + nDelta > rc.right()) rc.setLeft(rc.right() - nDelta);
		if (rc.left() < m_rcFrame.left()) rc.setLeft(m_rcFrame.left());
		//bottom
		if (rc.bottom() < rc.top() + nDelta) rc.setBottom(rc.top() + nDelta);
		if (rc.bottom() > m_rcFrame.bottom()) rc.setBottom(m_rcFrame.bottom());
		break;
	case CropEditVView::BlockID_6:
		//bottom
		if (rc.bottom() < rc.top() + nDelta) rc.setBottom(rc.top() + nDelta);
		if (rc.bottom() > m_rcFrame.bottom()) rc.setBottom(m_rcFrame.bottom());
		break;
	case CropEditVView::BlockID_7:
		//right
		if (rc.right() < rc.left() + nDelta) rc.setRight(rc.left() + nDelta);
		if (rc.right() > m_rcFrame.right()) rc.setRight(m_rcFrame.right());
		//bottom
		if (rc.bottom() < rc.top() + nDelta) rc.setBottom(rc.top() + nDelta);
		if (rc.bottom() > m_rcFrame.bottom()) rc.setBottom(m_rcFrame.bottom());
		break;
	case CropEditVView::BlockID_100:
	{
		QPoint ptCenter = rc.center();
		if (rc.left() < m_rcFrame.left())
		{
			ptCenter.setX(ptCenter.x() + m_rcFrame.left() - rc.left());
		}
		if (rc.top() < m_rcFrame.top())
		{
			ptCenter.setY(ptCenter.y() + m_rcFrame.top() - rc.top());
		}
		if (rc.right() > m_rcFrame.right())
		{
			ptCenter.setX(ptCenter.x() - (rc.right() - m_rcFrame.right()));
		}
		if (rc.bottom() > m_rcFrame.bottom())
		{
			ptCenter.setY(ptCenter.y() - (rc.bottom() - m_rcFrame.bottom()));
		}
		rc.moveCenter(ptCenter);
		//Debug() << "rc: " << rc << "\r\n";
		break;
	}
	default:
		break;
	}
	m_rcAdjust = rc;
}

void CropEditVView::SpecialRestrict(BlockID nId, const QRect &rect)
{
	static const int blockW = 8;
	static const int nDelta = 2 * blockW - 1;
	float fRatio = m_AspectRatio == AspectRatio_16_9 ? 16.0 / 9.0 : 4.0 / 3.0;
	int nMinH = nDelta + 1;
	int nMinW = nMinH * fRatio;
	QRect rc = rect;
	int nW, nH, nTemp;
	switch (nId)
	{
	case CropEditVView::BlockID_0:
		nW = rc.width();
		nH = nW / fRatio;
		rc.setTop(rc.bottom() - nH + 1);

		if (rc.left() < m_rcFrame.left())
		{
			rc.setLeft(m_rcFrame.left());
			nH = rc.width() / fRatio;
			rc.setTop(rc.bottom() - nH + 1);
			
			if (rc.top() < m_rcFrame.top())
			{
				rc.setTop(m_rcFrame.top());
				rc.setLeft(rc.right() - rc.height() * fRatio + 1);
			}
		}
		else if (rc.top() < m_rcFrame.top()/* && rc.left() >= m_rcFrame.left()*/)
		{
			//qDebug() << "2: " << rc << ";" << m_rcFrame;
			rc.setTop(m_rcFrame.top());
			nW = rc.height() * fRatio;
			rc.setLeft(rc.right() - nW + 1);
		}
		else if (rc.left() + nMinW - 1 > rc.right())
		{
			rc.setLeft(rc.right() - nMinW + 1);
			rc.setTop(rc.bottom() - nMinH + 1);
		}
		break;
	case CropEditVView::BlockID_1:
	case CropEditVView::BlockID_2:
		//nH随rc，调整nW
		nH = rc.height();
		nW = nH * fRatio;
		rc.setWidth(nW);

		if (rc.right() > m_rcFrame.right())
		{
			rc.setRight(m_rcFrame.right());
			nH = rc.width() / fRatio;
			rc.setTop(rc.bottom() - nH + 1);

			if (rc.top() < m_rcFrame.top())
			{
				rc.setTop(m_rcFrame.top());
				rc.setRight(rc.left()+ rc.height() * fRatio - 1);
			}
		
		}
		else if (rc.top() < m_rcFrame.top())
		{
			rc.setTop(m_rcFrame.top());
			nW = rc.height() * fRatio;
			rc.setRight(rc.left() + nW - 1);
		}
		else if (rc.left() + nMinW - 1 > rc.right())
		{
			rc.setRight(rc.left() + nMinW - 1);
			rc.setTop(rc.bottom() - nMinH + 1);
		}
		break;
	case CropEditVView::BlockID_3:
	case CropEditVView::BlockID_5:
		//nW随rc，调整nH
		nW = rc.width();
		nH = nW / fRatio;
		rc.setHeight(nH);
		if (rc.left() < m_rcFrame.left())
		{
			rc.setLeft(m_rcFrame.left());
			nH = rc.width() / fRatio;
			rc.setHeight(nH);

			if (rc.bottom() > m_rcFrame.bottom())
			{
				rc.setBottom(m_rcFrame.bottom());
				rc.setLeft(rc.right() - rc.height() * fRatio + 1);
			}
		}
		else if (rc.bottom() > m_rcFrame.bottom())
		{
			rc.setBottom(m_rcFrame.bottom());
			rc.setLeft(rc.right() - rc.height() * fRatio + 1);
		}
		else if (rc.left() + nMinW - 1 > rc.right())
		{
			rc.setLeft(rc.right() - nMinW + 1);
			rc.setBottom(rc.top() + nMinH - 1);
		}
		break;
	case CropEditVView::BlockID_4:
		rc.setHeight(rc.width() / fRatio);
		if (rc.right() > m_rcFrame.right())
		{
			rc.setRight(m_rcFrame.right());
			rc.setBottom(rc.top() + rc.width() / fRatio - 1);

			if (rc.bottom() > m_rcFrame.bottom())
			{
				rc.setBottom(m_rcFrame.bottom());
				rc.setRight(rc.left() + rc.height() * fRatio - 1);
			}

		}
		else if (rc.bottom() > m_rcFrame.bottom())
		{
			rc.setBottom(m_rcFrame.bottom());
			rc.setRight(rc.left() + rc.height() * fRatio - 1);
		}
		else if (rc.left() + nMinW - 1 > rc.right())
		{
			rc.setRight(rc.left() + nMinW - 1);
			rc.setBottom(rc.top() + nMinH - 1);
		}

		break;
	case CropEditVView::BlockID_6:
	case CropEditVView::BlockID_7:
		//nH随rc，调整nW
		nH = rc.height();
		nW = nH * fRatio;
		rc.setWidth(nW);
		if (rc.bottom() > m_rcFrame.bottom())
		{
			rc.setBottom(m_rcFrame.bottom());
			rc.setRight(rc.left() + rc.height() * fRatio - 1);

			if (rc.right() > m_rcFrame.right())
			{
				rc.setRight(m_rcFrame.right());
				rc.setBottom(rc.top() + rc.width() / fRatio - 1);
			}
		}
		else if (rc.right() > m_rcFrame.right())
		{
			rc.setRight(m_rcFrame.right());
			rc.setBottom(rc.top() + rc.width() / fRatio - 1);
		}
		else if (rc.left() + nMinW - 1 > rc.right())
		{
			rc.setRight(rc.left() + nMinW - 1);
			rc.setBottom(rc.top() + nMinH - 1);
		}
		break;
	case CropEditVView::BlockID_100:
	{
		QPoint ptCenter = rc.center();
		if (rc.left() < m_rcFrame.left())
		{
			ptCenter.setX(ptCenter.x() + m_rcFrame.left() - rc.left());
		}
		if (rc.top() < m_rcFrame.top())
		{
			ptCenter.setY(ptCenter.y() + m_rcFrame.top() - rc.top());
		}
		if (rc.right() > m_rcFrame.right())
		{
			ptCenter.setX(ptCenter.x() - (rc.right() - m_rcFrame.right()));
		}
		if (rc.bottom() > m_rcFrame.bottom())
		{
			ptCenter.setY(ptCenter.y() - (rc.bottom() - m_rcFrame.bottom()));
		}
		rc.moveCenter(ptCenter);
		break;
	}
	default:
		break;
	}
	m_rcAdjust = rc;
	//qDebug() << "m_rcAdjust" << m_rcAdjust;
}

void CropEditVView::UpdateDestCropRect()
{
	QRectF rc;
	rc.setLeft((qreal)(m_rcAdjust.left() - m_rcFrame.left()) / (qreal)m_rcFrame.width());
	rc.setTop((qreal)(m_rcAdjust.top() - m_rcFrame.top()) / (qreal)m_rcFrame.height());
	rc.setWidth((qreal)m_rcAdjust.width() / (qreal)m_rcFrame.width());
	rc.setHeight((qreal)m_rcAdjust.height() / (qreal)m_rcFrame.height());
	emit signalCropRectUpdate(rc);
}

void CropEditVView::SetAdjustRect(BlockID nId, const QRect &rect)
{	
	switch (m_AspectRatio)
	{
	case AspectRatio_Manually:
		ManuallyRestrict(nId, rect);
		break;
	case AspectRatio_16_9:
	case AspectRatio_4_3:
		SpecialRestrict(nId, rect);
		break;
	}
	UpdateUi();
	UpdateDestCropRect();
}

QRect CropEditVView::GetAdjustRect()
{
	return m_rcAdjust;
}

bool CropEditVView::PtInBlock(const QPoint &pt)
{
	for (auto block : m_blockViews)
	{
		if (block->geometry().contains(pt)) return true;
	}
	return false;
}

void CropEditVView::MousePress(int nId, const QPoint &p)
{
	m_bPressed = true;
	m_pPressPt = p;
	m_ptPressCenter = m_rcAdjust.center();
	m_pPrev = p;
	m_pPressId = (BlockID)nId;
}

void CropEditVView::MouseRelease(const QPoint &p)
{
	m_bPressed = false;
	m_pReleasePt = p;
}

void CropEditVView::MouseMove(const QPoint &p)
{
	if (!m_bPressed) return;
	int nDx, nDy;

	QRect rc = m_rcAdjust;
	QPoint ptTemp;
	switch (m_pPressId)
	{
	case CropEditVView::BlockID_0:
		rc.setTopLeft(p);
		break;
	case CropEditVView::BlockID_1:
		rc.setTop(p.y());
		break;
	case CropEditVView::BlockID_2:
		rc.setTopRight(p);
		break;
	case CropEditVView::BlockID_3:
		rc.setLeft(p.x());
		break;
	case CropEditVView::BlockID_4:
		rc.setRight(p.x());
		break;
	case CropEditVView::BlockID_5:
		rc.setBottomLeft(p);
		break;
	case CropEditVView::BlockID_6:
		rc.setBottom(p.y());
		break;
	case CropEditVView::BlockID_7:
		rc.setBottomRight(p);
		break;
	case CropEditVView::BlockID_100:
		nDx = p.x() - m_pPressPt.x();
		nDy = p.y() - m_pPressPt.y();
		ptTemp.setX(m_ptPressCenter.x() + nDx);
		ptTemp.setY(m_ptPressCenter.y() + nDy);
		rc.moveCenter(ptTemp);
		break;
	default:
		break;
	}
	m_pPrev = p;
	SetAdjustRect(m_pPressId,rc);
}

void CropEditVView::Reset()
{
	m_rcAdjust = m_rcFrame;
	UpdateUi();
	UpdateDestCropRect();
}

void CropEditVView::Init()
{
	for (int i = 0; i < 4;++i )
	{
		if (i < 2)
		{
			m_lineViews.push_back(new CropLineView(Qt::Horizontal, m_parentWidget));
		}
		else
		{
			m_lineViews.push_back(new CropLineView(Qt::Vertical, m_parentWidget));
		}
	}
	for (int i = BlockID_0; i <= BlockID_7; ++i)
	{
		auto block = new CropBlockView(m_parentWidget);
		block->setProperty("id", i);
		SetBlockViewCursor((BlockID)i,block);

		connect(block, &CropBlockView::signalLBtnPressed, this, [&](QPoint p){
			MousePress(sender()->property("id").toInt(), p);
		});
		connect(block, &CropBlockView::signalLBntRelease, this, [&](QPoint p){
			MouseRelease(p);
		});
		connect(block, &CropBlockView::signalMouseMove, this, [&](QPoint p){
			MouseMove(p);
		});
		m_blockViews.push_back(block);
	}
}

void CropEditVView::SetBlockViewCursor(BlockID nId, CropBlockView *pView)
{
	switch (nId)
	{
	case CropEditVView::BlockID_0:
	case CropEditVView::BlockID_7:
		pView->setCursor(QCursor(Qt::SizeFDiagCursor));
		break;
	case CropEditVView::BlockID_1:
	case CropEditVView::BlockID_6:
		pView->setCursor(QCursor(Qt::SizeVerCursor));
		break;
	case CropEditVView::BlockID_2:
	case CropEditVView::BlockID_5:
		pView->setCursor(QCursor(Qt::SizeBDiagCursor));
		break;
	case CropEditVView::BlockID_3:
	case CropEditVView::BlockID_4:
		pView->setCursor(QCursor(Qt::SizeHorCursor));
		break;
	}
}

void CropEditVView::UpdateUi()
{
	m_lineViews[0]->setGeometry(m_rcAdjust.left(), m_rcAdjust.top(), m_rcAdjust.size().width(), m_rcAdjust.size().height());
	m_lineViews[1]->setGeometry(m_rcAdjust.left(), m_rcAdjust.bottom(), m_rcAdjust.size().width(), m_rcAdjust.size().height());
	m_lineViews[2]->setGeometry(m_rcAdjust.left(), m_rcAdjust.top(), m_rcAdjust.size().width(), m_rcAdjust.size().height());
	m_lineViews[3]->setGeometry(m_rcAdjust.right(), m_rcAdjust.top(), m_rcAdjust.size().width(), m_rcAdjust.size().height());
	
	int nHalfW = 4;
	int nHorPos0 = m_rcAdjust.top() - nHalfW;
	int nHorPos1 = m_rcAdjust.top() + m_rcAdjust.height() / 2 - nHalfW;
	int nHorPos2 = m_rcAdjust.bottom() - (nHalfW - 1);
	int nVerPos0 = m_rcAdjust.left() - nHalfW;
	int nVerPos1 = m_rcAdjust.left() + m_rcAdjust.width() / 2 - nHalfW;
	int nVerPos2 = m_rcAdjust.right() - (nHalfW - 1);

	m_blockViews[BlockID_0]->move(nVerPos0, nHorPos0);
	m_blockViews[BlockID_1]->move(nVerPos1, nHorPos0);
	m_blockViews[BlockID_2]->move(nVerPos2, nHorPos0);
	m_blockViews[BlockID_3]->move(nVerPos0, nHorPos1);
	m_blockViews[BlockID_4]->move(nVerPos2, nHorPos1);
	m_blockViews[BlockID_5]->move(nVerPos0, nHorPos2);
	m_blockViews[BlockID_6]->move(nVerPos1, nHorPos2);
	m_blockViews[BlockID_7]->move(nVerPos2, nHorPos2);
}

//CropLineView
CropLineView::CropLineView(Qt::Orientation ori,QWidget *parent)
:QWidget(parent)
{
	m_Orientation = ori;
	m_Orientation == Qt::Horizontal ? setFixedHeight(1) : setFixedWidth(1);
}

void CropLineView::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.fillRect(rect(), QColor(0, 0, 0));

	QRect rcAll = rect();
	QRect rcDraw = rcAll;
	QColor clrDraw(255, 255, 255);

	static const int nDelta = 3;
	int len = m_Orientation == Qt::Horizontal ? rcAll.width() : rcAll.height();

	if (len <= nDelta)
	{
	}
	else if (len > nDelta && len <= 2 * nDelta)
	{
		if (m_Orientation == Qt::Horizontal)
		{
			rcDraw.setLeft(nDelta);
			rcDraw.setWidth(len - nDelta);
		}
		else
		{
			rcDraw.setTop(nDelta);
			rcDraw.setHeight(len - nDelta);
		}
		painter.fillRect(rcDraw, clrDraw);
	}
	else
	{
		int nDrawCnts = len / (nDelta * 2);
		int i = 0;
		for (; i < nDrawCnts; ++i)
		{
			if (m_Orientation == Qt::Horizontal)
			{
				rcDraw.setLeft(nDelta + nDelta * 2 * i);
				rcDraw.setWidth(nDelta);
			}
			else
			{
				rcDraw.setTop(nDelta + nDelta * 2 * i);
				rcDraw.setHeight(nDelta);
			}
			painter.fillRect(rcDraw, clrDraw);
		}
		int nOverMuch = len % (nDelta * 2);
		if (nOverMuch <= nDelta)
		{
		}
		else
		{
			if (m_Orientation == Qt::Horizontal)
			{
				rcDraw.setLeft(nDelta + nDelta * 2 * i);
				rcDraw.setWidth(nOverMuch - nDelta);
			}
			else
			{
				rcDraw.setTop(nDelta + nDelta * 2 * i);
				rcDraw.setHeight(nOverMuch - nDelta);
			}
			painter.fillRect(rcDraw, clrDraw);
		}
	}
	QWidget::paintEvent(event);
}

//CropBlockView
CropBlockView::CropBlockView(QWidget *parent)
:QWidget(parent)
{
	setFixedSize(w, h);
	setMouseTracking(true);
}

void CropBlockView::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	
	painter.fillRect(rect(), QColor(0,0,0));

	QPen pen;
	pen.setWidth(1);
	pen.setColor(QColor(255, 255, 255));
	painter.setPen(pen);
	painter.drawRect(rect().adjusted(0,0,-1,-1));

	QWidget::paintEvent(event);
}

void CropBlockView::mousePressEvent(QMouseEvent *event)
{
	if (event->button() != Qt::LeftButton) return;
	emit signalLBtnPressed(mapToParent(event->localPos().toPoint()));
	event->accept();
}

void CropBlockView::mouseMoveEvent(QMouseEvent *event)
{
	//qDebug() << "CropBlockView::mouseMoveEvent:" << mapToParent(event->localPos().toPoint());
	emit signalMouseMove(mapToParent(event->localPos().toPoint()));
	event->accept();
}

void CropBlockView::mouseReleaseEvent(QMouseEvent *event)
{
	if (event->button() != Qt::LeftButton) return;
	emit signalLBntRelease(mapToParent(event->localPos().toPoint()));
	event->accept();
}

//MagicRect
MagicRect::MagicRect()
{
	m_Ratio = AspectRatio_Manually;
}

MagicRect::~MagicRect()
{

}

QRect& MagicRect::operator=(const QRect &rc)
{
	this->setTopLeft(rc.topLeft());
	this->setSize(rc.size());
	return *this;
}

void MagicRect::SetRatio(AspectRatio ratio)
{

}

void MagicRect::SetFrameRect(const QRect &rc)
{
	m_rcFrameRect = rc;
}