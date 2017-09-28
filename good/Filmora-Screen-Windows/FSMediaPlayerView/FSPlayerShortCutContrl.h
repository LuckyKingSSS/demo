#ifndef FSPLAYERSHORTCUTCONTRL_H
#define FSPLAYERSHORTCUTCONTRL_H

#include <QObject>
class QShortcut;

class FSPlayerShortCutContrl : public QObject
{
    Q_OBJECT

public:
    enum ShortCutId{
        Space_ShortCut,
        Stop_ShortCut,
        Forward_ShortCut,
        Bacward_ShortCut,
        FullScreen_ShortCut,
        Alt_ShortCut
    };

public:
    FSPlayerShortCutContrl(QObject *parent);
    ~FSPlayerShortCutContrl();

    void EnableShortCut(ShortCutId id, bool bEnable);
    bool isPressingSpace();
    void SetSpacePressing(bool bPressing);

private:
    void SetShortCut();
    void slotLongSpaceShortCut();

    bool eventFilter(QObject *obj, QEvent *e);

signals:
    void ShortCutActivated(ShortCutId id);
    void LongPressedShortCut(ShortCutId id,bool bPress);

private:
    QShortcut              *m_pScSpace;
    QShortcut              *m_pScStop;
    QShortcut              *m_pScForward;
    QShortcut              *m_pScBacward;
    QShortcut              *m_pScFullScrn;
    QShortcut              *m_pScAlt;

    bool                    m_bEnableSpace;
    bool                    m_bSpacePressOn;
    bool                    m_bSpaceActived;

    bool                    m_bAltPressOn;
    bool                    m_bAltActived;
    
};

#endif // FSPLAYERSHORTCUTCONTRL_H
