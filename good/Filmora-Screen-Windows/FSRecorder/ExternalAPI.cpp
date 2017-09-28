#include "stdafx.h"
#include "ExternalAPI.h"


#ifdef WIN32
#include <windows.h>
#include <tlhelp32.h>
#endif


static QWidget *s_RecordPanel = nullptr;
void gFSSetRecordPanel(QWidget *widget)
{
	s_RecordPanel = widget;
}

QWidget *gFSGetRecordPanel()
{
	return s_RecordPanel;
}


static QWidget *g_pMainWindow = nullptr;
void gFSSetRecordMainWindow(QWidget *widget)
{
	g_pMainWindow = widget;
}

QWidget* gFSRecordMainWindow()
{
	return g_pMainWindow;
}


//////////////////////////////////////////////////////////////////////////


VolumeBar::VolumeBar(QWidget *parent)
:QPushButton(parent)
, m_Value(0)
, m_Min(0)
, m_Max(100)
{
    m_pTimer = new QElapsedTimer();
    m_pValueWidget = new QWidget(this);
    m_pValueWidget->setObjectName("m_pValueWidget");
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowSystemMenuHint | Qt::FramelessWindowHint);
    setObjectName("VoluemBar");
    SetFixSize({ 20, 7 });
    m_pTimer->start();
}

VolumeBar::~VolumeBar()
{
    delete m_pTimer;
}

void VolumeBar::SetFixSize(QSize size)
{
    resize(size);
    m_pValueWidget->resize(m_pValueWidget->width(), size.height());
}

void VolumeBar::SetValue(int value)
{

    if (m_pTimer->hasExpired(40))
    {

        m_Value = 0.92*m_Value + (1 - 0.92)*value;//µÍÍ¨ÂË²¨
        m_pValueWidget->setFixedWidth(width()*qMin(1.0*m_Value / (m_Max - m_Min - 45), 1.0));
        repaint();
        m_pTimer->restart();
    }

}

void VolumeBar::SetRange(int max, int min)
{
    m_Min = qMin(max, min);
    m_Max = qMax(max, min);
}

void VolumeBar::resizeEvent(QResizeEvent *)
{
    m_pValueWidget->setFixedWidth(width()*m_Value / (m_Max - m_Min));
}

//////////////////////////////////////////////////////////////////////////
