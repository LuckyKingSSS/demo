#include "stdafx.h"
#include "FPIPEditBaseWidget.h"
#include "FPropertySlider.h"
#include "inc_MediaPlayerView/MediaPlayerControl.h"
#include "inc_MediaPlayerView/MediaPlayerView.h"
#include "inc_CommonLib/ColorSelecterComboBox.h"
#include "inc_FProject/FNLETimeline.h"
#include "inc_CommonLib/externalapi.h"  // test
#include <QStackedWidget>
#include <QPainter>


/*MaskPanel*/
MaskPanel::MaskPanel(QWidget *parent /*= 0*/)
	: FBaseWidget(parent)
	, m_CurrentMaskId(-1)
{
	m_pListWidget = new FListWidget(this);
	m_pListWidget->setViewMode(QListView::IconMode);
	m_pListWidget->setSpacing(20);
	m_pListWidget->setResizeMode(QListView::Adjust);
	m_pWidth = new FPropertySlider(this);
	m_pWidth->SetSliderObjectName("FVideoPanel_Slider");
	m_pHeight = new FPropertySlider(this);
	m_pHeight->SetSliderObjectName("FVideoPanel_Slider");
	m_pLeft = new FPropertySlider(this);
	m_pLeft->SetSliderObjectName("FVideoPanel_Slider");
	m_pTop = new FPropertySlider(this);
	m_pTop->SetSliderObjectName("FVideoPanel_Slider");

	Init();
	ConnectSignals();
}

MaskPanel::~MaskPanel()
{

}

void MaskPanel::SetMaskThumbFile(QStringList files)
{
	int id = 0;
	for (auto file : files){
		QListWidgetItem *pItem = new QListWidgetItem();
		pItem->setSizeHint(QSize(80, 70));
		pItem->setToolTip(tr("Double click to apply"));
		m_pListWidget->addItem(pItem);
		MaskWidgetItem *pItemWidget = new MaskWidgetItem(id++, file, m_pListWidget);
		connect(pItemWidget, &MaskWidgetItem::sigItemClicked, this, &MaskPanel::slotItemClicked);
		m_lstItems.append(pItemWidget);
		m_pListWidget->setItemWidget(pItem, pItemWidget);
	}
}

void MaskPanel::resizeEvent(QResizeEvent *)
{
	int left, top, right;
	left = 5, top = 0, right = 0;
	const int SliderHeight = 20;
	m_pListWidget->setGeometry(left, top, width() - left - right, height() - 200);
	top = m_pListWidget->geometry().bottom() + 20;
	left = 0;
	m_pWidth->setGeometry(left, top, width() - left - right, SliderHeight);
	top = m_pWidth->geometry().bottom() + 5;
	m_pHeight->setGeometry(left, top, width() - left - right, SliderHeight);
	top = m_pHeight->geometry().bottom() + 5;
	m_pLeft->setGeometry(left, top, width() - left - right, SliderHeight);
	top = m_pLeft->geometry().bottom() + 5;
	m_pTop->setGeometry(left, top, width() - left - right, SliderHeight);
}

void MaskPanel::Init()
{
	m_pWidth->SetAlignmentOfProperty(Qt::AlignRight);
	m_pHeight->SetAlignmentOfProperty(Qt::AlignRight);
	m_pLeft->SetAlignmentOfProperty(Qt::AlignRight);
	m_pTop->SetAlignmentOfProperty(Qt::AlignRight);
	m_pWidth->SetPropertyName(tr("Width:"));
	m_pHeight->SetPropertyName(tr("Height:"));
	m_pLeft->SetPropertyName(tr("Left:"));
	m_pTop->SetPropertyName(tr("Top:"));
	m_pWidth->SetRange(0, 1);
	m_pWidth->SetDecimalPlaces(2);
	m_pHeight->SetRange(0, 1);
	m_pHeight->SetDecimalPlaces(2);
	m_pLeft->SetDecimalPlaces(2);
	m_pTop->SetDecimalPlaces(2);
	m_pWidth->SetPropertyWidth(60);
	m_pHeight->SetPropertyWidth(60);
	m_pLeft->SetPropertyWidth(60);
	m_pTop->SetPropertyWidth(60);
	m_pWidth->SetSliderWidth(180);
	m_pHeight->SetSliderWidth(180);
	m_pLeft->SetSliderWidth(180);
	m_pTop->SetSliderWidth(180);
}

void MaskPanel::ConnectSignals()
{
	connect(m_pListWidget, &QListWidget::itemDoubleClicked, this, &MaskPanel::slotItemDoubleClicked);
	connect(m_pWidth, &FPropertySlider::sigValueChanged, this, &MaskPanel::sigWidthChanged);
	connect(m_pHeight, &FPropertySlider::sigValueChanged, this, &MaskPanel::sigHeightChanged);
	connect(m_pLeft, &FPropertySlider::sigValueChanged, this, &MaskPanel::sigLeftChanged);
	connect(m_pTop, &FPropertySlider::sigValueChanged, this, &MaskPanel::sigTopChanged);
}

void MaskPanel::slotItemClicked(int id)
{
	if (m_CurrentMaskId != id){
		if (m_CurrentMaskId != -1) m_lstItems[m_CurrentMaskId]->UnSelect();
		m_CurrentMaskId = id;
		m_lstItems[m_CurrentMaskId]->Select();
	}
}

