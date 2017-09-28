#include "stdafx.h"
#include "FParameterSettingWnd.h"
#include "..\include\inc_FCommonLib\FCommonWidgets.h"
#include "FEncodeDataControl.h"


FParameterSettingWnd::FParameterSettingWnd(QWidget *parent)
	: FPropertyDialog(nullptr,parent)
{
	setAttribute(Qt::WA_StyledBackground);
	initUI();
	initData();
	initConnections();
	resize(450,470);
}

FParameterSettingWnd::~FParameterSettingWnd()
{

}

void FParameterSettingWnd::setCurType(QString strCurType)
{
	 getMediaType(strCurType);
}

void FParameterSettingWnd::initConnections()
{
	ConncetVideoEncoder();
	ConnectVideoResolution();
	ConnectVideoFrameRate();
	ConncetVideoBitRate();
	ConnectAudioEncoder();
	ConnectAudioChannel();
	ConnectAudioSampleRate();
	ConnectAudioBitRate();
	ConnectOKBtn();
	ConnectCancelBtn();
	ConnectDefaultParam();
}

void FParameterSettingWnd::initUI()
{
	int iOffsetHeight = getHeaderHeight();
	int iTopLevelLabelWidth = 50;
	int iTopLevelLabelHeight = 23;
	int iSecondLevelLabelWidth = 132;
	int iSecondLevelLabelHeight = 23;
	int iSecondLevelComboHeight = 25;
	int iSecondLevelComboWidth = 173;
	int iLavelComboSpace = 8;           // combobox和前面的label的间距
	int iComboSpace = 6;               // 上下combobox 之间的间隔
	QPoint firstReferencePos(20,10);   
	QPoint secondReferencePos(20,50);
	QPoint thirdReferencePos(20,220);

     m_Quality = new FLabel(this);
	 m_Quality->setText(tr("Quality"));
	 m_Quality->setGeometry(firstReferencePos.x(), firstReferencePos.y() + iOffsetHeight, iTopLevelLabelWidth, iTopLevelLabelHeight);

	 m_BestQuality = new FRadioButton(this);
	 m_BestQuality->setText(tr("Best"));
	 m_BestQuality->setGeometry(90, firstReferencePos.y() + iOffsetHeight, iTopLevelLabelWidth, iTopLevelLabelHeight);

	 m_BetterQuality = new FRadioButton(this);
	 m_BetterQuality->setText(tr("Better"));
	 m_BetterQuality->setGeometry(202, firstReferencePos.y() + iOffsetHeight, iTopLevelLabelWidth, iTopLevelLabelHeight);

	 m_GoodQuality = new FRadioButton(this);
	 m_GoodQuality->setText(tr("Good"));
	 m_GoodQuality->setGeometry(320, firstReferencePos.y() + iOffsetHeight, iTopLevelLabelWidth, iTopLevelLabelHeight);

	 m_Video = new FLabel(this);
	 m_Video->setText(tr("Video"));
	 m_Video->setGeometry(secondReferencePos.x(), secondReferencePos.y() + iOffsetHeight, iTopLevelLabelWidth, iTopLevelLabelHeight);

	 m_Encoder = new FLabel(this);
	 m_Encoder->setText(tr("Encoder:"));
	 m_Encoder->setObjectName("EncoderLabel");
	 m_Encoder->setGeometry(0, 70 + iOffsetHeight, iSecondLevelLabelWidth, iSecondLevelLabelHeight);
	
	 int iComboStartX = iSecondLevelLabelWidth + iLavelComboSpace; //combobox x轴起点坐标
	 int iComboStartY =100;
	 m_EncoderCombo = new FComboBox(this);
	 m_EncoderCombo->setGeometry(iComboStartX, iComboStartY, iSecondLevelComboWidth, iSecondLevelComboHeight);


	 int iYBeginpos = m_EncoderCombo->geometry().bottom() + iComboSpace;
	 int iLabelYBeginpos = m_EncoderCombo->geometry().bottom() + iComboSpace + iSecondLevelComboHeight / 2;
	 m_Resolution = new FLabel(this);
	 m_Resolution->setText(tr("Resolution:"));
	 m_Resolution->setGeometry(0, iLabelYBeginpos, iSecondLevelLabelWidth, iSecondLevelLabelHeight);

	 m_ResolutionCombo = new FComboBox(this);
	 m_ResolutionCombo->setGeometry(iComboStartX, iYBeginpos, iSecondLevelComboWidth, iSecondLevelComboHeight);

	 iYBeginpos = m_ResolutionCombo->geometry().bottom() + iComboSpace;
	 iLabelYBeginpos = m_ResolutionCombo->geometry().bottom() + iComboSpace + iSecondLevelComboHeight / 2;
	 m_FrameRate = new FLabel(this);
	 m_FrameRate->setText(tr("Frame Rate:"));
	 m_FrameRate->setGeometry(0, iLabelYBeginpos, iSecondLevelLabelWidth, iSecondLevelLabelHeight);

	 m_FrameRateCombo = new FComboBox(this);
	 iComboStartY = m_ResolutionCombo->geometry().bottom() + iComboSpace;
	 m_FrameRateCombo->setGeometry(iComboStartX, iComboStartY, iSecondLevelComboWidth, iSecondLevelComboHeight);

	 iLabelYBeginpos = m_FrameRateCombo->geometry().bottom() + +iComboSpace + iSecondLevelComboHeight / 2;
	 m_BitRate = new FLabel(this);
	 m_BitRate->setText(tr("Bit Rate:"));
	 m_BitRate->setGeometry(0, iLabelYBeginpos, iSecondLevelLabelWidth, iSecondLevelLabelHeight);

	 m_BitRateCombo = new FComboBox(this);
	 iComboStartY = m_FrameRateCombo->geometry().bottom() + iComboSpace;
	 m_BitRateCombo->setGeometry(iComboStartX, iComboStartY, iSecondLevelComboWidth, iSecondLevelComboHeight);

	 m_Audio = new FLabel(this);
	 m_Audio->setText(tr("Audio"));
	 m_Audio->setGeometry(thirdReferencePos.x(), thirdReferencePos.y() + iOffsetHeight, iTopLevelLabelWidth, iTopLevelLabelHeight);

	 m_AuidoEncoder = new FLabel(this);
	 m_AuidoEncoder->setText(tr("Encoder:"));

	 iLabelYBeginpos = 285;
	 m_AuidoEncoder->setGeometry(0, iLabelYBeginpos, iSecondLevelLabelWidth, iSecondLevelLabelHeight);
	 m_AudioEncoderCombo = new FComboBox(this);
	 m_AudioEncoderCombo->setGeometry(iComboStartX, 253 + iOffsetHeight, iSecondLevelComboWidth, iSecondLevelComboHeight);

	 m_AudioChanel = new FLabel(this);
	 m_AudioChanel->setText(tr("Chanel:"));
	 iLabelYBeginpos = m_AudioEncoderCombo->geometry().bottom() + +iComboSpace + iSecondLevelComboHeight / 2;
	 m_AudioChanel->setGeometry(0, 290 + iOffsetHeight,iSecondLevelLabelWidth, iSecondLevelLabelHeight);

	 iComboStartY = m_AudioEncoderCombo->geometry().bottom() + iComboSpace;
	 m_AuidoChanelCombo = new FComboBox(this);
	 m_AuidoChanelCombo->setGeometry(iComboStartX, iComboStartY, iSecondLevelComboWidth, iSecondLevelComboHeight);

	 m_SampleRate = new FLabel(this);
	 m_SampleRate->setText(tr("Sample Rate:"));
	 iLabelYBeginpos = m_AuidoChanelCombo->geometry().bottom() + +iComboSpace + iSecondLevelComboHeight / 2;
	 m_SampleRate->setGeometry(0, iLabelYBeginpos, iSecondLevelLabelWidth, iSecondLevelLabelHeight);


	 m_SampleRateCombo = new FComboBox(this);
	 iComboStartY = m_AuidoChanelCombo->geometry().bottom() + iComboSpace;
	 m_SampleRateCombo->setGeometry(iComboStartX, iComboStartY, iSecondLevelComboWidth, iSecondLevelComboHeight);


	 m_AudioBitRate = new FLabel(this);
	 m_AudioBitRate->setText(tr("Bit Rate:"));
	 iLabelYBeginpos = m_SampleRateCombo->geometry().bottom() + +iComboSpace + iSecondLevelComboHeight / 2;
	 m_AudioBitRate->setGeometry(0, iLabelYBeginpos, iSecondLevelLabelWidth, iSecondLevelLabelHeight);

	 m_AudioBitRateCombo = new FComboBox(this);
	 iComboStartY = m_SampleRateCombo->geometry().bottom() + iComboSpace;
	 m_AudioBitRateCombo->setGeometry(iComboStartX, iComboStartY, iSecondLevelComboWidth, iSecondLevelComboHeight);

	 m_DefultBtn = new FButton(this);
	 m_DefultBtn->setText(tr("Defult"));
	 m_DefultBtn->setGeometry(7, 403 + iOffsetHeight, 90, 30);

	 m_OkBtn = new FButton(this);
	 m_OkBtn->setText(tr("OK"));
	 m_OkBtn->setGeometry(272, 403 + iOffsetHeight, 85, 30);

     m_CancleBtn = new FButton(this);
	 m_CancleBtn->setText(tr("Cancle"));
	 m_CancleBtn->setGeometry(360, 403 + iOffsetHeight, 85, 30);

	 m_titlebar->setObjectName("FParameterSetting_m_titlebar");
	 m_titlebar->GetButton(TopLevelWindowTitleBar::Button::Close)->setObjectName("FParameterSetting_mCloseBtn");
	 m_titlebar->SetTitleName(tr("Setting"));	 
}

