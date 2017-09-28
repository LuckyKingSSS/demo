#ifndef FPRESENTWNDMGR_H
#define FPRESENTWNDMGR_H

#include <QObject>
#include <QStringList>

class FPresentWndMgr : public QObject
{
	Q_OBJECT

public:
	FPresentWndMgr(QObject *parent);
	~FPresentWndMgr();
	QStringList GetTotalColors();
private:
	
};

#endif // FPRESENTWNDMGR_H
