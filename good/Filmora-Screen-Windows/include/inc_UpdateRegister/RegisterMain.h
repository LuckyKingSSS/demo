#ifndef REGISTERMAIN_H
#define REGISTERMAIN_H

#include "inc_UpdateRegister/UpdateRegister_global.h"

#include <QDialog>

class RegisterMainImpl;

//注册模块
class UPDATEREGISTER_EXPORT RegisterMain : public QDialog
{
	Q_OBJECT

public:
	RegisterMain(QWidget *parent = 0);
	~RegisterMain();

	//注册接口
	void DoRegister();
	//启动时注册检查
	void RegisterCheckOnStart();
	//取消注册
	void DoUnregistered();
	//检查是否注册
	bool CheckRegister();
	
signals:
	//立刻购买信号
	void BuyNow();
	//free trial信号
	void FreeTrial();
	//检测到用户是退款用户时，发送此信号
	void ReFund();
	//注册是否成功信号
	void RegisterResult(bool bSuccessfull);  // 关闭窗口，该信号会发出
	//启动注册信号
	void singalStartRegister(const QDateTime time);
	void signalCheckOnStart(bool bResult);  // for wb
	void signalCheckOnRegist(bool bResult,  QDateTime time);  // for wb

private:
	RegisterMainImpl *m_pImpl;
};


class UPDATEREGISTER_EXPORT RegisterInit
{
public:
	static void SetBuyNowUrl(QString url);
	static QString GetBuyNowUrl();
};

#endif // REGISTERMAIN_H