void MaskPanel::slotItemDoubleClicked(QListWidgetItem* pItem)
{
	emit sigItemDoubleClicked(m_pListWidget->row(pItem));
}


/*MaskWidgetItem*/
MaskWidgetItem::MaskWidgetItem(int id /*= 0*/, QString ImagePath /*= ""*/, QWidget *parent /*= nullptr*/)
	:FBaseWidget(parent)
	, m_Id(id)
	, m_State(UnSelected)
	, m_BGImagePath(ImagePath)
{

}

MaskWidgetItem::~MaskWidgetItem()
{

}

int MaskWidgetItem::GetID()
{
	return m_Id;
}

void MaskWidgetItem::Select()
{
	m_State = Selected;
	update();
}

void MaskWidgetItem::UnSelect()
{
	m_State = UnSelected;
	update();
}

void MaskWidgetItem::paintEvent(QPaintEvent *)
{
	QPainter p(this);
	// »æÖÆ±³¾°Í¼
	p.drawPixmap(rect(), QPixmap(m_BGImagePath));
	// »æÖÆ±ß¿ò
	if (m_State == Selected){
		QPen pen;
		pen.setColor(Qt::red);
		pen.setWidth(2);
		p.setPen(pen);
		p.drawRect(QRect(1, 1, width() - 2, height() - 2));
	}
}

void MaskWidgetItem::mousePressEvent(QMouseEvent *e)
{
	FBaseWidget::mousePressEvent(e);
	m_State = Selected;
	emit sigItemClicked(m_Id);
	update();
}


/*FDirectionWidget*/
FDirectionWidget::FDirectionWidget(int id /*= 0*/, QString imagepath /*= ""*/, QWidget *parent /*= 0*/)
	:FBaseWidget(parent)
	, m_Id(id)
	, m_State(UnSelected)
	, m_ImagePath(imagepath)
{

}

FDirectionWidget::~FDirectionWidget()
{

}

void FDirectionWidget::SetState(State s)
{
	if (m_State != s){
		m_State = s;
		emit StateChanged(m_State);
		update();
	}
}

void FDirectionWidget::ClickButton()
{
	SetState(Selected);
	emit Clicked(m_Id);
}

int FDirectionWidget::GetId()
{
	return m_Id;
}

void FDirectionWidget::paintEvent(QPaintEvent *)
{
	QPainter p(this);
	if (m_State == Selected){
		p.drawPixmap(QRect(2, 2, width() - 4, height() - 4), QPixmap(m_ImagePath));
	}
}

void FDirectionWidget::mousePressEvent(QMouseEvent *)
{
	if (m_State == UnSelected){
		SetState(Selected);
		emit Clicked(m_Id);
	}
}

void FDirectionWidget::enterEvent(QEvent *)
{
	setCursor(Qt::PointingHandCursor);
}


/*FDirectionPanel*/
FDirectionPanel::FDirectionPanel(QWidget *parent /*= 0*/)
	:FBaseWidget(parent)
	, m_CurrentDirection(D_TOP)
{
	FDirectionWidget *pDW = nullptr;
	m_lstBtns.append(pDW = new FDirectionWidget(D_LEFT_TOP, ":/FPropertyPanel/Resources/D_Left_Top.png", this));
	pDW->setObjectName(tr("FDirectionWidget_D_LEFT_TOP"));
	m_lstBtns.append(pDW = new FDirectionWidget(D_TOP, ":/FPropertyPanel/Resources/D_Top.png", this));
	pDW->setObjectName(tr("FDirectionWidget_D_TOP"));
	m_lstBtns.append(pDW = new FDirectionWidget(D_RIGHT_TOP, ":/FPropertyPanel/Resources/D_Right_Top.png", this));
	pDW->setObjectName(tr("FDirectionWidget_D_RIGHT_TOP"));
	m_lstBtns.append(pDW = new FDirectionWidget(D_LEFT, ":/FPropertyPanel/Resources/D_Left.png", this));
	pDW->setObjectName(tr("FDirectionWidget_D_LEFT"));
	m_lstBtns.append(pDW = new FDirectionWidget(D_RIGHT, ":/FPropertyPanel/Resources/D_Right.png", this));
	pDW->setObjectName(tr("FDirectionWidget_D_RIGHT"));
	m_lstBtns.append(pDW = new FDirectionWidget(D_LEFT_DOWN, ":/FPropertyPanel/Resources/D_Left_Down.png", this));
	pDW->setObjectName(tr("FDirectionWidget_D_LEFT_DOWN"));
	m_lstBtns.append(pDW = new FDirectionWidget(D_DOWN, ":/FPropertyPanel/Resources/D_Down.png", this));
	pDW->setObjectName(tr("FDirectionWidget_D_DOWN"));
	m_lstBtns.append(pDW = new FDirectionWidget(D_RIGHT_DOWN, ":/FPropertyPanel/Resources/D_Right_Down.png", this));
	pDW->setObjectName(tr("FDirectionWidget_D_RIGHT_DOWN"));

	for (auto pBtn : m_lstBtns){
		connect(pBtn, &FDirectionWidget::Clicked, this, [&](int id){
			slotDirectionChanged(DIRECTION(id));
		});
	}
}

FDirectionPanel::~FDirectionPanel()
{

}

void FDirectionPanel::SetDirection(DIRECTION d)
{
	slotDirectionChanged(d);
}

