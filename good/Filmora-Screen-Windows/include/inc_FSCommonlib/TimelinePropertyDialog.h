#ifndef TIMELINEPROPERTYDIALOG_H
#define TIMELINEPROPERTYDIALOG_H

#include "inc_FSCommonlib/FSCommonWidget.h"


class FSNLETimeline;
class FSButton;

class FSCOMMONLIB_EXPORT TimelinePropertyDialog : public FSBaseDialog
{
    Q_OBJECT

public:
    TimelinePropertyDialog(QWidget *parent);

    void DoModal(FSNLETimeline *timeline);

    static QString GetPropertyString(FSNLETimeline *timeline);

protected:
    void resizeEvent(QResizeEvent *event);
    bool eventFilter(QObject *watcher, QEvent *event);
    QTextEdit *m_te;
    FSButton *m_btn;
};

#endif // TIMELINEPROPERTYDIALOG_H
