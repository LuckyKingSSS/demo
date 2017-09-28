/*
*Author:		zhy
*Date:			2016.09.30
*Description:	
*/

#ifndef TOPLEVELWINDOWTITLEBAR_P
#define TOPLEVELWINDOWTITLEBAR_P


#include <QtWidgets/QtWidgets>
#include <QtCore/QMap>


#include "inc_CommonLib/TopLevelWindowTitleBar.h"

class MagicLabel;
class BaseButton;
class TopLevelWindowTitleBarResizeMoveControl_p;
class SystemMenuControl;
class TopLevelWindowTitleBar_p : public QWidget
{
	Q_OBJECT
public:
	TopLevelWindowTitleBar_p(QWidget *parent);

	void SetControlWidget(QWidget *titleBar, QWidget *toplevelWindow);
	void SetTitleName(QString name);
	QString GetTitleName();
	void SetTitleNameBold(bool enable);
	void SetTitleNameSize(int size);
	void SetTitleNameAlign(Qt::Alignment flag);
	QLabel* GetTitleLabel();

	void InitWindowMaximumButtonState(bool maximum);
	void SetTitleBarColor(QColor color);
	void SetButtons(int button);
	void SetButtonWidth(int width);
	void SetMininMaximButton(int id);

	void SetAutoCloseWindow(bool enable);
	void SetTitleIcon(QPixmap pixmap);

	void AddSuffixButton(QWidget *btn);
	void AddSuffixSpace(int space);

	void SetSystemMenuEnabled(bool enabled);

	BaseButton* GetButton(int id);
	TopLevelWindowTitleBarResizeMoveControl_p* GetConrtol();

	bool PopupMenu(QPoint point);

protected:
	void resizeEvent(QResizeEvent *event);
	void mouseDoubleClickEvent(QMouseEvent *event);
	bool eventFilter(QObject *watched, QEvent *event);
	//void paintEvent(QPaintEvent *event);

private:
	void CreateButton(int id, QString objectName);
	void CreateStateButton(int firstId, QString firstObjectName, int secondId, QString secondObjectName);
	QList<BaseButton*> GetButtons();
	void ButtonClicked(int id);
	
private:
	MagicLabel *m_title;
	QLabel *m_icon;
	QMap<int, BaseButton*> m_buttons;
	QList<QWidget*> m_extraWidgetsRight;
	QList<QWidget*> m_extraWidgetsLeft;

	QColor m_titleBarColor;
	int m_buttonEnum;

	bool m_autoCloseWindow;
	int m_btnWidth;
	TopLevelWindowTitleBarResizeMoveControl_p *m_control;
	SystemMenuControl *m_systemMenuControl;
};


class DashlineWidget;
class TopLevelWindowTitleBarResizeMoveControl_p : public TopLevelWindowTitleBarResizeMoveControl
{
	Q_OBJECT
public:
	TopLevelWindowTitleBarResizeMoveControl_p();

	void SetControlWidget(QWidget *titleBar, QWidget *toplevelWindow) override;
	QWidget* GetTitleBar();
	QWidget* GetTopLevelWindow();

	void SetResizeable(bool enable) override;
    void SetMoveable(bool enable) override;
    void SetDragMaximum(bool enable) override;
	void SetDashlineColor(QColor color) override;
	void SetBorderColor(QColor color) override;
	void SetBorderWidth(int width) override;
    QColor GetBorderColor() override;
    int GetBorderWidth() override;
	void SetSystemBorderVisible(bool visible) override;
	bool GetSystemBorderVisible() override;

	bool IsMaximized() override;
	void ShowMaximized() override;
	void ShowNormal() override;
	void ShowMinimized() override;
	void CloseWindow() override;



protected:
	bool eventFilter(QObject *watched, QEvent *event);

	void MouseMove(QPoint p);
	bool CanResize();
	bool Resizeing();
	void SetResizeing(bool resize);
	TopLevelWindowTitleBar_p* GetDefaultTitleBar();

	
private:
	QWidget *m_titleBar;
	QWidget *m_toplevelWindow;
	bool m_toplevelWindowCanResize;
	bool m_toplevelDragMaximum;
	bool m_toplevelWindowFirstShow;

	bool m_mousePress;
	QPoint m_curPoint;

	bool m_left;
	bool m_top;
	bool m_right;
	bool m_bottom;

	bool m_resizeing;
	bool m_hint;
	DashlineWidget *m_dashedWidget;


	///
	bool m_titleMousePress;
	bool m_maximizedDragResize;
	bool m_maximizedDragResize_fullscreen;
	QPoint m_titleousePressPoint;

	QRect m_normalGeometry;
	QRect m_maxGeometry;

	QColor m_borderColor;
	int m_borderWidth;
	bool m_isMaximized;
	bool m_systemBorder;
    bool m_toplevelWindowCanMove;

private:
	friend class TopLevelWindowTitleBar_p;
};

class DashlineWidget :public QWidget
{
	Q_OBJECT
public:
	DashlineWidget(QWidget *parent);

	void SetColor(QColor color);

	void MovePosition(QRect r);
	void HideDashline();
	void ShowDashLine();

protected:
	void paintEvent(QPaintEvent *event);

	QColor m_color;
};



class SystemMenuControl : public QObject, public QAbstractNativeEventFilter
{
	Q_OBJECT
public:
	enum{ RESTORE = WM_USER + 1000, MINIMIZE, MAXIMIZE};
public:
	SystemMenuControl(TopLevelWindowTitleBar_p *parent);
	~SystemMenuControl();

	bool PopupMenu( QPoint pos);

	void UpdateMenuState();

	void SetEnabled(bool enabled);

Q_SIGNALS:
	void triggered(int id);

protected:
	bool	nativeEventFilter(const QByteArray & eventType, void * message, long * result) override;

	void EnabledMenu(int id, bool enabled);
private:
	TopLevelWindowTitleBar_p *m_topLevelWindowTitleBar_p;
	QWidget *m_topLevelWindow;
	bool m_enabled;
	DWORD m_popupTick;
};





#endif // TIMELINEVIEWRULER_H
