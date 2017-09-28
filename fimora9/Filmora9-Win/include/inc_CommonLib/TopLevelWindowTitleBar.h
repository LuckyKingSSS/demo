#pragma once
/*
*Author:		zhy
*Date:			2016.09.30
*Description:	顶层窗口标题栏，默认自带最小，最大，关闭按钮，支持拖拽，双击全屏功能
				创建时传递顶层窗口指针，支持顶层窗口拖拽大小。
				拓展：继承TopLevelWindowTitleBar，拓展标题栏其它功能
				注意：使用此标题，顶层窗口需要先设置大小，如：resize()/setFixedSize等
*/
#include "inc_CommonLib/commonlib_global.h"

#include <QtWidgets/QWidget>
#include <QtCore/QString>
#include <QtWidgets/QLabel>

class BaseButton;
class TopLevelWindowTitleBar_p;
class TopLevelWindowTitleBarResizeMoveControl;

class COMMONLIB_EXPORT TopLevelWindowTitleBar : public QWidget
{
	Q_OBJECT
public:
	enum Button{
		None = 0x1,
		Minimized = 0x2,
		Normal = 0x4,
		Maximized = 0x8,
		Close = 0x10,
	};
public:
	TopLevelWindowTitleBar(QWidget *parent);
	// 初始化标题栏最大/最小化按钮，首次启动顶层窗口最大化参数为true,反之为false
	void InitWindowMaximumButtonState(bool maximum);
	// 设置标题栏字体属性名称
	void SetTitleName(QString name);
	QString GetTitleName();
	void SetTitleNameBold(bool enable);
	void SetTitleNameSize(int size);
	void SetTitleNameAlign(Qt::Alignment flag);
	QLabel* GetTitleLabel();

	// 顶层窗口是否能拖拽大小
	void SetResizeble(bool enable);
	// 顶层窗口拖拽到屏幕最上方是否进行最大化
	void SetDragMaximum(bool enable);

	void SetTitleBarColor(QColor color);
	// 定制显示的按钮
	void SetButtons(int button);
	
	// 设置按钮的宽
	void SetButtonWidth(int width);

	// 设置拖拽虚线颜色
	void SetDashlineColor(QColor color);
	// 点击关闭按钮关闭窗口
	void SetAutoCloseWindow(bool enable);
	// 设置图标
	void SetTitleIcon(QPixmap pixmap);

	bool IsMaximized();
	void ShowMaximized();
	void ShowNormal();
	void ShowMinimized();
	void CloseWindow();

	// 右边增加按钮
	void AddSuffixButton(QWidget *btn);
	void AddSuffixSpace(int space);

	// 使用系统边框,false:使用自绘边框
	void SetSystemBorderVisible(bool visible);
	bool GetSystemBorderVisible();
	void SetSystemMenuEnabled(bool enabled);

	BaseButton* GetButton(int id);

	TopLevelWindowTitleBarResizeMoveControl* GetControl();
Q_SIGNALS:
	// 按钮信号
	void ButtonClick(Button id);
    void StartDashedMove(QPoint);
    void DashedMove(QPoint);
    void EndDashedMove(QPoint);
private:
	TopLevelWindowTitleBar_p *m_p;
};


/*
	标题栏控制器，创建后使用SetControlWidget传入标题栏和顶层窗口指针，
	可获得移动窗口，拖拽窗口大小功能
*/
class COMMONLIB_EXPORT TopLevelWindowTitleBarResizeMoveControl : public QObject
{
	Q_OBJECT
public:
	static TopLevelWindowTitleBarResizeMoveControl* CreateControl();

	// 设置要控制的标题栏和顶层窗口
	virtual void SetControlWidget(QWidget *titleBar, QWidget *toplevelWindow) = 0;
	// 是否可拖拽大小
	virtual void SetResizeable(bool enable) = 0;
    virtual void SetMoveable(bool enable) = 0;
    // 拖拽到屏幕最上方是否自动全屏
	virtual void SetDragMaximum(bool enable) = 0;
	// 设置拖拽虚线颜色
	virtual void SetDashlineColor(QColor color) = 0;
	virtual void SetBorderColor(QColor color) = 0;
    virtual QColor GetBorderColor() = 0;
	virtual void SetBorderWidth(int width) = 0;
    virtual int GetBorderWidth() = 0;
	virtual void SetSystemBorderVisible(bool visible) = 0;
	virtual bool GetSystemBorderVisible() = 0;

	virtual bool IsMaximized() = 0;
	virtual void ShowMaximized() = 0;
	virtual void ShowNormal() = 0;
	virtual void ShowMinimized() = 0;
	virtual void CloseWindow() = 0;

signals:
    void StartDashedMove(QPoint);
    void DashedMove(QPoint);
    void EndDashedMove(QPoint);
};

