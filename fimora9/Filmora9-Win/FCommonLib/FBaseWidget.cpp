#include "stdafx.h"
#include "inc_FCommonLib/FBaseWidget.h"

FBaseWidget::FBaseWidget(QWidget *parent)
    : QWidget(parent)
{
	setAttribute(Qt::WA_StyledBackground);
}

void FBaseWidget::RetranslateUi()
{

}

void FBaseWidget::TranslateText()
{

}

HWND FBaseWidget::getHwnd()
{
	return (HWND)winId();
}

bool FBaseWidget::event(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange)
    {
        RetranslateUi();
    }
    return QWidget::event(event);
}

//////////////////////////////////////////////////////////////////////////
FUpdateBaseWidget::FUpdateBaseWidget(QWidget *parent)
:FBaseWidget(parent)
{

}

void FUpdateBaseWidget::Update()
{

}
