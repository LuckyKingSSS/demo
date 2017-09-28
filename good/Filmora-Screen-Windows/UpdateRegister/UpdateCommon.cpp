#include "stdafx.h"
#include "UpdateCommon.h"
#include "CommonWidgets.h"
#include "InternalAPI.h"
#include <QDomDocument>
#include <QMessageBox>

//////////////////////////////////////////////////////////////////////////
//VersionInfo
VersionInfo::VersionInfo(QString xml, QObject *parent)
: QObject(parent)
, m_xml(xml)
, m_newVersionPath("")
{
	ParseXml();
}

void VersionInfo::SetXML(QString xml)
{
	m_xml = xml;
	ParseXml();
}

bool VersionInfo::HasUpdateVersion()
{
	return m_newVersionPath.isEmpty() ? false : true;
}

QList<QPair<QString, QString>> VersionInfo::GetWhatNews()
{
	return m_whatNews;
}

QString VersionInfo::GetNewVersionPath()
{
	return m_newVersionPath;
}

QString VersionInfo::GetNewVersionName()
{
	return m_newVersionPath.mid(m_newVersionPath.lastIndexOf("/") + 1);
}

void VersionInfo::ParseXml()
{
	m_whatNews.clear();
	m_newVersionPath.clear();

	QDomDocument doc;
	QString strError = "";
	int nRow = 0, nColumn = 0;
	doc.setContent(m_xml, false, &strError, &nRow, &nColumn);
	QDomElement root = doc.documentElement();
	m_newVersionPath = root.elementsByTagName("ExeUrl_x64").item(0).toElement().text();
	QDomNodeList items = root.elementsByTagName("WhatNews").item(0).toElement().elementsByTagName("Item");
	for (int i = 0; i < items.count(); ++i){
		QDomElement item = items.item(i).toElement();
		QString versionNum = item.elementsByTagName("Version").at(0).toElement().text();
		QString text = item.elementsByTagName("Text").at(0).toElement().text();
		text = Normalization(text);
		m_whatNews.append(QPair<QString, QString>(versionNum, text));
	}
}

QString VersionInfo::Normalization(QString text)
{
	if (!text.contains("<ol>")){
		text.insert(0, "<ol>");
		text.append("</ol>");
	}
	return text;
}

//////////////////////////////////////////////////////////////////////////

NetworkCheck::NetworkCheck()
:QObject()
{
	m_pThread = new QThread;
	m_pTimer = new QTimer(this);
	m_pTimer->setInterval(30000);  //定时器默认30秒
	connect(m_pTimer, &QTimer::timeout, this, [&](){
		emit TimeOut();
	});
	m_pThread->start();
	moveToThread(m_pThread);
}

NetworkCheck::~NetworkCheck()
{
	m_pThread->deleteLater();
}

void NetworkCheck::SetInterval(int msec)
{
	m_pTimer->setInterval(msec);
}

void NetworkCheck::SetBytesReceived(int64_t bytesReceived)
{
	m_bytesReceived = bytesReceived;
}

int64_t NetworkCheck::GetLastBytesReceived()
{
	return m_bytesReceived;
}

void NetworkCheck::Check()
{
	QMetaObject::invokeMethod(this, "_Check", Qt::QueuedConnection);
}

void NetworkCheck::StopCheck()
{
	QMetaObject::invokeMethod(this, "_StopCheck", Qt::QueuedConnection);
}

Q_INVOKABLE void NetworkCheck::_StopCheck()
{
	m_pTimer->stop();
}

Q_INVOKABLE void NetworkCheck::_Check()
{
	m_pTimer->start();
}

//////////////////////////////////////////////////////////////////////////
//DownloadMgr
//#include <QMessageBox>
static std::string sError_DownloadError = "Failed to download the update.";
static std::string sError_FileOpenError = "File open error!";

DownloadMgr::DownloadMgr(QObject *parent)
: QObject(parent)
, m_reply(0)
, m_bInstall(false)
{
	tr("Failed to download the update.");
	tr("File open error!");
	m_process = new QProcess;
	connect(m_process, &QProcess::started, this, &DownloadMgr::InstallStarted);
	m_messageBox = new URMessageBox(URMessageBox::YesCancelMode, 0);
	m_networkCheck = new NetworkCheck;
	connect(m_networkCheck, &NetworkCheck::TimeOut, this, [&](){
		if (m_bytesReceived == m_networkCheck->GetLastBytesReceived()){
			m_reply->abort();
			m_networkCheck->StopCheck();
			m_errorString = sError_DownloadError;
			emit Error(tr(m_errorString.c_str()));  // tr("Failed to download the update.");
		}
		else{
			m_networkCheck->SetBytesReceived(m_bytesReceived);
		}
	});
	connect(m_messageBox, &URMessageBox::Yes, [&](){
		m_bInstall = true;
		emit BeginInstall();
		emit Close();
	});
	connect(m_messageBox, &URMessageBox::Cancel, [&](){
		m_bInstall = false;
		emit CancelInstall();
		emit Close();
	});
	m_networkMgr = new QNetworkAccessManager(this);
	connect(m_networkMgr, &QNetworkAccessManager::finished, this, &DownloadMgr::ReplyFinished);
	connect(m_networkMgr, &QNetworkAccessManager::sslErrors, this, [&](QNetworkReply *reply, const QList<QSslError> &errors){
		m_reply->abort();
		m_networkCheck->StopCheck();
		m_errorString = sError_DownloadError;
		emit Error(tr(m_errorString.c_str()));  // tr("Failed to download the update.");
	});
}

