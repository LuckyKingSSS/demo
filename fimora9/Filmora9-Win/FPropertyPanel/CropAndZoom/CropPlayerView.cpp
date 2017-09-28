#include "stdafx.h"
#include <QPainter>
#include "inc_CommonLib/externalapi.h"
#include "inc_FProject/FNLETimeline.h"
#include "inc_MediaPlayerView\MediaPlayerView.h"
#include "MediaPlayer\FPannelMediaPlayer.h"
#include "CropPlayerView.h"
#include "inc_CommonLib/NLEWraper.h"



CropPlayerView::CropPlayerView(FNLETimeline* timeline, QWidget *parent)
	: FBaseWidget(parent), m_Timeline(timeline)
{
	InitUI();
	InitConnections();
	InitData();
}

CropPlayerView::~CropPlayerView()
{

}

void CropPlayerView::SetTimeline(FNLETimeline* timeline)
{
	m_Timeline = timeline;
	Q_ASSERT(m_Timeline != NULL);
	IMediaPlayerControl* mediaPlayerControl = m_PlayerControl;
	Q_ASSERT(mediaPlayerControl != NULL);
	mediaPlayerControl->Open(1, m_Timeline);
	//获取多媒体文件的时长
	int iDurationTime = mediaPlayerControl->Duration();  //获取的是帧数
	SetDuration(iDurationTime);
}

void CropPlayerView::SetDuration(int iDurationTime)
{
	m_CropPlayerToolbar->SetDuration(iDurationTime);
}

void CropPlayerView::resizeEvent(QResizeEvent *event)
{
	const int iLeft = 15;
	const int iTop = 75;
	const int iPlayerHeight = 480;
	const int iPlayerWidth = 450;
	const int iSpace = 15;
	const int iVerticalSpace = 35;
	m_EditView->setGeometry(iLeft, iTop, iPlayerWidth,iPlayerHeight);
	int iRight =  m_EditView->geometry().right();
	m_PauseStateWidget->setGeometry(iLeft, iTop, iPlayerWidth, iPlayerHeight);
	m_PreViewPlayer->setGeometry(iRight + iSpace, iTop, iPlayerWidth, iPlayerHeight);
	int iBottom = m_EditView->geometry().bottom();
	m_CropPlayerToolbar->setGeometry(iLeft, iBottom, 900, 60);
}

void CropPlayerView::InitUI()
{
	const int iToolbarHeight = 60;
	const int iPlayerHeight = 480;
	const int iPlayerWidth = 450;
	m_PreViewPlayer = new MediaPlayerView(this);
	m_PlayerControl = m_PreViewPlayer->GetMediaPlayerControl();
	m_EditView = new FEditView(m_Timeline, m_PlayerControl, this);
	m_CropPlayerToolbar = new FPannelMediaPlayerBar(m_PlayerControl, this);
	m_PauseStateWidget = new FPauseStateView(m_PlayerControl, this);
	m_PauseStateWidget->hide();
}

void CropPlayerView::InitConnections()
{
	connect(m_EditView, &FEditView::sigPausePlay, this, [&](QRect CropRect){
		m_EditView->hide();
		m_PauseStateWidget->show();
		m_PauseStateWidget->SetCropRegion(CropRect);
	});
	connect(m_PauseStateWidget, &FPauseStateView::sigStartPlay, this, [&](QRect CropRect){
		m_EditView->show();
		m_PauseStateWidget->hide();
		m_EditView->SetCropRegion(CropRect);
	});
}

void CropPlayerView::InitData()
{
	//m_PlayerControl = new MediaPlayerControl();
}

FEditView::FEditView(FNLETimeline* timeline, IMediaPlayerControl *control, QWidget *parent) :
FBaseWidget(parent), m_MediaPlayerCtl(control), m_Timeline(timeline)
{
	InitUI();
	InitConnections();
	InitData();
}

void FEditView::SetPlayerRender()
{
	Q_ASSERT(m_Timeline);
	INLERender *pVideoRender = nullptr;
	INLERender *pAudioRender = nullptr;
	if (m_Timeline == nullptr)
	{
		return;
	}
	if (!m_Timeline->GetRender(&pAudioRender, &pVideoRender))
	{
		return;
	}

	INLERender *pVideoOrignalRender;
	HRESULT hr = pVideoRender->QueryInterface(IID_INLEOrignalRender, (LPVOID*)&pVideoOrignalRender);
	Q_ASSERT(SUCCEEDED(hr));

	if (!SUCCEEDED(m_CropEditPlayerView->SetVideoInput(0, pVideoOrignalRender)))
	{
		return;
	}	
}