void FParameterSettingWnd::initData()
{
	/*m_SupportVidioType = { "WMV", "MP4", "AVI", "MOV", "F4V", "MKV", "TS", "3GP", "MPEG-2", "WEBM", "GIF" };*/
	m_SupportVidioType<<"WMV" << "MP4" << "AVI" << "MOV" << "F4V" << "MKV" << "TS" << "3GP" << "MPEG-2" << "WEBM" << "GIF";
	//m_SupportAudioType = { "MP3" };
	m_SupportAudioType << "MP3";
    m_DataControl = new FEncodeDataControl(nullptr);
	m_CurType = FEncodeDataControl::Type_MOV;
	//todo:暂时使用mov类型做测试
	m_DefaultParameter = m_DataControl->GetDefaultParameter(FEncodeDataControl::Type_MOV);
	QString strDefVideoEncoder = m_DefaultParameter[FEncodeDataControl::Video_Encoder];
	LoadVideoEncoderComboBox(FEncodeDataControl::Type_MOV, strDefVideoEncoder);
	QString strDefVideoResolution = m_DefaultParameter[FEncodeDataControl::Video_Resolution];
	LoadVideoResolutionComboBox(FEncodeDataControl::Type_MOV, strDefVideoEncoder,strDefVideoResolution);
	QString strCurFrameRate = m_DefaultParameter[FEncodeDataControl::Video_FrameRate];
	QString strCurResolution = m_DefaultParameter[FEncodeDataControl::Video_Resolution];
	LoadVideoFrameRateComboBox(FEncodeDataControl::Type_MOV, strDefVideoEncoder, strCurResolution, strCurFrameRate);
	QString strCurBitate = m_DefaultParameter[FEncodeDataControl::Video_BitRate];
	LoadVideoBitRateComboBox(FEncodeDataControl::Type_MOV, strDefVideoEncoder, strCurResolution, strCurBitate);
	QString strCurAudioEncoder = m_DefaultParameter[FEncodeDataControl::Audio_Encoder];
	LoadAudioEncoderComboBox(FEncodeDataControl::Type_MOV, strCurAudioEncoder);
	QString strDefChannel = m_DefaultParameter[FEncodeDataControl::Audio_Channel];
	LoadAudioChannelComboBox(FEncodeDataControl::Type_MOV, strCurAudioEncoder, strDefChannel);
	QString strDefSampleRate = m_DefaultParameter[FEncodeDataControl::Audio_SampleRate];
	LoadAudioSampleRateComboBox(FEncodeDataControl::Type_MOV, strCurAudioEncoder, strDefChannel, strDefSampleRate);
	QString strDefAudioBitrate = m_DefaultParameter[FEncodeDataControl::Audio_BitRate];
	LoadAudioBitRateComboBox(FEncodeDataControl::Type_MOV, strCurAudioEncoder, strDefChannel, strDefAudioBitrate);
}

