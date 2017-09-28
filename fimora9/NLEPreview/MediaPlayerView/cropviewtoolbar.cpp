#include "stdafx.h"
#include "cropviewtoolbar.h"
#include <qtwidgets/QButtonGroup>
#include <qtwidgets/QRadioButton>
#include "inc_CommonLib/CommonWidgets.h"
#include "inc_CommonLib/externalapi.h"
#include "mediaplayercontrolprivate.h"

CropViewToolBar::CropViewToolBar(MediaPlayerControl *control, QWidget *parent)
: ToolBarBase(control,parent)
{
	setAttribute(Qt::WA_StyledBackground);
	setObjectName("MediaPlayerView_CropViewToolBar");

	m_pLabelConstrain = new QLabel(tr("Constrain:"), this);
	m_pRadioManually = new QRadioButton(tr("Manually"),this);
	m_pRadio169 = new QRadioButton(tr("16:9"), this);
	m_pRadio43 = new QRadioButton(tr("4:3"), this);
	QButtonGroup *pGroup = new QButtonGroup(this);
	pGroup->addButton(m_pRadioManually, CropView_BtnID_RadioManually);
	pGroup->addButton(m_pRadio169, CropView_BtnID_Radio169);
	pGroup->addButton(m_pRadio43, CropView_BtnID_Radio43);

	m_pBtnReset = new QPushButton(tr("Reset"), this);
	m_pBtnReset->setProperty("id", CropView_BtnID_Reset);
	m_pBtnOK = new QPushButton(tr("OK"),this);
	m_pBtnOK->setProperty("id", CropView_BtnID_OK);
	m_pBtnCancel = new QPushButton(tr("Cancel"),this);
	m_pBtnCancel->setProperty("id", CropView_BtnID_Cancel);

	m_pCurrentFrame = new FrameLabel(this);

	connect(this, &ToolBarBase::Clicked, this, [&](int id,QPoint p){
		emit signalButtonClicked(id);
	});

	connect(pGroup, SIGNAL(buttonToggled(int, bool)), this, SLOT(slotButtonToggled(int, bool)));

	connect(m_pBtnReset, &QPushButton::clicked, this, [&](){
		m_pRadioManually->setChecked(true);
		emit signalButtonClicked(sender()->property("id").toInt());
	});
	connect(m_pBtnOK, &QPushButton::clicked, this, [&](){
		emit signalButtonClicked(sender()->property("id").toInt());
	});
	connect(m_pBtnCancel, &QPushButton::clicked, this, [&](){
		emit signalButtonClicked(sender()->property("id").toInt());
	});
	m_pRadioManually->setChecked(true);
}

CropViewToolBar::~CropViewToolBar()
{

}

void CropViewToolBar::slotButtonToggled(int id, bool bCheck)
{
	if (bCheck)
		emit signalButtonClicked(id);
}

void CropViewToolBar::Open()
{
	//ToolBarBase::Open();
}


void CropViewToolBar::Close()
{
	ToolBarPlayButtonStateChanged(ToolBar::ToolBar_Play);
	m_slider->setValue(0);
	RulerSeekCallback(0);

	m_pCurrentFrame->SetCurrentFrame(0);
	m_pCurrentFrame->SetDuration(0);
}

void CropViewToolBar::Play()
{
	ToolBarBase::Play();
}

void CropViewToolBar::Stop()
{
	ToolBarPlayButtonStateChanged(ToolBar::ToolBar_Play);
	m_slider->setValue(0);
	RulerSeekCallback(0);

	m_pCurrentFrame->SetCurrentFrame(0);
}

void CropViewToolBar::Pause()
{
	ToolBarBase::Pause();
}


void CropViewToolBar::SeekCallback(int frames, int accurate)
{
	if (m_slider->canSeek())
	{
		m_slider->setValue(frames);
		m_pCurrentFrame->SetCurrentFrame(frames);
	}
}

void CropViewToolBar::SeekManual(int frames, int accurate)
{
	if (m_mediaPlayerControl->HaveSource())
	{
		m_mediaPlayerControl->Seek(frames, accurate);
		RulerSeekCallback(frames);
		m_pCurrentFrame->SetCurrentFrame(frames);
	}

}

