#ifndef TIMELINEPROPERTYDIALOG_P_H
#define TIMELINEPROPERTYDIALOG_P_H


#include <QtCore/QObject>



class FSNLETimeline;
class TimelineProperty : public QObject
{
    Q_OBJECT
public:
    TimelineProperty(FSNLETimeline *timeline, QObject *parent);

    virtual QString GetPropertyString();

protected:
    virtual QString GetFileLocation();
    virtual QString GetVideoProperty(FSNLETimeline *timeline);
    virtual QString GetAudioProperty(FSNLETimeline *timeline);

    QString GetHeadString(FSNLETimeline *timeline, bool audio = false);
    QString GetKbps(int bps);
    QString GetKHz(int hz);
protected:
    FSNLETimeline *m_timeline;
    QString m_propertyString;
};



class Complex_Property : public TimelineProperty
{
    Q_OBJECT
public:
    Complex_Property(FSNLETimeline *timeline, QObject *parent);

    QString GetPropertyString() override;
};

class Video_Property : public TimelineProperty
{
    Q_OBJECT
public:
    Video_Property(FSNLETimeline *timeline, QObject *parent);

    QString GetPropertyString() override;

};


class Audio_Property : public TimelineProperty
{
    Q_OBJECT
public:
    Audio_Property(FSNLETimeline *timeline, QObject *parent);
    QString GetPropertyString() override;

};

class AudioVideo_Property : public TimelineProperty
{
    Q_OBJECT
public:
    AudioVideo_Property(FSNLETimeline *timeline, QObject *parent);
    QString GetPropertyString() override;

};


class Image_Property : public TimelineProperty
{
    Q_OBJECT
public:
    Image_Property(FSNLETimeline *timeline, QObject *parent);
    QString GetPropertyString() override;

private:
    QString GetVideoProperty(FSNLETimeline *timeline) override;

};

class Caption_Property : public TimelineProperty
{
    Q_OBJECT
public:
    Caption_Property(FSNLETimeline *timeline, QObject *parent);

};

class MouseFilter_Property : public TimelineProperty
{
    Q_OBJECT
public:
    MouseFilter_Property(FSNLETimeline *timeline, QObject *parent);

};




#endif // TIMELINEPROPERTYDIALOG_H
