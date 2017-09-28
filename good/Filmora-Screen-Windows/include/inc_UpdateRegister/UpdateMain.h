#ifndef UPDATEMAIN_H
#define UPDATEMAIN_H

#include "inc_UpdateRegister/UpdateRegister_global.h"

#include <QWidget>

class UpdateMainImpl;

//在线升级模块
class UPDATEREGISTER_EXPORT UpdateMain : public QDialog
{
	Q_OBJECT

public:
	UpdateMain(QWidget *parent = 0);
	/**
	 *@brief     在线升级，创建窗口后，即调用此方法
	 *@param     productId 当前产品id
	 *@param     version   当前产品版本号
	 *@param     email     用户邮箱
	 */
	void LiveUpdate(QString productId, QString version, QString email);

	//在线升级，productId、version、email等参数，默认从配置文件中自行读入
	void LiveUpdate();

	//判断是否有可更新版本。若是网络问题，同样返回false
	bool IsNeedUpdate();

	//安装新版本，需要主程序主动调用，才会开始安装新版本
	void Install();

signals:
	//开始安装信号
	void BeginInstall();
	//取消安装信号
	void CancelInstall();
	//新版本安装程序完全启动信号。逻辑上，主程序接收到此信号后，就可以退出了。
	void InstallStarted();
	//关闭窗口信号
	void Close();
	//Remind Me Later信号
	void RemindMeLater();

private:
	UpdateMainImpl *m_pImpl;
};

class UPDATEREGISTER_EXPORT UpdateInit
{
public:
	static void SetUpdateUrl(QString url);
	static QString GetUpdateUrl();
};

#endif // UPDATEMAIN_H
