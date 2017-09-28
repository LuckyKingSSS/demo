#pragma once

#include <QString>
#include <QStringList>
#include <QMap>

struct Resolution
{
    int mWidth;
    int mHeight;
    Resolution(){}
    Resolution(int width, int height):mWidth(width),mHeight(height){}
    Resolution(const QString &str){
        QStringList list = str.split('*', QString::SkipEmptyParts);
        Q_ASSERT(list.size() == 2);
        if (list.size() == 2){
            mWidth = list.at(0).toInt();
            mHeight = list.at(1).toInt();
        }
    }
    int Area(){
        return mWidth*mHeight;
    }
};

struct ExportSettingInfo
{
    QString filename;
    QString filefourcc;

    QString videofourcc;
    Resolution	resolution;
    double	videoframerate;
    int		videobitrate;

    QString audiofourcc;
    int		audiochannel;
    int		audiosamplerate;
    int		audiobitrate;
};

struct EncParam_Video{
    QString     mVideoFourcc;
    QString     mResolution;
    QString     mDefVideoBitrate;
    QString     mDefVideoFramerate;
    QStringList mVideoFramerates;
    QStringList mVideoBitrates;
};

struct EncParam_Audio{
    QString     mChannel;
    QString     mAudioFourcc;
    QString     mDefSampleRate;
    QString     mDefBitrate;
    QStringList mAudioSamplerates;
    QStringList mAudioBitrates;
};

struct VideoEnc
{
    QString mVideodefFcc;
    QString mVideodefResolution;
    QList<EncParam_Video> mEncParams;
};

struct AudioEnc
{
    QString mAudiodefChannel;
    QString mAudiodefFcc;    
    QList<EncParam_Audio> mEncParams;
    void clear(){

    }
};


struct FormatInfo
{
    QString mName;    // MP4 ,WMV ,MOV
    QString mFormat;  //.mp4 .wmv .mov
    QString mFileFcc;

    VideoEnc mVideoEnc;
    AudioEnc mAudioEnc;
};

Q_DECLARE_METATYPE(FormatInfo);