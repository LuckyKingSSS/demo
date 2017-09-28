#include "stdafx.h"

#include "AudioPillarView.h"
#include <QtWidgets/QSlider>
#include <QtCore/QTimer>

CAudioPillarView::CAudioPillarView(QWidget *parent /*= NULL*/)
:QWidget(parent)
{
	m_rangeCount = 100;

	m_pLeftVolumeView = new QLabel(this);
	m_pRightVolumeView = new QLabel(this);
	m_pLeftVolumeView->setStyleSheet("QLabel{background-color:green;}");
	m_pRightVolumeView->setStyleSheet("QLabel{background-color:green;}");

}


CAudioPillarView::~CAudioPillarView()
{

}

void CAudioPillarView::SetPallarRange(int nMinValue, int nMaxValue)
{
	m_rangeCount = nMaxValue - nMinValue;
	if (m_rangeCount <= 0)
	{
		m_rangeCount = 100;
	}
}


void CAudioPillarView::SetPillarValue(int nLeftVolume, int nRightVolume)
{
	int nLeftHeight = nLeftVolume * height() / m_rangeCount;
	int nRightHeight = nRightVolume * height() / m_rangeCount;
	int w = (width() - 2) / 2;
	m_pLeftVolumeView->setGeometry(0, height() - nLeftHeight, w, nLeftHeight);
	int left = m_pLeftVolumeView->geometry().right() + 2;
	m_pRightVolumeView->setGeometry(left, height() - nRightHeight, w, nRightHeight);
	update();
}

void CAudioPillarView::showEvent(QShowEvent *event)
{
	SetPillarValue(0, 0);
}

