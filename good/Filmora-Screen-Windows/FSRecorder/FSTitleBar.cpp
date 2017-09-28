#include "stdafx.h"
#include "FSTitleBar.h"

FSRecorderMenuBar::FSRecorderMenuBar(QWidget *parent)
: FSBaseWidget(parent)
{
	Init();
}

FSRecorderMenuBar::~FSRecorderMenuBar()
{

}

void FSRecorderMenuBar::RetranslateUi()
{
	m_pMenuCapture->setTitle(tr("Capture"));
	m_pMenuSettings->setTitle(tr("Settings"));
	m_pMenuHelp->setTitle(tr("Help"));
	m_pMenuLang->setTitle(tr("Language"));

	m_pActStart->setText(tr("Start"));
	m_pActStop->setText(tr("Stop"));
	m_pActClose->setText(tr("Close"));

	m_pActLang_Eng->setText(tr("English"));
	m_pActOptions->setText(tr("Options"));

	m_pActSupportCenter->setText(tr("Support Center"));
	m_pActAbout->setText(tr("About"));
	m_pActEditor->setText(tr("Editor"));
}

void FSRecorderMenuBar::resizeEvent(QResizeEvent *e)
{
	m_pMenuBar->setGeometry(rect());
}

void FSRecorderMenuBar::mousePressEvent(QMouseEvent *e)
{
	FSBaseWidget::mousePressEvent(e);
}

void FSRecorderMenuBar::Init()
{
	m_pMenuBar = new QMenuBar(this);
	m_pMenuCapture = new QMenu(this);
	m_pActStart = new QAction(this);
	m_pActStop = new QAction(this);
	m_pActClose = new QAction(this);
	m_pMenuCapture->addAction(m_pActStart);
	m_pMenuCapture->addAction(m_pActStop);
	m_pMenuCapture->addAction(m_pActClose);

	m_pMenuSettings = new QMenu(this);
	m_pMenuLang = new QMenu(this);
	m_pActLang_Eng = new QAction(this);
	m_pMenuLang->addAction(m_pActLang_Eng);
	m_pActOptions = new QAction(this);
	m_pMenuSettings->addMenu(m_pMenuLang);
	m_pMenuSettings->addAction(m_pActOptions);

	m_pMenuHelp = new QMenu(this);
	m_pActSupportCenter = new QAction(this);
	m_pActAbout = new QAction(this);
	m_pMenuHelp->addAction(m_pActSupportCenter);
	m_pMenuHelp->addAction(m_pActAbout);
	
	m_pActEditor = new QAction(this);
	m_pMenuBar->addMenu(m_pMenuCapture);
	m_pMenuBar->addMenu(m_pMenuSettings);
	m_pMenuBar->addMenu(m_pMenuHelp);
	m_pMenuBar->addAction(m_pActEditor);
	RetranslateUi();
	ConnectSignals();
}

void FSRecorderMenuBar::ConnectSignals()
{
	connect(m_pActStart, &QAction::triggered, this, [&](bool bCheck){
		emit sigStart();
	});
	connect(m_pActStop, &QAction::triggered, this, [&](bool bCheck){
		emit sigStop();
	});
	connect(m_pActClose, &QAction::triggered, this, [&](bool bCheck){
		emit sigClose();
	});
	connect(m_pActLang_Eng, &QAction::triggered, this, [&](bool bCheck){

	});
	connect(m_pActOptions, &QAction::triggered, this, [&](bool bCheck){

	});
	connect(m_pActSupportCenter, &QAction::triggered, this, [&](bool bCheck){

	});
	connect(m_pActAbout, &QAction::triggered, this, [&](bool bCheck){

	});
	connect(m_pActEditor, &QAction::triggered, this, [&](bool bCheck){

	});
}

FSTitleBar::FSTitleBar(QWidget *parent)
:FSBaseWidget(parent)
{
	m_pMoveWidget = nullptr;
	m_pMenuBar = new FSRecorderMenuBar(this);
	m_pBtnMin = new QPushButton(this);
	m_pBtnMin->setObjectName("FSTitleBar_MinButton");
	m_pBtnExit = new QPushButton(this);
	m_pBtnExit->setObjectName("FSTitleBar_ExitButton");
	connect(m_pMenuBar, &FSRecorderMenuBar::sigClose, this, [&](){
		emit sigClosed();
	});
	connect(m_pBtnMin, &QPushButton::clicked, this, [&](){
		m_pMoveWidget->showMinimized();
		emit sigMinimize();
	});
	connect(m_pBtnExit, &QPushButton::clicked, this, [&](){
		emit sigClosed();
	});
	SetMoveWidget(parent);
}

FSTitleBar::~FSTitleBar()
{
}

FSRecorderMenuBar * FSTitleBar::GetMenuBar()
{
	return m_pMenuBar;
}

void FSTitleBar::SetMoveWidget(QWidget *pW)
{
	m_pMoveWidget = pW;
}

void FSTitleBar::resizeEvent(QResizeEvent *e)
{
	static const int buttonW = height();
	static const int buttonH = height();
	static const int menuBarW = 218;
	m_pMenuBar->setGeometry(0, 0, menuBarW, height());

	int l = width() - 2 * buttonW;
	int t = (height() - buttonH) * 0.5;
	m_pBtnMin->setGeometry(l,t, buttonW, buttonH);
	l = m_pBtnMin->geometry().right() + 1;
	m_pBtnExit->setGeometry(l, t, buttonW, buttonH);
}

void FSTitleBar::mousePressEvent(QMouseEvent *e)
{
	m_ptPress = e->globalPos();
	if(m_pMoveWidget) m_ptPosWhenPress = m_pMoveWidget->pos();
}

void FSTitleBar::mouseMoveEvent(QMouseEvent *e)
{
	QPoint curPt = e->globalPos();
	curPt -= m_ptPress;
	if (m_pMoveWidget) m_pMoveWidget->move(m_ptPosWhenPress + curPt);
}
