#include "stdafx.h"
#include "FSOptionWidget.h"
#include "FSFormatDataWidget.h"

#include "inc_CommonLib/FileOperation.h"
#include "inc_CommonLib/XmlOperation.h"

#include "inc_FSCommonlib/FSCommonWidget.h"
#include "inc_FSCommonlib/ExternalAPI.h"
#include "FSExportSettingData.h"

#ifdef WIN32
#include "shlobj.h"
#endif
#include "inc_FSCommonlib/gathread.h"

FSOptionWidget::FSOptionWidget(QWidget *parent, FSFormatDataWidget *pDataWidget)
    : QWidget(parent)
    , mbRecorderState(true)
    , mbVideoParamVisible(true)
    , mpDataFormatWidget(pDataWidget)
    , m_DefaultFileName(tr("My Video"))
{
    Q_ASSERT(pDataWidget);
    CreateUI();
    TranslateUI();
    ConnectSignals();
}

FSOptionWidget::~FSOptionWidget()
{

}


void FSOptionWidget::CheckUserInput()
{
    if (mpLineEditName->text().trimmed().isEmpty())
    {
        mpLineEditName->setText(m_DefaultFileName);
    }
}

QString FSOptionWidget::GetFilePathName()
{
    CheckUserInput();
    QString path = mpLineEditPath->text();
    QString name = mpLineEditName->text();
    path = QDir::toNativeSeparators(path);
    path = path.split(QDir::separator(), QString::SkipEmptyParts).join(QDir::separator());
    return QString("%1%2%3").arg(path).arg(QDir::separator()).arg(name);
}

void FSOptionWidget::SetRecorderExportState(bool bRecorderExport)
{
    mbRecorderState = bRecorderExport;
    mpLabelName->setVisible(!mbRecorderState);
    mpLineEditName->setVisible(!mbRecorderState);
    resizeEvent(NULL);
}

void FSOptionWidget::SetExportFileName(QString name)
{
    if (!name.trimmed().isEmpty())
    {
        m_DefaultFileName = name;
        mpLineEditName->setText(m_DefaultFileName);
    }
        
}

void FSOptionWidget::OnCurrentRowChangedSlot(int index)
{
    Q_ASSERT(sender());
    if (index >= 0)
    {
        QString currenEncoder = mpDataFormatWidget->GetCurrentText(index, FSFormatDataWidget::Video_Encoder);
        QString currentResolution = mpDataFormatWidget->GetCurrentText(index, FSFormatDataWidget::Video_Resolution);
        QString currentFrameRate = mpDataFormatWidget->GetCurrentText(index, FSFormatDataWidget::Video_FrameRate);
        QString currentBitRate = mpDataFormatWidget->GetCurrentText(index, FSFormatDataWidget::Video_BitRate);
        QString currentAudiofCC = mpDataFormatWidget->GetCurrentText(index, FSFormatDataWidget::Audio_Encoder);
        QString currentChannel = mpDataFormatWidget->GetCurrentText(index, FSFormatDataWidget::Audio_Channel);
        QString currentSampleRate = mpDataFormatWidget->GetCurrentText(index, FSFormatDataWidget::Audio_SampleRate);
        QString currentAudioBitRate = mpDataFormatWidget->GetCurrentText(index, FSFormatDataWidget::Audio_BitRate);
        SetVideoEncoderComboBox(mpDataFormatWidget->GetVideoEncoderList(index), currenEncoder);
        SetVideoResolutionComboBox(mpDataFormatWidget->GetResolutionList(currenEncoder, index), currentResolution);
        SetVideoFrameRateComboBox(mpDataFormatWidget->GetFramerateListWithFccAndResolution(index, currenEncoder, currentResolution), currentFrameRate);
        SetVideoBitRateComboBox(mpDataFormatWidget->GetVideoEncParam(index, currentResolution).mVideoBitrates, currentBitRate);        
        SetAudioEncoderComboBox(mpDataFormatWidget->GetAudioEncoderList(index),currentAudiofCC);
        SetAudioChannelComboBox(mpDataFormatWidget->GetChannelList(currentAudiofCC, index), currentChannel);
        SetAudioBitRateComboBox(mpDataFormatWidget->GetAudioBitrateListWithFccAndChannel(index, currentAudiofCC, currentChannel), currentAudioBitRate);
        SetAudioSampleRateComboBox(mpDataFormatWidget->GetAudioEncParam(index, currentChannel).mAudioSamplerates, currentSampleRate);
        SetAudioParamOptionVisible(index != 2);
    }
}


