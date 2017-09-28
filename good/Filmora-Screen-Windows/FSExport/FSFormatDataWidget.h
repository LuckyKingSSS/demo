#ifndef FSFORMATDATAWIDGET_H
#define FSFORMATDATAWIDGET_H
#include "FSExportDataDef.h"

#include <QtWidgets/QWidget>
#include <QtXml/QDomDocument>

class FSLabel;
class FSListWidget;

class FSFormatDataWidget : public QWidget
{
    Q_OBJECT

public:
    enum OptionCurrentText{
        Video_Encoder =0,
        Video_Resolution,
        Video_FrameRate,
        Video_BitRate,
        Audio_Encoder,
        Audio_Channel,
        Audio_BitRate,
        Audio_SampleRate
    };
signals:
    void CurrentRowChanged(int row);

public:
    FSFormatDataWidget(const QString  &title,QWidget *parent);
    ~FSFormatDataWidget();

    QStringList GetResolutionList(const QString  & fcc, int index);
    QStringList GetFramerateListWithFccAndResolution(int index, const QString &Fcc, const QString &resolution);
    QStringList GetVideoEncoderList(int index);
    EncParam_Video GetVideoEncParam(int index, QString resolustion);

    EncParam_Audio GetAudioEncParam(int index, QString channel);
    QStringList GetChannelList(const QString &fcc, int index);
    QStringList GetAudioEncoderList(int index);
    QStringList GetAudioBitrateListWithFccAndChannel(int index, const QString &Fcc, const QString &Channel);

    void SetDefaultExportRsolution(QSize resolution);

    void GetExportSettingInfo(ExportSettingInfo &info);

    int GetCurrentIndex();
    QString GetCurrentText(int index, OptionCurrentText currenType);
    void SetCurrentText(const QString  & text,OptionCurrentText currenType);
    bool OpenXmlFile(QDomDocument &doc, QString strpath);

private:
    void CreateUI();
    void ParseFormatInfo();
    void AddItem(FormatInfo info);
    double FrameRateStr2Double(QString &frameRate);
    int    BitRate2Int(QString & bitRate);
    int    SimpleRate2Int(QString &simpleRate);

    QStringList InitOptionCurrentText(const FormatInfo &info);

    EncParam_Video GetVideoEncParamWithResolution(const QList <EncParam_Video> &list, const QString &resolution);
    EncParam_Audio GetAudioEncParamWithChannel(const QList <EncParam_Audio> &list, const QString &channel);

    void GetVideoEncParamListWithFcc(const QString  & fcc, const FormatInfo &info, QList <EncParam_Video> &list);
    void GetAudioEncParamListWithFcc(const QString  & fcc, const FormatInfo &info, QList <EncParam_Audio> &list);


    Q_INVOKABLE void ParseFormatInfoReady();
    

private:
    FSLabel              *mpLabelTitle;
    FSListWidget         *mpListWidget;
    QVector< FormatInfo > mInfoList;
    
};

#endif // FSFORMATDATAWIDGET_H
