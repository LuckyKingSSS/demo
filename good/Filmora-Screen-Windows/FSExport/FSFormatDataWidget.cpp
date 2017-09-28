#include "stdafx.h"
#include "FSFormatDataWidget.h"
#include "FSExportDataDef.h"

#include "inc_FSCommonlib/FSCommonWidget.h"
#include "inc_CommonLib/PathOperation.h"
#include "inc_CommonLib/FileOperation.h"
#include "inc_CommonLib/StringOperation.h"
#include "inc_CommonLib/XmlOperation.h"
#include <QtXml/QDomDocument>
#include <QtConcurrent/QtConcurrent>
#include "FSExport.h"

#define FormatFileName "Format.dat"

FSFormatDataWidget::FSFormatDataWidget(const QString &title,QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_StyledBackground);
    CreateUI();
    mpLabelTitle->setText(title);
    QtConcurrent::run([this](){ParseFormatInfo();});
    
}

FSFormatDataWidget::~FSFormatDataWidget()
{

}

void FSFormatDataWidget::CreateUI()
{
    mpLabelTitle     = new FSLabel(this);
    mpListWidget     = new FSListWidget("FSFormatDataWidget_mpListWidget",this);
    QVBoxLayout *pVL = new QVBoxLayout(this);

    pVL->addWidget(mpLabelTitle);
    pVL->addWidget(mpListWidget);
    pVL->setMargin(1);
    pVL->setSpacing(0);


    mpLabelTitle->setObjectName("FSFormatDataWidget_mpLabelTitle");
    mpLabelTitle->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    mpLabelTitle->setFixedHeight(40);
    mpLabelTitle->setContentsMargins(32,5,12,5);
    mpLabelTitle->setAttribute(Qt::WA_StyledBackground);
    mpListWidget->setObjectName("FSFormatDataWidget_mpListWidget");
    mpListWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mpListWidget->setFrameShape(QFrame::NoFrame);
    mpListWidget->setFocusPolicy(Qt::NoFocus);
    mpListWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mpListWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    setFixedWidth(200);
}

bool FSFormatDataWidget::OpenXmlFile(QDomDocument &doc, QString strFile)
{
    QFile file(strFile);
    QString strError = "";
    int nRow = 0, nColumn = 0;
    if (!file.open(QIODevice::ReadOnly)) return false;
    if (!doc.setContent(&file, false, &strError, &nRow, &nColumn))
    {
        file.close();
        return false;
    }
    file.close();
    return true;
}