void FSOptionWidget::ConncetVideoEncoder()
{
    connect(mpComboBoxVideoEncoder, &FSComboBox::currentTextChanged, this, &FSOptionWidget::OnVideoEncoderSlot);
}

void FSOptionWidget::ConnectVideoResolution()
{
    connect(mpComboBoxVideoResolution, &FSComboBox::currentTextChanged, this, &FSOptionWidget::OnVideoResolutionSlot);
}

void FSOptionWidget::ConnectVideoFrameRate()
{
    connect(mpComboBoxVideoFrameRate, &FSComboBox::currentTextChanged, this, &FSOptionWidget::OnVideoFrameRateSlot);
}

void FSOptionWidget::ConncetVideoBitRate()
{
    connect(mpComboBoxVideoBitRate, &FSComboBox::currentTextChanged, this, &FSOptionWidget::OnVideoBitRateSlot);
}

void FSOptionWidget::ConnectAudioEncoder()
{
    connect(mpComboBoxAudioEncoder, &FSComboBox::currentTextChanged,this,&FSOptionWidget::OnAudioEncoderSlot);
}

void FSOptionWidget::ConnectAudioChannel()
{
    connect(mpComboBoxAudioChannel, &FSComboBox::currentTextChanged, this, &FSOptionWidget::OnAudioChannelSlot);
}

void FSOptionWidget::ConnectAudioSampleRate()
{
    connect(mpComboBoxAudioSampleRate, &FSComboBox::currentTextChanged, this, &FSOptionWidget::OnAudioSampleRateSlot);
}

void FSOptionWidget::ConnectAudioBitRate()
{
    connect(mpComboBoxAudioBitRate, &FSComboBox::currentTextChanged, this, &FSOptionWidget::OnAudioBitRateSlot);
}


void FSOptionWidget::DisconncetVideoEncoder()
{
    disconnect(mpComboBoxVideoEncoder, &FSComboBox::currentTextChanged, this, &FSOptionWidget::OnVideoEncoderSlot);
}

void FSOptionWidget::DisconnectVideoResolution()
{
    disconnect(mpComboBoxVideoResolution, &FSComboBox::currentTextChanged, this, &FSOptionWidget::OnVideoResolutionSlot);
}

void FSOptionWidget::DisconnectVideoFrameRate()
{
    disconnect(mpComboBoxVideoFrameRate, &FSComboBox::currentTextChanged, this, &FSOptionWidget::OnVideoFrameRateSlot);
}

void FSOptionWidget::DisConncetVideoBitRate()
{
    disconnect(mpComboBoxVideoBitRate, &FSComboBox::currentTextChanged, this, &FSOptionWidget::OnVideoBitRateSlot);
}

void FSOptionWidget::DisconnectAudioEncoder()
{
    disconnect(mpComboBoxAudioEncoder, &FSComboBox::currentTextChanged, this, &FSOptionWidget::OnAudioEncoderSlot);
}

void FSOptionWidget::DisconnectAudioChannel()
{
    disconnect(mpComboBoxAudioChannel, &FSComboBox::currentTextChanged, this, &FSOptionWidget::OnAudioChannelSlot);
}

