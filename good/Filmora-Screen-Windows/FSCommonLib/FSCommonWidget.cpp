#include "stdafx.h"
#include "inc_FSCommonlib/FSCommonWidget.h"
#include "inc_CommonLib/TopLevelWindowTitleBar.h"
#include "inc_CommonLib/StringOperation.h"
#include "inc_FSCommonlib/ExternalAPI.h"
#include "inc_CommonLib/DragValueChangedHelper.h"

FSBaseWidget::FSBaseWidget(QWidget *parent)
	: QWidget(parent)
{
	setAttribute(Qt::WA_StyledBackground);
	
}

void FSBaseWidget::RetranslateUi()
{

}

void FSBaseWidget::TranslateText()
{

}

bool FSBaseWidget::event(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		RetranslateUi();
	}
	return QWidget::event(e);
}


/**************************FSBaseDialog**************************/
FSBaseDialog::FSBaseDialog(const QString &trtitle, const char *title, QWidget *parent)
:BaseDialog(trtitle, parent)
, m_title(title)
{
	Init();
}


FSBaseDialog::FSBaseDialog(const QString &title, QWidget *parent)
:BaseDialog(title, parent)
{
	Init();
}

void FSBaseDialog::RetranslateUi()
{

}

void FSBaseDialog::resizeEvent(QResizeEvent *event)
{
	const int margins = 1;
	m_titlebar->setGeometry(margins, margins, width() - 2 * margins, m_titleHeight);
}

void FSBaseDialog::Init()
{
	m_titlebar->SetSystemBorderVisible(false);
	m_titlebar->GetControl()->SetBorderWidth(1);
	m_titlebar->GetControl()->SetBorderColor(QColor(102, 102, 102));
}

bool FSBaseDialog::event(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		BaseDialog::SetTitle(tr(m_title.c_str()));
		RetranslateUi();
	}
    return BaseDialog::event(e);
}


/**************************FSBaseButton**************************/
FSBaseButton::FSBaseButton(int id, const QString &objectname, QWidget *parent)
    :BaseButton(id,objectname,parent)
{
    installEventFilter(this);
}

FSBaseButton::FSBaseButton(const QString &objectname, QWidget *parent)
    : BaseButton(objectname,parent)
{
    installEventFilter(this);
}


bool FSBaseButton::eventFilter(QObject *obj, QEvent *e)
{
    if (obj == this&&e->type() == QEvent::MouseButtonPress)
    {
        emit pressed();
    }
    return false;
}

/**************************FSStateBaseButton**************************/
FSStateBaseButton::FSStateBaseButton(int id, const QString &objectname, QWidget *parent)
:BaseStateButton(id, objectname, parent)
{

}

//void FSStateBaseButton::AddButton( BaseButton *button)
//{
//	BaseStateButton::AddButton(button);
//}

/**************************FSListWidget**************************/
FSListWidget::FSListWidget(const QString &objName,QWidget *pParent)
    :QListWidget(pParent)
{
    setAttribute(Qt::WA_StyledBackground);
    setObjectName(objName);
}


/**************************FSLineEdit**************************/
FSLineEdit::FSLineEdit(const QString &objName,QWidget *pParent)
    :QLineEdit(pParent)
{
    connect(this, &QLineEdit::editingFinished, this, &QLineEdit::clearFocus);
    setObjectName(objName);
}

FSLineEdit::FSLineEdit(QWidget *pParent)
: FSLineEdit("", pParent)
{

}

void FSLineEdit::SetTextBlockSignal(const QString &text)
{
    blockSignals(true);
    setText(text);
    blockSignals(false);
}

bool FSLineEdit::eventFilter(QObject * watched, QEvent * event)
{
    if (watched != this)
    {
        if (event->type() == QEvent::MouseButtonPress)
        {
            this->editingFinished();
        }
    }
    return false;
}

void FSLineEdit::focusInEvent(QFocusEvent *event)
{

    qApp->installEventFilter(this);
    QLineEdit::focusInEvent(event);
}

void FSLineEdit::focusOutEvent(QFocusEvent *event)
{
    qApp->removeEventFilter(this);
    QLineEdit::focusOutEvent(event);

}
/**************************FSComboBox**************************/
FSComboBox::FSComboBox(const QString &objName, QWidget *pParent)
:QComboBox(pParent)
, m_bEnableWheel(true)
{
    setObjectName(objName);
    setAttribute(Qt::WA_StyledBackground);
    QListView *view = new QListView(this);
    view->setObjectName("FSCommonLib_FSComboBox_QListView");
    this->setView(view);
    m_autoFixedMenuWidth = true;
    setFocusPolicy(Qt::NoFocus);
}


FSComboBox::FSComboBox(QWidget *pParent)
    :QComboBox(pParent)
{
    setAttribute(Qt::WA_StyledBackground);
    QListView *view = new QListView(this);
    view->setObjectName("FSCommonLib_FSComboBox_QListView");
    this->setView(view);
    m_autoFixedMenuWidth = true;

}

void FSComboBox::EnableWheelEvent(bool bEnable)
{
    m_bEnableWheel = bEnable;
}

void FSComboBox::SetAutoFixedMenuWidth(bool enabled)
{
    m_autoFixedMenuWidth = enabled;
}



