#include "stdafx.h"
#include "FSPropertyView.h"
#include "FSEditHelper.h"
#include "inc_FSMediaPlayerView/IFSMediaPlayer.h"
#include "inc_CommonLib/GlobalResources.h"
#include "inc_CommonLib/NLEWraper.h"
#include "inc_CommonLib/CommonWidgets.h"
//#include "FSColorSelecter.h"
#include "Interface/INLECaptionTipTextObj.h"
#include "Interface/INLECaptionTextObj.h"
#include "Interface/INLECaptionShapeObj.h"

#include "Include/NLECaptionDefine.h" 
#include "Interface/INLECaptionBaseObj.h"
#include "inc_FSTimelineView/fstimelineview.h"
#include "FSUnDoReDoCommand.h"
#include "inc_FSTimelineView/UndoCommandBase.h"
#include "inc_FSCommonlib/gathread.h"
#include "inc_FSCommonlib/ExternalAPI.h"
#include "inc_CommonLib/DragValueChangedHelper.h"
#include "inc_CommonLib/externalapi.h"
#include "inc_FSCommonlib/FSTipWidget.h"
#include "inc_CommonLib/ColorSelecterComboBox.h"
#include "FSTextShadowSelecter.h"

IFSOptionView::IFSOptionView(OptionType type, QWidget *pParent )
    :FSBaseWidget(pParent),
    mType(type)
{
    QVBoxLayout *pMainVL = new QVBoxLayout(this);
    m_pOptionWidget = new QWidget(this);
    m_pTitleWidget = new FSPropertyTitle(this);
    pMainVL->addWidget(m_pTitleWidget);
    pMainVL->addWidget(m_pOptionWidget);
    pMainVL->setMargin(0);
    pMainVL->setSpacing(0);
    ConnectSignals();
    m_pOptionWidget->setObjectName("IFSOptionView_m_pOptionWidget");
}

IFSOptionView::~IFSOptionView()
{

}

IFSOptionView::OptionType IFSOptionView::GetOptionType()
{
    return mType;
}

void IFSOptionView::SetOptionType(OptionType type)
{
    mType = type;
}

void IFSOptionView::SetTitle(const QString &title)
{
    m_pTitleWidget->SetTitleName(title);
}

void IFSOptionView::SetCandelete(bool bCandelete)
{
    m_pTitleWidget->SetCandelete(bCandelete);
}


void IFSOptionView::OnReset()
{

}

void IFSOptionView::showEvent(QShowEvent *event)
{
    RetranslateUi();
    FSBaseWidget::showEvent(event);
}

void IFSOptionView::ConnectSignals()
{
    connect(m_pTitleWidget, &FSPropertyTitle::Delete, this, &IFSOptionView::OnDeleteSlot);
    connect(m_pTitleWidget, &FSPropertyTitle::ShowView, this, &IFSOptionView::OnShowViewSlot);
    connect(m_pTitleWidget, &FSPropertyTitle::Reset, this, &IFSOptionView::OnReset);
}

void IFSOptionView::OnDeleteSlot()
{
    setVisible(false);
    emit DeletePropoty();
}

