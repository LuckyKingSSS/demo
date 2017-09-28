#ifndef LIVEUPDATEMGR_H
#define LIVEUPDATEMGR_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QSettings>

class ProgressDisplay;
class VersionDisplay;
class HintDisplay;
class VersionInfo;
class DownloadMgr;

class LiveUpdateMgr : public QObject
{
	Q_OBJECT

public:
	enum CheckType{AutoCheck, ManualCheck};

public:
	LiveUpdateMgr(ProgressDisplay *progressDisplay, VersionDisplay *versionDisplay, HintDisplay *hintDisplay, QObject *parent = 0);
	~LiveUpdateMgr();

	//检查更新
	void LiveUpdate(QString productId, QString version, QString email);
	void LiveUpdate();
	bool IsNeedUpdate();
	//cancel 取消下载
	void Cancel();
	//安装新版本
	void Install();

signals:
	void SetCurrentWidget(QWidget *);
	void Close();
	//开始安装信号
	void BeginInstall();
	//取消安装信号
	void CancelInstall();
	//新版本安装程序完全启动信号
	void InstallStarted();
	//Remind Me Latter信号
	void RemindMeLater();

private:
	void _LiveUpdate();
	void _LiveUpdate(QString productId, QString version, QString email);
	void UpdateNow();
	void ReplyFinished(QNetworkReply *);
	QUrl FormatURL(const QString& productId, const QString& version, const QString& email);
	void DownloadProgress(qint64 bytesReceived, qint64 bytesTotal);
	QString BytesToString(qint64 bytes);
	void DownloadXML();

private:
	CheckType m_checkType;
	QString m_updateUrl;
	QSettings m_settings;
	QNetworkAccessManager *m_networkMgr;
	QNetworkRequest m_request;
	ProgressDisplay *m_progressDisplay;
	VersionDisplay *m_versionDisplay;
	HintDisplay *m_hintDisplay;
	VersionInfo *m_versionInfo;
	QNetworkReply *m_reply;
	qint64 m_bytesTotal;
	DownloadMgr *m_downloadMgr;
};

#endif // LIVEUPDATEMGR_H