void CropViewToolBar::SetDuration(int duration)
{
	ToolBarBase::SetDuration(duration);
	m_pCurrentFrame->SetDuration(duration);
}


void CropViewToolBar::SetFPS(float fps)
{
	m_pCurrentFrame->SetFPS(fps);
}

void CropViewToolBar::resizeEvent(QResizeEvent *event)
{
	static const int nImgBtnW = 14;
	static const int nImgBtnH = 14;
	static const int nBigBtnW = 80;
	static const int nBigBtnH = 24;
	static const int nSmallBtnW = 70;
	static const int nSmallBtnH = 24;

	int nHspacing = 16;
	int nVspacing = 90;
	
	int nTotalW = rect().width();
	int nTotalH = rect().height();

	int nLeft = nHspacing;
	int nTop = nVspacing;

	m_slider->setGeometry(nLeft, nTop, nTotalW - 2 * nHspacing, m_slider->Height());

	nVspacing = 20;
	nTop = m_slider->geometry().bottom() + nVspacing;
	GetButton(ToolBar_Play)->setGeometry(nLeft, nTop, nImgBtnW, nImgBtnH);

	nHspacing = 5;
	nLeft = GetButton(ToolBar_Play)->geometry().right() + nHspacing;
	GetButton(ToolBar_Stop)->setGeometry(nLeft, nTop, nImgBtnW, nImgBtnH);

	int nFramelabelW = 102;
	int nFrameLabelH = 14;
	nHspacing = 16;
	nLeft = nTotalW - nHspacing - nFramelabelW;
	m_pCurrentFrame->setGeometry(nLeft, nTop, nFramelabelW, nFrameLabelH);

	nVspacing = 55;
	nLeft = nHspacing;
	nTop = m_pCurrentFrame->geometry().bottom() + nVspacing;

	int nRadioW = 75;
	int nRadioH = 18;
	m_pLabelConstrain->setGeometry(nLeft, nTop, nRadioW, nRadioH);

	nLeft = m_pLabelConstrain->geometry().right() + 2 * nHspacing;
	m_pRadioManually->setGeometry(nLeft, nTop, nRadioW, nRadioH);

	nLeft = m_pRadioManually->geometry().right() + 2 * nHspacing;
	m_pRadio169->setGeometry(nLeft, nTop, nRadioW, nRadioH);

	nLeft = m_pRadio169->geometry().right() + 2 * nHspacing;
	m_pRadio43->setGeometry(nLeft, nTop, nRadioW, nRadioH);
	
	nVspacing = 30;
	nLeft = nHspacing;
	nTop = m_pRadio43->geometry().bottom() + nVspacing;

	m_pBtnReset->setGeometry(nLeft, nTop, nBigBtnW, nBigBtnH);
	nLeft = nTotalW - nHspacing - nSmallBtnW;
	m_pBtnCancel->setGeometry(nLeft, nTop, nSmallBtnW, nSmallBtnH);
	nLeft = nTotalW - nSmallBtnW * 2 - nHspacing * 2;
	m_pBtnOK->setGeometry(nLeft, nTop, nSmallBtnW, nSmallBtnH);
}



//FrameLabel
FrameLabel::FrameLabel(QWidget* parent)
:QWidget(parent)
{
	m_duration = 0;
	m_fps = 25.0;
	m_curFrame = new ToolBarLabel(this);
	m_curFrame->setText("00:00:00/00:00:00");
}

FrameLabel::~FrameLabel()
{

}

void FrameLabel::SetFPS(float fps)
{
	m_fps = fps;
}

void FrameLabel::SetCurrentFrame(int frames)
{
	frames = qBound(0, frames, m_duration);
	QString strCurFrame = gFrame2StringSecond(frames, m_fps);
	m_curFrame->setText(strCurFrame + "/" + m_strDerationStr);
}

void FrameLabel::SetDuration(int duration)
{
	m_duration = duration;
	m_strDerationStr = gFrame2StringSecond(m_duration, m_fps);
}


void FrameLabel::resizeEvent(QResizeEvent *event)
{
	m_curFrame->setGeometry(rect());
}