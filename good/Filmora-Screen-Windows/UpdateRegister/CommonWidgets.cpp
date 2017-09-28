#include "stdafx.h"
#include "CommonWidgets.h"

//////////////////////////////////////////////////////////////////////////
ImageButton::ImageButton(QWidget *parent /*= 0*/)
: QWidget(parent)
{
	setAttribute(Qt::WA_StyledBackground);
}

void ImageButton::mousePressEvent(QMouseEvent *event)
{
	emit Clicked();
	QWidget::mousePressEvent(event);
}

/*URBaseWidget*/
URBaseWidget::URBaseWidget(QWidget *parent)
: QWidget(parent)
{
	setAttribute(Qt::WA_StyledBackground);
}

void URBaseWidget::RetranslateUi()
{

}

bool URBaseWidget::event(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange){
		RetranslateUi();
	}
	return QWidget::event(e);
}

//////////////////////////////////////////////////////////////////////////
TitleBar::TitleBar(QWidget *parent /*= 0*/)
: URBaseWidget(parent)
{
	setAttribute(Qt::WA_StyledBackground);
	m_parent = parent;
	m_label = new QLabel(this);
	m_label->setObjectName("UpdateRegister_TitleBar_CaptionLabel");
	m_closeBtn = new ImageButton(this);
	m_closeBtn->setObjectName("UpdateRegister_TitleBar_CloseBtn");
	connect(m_closeBtn, &ImageButton::Clicked, this, &TitleBar::Close);
}

void TitleBar::SetTitle(QString title)
{
	m_label->setText(title);
}

void TitleBar::resizeEvent(QResizeEvent *)
{
	int left = 20;
	const int btnWidth = 20;
	m_label->setGeometry(left, 0, width() - left - btnWidth, height());  // width() - left - btnWidth
	m_closeBtn->setGeometry(width() - btnWidth, 0, btnWidth, height());
}

void TitleBar::mousePressEvent(QMouseEvent *event)
{
	m_pressPoint = event->pos();
}

void TitleBar::mouseMoveEvent(QMouseEvent *event)
{
	QPoint movePoint = event->pos();
	int x = movePoint.x() - m_pressPoint.x();
	int y = movePoint.y() - m_pressPoint.y();
	m_parent ? emit Move(x, y) : this->move(x, y);
}

//////////////////////////////////////////////////////////////////////////
URMessageBox::URMessageBox(ButtonMode mode /*OkMode*/, QWidget *parent /*= 0*/)
: QDialog(parent)
, m_mode(mode)
{
	setAttribute(Qt::WA_StyledBackground);
	setWindowFlags(Qt::Widget | Qt::FramelessWindowHint);
	setModal(true);
	m_titleBar = new TitleBar(this);
	m_okBtn = new QPushButton(/*tr("Ok"),*/ this);
	m_yesBtn = new QPushButton(/*tr("Yes"),*/ this);
	m_cancelBtn = new QPushButton(/*tr("Cancel"),*/ this);
	m_label = new QLabel(this);
	m_label->setWordWrap(true);
	m_label->setObjectName("URMessageBox_Label");
	connect(m_titleBar, &TitleBar::Close, this, [&](){
		emit Close();
		close();
	});
	connect(m_titleBar, &TitleBar::Move, this, [&](int x, int y){  // ÍÏ¶¯Âß¼­
		QRect rect = geometry();
		this->setGeometry(rect.x() + x, rect.y() + y, rect.width(), rect.height());
	});
	connect(m_okBtn, &QPushButton::clicked, this, [&](){
		emit Ok();
		close();
	});
	connect(m_yesBtn, &QPushButton::clicked, this, [&](){
		emit Yes();
		close();
	});
	connect(m_cancelBtn, &QPushButton::clicked, this, [&](){
		emit Cancel();
		close();
	});
	setFixedSize(QSize(570, 160));
	m_titleBar->setStyleSheet("QLabel#UpdateRegister_TitleBar_CaptionLabel{font:14px \"Lato\"; color:#C3C6C9;}TitleBar{background-color:#1E252B;}ImageButton#UpdateRegister_TitleBar_CloseBtn{background-position:center;background-repeat:no-repeat;background-image:url(:/UpdateRegister/Resources/x_small_n.png);}ImageButton#UpdateRegister_TitleBar_CloseBtn:hover{background-position:center;background-repeat:no-repeat;background-image:url(:/UpdateRegister/Resources/x_small_h.png);}ImageButton#UpdateRegister_TitleBar_CloseBtn:pressed{background-position:center;background-repeat:no-repeat;background-image:url(:/UpdateRegister/Resources/x_small_p.png);}ImageButton#UpdateRegister_TitleBar_CloseBtn:disabled{background-position:center;background-repeat:no-repeat;background-image:url(:/UpdateRegister/Resources/x_small_d.png);}");
	setStyleSheet("QDialog{border:1px solid #666666;background-color:#27313C;}QPushButton{font:14px \"Lato\";color:#E6E8E9;background-color:#27313C;border:1px solid #93989D;}QPushButton:hover{background-color:#61DED0;border:0px;}QLabel#URMessageBox_Label{ background - color:#27313C; border:0px; font:14px \"Lato\";color:#516A7C;}ImageButton{background-image:url(:/UpdateRegister/Resources/x_small_n.png);background-position:center;background-repeat:no-repeat;}QPushButton{font:14px \"Lato\";color:#E6E8E9;background-color:#27313C;border:1px solid #93989D;}");
	if (m_mode == OkMode){
		m_okBtn->show();
		m_yesBtn->hide();
		m_cancelBtn->hide();
	}
	else{
		m_okBtn->hide();
		m_yesBtn->show();
		m_cancelBtn->show();
	}
	RetranslateUi();
}

void URMessageBox::Information(QString title, QString message)
{
	m_titleBar->SetTitle(title);
	m_label->setText(message);
	show();
}

void URMessageBox::resizeEvent(QResizeEvent *)
{
	int left = 1, top = 1;
	const int titleBarHeight = 30;
	m_titleBar->setGeometry(left, top, width() - 2, titleBarHeight);
	top = m_titleBar->geometry().bottom() + 5;
	left = 20;
	m_label->setGeometry(left, top, width() - 20 - 20, 80);
	top = m_label->geometry().bottom() + 5;

	left = 20;
	m_yesBtn->setGeometry(left, top, 80, 30);
	left = width() - 20 - 80;
	m_cancelBtn->setGeometry(left, top, 80, 30);
	m_okBtn->setGeometry(left, top, 80, 30);
}

bool URMessageBox::event(QEvent* e)
{
	if (e->type() == QEvent::LanguageChange){
		RetranslateUi();
	}
	return QDialog::event(e);
}

void URMessageBox::RetranslateUi()
{
	m_okBtn->setText(tr("OK"));
	m_yesBtn->setText(tr("Yes"));
	m_cancelBtn->setText(tr("Cancel"));
}