void FEditView::Open()
{

}

void FEditView::SetCropRegion(QRect regionRec)
{
	m_CropEditMgr->SetAdjustRect(CropEditMgr::BlockID_100, regionRec);
}

void FEditView::resizeEvent(QResizeEvent *event)
{
	m_PreViewPlayer->setGeometry(0, 0, width(), height());
	//m_CropRegion->setGeometry(0, 0, width() - 50, height() - 50);
	RECT rShowRec = { 0 };
	rShowRec.left = 0;
	rShowRec.top = 0;
	rShowRec.right = width();
	rShowRec.bottom = height();

	NLEComPtr<IPlayerViewMgr> playerMgr = m_MediaPlayerCtl->GetMediaPlayerMgr();
	if (playerMgr != nullptr)
	{
		if (m_CropEditPlayerView == nullptr)
		{
			m_CropEditPlayerView = playerMgr->CreatePlayView((HVIEW)m_PreViewPlayer->winId(), rShowRec);
		}
		else
		{
			m_CropEditPlayerView->SetParentWND((HVIEW)m_PreViewPlayer->winId());
			auto hwnd = m_CropEditPlayerView->GetWND();
#ifdef WIN32
			SetWindowPos((HWND)m_CropEditPlayerView->GetWND(), 0, 0, 0, rShowRec.right, rShowRec.bottom, 0x0010); //最后一个参数SWP_NOACTIVATE，跨平台编译才能通过
#endif
		}
		SetPlayerRender();
	}
	emit sigPlayViewChanged(QRect(0, 0, rShowRec.right, rShowRec.bottom));
}

void FEditView::mousePressEvent(QMouseEvent *event)
{
	QPoint pt = event->localPos().toPoint();
	QRect adjustRec = m_CropEditMgr->GetAdjustRect();
	bool bInAdjustRect = m_CropEditMgr->GetAdjustRect().contains(pt);
	bool bInBlocks = m_CropEditMgr->PtInBlock(pt);

	if (bInAdjustRect && !bInBlocks)
	{
		m_CropEditMgr->MousePress(CropEditMgr::BlockID_100, pt);
	}
	event->ignore();
}

void FEditView::mouseMoveEvent(QMouseEvent *event)
{
	QPoint pt = event->localPos().toPoint();
	bool bInAdjustRect = m_CropEditMgr->GetAdjustRect().contains(pt);
	bool bInBlocks = m_CropEditMgr->PtInBlock(pt);
	if (bInAdjustRect && !bInBlocks)
	{
		qDebug() << "QApplication::setOverrideCursor";
		QApplication::setOverrideCursor(QCursor(Qt::SizeAllCursor));
	}
	else
	{
		qDebug() << "QApplication::restoreOverrideCursor()";
		QApplication::restoreOverrideCursor();
	}

	m_CropEditMgr->MouseMove(pt);
	
	event->ignore();
}

void FEditView::mouseReleaseEvent(QMouseEvent *event)
{
	m_CropEditMgr->MouseRelease(event->localPos().toPoint());
	event->ignore();
}

void FEditView::InitUI()
{
	setObjectName("FEditView");
	m_PreViewPlayer = new FBaseWidget(this);
	m_CropEditMgr = new CropEditMgr(this);
	m_EditViewMouseEvent = new FEditViewMouseEvent(this);
	m_CropRegion = new FCropRegionRect(this);
	m_CropRegion->setWindowOpacity(0);
	m_CropRegion->setAutoFillBackground(true);
	m_CropRegion->setAttribute(Qt::WA_TranslucentBackground, true);
}

