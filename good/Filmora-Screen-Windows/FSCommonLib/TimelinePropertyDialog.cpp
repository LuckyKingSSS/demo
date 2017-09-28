#include "stdafx.h"

#include "inc_FSCommonlib/TimelinePropertyDialog.h"


#include "INLEIterator.h"
#include "INLETimelineBase.h"

#include "inc_FSCommonlib/FSNLETimeline.h"
#include "TimelinePropertyDialog_p.h"

#include "inc_CommonLib/TopLevelWindowTitleBar.h"

TimelinePropertyDialog::TimelinePropertyDialog(QWidget *parent)
: FSBaseDialog(tr("Filmora Scrn"), "Filmora Scrn", parent)
{
    setObjectName("TimelinePropertyDialog");
    resize(400, 500);
    m_titlebar->SetButtons(TopLevelWindowTitleBar::Close);
    m_titlebar->SetResizeble(false);
    m_titlebar->setObjectName("TimelinePropertyDialog_m_titlebar");
    m_titlebar->GetButton(TopLevelWindowTitleBar::Close)->setObjectName("TitleBar_CloseButton");

    m_te = new QTextEdit(this);
    m_te->setObjectName("TimelinePropertyDialog_TextEdit");
    m_te->setFrameStyle(QFrame::NoFrame);
    m_te->setReadOnly(true);
    m_te->setTextInteractionFlags(Qt::NoTextInteraction);
    m_te->installEventFilter(this);
    m_te->setWordWrapMode(QTextOption::NoWrap);

    m_btn = new FSButton(this);
    m_btn->setObjectName("TimelinePropertyDialog_OK");
    m_btn->setText(tr("OK"));
    connect(m_btn, &QPushButton::clicked, this, [&](){
        done(1);
    });
}

TimelineProperty* sCreatePropertyFromTimeline(FSNLETimeline *timeline, QObject *pThis)
{
    TimelineProperty *propertyMgr = nullptr;
    int type = timeline->GetNLETimelineType();
    if (type == TimelineType_Complex)
        propertyMgr = new Complex_Property(timeline, pThis);
    else if (type == TimelineType_Video)
        propertyMgr = new Video_Property(timeline, pThis);
    else if (type == TimelineType_Audio)
        propertyMgr = new Audio_Property(timeline, pThis);
    else if (type == TimelineType_VideoAudio)
        propertyMgr = new AudioVideo_Property(timeline, pThis);
    else if (type == TimelineType_Image)
        propertyMgr = new Image_Property(timeline, pThis);
    else if (type == TimelineType_Annotaions_Caption)
        propertyMgr = new Caption_Property(timeline, pThis);

    return propertyMgr;

}
void TimelinePropertyDialog::DoModal(FSNLETimeline *timeline)
{
    TimelineProperty *propertyMgr = sCreatePropertyFromTimeline(timeline, this);
    
    QString text("Unknow Type");
    if (propertyMgr)
        text = propertyMgr->GetPropertyString();
    m_te->setPlainText(text);

    FSBaseDialog::DoModal();
}

QString TimelinePropertyDialog::GetPropertyString(FSNLETimeline *timeline)
{
    TimelineProperty *propertyMgr = sCreatePropertyFromTimeline(timeline, nullptr);
    QString text("Unknow Type");
    if (propertyMgr)
    {
        text = propertyMgr->GetPropertyString();
        delete propertyMgr;
    }
    return text;
}


void TimelinePropertyDialog::resizeEvent(QResizeEvent *event)
{
    const int bottomHeight =50;
    const int margins = 20;
    m_titlebar->setGeometry(1, 1, width()-2, 34);
    QRect rect = this->rect().adjusted(margins, margins + m_titlebar->height(), -margins, -margins);
    m_te->setGeometry(rect.x(), rect.top() + 1, rect.width(), rect.height() - bottomHeight);
    const int btnwidth = 85;
    m_btn->setGeometry((width() - btnwidth) / 2, height() - bottomHeight, btnwidth, 30);
}



bool TimelinePropertyDialog::eventFilter(QObject *watcher, QEvent *event)
{
    if (watcher == m_te)
    {
        if (event->type() == QEvent::Enter)
        {
            qApp->setOverrideCursor(Qt::ArrowCursor);
        }
        else if (event->type() == QEvent::Leave)
        {
            qApp->restoreOverrideCursor();
        }
    }

    return FSBaseDialog::eventFilter(watcher, event);
}

