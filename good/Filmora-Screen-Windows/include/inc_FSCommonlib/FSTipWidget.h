#ifndef FSTIPWIDGET_H
#define FSTIPWIDGET_H

#include "inc_FSCommonlib/fscommonlib_global.h"


#include <QtWidgets/QtWidgets>

class QTimer;

class FSCOMMONLIB_EXPORT FSTipWidget :public QLabel
{
    Q_OBJECT
public:
    static FSTipWidget &Instance();

    void SetMesseage(const QString &msg, const QPoint* pPoint = NULL);
private:
    FSTipWidget();
    ~FSTipWidget();

    void OnTimer();

    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);

    bool eventFilter(QObject *, QEvent *);

private:
    QWidget *mpParent;
    QTimer  *mpTimer;
    bool     mbEnter;
    float    mnTransparent;
    int      mCountNum;
};


#endif // FSTIPWIDGET_H