void FEditView::InitConnections()
{
	connect(this, &FEditView::sigPlayViewChanged, this, [&](QRect newRect){
		m_CropEditMgr->SetFrameRect(newRect);
		int l = newRect.left();
		int t = newRect.top();
		int nW = newRect.width();
		int nH = newRect.height();
		QRectF region = m_Timeline->GetCropRegion();
		m_CropEditMgr->SetAdjustRect(CropEditMgr::BlockID_100, QRect(l + nW * region.left(), t + nH * region.top(), nW * region.width(), nH * region.height()));
	});

	connect(m_CropEditMgr, &CropEditMgr::signalCropRectUpdate, this, [&](QRectF rc){
		m_Timeline->SetCropRegion(rc);
	});

	connect(m_MediaPlayerCtl, &IMediaPlayerControl::PlayPauseCallback, this, [&,this](){
		QRect adjustRect = m_CropEditMgr->GetAdjustRect();
		emit sigPausePlay(adjustRect);
	});  
}

void FEditView::InitData()
{

}

CropEditMgr::CropEditMgr(QObject *parent)
	:QObject(parent)
{
	m_AspectRatio = AspectRatio_Manually;
	m_bPressed = false;
	m_parentWidget = dynamic_cast<QWidget*>(parent);
	Q_ASSERT(m_parentWidget);
	Init();
}

void CropEditMgr::SetFrameRect(const QRect &rect)
{
	m_rcFrame = rect;
	//m_rcAdjust = rect;
	//SetAdjustRect(BlockID_100,rect);
}

void CropEditMgr::SetAspectRatio(AspectRatio ratio)
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