//////////////////////////////////////////////////////////////////////////


#define NEWLINE QString("\n")
#define PROPERTY_KEY(x) \
    x + QString(":")
//#define PROPERTY_INTERNAL_WIDTH 10

#define TRACKINDEX(index) QString("%1 #%2: ").arg(tr("Track")).arg(index)


QString sPropertyKeyValue(QString key, QString value)
{
    const int charcount = 15;
    key = QString("    ") + key;
    int count = qMax(charcount - key.size(), 0);
    while (count-- > 0)
    {
        key += QString(" ");
    }
    return QString("%1  %2").arg(key).arg(value);
}

TimelineProperty::TimelineProperty(FSNLETimeline *timeline, QObject *parent)
:QObject(parent)
, m_timeline(timeline)
{

}


QString TimelineProperty::GetPropertyString()
{

    Q_ASSERT(false);
    return m_propertyString;
}


QString sGetFourCCString(int fourcc)
{
#define FOURCC_VALUE(fourCC, index) ( 0xff & (fourCC >> (index * 8) ) )

    char szFourcc[5] = { 0 };
    sprintf(szFourcc, "%C%C%C%C",
        FOURCC_VALUE(fourcc, 0),
        FOURCC_VALUE(fourcc, 1),
        FOURCC_VALUE(fourcc, 2),
        FOURCC_VALUE(fourcc, 3)

        );

    return QString(szFourcc);
}

QString TimelineProperty::GetVideoProperty(FSNLETimeline *timeline)
{
    int tempType = timeline->GetNLETimelineType();
    Q_ASSERT(tempType == TimelineType_Video || tempType == TimelineType_VideoAudio);
    QString text;
    QSizeF size = timeline->GetSourceSize();
    text += sPropertyKeyValue(PROPERTY_KEY(tr("Dimensions")), QString("%1*%2").arg(size.width()).arg(size.height()));
    text += NEWLINE;

    INT fourcc = 0;
    HRESULT hr = timeline->GetProp()->GetInt(NLEKey::Codec::Video::kFourCC, &fourcc);
    QString fourccString = sGetFourCCString(fourcc);
    text += sPropertyKeyValue(PROPERTY_KEY(tr("Format")), fourccString);
    text += NEWLINE;

    float framerate = timeline->GetSourceFramerate();
    text += sPropertyKeyValue(PROPERTY_KEY(tr("Frame Rate")), QString("%1 FPS").arg(framerate, 0, 'f', 2));
    text += NEWLINE;

    INT bitrate = 0;
    timeline->GetProp()->GetInt(NLEKey::Codec::Video::kBitrate, &bitrate);
    text += sPropertyKeyValue(PROPERTY_KEY(tr("Bit Rate")), QString("%1 kb/s").arg(GetKbps(bitrate)));
    text += NEWLINE;

    return text;
}


QString TimelineProperty::GetAudioProperty(FSNLETimeline *timeline)
{
    Q_ASSERT(timeline->GetNLETimelineType() == TimelineType_Audio || timeline->GetNLETimelineType() == TimelineType_VideoAudio);


    QString text;

    INT fourcc = 0;
    timeline->GetProp()->GetInt(NLEKey::Codec::Audio::kFourCC, &fourcc);
    QString fourccString = sGetFourCCString(fourcc);

    text += sPropertyKeyValue(PROPERTY_KEY(tr("Format")), QString("%1").arg(fourccString));
    text += NEWLINE;

    INT rate = 0;
    timeline->GetProp()->GetInt(NLEKey::Source::kSource_SampleRate, &rate);
    text += sPropertyKeyValue(PROPERTY_KEY(tr("Rate")), QString("%1 kHz").arg(GetKHz(rate)));
    text += NEWLINE;

    return text;
}

QString TimelineProperty::GetFileLocation()
{
    QString filePath = m_timeline->GetFilePath();
    QString text;
    if (!filePath.isEmpty())
        text = QString("%1: %2").arg(tr("File Location")).arg(filePath);

    return text;
}

#include "cCaptureStream.h"

