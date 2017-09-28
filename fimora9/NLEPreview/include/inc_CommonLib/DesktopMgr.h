#ifndef DESKTOPMGR_H
#define DESKTOPMGR_H

#include <QObject>
#include "inc_CommonLib/commonlib_global.h"
class QDesktopWidget;
class COMMONLIB_EXPORT DesktopMgr : public QObject
{
	Q_OBJECT

public:
	static DesktopMgr* GetInstance();
	~DesktopMgr();

	QWidget* MainScreen();
	QWidget* ScreenWithPoint(QPoint pt);
private:
	DesktopMgr(QObject *parent);
	QDesktopWidget *m_pDesktop;
};

#endif // DESKTOPMGR_H
