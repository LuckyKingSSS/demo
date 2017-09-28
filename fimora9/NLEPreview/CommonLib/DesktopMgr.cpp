#include "stdafx.h"
#include "inc_CommonLib/DesktopMgr.h"

DesktopMgr::DesktopMgr(QObject *parent)
	: QObject(parent)
{
	m_pDesktop = qApp->desktop();
}

DesktopMgr::~DesktopMgr()
{

}

QWidget* DesktopMgr::MainScreen()
{
	return m_pDesktop->screen(m_pDesktop->primaryScreen());
}

QWidget* DesktopMgr::ScreenWithPoint(QPoint pt)
{
	auto cnt = m_pDesktop->screenCount();
	for (int i = 0; i < cnt; ++i)
	{
		auto scrn = m_pDesktop->screen(i);
		if (scrn->geometry().contains(pt))
			return scrn;
	}
	return MainScreen();
}

DesktopMgr* DesktopMgr::GetInstance()
{
	static DesktopMgr ins(0);
	return &ins;
}
