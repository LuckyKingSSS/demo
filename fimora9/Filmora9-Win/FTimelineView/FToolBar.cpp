#include "stdafx.h"
#include "FToolBar.h"

#include "inc_FCommonLib/FCommonWidgets.h"

FToolBar::FToolBar(QWidget *parent)
    : QWidget(parent)
{

    m_btnUndo = new FImageButton(Undo, "FTimelineView_FToolBar_Undo", this);
    m_btnRedo = new FImageButton(Redo, "FTimelineView_FToolBar_Redo", this);
    m_btnSplit = new FImageButton(Split, "FTimelineView_FToolBar_Split", this);

    connect(m_btnUndo, &FImageButton::Clicked, this, &FToolBar::slotButtonClicked);
    connect(m_btnRedo, &FImageButton::Clicked, this, &FToolBar::slotButtonClicked);
    connect(m_btnSplit, &FImageButton::Clicked, this, &FToolBar::slotButtonClicked);
}

FToolBar::~FToolBar()
{

}

void FToolBar::slotButtonClicked(int type)
{
    emit sigButtonClicked((ToolBarType)type);
}

void FToolBar::resizeEvent(QResizeEvent *event)
{
    const int spacing = 10;
    const int btnwidth = 22;
    int left = 20;
    int top = (height() - btnwidth) / 2;


    m_btnUndo->setGeometry(left, top, btnwidth, btnwidth);
    left = m_btnUndo->geometry().right() + spacing;
    m_btnRedo->setGeometry(left, top, btnwidth, btnwidth);
    left = m_btnRedo->geometry().right() + spacing;
    m_btnSplit->setGeometry(left, top, btnwidth, btnwidth);
    left = m_btnSplit->geometry().right() + spacing;

}