DIRECTION FDirectionPanel::GetDirection()
{
	return m_CurrentDirection;
}

void FDirectionPanel::resizeEvent(QResizeEvent *)
{
	int left, top;
	left = top = 0;
	const float W = 1.0*(width() - 2) / 3;
	const float H = 1.0*(height() - 2) / 3;
	m_lstBtns[0]->setGeometry(left, top, W, H);
	left = m_lstBtns[0]->geometry().right() + 2;
	m_lstBtns[1]->setGeometry(left, top, W, H);
	left = m_lstBtns[1]->geometry().right() + 2;
	m_lstBtns[2]->setGeometry(left, top, W, H);
	left = 0;
	top = m_lstBtns[0]->geometry().bottom() + 2;
	m_lstBtns[3]->setGeometry(left, top, W, H);
	left = m_lstBtns[3]->geometry().right() + W + 3;
	m_lstBtns[4]->setGeometry(left, top, W, H);
	left = 0;
	top = m_lstBtns[3]->geometry().bottom() + 2;
	m_lstBtns[5]->setGeometry(left, top, W, H);
	left = m_lstBtns[5]->geometry().right() + 2;
	m_lstBtns[6]->setGeometry(left, top, W, H);
	left = m_lstBtns[6]->geometry().right() + 2;
	m_lstBtns[7]->setGeometry(left, top, W, H);
}

void FDirectionPanel::slotDirectionChanged(DIRECTION d)
{
	if (d != m_CurrentDirection){
		m_CurrentDirection = d;
		for (auto pBtn : m_lstBtns){
			if (pBtn->GetId() != m_CurrentDirection) pBtn->SetState(FDirectionWidget::UnSelected);
			else pBtn->SetState(FDirectionWidget::Selected);
		}
		emit sigDirectionChanged(m_CurrentDirection);
	}
}


/*FClickShowWidget*/
FClickShowWidget::FClickShowWidget(int id /*= 0*/, QWidget *parent /*= 0*/)
	:FBaseWidget(parent)
	, m_id(id)
	, m_state(State::Closed_State)
	, m_titleHeight(25)
	, m_bottomHeight(300)
	, m_pShowWidget(0)
{
	setAttribute(Qt::WA_StyledBackground);
	m_pTextLbl = new FLabel(this);
	m_pStackedWidget = new QStackedWidget(this);
}

FClickShowWidget::~FClickShowWidget()
{

}

void FClickShowWidget::SetShowWidget(QWidget* pWidget)
{
	m_pShowWidget = pWidget;
	m_pStackedWidget->addWidget(m_pShowWidget);
}

QWidget *FClickShowWidget::GetShowWidget()
{
	return m_pShowWidget;
}

void FClickShowWidget::SetTextAlignment(Qt::Alignment align)
{
	m_pTextLbl->setAlignment(align);
}

void FClickShowWidget::SetText(QString text)
{
	m_pTextLbl->setText(text);
}

bool FClickShowWidget::Opened()
{
	return m_state == Opened_State;
}

void FClickShowWidget::SetState(State s)
{
	m_state = s;
	update();
}

void FClickShowWidget::SetTitleHeight(int h)
{
	m_titleHeight = h;
	resizeEvent(nullptr);
}

void FClickShowWidget::SetBottomHeight(int h)
{
	m_bottomHeight = h;
	resizeEvent(nullptr);
}

int FClickShowWidget::Height()
{
	if (Opened()){
		return m_titleHeight + 2 + m_bottomHeight;
	}
	return m_titleHeight;
}

void FClickShowWidget::resizeEvent(QResizeEvent *)
{
	int left, top;
	left = top = 0;
	m_pTextLbl->setGeometry(left, top, width() - 25, m_titleHeight);
	top = m_pTextLbl->geometry().bottom() + 2;
	m_pStackedWidget->setGeometry(left, top, width(), m_bottomHeight);
}

void FClickShowWidget::paintEvent(QPaintEvent *)
{
	QPainter p(this);
	if (m_state == Opened_State){

	}
	else{

	}
}

void FClickShowWidget::mouseReleaseEvent(QMouseEvent *event)
{
	QRect r(0, 0, width(), m_titleHeight);
	if (r.contains(event->pos())) emit Clicked(m_id);
}


/*FChromaKeyPanel*/
FChromaKeyPanel::FChromaKeyPanel(QWidget *parent /*= 0*/)
	:FBaseWidget(parent)
{
	setAttribute(Qt::WA_StyledBackground);
	m_pCheckBox = new FCheckBox(this);
	m_pColorLbl = new FLabel(this);
	m_pColorShowLbl = new FLabel(this);
	m_pIntensityLbl = new FLabel(this);
	m_pSlider = new FPropertySlider(this);
	m_pSlider->SetSliderObjectName("FVideoPanel_Slider");
	m_pSlider->SetRange(1, 100);
	m_pInfoLbl = new FLabel(this);
	m_pInfoLbl->setWordWrap(true);
	m_pSlider->SetPropertyName("");
	m_pSlider->SetPropertyWidth(0);
	m_pSlider->SetSliderWidth(160);
	connect(m_pCheckBox, &FCheckBox::stateChanged, this, &FChromaKeyPanel::slotStateChanged);
	connect(m_pSlider, &FPropertySlider::sigValueChanged, this, &FChromaKeyPanel::sigIntensityLevelChanged);
	slotStateChanged(m_pCheckBox->checkState());
	RetranslateUi();
}