void FParameterSettingWnd::showParamWnd()
{
	if (m_IsVideo)
	{
		int iOffsetHeight = getHeaderHeight();
		m_Quality->setGeometry(20, 10 + iOffsetHeight, 50, 15);
		m_Quality->show();
		m_BestQuality->setGeometry(90, 23 + iOffsetHeight, 50, 23);
		m_BestQuality->show();
		m_BetterQuality->setGeometry(202, 23 + iOffsetHeight, 50, 23);
		m_BetterQuality->show();
		m_GoodQuality->setGeometry(320, 23 + iOffsetHeight, 50, 23);
		m_GoodQuality->show();
		m_Video->setGeometry(20, 50 + iOffsetHeight, 50, 23);
		m_Video->show();
		m_Encoder->setGeometry(0, 90 + iOffsetHeight, 132, 23);
		m_Encoder->show();
		m_EncoderCombo->setGeometry(140, 82 + iOffsetHeight, 173, 25);
		m_EncoderCombo->show();
		m_Resolution->setGeometry(0, 118 + iOffsetHeight, 132, 23);
		m_Resolution->show();
		m_ResolutionCombo->setGeometry(140, 110 + iOffsetHeight, 173, 25);
		m_ResolutionCombo->show();
		m_FrameRate->setGeometry(0, 145 + iOffsetHeight, 132, 23);
		m_FrameRate->show();
		m_FrameRateCombo->setGeometry(140, 140 + iOffsetHeight, 173, 25);
		m_FrameRateCombo->show();
		m_BitRate->setGeometry(0, 174 + iOffsetHeight, 132, 23);
		m_BitRate->show();
		m_BitRateCombo->setGeometry(140, 166 + iOffsetHeight, 173, 25);
		m_BitRateCombo->show();
		m_Audio->setGeometry(18, 222 + iOffsetHeight, 60, 25);
		m_Audio->show();
		m_AuidoEncoder->setGeometry(0, 260 + iOffsetHeight, 132, 23);
		m_AuidoEncoder->show();
		m_AudioEncoderCombo->setGeometry(140, 253 + iOffsetHeight, 173, 25);
		m_AudioEncoderCombo->show();
		m_AudioChanel->setGeometry(0, 290 + iOffsetHeight, 132, 23);
		m_AudioChanel->show();
		m_AuidoChanelCombo->setGeometry(140, 280 + iOffsetHeight, 173, 25);
		m_AuidoChanelCombo->show();
		m_SampleRate->setGeometry(0, 315 + iOffsetHeight, 132, 23);
		m_SampleRate->show();	
		m_SampleRateCombo->setGeometry(140, 309 + iOffsetHeight, 173, 25);
		m_SampleRateCombo->show();
		m_AudioBitRate->setGeometry(0, 345 + iOffsetHeight, 132, 23);
		m_AudioBitRate->show();
		m_AudioBitRateCombo->setGeometry(140, 335 + iOffsetHeight, 173, 25);
		m_AudioBitRateCombo->show();
		m_DefultBtn->setGeometry(7, 403 + iOffsetHeight, 90, 30);

		m_OkBtn->setGeometry(272, 403 + iOffsetHeight, 85, 30);

		m_CancleBtn->setGeometry(360, 403 + iOffsetHeight, 85, 30);
	}
}