void CropEditMgr::ManuallyRestrict(BlockID nId, const QRect &rect)
{
	static const int blockW = 8;
	static const int nDelta = 2 * blockW - 1;
	QRect rc = rect;
	switch (nId)
	{
	case CropEditMgr::BlockID_0:
		//left
		if (rc.left() + nDelta > rc.right()) rc.setLeft(rc.right() - nDelta);
		if (rc.left() < m_rcFrame.left()) rc.setLeft(m_rcFrame.left());
		//top
		if (rc.top() + nDelta > rc.bottom()) rc.setTop(rc.bottom() - nDelta);
		if (rc.top() < m_rcFrame.top()) rc.setTop(m_rcFrame.top());
		break;
	case CropEditMgr::BlockID_1:
		//top
		if (rc.top() + nDelta > rc.bottom()) rc.setTop(rc.bottom() - nDelta);
		if (rc.top() < m_rcFrame.top()) rc.setTop(m_rcFrame.top());
		break;
	case CropEditMgr::BlockID_2:
		//right
		if (rc.right() < rc.left() + nDelta) rc.setRight(rc.left() + nDelta);
		if (rc.right() > m_rcFrame.right()) rc.setRight(m_rcFrame.right());
		//top
		if (rc.top() + nDelta > rc.bottom()) rc.setTop(rc.bottom() - nDelta);
		if (rc.top() < m_rcFrame.top()) rc.setTop(m_rcFrame.top());
		break;
	case CropEditMgr::BlockID_3:
		//left
		if (rc.left() + nDelta > rc.right()) rc.setLeft(rc.right() - nDelta);
		if (rc.left() < m_rcFrame.left()) rc.setLeft(m_rcFrame.left());
		break;
	case CropEditMgr::BlockID_4:
		//right
		if (rc.right() < rc.left() + nDelta) rc.setRight(rc.left() + nDelta);
		if (rc.right() > m_rcFrame.right()) rc.setRight(m_rcFrame.right());
		break;
	case CropEditMgr::BlockID_5:
		//left
		if (rc.left() + nDelta > rc.right()) rc.setLeft(rc.right() - nDelta);
		if (rc.left() < m_rcFrame.left()) rc.setLeft(m_rcFrame.left());
		//bottom
		if (rc.bottom() < rc.top() + nDelta) rc.setBottom(rc.top() + nDelta);
		if (rc.bottom() > m_rcFrame.bottom()) rc.setBottom(m_rcFrame.bottom());
		break;
	case CropEditMgr::BlockID_6:
		//bottom
		if (rc.bottom() < rc.top() + nDelta) rc.setBottom(rc.top() + nDelta);
		if (rc.bottom() > m_rcFrame.bottom()) rc.setBottom(m_rcFrame.bottom());
		break;
	case CropEditMgr::BlockID_7:
		//right
		if (rc.right() < rc.left() + nDelta) rc.setRight(rc.left() + nDelta);
		if (rc.right() > m_rcFrame.right()) rc.setRight(m_rcFrame.right());
		//bottom
		if (rc.bottom() < rc.top() + nDelta) rc.setBottom(rc.top() + nDelta);
		if (rc.bottom() > m_rcFrame.bottom()) rc.setBottom(m_rcFrame.bottom());
		break;
	case CropEditMgr::BlockID_100:
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

void CropEditMgr::SpecialRestrict(BlockID nId, const QRect &rect)
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
	case CropEditMgr::BlockID_0:
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
	case CropEditMgr::BlockID_1:
	case CropEditMgr::BlockID_2:
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
				rc.setRight(rc.left() + rc.height() * fRatio - 1);
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
	case CropEditMgr::BlockID_3:
	case CropEditMgr::BlockID_5:
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
	case CropEditMgr::BlockID_4:
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
	case CropEditMgr::BlockID_6:
	case CropEditMgr::BlockID_7:
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
	case CropEditMgr::BlockID_100:
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

void CropEditMgr::UpdateDestCropRect()
{
	QRectF rc;
	rc.setLeft((qreal)(m_rcAdjust.left() - m_rcFrame.left()) / (qreal)m_rcFrame.width());
	rc.setTop((qreal)(m_rcAdjust.top() - m_rcFrame.top()) / (qreal)m_rcFrame.height());
	rc.setWidth((qreal)m_rcAdjust.width() / (qreal)m_rcFrame.width());
	rc.setHeight((qreal)m_rcAdjust.height() / (qreal)m_rcFrame.height());
	emit signalCropRectUpdate(rc);
}

void CropEditMgr::SetAdjustRect(BlockID nId, const QRect &rect)
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

QRect CropEditMgr::GetAdjustRect()
{
	return m_rcAdjust;
}

bool CropEditMgr::PtInBlock(const QPoint &pt)
{
	QPoint ptTmp = pt;
	for (auto block : m_blockViews)
	{
		//判断的时候要减去坐标轴所占的像素
		ptTmp.setX(pt.x() - 1);
		QRect rc = block->geometry();
		if (rc.contains(ptTmp, false))
		{
			return true;
		}
	}
	return false;
}

void CropEditMgr::MousePress(int nId, const QPoint &p)
{
	m_bPressed = true;
	m_pPressPt = p;
	m_ptPressCenter = m_rcAdjust.center();
	m_pPrev = p;
	m_pPressId = (BlockID)nId;
}

void CropEditMgr::MouseRelease(const QPoint &p)
{
	m_bPressed = false;
	m_pReleasePt = p;
}

void CropEditMgr::MouseMove(const QPoint &p)
{
	if (!m_bPressed) return;
	int nDx, nDy;

	QRect rc = m_rcAdjust;
	QPoint ptTemp;
	switch (m_pPressId)
	{
	case CropEditMgr::BlockID_0:
		rc.setTopLeft(p);
		break;
	case CropEditMgr::BlockID_1:
		rc.setTop(p.y());
		break;
	case CropEditMgr::BlockID_2:
		rc.setTopRight(p);
		break;
	case CropEditMgr::BlockID_3:
		rc.setLeft(p.x());
		break;
	case CropEditMgr::BlockID_4:
		rc.setRight(p.x());
		break;
	case CropEditMgr::BlockID_5:
		rc.setBottomLeft(p);
		break;
	case CropEditMgr::BlockID_6:
		rc.setBottom(p.y());
		break;
	case CropEditMgr::BlockID_7:
		rc.setBottomRight(p);
		break;
	case CropEditMgr::BlockID_100:
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
	SetAdjustRect(m_pPressId, rc);
}

void CropEditMgr::Reset()
{
	m_rcAdjust = m_rcFrame;
	UpdateUi();
	UpdateDestCropRect();
}

void CropEditMgr::Init()
{
	for (int i = 0; i < 4; ++i)
	{
		if (i < 2)
		{
			m_lineViews.push_back(new CropLineWidget(Qt::Horizontal, m_parentWidget));
		}
		else
		{
			m_lineViews.push_back(new CropLineWidget(Qt::Vertical, m_parentWidget));
		}
	}
	for (int i = BlockID_0; i <= BlockID_7; ++i)
	{
		auto block = new CropBlockWidget(m_parentWidget);
		block->setProperty("id", i);
		SetBlockViewCursor((BlockID)i, block);

		connect(block, &CropBlockWidget::signalLBtnPressed, this, [&](QPoint p){
			MousePress(sender()->property("id").toInt(), p);
		});
		connect(block, &CropBlockWidget::signalLBntRelease, this, [&](QPoint p){
			MouseRelease(p);
		});
		connect(block, &CropBlockWidget::signalMouseMove, this, [&](QPoint p){
			MouseMove(p);
		});
		m_blockViews.push_back(block);
	}
}

void CropEditMgr::SetBlockViewCursor(BlockID nId, CropBlockWidget *pView)
{
	switch (nId)
	{
	case CropEditMgr::BlockID_0:
	case CropEditMgr::BlockID_7:
		pView->setCursor(QCursor(Qt::SizeFDiagCursor));
		break;
	case CropEditMgr::BlockID_1:
	case CropEditMgr::BlockID_6:
		pView->setCursor(QCursor(Qt::SizeVerCursor));
		break;
	case CropEditMgr::BlockID_2:
	case CropEditMgr::BlockID_5:
		pView->setCursor(QCursor(Qt::SizeBDiagCursor));
		break;
	case CropEditMgr::BlockID_3:
	case CropEditMgr::BlockID_4:
		pView->setCursor(QCursor(Qt::SizeHorCursor));
		break;
	}
}

void CropEditMgr::UpdateUi()
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

CropLineWidget::CropLineWidget(Qt::Orientation ori, QWidget *parent) :FBaseWidget(parent)
{
	m_Orientation = ori;
	m_Orientation == Qt::Horizontal ? setFixedHeight(1) : setFixedWidth(1);
}

void CropLineWidget::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	QRect rec = geometry();
	QRect rec2 = rect();
	painter.fillRect(rect(), QColor(0, 0, 0));

	QRect rcAll = rect();
	QRect rcDraw = rcAll;
	QColor clrDraw(255, 255, 255);

	static const int nDelta = 3;
	int len = (m_Orientation == Qt::Horizontal) ? rcAll.width() : rcAll.height();

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
	FBaseWidget::paintEvent(event);
}

CropBlockWidget::CropBlockWidget(QWidget *parent) :FBaseWidget(parent)
{
	setFixedSize(w, h);
	setMouseTracking(true);
}

void CropBlockWidget::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.fillRect(rect(), QColor(0, 0, 0));
	QPen pen;
	pen.setWidth(1);
	pen.setColor(QColor(255, 255, 255));
	painter.setPen(pen);
	painter.drawRect(rect().adjusted(0, 0, -1, -1));
	FBaseWidget::paintEvent(event);
}

void CropBlockWidget::mousePressEvent(QMouseEvent *event)
{
	if (event->button() != Qt::LeftButton) return;
	emit signalLBtnPressed(mapToParent(event->localPos().toPoint()));
	event->accept();
}

void CropBlockWidget::mouseMoveEvent(QMouseEvent *event)
{
	emit signalMouseMove(mapToParent(event->localPos().toPoint()));
	event->accept();
}

void CropBlockWidget::mouseReleaseEvent(QMouseEvent *event)
{
	if (event->button() != Qt::LeftButton) return;
	emit signalLBntRelease(mapToParent(event->localPos().toPoint()));
	event->accept();
}

FEditViewMouseEvent::FEditViewMouseEvent(FBaseWidget *view)
{
	m_displayView = view;
	m_elapsedTimer.start();
	m_bIsPauseState = false;
	qApp->installNativeEventFilter(this);
}

FEditViewMouseEvent::~FEditViewMouseEvent()
{
	qApp->removeNativeEventFilter(this);
}

bool FEditViewMouseEvent::nativeEventFilter(const QByteArray & eventType, void * message, long * result)
{
#ifdef WIN32
	MSG *msg = (MSG*)(message);
	HWND hwnd = GetMsgHander(msg);
	if (msg->message == WM_LBUTTONDOWN)
	{
		if (hwnd == m_displayView->getHwnd())
		{
			QWidget *widget = QWidget::find((WId)hwnd);
			QMouseEvent mouseEvent(QEvent::MouseButtonPress, QPoint((LOWORD(msg->lParam)), (HIWORD(msg->lParam))), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
			QApplication::sendEvent(widget, &mouseEvent);

			if (m_elapsedTimer.elapsed() < QApplication::doubleClickInterval())
			{
				QMouseEvent doublieMouseEvent(QEvent::MouseButtonDblClick, QPoint((LOWORD(msg->lParam)), (HIWORD(msg->lParam))), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
				QApplication::sendEvent(widget, &doublieMouseEvent);
			}
			else
				m_elapsedTimer.restart();
		}
	}
	else if (msg->message == WM_LBUTTONUP)
	{
		if (hwnd == m_displayView->getHwnd())
		{
			QWidget *widget = QWidget::find((WId)hwnd);
			QMouseEvent mouseEvent(QEvent::MouseButtonRelease, QPoint((LOWORD(msg->lParam)), (HIWORD(msg->lParam))), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
			QApplication::sendEvent(widget, &mouseEvent);
		}
	}
	else if (msg->message == WM_MOUSEMOVE)
	{
		if (hwnd == m_displayView->getHwnd())
		{
			QWidget *widget = QWidget::find((WId)hwnd);
			QMouseEvent mouseEvent(QEvent::MouseMove, QPoint(PointDPIRatio(short(msg->lParam)), PointDPIRatio(short(msg->lParam >> 16))), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
			QApplication::sendEvent(widget, &mouseEvent);
		}
	}
#endif 
	return false;
}

int FEditViewMouseEvent::PointDPIRatio(int x)
{
	return x / gGetDevicePixelRatio();
}

HWND FEditViewMouseEvent::GetMsgHander(MSG *msg)
{
	HWND hwnd;
	if (m_bIsPauseState)
	{
		hwnd = msg->hwnd;
	}
	else
	{
		hwnd = ::GetParent(msg->hwnd);
	}
	return hwnd;
}

FCropRegionRect::FCropRegionRect(QWidget* parent) :FBaseWidget(parent)
{
	//setWindowOpacity(0);
	//setWindowFlags(Qt::FramelessWindowHint);
	setAttribute(Qt::WA_TranslucentBackground, true);
}

void FCropRegionRect::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.save();
	QPen pen(Qt::DashLine);
	pen.setColor(QColor(255,255,255));
	pen.setWidth(1);

	painter.setPen(pen);
	painter.setBrush(Qt::NoBrush);
	QRect rec = rect();
	painter.drawRect(QRect(rect()));
	painter.restore();
	setAttribute(Qt::WA_TranslucentBackground, true);
}

FPauseStateView::FPauseStateView(IMediaPlayerControl* pPlayerControl, QWidget* parent) :
FBaseWidget(parent),
m_pPlayerControl(pPlayerControl),
m_PlayerBackgroundColor(Qt::black)
{
	InitUI();
	InitConnections();
	m_CropRegionMgr->SetFrameRect(rect());
}

void FPauseStateView::SetPlayerBackgroundColor(QColor color)
{
	m_PlayerBackgroundColor = color;
	auto c = qRgba(color.red(), color.green(), color.blue(), color.alpha());
	m_pPlayerControl->GetTimeline()->GetPropW()->SetInt(NLEKey::Render::kRender_BackgroundColor, c);
}

void FPauseStateView::SetCropRegion(QRect rec)
{
	m_CropRegionMgr->SetAdjustRect(CropEditMgr::BlockID_100, rec);
}

HWND FPauseStateView::getHwnd()
{
	return (HWND)this->winId();
}

void FPauseStateView::ResetRegion()
{
	m_CropRegionMgr->Reset();
}

QImage FPauseStateView::GetCurBackgroundImg()
{
	INLERender *pVideoRender = NULL;
	QSize  playerRenderSize = m_pPlayerControl->GetTimeline()->GetRenderSize();
	QImage backGroundImg(playerRenderSize.width(), playerRenderSize.height(), QImage::Format_ARGB32);
	backGroundImg.fill(m_PlayerBackgroundColor);

	HRESULT hr = m_pPlayerControl->GetTimeline()->GetRender(&pVideoRender, NULL);
	if (!SUCCEEDED(hr)) return backGroundImg;
	if (!pVideoRender) return backGroundImg;

	NLEComPtr<INLEFrame> pVideoFrame = NLEWraper::GetInstance().GetPlayerMaterProvider()->CreateVideoFrame(playerRenderSize.width(), playerRenderSize.height());
	if (!pVideoFrame) return backGroundImg;

	bool bRender = true;
	bRender &= SUCCEEDED(pVideoRender->Prepare(pVideoFrame));
	bRender &= SUCCEEDED(pVideoRender->Render(pVideoFrame));

	if (!bRender) return backGroundImg;

	NLEMediaInfo Info;
	pVideoFrame->GetMediaInfo(&Info);

	backGroundImg = QImage((uchar*)pVideoFrame->GetData(), Info.video.nWidth, Info.video.nHeight, QImage::Format_ARGB32);
	return backGroundImg;
}

void FPauseStateView::InitUI()
{
	m_CropRegionMgr = new CropEditMgr(this);
	m_PauseStateViewEvent = new FEditViewMouseEvent(this);
	m_PauseStateViewEvent->SetPauseState(true);
}

void FPauseStateView::InitConnections()
{
	connect(m_pPlayerControl, &IMediaPlayerControl::PlayStartCallback, this, [&, this](){
		QRect adjustRect = m_CropRegionMgr->GetAdjustRect();
		emit sigStartPlay(adjustRect);
	});
}

QImage FPauseStateView::GetDefaultBackImage()
{
	INLERender *pVideoRender = NULL;
	QSize  playerRenderSize = m_pPlayerControl->GetTimeline()->GetRenderSize();
	QImage backGroundImg(playerRenderSize.width(), playerRenderSize.height(), QImage::Format_ARGB32);
	backGroundImg.fill(m_PlayerBackgroundColor);
	return backGroundImg;
}

void FPauseStateView::paintEvent(QPaintEvent *event)
{
	if (!isVisible())
		return;
	QPainter painter(this);
	double offsetx = 0.0;
	double offsety = 0.0;
	int nStart;
	QImage defauleBackGroundImage = GetDefaultBackImage();
	painter.drawImage(QPoint(offsetx, offsety), defauleBackGroundImage);

	if (m_pPlayerControl!= nullptr && m_pPlayerControl->HaveSource())
	{
		nStart = ::GetTickCount();
		QImage backGroundImg = GetCurBackgroundImg();
		QRect rec = backGroundImg.rect();
		QRect rawRec = rec;
		double dImageRate = m_pPlayerControl->GetRatio();
		double dWindowRate = (double)width() / (double)height();
		int iWidth = 0;
		int iHeight = 0;
		if (dImageRate > dWindowRate)
		{
			iWidth = width();
			iHeight = width() / dImageRate;
		}
		else
		{
			iHeight = height();
			iWidth = iHeight*dImageRate;
		}

		rec.setWidth(iWidth);
		rec.setHeight(iHeight);
		int iLeft = (width() - rec.width()) / 2;
		int iTop = (height() - rec.height()) /2;
		rec.setLeft(iLeft);
		rec.setTop(iTop);
		painter.drawImage(rec, backGroundImg, rawRec);
	}
}

void FPauseStateView::mousePressEvent(QMouseEvent *event)
{
	QPoint pt = event->localPos().toPoint();
	QRect adjustRec = m_CropRegionMgr->GetAdjustRect();
	bool bInAdjustRect = m_CropRegionMgr->GetAdjustRect().contains(pt);
	bool bInBlocks = m_CropRegionMgr->PtInBlock(pt);

	if (bInAdjustRect && !bInBlocks)
	{
		m_CropRegionMgr->MousePress(CropEditMgr::BlockID_100, pt);
	}
	event->ignore();
}

void FPauseStateView::mouseReleaseEvent(QMouseEvent *event)
{
	m_CropRegionMgr->MouseRelease(event->localPos().toPoint());
	event->ignore();
}

void FPauseStateView::mouseMoveEvent(QMouseEvent *event)
{
	QPoint pt = event->localPos().toPoint();
	bool bInAdjustRect = m_CropRegionMgr->GetAdjustRect().contains(pt);
	bool bInBlocks = m_CropRegionMgr->PtInBlock(pt);
	if (bInAdjustRect && !bInBlocks)
	{
		qDebug() << "QApplication::setOverrideCursor";
		QApplication::setOverrideCursor(QCursor(Qt::SizeAllCursor));
	}
	else
	{
		qDebug() << "QApplication::restoreOverrideCursor()";
		QApplication::restoreOverrideCursor();
	}
	m_CropRegionMgr->MouseMove(pt);
	event->ignore();
}

void FPauseStateView::resizeEvent(QResizeEvent *event)
{
	m_CropRegionMgr->SetFrameRect(QRect(0, 0, width(), height()));
}