void FSOptionWidget::DisconnectAudioSampleRate()
{
    disconnect(mpComboBoxAudioSampleRate, &FSComboBox::currentTextChanged, this, &FSOptionWidget::OnAudioSampleRateSlot);
}

void FSOptionWidget::DisconnectAudioBitRate()
{
    disconnect(mpComboBoxAudioBitRate, &FSComboBox::currentTextChanged, this, &FSOptionWidget::OnAudioBitRateSlot);
}

void FSOptionWidget::OnVideoEncoderSlot(const QString &text)
{
    mpDataFormatWidget->SetCurrentText(text, FSFormatDataWidget::Video_Encoder);
    mpDataFormatWidget->CurrentRowChanged(mpDataFormatWidget->GetCurrentIndex());
}

void FSOptionWidget::OnVideoResolutionSlot(const QString &text)
{
    mpDataFormatWidget->SetCurrentText(text, FSFormatDataWidget::Video_Resolution);
    QString bitRate = mpDataFormatWidget->GetVideoEncParam(mpDataFormatWidget->GetCurrentIndex(), text).mDefVideoBitrate;
    mpDataFormatWidget->SetCurrentText(bitRate, FSFormatDataWidget::Video_BitRate);
    mpDataFormatWidget->CurrentRowChanged(mpDataFormatWidget->GetCurrentIndex());
}

void FSOptionWidget::OnVideoFrameRateSlot(const QString &text)
{
    mpDataFormatWidget->SetCurrentText(text, FSFormatDataWidget::Video_FrameRate);
    mpDataFormatWidget->CurrentRowChanged(mpDataFormatWidget->GetCurrentIndex());
}

void FSOptionWidget::OnVideoBitRateSlot(const QString& text)
{
    mpDataFormatWidget->SetCurrentText(text, FSFormatDataWidget::Video_BitRate);
    mpDataFormatWidget->CurrentRowChanged(mpDataFormatWidget->GetCurrentIndex());
}

void FSOptionWidget::OnAudioEncoderSlot(const QString &text)
{
    mpDataFormatWidget->SetCurrentText(text, FSFormatDataWidget::Audio_Encoder);
    mpDataFormatWidget->CurrentRowChanged(mpDataFormatWidget->GetCurrentIndex());
}

void FSOptionWidget::OnAudioChannelSlot(const QString &text)
{
    mpDataFormatWidget->SetCurrentText(text, FSFormatDataWidget::Audio_Channel);
    mpDataFormatWidget->CurrentRowChanged(mpDataFormatWidget->GetCurrentIndex());
}

void FSOptionWidget::OnAudioSampleRateSlot(const QString &text)
{
    mpDataFormatWidget->SetCurrentText(text, FSFormatDataWidget::Audio_SampleRate);
    mpDataFormatWidget->CurrentRowChanged(mpDataFormatWidget->GetCurrentIndex());
}

void FSOptionWidget::OnAudioBitRateSlot(const QString &text)
{
    mpDataFormatWidget->SetCurrentText(text, FSFormatDataWidget::Audio_BitRate);
    mpDataFormatWidget->CurrentRowChanged(mpDataFormatWidget->GetCurrentIndex());
}

void FSOptionWidget::OnSaveToClickedSlot()
{
    QString strPath=QFileDialog::getExistingDirectory(this, tr("Please select directory"), FSExportSettingDataMgr::GetInstance()->GetLocalSettingData()->GetLocalFilePath());
    if (!strPath.isEmpty())
    {
        mpLineEditPath->setText(strPath);
        mpLineEditPath->setCursorPosition(0);
        FSExportSettingDataMgr::GetInstance()->GetLocalSettingData()->SetLocalFilePath(strPath);
    }    
}

