#include "stdafx.h"
#include "FTimelineViewPrivate.h"

#include "FToolBar.h"
#include "FTracksView.h"
#include "FRuler.h"

#include "inc_FProject/FSequence.h"
#include "inc_FProject/FDateDefine.h"

#include "inc_CommonLib/externalapi.h"

FTimelineViewPrivate::FTimelineViewPrivate(QWidget *parent)
: FBaseWidget(parent)
    , m_nRealTopPosition(0)
{

    //setStyleSheet("FTimelineViewPrivate{background-color:gray}");

    m_pToolBar = new FToolBar(this);
    m_pRuler = new FRuler(this);
    m_pTrackView = new FTracksView(m_pRuler->Ruler(), this);

    m_pScroolBarH = new QScrollBar(Qt::Horizontal,this);
    //m_pScroolBarH->hide();
    m_pScroolBarH->setRange(0, 100);
    m_pScroolBarV = new QScrollBar(this);
    m_pScroolBarV->setSingleStep(10);
    //m_pScroolBarV->hide();
    m_pScroolBarV->setRange(0, 0);

    connect(m_pToolBar, &FToolBar::sigButtonClicked, this, &FTimelineViewPrivate::slotToolButtonClicked);


    connect(m_pScroolBarH, &QSlider::valueChanged, this, &FTimelineViewPrivate::slotScroolBarHValueChanged);
    connect(m_pScroolBarV, &QSlider::valueChanged, this, &FTimelineViewPrivate::slotScroolBarVValueChanged);

    setStyleSheet(gGetStyleSheet(":/FTimelineView/Resources/Resources/styles_light.qss"));
}

FTimelineViewPrivate::~FTimelineViewPrivate()
{

}

bool FTimelineViewPrivate::Load(FSequencePtr sequence)
{

    return m_pTrackView->Load(sequence);
}

void FTimelineViewPrivate::resizeEvent(QResizeEvent *event)
{
    const int scrollBarSpace = 14;
    const int toolBarH = 28;
    const int rulerH = 46;
    const int trackHeader = m_pTrackView->GetHeadWidth();
    const int rightMargin = 14;

    m_pToolBar->setGeometry(0, 0, width() - rightMargin, toolBarH);

    int left = trackHeader;
    int top = m_pToolBar->geometry().bottom() + 1;
    m_pRuler->setGeometry(left, top, width() - trackHeader - rightMargin, rulerH);

    //m_pRulerBkGround->setGeometry(0, m_pRuler->geometry().top(), width(), m_pRuler->geometry().height());

    //m_pRulerToolBarMask->setGeometry(0, m_pToolBar->geometry().top(), m_pToolBar->width(), m_pToolBar->height() + m_pRuler->height());

    if (m_nRealTopPosition == 0)
        top = m_pRuler->geometry().bottom() + 1;
    else
        top = m_nRealTopPosition + 1;

    int viewportHeiht = height() - scrollBarSpace - m_pRuler->geometry().bottom();
    int h = qMax(m_pTrackView->Height(), viewportHeiht);

    auto newSV = m_pScroolBarV->value() + (h - m_pTrackView->height());

    left = 0;
    m_pTrackView->setGeometry(left, top, width() - scrollBarSpace, h - 1);

    left = trackHeader;
    top = height() - scrollBarSpace;
    m_pScroolBarH->setGeometry(left, top, width() - trackHeader - rightMargin, scrollBarSpace);

    left = width() - rightMargin;
    top = m_pRuler->geometry().bottom() + 1;
    h = height() - m_pRuler->geometry().bottom() - m_pScroolBarH->height();
    m_pScroolBarV->setGeometry(left, top, scrollBarSpace, h);

    int trackHeightTemp = viewportHeiht - m_pTrackView->Height();
    m_pScroolBarV->setPageStep(m_pScroolBarV->height());
    //m_pScroolBarV->setVisible(trackHeightTemp < 0);
    m_pScroolBarV->setRange(0, qMax(0, -trackHeightTemp));

    m_pScroolBarV->setValue(newSV);
}

void FTimelineViewPrivate::slotScroolBarHValueChanged(int value)
{

}

void FTimelineViewPrivate::slotScroolBarVValueChanged(int value)
{
    m_pTrackView->move(QPoint(0, m_pRuler->geometry().bottom() - value));
    m_nRealTopPosition = m_pTrackView->pos().y();
}

void FTimelineViewPrivate::slotToolButtonClicked(int type)
{
    auto buttonType = (FToolBar::ToolBarType)type;
    switch (buttonType)
    {
    case FToolBar::Undo:
        FSEQUNECE_UNDO->Undo();
        break;
    case FToolBar::Redo:
        FSEQUNECE_UNDO->Redo();

        break;
    case FToolBar::Split:
        m_pTrackView->GetSequence()->Split();
        break;
    default:
        break;
    }
}
