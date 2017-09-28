#include "stdafx.h"
#include "inc_FCommonLib/FBaseDialog.h"

FBaseDialog::FBaseDialog(QWidget *parent)
: BaseDialog("", parent)
{

}

FBaseDialog::~FBaseDialog()
{

}

void FBaseDialog::SetTitle(QString title)
{
    BaseDialog::SetTitle(title);
}

void FBaseDialog::SetBorderWidth(int width)
{
    m_titlebar->GetControl()->SetBorderWidth(width);
}

int FBaseDialog::GetBorderWidth()
{
    return m_titlebar->GetControl()->GetBorderWidth();
}

void FBaseDialog::SetBorderColor(QColor color)
{
    m_titlebar->GetControl()->SetBorderColor(color);
}

QColor FBaseDialog::GetBorderColor()
{
    return m_titlebar->GetControl()->GetBorderColor();
}

int FBaseDialog::DoModal()
{
    return BaseDialog::DoModal();
}

void FBaseDialog::resizeEvent(QResizeEvent *event)
{
    m_titlebar->setGeometry(GetBorderWidth(), 0, width() - 2 * GetBorderWidth(), m_titleHeight);
}