FChromaKeyPanel::~FChromaKeyPanel()
{

}

void FChromaKeyPanel::RetranslateUi()
{
	m_pCheckBox->setText(tr("Make parts of the clip transparent"));
	m_pColorLbl->setText(tr("Current Color:"));
	m_pIntensityLbl->setText(tr("Intensity Level:"));
	m_pInfoLbl->setText(tr("Clip the color you want to make tansparent, then drag or adjust the intensity level. As you drag, the selection grows to include areas that use the similar colors."));
}

void FChromaKeyPanel::resizeEvent(QResizeEvent *)
{
	int left, top;
	left = top = 0;
	m_pCheckBox->setGeometry(left, top, width(), 25);
	top = m_pCheckBox->geometry().bottom() + 5;
	left = 0.1*width();
	m_pColorLbl->setGeometry(left, top, 90, 30);
	left = m_pColorLbl->geometry().right() + 15;
	m_pColorShowLbl->setGeometry(left, top, 90, 30);
	left = 0.1*width();
	top = m_pColorLbl->geometry().bottom() + 20;
	m_pIntensityLbl->setGeometry(left, top, 100, 30);
	top = m_pIntensityLbl->geometry().bottom() + 5;
	left = 0;
	m_pSlider->setGeometry(left, top, width() - left, 20);
	top = m_pSlider->geometry().bottom() + 40;
	left = 30;
	m_pInfoLbl->setGeometry(left, top, width() - left - 20, 80);
}

void FChromaKeyPanel::slotStateChanged(int checked)
{
	m_pColorLbl->setEnabled(checked);
	m_pIntensityLbl->setEnabled(checked);
	m_pSlider->setEnabled(checked);
	m_pInfoLbl->setEnabled(checked);
}

/*FShadowPanel*/
FShadowPanel::FShadowPanel(QWidget *parent /*= 0*/)
	:FBaseWidget(parent)
{
	setAttribute(Qt::WA_StyledBackground);
	m_pCheckBox = new FCheckBox(this);
	m_pDistance = new FPropertySlider(this);
	m_pDistance->SetSliderObjectName("FVideoPanel_Slider");
	m_pDistance->SetRange(0, 30);
	m_pDistance->SetSliderWidth(150);
	m_pDistance->SetAlignmentOfProperty(Qt::AlignRight);
	m_pBlur = new FPropertySlider(this);
	m_pBlur->SetRange(0, 10);
	m_pBlur->SetSliderObjectName("FVideoPanel_Slider");
	m_pBlur->SetSliderWidth(150);
	m_pBlur->SetAlignmentOfProperty(Qt::AlignRight);
	m_pTransparency = new FPropertySlider(this);
	m_pTransparency->SetSliderObjectName("FVideoPanel_Slider");
	m_pTransparency->SetRange(0, 100);
	m_pTransparency->SetSliderWidth(150);
	m_pTransparency->SetAlignmentOfProperty(Qt::AlignRight);
	m_pShadowColorLbl = new FLabel(this);
	m_pColorSelecter = new ColorSelecterComboBox(this);
	m_pShadowDirectionLbl = new FLabel(this);
	m_pShadowDirection = new FDirectionPanel(this);
	m_pShadowDirection->setObjectName("DirectionPanel");

	ConnectSignals();
	slotStateChanged(m_pCheckBox->checkState());
	RetranslateUi();
}

FShadowPanel::~FShadowPanel()
{

}

void FShadowPanel::RetranslateUi()
{
	m_pCheckBox->setText(tr("Apply Shadow to the clip"));
	m_pDistance->SetPropertyName(tr("Distance:"));
	m_pBlur->SetPropertyName(tr("Blur:"));
	m_pTransparency->SetPropertyName(tr("Transparency:"));
	m_pShadowColorLbl->setText(tr("Shadow Color:"));
	m_pShadowDirectionLbl->setText(tr("Shadow Direction:"));
}

void FShadowPanel::resizeEvent(QResizeEvent *)
{
	int left, top;
	left = top = 0;
	m_pCheckBox->setGeometry(left, top, width() - left, 25);
	top = m_pCheckBox->geometry().bottom() + 10;
	left = 10;
	m_pDistance->setGeometry(left, top, width() - left, 20);
	top = m_pDistance->geometry().bottom() + 10;
	m_pBlur->setGeometry(left, top, width() - left, 20);
	top = m_pBlur->geometry().bottom() + 10;
	m_pTransparency->setGeometry(left, top, width() - left, 20);
	top = m_pTransparency->geometry().bottom() + 20;
	m_pShadowColorLbl->setGeometry(left, top, 80, 20);
	left = m_pShadowColorLbl->geometry().right() + 5;
	m_pColorSelecter->setGeometry(left, top, 140, 20);
	left = m_pShadowColorLbl->geometry().left();
	top = m_pShadowColorLbl->geometry().bottom() + 20;
	m_pShadowDirectionLbl->setGeometry(left, top, 110, 20);
	left = m_pShadowDirectionLbl->geometry().right() + 50;
	m_pShadowDirection->setGeometry(left, top, 77, 77);
}

