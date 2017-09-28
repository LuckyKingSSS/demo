#include "stdafx.h"
#include "inc_FSCommonlib/DesktopRectangleMgr.h"

DesktopRectangleMgr* DesktopRectangleMgr::GetInstance()
{
    static DesktopRectangleMgr instance(0);
    return &instance;
}
DesktopRectangleMgr::DesktopRectangleMgr(QObject *parent)
: QObject(parent)
{
    m_screenType = 1;
    m_offsetRect = QRect(0, 0, 0, 0);
    m_primaryIndex = QApplication::desktop()->primaryScreen();
    m_secondIndex = 0;
    m_prePrimaryIndex = m_primaryIndex;
    m_preSecondIndex = m_prePrimaryIndex;

    //SwitchPrimary2Screen(QApplication::desktop()->primaryScreen());
    CalSecondScreenIndex();

}


void DesktopRectangleMgr::CalSecondScreenIndex()
{
    m_secondIndex = 0;
    while (m_secondIndex == PrimaryScreenIndex())
    {
        ++m_secondIndex;
    }
    m_secondIndex = qMin(m_secondIndex, ScreenCount() - 1);
}


QRect DesktopRectangleMgr::PrimaryScreen()
{
    return QApplication::desktop()->screenGeometry(m_primaryIndex);
}

QRect DesktopRectangleMgr::SecondScreen()
{
    return QApplication::desktop()->screenGeometry(m_secondIndex);

}

void DesktopRectangleMgr::SwitchPrimary2Screen(int index)
{
    Q_ASSERT(index <  ScreenCount());
    //if (index != m_primaryIndex)
    {
        m_prePrimaryIndex = m_primaryIndex;
        m_preSecondIndex = m_secondIndex;
        m_screenType = !m_screenType;
        m_primaryIndex = index;
        CalSecondScreenIndex();
        Q_ASSERT(m_primaryIndex < ScreenCount());
        Q_ASSERT(m_secondIndex < ScreenCount());
        emit sigPrimaryScreenChanged();
    }
}

bool DesktopRectangleMgr::IsPrimaryScreen()
{
    return m_screenType;
}

int DesktopRectangleMgr::PrimaryScreenIndex()
{
    return m_primaryIndex;
}

int DesktopRectangleMgr::SecondScreenIndex()
{
    return m_secondIndex;
}

QRect DesktopRectangleMgr::ScreenGeometry(int index)
{
    return QApplication::desktop()->screenGeometry(index);
}

QRect DesktopRectangleMgr::ScreenAvailableGeometry(QPoint point)
{
    return QApplication::desktop()->availableGeometry(point);

}

QRect DesktopRectangleMgr::ScreenAvailableGeometry(int index)
{
    return QApplication::desktop()->availableGeometry(index);

}

int DesktopRectangleMgr::ScreenCount()
{
    return QApplication::desktop()->screenCount();
}

QRect DesktopRectangleMgr::PrePrimaryScreen()
{
    return QApplication::desktop()->screenGeometry(m_prePrimaryIndex);

}

QRect DesktopRectangleMgr::PreSecondScreen()
{
    return QApplication::desktop()->screenGeometry(m_preSecondIndex);

}