DownloadMgr::~DownloadMgr()
{
	m_networkCheck->deleteLater();
	m_messageBox->deleteLater();
}

void DownloadMgr::Download(QString filePath, QUrl url)
{
	m_file.setFileName(filePath);
	if (m_file.exists()) m_file.remove();  // 如果该目录下已有同名文件，则先删除旧文件
	
	m_reply = m_networkMgr->get(QNetworkRequest(url));
	//m_reply = m_networkMgr->get(QNetworkRequest(QUrl("http://sw.bos.baidu.com/sw-search-sp/av_aladdin/698fafcf7bf29/rj_ye4862.exe")));  // test
	connect(m_reply, &QNetworkReply::readyRead, this, &DownloadMgr::ReadyRead);
	connect(m_reply, &QNetworkReply::downloadProgress, this, &DownloadMgr::DownloadProgressMgr);
	connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(ReplyError(QNetworkReply::NetworkError)));
	m_networkCheck->SetBytesReceived(0);
	m_networkCheck->Check();
}

void DownloadMgr::DownloadCancel()
{
	if (m_file.exists()) m_file.remove();
	if (m_networkCheck) m_networkCheck->StopCheck();
	if (m_reply) {
		m_reply->abort();
		m_reply->deleteLater();
	}
}

void DownloadMgr::Install()
{
	if (m_bInstall){
		//m_file.setFileName("C:/Users/Administrator/Desktop/ha_innosetup5309_skygz.exe");  // test
		m_process->setProgram(m_file.fileName());
		//QStringList arguments;
		//arguments << "/SILENT" << "/REINSTALL";
		//m_process->setArguments(arguments);
		m_process->start();
		m_process->waitForStarted();
	}
}

QString DownloadMgr::GetSetupFilePath()
{
	return m_file.fileName();
}

void DownloadMgr::InstallSetupFile(QString path)
{
	if (QFileInfo(path).exists()){
		m_file.setFileName(path);
		m_messageBox->Information(tr("Information"), tr("A new version has been found in your device. However, the program is running. Please click Yes to terminate the program and install the new version. Click Cancel to install it next time."));
	}
}

bool DownloadMgr::event(QEvent* event)
{
	if (event->type() == QEvent::LanguageChange){
		ReTranslateUI();
	}
	return QObject::event(event);
}

void DownloadMgr::ReTranslateUI()
{
	emit Error(tr(m_errorString.c_str()));
}

void DownloadMgr::ReplyFinished(QNetworkReply *reply)
{
	QNetworkReply::NetworkError code = reply->error();
	if (code == QNetworkReply::NoError){
		m_file.close();
		QUrl url = reply->header(QNetworkRequest::LocationHeader).toUrl();
		if (!url.isEmpty()){  //必须添加判断
			m_reply->deleteLater();
			m_reply = m_networkMgr->get(QNetworkRequest(url));
			connect(m_reply, &QNetworkReply::readyRead, this, &DownloadMgr::ReadyRead);
			connect(m_reply, &QNetworkReply::downloadProgress, this, &DownloadMgr::DownloadProgressMgr);
			connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(ReplyError(QNetworkReply::NetworkError)));
			connect(m_reply, &QNetworkReply::sslErrors, this, [&](){
				m_reply->abort();
				m_networkCheck->StopCheck();
				m_errorString = sError_DownloadError;
				emit Error(tr(m_errorString.c_str()));  // tr("Failed to download the update.");
			});
		}
	}
	else{
		if (code != QNetworkReply::OperationCanceledError){  // 排除主动Cancel操作
			m_reply->abort();
			m_networkCheck->StopCheck();
			m_errorString = sError_DownloadError;
			emit Error(tr(m_errorString.c_str()));  // tr("Failed to download the update.");
		}
	}
}