void FSOptionWidget::SetVideoParamOptionVisible(bool bvisible)
{
    mbVideoParamVisible = bvisible;
    mpLabelVideo->setVisible(bvisible);
    mpLabelVideoEncoder->setVisible(bvisible);
    mpLabelVideoResolution->setVisible(bvisible);
    mpLabelVideoFrameRate->setVisible(bvisible);
    mpLabelVideoBitRate->setVisible(bvisible);
    mpComboBoxVideoEncoder->setVisible(bvisible);
    mpComboBoxVideoResolution->setVisible(bvisible);
    mpComboBoxVideoFrameRate->setVisible(bvisible);
    mpComboBoxVideoBitRate->setVisible(bvisible);
    resizeEvent(NULL);
}

void FSOptionWidget::SetAudioParamOptionVisible(bool bvisible)
{
    mpLabelAudio->setVisible(bvisible);
    mpLabelAudioEncoder->setVisible(bvisible);
    mpLabelAudioChannel->setVisible(bvisible);
    mpLabelAudioSampleRate->setVisible(bvisible);
    mpLabelAudioBitRate->setVisible(bvisible);
    mpComboBoxAudioEncoder->setVisible(bvisible);
    mpComboBoxAudioChannel->setVisible(bvisible);
    mpComboBoxAudioSampleRate->setVisible(bvisible);
    mpComboBoxAudioBitRate->setVisible(bvisible);
}

void FSOptionWidget::SetVideoEncoderComboBox(const QStringList &listFcc, const QString &currentFcc)
{
    DisconncetVideoEncoder();
    mpComboBoxVideoEncoder->clear();
   
    if (currentFcc.isEmpty() || listFcc.isEmpty())
    {
        return;
    }
    int index = listFcc.indexOf(currentFcc);
    index = qMax(index, 0);
    mpComboBoxVideoEncoder->addItems(listFcc);
    mpComboBoxVideoEncoder->setCurrentIndex(index);
    mpDataFormatWidget->SetCurrentText(listFcc.at(index), FSFormatDataWidget::Video_Encoder);
    
    ConncetVideoEncoder();

}

void FSOptionWidget::SetVideoResolutionComboBox(const QStringList &listResolution, const QString &currentResolution)
{
    DisconnectVideoResolution();
    mpComboBoxVideoResolution->clear();
    if (currentResolution.isEmpty() || listResolution.isEmpty())
    {
        return;
    }
    int index = listResolution.indexOf(currentResolution);
    if (index < 0 && !listResolution.isEmpty())
    {
        mpDataFormatWidget->SetCurrentText(listResolution.at(0), FSFormatDataWidget::Video_Resolution);
    }
    index = qMax(index, 0);
    mpComboBoxVideoResolution->addItems(listResolution);
    mpComboBoxVideoResolution->setCurrentIndex(index);
    ConnectVideoResolution();
}

void FSOptionWidget::SetVideoFrameRateComboBox(const QStringList &listFrameRate, const QString &currentFrameRate)
{
    DisconnectVideoFrameRate();
    mpComboBoxVideoFrameRate->clear();
    if (currentFrameRate.isEmpty() || listFrameRate.isEmpty())
    {
        return;
    }
    int index = listFrameRate.indexOf(currentFrameRate);
    if (index < 0 && !listFrameRate.isEmpty())
    {
        mpDataFormatWidget->SetCurrentText(listFrameRate.at(0), FSFormatDataWidget::Video_FrameRate);
    }
    index = qMax(index, 0);
    mpComboBoxVideoFrameRate->addItems(listFrameRate);
    mpComboBoxVideoFrameRate->setCurrentIndex(index);
    ConnectVideoFrameRate();
}

void FSOptionWidget::SetVideoBitRateComboBox(const QStringList &listBitRate, const QString &currentBitRate)
{
    DisConncetVideoBitRate();
    mpComboBoxVideoBitRate->clear();
    if (currentBitRate.isEmpty() || listBitRate.isEmpty())
    {
        return;
    }
    int index = listBitRate.indexOf(currentBitRate);
    if (index < 0 && !listBitRate.isEmpty())
    {
        mpDataFormatWidget->SetCurrentText(listBitRate.at(0), FSFormatDataWidget::Video_BitRate);
    }
    index = qMax(index, 0);
    mpComboBoxVideoBitRate->addItems(listBitRate);
    mpComboBoxVideoBitRate->setCurrentIndex(index);
    ConncetVideoBitRate();
}

