#include "stdafx.h"
#include "inc_FCommonLib/FCommonWidgets.h"

#include "inc_CommonLib/externalapi.h"


FImageButton::FImageButton(int id, const QString &objectname, QWidget *parent)
:BaseButton(id, objectname, parent)
{

}

FImageButton::FImageButton(const QString &objectname, QWidget *parent)
: BaseButton(objectname, parent)
{

}

FStateButton::FStateButton(int id, const QString &objectname, QWidget *parent)
: BaseStateButton(id, objectname, parent)
{

}

FButton::FButton(QWidget *parent)
: QPushButton(parent)
{

}

FAction::FAction(QObject *parent)
: QAction(parent)
{

}

FMenu::FMenu(QWidget *parent)
: QMenu(parent)
{

}

FLabel::FLabel(QWidget *parent)
: QLabel(parent)
{
	//setAttribute(Qt::WA_StyledBackground);
}

FListWidget::FListWidget(QWidget *parent)
: QListWidget(parent)
{

}

FLineEdit::FLineEdit(QWidget *parent)
: QLineEdit(parent)
{

}

FComboBox::FComboBox(QWidget *parent)
: QComboBox(parent)
{
    setAttribute(Qt::WA_StyledBackground);
    this->setView(static_cast<QAbstractItemView *>(new QListView(this)));
    m_autoFixedMenuWidth = true;
}

void FComboBox::SetAutoFixedMenuWidth(bool enabled)
{
    m_autoFixedMenuWidth = enabled;

}

void FComboBox::FixedMenuWidth()
{
    int count = this->count();
    int wTemp = width();
    for (int i = 0; i < count; ++i)
    {
        wTemp = qMax(wTemp, gGetRetranslateUiTextLenght(view()->font(), itemText(i), wTemp - 20, 20));
    }
    //wTemp += 20;
    view()->setMinimumWidth(wTemp);
}

void FComboBox::mousePressEvent(QMouseEvent *event)
{
    if (m_autoFixedMenuWidth)
        FixedMenuWidth();
    QComboBox::mousePressEvent(event);
}


FSpinBox::FSpinBox(QWidget *parent)
:QSpinBox(parent)
{

}

FCheckBox::FCheckBox(QWidget *parent)
: QCheckBox(parent)
{

}
FRadioButton::FRadioButton(QWidget *parent)
: QRadioButton(parent)
{

}

FScrollArea::FScrollArea(QWidget *parent)
	: QScrollArea(parent)
{

}
