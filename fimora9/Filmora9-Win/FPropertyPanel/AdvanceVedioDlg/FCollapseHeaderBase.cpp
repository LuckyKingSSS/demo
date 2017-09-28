#include "stdafx.h"
#include "FCollapseHeaderBase.h"
#include "inc_FCommonLib\FCommonWidgets.h"

FCollapseHeaderBase::FCollapseHeaderBase(QWidget *parent)
	: FBaseWidget(parent)
{

}

FCollapseHeaderBase::~FCollapseHeaderBase()
{

}

void FCollapseHeaderBase::mousePressEvent(QMouseEvent *e)
{
	bool bCurState = GetCurState();
	emit sigCollapse(!bCurState);
	qDebug() << "mousePressEvent emit";
	SetCurState(!bCurState);
}

FContexBaseWidget::FContexBaseWidget(QWidget *parent) :FBaseWidget(parent)
{

}

FContexBaseWidget::~FContexBaseWidget()
{

}

void FContexBaseWidget::resetConfig()
{

}