void FSFormatDataWidget::ParseFormatInfo()
{
    QString formatFile = PathOperation::MakeWorkPath(FormatFileName);
    bool bExisted = FileOperation::IsFileExist(formatFile);
    Q_ASSERT(bExisted);

    QDomDocument doc;
    if (!OpenXmlFile(doc, formatFile))
    {
        //CustomLogOutput(QString("ParseFormatInfo:%1:Open:%2").arg(formatFile).arg(false));
        return;
    }
    //CustomLogOutput(QString("ParseFormatInfo:%1:Open:%2").arg(formatFile).arg(true));
    QDomElement rootElem = doc.documentElement();

    QDomElement formatinfoElem = rootElem.firstChildElement();
    if (formatinfoElem.isNull() || formatinfoElem.tagName() != "FormatInfo") return;

    while (!formatinfoElem.isNull())
    {
        QString fourcc = formatinfoElem.attribute("FourCC");
        if (fourcc == "MP4 " || fourcc == "MOV "|| fourcc== "gif ")
        {
            FormatInfo info;
            info.mFileFcc = fourcc;
            info.mFormat = formatinfoElem.attribute("Format");

            QDomElement tmpElem = formatinfoElem.firstChildElement("Name");
            if (!tmpElem.isNull())
            {
                info.mName = tmpElem.text();
            }

            tmpElem = tmpElem.nextSiblingElement("VideoEnc");
            if (!tmpElem.isNull())
            {
                info.mVideoEnc.mVideodefFcc = tmpElem.attribute("defVideoFourCC");
                info.mVideoEnc.mVideodefResolution = tmpElem.attribute("defResolution");

                QDomElement tmpElem2 = tmpElem.firstChildElement("EncParam");
                while (!tmpElem2.isNull())
                {
                    if (tmpElem2.attribute("VideoFourCC") == info.mVideoEnc.mVideodefFcc)
                    {
                        QString resolution = tmpElem2.attribute("Resolution");
                        QString defBitRate = tmpElem2.attribute("defVideoBitrate");
                        QString defFrameRate = tmpElem2.attribute("defFrameRate");

                        EncParam_Video para;
                        para.mVideoFourcc = info.mVideoEnc.mVideodefFcc;
                        para.mResolution = resolution;
                        para.mDefVideoBitrate = defBitRate;
                        para.mDefVideoFramerate = defFrameRate;

                        QDomElement tmpElem3 = tmpElem2.firstChildElement("FrameRate");
                        if (!tmpElem3.isNull())
                        {
                            QStringList list = tmpElem3.text().split(';', QString::SkipEmptyParts);
                            para.mVideoFramerates = list;
                        }
                        QDomElement tmpElem4 = tmpElem2.firstChildElement("VideoBitrate");
                        if (!tmpElem4.isNull())
                        {
                            para.mVideoBitrates = tmpElem4.text().split(';', QString::SkipEmptyParts);
                        }
                        info.mVideoEnc.mEncParams.push_back(para);

                    }
                    tmpElem2 = tmpElem2.nextSiblingElement();
                }
            }

            tmpElem = tmpElem.nextSiblingElement("AudioEnc");
            if (!tmpElem.isNull())
            {
                info.mAudioEnc.mAudiodefFcc = tmpElem.attribute("defAudioFourCC");
                info.mAudioEnc.mAudiodefChannel = tmpElem.attribute("defChannel");

                QDomElement tmpElem2 = tmpElem.firstChildElement("EncParam");
                while (!tmpElem2.isNull())
                {
                    if (tmpElem2.attribute("AudioFourCC") == info.mAudioEnc.mAudiodefFcc)
                    {
                        EncParam_Audio audioParam;
                        audioParam.mAudioFourcc = info.mAudioEnc.mAudiodefFcc;
                        audioParam.mChannel = tmpElem2.attribute("Channel");
                        audioParam.mDefBitrate = tmpElem2.attribute("defAudioBitrate");
                        audioParam.mDefSampleRate = tmpElem2.attribute("defSampleRate");

                        QDomElement tmpElem3 = tmpElem2.firstChildElement("SampleRate");
                        if (!tmpElem3.isNull())
                        {
                            QStringList list = tmpElem3.text().split(';', QString::SkipEmptyParts);
                            audioParam.mAudioSamplerates = list;
                        }
                        QDomElement tmpElem4 = tmpElem2.firstChildElement("AudioBitrate");
                        if (!tmpElem4.isNull())
                        {
                            audioParam.mAudioBitrates = tmpElem4.text().split(';', QString::SkipEmptyParts);
                        }
                        info.mAudioEnc.mEncParams.push_back(audioParam);
                    }
                    tmpElem2 = tmpElem2.nextSiblingElement();
                }
            }
            mInfoList.push_back(info);
            AddItem(info);
        }
        formatinfoElem = formatinfoElem.nextSiblingElement();
    }
    mpListWidget->setCurrentRow(0);
    ParseFormatInfoReady();
}

void FSFormatDataWidget::AddItem(FormatInfo info)
{
    QListWidgetItem *pItem = new QListWidgetItem(info.mName);
    pItem->setData(Qt::UserRole, InitOptionCurrentText(info));
    mpListWidget->addItem(pItem);
}

double FSFormatDataWidget::FrameRateStr2Double(QString &frameRate)
{
    if (!frameRate.isEmpty())
    {
        QStringList list = frameRate.split(' ');
        Q_ASSERT(2 == list.size());
        if (list.size() != 2)
            return 0.0;
        return list.at(0).toDouble();
    }
    return 0.0;
}

