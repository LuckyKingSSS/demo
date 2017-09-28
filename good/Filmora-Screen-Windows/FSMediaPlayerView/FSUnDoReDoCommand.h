#ifndef FSUNDOREDOCOMMEND_H
#define FSUNDOREDOCOMMEND_H

#include <QObject>
#include <QtWidgets/QUndoCommand>
#include "Include/NLECaptionDefine.h" 
#include "inc_FSTimelineView/UndoCommandBase.h"

class FSUnDoReDoCommand : public UndoCommand
{
    Q_OBJECT

Q_SIGNALS :
    void signalUpdate();

public:
    FSUnDoReDoCommand(QObject *parent = nullptr);
    ~FSUnDoReDoCommand();

protected:
    virtual void Undo();
    virtual void Redo();

    
};


class IFSEditHelper;
class FSVisaulUndoCommand :public FSUnDoReDoCommand{
    Q_OBJECT 
public:
    FSVisaulUndoCommand(IFSEditHelper *pEditHelper, QPair<double, double> newScale, QPair<double, double> newPos, double newAngle, bool bNewObj);
    ~FSVisaulUndoCommand();

protected:
    virtual void Undo();
    virtual void Redo();


protected:
    IFSEditHelper *m_pEditHelper;
    QPair<double, double> m_NewScale;
    QPair<double, double> m_OldScale;
    QPair<double, double> m_NewPos;
    QPair<double, double> m_OldPos;
    double m_NewAngle;
    double m_OldAngle;
};


class FSNLETimeline;
class FSAudioUnDoCommand :public FSUnDoReDoCommand{
    Q_OBJECT
public:
    FSAudioUnDoCommand(IFSEditHelper *pEditHelper, int newVoluem, bool isMix2MononChecked, bool bNewObj);
    ~FSAudioUnDoCommand();

protected:
    virtual void Undo();
    virtual void Redo();

private:
    virtual void redo();
    virtual void undo();


private:
    IFSEditHelper *m_pEditHelper;
    FSNLETimeline *m_pTimeline;
    double m_NewVoluem;
    double m_OldVoluem;

    bool m_NewMix2MononChecked;
    bool m_OldMix2MononChecked;
};







class FSCursorVisualUnDoCommand :public FSUnDoReDoCommand{
    Q_OBJECT
public:
    FSCursorVisualUnDoCommand(IFSEditHelper *pEditHelper, double scale, double Opacity);
    ~FSCursorVisualUnDoCommand();

protected:
    virtual void Undo();
    virtual void Redo();



private:
    IFSEditHelper *m_pEditHelper;
    double m_OldScale;
    double m_NewScale;

    double m_OldOpacity;
    double m_NewOpacity;
};


class FSMouseHighlightUnDoCommand :public FSUnDoReDoCommand{
    Q_OBJECT
public:
    FSMouseHighlightUnDoCommand(IFSEditHelper *pEditHelper, QColor color);
    ~FSMouseHighlightUnDoCommand();

protected:
    virtual void Undo();
    virtual void Redo();



protected:
    IFSEditHelper *m_pEditHelper;
    QColor m_NewColor;
    QColor m_OldColor;
};

class FSMouseRingsUnDoCommand :public FSMouseHighlightUnDoCommand{
    Q_OBJECT
public:
    FSMouseRingsUnDoCommand(IFSEditHelper *pEditHelper, QColor color);
    ~FSMouseRingsUnDoCommand();

protected:
    virtual void Undo();
    virtual void Redo();



};

class FSMouseClickSoundUnDoCommand :public FSUnDoReDoCommand{
    Q_OBJECT
public:
    FSMouseClickSoundUnDoCommand(IFSEditHelper *pEditHelper, int voluem);
    ~FSMouseClickSoundUnDoCommand();

protected:
    virtual void Undo();
    virtual void Redo();

private:
    virtual void redo();
    virtual void undo();

private:
    IFSEditHelper *m_pEditHelper;
    int m_OldVoluem;
    int m_NewVoluem;

};

