#include "stdafx.h"
#include "inc_FCommonLib\FCommonWidgets.h"
#include "FHistogramPannel.h"


FHistogramPannel::FHistogramPannel(QWidget *parent)
	: FBaseWidget(parent)
{
	InitUI();
	InitConnections();
}

FHistogramPannel::~FHistogramPannel()
{

}

void FHistogramPannel::InitUI()
{
	const int iHeaderheight = 20;
	m_pHeader = new FLabel(nullptr);
	m_pHeader->setText(tr("Histogram"));
	m_pHeader->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	m_pHeader->setFixedHeight(iHeaderheight);

	m_pMatrixGraph = new FMatrixDrawPannel(nullptr);
	QVBoxLayout* vLayout = new QVBoxLayout();
	vLayout->addWidget(m_pHeader);
	vLayout->addWidget(m_pMatrixGraph);
	vLayout->setContentsMargins(2, 2, 2, 2);
	setLayout(vLayout);
}

void FHistogramPannel::InitConnections()
{

}

FMatrixDrawPannel::FMatrixDrawPannel(QWidget *parent) :FBaseWidget(parent)
{
	InitUI();
	InitConnections();
}

FMatrixDrawPannel::~FMatrixDrawPannel()
{

}

void FMatrixDrawPannel::paintEvent(QPaintEvent *e)
{

}

void FMatrixDrawPannel::InitUI()
{

}

void FMatrixDrawPannel::InitConnections()
{

}

void FMatrixDrawPannel::GetGraphData()
{

}