void FShadowPanel::ConnectSignals()
{
	connect(m_pCheckBox, &FCheckBox::stateChanged, this, &FShadowPanel::slotStateChanged);
	connect(m_pDistance, &FPropertySlider::sigValueChanged, this, &FShadowPanel::sigDistanceChanged);
	connect(m_pBlur, &FPropertySlider::sigValueChanged, this, &FShadowPanel::sigBlurChanged);
	connect(m_pTransparency, &FPropertySlider::sigValueChanged, this, &FShadowPanel::sigTransparencyChanged);
	connect(m_pColorSelecter, &ColorSelecterComboBox::CurrentColorChanged, this, &FShadowPanel::sigShadowColorChanged);
	connect(m_pShadowDirection, &FDirectionPanel::sigDirectionChanged, this, &FShadowPanel::sigShadowDirectionChanged);
}

void FShadowPanel::slotStateChanged(int checked)
{
	m_pDistance->setEnabled(checked);
	m_pBlur->setEnabled(checked);
	m_pTransparency->setEnabled(checked);
	m_pShadowColorLbl->setEnabled(checked);
	m_pColorSelecter->setEnabled(checked);
	m_pShadowDirectionLbl->setEnabled(checked);
	m_pShadowDirection->setEnabled(checked);
}


/*FBorderPanel*/
FBorderPanel::FBorderPanel(QWidget *parent /*= 0*/)
	:FBaseWidget(parent)
{
	setAttribute(Qt::WA_StyledBackground);
	m_pCheckBox = new FCheckBox(this);
	m_pDistance = new FPropertySlider(this);
	m_pDistance->SetSliderObjectName("FVideoPanel_Slider");
	m_pDistance->SetRange(0, 10);
	m_pDistance->SetSliderWidth(150);
	m_pDistance->SetAlignmentOfProperty(Qt::AlignRight);
	m_pBlur = new FPropertySlider(this);
	m_pBlur->SetSliderObjectName("FVideoPanel_Slider");
	m_pBlur->SetRange(0, 10);
	m_pBlur->SetSliderWidth(150);
	m_pBlur->SetAlignmentOfProperty(Qt::AlignRight);
	m_pTransparency = new FPropertySlider(this);
	m_pTransparency->SetSliderObjectName("FVideoPanel_Slider");
	m_pTransparency->SetRange(0, 100);
	m_pTransparency->SetSliderWidth(150);
	m_pTransparency->SetAlignmentOfProperty(Qt::AlignRight);
	m_pGradientColorLbl = new FLabel(this);
	m_pStartLbl = new FLabel(this);
	m_pStartLbl->setAlignment(Qt::AlignRight);
	m_pStartColorSelecter = new ColorSelecterComboBox(this);
	m_pEndLbl = new FLabel(this);
	m_pEndLbl->setAlignment(Qt::AlignRight);
	m_pEndColorSelecter = new ColorSelecterComboBox(this);
	m_pGradientDirectionLbl = new FLabel(this);
	m_pGradientDirection = new FDirectionPanel(this);
	m_pGradientDirection->setObjectName("DirectionPanel");

	ConnectSignals();
	slotStateChanged(m_pCheckBox->checkState());
	RetranslateUi();
}

FBorderPanel::~FBorderPanel()
{

}

void FBorderPanel::RetranslateUi()
{
	m_pCheckBox->setText(tr("Apply Shadow to the clip"));
	m_pDistance->SetPropertyName(tr("Distance:"));
	m_pBlur->SetPropertyName(tr("Blur:"));
	m_pTransparency->SetPropertyName(tr("Transparency:"));
	m_pGradientColorLbl->setText(tr("Gradient Color:"));
	m_pGradientDirectionLbl->setText(tr("Gradient Direction:"));
	m_pStartLbl->setText(tr("Start:"));
	m_pEndLbl->setText(tr("End:"));
}

void FBorderPanel::resizeEvent(QResizeEvent *)
{
	int left, top;
	left = top = 0;
	m_pCheckBox->setGeometry(left, top, width() - left, 25);
	top = m_pCheckBox->geometry().bottom() + 10;
	m_pDistance->setGeometry(left, top, width() - left, 20);
	top = m_pDistance->geometry().bottom() + 10;
	m_pBlur->setGeometry(left, top, width() - left, 20);
	top = m_pBlur->geometry().bottom() + 10;
	m_pTransparency->setGeometry(left, top, width() - left, 20);
	top = m_pTransparency->geometry().bottom() + 20;
	left = 10;
	m_pGradientColorLbl->setGeometry(left, top, 100, 20);
	top = m_pGradientColorLbl->geometry().bottom() + 5;
	left = 20;
	m_pStartLbl->setGeometry(left, top, 40, 20);
	left = m_pStartLbl->geometry().right() + 2;
	m_pStartColorSelecter->setGeometry(left, top, 60, 20);
	left = m_pStartColorSelecter->geometry().right() + 20;
	m_pEndLbl->setGeometry(left, top, 40, 20);
	left = m_pEndLbl->geometry().right() + 2;
	m_pEndColorSelecter->setGeometry(left, top, 60, 20);
	left = 10;
	top = m_pStartLbl->geometry().bottom() + 20;
	m_pGradientDirectionLbl->setGeometry(left, top, 120, 20);
	left = m_pGradientDirectionLbl->geometry().right() + 50;
	m_pGradientDirection->setGeometry(left, top, 77, 77);
}

