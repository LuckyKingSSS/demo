#include "stdafx.h"
#include "SelectionWidget.h"

SelectionWidget::SelectionWidget(QWidget *parent)
	: QWidget(parent)
{
	setAttribute(Qt::WA_TransparentForMouseEvents);
	setVisible(false);
}

SelectionWidget::~SelectionWidget()
{

}

void SelectionWidget::Show(QRect rc, bool bShow)
{
	if (isVisible() != bShow)
		setVisible(bShow);

	if (bShow)
	{
		m_rcSelection = rc;
		update();
	}
}

void SelectionWidget::paintEvent(QPaintEvent *e)
{
	QPainter painter(this);
	QPen pen;
	pen.setColor(QColor(218, 209, 202));
	pen.setStyle(Qt::DotLine);
	pen.setWidth(1);
	painter.setPen(pen);
	painter.setBrush(Qt::transparent);
	painter.drawRect(m_rcSelection);
}
