#ifndef PRIMARYSCREENMANAGER_H
#define PRIMARYSCREENMANAGER_H

#include <QtCore/QObject>
#include "inc_FSCommonlib/DesktopRectangleMgr.h"

class DesktopRectangleMgr;
class RecorderViewsMgr;
class RecordHelper;
class PrimaryScreenManager : public DesktopRectangleMgr
{
    Q_OBJECT

public:
    static PrimaryScreenManager* GetInstance();

    void SwitchRecordScreen();
   
    int PrimaryScreenWidth();
    int PrimaryScreenHeight();

    int SecondScreenWidth();
    int SecondScreenHeight();


    bool PanelInRecordingRectangle(QWidget *panel);

    QRect DefaultRecordRect();
    void SetDefaultRecordPanelPosition();

protected:
    PrimaryScreenManager(QObject *parent);
    bool eventFilter(QObject *watcher, QEvent *event);

    void PrimaryScreenChanged();

private:
    RecorderViewsMgr *m_recorderViewsMgr;
    RecordHelper *m_pHelper;
};

#endif // PRIMARYSCREENMANAGER_H
