#ifndef DESKTOPRECTANGLEMGR_H
#define DESKTOPRECTANGLEMGR_H

#include "inc_FSCommonlib/fscommonlib_global.h"
#include <QtCore/QObject>
#include <QtCore/QRect>

// 管理录制时主屏和副屏的位置，一般主屏为录制区，副屏为面板放置区
// 此处主屏为逻辑上定义的主屏,主屏外其余都会副屏
class FSCOMMONLIB_EXPORT DesktopRectangleMgr : public QObject
{
    Q_OBJECT
public:
    static DesktopRectangleMgr* GetInstance();

    //  主屏
    QRect PrimaryScreen();
    // 副屏,返回index最小的副屏
    QRect SecondScreen();
    // 将主屏到第index副屏
    void SwitchPrimary2Screen(int index);
    bool IsPrimaryScreen();

    // 主屏index
    int PrimaryScreenIndex();
    int SecondScreenIndex();

    QRect ScreenGeometry(int index);
    QRect ScreenAvailableGeometry(QPoint point);
    QRect ScreenAvailableGeometry(int index);
    int ScreenCount();

    //  上一次主屏
    QRect PrePrimaryScreen();
    // 上一次副屏,返回index最小的副屏
    QRect PreSecondScreen();

signals:
    void sigPrimaryScreenChanged();

protected:
    DesktopRectangleMgr(QObject *parent);

    void CalSecondScreenIndex();

private:
    QRect m_offsetRect;
    int m_primaryIndex;
    int m_secondIndex;
    int m_prePrimaryIndex;
    int m_preSecondIndex;
    int m_screenType;
};

#endif // DESKTOPRECTANGLEMGR_H