class FSMouseClickSoundDeleteConmmand:public FSUnDoReDoCommand{
    Q_OBJECT
public:
    FSMouseClickSoundDeleteConmmand(IFSEditHelper *pEditHelper);
    ~FSMouseClickSoundDeleteConmmand();

protected:
    virtual void Undo();
    virtual void Redo();


private:
    IFSEditHelper *m_pEditHelper;
    QString        m_SoundFilePath;

};


class INLETimelineBase;
class FSMouseHighLightDeleteCommand :public FSUnDoReDoCommand{
    Q_OBJECT
public:
    FSMouseHighLightDeleteCommand(IFSEditHelper *pEditHelper);
    ~FSMouseHighLightDeleteCommand();
protected:
    virtual void Undo();
    virtual void Redo();



private:
    IFSEditHelper *m_pEditHelper;

    INLETimelineBase *m_pTimelineBase;
};

class FSMouseRingsDeleteCommand :public FSUnDoReDoCommand{
    Q_OBJECT
public:
    FSMouseRingsDeleteCommand(IFSEditHelper *pEditHelper);
    ~FSMouseRingsDeleteCommand();

protected:
    virtual void Undo();
    virtual void Redo();


private:
    IFSEditHelper *m_pEditHelper;
    INLETimelineBase *m_pTimelineBase;
};

class FSCaptionChangeCommand:public FSUnDoReDoCommand{
    Q_OBJECT
public:
    FSCaptionChangeCommand(IFSEditHelper *pHelper);
    ~FSCaptionChangeCommand();
protected:
    virtual void Undo();
    virtual void Redo();


private:
    IFSEditHelper *m_pEditHelper;
    bool           m_bFirst;
};

class FSMaskPathChangeCommand :public FSUnDoReDoCommand{
    Q_OBJECT
public:
    FSMaskPathChangeCommand(IFSEditHelper *pHelper,const QString &MaskPathStr);
    ~FSMaskPathChangeCommand();

protected:
    virtual void Undo();
    virtual void Redo();

private:
    IFSEditHelper *m_pEditHelper;
    QString        m_NewMaskPathStr;
    QString        m_OldMaskPathStr;
};

class FSDenoiseCommand :public FSUnDoReDoCommand{
    Q_OBJECT
public:
    FSDenoiseCommand(IFSEditHelper * pHelper, const int & Level);
    ~FSDenoiseCommand();
protected:
    virtual void Undo();
    virtual void Redo();

	virtual void redo() override;
	virtual void undo() override;

private:
    IFSEditHelper *m_pEditHelper;
    int m_NewLevel;
    int m_OldLevel;
};

class FSDeltaVisualCommand :public FSUnDoReDoCommand{
    Q_OBJECT
public:
    FSDeltaVisualCommand(QList<IFSEditHelper *> pHelperList, QPointF Deltascale, QPointF DeltaPos, double DeltaAngle, int index);
    ~FSDeltaVisualCommand();

protected:
    virtual void Undo();
    virtual void Redo();


private:
    QList<IFSEditHelper *> m_pHelperList;
    QPointF m_Deltascale;
    QPointF m_DeltaPos;
    double  m_DeltaAngle;
    bool    m_bFirst;
    int     m_index;
};

class FSHotKeyCommand :public FSUnDoReDoCommand
{
    Q_OBJECT
public:
    FSHotKeyCommand(IFSEditHelper * pHelper, bool bEnable, int bFliterType, QString size, QString Position, QString Style, QColor color, int opacity);
    ~FSHotKeyCommand();

protected:
    virtual void Undo();
    virtual void Redo();

private:
    IFSEditHelper *m_pEditHelper;
    bool m_bNewEnable;
    bool m_bOldEnable;

    int m_bNewFliterType;
    int m_bOldFliterType;

    QString m_NewSize;
    QString m_OldSize;

    QString m_NewPosition;
    QString m_OldPosition;

    QString m_NewStyle;
    QString m_OldStyle;

    QColor m_NewColor;
    QColor m_OldColor;

    int m_NewOpacity;
    int m_OldOpacity;
};

#endif // FSUNDOREDOCOMMEND_H
