#include "stdafx.h"
#include "UpdateRegisterPurchaseHelper.h"
#include "inc_UpdateRegister/UpdateMain.h"
#include "inc_UpdateRegister/RegisterMain.h"
#include "Communicator.h"
#include "inc_CommonLib/PathOperation.h"
#include "inc_FSCommonlib/ExternalAPI.h"
#include "inc_CommonLib/XmlOperation.h"
#include "inc_FSCommonlib/CBSUrlReader.h"
#include "RecorderViewsMgr.h"
#include "FSRecorder.h"


UpdateRegisterPurchaseHelper::UpdateRegisterPurchaseHelper(QObject *parent)
	: QObject(parent)
{
	m_pCBSUrlReader = new CBSUrlReader(this);
	UpdateRegisterInit::SetConfigFileDir(qApp->applicationDirPath());
	UpdateInit::SetUpdateUrl("http://cbs.wondershare.com/go.php?m=upgrade_info&pid=&version=&email=");
	RegisterInit::SetBuyNowUrl(m_pCBSUrlReader->ReadCustomerUrl(GetCurrentLang(), "OrderPage"));
	m_pUpdateMain = new UpdateMain(0);
	m_pSetting = new QSettings(PathOperation::MakeWorkPath("UpdateRegister.ini"), QSettings::IniFormat);
	connect(m_pUpdateMain, &UpdateMain::BeginInstall, this, &UpdateRegisterPurchaseHelper::OnBeginInstall);
	connect(m_pUpdateMain, &UpdateMain::InstallStarted, this, &UpdateRegisterPurchaseHelper::OnInstallStarted);
	connect(&m_timer, &QTimer::timeout, this, &UpdateRegisterPurchaseHelper::OnAutoQuery);

	RebuildRegisterMain();
	connect(Communicator::GetInstance(), &Communicator::sigRegisterResult, this, [&](bool bSucc){RebuildRegisterMain(); });
}

UpdateRegisterPurchaseHelper* UpdateRegisterPurchaseHelper::GetInstance()
{
	static UpdateRegisterPurchaseHelper ins;
	return &ins;
}

void UpdateRegisterPurchaseHelper::DoAutoUpdate()
{
	bool isAutoUpdateOn = true;  //从文件配置文件读取
	auto path = FSGetFilmoraScreenDirectory() + "/settingdatas.xml";
	if (!QFile::exists(path))
		isAutoUpdateOn = true;
	else
	{
		QDomDocument doc;
		XmlOperation::OpenXmlFile(doc, path);
		auto rootElem = doc.documentElement();
		if (!rootElem.isNull())
		{
			auto autoElem = rootElem.firstChildElement("AutoUpgrade");
			if (!autoElem.isNull()) 
			{
				isAutoUpdateOn = autoElem.text() == "0" ? false : true;
			}
		}
	}
	if (!isAutoUpdateOn) return;
	m_timer.start(60 * 60 * 1000); //每小时查询一次
	QTimer::singleShot(0, [&]{OnAutoQuery(); });
}

void UpdateRegisterPurchaseHelper::OnBeginInstall()
{
	ProjPreproccessor::GetInstance()->DoQuestionSaving();
	m_pUpdateMain->Install();
}

void UpdateRegisterPurchaseHelper::OnInstallStarted()
{
	m_pSetting->beginGroup("UpdateRegister");
	m_pSetting->setValue("LastUpdateTime", QDateTime::currentDateTime());
	m_pSetting->endGroup();
	qApp->quit();
}

void UpdateRegisterPurchaseHelper::OnAutoQuery()
{
	//是否到时间做升级检查了
	bool bQuery = false;     //重记录在文件的时间来判断
	m_pSetting->beginGroup("UpdateRegister");
	auto datetime = m_pSetting->value("LastUpdateTime").toDateTime();
	m_pSetting->endGroup();
	auto delta = QDateTime::currentDateTime().toMSecsSinceEpoch() - datetime.toMSecsSinceEpoch();
	bQuery = delta > (7 * 24 * 60 * 60 * 1000);

	if (!bQuery) return;

	if (!m_pUpdateMain->IsNeedUpdate()) return;
	
	m_timer.stop();
	m_pUpdateMain->LiveUpdate();
}

void UpdateRegisterPurchaseHelper::RebuildRegisterMain(bool bTellEditor/* = false*/)
{
	if (m_pRegisterMain)
	{
		delete m_pRegisterMain;
		m_pRegisterMain = nullptr;
	}
	m_pRegisterMain = new RegisterMain(0);
	connect(m_pRegisterMain, &RegisterMain::signalCheckOnRegist, this, &UpdateRegisterPurchaseHelper::singnalSucessRegister);
	connect(m_pRegisterMain, &RegisterMain::singalStartRegister, this, &UpdateRegisterPurchaseHelper::singnalStartRegister);
	connect(m_pRegisterMain, &RegisterMain::signalCheckOnStart, this, &UpdateRegisterPurchaseHelper::signalCheckOnStart);
	connect(m_pRegisterMain, &RegisterMain::signalCheckOnRegist, this, &UpdateRegisterPurchaseHelper::sigRegisterResult);
	if(bTellEditor) emit sigRegisterResult(IsRegistered());
	//RecorderViewsMgr::GetInstance()->GetFSRecorder()->RetranslateUi();
}

QString UpdateRegisterPurchaseHelper::GetCurrentLang()
{
	QString filmoraScrnDir = FSGetFilmoraScreenDirectory();
	QString iniPath = filmoraScrnDir + "/FilmoraScrn.ini";
	if (!QFile(iniPath).exists()) { return "ENG"; }
	QSettings setting(iniPath, QSettings::IniFormat);
	setting.beginGroup("System");
	auto langname = setting.value("DefInstallLang").toString();
	setting.endGroup();
	return langname;

}

void UpdateRegisterPurchaseHelper::Register()
{
	m_pRegisterMain->DoRegister();
}

void UpdateRegisterPurchaseHelper::Purchase()
{
	auto url = m_pCBSUrlReader->ReadCustomerUrl(GetCurrentLang(), "OrderPage");
	QDesktopServices::openUrl(QUrl(url));
}

void UpdateRegisterPurchaseHelper::RegisterCheckOnStart()
{
	m_pRegisterMain->RegisterCheckOnStart();
}

bool UpdateRegisterPurchaseHelper::IsRegistered()
{
	return m_pRegisterMain->CheckRegister();
}