void FBorderPanel::ConnectSignals()
{
	connect(m_pCheckBox, &FCheckBox::stateChanged, this, &FBorderPanel::slotStateChanged);
	connect(m_pDistance, &FPropertySlider::sigValueChanged, this, &FBorderPanel::sigDistanceChanged);
	connect(m_pBlur, &FPropertySlider::sigValueChanged, this, &FBorderPanel::sigBlurChanged);
	connect(m_pTransparency, &FPropertySlider::sigValueChanged, this, &FBorderPanel::sigTransparencyChanged);
	connect(m_pStartColorSelecter, &ColorSelecterComboBox::CurrentColorChanged, this, &FBorderPanel::sigGradientColorStartChanged);
	connect(m_pEndColorSelecter, &ColorSelecterComboBox::CurrentColorChanged, this, &FBorderPanel::sigGradientColorEndChanged);
	connect(m_pGradientDirection, &FDirectionPanel::sigDirectionChanged, this, &FBorderPanel::sigGradientDirectionChanged);
}

void FBorderPanel::slotStateChanged(int checked)
{
	m_pDistance->setEnabled(checked);
	m_pBlur->setEnabled(checked);
	m_pTransparency->setEnabled(checked);
	m_pGradientColorLbl->setEnabled(checked);
	m_pStartLbl->setEnabled(checked);
	m_pStartColorSelecter->setEnabled(checked);
	m_pEndLbl->setEnabled(checked);
	m_pEndColorSelecter->setEnabled(checked);
	m_pGradientDirectionLbl->setEnabled(checked);
	m_pGradientDirection->setEnabled(checked);
}

/*EffectPanel*/
EffectPanel::EffectPanel(QWidget *parent /*= 0*/)
	:FBaseWidget(parent)
	, m_currentId(0)
{
	FClickShowWidget *pItems = 0;
	m_lstItems.append(pItems = new FClickShowWidget(0, this));
	pItems->SetTextAlignment(Qt::AlignLeft);
	pItems->SetText(tr("Chroma Key (Green Screen)"));
	m_lstItems.append(pItems = new FClickShowWidget(1, this));
	pItems->SetTextAlignment(Qt::AlignLeft);
	pItems->SetText(tr("Shadow"));
	m_lstItems.append(pItems = new FClickShowWidget(2, this));
	pItems->SetTextAlignment(Qt::AlignLeft);
	pItems->SetText(tr("Border"));
	m_lstItems.append(pItems = new FClickShowWidget(3, this));
	pItems->SetTextAlignment(Qt::AlignLeft);
	pItems->SetText(tr("Flip"));

	m_pChromaKeyPanel = new FChromaKeyPanel(0);
	m_lstItems[0]->SetShowWidget(m_pChromaKeyPanel);
	m_pShadowPanel = new FShadowPanel(0);
	m_lstItems[1]->SetShowWidget(m_pShadowPanel);
	m_pBorderPanel = new FBorderPanel(0);
	m_lstItems[2]->SetShowWidget(m_pBorderPanel);
	m_pRotatePanel = new FPropertyRotate(0);
	m_lstItems[3]->SetBottomHeight(40);
	m_lstItems[3]->SetShowWidget(m_pRotatePanel);

	ConnectSignals();

	OpenItem(m_currentId);
}

EffectPanel::~EffectPanel()
{
	//delete m_pChromaKeyPanel;
	//delete m_pShadowPanel;
	//delete m_pBorderPanel;
	//delete m_pRotatePanel;
}

void EffectPanel::OpenItem(int id)
{
	m_currentId = qBound(0, id, m_lstItems.count() - 1);
	for (int i = 0; i < m_currentId; ++i) {
		m_lstItems[i]->SetState(FClickShowWidget::Closed_State);
	}
	m_lstItems[m_currentId]->SetState(FClickShowWidget::Opened_State);
	for (int i = m_currentId + 1; i < m_lstItems.count(); ++i) {
		m_lstItems[i]->SetState(FClickShowWidget::Closed_State);
	}
	resizeEvent(nullptr);
}

void EffectPanel::resizeEvent(QResizeEvent *)
{
	int left, top;
	left = 10;
	top = 0;
	for (auto pItem : m_lstItems){
		pItem->setGeometry(left, top, width(), pItem->Height());
		top = pItem->geometry().bottom() + 2;
	}
}