QString TimelineProperty::GetHeadString(FSNLETimeline *timeline, bool audio)
{
    QString text("Unknow Type");
    int streamType = timeline->GetRecordingStreamType();
    if (streamType == NLE_SCREEN_RECORDING_STREAM_TYPE_DESKTOP_VIDEO)
    {
        if (!audio)
            text = tr("Screen Video");
        else
            text = tr("System Audio");

    }
    else if (streamType == NLE_SCREEN_RECORDING_STREAM_TYPE_GAME_VIDEO)
    {
        text = tr("Game Video");

    }
    else if (streamType == NLE_SCREEN_RECORDING_STREAM_TYPE_WEBCAM_VIDEO)
    {
        text = tr("Camera Video");

    }
    else if (streamType == NLE_SCREEN_RECORDING_STREAM_TYPE_SYSTEM_AUDIO)
    {
        text = tr("System Audio");

    }
    else if (streamType == NLE_SCREEN_RECORDING_STREAM_TYPE_MICROPHONE_AUDIO)
    {
        text = tr("Microphone Audio");

    }
    else if (streamType == NLE_SCREEN_RECORDING_STREAM_TYPE_UNKNOWN)
    {
        int nleType = timeline->GetNLETimelineType();
        if (nleType == TimelineType_Video || nleType == TimelineType_VideoAudio)
        {
            if (!audio)
                text = tr("Video");
            else
                text = tr("Audio");

        }
        else if (nleType == TimelineType_Audio)
        {
            text = tr("Audio");
        }
        else if (nleType == TimelineType_Image)
        {
            text = tr("Image");
        }
        else if (nleType == TimelineType_Annotaions_Caption)
        {
            text = tr("Caption");
        }

    }

    return text;
}

QString TimelineProperty::GetKbps(int bps)
{
    if (bps < 1024)
    {
        return QString("%1").arg(1.0 * bps / 1024, 0, 'f', 2, QChar('0'));
    }
    else
        return QString("%1").arg(bps / 1024);
}


QString TimelineProperty::GetKHz(int hz)
{
    return QString("%1").arg(1.0 * hz / 1000, 0, 'f', 2, QChar('0'));

}

//////////////////////////////////////////////////////////////////////////

Complex_Property::Complex_Property(FSNLETimeline *timeline, QObject *parent)
:TimelineProperty(timeline, parent)
{

}


QString Complex_Property::GetPropertyString()
{

    m_propertyString = GetFileLocation();
    m_propertyString += NEWLINE + NEWLINE;

    QList<NLEComPtr<INLETimelineBase>> timelines;
    NLEComPtr<INLEIterator> pIterator = NULL;
    m_timeline->GetSrc()->QueryInterface(IID_INLEIterator, (void**)&pIterator);
    for (pIterator->MoveToBegin(); pIterator->Current(); pIterator->MoveToNext())
    {
        NLEComPtr<INLETimelineBase> pTimelineTemp;
        pIterator->Current()->QueryInterface(IID_INLETimelineBase, (void**)&pTimelineTemp);
        NLE_ASSERT(pTimelineTemp);
        timelines.push_back(pTimelineTemp);
    }

    QList<QString> videoPropertys;
    QList<QString> audioPropertys;
    
    QString text;
    for (auto temp : timelines)
    {
        FSNLETimeline tempTimeline(temp);
        int streamType = tempTimeline.GetRecordingStreamType();
        if (streamType == NLE_SCREEN_RECORDING_STREAM_TYPE_DESKTOP_VIDEO ||
            streamType == NLE_SCREEN_RECORDING_STREAM_TYPE_GAME_VIDEO
            )
        {
            text = GetHeadString(&tempTimeline);
            text += NEWLINE;
            text += GetVideoProperty(&tempTimeline);
            videoPropertys.push_back(text);

            //  桌面录制有系统声音
            if (tempTimeline.GetNLETimelineType() == TimelineType_VideoAudio)
            {
                text = GetHeadString(&tempTimeline, true);
                text += NEWLINE;
                text += GetAudioProperty(&tempTimeline);
                audioPropertys.push_back(text);
            }

        }
        else if ( streamType == NLE_SCREEN_RECORDING_STREAM_TYPE_WEBCAM_VIDEO )
        {
            text = GetHeadString(&tempTimeline);
            text += NEWLINE;
            text += GetVideoProperty(&tempTimeline);
            videoPropertys.push_back(text);
        }
        else if (streamType == NLE_SCREEN_RECORDING_STREAM_TYPE_SYSTEM_AUDIO ||
            streamType == NLE_SCREEN_RECORDING_STREAM_TYPE_MICROPHONE_AUDIO
            )
        {
            text = GetHeadString(&tempTimeline, true);
            text += NEWLINE;
            text += GetAudioProperty(&tempTimeline);
            audioPropertys.push_back(text);
        }

    }


    int trackIndex = 1;
    for (auto temp : videoPropertys)
    {
        m_propertyString += TRACKINDEX(trackIndex++) + temp;
        m_propertyString += NEWLINE;
    }
    for (auto temp : audioPropertys)
    {
        m_propertyString += TRACKINDEX(trackIndex++) + temp;
        m_propertyString += NEWLINE;
    }

    return m_propertyString;
}


