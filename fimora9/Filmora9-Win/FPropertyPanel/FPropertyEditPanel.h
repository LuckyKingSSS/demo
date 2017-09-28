#ifndef FPROPERTYEDITPANEL_H
#define FPROPERTYEDITPANEL_H

#include <QtWidgets/QWidget>

#include "FPropertyPanel.h"

class FPropertyEditPanel : public FPropertyPanel
{
    Q_OBJECT

public:
    FPropertyEditPanel(QWidget *parent);
    ~FPropertyEditPanel();


    QObject* GetTimelineManager();

public:
    void GetTimelineManager(QObject *manager);

signals:
    // 通知外部更新数据（时间线，EditPanle等）
    void sigUpdate();
    // 通知属性面板更新数据
    void sigUpdatePropertyPanel();

private:
    QObject *m_timelineManager;
};

#endif // FPROPERTYEDITPANEL_H
