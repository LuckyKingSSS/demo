#include "stdafx.h"
#include "OutputSet.h"
#include "inc_CommonLib/TopLevelWindowTitleBar.h"

OutputSet::OutputSet(const QString & title, QWidget *pParent) 
:BaseDialog(title,pParent)
{
    SetButtons(TopLevelWindowTitleBar::Button::Close);
    setFixedSize(400, 400);
    SetResizeble(false);
    SetButtonWidth(40);
}


OutputSet::~OutputSet()
{
}

void OutputSet::SetButtons(int buttons)
{
    m_titlebar->SetButtons(buttons);
}

void OutputSet::SetResizeble(bool beResizeble)
{
    m_titlebar->SetResizeble(beResizeble);
}

void OutputSet::SetButtonWidth(int width)
{
    m_titlebar->SetButtonWidth(width);
}
