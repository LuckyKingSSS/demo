#include "stdafx.h"
#include "FTypeLabel.h"

FTypeLabel::FTypeLabel(QWidget *parent)
	: FLabel(parent)
{

}

FTypeLabel::~FTypeLabel()
{

}

void FTypeLabel::mousePressEvent(QMouseEvent *event)
{
	QString strText = text();
	emit sigClicked(strText);
}