void FParameterSettingWnd::LoadVideoEncoderComboBox(FEncodeDataControl::MediaType type, QString strDefParameter)
{
	QStringList encoderList;
	encoderList = m_DataControl->GetVideoEncoderList(type);
	m_EncoderCombo->insertItems(0, encoderList);
	//设置默认选中项
	m_EncoderCombo->setCurrentText(strDefParameter);
}

void FParameterSettingWnd::LoadVideoResolutionComboBox(FEncodeDataControl::MediaType type,QString strCurFcc, QString strDefParameter)
{
	QStringList resolutionList;
	resolutionList = m_DataControl->GetResolutionList(strCurFcc, type);
	m_ResolutionCombo->insertItems(0, resolutionList);
	m_ResolutionCombo->setCurrentText(strDefParameter);
}

void FParameterSettingWnd::LoadVideoFrameRateComboBox(FEncodeDataControl::MediaType type,QString strCurFcc,QString strCurResolution, QString strDefParameter)
{
	QStringList framrateList;
	framrateList = m_DataControl->GetVidioFramerateList(type,strCurFcc,strCurResolution);
	m_FrameRateCombo->insertItems(0, framrateList);
	m_FrameRateCombo->setCurrentText(strDefParameter);
}

void FParameterSettingWnd::LoadVideoBitRateComboBox(FEncodeDataControl::MediaType type,QString strCurFcc, QString currentResolution,QString strDefParameter)
{
	QStringList biteratelist;
	biteratelist =  m_DataControl->GetVidioBitrateList(type, strCurFcc, currentResolution);
	m_BitRateCombo->insertItems(0, biteratelist);
	m_BitRateCombo->setCurrentText(strDefParameter);
}

