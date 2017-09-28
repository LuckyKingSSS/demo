#ifndef UPDATEHELPER_H
#define UPDATEHELPER_H

#include <QObject>
class UpdateMain;
class RegisterMain;
class CBSUrlReader;
class UpdateRegisterPurchaseHelper : public QObject
{
	Q_OBJECT
public:
	static UpdateRegisterPurchaseHelper* GetInstance();
	void DoAutoUpdate();
	void Register();
	void Purchase();
	//启动的时候进行是否注册的检查
	void RegisterCheckOnStart();

	bool IsRegistered();
	void RebuildRegisterMain(bool bTellEditor = false);
private:
	UpdateRegisterPurchaseHelper(QObject *parent = 0);
	void OnBeginInstall();
	void OnInstallStarted();
	void OnAutoQuery();
	QString GetCurrentLang();

Q_SIGNALS:
	//成功注册的信号
	void singnalSucessRegister(bool bResult);
	void singnalStartRegister(const QDateTime time);
	//启动时判断注册
	void signalCheckOnStart(bool bResult);
	void sigRegisterResult(bool bResult);
private:
	UpdateMain *m_pUpdateMain;
	RegisterMain* m_pRegisterMain;
	QTimer m_timer;
	QSettings *m_pSetting;
	CBSUrlReader *m_pCBSUrlReader;
};

#endif // UPDATEHELPER_H