void FSComboBox::FixedMenuWidth()
{
    int count = this->count();
    int wTemp = width();
    for (int i = 0; i < count; ++i)
    {
        wTemp = qMax(wTemp, GetRetranslateUiTextLenght(view()->font(), itemText(i), wTemp - 20, 20));
    }
    //wTemp += 20;
    view()->setMinimumWidth(wTemp);

}

void FSComboBox::mousePressEvent(QMouseEvent *event)
{
    if (m_autoFixedMenuWidth)
        FixedMenuWidth();
    QComboBox::mousePressEvent(event);
}

void FSComboBox::wheelEvent(QWheelEvent *e)
{
    if (m_bEnableWheel)
    {
        QWheelEvent event(e->posF(),e->globalPosF(), -e->delta(),
            e->buttons(), e->modifiers(),
            e->orientation());
        QComboBox::wheelEvent(&event);
    }
        
}

//////////////////////////////////////////////////////////////////////////
FSButton::FSButton(QWidget *parent)
:QPushButton(parent)
{
    setAttribute(Qt::WA_StyledBackground);
    setFocusPolicy(Qt::NoFocus);
}

//////////////////////////////////////////////////////////////////////////


FSAction::FSAction(QObject *parent)
:QAction(parent)
{
}

//////////////////////////////////////////////////////////////////////////
FSMenu::FSMenu(QWidget *parent)
: QMenu(parent)
{
}

//////////////////////////////////////////////////////////////////////////
FSLabel::FSLabel(QWidget *parent)
: QLabel(parent)
{

}


//////////////////////////////////////////////////////////////////////////
FSMessageBox::FSMessageBox(const QString &title, const QString &text, Qt::AlignmentFlag flag, int buttons, QWidget *parent)
: FSBaseDialog(title, parent)
{
	m_lbIcon = new FSLabel(this);
	m_lbText = new QTextEdit(this);
	m_lbText->setWordWrapMode(QTextOption::WordWrap);
	m_lbText->setReadOnly(true);
	m_lbText->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	m_lbText->setFrameStyle(QFrame::NoFrame);
	//m_lbText->setTextInteractionFlags(Qt::NoTextInteraction);
	m_lbText->installEventFilter(this);
    m_lbText->setFixedHeight(78);
	m_titlebar->GetButton(TopLevelWindowTitleBar::Close)->setObjectName("TitleBar_CloseButton");
    m_titlebar->setFixedHeight(34);

	m_lbIcon->setFixedSize(76, 78);
	QHBoxLayout *hlayout = new QHBoxLayout;
	hlayout->addWidget(m_lbIcon);
	hlayout->addWidget(m_lbText);
	hlayout->setSpacing(28);
    hlayout->setMargin(0);

	m_btnLayout = new QHBoxLayout;
	m_btnLayout->setSpacing(10);
    if (flag == Qt::AlignRight || flag==Qt::AlignCenter)
	    m_btnLayout->addStretch(10);
	CreateButtons((Button)buttons);
	for (auto btn : m_bnts)
	{
		m_btnLayout->addWidget(btn);
		connect(btn, &QPushButton::clicked, this, [&](){
			done((Button)(sender()->property("id").toInt()));
		});
	}
    if (flag == Qt::AlignLeft || flag == Qt::AlignCenter)
	    m_btnLayout->addStretch(10);
    m_btnLayout->setMargin(0);

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addLayout(hlayout);
    mainLayout->addStretch();
	mainLayout->addLayout(m_btnLayout);
	mainLayout->setContentsMargins(21, 30, 29, 30);
	mainLayout->setSpacing(10);

	resize(580, 245);

	m_lbText->setText(text);

	m_layoutWidget = new QWidget(this);
	m_layoutWidget->setLayout(mainLayout);
    setAttribute(Qt::WA_StyledBackground);
    m_lbText->setAttribute(Qt::WA_StyledBackground);
	RetranslateUi();
    m_titlebar->setObjectName("FSMessageBox_m_titlebar");
    m_lbText->setObjectName("FSMessageBox_TextEdit");
	SetIconObjectName("MessageBox_Icon_Information");
    setObjectName("FSMessageBox");
    RetranslateUi();
}


void FSMessageBox::SetButtonText(Button id, QString text)
{
	for (auto btn : m_bnts)
	{
		if (id == btn->property("id").toInt())
		{
			btn->setText(text);
			break;
		}
	}
}


QPushButton* FSMessageBox::GetButton(Button id)
{
	QPushButton *retbtn = nullptr;
	for (auto btn : m_bnts)
	{
		if (id == btn->property("id").toInt())
		{
			retbtn = btn;
			break;
		}
	}
	return retbtn;
}

void FSMessageBox::SetIconObjectName(QString objectName)
{
    m_lbIcon->setObjectName(objectName);
}

void FSMessageBox::SetIconVisible(bool visible)
{
    m_lbIcon->setVisible(visible);
}