void FParameterSettingWnd::LoadAudioEncoderComboBox(FEncodeDataControl::MediaType type, QString strDefParameter)
{
	QStringList audioEncodeList;
	audioEncodeList = m_DataControl->GetAudioEncoderList(type);
	m_AudioEncoderCombo->insertItems(0, audioEncodeList);
	m_AudioEncoderCombo->setCurrentText(strDefParameter);
}

void FParameterSettingWnd::LoadAudioChannelComboBox(FEncodeDataControl::MediaType type, QString strCurFcc, QString strDefParameter)
{
	QStringList audioChannelList;
	audioChannelList = m_DataControl->GetAudioChannelList(strCurFcc, type);
	m_AuidoChanelCombo->insertItems(0, audioChannelList);
	m_AuidoChanelCombo->setCurrentText(strDefParameter);
}

void FParameterSettingWnd::LoadAudioSampleRateComboBox(FEncodeDataControl::MediaType type, QString strCurFcc, QString currentChannel,QString strDefParameter)
{
	QStringList audioSampleRateList;
	audioSampleRateList = m_DataControl->GetAudioSampleRateList(type, strCurFcc, currentChannel);
	m_SampleRateCombo->insertItems(0, audioSampleRateList);
	m_SampleRateCombo->setCurrentText(strDefParameter);
}

void FParameterSettingWnd::LoadAudioBitRateComboBox(FEncodeDataControl::MediaType type, QString currentAudiofCC, QString currentChannel,QString strDefParameter)
{
	QStringList biteRateList;
	biteRateList = m_DataControl->GetAudioBitrateList(type, currentAudiofCC, currentChannel);
	m_AudioBitRateCombo->insertItems(0,biteRateList);
	m_AudioBitRateCombo->setCurrentText(strDefParameter);
}

