#include "stdafx.h"
#include "FSUnDoReDoCommand.h"
#include "FSEditHelper.h"

FSUnDoReDoCommand::FSUnDoReDoCommand(QObject *parent)
:UndoCommand()
{
}

FSUnDoReDoCommand::~FSUnDoReDoCommand()
{

}

void FSUnDoReDoCommand::Undo()
{
    emit signalUpdate(); 
}

void FSUnDoReDoCommand::Redo()
{
    emit signalUpdate();   
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FSVisaulUndoCommand::FSVisaulUndoCommand(IFSEditHelper *pEditHelper, QPair<double, double> Scale, QPair<double, double> Pos, double Angle, bool bNew)
    :FSUnDoReDoCommand()
{
    Q_ASSERT(pEditHelper);
    m_pEditHelper = pEditHelper;
    if (bNew){
        pEditHelper->GetPosition(m_NewPos.first, m_NewPos.second);
        pEditHelper->GetScale(m_NewScale.first, m_NewScale.second);
        m_NewAngle = pEditHelper->GetAngle();
        m_OldPos = Pos;
        m_OldScale = Scale;
        m_OldAngle = Angle;
    }
    else{
        pEditHelper->GetPosition(m_OldPos.first, m_OldPos.second);
        pEditHelper->GetScale(m_OldScale.first, m_OldScale.second);
        m_OldAngle = pEditHelper->GetAngle();
        m_NewPos = Pos;
        m_NewScale = Scale;
        m_NewAngle = Angle;
    }
    connect(this, &FSVisaulUndoCommand::signalUpdate, m_pEditHelper, &IFSEditHelper::CmdUpdate, Qt::UniqueConnection);
}

FSVisaulUndoCommand::~FSVisaulUndoCommand()
{

}

void FSVisaulUndoCommand::Undo()
{   
    m_pEditHelper->SetScale(m_OldScale.first, m_OldScale.second);
    m_pEditHelper->SetAngle(m_OldAngle);
    m_pEditHelper->SetPosition(m_OldPos.first, m_OldPos.second);
    m_pEditHelper->Updata();
    FSUnDoReDoCommand::Undo();
}

void FSVisaulUndoCommand::Redo()
{   
    m_pEditHelper->SetScale(m_NewScale.first, m_NewScale.second);
    m_pEditHelper->SetAngle(m_NewAngle);
    m_pEditHelper->SetPosition(m_NewPos.first, m_NewPos.second);
    m_pEditHelper->Updata();
    FSUnDoReDoCommand::Redo();
}




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FSAudioUnDoCommand::FSAudioUnDoCommand(IFSEditHelper *pEditHelper, int Voluem, bool isMix2MononChecked, bool bNew)
    :FSUnDoReDoCommand()
{
    Q_ASSERT(pEditHelper);
    m_pTimeline = pEditHelper->GetTimeLine();
    if (bNew)
    {
        m_NewVoluem = m_pTimeline->GetVolume();
        m_NewMix2MononChecked = m_pTimeline->GetMix2Mono();
        m_OldVoluem = Voluem;
        m_OldMix2MononChecked = isMix2MononChecked;
    }
    else{
        m_OldVoluem = m_pTimeline->GetVolume();
        m_OldMix2MononChecked = m_pTimeline->GetMix2Mono();
        m_NewVoluem = Voluem;
        m_NewMix2MononChecked = isMix2MononChecked;
    }
    connect(this, &FSAudioUnDoCommand::signalUpdate, pEditHelper, &IFSEditHelper::CmdUpdate, Qt::UniqueConnection);
}

FSAudioUnDoCommand::~FSAudioUnDoCommand()
{

}

void FSAudioUnDoCommand::Undo()
{
    m_pTimeline->SetVolume(m_OldVoluem);
    m_pTimeline->SetMix2Mono(m_OldMix2MononChecked);
    FSUnDoReDoCommand::Undo();
}

void FSAudioUnDoCommand::Redo()
{
    m_pTimeline->SetVolume(m_NewVoluem);
    m_pTimeline->SetMix2Mono(m_NewMix2MononChecked);
    FSUnDoReDoCommand::Redo();
}




void FSAudioUnDoCommand::redo()
{
    Redo();
	QTimer::singleShot(0, [&]{emit signalTrackUpdate(); });
}

void FSAudioUnDoCommand::undo()
{
    Undo();
	QTimer::singleShot(0, [&]{emit signalTrackUpdate(); });
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FSCursorVisualUnDoCommand::FSCursorVisualUnDoCommand(IFSEditHelper *pEditHelper, double scale, double Opacity)
    :FSUnDoReDoCommand()
    , m_NewScale(scale)
    , m_NewOpacity(Opacity)
{
    Q_ASSERT(pEditHelper);
    Q_ASSERT((pEditHelper->GetTimeLine()->IsDesktopVideo()));
    m_pEditHelper = pEditHelper;
    m_OldScale=(static_cast<FSVideoPicEditHelper *>(m_pEditHelper))->GetMouseScale();
    m_OldOpacity = (static_cast<FSVideoPicEditHelper *>(m_pEditHelper))->GetMouseOpacity();
    connect(this, &FSCursorVisualUnDoCommand::signalUpdate, m_pEditHelper, &IFSEditHelper::CmdUpdate, Qt::UniqueConnection);
}

FSCursorVisualUnDoCommand::~FSCursorVisualUnDoCommand()
{

}

void FSCursorVisualUnDoCommand::Undo()
{
    (static_cast<FSVideoPicEditHelper *>(m_pEditHelper))->SetMouseScale(m_OldScale);
    (static_cast<FSVideoPicEditHelper *>(m_pEditHelper))->SetMouseOpacity(m_OldOpacity);
    m_pEditHelper->Updata();
    FSUnDoReDoCommand::Undo();
}

void FSCursorVisualUnDoCommand::Redo()
{
    (static_cast<FSVideoPicEditHelper *>(m_pEditHelper))->SetMouseScale(m_NewScale);
    (static_cast<FSVideoPicEditHelper *>(m_pEditHelper))->SetMouseOpacity(m_NewOpacity);
    m_pEditHelper->Updata();
    FSUnDoReDoCommand::Redo();
}




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FSMouseHighlightUnDoCommand::FSMouseHighlightUnDoCommand(IFSEditHelper *pEditHelper, QColor color)
    :FSUnDoReDoCommand()
    , m_NewColor(color)
{
    Q_ASSERT(pEditHelper);
    Q_ASSERT((pEditHelper->GetTimeLine()->IsDesktopVideo()));
    m_pEditHelper = pEditHelper;
    m_OldColor = (static_cast<FSVideoPicEditHelper *>(m_pEditHelper))->GetHighlightColor();
    connect(this, &FSMouseHighlightUnDoCommand::signalUpdate, m_pEditHelper, &IFSEditHelper::CmdUpdate, Qt::UniqueConnection);
}

FSMouseHighlightUnDoCommand::~FSMouseHighlightUnDoCommand()
{

}

void FSMouseHighlightUnDoCommand::Undo()
{
    (static_cast<FSVideoPicEditHelper *>(m_pEditHelper))->SetHighlightColor(m_OldColor);
    m_pEditHelper->Updata();
    FSUnDoReDoCommand::Undo();
}

void FSMouseHighlightUnDoCommand::Redo()
{
    (static_cast<FSVideoPicEditHelper *>(m_pEditHelper))->SetHighlightColor(m_NewColor);
    m_pEditHelper->Updata();
    FSUnDoReDoCommand::Redo();
}




///////////////////////////////////////////////////////////////////////////////////////////////////////////////
FSMouseRingsUnDoCommand::FSMouseRingsUnDoCommand(IFSEditHelper *pEditHelper, QColor color)
    :FSMouseHighlightUnDoCommand(pEditHelper, color)
{
    m_OldColor = (static_cast<FSVideoPicEditHelper *>(pEditHelper))->GetMouseRingColor();
    connect(this, &FSMouseRingsUnDoCommand::signalUpdate, m_pEditHelper, &IFSEditHelper::CmdUpdate, Qt::UniqueConnection);
}

FSMouseRingsUnDoCommand::~FSMouseRingsUnDoCommand()
{

}

void FSMouseRingsUnDoCommand::Undo()
{
    (static_cast<FSVideoPicEditHelper *>(m_pEditHelper))->SetMouseRingColor(m_OldColor);
    m_pEditHelper->Updata();
    FSUnDoReDoCommand::Undo();
}

void FSMouseRingsUnDoCommand::Redo()
{
    (static_cast<FSVideoPicEditHelper *>(m_pEditHelper))->SetMouseRingColor(m_NewColor);
    m_pEditHelper->Updata();
    FSUnDoReDoCommand::Redo();
}




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FSMouseClickSoundUnDoCommand::FSMouseClickSoundUnDoCommand(IFSEditHelper *pEditHelper, int voluem)
    :FSUnDoReDoCommand()
    , m_NewVoluem(voluem)
{
    Q_ASSERT(pEditHelper);
    Q_ASSERT((pEditHelper->GetTimeLine()->IsDesktopVideo()));
    m_pEditHelper = pEditHelper;
    m_OldVoluem = (static_cast<FSVideoPicEditHelper *>(m_pEditHelper))->GetMouseClickSound();
    connect(this, &FSMouseClickSoundUnDoCommand::signalUpdate, m_pEditHelper, &IFSEditHelper::CmdUpdate, Qt::UniqueConnection);
}

FSMouseClickSoundUnDoCommand::~FSMouseClickSoundUnDoCommand()
{

}

void FSMouseClickSoundUnDoCommand::Undo()
{
    (static_cast<FSVideoPicEditHelper *>(m_pEditHelper))->SetMouseClickSound(m_OldVoluem);
    FSUnDoReDoCommand::Undo();
}

void FSMouseClickSoundUnDoCommand::Redo()
{
    (static_cast<FSVideoPicEditHelper *>(m_pEditHelper))->SetMouseClickSound(m_NewVoluem);
    FSUnDoReDoCommand::Redo();
}





void FSMouseClickSoundUnDoCommand::redo()
{
    Redo();
}

void FSMouseClickSoundUnDoCommand::undo()
{
    Undo();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FSMouseClickSoundDeleteConmmand::FSMouseClickSoundDeleteConmmand(IFSEditHelper *pEditHelper)
    :FSUnDoReDoCommand()
{
    Q_ASSERT(pEditHelper);
    Q_ASSERT((pEditHelper->GetTimeLine()->IsDesktopVideo()));
    Q_ASSERT(static_cast<FSVideoPicEditHelper *>(pEditHelper)->IsContainMouseSound(m_SoundFilePath));
    m_pEditHelper = pEditHelper;
    connect(this, &FSMouseClickSoundDeleteConmmand::signalUpdate, m_pEditHelper, &IFSEditHelper::CmdUpdate, Qt::UniqueConnection);
}

FSMouseClickSoundDeleteConmmand::~FSMouseClickSoundDeleteConmmand()
{

}

void FSMouseClickSoundDeleteConmmand::Undo()
{
    static_cast<FSVideoPicEditHelper *>(m_pEditHelper)->AddMouseSound(m_SoundFilePath);
    FSUnDoReDoCommand::Undo();
}

void FSMouseClickSoundDeleteConmmand::Redo()
{
    static_cast<FSVideoPicEditHelper *>(m_pEditHelper)->DeleteMouseClickSound();
    FSUnDoReDoCommand::Redo();
}




////////////////////////////////////////////////////////////////////////////////////////////////////////////
FSMouseHighLightDeleteCommand::FSMouseHighLightDeleteCommand(IFSEditHelper *pEditHelper)
    :FSUnDoReDoCommand()
    , m_pTimelineBase(NULL)
{
    Q_ASSERT(pEditHelper);
    Q_ASSERT((pEditHelper->GetTimeLine()->IsDesktopVideo()));
    Q_ASSERT(static_cast<FSVideoPicEditHelper *>(pEditHelper)->IsContainMouseHighlight(&m_pTimelineBase));
    m_pEditHelper = pEditHelper;
    connect(this, &FSMouseHighLightDeleteCommand::signalUpdate, m_pEditHelper, &IFSEditHelper::CmdUpdate, Qt::UniqueConnection);
}

FSMouseHighLightDeleteCommand::~FSMouseHighLightDeleteCommand()
{

}

void FSMouseHighLightDeleteCommand::Undo()
{
    static_cast<FSVideoPicEditHelper *>(m_pEditHelper)->AddMouseHighLight(m_pTimelineBase);
    FSUnDoReDoCommand::Undo();
}

void FSMouseHighLightDeleteCommand::Redo()
{
    static_cast<FSVideoPicEditHelper *>(m_pEditHelper)->DeleteMouseHighlight();
    FSUnDoReDoCommand::Redo();
}



/////////////////////////////////////////////////////////////////////////////////////////////////////
FSMouseRingsDeleteCommand::FSMouseRingsDeleteCommand(IFSEditHelper *pEditHelper)
    :FSUnDoReDoCommand()
{
    Q_ASSERT(pEditHelper);
    Q_ASSERT((pEditHelper->GetTimeLine()->IsDesktopVideo()));
    Q_ASSERT(static_cast<FSVideoPicEditHelper *>(pEditHelper)->IsContainMouseRings(&m_pTimelineBase));
    m_pEditHelper = pEditHelper;
    connect(this, &FSMouseRingsDeleteCommand::signalUpdate, m_pEditHelper, &IFSEditHelper::CmdUpdate, Qt::UniqueConnection);
}

FSMouseRingsDeleteCommand::~FSMouseRingsDeleteCommand()
{

}

void FSMouseRingsDeleteCommand::Undo()
{
    static_cast<FSVideoPicEditHelper *>(m_pEditHelper)->AddMouseRings(m_pTimelineBase);
    FSUnDoReDoCommand::Undo();
}

void FSMouseRingsDeleteCommand::Redo()
{
    static_cast<FSVideoPicEditHelper *>(m_pEditHelper)->DeleteMouseRing();
    FSUnDoReDoCommand::Redo();
}




////////////////////////////////////////////////////////////////////////////////////////////////////////
FSCaptionChangeCommand::FSCaptionChangeCommand(IFSEditHelper *pHelper)
    :FSUnDoReDoCommand()
    , m_bFirst(true)
{
    Q_ASSERT(pHelper);
    m_pEditHelper = pHelper;
    connect(this, &FSCaptionChangeCommand::signalUpdate, m_pEditHelper, &IFSEditHelper::CmdUpdate, Qt::UniqueConnection);
}

FSCaptionChangeCommand::~FSCaptionChangeCommand()
{

}

void FSCaptionChangeCommand::Undo()
{
    static_cast<FSTextEditHelper *>(m_pEditHelper)->GetCaption()->UnDo();
    m_pEditHelper->Updata();
    FSUnDoReDoCommand::Undo();
    qDebug() << "FSCaptionChangeCommand::Undo()";
}

void FSCaptionChangeCommand::Redo()
{
    if (!m_bFirst)
    {
        static_cast<FSTextEditHelper *>(m_pEditHelper)->GetCaption()->ReDo();    
    }   

    //放到事件循环末尾通知界面重绘，让Caption设置属性生效。
    QTimer::singleShot(0, [this]{
        m_pEditHelper->Updata();
        FSUnDoReDoCommand::Redo();
    });
    m_bFirst = false;
}



//////////////////////////////////////////////////////////////////////////////////////
FSMaskPathChangeCommand::FSMaskPathChangeCommand(IFSEditHelper *pHelper,const QString &MaskPathStr)
    :FSUnDoReDoCommand()
    , m_OldMaskPathStr(MaskPathStr)
{
    Q_ASSERT(pHelper);
    Q_ASSERT(((pHelper->GetEditType() == IFSEditHelper::Edit_Video) || (pHelper->GetEditType() == IFSEditHelper::Edit_Picture)));
    m_pEditHelper = pHelper;
    m_NewMaskPathStr=static_cast<FSVideoPicEditHelper *>(m_pEditHelper)->GetMaskPath();
    connect(this, &FSMaskPathChangeCommand::signalUpdate, m_pEditHelper, &IFSEditHelper::CmdUpdate, Qt::UniqueConnection);
}

FSMaskPathChangeCommand::~FSMaskPathChangeCommand()
{

}

void FSMaskPathChangeCommand::Undo()
{
    static_cast<FSVideoPicEditHelper *>(m_pEditHelper)->SetMaskPath(m_OldMaskPathStr);
    static_cast<FSVideoPicEditHelper *>(m_pEditHelper)->EnableScaleMode(false);
    m_pEditHelper->Updata();
    FSUnDoReDoCommand::Undo();
}

void FSMaskPathChangeCommand::Redo()
{
    static_cast<FSVideoPicEditHelper *>(m_pEditHelper)->SetMaskPath(m_NewMaskPathStr);
    static_cast<FSVideoPicEditHelper *>(m_pEditHelper)->EnableScaleMode(false);
    m_pEditHelper->Updata();
    FSUnDoReDoCommand::Redo();
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FSDenoiseCommand::FSDenoiseCommand(IFSEditHelper * pHelper, const int & Level)
    :FSUnDoReDoCommand(),
    m_NewLevel(Level)
{
    Q_ASSERT(pHelper);
    m_pEditHelper = pHelper;
    m_OldLevel=m_pEditHelper->GetTimeLine()->GetDenoiseLevel();
    connect(this, &FSDenoiseCommand::signalUpdate, m_pEditHelper, &IFSEditHelper::CmdUpdate, Qt::UniqueConnection);
}

FSDenoiseCommand::~FSDenoiseCommand()
{

}

void FSDenoiseCommand::Undo()
{
    m_pEditHelper->GetTimeLine()->SetDenoiseLevel(m_OldLevel);
    FSUnDoReDoCommand::Undo();
}

void FSDenoiseCommand::Redo()
{
    m_pEditHelper->GetTimeLine()->SetDenoiseLevel(m_NewLevel);
    FSUnDoReDoCommand::Redo();
}

void FSDenoiseCommand::redo()
{
	Redo();
	QTimer::singleShot(0, [&]{emit signalTrackUpdate(); });
}

void FSDenoiseCommand::undo()
{
	Undo();
	QTimer::singleShot(0, [&]{emit signalTrackUpdate(); });
}

////////////////////////////////////////////////////////////////////////////////////////
FSDeltaVisualCommand::FSDeltaVisualCommand(QList<IFSEditHelper *> pHelperList, QPointF Deltascale, QPointF DeltaPos, double DeltaAngle, int index)
    :FSUnDoReDoCommand(),
    m_pHelperList(pHelperList),
    m_Deltascale(Deltascale),
    m_DeltaPos(DeltaPos),
    m_DeltaAngle(DeltaAngle),
    m_index(index),
    m_bFirst(true)
{
    for (auto item : m_pHelperList)
    {
        connect(this, &FSDeltaVisualCommand::signalUpdate, item, &IFSEditHelper::CmdUpdate, Qt::UniqueConnection);
    }
}

FSDeltaVisualCommand::~FSDeltaVisualCommand()
{

}

void FSDeltaVisualCommand::Undo()
{
    QPointF pt(1.0 / m_Deltascale.x(), 1.0 / m_Deltascale.y());
    QPointF pt1(-m_DeltaPos.x(), -m_DeltaPos.y());
    for (auto item :m_pHelperList)
    {
        if (item->GetEditType()==IFSEditHelper::EditType::Edit_Text)
            continue;
        if (qAbs(pt.x() - 1)>1e-4 || qAbs(pt.y() - 1)>1e-4)
            item->SetScaleWithDelta(pt, m_index);
        if (qAbs(pt1.x())>1e-4 || qAbs(pt1.y())>1e-4)
            item->SetPosWithDelta(pt1);
        if (qAbs(m_DeltaAngle)>1e-4)
            item->SetAngleWithDelta(-m_DeltaAngle,false);
    }
    FSUnDoReDoCommand::Undo();
}

void FSDeltaVisualCommand::Redo()
{
    if (m_bFirst)
    {
        m_bFirst = false;
        return;
    }
    for (auto item : m_pHelperList)
    {
        if (item->GetEditType() == IFSEditHelper::EditType::Edit_Text)
        {
            continue;
        }
            
        if (qAbs(m_Deltascale.x() - 1)>1e-4 || qAbs(m_Deltascale.y() - 1))
            item->SetScaleWithDelta(m_Deltascale, m_index);
        if (qAbs(m_DeltaPos.x())>1e-4 || qAbs(m_DeltaPos.y()))
            item->SetPosWithDelta(m_DeltaPos);
        if (qAbs(m_DeltaAngle)>1e-4)
            item->SetAngleWithDelta(m_DeltaAngle,false);
    }
    FSUnDoReDoCommand::Redo();
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FSHotKeyCommand::FSHotKeyCommand(IFSEditHelper * pHelper, bool bEnable, int bFliterType, QString size, QString Position, QString Style, QColor color, int opacity)
    :FSUnDoReDoCommand(),
    m_bNewEnable(bEnable),
    m_bNewFliterType(bFliterType),
    m_NewSize(size),
    m_NewPosition(Position),
    m_NewStyle(Style),
    m_NewColor(color),
    m_NewOpacity(opacity)
{
    Q_ASSERT(pHelper);
    m_pEditHelper = pHelper;

    FSVideoPicEditHelper *pTemp=static_cast<FSVideoPicEditHelper *>(m_pEditHelper);
    QStringList list;
    m_bOldEnable = pTemp->IsHotKeyEnable();
    m_OldSize = m_NewSize;
    pTemp->GetHotKeySize(list, m_OldSize);
    m_OldPosition = m_NewPosition;
    pTemp->GetHotKeyPosition(list, m_OldPosition);
    m_OldStyle = m_NewStyle;
    pTemp->GetHotKeyStyle(list, m_OldStyle);
    m_OldColor = pTemp->GetHotKeyColor();
    m_OldOpacity = pTemp->GetHotKeyOpacity();
    m_bOldFliterType = pTemp->GetHotKeyFliterType();
    connect(this, &FSHotKeyCommand::signalUpdate, m_pEditHelper, &IFSEditHelper::CmdUpdate, Qt::UniqueConnection);
}

FSHotKeyCommand::~FSHotKeyCommand()
{

}

void FSHotKeyCommand::Undo()
{
    FSVideoPicEditHelper *pTemp = static_cast<FSVideoPicEditHelper *>(m_pEditHelper);
    pTemp->SetHotKeyEnable(m_bOldEnable);
    pTemp->SetHotKeySize(m_OldSize);
    pTemp->SetHotKeyPosition(m_OldPosition);
    pTemp->SetHotKeyStyle(m_OldStyle);
    pTemp->SetHotKeyColor(m_OldColor);
    pTemp->SetHotKeyOpacity(m_OldOpacity);
    pTemp->SetHotKeyFliterType(m_bOldFliterType);
    QTimer::singleShot(0, [&]{FSUnDoReDoCommand::Undo(); });

}

void FSHotKeyCommand::Redo()
{
    FSVideoPicEditHelper *pTemp = static_cast<FSVideoPicEditHelper *>(m_pEditHelper);
    pTemp->SetHotKeyEnable(m_bNewEnable);
    pTemp->SetHotKeySize(m_NewSize);
    pTemp->SetHotKeyPosition(m_NewPosition);
    pTemp->SetHotKeyStyle(m_NewStyle);
    pTemp->SetHotKeyColor(m_NewColor);
    pTemp->SetHotKeyOpacity(m_NewOpacity);
    pTemp->SetHotKeyFliterType(m_bNewFliterType);
    QTimer::singleShot(0, [&]{FSUnDoReDoCommand::Redo(); });
}
