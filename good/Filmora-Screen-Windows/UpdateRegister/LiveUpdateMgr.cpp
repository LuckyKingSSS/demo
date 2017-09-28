#include "stdafx.h"
#include "LiveUpdateMgr.h"
#include "UpdateCommon.h"
#include "inc_UpdateRegister/UpdateMain.h"
#include "inc_UpdateRegister/UpdateRegister_global.h"
#include <QNetworkReply>


LiveUpdateMgr::LiveUpdateMgr(ProgressDisplay *progressDisplay, VersionDisplay *versionDisplay, HintDisplay *hintDisplay, QObject *parent)
: QObject(parent)
, m_checkType(ManualCheck)
, m_bytesTotal(0)
, m_updateUrl(UpdateInit::GetUpdateUrl())
, m_settings(UpdateRegisterInit::GetConfigFileDir() + "/UpdateRegister.ini", QSettings::IniFormat)
{
	m_progressDisplay = progressDisplay;
	m_versionDisplay = versionDisplay;
	m_hintDisplay = hintDisplay;
	m_versionInfo = new VersionInfo("", this);
	m_networkMgr = new QNetworkAccessManager(this);
	m_downloadMgr = new DownloadMgr(this);

	connect(m_networkMgr, &QNetworkAccessManager::finished, this, &LiveUpdateMgr::ReplyFinished);
	connect(m_versionDisplay, &VersionDisplay::UpdateNow, this, &LiveUpdateMgr::UpdateNow);
	connect(m_versionDisplay, &VersionDisplay::RemindMeLater, this, &LiveUpdateMgr::RemindMeLater);
	connect(m_downloadMgr, &DownloadMgr::DownloadProgress, this, &LiveUpdateMgr::DownloadProgress);
	connect(m_downloadMgr, &DownloadMgr::Close, this, &LiveUpdateMgr::Close);
	connect(m_downloadMgr, &DownloadMgr::BeginInstall, this, &LiveUpdateMgr::BeginInstall);
	connect(m_downloadMgr, &DownloadMgr::InstallStarted, this, &LiveUpdateMgr::InstallStarted);

	connect(m_downloadMgr, &DownloadMgr::CancelInstall, this, [&](){
		//取消安装，则记录文件路径
		m_settings.beginGroup("UpdateRegister");
		m_settings.setValue("SetupFile", m_downloadMgr->GetSetupFilePath());
		m_settings.endGroup();
		emit CancelInstall();
	}); //&LiveUpdateMgr::CancelInstall);
	connect(m_downloadMgr, &DownloadMgr::Error, this, [&](QString error){
		m_hintDisplay->SetHint(error);
		emit SetCurrentWidget(m_hintDisplay);
	});
	connect(m_networkMgr, &QNetworkAccessManager::sslErrors, this, [&](){
		if (m_checkType == ManualCheck){
			m_hintDisplay->SetHint(tr("Failed to check update!"));
		}
		emit SetCurrentWidget(m_hintDisplay);
	});
	connect(m_progressDisplay, &ProgressDisplay::Close, this, [&](){
		m_downloadMgr->DownloadCancel();
		emit Close();
	});
	connect(m_hintDisplay, &HintDisplay::Close, this, &LiveUpdateMgr::Close);
	emit SetCurrentWidget(m_progressDisplay);
}

LiveUpdateMgr::~LiveUpdateMgr()
{
	delete m_downloadMgr;
}

void LiveUpdateMgr::LiveUpdate(QString productId, QString version, QString email)
{
	m_checkType = ManualCheck;
	_LiveUpdate(productId, version, email);
}

void LiveUpdateMgr::LiveUpdate()
{
	m_checkType = ManualCheck;
	_LiveUpdate();
}

bool LiveUpdateMgr::IsNeedUpdate()
{
    m_checkType = AutoCheck;
    m_settings.beginGroup("UpdateRegister");
    QString ProdID = m_settings.value("ProdID").toString();
    QString Version = m_settings.value("Version").toString();
    if (Version.count(".") == 3){ // 若版本号有四位，则只取前三位
        Version = Version.mid(0, Version.lastIndexOf("."));
    }
    QString Email = m_settings.value("Email").toString();
    m_settings.endGroup();
    m_request.setUrl(FormatURL(ProdID, Version, Email));

    NetworkAccessManagerTimeout *networkMrg = new NetworkAccessManagerTimeout;
    QByteArray byteArray = networkMrg->get(m_request);
    if (!byteArray.isEmpty())
    {
        m_versionInfo->SetXML(byteArray);
        return m_versionInfo->HasUpdateVersion();
    }

 //   m_reply = m_networkMgr->get(m_request);
 //   QEventLoop eventLoop;
 //   connect(m_networkMgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
 //   QTimer timerTemp;
 //   bool bTimeout = false;
 //   connect(&timerTemp, &QTimer::timeout, this, [&](){
 //       bTimeout = true;
 //       eventLoop.quit();
 //   });
 //   timerTemp.start(3000);

	//eventLoop.exec();       // block until finish
 //   if (m_reply->error() != QNetworkReply::NoError || bTimeout){  // 网络错误等
	//	return false;
	//}
	//else{  // 网络正常
	//	QString xml = "";
	//	while (m_reply->bytesAvailable()){
	//		QByteArray buf = m_reply->readAll();
	//		xml.append(buf);
	//	}
	//	m_reply->deleteLater();
	//	m_versionInfo->SetXML(xml);
	//	return m_versionInfo->HasUpdateVersion();
	//}
	return false;
}