void FParameterSettingWnd::ConncetVideoEncoder()
{
	connect(m_EncoderCombo, &FComboBox::currentTextChanged, this, &FParameterSettingWnd::OnVideoEncoderSlot);
}

void FParameterSettingWnd::ConnectVideoResolution()
{
	connect(m_ResolutionCombo, &FComboBox::currentTextChanged, this, &FParameterSettingWnd::OnVideoResolutionSlot);
}

void FParameterSettingWnd::ConnectVideoFrameRate()
{
	connect(m_FrameRateCombo, &FComboBox::currentTextChanged, this, &FParameterSettingWnd::OnVideoFrameRateSlot);
}

void FParameterSettingWnd::ConncetVideoBitRate()
{
	connect(m_BitRateCombo, &FComboBox::currentTextChanged, this, &FParameterSettingWnd::OnVideoBitRateSlot);
}

void FParameterSettingWnd::ConnectAudioEncoder()
{
	connect(m_AudioEncoderCombo, &FComboBox::currentTextChanged, this, &FParameterSettingWnd::OnAudioEncoderSlot);
}

void FParameterSettingWnd::ConnectAudioChannel()
{
	connect(m_AuidoChanelCombo, &FComboBox::currentTextChanged, this, &FParameterSettingWnd::OnAudioChannelSlot);
}

void FParameterSettingWnd::ConnectAudioSampleRate()
{
	connect(m_SampleRateCombo, &FComboBox::currentTextChanged, this, &FParameterSettingWnd::OnAudioSampleRateSlot);
}

void FParameterSettingWnd::ConnectAudioBitRate()
{
	connect(m_AudioBitRateCombo, &FComboBox::currentTextChanged, this, &FParameterSettingWnd::OnAudioBitRateSlot);
}

void FParameterSettingWnd::ConnectDefaultParam()
{
	connect(m_DefultBtn, &FButton::clicked, this, &FParameterSettingWnd::OnClickDefaultBtn);
}

void FParameterSettingWnd::ConnectOKBtn()
{
	connect(m_OkBtn, &FButton::clicked, this, &FParameterSettingWnd::OnClickOKBtn);
}

void FParameterSettingWnd::ConnectCancelBtn()
{
	connect(m_CancleBtn, &FButton::clicked, this, &FParameterSettingWnd::OnClickCancleBtn);
}

void FParameterSettingWnd::OnVideoEncoderSlot(const QString &text)
{
	//重新加载所有配置
	QStringList curResolutionList = m_DataControl->GetResolutionList(text,m_CurType);
	m_ResolutionCombo->clear();
	m_ResolutionCombo->insertItems(0, curResolutionList);
	QString strDefResolution = m_DataControl->GetDefaultVideoResolution(m_CurType);
	m_ResolutionCombo->setCurrentText(strDefResolution);
	QStringList strFrameRateList = m_DataControl->GetVidioFramerateList(m_CurType, text, strDefResolution);
	m_FrameRateCombo->clear();
	m_FrameRateCombo->insertItems(0, strFrameRateList);
	QString strDefaultFrameRate = m_DataControl->GetDefaultVideoFramerate(m_CurType, text, strDefResolution);
	m_FrameRateCombo->setCurrentText(strDefaultFrameRate);
	QStringList strBitrateList = m_DataControl->GetVidioBitrateList(m_CurType, text, strDefResolution);
	m_BitRateCombo->clear();
	m_BitRateCombo->insertItems(0, strBitrateList);
	QString strDefaultBitrate = m_DataControl->GetDefaultVideoBitrate(m_CurType, text, strDefResolution);
	m_BitRateCombo->setCurrentText(strDefaultBitrate);
}

