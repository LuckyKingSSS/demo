#include "stdafx.h"
#include "..\include\inc_FCommonLib\FCommonWidgets.h"
#include "FPreviewPlayerHeader.h"

FPreviewPlayerHeader::FPreviewPlayerHeader(QWidget *parent)
	: FBaseWidget(parent)
{
	InitUI();
	InitConnections();
}

FPreviewPlayerHeader::~FPreviewPlayerHeader()
{

}

void FPreviewPlayerHeader::InitUI()
{
	m_RedoBtn = new FButton(this);
	m_RedoBtn->setObjectName("FButton_Redo");
	m_UndoBtn = new FButton(this);
	m_UndoBtn->setObjectName("FButton_Undo");
	m_ShowOriginalBtn = new FButton(this);
	m_ShowOriginalBtn->setObjectName("FButton_ShowOriginal");
	m_CompareMenuBtn = new FButton(this);
	m_CompareMenuBtn->setObjectName("FButton_CompareMenu");
}

void FPreviewPlayerHeader::InitConnections()
{

}

void FPreviewPlayerHeader::resizeEvent(QResizeEvent *event)
{
	const int iBtnHeight = 25;
	const int iBtnWidth = 25;
	int iWidth = width();
	int iHeight = height();
	m_UndoBtn->setGeometry(3, (iHeight - iBtnWidth) / 2, iBtnHeight, iBtnWidth);
	int iLeft = m_UndoBtn->geometry().right() + 5;
	m_RedoBtn->setGeometry(iLeft, (iHeight - iBtnWidth) / 2, iBtnHeight, iBtnWidth);

	m_CompareMenuBtn->setGeometry(iWidth - iBtnWidth, (iHeight - iBtnWidth) / 2, iBtnHeight, iBtnWidth);
	iLeft = m_CompareMenuBtn->geometry().left() - iBtnWidth - 5;
	m_ShowOriginalBtn->setGeometry(iLeft, (iHeight - iBtnWidth) / 2, iBtnHeight, iBtnWidth);
}