void IFSOptionView::OnShowViewSlot(bool bShow)
{
    m_pOptionWidget->setVisible(bShow);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FSVisualBaseOption::FSVisualBaseOption(QWidget *pParent)
    :IFSOptionView(IFSOptionView::TypeVisualBase,pParent)
    , m_ScaleX(1.0)
    , m_ScaleY(1.0)
    , m_bChangeFlag(0)
{
    CreateUI();
    RetranslateUi();
    InitRange();
    ConnectPositionXValueChanged(true);
    ConnectPositionYValueChanged(true);
    ConnectRotationValueChanged(true);
    ConnectScaleValueChanged(true);
}

FSVisualBaseOption::~FSVisualBaseOption()
{

}


void FSVisualBaseOption::UpdataParma(IFSEditHelper *pHelper)
{
    Q_ASSERT(pHelper);
    bool reg,reg1,reg2;
    if (pHelper)
    {
        IFSEditHelper::EditType type = pHelper->GetEditType();
        double positionX=0, positionY=0, rolation=0;
        pHelper->GetPosition(positionX, positionY);
        pHelper->GetScale(m_ScaleX, m_ScaleY);
        pHelper->GetDefaultAngle(m_defaultAngle);
        pHelper->GetDefaultPosiont(m_defaultPosX, m_defaultPosY);
        pHelper->GetDefaultScale(m_defaultScaleX, m_defaultScaleY);
        m_ScaleDisplay = qMax(m_ScaleX, m_ScaleY);
        reg = UpdatePosition(positionX, positionY);
        reg1 = UpdateScale(qMax(m_ScaleX, m_ScaleY), qMax(m_ScaleX, m_ScaleY));
        reg2 = UpdateRotation(pHelper->GetAngle());
        m_pTitleWidget->SetResetable(reg || reg1 || reg2);
    }
    
}

void FSVisualBaseOption::OnReset()
{
    ConnectPositionXValueChanged(false);
    ConnectPositionYValueChanged(false);
    ConnectRotationValueChanged(false);
    ConnectScaleValueChanged(false);
    m_ScaleDisplay = qMax(m_defaultScaleX, m_defaultScaleY);
    m_pDoubleSpinBoxScale->setValue(qMax(m_defaultScaleX, m_defaultScaleY) * 100);
    if (qAbs(m_defaultScaleY - m_ScaleY)>1e-4 || qAbs(m_defaultScaleX - m_ScaleX)>1e-4)
        m_bChangeFlag |= 0x2;
    m_ScaleX = m_defaultScaleX;
    m_ScaleY = m_defaultScaleY;
    
    double angle, posx, posy;
    GetRotation(angle);
    GetPosition(posx, posy);
    if (qAbs(m_defaultAngle - angle)>1e-2)
        m_bChangeFlag |= 0x4;
    if (qAbs(m_defaultPosX - posx)>1e-2 || qAbs(m_defaultPosY - posy)>1e-2)
        m_bChangeFlag |= 0x1;
    m_pDoubleSpinBoxRotation->setValue(m_defaultAngle);
    m_pDoubleSpinBoxPositionX->setValue(m_defaultPosX);
    m_pDoubleSpinBoxPositionY->setValue(m_defaultPosY);
    m_pButtonScale->setVisible(false);
    m_pButtonRotation->setVisible(false);
    m_pButtonPositionX->setVisible(false);
    m_pButtonPositionY->setVisible(false);
    m_pTitleWidget->SetResetable(IsTitleResetable());
    ConnectPositionXValueChanged(true);
    ConnectPositionYValueChanged(true);
    ConnectRotationValueChanged(true);
    ConnectScaleValueChanged(true);
    emit ParmaChanged();
}

bool FSVisualBaseOption::IsTitleResetable()
{
    return m_pButtonPositionY->isVisible() || m_pButtonPositionX->isVisible() || m_pButtonRotation->isVisible() || m_pButtonScale->isVisible();
}

bool FSVisualBaseOption::UpdateScale(double x, double y)
{
    Q_UNUSED(y);
    bool reg = (qAbs(x -qMax(m_defaultScaleX,m_defaultScaleY))>1e-2);
    ConnectScaleValueChanged(false);
    m_pButtonScale->setVisible(reg);
    m_pDoubleSpinBoxScale->setValue(x * 100);
    ConnectScaleValueChanged(true);
    return reg;
}

bool FSVisualBaseOption::UpdatePosition(double x, double y)
{
    bool regX = qAbs(x - m_defaultPosX) > 1e-1;
    bool regY = qAbs(y - m_defaultPosY) > 1e-1;
    ConnectPositionXValueChanged(false);
    ConnectPositionYValueChanged(false);
    m_pButtonPositionX->setVisible(regX);
    m_pButtonPositionY->setVisible(regY);
    m_pDoubleSpinBoxPositionX->setValue(x);
    m_pDoubleSpinBoxPositionY->setValue(y);
    ConnectPositionXValueChanged(true);
    ConnectPositionYValueChanged(true);
    return regX||regY;
}

bool FSVisualBaseOption::UpdateRotation(double rotation)
{
    if (qAbs(rotation - 360) < 1e-1)
        rotation = 0;
    bool reg = (qAbs(rotation - m_defaultAngle)>1e-2);
    ConnectRotationValueChanged(false);
    m_pButtonRotation->setVisible(reg);
    m_pDoubleSpinBoxRotation->setValue(rotation);
    ConnectRotationValueChanged(true);
    return reg;
}



void FSVisualBaseOption::GetPosition(double & positionX, double &positionY)
{
    positionX=m_pDoubleSpinBoxPositionX->text().toDouble();
    positionY = m_pDoubleSpinBoxPositionY->text().toDouble();
}

void FSVisualBaseOption::GetRotation(double & rotation)
{
    QString text = m_pDoubleSpinBoxRotation->text();
    rotation = text.left(text.length()-1).toDouble();
}

void FSVisualBaseOption::GetScale(double &x, double& y)
{
    if (m_ScaleX > m_ScaleY)
    {
        double tempScaleX = m_ScaleX;
        m_ScaleX = m_ScaleDisplay;
        m_ScaleY = m_ScaleY*m_ScaleX / tempScaleX;
    }
    else{
        double tempScaleY = m_ScaleY;
        m_ScaleY = m_ScaleDisplay;
        m_ScaleX = m_ScaleX*m_ScaleY / tempScaleY;
    }
    x = m_ScaleX;
    y = m_ScaleY;
}


void FSVisualBaseOption::ResetChangeFlag()
{
    m_bChangeFlag = 0;
}

bool FSVisualBaseOption::isPosChanged()
{
    return m_bChangeFlag & 0x1;
}

bool FSVisualBaseOption::isScaleChanged()
{
    return m_bChangeFlag & 0x2;
}

bool FSVisualBaseOption::isAngleChanged()
{
    return m_bChangeFlag & 0x4;
}

void FSVisualBaseOption::CreateUI()
{
    m_pLabelScale     = new FSLabel(m_pOptionWidget);
    m_pLabelRotation = new FSLabel(m_pOptionWidget);
    m_pLabelPosition = new FSLabel(m_pOptionWidget);
    m_pLabelPositionX = new FSLabel(m_pOptionWidget);
    m_pLabelPositionY = new FSLabel(m_pOptionWidget);

    QLabel *pLabel1 = new QLabel(m_pOptionWidget);
    QLabel *pLabel2 = new QLabel(m_pOptionWidget);
    QLabel *pLabel3 = new QLabel(m_pOptionWidget);
    QLabel *pLabel4 = new QLabel(m_pOptionWidget);
    QLabel *pLabel5 = new QLabel(m_pOptionWidget);

    m_pGroupButton    = new QButtonGroup(m_pOptionWidget);
    m_pButtonScale    = new FSButton(pLabel1);
    m_pButtonRotation = new FSButton(pLabel2);
    m_pButtonPositionX = new FSButton(pLabel3);
    m_pButtonPositionY = new FSButton(pLabel4);
    pLabel1->setFixedSize(14, 14);
    pLabel2->setFixedSize(14, 14);
    pLabel3->setFixedSize(14, 14);
    pLabel4->setFixedSize(14, 14);

    QButtonGroup  *pButtonGroup = new QButtonGroup(m_pOptionWidget);
    pButtonGroup->addButton(m_pButtonScale, 0);
    pButtonGroup->addButton(m_pButtonRotation, 1);
    pButtonGroup->addButton(m_pButtonPositionX, 2);
    pButtonGroup->addButton(m_pButtonPositionY, 3);
    m_pDoubleSpinBoxScale = new FSDoubleSpinBox(m_pOptionWidget);
    m_pDoubleSpinBoxRotation = new AngleDoubleSpinBox(m_pOptionWidget);
    m_pDoubleSpinBoxPositionX = new FSDoubleSpinBox(m_pOptionWidget);
    m_pDoubleSpinBoxPositionY = new FSDoubleSpinBox(m_pOptionWidget);
	//设置spinbox对应的精度
	m_pDoubleSpinBoxScale->setDecimals(0);
	m_pDoubleSpinBoxRotation->setDecimals(1);
	m_pDoubleSpinBoxPositionX->setDecimals(1);
	m_pDoubleSpinBoxPositionY->setDecimals(1);
    
    QString str = QString("%1").arg(QChar(0x00B0));
    m_pDoubleSpinBoxRotation->setSuffix(str);

    m_pDoubleSpinBoxScale->setSuffix("%");

    QHBoxLayout *pHL1 = new QHBoxLayout();
    QHBoxLayout *pHL11 = new QHBoxLayout();
    QHBoxLayout *pHL111 = new QHBoxLayout();
    QHBoxLayout *pHL2 = new QHBoxLayout();
    QHBoxLayout *pHL22 = new QHBoxLayout();
    QHBoxLayout *pHL222 = new QHBoxLayout();
    QHBoxLayout *pHL3 = new QHBoxLayout();
    QHBoxLayout *pHL33 = new QHBoxLayout();
    QHBoxLayout *pHL333 = new QHBoxLayout();
    QHBoxLayout *pHL4 = new QHBoxLayout();
    QHBoxLayout *pHL44 = new QHBoxLayout();
    QHBoxLayout *pHL444 = new QHBoxLayout();
    QVBoxLayout *pVL1 = new QVBoxLayout(m_pOptionWidget);


    int space = 12, L = 14, R = 21, w = 50, h = 20, w1 = 12, btnW = 14,T=10,B=12;

    pHL11->addSpacing(w1+4);
    pHL11->addWidget(m_pDoubleSpinBoxScale);
    pHL11->addStretch();
    pHL11->addWidget(pLabel1);
    pHL11->setSpacing(0);
    pHL11->setMargin(0);

    pHL111->addStretch();
    pHL111->addWidget(m_pLabelScale);
    pHL111->setSpacing(0);
    pHL111->setMargin(0);

    pHL1->addLayout(pHL111);
    pHL1->addLayout(pHL11);
    pHL1->setSpacing(6);

    pHL22->addSpacing(w1+4);
    pHL22->addWidget(m_pDoubleSpinBoxRotation);
    pHL22->addStretch();
    pHL22->addWidget(pLabel2);
    pHL22->setSpacing(0);
    pHL22->setMargin(0);

    pHL222->addStretch();
    pHL222->addWidget(m_pLabelRotation);
    pHL222->setSpacing(0);
    pHL222->setMargin(0);

    pHL2->addLayout(pHL222);
    pHL2->addLayout(pHL22);
    pHL2->setSpacing(6);

    pHL33->addWidget(m_pLabelPositionX);
    pHL33->addSpacing(4);
    pHL33->addWidget(m_pDoubleSpinBoxPositionX);
    pHL33->addStretch();
    pHL33->addWidget(pLabel3);
    pHL33->setSpacing(0);
    pHL33->setMargin(0);

    pHL333->addStretch();
    pHL333->addWidget(m_pLabelPosition);
    pHL333->setSpacing(0);
    pHL333->setMargin(0);

    pHL3->addLayout(pHL333);
    pHL3->addLayout(pHL33);
    pHL3->setSpacing(6);

    pHL44->addWidget(m_pLabelPositionY);
    pHL44->addSpacing(4);
    pHL44->addWidget(m_pDoubleSpinBoxPositionY);
    pHL44->addStretch();
    pHL44->addWidget(pLabel4);
    pHL44->setSpacing(0);
    pHL44->setMargin(0);

    pHL444->addStretch();
    pHL444->addWidget(pLabel5);
    pHL444->setSpacing(0);
    pHL444->setMargin(0);

    pHL4->addLayout(pHL444);
    pHL4->addLayout(pHL44);
    pHL4->setSpacing(6);

    pVL1->addLayout(pHL1);
    pVL1->addLayout(pHL2);
    pVL1->addSpacing(15);
    pVL1->addLayout(pHL3);
    pVL1->addLayout(pHL4);
    pVL1->addStretch();
    pVL1->setSpacing(10);
    pVL1->setContentsMargins(L, T, R, B);

    m_pDoubleSpinBoxScale->setButtonSymbols(QAbstractSpinBox::NoButtons);
    m_pDoubleSpinBoxRotation->setButtonSymbols(QAbstractSpinBox::NoButtons);
    m_pDoubleSpinBoxPositionX->setButtonSymbols(QAbstractSpinBox::NoButtons);
    m_pDoubleSpinBoxPositionY->setButtonSymbols(QAbstractSpinBox::NoButtons);

    m_pLabelPositionX->setFixedSize(w1, h);
    m_pLabelPositionY->setFixedSize(w1, h);

    m_pButtonScale->setFixedSize(btnW, btnW);
    m_pButtonRotation->setFixedSize(btnW, btnW);
    m_pButtonPositionX->setFixedSize(btnW, btnW);
    m_pButtonPositionY->setFixedSize(btnW, btnW);

    m_pDoubleSpinBoxScale->setFixedSize(w, h);
    m_pDoubleSpinBoxRotation->setFixedSize(w, h);
    m_pDoubleSpinBoxPositionX->setFixedSize(w, h);
    m_pDoubleSpinBoxPositionY->setFixedSize(w, h);


    pLabel5->setFixedSize(w, h);
    m_pLabelScale->setObjectName("FSVisualPorpertyView_pLabelScale");
    m_pLabelRotation->setObjectName("FSVisualPorpertyView_pLabelRotation");
    m_pLabelPosition->setObjectName("FSVisualPorpertyView_pLabelPosition");
    m_pLabelPositionX->setObjectName("FSVisualPorpertyView_pLabelPositionX");
    m_pLabelPositionY->setObjectName("FSVisualPorpertyView_pLabelPositionY");
    m_pButtonScale->setObjectName("FSPropertyTitle_m_pResetBtn");
    m_pButtonRotation->setObjectName("FSPropertyTitle_m_pResetBtn");
    m_pButtonPositionX->setObjectName("FSPropertyTitle_m_pResetBtn");
    m_pButtonPositionY->setObjectName("FSPropertyTitle_m_pResetBtn");
    m_pDoubleSpinBoxScale->setObjectName("FSVisualPorpertyView_pDoubleSpinBox");
    m_pDoubleSpinBoxRotation->setObjectName("FSVisualPorpertyView_pDoubleSpinBox");
    m_pDoubleSpinBoxPositionX->setObjectName("FSVisualPorpertyView_pDoubleSpinBox");
    m_pDoubleSpinBoxPositionY->setObjectName("FSVisualPorpertyView_pDoubleSpinBox");

    connect(m_pDoubleSpinBoxScale, SIGNAL(valueChanged(double)), this, SLOT(ChangeSliderValueSlot(double)));
    connect(pButtonGroup, SIGNAL(buttonClicked(int)), this, SLOT(OnButtonClieckedSlot(int)));
}

void FSVisualBaseOption::RetranslateUi()
{
    m_pLabelScale->setText(tr("Scale"));
    m_pLabelRotation->setText(tr("Rotation"));
    m_pLabelPosition->setText(tr("Position"));
    m_pLabelPositionX->setText(tr("X"));
    m_pLabelPositionY->setText(tr("Y"));

    m_pLabelScale->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_pLabelRotation->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_pLabelPosition->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_pLabelPositionX->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_pLabelPositionY->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    int w = 63,h=20;
    int w1 = GetRetranslateUiTextLenght(m_pLabelScale->font(), m_pLabelScale->text(), w);
    int w2 = GetRetranslateUiTextLenght(m_pLabelRotation->font(), m_pLabelRotation->text(), w);
    int w3 = GetRetranslateUiTextLenght(m_pLabelPosition->font(), m_pLabelPosition->text(), w);
    w = qMax(w1, w);
    w = qMax(w2, w);
    w = qMax(w3, w);
    m_pLabelScale->setFixedSize(w, h);
    m_pLabelRotation->setFixedSize(w,h);
    m_pLabelPosition->setFixedSize(w, h);
    
}

void FSVisualBaseOption::ConnectScaleValueChanged(bool bConnect)
{
    if (bConnect)
    {
        connect(m_pDoubleSpinBoxScale, SIGNAL(valueChanged(double)), this, SLOT(OnScaleValueChangedSlot(double)));
    }
    else
    {
        disconnect(m_pDoubleSpinBoxScale, SIGNAL(valueChanged(double)), this, SLOT(OnScaleValueChangedSlot(double)));
    }
}

void FSVisualBaseOption::ConnectPositionXValueChanged(bool bConnect)
{
    if (bConnect)
    {
        connect(m_pDoubleSpinBoxPositionX, SIGNAL(valueChanged(double)), this, SLOT(OnPositionXValueChangedSlot(double)));
    }
    else
    {
        disconnect(m_pDoubleSpinBoxPositionX, SIGNAL(valueChanged(double)), this, SLOT(OnPositionXValueChangedSlot(double)));
    }
}

void FSVisualBaseOption::ConnectPositionYValueChanged(bool bConnect)
{
    if (bConnect)
    {
        connect(m_pDoubleSpinBoxPositionY, SIGNAL(valueChanged(double)), this, SLOT(OnPositionYValueChangedSlot(double)));
    }
    else
    {
        disconnect(m_pDoubleSpinBoxPositionY, SIGNAL(valueChanged(double)), this, SLOT(OnPositionYValueChangedSlot(double)));
    }
}

void FSVisualBaseOption::ConnectRotationValueChanged(bool bConnect)
{
    if (bConnect)
    {
        connect(m_pDoubleSpinBoxRotation, SIGNAL(valueChanged(double)), this, SLOT(OnRotationValueChangedSlot(double)));
    }
    else
    {
        disconnect(m_pDoubleSpinBoxRotation, SIGNAL(valueChanged(double)), this, SLOT(OnRotationValueChangedSlot(double)));
    }
}

void FSVisualBaseOption::InitRange()
{
    m_pDoubleSpinBoxScale->setRange(5, 500);
    m_pDoubleSpinBoxScale->setValue(100);
    m_pDoubleSpinBoxPositionX->setRange(-90000, 90000);
    m_pDoubleSpinBoxPositionX->setValue(0);
    m_pDoubleSpinBoxPositionY->setRange(-90000, 90000);
    m_pDoubleSpinBoxPositionY->setValue(0);
    m_pDoubleSpinBoxRotation->setRange(0, 359);
    m_pDoubleSpinBoxRotation->setValue(0);
    m_pDoubleSpinBoxRotation->setFocusPolicy(Qt::ClickFocus);
    m_pDoubleSpinBoxPositionX->setFocusPolicy(Qt::ClickFocus);
    m_pDoubleSpinBoxPositionY->setFocusPolicy(Qt::ClickFocus);
    m_pDoubleSpinBoxScale->setFocusPolicy(Qt::ClickFocus);
    m_pButtonPositionX->setVisible(false);
    m_pButtonPositionY->setVisible(false);
    m_pButtonScale->setVisible(false);
    m_pButtonRotation->setVisible(false);
}

void FSVisualBaseOption::OnScaleValueChangedSlot(double value)
{
    m_ScaleDisplay = value/100.0;
    m_pButtonScale->setVisible(qAbs(value-qMax(m_defaultScaleX,m_defaultScaleY)*100)>1e-2);
    m_pTitleWidget->SetResetable(IsTitleResetable());
    m_bChangeFlag |= 0x2;
    emit ParmaChanged();
}

void FSVisualBaseOption::OnPositionXValueChangedSlot(double value)
{
    Q_UNUSED(value);
    m_pButtonPositionX->setVisible(qAbs(m_pDoubleSpinBoxPositionX->value() - m_defaultPosX)>1e-2);
    m_pTitleWidget->SetResetable(IsTitleResetable());
    m_bChangeFlag |= 0x1;
    emit ParmaChanged();
}

void FSVisualBaseOption::OnPositionYValueChangedSlot(double value)
{
    Q_UNUSED(value);
    m_pButtonPositionY->setVisible(qAbs(m_pDoubleSpinBoxPositionY->value() - m_defaultPosY) > 1e-2);
    m_pTitleWidget->SetResetable(IsTitleResetable());
    m_bChangeFlag |= 0x1;
    emit ParmaChanged();
}

void FSVisualBaseOption::OnRotationValueChangedSlot(double value)
{
    m_pButtonRotation->setVisible(qAbs(value - m_defaultAngle)>1e-2);
    m_pTitleWidget->SetResetable(IsTitleResetable());
    m_bChangeFlag |= 0x4;
    emit ParmaChanged();
}

void FSVisualBaseOption::OnButtonClieckedSlot(int id)
{
    QButtonGroup *pBtn = static_cast<QButtonGroup *>(sender());
    Q_ASSERT(pBtn);
    pBtn->button(id)->setVisible(false);
    switch (id)
    {
    case 0:
        m_ScaleX = m_defaultScaleX;
        m_ScaleY = m_defaultScaleY;
        m_bChangeFlag |= 0x2;
        ConnectScaleValueChanged(false);
        m_ScaleDisplay = qMax(m_defaultScaleX, m_defaultScaleY);
        m_pDoubleSpinBoxScale->setValue(qMax(m_defaultScaleX, m_defaultScaleY) * 100);
        ConnectScaleValueChanged(true);
        m_pTitleWidget->SetResetable(IsTitleResetable());
        emit ParmaChanged();
        break;
    case 1:
        m_bChangeFlag |= 0x4;
        m_pDoubleSpinBoxRotation->blockSignals(true);
        m_pDoubleSpinBoxRotation->setValue(m_defaultAngle); 
        m_pDoubleSpinBoxRotation->blockSignals(false);
        m_pTitleWidget->SetResetable(IsTitleResetable());
        emit ParmaChanged();
        break;
    case 2:
        m_bChangeFlag |= 0x1;
        m_pDoubleSpinBoxPositionX->blockSignals(true);
        m_pDoubleSpinBoxPositionX->setValue(m_defaultPosX);
        m_pDoubleSpinBoxPositionX->blockSignals(false);
        m_pTitleWidget->SetResetable(IsTitleResetable());
        emit ParmaChanged();
        break;
    case 3:
        m_bChangeFlag |= 0x1;
        m_pDoubleSpinBoxPositionY->blockSignals(true);
        m_pDoubleSpinBoxPositionY->setValue(m_defaultPosY);
        m_pDoubleSpinBoxPositionY->blockSignals(false);
        m_pTitleWidget->SetResetable(IsTitleResetable());
        emit ParmaChanged();
        break;
    default:
        break;
    }
}


/************************************************************************/
/*                          FSPropertyView                              */
/************************************************************************/
FSPropertyView::FSPropertyView(QWidget *parent)
    : FSBaseWidget(parent)
    , m_pEditorHelperManager(NULL)
    , m_pPlayerEditorManager(NULL)
    , m_pEditHelper(NULL)
    , m_MouseEffictType(Mouse_UnKown)
{
    CreateUI();
    RetranslateUi();
    ConnectSignal();
    SetEditHelper(NULL);
}

FSPropertyView::~FSPropertyView()
{

}

void FSPropertyView::resizeEvent(QResizeEvent *e)
{
    m_pStyleTitle->setGeometry(0, 0, m_pWidgetMain->width(), 35);
    m_pButtonShowProperty->move(width() - m_pButtonShowProperty->width(), 0);
    QWidget::resizeEvent(e);
}

void FSPropertyView::SetEditHelperManager(IFSEditHelperManager *pEditorHelperManager)
{
    Q_ASSERT(pEditorHelperManager);
    m_pEditorHelperManager = pEditorHelperManager;
    connect(pEditorHelperManager, &IFSEditHelperManager::CurrentEditHelperChanged, this, &FSPropertyView::SetEditHelper, Qt::UniqueConnection);
    connect(pEditorHelperManager, &IFSEditHelperManager::TransformInfoChanged, this, &FSPropertyView::OnTransformInfoChangedSlot, Qt::UniqueConnection);
    connect(pEditorHelperManager, &IFSEditHelperManager::EndChanged, this, &FSPropertyView::OnEndChangedSlot, Qt::UniqueConnection);
    connect(pEditorHelperManager, &IFSEditHelperManager::MouseEffictAdded, this, &FSPropertyView::OnMouseEffictAdded, Qt::UniqueConnection);
    connect(pEditorHelperManager, &IFSEditHelperManager::TextChanged, this, &FSPropertyView::OnTextChanged, Qt::UniqueConnection);
}

void FSPropertyView::SetPlayerEditManager(IPlayerEditorManager *pPlayerEditManager)
{
    Q_ASSERT(pPlayerEditManager);
    m_pPlayerEditorManager = pPlayerEditManager;
}

void FSPropertyView::SetEditHelper(IFSEditHelper *pHelper)
{
    FSNLETimeline *pTimeLine = NULL;
    NLECaptionObjData objData;
    objData.pObj = NULL;
    if (pHelper)
    {
        connect(pHelper, &IFSEditHelper::CmdUpdate, this, &FSPropertyView::OnCmdUpdataSlot, Qt::UniqueConnection);
	}
	
	if (pHelper == NULL){
		m_pEditHelper = pHelper;
		SetVisibleView(0X00);
		m_pStachedWidget->currentWidget()->setVisible(false);
	}
    else if ((m_pEditHelper != pHelper&&pHelper != NULL) || (m_MouseEffictType != Mouse_UnKown))
    {
        m_pEditHelper = pHelper;
        pTimeLine = m_pEditHelper->GetTimeLine();
        TimelineType type = pTimeLine->GetType();
        switch (type)
        {
        case TimelineType_Video:
            {               
                if (!pTimeLine->IsDesktopVideo()){                    
                    SetVisibleView(0x01);
                    m_pStachedWidget->setCurrentIndex(0);
                    m_pButtonVisual->setChecked(true); 
                }
                else
                {
                    SetVisibleView(0x19);
    
                    if (m_MouseEffictType != Mouse_UnKown)
                    {
                        m_pStachedWidget->setCurrentIndex(3);
                        m_pButtonCursor->setChecked(true);
                        m_MouseEffictType = Mouse_UnKown;
                    }
                    else if (m_pStachedWidget->currentIndex() != 0){
                        m_pStachedWidget->setCurrentIndex(0); 
                        m_pButtonVisual->setChecked(true);
                    }                    
                    QString strPath;
                    INLETimelineBase *pBaseTimeline = NULL;
                    
                    GetOptionView(IFSOptionView::TypeCursorClickRing)->setVisible(static_cast<FSVideoPicEditHelper *>(m_pEditHelper)->IsContainMouseRings(&pBaseTimeline));
                    GetOptionView(IFSOptionView::TypeCursorClickSound)->setVisible(static_cast<FSVideoPicEditHelper *>(m_pEditHelper)->IsContainMouseSound(strPath));
                    GetOptionView(IFSOptionView::TypeCursorHighlight)->setVisible(static_cast<FSVideoPicEditHelper *>(m_pEditHelper)->IsContainMouseHighlight(&pBaseTimeline));
                    QStringList list=m_pEditHelper->GetMouseEffectOrderList();
                    list.push_front("FSCursorBaseOption");
                    m_pScrollAreaCursor->SortWidget(list);
                }
                GetOptionView(IFSOptionView::TypeVisualBase)->setVisible(true);
                GetOptionView(IFSOptionView::TypeVideoAudioSpeed)->setVisible(true);
                GetOptionView(IFSOptionView::TypeAudioSpeed)->setVisible(false);
                m_pStachedWidget->currentWidget()->setVisible(true);
            }
            break;
        case TimelineType_Audio:
            {
                SetVisibleView(0x02);
                m_pStachedWidget->setCurrentIndex(1);               
                m_pButtonAudio->setChecked(true);   
                GetOptionView(IFSOptionView::TypeVideoAudioSpeed)->setVisible(false);
                GetOptionView(IFSOptionView::TypeAudioSpeed)->setVisible(true);
                m_pStachedWidget->currentWidget()->setVisible(true);
            }
            break;
        case TimelineType_VideoAudio:
            {
                m_pStachedWidget->currentWidget()->setVisible(false);
                if (!pTimeLine->IsDesktopVideo()){
                    SetVisibleView(0x03);
                    if (m_pStachedWidget->currentIndex() != 0){
                        m_pStachedWidget->setCurrentIndex(0);
                    }
                    m_pButtonVisual->setChecked(true);                    
                }
                else
                {
                    SetVisibleView(0x1B);                  
                    if (m_MouseEffictType != Mouse_UnKown)
                    {
                        m_pStachedWidget->setCurrentIndex(3);
                        m_pButtonCursor->setChecked(true);
                        m_MouseEffictType = Mouse_UnKown;
                    }
                    else if (m_pStachedWidget->currentIndex() != 0 ){
                        m_pStachedWidget->setCurrentIndex(0);
                        m_pButtonVisual->setChecked(true);
                    }                    
                    QString strPath;
                    INLETimelineBase *pBaseTimeline = NULL;
                    
                    GetOptionView(IFSOptionView::TypeCursorClickRing)->setVisible(static_cast<FSVideoPicEditHelper *>(m_pEditHelper)->IsContainMouseRings(&pBaseTimeline));
                    GetOptionView(IFSOptionView::TypeCursorClickSound)->setVisible(static_cast<FSVideoPicEditHelper *>(m_pEditHelper)->IsContainMouseSound(strPath));
                    GetOptionView(IFSOptionView::TypeCursorHighlight)->setVisible(static_cast<FSVideoPicEditHelper *>(m_pEditHelper)->IsContainMouseHighlight(&pBaseTimeline));
                    QStringList list = m_pEditHelper->GetMouseEffectOrderList();
                    list.push_front("FSCursorBaseOption");
                    m_pScrollAreaCursor->SortWidget(list);
                }
                GetOptionView(IFSOptionView::TypeVisualBase)->setVisible(true);
                GetOptionView(IFSOptionView::TypeVideoAudioSpeed)->setVisible(true);
                GetOptionView(IFSOptionView::TypeAudioSpeed)->setVisible(false);
                m_pStachedWidget->currentWidget()->setVisible(true);
            }
            break;
        case TimelineType_Annotaions_Overlay:
        case TimelineType_Image:
            SetVisibleView(0x01);
            m_pStachedWidget->setCurrentIndex(0);            
            m_pButtonVisual->setChecked(true);
            GetOptionView(IFSOptionView::TypeVisualBase)->setVisible(true);
            GetOptionView(IFSOptionView::TypeVideoAudioSpeed)->setVisible(false);
            m_pStachedWidget->currentWidget()->setVisible(true);
            break;
        case TimelineType_Annotaions_Caption:
            {
                FSTextEditHelper *pHelper = static_cast<FSTextEditHelper *>(m_pEditHelper);
                SetVisibleView(pHelper->IsLineShape()?0x04:0x05);
                
                if ( m_pStachedWidget->currentIndex()!=2)
                {
                    m_pStachedWidget->setCurrentIndex(2);                   
                }
                m_pButtonAnnotation->setChecked(true);
                             
                objData=pHelper->GetCaptionBaseObj();
                GetOptionView(IFSOptionView::TypeVisualBase)->setVisible(!pHelper->IsLineShape());
                GetOptionView(IFSOptionView::TypeVideoAudioSpeed)->setVisible(false);
                GetOptionView(IFSOptionView::TypeAnotation)->setVisible(objData.ObjType != NLECaptionObjType::NLE_DOT_TEXT);
                GetOptionView(IFSOptionView::TypeTextBase)->setVisible(objData.ObjType == NLECaptionObjType::NLE_DOT_TIPTEXT||objData.ObjType==NLECaptionObjType::NLE_DOT_TEXT);
                GetOptionView(IFSOptionView::TypeTextBorder)->setVisible(objData.ObjType == NLECaptionObjType::NLE_DOT_TIPTEXT || objData.ObjType == NLECaptionObjType::NLE_DOT_TEXT);
                GetOptionView(IFSOptionView::TypeTextShadow)->setVisible(objData.ObjType == NLECaptionObjType::NLE_DOT_TIPTEXT || objData.ObjType == NLECaptionObjType::NLE_DOT_TEXT);
                GetOptionView(IFSOptionView::TypeCaptionSvgOption)->setVisible(false);
                m_pStachedWidget->currentWidget()->setVisible(true);
            }
            break;
        case TimelineType_Annotaions_Element:
        {
            FSTextEditHelper *pHelper = static_cast<FSTextEditHelper *>(m_pEditHelper);
            SetVisibleView(0x05);
            if (m_pStachedWidget->currentIndex() != 2)
            {
                m_pStachedWidget->setCurrentIndex(2);
            }
            m_pButtonAnnotation->setChecked(true);
            GetOptionView(IFSOptionView::TypeAnotation)->setVisible(false);
            GetOptionView(IFSOptionView::TypeTextBase)->setVisible(false);
            GetOptionView(IFSOptionView::TypeTextBorder)->setVisible(false);
            GetOptionView(IFSOptionView::TypeTextShadow)->setVisible(false);
            GetOptionView(IFSOptionView::TypeVisualBase)->setVisible(true);
            GetOptionView(IFSOptionView::TypeVideoAudioSpeed)->setVisible(false);
            GetOptionView(IFSOptionView::TypeCaptionSvgOption)->setVisible(true);
            m_pStachedWidget->currentWidget()->setVisible(true);

        }
            break;
        default:
            break;
        }
        UpdataParam();
    }
    else if (m_pEditHelper&&pHelper == m_pEditHelper)
    {
        UpdataParam();
    }
}

void FSPropertyView::OnTransformInfoChangedSlot(IFSEditHelper *pHelper)
{
    Q_ASSERT(pHelper);
    if (!m_pEditHelper)
    {
        m_pEditHelper = pHelper;
    }
    
    if (pHelper&&pHelper == m_pEditHelper)
    {       
        UpdataParam();
    }
}

void FSPropertyView::OnEndChangedSlot(UndoCommand *pCmd)
{
    if (pCmd)
    {
        m_pPlayerEditorManager->PushCommand(pCmd);
    }
}

void FSPropertyView::OnMouseEffictAdded(int mouseEffectType)
{
    m_MouseEffictType = (MouseEffictType)(mouseEffectType);
    if (m_pEditHelper)
        SetEditHelper(m_pEditHelper);
}

void FSPropertyView::OnCmdUpdataSlot()
{
     UpdataParam();
}

void FSPropertyView::OnSpeedChanged()
{
    FSSpeedOption *pSender = static_cast<FSSpeedOption *>(sender());
    Q_ASSERT(pSender);
    if (m_pEditHelper)
        emit sigSpeedChanged(m_pEditHelper->GetTimeLine(), pSender->GetSpeedScale());
}

void FSPropertyView::OnTextChanged(QString)
{
    if (!m_pEditHelper)
        return;
    FSTextBaseOption * pView = static_cast <FSTextBaseOption *>(GetOptionView(IFSOptionView::TypeTextBase));
    if (pView&&pView->isVisible());
    {
        pView->SetText(static_cast<FSTextEditHelper *>(m_pEditHelper)->GetText());
    }
}

void FSPropertyView::OnButtonTogged(int id, bool bChecked)
{
    if (!bChecked&&id == 4)
    {
        if (m_pEditHelper&&m_pEditHelper->GetTimeLine()->IsDesktopVideo())
        {
            FSVideoPicEditHelper *pHelper = static_cast<FSVideoPicEditHelper *>(m_pEditHelper);
            ushort shortArray[3] = { 0xE842, 0xE868, 0xE82B };
            QString temp = QString::fromUtf16(shortArray, 3);
            if (pHelper->GetHotKeyText()!=temp)
                pHelper->SetHotKeyText("");
            m_pEditHelper->Updata();
        }
    }
}

void FSPropertyView::AddOptionWidget(const QString &Tiltle, const char *sztitle, bool bCanDelete, IFSOptionView::OptionType optionType)
{
    IFSOptionView *pOptionView = NULL;

    switch (optionType)
    {
    case IFSOptionView::TypeVisualBase:
        pOptionView = new FSVisualBaseOption(this);
        m_pScrollAreaVisual->AddWidget(pOptionView,"FSVisualBaseOption");
        break;
    case IFSOptionView::TypeAudioBase:
        pOptionView = new FSAudioBaseOption(this);
        m_pScrollAreaAudio->AddWidget(pOptionView,"FSAudioBaseOption");
        break;
    case IFSOptionView::TypeAudioDenoise:
        pOptionView = new FSDenoiseOption(this);
        m_pScrollAreaAudio->AddWidget(pOptionView, "FSDenoiseOption");
        break;
    case IFSOptionView::TypeVideoAudioSpeed:
    {
        pOptionView = new FSSpeedOption(this);
        m_pScrollAreaVisual->AddWidget(pOptionView, "FSSpeedOption");
        connect(static_cast<FSSpeedOption *>(pOptionView), &FSSpeedOption::speedChanged, this, &FSPropertyView::OnSpeedChanged);
    }        
        break;
    case IFSOptionView::TypeAudioSpeed:
        pOptionView = new FSAudioSpeedOption(this);
        m_pScrollAreaAudio->AddWidget(pOptionView, "FSAudioSpeedOption");
        connect(static_cast<FSAudioSpeedOption *>(pOptionView), &FSAudioSpeedOption::speedChanged, this, &FSPropertyView::OnSpeedChanged);
        break;
    case IFSOptionView::TypeTextBase:
        pOptionView = new FSTextBaseOption(this);
        m_pScrollAreaAnnotation->AddWidget(pOptionView,"FSTextBaseOption");
        connect(static_cast<FSTextBaseOption *>(pOptionView), &FSTextBaseOption::sigCharSelecetChanged, this, [&](int start,int len){
            if (m_pEditHelper&&m_pEditHelper->GetEditType() == IFSEditHelper::Edit_Text)
            {
                FSTextEditHelper *pHelper = static_cast<FSTextEditHelper *>(m_pEditHelper);
                pHelper->SetCharSelect(start, len);
                pHelper->Updata();
            }
        });
        connect(static_cast<FSTextBaseOption *>(pOptionView), &FSTextBaseOption::sigKeyPress, this, [&](QKeyEvent *e){
            if (m_pEditHelper&&m_pEditHelper->GetEditType() == IFSEditHelper::Edit_Text)
            {
                FSTextEditHelper *pHelper = static_cast<FSTextEditHelper *>(m_pEditHelper);
                disconnect(m_pEditorHelperManager, &IFSEditHelperManager::TextChanged, this, &FSPropertyView::OnTextChanged);  
                if (   !(((e->modifiers()&Qt::ControlModifier) && (e->key() == Qt::Key_Z))
                    || ((e->modifiers()&Qt::ControlModifier) && (e->key() == Qt::Key_Y))
                    || ((e->modifiers()&Qt::ControlModifier) && (e->key() == Qt::Key_Left))
                    || ((e->modifiers()&Qt::ControlModifier) && (e->key() == Qt::Key_Right))
                    || ((e->modifiers()&Qt::ControlModifier) && (e->key() == Qt::Key_Up))
                    || ((e->modifiers()&Qt::ControlModifier) && (e->key() == Qt::Key_Down)))
                )
                    pHelper->DoKeyPressEvent(e);              
                connect(m_pEditorHelperManager, &IFSEditHelperManager::TextChanged, this, &FSPropertyView::OnTextChanged, Qt::UniqueConnection);
                if ((e->modifiers()&Qt::ControlModifier) && (e->key() == Qt::Key_Z))
                {
                    m_pPlayerEditorManager->Undo();
                }
                else if ((e->modifiers()&Qt::ControlModifier) && (e->key() == Qt::Key_Y))
                {
                    m_pPlayerEditorManager->Redo();
                }
                pHelper->Updata();
            }
        });
        connect(static_cast<FSTextBaseOption *>(pOptionView), &FSTextBaseOption::sigInputMethod, this, [&](QInputMethodEvent *e){
            if (m_pEditHelper&&m_pEditHelper->GetEditType() == IFSEditHelper::Edit_Text)
            {
                FSTextEditHelper *pHelper = static_cast<FSTextEditHelper *>(m_pEditHelper);
                disconnect(m_pEditorHelperManager, &IFSEditHelperManager::TextChanged, this, &FSPropertyView::OnTextChanged);
                pHelper->DoinputMethodEvent(e);               
                connect(m_pEditorHelperManager, &IFSEditHelperManager::TextChanged, this, &FSPropertyView::OnTextChanged, Qt::UniqueConnection);
                pHelper->Updata();
            }
        });
        break;
    case IFSOptionView::TypeAnotation:
    {
        pOptionView = new FSAnotationOption(this);
        m_pScrollAreaAnnotation->AddWidget(pOptionView, "FSAnotationOption");

        connect(static_cast<FSAnotationOption *>(pOptionView), &FSAnotationOption::startValueChanged, this, [&](double){
            if (m_pEditHelper&&m_pEditHelper->GetEditType() == IFSEditHelper::Edit_Text)
            {
                FSTextEditHelper *pHelper = static_cast<FSTextEditHelper *>(m_pEditHelper);
                pHelper->GetCaptionBaseObj().pObj->SetUnDoReDoStep();
            }
        });
    }
        break;
    case IFSOptionView::TypeCaptionSvgOption:
    {
        pOptionView = new FSSVGCaptionOption(this);
        m_pScrollAreaAnnotation->AddWidget(pOptionView, "FSSVGCaptionOption");
    }
        break;
    case IFSOptionView::TypeCursorBase:
        pOptionView = new FSCursorBaseOption(this);
        m_pScrollAreaCursor->AddWidget(pOptionView, "FSCursorBaseOption");
        break;
    case IFSOptionView::TypeCursorHighlight:
        pOptionView = new FSCursorHighlightOption(this);
        m_pScrollAreaCursor->AddWidget(pOptionView, Default_CurssorHighlight);
        break;
    case IFSOptionView::TypeCursorClickSound:
        pOptionView = new FSCursorClickSoundOption(this);
        m_pScrollAreaCursor->AddWidget(pOptionView, Default_CurssorVolume);
        break;
    case IFSOptionView::TypeCursorClickRing:
        pOptionView = new FSCursorClickRingOption(this);
        m_pScrollAreaCursor->AddWidget(pOptionView, Default_CurssorRingColor);
        break;
    case IFSOptionView::TypeHotKeyTip:
        pOptionView = new FSHotKetTipOption(this);
        m_pScrollAreaHotKey->AddWidget(pOptionView, "FSHotKetTipOption");
        break;

    case IFSOptionView::TypeTextBorder:
        pOptionView = new FSTextBorderOption(this);
        m_pScrollAreaAnnotation->AddWidget(pOptionView, "FSTextBorderOption");
        break;
    case IFSOptionView::TypeTextShadow:
        pOptionView = new FSTextShadowOption(this);
        m_pScrollAreaAnnotation->AddWidget(pOptionView, "FSTextShadowOption");
        break;
    default:
        break;
    }

    if (pOptionView)
    {
        pOptionView->SetTitle(Tiltle);
        pOptionView->setProperty("QByteArray", QByteArray(sztitle));
        pOptionView->SetCandelete(bCanDelete);
        connect(pOptionView, &IFSOptionView::ParmaChanged, this, &FSPropertyView::OnParmaChangedSlot,Qt::QueuedConnection);
        connect(pOptionView, &IFSOptionView::DeletePropoty, this, &FSPropertyView::OnDeletePropotySlot);
    }
}

IFSOptionView * FSPropertyView::GetOptionView(IFSOptionView::OptionType optionType)
{
    IFSOptionView *pView = NULL;
    switch (optionType)
    {
    case IFSOptionView::TypeVisualBase:
    case IFSOptionView::TypeVideoAudioSpeed:
        for (auto view : m_pScrollAreaVisual->GetWidgets())
        {
            if (static_cast<IFSOptionView *>(view)->GetOptionType() == optionType)
            {
                pView = static_cast<IFSOptionView *>(view);
            }
        }
        break;
    case IFSOptionView::TypeAudioBase:
    case IFSOptionView::TypeAudioDenoise:
    case IFSOptionView::TypeAudioSpeed:

        for (auto view : m_pScrollAreaAudio->GetWidgets())
        {
            if (static_cast<IFSOptionView *>(view)->GetOptionType() == optionType)
            {
                pView = static_cast<IFSOptionView *>(view);
            }
        }
        break;
    case IFSOptionView::TypeTextBase:
    case IFSOptionView::TypeAnotation:
    case IFSOptionView::TypeTextBorder:
    case IFSOptionView::TypeTextShadow:
    case IFSOptionView::TypeCaptionSvgOption:
    {
        for (auto view : m_pScrollAreaAnnotation->GetWidgets())
        {
            if (static_cast<IFSOptionView *>(view)->GetOptionType() == optionType)
            {
                pView = static_cast<IFSOptionView *>(view);
            }
        }
    }
        break;
    case IFSOptionView::TypeCursorBase:
    case IFSOptionView::TypeCursorHighlight:
    case IFSOptionView::TypeCursorClickSound:
    case IFSOptionView::TypeCursorClickRing:
    {
        for (auto view : m_pScrollAreaCursor->GetWidgets())
        {
            if (static_cast<IFSOptionView *>(view)->GetOptionType() == optionType)
            {
                pView = static_cast<IFSOptionView *>(view);
            }
        }
    }
        break;

    case IFSOptionView::TypeHotKeyTip:
    {
        for (auto view : m_pScrollAreaHotKey->GetWidgets())
        {
            if (static_cast<IFSOptionView *>(view)->GetOptionType() == optionType)
            {
                pView = static_cast<IFSOptionView *>(view);
            }
        }
    }
    default:
        break;
    }
    return pView;
}

void FSPropertyView::UpdataParam()
{
    if (!m_pEditHelper)
        return;
    if (m_pButtonVisual->isVisible()){
        for (auto OptionView : m_pScrollAreaVisual->GetWidgets()){
            IFSOptionView *pView = static_cast<IFSOptionView *>(OptionView);
            if (pView->isVisible()){
                pView->UpdataParma(m_pEditHelper);
            }
        }
    }

    if (m_pButtonAudio->isVisible()){
        for (auto OptionView : m_pScrollAreaAudio->GetWidgets()){
            IFSOptionView *pView = static_cast<IFSOptionView *>(OptionView);
            if (pView->isVisible()){
                pView->UpdataParma(m_pEditHelper);
            }
        }
    }

    if (m_pButtonAnnotation->isVisible()){
        for (auto OptionView : m_pScrollAreaAnnotation->GetWidgets()){
            IFSOptionView *pView = static_cast<IFSOptionView *>(OptionView);
            if (pView->isVisible()){
                pView->UpdataParma(m_pEditHelper);
            }
        }
    }
    if (m_pButtonCursor->isChecked())
    {
        QString strPath;
        INLETimelineBase *pBaseTimeline = NULL;
        GetOptionView(IFSOptionView::TypeCursorClickRing)->setVisible(static_cast<FSVideoPicEditHelper *>(m_pEditHelper)->IsContainMouseRings(&pBaseTimeline));
        GetOptionView(IFSOptionView::TypeCursorClickSound)->setVisible(static_cast<FSVideoPicEditHelper *>(m_pEditHelper)->IsContainMouseSound(strPath));
        GetOptionView(IFSOptionView::TypeCursorHighlight)->setVisible(static_cast<FSVideoPicEditHelper *>(m_pEditHelper)->IsContainMouseHighlight(&pBaseTimeline));
    }
    if (m_pButtonCursor->isVisible()){
        for (auto OptionView : m_pScrollAreaCursor->GetWidgets()){
            IFSOptionView *pView = static_cast<IFSOptionView *>(OptionView);
            if (pView->isVisible()){
                pView->UpdataParma(m_pEditHelper);
            }
        }
    }
    if (m_pButtonHotKey->isVisible())
    {
        for (auto OptionView : m_pScrollAreaHotKey->GetWidgets()){
            IFSOptionView *pView = static_cast<IFSOptionView *>(OptionView);
            if (pView->isVisible()){
                pView->UpdataParma(m_pEditHelper);
            }
        }
    }
}

void FSPropertyView::TransfromGaInfo(int iType)
{
	switch (iType)
	{
	case IFSOptionView::TypeCursorHighlight:
		GAThread::getInstance()->insertEventItem(Clip_Edit, "DeleteCursorHighLight", "");
		break;
	case IFSOptionView::TypeCursorClickSound:
		GAThread::getInstance()->insertEventItem(Clip_Edit, "DeleteLeftClickSound", "");
		break;
	case IFSOptionView::TypeCursorClickRing:
		GAThread::getInstance()->insertEventItem(Clip_Edit, "DeleteLeftClickRing", "");
		break;
	default:
		break;
	}
}

void FSPropertyView::OnButtonClickedSlot(int id)
{    
    m_pStachedWidget->setCurrentIndex(id);   
    UpdataParam();
}


void FSPropertyView::OnParmaChangedSlot()
{  
    IFSOptionView* pView=static_cast<IFSOptionView*>(sender());
    Q_ASSERT(pView);
    if (m_pEditHelper)
    {
        IFSOptionView::OptionType type = pView->GetOptionType();
        switch (type)
        {
        case IFSOptionView::TypeVisualBase:
        {
            double positionX, positionY, ScaleX, ScaleY, rolation;
            FSVisualBaseOption *pVisualBase = static_cast<FSVisualBaseOption *>(pView);
            pVisualBase->GetPosition(positionX, positionY);
            pVisualBase->GetRotation(rolation);
            pVisualBase->GetScale(ScaleX, ScaleY);
            if (m_pEditHelper->GetEditType() == IFSEditHelper::Edit_Text)
            {            
                if (pVisualBase->isScaleChanged())
                {
                    m_pEditHelper->SetScale(ScaleX, ScaleY);
					//上传数据
					GAThread::getInstance()->insertEventItem(Clip_Edit,"VisualScale","");
                }                                   
                if (pVisualBase->isPosChanged())
                {
                    m_pEditHelper->SetPosition(positionX, positionY);
					//上传数据
					GAThread::getInstance()->insertEventItem(Clip_Edit, "VisualPosition", "");
                }
                if (pVisualBase->isAngleChanged())
                {
                    m_pEditHelper->SetAngle(rolation);
                    //上传数据
                    GAThread::getInstance()->insertEventItem(Clip_Edit, "VisualRotate", "");
                }
                pVisualBase->ResetChangeFlag();
                m_pEditHelper->Updata();
            }
            else
            {
                m_pPlayerEditorManager->PushCommand(new FSVisaulUndoCommand(m_pEditHelper, { ScaleX, ScaleY }, { positionX, positionY }, rolation, false));
            }
        }
            break;
        case IFSOptionView::TypeAudioBase:
        {
            double voluem;
            bool isChecked;
            FSAudioBaseOption *pAudioBase = static_cast<FSAudioBaseOption *>(pView);
            pAudioBase->GetVoluem(voluem);
            isChecked=pAudioBase->isMix2MonoChecked();
            m_pPlayerEditorManager->PushCommand(new FSAudioUnDoCommand(m_pEditHelper, voluem, isChecked, false));
			//上传GA数据
			GAThread::getInstance()->insertEventItem(Clip_Edit, "AudioVolume", "");
			GAThread::getInstance()->insertEventItem(Clip_Edit, "AudioMixtoMono", isChecked?"YES":"NO");
        }          
            break;
        case IFSOptionView::TypeAudioDenoise:
        {
            int denoiseLevel=0;
            FSDenoiseOption *pDenoiseVeiw = static_cast<FSDenoiseOption *>(pView);
            if (pDenoiseVeiw->IsDenoiseEnable())
            {
                denoiseLevel=pDenoiseVeiw->GetDenoiseLevel();
            }
            else
            {
                m_pEditHelper->SetLastDenoiseLevel(pDenoiseVeiw->GetLastDenoiseLevel());
            }
			auto cmd = new FSDenoiseCommand(m_pEditHelper, denoiseLevel);
			connect(cmd, &FSDenoiseCommand::signalUpdate, this, [&]{emit sigDenoise(m_pEditHelper->GetTimeLine()); });
			m_pPlayerEditorManager->PushCommand(cmd);
        }
            break;
        case IFSOptionView::TypeTextBase:
        {
            FSTextBaseOption *pTextBase = static_cast<FSTextBaseOption *>(pView);
            QString font=pTextBase->GetFont();
            int fontSize = pTextBase->GetFontSize();
            QColor textColor = pTextBase->GetTextColor();
            NLEAlignType type = pTextBase->GetTextAlign();
            bool bBold = pTextBase->IsFontBold();
            bool bItalic = pTextBase->IsFontItalic();
            FSTextEditHelper *pHelper = static_cast<FSTextEditHelper *>(m_pEditHelper);
            m_pPlayerEditorManager->BeginComposite();
			FSNLETimeline* timeline = pHelper->GetTimeLine();
            if (pTextBase->isFontNameChanged())
            {  
                pHelper->GetCaptionBaseObj().pObj->SetUnDoReDoStep();
                pHelper->SetFont(font);
				QString strName = timeline->GetName();
				strName.append("Font");
				GAThread::getInstance()->insertEventItem(Annotation_Edit, strName,"");
            }               
            if (pTextBase->isFontSizeChanged())
            {
                pHelper->GetCaptionBaseObj().pObj->SetUnDoReDoStep();
				QString strName = timeline->GetName();
				strName.append("Size");
				GAThread::getInstance()->insertEventItem(Annotation_Edit, strName, "");
                pHelper->SetFontSize(fontSize);
                
            }              
            if (pTextBase->isFontColorChanged())
            {
                pHelper->SetTextColor(textColor);
                pHelper->SetFontBold(bBold);
                pHelper->SetFontItalic(bItalic);
				QString strName = timeline->GetName();
				strName.append("Color");
				GAThread::getInstance()->insertEventItem(Annotation_Edit, strName, "");
            }               
            if (pTextBase->isTextAlignChanged())
            {
                pHelper->SetTextAlign(type);
				QString strName = timeline->GetName();
				strName.append("Align");
				GAThread::getInstance()->insertEventItem(Annotation_Edit, strName, "");
            }                
            pTextBase->ResetChangeFlag();
            m_pPlayerEditorManager->EndComposite();
            pHelper->Updata();
        }
            break;
        case IFSOptionView::TypeAnotation:
        {
            FSAnotationOption *pAnotation = static_cast<FSAnotationOption *>(pView);
            FSTextEditHelper *pHelper = static_cast<FSTextEditHelper *>(m_pEditHelper);
            int w = pAnotation->GetBorderWidth();
            QColor shapeColor = pAnotation->GetShapeColor();
            QColor BorderColor = pAnotation->GetBoderShapeColor();
            m_pPlayerEditorManager->BeginComposite();
            FSNLETimeline* timeline = pHelper->GetTimeLine();
            if (pAnotation->IsShapeColorChanged())
            {
                QString strName = timeline->GetName();
                strName.append("Color");
                GAThread::getInstance()->insertEventItem(Annotation_Edit, strName, "");
                pHelper->SetBackGroundColor(shapeColor);
            }
            if (pAnotation->IsBorderColorChanged())
            {
                QString strName = timeline->GetName();
                strName.append("Color");
                GAThread::getInstance()->insertEventItem(Annotation_Edit, strName, "");
                pHelper->SetBorderBackGroundColor(BorderColor);
            }
            if (pAnotation->IsBorderWidthChanged())
            {
                QString strName = timeline->GetName();
                strName.append("BorderWidth");
                GAThread::getInstance()->insertEventItem(Annotation_Edit, strName, "");
                pHelper->SetBorderWidth(w);
            }
            m_pPlayerEditorManager->EndComposite();
            pAnotation->ResetChangeFlag();
            pHelper->Updata();
        }
            break;

        case IFSOptionView::TypeTextBorder:
        {
            FSTextBorderOption *pTextBorderOption = static_cast<FSTextBorderOption *>(pView);
            FSTextEditHelper *pHelper = static_cast<FSTextEditHelper *>(m_pEditHelper);
            QColor color = pTextBorderOption->GetTextBorderColor();
            int Opacity = pTextBorderOption->GetTextBorderOpacity();
            int size = pTextBorderOption->GetTextBorderSize();
            int Blur = pTextBorderOption->GetTextBorderBlur();
            if (pTextBorderOption->IsColorChanged())
            {
                pHelper->SetTextBorderColor(color);
            }
            if (pTextBorderOption->IsOpacityChanged())
            {
                pHelper->SetTextBorderOpacity(Opacity);
            }
            if (pTextBorderOption->IsBlurChanged())
            {
                pHelper->SetTextBorderBlur(Blur);
            }
            if (pTextBorderOption->IsSizeChanged())
            {
                pHelper->SetTextBorderSize(size);
            }
            pTextBorderOption->ResetChangeFlag();
            pHelper->Updata();
        }
            break;

        case IFSOptionView::TypeTextShadow:
        {
            FSTextShadowOption *pTextShadowOption = static_cast<FSTextShadowOption *>(pView);
            FSTextEditHelper *pHelper = static_cast<FSTextEditHelper *>(m_pEditHelper);
            QColor color = pTextShadowOption->GetTextShadowColor();
            int Opacity = pTextShadowOption->GetTextShadowOpacity();
            int distance = pTextShadowOption->GetTextShadowDistance();
            int Blur = pTextShadowOption->GetTextShadowBlur();
            int type = pTextShadowOption->GetTextShadowDerection();
            if (pTextShadowOption->IsTypeChanged())
            {
                pHelper->SetTextShadowDerection(type);
            }
            if (pTextShadowOption->IsColorChanged())
            {
                pHelper->SetTextShadowColor(color);
            }
            if (pTextShadowOption->IsOpacityChanged())
            {
                pHelper->SetTextShadowOpacity(Opacity);
            }
            if (pTextShadowOption->IsBlurChanged())
            {
                pHelper->SetTextShadowBlur(Blur);
            }
            if (pTextShadowOption->IsDistanceChanged())
            {
                pHelper->SetTextShadowDistance(distance);
            }

            pTextShadowOption->ResetChangeFlag();
            pHelper->Updata();
        }
            break;

        case IFSOptionView::TypeCaptionSvgOption:
        {
            FSSVGCaptionOption *pSVGCaptionOption = static_cast<FSSVGCaptionOption *>(pView);
            FSTextEditHelper *pHelper = static_cast<FSTextEditHelper *>(m_pEditHelper);
            auto colorMap=pSVGCaptionOption->GetColors();
            QList<int> keys = colorMap.keys();
            pHelper->GetCaptionBaseObj().pObj->SetUnDoReDoStep();
            for (int i = 0; i < keys.size();i++)
            {
                if (colorMap.value(keys.at(i)).first)
                {
                    pHelper->SetCaptionSvgColor(keys.at(i), colorMap.value(keys.at(i)).second);
                    colorMap[keys.at(i)].first = false;
                }
            }
            pHelper->Updata();
        }
            break;
        case IFSOptionView::TypeCursorBase:
        {
            FSCursorBaseOption *pCursorBase = static_cast<FSCursorBaseOption *>(pView);
            FSVideoPicEditHelper *pHelper = static_cast<FSVideoPicEditHelper *>(m_pEditHelper);
            m_pPlayerEditorManager->PushCommand(new FSCursorVisualUnDoCommand(m_pEditHelper, pCursorBase->GetMouseScale(), pCursorBase->GetMouseOpacity()));
			//上传GA数据
			GAThread::getInstance()->insertEventItem(Clip_Edit, "CursorScale", "");
			GAThread::getInstance()->insertEventItem(Clip_Edit, "CursorOpacity", "");
        }           
            break;
        case IFSOptionView::TypeCursorHighlight:
        {
            FSCursorHighlightOption *pCursorHighlightOption = static_cast<FSCursorHighlightOption *>(pView);
            FSVideoPicEditHelper *pHelper = static_cast<FSVideoPicEditHelper *>(m_pEditHelper);
            m_pPlayerEditorManager->PushCommand(new FSMouseHighlightUnDoCommand(m_pEditHelper, pCursorHighlightOption->GetColor()));
            pHelper->Updata();
			//上传GA数据
			GAThread::getInstance()->insertEventItem(Clip_Edit, "CursorHightColor", "");
        }            
            break;
        case IFSOptionView::TypeCursorClickSound:
        {
            FSCursorClickSoundOption *pCursorClickSound = static_cast<FSCursorClickSoundOption *>(pView);
            FSVideoPicEditHelper *pHelper = static_cast<FSVideoPicEditHelper *>(m_pEditHelper);
            double value;
            pCursorClickSound->GetVoluem(value);
            m_pPlayerEditorManager->PushCommand(new FSMouseClickSoundUnDoCommand(m_pEditHelper, value));
            pHelper->Updata();
			//上传GA数据
			GAThread::getInstance()->insertEventItem(Clip_Edit, "LeftClickRingColor", "");
        }            
            break;
        case IFSOptionView::TypeCursorClickRing:
        {
            FSCursorClickRingOption *pCursorClickRing = static_cast<FSCursorClickRingOption *>(pView);
            FSVideoPicEditHelper *pHelper = static_cast<FSVideoPicEditHelper *>(m_pEditHelper);
            m_pPlayerEditorManager->PushCommand(new FSMouseRingsUnDoCommand(m_pEditHelper, pCursorClickRing->GetColor()));
            pHelper->Updata();
			//上传GA数据
			GAThread::getInstance()->insertEventItem(Clip_Edit, "LeftClickRingVolume", "");
        }         
            break;
        case IFSOptionView::TypeHotKeyTip:
        {
            FSHotKetTipOption *pHotKetTipOption = static_cast<FSHotKetTipOption *>(pView);
            FSVideoPicEditHelper *pHelper = static_cast<FSVideoPicEditHelper *>(m_pEditHelper);
            int opacity=pHotKetTipOption->GetHotKeyOpacity();
            QColor color=pHotKetTipOption->GetHotKeyColor();
            QString Pos=pHotKetTipOption->GetHotKeyPosition();
            QString size=pHotKetTipOption->GetHotKeySize();
            QString style=pHotKetTipOption->GetHotKeyStyle();
            bool bEnable=pHotKetTipOption->HotKeyEnable();
            int bFilterType = pHotKetTipOption->GetFliterType();
            m_pPlayerEditorManager->PushCommand(new FSHotKeyCommand(m_pEditHelper, bEnable, bFilterType, size, Pos, style, color, opacity));
        }
            break;
        default:
            break;
        }
    }

	
}

void FSPropertyView::OnDeletePropotySlot()
{
    IFSOptionView* pView = static_cast<IFSOptionView*>(sender());
    Q_ASSERT(pView);
    if (m_pEditHelper)
    {
        IFSOptionView::OptionType type = pView->GetOptionType();
        switch (type)
        {
        case IFSOptionView::TypeCursorHighlight:
            m_pPlayerEditorManager->PushCommand(new FSMouseHighLightDeleteCommand(m_pEditHelper));
            m_pEditHelper->Updata();
            break;
        case IFSOptionView::TypeCursorClickSound:
            m_pPlayerEditorManager->PushCommand(new FSMouseClickSoundDeleteConmmand(m_pEditHelper));
            m_pEditHelper->Updata();
            break;
        case IFSOptionView::TypeCursorClickRing:
            m_pPlayerEditorManager->PushCommand(new FSMouseRingsDeleteCommand(m_pEditHelper));
            m_pEditHelper->Updata();
            break;
        default:
            break;
        }
		//上传GA数据
		TransfromGaInfo(type);
    }
}

void FSPropertyView::CreateUI()
{

    m_pWidgetMain = new FSLabel(this);
    m_pStyleTitle = new FSLabel(m_pWidgetMain);
    m_pWidgetTitle = new QWidget(m_pWidgetMain);
    m_pButtonGroup = new QButtonGroup(this);
    m_pButtonVisual = new FSButton(m_pWidgetTitle);
    m_pButtonCursor = new FSButton(m_pWidgetTitle);
    m_pButtonAudio = new FSButton(m_pWidgetTitle);
    m_pButtonHotKey = new FSButton(m_pWidgetTitle);
    m_pButtonAnnotation = new FSButton(m_pWidgetTitle);

    m_pButtonShowProperty = new FSStateBaseButton(0, "", this);
    FSBaseButton *pShowPropertyBtn = new FSBaseButton(0, "FSPropertyView_m_pShowPropertyBtn", this);
    FSBaseButton *pHidePropertyBtn = new FSBaseButton(1, "FSPropertyView_m_pHidePropertyBtn", this);
    m_pButtonShowProperty->AddButton(pShowPropertyBtn);
    m_pButtonShowProperty->AddButton(pHidePropertyBtn);
    m_pButtonShowProperty->setFixedSize(35, 35);

    connect(m_pButtonShowProperty, &FSStateBaseButton::Clicked, this, [&](int id, QPoint pt){
        SetShowHideButtonState(id == 1);
    });

    QShortcut *shortcutCtrlRight = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Right), this);
    shortcutCtrlRight->setContext(Qt::ApplicationShortcut);
    connect(shortcutCtrlRight, &QShortcut::activated, this, [&](){
        bool bid = m_pButtonShowProperty->CurrentButtonId();
        SetShowHideButtonState(bid);
        m_pButtonShowProperty->SetCurrentButtonId(bid ? 0 : 1);
    });


    m_pButtonGroup->addButton(m_pButtonVisual, 0);
    m_pButtonGroup->addButton(m_pButtonAudio, 1);
    m_pButtonGroup->addButton(m_pButtonAnnotation, 2);
    m_pButtonGroup->addButton(m_pButtonCursor, 3);
    m_pButtonGroup->addButton(m_pButtonHotKey, 4);


    m_pButtonVisual->setCheckable(true);
    m_pButtonCursor->setCheckable(true);
    m_pButtonAudio->setCheckable(true);
    m_pButtonAnnotation->setCheckable(true);
    m_pButtonHotKey->setCheckable(true);

    m_pButtonVisual->setFocusPolicy(Qt::NoFocus);
    m_pButtonCursor->setFocusPolicy(Qt::NoFocus);
    m_pButtonAudio->setFocusPolicy(Qt::NoFocus);
    m_pButtonAnnotation->setFocusPolicy(Qt::NoFocus);
    m_pButtonHotKey->setFocusPolicy(Qt::NoFocus);

    m_pStachedWidget = new QStackedWidget(m_pWidgetMain);


    m_pScrollAreaVisual = new FSScrollArea(m_pStachedWidget);
    m_pScrollAreaAudio = new FSScrollArea(m_pStachedWidget);
    m_pScrollAreaAnnotation = new FSScrollArea(m_pStachedWidget);
    m_pScrollAreaCursor = new FSScrollArea(m_pStachedWidget);
    m_pScrollAreaHotKey = new FSScrollArea(m_pStachedWidget);


    QHBoxLayout *pHL = new QHBoxLayout(m_pWidgetTitle);
    pHL->addWidget(m_pButtonVisual);
    pHL->addWidget(m_pButtonAudio);
    pHL->addWidget(m_pButtonAnnotation);
    pHL->addWidget(m_pButtonCursor);
    pHL->addWidget(m_pButtonHotKey);
    pHL->addStretch();
    //pHL->addWidget(pButtonShowProperty);
    pHL->setMargin(0);
    pHL->setSpacing(0);

    QVBoxLayout *pVL = new QVBoxLayout(m_pWidgetMain);
    pVL->addWidget(m_pWidgetTitle);
    pVL->addWidget(m_pStachedWidget);
    pVL->setMargin(0);
    pVL->setSpacing(0);


    QVBoxLayout *pVLMain = new QVBoxLayout(this);
    pVLMain->addWidget(m_pWidgetMain);
    pVLMain->setMargin(0);
    pVLMain->setSpacing(0);

    m_pWidgetMain->setAlignment(Qt::AlignCenter);
    m_pWidgetMain->setWordWrap(true);

    m_pStachedWidget->addWidget(m_pScrollAreaVisual);
    m_pStachedWidget->addWidget(m_pScrollAreaAudio);
    m_pStachedWidget->addWidget(m_pScrollAreaAnnotation);
    m_pStachedWidget->addWidget(m_pScrollAreaCursor);
    m_pStachedWidget->addWidget(m_pScrollAreaHotKey);

    m_pButtonVisual->setFixedSize(35, 35);
    m_pButtonCursor->setFixedSize(35, 35);
    m_pButtonAudio->setFixedSize(35, 35);
    m_pButtonAnnotation->setFixedSize(35, 35);
    m_pButtonHotKey->setFixedSize(35, 35);
    m_pStyleTitle->setObjectName("FSPropertyView_m_pStyleTitle");
    m_pButtonVisual->setObjectName("FSPropertyView_m_pButtonVisual");
    m_pButtonCursor->setObjectName("FSPropertyView_m_pButtonCursor");
    m_pButtonAudio->setObjectName("FSPropertyView_m_pButtonAudio");
    m_pButtonAnnotation->setObjectName("FSPropertyView_m_pButtonAnnotation");
    m_pButtonHotKey->setObjectName("FSPropertyView_m_pButtonHotKey");
    m_pWidgetTitle->setObjectName("FSPropertyView_m_pWidgetTitle");
    m_pWidgetMain->setObjectName("FSPropertyView_m_pTextLabel");
    setObjectName("FSPropertyView");

    AddOptionWidget(tr("Transform"), "Transform", false, IFSOptionView::OptionType::TypeVisualBase);
    AddOptionWidget(tr("Speed"), "Speed", false, IFSOptionView::TypeVideoAudioSpeed);
    AddOptionWidget(tr("Audio"), "Audio", false, IFSOptionView::OptionType::TypeAudioBase);
    AddOptionWidget(tr("Denoise"), "Denoise", false, IFSOptionView::OptionType::TypeAudioDenoise);
    AddOptionWidget(tr("Speed"), "Speed", false, IFSOptionView::TypeAudioSpeed);
    AddOptionWidget(tr("Annotation"), "Annotation", false, IFSOptionView::OptionType::TypeAnotation);
    AddOptionWidget(tr("Text"), "Text", false, IFSOptionView::OptionType::TypeTextBase);
    AddOptionWidget(tr("Text Border"), "Text Border", false, IFSOptionView::OptionType::TypeTextBorder);
    AddOptionWidget(tr("Text Shadow"), "Text Shadow", false, IFSOptionView::OptionType::TypeTextShadow);
    AddOptionWidget(tr("Element"), "Element", false, IFSOptionView::OptionType::TypeCaptionSvgOption);
    AddOptionWidget(tr("Cursor"), "Cursor", false, IFSOptionView::OptionType::TypeCursorBase);
    AddOptionWidget(tr("HighLight"), "HighLight", true, IFSOptionView::OptionType::TypeCursorHighlight);
    AddOptionWidget(tr("Left Click Sound"), "Left Click Sound", true, IFSOptionView::OptionType::TypeCursorClickSound);
    AddOptionWidget(tr("Left Click Ring"), "Left Click Ring", true, IFSOptionView::OptionType::TypeCursorClickRing);
    AddOptionWidget(tr("HotKey Tips"), "HotKey Tips", false, IFSOptionView::OptionType::TypeHotKeyTip);
}

