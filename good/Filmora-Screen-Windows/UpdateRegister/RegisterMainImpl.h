#ifndef REGISTERMAINIMPL_H
#define REGISTERMAINIMPL_H

#include <QDialog>
#include <QSettings>

#include "RegHelper.h"
#include "CBSHelper.h"
#include "CBSUrlReader.h"
#include "CommonWidgets.h"

class QLabel;
class QLineEdit;
class QPushButton;
class CheckTimer;
class TitleBar;
class URMessageBox;

#include <QThread>
class CheckTimer : public QObject
{
	Q_OBJECT

public:
	CheckTimer();
	~CheckTimer();
	//开启定时检查
	void Check();
	//关闭检查
	void StopCheck();

signals:
	void TimeOut();

private:
	Q_INVOKABLE void _Check();
	Q_INVOKABLE void _StopCheck();

private:
	QThread *m_pThread;
	QTimer *m_pTimer;
};


class LineEditInvalidInputAlert;

class RegisterMainImpl : public URBaseWidget
{
	Q_OBJECT

public:
	//检查类型，CheckOnStart：在主进程启动时作检查；CheckOnRegist：在注册时作检查
	enum CheckType{ CheckOnStart, CheckOnRegist };

public:
	RegisterMainImpl(QWidget *parent = 0);
	~RegisterMainImpl();
	//启动模块后，立即加载注册DLL
	bool LoadRegisterDLL();
	//启动模块后，自动检查是否注册过
	bool CheckRegister();
	//启动时，开启注册检查
	void RegisterCheckOnStart();
	//取消注册接口
	void DoUnregistered();
	void RegCheckFunc(CBSRegCheckResult checkResult, wchar_t* pszMessage);
	//写入注册成功事件信息到注册表中
	void WriteRegDate();
	// 读取当前语言设置
	QString GetLocalLanguage();
	// 获取产品id，此id与当前语言设置有关
	int GetProductID();
	// 获取当前版本号
	QString GetCurVersion();

Q_SIGNALS:  //for wb
	//启动注册信号
	void singalStartRegister(const QDateTime time);
	void signalCheckOnStart(bool bResult);
	void signalCheckOnRegist(bool bResult, QDateTime time);

protected:
	void resizeEvent(QResizeEvent*) override;
	void showEvent(QShowEvent*) override;
	virtual void RetranslateUi();
	bool eventFilter(QObject *, QEvent *) override;

signals:
	//立刻购买信号
	void BuyNow();
	//free trial信号
	void FreeTrial();
	//关闭窗口信号
	void Close();
	//退款信号
	void ReFund();
	//移动窗口信号
	void Move(int x, int y);
	//注册成功信息
	void singnalSucessReg(QDateTime time);

private:
	void RegisterFunc();
	void BuyNowFunc();
	void TrialFunc();
	void CloseFunc();
    bool IsHex(QString text);
	// char* to wchar_t*
	wchar_t *AToW(char* src);
	// wchar_t * to char *
	char *WToA(wchar_t *src);
	// 设置按钮状态
	void SetButtonsEnable(bool enabled);
	wchar_t* _GetLocalLanguage();

private:
	bool m_bReturn;
	bool m_bDone;
	QSettings m_settings;
	CheckType m_checkType;
	CheckTimer *m_pCheckTimer;
	QString m_prodId;
	QString m_curVersion;
	HMODULE m_registerDLL;
	IRegHelper *m_regHelper;
	ICBSHelper *m_cbsHelper;
	CBSUrlReader *m_cbsUrlReader;
	QLabel *m_captionLabel;
	QLabel *m_emailLabel;
	QLabel *m_regCodeLabel;
    LineEditInvalidInputAlert *m_email;
    LineEditInvalidInputAlert *m_regCode;
    QPushButton *m_registerBtn;
	QPushButton *m_buyNowBtn;
	QPushButton *m_trialBtn;
	QPushButton *m_closeBtn;
	TitleBar *m_titleBar;
	URMessageBox *m_messageBox;
};

class LineEditInvalidInputAlert : public QLineEdit
{
    Q_OBJECT
public:
    LineEditInvalidInputAlert(QWidget *parent);
    
    void Invalid(bool invalid);
protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
    bool eventFilter(QObject *watcher, QEvent *event);
private:
    bool m_invalid;
    QWidget *m_alertWidget;
};


#endif // REGISTERMAINIMPL_H