void FParameterSettingWnd::OnVideoResolutionSlot(const QString &text)
{
	m_ResolutionCombo->setCurrentText(text);
	QString strCurFcc = m_EncoderCombo->currentText();
	QStringList strFrameRateList = m_DataControl->GetVidioFramerateList(m_CurType, strCurFcc, text);
	m_FrameRateCombo->clear();
	m_FrameRateCombo->insertItems(0, strFrameRateList);
	QString strDefFrameRate = m_DataControl->GetDefaultVideoFramerate(m_CurType, strCurFcc, text);
	m_FrameRateCombo->setCurrentText(strDefFrameRate);
	QStringList strBitrateList = m_DataControl->GetVidioBitrateList(m_CurType, strCurFcc, text);
	m_BitRateCombo->clear();
	m_BitRateCombo->insertItems(0, strBitrateList);
	QString strDefaultBitrate = m_DataControl->GetDefaultVideoBitrate(m_CurType, strCurFcc, text);
	m_BitRateCombo->setCurrentText(strDefaultBitrate);
}

void FParameterSettingWnd::OnVideoFrameRateSlot(const QString &text)
{
	m_FrameRateCombo->setCurrentText(text);
}

void FParameterSettingWnd::OnVideoBitRateSlot(const QString& text)
{
	m_BitRateCombo->setCurrentText(text);
}

void FParameterSettingWnd::OnAudioEncoderSlot(const QString &text)
{
	m_AudioEncoderCombo->setCurrentText(text);
	QStringList strChannelList = m_DataControl->GetAudioChannelList(text,m_CurType);
	m_AuidoChanelCombo->clear();
	m_AuidoChanelCombo->insertItems(0, strChannelList);
	QString strDefChannel = m_DataControl->GetDefaultAudioChannel(m_CurType);
	m_AuidoChanelCombo->setCurrentText(strDefChannel);
	QStringList strAudioBitrateList = m_DataControl->GetAudioBitrateList(m_CurType, text, strDefChannel);
	m_AudioBitRateCombo->clear();
	m_AudioBitRateCombo->insertItems(0, strAudioBitrateList);
	QString strDefBitrate = m_DataControl->GetDefaultAudioBitrate(m_CurType, text, strDefChannel);
	m_AudioBitRateCombo->setCurrentText(strDefBitrate);
	QStringList strAudioSampleRateList = m_DataControl->GetAudioSampleRateList(m_CurType, text, strDefChannel);
	m_SampleRateCombo->clear();
	m_SampleRateCombo->insertItems(0,strAudioSampleRateList);
	QString strDefauleSampleRate = m_DataControl->GetDefaultAudioSampleRate(m_CurType, text, strDefChannel);
	m_SampleRateCombo->setCurrentText(strDefauleSampleRate);
}

void FParameterSettingWnd::OnAudioChannelSlot(const QString &text)
{
	QString strCurEncoder = m_AudioEncoderCombo->currentText();
	QStringList strAudioBitrateList = m_DataControl->GetAudioBitrateList(m_CurType, strCurEncoder, text);
	m_AudioBitRateCombo->clear();
	m_AudioBitRateCombo->insertItems(0, strAudioBitrateList);
	QString strDefBitrate = m_DataControl->GetDefaultAudioBitrate(m_CurType, strCurEncoder, text);
	m_AudioBitRateCombo->setCurrentText(strDefBitrate);
	QStringList strAudioSampleRateList = m_DataControl->GetAudioSampleRateList(m_CurType, strCurEncoder, text);
	m_SampleRateCombo->clear();
	m_SampleRateCombo->insertItems(0, strAudioSampleRateList);
	QString strDefauleSampleRate = m_DataControl->GetDefaultAudioSampleRate(m_CurType, strCurEncoder, text);
	m_SampleRateCombo->setCurrentText(strDefauleSampleRate);
}

void FParameterSettingWnd::OnAudioSampleRateSlot(const QString &text)
{

}

void FParameterSettingWnd::OnAudioBitRateSlot(const QString &text)
{

}