static void ShowMaskWidget(QWidget *widget)
{
	auto scrn = qApp->desktop()->availableGeometry();
	widget->setGeometry(scrn);
	widget->setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
	widget->setWindowOpacity(0.01);
	//widget->setStyleSheet("background: rgb(0,0,0);");
	widget->setVisible(true);
}
FSMessageBox::Button FSMessageBox::information(const QString & title, const QString & text, int buttons, QWidget * parent, Qt::AlignmentFlag flag /*= Qt::AlignRight*/, int w /*= 580*/, int h /*= 245*/)
{
    FSMessageBox dlg(title, text, flag, buttons, parent);
    dlg.resize(w, h);
    dlg.SetIconObjectName("MessageBox_Icon_Information");
    return (Button)dlg.DoModal();
}

FSMessageBox::Button FSMessageBox::informationOk(const QString & title, const QString & text, QWidget * parent, int w , int h )
{
	FSMessageBox dlg(title, text,Qt::AlignRight,FSMessageBox::OK, parent);
    dlg.resize(w, h);
	dlg.SetIconObjectName("MessageBox_Icon_Information");
	return (Button)dlg.DoModal();
}

FSMessageBox::Button FSMessageBox::informationOkEx(const QString & title, const QString & text, int w /*= 580*/, int h /*= 245*/)
{
	FSBaseWidget widget(nullptr);
	ShowMaskWidget(&widget);
	FSMessageBox dlg(title, text, Qt::AlignRight, FSMessageBox::OK, nullptr);
	dlg.resize(w, h);
	dlg.SetIconObjectName("MessageBox_Icon_Information");
	dlg.setWindowFlags(dlg.windowFlags() | Qt::WindowStaysOnTopHint);
	return (Button)dlg.DoModal();
}

FSMessageBox::Button FSMessageBox::informationYes(const QString & title, const QString & text, QWidget * parent, int w, int h)
{
    FSMessageBox dlg(title, text, Qt::AlignRight, FSMessageBox::Yes, parent);
    dlg.resize(w, h);
    dlg.SetIconObjectName("MessageBox_Icon_Information");

	return (Button)dlg.DoModal();
}


FSMessageBox::Button FSMessageBox::informationYesEx(const QString & title, const QString & text, int w /*= 580*/, int h /*= 245*/)
{
	FSBaseWidget widget(nullptr);
	ShowMaskWidget(&widget);
	FSMessageBox dlg(title, text, Qt::AlignRight, FSMessageBox::Yes, nullptr);
	dlg.resize(w, h);
	dlg.SetIconObjectName("MessageBox_Icon_Information");
    dlg.setWindowFlags(dlg.windowFlags() | Qt::WindowStaysOnTopHint);

	return (Button)dlg.DoModal();
}

FSMessageBox::Button FSMessageBox::questionYesNo(const QString & title, const QString & text, QWidget * parent, int w, int h)
{
    FSMessageBox dlg(title, text, Qt::AlignRight, FSMessageBox::Yes | FSMessageBox::No, parent);
    dlg.resize(w, h);
    dlg.SetIconObjectName("MessageBox_Icon_Question");
	return (Button)dlg.DoModal();
}

FSMessageBox::Button FSMessageBox::questionYesNoEx(const QString & title, const QString & text, int w /*= 580*/, int h /*= 245*/)
{
	FSBaseWidget widget(nullptr);
	ShowMaskWidget(&widget);
	FSMessageBox dlg(title, text, Qt::AlignRight, FSMessageBox::Yes | FSMessageBox::No, nullptr);
	dlg.resize(w, h);
	dlg.SetIconObjectName("MessageBox_Icon_Question");
    dlg.setWindowFlags(dlg.windowFlags() | Qt::WindowStaysOnTopHint);
    return (Button)dlg.DoModal();
}

FSMessageBox::Button FSMessageBox::questionYesNoCancel(const QString & title, const QString & text, QWidget *parent, int w, int h)
{
    FSMessageBox dlg(title, text, Qt::AlignRight, FSMessageBox::Yes | FSMessageBox::No | FSMessageBox::Cancel, parent);
    dlg.resize(w, h);
    dlg.SetIconObjectName("MessageBox_Icon_Question");

	return (Button)dlg.DoModal();
}

FSMessageBox::Button FSMessageBox::questionYesNoCancelEx(const QString & title, const QString & text, int w /*= 580*/, int h /*= 245*/)
{
	FSBaseWidget widget(nullptr);
	ShowMaskWidget(&widget);
	FSMessageBox dlg(title, text, Qt::AlignRight, FSMessageBox::Yes | FSMessageBox::No | FSMessageBox::Cancel, nullptr);
	dlg.resize(w, h);
	dlg.SetIconObjectName("MessageBox_Icon_Question");
    dlg.setWindowFlags(dlg.windowFlags() | Qt::WindowStaysOnTopHint);

	return (Button)dlg.DoModal();
}

void FSMessageBox::CreateButtons(Button buttons)
{
	auto CreateButton = [&](Button btnid, QString text)->QPushButton*{
		auto btn = new QPushButton(0);
		btn->setText(text);
		btn->setProperty("id", btnid);
		btn->setFixedSize(95, 34);
        btn->setObjectName("FSMessageBox_btn");
		return btn;
	};
	if (buttons & Yes)
	{
		m_bnts.push_back(CreateButton(Yes, tr("Yes")));
	}
	if (buttons & No)
	{
		m_bnts.push_back(CreateButton(No, tr("No")));
	}
	if (buttons & OK)
	{
		m_bnts.push_back(CreateButton(Cancel, tr("OK")));

	}
	if (buttons & Cancel)
	{
		m_bnts.push_back(CreateButton(Cancel, tr("Cancel")));
	}
	
}