void FSPropertyView::RetranslateUi()
{
    
    m_pButtonVisual->setToolTip(tr("Transform"));
    m_pButtonCursor->setToolTip(tr("Cursor"));
    m_pButtonAudio->setToolTip(tr("Audio"));
    m_pButtonAnnotation->setToolTip(tr("Annotations"));
    m_pButtonHotKey->setToolTip(tr("HotKey Tips"));
    m_pWidgetMain->setText(tr("Select clip to show its properties"));
    m_pButtonShowProperty->GetButton(0)->setToolTip(tr("Hide Properties list"));
    m_pButtonShowProperty->GetButton(1)->setToolTip(tr("Show Properties list"));


    for (auto OptionView : m_pScrollAreaVisual->GetWidgets()){
        IFSOptionView *pView = static_cast<IFSOptionView *>(OptionView);
        auto bytearray = pView->property("QByteArray").toByteArray();
        pView->SetTitle(tr(bytearray.constData()));
    }

    for (auto OptionView : m_pScrollAreaAudio->GetWidgets()){
        IFSOptionView *pView = static_cast<IFSOptionView *>(OptionView);
        auto bytearray = pView->property("QByteArray").toByteArray();
        pView->SetTitle(tr(bytearray.constData()));
    }

    for (auto OptionView : m_pScrollAreaAnnotation->GetWidgets()){
        IFSOptionView *pView = static_cast<IFSOptionView *>(OptionView);
        auto bytearray = pView->property("QByteArray").toByteArray();
        pView->SetTitle(tr(bytearray.constData()));
    }

    for (auto OptionView : m_pScrollAreaCursor->GetWidgets()){
        IFSOptionView *pView = static_cast<IFSOptionView *>(OptionView);
        auto bytearray = pView->property("QByteArray").toByteArray();
        pView->SetTitle(tr(bytearray.constData()));
    }

    for (auto OptionView : m_pScrollAreaHotKey->GetWidgets()){
        IFSOptionView *pView = static_cast<IFSOptionView *>(OptionView);
        auto bytearray = pView->property("QByteArray").toByteArray();
        pView->SetTitle(tr(bytearray.constData()));
    }

}


void FSPropertyView::ConnectSignal()
{
    connect(m_pButtonGroup, SIGNAL(buttonClicked(int)), this, SLOT(OnButtonClickedSlot(int)));
    connect(m_pButtonGroup, SIGNAL(buttonToggled(int, bool)), this, SLOT(OnButtonTogged(int, bool)));
}

void FSPropertyView::SetVisibleView(int flag)
{
    int flag1 = flag & 0x00ff;
    m_pButtonVisual->setVisible(flag1 & 0x01);
    m_pButtonAudio->setVisible(flag1 & 0x02);
    m_pButtonAnnotation->setVisible(flag1 & 0x04);
    m_pButtonCursor->setVisible(flag1 & 0x08);
    m_pButtonHotKey->setVisible(flag1 & 0x10);
}

void FSPropertyView::SetShowHideButtonState(bool showState)
{
    m_pWidgetMain->setVisible(showState);
    m_pButtonShowProperty->setFixedSize(showState ? QSize(35, 35) : QSize(24, 35));
    m_pButtonShowProperty->move(width() - m_pButtonShowProperty->width(), 0);
    emit sigShowPropertyView(showState);
}

/////////////////////////////////////////////////////////////////////////////

FSPropertyTitle::FSPropertyTitle( QWidget *pParent)
    :QWidget(pParent)
    , m_bCandelete(false)
    , m_bEdited(false)
    , m_bShow(true)
{
    CreateUI();
    ConnectSignals();
}

FSPropertyTitle::~FSPropertyTitle()
{

}

void FSPropertyTitle::SetTitleName(const QString &strName)
{
    m_pTitleLabel->setText(strName);
}

void FSPropertyTitle::SetCandelete(bool bCandelete)
{
    m_bCandelete = bCandelete;
    m_pDeleteBtn->setVisible(m_bCandelete);
}

void FSPropertyTitle::SetResetable(bool bResetable)
{
    m_pResetBtn->setVisible(bResetable);
}


void FSPropertyTitle::SetTitleHeight(int height)
{
    setFixedHeight(height);
}

void FSPropertyTitle::CreateUI()
{
    m_pTitleBtn    = new QPushButton(this);
    m_pDeleteBtn   = new FSButton(m_pTitleBtn);
    m_pTitleLabel  = new FSLabel(m_pTitleBtn);
    m_pResetBtn    = new FSButton(m_pTitleBtn);
    m_pButtonGroup = new QButtonGroup(this);

    m_pButtonGroup->addButton(m_pTitleBtn, ID_TITLE);
    m_pButtonGroup->addButton(m_pDeleteBtn, ID_DELETE);
    m_pButtonGroup->addButton(m_pResetBtn, ID_RESET);
    m_pResetBtn->setVisible(m_bEdited);
    m_pDeleteBtn->setVisible(m_bCandelete);
    m_pTitleLabel->setAlignment(Qt::AlignCenter);
    setFixedHeight(39);
    m_pTitleBtn->setFocusPolicy(Qt::NoFocus);
    m_pDeleteBtn->setObjectName("FSPropertyTitle_m_pDeleteBtn");
    m_pResetBtn->setObjectName("FSPropertyTitle_m_pResetBtn");
    m_pTitleBtn->setObjectName("FSPropertyTitle_m_pTitleBtn");
    m_pTitleLabel->setObjectName("FSPropertyTitle_m_pTitleLabel");
}

void FSPropertyTitle::ButtonClickedSlot(int id)
{
    QButtonGroup *pBtn = static_cast<QButtonGroup *>(sender());
    switch (id)
    {
    case ID_TITLE:
        emit ShowView(!m_bShow);
        m_bShow = !m_bShow;
        break;
    case ID_DELETE:
        emit Delete();
        break;
    case ID_RESET:
        emit Reset();
        pBtn->button(id)->setVisible(false);
        break;
    default:
        break;
    }
}

void FSPropertyTitle::ConnectSignals()
{
    connect(m_pButtonGroup, SIGNAL(buttonClicked(int)), this, SLOT(ButtonClickedSlot(int)));
}