void DownloadMgr::ReadyRead()
{
	QString contentTypeHeader = m_reply->header(QNetworkRequest::ContentTypeHeader).toString();
	if (m_reply->error() == QNetworkReply::NoError){
		bool canDownLoad = ((contentTypeHeader == "application/octet-stream") || (contentTypeHeader == "application/x-msdownload"));
		if (canDownLoad){
			if (!m_file.isOpen()){
				if (false == m_file.open(QIODevice::Append)){
					m_reply->abort();
					m_networkCheck->StopCheck();
					m_errorString = sError_FileOpenError;
					emit Error(tr(m_errorString.c_str()));  // tr("File open error!");
				}
			}
			m_file.write(m_reply->readAll());  // 接收到的数据追加到文件
		}
	}
	else{
		m_reply->abort();
		m_networkCheck->StopCheck();
		m_errorString = sError_DownloadError;
		emit Error(tr(m_errorString.c_str()));  // tr("Failed to download the update.");
	}
}

void DownloadMgr::DownloadProgressMgr(qint64 bytesReceived, qint64 bytesTotal)
{
	m_bytesReceived = bytesReceived;
	emit DownloadProgress(bytesReceived, bytesTotal);
	if (bytesTotal && bytesReceived == bytesTotal){  //下载完成
		m_file.close();  // 下载完成，关闭文件
		m_reply->close();
		m_networkCheck->StopCheck();
		emit DownloadFinished();
		m_messageBox->Information(tr("Information"), tr("Download completed. However, the program is running. Please click Yes to terminate the program and install the new version. Click Cancel to install it next time."));
	}
}

void DownloadMgr::ReplyError(QNetworkReply::NetworkError code)
{
	m_reply->abort();
	m_networkCheck->StopCheck();
	m_errorString = sError_DownloadError;
	emit Error(tr(m_errorString.c_str()));  // tr("Failed to download the update.");
}

//////////////////////////////////////////////////////////////////////////
//ProgressDisplay
ProgressDisplay::ProgressDisplay(QWidget *parent)
: URBaseWidget(parent)
, m_showState(ShowState_Normal)
{
	setAttribute(Qt::WA_StyledBackground);
	m_infoLabel = new QLabel(this);
	m_valueLabel = new QLabel(this);
	m_progressBar = new QProgressBar(this);
	m_closeBtn = new QPushButton(/*tr("Close"),*/ this);
	m_progressBar->setTextVisible(false);
	connect(m_closeBtn, &QPushButton::clicked, this, &ProgressDisplay::Close);
	connect(m_progressBar, &QProgressBar::valueChanged, this, [&](){
		m_valueLabel->setText(m_progressBar->text());
	});
	m_infoLabel->setObjectName("UpdateProgressLabel");
	m_progressBar->setObjectName("UpdateProgressBar");
	RetranslateUi();
}

void ProgressDisplay::SetText(QString info)
{
	//QString elideStr = m_infoLabel->fontMetrics().elidedText(info, Qt::ElideRight, m_infoLabel->width(), Qt::TextShowMnemonic);
	//m_infoLabel->setText(elideStr);
	m_infoLabel->setText(info);
}

void ProgressDisplay::SetBusyEffect()
{
	m_progressBar->setMaximum(0);
	m_progressBar->setMinimum(0);
}

void ProgressDisplay::SetMaximum(qint64 max)
{
	m_progressBar->setMaximum(max);
}

void ProgressDisplay::SetMinimum(qint64 min)
{
	m_progressBar->setMinimum(min);
}

void ProgressDisplay::SetRange(qint64 min, qint64 max)
{
	if (min <= max){
		m_progressBar->setMinimum(min);
		m_progressBar->setMaximum(max);
	}
}

void ProgressDisplay::SetFormat(const QString& format)
{
	m_progressBar->setFormat(format);
}

void ProgressDisplay::SetValue(qint64 value)
{
	m_progressBar->setValue(value);
}

void ProgressDisplay::SetShowState(ShowState state)
{
	m_showState = state;
	(m_showState == ShowState_Normal) ? m_closeBtn->hide() : m_closeBtn->show();
}

ProgressDisplay::ShowState ProgressDisplay::GetCurrentState()
{
	return m_showState;
}

void ProgressDisplay::RetranslateUi()
{
	m_closeBtn->setText(tr("Close"));
}

void ProgressDisplay::resizeEvent(QResizeEvent *)
{
	int left = 20;
	int right = 50;
	const int wid = width() - left - right;
	const int labelHeight = 20;
	const int barHeight = 4;
	int top = height() / 2 - 30;
	m_infoLabel->setGeometry(left, top - labelHeight - 2, width() - left, labelHeight);
	top = m_infoLabel->geometry().bottom() + 20;
	m_progressBar->setGeometry(left, top, wid, barHeight);
	left = m_progressBar->geometry().right() + 5;
	m_valueLabel->setGeometry(left, top - 20, 40, 40);
	m_closeBtn->setGeometry(width() - 20 - 80, height() - 50, 80, 30);
}