void FSMessageBox::resizeEvent(QResizeEvent *event)
{
	FSBaseDialog::resizeEvent(event);
	const int margins = 1;
	m_layoutWidget->setGeometry(margins, m_titleHeight, width() - 2 * margins, height() - m_titleHeight - margins);


}

bool FSMessageBox::eventFilter(QObject *watcher, QEvent *event)
{
	if (watcher == m_lbText)
	{
		if (event->type() == QEvent::Enter)
		{
			qApp->setOverrideCursor(Qt::ArrowCursor);
		}
		else if (event->type() == QEvent::Leave)
		{
			qApp->restoreOverrideCursor();
		}
		else if (event->type() == QEvent::Show)
		{
			auto pageSize = m_lbText->document()->documentLayout()->documentSize();
			int preHeight = m_lbText->height();
			if (preHeight < pageSize.height())
			{
				resize(QSize(size().width(), size().height() + pageSize.height() - preHeight + 10));
                m_lbText->setFixedHeight(pageSize.height());
			}
		}
	}

	return FSBaseDialog::eventFilter(watcher, event);
}

void FSMessageBox::RetranslateUi()
{
    m_titlebar->GetButton(TopLevelWindowTitleBar::Close)->setToolTip(tr("Close"));
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
CategoryBar::CategoryBar(const QString &objname, QWidget *parent)
	:FSBaseWidget(parent)
{
	setObjectName(objname);
	m_nBottomLineHeight = 1;
	m_curId = -1;
    m_pHL = new QHBoxLayout(this);
    m_pHL->setSpacing(1);
    m_pHL->setMargin(0);
    m_pHL->addStretch();
}

void CategoryBar::AddItem(int id, const QString &objname, const QString &text, const char *sztext)
{
	auto item = new CategoryBarButton(id, objname, this);
	item->SetBottomLineHeight(m_nBottomLineHeight);
	item->setVisible(true);
	item->SetText(text);
    item->setProperty("QByteArray", QByteArray(sztext));
	item->SetColors(normalTextColor, selectedTextColor, normalBackgroundColor, selectedBackgroundColor, normalBottomLineColor, selectedBottomLineColor);
	connect(item, &CategoryBarButton::sigSelected, this, [&](int id){
		SelectItem(id);
	});
    item->setMinimumWidth(item->Width());
	m_lstItems.push_back(item);
    m_pHL->insertWidget(m_pHL->count()-1, item);
    //m_pHL->addWidget(item);
}

void CategoryBar::SelectItem(int id)
{
	if (id == m_curId) return;
	bool bRightId = false;
	for (auto item : m_lstItems)
	{
		if (item->GetId() == id)
		{
			bRightId = true;
			break;
		}
	}
	if (!bRightId) return;

	int oldid = m_curId;
	for (auto item : m_lstItems)
	{
		if (item->GetId() == id)
		{
			m_curId = id;
			item->SetButtonState(CategoryBarButton::BUTTONSTATE_SELECTED);
		}
		else
		{
			item->SetButtonState(CategoryBarButton::BUTTONSTATE_NORMAL);
		}
	}
	emit sigItemChanged(oldid, m_curId);
}

void CategoryBar::SetBottomLineHeight(int h)
{
	m_nBottomLineHeight = qMax(1, h);
}

QList<CategoryBarButton*> CategoryBar::GetItems()
{
    return m_lstItems;
}

QColor CategoryBar::GetNormalTextColor()
{
	return normalTextColor;
}

void CategoryBar::SetNormalTextColor(QColor clr)
{
	normalTextColor = clr;
}

QColor CategoryBar::GetSelectedTextColor()
{
	return selectedTextColor;
}

void CategoryBar::SetSelectedTextColor(QColor clr)
{
	selectedTextColor = clr;
}

QColor CategoryBar::GetNormalBackgroundColor()
{
	return normalBackgroundColor;
}

void CategoryBar::SetNormalBackgroundColor(QColor clr)
{
	normalBackgroundColor = clr;
}

QColor CategoryBar::GetSelectedBackgroundColor()
{
	return selectedBackgroundColor;
}

void CategoryBar::SetSelectedBackgroundColor(QColor clr)
{
	selectedBackgroundColor = clr;
}

QColor CategoryBar::GetNormalBottomLineColor()
{
	return normalBottomLineColor;
}

void CategoryBar::SetNormalBottomLineColor(QColor clr)
{
	normalBottomLineColor = clr;
}

QColor CategoryBar::GetSelectedBottomLineColor()
{
	return selectedBottomLineColor;
}

void CategoryBar::SetSelectedBottomLineColor(QColor clr)
{
	selectedBottomLineColor = clr;
}

void CategoryBar::RetranslateUi()
{

}

//////////////////////////////////////////////////////////////////////////
CategoryBarButton::CategoryBarButton(int id, const QString &objname, QWidget *parent)
	: FSBaseWidget(parent)
{
	setObjectName(objname);
	m_nBottomLineHeight = 1;
	m_id = id;
	m_state = BUTTONSTATE_NORMAL;
}

void CategoryBarButton::SetText(const QString &text)
{
	m_text = text;
}

void CategoryBarButton::SetColors(QColor normalTextColor,
	QColor selectedTextColor,
	QColor normalBackgroundColor,
	QColor selectedBackgroundColor,
	QColor normalBottomLineColor,
	QColor selectedBottomLineColor)
{
	this->normalTextColor = normalTextColor;
	this->selectedTextColor = selectedTextColor;
	this->normalBackgroundColor = normalBackgroundColor;
	this->selectedBackgroundColor = selectedBackgroundColor;
	this->normalBottomLineColor = normalBottomLineColor;
	this->selectedBottomLineColor = selectedBottomLineColor;
}

void CategoryBarButton::SetButtonState(BUTTONSTATE s)
{
	m_state = s;
	update();
}

int CategoryBarButton::GetId()
{
	return m_id;
}

QPixmap CategoryBarButton::GetNormalImage()
{
	return normalImg;
}

void CategoryBarButton::SetNormalImage(QPixmap px)
{
	normalImg = px;
}

QPixmap CategoryBarButton::GetSelectedImage()
{
	return selectedImg;
}

void CategoryBarButton::SetSelectedImage(QPixmap px)
{
	selectedImg = px;
}

void CategoryBarButton::SetBottomLineHeight(int h)
{
	m_nBottomLineHeight = h;
}

int CategoryBarButton::Width()
{
    return 40;

	//int imgW = !normalImg.isNull() ? normalImg.width() : 0;
	//int margin = 7;
 //   int textLen = StringOperation::GetTextTrueLenght(((QWidget*)parent())->font(), m_text);

	//int leftLen = imgW == 0 ? margin : (margin + imgW + margin);
	//return leftLen + textLen + margin+1;
}

void CategoryBarButton::paintEvent(QPaintEvent *e)
{
	int margin = 6;
	int imgW = !normalImg.isNull() ? normalImg.width() : 0;
	int imgH = !normalImg.isNull() ? normalImg.height() : 0;
	int bottomLineH = m_nBottomLineHeight;
	QPainter painter(this);

	painter.fillRect(rect(), m_state == BUTTONSTATE_NORMAL ? normalBackgroundColor : selectedBackgroundColor);

	QRect rcImg(margin, (height() - bottomLineH - imgH)*0.5, imgW, imgH);
	painter.drawPixmap(rcImg, m_state == BUTTONSTATE_NORMAL ? normalImg : selectedImg);

	//int left = imgW == 0 ? margin : (margin + imgW + margin);

	//QRect rcText(left, 0, width() - (left), height() - bottomLineH);
	//QTextOption op(Qt::AlignVCenter);
	//painter.setPen(m_state == BUTTONSTATE_NORMAL ? normalTextColor : selectedTextColor);
	//painter.setFont(((QWidget*)parent())->font());
	//painter.drawText(rcText, m_text, op);

	QRect rcBottomLine(0, height() - bottomLineH, width(), bottomLineH);
    painter.fillRect(rcBottomLine, normalBottomLineColor);
    //painter.fillRect(rcBottomLine, m_state == BUTTONSTATE_NORMAL ? normalBottomLineColor : selectedBottomLineColor);
}

void CategoryBarButton::mousePressEvent(QMouseEvent *e)
{
	QWidget::mousePressEvent(e);
	if (m_state == BUTTONSTATE_SELECTED) return;
	m_state = BUTTONSTATE_SELECTED;
	repaint();
	emit sigSelected(m_id);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////
FSIconButton::FSIconButton(QWidget *pParent)
    :FSButton(pParent)
    , m_State(1)
{
    m_pBarLabel = new FSLabel(this);

    setMouseTracking(true);
    m_pBarLabel->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    m_pBarLabel->setContentsMargins(5, 0, 0, 0);
}

FSIconButton::~FSIconButton()
{

}


void FSIconButton::SetEnable(bool bEnable)
{
    m_State = bEnable ? 1 : 3;
    setEnabled(bEnable);
    m_pBarLabel->setText(QString("<img src='%1'/>").arg(bEnable ? GetNormalImagePath() : GetDisabledImagePath()));
}

void FSIconButton::resizeEvent(QResizeEvent *)
{
    m_pBarLabel->setGeometry(0, 0, width(), height());
}

void FSIconButton::leaveEvent(QEvent *e)
{
    if (isEnabled())
    {
        m_State = 1;
        m_pBarLabel->setText(QString("<img src='%1'/>").arg(GetNormalImagePath()));
        QPushButton::leaveEvent(e);
    }
}

void FSIconButton::enterEvent(QEvent *e)
{
    if (isEnabled())
    {
        m_State = 2;
        m_pBarLabel->setText(QString("<img src='%1'/>").arg(GetHoverImagePath()));
        QPushButton::enterEvent(e);
    }
}

QString FSIconButton::GetNormalImagePath()
{
    return normalImagePath;
}

void FSIconButton::SetNormalImagePath(QString path)
{
    normalImagePath = path;
}

QString FSIconButton::GetHoverImagePath()
{
    return hoverImagePath;
}

void FSIconButton::SetHoverImagePath(QString path)
{
    hoverImagePath = path;
}

QString FSIconButton::GetDisabledImagePath()
{
    return disabledImagePath;
}

void FSIconButton::SetDisabledImagePath(QString path)
{
    disabledImagePath = path;
    SetEnable(isEnabled());
}
//
////////////////////////////////////////////////////////////////////////////
//FSLineEditEx::FSLineEditEx(const QString &objName, QWidget *pParent)
//	:FSLineEdit(objName,pParent)
//{
//}
//
//bool FSLineEditEx::eventFilter(QObject * watched, QEvent * event)
//{
//	if (watched != this)
//	{
//		if (event->type() == QEvent::MouseButtonPress)
//		{
//			this->editingFinished();
//		}
//	}
//	return false;
//}
//
//void FSLineEditEx::focusInEvent(QFocusEvent *event)
//{
//
//    qApp->installEventFilter(this);
//    FSLineEdit::focusInEvent(event);
//}
//
//void FSLineEditEx::focusOutEvent(QFocusEvent *event)
//{
//    qApp->removeEventFilter(this);
//    FSLineEdit::focusOutEvent(event);
//
//}

FSNormalButton::FSNormalButton(int id, const QString &objectname, QWidget *parent) :FSBaseButton(id, objectname, parent)
{
	installEventFilter(this);
	m_TextLabel = new QLabel(this);
	m_TextLabel->setObjectName("FSNormalButtonText");
}

FSNormalButton::FSNormalButton(const QString &objectname, QWidget *parent) : FSBaseButton(objectname, parent)
{
	installEventFilter(this);
	m_TextLabel = new QLabel(this);
	m_TextLabel->setObjectName("FSNormalButtonText");
}

void FSNormalButton::setText(QString strText)
{
	m_strText = strText;
	m_TextLabel->setText(strText);
}

void FSNormalButton::resizeEvent(QResizeEvent *event)
{
	int iHeight = m_TextLabel->fontMetrics().height();
	int iWidth = StringOperation::GetTextTrueLenght(m_TextLabel->font(), m_TextLabel->text());
	m_TextLabel->setGeometry(width() - 69 - iWidth, (height() - iHeight) / 2, iWidth, iHeight);
}

bool FSNormalButton::eventFilter(QObject *obj, QEvent *e)
{
	if (obj == this && e->type() == QEvent::MouseButtonPress)
	{
		emit pressed();
	}
	return false;
}

OneApplication::OneApplication(QString key)
:m_mutexName(key)
{
    m_handle = NULL;
}

OneApplication::~OneApplication()
{
    if (m_handle)
        CloseHandle(m_handle);
}

bool OneApplication::IsOneApp()
{
    auto stringw = m_mutexName.toStdWString();
    auto handleTemp = ::OpenMutexW(MUTEX_ALL_ACCESS, FALSE, stringw.c_str());
    //auto retHandel = m_handle;
    bool retValue = (handleTemp == NULL);
    if (handleTemp == NULL)
    {
        m_handle = ::CreateMutexW(0, TRUE, stringw.c_str());
    }
    else
    {
        ::CloseHandle(handleTemp);
    }
    return retValue;
}

void OneApplication::ShowWindow(QString wndName)
{
    //已经开启一个recorder, 弹出已经开启的recorder
    auto stringw = wndName.toStdWString();
    HWND hwnd = ::FindWindow(nullptr, stringw.c_str());
    if (hwnd)
    {
        SetForegroundWindow(hwnd);
        ::ShowWindow(hwnd, SW_NORMAL);
    }
}

bool OneApplication::IsWindowsVisible(QString wndName)
{
	auto stringw = wndName.toStdWString();
	HWND hwnd = ::FindWindow(nullptr, stringw.c_str());
	if (hwnd)
	{
		return ::IsWindowVisible(hwnd);
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
FSDoubleSpinBox::FSDoubleSpinBox(QWidget *pParent)
    :QDoubleSpinBox(pParent)
{
    m_firstFocus = false;

    m_dragHelper = new IDragValueChangedHelper(this);
    m_dragHelper->SetSlowStep(0.1);
    m_dragHelper->SetLoop(false);
    connect(m_dragHelper, &IDragValueChangedHelper::sigValueChanged, this, [&](double value){
        setValue(value);
    });

    connect(this, SIGNAL(valueChanged(double)), this, SLOT(SlotValueChanged(double)));

    connect(this, &QDoubleSpinBox::editingFinished, this, &QDoubleSpinBox::clearFocus);

    lineEdit()->installEventFilter(this);
}

FSDoubleSpinBox::~FSDoubleSpinBox()
{

}


void FSDoubleSpinBox::SetDragEnabled(bool enabled)
{
    m_dragHelper->SetDragEnabled(enabled);
}

void FSDoubleSpinBox::setValue(double val)
{
    m_dragHelper->SetValue(val);
    QDoubleSpinBox::setValue(val);
}

void FSDoubleSpinBox::setRange(double min, double max)
{
    m_dragHelper->SetRange(min, max);
    QDoubleSpinBox::setRange(min, max);
}

void FSDoubleSpinBox::setDecimals(int prec)
{
    QDoubleSpinBox::setDecimals(prec);

}

void FSDoubleSpinBox::SetLoop(bool loop)
{
    m_dragHelper->SetLoop(loop);
}

IDragValueChangedHelper* FSDoubleSpinBox::GetHelper()
{
    return m_dragHelper;
}

void FSDoubleSpinBox::SlotValueChanged(double value)
{
    m_dragHelper->SetValue(value);

}

void FSDoubleSpinBox::wheelEvent(QWheelEvent *event)
{
    //QDoubleSpinBox::wheelEvent(event);
    //lineEdit()->setSelection(0, 0);
}

bool FSDoubleSpinBox::eventFilter(QObject *watcher, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent *pEvent = static_cast<QMouseEvent*>(event);
        if (watcher == lineEdit())
        {
            if (!lineEdit()->hasSelectedText() && m_firstFocus)
            {
                selectAll();
                m_firstFocus = false;
                return true;
            }
        }
        else if (!rect().contains(mapFromGlobal(pEvent->globalPos())))
        {
            editingFinished();
        }
    }
    return false;
}

void FSDoubleSpinBox::focusInEvent(QFocusEvent *event)
{
    m_firstFocus = true;
    qApp->installEventFilter(this);
    QDoubleSpinBox::focusInEvent(event);
}

void FSDoubleSpinBox::focusOutEvent(QFocusEvent *event)
{
    qApp->removeEventFilter(this);
    QDoubleSpinBox::focusOutEvent(event);

}

//////////////////////////////////////////////////////////////////////////////////////////////////////
FSSpinBox::FSSpinBox(QWidget *pParent) 
    :QSpinBox(pParent)
{
}

FSSpinBox::~FSSpinBox()
{

}

void FSSpinBox::wheelEvent(QWheelEvent *event)
{
    //QSpinBox::wheelEvent(event);
    //lineEdit()->setSelection(0, 0);
}

//////////////////////////////////////////////////////////////////////////
FSMEssageConfirmDlg::FSMEssageConfirmDlg(const QString &title, const QString &text, const QString & confirmText, Qt::AlignmentFlag flag, int buttons, QWidget *parent)
:FSMessageBox(title, text, flag, buttons, parent)
{
    m_pCbDontShow = new QCheckBox(this);
    m_pCbDontShow->setObjectName("FSConfirmDlg_m_pCbDontShow");
    m_pCbDontShow->setText(confirmText);
}

std::tuple<bool, FSMessageBox::Button> FSMEssageConfirmDlg::questionYesNo(const QString & title, const QString & text, const QString & confirmText, bool bChecked, QWidget * parent)
{
    FSMEssageConfirmDlg dlg(title, text, confirmText, Qt::AlignRight, FSMessageBox::Yes | FSMessageBox::No, parent);
    dlg.SetChecked(bChecked);
    int button = dlg.DoModal();
    bool bCheck = dlg.IsCheck();
    return std::make_tuple(bCheck, (FSMessageBox::Button)button);
}

void FSMEssageConfirmDlg::SetChecked(bool checked)
{
    m_pCbDontShow->setChecked(checked);
}

bool FSMEssageConfirmDlg::IsCheck()
{
    return m_pCbDontShow->isChecked();

}

void FSMEssageConfirmDlg::resizeEvent(QResizeEvent *e)
{
    FSMessageBox::resizeEvent(e);
    //QPoint lb = mapFromGlobal(m_lbText->mapToGlobal(m_lbText->geometry().bottomLeft()));
    //int l = m_lbIcon->width() + 53;
    //int t = m_lbIcon->height() + 28;
    //auto pageHeight = qMin<int>(m_lbText->document()->documentLayout()->documentSize().height(), m_lbText->height()) + m_lbText->geometry().top();
    //t = pageHeight + 22;
    //m_pCbDontShow->setGeometry(l, t, width() - 2 * l, 20);
}

void FSMEssageConfirmDlg::showEvent(QShowEvent *e)
{
    FSMessageBox::showEvent(e);
    QPoint lb = mapFromGlobal(m_lbText->mapToGlobal(m_lbText->geometry().bottomLeft()));
    int l = m_lbIcon->width() + 53;
    int t = m_lbIcon->height() + 28;
    int kkk = m_lbText->document()->documentLayout()->documentSize().height();
    int uuu = m_lbText->height();
    auto pageHeight = qMin<int>(m_lbText->document()->documentLayout()->documentSize().height(), m_lbText->height()) + m_lbText->geometry().top();
    t = pageHeight + m_titlebar->geometry().bottom() + 20;
    m_pCbDontShow->setGeometry(l, t, width() - 2 * l, 20);
}

//////////////////////////////////////////////////////////////////////////
FSFileNameLineEdit::FSFileNameLineEdit(QWidget *pParent)
:FSFileNameLineEdit("", pParent)
{

}

FSFileNameLineEdit::FSFileNameLineEdit(const QString &objName, QWidget *pParent)
: FSLineEdit(objName, pParent)
{
    setValidator(new QRegExpValidator(QRegExp("[^/\\\\:*?\"<>|]+")));
    connect(this, &QLineEdit::editingFinished, this, [&](){
        if (text().isEmpty())
            SetTextBlockSignal(m_deleteString);
    });

}

void FSFileNameLineEdit::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Delete || event->key() == Qt::Key_Backspace)
    {
        m_deleteString = text();
    }

    FSLineEdit::keyPressEvent(event);
}



//////////////////////////////////////////////////////////////////////////
CusMenuItemBase::CusMenuItemBase(QWidget *parent /*= nullptr*/)
	:QWidget(parent)
	, m_leftPadding(0)
{

}

void CusMenuItemBase::SetLeftPadding(int margin)
{
	m_leftPadding = margin;
}

int CusMenuItemBase::GetLeftPadding()
{
	return m_leftPadding;
}

//////////////////////////////////////////////////////////////////////////
CusMenu::CusMenu(QWidget *parent)
	:QMenu(parent)
{
	m_leftPadding = 0;
	m_parent = parent;
	connect(this, &QMenu::aboutToShow, this, &CusMenu::OnAbout2Show);
	connect(this, &QMenu::aboutToHide, this, &CusMenu::OnAbout2Hide);
}

void CusMenu::AddWidget(CusMenuItemBase *w)
{
	if (!w) return;
	int cnt = this->actions().count();

	auto act = addAction("");
	w->setParent(this);
	w->setVisible(false);
	w->setProperty("action", qptrdiff(act));
	connect(w, &CusMenuItemBase::sigActivate, this, [&]{hide();});
	m_widgets.push_back(w);
}

void CusMenu::SetLeftPadding(int padding)
{
	m_leftPadding = padding;
}

int CusMenu::GetLeftPadding()
{
	return m_leftPadding;
}

void CusMenu::clear()
{
	m_widgets.clear();
	QMenu::clear();
}

void CusMenu::OnAbout2Show()
{
	for (auto w : m_widgets)
	{
		auto act = reinterpret_cast<QAction*>(w->property("action").value<qptrdiff>());
		w->SetLeftPadding(m_leftPadding);
		w->setGeometry(actionGeometry(act));
		w->Resize();
		w->setVisible(true);
		w->raise();
	}
}

void CusMenu::OnAbout2Hide()
{
	for (auto w : m_widgets)
		w->setVisible(false);
}

bool CusMenu::eventFilter(QObject *obj, QEvent *e)
{
	return false;
}

//////////////////////////////////////////////////////////////////////////
ColorItem::ColorItem(QColor clr, QColor selectClr, QWidget *parent)
	:FSBaseWidget(parent)
	, m_bSelected(false)
	, m_bHoverd(false)
	, m_color(clr)
	, m_selectedColor(selectClr)
{

}

void ColorItem::SetColor(QColor clr)
{
	m_color = clr;
}

QColor ColorItem::GetColor()
{
	return m_color;
}

void ColorItem::SetSelectBorderColor(QColor clr)
{
	m_selectedColor = clr;
}

void ColorItem::SetSelected(bool bSelected)
{
	m_bSelected = bSelected;
	update();
}

bool ColorItem::IsSelected()
{
	return m_bSelected;
}

void ColorItem::mousePressEvent(QMouseEvent *e)
{
	SetSelected(true);
	emit sigCliked(m_color);
}

void ColorItem::enterEvent(QEvent *e)
{
	m_bHoverd = true;
	update();
}

void ColorItem::leaveEvent(QEvent *e)
{
	m_bHoverd = false;
	update();
}

void ColorItem::paintEvent(QPaintEvent *e)
{
	QPainter p(this);
	p.fillRect(rect(), m_color);
	if (m_bHoverd || m_bSelected)
	{
		p.save();
		p.setPen(m_selectedColor);
		p.drawRect(rect().adjusted(1,1,-2,-2));
		p.setPen(Qt::black);
		p.drawRect(rect().adjusted(2, 2, -3, -3));
		p.restore();
	}
}

//////////////////////////////////////////////////////////////////////////
ColorMenuItem::ColorMenuItem(QWidget *parent)
	:CusMenuItemBase(parent)
{
	setMouseTracking(true);
	setVisible(false);
}

void ColorMenuItem::AddColor(QColor clr)
{
	for (auto item : m_items)
	{
		if (item->GetColor() == clr)
			return;
	}

	auto pItem = new ColorItem(clr, QColor(255, 255, 255), this);
	pItem->setVisible(true);
	m_items.push_back(pItem);
	connect(pItem, &ColorItem::sigCliked, this, [&](QColor clr){
		for (auto item : m_items)
			item->SetSelected(item == sender());
		emit sigSelectedChanged(clr);
		emit sigActivate();
	});
	resizeEvent(nullptr);
}

void ColorMenuItem::SetSelectColor(QColor clr)
{
	for (auto item : m_items)
	{
		item->SetSelected(item->GetColor() == clr);
	}
}

void ColorMenuItem::SetToolTip(QColor clr, QString toolTip)
{
	for (auto item : m_items)
	{
		if (item->GetColor() == clr)
		{
			item->setToolTip(toolTip);
		}
	}
}

void ColorMenuItem::Resize()
{
	resizeEvent(nullptr);
}

void ColorMenuItem::showEvent(QShowEvent *e)
{
}

void ColorMenuItem::resizeEvent(QResizeEvent *e)
{
	int l = itemMargin;
	int t = (height() - itemH) * 0.5;
	for (int i = 0; i < m_items.size(); ++i)
	{
		m_items[i]->setGeometry(GetLeftPadding() + i*(itemW + itemMargin), t, itemW, itemH);
	}
}

void ColorMenuItem::mousePressEvent(QMouseEvent *e)
{
	e->accept();
}