void FSOptionWidget::SetAudioEncoderComboBox(const QStringList &listFcc, const QString &currentFcc)
{
    DisconnectAudioEncoder();
    mpComboBoxAudioEncoder->clear();
    if (currentFcc.isEmpty() || listFcc.isEmpty())
    {
        return;
    }   
    int index = listFcc.indexOf(currentFcc);
    if (index < 0)
    {
        mpDataFormatWidget->SetCurrentText(listFcc.at(0), FSFormatDataWidget::Audio_Encoder);
    }
    index = qMax(index, 0);
    mpComboBoxAudioEncoder->addItems(listFcc);
    mpComboBoxAudioEncoder->setCurrentIndex(index);
    ConnectAudioEncoder();
}

void FSOptionWidget::SetAudioChannelComboBox(const QStringList &listChannel, const QString &currentChannel)
{
    DisconnectAudioChannel();
    mpComboBoxAudioChannel->clear();
    if (currentChannel.isEmpty() || listChannel.isEmpty())
    {
        return;
    }
    int index = listChannel.indexOf(currentChannel);
    if (index < 0 && !listChannel.isEmpty())
    {
        mpDataFormatWidget->SetCurrentText(listChannel.at(0), FSFormatDataWidget::Audio_Channel);
    }
    index = qMax(index, 0);
    mpComboBoxAudioChannel->addItems(listChannel);
    mpComboBoxAudioChannel->setCurrentIndex(index);
    ConnectAudioChannel();
}

void FSOptionWidget::SetAudioSampleRateComboBox(const QStringList &listSampleRate, const QString &currentSampleRate)
{
    DisconnectAudioSampleRate();
    mpComboBoxAudioSampleRate->clear();
    if (currentSampleRate.isEmpty() || listSampleRate.isEmpty())
    {
        return;
    }
    int index = listSampleRate.indexOf(currentSampleRate);
    if (index < 0 && !listSampleRate.isEmpty())
    {
        mpDataFormatWidget->SetCurrentText(listSampleRate.at(0), FSFormatDataWidget::Audio_SampleRate);
    }
    index = qMax(index, 0);
    mpComboBoxAudioSampleRate->addItems(listSampleRate);
    mpComboBoxAudioSampleRate->setCurrentIndex(index);
    ConnectAudioSampleRate();
}

void FSOptionWidget::SetAudioBitRateComboBox(const QStringList &listBitRate, const QString &currentBitRate)
{
    DisconnectAudioBitRate();
    mpComboBoxAudioBitRate->clear();
    if (currentBitRate.isEmpty() || listBitRate.isEmpty())
    {
        return;
    }
    int index = listBitRate.indexOf(currentBitRate);
    if (index < 0 && !listBitRate.isEmpty())
    {
        mpDataFormatWidget->SetCurrentText(listBitRate.at(0), FSFormatDataWidget::Audio_BitRate);
    }
    index = qMax(index, 0);
    mpComboBoxAudioBitRate->addItems(listBitRate);
    mpComboBoxAudioBitRate->setCurrentIndex(index);
    ConnectAudioBitRate();
}