void EffectPanel::ConnectSignals()
{
	for (auto pItem : m_lstItems){
		connect(pItem, &FClickShowWidget::Clicked, this, &EffectPanel::OpenItem);
	}
	connect(m_pChromaKeyPanel, &FChromaKeyPanel::sigIntensityLevelChanged, this, &EffectPanel::sigIntensityLevelChanged);
	connect(m_pShadowPanel, &FShadowPanel::sigDistanceChanged, this, &EffectPanel::sigShadowDistanceChanged);
	connect(m_pShadowPanel, &FShadowPanel::sigBlurChanged, this, &EffectPanel::sigShadowBlurChanged);
	connect(m_pShadowPanel, &FShadowPanel::sigTransparencyChanged, this, &EffectPanel::sigShadowTransparencyChanged);
	connect(m_pShadowPanel, &FShadowPanel::sigShadowColorChanged, this, &EffectPanel::sigShadowColorChanged);
	connect(m_pShadowPanel, &FShadowPanel::sigShadowDirectionChanged, this, &EffectPanel::sigShadowDirectionChanged);
	connect(m_pBorderPanel, &FBorderPanel::sigDistanceChanged, this, &EffectPanel::sigBorderDistanceChanged);
	connect(m_pBorderPanel, &FBorderPanel::sigBlurChanged, this, &EffectPanel::sigBorderBlurChanged);
	connect(m_pBorderPanel, &FBorderPanel::sigTransparencyChanged, this, &EffectPanel::sigBorderTransparencyChanged);
	connect(m_pBorderPanel, &FBorderPanel::sigGradientColorStartChanged, this, &EffectPanel::sigBorderGradientColorStartChanged);
	connect(m_pBorderPanel, &FBorderPanel::sigGradientColorEndChanged, this, &EffectPanel::sigBorderGradientColorEndChanged);
	connect(m_pBorderPanel, &FBorderPanel::sigGradientDirectionChanged, this, &EffectPanel::sigBorderGradientDirectionChanged);
	connect(m_pRotatePanel, &FPropertyRotate::sigRotateChanged, this, &EffectPanel::sigRotateChanged);
}

/*FPIPEditSettingPanel*/
FPIPEditSettingPanel::FPIPEditSettingPanel(QWidget *parent /*= 0*/)
	:FBaseWidget(parent)
{
	m_pMaskBtn = new FButton(this);
	m_pMaskBtn->setText(tr("Mask"));
	m_pEffectBtn = new FButton(this);
	m_pEffectBtn->setText(tr("Effect"));
	m_pStackedWidget = new QStackedWidget(this);
	m_pMaskPanel = new MaskPanel;
	m_pEffectPanel = new EffectPanel;
	m_pStackedWidget->addWidget(m_pMaskPanel);
	m_pStackedWidget->addWidget(m_pEffectPanel);

	ConnectSignals();
}

FPIPEditSettingPanel::~FPIPEditSettingPanel()
{

}

void FPIPEditSettingPanel::SetMaskThumbFile(QStringList files)
{
	m_pMaskPanel->SetMaskThumbFile(files);
}

void FPIPEditSettingPanel::resizeEvent(QResizeEvent *)
{
	int left, top;
	left = 20, top = 10;
	m_pMaskBtn->setGeometry(left, top, 50, 20);
	left = m_pMaskBtn->geometry().right() + 50;
	m_pEffectBtn->setGeometry(left, top, 50, 20);
	top = m_pMaskBtn->geometry().bottom() + 20;
	left = 0;
	m_pStackedWidget->setGeometry(left, top, width(), height() - top);
}

void FPIPEditSettingPanel::ConnectSignals()
{
	connect(m_pMaskBtn, &FButton::clicked, this, [&](){
		m_pStackedWidget->setCurrentWidget(m_pMaskPanel);
	});
	connect(m_pEffectBtn, &FButton::clicked, this, [&](){
		m_pStackedWidget->setCurrentWidget(m_pEffectPanel);
	});
	connect(m_pMaskPanel, &MaskPanel::sigItemDoubleClicked, this, &FPIPEditSettingPanel::sigMaskItemDoubleClicked);
	connect(m_pMaskPanel, &MaskPanel::sigWidthChanged, this, &FPIPEditSettingPanel::sigMaskWidthChanged);
	connect(m_pMaskPanel, &MaskPanel::sigHeightChanged, this, &FPIPEditSettingPanel::sigMaskHeightChanged);
	connect(m_pMaskPanel, &MaskPanel::sigLeftChanged, this, &FPIPEditSettingPanel::sigMaskLeftChanged);
	connect(m_pMaskPanel, &MaskPanel::sigTopChanged, this, &FPIPEditSettingPanel::sigMaskTopChanged);
	connect(m_pEffectPanel, &EffectPanel::sigIntensityLevelChanged, this, &FPIPEditSettingPanel::sigIntensityLevelChanged);
	connect(m_pEffectPanel, &EffectPanel::sigShadowDistanceChanged, this, &FPIPEditSettingPanel::sigShadowDistanceChanged);
	connect(m_pEffectPanel, &EffectPanel::sigShadowBlurChanged, this, &FPIPEditSettingPanel::sigShadowBlurChanged);
	connect(m_pEffectPanel, &EffectPanel::sigShadowTransparencyChanged, this, &FPIPEditSettingPanel::sigShadowTransparencyChanged);
	connect(m_pEffectPanel, &EffectPanel::sigShadowColorChanged, this, &FPIPEditSettingPanel::sigShadowColorChanged);
	connect(m_pEffectPanel, &EffectPanel::sigShadowDirectionChanged, this, &FPIPEditSettingPanel::sigShadowDirectionChanged);
	connect(m_pEffectPanel, &EffectPanel::sigBorderDistanceChanged, this, &FPIPEditSettingPanel::sigBorderDistanceChanged);
	connect(m_pEffectPanel, &EffectPanel::sigBorderBlurChanged, this, &FPIPEditSettingPanel::sigBorderBlurChanged);
	connect(m_pEffectPanel, &EffectPanel::sigBorderTransparencyChanged, this, &FPIPEditSettingPanel::sigBorderTransparencyChanged);
	connect(m_pEffectPanel, &EffectPanel::sigBorderGradientColorStartChanged, this, &FPIPEditSettingPanel::sigBorderGradientColorStartChanged);
	connect(m_pEffectPanel, &EffectPanel::sigBorderGradientColorEndChanged, this, &FPIPEditSettingPanel::sigBorderGradientColorEndChanged);
	connect(m_pEffectPanel, &EffectPanel::sigBorderGradientDirectionChanged, this, &FPIPEditSettingPanel::sigBorderGradientDirectionChanged);
	connect(m_pEffectPanel, &EffectPanel::sigRotateChanged, this, &FPIPEditSettingPanel::sigRotateChanged);
}

