#pragma once

#include "inc_CommonLib/commonlib_global.h"

#include <QtWidgets/QWidget>



class COMMONLIB_EXPORT NLEBaseWidget : public QWidget
{
	Q_OBJECT
public:
	typedef QList<NLEBaseWidget*> NLEBaseWidgetList;
public:
	NLEBaseWidget(QWidget *parent);

	virtual void SetFocus(bool foucs);
	virtual void MouseMove(QPoint p);


	static NLEBaseWidgetList GetNLEBaseWidgets();
	
};

class QShortcut;
class COMMONLIB_EXPORT NLEGlobalShortcut : public QObject
{
	Q_OBJECT
public:
	enum 
	{
		Key_Space,
		Key_Delete,
		Key_Copy,
		Key_Cut,
		Key_Pasete,

		Key_User = 0x1000
	};
public:
	// 父亲默认为gGetMainWindow()
	static NLEGlobalShortcut* GetInstance();

	// 用于在QDilaog窗口中,需要一个新的shortcut父窗口
	static NLEGlobalShortcut* CreateInstance(QWidget *parent);
	void AddShortcut(QKeySequence key, int id);
	void SetShortcutEnabled(int id, bool enable);
	void EnableShortcuts(bool bEnable);

	NLEGlobalShortcut(QWidget *parent);

Q_SIGNALS:
	void actived(int id);

private:
	QList<QShortcut*> m_shortcuts;
};