void FSOptionWidget::resizeEvent(QResizeEvent *e)
{
    int lineEditW = 300;
    int btnW = 60;
    int vSpacing = 10;
    int hSpacing = 8;
    int leftMargin = 38;
    int topMargin  = 38;
    
    int w          = 160;
    int h          = 25;
    int boxh = 30;
    int offSet     = 54;
    

    mpLabelDescription->setGeometry(leftMargin, topMargin, w, h);

    int wTemp = w - 72;
    int w1 = GetRetranslateUiTextLenght(mpLabelName->font(), mpLabelName->text(), wTemp);
    int w2 = GetRetranslateUiTextLenght(mpLabelSaveTo->font(), mpLabelSaveTo->text(), wTemp);
    wTemp = qMax(w1, wTemp);
    wTemp = qMax(w2, wTemp);

    mpLabelName->setGeometry(mpLabelDescription->geometry().left(), mpLabelDescription->geometry().bottom() + 20, wTemp, boxh);
    mpLineEditName->setGeometry(mpLabelName->geometry().right() + 20, mpLabelName->geometry().top(), lineEditW, boxh);

    topMargin = mbRecorderState ? mpLabelDescription->geometry().bottom() + 20 : mpLabelName->geometry().bottom() + 20;

    mpLabelSaveTo->setGeometry(mpLabelDescription->geometry().left(), topMargin, wTemp, boxh);
    mpLineEditPath->setGeometry(mpLabelSaveTo->geometry().right() + 20, mpLabelSaveTo->geometry().top(), lineEditW-29, boxh);
    mpButtonSave->setGeometry(mpLineEditPath->geometry().right(), mpLineEditPath->geometry().top(), 29, boxh);

    const int mpLbExtraWidth = 36;
    mpLabelVideo->setGeometry(leftMargin, topMargin + 49, w, boxh);
    mpLabelVideoEncoder->setGeometry(leftMargin + offSet, mpLabelVideo->geometry().bottom() + 1, w + mpLbExtraWidth, h);
    mpLabelVideoResolution->setGeometry(mpLabelVideoEncoder->geometry().right(), mpLabelVideoEncoder->geometry().top(), w + mpLbExtraWidth, h);
    mpComboBoxVideoEncoder->setGeometry(mpLabelVideoEncoder->geometry().left(), mpLabelVideoEncoder->geometry().bottom() + 4, w, boxh);
    mpComboBoxVideoResolution->setGeometry(mpLabelVideoResolution->geometry().left(), mpLabelVideoResolution->geometry().bottom() + 4, w, boxh);

    mpLabelVideoFrameRate->setGeometry(mpComboBoxVideoEncoder->geometry().left(), mpComboBoxVideoEncoder->geometry().bottom() + 3, w + mpLbExtraWidth, h);
    mpComboBoxVideoFrameRate->setGeometry(mpLabelVideoFrameRate->geometry().left(), mpLabelVideoFrameRate->geometry().bottom() + 4, w, boxh);
    mpLabelVideoBitRate->setGeometry(mpLabelVideoFrameRate->geometry().right(), mpLabelVideoFrameRate->geometry().top(), w + mpLbExtraWidth, h);
    mpComboBoxVideoBitRate->setGeometry(mpLabelVideoBitRate->geometry().left(), mpLabelVideoBitRate->geometry().bottom() + 4, w, boxh);

    topMargin = mbVideoParamVisible ? mpComboBoxVideoFrameRate->geometry().bottom() + 30 : mpLabelSaveTo->geometry().bottom() + 30;
    mpLabelAudio->setGeometry(leftMargin, topMargin, w, boxh);
    mpLabelAudioEncoder->setGeometry(leftMargin + offSet, mpLabelAudio->geometry().bottom() + 1, w + mpLbExtraWidth, h);
    mpComboBoxAudioEncoder->setGeometry(mpLabelAudioEncoder->geometry().left(), mpLabelAudioEncoder->geometry().bottom() + 4, w, boxh);
    mpLabelAudioChannel->setGeometry(mpLabelAudioEncoder->geometry().right(), mpLabelAudioEncoder->geometry().top(), w + mpLbExtraWidth, h);
    mpComboBoxAudioChannel->setGeometry(mpLabelAudioChannel->geometry().left(), mpLabelAudioChannel->geometry().bottom() + 4, w, boxh);
    mpLabelAudioSampleRate->setGeometry(mpComboBoxAudioEncoder->geometry().left(), mpComboBoxAudioEncoder->geometry().bottom() + 3, w + mpLbExtraWidth, h);
    mpComboBoxAudioSampleRate->setGeometry(mpLabelAudioSampleRate->geometry().left(), mpLabelAudioSampleRate->geometry().bottom() + 4, w, boxh);
    mpLabelAudioBitRate->setGeometry(mpLabelAudioSampleRate->geometry().right(), mpLabelAudioSampleRate->geometry().top(), w + mpLbExtraWidth, h);
    mpComboBoxAudioBitRate->setGeometry(mpLabelAudioBitRate->geometry().left(), mpLabelAudioBitRate->geometry().bottom() + 4, w, boxh);

}



