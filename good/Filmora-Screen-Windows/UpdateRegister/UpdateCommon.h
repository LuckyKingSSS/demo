#ifndef UPDATECOMMON_H
#define UPDATECOMMON_H
#include "CommonWidgets.h"
#include <QWidget>

//代表返回的XML文件的类
class VersionInfo : public QObject
{
	Q_OBJECT

public:
	VersionInfo(QString xml, QObject *parent = 0);

	//设置版本信息的XML文件
	void SetXML(QString xml);

	//是否有可更新版本
	bool HasUpdateVersion();

	//返回WhatNews标签内容
	//QString GetWhatNews();
	QList<QPair<QString, QString>> GetWhatNews();

	//返回新版本下载路径
	QString GetNewVersionPath();

	//返回新版本文件名
	QString GetNewVersionName();

private:
	void ParseXml();
	QString Normalization(QString text);

private:
	QString m_xml;
	QString m_newVersionPath;
	QList<QPair<QString, QString>> m_whatNews;  //QPair<Version, Text>
};

//下载网络检查类
#include <QThread>
#include <QTimer>
class NetworkCheck : public QObject
{
	Q_OBJECT

public:
	NetworkCheck();
	~NetworkCheck();
	//设置间隔时间
	void SetInterval(int msec);
	//设置bytesReceived
	void SetBytesReceived(int64_t bytesReceived);
	int64_t GetLastBytesReceived();
	//检测
	void Check();
	//停止检测
	void StopCheck();

signals:
	//超时信号
	void TimeOut();

protected:
	Q_INVOKABLE void _StopCheck();
	Q_INVOKABLE void _Check();

private:
	QThread *m_pThread;
	QTimer *m_pTimer;
	int64_t m_bytesReceived;
};

//下载管理类
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QProcess>
class URMessageBox;
class DownloadMgr : public QObject
{
	Q_OBJECT

public:
	DownloadMgr(QObject *parent = 0);
	~DownloadMgr();
	//下载接口，传入文件和下载路径
	void Download(QString filePath, QUrl url);
	//取消下载
	void DownloadCancel();
	//安装新版本
	void Install();
	//获取下载文件路径（完整路径，包含文件名）
	QString GetSetupFilePath();
	//安装指定文件
	void InstallSetupFile(QString path);

signals:
	//下载进度信息信号
	void DownloadProgress(qint64 bytesReceived, qint64 bytesTotal);
	//出错信号
	void Error(QString error);
	//关闭窗口信号
	void Close();
	//下载完成信号
	void DownloadFinished();
	//开始安装信号
	void BeginInstall();
	//取消安装信号
	void CancelInstall();
	//新版本安装程序完全启动信号
	void InstallStarted();

protected:
	bool event(QEvent*) override;
	void ReTranslateUI();

private slots:
	void ReplyFinished(QNetworkReply *reply);
	void ReadyRead();
	void DownloadProgressMgr(qint64 bytesReceived, qint64 bytesTotal);
	void ReplyError(QNetworkReply::NetworkError code);	

private:
	QProcess *m_process;
	QFile m_file;
	QUrl m_downloadUrl;
	QNetworkAccessManager *m_networkMgr;
	QNetworkReply *m_reply;
	NetworkCheck *m_networkCheck;
	int64_t m_bytesReceived;
	URMessageBox *m_messageBox;
	bool m_bInstall;
	std::string m_errorString;
};

//显示进度信息的窗口
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>

class ProgressDisplay : public URBaseWidget
{
	Q_OBJECT
public:
	enum ShowState{ShowState_Normal, ShowState_Download};

public:
	ProgressDisplay(QWidget *parent = 0);
	//设置进度信息
	void SetText(QString info);
	//设置进度条为busy效果
	void SetBusyEffect();
	//设置进度条最大值
	void SetMaximum(qint64 max);
	//设置进度条最小值
	void SetMinimum(qint64 min);
	//设置进度条范围
	void SetRange(qint64 min, qint64 max);
	//设置进度条显示格式
	void SetFormat(const QString& format);
	//设置进度条当前值
	void SetValue(qint64 value);
	//设置当前显示状态
	void SetShowState(ShowState state);
	ShowState GetCurrentState();

signals:
	//close按钮信号
	void Close();

protected:
	void resizeEvent(QResizeEvent *) override;
	virtual void RetranslateUi();

private:
	ShowState m_showState;
	QLabel *m_infoLabel;
	QLabel *m_valueLabel;
	QProgressBar *m_progressBar;
	QPushButton *m_closeBtn;
};

//显示可更新版本信息的窗口
#include <QPushButton>
#include <QScrollArea>
class VersionDisplay : public URBaseWidget
{
	Q_OBJECT

public:
	VersionDisplay(QWidget *parent = 0);
	//设置展示的内容
	void SetText(QString text);

signals:
	//Update Now 按钮触发信号
	void UpdateNow();
	//Remind Me Later 按钮触发信号
	void RemindMeLater();

protected:
	void resizeEvent(QResizeEvent *) override;
	virtual void RetranslateUi();

private:
	QScrollArea *m_scrollArea;
	QLabel *m_versionsLabel;
	QPushButton *m_updateNowBtn;
	QPushButton *m_remindMeLaterBtn;
};

//显示无下载更新，以及各种错误提示的窗口
class HintDisplay : public URBaseWidget
{
	Q_OBJECT

public:
	HintDisplay(QWidget *parent = 0);
	//设置提示信息内容
	void SetHint(QString hint);

protected:
	void resizeEvent(QResizeEvent *) override;
	virtual void RetranslateUi();

signals:
	//关闭窗口信号
	void Close();

private:
	QLabel *m_label;
	QPushButton *m_closeBtn;
};

class NetworkAccessManagerTimeout : public QThread
{
    // 解决偶发的QNetworkAccessManager->get卡死的问题
    // 开线程进行get操作，如超时直接TerminateThread线程
    Q_OBJECT
public:
    NetworkAccessManagerTimeout(QObject *parent = nullptr);
    ~NetworkAccessManagerTimeout();

    QByteArray get(QNetworkRequest request, int waittime = 5000);

    bool error();

protected:
    void run() override;

private:
    QNetworkRequest m_request;
    QNetworkReply *m_reply;
    QByteArray m_bypteArray;
    bool m_bError;
};
#endif // UPDATECOMMON_H
