#include "inc_CommonLib/BaseDialog.h"

#include "inc_CommonLib/TopLevelWindowTitleBar.h"

BaseDialog::BaseDialog(const QString &title, QWidget *parent)
	: QDialog(parent)
	, m_titleHeight(34)
{
	setWindowTitle(title);
	m_titlebar = new TopLevelWindowTitleBar(this);
	m_titlebar->SetTitleName(title);
	m_titlebar->InitWindowMaximumButtonState(false);
	m_titlebar->SetButtons(TopLevelWindowTitleBar::Close);
	m_titlebar->SetResizeble(false);
	m_titlebar->SetDragMaximum(false);
}

BaseDialog::~BaseDialog()
{

}

void BaseDialog::SetTitle(QString title)
{
	m_titlebar->SetTitleName(title);
}

int BaseDialog::DoModal()
{
	return exec();
}

void BaseDialog::resizeEvent(QResizeEvent *event)
{
	m_titlebar->setGeometry(0, 0, width(), m_titleHeight);
}
