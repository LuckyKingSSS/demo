#ifndef FSOPTIONWIDGET_H
#define FSOPTIONWIDGET_H

#include <QtWidgets/QWidget>

class FSLabel;
class FSButton;
class FSLineEdit;
class FSComboBox;
class FSFormatDataWidget;

class FSOptionWidget : public QWidget
{
    Q_OBJECT


public:
    FSOptionWidget(QWidget *parent, FSFormatDataWidget *pDataWidget);
    ~FSOptionWidget();

    void CheckUserInput();
    QString GetFilePathName();

    void SetRecorderExportState(bool bRecorderExport);
    void SetExportFileName(QString name);

protected:
    void resizeEvent(QResizeEvent *e);

private:
    void CreateUI();
    void TranslateUI();

    void ConnectSignals();

    void SetVideoEncoderComboBox(const QStringList &listFcc, const QString &currentFcc);
    void SetVideoResolutionComboBox(const QStringList &listResolution, const QString &currentResolution);
    void SetVideoFrameRateComboBox(const QStringList &listFrameRate, const QString &currentFrameRate);
    void SetVideoBitRateComboBox(const QStringList &listBitRate, const QString &currentBitRate);
    void SetAudioEncoderComboBox(const QStringList &listFcc, const QString &currentFcc);
    void SetAudioChannelComboBox(const QStringList &listChannel, const QString &currentChannel);
    void SetAudioSampleRateComboBox(const QStringList &listSampleRate, const QString &currentSampleRate);
    void SetAudioBitRateComboBox(const QStringList &listBitRate, const QString &currentBitRate);

    void OnCurrentRowChangedSlot(int index);


    void ConncetVideoEncoder();
    void ConnectVideoResolution();
    void ConnectVideoFrameRate();
    void ConncetVideoBitRate();
    void ConnectAudioEncoder();
    void ConnectAudioChannel();
    void ConnectAudioSampleRate();
    void ConnectAudioBitRate();

    void DisconncetVideoEncoder();
    void DisconnectVideoResolution();
    void DisconnectVideoFrameRate();
    void DisConncetVideoBitRate();
    void DisconnectAudioEncoder();
    void DisconnectAudioChannel();
    void DisconnectAudioSampleRate();
    void DisconnectAudioBitRate();

    void OnVideoEncoderSlot(const QString &text);
    void OnVideoResolutionSlot(const QString &text);
    void OnVideoFrameRateSlot(const QString &text);
    void OnVideoBitRateSlot(const QString& text);
    void OnAudioEncoderSlot(const QString &text);
    void OnAudioChannelSlot(const QString &text);
    void OnAudioSampleRateSlot(const QString &text);
    void OnAudioBitRateSlot(const QString &text);

    void OnSaveToClickedSlot();
    

    void SetVideoParamOptionVisible(bool bvisible);
    void SetAudioParamOptionVisible(bool bvisible);

private:
    bool        mbRecorderState;
    bool        mbVideoParamVisible;
    //OutPut Description
    FSLabel    *mpLabelDescription;
    FSLabel    *mpLabelName;
    FSLabel    *mpLabelSaveTo;
    FSLineEdit *mpLineEditName;
    FSLineEdit *mpLineEditPath;
    FSButton   *mpButtonSave;

    //Video
    FSLabel    *mpLabelVideo;
    FSLabel    *mpLabelVideoEncoder;
    FSLabel    *mpLabelVideoResolution;
    FSLabel    *mpLabelVideoFrameRate;
    FSLabel    *mpLabelVideoBitRate;
    FSComboBox *mpComboBoxVideoEncoder;
    FSComboBox *mpComboBoxVideoResolution;
    FSComboBox *mpComboBoxVideoFrameRate;
    FSComboBox *mpComboBoxVideoBitRate;

    //Audio
    FSLabel    *mpLabelAudio;
    FSLabel    *mpLabelAudioEncoder;
    FSLabel    *mpLabelAudioChannel;
    FSLabel    *mpLabelAudioSampleRate;
    FSLabel    *mpLabelAudioBitRate;
    FSComboBox *mpComboBoxAudioEncoder;
    FSComboBox *mpComboBoxAudioChannel;
    FSComboBox *mpComboBoxAudioSampleRate;
    FSComboBox *mpComboBoxAudioBitRate;



    FSFormatDataWidget *mpDataFormatWidget;

    QString    m_DefaultFileName;

    
};

#endif // FSOPTIONWIDGET_H
