#include "stdafx.h"

#include "FExportSettingWnd.h"
#include "FParameterSettingWnd.h"
#include "FSExportHelperManager.h"
#include "NLEWraper.h"
#include "NLETimeline.h"
#include "inc_FProject/FNLETimeline.h"

FExportSettingWnd::FExportSettingWnd(QWidget *parent)
	: FBaseWidget(parent)
{
	setAttribute(Qt::WA_StyledBackground);
	initUI();
	initData();
	initConnections();
}

FExportSettingWnd::~FExportSettingWnd()
{

}

void FExportSettingWnd::initUI()
{
	m_HeadTitle = new FLabel(this);
	m_HeadTitle->setText(tr("Output descriptions"));
	m_HeadTitle->setObjectName("FLabel_Output");
	m_HeadTitle->setGeometry(50, 20, 120, 20);

	m_ExportName = new FLabel(this);
	m_ExportName->setText(tr("Name:"));
	m_ExportName->setGeometry(80,50,45,20);

	m_ExportNameEdit = new FLineEdit(this);
	m_ExportNameEdit->setObjectName("FLabel_ExportName");
	m_ExportNameEdit->setGeometry(130,45,320,23);

	m_SavePath = new FLabel(this);
	m_SavePath->setText(tr("Save to:"));
	m_SavePath->setGeometry(75, 105, 46, 20);

	m_PathSelectEdit = new FPathSelectEdit(this);
	m_PathSelectEdit->setObjectName("FExportSettingWnd_PathSelectEdit");
	m_PathSelectEdit->setGeometry(130, 100, 320, 23);

	m_ResolutionLabel = new FLabel(this);
	m_ResolutionLabel->setText(tr("Resolution:"));
	m_ResolutionLabel->setGeometry(55,162,63,20);

	m_ResolutionValue = new FLabel(this);
	//具体值找control获取
	m_ResolutionValue->setGeometry(130, 160, 63, 20);

	m_SettingBtn = new FButton(this);
	m_SettingBtn->setText(tr("Settings"));
	m_SettingBtn->setObjectName("FButton_Setting");
	m_SettingBtn->setGeometry(350,153,100,30);

	m_ExportFileSize = new FLabel(this);
	m_ExportFileSize->setText(tr("Size:"));
	m_ExportFileSize->setGeometry(90, 210, 30, 20);

	m_ExportFileSizeValue = new FLabel(this);
	m_ExportFileSizeValue->setGeometry(132,210,30,20);

	m_ExportFileTime = new FLabel(this);
	m_ExportFileTime->setText(tr("Duration:"));
	m_ExportFileTime->setGeometry(62,262,55,20);

	m_ExportFileTimeValue = new FLabel(this);
	m_ExportFileTimeValue->setGeometry(130, 263, 60, 20);

	m_ExportBtn = new FButton(this);
	m_ExportBtn->setText(tr("Export"));
	m_ExportBtn->setObjectName("FExportSettingWnd_ExportBtn");
	m_ExportBtn->setGeometry(440, 485, 100, 30);

	//导出参数设置窗口
	m_exportSetting = new FParameterSettingWnd(nullptr);
}

void FExportSettingWnd::initConnections()
{
	connect(m_SettingBtn, &FButton::clicked, this, [&,this](){
		m_exportSetting->OnMediaTypeChanged(m_CurType);
		m_exportSetting->DoModal();
	});	
	connect(m_exportSetting, &FParameterSettingWnd::sigConfirmParameter, this, &FExportSettingWnd::slotOnConfirmSetting);

	connect(m_ExportBtn, &FButton::clicked, this, &FExportSettingWnd::slotOnExport);

	
}

void FExportSettingWnd::initData()
{
	m_ExportHelper = nullptr;
}

void FExportSettingWnd::slotOnItemSelect(QString strType)
{
	//更新界面
	m_CurType = strType;

}