int FSFormatDataWidget::BitRate2Int(QString & bitRate)
{
    if (!bitRate.isEmpty())
    {
        QStringList list = bitRate.split(' ');
        Q_ASSERT(2 == list.size());
        if (list.size() != 2)
            return 0;
        return list.at(0).toInt() * 1000;  //kbps -> bps
    }
    return 0;
}

int FSFormatDataWidget::SimpleRate2Int(QString &simpleRate)
{
    if (!simpleRate.isEmpty())
    {
        QStringList list = simpleRate.split(' ');
        Q_ASSERT(list.size() == 2);
        if (list.size() != 2) return 0.0;
        return list[0].toInt();
    }
    return 0.0;
}

QStringList FSFormatDataWidget::InitOptionCurrentText(const FormatInfo &info)
{
    QStringList list;
    QList <EncParam_Video> Paramlist;
    GetVideoEncParamListWithFcc(info.mVideoEnc.mVideodefFcc, info, Paramlist);
    list.push_back(info.mVideoEnc.mVideodefFcc);
    list.push_back(info.mVideoEnc.mVideodefResolution);    
    EncParam_Video  param = GetVideoEncParamWithResolution(Paramlist, info.mVideoEnc.mVideodefResolution);
    list.push_back(param.mDefVideoFramerate);
    list.push_back(param.mDefVideoBitrate);

    QList <EncParam_Audio> AudioParamlist;
    GetAudioEncParamListWithFcc(info.mAudioEnc.mAudiodefFcc, info, AudioParamlist);
    list.push_back(info.mAudioEnc.mAudiodefFcc);
    list.push_back(info.mAudioEnc.mAudiodefChannel);
    EncParam_Audio  Audioparam = GetAudioEncParamWithChannel(AudioParamlist, info.mAudioEnc.mAudiodefChannel);
    list.push_back(Audioparam.mDefBitrate);
    list.push_back(Audioparam.mDefSampleRate);
    return list;
}

void FSFormatDataWidget::GetVideoEncParamListWithFcc(const QString  & fcc, const FormatInfo &info, QList <EncParam_Video> &list)
{
    for each (auto var in info.mVideoEnc.mEncParams)
    {
        if (var.mVideoFourcc == fcc)
        {
            list.push_back(var);
        }
    }
}

void FSFormatDataWidget::GetAudioEncParamListWithFcc(const QString & fcc, const FormatInfo &info, QList <EncParam_Audio> &list)
{
    for each (auto var in info.mAudioEnc.mEncParams)
    {
        if (var.mAudioFourcc == fcc)
        {
            list.push_back(var);
        }
    }
}

EncParam_Video FSFormatDataWidget::GetVideoEncParamWithResolution(const QList <EncParam_Video> &list, const QString &resolution)
{
    QPair<int, int> pair = {0,0};
    for (int i = 0; i < list.size();i++)
    {
        if (list.at(i).mResolution == resolution)
            return list.at(i);
        if (i == 0)
        {
            pair.second = qAbs(Resolution(resolution).Area() - Resolution(list.at(i).mResolution).Area());
        }
        else 
        {
            int sub = qAbs(Resolution(resolution).Area() - Resolution(list.at(i).mResolution).Area());
            if (pair.second > sub)
            {
                pair.first = i;
                pair.second = sub;
            }           
        }
    }
    if (list.isEmpty())
        return EncParam_Video();
    return list.at(pair.first);
}


EncParam_Audio FSFormatDataWidget::GetAudioEncParamWithChannel(const QList <EncParam_Audio> &list, const QString &channel)
{
    EncParam_Audio param;
    for (int i = 0; i < list.size(); i++)
    {
        if (list.at(i).mChannel ==channel)
            return list.at(i);
    }
    if (!list.isEmpty())
        param = list.at(0);
    return param;
}

Q_INVOKABLE void FSFormatDataWidget::ParseFormatInfoReady()
{
    connect(mpListWidget, &QListWidget::currentRowChanged, this, &FSFormatDataWidget::CurrentRowChanged);
    if (mpListWidget->currentRow()>=0)
        emit CurrentRowChanged(mpListWidget->currentRow());
}

