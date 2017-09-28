#ifndef FPARAMETERSETTINGWND_H
#define FPARAMETERSETTINGWND_H

#include "CommonWidgets.h"
#include "FPropertyDialog.h"
#include "FEncodeDataControl.h"

class FLabel;
class FRadioButton;
class FComboBox;
class FButton;

#define  VIDEO_ENCODER     "VideoEncoder"
#define  VIDEO_RESOLUTION  "VideoResolution"
#define  VIDEO_FRAMERATE   "VideoFrameRate"
#define  VIDEO_BITRATE     "VideoBitRate"
#define  AUDIO_ENCODER     "AudioEncoder"
#define  AUDIO_CHANNEL     "AudioChannel"
#define  AUDIO_SAMPLERATE  "AudioSampleRate"
#define  AUDIO_BITRATE     "AudioBitRate"
#define  FILE_FOURCC       "FileFourCC"
#define  FILE_FORMAT       "FileFormat"


class FParameterSettingWnd : public FPropertyDialog
{
	Q_OBJECT

public:
	FParameterSettingWnd(QWidget *parent);
	~FParameterSettingWnd();
	void setCurType(QString strCurType);
	int getHeaderHeight(){ return m_titlebar->height(); };
	void OnMediaTypeChanged(QString strCurType);
private:
	void initConnections();
	void initUI();
	void initData();
	void showParamWnd();
	void LoadVideoEncoderComboBox(FEncodeDataControl::MediaType type, QString strDefParameter);
	void LoadVideoResolutionComboBox(FEncodeDataControl::MediaType type, QString strCurFcc, QString strDefParameter);
	void LoadVideoFrameRateComboBox(FEncodeDataControl::MediaType type, QString strCurFcc, QString strCurResolution, QString strDefParameter);
	void LoadVideoBitRateComboBox(FEncodeDataControl::MediaType type, QString strCurFcc, QString currentResolution, QString strDefParameter);
	void LoadAudioEncoderComboBox(FEncodeDataControl::MediaType type, QString strDefParameter);
	void LoadAudioChannelComboBox(FEncodeDataControl::MediaType type, QString strCurFcc, QString strDefParameter);
	void LoadAudioSampleRateComboBox(FEncodeDataControl::MediaType type, QString strCurFcc, QString currentChannel, QString strDefParameter);
	void LoadAudioBitRateComboBox(FEncodeDataControl::MediaType type, QString currentAudiofCC, QString currentChannel, QString strDefParameter);

	void ConncetVideoEncoder();
	void ConnectVideoResolution();
	void ConnectVideoFrameRate();
	void ConncetVideoBitRate();
	void ConnectAudioEncoder();
	void ConnectAudioChannel();
	void ConnectAudioSampleRate();
	void ConnectAudioBitRate();
	void ConnectDefaultParam();
	void ConnectOKBtn();
	void ConnectCancelBtn();


	void OnVideoEncoderSlot(const QString &text);
	void OnVideoResolutionSlot(const QString &text);
	void OnVideoFrameRateSlot(const QString &text);
	void OnVideoBitRateSlot(const QString& text);
	void OnAudioEncoderSlot(const QString &text);
	void OnAudioChannelSlot(const QString &text);
	void OnAudioSampleRateSlot(const QString &text);
	void OnAudioBitRateSlot(const QString &text);
	void OnClickDefaultBtn();
	void OnClickOKBtn();
	void OnClickCancleBtn();
	void GetUserSelectParam();
	FEncodeDataControl::MediaType getMediaType(QString strType);

   
Q_SIGNALS:
	void sigConfirmParameter(QMap<QString, QString> totalParams);

private:
	QList<QString> m_SupportVidioType;
	QList<QString> m_SupportAudioType;
	bool           m_IsVideo;   //当前类型是否是视频类型
	FLabel*        m_Quality;
	FRadioButton* m_BestQuality;
	FRadioButton* m_BetterQuality;
	FRadioButton* m_GoodQuality;
	FLabel*       m_Video;
	FLabel*       m_Encoder;
	FComboBox*    m_EncoderCombo;
	FLabel*       m_Resolution;
	FComboBox*    m_ResolutionCombo;
	FLabel*       m_FrameRate;
	FComboBox*    m_FrameRateCombo;
	FLabel*       m_BitRate;
	FComboBox*    m_BitRateCombo;
	FLabel*       m_Audio;
	FLabel*       m_AuidoEncoder;
	FComboBox*    m_AudioEncoderCombo;
	FLabel*       m_AudioChanel;
	FComboBox*    m_AuidoChanelCombo;
	FLabel*       m_SampleRate;
	FComboBox*    m_SampleRateCombo;
	FLabel*       m_AudioBitRate;
	FComboBox*    m_AudioBitRateCombo;
	FButton*     m_DefultBtn;
	FButton*     m_OkBtn;
	FButton*     m_CancleBtn;
	FEncodeDataControl::MediaType    m_CurType;
	FEncodeDataControl*  m_DataControl;
	QMap<FEncodeDataControl::OptionCurrentText, QString> m_DefaultParameter;
	QMap<QString, QString> m_UserSelectParameter; //用户选择的参数
};

#endif // FPARAMETERSETTINGWND_H