void FParameterSettingWnd::OnClickDefaultBtn()
{
	//设置默认的配置
	QString strDefVideoEncoder = m_DataControl->GetDefaultVideoFourcc(m_CurType);
	QString strDefAudioEncoder = m_DataControl->GetDefaultAudioFourcc(m_CurType);
	OnAudioEncoderSlot(strDefAudioEncoder);
	OnVideoEncoderSlot(strDefVideoEncoder);
}

void FParameterSettingWnd::OnClickOKBtn()
{
	//设置窗口数据到父窗口
	GetUserSelectParam();
	emit sigConfirmParameter(m_UserSelectParameter);
	close();
}

void FParameterSettingWnd::OnClickCancleBtn()
{
	//关闭窗口
	close();
}

void FParameterSettingWnd::GetUserSelectParam()
{
	QString strVidioEncoder = m_EncoderCombo->currentText();
	QString strVidioResolution = m_ResolutionCombo->currentText();
	QString strVidioFrameRate = m_FrameRateCombo->currentText();
	QString strVidioBitrate = m_BitRateCombo->currentText();
	QString strAudioEncoder = m_AudioEncoderCombo->currentText();
	QString strAudioChannel = m_AuidoChanelCombo->currentText();
	QString strAudioSampleRate = m_SampleRateCombo->currentText();
	QString strAudioBitrate = m_AudioBitRateCombo->currentText();
	QString strCurFourCC = m_DataControl->GetCurFourCC(m_CurType);
	QString strFileFormat = m_DataControl->GetCurFileFormat(m_CurType);
	m_UserSelectParameter.insert(VIDEO_ENCODER, strVidioEncoder);
	m_UserSelectParameter.insert(VIDEO_RESOLUTION, strVidioResolution);
	m_UserSelectParameter.insert(VIDEO_FRAMERATE, strVidioFrameRate);
	m_UserSelectParameter.insert(VIDEO_BITRATE, strVidioBitrate);
	m_UserSelectParameter.insert(AUDIO_ENCODER, strAudioEncoder);
	m_UserSelectParameter.insert(AUDIO_CHANNEL, strAudioChannel);
	m_UserSelectParameter.insert(AUDIO_SAMPLERATE, strAudioSampleRate);
	m_UserSelectParameter.insert(AUDIO_BITRATE, strAudioBitrate);
	m_UserSelectParameter.insert(FILE_FOURCC, strCurFourCC);
	m_UserSelectParameter.insert(FILE_FORMAT, strFileFormat);
}

void FParameterSettingWnd::OnMediaTypeChanged(QString strCurType)
{
	m_CurType = getMediaType(strCurType);
}

FEncodeDataControl::MediaType FParameterSettingWnd::getMediaType(QString strType)
{
	FEncodeDataControl::MediaType type = FEncodeDataControl::Type_MPEG2;
	int iIndex = 0;
	for (int i = 0; i < m_SupportVidioType.length(); ++i)
	{
		//比较前先清空空白字符
		QString strTypeTmp = strType;
		strTypeTmp = strTypeTmp.trimmed();
		QString strTmp = m_SupportVidioType[i];
		strTmp = strTmp.trimmed();
		if (!strTmp.compare(strTypeTmp, Qt::CaseInsensitive))
		{
			iIndex = i;
			m_SupportVidioType[i] = strType;
			type = (FEncodeDataControl::MediaType)iIndex;
			m_IsVideo = true;
			return type;
		}
	}

	for (int i = 0; i < m_SupportAudioType.length(); ++i)
	{
		//比较前先清空空白字符
		QString strTypeTmp = strType;
		strTypeTmp = strTypeTmp.trimmed();
		QString strTmp = m_SupportAudioType[i];
		strTmp = strTmp.trimmed();
		if (!strTmp.compare(strTypeTmp, Qt::CaseInsensitive))
		{
			iIndex = i;
			m_SupportAudioType[i] = strType;
			type = (FEncodeDataControl::MediaType)iIndex;
			m_IsVideo = false;
			return type;
		}
	}
	return type;
}