void FExportSettingWnd::slotOnConfirmSetting(QMap<QString, QString> UserSelectParameter)
{
	QString strResolution = UserSelectParameter[VIDEO_RESOLUTION];
	m_ResolutionValue->setText(strResolution);
	m_ExportInfo.videofourcc = UserSelectParameter[VIDEO_ENCODER];
	QString strBitrate = UserSelectParameter[VIDEO_BITRATE];
	m_ExportInfo.videobitrate = getVedioBitrate(strBitrate);
	//帧率需要使用浮点数表示
	m_ExportInfo.videoframerate = getVedioFramerate(UserSelectParameter[VIDEO_FRAMERATE]);
	m_ExportInfo.resolution = strResolution;
	m_ExportInfo.audiobitrate = getAudioBitrate(UserSelectParameter[AUDIO_BITRATE]);
	m_ExportInfo.audiochannel = UserSelectParameter[AUDIO_CHANNEL].toInt();
	m_ExportInfo.audiofourcc = UserSelectParameter[AUDIO_ENCODER];
	m_ExportInfo.audiosamplerate = getAudioSampleRate(UserSelectParameter[AUDIO_SAMPLERATE]);
	m_ExportInfo.filefourcc = UserSelectParameter[FILE_FOURCC];
	QString strExportFilePath = m_PathSelectEdit->text();
	QString strExportFileName = m_ExportNameEdit->text();
	QString strExportFileFormat = UserSelectParameter[FILE_FORMAT];
	m_ExportInfo.filename = strExportFilePath + "/" +strExportFileName + strExportFileFormat;
}

#include "inc_FProject/FNLETimelineFactory.h"
void FExportSettingWnd::slotOnExport()
{
	//模拟数据
	QString strFilePath = "C:\\Users\\ws\\Videos\\king.mp4";
	NLEComPtr<INLETimelineBase> pEmptyTimeline = NLEWraper::GetInstance().GetPlayerMaterProvider()->CreateEmptyTimeline();
	Q_ASSERT(pEmptyTimeline);
	NLEComPtr<INLETimelineBase> pTimeline = NLEWraper::GetInstance().GetPlayerMaterProvider()->CreateMediaTimeline(strFilePath);
	Q_ASSERT(pTimeline);
	pEmptyTimeline->AddClip(pTimeline);
	QList<NLEComPtr<FNLETimeline>> list;
	//list.push_back(new FNLETimeline(pEmptyTimeline));
    list.push_back(FNLETimelineFactory::GetInstance()->CreateTimeline(pEmptyTimeline));


////////////////////////////////////////////////////////////////
	if (m_ExportHelper == nullptr)
	{
		m_ExportHelper = new FSExportHelperManager(list, m_ExportInfo, this);
		connect(m_ExportHelper, &FSExportHelperManager::sigExportState, this, &FExportSettingWnd::slotOnExportStateChange);
		m_ExportHelper->Start();
	}
}

void FExportSettingWnd::slotOnExportStateChange(FSExportHelper::FSEXPORT_STATE eState, int nProgress)
{
	qDebug() << "export state is : " << eState << "  Export progress is : " << nProgress;
}

int FExportSettingWnd::getVedioBitrate(QString strBitrate)
{
	int iIndex = strBitrate.indexOf("kbps");
	if (iIndex != -1)
	{
		strBitrate = strBitrate.mid(0, iIndex);
	}
	return strBitrate.toInt()*1000;
}

float FExportSettingWnd::getVedioFramerate(QString strFramerate)
{
	int iIndex = strFramerate.indexOf("fps");
	if (iIndex != -1)
	{
		strFramerate = strFramerate.mid(0, iIndex);
	}
	return strFramerate.toInt();
}

int FExportSettingWnd::getAudioBitrate(QString strBitrate)
{
	int iIndex = strBitrate.indexOf("kbps");
	if (iIndex != -1)
	{
		strBitrate = strBitrate.mid(0, iIndex);
	}
	return strBitrate.toInt() * 1000;
}

int FExportSettingWnd::getAudioSampleRate(QString strSamplerate)
{
	int iIndex = strSamplerate.indexOf("Hz");
	if (iIndex != -1)
	{
		strSamplerate = strSamplerate.mid(0, iIndex);
	}
	return strSamplerate.toInt();
}

void FExportSettingWnd::paintEvent(QPaintEvent *event)
{
	FBaseWidget::paintEvent(event);
}
