#include "stdafx.h"
#include "FSPlayerShortCutContrl.h"

FSPlayerShortCutContrl::FSPlayerShortCutContrl(QObject *parent)
    : QObject(parent)
    , m_bSpaceActived(false)
    , m_bSpacePressOn(false)
    , m_bAltActived(false)
    , m_bAltPressOn(false)
{
    SetShortCut();
    qApp->installEventFilter(this);
    
}

FSPlayerShortCutContrl::~FSPlayerShortCutContrl()
{
}

void FSPlayerShortCutContrl::EnableShortCut(ShortCutId id, bool bEnable)
{
    switch (id)
    {
    case FSPlayerShortCutContrl::Space_ShortCut:
        m_bEnableSpace = bEnable;
        break;
    case FSPlayerShortCutContrl::Stop_ShortCut:
        m_pScStop->setEnabled(bEnable);
        break;
    case FSPlayerShortCutContrl::Forward_ShortCut:
        m_pScForward->setEnabled(bEnable);
        break;
    case FSPlayerShortCutContrl::Bacward_ShortCut:
        m_pScBacward->setEnabled(bEnable);
        break;
    case FSPlayerShortCutContrl::FullScreen_ShortCut:
        m_pScFullScrn->setEnabled(bEnable);
        break;
    case FSPlayerShortCutContrl::Alt_ShortCut:
        m_pScAlt->setEnabled(bEnable);
        break;
    default:
        break;
    }
}

bool FSPlayerShortCutContrl::isPressingSpace()
{
    return m_bSpacePressOn;
}

void FSPlayerShortCutContrl::SetSpacePressing(bool bPressing)
{
    m_bSpacePressOn = bPressing;
}

void FSPlayerShortCutContrl::SetShortCut()
{
    m_pScStop = new QShortcut(QKeySequence(Qt::Key_Slash + Qt::ControlModifier), static_cast<QWidget *>(this->parent()));
    m_pScStop->setContext(Qt::ApplicationShortcut);
    connect(m_pScStop, &QShortcut::activated, [this]{
        emit ShortCutActivated(Stop_ShortCut);
    });

    m_pScForward = new QShortcut(QKeySequence(Qt::Key_Left), static_cast<QWidget *>(this->parent()));
    m_pScForward->setContext(Qt::ApplicationShortcut);
    connect(m_pScForward, &QShortcut::activated, [this]{
        emit ShortCutActivated(Forward_ShortCut);
    });

    m_pScBacward = new QShortcut(QKeySequence(Qt::Key_Right), static_cast<QWidget *>(this->parent()));
    m_pScBacward->setContext(Qt::ApplicationShortcut);
    connect(m_pScBacward, &QShortcut::activated, [this]{
        emit ShortCutActivated(Bacward_ShortCut);
    });

    m_pScFullScrn = new QShortcut(QKeySequence("Alt+Return"), static_cast<QWidget *>(this->parent()));
    m_pScFullScrn->setContext(Qt::ApplicationShortcut);
    connect(m_pScFullScrn, &QShortcut::activated, [this]{
        emit ShortCutActivated(FullScreen_ShortCut);
    });

    m_pScAlt = new QShortcut(QKeySequence(Qt::Key_Alt), static_cast<QWidget *>(this->parent()));
    m_pScAlt->setContext(Qt::ApplicationShortcut);
    m_pScAlt->setAutoRepeat(false);
    connect(m_pScAlt, &QShortcut::activated, [&]{
        m_bAltActived = true;
        m_bAltPressOn = true;
        QTimer::singleShot(200, this, [&]{
            m_bAltActived = false;
        });
    });

}

void FSPlayerShortCutContrl::slotLongSpaceShortCut()
{
    if (m_bSpaceActived)
    {
        m_bSpaceActived = false;
    }
}

bool FSPlayerShortCutContrl::eventFilter(QObject *obj, QEvent *e)
{
    if (e->type() == QEvent::KeyRelease)
    {
        if (static_cast<QKeyEvent *>(e)->key() == Qt::Key_Space)
        {
            if (m_bSpaceActived)
            {
                m_bSpaceActived = false;
                emit ShortCutActivated(Space_ShortCut);
            }   

            if (!static_cast<QKeyEvent *>(e)->isAutoRepeat())
            {
                m_bSpacePressOn = false;  
            } 
            emit LongPressedShortCut(Space_ShortCut, m_bSpacePressOn);
            
        }
        else if (static_cast<QKeyEvent *>(e)->key() == Qt::Key_Alt)
        {
            m_bAltActived = false;              
            if (m_bAltPressOn)
            {
                emit LongPressedShortCut(Alt_ShortCut, false);
                m_bAltPressOn = false;
            }            
        }
    }
    else if (e->type() == QEvent::KeyPress)
    {
        if (static_cast<QKeyEvent *>(e)->key() == Qt::Key_Alt)
        {
            if (!m_bAltActived)
            {
                m_bAltActived = true;
                m_bAltPressOn = false;
                QTimer::singleShot(200, this, [&]{
                    if (m_bAltActived)
                    {
                        m_bAltPressOn = true;
                        emit LongPressedShortCut(Alt_ShortCut, true);
                    }
                });
            }
        }
        else if (static_cast<QKeyEvent *>(e)->key() == Qt::Key_Space)
        {
            
            if (!static_cast<QKeyEvent *>(e)->isAutoRepeat() && m_bEnableSpace)
            {
                m_bSpacePressOn = true;
                m_bSpaceActived = true;
                QTimer::singleShot(200, this, &FSPlayerShortCutContrl::slotLongSpaceShortCut);
            }
        }
    }
   
    return false;
}