//////////////////////////////////////////////////////////////////////////
//VersionDisplay
VersionDisplay::VersionDisplay(QWidget *parent)
: URBaseWidget(parent)
{
	setAttribute(Qt::WA_StyledBackground);
	m_scrollArea = new QScrollArea(this);
	m_versionsLabel = new QLabel;
	m_versionsLabel->setWordWrap(true);   // QLabel多行展示
	m_versionsLabel->setObjectName("VersionDisplay_VersionLabel");
	m_scrollArea->setWidgetResizable(true);	
	m_scrollArea->setWidget(m_versionsLabel);
	m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	m_updateNowBtn = new QPushButton(/*tr("Update Now"),*/ this);
	m_remindMeLaterBtn = new QPushButton(/*tr("Remind Me Later"),*/ this);
	connect(m_updateNowBtn, &QPushButton::clicked, this, &VersionDisplay::UpdateNow);
	connect(m_remindMeLaterBtn, &QPushButton::clicked, this, &VersionDisplay::RemindMeLater);
	RetranslateUi();
}

void VersionDisplay::SetText(QString text)
{
	m_versionsLabel->setText(text);
}

void VersionDisplay::RetranslateUi()
{
	m_updateNowBtn->setText(tr("Update Now"));
	m_remindMeLaterBtn->setText(tr("Remind Me Later"));
	resizeEvent(nullptr);
}

void VersionDisplay::resizeEvent(QResizeEvent *)
{
	int left = 20;
	int top = 0;
	const int textHeight = height() - 50;
	const int btnHeight = 30;
	m_scrollArea->setGeometry(left, top, width() - left, textHeight);
	top = m_scrollArea->geometry().bottom() + 10;
	int UpdateNowBtnWidth = InternalAPI::GetTextTrueLength(m_updateNowBtn->font(), m_updateNowBtn->text()) + 20;
	int RemindMeLaterBtnWidth = InternalAPI::GetTextTrueLength(m_remindMeLaterBtn->font(), m_remindMeLaterBtn->text()) + 20;
	m_remindMeLaterBtn->setGeometry(width() - 20 - RemindMeLaterBtnWidth, top, RemindMeLaterBtnWidth, btnHeight);
	left = m_remindMeLaterBtn->geometry().left();
	m_updateNowBtn->setGeometry(left - 20 - UpdateNowBtnWidth, top, UpdateNowBtnWidth, btnHeight);
}

//////////////////////////////////////////////////////////////////////////
//HintDisplay
HintDisplay::HintDisplay(QWidget *parent)
: URBaseWidget(parent)
{
	setAttribute(Qt::WA_StyledBackground);
	m_label = new QLabel(this);
	m_closeBtn = new QPushButton(/*tr("Close"),*/ this);
	connect(m_closeBtn, &QPushButton::clicked, this, &HintDisplay::Close);
	RetranslateUi();
}

void HintDisplay::SetHint(QString hint)
{
	m_label->setText(hint);
}

void HintDisplay::RetranslateUi()
{
	m_closeBtn->setText(tr("Close"));
}

void HintDisplay::resizeEvent(QResizeEvent *)
{
	int left = 10, top = 0;
	m_label->setGeometry(left, top, width() - left, height() - top - 70);
	top = m_label->geometry().bottom() + 10;
	m_closeBtn->setGeometry(width() - 20 - 80, top, 80, 30);
}

/*NetworkAccessManagerTimeout*/
NetworkAccessManagerTimeout::NetworkAccessManagerTimeout(QObject *parent)
:QThread(parent)
, m_reply(nullptr)
{
    m_bError = true;
}

NetworkAccessManagerTimeout::~NetworkAccessManagerTimeout()
{

}

QByteArray NetworkAccessManagerTimeout::get(QNetworkRequest request, int waittime /*= 3000*/)
{
    m_request = request;
    m_bError = true;
    start();
    wait(waittime);
    if (isRunning())
    {
        //terminate();
        //TerminateThread(this->currentThreadId(), 0);
    }
    else
    {
        this->deleteLater();
    }
    return m_bypteArray;
}


void NetworkAccessManagerTimeout::run()
{
  
    QNetworkAccessManager manager;
    QEventLoop eventLoop;
    connect(&manager, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
    // 线程退出m_reply会被自动析构提
    m_reply  = manager.get(m_request);
    eventLoop.exec();
    if (m_reply->error() == QNetworkReply::NoError)
    {
        m_bypteArray = m_reply->readAll();
    }
}