void FSOptionWidget::CreateUI()
{
    mpLabelDescription = new FSLabel(this);
    mpLabelName        = new FSLabel(this);
    mpLabelSaveTo      = new FSLabel(this);
    mpLineEditName     = new FSLineEdit("FSOptionWidget_LineEditName",this);
    mpLineEditPath     = new FSLineEdit("FSOptionWidget_LineEditPath", this);
	mpLineEditPath->setReadOnly(true);

    mpButtonSave       = new FSButton(this);

    mpLabelVideo              = new FSLabel(this);
    mpLabelVideoEncoder       = new FSLabel(this);
    mpLabelVideoResolution    = new FSLabel(this);
    mpLabelVideoFrameRate     = new FSLabel(this);
    mpLabelVideoBitRate       = new FSLabel(this);
    mpComboBoxVideoEncoder    = new FSComboBox("FSOptionWidget_ComboBoxVideoEncoder", this);
    mpComboBoxVideoResolution = new FSComboBox("FSOptionWidget_ComboBoxVideoResolution", this);
    mpComboBoxVideoFrameRate  = new FSComboBox("FSOptionWidget_ComboBoxVideoFrameRate",this);
    mpComboBoxVideoBitRate    = new FSComboBox("FSOptionWidget_ComboBoxVideoBitRate", this);

    mpLabelAudio              = new FSLabel(this);
    mpLabelAudioEncoder       = new FSLabel(this);
    mpLabelAudioChannel       = new FSLabel(this);
    mpLabelAudioSampleRate    = new FSLabel(this);
    mpLabelAudioBitRate       = new FSLabel(this);
    mpComboBoxAudioEncoder    = new FSComboBox("FSOptionWidget_ComboBoxAudioEncoder", this);
    mpComboBoxAudioChannel    = new FSComboBox("FSOptionWidget_ComboBoxAudioChannel", this);
    mpComboBoxAudioSampleRate = new FSComboBox("FSOptionWidget_ComboBoxAudioSampleRate", this);
    mpComboBoxAudioBitRate    = new FSComboBox("FSOptionWidget_ComboBoxAudioBitRate", this);


    QRegExp regx("[^\\*\\$\\!\\(\\)\\{\\}\\[\\]\\,\\|\\^\\<\\>\\(\\)\\-\\=\\+\\&\\~\\`\\#\\@\\\\%\\?\\/\\:\\;\\'\\\"]+");
    QValidator *validator = new QRegExpValidator(regx, this);
    mpLineEditName->setValidator(validator);
    setAttribute(Qt::WA_StyledBackground);
    mpLabelName->setVisible(false);
    mpLineEditName->setVisible(false);
    mpLineEditName->setMaxLength(80);
    mpLabelDescription->setObjectName("FSOptionWidget_LabelDescription");
    mpLabelName->setObjectName("FSOptionWidget_LabelName");
    mpLabelSaveTo->setObjectName("FSOptionWidget_LabelSaveTo");
    mpButtonSave->setObjectName("FSOptionWidget_ButtonSave");

    mpLabelVideo->setObjectName("FSOptionWidget_LabelVideo");
    mpLabelVideoEncoder->setObjectName("FSOptionWidget_LabelVideoEncoder");
    mpLabelVideoResolution->setObjectName("FSOptionWidget_LabelVideoResolution");
    mpLabelVideoFrameRate->setObjectName("FSOptionWidget_LabelVideoFrameRate");
    mpLabelVideoBitRate->setObjectName("FSOptionWidget_LabelVideoBitRate");
    mpLabelAudioChannel->setObjectName("FSOptionWidget_LabelAudioChannel");
    mpLabelAudioSampleRate->setObjectName("FSOptionWidget_LabelAudioSampleRate");
    mpLabelAudioBitRate->setObjectName("FSOptionWidget_LabelAudioBitRate");
    mpLabelAudio->setObjectName("FSOptionWidget_LabelAudio");
    mpLabelAudioEncoder->setObjectName("FSOptionWidget_LabelAudioEncoder");
    setObjectName("FSOptionWidget");
    mpLineEditPath->setText(FSExportSettingDataMgr::GetInstance()->GetLocalSettingData()->GetLocalFilePath());
    mpLineEditPath->setCursorPosition(0);

}