Video_Property::Video_Property(FSNLETimeline *timeline, QObject *parent)
:TimelineProperty(timeline, parent)
{

}


QString Video_Property::GetPropertyString()
{
    m_propertyString = GetFileLocation();
    m_propertyString += NEWLINE + NEWLINE;

   
    QString text = GetHeadString(m_timeline);
    text += NEWLINE;
    text += GetVideoProperty(m_timeline);
    m_propertyString += TRACKINDEX(1) + text;

    return m_propertyString;
}

Audio_Property::Audio_Property(FSNLETimeline *timeline, QObject *parent)
:TimelineProperty(timeline, parent)
{

}


QString Audio_Property::GetPropertyString()
{
    m_propertyString = GetFileLocation();
    m_propertyString += NEWLINE + NEWLINE;


    QString text = GetHeadString(m_timeline);
    text += NEWLINE;
    text += GetAudioProperty(m_timeline);
    m_propertyString += TRACKINDEX(1) + text;

    return m_propertyString;
}


AudioVideo_Property::AudioVideo_Property(FSNLETimeline *timeline, QObject *parent)
:TimelineProperty(timeline, parent)
{

}


QString AudioVideo_Property::GetPropertyString()
{
    m_propertyString = GetFileLocation();
    m_propertyString += NEWLINE + NEWLINE;


    QString text = GetHeadString(m_timeline);
    text += NEWLINE;
    text += GetVideoProperty(m_timeline);
    m_propertyString += TRACKINDEX(1) + text;

    m_propertyString += NEWLINE;

    QString text2 = GetHeadString(m_timeline, true);
    text2 += NEWLINE;
    text2 += GetAudioProperty(m_timeline);
    m_propertyString += TRACKINDEX(2) + text2;

    return m_propertyString;
}

Image_Property::Image_Property(FSNLETimeline *timeline, QObject *parent)
:TimelineProperty(timeline, parent)
{

}


QString Image_Property::GetPropertyString()
{
    m_propertyString = GetFileLocation();
    m_propertyString += NEWLINE + NEWLINE;


    QString text = GetHeadString(m_timeline);
    text += NEWLINE;
    text += GetVideoProperty(m_timeline);
    m_propertyString += TRACKINDEX(1) + text;

    return m_propertyString;
}


QString Image_Property::GetVideoProperty(FSNLETimeline *timeline)
{
    QString text;


    QSizeF size = timeline->GetSourceSize();
    text += sPropertyKeyValue(PROPERTY_KEY(tr("Dimensions")), QString("%1*%1").arg(size.width()).arg(size.height()));
    text += NEWLINE;


    INT fourcc = 0;
    timeline->GetProp()->GetInt(NLEKey::Codec::kFourCC, &fourcc);
    QString fourccString = sGetFourCCString(fourcc);
    text += sPropertyKeyValue(PROPERTY_KEY(tr("Format")), QString("%1").arg(fourccString));
    text += NEWLINE;
    return text;

}


Caption_Property::Caption_Property(FSNLETimeline *timeline, QObject *parent)
:TimelineProperty(timeline, parent)
{

}



MouseFilter_Property::MouseFilter_Property(FSNLETimeline *timeline, QObject *parent)
:TimelineProperty(timeline, parent)
{

}