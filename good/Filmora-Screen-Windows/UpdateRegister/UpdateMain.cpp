#include "stdafx.h"
#include "inc_UpdateRegister/UpdateMain.h"
#include "UpdateMainImpl.h"
#include <QPainter>
#include <QHBoxLayout>

UpdateMain::UpdateMain(QWidget *parent)
	: QDialog(parent)
{
	setAttribute(Qt::WA_StyledBackground);
	setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
	resize(QSize(570, 250));

	m_pImpl = new UpdateMainImpl;

	QHBoxLayout *pLayout = new QHBoxLayout(this);
	pLayout->addWidget(m_pImpl);
	pLayout->setMargin(0);

	connect(m_pImpl, &UpdateMainImpl::Close, this, [&](){
		emit Close();
		close();
	});
	connect(m_pImpl, &UpdateMainImpl::BeginInstall, this, &UpdateMain::BeginInstall);
	connect(m_pImpl, &UpdateMainImpl::CancelInstall, this, [&](){
		emit CancelInstall();
		close();
	}); // &UpdateMain::CancelInstall);
	connect(m_pImpl, &UpdateMainImpl::InstallStarted, this, [&](){
		emit InstallStarted();
		close();
	}); // &UpdateMain::InstallStarted);
	connect(m_pImpl, &UpdateMainImpl::RemindMeLater, this, [&](){
		emit RemindMeLater();
		close();
	});
	connect(m_pImpl, &UpdateMainImpl::Resize, this, [&](QSize size){
		resize(size);
	});
	connect(m_pImpl, &UpdateMainImpl::Move, this, [&](int x, int y){
		QRect rect = this->geometry();
		this->setGeometry(rect.x() + x, rect.y() + y, rect.width(), rect.height());
	});
	//setStyleSheet("background-color:#666666");
	//setStyleSheet(UpdateRegisterInit::gGetStyleSheet(":/UpdateRegister/Resources/UpdateRegister.css"));
}

void UpdateMain::LiveUpdate(QString productId, QString version, QString email)
{
	show();
	m_pImpl->LiveUpdate(productId, version, email);
}

void UpdateMain::LiveUpdate()
{
	show();
	m_pImpl->LiveUpdate();
}

bool UpdateMain::IsNeedUpdate()
{
	return m_pImpl->IsNeedUpdate();
}

void UpdateMain::Install()
{
	m_pImpl->Install();
}

//////////////////////////////////////////////////////////////////////////

static QString g_UpdateUrl;

void UpdateInit::SetUpdateUrl(QString url)
{
	g_UpdateUrl = url;
}

QString UpdateInit::GetUpdateUrl()
{
	return g_UpdateUrl;
}