void FSPropertyTitle::resizeEvent(QResizeEvent *)
{
    int btnWidth = 14;
    int space = 2;
    int left = 35;
    m_pTitleBtn->setGeometry(this->geometry());
    m_pTitleLabel->setGeometry(left, (this->height() - 20) / 2, width() -2*left, 20);
    m_pResetBtn->setGeometry(m_pTitleLabel->geometry().right(), (this->height() - btnWidth) / 2, btnWidth, btnWidth);
    m_pDeleteBtn->setGeometry(m_pResetBtn->geometry().right() + space, (this->height() - btnWidth) / 2, btnWidth, btnWidth);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FSScrollArea::FSScrollArea(QWidget *pParent)
    :QScrollArea(pParent)
{
    m_pWidget = new QWidget(this);
    setWidget(m_pWidget);
    setWidgetResizable(true);
    setFrameShape(QFrame::NoFrame);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_pVL = new QVBoxLayout(m_pWidget);
    m_pVL->addStretch(100);
    m_pVL->setMargin(0);
    m_pVL->setSpacing(0);
    m_pWidget->setObjectName("FSScrollArea_m_pWidget");
    verticalScrollBar()->setObjectName("FSScrollArea_verticalScrollBar");
}

FSScrollArea::~FSScrollArea()
{

}

void FSScrollArea::AddWidget(QWidget *pWidget,QString name)
{
    Q_ASSERT(pWidget);
    m_pVL->insertWidget(qMax(0,m_pVL->count() - 1), pWidget);
    mWidgetMap.insert(name, pWidget);
}

QList<QWidget *> FSScrollArea::GetWidgets()
{
    QList<QWidget *> list;
    for (int i = 0; i < m_pVL->count();i++)
    {
        if (m_pVL->itemAt(i)->widget())
            list.push_back(m_pVL->itemAt(i)->widget());
    }
    return list;
}



void FSScrollArea::SortWidget(QStringList WidgetNameList)
{
    if (m_pVL)
    {
        delete m_pVL;
    }        
    m_pVL = new QVBoxLayout(m_pWidget);
    m_pVL->addStretch(100);
    m_pVL->setMargin(0);
    m_pVL->setSpacing(0);
    QList<QWidget*>list = mWidgetMap.values();
    for (auto item : WidgetNameList)
    {
        if (mWidgetMap.contains(item))
        {
            m_pVL->insertWidget(qMax(0, m_pVL->count() - 1), mWidgetMap.value(item));
            list.removeOne(mWidgetMap.value(item));
        }            
    }
    for (auto item : list)
    {
        m_pVL->insertWidget(qMax(0, m_pVL->count() - 1), item);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////
FSCursorClickSoundOption::FSCursorClickSoundOption(QWidget *pParent)
    :IFSOptionView(IFSOptionView::TypeCursorClickSound,pParent)
{
    CreateUI();
    ConnectVoluemValueChanged(true);
}

FSCursorClickSoundOption::~FSCursorClickSoundOption()
{

}

void FSCursorClickSoundOption::UpdataParma(IFSEditHelper *pHelper)
{
    Q_ASSERT(pHelper);
    if (pHelper&&pHelper->GetTimeLine()->IsDesktopVideo())
    {
        FSVideoPicEditHelper *pTempHelper = static_cast<FSVideoPicEditHelper *>(pHelper);
        ConnectVoluemValueChanged(false);
        pTempHelper->GetDefaultCursorVolume(m_defaultVolume);
        m_defaultVolume = TranslateValueX(m_defaultVolume);
        int value = pTempHelper->GetMouseClickSound();
        m_pSliderVoluem->setValue(TranslateValueX(value),false);
        m_pDoubleSpinBoxVoluem->setValue(TranslateValueX(value));
        m_pButtonVoluem->setVisible(m_pSliderVoluem->value() != m_defaultVolume);
        m_pTitleWidget->SetResetable(IsTitleResetable());
        ConnectVoluemValueChanged(true);
    }
}

void FSCursorClickSoundOption::GetVoluem(double & Voluem)
{
    Voluem = TranslateValueY(m_pSliderVoluem->value());
}

void FSCursorClickSoundOption::OnReset()
{
    ConnectVoluemValueChanged(false);
    m_pSliderVoluem->setValue(m_defaultVolume, false);
    m_pDoubleSpinBoxVoluem->setValue(m_defaultVolume);
    m_pButtonVoluem->setVisible(false);
    m_pTitleWidget->SetResetable(IsTitleResetable());
    ConnectVoluemValueChanged(true);
    emit ParmaChanged();
}

bool FSCursorClickSoundOption::IsTitleResetable()
{
    return m_pButtonVoluem->isVisible();
}

void FSCursorClickSoundOption::CreateUI()
{
    QLabel *pLabel         = new QLabel(m_pOptionWidget);
    m_pButtonVoluem        = new FSButton(pLabel);
    m_pSliderVoluem        = new ToolBarSlider(m_pOptionWidget, Qt::Horizontal);
    m_pDoubleSpinBoxVoluem = new FSDoubleSpinBox(m_pOptionWidget);
    m_pLabelVoluem         = new FSLabel(m_pOptionWidget);
    m_pVL                  = new QVBoxLayout(m_pOptionWidget);

	//设置spinBox的精度
	m_pDoubleSpinBoxVoluem->setDecimals(0);

    QHBoxLayout *pMainHL = new QHBoxLayout();
    pMainHL->addWidget(m_pLabelVoluem);
    pMainHL->addWidget(m_pSliderVoluem);
    pMainHL->addWidget(m_pDoubleSpinBoxVoluem);
    pMainHL->addWidget(pLabel);
    pMainHL->setContentsMargins(14, 0, 21, 0);
    pMainHL->setSpacing(6);

    m_pVL->addLayout(pMainHL);
    m_pVL->setSpacing(0);
    m_pVL->setContentsMargins(0,17,0,17);

    m_pLabelVoluem->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    m_pLabelVoluem->setFixedWidth(63);

    m_pDoubleSpinBoxVoluem->setButtonSymbols(QAbstractSpinBox::NoButtons);
    m_pDoubleSpinBoxVoluem->setFixedSize(50, 20);

    pLabel->setFixedSize(14, 14);
    m_pButtonVoluem->setFixedSize(14, 14);
    m_pSliderVoluem->setDuration(100);
    m_pSliderVoluem->SetChunkHeight(10);
    m_pDoubleSpinBoxVoluem->setRange(0, 100);

    m_pButtonVoluem->setObjectName("FSPropertyTitle_m_pResetBtn");
    m_pDoubleSpinBoxVoluem->setObjectName("FSVisualPorpertyView_pDoubleSpinBox");
    m_pSliderVoluem->setObjectName("FSVisualPorpertyView_pToolBarSlider");
    m_pLabelVoluem->setObjectName("FSCursorClickSoundOption_m_pLabelVoluem");

    connect(m_pSliderVoluem, &ToolBarSlider::valueChanged, [this](double value){
        m_pDoubleSpinBoxVoluem->setValue(value);
        m_pButtonVoluem->setVisible(value != m_defaultVolume);
        m_pTitleWidget->SetResetable(IsTitleResetable());
    });

    connect(m_pButtonVoluem, &FSButton::clicked, [this](){
        OnReset();
    });
    m_pButtonVoluem->setVisible(false);

    RetranslateUi();
}

void FSCursorClickSoundOption::RetranslateUi()
{
    m_pLabelVoluem->setText(tr("Volume"));

}

int FSCursorClickSoundOption::TranslateValueX(int x)
{
    return 0.5*x + 50;
}



void FSCursorClickSoundOption::ConnectVoluemValueChanged(bool bConnect)
{
    if (!bConnect)
    {
        disconnect(m_pDoubleSpinBoxVoluem, SIGNAL(valueChanged(double)), this, SLOT(OnVoluemValueChangedSlot(double)));
    }
    else
    {
        connect(m_pDoubleSpinBoxVoluem, SIGNAL(valueChanged(double)), this, SLOT(OnVoluemValueChangedSlot(double)));
    }
}

int FSCursorClickSoundOption::TranslateValueY(int y)
{
    return 2 * y - 100;
}

void FSCursorClickSoundOption::OnVoluemValueChangedSlot(double value)
{
    m_pSliderVoluem->setValue(value, false);
    m_pButtonVoluem->setVisible(value!= m_defaultVolume);
    m_pTitleWidget->SetResetable(IsTitleResetable());    
    emit ParmaChanged();
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////
FSBackground1Option::FSBackground1Option(QWidget *pParent)
    :IFSOptionView(IFSOptionView::TypeBackground1,pParent)
    , m_CurrentColor("#000000")
    , m_bChangedFlag(0)
{
    CreateUI();
    ResetChangeFlag();
}

FSBackground1Option::~FSBackground1Option()
{

}

void FSBackground1Option::UpdataParma(IFSEditHelper *pHelper)
{
    Q_ASSERT(pHelper);
    if (pHelper)
    {
        FSTextEditHelper *pTempHelper = static_cast<FSTextEditHelper *>(pHelper);
        NLECaptionObjData objData = pTempHelper->GetCaptionBaseObj();
        if (objData.ObjType == NLECaptionObjType::NLE_DOT_TIPTEXT)
        {
            m_CurrentColor = QColor(pTempHelper->GetBackGroundColor().name(QColor::HexRgb));
            m_pComBoxColor->SetCurrentColor(m_CurrentColor);
            pTempHelper->GetDefaultTextBgColor(m_defaultColor);
            m_defaultColor = QColor(m_defaultColor.name(QColor::HexRgb));
            m_pButtonColor->setVisible(m_CurrentColor != m_defaultColor);
            m_pTitleWidget->SetResetable(IsTitleResetable());
        }
    }    
}

QColor FSBackground1Option::GetColor()
{
    return m_CurrentColor;
}

bool FSBackground1Option::isBgColorChanged()
{
    return m_bChangedFlag & 0x1;
}

void FSBackground1Option::ResetChangeFlag()
{
    m_bChangedFlag = 0;
}

void FSBackground1Option::OnReset()
{
    m_pButtonColor->setVisible(false);
    if (m_defaultColor != m_CurrentColor)
        m_bChangedFlag |= 0x1;
    m_pComBoxColor->SetCurrentColor(m_defaultColor);
    m_CurrentColor = m_defaultColor;
    m_pTitleWidget->SetResetable(IsTitleResetable());
    emit ParmaChanged();
}

bool FSBackground1Option::IsTitleResetable()
{
    return m_pButtonColor->isVisible();
}

void FSBackground1Option::OnColorResetBtnClieck()
{
    m_pButtonColor->setVisible(false);
    m_pTitleWidget->SetResetable(IsTitleResetable());
    m_pComBoxColor->SetCurrentColor(m_defaultColor);
    if (m_defaultColor != m_CurrentColor)
        m_bChangedFlag |= 0x1;
    m_CurrentColor = m_defaultColor;
    emit ParmaChanged();
}

void FSBackground1Option::OnColorChanged(QColor color)
{
    if (color != m_CurrentColor)
        m_bChangedFlag |= 0x1;
    m_CurrentColor = color;
    m_pButtonColor->setVisible(color != m_defaultColor);
    m_pTitleWidget->SetResetable(IsTitleResetable());
    emit ParmaChanged();
}

void FSBackground1Option::CreateUI()
{
    m_pLabelColor  = new FSLabel(m_pOptionWidget);
    m_pComBoxColor = new ColorSelecterComboBox(m_pOptionWidget);
    QLabel *pLabel = new QLabel(m_pOptionWidget);
    m_pButtonColor = new FSButton(pLabel);



    m_pVL = new QVBoxLayout(m_pOptionWidget);

    QHBoxLayout *pHL = new QHBoxLayout();
    pHL->addWidget(m_pLabelColor);
    pHL->addWidget(m_pComBoxColor);
    pHL->addStretch();
    pHL->addWidget(pLabel);   
    pHL->setContentsMargins(14, 17, 21, 17);
    pHL->setSpacing(10);

    m_pVL->addLayout(pHL);
    m_pVL->setMargin(0);
    m_pVL->setSpacing(0);


    m_pLabelColor->setFixedSize(63, 20);
    pLabel->setFixedSize(14, 14);
    m_pButtonColor->setFixedSize(14, 14);
    m_pComBoxColor->setFixedSize(70, 20);


    m_pLabelColor->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    m_pButtonColor->setObjectName("FSPropertyTitle_m_pResetBtn");
    m_pLabelColor->setObjectName("FSPropertyTitle_m_pLabelColor");



    connect(m_pButtonColor, &FSButton::clicked, this,&FSBackground1Option::OnColorResetBtnClieck);
    connect(m_pComBoxColor, &ColorSelecterComboBox::CurrentColorChanged, this,&FSBackground1Option::OnColorChanged);

    RetranslateUi();
}


void FSBackground1Option::RetranslateUi()
{
    m_pLabelColor->setText(tr("Color"));

}

////////////////////////////////////////////////////////////////////////////////////////////////////////
FSTextBaseOption::FSTextBaseOption(QWidget *pParent)
    :IFSOptionView(IFSOptionView::TypeTextBase,pParent)
    , m_bChangedFlag(0)
{
    CreateUI();    
    InitUIText();
    ConnectSignals();
}

FSTextBaseOption::~FSTextBaseOption()
{

}

void FSTextBaseOption::UpdataParma(IFSEditHelper *pHelper)
{
    Q_ASSERT(pHelper);
    if (pHelper)
    {
        FSTextEditHelper *pTempHelper = static_cast<FSTextEditHelper *>(pHelper);
        NLECaptionObjData objData = pTempHelper->GetCaptionBaseObj();
        if (objData.ObjType == NLECaptionObjType::NLE_DOT_TIPTEXT || objData.ObjType == NLECaptionObjType::NLE_DOT_TEXT)
        {
            BlockSignals(true);
            pTempHelper->GetDefaultFontName(m_DefaultFontName);
            pTempHelper->GetDefaultFontSize(m_DefaultFontSize);
            pTempHelper->GetDefaultFontColor(m_DefaultFontColor);
            pTempHelper->GetDefaultTextAlignType(m_DefaultType);
            pTempHelper->GetDefaultFontBold(m_bDefaultBold);
            pTempHelper->GetDefaultFontItalic(m_bDefaultItalic);
            pTempHelper->GetDefaultShadowDerection(m_shadowDerection);
            QString fontName = pTempHelper->GetFont();
            int fontSize = pTempHelper->GetFontSize();
            QColor fontColor = pTempHelper->GetTextColor();
            NLEAlignType type = pTempHelper->GetTextAlign();
            bool bItalic = pTempHelper->GetFontIatlic();
            bool bBold = pTempHelper->GetFontBold();
            int derection = pTempHelper->GetTextShadowDerection();
            SetFont(fontName);
            SetFontSize(fontSize);
            SetTextColor(fontColor);
            SetTextAlign(type);
            SetFontBold(bBold);
            SetFontItalic(bItalic);
            SetText(pTempHelper->GetText());
            m_pButtonFontReset->setVisible(fontName != m_DefaultFontName);
            m_pButtonSizeReset->setVisible(fontSize != m_DefaultFontSize);
            m_pButtonColorReset->setVisible(IsStyleResetable());
            m_pButtonAlignReset->setVisible(type!=m_DefaultType);
            m_pTitleWidget->SetResetable(IsTitleResetable());
            pTempHelper->SetTextChangeCallBack();
            BlockSignals(false);
        }
    }
}

QString FSTextBaseOption::GetFont()
{
    return m_FontMap.value(m_pComboBoxFont->currentText());
}

void FSTextBaseOption::SetFont(QString FontName)
{
    if (!FontName.isEmpty())
    {
        if (!m_FontMap.values().contains(FontName))
        {
            m_FontMap.insert(FontName, FontName);
            m_pComboBoxFont->clear();
            m_pComboBoxFont->addItems(m_FontMap.keys());
        }
        m_pComboBoxFont->setCurrentText(FontName);
    }   
}

int FSTextBaseOption::GetFontSize()
{
    return m_pComboBoxSize->currentText().toInt();
}

void FSTextBaseOption::SetFontSize(int size)
{
    if (size > 0)
    {
        if (!m_FontSizeList.contains(QString::number(size)))
        {
            m_FontSizeList.push_front(QString::number(size));
            m_pComboBoxSize->clear();
            m_pComboBoxSize->addItems(m_FontSizeList);
        }
        m_pComboBoxSize->setCurrentText(QString::number(size));
    }    
}

QColor FSTextBaseOption::GetTextColor()
{
    return m_pComboBoxColor->GetCurrentColor();
}

void FSTextBaseOption::SetTextColor(QColor color)
{
    m_pComboBoxColor->SetCurrentColor(color);
}

NLEAlignType FSTextBaseOption::GetTextAlign()
{
    if (m_pButtonAlignLeft->isChecked())
    {
        return NLEAlignType::NLE_AT_LEFT;
    }
    if (m_PButtonAlignCenter->isChecked())
    {
        return NLEAlignType::NLE_AT_CENTER;
    }
    if (m_pButtonAlignRight->isChecked())
    {
        return NLEAlignType::NLE_AT_RIGHT;
    }
    if (m_pButtonAlign2Side->isChecked())
    {
        return NLEAlignType::NLE_AT_AVERAGE;
    }
    return  NLEAlignType::NLE_AT_LEFT;
}

void FSTextBaseOption::SetTextAlign(NLEAlignType type)
{
    m_pButtonAlignLeft->setChecked(type == NLEAlignType::NLE_AT_LEFT);
    m_PButtonAlignCenter->setChecked(type == NLEAlignType::NLE_AT_CENTER);
    m_pButtonAlignRight->setChecked(type == NLEAlignType::NLE_AT_RIGHT);
    m_pButtonAlign2Side->setChecked(type == NLEAlignType::NLE_AT_AVERAGE);
}

bool FSTextBaseOption::IsFontBold()
{
    return m_pButtonBold->isChecked();
}

void FSTextBaseOption::SetFontBold(bool bBold)
{
    m_pButtonBold->setChecked(bBold);
}

bool FSTextBaseOption::IsFontItalic()
{
    return m_PButtonItalic->isChecked();
}

void FSTextBaseOption::SetFontItalic(bool bItalic)
{
    m_PButtonItalic->setChecked(bItalic);
}

bool FSTextBaseOption::isFontNameChanged()
{
    return m_bChangedFlag & 0x1;
}

bool FSTextBaseOption::isFontSizeChanged()
{
    return m_bChangedFlag & 0x2;
}

bool FSTextBaseOption::isFontColorChanged()
{
    return m_bChangedFlag & 0x4;
}

bool FSTextBaseOption::isTextAlignChanged()
{
    return m_bChangedFlag & 0x8;
}

void FSTextBaseOption::ResetChangeFlag()
{
    m_bChangedFlag = 0;
}

void FSTextBaseOption::SetText(QString text)
{
    BlockSignals(true);
    m_pTextEdit->setText(text);    
    BlockSignals(false);
}

void FSTextBaseOption::OnReset()
{
    BlockSignals(true);
    m_pButtonFontReset->setVisible(false);
    m_pButtonSizeReset->setVisible(false);
    m_pButtonColorReset->setVisible(false);
    m_pButtonAlignReset->setVisible(false);
    m_pTitleWidget->SetResetable(false);
    if (m_DefaultFontName != GetFont())
        m_bChangedFlag |= 0x1;
    SetFont(m_DefaultFontName);
    if (m_DefaultFontSize != GetFontSize())
        m_bChangedFlag |= 0X2;
    SetFontSize(m_DefaultFontSize);
    if (IsStyleResetable())
        m_bChangedFlag |= 0X4;
    SetTextColor(m_DefaultFontColor);
    SetFontBold(m_bDefaultBold);
    SetFontItalic(m_bDefaultItalic);
    if (m_DefaultType != GetTextAlign())
        m_bChangedFlag |= 0x8;
    SetTextAlign(m_DefaultType);
    BlockSignals(false);
    emit ParmaChanged();
}

bool FSTextBaseOption::IsTitleResetable()
{
    return m_pButtonFontReset->isVisible() || m_pButtonSizeReset->isVisible() || m_pButtonColorReset->isVisible() || m_pButtonAlignReset->isVisible();
}

bool FSTextBaseOption::IsStyleResetable()
{
    return m_DefaultFontColor != GetTextColor() || (m_bDefaultBold != IsFontBold()) || (m_bDefaultItalic != IsFontItalic());
}

void FSTextBaseOption::CreateUI()
{
    m_pButtonGroup = new QButtonGroup(this);
    m_pButtonGroup1 = new QButtonGroup(this);
    m_pButtonGroup1->setExclusive(false);

    m_pLabelFont   = new FSLabel(this);
    m_pLabelColor  = new FSLabel(this);
    m_pLabelSize   = new FSLabel(this);
    m_pLabelAlign  = new FSLabel(this);
    m_pLabelContent = new FSLabel(this);

    m_pComboBoxFont  = new FSComboBox(this);
    m_pComboBoxColor = new ColorSelecterComboBox(this);
    m_pComboBoxSize  = new FSComboBox(this);

    m_pButtonAlignLeft   = new FSButton(this);
    m_PButtonAlignCenter = new FSButton(this);
    m_pButtonAlignRight  = new FSButton(this);
    m_pButtonAlign2Side  = new FSButton(this);
    m_pButtonAlignReset  = new FSButton(this);
    m_pButtonFontReset   = new FSButton(this);
    m_pButtonSizeReset   = new FSButton(this);
    m_pButtonColorReset  = new FSButton(this);

    m_pButtonBold      = new FSButton(this);
    m_PButtonItalic    = new FSButton(this);

    m_pTextEdit        = new FSTextEdit(this);


    int h = 20, w = 55, space = 10, L = 14, R = 21, btnW = 14,alignW=17;

    m_pButtonGroup->addButton(m_pButtonFontReset, 0);
    m_pButtonGroup->addButton(m_pButtonSizeReset, 1);
    m_pButtonGroup->addButton(m_pButtonColorReset, 2);   
    m_pButtonGroup->addButton(m_pButtonAlignReset, 3);
    m_pButtonGroup->addButton(m_pButtonAlignLeft, 4);
    m_pButtonGroup->addButton(m_PButtonAlignCenter, 5);
    m_pButtonGroup->addButton(m_pButtonAlignRight, 6);
    m_pButtonGroup->addButton(m_pButtonAlign2Side, 7);

    m_pButtonGroup1->addButton(m_pButtonBold, 0);
    m_pButtonGroup1->addButton(m_PButtonItalic, 1);


    QVBoxLayout *pVL  = new QVBoxLayout(m_pOptionWidget);

    QVBoxLayout *pVL1 = new QVBoxLayout();
    QHBoxLayout *pHL1 = new QHBoxLayout();
    QHBoxLayout *pHL2 = new QHBoxLayout();
    QHBoxLayout *pHL3 = new QHBoxLayout();
    QHBoxLayout *pHL4 = new QHBoxLayout();
    QHBoxLayout *pHL5 = new QHBoxLayout();
    QHBoxLayout *pHL6 = new QHBoxLayout();

    pHL5->addWidget(m_pButtonBold);
    pHL5->addWidget(m_PButtonItalic);
    pHL5->setSpacing(0);
    pHL5->setMargin(0);

    pHL1->addWidget(m_pLabelFont);
    pHL1->addWidget(m_pComboBoxFont);
    pHL1->addStretch();
    pHL1->addWidget(m_pButtonFontReset);
    pHL1->setContentsMargins(L, 0, R,0);
    pHL1->setSpacing(space);

    pHL2->addWidget(m_pLabelSize);
    pHL2->addWidget(m_pComboBoxSize);
    pHL2->addStretch();
    pHL2->addWidget(m_pButtonSizeReset);
    pHL2->setContentsMargins(L, 0,R, 0);
    pHL2->setSpacing(space);

    pHL3->addWidget(m_pLabelColor);
    pHL3->addLayout(pHL5);
    pHL3->addWidget(m_pComboBoxColor);
    pHL3->addStretch();
    pHL3->addWidget(m_pButtonColorReset);
    pHL3->setContentsMargins(L, 0, R, 0);
    pHL3->setSpacing(space);

    pHL4->addWidget(m_pLabelAlign);
    pHL4->addWidget(m_pButtonAlignLeft);
    pHL4->addWidget(m_PButtonAlignCenter);
    pHL4->addWidget(m_pButtonAlignRight);
    pHL4->addWidget(m_pButtonAlign2Side);
    pHL4->addStretch();
    pHL4->addWidget(m_pButtonAlignReset);
    pHL4->setContentsMargins(L,0, R, 0);
    pHL4->setSpacing(space);

    pVL1->addWidget(m_pLabelContent);
    pVL1->addStretch();
    pVL1->setMargin(0);
    pVL1->setSpacing(0);

    pHL6->addLayout(pVL1);
    pHL6->addWidget(m_pTextEdit);
    pHL6->addSpacing(btnW);
    pHL6->setContentsMargins(L, 0, R, 0);
    pHL6->setSpacing(space);

    pVL->addLayout(pHL1);
    pVL->addLayout(pHL2);
    pVL->addLayout(pHL3);
    pVL->addLayout(pHL4);
    pVL->addSpacing(6);
    pVL->addLayout(pHL6);
    pVL->setContentsMargins(0,9,0,13);
    pVL->setSpacing(space);

    m_pButtonAlignReset->setFixedSize(btnW, btnW);
    m_pButtonColorReset->setFixedSize(btnW, btnW);
    m_pButtonSizeReset->setFixedSize(btnW, btnW);
    m_pButtonFontReset->setFixedSize(btnW, btnW);

    m_pComboBoxFont->setFixedSize(145, h);
    m_pComboBoxSize->setFixedSize(70, h);
    m_pComboBoxColor->setFixedSize(w, h);

    m_pButtonAlignLeft->setFixedSize(alignW, alignW);
    m_PButtonAlignCenter->setFixedSize(alignW, alignW);
    m_pButtonAlignRight->setFixedSize(alignW, alignW);
    m_pButtonAlign2Side->setFixedSize(alignW, alignW);

    m_pButtonBold->setFixedSize(btnW, btnW);
    m_PButtonItalic->setFixedSize(btnW, btnW);

    m_pTextEdit->setFixedHeight(100);

    m_pLabelFont->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    m_pLabelSize->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    m_pLabelColor->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    m_pLabelAlign->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    m_pLabelContent->setAlignment(Qt::AlignTop | Qt::AlignRight);

    m_pButtonAlignLeft->setCheckable(true);
    m_PButtonAlignCenter->setCheckable(true);
    m_pButtonAlignRight->setCheckable(true);
    m_pButtonAlign2Side->setCheckable(true);

    m_pButtonBold->setCheckable(true);
    m_PButtonItalic->setCheckable(true);
    m_pTextEdit->setUndoRedoEnabled(false);
    m_pButtonFontReset->setObjectName("FSPropertyTitle_m_pResetBtn");
    m_pButtonSizeReset->setObjectName("FSPropertyTitle_m_pResetBtn");
    m_pButtonColorReset->setObjectName("FSPropertyTitle_m_pResetBtn");
    m_pButtonAlignReset->setObjectName("FSPropertyTitle_m_pResetBtn");

    m_pLabelFont->setObjectName("FSPropertyTitle_m_pLabelFont");
    m_pLabelSize->setObjectName("FSPropertyTitle_m_pLabelSize");
    m_pLabelColor->setObjectName("FSPropertyTitle_m_pLabelColor");
    m_pLabelAlign->setObjectName("FSPropertyTitle_m_pLabelAlign");
    m_pLabelContent->setObjectName("FSPropertyTitle_m_pLabelFont");

    m_pButtonBold->setObjectName("FSTextBaseOption_m_pButtonBold");
    m_PButtonItalic->setObjectName("FSTextBaseOption_m_PButtonItalic");
    m_pButtonAlignLeft->setObjectName("FSTextBaseOption_m_pButtonAlignLeft");
    m_PButtonAlignCenter->setObjectName("FSTextBaseOption_m_PButtonAlignCenter");
    m_pButtonAlignRight->setObjectName("FSTextBaseOption_m_pButtonAlignRight");
    m_pButtonAlign2Side->setObjectName("FSTextBaseOption_m_pButtonAlign2Side");

    m_pComboBoxFont->setObjectName("FSTextBaseOption_m_pComboBox");
    m_pComboBoxSize->setObjectName("FSTextBaseOption_m_pComboBox");
    m_pTextEdit->setObjectName("FSTextBaseOption_m_pTextEdit");
    RetranslateUi();
}

void FSTextBaseOption::OnButtonClikedSlot(int id)
{
    QButtonGroup *pGroup = static_cast<QButtonGroup *> (sender());
    switch (id)
    {
    case 0:
        m_pButtonFontReset->setVisible(false);
        m_bChangedFlag |= 0x1;
        SetFont(m_DefaultFontName);
        break;
    case 1:
        m_bChangedFlag |= 0X2;
        m_pButtonSizeReset->setVisible(false);
        SetFontSize(m_DefaultFontSize);
        break;
    case 2:
        m_bChangedFlag |= 0X4;
        m_pButtonColorReset->setVisible(false);
        SetTextColor(m_DefaultFontColor);
        SetFontBold(m_bDefaultBold);
        SetFontItalic(m_bDefaultItalic);
        break;
    case 3:       
        m_bChangedFlag |= 0x8;
        m_pButtonAlignReset->setVisible(false);
        SetTextAlign(m_DefaultType);
        break;
    case 4:
    case 5:
    case 6:
    case 7:
        m_bChangedFlag |= 0x8;
        break;
    default:
        break;
    }
    m_pTitleWidget->SetResetable(IsTitleResetable());
    emit ParmaChanged();
}

void FSTextBaseOption::OnButtonGroup1ClikedSlot(int id)
{
    QButtonGroup *pGroup = static_cast<QButtonGroup *> (sender());
    switch (id)
    {
    case 0:
        SetFontBold(IsFontBold());       
        break;
    case 1:
        SetFontItalic(IsFontItalic());
        break;
    }
    m_bChangedFlag |= 0X4;
    m_pButtonColorReset->setVisible(IsStyleResetable());
    m_pTitleWidget->SetResetable(IsTitleResetable());
    emit ParmaChanged();
}

void FSTextBaseOption::OnFontChangedSlot(QString text)
{
    m_pButtonFontReset->setVisible(GetFont()!=m_DefaultFontName);
    m_pTitleWidget->SetResetable(IsTitleResetable());
    m_pComboBoxFont->setToolTip(text);
    m_bChangedFlag |= 0x1;
    emit ParmaChanged();
}

void FSTextBaseOption::OnSizeChangedSlot(QString text)
{
    m_pButtonSizeReset->setVisible(GetFontSize()!=m_DefaultFontSize);
    m_pTitleWidget->SetResetable(IsTitleResetable());
    m_bChangedFlag |= 0x2;
    emit ParmaChanged();
}

void FSTextBaseOption::ConnectSignals()
{
    connect(m_pComboBoxFont, &QComboBox::currentTextChanged, this, &FSTextBaseOption::OnFontChangedSlot);
    connect(m_pComboBoxSize, &QComboBox::currentTextChanged, this, &FSTextBaseOption::OnSizeChangedSlot);
    connect(m_pButtonGroup, SIGNAL(buttonClicked(int)), this, SLOT(OnButtonClikedSlot(int)));
    connect(m_pButtonGroup1, SIGNAL(buttonClicked(int)), this, SLOT(OnButtonGroup1ClikedSlot(int)));
    connect(m_pComboBoxColor, &ColorSelecterComboBox::CurrentColorChanged, [this](QColor color){
        m_pButtonColorReset->setVisible(color!=m_DefaultFontColor);
        m_pTitleWidget->SetResetable(IsTitleResetable());
        m_bChangedFlag |= 0x4;
        emit ParmaChanged();
    });
    connect(m_pTextEdit, &FSTextEdit::selectionChanged, this, [&](){
        QTextCursor textCursor = m_pTextEdit->textCursor();
        m_pTextEdit->SetPos(textCursor.position());
        emit sigCharSelecetChanged(textCursor.selectionStart(), textCursor.selectedText().length());
    });

    connect(m_pTextEdit, &FSTextEdit::cursorPositionChanged, this, [&](){
        QTextCursor textCursor = m_pTextEdit->textCursor();
        m_pTextEdit->SetPos(textCursor.position());
        emit sigCharSelecetChanged(textCursor.selectionStart(), textCursor.selectedText().length());
    });

    connect(m_pTextEdit, &FSTextEdit::sigKeyPress, this, [&](QKeyEvent *e){
        emit sigKeyPress(e); 

    });
    connect(m_pTextEdit, &FSTextEdit::sigInputMethod, this, [&](QInputMethodEvent *e){
        emit sigInputMethod(e);
    });
}


void FSTextBaseOption::InitUIText()
{
    m_FontMap.clear();
    ICaptionEnvironment::CaptionFonts fontList = ICaptionEnvironment::GetInstance()->GetNLEFonts();
    for (auto item:fontList)
    {
        m_FontMap.insert(std::get<0>(item), std::get<1>(item));
    }
    for (auto itemText : m_FontMap.keys())
    {
        m_pComboBoxFont->addItem(itemText);
        m_pComboBoxFont->setItemData(m_pComboBoxFont->count() - 1, itemText, Qt::ToolTipRole);
    }
    m_FontSizeList.clear();
    for (int i = 8; i <= 160;i++)
    {
        m_FontSizeList.push_back(QString::number(i));
        m_pComboBoxSize->addItem(m_FontSizeList.at(i-8));
    }
}

void FSTextBaseOption::RetranslateUi()
{
    m_pLabelFont->setText(tr("Font"));
    m_pLabelSize->setText(tr("Size"));
    m_pLabelColor->setText(tr("Style"));
    m_pLabelAlign->setText(tr("Align"));
    m_pLabelContent->setText(tr("Content"));

    int w = 63;
    int w1 = GetRetranslateUiTextLenght(m_pLabelFont->font(), m_pLabelFont->text(), w);
    int w2 = GetRetranslateUiTextLenght(m_pLabelSize->font(), m_pLabelSize->text(), w);
    int w3 = GetRetranslateUiTextLenght(m_pLabelColor->font(), m_pLabelColor->text(), w);
    int w4 = GetRetranslateUiTextLenght(m_pLabelAlign->font(), m_pLabelAlign->text(), w);
    int w5 = GetRetranslateUiTextLenght(m_pLabelContent->font(), m_pLabelContent->text(), w);
    w = qMax(w1, w);
    w = qMax(w2, w);
    w = qMax(w3, w);
    w = qMax(w4, w);
    w = qMax(w5, w);

    m_pLabelFont->setFixedSize(w, 20);
    m_pLabelSize->setFixedSize(w, 20);
    m_pLabelColor->setFixedSize(w, 20);
    m_pLabelAlign->setFixedSize(w, 17);
    m_pLabelContent->setFixedSize(w, 20);
}

void FSTextBaseOption::BlockSignals(bool bBlock)
{
    m_pComboBoxSize->blockSignals(bBlock);
    m_pComboBoxFont->blockSignals(bBlock);
    m_pButtonGroup->blockSignals(bBlock);
    m_pButtonGroup->blockSignals(bBlock);
    m_pTextEdit->blockSignals(bBlock);
}

FSTextEdit * FSTextBaseOption::GetTextEdit()
{
    return m_pTextEdit;
}

///////////////////////////////////////////////////////////////////////////////////////
FSCursorBaseOption::FSCursorBaseOption(QWidget *pParent)
    :IFSOptionView(IFSOptionView::TypeCursorBase,pParent)
{
    CreateUI();
    ConnectSignals();
}

FSCursorBaseOption::~FSCursorBaseOption()
{

}

void FSCursorBaseOption::UpdataParma(IFSEditHelper *pHelper)
{
    Q_ASSERT(pHelper);
    if (pHelper)
    {
        ConnectScaleChangedSignals(false);
        ConnectOpacityChangedSignals(false);
        FSVideoPicEditHelper *pTempHelper = static_cast<FSVideoPicEditHelper *>(pHelper);
        double Opacity;
        pTempHelper->GetDefaultCursorScale(m_DefaultScale);
        pTempHelper->GetDefaultCursorOptacity(Opacity);
        m_DefaultOpacity = Opacity * 100;
        m_DefaultScale = 100 * m_DefaultScale;
        double valueScale = pTempHelper->GetMouseScale() * 100;
        double valueOpacity = pTempHelper->GetMouseOpacity() * 100;
        m_pSliderScale->setValue(valueScale);
        m_pSliderOpacity->setValue(valueOpacity);
        m_pDoubleSpinBoxScale->setValue(valueScale);
        m_pDoubleSpinBoxOpacity->setValue(valueOpacity);
        m_pButtonScale->setVisible(qAbs(valueScale-m_DefaultScale)>1e-2);
        m_pButtonOpacity->setVisible(qAbs(valueOpacity - m_DefaultOpacity)>1e-2);
        m_pTitleWidget->SetResetable(IsTitleResetable());
        ConnectScaleChangedSignals(true);
        ConnectOpacityChangedSignals(true);
    }
}

double FSCursorBaseOption::GetMouseScale()
{
    return m_pDoubleSpinBoxScale->value() / 100;
}

double FSCursorBaseOption::GetMouseOpacity()
{
    return m_pDoubleSpinBoxOpacity->value() / 100;
}

void FSCursorBaseOption::OnReset()
{
    ConnectScaleChangedSignals(false);
    ConnectOpacityChangedSignals(false);
    m_pSliderScale->setValue(m_DefaultScale);
    m_pSliderOpacity->setValue(m_DefaultOpacity);

    m_pDoubleSpinBoxScale->setValue(m_DefaultScale);
    m_pDoubleSpinBoxOpacity->setValue(m_DefaultOpacity);
    m_pButtonScale->setVisible(false);
    m_pButtonOpacity->setVisible(false);
    m_pTitleWidget->SetResetable(IsTitleResetable());
    emit ParmaChanged();
    ConnectScaleChangedSignals(true);
    ConnectOpacityChangedSignals(true);
}

bool FSCursorBaseOption::IsTitleResetable()
{
    return m_pButtonScale->isVisible() || m_pButtonOpacity->isVisible();
}

void FSCursorBaseOption::OnScaleValueChangedSlot(double value)
{
    m_pSliderScale->setValue(value-5);
    m_pButtonScale->setVisible(qAbs(value - m_DefaultScale)>1e-2);
    m_pTitleWidget->SetResetable(IsTitleResetable());
    emit ParmaChanged();
}

void FSCursorBaseOption::OnOpacityValueChangedSlot(double value)
{
    m_pSliderOpacity->setValue(value);
    m_pButtonOpacity->setVisible(qAbs(value - m_DefaultOpacity)>1e-2);
    m_pTitleWidget->SetResetable(IsTitleResetable());
    emit ParmaChanged();
}

void FSCursorBaseOption::CreateUI()
{
    m_pLabelScale   = new FSLabel(this);
    m_pLabelOpacity = new FSLabel(this);

    m_pSliderScale   = new ToolBarSlider(this,Qt::Horizontal);
    m_pSliderOpacity = new ToolBarSlider(this,Qt::Horizontal);

    m_pButtonScale   = new FSButton(this);
    m_pButtonOpacity = new FSButton(this);

    m_pDoubleSpinBoxOpacity = new FSDoubleSpinBox(this);
    m_pDoubleSpinBoxScale = new FSDoubleSpinBox(this);


	m_pDoubleSpinBoxOpacity->setDecimals(0);
	m_pDoubleSpinBoxScale->setDecimals(0);

    m_pDoubleSpinBoxOpacity->setSuffix("%");
    m_pDoubleSpinBoxScale->setSuffix("%");

    QVBoxLayout *pVL = new QVBoxLayout(m_pOptionWidget);

    QHBoxLayout *pHL1 = new QHBoxLayout();
    QHBoxLayout *pHL2 = new QHBoxLayout();

    pHL1->addWidget(m_pLabelScale);
    pHL1->addWidget(m_pSliderScale);
    pHL1->addWidget(m_pDoubleSpinBoxScale);
    pHL1->addStretch();
    pHL1->addWidget(m_pButtonScale);
    pHL1->setContentsMargins(14, 10, 21, 0);
    pHL1->setSpacing(10);

    pHL2->addWidget(m_pLabelOpacity);
    pHL2->addWidget(m_pSliderOpacity);
    pHL2->addWidget(m_pDoubleSpinBoxOpacity);
    pHL2->addStretch();
    pHL2->addWidget(m_pButtonOpacity);
    pHL2->setContentsMargins(14, 10, 21, 0);
    pHL2->setSpacing(10);

    pVL->addLayout(pHL1);
    pVL->addLayout(pHL2);
    pVL->setContentsMargins(0,0,0,13);
    pVL->setSpacing(0);

    m_pDoubleSpinBoxScale->setRange(0, 500);
    m_pSliderScale->setDuration(495);
    m_pDoubleSpinBoxOpacity->setRange(0, 100);
    m_pSliderOpacity->setDuration(100);

    m_pDoubleSpinBoxScale->setButtonSymbols(QAbstractSpinBox::NoButtons);
    m_pDoubleSpinBoxOpacity->setButtonSymbols(QAbstractSpinBox::NoButtons);
    m_pDoubleSpinBoxScale->setFixedSize(50, 20);
    m_pDoubleSpinBoxOpacity->setFixedSize(50, 20);



    m_pLabelScale->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    m_pLabelOpacity->setAlignment(Qt::AlignVCenter | Qt::AlignRight);

    m_pButtonScale->setFixedSize(14, 14);
    m_pButtonOpacity->setFixedSize(14, 14);

    //m_pLabelScale->setFixedSize(63, 20);
    //m_pLabelOpacity->setFixedSize(63, 20);
    m_pSliderScale->SetChunkHeight(10);
    m_pSliderOpacity->SetChunkHeight(10);
    m_pSliderScale->setFixedWidth(110);
    m_pSliderOpacity->setFixedWidth(110);
    m_pButtonScale->setObjectName("FSPropertyTitle_m_pResetBtn");
    m_pButtonOpacity->setObjectName("FSPropertyTitle_m_pResetBtn");
    m_pSliderOpacity->setObjectName("FSVisualPorpertyView_pToolBarSlider");
    m_pSliderScale->setObjectName("FSVisualPorpertyView_pToolBarSlider");
    m_pDoubleSpinBoxScale->setObjectName("FSVisualPorpertyView_pDoubleSpinBox");
    m_pDoubleSpinBoxOpacity->setObjectName("FSVisualPorpertyView_pDoubleSpinBox");
    m_pLabelOpacity->setObjectName("FSCursorBaseOption_m_pLabelOpacity");
    m_pLabelScale->setObjectName("FSCursorBaseOption_m_pLabelScale");

    RetranslateUi();

}

void FSCursorBaseOption::ConnectSignals()
{
    connect(m_pButtonScale, &FSButton::clicked, [this](){
        m_pSliderScale->setValue(m_DefaultScale);
        m_pDoubleSpinBoxScale->setValue(m_DefaultScale);
        m_pButtonScale->setVisible(false);
        m_pTitleWidget->SetResetable(IsTitleResetable());
        emit ParmaChanged();
    });

    connect(m_pButtonOpacity, &FSButton::clicked, [this](){
        m_pSliderOpacity->setValue(m_DefaultOpacity);
        m_pDoubleSpinBoxOpacity->setValue(m_DefaultOpacity);
        m_pButtonOpacity->setVisible(false);
        m_pTitleWidget->SetResetable(IsTitleResetable());
        emit ParmaChanged();
    });

    connect(m_pSliderScale, &ToolBarSlider::valueChanged, [this](float value){
        m_pDoubleSpinBoxScale->setValue(value+5);
    });
    connect(m_pSliderOpacity, &ToolBarSlider::valueChanged, [this](float value){
        m_pDoubleSpinBoxOpacity->setValue(value);
    });
    ConnectScaleChangedSignals(true);
    ConnectOpacityChangedSignals(true);
}



void FSCursorBaseOption::ConnectScaleChangedSignals(bool bConnect)
{
    if (bConnect)
    {
        connect(m_pDoubleSpinBoxScale, SIGNAL(valueChanged(double)), this, SLOT(OnScaleValueChangedSlot(double)));
    }
    else{
        disconnect(m_pDoubleSpinBoxScale, SIGNAL(valueChanged(double)), this, SLOT(OnScaleValueChangedSlot(double)));
    }
}

void FSCursorBaseOption::ConnectOpacityChangedSignals(bool bConnect)
{
    if (bConnect)
    {
        connect(m_pDoubleSpinBoxOpacity, SIGNAL(valueChanged(double)), this, SLOT(OnOpacityValueChangedSlot(double)));
    }
    else{
        disconnect(m_pDoubleSpinBoxOpacity, SIGNAL(valueChanged(double)), this, SLOT(OnOpacityValueChangedSlot(double)));
    }
}

void FSCursorBaseOption::RetranslateUi()
{
    m_pLabelScale->setText(tr("Scale"));
    m_pLabelOpacity->setText(tr("Opacity"));

    int w = 63;
    int w1 = GetRetranslateUiTextLenght(m_pLabelScale->font(), m_pLabelScale->text(), w);
    int w2 = GetRetranslateUiTextLenght(m_pLabelOpacity->font(), m_pLabelOpacity->text(), w);
    w = qMax(w1, w);
    w = qMax(w2, w);
    m_pLabelScale->setFixedSize(w, 20);
    m_pLabelOpacity->setFixedSize(w, 20);

}

//////////////////////////////////////////////////////////////////////////////////////////////////////
FSCursorHighlightOption::FSCursorHighlightOption(QWidget *pParent)
    :FSBackground1Option(pParent)
{
    SetOptionType(IFSOptionView::TypeCursorHighlight);
}

FSCursorHighlightOption::~FSCursorHighlightOption()
{

}


void FSCursorHighlightOption::UpdataParma(IFSEditHelper *pHelper)
{
    Q_ASSERT(pHelper);
    if (pHelper&&pHelper->GetTimeLine()->IsDesktopVideo())
    {
        FSVideoPicEditHelper *pTempHelper = static_cast<FSVideoPicEditHelper *>(pHelper);
        pTempHelper->GetDefaultCursorHighlight(m_defaultColor);
        QColor color = pTempHelper->GetHighlightColor();
        m_pButtonColor->setVisible(color != m_defaultColor);
        m_pTitleWidget->SetResetable(IsTitleResetable());
        m_pComBoxColor->SetCurrentColor(color);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
FSAudioBaseOption::FSAudioBaseOption(QWidget *pParent)
    :FSCursorClickSoundOption(pParent)
{
    SetOptionType(IFSOptionView::TypeAudioBase);
    m_pCheckBox = new QCheckBox(pParent);
    QHBoxLayout *pHL = new QHBoxLayout();
    pHL->setContentsMargins(14, 10, 12, 0);
    pHL->addSpacing(73);
    pHL->addWidget(m_pCheckBox);
    m_pVL->addLayout(pHL);
    m_pCheckBox->setFocusPolicy(Qt::NoFocus);
    m_pCheckBox->setObjectName("FSAudioBaseOption_m_pCheckBox");
    RetranslateUi();
    ConnectMix2MononStateChnage(true);
}

FSAudioBaseOption::~FSAudioBaseOption()
{
    
}



void FSAudioBaseOption::UpdataParma(IFSEditHelper *pHelper)
{

    Q_ASSERT(pHelper);
    if (pHelper)
    {
        int voluem = pHelper->GetTimeLine()->GetVolume();
        bool bEnableMix2Mono = pHelper->GetTimeLine()->GetMix2Mono();
        ConnectMix2MononStateChnage(false);
        ConnectVoluemValueChanged(false);
        pHelper->GetDefaultVolume(m_defaultVolume);
        m_defaultVolume = TranslateValueX(m_defaultVolume);
        m_pCheckBox->setChecked(bEnableMix2Mono);
        m_pSliderVoluem->setValue(TranslateValueX(voluem), false);
        m_pDoubleSpinBoxVoluem->setValue(TranslateValueX(voluem));
        m_pButtonVoluem->setVisible(m_pSliderVoluem->value() != m_defaultVolume);
        m_pTitleWidget->SetResetable(IsTitleResetable());
        ConnectVoluemValueChanged(true);
        ConnectMix2MononStateChnage(true);
    }
}

void FSAudioBaseOption::RetranslateUi()
{
    m_pCheckBox->setText(tr("Mix to mono"));
    FSCursorClickSoundOption::RetranslateUi();

}

bool FSAudioBaseOption::isMix2MonoChecked()
{
    return m_pCheckBox->isChecked();
}

void FSAudioBaseOption::ConnectMix2MononStateChnage(bool bConnect)
{
    if (bConnect)
    {
        connect(m_pCheckBox, &QCheckBox::stateChanged, this, &FSAudioBaseOption::OnMix2MonoStateChnaged);
    }
    else
    {
        disconnect(m_pCheckBox, &QCheckBox::stateChanged, this, &FSAudioBaseOption::OnMix2MonoStateChnaged);
    }
    
}

void FSAudioBaseOption::OnMix2MonoStateChnaged(int checkState)
{
    emit ParmaChanged();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
FSCursorClickRingOption::FSCursorClickRingOption(QWidget *pParent)
    :FSBackground1Option(pParent)
{
    SetOptionType(IFSOptionView::TypeCursorClickRing);
}

FSCursorClickRingOption::~FSCursorClickRingOption()
{

}

void FSCursorClickRingOption::UpdataParma(IFSEditHelper *pHelper)
{
    Q_ASSERT(pHelper);
    if (pHelper&&pHelper->GetTimeLine()->IsDesktopVideo())
    {
        FSVideoPicEditHelper *pTempHelper = static_cast<FSVideoPicEditHelper *>(pHelper);
        pTempHelper->GetDefaultCursorRingColor(m_defaultColor);
        QColor color=pTempHelper->GetMouseRingColor();
        m_pButtonColor->setVisible(color != m_defaultColor);
        m_pTitleWidget->SetResetable(IsTitleResetable());
        m_pComBoxColor->SetCurrentColor(color);
    }
}

AngleDoubleSpinBox::AngleDoubleSpinBox(QWidget *pParent)
:FSDoubleSpinBox(pParent)
{
    //m_dragHelper = new IDragValueChangedHelper(this);
    m_dragHelper->SetRange(0.0, 359.0);
    m_dragHelper->SetLoop(true);
    //connect(m_dragHelper, &IDragValueChangedHelper::sigValueChanged, this, [&](double value){
    //    setValue(value);
    //});

    //connect(this, SIGNAL(valueChanged(double)), this, SLOT(SlotValueChanged(double)));

    //connect(this, &QDoubleSpinBox::editingFinished, this, &QDoubleSpinBox::clearFocus);
}
AngleDoubleSpinBox::~AngleDoubleSpinBox()
{

}


void AngleDoubleSpinBox::keyPressEvent(QKeyEvent *event)
{
    double min, max, val;
    min = minimum();
    max = maximum();
    val = value();
    if (event->key() == Qt::Key_Down&& qAbs(val - min) < 1)
    {
        setValue(max);
    }
    else if (event->key() == Qt::Key_Up && qAbs(val - max) < 1)
    {
        setValue(min);
    }
    FSDoubleSpinBox::keyPressEvent(event);
}


//////////////////////////////////////////////////////////////////////////////////////////////////
FSDenoiseOption::FSDenoiseOption(QWidget *pParent)
    :IFSOptionView(TypeAudioDenoise,pParent)
    , m_LastDenoiseLevel(0)
{
    CreateUI();
    connectSignals();
    RetranslateUi();
}

FSDenoiseOption::~FSDenoiseOption()
{

}

void FSDenoiseOption::UpdataParma(IFSEditHelper *pHelper)
{
    Q_ASSERT(pHelper);
    if (pHelper)
    {
        int level=pHelper->GetTimeLine()->GetDenoiseLevel();
        m_LastDenoiseLevel = pHelper->GetLastDenoiseLevel();
        pHelper->GetDefaultDenoiseLevel(m_DefaultDenoiseLevel);
        m_pTitleWidget->SetResetable(level != m_DefaultDenoiseLevel);
        BlockSignals(true);
        m_pCheckBox->setChecked(level > 0);
        EnableWidget(level>0 ? true : false);
        if (level > 0)
        {
            level = qBound(1, level, 3);
            m_pButtonGroup->button(level)->setChecked(true);
        }
        else
        {
            ClearGroupButtonCheckState();
        }
        BlockSignals(false);
    }
}

void FSDenoiseOption::RetranslateUi()
{
    m_pEnableLabel->setText(tr("Enable"));
    m_pLevelLable->setText(tr("Value"));
    m_pLowRadioBtn->setText(tr("Low"));
    m_pMidRadioBtn->setText(tr("Mid"));
    m_pHighRadioBtn->setText(tr("High"));
}

void FSDenoiseOption::OnReset()
{
    BlockSignals(true);
    
    if (m_DefaultDenoiseLevel > 0)
    {        
        m_pCheckBox->setChecked(true);       
        m_pButtonGroup->button(m_DefaultDenoiseLevel)->setChecked(true);        
    }
    else
    {   
        m_LastDenoiseLevel = GetDenoiseLevel();
        m_pCheckBox->setChecked(false);
        EnableWidget(false);
        ClearGroupButtonCheckState();
    }
    BlockSignals(false);
}

bool FSDenoiseOption::IsDenoiseEnable()
{
    return m_pCheckBox->isChecked();
}

FSDenoiseOption::DeNoiseLevel FSDenoiseOption::GetDenoiseLevel()
{
    return (DeNoiseLevel) m_pButtonGroup->checkedId();
}

int FSDenoiseOption::GetLastDenoiseLevel()
{
    return m_LastDenoiseLevel;
}

void FSDenoiseOption::ConnectDenoiseEnableStateChanged(bool bConnect)
{
    if (bConnect)
    {
        connect(m_pCheckBox, &QCheckBox::stateChanged, this, &FSDenoiseOption::OnDenoiseEnableStateChanged);
    }
    else
    {
        disconnect(m_pCheckBox, &QCheckBox::stateChanged, this, &FSDenoiseOption::OnDenoiseEnableStateChanged);
    }
}

void FSDenoiseOption::OnDenoiseEnableStateChanged(int checkState)
{  
    m_pButtonGroup->blockSignals(true);
    EnableWidget(checkState>0 ? true : false);
    if (checkState > 0)
    {
        m_pButtonGroup->button(m_LastDenoiseLevel>0?m_LastDenoiseLevel:DeNoise_MID_LEVEL)->setChecked(true);
    }
    else
    {
        m_LastDenoiseLevel = GetDenoiseLevel();
        ClearGroupButtonCheckState();
    }
    m_pButtonGroup->blockSignals(false);
    m_pTitleWidget->SetResetable(GetDenoiseLevel() != m_DefaultDenoiseLevel);
    emit ParmaChanged();
}

void FSDenoiseOption::ConnectDenoiseLevelChanged(bool bConnect)
{
    if (bConnect)
    {
        connect(m_pButtonGroup, SIGNAL(buttonToggled(int, bool)), this, SLOT(OnDenoiseLevelChanged(int, bool)));
    }
    else
    {
        disconnect(m_pButtonGroup, SIGNAL(buttonToggled(int, bool)), this, SLOT(OnDenoiseLevelChanged(int, bool)));
    }
}

void FSDenoiseOption::OnDenoiseLevelChanged(int id, bool bChecked)
{
    if (bChecked)
    {
        m_pTitleWidget->SetResetable(GetDenoiseLevel() != m_DefaultDenoiseLevel);
        emit ParmaChanged();
    }
}

void FSDenoiseOption::CreateUI()
{
    m_pButtonGroup    = new QButtonGroup(this);
    m_pLevelLable     = new FSLabel(this);
    m_pEnableLabel    = new FSLabel(this);
    m_pCheckBox       = new QCheckBox(this);
    m_pLowRadioBtn    = new QRadioButton(this);
    m_pMidRadioBtn    = new QRadioButton(this);
    m_pHighRadioBtn   = new QRadioButton(this);

    m_pButtonGroup->addButton(m_pLowRadioBtn, DeNoise_LOW_LEVEL);
    m_pButtonGroup->addButton(m_pMidRadioBtn, DeNoise_MID_LEVEL);
    m_pButtonGroup->addButton(m_pHighRadioBtn, DeNoise_HIGH_LEVEL);

    QVBoxLayout *pVL = new QVBoxLayout(m_pOptionWidget);

    QHBoxLayout *pHL1 = new QHBoxLayout();
    QHBoxLayout *pHL2 = new QHBoxLayout();

    QVBoxLayout *pVL1 = new QVBoxLayout();
    QVBoxLayout *pVL2 = new QVBoxLayout();

    int h = 20, w = 70, space = 10;

    pHL1->addWidget(m_pEnableLabel);
    pHL1->addWidget(m_pCheckBox);
    pHL1->setSpacing(space);

    pVL1->addWidget(m_pLevelLable);
    pVL1->addStretch();

    pVL2->addWidget(m_pLowRadioBtn);
    pVL2->addWidget(m_pMidRadioBtn);
    pVL2->addWidget(m_pHighRadioBtn);

    pHL2->addLayout(pVL1);
    pHL2->addLayout(pVL2);
    pHL2->setSpacing(space);
    
    pVL->addLayout(pHL1);
    pVL->addSpacing(space);
    pVL->addLayout(pHL2);
    pVL->setContentsMargins(14, 10, 21, 10);

    m_pLevelLable->setFixedHeight(h);
    m_pEnableLabel->setFixedHeight(h);
    m_pLowRadioBtn->setFixedHeight(h);
    m_pMidRadioBtn->setFixedHeight(h);
    m_pHighRadioBtn->setFixedHeight(h);

    m_pCheckBox->setChecked(true);
    m_pMidRadioBtn->setChecked(true);

    m_pCheckBox->setFocusPolicy(Qt::NoFocus);
    m_pLevelLable->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    m_pEnableLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight);

    m_pLevelLable->setObjectName("FSDenoiseOption_m_pLevelLable");
    m_pEnableLabel->setObjectName("FSDenoiseOption_m_pEnableLabel");
}

void FSDenoiseOption::connectSignals()
{
    ConnectDenoiseEnableStateChanged(true);
    ConnectDenoiseLevelChanged(true);
}


void FSDenoiseOption::ClearGroupButtonCheckState()
{
    m_pButtonGroup->setExclusive(false);
    m_pLowRadioBtn->setChecked(false);
    m_pMidRadioBtn->setChecked(false);
    m_pHighRadioBtn->setChecked(false);
    m_pButtonGroup->setExclusive(true);
}

void FSDenoiseOption::EnableWidget(bool bEnable)
{
    m_pLowRadioBtn->setEnabled(bEnable);
    m_pMidRadioBtn->setEnabled(bEnable);
    m_pHighRadioBtn->setEnabled(bEnable);
}

void FSDenoiseOption::BlockSignals(bool bBlock)
{
    m_pCheckBox->blockSignals(bBlock);
    m_pButtonGroup->blockSignals(bBlock);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
FSHotKetTipOption::FSHotKetTipOption(QWidget *pParent)
    :IFSOptionView(OptionType::TypeHotKeyTip, pParent)
{
    CreateUI();
    RetranslateUi();
    ConnectSignals();
}

FSHotKetTipOption::~FSHotKetTipOption()
{

}

void FSHotKetTipOption::UpdataParma(IFSEditHelper *pHelper)
{
    Q_ASSERT(pHelper);
    if (pHelper)
    {
        BlockSignals(true);
        InitDefaultParma(pHelper);
        UpdataUI(pHelper);
        UpadaResetUI();
        m_pCheckBoxEnable->setChecked((static_cast<FSVideoPicEditHelper *>(pHelper))->IsHotKeyEnable());
        EnableWidget(m_pCheckBoxEnable->isChecked());
        BlockSignals(false);
        
    }
}

QString FSHotKetTipOption::GetHotKeySize()
{
    return m_pButtonGroup->checkedButton()->text();
}

bool FSHotKetTipOption::HotKeyEnable()
{
    return m_pCheckBoxEnable->isChecked();
}

QString FSHotKetTipOption::GetHotKeyPosition()
{
    return m_pComboBoxPosition->currentText();
}

QString FSHotKetTipOption::GetHotKeyStyle()
{
    return m_pComboBoxStyle->currentText();
}

QColor FSHotKetTipOption::GetHotKeyColor()
{
    return m_pColorSelecter->GetCurrentColor();
}

int FSHotKetTipOption::GetHotKeyOpacity()
{
    return Opacity2Alpha(m_pSpinBoxOpacity->value());
}

int FSHotKetTipOption::GetFliterType()
{
    return m_pButtonHotKeyFliter->isChecked()?NLEKeyFrameType::HOT_KEY:(NORMAL|HOT_KEY|MARKER);
}

void FSHotKetTipOption::RetranslateUi()
{
    m_pLabelEnable->setText(tr("Enable"));
    m_pLabelSize->setText(tr("Size"));
    m_pLabelPosition->setText(tr("Position"));
    m_pLabelStyle->setText(tr("Style"));
    m_pLabelColor->setText(tr("Color"));
    m_pLabelOpacity->setText(tr("Opacity"));
    m_pButtonHotKeyFliter->setText(tr("HotKeys"));
    m_pButtonAllHotKey->setText(tr("All keys"));

    m_pLabelFliter->setText(tr("Fliter"));

    m_pButtonSmall->setText(tr("S"));
    m_pButtonMid->setText(tr("M"));
    m_pButtonLarge->setText(tr("L"));
    tr("Top");
    tr("Bottom");
    tr("Default");
    tr("Hollow-Out");
    tr("Draft");
}

void FSHotKetTipOption::OnReset()
{
    BlockSignals(true);
    m_pCheckBoxEnable->setChecked(m_bDefaultEnable);
    m_pComboBoxPosition->setCurrentText(m_DefaultPoint);
    m_pComboBoxStyle->setCurrentText(m_DefaultStyle);
    m_pColorSelecter->SetCurrentColor(m_DefaultColor);
    m_pSpinBoxOpacity->setValue(m_nDefaultOpacity);
    m_pButtonLarge->setChecked(m_nDefaultSize == m_pButtonLarge->text());
    m_pButtonMid->setChecked(m_nDefaultSize == m_pButtonMid->text());
    m_pButtonSmall->setChecked(m_nDefaultSize == m_pButtonSmall->text());
    UpadaResetUI();
    EnableWidget(m_pCheckBoxEnable->isChecked());
    BlockSignals(false);
    emit ParmaChanged();
}

void FSHotKetTipOption::CreateUI()
{
    m_pLabelEnable = new FSLabel(this);
    m_pLabelSize = new FSLabel(this);
    m_pLabelFliter = new FSLabel(this);
    m_pLabelPosition = new FSLabel(this);
    m_pLabelStyle = new FSLabel(this);
    m_pLabelColor = new FSLabel(this);
    m_pLabelOpacity = new FSLabel(this);

    m_pCheckBoxEnable = new QCheckBox(this);

    m_pButtonGroup = new QButtonGroup(this);
    m_pButtonSmall = new QRadioButton(this);
    m_pButtonMid = new QRadioButton(this);
    m_pButtonLarge = new QRadioButton(this);

    m_pButtonGroup1       = new QButtonGroup(this);
    m_pButtonHotKeyFliter = new QRadioButton(this);
    m_pButtonAllHotKey    = new QRadioButton(this);

    m_pComboBoxPosition = new FSComboBox(this);
    m_pComboBoxStyle = new FSComboBox(this);
    m_pColorSelecter = new ColorSelecterComboBox(this);
    m_pSpinBoxOpacity = new FSDoubleSpinBox(this);

    FSLabel *pLabel1 = new FSLabel(this);
    FSLabel *pLabel2 = new FSLabel(this);
    FSLabel *pLabel3 = new FSLabel(this);
    FSLabel *pLabel4 = new FSLabel(this);
    FSLabel *pLabel5 = new FSLabel(this);
    FSLabel *pLabel6 = new FSLabel(this);

    m_pButtonResetSize     = new FSButton(pLabel1);
    m_pButtonResetPosition = new FSButton(pLabel2);
    m_pButtonResetStyle    = new FSButton(pLabel3);
    m_pButtonResetColor    = new FSButton(pLabel4);
    m_pButtonResetOpacity  = new FSButton(pLabel5);
    m_pButtonResetFliter   = new FSButton(pLabel6);

    FSLabel *pLabel = new FSLabel(this);

    m_pLabelEnable->setAlignment(Qt::AlignVCenter|Qt::AlignRight);
    m_pLabelSize->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    m_pLabelPosition->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    m_pLabelStyle->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    m_pLabelColor->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    m_pLabelOpacity->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    m_pLabelFliter->setAlignment(Qt::AlignVCenter | Qt::AlignRight);

    int h = 20, w1 = 70, w2 = 100, space = 10;
    int btnW = 14;

    m_pLabelEnable->setFixedHeight(h);
    m_pLabelSize->setFixedHeight(h);
    m_pLabelFliter->setFixedHeight(h);
    m_pLabelPosition->setFixedHeight(h);
    m_pLabelStyle->setFixedHeight(h);
    m_pLabelColor->setFixedHeight(h);
    m_pLabelOpacity->setFixedHeight(h);

    m_pButtonSmall->setFixedHeight(h);
    m_pButtonMid->setFixedHeight(h);
    m_pButtonLarge->setFixedHeight(h);
    m_pButtonHotKeyFliter->setFixedHeight(h);
    m_pButtonAllHotKey->setFixedHeight(h);

    m_pCheckBoxEnable->setFixedHeight(h);
    m_pComboBoxPosition->setFixedSize(w2,h);
    m_pComboBoxStyle->setFixedSize(w2,h);
    m_pColorSelecter->setFixedSize(w1,h);
    m_pSpinBoxOpacity->setFixedSize(w1,h);

    m_pButtonResetSize->setFixedSize(btnW, btnW);
    m_pButtonResetPosition->setFixedSize(btnW, btnW);
    m_pButtonResetStyle->setFixedSize(btnW, btnW);
    m_pButtonResetColor->setFixedSize(btnW, btnW);
    m_pButtonResetOpacity->setFixedSize(btnW, btnW);
    m_pButtonResetFliter->setFixedSize(btnW, btnW);
    pLabel->setFixedSize(btnW, btnW);
    pLabel1->setFixedSize(btnW, btnW);
    pLabel2->setFixedSize(btnW, btnW);
    pLabel3->setFixedSize(btnW, btnW);
    pLabel4->setFixedSize(btnW, btnW);
    pLabel5->setFixedSize(btnW, btnW);
    pLabel6->setFixedSize(btnW, btnW);

    QHBoxLayout *pHL  = new QHBoxLayout(m_pOptionWidget);
    QVBoxLayout *pVL = new QVBoxLayout();
    QVBoxLayout *pVL1 = new QVBoxLayout();
    QVBoxLayout *pVL2 = new QVBoxLayout();
    

    QHBoxLayout *pHL1 = new QHBoxLayout();
    QHBoxLayout *pHL3 = new QHBoxLayout();

    pHL1->addWidget(m_pButtonSmall);
    pHL1->addWidget(m_pButtonMid);
    pHL1->addWidget(m_pButtonLarge);

    pHL3->addWidget(m_pButtonHotKeyFliter);
    pHL3->addWidget(m_pButtonAllHotKey);

    pVL->addWidget(m_pLabelEnable);
    pVL->addWidget(m_pLabelSize);
    pVL->addWidget(m_pLabelFliter);
    pVL->addWidget(m_pLabelPosition);
    pVL->addWidget(m_pLabelStyle);
    pVL->addWidget(m_pLabelColor);
    pVL->addWidget(m_pLabelOpacity);
    pVL->setSpacing(space);
    pVL->setMargin(0);

    pVL1->addWidget(m_pCheckBoxEnable);
    pVL1->addLayout(pHL1);
    pVL1->addLayout(pHL3);
    pVL1->addWidget(m_pComboBoxPosition);
    pVL1->addWidget(m_pComboBoxStyle);
    pVL1->addWidget(m_pColorSelecter);
    pVL1->addWidget(m_pSpinBoxOpacity);
    pVL1->setSpacing(space);
    pVL1->setMargin(0);

    pVL2->addWidget(pLabel);
    pVL2->addWidget(pLabel1);
    pVL2->addWidget(pLabel6);
    pVL2->addWidget(pLabel2);
    pVL2->addWidget(pLabel3);
    pVL2->addWidget(pLabel4);
    pVL2->addWidget(pLabel5);
    pVL2->setSpacing(space);
    pVL2->setMargin(0);

    pHL->addStretch(1);
    pHL->addLayout(pVL,1);
    pHL->addSpacing(space);
    pHL->addLayout(pVL1,1);
    pHL->addStretch(1);
    pHL->addLayout(pVL2);
    pHL->setSpacing(0);   
    pHL->setContentsMargins(14, 10, 21, 12);

    m_pSpinBoxOpacity->setButtonSymbols(QAbstractSpinBox::NoButtons);
    m_pSpinBoxOpacity->setDecimals(0);
    m_pSpinBoxOpacity->setRange(0, 100);
    m_pSpinBoxOpacity->setSuffix("%");

    m_pButtonGroup->addButton(m_pButtonSmall, 0);
    m_pButtonGroup->addButton(m_pButtonMid, 1);
    m_pButtonGroup->addButton(m_pButtonLarge, 2);

    m_pButtonGroup1->addButton(m_pButtonHotKeyFliter, 0);
    m_pButtonGroup1->addButton(m_pButtonAllHotKey, 1);

    m_pLabelEnable->setObjectName("FSDenoiseOption_m_pLevelLable");
    m_pLabelSize->setObjectName("FSDenoiseOption_m_pLevelLable");
    m_pLabelPosition->setObjectName("FSDenoiseOption_m_pLevelLable");
    m_pLabelStyle->setObjectName("FSDenoiseOption_m_pLevelLable");
    m_pLabelColor->setObjectName("FSDenoiseOption_m_pLevelLable");
    m_pLabelOpacity->setObjectName("FSDenoiseOption_m_pLevelLable");
    m_pLabelFliter->setObjectName("FSDenoiseOption_m_pLevelLable");
    m_pComboBoxPosition->setObjectName("FSTextBaseOption_m_pComboBox");
    m_pComboBoxStyle->setObjectName("FSTextBaseOption_m_pComboBox");
    m_pSpinBoxOpacity->setObjectName("FSVisualPorpertyView_pDoubleSpinBox");
    
    m_pButtonResetSize->setObjectName("FSPropertyTitle_m_pResetBtn");
    m_pButtonResetFliter->setObjectName("FSPropertyTitle_m_pResetBtn");
    m_pButtonResetPosition->setObjectName("FSPropertyTitle_m_pResetBtn");
    m_pButtonResetStyle->setObjectName("FSPropertyTitle_m_pResetBtn");
    m_pButtonResetColor->setObjectName("FSPropertyTitle_m_pResetBtn");
    m_pButtonResetOpacity->setObjectName("FSPropertyTitle_m_pResetBtn");
    EnableWidget(false);
    m_pCheckBoxEnable->setFocusPolicy(Qt::NoFocus);
}

void FSHotKetTipOption::ConnectSignals()
{
    connect(m_pCheckBoxEnable, &QCheckBox::stateChanged, this, &FSHotKetTipOption::OnCheckBoxStateChanged);
    connect(m_pButtonGroup, SIGNAL(buttonToggled(int, bool)), this, SLOT(OnHotKeySizeChanged(int,bool)));
    connect(m_pButtonGroup1, SIGNAL(buttonToggled(int, bool)), this, SLOT(OnHotKeyFliterChanged(int, bool)));
    connect(m_pComboBoxPosition, SIGNAL(currentTextChanged(QString)), this, SLOT(OnPositionChanged(QString)));
    connect(m_pComboBoxStyle, SIGNAL(currentTextChanged(QString)), this, SLOT(OnStyleChanged(QString)));
    connect(m_pSpinBoxOpacity, SIGNAL(valueChanged(double)), this, SLOT(OnOpacityChanged(double)));
    connect(m_pColorSelecter, &ColorSelecterComboBox::CurrentColorChanged, this, &FSHotKetTipOption::OnColorChanged);
    connect(m_pButtonResetColor, &FSButton::clicked, this, [&]{
        BlockSignals(true);
        m_pColorSelecter->SetCurrentColor(m_DefaultColor);
        m_pButtonResetColor->setVisible(IsColorResetable());
        m_pTitleWidget->SetResetable(IsResetable());
        BlockSignals(false);
        emit ParmaChanged();
    });
    connect(m_pButtonResetSize, &FSButton::clicked, this, [&]{
        BlockSignals(true);
        m_pButtonLarge->setChecked(m_nDefaultSize == m_pButtonLarge->text());
        m_pButtonMid->setChecked(m_nDefaultSize == m_pButtonMid->text());
        m_pButtonSmall->setChecked(m_nDefaultSize == m_pButtonSmall->text());
        m_pButtonResetSize->setVisible(IsSizeResetable());
        BlockSignals(false);
        m_pTitleWidget->SetResetable(IsResetable());
        emit ParmaChanged();
    });
    connect(m_pButtonResetStyle, &FSButton::clicked, this, [&]{
        BlockSignals(true);
        m_pComboBoxStyle->setCurrentText(m_DefaultStyle);
        m_pTitleWidget->SetResetable(IsResetable());
        m_pButtonResetStyle->setVisible(IsStyleResetable());
        BlockSignals(false);
        emit ParmaChanged();
    });
    connect(m_pButtonResetPosition, &FSButton::clicked, this, [&]{
        BlockSignals(true);
        m_pComboBoxPosition->setCurrentText(m_DefaultPoint);
        m_pTitleWidget->SetResetable(IsResetable());
        m_pButtonResetPosition->setVisible(IsPositionResetable());
        BlockSignals(false);
        emit ParmaChanged();
    });
    connect(m_pButtonResetOpacity, &FSButton::clicked, this, [&]{
        BlockSignals(true);
        m_pSpinBoxOpacity->setValue(Alpha2Opacity(m_nDefaultOpacity));
        m_pTitleWidget->SetResetable(IsResetable());
        m_pButtonResetOpacity->setVisible(IsOpacityResetable());
        BlockSignals(false);
        emit ParmaChanged();
    });
    connect(m_pButtonResetFliter, &FSButton::clicked, this, [&]{
        BlockSignals(true);
        
        m_pButtonAllHotKey->setChecked((m_nFliter&NLEKeyFrameType::NORMAL) && (m_nFliter&&NLEKeyFrameType::MARKER) && (m_nFliter&&NLEKeyFrameType::HOT_KEY));
        m_pButtonHotKeyFliter->setChecked(!m_pButtonAllHotKey->isChecked());
        m_pTitleWidget->SetResetable(IsResetable());
        m_pButtonResetFliter->setVisible(false);
        BlockSignals(false);
        emit ParmaChanged();
    });
}

void FSHotKetTipOption::OnCheckBoxStateChanged(int state)
{
    QPoint pt = mapToGlobal(m_pCheckBoxEnable->geometry().bottomLeft()) + QPoint(-20,50);
    EnableWidget(state > 0);
    emit ParmaChanged();
}

void FSHotKetTipOption::EnableWidget(bool bEnable)
{
    m_pButtonSmall->setEnabled(bEnable);
    m_pButtonMid->setEnabled(bEnable);
    m_pButtonLarge->setEnabled(bEnable);
    m_pButtonResetFliter->setEnabled(bEnable);
    m_pButtonHotKeyFliter->setEnabled(bEnable);
    m_pButtonAllHotKey->setEnabled(bEnable);
    m_pComboBoxPosition->setEnabled(bEnable);
    m_pComboBoxStyle->setEnabled(bEnable);
    m_pColorSelecter->setEnabled(bEnable);
    m_pSpinBoxOpacity->setEnabled(bEnable);
    m_pButtonResetColor->setEnabled(bEnable);
    m_pButtonResetPosition->setEnabled(bEnable);
    m_pButtonResetStyle->setEnabled(bEnable);
    m_pButtonResetSize->setEnabled(bEnable);
    m_pButtonResetOpacity->setEnabled(bEnable);
}

void FSHotKetTipOption::BlockSignals(bool bBlock)
{
    m_pCheckBoxEnable->blockSignals(bBlock);
    m_pButtonGroup->blockSignals(bBlock);
    m_pButtonGroup1->blockSignals(bBlock);
    m_pComboBoxPosition->blockSignals(bBlock);
    m_pComboBoxStyle->blockSignals(bBlock);
    m_pColorSelecter->blockSignals(bBlock);
    m_pSpinBoxOpacity->blockSignals(bBlock);
}

void FSHotKetTipOption::InitDefaultParma(IFSEditHelper *pHelper)
{
    Q_ASSERT(pHelper);
    pHelper->GetDefaultHotKeyEnable(m_bDefaultEnable);
    pHelper->GetDefaultHotKeySize(m_nDefaultSize);
    pHelper->GetDefaultHotKeyPosition(m_DefaultPoint);
    pHelper->GetDefaultHotKeyStyle(m_DefaultStyle);
    pHelper->GetDefaultHotKeyColor(m_DefaultColor);
    pHelper->GetDefaultHotKeyOpacity(m_nDefaultOpacity);
    pHelper->GetDefaultHotKeyFliter(m_nFliter);
}

void FSHotKetTipOption::UpdataUI(IFSEditHelper *pHelper)
{
    QString cur;
    QStringList list;
    Q_ASSERT(pHelper);
    FSVideoPicEditHelper *pTemp=static_cast<FSVideoPicEditHelper *>(pHelper);
    pTemp->GetHotKeySize(list, cur);
    if (!list.isEmpty())
    {
        m_pButtonGroup->button(qBound(0,list.indexOf(cur, 0),2))->setChecked(true);
        list.clear();  
        cur.clear();
    }
    pTemp->GetHotKeyPosition(list, cur);
    if (!list.isEmpty())
    {
        m_pComboBoxPosition->clear();
        for (auto item:list)
        {
            m_pComboBoxPosition->addItem(tr(item.toLocal8Bit().data()));
        }
        m_pComboBoxPosition->setCurrentText(cur);
        list.clear();
        cur.clear();
    }
    pTemp->GetHotKeyStyle(list, cur);
    if (!list.isEmpty())
    {
        m_pComboBoxStyle->clear();
        for (auto item : list)
        {
            m_pComboBoxStyle->addItem(tr(item.toLocal8Bit().data()));
        }
        m_pComboBoxStyle->setCurrentText(cur);
        list.clear();
        cur.clear();
    }
    m_pColorSelecter->SetCurrentColor(pTemp->GetHotKeyColor());
    m_pSpinBoxOpacity->setValue(Alpha2Opacity(pTemp->GetHotKeyOpacity()));
    int type = pTemp->GetHotKeyFliterType();
    QString str = pTemp->GetHotKeyText().trimmed();
    
    
    m_pButtonAllHotKey->setChecked((type&NLEKeyFrameType::NORMAL)&&(type&&NLEKeyFrameType::MARKER)&&(type&&NLEKeyFrameType::HOT_KEY));
    m_pButtonHotKeyFliter->setChecked(!m_pButtonAllHotKey->isChecked());
    if (str.isEmpty())
    {
        ushort shortArray[3] = { 0xE842, 0xE868, 0xE82B };

        pTemp->SetHotKeyText(QString::fromUtf16(shortArray, 3));
        pTemp->Updata();
    }
}

void FSHotKetTipOption::UpadaResetUI()
{
    m_pButtonResetSize->setVisible(IsSizeResetable());
    m_pButtonResetPosition->setVisible(IsPositionResetable());
    m_pButtonResetColor->setVisible(IsColorResetable());
    m_pButtonResetStyle->setVisible(IsStyleResetable());
    m_pButtonResetOpacity->setVisible(IsOpacityResetable());
    m_pButtonResetFliter->setVisible(IsFliterResetable());
    m_pTitleWidget->SetResetable(IsResetable());
}

int FSHotKetTipOption::Alpha2Opacity(int alpha)
{
    return qRound(100.0*alpha / 255);
}

int FSHotKetTipOption::Opacity2Alpha(int Opacity)
{
    return qRound(Opacity*255.0 / 100.0);

}

bool FSHotKetTipOption::IsSizeResetable()
{
    return m_pButtonGroup->checkedButton()->text() != m_nDefaultSize;
}

bool FSHotKetTipOption::IsPositionResetable()
{
    return m_pComboBoxPosition->currentText() != m_DefaultPoint;
}

bool FSHotKetTipOption::IsColorResetable()
{
    return m_pColorSelecter->GetCurrentColor().name(QColor::HexRgb) != m_DefaultColor.name(QColor::HexRgb);
}

bool FSHotKetTipOption::IsStyleResetable()
{
    return m_pComboBoxStyle->currentText() != m_DefaultStyle;
}

bool FSHotKetTipOption::IsOpacityResetable()
{
    return Opacity2Alpha(m_pSpinBoxOpacity->value()) != m_nDefaultOpacity;
}

bool FSHotKetTipOption::IsFliterResetable()
{
    if (m_pButtonAllHotKey->isChecked())
    {
        return !((m_nFliter&NLEKeyFrameType::NORMAL) && (m_nFliter&NLEKeyFrameType::HOT_KEY) && (m_nFliter&NLEKeyFrameType::MARKER));
    }
    else{
        return !(m_nFliter&NLEKeyFrameType::HOT_KEY);
    }
}

bool FSHotKetTipOption::IsResetable()
{
    return IsOpacityResetable() || IsPositionResetable() || IsColorResetable() || IsSizeResetable() || IsStyleResetable();
}

void FSHotKetTipOption::OnColorChanged(QColor color)
{
    
    m_pButtonResetColor->setVisible(IsColorResetable());
    m_pTitleWidget->SetResetable(IsResetable());
    QTimer::singleShot(0, [&]{emit ParmaChanged(); });
}

void FSHotKetTipOption::OnHotKeySizeChanged(int id, bool bChecked)
{
    if (bChecked)
    {      
        m_pButtonResetSize->setVisible(IsSizeResetable());
        m_pTitleWidget->SetResetable(IsResetable());
        emit ParmaChanged();
    }
}


void FSHotKetTipOption::OnHotKeyFliterChanged(int id, bool bChecked)
{
    if (bChecked)
    {       
        m_pButtonResetFliter->setVisible(IsFliterResetable());
        m_pTitleWidget->SetResetable(IsResetable());
        emit ParmaChanged();
    }
}

void FSHotKetTipOption::OnStyleChanged(QString)
{
    m_pButtonResetStyle->setVisible(IsStyleResetable());
    m_pTitleWidget->SetResetable(IsResetable());
    emit ParmaChanged();
}

void FSHotKetTipOption::OnPositionChanged(QString)
{
    m_pButtonResetPosition->setVisible(IsPositionResetable());
    m_pTitleWidget->SetResetable(IsResetable());
    emit ParmaChanged();
}

void FSHotKetTipOption::OnOpacityChanged(double)
{
    m_pButtonResetOpacity->setVisible(IsOpacityResetable());
    m_pTitleWidget->SetResetable(IsResetable());
    emit ParmaChanged();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
FSSpeedOption::FSSpeedOption(QWidget *pParent)
    :IFSOptionView(IFSOptionView::TypeVideoAudioSpeed,pParent)
    , m_frameRate(25.0)
{
    CreateUI();
    RetranslateUi();
    ConnectSignals();
}

FSSpeedOption::~FSSpeedOption()
{

}

void FSSpeedOption::UpdataParma(IFSEditHelper *pHelper)
{
    if (pHelper)
    {
        float renderScale = pHelper->GetTimeLine()->GetRenderScale() * 100;
        int leng = pHelper->GetTimeLine()->GetPlayLen()*renderScale/100;
        m_frameRate = pHelper->GetTimeLine()->GetRenderFrameRate();
        m_pSpeedSpinBox->blockSignals(true);
        m_pSpeedSpinBox->setRange(1, pHelper->GetTimeLine()->GetPlayLen()*100);
        m_pSpeedSpinBox->setValue(1.0 / pHelper->GetTimeLine()->GetRenderScale() * 100);
        m_pLabelDurationText->setText(gFrame2String(pHelper->GetTimeLine()->GetPlayLen(), m_frameRate));
        m_pTitleWidget->SetResetable(qAbs(1 - renderScale / 100.0) > 1e-4);
        m_pSpeedSpinBox->blockSignals(false);
    }
}

float FSSpeedOption::GetSpeedScale()
{
    QString text = m_pSpeedSpinBox->text();
    return text.left(text.length() - 1).toFloat() / 100.0;
}

void FSSpeedOption::OnReset()
{
    m_pTitleWidget->SetResetable(false);
    m_pSpeedSpinBox->setValue(100);
}

void FSSpeedOption::RetranslateUi()
{
    m_pLabelSpeed->setText(tr("Speed"));
    m_pLabelDuration->setText(tr("Duration"));
}

void FSSpeedOption::CreateUI()
{
    m_pLabelSpeed        = new FSLabel(this);
    m_pLabelDuration     = new FSLabel(this);
    m_pSpeedSpinBox      = new FSDoubleSpinBox(this);
    m_pLabelDurationText = new FSLabel(this);

    QHBoxLayout *pHL  = new QHBoxLayout(m_pOptionWidget);
    QVBoxLayout *pVL  = new QVBoxLayout();
    QVBoxLayout *pVL1 = new QVBoxLayout();
   

    int h = 20,w=60,space=10;

    pVL->addWidget(m_pLabelSpeed);
    pVL->addWidget(m_pLabelDuration);
    pVL->setSpacing(space);

    pVL1->addWidget(m_pSpeedSpinBox);
    pVL1->addWidget(m_pLabelDurationText);
    pVL1->setSpacing(space);

    pHL->addLayout(pVL);
    pHL->addLayout(pVL1);
    pHL->setSpacing(space);
    pHL->setContentsMargins(14, 12, 21, 12);

    m_pLabelSpeed->setFixedHeight(h);
    m_pLabelDuration->setFixedHeight(h);
    m_pSpeedSpinBox->setFixedSize(w,h);
    m_pLabelDurationText->setFixedHeight(h);
    m_pLabelSpeed->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_pLabelDuration->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    m_pSpeedSpinBox->setDecimals(0);
    m_pSpeedSpinBox->setSuffix("%");
    m_pSpeedSpinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);




    m_pLabelSpeed->setObjectName("FSDenoiseOption_m_pLevelLable");
    m_pLabelDuration->setObjectName("FSDenoiseOption_m_pLevelLable");
    m_pSpeedSpinBox->setObjectName("FSVisualPorpertyView_pDoubleSpinBox");
    m_pLabelDurationText->setObjectName("FSDenoiseOption_m_pLevelLable");
}

void FSSpeedOption::ConnectSignals()
{
    connect(m_pSpeedSpinBox, SIGNAL(valueChanged(QString)), this, SLOT(OnSpinBoxValueChanged(QString)));
    connect(m_pSpeedSpinBox->GetHelper(), &IDragValueChangedHelper::sigEndValueChanged, this, [&](){
        emit speedChanged();
    });
    connect(m_pSpeedSpinBox, &FSDoubleSpinBox::editingFinished, this, [&](){
        emit speedChanged();
    });
}

void FSSpeedOption::setDurationText(QString text)
{
    m_pLabelDurationText->setText(text);
}


void FSSpeedOption::OnSpinBoxValueChanged(QString text)
{
    float leng = 1.0* m_pSpeedSpinBox->maximum() / text.left(text.length() - 1).toInt();
    m_pLabelDurationText->setText(gFrame2String(leng, m_frameRate));
    m_pTitleWidget->SetResetable(qAbs(1 - text.left(text.length() - 1).toInt() / 100.0) > 1e-4);   
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FSAudioSpeedOption::FSAudioSpeedOption(QWidget *pParent)
    :FSSpeedOption(pParent)
{
    SetOptionType(IFSOptionView::TypeAudioSpeed);
}

FSAudioSpeedOption::~FSAudioSpeedOption()
{

}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FSAnotationOption::FSAnotationOption(QWidget *pParent)
    :IFSOptionView(TypeAnotation,pParent)
{
    CreateUI();
    RetranslateUi();
    ConnectSignals();
}

FSAnotationOption::~FSAnotationOption()
{

}

void FSAnotationOption::UpdataParma(IFSEditHelper *pHelper)
{
    Q_ASSERT(pHelper);
    if (pHelper)
    {
        FSTextEditHelper *pTempHelper = static_cast<FSTextEditHelper *>(pHelper);
        NLECaptionObjData objData = pTempHelper->GetCaptionBaseObj();
        if (objData.ObjType == NLECaptionObjType::NLE_DOT_SHAPE || objData.ObjType == NLECaptionObjType::NLE_DOT_TIPTEXT)
        {
            BlockSignals(true);
            pTempHelper->GetDefaultTextBorderBgColor(m_defaultBorderColor);
            pTempHelper->GetDefaultTextBgColor(m_defaultShapeColor);
            pTempHelper->GetDefaultTextBorderWidht(m_defaultBorderWidht);
            m_currentShapeColor = pTempHelper->GetBackGroundColor();
            m_currentBorderColor = pTempHelper->GetBorderBackGroundColor();
            int borderW = pTempHelper->GetBorderWidth();
            m_pSelecterShapeColor->SetCurrentColor(QColor(m_currentShapeColor.name(QColor::HexRgb)));
            m_pSelecterBorderColor->SetCurrentColor(QColor(m_currentBorderColor.name(QColor::HexRgb)));
            m_pSpinBoxBorder->setValue(borderW);
            m_pSpinBoxBorderOpacity->setValue(Alpha2Opacity(m_currentBorderColor.alpha()));
            m_pSpinBoxShapeOpacity->setValue(Alpha2Opacity(m_currentShapeColor.alpha()));
            m_pButtonShapeOpacityReset->setVisible(IsShapeResetable());
            m_pButtonBorderReset->setVisible(IsBorderWidthResetable());
            m_pButtonBorderOpacityReset->setVisible(IsBorderShapeResetable());
            m_pTitleWidget->SetResetable(IsResetable());
            BlockSignals(false);
        }
    }
}

QColor FSAnotationOption::GetShapeColor()
{
    return m_currentShapeColor;
}

QColor FSAnotationOption::GetBoderShapeColor()
{
    return m_currentBorderColor;
}

int FSAnotationOption::GetBorderWidth()
{
    return m_pSpinBoxBorder->value();
}

bool FSAnotationOption::IsShapeColorChanged()
{
    return m_bChangeFlag & 0x01;
}

bool FSAnotationOption::IsBorderColorChanged()
{
    return m_bChangeFlag & 0x04;
}

bool FSAnotationOption::IsBorderWidthChanged()
{
    return m_bChangeFlag & 0x02;
}

void FSAnotationOption::ResetChangeFlag()
{
    m_bChangeFlag = 0;
}

void FSAnotationOption::OnReset()
{
    BlockSignals(true);
    if (m_currentShapeColor != m_defaultShapeColor)
        m_bChangeFlag = m_bChangeFlag | 0x01;
    m_currentShapeColor = m_defaultShapeColor;
    m_pSelecterShapeColor->SetCurrentColor(QColor(m_defaultShapeColor.name(QColor::HexRgb)));
    m_pSpinBoxShapeOpacity->setValue(Alpha2Opacity(m_defaultShapeColor.alpha()));

    if (m_currentBorderColor != m_defaultBorderColor)
        m_bChangeFlag = m_bChangeFlag | 0x04;
    m_currentBorderColor = m_defaultBorderColor;
    m_pSelecterBorderColor->SetCurrentColor(QColor(m_defaultBorderColor.name(QColor::HexRgb)));
    m_pSpinBoxBorderOpacity->setValue(Alpha2Opacity(m_defaultBorderColor.alpha()));   

    if (GetBorderWidth() != m_defaultBorderWidht)
        m_bChangeFlag = m_bChangeFlag | 0x02;
    m_pSpinBoxBorder->setValue(m_defaultBorderWidht);
    m_pButtonBorderReset->setVisible(false);
    m_pButtonShapeOpacityReset->setVisible(false);
    m_pTitleWidget->SetResetable(false);
    m_pButtonBorderOpacityReset->setVisible(false);
    BlockSignals(false);
    emit ParmaChanged();
}

void FSAnotationOption::RetranslateUi()
{
    m_pLabelShapeOpacity->setText(tr("Opacity"));
    m_pLabelBorderOpacity->setText(tr("Opacity"));
    m_pLabelBorder->setText(tr("Border"));
    int w = 63;
    int w1 = GetRetranslateUiTextLenght(m_pLabelShapeOpacity->font(), m_pLabelShapeOpacity->text(), w);
    int w2 = GetRetranslateUiTextLenght(m_pLabelBorderOpacity->font(), m_pLabelBorderOpacity->text(), w);
    int w3 = GetRetranslateUiTextLenght(m_pLabelBorder->font(), m_pLabelBorder->text(), w);
    w = qMax(w1, w);
    w = qMax(w2, w);
    w = qMax(w3, w);
    m_pLabelShapeOpacity->setFixedSize(w, 20);
    m_pLabelBorderOpacity->setFixedSize(w, 20);
    m_pLabelBorder->setFixedSize(w, 20);
}

void FSAnotationOption::CreateUI()
{
    int h = 20, w = 60, space = 10,L=14,R=21,T=14,B=12,btnW=14;

    QLabel *pLine = new QLabel(m_pOptionWidget);
    QLabel *pLabel  = new QLabel(m_pOptionWidget);
    QLabel *pLabel1 = new QLabel(m_pOptionWidget);
    QLabel *pLabel2 = new QLabel(m_pOptionWidget);
    QLabel *pLabel3 = new QLabel(m_pOptionWidget);

    m_pLabelShapeOpacity  = new FSLabel(m_pOptionWidget);
    m_pLabelBorder        = new FSLabel(m_pOptionWidget);
    m_pLabelBorderOpacity = new FSLabel(m_pOptionWidget);

    m_pButtonShapeOpacityReset  = new FSButton(pLabel1);
    m_pButtonBorderReset        = new FSButton(pLabel2);
    m_pButtonBorderOpacityReset = new FSButton(pLabel3);

    m_pSpinBoxShapeOpacity  = new FSDoubleSpinBox(m_pOptionWidget);
    m_pSelecterShapeColor   = new ColorSelecterComboBox(m_pOptionWidget);
    m_pSpinBoxBorder        = new FSDoubleSpinBox(m_pOptionWidget);

    m_pSpinBoxBorderOpacity = new FSDoubleSpinBox(m_pOptionWidget);
    m_pSelecterBorderColor  = new ColorSelecterComboBox(m_pOptionWidget);

    QVBoxLayout *pVL  = new QVBoxLayout(m_pOptionWidget);
    QHBoxLayout *pHL0  = new QHBoxLayout();
    QHBoxLayout *pHL00 = new QHBoxLayout();
    QHBoxLayout *pHL000 = new QHBoxLayout();
    QHBoxLayout *pHL1 = new QHBoxLayout();
    QHBoxLayout *pHL11 = new QHBoxLayout();
    QHBoxLayout *pHL111 = new QHBoxLayout();
    QHBoxLayout *pHL2 = new QHBoxLayout();
    QHBoxLayout *pHL22 = new QHBoxLayout();
    QHBoxLayout *pHL222 = new QHBoxLayout();


    
    pLine->setFixedHeight(1);

    pHL00->addStretch();
    pHL00->addWidget(m_pLabelShapeOpacity);
    pHL00->setMargin(0);
    
    pHL000->addWidget(m_pSpinBoxShapeOpacity);
    pHL000->addWidget(m_pSelecterShapeColor);
    pHL000->addStretch();
    pHL000->addWidget(pLabel1);
    pHL000->setSpacing(space);
    pHL000->setMargin(0);

    pHL0->addLayout(pHL00);
    pHL0->addLayout(pHL000);   
    pHL0->setSpacing(space);
    pHL0->setContentsMargins(L, 0, R, 0);

    pHL11->addStretch();
    pHL11->addWidget(m_pLabelBorder);
    pHL11->setMargin(0);
    
    pHL111->addWidget(m_pSpinBoxBorder);
    pHL111->addWidget(pLabel);
    pHL111->addStretch();
    pHL111->addWidget(pLabel2);
    pHL111->setSpacing(space);
    pHL111->setMargin(0);

    pHL1->addLayout(pHL11);
    pHL1->addLayout(pHL111);
    pHL1->setSpacing(space);
    pHL1->setContentsMargins(L, 0, R, 0);

    pHL22->addStretch();
    pHL22->addWidget(m_pLabelBorderOpacity);
    pHL22->setMargin(0);

    
    pHL222->addWidget(m_pSpinBoxBorderOpacity);
    pHL222->addWidget(m_pSelecterBorderColor);
    pHL222->addStretch();
    pHL222->addWidget(pLabel3);
    pHL222->setSpacing(space);
    pHL222->setMargin(0);

    pHL2->addLayout(pHL22);
    pHL2->addLayout(pHL222);
    pHL2->setSpacing(space);
    pHL2->setContentsMargins(L,0,R,0);

    pVL->addLayout(pHL0);
    pVL->addWidget(pLine);
    pVL->addLayout(pHL1);
    pVL->addLayout(pHL2);
    pVL->setSpacing(space);
    pVL->setContentsMargins(0, T, 0, B);

    m_pButtonShapeOpacityReset->setFixedSize(btnW,btnW);
    m_pButtonBorderReset->setFixedSize(btnW, btnW);
    m_pButtonBorderOpacityReset->setFixedSize(btnW, btnW);

    m_pSpinBoxBorderOpacity->setFixedSize(w-20, h);
    m_pSpinBoxBorder->setFixedSize(w - 20, h);
    m_pSpinBoxShapeOpacity->setFixedSize(w - 20, h);

    m_pSpinBoxBorderOpacity->setRange(0,100);
    m_pSpinBoxBorder->setRange(0, 50);
    m_pSpinBoxShapeOpacity->setRange(0, 100);

    m_pSelecterBorderColor->setFixedSize(w, h);
    m_pSelecterShapeColor->setFixedSize(w, h);
    pLabel->setFixedSize(w, h);
    pLabel1->setFixedSize(btnW, btnW);
    pLabel2->setFixedSize(btnW, btnW);
    pLabel3->setFixedSize(btnW, btnW);

    m_pLabelShapeOpacity->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    m_pLabelBorder->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_pLabelBorderOpacity->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    m_pSpinBoxBorderOpacity->setDecimals(0);
    m_pSpinBoxBorderOpacity->setButtonSymbols(QAbstractSpinBox::NoButtons);

    m_pSpinBoxShapeOpacity->setDecimals(0);
    m_pSpinBoxShapeOpacity->setButtonSymbols(QAbstractSpinBox::NoButtons);

    m_pSpinBoxBorder->setDecimals(0);
    m_pSpinBoxBorder->setButtonSymbols(QAbstractSpinBox::NoButtons);

    m_pSpinBoxShapeOpacity->setSuffix("%");
    m_pSpinBoxBorderOpacity->setSuffix("%");

    m_pLabelShapeOpacity->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_pLabelBorder->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_pLabelBorderOpacity->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    m_pLabelShapeOpacity->setObjectName("FSDenoiseOption_m_pLevelLable");
    m_pLabelBorder->setObjectName("FSDenoiseOption_m_pLevelLable");
    m_pLabelBorderOpacity->setObjectName("FSDenoiseOption_m_pLevelLable");
    pLine->setObjectName("FSAnotationOption_pLine");

    m_pSpinBoxBorderOpacity->setObjectName("FSVisualPorpertyView_pDoubleSpinBox");
    m_pSpinBoxBorder->setObjectName("FSVisualPorpertyView_pDoubleSpinBox");
    m_pSpinBoxShapeOpacity->setObjectName("FSVisualPorpertyView_pDoubleSpinBox");


    m_pButtonShapeOpacityReset->setObjectName("FSPropertyTitle_m_pResetBtn");
    m_pButtonBorderReset->setObjectName("FSPropertyTitle_m_pResetBtn");
    m_pButtonBorderOpacityReset->setObjectName("FSPropertyTitle_m_pResetBtn");   
}

void FSAnotationOption::BlockSignals(bool bBlock)
{
    m_pSpinBoxBorderOpacity->blockSignals(bBlock);
    m_pSpinBoxBorder->blockSignals(bBlock);
    m_pSpinBoxShapeOpacity->blockSignals(bBlock);
    m_pSelecterBorderColor->blockSignals(bBlock);
    m_pSelecterShapeColor->blockSignals(bBlock);
}

int FSAnotationOption::Alpha2Opacity(int alpha)
{
    return qRound(100.0*alpha / 255);
}

int FSAnotationOption::Opacity2Alpha(int Opacity)
{
    return qRound(Opacity*255.0 / 100.0);
}

bool FSAnotationOption::IsResetable()
{
    return IsShapeResetable() || IsBorderWidthResetable() || IsBorderShapeResetable();
}

bool FSAnotationOption::IsShapeResetable()
{
    return m_currentShapeColor != m_defaultShapeColor;
}

bool FSAnotationOption::IsBorderWidthResetable()
{
    int v = m_pSpinBoxBorder->value();
    return v != m_defaultBorderWidht; 
}

bool FSAnotationOption::IsBorderShapeResetable()
{
    return m_currentBorderColor != m_defaultBorderColor;
}

void FSAnotationOption::ConnectSignals()
{
    connect(m_pSpinBoxShapeOpacity,SIGNAL(valueChanged(QString)),this,SLOT(SlotShapeOpacityChanged(QString)));
    connect(m_pSpinBoxBorderOpacity, SIGNAL(valueChanged(QString)), this, SLOT(SlotBorderOpacityChanged(QString)));
    connect(m_pSpinBoxBorder, SIGNAL(valueChanged(QString)), this, SLOT(SlotBorderWidthChanged(QString)));

    connect(m_pSpinBoxBorder->GetHelper(),        &IDragValueChangedHelper::sigStartValueChanged, this, &FSAnotationOption::startValueChanged);
    connect(m_pSpinBoxBorderOpacity->GetHelper(), &IDragValueChangedHelper::sigStartValueChanged, this, &FSAnotationOption::startValueChanged);
    connect(m_pSpinBoxShapeOpacity->GetHelper(), &IDragValueChangedHelper::sigStartValueChanged, this, &FSAnotationOption::startValueChanged);
    connect(m_pSelecterShapeColor, &ColorSelecterComboBox::CurrentColorChanged, this, [&](QColor c){
        m_currentShapeColor.setBlue(c.blue());
        m_currentShapeColor.setRed(c.red());
        m_currentShapeColor.setGreen(c.green());
        m_pButtonShapeOpacityReset->setVisible(IsShapeResetable());
        m_pTitleWidget->SetResetable(IsResetable());
        m_bChangeFlag = m_bChangeFlag | 0x01;
        emit ParmaChanged();
    });
    connect(m_pSelecterBorderColor, &ColorSelecterComboBox::CurrentColorChanged, this, [&](QColor c){
        m_currentBorderColor.setBlue(c.blue());
        m_currentBorderColor.setRed(c.red());
        m_currentBorderColor.setGreen(c.green());
        m_pButtonBorderOpacityReset->setVisible(IsBorderShapeResetable());
        m_pTitleWidget->SetResetable(IsResetable());
        m_bChangeFlag = m_bChangeFlag | 0x04;
        emit ParmaChanged();
    });

    connect(m_pButtonShapeOpacityReset,  &FSButton::clicked, this, [&](){
        m_currentShapeColor = m_defaultShapeColor;
        BlockSignals(true);
        m_pSelecterShapeColor->SetCurrentColor(QColor(m_defaultShapeColor.name(QColor::HexRgb)));
        m_pSpinBoxShapeOpacity->setValue(Alpha2Opacity(m_defaultShapeColor.alpha()));
        BlockSignals(false);
        m_pButtonShapeOpacityReset->setVisible(IsShapeResetable());
        m_pTitleWidget->SetResetable(IsResetable());
        m_bChangeFlag = m_bChangeFlag | 0x01;
        emit ParmaChanged();
    });
    connect(m_pButtonBorderOpacityReset, &FSButton::clicked, this, [&](){
        m_currentBorderColor = m_defaultBorderColor;
        BlockSignals(true);
        m_pSelecterBorderColor->SetCurrentColor(QColor(m_defaultBorderColor.name(QColor::HexRgb)));
        m_pSpinBoxBorderOpacity->setValue(Alpha2Opacity(m_defaultBorderColor.alpha()));
        BlockSignals(false);
        m_pButtonBorderOpacityReset->setVisible(IsBorderShapeResetable());
        m_pTitleWidget->SetResetable(IsResetable());
        m_bChangeFlag = m_bChangeFlag | 0x04;
        emit ParmaChanged();
    });
    connect(m_pButtonBorderReset,        &FSButton::clicked, this, [&](){      
        BlockSignals(true);       
        m_pSpinBoxBorder->setValue(m_defaultBorderWidht);
        BlockSignals(false);
        m_pButtonBorderReset->setVisible(IsBorderWidthResetable());
        m_pTitleWidget->SetResetable(IsResetable());
        m_bChangeFlag = m_bChangeFlag | 0x02;
        emit ParmaChanged();
    });
}

void FSAnotationOption::SlotShapeOpacityChanged(QString v)
{
    int a = Opacity2Alpha(v.mid(0,v.length()-1).toInt());
    m_currentShapeColor.setAlpha(a);
    m_pButtonShapeOpacityReset->setVisible(IsShapeResetable());
    m_pTitleWidget->SetResetable(IsResetable());
    m_bChangeFlag = m_bChangeFlag | 0x01;
    emit ParmaChanged();
}

void FSAnotationOption::SlotBorderOpacityChanged(QString v)
{
    int a = Opacity2Alpha(v.mid(0, v.length() - 1).toInt());
    m_currentBorderColor.setAlpha(a);
    m_pButtonBorderOpacityReset->setVisible(IsBorderShapeResetable());
    m_pTitleWidget->SetResetable(IsResetable());
    m_bChangeFlag = m_bChangeFlag | 0x04;
    emit ParmaChanged();
}

void FSAnotationOption::SlotBorderWidthChanged(QString w)
{
    m_pButtonBorderReset->setVisible(IsBorderWidthResetable());
    m_pTitleWidget->SetResetable(IsResetable());
    m_bChangeFlag = m_bChangeFlag | 0x02;
    emit ParmaChanged();
}




FSTextEdit::FSTextEdit(QWidget *pParent)
    :QTextEdit(pParent)
    , m_pos(0)
{
    setAcceptDrops(false);
}

FSTextEdit::~FSTextEdit()
{

}

void FSTextEdit::SetPos(int pos)
{    
    m_pos = pos;
}

void FSTextEdit::keyPressEvent(QKeyEvent *e)
{
    emit sigKeyPress(e);
    QTextEdit::keyPressEvent(e);
    if (e->modifiers()&Qt::ControlModifier&&e->key() == Qt::Key_A)
        return;
    QTimer::singleShot(100, [&]()
    { 
        blockSignals(true);
        QTextCursor temptextCursor = textCursor();
        temptextCursor.setPosition(m_pos);
        setTextCursor(temptextCursor);
        blockSignals(false);
        emit selectionChanged();
    });
}

void FSTextEdit::inputMethodEvent(QInputMethodEvent *e)
{  
    if (!textCursor().selectedText().isEmpty())
    {
        QKeyEvent E(QEvent::KeyPress, Qt::Key_Backspace, Qt::NoModifier);
        emit sigKeyPress(&E);
    }

    emit sigInputMethod(e);
    QTextEdit::inputMethodEvent(e);   
    QTimer::singleShot(100, [&]()
    {
        blockSignals(true);
        QTextCursor temptextCursor = textCursor();
        temptextCursor.setPosition(m_pos);
        setTextCursor(temptextCursor);
        blockSignals(false);
        emit selectionChanged();
    });
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FSTextBorderOption::FSTextBorderOption(QWidget *pParent)
    :IFSOptionView(TypeTextBorder,pParent)
{
    CreateUI();
    RetranslateUi();
    ConnectSignals();
}

FSTextBorderOption::~FSTextBorderOption()
{

}

void FSTextBorderOption::UpdataParma(IFSEditHelper *pHelper)
{
    if (pHelper)
    {
        BlockSignals(true);
        InitDefaultParma(pHelper);
        UpdataUI(pHelper);
        UpadaResetUI();
        BlockSignals(false);
    }
}

QColor FSTextBorderOption::GetTextBorderColor()
{
    return m_pColorSelecter->GetCurrentColor();
}

int FSTextBorderOption::GetTextBorderOpacity()
{
    return Opacity2Alpha(m_pSpinBoxOpacity->value());
}

int FSTextBorderOption::GetTextBorderSize()
{
    return qRound(m_pSpinBoxSize->value());
}

int FSTextBorderOption::GetTextBorderBlur()
{
    return qRound(m_pSpinBoxBlur->value());
}

bool FSTextBorderOption::IsColorChanged()
{
    return m_ChangeFlag & 0x01;
}

bool FSTextBorderOption::IsOpacityChanged()
{
    return m_ChangeFlag & 0x02;
}

bool FSTextBorderOption::IsBlurChanged()
{
    return m_ChangeFlag & 0x04;
}

bool FSTextBorderOption::IsSizeChanged()
{
    return m_ChangeFlag & 0x08;
}

void FSTextBorderOption::ResetChangeFlag()
{
    m_ChangeFlag = 0;
}

void FSTextBorderOption::OnReset()
{
    BlockSignals(true);
    m_pColorSelecter->SetCurrentColor(m_DefaultColor);
    m_pSpinBoxOpacity->setValue(Alpha2Opacity(m_DefaultOpacity));
    m_pSpinBoxSize->setValue(m_DefaultSize);
    m_pSpinBoxBlur->setValue(m_DefaultBlur);
    m_pButtonResetOpacity->setVisible(false);
    m_pButtonResetSize->setVisible(false);
    m_pButtonResetColor->setVisible(false);
    m_pButtonResetBlur->setVisible(false);
    m_pTitleWidget->SetResetable(false);
    m_ChangeFlag = 0x0f;
    BlockSignals(false);
    emit ParmaChanged();
}

void FSTextBorderOption::RetranslateUi()
{
    m_pLabelColor->setText(tr("Color"));
    m_pLabelOpacity->setText(tr("Opacity"));
    m_pLabelBlur->setText(tr("Blur"));
    m_pLabelSize->setText(tr("Size"));
}

void FSTextBorderOption::OnOpacityValueChanged(double)
{
    m_ChangeFlag |= 0x02;
    m_pButtonResetOpacity->setVisible(IsOpacityResetable());
    m_pTitleWidget->SetResetable(IsResetable());
    emit ParmaChanged();
}

void FSTextBorderOption::OnBlurValueChanged(double)
{
    m_ChangeFlag |= 0x04;
    m_pButtonResetBlur->setVisible(IsBlurResetable());
    m_pTitleWidget->SetResetable(IsResetable());
    emit ParmaChanged();
}

void FSTextBorderOption::OnSizeValueChanged(double)
{
    m_ChangeFlag |= 0x08;
    m_pButtonResetSize->setVisible(IsSizeResetable());
    m_pTitleWidget->SetResetable(IsResetable());
    emit ParmaChanged();
}

void FSTextBorderOption::CreateUI()
{
    m_pColorSelecter  = new ColorSelecterComboBox(this);
    m_pSpinBoxOpacity = new FSDoubleSpinBox(this);
    m_pSpinBoxSize    = new FSDoubleSpinBox(this);
    m_pSpinBoxBlur    = new FSDoubleSpinBox(this);

    m_pLabelColor   = new FSLabel(this);
    m_pLabelOpacity = new FSLabel(this);
    m_pLabelBlur    = new FSLabel(this);
    m_pLabelSize    = new FSLabel(this);

    m_pButtonResetSize    = new FSButton(this);
    m_pButtonResetBlur    = new FSButton(this);
    m_pButtonResetColor   = new FSButton(this);
    m_pButtonResetOpacity = new FSButton(this);

    int h = 20, w = 60, space = 10, L = 14, R = 21, btnW = 14,T=12,B=12;
    QVBoxLayout *pVL  = new QVBoxLayout(m_pOptionWidget);
    QHBoxLayout *pHL1 = new QHBoxLayout();
    QHBoxLayout *pHL11 = new QHBoxLayout();
    QHBoxLayout *pHL111 = new QHBoxLayout();
    QHBoxLayout *pHL2 = new QHBoxLayout();
    QHBoxLayout *pHL22 = new QHBoxLayout();
    QHBoxLayout *pHL222 = new QHBoxLayout();
    QHBoxLayout *pHL3 = new QHBoxLayout();
    QHBoxLayout *pHL33 = new QHBoxLayout();
    QHBoxLayout *pHL333 = new QHBoxLayout();
    QHBoxLayout *pHL4 = new QHBoxLayout();
    QHBoxLayout *pHL44 = new QHBoxLayout();
    QHBoxLayout *pHL444 = new QHBoxLayout();

    pHL11->addStretch();
    pHL11->addWidget(m_pLabelColor);
    pHL11->setMargin(0);
    pHL11->setSpacing(0);
  
    pHL111->addWidget(m_pColorSelecter);
    pHL111->addStretch();
    pHL111->addWidget(m_pButtonResetColor);
    pHL111->setMargin(0);
    pHL111->setSpacing(0);

    pHL1->addLayout(pHL11);
    pHL1->addLayout(pHL111);
    pHL1->setMargin(0);
    pHL1->setSpacing(space);

    pHL22->addStretch();
    pHL22->addWidget(m_pLabelOpacity);
    pHL22->setMargin(0);
    pHL22->setSpacing(0);

    pHL222->addWidget(m_pSpinBoxOpacity);
    pHL222->addStretch();
    pHL222->addWidget(m_pButtonResetOpacity);
    pHL222->setMargin(0);
    pHL222->setSpacing(0);

    pHL2->addLayout(pHL22);
    pHL2->addLayout(pHL222);
    pHL2->setMargin(0);
    pHL2->setSpacing(space);

    pHL33->addStretch();
    pHL33->addWidget(m_pLabelBlur);
    pHL33->setMargin(0);
    pHL33->setSpacing(0);

    pHL333->addWidget(m_pSpinBoxBlur);
    pHL333->addStretch();
    pHL333->addWidget(m_pButtonResetBlur);
    pHL333->setMargin(0);
    pHL333->setSpacing(0);

    pHL3->addLayout(pHL33);
    pHL3->addLayout(pHL333);
    pHL3->setMargin(0);
    pHL3->setSpacing(space);

    pHL44->addStretch();
    pHL44->addWidget(m_pLabelSize);
    pHL44->setMargin(0);
    pHL44->setSpacing(0);

    pHL444->addWidget(m_pSpinBoxSize);
    pHL444->addStretch();
    pHL444->addWidget(m_pButtonResetSize);
    pHL444->setMargin(0);
    pHL444->setSpacing(0);

    pHL4->addLayout(pHL44);
    pHL4->addLayout(pHL444);
    pHL4->setMargin(0);
    pHL4->setSpacing(space);


    pVL->addLayout(pHL1);
    pVL->addLayout(pHL2);
    pVL->addLayout(pHL3);
    pVL->addLayout(pHL4);
    pVL->setSpacing(space);
    pVL->setContentsMargins(L, T, R, B);

    m_pLabelColor->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_pLabelOpacity->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_pLabelBlur->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_pLabelSize->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    m_pLabelColor->setFixedSize(w,h);
    m_pLabelOpacity->setFixedSize(w, h);
    m_pLabelBlur->setFixedSize(w, h);
    m_pLabelSize->setFixedSize(w, h);

    m_pButtonResetSize->setFixedSize(btnW, btnW);
    m_pButtonResetBlur->setFixedSize(btnW, btnW);
    m_pButtonResetColor->setFixedSize(btnW, btnW);
    m_pButtonResetOpacity->setFixedSize(btnW, btnW);

    m_pColorSelecter->setFixedSize(w, h);
    m_pSpinBoxOpacity->setFixedSize(w, h);
    m_pSpinBoxSize->setFixedSize(w, h);
    m_pSpinBoxBlur->setFixedSize(w, h);

    m_pSpinBoxSize->setRange(0, 50);
    m_pSpinBoxBlur->setRange(0, 10);
    m_pSpinBoxOpacity->setRange(0, 100);

    m_pSpinBoxSize->setDecimals(0);
    m_pSpinBoxBlur->setDecimals(0);
    m_pSpinBoxOpacity->setDecimals(0);

    m_pSpinBoxOpacity->setSuffix("%");

    m_pSpinBoxOpacity->setButtonSymbols(QAbstractSpinBox::NoButtons);
    m_pSpinBoxSize->setButtonSymbols(QAbstractSpinBox::NoButtons);
    m_pSpinBoxBlur->setButtonSymbols(QAbstractSpinBox::NoButtons);


    m_pButtonResetSize->setObjectName("FSPropertyTitle_m_pResetBtn");
    m_pButtonResetBlur->setObjectName("FSPropertyTitle_m_pResetBtn");
    m_pButtonResetColor->setObjectName("FSPropertyTitle_m_pResetBtn");
    m_pButtonResetOpacity->setObjectName("FSPropertyTitle_m_pResetBtn"); 
    m_pSpinBoxOpacity->setObjectName("FSVisualPorpertyView_pDoubleSpinBox");
    m_pSpinBoxSize->setObjectName("FSVisualPorpertyView_pDoubleSpinBox");
    m_pSpinBoxBlur->setObjectName("FSVisualPorpertyView_pDoubleSpinBox");
    m_pLabelColor->setObjectName("FSDenoiseOption_m_pLevelLable");
    m_pLabelOpacity->setObjectName("FSDenoiseOption_m_pLevelLable");
    m_pLabelBlur->setObjectName("FSDenoiseOption_m_pLevelLable");
    m_pLabelSize->setObjectName("FSDenoiseOption_m_pLevelLable");
}

void FSTextBorderOption::ConnectSignals()
{
    connect(m_pButtonResetColor, &FSButton::clicked, [&](){
        BlockSignals(true);
        m_ChangeFlag |= 0x01;
        m_pColorSelecter->SetCurrentColor(m_DefaultColor);
        m_pButtonResetColor->setVisible(false);
        m_pTitleWidget->SetResetable(IsResetable());
        BlockSignals(false);
        emit ParmaChanged();
    
    });
    connect(m_pButtonResetOpacity, &FSButton::clicked, [&](){
        BlockSignals(true);
        m_ChangeFlag |= 0x02;
        m_pSpinBoxOpacity->setValue(Alpha2Opacity(m_DefaultOpacity));
        m_pButtonResetOpacity->setVisible(false);
        m_pTitleWidget->SetResetable(IsResetable());
        BlockSignals(false);
        emit ParmaChanged();
    });
    connect(m_pButtonResetBlur, &FSButton::clicked, [&](){
        BlockSignals(true);
        m_ChangeFlag |= 0x04;
        m_pSpinBoxBlur->setValue(m_DefaultBlur);
        m_pButtonResetBlur->setVisible(false);
        m_pTitleWidget->SetResetable(IsResetable());
        BlockSignals(false);
        emit ParmaChanged();
    });
    connect(m_pButtonResetSize, &FSButton::clicked, [&](){
        BlockSignals(true);
        m_ChangeFlag |= 0x08;
        m_pSpinBoxSize->setValue(m_DefaultSize);
        m_pButtonResetSize->setVisible(false);
        m_pTitleWidget->SetResetable(IsResetable());
        BlockSignals(false);
        emit ParmaChanged();
    });

    connect(m_pSpinBoxOpacity, SIGNAL(valueChanged(double)),              this, SLOT(OnOpacityValueChanged(double)));
    connect(m_pSpinBoxBlur,    SIGNAL(valueChanged(double)),              this, SLOT(OnBlurValueChanged(double)));
    connect(m_pSpinBoxSize,    SIGNAL(valueChanged(double)),              this, SLOT(OnSizeValueChanged(double)));
    connect(m_pColorSelecter,  SIGNAL(CurrentColorChanged(QColor)),       this, SLOT(OnCurrentColorChanged(QColor)));
}

void FSTextBorderOption::BlockSignals(bool bBlock)
{
    m_pSpinBoxBlur->blockSignals(bBlock);
    m_pSpinBoxOpacity->blockSignals(bBlock);
    m_pSpinBoxSize->blockSignals(bBlock);
    m_pColorSelecter->blockSignals(bBlock);
}

void FSTextBorderOption::OnCurrentColorChanged(QColor color)
{
    m_ChangeFlag |= 0x01;
    m_pButtonResetColor->setVisible(IsColorResetable());
    m_pTitleWidget->SetResetable(IsResetable());
    emit ParmaChanged();
}

void FSTextBorderOption::InitDefaultParma(IFSEditHelper *pHelper)
{
    Q_ASSERT(pHelper);
    if (pHelper)
    {
        pHelper->GetDefualtTextBorderColor(m_DefaultColor);
        pHelper->GetDefualtTextBorderSize(m_DefaultSize);
        pHelper->GetDefualtTextBorderOpacity(m_DefaultOpacity);
        pHelper->GetDefualtTextBorderBlur(m_DefaultBlur);
    }    
}

void FSTextBorderOption::UpdataUI(IFSEditHelper *pHelper)
{
    Q_ASSERT(pHelper);
    if (pHelper)
    {
        FSTextEditHelper *pTempHelper = static_cast<FSTextEditHelper *>(pHelper);
        m_pColorSelecter->SetCurrentColor(pTempHelper->GetTextBorderColor());
        m_pSpinBoxOpacity->setValue(Alpha2Opacity(pTempHelper->GetTextBorderOpacity()));
        m_pSpinBoxBlur->setValue(pTempHelper->GetTextBorderBlur());
        m_pSpinBoxSize->setValue(pTempHelper->GetTextBorderSize());
    }  
}

void FSTextBorderOption::UpadaResetUI()
{
    m_pButtonResetColor->setVisible(IsColorResetable());
    m_pButtonResetOpacity->setVisible(IsOpacityResetable());
    m_pButtonResetBlur->setVisible(IsBlurResetable());
    m_pButtonResetSize->setVisible(IsSizeResetable());
    m_pTitleWidget->SetResetable(IsResetable());
}

bool FSTextBorderOption::IsResetable()
{
    return IsOpacityResetable() || IsColorResetable() || IsBlurResetable() || IsSizeResetable();
}

bool FSTextBorderOption::IsOpacityResetable()
{
    return Opacity2Alpha(m_pSpinBoxOpacity->value())!=m_DefaultOpacity;
}

bool FSTextBorderOption::IsBlurResetable()
{
    return GetTextBorderBlur() != m_DefaultBlur;
}

bool FSTextBorderOption::IsSizeResetable()
{
    return GetTextBorderSize()!= m_DefaultSize;
}

bool FSTextBorderOption::IsColorResetable()
{
    return m_pColorSelecter->GetCurrentColor() != m_DefaultColor;
}

int FSTextBorderOption::Alpha2Opacity(int alpha)
{
    return qRound(100.0*alpha / 255);
}

int FSTextBorderOption::Opacity2Alpha(int Opacity)
{
    return qRound(Opacity*255.0 / 100.0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FSTextShadowOption::FSTextShadowOption(QWidget *pParent)
    :IFSOptionView(TypeTextShadow,pParent)
{
    CreateUI();
    RetranslateUi();
    ConnectSignals();
}

FSTextShadowOption::~FSTextShadowOption()
{
    
}

void FSTextShadowOption::UpdataParma(IFSEditHelper *pHelper)
{
    Q_ASSERT(pHelper);
    if (pHelper)
    {
        BlockSignals(true);
        InitDefaultParma(pHelper);
        UpdataUI(pHelper);
        UpadaResetUI();
        BlockSignals(false);
    }
}

QColor FSTextShadowOption::GetTextShadowColor()
{
    return m_pColorSelecter->GetCurrentColor();
}

int FSTextShadowOption::GetTextShadowOpacity()
{
    return Opacity2Alpha(m_pSpinBoxOpacity->value());
}

int FSTextShadowOption::GetTextShadowDistance()
{
    return qRound(m_pSpinBoxDistance->value());
}

int FSTextShadowOption::GetTextShadowBlur()
{
    return qRound(m_pSpinBoxBlur->value());
}

int FSTextShadowOption::GetTextShadowDerection()
{
    return m_pTextShadowSelecter->GetCurrentIndex();
}

bool FSTextShadowOption::IsColorChanged()
{
    return m_ChangeFlag & 0x02;
}

bool FSTextShadowOption::IsOpacityChanged()
{
    return m_ChangeFlag & 0x04;
}

bool FSTextShadowOption::IsBlurChanged()
{
    return m_ChangeFlag & 0x08;
}

bool FSTextShadowOption::IsTypeChanged()
{
    return m_ChangeFlag & 0x01;
}

bool FSTextShadowOption::IsDistanceChanged()
{
    return m_ChangeFlag & 0x10;
}

void FSTextShadowOption::ResetChangeFlag()
{
    m_ChangeFlag = 0;
}

void FSTextShadowOption::OnReset()
{
    BlockSignals(true);
    m_pColorSelecter->SetCurrentColor(m_DefaultColor);
    m_pSpinBoxOpacity->setValue(Alpha2Opacity(m_DefaultOpacity));
    m_pSpinBoxDistance->setValue(m_DefaultDistance);
    m_pSpinBoxBlur->setValue(m_DefaultBlur);
    m_pTextShadowSelecter->SetCurrentIndex(m_DefaultDerection);
    m_pButtonResetOpacity->setVisible(false);
    m_pButtonResetDistance->setVisible(false);
    m_pButtonResetColor->setVisible(false);
    m_pButtonResetBlur->setVisible(false);
    m_pButtonResetDerection->setVisible(false);
    m_pTitleWidget->SetResetable(false);
    m_ChangeFlag = 0x1f;
    BlockSignals(false);
    emit ParmaChanged();
}

void FSTextShadowOption::RetranslateUi()
{
    m_pLabelDerection->setText(tr("Type"));
    m_pLabelColor->setText(tr("Color"));
    m_pLabelOpacity->setText(tr("Opacity"));
    m_pLabelBlur->setText(tr("Blur"));
    m_pLabelDistance->setText(tr("Distance"));
}

void FSTextShadowOption::OnOpacityValueChanged(double)
{
    m_ChangeFlag |= 0x04;
    m_pButtonResetOpacity->setVisible(IsOpacityResetable());
    m_pTitleWidget->SetResetable(IsResetable());
    emit ParmaChanged();
}

void FSTextShadowOption::OnBlurValueChanged(double)
{
    m_ChangeFlag |= 0x08;
    m_pButtonResetBlur->setVisible(IsBlurResetable());
    m_pTitleWidget->SetResetable(IsResetable());
    emit ParmaChanged();
}

void FSTextShadowOption::OnTypeValueChanged(int)
{
    m_ChangeFlag |= 0x01;
    m_pButtonResetDerection->setVisible(IsTypeResetable());
    m_pTitleWidget->SetResetable(IsResetable());
    emit ParmaChanged();
}

void FSTextShadowOption::OnCurrentColorChanged(QColor)
{
    m_ChangeFlag |= 0x02;
    m_pButtonResetColor->setVisible(IsColorResetable());
    m_pTitleWidget->SetResetable(IsResetable());
    emit ParmaChanged();
}

void FSTextShadowOption::OnDistanceChanged(double)
{
    m_ChangeFlag |= 0x10;
    m_pButtonResetDistance->setVisible(IsDistanceResetable());
    m_pTitleWidget->SetResetable(IsResetable());
    emit ParmaChanged();
}

void FSTextShadowOption::CreateUI()
{
    m_pTextShadowSelecter = new FSTextShadowSelecter(this);
    m_pColorSelecter = new ColorSelecterComboBox(this);
    m_pSpinBoxOpacity = new FSDoubleSpinBox(this);
    m_pSpinBoxDistance = new FSDoubleSpinBox(this);
    m_pSpinBoxBlur = new FSDoubleSpinBox(this);

    m_pLabelDerection = new FSLabel(this);
    m_pLabelColor = new FSLabel(this);
    m_pLabelOpacity = new FSLabel(this);
    m_pLabelBlur = new FSLabel(this);
    m_pLabelDistance = new FSLabel(this);

    m_pButtonResetDerection = new FSButton(this);
    m_pButtonResetDistance = new FSButton(this);
    m_pButtonResetBlur = new FSButton(this);
    m_pButtonResetColor = new FSButton(this);
    m_pButtonResetOpacity = new FSButton(this);

    int h = 20, w = 60, space = 10, L = 14, R = 21, btnW = 14, T = 12, B = 12;
    QVBoxLayout *pVL = new QVBoxLayout(m_pOptionWidget);
    QHBoxLayout *pHL0 = new QHBoxLayout();
    QHBoxLayout *pHL00 = new QHBoxLayout();
    QHBoxLayout *pHL000 = new QHBoxLayout();
    QHBoxLayout *pHL1 = new QHBoxLayout();
    QHBoxLayout *pHL11 = new QHBoxLayout();
    QHBoxLayout *pHL111 = new QHBoxLayout();
    QHBoxLayout *pHL2 = new QHBoxLayout();
    QHBoxLayout *pHL22 = new QHBoxLayout();
    QHBoxLayout *pHL222 = new QHBoxLayout();
    QHBoxLayout *pHL3 = new QHBoxLayout();
    QHBoxLayout *pHL33 = new QHBoxLayout();
    QHBoxLayout *pHL333 = new QHBoxLayout();
    QHBoxLayout *pHL4 = new QHBoxLayout();
    QHBoxLayout *pHL44 = new QHBoxLayout();
    QHBoxLayout *pHL444 = new QHBoxLayout();

    pHL00->addStretch();
    pHL00->addWidget(m_pLabelDerection);    
    pHL00->setMargin(0);
    pHL00->setSpacing(0);

    pHL000->addWidget(m_pTextShadowSelecter);
    pHL000->addStretch();
    pHL000->addWidget(m_pButtonResetDerection);
    pHL000->setMargin(0);
    pHL000->setSpacing(0);

    pHL0->addLayout(pHL00);
    pHL0->addLayout(pHL000);
    pHL0->setMargin(0);
    pHL0->setSpacing(space);

    pHL11->addStretch();
    pHL11->addWidget(m_pLabelColor);
    pHL11->setMargin(0);
    pHL11->setSpacing(0);

    pHL111->addWidget(m_pColorSelecter);
    pHL111->addStretch();
    pHL111->addWidget(m_pButtonResetColor);
    pHL111->setMargin(0);
    pHL111->setSpacing(0);

    pHL1->addLayout(pHL11);
    pHL1->addLayout(pHL111);
    pHL1->setMargin(0);
    pHL1->setSpacing(space);

    pHL22->addStretch();
    pHL22->addWidget(m_pLabelOpacity);
    pHL22->setMargin(0);
    pHL22->setSpacing(0);

    pHL222->addWidget(m_pSpinBoxOpacity);
    pHL222->addStretch();
    pHL222->addWidget(m_pButtonResetOpacity);
    pHL222->setMargin(0);
    pHL222->setSpacing(0);

    pHL2->addLayout(pHL22);
    pHL2->addLayout(pHL222);
    pHL2->setMargin(0);
    pHL2->setSpacing(space);

    pHL33->addStretch();
    pHL33->addWidget(m_pLabelBlur);
    pHL33->setMargin(0);
    pHL33->setSpacing(0);

    pHL333->addWidget(m_pSpinBoxBlur);
    pHL333->addStretch();
    pHL333->addWidget(m_pButtonResetBlur);
    pHL333->setMargin(0);
    pHL333->setSpacing(0);

    pHL3->addLayout(pHL33);
    pHL3->addLayout(pHL333);
    pHL3->setMargin(0);
    pHL3->setSpacing(space);

    pHL44->addStretch();
    pHL44->addWidget(m_pLabelDistance);
    pHL44->setMargin(0);
    pHL44->setSpacing(0);

    pHL444->addWidget(m_pSpinBoxDistance);
    pHL444->addStretch();
    pHL444->addWidget(m_pButtonResetDistance);
    pHL444->setMargin(0);
    pHL444->setSpacing(0);

    pHL4->addLayout(pHL44);
    pHL4->addLayout(pHL444);
    pHL4->setMargin(0);
    pHL4->setSpacing(space);

    pVL->addLayout(pHL0);
    pVL->addLayout(pHL1);
    pVL->addLayout(pHL2);
    pVL->addLayout(pHL3);
    pVL->addLayout(pHL4);
    pVL->setSpacing(space);
    pVL->setContentsMargins(L, T, R, B);

    m_pLabelDerection->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_pLabelColor->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_pLabelOpacity->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_pLabelBlur->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_pLabelDistance->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    m_pLabelDerection->setFixedSize(w, h);
    m_pLabelColor->setFixedSize(w, h);
    m_pLabelOpacity->setFixedSize(w, h);
    m_pLabelBlur->setFixedSize(w, h);
    m_pLabelDistance->setFixedSize(w, h);

    m_pButtonResetDerection->setFixedSize(btnW, btnW);
    m_pButtonResetDistance->setFixedSize(btnW, btnW);
    m_pButtonResetBlur->setFixedSize(btnW, btnW);
    m_pButtonResetColor->setFixedSize(btnW, btnW);
    m_pButtonResetOpacity->setFixedSize(btnW, btnW);

    m_pTextShadowSelecter->setFixedSize(45,30);
    m_pColorSelecter->setFixedSize(w, h);
    m_pSpinBoxOpacity->setFixedSize(w, h);
    m_pSpinBoxDistance->setFixedSize(w, h);
    m_pSpinBoxBlur->setFixedSize(w, h);


    m_pSpinBoxDistance->setRange(0, 20);
    m_pSpinBoxBlur->setRange(0, 10);
    m_pSpinBoxOpacity->setRange(0, 100);

    m_pSpinBoxDistance->setDecimals(0);
    m_pSpinBoxBlur->setDecimals(0);
    m_pSpinBoxOpacity->setDecimals(0);

    m_pSpinBoxOpacity->setSuffix("%");

    m_pSpinBoxOpacity->setButtonSymbols(QAbstractSpinBox::NoButtons);
    m_pSpinBoxDistance->setButtonSymbols(QAbstractSpinBox::NoButtons);
    m_pSpinBoxBlur->setButtonSymbols(QAbstractSpinBox::NoButtons);

    m_pButtonResetDerection->setObjectName("FSPropertyTitle_m_pResetBtn");
    m_pButtonResetDistance->setObjectName("FSPropertyTitle_m_pResetBtn");
    m_pButtonResetBlur->setObjectName("FSPropertyTitle_m_pResetBtn");
    m_pButtonResetColor->setObjectName("FSPropertyTitle_m_pResetBtn");
    m_pButtonResetOpacity->setObjectName("FSPropertyTitle_m_pResetBtn");
    m_pSpinBoxOpacity->setObjectName("FSVisualPorpertyView_pDoubleSpinBox");
    m_pSpinBoxDistance->setObjectName("FSVisualPorpertyView_pDoubleSpinBox");
    m_pSpinBoxBlur->setObjectName("FSVisualPorpertyView_pDoubleSpinBox");
    m_pLabelDerection->setObjectName("FSDenoiseOption_m_pLevelLable");
    m_pLabelColor->setObjectName("FSDenoiseOption_m_pLevelLable");
    m_pLabelOpacity->setObjectName("FSDenoiseOption_m_pLevelLable");
    m_pLabelBlur->setObjectName("FSDenoiseOption_m_pLevelLable");
    m_pLabelDistance->setObjectName("FSDenoiseOption_m_pLevelLable");
}

void FSTextShadowOption::ConnectSignals()
{
    connect(m_pButtonResetDerection, &FSButton::clicked, [&](){
        BlockSignals(true);
        m_ChangeFlag |= 0x01;
        m_pTextShadowSelecter->SetCurrentIndex(m_DefaultDerection);
        m_pButtonResetDerection->setVisible(false);
        m_pTitleWidget->SetResetable(IsResetable());
        BlockSignals(false);
        emit ParmaChanged();

    });
    connect(m_pButtonResetColor, &FSButton::clicked, [&](){
        BlockSignals(true);
        m_ChangeFlag |= 0x02;
        m_pColorSelecter->SetCurrentColor(m_DefaultColor);
        m_pButtonResetColor->setVisible(false);
        m_pTitleWidget->SetResetable(IsResetable());
        BlockSignals(false);
        emit ParmaChanged();

    });
    connect(m_pButtonResetOpacity, &FSButton::clicked, [&](){
        BlockSignals(true);
        m_ChangeFlag |= 0x04;
        m_pSpinBoxOpacity->setValue(Alpha2Opacity(m_DefaultOpacity));
        m_pButtonResetOpacity->setVisible(false);
        m_pTitleWidget->SetResetable(IsResetable());
        BlockSignals(false);
        emit ParmaChanged();
    });
    connect(m_pButtonResetBlur, &FSButton::clicked, [&](){
        BlockSignals(true);
        m_ChangeFlag |= 0x08;
        m_pSpinBoxBlur->setValue(m_DefaultBlur);
        m_pButtonResetBlur->setVisible(false);
        m_pTitleWidget->SetResetable(IsResetable());
        BlockSignals(false);
        emit ParmaChanged();
    });
    connect(m_pButtonResetDistance, &FSButton::clicked, [&](){
        BlockSignals(true);
        m_ChangeFlag |= 0x10;
        m_pSpinBoxDistance->setValue(m_DefaultDistance);
        m_pButtonResetDistance->setVisible(false);
        m_pTitleWidget->SetResetable(IsResetable());
        BlockSignals(false);
        emit ParmaChanged();
    });

    connect(m_pTextShadowSelecter, SIGNAL(CurentIndexChanged(int)), this, SLOT(OnTypeValueChanged(int)));
    connect(m_pSpinBoxOpacity, SIGNAL(valueChanged(double)), this, SLOT(OnOpacityValueChanged(double)));
    connect(m_pSpinBoxBlur, SIGNAL(valueChanged(double)), this, SLOT(OnBlurValueChanged(double)));
    connect(m_pSpinBoxDistance, SIGNAL(valueChanged(double)), this, SLOT(OnDistanceChanged(double)));
    connect(m_pColorSelecter, SIGNAL(CurrentColorChanged(QColor)), this, SLOT(OnCurrentColorChanged(QColor)));
}

void FSTextShadowOption::BlockSignals(bool bBlock)
{
    m_pTextShadowSelecter->blockSignals(bBlock);
    m_pSpinBoxOpacity->blockSignals(bBlock);
    m_pSpinBoxBlur->blockSignals(bBlock);
    m_pSpinBoxDistance->blockSignals(bBlock);
    m_pColorSelecter->blockSignals(bBlock);
}

void FSTextShadowOption::InitDefaultParma(IFSEditHelper *pHelper)
{
    Q_ASSERT(pHelper);
    if (pHelper)
    {
        pHelper->GetDefualtTextShadowBlur(m_DefaultBlur);
        pHelper->GetDefualtTextShadowColor(m_DefaultColor);
        pHelper->GetDefualtTextShadowDerection(m_DefaultDerection);
        pHelper->GetDefualtTextShadowDistance(m_DefaultDistance);
        pHelper->GetDefualtTextShadowOpacity(m_DefaultOpacity);
    }
}

void FSTextShadowOption::UpdataUI(IFSEditHelper *pHelper)
{
    Q_ASSERT(pHelper);
    if (pHelper)
    {
        FSTextEditHelper *pTempHelper = static_cast<FSTextEditHelper *>(pHelper);
        m_pColorSelecter->SetCurrentColor(pTempHelper->GetTextShadowColor());
        m_pSpinBoxOpacity->setValue(Alpha2Opacity(pTempHelper->GetTextShadowOpacity()));
        m_pSpinBoxBlur->setValue(pTempHelper->GetTextShadowBlur());
        m_pSpinBoxDistance->setValue(pTempHelper->GetTextShadowDistance());
        m_pTextShadowSelecter->SetCurrentIndex(pTempHelper->GetTextShadowDerection());
    }
}

void FSTextShadowOption::UpadaResetUI()
{
    m_pButtonResetColor->setVisible(IsColorResetable());
    m_pButtonResetOpacity->setVisible(IsOpacityResetable());
    m_pButtonResetBlur->setVisible(IsBlurResetable());
    m_pButtonResetDistance->setVisible(IsDistanceResetable());
    m_pButtonResetDerection->setVisible(IsTypeResetable());
    m_pTitleWidget->SetResetable(IsResetable());
}
    
int FSTextShadowOption::Alpha2Opacity(int alpha)
{
    return qRound(100.0*alpha / 255);
}

int FSTextShadowOption::Opacity2Alpha(int Opacity)
{
    return qRound(Opacity*255.0 / 100.0);
}

bool FSTextShadowOption::IsResetable()
{
    return IsOpacityResetable() || IsBlurResetable() || IsColorResetable() || IsDistanceResetable() ||IsTypeResetable();
}

bool FSTextShadowOption::IsOpacityResetable()
{
    return m_DefaultOpacity != GetTextShadowOpacity();
}

bool FSTextShadowOption::IsBlurResetable()
{
    return m_DefaultBlur != GetTextShadowBlur();
}

bool FSTextShadowOption::IsTypeResetable()
{
    return m_DefaultDerection != GetTextShadowDerection();
}

bool FSTextShadowOption::IsColorResetable()
{
    return m_DefaultColor != GetTextShadowColor();
}

bool FSTextShadowOption::IsDistanceResetable()
{
    return m_DefaultDistance != GetTextShadowDistance();
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FSSVGCaptionOption::FSSVGCaptionOption(QWidget *pParent)
    :IFSOptionView(TypeCaptionSvgOption, pParent)
{
    m_pVL = new QVBoxLayout(m_pOptionWidget);
}

FSSVGCaptionOption::~FSSVGCaptionOption()
{

}

void FSSVGCaptionOption::UpdataParma(IFSEditHelper *pHelper)
{
    Q_ASSERT(pHelper);
    if (pHelper)
    {
        QStringList list;
        pHelper->GetDefualtSvgCaptionColors(list);
        FSTextEditHelper *pTempHelper = static_cast<FSTextEditHelper *>(pHelper);
        int count;
        if (pTempHelper->GetCaptionSvgCount(count))
        {
            UpdataUI(count);
            for (int i = 0; i < count&&i < list.size(); i++)
            {
                m_pItemList.at(i)->SetIndex(i);
                m_pItemList.at(i)->SetDedaultColor(QColor(list.at(i)));
                QColor color;
                if (pTempHelper->GetCaptionSvgColor(i, color))
                {
                    m_pItemList.at(i)->SetCurrentColor(color);
                    m_ColorMap[i]= { false, color };
                }               
            }
        }
    }
}

QMap<int, QPair<bool, QColor>> & FSSVGCaptionOption::GetColors()
{
    return m_ColorMap;
}

void FSSVGCaptionOption::OnReset()
{
    for (int i = 0; i < m_ColorMap.keys().size() && i < m_pItemList.size();i++)
    {
        m_ColorMap[i] = {true,m_pItemList.at(i)->GetDedaultColor()};
    }
    emit ParmaChanged();
}

void FSSVGCaptionOption::RetranslateUi()
{
    for (int i = 0; i < m_pItemList.size(); i++)
    {
       m_pItemList.at(i)->RetranslateUi();
    }
}


void FSSVGCaptionOption::UpdataUI(int count)
{
    if (m_pItemList.size() > count)
    {
        for (int i = 0; i < m_pItemList.size(); i++)
        {
            m_pItemList.at(i)->setVisible(i < count ? true : false);
        }
    }
    else if (m_pItemList.size() <= count)
    {
        for (int i = 0; i < m_pItemList.size(); i++)
        {
            m_pItemList.at(i)->setVisible(true);
        }
        for (int i = m_pItemList.size(); i < count; i++)
        {
            auto item = new FSCaptionSvgItem(this);
            m_pVL->addWidget(item);
            m_pItemList.push_back(item);
            connect(item, &FSCaptionSvgItem::CurrentColorChanged, this, [&](QColor color, int index){
                if (m_ColorMap.contains(index))
                {
                    m_ColorMap[index] = { true, color };
                    emit ParmaChanged();
                }
            });
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FSCaptionSvgItem::FSCaptionSvgItem(QWidget *pParent)
    :FSBaseWidget(pParent)
{
    CreareUI();
    RetranslateUi();
}

FSCaptionSvgItem::~FSCaptionSvgItem()
{

}

QColor FSCaptionSvgItem::GetCurrentColor()
{
    return m_pColorSelecter->GetCurrentColor();
}

void FSCaptionSvgItem::SetCurrentColor(QColor color)
{
    m_pColorSelecter->SetCurrentColor(color);
    m_pButtonColorReset->setVisible(IsResetable());
}

bool FSCaptionSvgItem::IsResetable()
{
    return m_DefaultColor != m_pColorSelecter->GetCurrentColor();
}

void FSCaptionSvgItem::SetIndex(int index)
{
    m_IndexColor = index;
}

int FSCaptionSvgItem::GetIndex()
{
    return m_IndexColor;
}

void FSCaptionSvgItem::SetDedaultColor(QColor color)
{
    m_DefaultColor = color;
}

QColor FSCaptionSvgItem::GetDedaultColor()
{
    return m_DefaultColor;
}

void FSCaptionSvgItem::RetranslateUi()
{
    m_pLabelColor->setText(tr("Color"));
}

void FSCaptionSvgItem::CreareUI()
{
    m_pColorSelecter    = new ColorSelecterComboBox(this);
    m_pButtonColorReset = new FSButton(this);
    m_pLabelColor       = new FSLabel(this);

    int w = 60, h = 20, space = 10, L = 14, R = 21, btnW = 14;

    QHBoxLayout *pHL = new QHBoxLayout(this);
    QHBoxLayout *pHL1 = new QHBoxLayout();
    QHBoxLayout *pHL2 = new QHBoxLayout();

    pHL1->addStretch();
    pHL1->addWidget(m_pLabelColor);
    pHL1->setSpacing(0);
    pHL1->setMargin(0);

    pHL2->addWidget(m_pColorSelecter);
    pHL2->addStretch();
    pHL2->addWidget(m_pButtonColorReset);
    pHL2->setSpacing(0);
    pHL2->setMargin(0);


    pHL->addLayout(pHL1);
    pHL->addLayout(pHL2);
    pHL->setSpacing(space);
    pHL->setContentsMargins(L, 5, R, 5);

    m_pLabelColor->setFixedSize(w, h);
    m_pColorSelecter->setFixedSize(w, h);
    m_pButtonColorReset->setFixedSize(btnW, btnW);

    m_pLabelColor->setAlignment(Qt::AlignVCenter | Qt::AlignRight);

    m_pButtonColorReset->setObjectName("FSPropertyTitle_m_pResetBtn");
    m_pLabelColor->setObjectName("FSDenoiseOption_m_pLevelLable");

    connect(m_pButtonColorReset, &FSButton::clicked, this, [&](){
        m_pColorSelecter->SetCurrentColor(m_DefaultColor);
        m_pButtonColorReset->setVisible(false);
        emit CurrentColorChanged(m_DefaultColor,m_IndexColor);
    });

    connect(m_pColorSelecter, &ColorSelecterComboBox::CurrentColorChanged, this, [&](QColor color){
        m_pButtonColorReset->setVisible(color != m_DefaultColor);
        emit CurrentColorChanged(color,m_IndexColor);
    });
}