/*FPIPEditPlayer*/
FPIPEditPlayer::FPIPEditPlayer(QWidget *parent /*= 0*/)
	:FBaseWidget(parent)
	, m_bPlaying(false)
{
	m_pPlayerView = new MediaPlayerView(this);
	m_pPlayerCtrl = m_pPlayerView->GetMediaPlayerControl();
	m_pDurationSlider = new ToolBarSlider(this);
	m_pDurationSlider->setObjectName("FVideoPanel_Slider");
	m_pPlayBtn = new FStateButton(0, "FPIPEditPlayer_Play", this);
	m_pPlayBtn->AddButton(0, "FStateButton_Play");
	m_pPlayBtn->AddButton(1, "FStateButton_Pause");
	m_pStopBtn = new FImageButton("FImageButton_Stop", this);
	m_pOkBtn = new FButton(this);
	m_pCancelBtn = new FButton(this);
	ConnectSignals();
	RetranslateUi();
	setStyleSheet(gGetStyleSheet(":/FPropertyPanel/FPropertyPanel.css"));
}

FPIPEditPlayer::~FPIPEditPlayer()
{
	delete m_pPlayerCtrl;
}

bool FPIPEditPlayer::Open(int bSource, FNLETimeline *timeline)
{
	Q_ASSERT(timeline);
	m_pTimeline = timeline;
	m_pDurationSlider->setDuration(m_pTimeline->GetTotalLength());
	return m_pPlayerCtrl->Open(bSource, m_pTimeline);
}

void FPIPEditPlayer::Play()
{
	m_pPlayerCtrl->Play();
}

bool FPIPEditPlayer::IsPlaying()
{
	return m_pPlayerCtrl->IsPlaying();
}

void FPIPEditPlayer::Pause()
{
	m_pPlayerCtrl->Pause();
}

void FPIPEditPlayer::Stop()
{
	m_pPlayerCtrl->Stop();
}

void FPIPEditPlayer::Update()
{
	m_pPlayerCtrl->Update();
}

void FPIPEditPlayer::resizeEvent(QResizeEvent *)
{
	int left, top, right;
	left = 10, right = 10;
	top = 0.15*height();
	m_pPlayerView->setGeometry(left, top, width() - left - right, 0.5*height());
	top = 0.75*height();
	m_pDurationSlider->setGeometry(left, top, width() - left - right, 15);
	top = m_pDurationSlider->geometry().bottom() + 5;
	m_pPlayBtn->setGeometry(left, top, 20, 20);
	left = m_pPlayBtn->geometry().right() + 10;
	m_pStopBtn->setGeometry(left, top, 20, 20);
	left = width() - 80;
	top = height() - 40;
	m_pCancelBtn->setGeometry(left, top, 70, 25);
	left = left - 80;
	m_pOkBtn->setGeometry(left, top, 70, 25);
}

void FPIPEditPlayer::RetranslateUi()
{
	m_pOkBtn->setText(tr("OK"));
	m_pCancelBtn->setText(tr("Cancel"));
}

void FPIPEditPlayer::ConnectSignals()
{
	connect(m_pDurationSlider, &ToolBarSlider::valueChanged, this, [&](double d){
		m_pPlayerCtrl->Seek(d, 0);
	});
	connect(m_pDurationSlider, &ToolBarSlider::startDrag, this, [&](){
		m_bPlaying = m_pPlayerCtrl->IsPlaying();
		m_pPlayerCtrl->Pause();
	});
	connect(m_pDurationSlider, &ToolBarSlider::endDrag, this, [&](){
		if (m_bPlaying){
			m_pPlayerCtrl->Play();
		}
	});
	connect(m_pPlayerCtrl, &IMediaPlayerControl::PlayingCallback, this, [&](int frames){
		m_pDurationSlider->setValue(frames);
	});
	connect(m_pPlayerCtrl, &IMediaPlayerControl::PlayFinishCallback, this, [&](){
		m_pPlayerCtrl->Stop();
		m_pDurationSlider->setValue(0);
		if (m_pPlayBtn->CurrentButtonId() == 1) m_pPlayBtn->ClickCurrentButton();
	});
	connect(m_pPlayBtn, &FStateButton::Clicked, this, [&](int id, QPoint p){
		if (id == 0) m_pPlayerCtrl->Play();
		else if (id == 1) m_pPlayerCtrl->Pause();
	});
	connect(m_pStopBtn, &FImageButton::Clicked, this, [&](){
		m_pPlayerCtrl->Stop();
		m_pDurationSlider->setValue(0);
		if (m_pPlayBtn->CurrentButtonId() == 1) m_pPlayBtn->ClickCurrentButton();
	});
	connect(m_pOkBtn, &FButton::clicked, this, &FPIPEditPlayer::sigOk);
	connect(m_pCancelBtn, &FButton::clicked, this, &FPIPEditPlayer::sigCancel);
}


