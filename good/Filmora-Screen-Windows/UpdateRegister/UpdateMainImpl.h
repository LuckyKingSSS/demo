#ifndef UPDATEMAINIMPL_H
#define UPDATEMAINIMPL_H

#include <QWidget>
#include <QStackedWidget>
#include "CommonWidgets.h"
class ProgressDisplay;
class VersionDisplay;
class HintDisplay;
class LiveUpdateMgr;
class TitleBar;

class UpdateMainImpl : public URBaseWidget
{
	Q_OBJECT

public:
	UpdateMainImpl(QWidget *parent = 0);
	~UpdateMainImpl();

	void LiveUpdate(QString productId, QString version, QString email);
	void LiveUpdate();
	bool IsNeedUpdate();
	void Install();

protected:
	void resizeEvent(QResizeEvent *) override;
	virtual void RetranslateUi();

signals:
	//关闭窗口信号
	void Close();
	//开始安装信号
	void BeginInstall();
	//取消安装信号
	void CancelInstall();
	//新版本安装程序完全启动信号
	void InstallStarted();
	//Remind Me Later信号
	void RemindMeLater();
	//窗口移动信号
	void Move(int x, int y);
	//resize信号
	void Resize(QSize);

private:
	TitleBar *m_titleBar;
	ProgressDisplay *m_progressDisplay;
	VersionDisplay *m_versionDisplay;
	HintDisplay *m_hintDisplay;
	LiveUpdateMgr *m_updateMgr;
	QStackedWidget *m_stackedWidget;
};

#endif // UPDATEMAINIMPL_H