QStringList FSFormatDataWidget::GetResolutionList(const QString  & fcc, int index)
{
    QStringList list;
    if (mpListWidget->count() >index&&mInfoList.size()>index)
    {
        
        QList <EncParam_Video> Paramlist;
        GetVideoEncParamListWithFcc(fcc, mInfoList[index], Paramlist);
        for (auto para : Paramlist)
        {
#ifndef _WIN64
            Resolution res(para.mResolution);
            if (res.Area() > 1920 * 1440) continue;
#endif
            list.push_back(para.mResolution);
        }
    }
    return list;
}

QStringList FSFormatDataWidget::GetFramerateListWithFccAndResolution(int index,const QString &Fcc, const QString &resolution)
{
    if (mpListWidget->count() > index&&mInfoList.size() > index)
    {
        QList <EncParam_Video> Paramlist;
        GetVideoEncParamListWithFcc(Fcc, mInfoList[index], Paramlist);
        for (auto para : Paramlist)
        {
            if (resolution == para.mResolution)
            {
                return para.mVideoFramerates;
            }
        }
    }   
    return QStringList();
}

QStringList FSFormatDataWidget::GetVideoEncoderList(int index)
{
    QStringList list;
    if (mpListWidget->count() > index&&mInfoList.size() > index)
    {
        for (auto Param : mInfoList[index].mVideoEnc.mEncParams)
        {
            if (!list.contains(Param.mVideoFourcc))
            {
                list.push_back(Param.mVideoFourcc);
            }
        }
    }
    return list;
}


EncParam_Video FSFormatDataWidget::GetVideoEncParam(int index, QString resolustion)
{
    if (mpListWidget->item(index)&&mInfoList.size()>index)
    {
        QList <EncParam_Video> List;
        GetVideoEncParamListWithFcc(GetCurrentText(index, Video_Encoder), mInfoList[index], List);
        return GetVideoEncParamWithResolution(List, resolustion);
    }
    return EncParam_Video();
}

EncParam_Audio FSFormatDataWidget::GetAudioEncParam(int index, QString channel)
{
    if (mpListWidget->item(index) && mInfoList.size() > index)
    {
        QList <EncParam_Audio> List;
        GetAudioEncParamListWithFcc(GetCurrentText(index, Audio_Encoder), mInfoList[index], List);
        return GetAudioEncParamWithChannel(List, channel);
    }
    return EncParam_Audio();
}

QStringList FSFormatDataWidget::GetChannelList(const QString &fcc, int index)
{
    QStringList list;
    if (mpListWidget->count() > index&&mInfoList.size() > index)
    {

        QList <EncParam_Audio> Paramlist;
        GetAudioEncParamListWithFcc(fcc, mInfoList[index], Paramlist);
        for (auto para : Paramlist)
        {
            list.push_back(para.mChannel);
        }
    }
    return list;
}

QStringList FSFormatDataWidget::GetAudioEncoderList(int index)
{
    QStringList list;
    if (mpListWidget->count() > index&&mInfoList.size() > index)
    {
        for (auto Param : mInfoList[index].mAudioEnc.mEncParams)
        {
            if (!list.contains(Param.mAudioFourcc))
            {
                list.push_back(Param.mAudioFourcc);
            }
        }
    }
    return list;
}

QStringList FSFormatDataWidget::GetAudioBitrateListWithFccAndChannel(int index, const QString &Fcc, const QString &Channel)
{
     if (mpListWidget->count() > index&&mInfoList.size() > index)
    {
        QList <EncParam_Audio> Paramlist;
        GetAudioEncParamListWithFcc(Fcc, mInfoList[index], Paramlist);
        for (auto para : Paramlist)
        {
            if (Channel == para.mChannel)
            {
                return para.mAudioBitrates;
            }
        }
    }   
    return QStringList();
}

