#include "stdafx.h"
#include "UpdateMainImpl.h"
#include "LiveUpdateMgr.h"
#include "UpdateCommon.h"
#include "CommonWidgets.h"
#include "inc_UpdateRegister/UpdateRegister_global.h"

UpdateMainImpl::UpdateMainImpl(QWidget *parent)
: URBaseWidget(parent)
{
	setAttribute(Qt::WA_StyledBackground);

	m_titleBar = new TitleBar(this);
	//m_titleBar->SetTitle(tr("Live Update"));

	m_stackedWidget = new QStackedWidget(this);

	m_progressDisplay = new ProgressDisplay;
	m_versionDisplay = new VersionDisplay;
	m_hintDisplay = new HintDisplay;
	
	m_stackedWidget->addWidget(m_progressDisplay);
	m_stackedWidget->addWidget(m_versionDisplay);
	m_stackedWidget->addWidget(m_hintDisplay);

	m_updateMgr = new LiveUpdateMgr(m_progressDisplay, m_versionDisplay, m_hintDisplay, this);
	connect(m_updateMgr, &LiveUpdateMgr::Close, this, &UpdateMainImpl::Close);
	connect(m_updateMgr, &LiveUpdateMgr::RemindMeLater, this, &UpdateMainImpl::RemindMeLater);
	connect(m_updateMgr, &LiveUpdateMgr::BeginInstall, this, &UpdateMainImpl::BeginInstall);
	connect(m_updateMgr, &LiveUpdateMgr::InstallStarted, this, &UpdateMainImpl::InstallStarted);
	connect(m_updateMgr, &LiveUpdateMgr::CancelInstall, this, &UpdateMainImpl::CancelInstall);
	connect(m_updateMgr, &LiveUpdateMgr::SetCurrentWidget, this, [&](QWidget *widget){
		m_stackedWidget->setCurrentWidget(widget);
		if (widget == m_versionDisplay){
			emit Resize(QSize(600, 300));
		}
		else{
			emit Resize(QSize(600, 250));
		}
		widget->show();
	});
	connect(m_titleBar, &TitleBar::Move, this, &UpdateMainImpl::Move);
	connect(m_titleBar, &TitleBar::Close, this, &UpdateMainImpl::Close);

	setStyleSheet(UpdateRegisterInit::gGetStyleSheet(":/UpdateRegister/Resources/UpdateRegister.css"));

	RetranslateUi();
}

UpdateMainImpl::~UpdateMainImpl()
{
	
}

void UpdateMainImpl::resizeEvent(QResizeEvent *)
{
	int left, top, right, bottom;
	left = top = right = bottom = 1;
	const int titleBtnWidth = 30;
	m_titleBar->setGeometry(left, top, width() - left - right, titleBtnWidth);
	top = m_titleBar->geometry().bottom() + 1;
	m_stackedWidget->setGeometry(left, top, width() - left - right, height() - top);
}

void UpdateMainImpl::RetranslateUi()
{
	m_titleBar->SetTitle(tr("Live Update"));
}

void UpdateMainImpl::LiveUpdate(QString productId, QString version, QString email)
{
	m_updateMgr->LiveUpdate(productId, version, email);
}

void UpdateMainImpl::LiveUpdate()
{
	m_updateMgr->LiveUpdate();
}

bool UpdateMainImpl::IsNeedUpdate()
{
	return m_updateMgr->IsNeedUpdate();
}

void UpdateMainImpl::Install()
{
	m_updateMgr->Install();
}

