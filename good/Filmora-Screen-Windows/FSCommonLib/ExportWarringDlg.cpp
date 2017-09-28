#include "stdafx.h"
#include "inc_FSCommonlib/ExportWarringDlg.h"
#include "inc_CommonLib/TopLevelWindowTitleBar.h"
#include "inc_UpdateRegister/RegisterMain.h"
#include "inc_FSCommonlib/CBSUrlReader.h"
#include "inc_FSCommonlib/gathread.h"
#include "inc_FSCommonlib/ExternalAPI.h"

ExportWarringDlg::ExportWarringDlg(QWidget *parent)
	: FSBaseDialog(tr("Trial Hint"), "Trial Hint", parent)
{
	m_titlebar->GetButton(TopLevelWindowTitleBar::Close)->setObjectName("TitleBar_CloseButton");
	m_titlebar->setObjectName("FSMessageBox_m_titlebar");
	m_titlebar->GetControl()->SetBorderColor(QColor(102, 102, 102));
	m_titlebar->GetControl()->SetBorderWidth(1);
	m_pLbLogo = new QLabel(this);
	m_pLbContent = new QLabel(this);
	m_pLbContent->setAlignment(Qt::AlignLeft | Qt::AlignTop);
	m_pLbContent->setWordWrap(true);
	m_pLbTrialVersion = new QLabel(this);
	m_pLbTrialVersion->setAlignment(Qt::AlignCenter);
	m_pLbRegisterVersion = new QLabel(this);
	m_pLbRegisterVersion->setAlignment(Qt::AlignCenter);

	m_pLbTrialImg = new QLabel(this);
	m_pLbRegisterImg = new QLabel(this);
	m_pLbTrialImg->setObjectName("ExportWarringDlg_m_pLbTrialImg");
	m_pLbRegisterImg->setObjectName("ExportWarringDlg_m_pLbRegisterImg");
	m_pBtnRegister = new QPushButton(this);
	m_pBtnBuyNow = new QPushButton(this);
	m_pBtnEvaluate = new QPushButton(this);

	m_pRegisterChecker = new RegisterMain();
	m_pRegisterChecker->setVisible(false);
	CBSUrlReader cbsReader(this);
	mOderPageUrl = cbsReader.ReadCustomerUrl("ENG", "OrderPage");
	resize(800, 534);
	setObjectName("ExportWarringDlg");
	RetranslateUi();

	connect(m_pBtnRegister, &QPushButton::clicked, this, [&](){
		m_pRegisterChecker->DoRegister();
	});
	connect(m_pBtnBuyNow, &QPushButton::clicked, this, [&](){
		QDesktopServices::openUrl(QUrl(mOderPageUrl));
	});
	connect(m_pBtnEvaluate, &QPushButton::clicked, this, [&](){done(1); });

	connect(m_pRegisterChecker, &RegisterMain::RegisterResult, this, [&](bool resualt){
		if (resualt)
			done(1);
	});

	connect(m_pRegisterChecker, &RegisterMain::singalStartRegister, this, [&](const QDateTime time){
		QDateTime registerTime = time;
		uint  timeall = time.toTime_t();
		GAThread::getInstance()->setStartRegisterTime(registerTime);
	});

	//ÉèÖÃ
	connect(m_pRegisterChecker, &RegisterMain::signalCheckOnRegist, this, [&](bool bResult, QDateTime time){
		QDateTime registerTime = GAThread::getInstance()->getStartRegisterTime();
		int iTotalSecs = registerTime.secsTo(time);
		GAThread::getInstance()->insertEventItem(RegisterTimer, "RegsterFinished", QString::number(iTotalSecs*1000));
	});
}

ExportWarringDlg::~ExportWarringDlg()
{
	m_pRegisterChecker->deleteLater();
}

bool ExportWarringDlg::IsRegister()
{
	return m_pRegisterChecker->CheckRegister();
}

void ExportWarringDlg::RetranslateUi()
{
	m_pLbLogo->setPixmap(QPixmap(":/FSCommonLib/Resources/light/filmorascrn_logo.png"));
	m_pLbContent->setText(tr("Your Copy of software is not registered and the outputted video will be added a watermark.Buy a License to remove the limitation to enjoy all the benefits of product lifetime support."));
	m_pLbTrialVersion->setText(tr("Trial Version"));
	m_pLbRegisterVersion->setText(tr("Registered Version"));
	m_pBtnRegister->setText(tr("Register"));
	m_pBtnBuyNow->setText(tr("Buy Now"));
	m_pBtnEvaluate->setText(tr("Continue"));
	m_titlebar->GetButton(TopLevelWindowTitleBar::Close)->setToolTip(tr("Close"));
}

void ExportWarringDlg::resizeEvent(QResizeEvent *e)
{
	m_titlebar->setGeometry(1, 1, width() - 2, 34);

	int w = 271;
	int h = 48;
	int l = (width() - w) * 0.5;
	int t = 52;
	m_pLbLogo->setGeometry(l, t, w, h);

	int margin = 64;
	t = m_pLbLogo->geometry().bottom() + 60;
	l = margin;
	w = width() - 2 * margin;
	h = 80;
	m_pLbContent->setGeometry(l, t, w, h);

	l = margin;
	t = m_pLbContent->geometry().bottom() + 1;
	w = 303;
	h = 20;
	m_pLbTrialVersion->setGeometry(l, t, w, h);

	l = m_pLbTrialVersion->geometry().right() + 68;
	m_pLbRegisterVersion->setGeometry(l, t, w, h);

	t = m_pLbTrialVersion->geometry().bottom() + 16;
	l = m_pLbTrialVersion->geometry().left();
	w = m_pLbTrialVersion->geometry().width();
	h = 170;
	m_pLbTrialImg->setGeometry(l, t, w, h);

	l = m_pLbRegisterVersion->geometry().left();
	m_pLbRegisterImg->setGeometry(l, t, w, h);

	l = m_pLbTrialImg->geometry().left();
	t = m_pLbTrialImg->geometry().bottom() + 30;
	w = 120;
	h = 40;

    int wTemp = GetRetranslateUiTextLenght(m_pBtnRegister->font(), m_pBtnRegister->text(), w, 10);
    m_pBtnRegister->setGeometry(l, t, wTemp, h);

    wTemp = GetRetranslateUiTextLenght(m_pBtnBuyNow->font(), m_pBtnBuyNow->text(), w, 10);
	l = m_pBtnRegister->geometry().right() + 300;
    m_pBtnBuyNow->setGeometry(l, t, wTemp, h);

    wTemp = GetRetranslateUiTextLenght(m_pBtnEvaluate->font(), m_pBtnEvaluate->text(), w, 10);
    l = m_pBtnBuyNow->geometry().right() + 10;
	m_pBtnEvaluate->setGeometry(l, t, wTemp, h);
}