void FSFormatDataWidget::SetDefaultExportRsolution(QSize size)
{
    QString resolution(QString("%1*%2").arg(size.width()).arg(size.height()));
    if (resolution.split("*", QString::SkipEmptyParts).size() != 2)
        return;
    for (int i = 0; i < mInfoList.size(); i++)
    {
        QListWidgetItem *pItem = mpListWidget->item(i);
        QStringList list = pItem->data(Qt::UserRole).toStringList();
        if (Video_Resolution < list.size())
        {
            list[Video_Resolution] = resolution;
            pItem->setData(Qt::UserRole, list);
        }
        mInfoList[i].mVideoEnc.mVideodefResolution = resolution;
        int total = INT_MAX;
        int index = 0;
        EncParam_Video VideoParma;
        QStringList list1 = resolution.split("*", QString::SkipEmptyParts);
        for (int j = 0; j < mInfoList.at(i).mVideoEnc.mEncParams.size(); j++)
        {
            QStringList list = mInfoList.at(i).mVideoEnc.mEncParams.at(j).mResolution.split("*",QString::SkipEmptyParts);         
            if (list.size() == 2)
            {
                int temp = qAbs(list.first().toInt()*list.last().toInt() - list1.first().toInt()*list1.last().toInt());
                if (total>temp)
                {
                    total = temp;
                    index = j;
                }
            }
        }
        if (index < mInfoList.at(i).mVideoEnc.mEncParams.size())
        {
            if (mInfoList.at(i).mVideoEnc.mEncParams.at(index).mResolution != resolution)
            {
                EncParam_Video VideoParma = mInfoList.at(i).mVideoEnc.mEncParams.at(index);
                VideoParma.mResolution = resolution;
                mInfoList[i].mVideoEnc.mEncParams.insert(index, VideoParma);
            }
        }
        QList<EncParam_Video> EncParamList = mInfoList.at(i).mVideoEnc.mEncParams;
    }
    QTimer::singleShot(0, [&](){emit CurrentRowChanged(mpListWidget->currentRow()); });
}

void FSFormatDataWidget::GetExportSettingInfo(ExportSettingInfo &info)
{
    if (mInfoList.size() > mpListWidget->currentRow())
    {
        info.filefourcc = mInfoList.at(mpListWidget->currentRow()).mFileFcc;
        info.filename   = mInfoList.at(mpListWidget->currentRow()).mFormat;
    }
    info.videofourcc = GetCurrentText(mpListWidget->currentRow(), Video_Encoder);
    info.resolution = Resolution(GetCurrentText(mpListWidget->currentRow(), Video_Resolution));
    info.videoframerate = FrameRateStr2Double(GetCurrentText(mpListWidget->currentRow(), Video_FrameRate));
    info.videobitrate = BitRate2Int(GetCurrentText(mpListWidget->currentRow(), Video_BitRate));
    info.audiofourcc = GetCurrentText(mpListWidget->currentRow(), Audio_Encoder);
    info.audiochannel = GetCurrentText(mpListWidget->currentRow(), Audio_Channel).toInt();
    info.audiobitrate = BitRate2Int(GetCurrentText(mpListWidget->currentRow(), Audio_BitRate));
    info.audiosamplerate = SimpleRate2Int(GetCurrentText(mpListWidget->currentRow(), Audio_SampleRate));
}


int FSFormatDataWidget::GetCurrentIndex()
{
    return mpListWidget->currentRow();
}

QString FSFormatDataWidget::GetCurrentText(int index, OptionCurrentText currenType)
{
    if (mpListWidget->count() > index&&mpListWidget->item(index)){
        return mpListWidget->item(index)->data(Qt::UserRole).toStringList().at(currenType);
    }
    return QString();
}

void FSFormatDataWidget::SetCurrentText(const QString & text, OptionCurrentText currenType)
{
    if (mpListWidget->currentRow() >= 0 && mpListWidget->currentItem())
    {
        QListWidgetItem *pItem = mpListWidget->currentItem();
        QStringList list = pItem->data(Qt::UserRole).toStringList();
        if (currenType < list.size())
        {
            list[currenType] = text;
            pItem->setData(Qt::UserRole, list);
        }        
    }
}