void LiveUpdateMgr::_LiveUpdate()
{
	m_settings.beginGroup("UpdateRegister");
	QString ProdID = m_settings.value("ProdID").toString();
	QString Version = m_settings.value("Version").toString();
	if (Version.count(".") == 3){ // 若版本号有四位，则只取前三位
		Version = Version.mid(0, Version.lastIndexOf("."));
	}
	QString Email = m_settings.value("Email").toString();
	m_settings.endGroup();
	_LiveUpdate(ProdID, Version, Email);
}

void LiveUpdateMgr::_LiveUpdate(QString productId, QString version, QString email)
{
	emit SetCurrentWidget(m_progressDisplay);
	m_progressDisplay->SetText(tr("Checking the version."));
	m_progressDisplay->SetBusyEffect();
	m_progressDisplay->SetShowState(ProgressDisplay::ShowState_Normal);
	m_settings.beginGroup("UpdateRegister");
	QString setupFile = m_settings.value("SetupFile").toString();
	m_settings.endGroup();
	if (!QFileInfo(setupFile).exists()){  // SetupFile文件不存在
		m_request.setUrl(FormatURL(productId, version, email));
        m_reply = m_networkMgr->get(m_request);
        connect(m_reply, &QNetworkReply::sslErrors, this, [&](){
            m_hintDisplay->SetHint(tr("Failed to check update!"));
            emit SetCurrentWidget(m_hintDisplay);
        });
        connect(m_reply, &QNetworkReply::readyRead, this, &LiveUpdateMgr::DownloadXML);
	}
	else{ // 直接安装已下载版本
		m_downloadMgr->InstallSetupFile(setupFile);
	}
}

//下载应用程序
void LiveUpdateMgr::UpdateNow()
{
	QString loadPath = m_versionInfo->GetNewVersionPath();
	QString filePath = QDir::tempPath() + "/" + m_versionInfo->GetNewVersionName();  // 下载到系统临时目录下
	if (!loadPath.isEmpty()){
		m_progressDisplay->SetText(tr("Downloading the update, please wait."));
		m_progressDisplay->SetShowState(ProgressDisplay::ShowState_Download);
		emit SetCurrentWidget(m_progressDisplay);
		m_downloadMgr->Download(filePath, QUrl(loadPath));
	}
}

void LiveUpdateMgr::ReplyFinished(QNetworkReply *reply)
{
	if (reply->error() != QNetworkReply::NoError){
		if (m_checkType == ManualCheck){  // 手动检查才会弹窗
			m_hintDisplay->SetHint(tr("Failed to check update!"));  // reply->errorString();
		}
		emit SetCurrentWidget(m_hintDisplay);
	}
}

void LiveUpdateMgr::Cancel()
{
	if (m_downloadMgr) m_downloadMgr->DownloadCancel();
}

void LiveUpdateMgr::Install()
{
	if (m_downloadMgr) m_downloadMgr->Install();
}

QUrl LiveUpdateMgr::FormatURL(const QString& productId, const QString& version, const QString& email)
{
	QString updateUrl = m_updateUrl;
	updateUrl.insert(updateUrl.indexOf("pid=") + 4, productId);
	updateUrl.insert(updateUrl.indexOf("version=") + 8, version);
	updateUrl.insert(updateUrl.indexOf("email=") + 6, email);
	return QUrl(updateUrl);
}

void LiveUpdateMgr::DownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
	if (m_progressDisplay && bytesTotal){
		m_bytesTotal = m_bytesTotal;
		m_progressDisplay->SetText(QString(tr("Downloading the update, please wait...(downloaded %1,total %2)")).arg(BytesToString(bytesReceived)).arg(BytesToString(bytesTotal)));
		m_progressDisplay->SetMaximum(bytesTotal);
		m_progressDisplay->SetValue(bytesReceived);
	}
}

QString LiveUpdateMgr::BytesToString(qint64 bytes)
{
	QString strUnit;
	double dSize = bytes * 1.0;
	if (dSize <= 0){
		dSize = 0.0;
	}
	else if (dSize < 1024){
		strUnit = "Bytes";
	}
	else if (dSize < 1024 * 1024){
		dSize /= 1024;
		strUnit = "KB";
	}
	else if (dSize < 1024 * 1024 * 1024){
		dSize /= (1024 * 1024);
		strUnit = "MB";
	}
	else{
		dSize /= (1024 * 1024 * 1024);
		strUnit = "GB";
	}
	return QString("%1 %2").arg(QString::number(dSize, 'f', 2)).arg(strUnit);
}

void LiveUpdateMgr::DownloadXML()
{
	emit SetCurrentWidget(m_versionDisplay);
	//QString contentTypeHeader = m_reply->header(QNetworkRequest::ContentTypeHeader).toString();
	QString xml = "";
	while (m_reply->bytesAvailable()){
		QByteArray buf = m_reply->readAll();
		xml.append(buf);
	}
	m_reply->deleteLater();

    m_versionInfo->SetXML(xml);
    if (!m_versionInfo->HasUpdateVersion()){  // 无可用更新
        m_hintDisplay->SetHint(tr("Your current version is the latest version!"));
        emit SetCurrentWidget(m_hintDisplay);
    }
    else{  // 有可用更新
        QString hint("<h4><b style=\"color:#D7D8DB\">" + tr("Find new versions. You can update to the latest version. Would you like to update now?") + "</b><br></h4>");
        QString title("<h4><b style=\"color:#D7D8DB\">" + tr("What's new:") + "</b></h4>");
        QString description;
        auto whatNews = m_versionInfo->GetWhatNews();
        for (auto news : whatNews){
            description.append(QString("<b style=\"color:#D7D8DB\">" + tr("Version %1:") + "</b><b style=\"color:#516A7C\">%2<b>").arg(news.first).arg(news.second));
        }
        m_versionDisplay->SetText(hint + title + description);
    }
}