void FSOptionWidget::TranslateUI()
{
    mpLabelDescription->setText(tr("Output Description"));
    mpLabelDescription->setAlignment( Qt::AlignLeft | Qt::AlignVCenter);

    mpLabelName->setText(tr("Name:"));
    mpLabelName->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    mpLabelSaveTo->setText(tr("Save to:"));
    mpLabelSaveTo->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    mpLabelVideo->setText(tr("Video"));
    mpLabelVideo->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    mpLabelVideoEncoder->setText(tr("Encoder"));
    mpLabelVideoEncoder->setAlignment(Qt::AlignLeft | Qt::AlignBottom);

    mpLabelVideoResolution->setText(tr("Resolution"));
    mpLabelVideoResolution->setAlignment(Qt::AlignLeft | Qt::AlignBottom);

    mpLabelVideoFrameRate->setText(tr("FrameRate"));
    mpLabelVideoFrameRate->setAlignment(Qt::AlignLeft | Qt::AlignBottom);

    mpLabelVideoBitRate->setText(tr("BitRate"));
    mpLabelVideoBitRate->setAlignment(Qt::AlignLeft | Qt::AlignBottom);

    mpLabelAudio->setText(tr("Audio"));
    mpLabelAudio->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    mpLabelAudioEncoder->setText(tr("Encoder"));
    mpLabelAudioEncoder->setAlignment(Qt::AlignLeft | Qt::AlignBottom);

    mpLabelAudioChannel->setText(tr("Channel"));
    mpLabelAudioChannel->setAlignment(Qt::AlignLeft | Qt::AlignBottom);

    mpLabelAudioSampleRate->setText(tr("SampleRate"));
    mpLabelAudioSampleRate->setAlignment(Qt::AlignLeft | Qt::AlignBottom);

    mpLabelAudioBitRate->setText(tr("BitRate"));
    mpLabelAudioBitRate->setAlignment(Qt::AlignLeft | Qt::AlignBottom);

    mpLineEditName->setText(tr("My Video"));

    resizeEvent(nullptr);

}

void FSOptionWidget::ConnectSignals()
{
    connect(mpDataFormatWidget, &FSFormatDataWidget::CurrentRowChanged, this, &FSOptionWidget::OnCurrentRowChangedSlot);
    connect(mpButtonSave, &FSButton::clicked, this, &FSOptionWidget::OnSaveToClickedSlot);
    connect(mpLineEditName, &QLineEdit::editingFinished, this, [&]()
    {
        if (mpLineEditName->text().trimmed().isEmpty())
        {
            CheckUserInput();
        }
    });
    ConnectAudioEncoder();
    ConnectVideoFrameRate();
    ConncetVideoEncoder();
    ConnectVideoResolution();
}
