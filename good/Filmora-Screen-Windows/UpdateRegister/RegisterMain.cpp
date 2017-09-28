#include "stdafx.h"
#include "inc_UpdateRegister/RegisterMain.h"
#include "inc_UpdateRegister/UpdateRegister_global.h"
#include "RegisterMainImpl.h"
#include <QPainter>
#include <QHBoxLayout>

RegisterMain::RegisterMain(QWidget *parent)
: QDialog(parent)
{
	setFixedSize(QSize(570, 260));
	setModal(true);

	m_pImpl = new RegisterMainImpl;

	QHBoxLayout *pLayout = new QHBoxLayout(this);
	pLayout->addWidget(m_pImpl);
	pLayout->setMargin(0);

	setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
	connect(m_pImpl, &RegisterMainImpl::BuyNow, this, &RegisterMain::BuyNow);
	connect(m_pImpl, &RegisterMainImpl::FreeTrial, this, [&](){
		emit RegisterResult(false);
		emit FreeTrial();
		close();  // 免费试用，关闭窗口
	});
	connect(m_pImpl, &RegisterMainImpl::Close, this, [&](){
		emit RegisterResult(CheckRegister());
		close();
	});
	connect(m_pImpl, &RegisterMainImpl::ReFund, this, &RegisterMain::ReFund);
	connect(m_pImpl, &RegisterMainImpl::Move, this, [&](int x, int y){
		QRect rect = this->geometry();
		this->setGeometry(rect.x() + x, rect.y() + y, rect.width(), rect.height());
	});
	connect(m_pImpl, &RegisterMainImpl::singalStartRegister, this, &RegisterMain::singalStartRegister);
	connect(m_pImpl, &RegisterMainImpl::signalCheckOnStart, this, &RegisterMain::signalCheckOnStart);
	connect(m_pImpl, &RegisterMainImpl::signalCheckOnRegist, this, &RegisterMain::signalCheckOnRegist);

	setAttribute(Qt::WA_StyledBackground);
}

RegisterMain::~RegisterMain()
{

}

void RegisterMain::DoRegister()
{
	show();
}

void RegisterMain::RegisterCheckOnStart()
{
	m_pImpl->RegisterCheckOnStart();
}

void RegisterMain::DoUnregistered()
{
	m_pImpl->DoUnregistered();
}

bool RegisterMain::CheckRegister()
{
	return m_pImpl->CheckRegister();
}

//////////////////////////////////////////////////////////////////////////

static QString g_ConfigFileDir;

void UpdateRegisterInit::SetConfigFileDir(QString dir)
{
	g_ConfigFileDir = dir;
}

QString UpdateRegisterInit::GetConfigFileDir()
{
	return g_ConfigFileDir;
}

QString UpdateRegisterInit::gGetStyleSheet(QString path)
{
	QFile file(path);
	file.open(QFile::ReadOnly);
	QTextStream filetext(&file);
	QString stylesheet = filetext.readAll();
	file.close();
	return stylesheet;
}
//////////////////////////////////////////////////////////////////////////

static QString g_BuyNowUrl;

void RegisterInit::SetBuyNowUrl(QString url)
{
	g_BuyNowUrl = url;
}

QString RegisterInit::GetBuyNowUrl()
{
	return g_BuyNowUrl;
}

