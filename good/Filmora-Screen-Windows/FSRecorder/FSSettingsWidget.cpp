#include "stdafx.h"
#include "FSSettingsWidget.h"
#include "SettingsDataManager.h"

#include "inc_FSCommonlib/VolumeSlider.h"

#include "inc_MKCollecter/MKCollecter.h"

#include "FSRecorder.h"
#include "inc_CommonLib/TopLevelWindowTitleBar.h"
#include "inc_FSCommonlib/GlobalDefine.h"
#include "Communicator.h"
#include "inc_CommonLib/StringOperation.h"
#include "PrimaryScreenManager.h"
#include "RecorderViewsMgr.h"

bool operator == (const CaptureDeviceInfo &l, const CaptureDeviceInfo &r);

static bool sIsKeyControl(int key)
{
	/*
	case Qt::Key_Shift: text = "Shift"; break;
	case Qt::Key_Control: text = "Ctrl"; break;
	case Qt::Key_Alt: text = "Alt"; break;
	*/
	if (key == Qt::Key_Shift || key == Qt::Key_Control || key == Qt::Key_Alt)
	{
		return true;
	}
	return false;
}

static int sComboBoxItemDataIndexFloat(QComboBox *cmb, float value)
{
	for (int i = 0; i < cmb->count(); ++i)
	{
		auto v = cmb->itemData(i).toFloat();
		if (qFuzzyCompare(v, value))
			return i;
	}
	return -1;
}

static int sTop = 40;
static const int sPanelMargins = 80;
static const int sPanelComboBoxHeight = 30;

FSSettingsWidget::FSSettingsWidget(QWidget *parent)
: QWidget(parent)
{
	resize(870, 380);

	m_bar = new SettingsWidgetBar(this);
	m_stackWidget = new QStackedWidget(this);

	connect(m_bar, &SettingsWidgetBar::signalPanelChanged, this, &FSSettingsWidget::PanelChanged);
	connect(m_bar, &SettingsWidgetBar::signalCaptureEnabled, this, &FSSettingsWidget::signalCaptureEnabled);

	auto CreatePanelFunc = [&](QString iconObjectName, QString trtext, const char *text, SettingsBasePanel *panel, bool haveCheckBox)
	{
		m_bar->AddWidget(iconObjectName, trtext, text, panel, haveCheckBox);
		m_stackWidget->addWidget(panel);

	};
	CreatePanelFunc("FSSettingsWidget_Screen", tr("Screen"), "Screen", new SettingsScreenPanel(this), true);
	CreatePanelFunc("FSSettingsWidget_Audio", tr("Audio"), "Audio", new SettingsAudioPanel(this), true);
	CreatePanelFunc("FSSettingsWidget_Camera", tr("Camera"), "Camera", new SettingsCameraPanel(this), true);
	CreatePanelFunc("FSSettingsWidget_Advanced", tr("Advanced"), "Advanced", new SettingsAdvancedPanel(this), false);
	m_bar->SetSelected(0, true);

}

FSSettingsWidget::~FSSettingsWidget()
{

}



bool FSSettingsWidget::GetCaptureEnabled()
{
	return m_bar->GetCaptureEnabled();
}

void FSSettingsWidget::CreatePanel(QString iconObjectName, QString trtext, const char *text, SettingsBasePanel *panel)
{
	m_bar->AddWidget(iconObjectName, trtext, text, panel, true);
	m_stackWidget->addWidget(panel);

}



void FSSettingsWidget::PanelChanged(SettingsBasePanel *panel)
{
	m_stackWidget->setCurrentWidget(panel);
}


void FSSettingsWidget::resizeEvent(QResizeEvent *event)
{

	int left = 0;
	int top = 0;

	const int barWidget = 285;

	m_bar->setGeometry(left, top, barWidget, height() - top);
	left = m_bar->geometry().right();
	m_stackWidget->setGeometry(barWidget, top, width() - barWidget, height() - top);

}



void FSSettingsWidget::showEvent(QShowEvent *event)
{
	auto mgr = SettingsDataManager::GetInstance();
	m_bar->SetChecked(1, mgr->GetAudioChecked());
	m_bar->SetChecked(2, mgr->GetCameraChecked() && mgr->GetCameraData()->GetDeviceSelected());
	m_bar->SetChecked(0, mgr->GetScreenChecked());

	m_bar->SetEnabled(2, mgr->GetCameraData()->GetDeviceSelected());
}

SettingsWidgetBar::SettingsWidgetBar(QWidget *parent)
:FSBaseWidget(parent)
, m_height(70)
{
	m_curWidget = nullptr;
    setObjectName("SettingsWidgetBar");

}


void SettingsWidgetBar::AddWidget(QString iconObjectName, QString trtext, const char *text, SettingsBasePanel *widget, bool haveCheckbox)
{
	Q_ASSERT(widget);

	if (m_curWidget == nullptr)
		m_curWidget = widget;

	auto item = new SettingsWidgetBarItem(iconObjectName, text, widget, haveCheckbox, this);
	m_items.push_back(item);
	connect(item, &SettingsWidgetBarItem::signalClicked, this, [&](SettingsBasePanel *panel){
		m_curWidget = panel;
		emit signalPanelChanged(m_curWidget);

		for (auto temp : m_items)
		{
			temp->SetSelected(temp == sender());
		}
	});

	connect(item, &SettingsWidgetBarItem::signalCheckBoxChanged, this, [&](bool checked){

		emit signalCaptureEnabled(GetCaptureEnabled());
	});


}

SettingsBasePanel* SettingsWidgetBar::GetCurrentWidget()
{
	return m_curWidget;
}

bool SettingsWidgetBar::GetCaptureEnabled()
{
	bool enalbed = false;
	for (auto temp : m_items)
	{
		enalbed |= temp->IsChecked();
	}
	return enalbed;
}

void SettingsWidgetBar::SetItemHeight(int height)
{
    m_height = height;
}

//void SettingsWidgetBar::SetCurrentWidget(SettingsBasePanel *widget)
//{
//	m_curWidget = widget;
//	emit PanelChanged(widget);
//}

void SettingsWidgetBar::SetChecked(int index, bool checked)
{
	Q_ASSERT(index < m_items.size());
	m_items[index]->SetChecked(checked);
}

void SettingsWidgetBar::SetSelected(int index, bool selected)
{
	Q_ASSERT(index < m_items.size());
	for (int i = 0; i < m_items.size(); ++i)
	{
		m_items[i]->SetSelected(i == index);
	}
}

void SettingsWidgetBar::SetEnabled(int index, bool enabled)
{
	Q_ASSERT(index < m_items.size());
	m_items[index]->SetEnabled(enabled);
}

void SettingsWidgetBar::resizeEvent(QResizeEvent *event)
{
	const int spacing = 10;
	int top = 0;
    int height = m_items.size() == 0 ? 90 : m_height;
	for (auto *w : m_items)
	{
		w->setGeometry(0, top, width(), height);
		top = w->geometry().bottom() + spacing;
	}
}
//

//////////////////////////////////////////////////////////////////////////
SettingsWidgetBarItem::SettingsWidgetBarItem(QString iconObjectName, const char *text, SettingsBasePanel *widget, bool haveCheckbox, QWidget *parent)
:FSBaseWidget(parent)
, m_selected(false)
{
    // 用于多语言
    QString temp = tr("Screen");
    QString temp2 = tr("Audio");
    QString temp3 = tr("Camera");
    QString temp4 = tr("Advanced");

	setObjectName("SettingsWidgetBarItem_SelectedItem");
	m_panel = widget;
	connect(m_panel, &SettingsBasePanel::signalPanelEnabled, this, [&](bool enalbed){

		SetEnabled(enalbed);
	});
	;
	connect(m_panel, &SettingsBasePanel::signalPanelSetChekced, this, [&](bool checked){
		SetChecked(checked);
		emit signalCheckBoxChanged(checked);

	});

	m_ckb = new QCheckBox(this);
	m_ckb->setVisible(haveCheckbox);
	connect(m_ckb, &QCheckBox::toggled, this, [&](bool checked){
		emit signalClicked(m_panel);
		emit signalCheckBoxChanged(checked);
		m_panel->SetPanelEnabled(checked, true);
		gSetContentChanged();

        m_icon->setChecked(checked);
        m_title->setChecked(checked);
	});

	m_icon = new FSButton(this);
    m_icon->setCheckable(true);
    m_icon->setAttribute(Qt::WA_TransparentForMouseEvents);
	m_icon->setObjectName(iconObjectName);

    m_title = new FSButton(this);
    m_title->setCheckable(true);
    m_title->setAttribute(Qt::WA_TransparentForMouseEvents);
	m_title->setText(tr(text));
    m_title->setObjectName(iconObjectName + "_title");
	m_title->setProperty("QByteArray", QByteArray(text));
}


bool SettingsWidgetBarItem::IsChecked()
{
	return m_ckb->isVisible() && m_ckb->isChecked();
}

void SettingsWidgetBarItem::SetChecked(bool checked)
{
	//if (m_ckb->isVisible())
	m_ckb->blockSignals(true);
	m_ckb->setChecked(checked);
	m_ckb->blockSignals(false);
	m_panel->SetPanelEnabled(checked, false);

    m_icon->setChecked(checked);
    m_title->setChecked(checked);
}

void SettingsWidgetBarItem::SetEnabled(bool enabled)
{
	m_ckb->setEnabled(enabled);
	m_icon->setEnabled(enabled);
	m_title->setEnabled(enabled);
	if (!enabled && IsChecked())
	{
		SetChecked(false);
	}
}

void SettingsWidgetBarItem::SetSelected(bool selected)
{
	m_selected = selected;
	update();
}

SettingsBasePanel* SettingsWidgetBarItem::GetPanel()
{
	return m_panel;
}

QColor SettingsWidgetBarItem::GetSelectedItemColor()
{
	return m_selectedColor;
}

void SettingsWidgetBarItem::SetSelectedItemColor(QColor color)
{
	m_selectedColor = color;
}

void SettingsWidgetBarItem::resizeEvent(QResizeEvent *event)
{
	const int spacing = 28;
	const int ckbw = 20;
	int left = 60;
	int top = 0;
	m_ckb->setGeometry(left, (height() - ckbw) / 2, ckbw, ckbw);
	left = m_ckb->geometry().right() + spacing;
	m_icon->setGeometry(left, top, 23, height());
	left = m_icon->geometry().right() + 21;
	m_title->setGeometry(left, top, width() - left, height());
}

void SettingsWidgetBarItem::mousePressEvent(QMouseEvent *event)
{
	emit signalClicked(m_panel);
}

void SettingsWidgetBarItem::paintEvent(QPaintEvent *event)
{
	if (m_selected)
	{
		QPainter painter(this);
		painter.fillRect(rect(), GetSelectedItemColor());
	}
}

void SettingsWidgetBarItem::RetranslateUi()
{
	auto bytearray = m_title->property("QByteArray").toByteArray();
	m_title->setText(tr(bytearray.constData()));
}

//////////////////////////////////////////////////////////////////////////
SettingsScreenPanel::SettingsScreenPanel(QWidget *parent)
:SettingsBasePanel(parent)
{
	CreatePanel(new SettingsScreenScreenPanel("SettingsScreenPanel_Screen", "Screen Capture", this));
	CreatePanel(new SettingsScreenGamePanel("SettingsScreenPanel_Game", "Game", this));
	
	//for (int i = 0; i < m_panels.size(); ++i)
	//{
	//	auto panel = m_panels[i];
	//	panel->SetRadioButtonChecked(panel->GetDeviceSelected());
	//}
	//CreatePanel(tr("Mobile Device"), new SettingsScreenScreenPanel("FSSettingsWidget_Screen", "Mobile Device", this));
}

void SettingsScreenPanel::CreatePanel( SettingsScreenBasePanel *panel)
{
	m_panels.push_back(panel);
	connect(panel, &SettingsScreenBasePanel::signalToggled, this, [&](bool checked){
		for (auto temp : m_panels)
		{
			temp->SetRadioButtonChecked(temp == (qobject_cast<SettingsScreenBasePanel*>(sender())));
			resizeEvent(nullptr);
		}
	});
	connect(panel, &SettingsScreenBasePanel::signalPanelSetChekced, this, &SettingsScreenPanel::signalPanelSetChekced);
}

void SettingsScreenPanel::SetPanelEnabled(bool enabled, bool manual)
{
	SettingsDataManager::GetInstance()->SetScreenChecked(enabled);


	bool readyEnabled = false;
	SettingsScreenGamePanel *gamePanel = nullptr;
	for (auto temp : m_panels)
	{
		gamePanel = qobject_cast<SettingsScreenGamePanel*>(temp);
		if (gamePanel)
			break;
	}
	Q_ASSERT(gamePanel);
	if (!enabled && gamePanel && gamePanel->GetRadioButtonChecked() && !gamePanel->HaveChooseGame())
	{
		readyEnabled = true;
	}
	else
	{
		readyEnabled = enabled;
	}

	for (auto temp : m_panels)
	{
		temp->SetPanelEnabled(enabled, manual);
		temp->setEnabled(readyEnabled);
	}
}

void SettingsScreenPanel::resizeEvent(QResizeEvent *event)
{
	int top = sTop;
	int left = 64;
	for (auto temp : m_panels)
	{
		temp->setGeometry(left, top, width(), temp->Height());
		top = temp->geometry().bottom() + 10;
	}

}

void SettingsScreenPanel::showEvent(QShowEvent *event)
{
	static bool firstUp = true;
	connect(Communicator::GetInstance(), &Communicator::sigClearRecorder, this, [&]{
		firstUp = true;
	}, Qt::UniqueConnection);
	if (firstUp)
	{

		for (int i = 0; i < m_panels.size(); ++i)
		{
			auto panel = m_panels[i];
			panel->SetRadioButtonChecked(panel->GetDeviceSelected());
		}
		resizeEvent(nullptr);

		firstUp = false;
	}
}

static const char* sNoAvailable = "No available";
static const char* sDontCapture = "Don't Capture";

SettingsCameraPanel::SettingsCameraPanel(QWidget *parent)
: SettingsBasePanel(parent)
{
	m_lbChooseCamera = new FSLabel(this);
    m_cmbChooseCamera = new FSComboBox(this);

	m_lbResolution = new FSLabel(this);
    m_cmbResolution = new FSComboBox(this);

	m_lbFrameRate = new FSLabel(this);
    m_cmbFrameRate = new FSComboBox(this);

    m_lbChooseCamera->setObjectName("SettingsCameraPanel_mlb");
    m_lbResolution->setObjectName("SettingsCameraPanel_mlb");
    m_lbFrameRate->setObjectName("SettingsCameraPanel_mlb");

    QString noAvailable(tr("No available"));
	m_cmbChooseCamera->addItem(tr(sNoAvailable));
	RetranslateUi();


	connect(m_cmbChooseCamera, SIGNAL(currentIndexChanged(int)), this, SLOT(ChooseCameraIndexChanged(int)));
	connect(m_cmbResolution, SIGNAL(currentIndexChanged(int)), this, SLOT(ResolutionIndexChanged(int)));
	connect(m_cmbFrameRate, SIGNAL(currentIndexChanged(int)), this, SLOT(FramerateIndexChanged(int)));

	m_btnText = new QPushButton("Camera", this);
	m_btnText->setGeometry(0, 0, 70, 22);
	m_btnText->hide();
	connect(m_btnText, &QPushButton::clicked, this, [&](){
		//QWidget *widget = new QWidget;
		//widget->resize(600, 500);
		//widget->show();

		//auto data = SettingsDataManager::GetInstance()->GetCameraData();
		//data->AddHWnd((HWND)widget->winId());
		//data->Start();
		//widget->update();

		//SettingsDataManager::GetInstance()->SaveSetting();
		SettingsDataManager::GetInstance()->GetCameraData()->DevicesChanged();
		//CameraPreview *dlg = new CameraPreview(0);
		//dlg->show();

		//MicrophonePreview *audio = new MicrophonePreview(0);
		//audio->show();
	});


	auto data = SettingsDataManager::GetInstance()->GetCameraData();
	connect(data, &CameraData::signalDeviceChagned, this, [&](){
		NewDevices();
        auto data = SettingsDataManager::GetInstance()->GetCameraData();

        //static bool firstInit = true;
        //if (!firstInit && !data->GetDevices().isEmpty())
        //{
        //    // 第一次初始化不要刷最佳分辨率
        //    RefreshResolutions(true);
        //}
        //firstInit = false;
		if (!data->GetDeviceSelected())
			emit signalPanelSetChekced(false);

	}, Qt::QueuedConnection);

}
void SettingsCameraPanel::RetranslateUi()
{
	m_lbChooseCamera->setText(tr("Choose a Camera"));
	m_lbResolution->setText(tr("Resolution"));
	m_lbFrameRate->setText(tr("Frame Rate"));
}

bool SettingsCameraPanel::RefreshDevices()
{
	auto data = SettingsDataManager::GetInstance()->GetCameraData();
	m_cmbChooseCamera->blockSignals(true);
	m_cmbChooseCamera->clear();
	auto devices = data->GetDevices();
	for (auto temp : devices)
	{
		m_cmbChooseCamera->addItem(QString::fromWCharArray(temp.displayName), QVariant::fromValue(temp));
	}
	m_cmbChooseCamera->blockSignals(false);

	return !devices.isEmpty();
}

void SettingsCameraPanel::RefreshResolutions(bool cmbChanged)
{
	auto data = SettingsDataManager::GetInstance()->GetCameraData();
	m_cmbResolution->blockSignals(true);
	m_cmbResolution->clear();
	auto resoluctions = data->GetResolutions();
	for (auto temp : resoluctions)
	{
		m_cmbResolution->addItem(QString("%1*%2").arg(temp.width()).arg(temp.height()), temp);
	}

	// 选择中间分辨率
	if (cmbChanged && !resoluctions.isEmpty())
	{
        QSize defaultResoluction = data->GetDefaultResoluction(resoluctions);

		data->SetResolution(defaultResoluction);
		m_cmbResolution->setCurrentIndex(m_cmbResolution->findData(defaultResoluction));
	}

	m_cmbResolution->blockSignals(false);
}

void SettingsCameraPanel::RefreshFramerates()
{
	auto data = SettingsDataManager::GetInstance()->GetCameraData();
	m_cmbFrameRate->blockSignals(true);
	m_cmbFrameRate->clear();
	for (auto temp : data->GetFramerates())
	{
		m_cmbFrameRate->addItem(QString("%1 fps").arg(temp), temp);
	}
	m_cmbFrameRate->blockSignals(false);
}

static void sComboBoxClear(QComboBox *cmb)
{
	cmb->blockSignals(true);
	cmb->clear();
	cmb->blockSignals(false);
}
static void sComboBoxAddItem(QComboBox *cmb, QString text)
{
	cmb->blockSignals(true);
	cmb->addItem(text);
	cmb->blockSignals(false);
}
static void sComboBoxSetCurrentIndex(QComboBox *cmb, int index)
{
	cmb->blockSignals(true);
	cmb->setCurrentIndex(index);
	cmb->blockSignals(false);
}

void gSetContentChanged()
{
	//ProjectSaver::GetInstance()->SetContentChanged(true);
}

//bool operator == (const CaptureDeviceInfo &l, const CaptureDeviceInfo &r)
//{
//	return QString::fromWCharArray(l.deviceID) == QString::fromWCharArray(r.deviceID);
//}

int sComboBoxFindDataCaptureDeviceInfo(QComboBox *cmb, const CaptureDeviceInfo &curData)
{
    for (int i = 0; i < cmb->count(); ++i)
    {
        auto tempData = cmb->itemData(i).value<CaptureDeviceInfo>();
        if (tempData == curData)
        {
            return i;
        }
    }
    return 0;
}

void SettingsCameraPanel::NewDevices()
{
	sComboBoxClear(m_cmbChooseCamera);
	sComboBoxClear(m_cmbResolution);
	sComboBoxClear(m_cmbFrameRate);


	bool haveDevice = RefreshDevices();
	auto data = SettingsDataManager::GetInstance()->GetCameraData();
	if (haveDevice)
	{
		// 有摄像头

		//int index = m_cmbChooseCamera->findData(QVariant::fromValue(data->GetCurrentDevice()));
		//int index = 0;
		//auto curData = data->GetCurrentDevice();
		//for (int i = 0; i < m_cmbChooseCamera->count(); ++i)
		//{
		//	auto tempData = m_cmbChooseCamera->itemData(i).value<CaptureDeviceInfo>();
		//	if (tempData == curData)
		//	{
		//		index = i;
		//		break;
		//	}
		//}
        auto curData = data->GetCurrentDevice();
        int index = sComboBoxFindDataCaptureDeviceInfo(m_cmbChooseCamera, curData);

		sComboBoxSetCurrentIndex(m_cmbChooseCamera, index);
		//m_cmbChooseCamera->setCurrentIndex(index);

		RefreshResolutions(false);

		index = m_cmbResolution->findData(data->GetResolution());
		index = qMax(0, index);
		sComboBoxSetCurrentIndex(m_cmbResolution, index);
		//m_cmbResolution->setCurrentIndex(index);

		RefreshFramerates();


		index = sComboBoxItemDataIndexFloat(m_cmbFrameRate, data->GetFramerate());
		index = qMax(0, index);
		sComboBoxSetCurrentIndex(m_cmbFrameRate, index);
		//m_cmbFrameRate->setCurrentIndex(index);

	}
	else
	{
		sComboBoxAddItem(m_cmbChooseCamera, tr(sNoAvailable));
		sComboBoxAddItem(m_cmbResolution, tr(sNoAvailable));
		sComboBoxAddItem(m_cmbFrameRate, tr(sNoAvailable));

	}


	//setEnabled(haveDevice);
	emit signalPanelEnabled(haveDevice);
}

void SettingsCameraPanel::ChooseCameraIndexChanged(int index)
{
	gSetContentChanged();
	_ChooseCameraIndexChanged(index);
}

void SettingsCameraPanel::ResolutionIndexChanged(int index)
{
	
	gSetContentChanged();
	_ResolutionIndexChanged(index);

}

void SettingsCameraPanel::FramerateIndexChanged(int index)
{
	gSetContentChanged();
	_FramerateIndexChanged(index);

}

void SettingsCameraPanel::_ChooseCameraIndexChanged(int index)
{
	auto data = SettingsDataManager::GetInstance()->GetCameraData();
	data->SetCurrentDevice(m_cmbChooseCamera->currentData().value<CaptureDeviceInfo>());

	RefreshResolutions();
	RefreshFramerates();
}

void SettingsCameraPanel::_ResolutionIndexChanged(int index)
{
	auto data = SettingsDataManager::GetInstance()->GetCameraData();
	data->SetResolution(m_cmbResolution->currentData().toSize());

	RefreshFramerates();

}

void SettingsCameraPanel::_FramerateIndexChanged(int index)
{
	auto data = SettingsDataManager::GetInstance()->GetCameraData();
	data->SetFramerate(m_cmbFrameRate->currentData().toFloat());
}


void SettingsCameraPanel::resizeEvent(QResizeEvent *event)
{
	const int margins = 64;
	int left = margins;
	const int lbw = 210;
	const int comw = 242;
	const int lbheight = sPanelComboBoxHeight;
	const int spacing = 28;
	int top = sTop;

	m_lbChooseCamera->setGeometry(left, top, lbw, lbheight);
	left = m_lbChooseCamera->geometry().right() + 83;
	m_cmbChooseCamera->setGeometry(left, top, comw, lbheight);
	top = m_cmbChooseCamera->geometry().bottom() + spacing;

	left = margins;
	m_lbResolution->setGeometry(left, top, lbw, lbheight);
	left = m_lbResolution->geometry().right() + 83;
	m_cmbResolution->setGeometry(left, top, comw, lbheight);
	top = m_cmbResolution->geometry().bottom() + spacing;

	left = margins;
	m_lbFrameRate->setGeometry(left, top, lbw, lbheight);
	left = m_lbFrameRate->geometry().right() + 83;
	m_cmbFrameRate->setGeometry(left, top, comw, lbheight);

}



void SettingsCameraPanel::showEvent(QShowEvent *event)
{
	static bool firstup = true;
	connect(Communicator::GetInstance(), &Communicator::sigClearRecorder, this, [&]{
		firstup = true;
	}, Qt::UniqueConnection);

	if (firstup)
	{
		NewDevices();
		firstup = false;
	}

}



void SettingsCameraPanel::SetPanelEnabled(bool enabled, bool manual)
{
	setEnabled(enabled);
	SettingsDataManager::GetInstance()->SetCameraChecked(enabled);
}

//////////////////////////////////////////////////////////////////////////
SettingsAudioPanel::SettingsAudioPanel(QWidget *parent)
: SettingsBasePanel(parent)
, m_bMicrophoneLast(true)
{
	m_lbMicrophone = new FSLabel(this);
    m_cmbMicrophone = new FSComboBox(this);

	m_lbComputerAudio = new FSLabel(this);
    m_cmbComputerAudio = new FSComboBox(this);

    m_lbComputerAudio->setObjectName("SettingsAudioPanel_m_lb");
    m_lbMicrophone->setObjectName("SettingsAudioPanel_m_lb");
	RetranslateUi();

	m_cmbComputerAudio->addItem(tr("Don't Capture"), 0);
	m_cmbComputerAudio->addItem(tr("Capture"), 1);

	connect(m_cmbMicrophone, SIGNAL(currentIndexChanged(int)), this, SLOT(MicrophoneIndexChanged(int)));
	connect(m_cmbComputerAudio, SIGNAL(currentIndexChanged(int)), this, SLOT(ComputerAudioIndexChanged(int)));

	auto data = SettingsDataManager::GetInstance()->GetAudioData();
	connect(data, &AudioData::signalDeviceChagned, this, [&](){

		NewDevices();

        //int index = m_cmbMicrophone->count() > 1 ? 1 : 0;
        //sComboBoxSetCurrentIndex(m_cmbMicrophone, index);
        //CaptureDeviceInfo info = m_cmbMicrophone->itemData(1).value<CaptureDeviceInfo>();
        //SettingsDataManager::GetInstance()->GetAudioData()->SetCurrentDevice(info);
    });

}
void SettingsAudioPanel::RetranslateUi()
{
	m_lbMicrophone->setText(tr("Microphone"));
	m_lbComputerAudio->setText(tr("Computer Audio"));
	m_cmbComputerAudio->setItemText(0, tr(sDontCapture));
	m_cmbComputerAudio->setItemText(1, tr("Capture"));
}



void SettingsAudioPanel::MicrophoneIndexChanged(int index)
{
	gSetContentChanged();
	_MicrophoneIndexChanged(index);

}

void SettingsAudioPanel::ComputerAudioIndexChanged(int index)
{
	gSetContentChanged();
	_ComputerAudioIndexChanged(index);

}

void SettingsAudioPanel::_MicrophoneIndexChanged(int index)
{
	auto data = SettingsDataManager::GetInstance()->GetAudioData();
	bool enabled = index != 0;
	CaptureDeviceInfo info = m_cmbMicrophone->currentData().value<CaptureDeviceInfo>();
	if (!enabled)
	{
		memset(&info, 0, sizeof(info));
	}
	data->SetCurrentDevice(info);
	data->SetDeviceEnabled(enabled);

	if ((!data->GetDeviceSelected() && !data->GetRecordSystemAudioSelected()))
	{
		emit signalPanelSetChekced(false);
	}
	else if (data->GetDeviceSelected() && !data->GetRecordSystemAudioSelected())
	{
		emit signalPanelSetChekced(true);
	}


	m_microphoneLast.Push(index);
	m_bMicrophoneLast = true;
}

void SettingsAudioPanel::_ComputerAudioIndexChanged(int index)
{
	auto data = SettingsDataManager::GetInstance()->GetAudioData();
	data->SetRecordSystemAudio(index != 0);

	if ((!data->GetDeviceSelected() && !data->GetRecordSystemAudioSelected()))
	{
		emit signalPanelSetChekced(false);
	}
	else if (!data->GetDeviceSelected() && data->GetRecordSystemAudioSelected())
	{
		emit signalPanelSetChekced(true);
	}

	m_computerAudioLast.Push(index);
	m_bMicrophoneLast = false;
}

void SettingsAudioPanel::resizeEvent(QResizeEvent *event)
{
	const int margins = 64;
	int left = margins;
	const int lbw = 210;
	const int comw = 242;
	const int lbheight = sPanelComboBoxHeight;
	const int spacing = 28;
	int top = sTop;

	m_lbComputerAudio->setGeometry(left, top, lbw, lbheight);
	left = m_lbComputerAudio->geometry().right() + 83;
	m_cmbComputerAudio->setGeometry(left, top, comw, lbheight);
	top = m_cmbComputerAudio->geometry().bottom() + spacing;

	left = margins;
	m_lbMicrophone->setGeometry(left, top, lbw, lbheight);
	left = m_lbMicrophone->geometry().right() + 83;
	m_cmbMicrophone->setGeometry(left, top, comw, lbheight);
	 
}



void SettingsAudioPanel::showEvent(QShowEvent *event)
{
	NewDevices();

}

void SettingsAudioPanel::SetPanelEnabled(bool enabled, bool manual)
{
	setEnabled(enabled);
	SettingsDataManager::GetInstance()->SetAudioChecked(enabled);
	if (enabled)
	{
		auto data = SettingsDataManager::GetInstance()->GetAudioData();
		if (!data->GetDeviceSelected() && !data->GetRecordSystemAudioSelected())
		{
			if (m_bMicrophoneLast)
			{
				m_cmbMicrophone->setCurrentIndex(qBound(0, m_microphoneLast.Pop(), m_cmbMicrophone->count() - 1));
				m_microphoneLast.Push(m_microphoneLast.Pop());
			}
			else
			{
				m_cmbComputerAudio->setCurrentIndex(m_computerAudioLast.Pop());
				m_computerAudioLast.Push(m_computerAudioLast.Pop());
			}

			// 确保有一个开启
			if (m_cmbMicrophone->currentIndex() == 0 && !m_cmbComputerAudio->currentData().toBool())
			{
				m_cmbComputerAudio->setCurrentIndex(1);
			}
		}
	}
}

bool SettingsAudioPanel::RefreshDevices()
{
	auto data = SettingsDataManager::GetInstance()->GetAudioData();
	m_cmbMicrophone->blockSignals(true);
	m_cmbMicrophone->clear();
	m_cmbMicrophone->addItem(tr(sDontCapture));
	auto devices = data->GetDevices();
	for (auto temp : devices)
	{
		m_cmbMicrophone->addItem(QString::fromWCharArray(temp.displayName), QVariant::fromValue(temp));
	}
	m_cmbMicrophone->blockSignals(false);

	return !devices.isEmpty();
}

void SettingsAudioPanel::NewDevices()
{
	// init ui
	bool haveDevices = RefreshDevices();
	auto data = SettingsDataManager::GetInstance()->GetAudioData();
	//if (haveDevices)
	//{
        //int index = m_cmbMicrophone->findData(QVariant::fromValue(data->GetCurrentDevice()));
        //index = qMax(0, index);
        auto curData = data->GetCurrentDevice();
        int index = sComboBoxFindDataCaptureDeviceInfo(m_cmbMicrophone, curData);

		sComboBoxSetCurrentIndex(m_cmbMicrophone, index);

	//}
	//else
	//{
	//	//sComboBoxClear(m_cmbMicrophone);
	//	//sComboBoxAddItem(m_cmbMicrophone, tr(sNoAvailable));
	//}

	//m_lbMicrophone->setEnabled(haveDevices);
	//m_cmbMicrophone->setEnabled(haveDevices);

	index = data->GetRecordSystemAudioSelected() ? 1 : 0;
	sComboBoxSetCurrentIndex(m_cmbComputerAudio, index);


	if (m_cmbMicrophone->currentIndex() == 0 && !m_cmbComputerAudio->currentData().toBool())
	{
		emit signalPanelSetChekced(false);
	}

	m_microphoneLast.Push(m_cmbMicrophone->currentIndex());
	m_computerAudioLast.Push(m_cmbComputerAudio->currentIndex());
}

//////////////////////////////////////////////////////////////////////////

static void sAbstractButtonSetChecked(QAbstractButton *button, bool checked)
{
    button->blockSignals(true);
    button->setChecked(checked);
    button->blockSignals(false);
}

SettingsAdvancedPanel::SettingsAdvancedPanel(QWidget *parent)
: SettingsBasePanel(parent)
{
    m_btnGeneral = new FSButton(this);
    m_btnHotKeys = new FSButton(this);

    m_btnGeneral->setCheckable(true);
    m_btnHotKeys->setCheckable(true);

    //m_btnGeneral->setAttribute(Qt::WA_TranslucentBackground);
    //m_btnHotKeys->setAttribute(Qt::WA_TranslucentBackground);

    m_btnGeneral->setChecked(true);




	//m_btnHotKeys->setEnabled(false);

	//m_btnGeneral->installEventFilter(this);
	//m_btnHotKeys->installEventFilter(this);


	m_line = new QWidget(this);
	m_line->setObjectName("SettingsAdvancedPanel_line");
    m_btnHotKeys->setObjectName("SettingsAdvancedPanel_mlb_hotkeys");
    m_btnGeneral->setObjectName("SettingsAdvancedPanel_mlb_general");
	m_General = new SettingsAdvancedGeneralPanel(this);
	m_HotKeys = new SettingsAdvancedHotKeysPanel(this);
	m_HotKeys->hide();


    connect(m_btnGeneral, &FSButton::toggled, this, [&](bool checked){

        sAbstractButtonSetChecked(m_btnGeneral, true);
        sAbstractButtonSetChecked(m_btnHotKeys, false);

        m_General->setVisible(true);
        m_HotKeys->setVisible(false);
    });

    connect(m_btnHotKeys, &FSButton::toggled, this, [&](bool checked){

        sAbstractButtonSetChecked(m_btnGeneral, false);
        sAbstractButtonSetChecked(m_btnHotKeys, true);

        m_General->setVisible(false);
        m_HotKeys->setVisible(true);
    });

	RetranslateUi();
}
void SettingsAdvancedPanel::RetranslateUi()
{
	m_btnGeneral->setText(tr("General"));
	m_btnHotKeys->setText(tr("HotKeys"));
}



void SettingsAdvancedPanel::resizeEvent(QResizeEvent *event)
{
	const int margins = 64;
	const int btnw = 70;
	const int btnh = 30;
	int left = margins;
	int top = sTop;

	m_btnGeneral->setGeometry(left, top, btnw, btnh);
	left = m_btnGeneral->geometry().right() + 70;
	m_btnHotKeys->setGeometry(left, top, 200, btnh);
	top = m_btnHotKeys->geometry().bottom() + 10;
	m_line->setGeometry(margins, top, width() - 2 * margins, 2);
	top = m_line->geometry().bottom() + 28;

	QRect wrect = QRect(margins, top, width() - 2 * margins, height() - top);



	m_General->setGeometry(wrect);
	m_HotKeys->setGeometry(wrect);

}

//////////////////////////////////////////////////////////////////////////
SettingsScreenBasePanel::SettingsScreenBasePanel(QString iconObjectName, const char *text, QWidget *parent)
:SettingsBasePanel(parent)
, m_height(30)
{
    QString(tr("Screen Capture"));
    QString(tr("Game"));

    m_normalColor = QColor("#7F9CAF");
    m_grayColor = QColor("#39404D");

	m_bPreState = true;

	m_radioButton = new QRadioButton(this);
    connect(m_radioButton, &QRadioButton::toggled, this, [&](bool bchecked){
        m_icon->setChecked(bchecked);
        m_title->setChecked(bchecked);
        emit signalToggled(bchecked); 
    });

	m_icon = new FSButton(this);
	m_icon->setObjectName(iconObjectName);
    m_icon->setCheckable(true);
    m_icon->setAttribute(Qt::WA_TransparentForMouseEvents);
    m_title = new FSButton(this);
    m_title->setObjectName(iconObjectName + "_title");
    m_title->setCheckable(true);
    m_title->setAttribute(Qt::WA_TransparentForMouseEvents);
	m_title->setText(tr(text));
	m_title->setProperty("QByteArray", QByteArray(text));

}

void SettingsScreenBasePanel::SetRadioButtonChecked(bool checked)
{
	m_radioButton->blockSignals(true);
	m_radioButton->setChecked(checked);
	for (auto temp : m_hideWidget)
	{
		temp->setVisible(checked);
	}
	GetData()->SetDeviceEnabled(checked);
	m_radioButton->blockSignals(false);
    m_icon->setChecked(checked);
    m_title->setChecked(checked);
}

bool SettingsScreenBasePanel::GetRadioButtonChecked()
{
    Q_ASSERT(false);
    return false;
}

int SettingsScreenBasePanel::Height()
{
	if (!m_radioButton->isChecked())
	{
		return m_height;
	}
	else
	{
		return ShowHeight();
	}
}




bool SettingsScreenBasePanel::GetDeviceSelected()
{
	return GetData()->GetDeviceSelected();
}



void SettingsScreenBasePanel::resizeEvent(QResizeEvent *event)
{
	int left = 0;
	m_top = 0;
	m_lineSpacing = 25;
	m_spacing = 27;
	m_titleWidth = 180;
	m_cmbWidth = 242;

	m_radioButton->setGeometry(left, m_top, 20, m_height);
	left = m_radioButton->geometry().right() +m_spacing;
	m_icon->setGeometry(left, m_top, m_height, m_height);
	left = m_icon->geometry().right() + m_spacing;

    m_titleWidth = GetRetranslateUiTextLenght(m_title->font(), m_title->text(), m_titleWidth, 10);
	m_title->setGeometry(left, m_top, m_titleWidth, m_height);

	m_titleLeft = left;
    m_cmbLeft = m_title->geometry().right();
    //m_cmbLeft = width() - m_cmbWidth - 127;
}

void SettingsScreenBasePanel::mousePressEvent(QMouseEvent *event)
{
	if (event->x() <= m_title->geometry().right())
	{
		emit signalToggled(m_radioButton->isChecked());

	}
	FSBaseWidget::mousePressEvent(event);
}

void SettingsScreenBasePanel::RetranslateUi()
{
    auto bytearray = m_title->property("QByteArray").toByteArray();
    m_title->setText(tr(bytearray.constData()));

    resizeEvent(nullptr);
}

//////////////////////////////////////////////////////////////////////////
SettingsScreenScreenPanel::SettingsScreenScreenPanel(QString iconObjectName, const char *text,  QWidget *parent)
:SettingsScreenBasePanel(iconObjectName, text, parent)
{
    m_cmbScreenCapture = new FSComboBox(this);
	m_cmbScreenCapture->addItem(tr("Full screen"), DesktopData::CaputerType_Fullscreen);
	//m_cmbScreenCapture->addItem("1080p HD", DesktopData::CaputerType_1080);
	//m_cmbScreenCapture->addItem("720p HD", DesktopData::CaputerType_720);
	m_cmbScreenCapture->addItem(tr("Set a custom size later"), DesktopData::CaputerType_Custom);
	connect(m_cmbScreenCapture, SIGNAL(currentIndexChanged(int)), this, SLOT(ScreenCaptureIndexChanged(int)));

    m_lbFrameRate = new FSButton(this);
    m_lbFrameRate->setAttribute(Qt::WA_TransparentForMouseEvents);
    m_lbFrameRate->setCheckable(true);
    m_lbFrameRate->setObjectName("SettingsScreenBasePanel_m_lbFrameRate");
    m_cmbFrameRate = new FSComboBox(this);
    auto view = new QListView(this);
    view->setObjectName("SettingsScreenBasePanel_ComboboxView");
    m_cmbFrameRate->setView(view);

	auto AddFramerate = [&](float framerate){
		m_cmbFrameRate->addItem(QString("%1 fps").arg(framerate), framerate);
    };

	AddFramerate(15.0f);
	AddFramerate(24.0f);
	AddFramerate(25.0f);
	AddFramerate(29.97f);
	AddFramerate(30.0f);
	AddFramerate(50.0f);
	AddFramerate(59.94f);
	AddFramerate(60.0f);
	AddFramerate(120.0f);



    float framerate = SettingsDataManager::GetInstance()->GetDesktopData()->GetDesktopMaxFramerate();
    for (int i = 0; i < m_cmbFrameRate->count(); ++i)
    {
        if (m_cmbFrameRate->itemData(i).toFloat() <= framerate)
        {
            m_cmbFrameRate->setItemData(i, m_normalColor, Qt::TextColorRole);
        }
        else
        {
            m_cmbFrameRate->setItemData(i, m_grayColor, Qt::TextColorRole);
        }
    }



	connect(m_cmbFrameRate, SIGNAL(currentIndexChanged(int)), this, SLOT(FramerateIndexChanged(int)));


	m_hideWidget.push_back(m_cmbScreenCapture);
	m_hideWidget.push_back(m_lbFrameRate);
	m_hideWidget.push_back(m_cmbFrameRate);

	RetranslateUi();
}

void SettingsScreenScreenPanel::RetranslateUi()
{
	m_cmbScreenCapture->setItemText(0, tr("Full screen"));
	m_cmbScreenCapture->setItemText(1, tr("Set a custom size later"));
	m_lbFrameRate->setText(tr("Frame Rate"));
    SettingsScreenBasePanel::RetranslateUi();
}

void SettingsScreenScreenPanel::SetPanelEnabled(bool enabled, bool manual)
{

}

void SettingsScreenScreenPanel::SetRadioButtonChecked(bool checked)
{
    auto preRadioButtonChecked = GetRadioButtonChecked();

    if (!checked && GetRadioButtonChecked())
	{
		m_bPreState = SettingsDataManager::GetInstance()->GetDesktopData()->GetDeviceEnabled();
        m_lbFrameRate->setChecked(checked);
	}

	SettingsScreenBasePanel::SetRadioButtonChecked(checked);

	QTimer::singleShot(0, [&](){
		if (GetRadioButtonChecked())
		{
			emit signalPanelSetChekced(m_bPreState);
            m_lbFrameRate->setChecked(m_bPreState);
		}
	});
	
}

bool SettingsScreenScreenPanel::GetRadioButtonChecked()
{
    return m_lbFrameRate->isVisible();
}

SettingBaseData * SettingsScreenScreenPanel::GetData()
{
	return SettingsDataManager::GetInstance()->GetDesktopData();
}

void SettingsScreenScreenPanel::ScreenCaptureIndexChanged(int index)
{
	gSetContentChanged();
	_ScreenCaptureIndexChanged(index);
}

void SettingsScreenScreenPanel::FramerateIndexChanged(int index)
{
	gSetContentChanged();
	_FramerateIndexChanged(index);
}

void SettingsScreenScreenPanel::_ScreenCaptureIndexChanged(int index)
{
	auto data = SettingsDataManager::GetInstance()->GetDesktopData();
	data->SetType((DesktopData::CaputerType)m_cmbScreenCapture->itemData(index).toInt());
}

void SettingsScreenScreenPanel::_FramerateIndexChanged(int index)
{
	auto data = SettingsDataManager::GetInstance()->GetDesktopData();
	data->SetFramerate(m_cmbFrameRate->itemData(index).toFloat());
}

void SettingsScreenScreenPanel::resizeEvent(QResizeEvent *event)
{
	SettingsScreenBasePanel::resizeEvent(event);

	m_cmbScreenCapture->setGeometry(m_cmbLeft, m_top, m_cmbWidth, m_height);
	m_top = m_cmbScreenCapture->geometry().bottom() + m_lineSpacing;
	//m_lbCaptureFrom->setGeometry(m_titleLeft, m_top, m_titleWidth, m_height);
	//m_cmbCaptureFrom->setGeometry(m_cmbLeft, m_top, m_cmbWidth, m_height);
	//m_top = m_cmbCaptureFrom->geometry().bottom() + m_lineSpacing;
	m_lbFrameRate->setGeometry(m_titleLeft, m_top, m_titleWidth, m_height);
	m_cmbFrameRate->setGeometry(m_cmbLeft, m_top, m_cmbWidth, m_height);
}


static int sComboBoxNearItemDataIndexFloat(QComboBox *cmb, float value)
{
    for (int i = 0; i < cmb->count(); ++i)
    {
        auto v = cmb->itemData(i).toFloat();
        if (value > v)
        {
            if (!qFuzzyCompare(value, v))
                continue;
            return i;
        }
        else
            return i;
    }
    return -1;
}
void SettingsScreenScreenPanel::showEvent(QShowEvent *event)
{
	auto data = SettingsDataManager::GetInstance()->GetDesktopData();
	if (data->GetDeviceEnabled())
	{
		////init ui
		int index = m_cmbScreenCapture->findData(data->GetType());
		index = qMax(0, index);
		sComboBoxSetCurrentIndex(m_cmbScreenCapture, index);
		//m_cmbScreenCapture->setCurrentIndex(index);

		//index = sComboBoxItemDataIndexFloat(m_cmbFrameRate, data->GetFramerate());
        index = sComboBoxNearItemDataIndexFloat(m_cmbFrameRate, data->GetFramerate());
        if (index < -1)
            index = 2; // 找不到index默认使用25帧
		sComboBoxSetCurrentIndex(m_cmbFrameRate, index);
		//m_cmbFrameRate->setCurrentIndex(index);
	}

}

int SettingsScreenScreenPanel::ShowHeight()
{
	return 120;
}



//////////////////////////////////////////////////////////////////////////
SettingsScreenGamePanel::SettingsScreenGamePanel(QString iconObjectName, const char *text, QWidget *parent)
:SettingsScreenBasePanel(iconObjectName, text, parent)
{
    m_cmbGame = new FSComboBox(this);
    auto view = new QListView(this);
    view->setObjectName("SettingsScreenBasePanel_ComboboxView");
    m_cmbGame->setView(view);
	RefreshGame();
	m_cmbGame->installEventFilter(this);

	m_btnRefresh = new FSButton(this);
	m_btnRefresh->setText("R");
	connect(m_btnRefresh, &FSButton::clicked, this, [&](){
		QString curGame = m_cmbGame->currentText();
		RefreshGame();

		int index = m_cmbGame->findText(curGame);
		index = qMax(0, index);
		//sComboBoxSetCurrentIndex(m_cmbGame, index);
		m_cmbGame->setCurrentIndex(index);
	});
	m_btnRefresh->hide();
    m_lbFrameRate = new FSButton(this);
    m_lbFrameRate->setAttribute(Qt::WA_TransparentForMouseEvents);
    m_lbFrameRate->setCheckable(true);
    m_lbFrameRate->setObjectName("SettingsScreenBasePanel_m_lbFrameRate");
    m_cmbFrameRate = new FSComboBox(this);

    QString noAvailable(tr("No available"));
    sComboBoxAddItem(m_cmbFrameRate, tr(sNoAvailable));
	m_cmbFrameRate->setEnabled(false);

	connect(m_cmbGame, SIGNAL(currentIndexChanged(int)), this, SLOT(GameIndexChanged(int)));
	connect(m_cmbFrameRate, SIGNAL(currentIndexChanged(int)), this, SLOT(FramerateIndexChanged(int)));

	m_hideWidget.push_back(m_cmbGame);
	m_hideWidget.push_back(m_lbFrameRate);
	m_hideWidget.push_back(m_cmbFrameRate);
	//m_hideWidget.push_back(m_btnRefresh);

	RetranslateUi();


	connect(SettingsDataManager::GetInstance()->GetGameData(), &GameData::signalCurrentDeviceLost, this, [&](){
		RefreshGame();
		NoAvialbleGame();

	}, Qt::QueuedConnection);
}

bool SettingsScreenGamePanel::HaveChooseGame()
{
	return m_cmbGame->currentIndex() != 0;
}

SettingBaseData * SettingsScreenGamePanel::GetData()
{
	return SettingsDataManager::GetInstance()->GetGameData();
}

void SettingsScreenGamePanel::RetranslateUi()
{
	m_lbFrameRate->setText(tr("Frame Rate"));
	m_cmbGame->setItemText(0, tr("Choose a Game"));
    SettingsScreenBasePanel::RetranslateUi();

}

void SettingsScreenGamePanel::resizeEvent(QResizeEvent *event)
{
	SettingsScreenBasePanel::resizeEvent(event);

    m_titleWidth = GetRetranslateUiTextLenght(m_lbFrameRate->font(), m_lbFrameRate->text(), m_titleWidth - 10) + 10;
    m_cmbLeft = m_titleLeft + m_titleWidth;

    m_cmbGame->setGeometry(m_cmbLeft, m_top, m_cmbWidth, m_height);
	m_btnRefresh->setGeometry(m_cmbGame->geometry().right() + 2, m_top, m_height, m_height);
	m_top = m_cmbGame->geometry().bottom() + m_lineSpacing;

	m_lbFrameRate->setGeometry(m_titleLeft, m_top, m_titleWidth, m_height);
	m_cmbFrameRate->setGeometry(m_cmbLeft, m_top, m_cmbWidth, m_height);
}

void SettingsScreenGamePanel::NoAvialbleGame()
{
	//int preFrameRate = m_cmbFrameRate->currentData().toInt();
	sComboBoxSetCurrentIndex(m_cmbGame, 0);
	sComboBoxClear(m_cmbFrameRate);
	sComboBoxAddItem(m_cmbFrameRate, tr(sNoAvailable));
	//m_cmbFrameRate->setItemData(0, preFrameRate);
	emit signalPanelSetChekced(false);
	m_cmbFrameRate->setEnabled(false);
    m_bPreState = false;
};

void SettingsScreenGamePanel::showEvent(QShowEvent *event)
{
	//init ui
	auto data = SettingsDataManager::GetInstance()->GetGameData();
	if (data->GetDeviceEnabled())
	{
		if (!data->GetGameName().isEmpty())
		{
			int index = m_cmbGame->findText(data->GetGameName(), Qt::MatchContains);
			index = qMax(0, index);
			sComboBoxSetCurrentIndex(m_cmbGame, index);

			static bool firstup = true;
			connect(Communicator::GetInstance(), &Communicator::sigClearRecorder, this, [&]{
				firstup = true;
			},Qt::UniqueConnection);
			if (firstup && index > 0)
			{
				//int preFramerate = m_cmbFrameRate->currentData().toFloat();
				ResetFrameratesComboBox();
				index = sComboBoxItemDataIndexFloat(m_cmbFrameRate, data->GetFramerate());
				index = qMax(0, index);
				sComboBoxSetCurrentIndex(m_cmbFrameRate, index);
				firstup = false;
			}

		
			index = sComboBoxItemDataIndexFloat(m_cmbFrameRate, data->GetFramerate());
			index = qMax(0, index);
			sComboBoxSetCurrentIndex(m_cmbFrameRate, index);
		}
		else
		{
			NoAvialbleGame();
		}
		//m_cmbFrameRate->setCurrentIndex(index);
	}

}

bool SettingsScreenGamePanel::eventFilter(QObject *watcher, QEvent *event)
{
	if (m_cmbGame == watcher)
	{
		if (event->type() == QEvent::MouseButtonPress)
		{
			//QMouseEvent *pEvent = static_cast<QMouseEvent*>(event);
			QString curGame = m_cmbGame->currentText();
			RefreshGame();
			int index = m_cmbGame->findText(curGame);
			index = qMax(0, index);
			sComboBoxSetCurrentIndex(m_cmbGame, index);
	

		}
	}

	return false;
}

int SettingsScreenGamePanel::ShowHeight()
{
	return 120;
}

void SettingsScreenGamePanel::RefreshGame()
{
	m_cmbGame->blockSignals(true);
	m_cmbGame->clear();
	m_cmbGame->addItem(tr("Choose a Game"));
    m_cmbGame->setItemData(m_cmbGame->count() - 1, QColor(m_normalColor), Qt::TextColorRole);
    auto processNames = ProcessManager::GetWindowProcess();

    QList<QString> GameBlacklist = SettingsDataManager::GetInstance()->GetGameData()->GetGameBlacklist();
    QList<QString> GameWhitelist = SettingsDataManager::GetInstance()->GetGameData()->GetGameWhitelist();
    ProcessDatas whitelist;
    ProcessDatas otherlist;

	for (auto temp : processNames)
    {

        if (!GameBlacklist.contains(temp.processName.toLower()))
		{
            if (GameWhitelist.contains(temp.processName.toLower()))
            {
                whitelist.push_back(temp);
            }
            else
            {
                otherlist.push_back(temp);
            }


		}

	}
    // 白名单
    for (auto temp : whitelist)
    {
        if (temp.name.isEmpty())
        {
            QFileInfo fileInfo(temp.processName);
            temp.name = fileInfo.baseName();
        }
        m_cmbGame->addItem(QString("%1").arg(temp.name), QVariant::fromValue(temp));
        m_cmbGame->setItemData(m_cmbGame->count() - 1, QColor(m_normalColor), Qt::TextColorRole);
    }
    // 其它名单
    for (auto temp : otherlist)
    {
        if (temp.name.isEmpty())
        {
            QFileInfo fileInfo(temp.processName);
            temp.name = fileInfo.baseName();
        }
        m_cmbGame->addItem(QString("%1").arg(temp.name), QVariant::fromValue(temp));
        m_cmbGame->setItemData(m_cmbGame->count() - 1, QColor(m_grayColor), Qt::TextColorRole);
    }

	m_cmbGame->blockSignals(false);

}

void SettingsScreenGamePanel::ResetFrameratesComboBox()
{
	m_cmbFrameRate->blockSignals(true);
	m_cmbFrameRate->clear();
	m_cmbFrameRate->blockSignals(false);

	auto AddFramerate = [&](float framerate){
		m_cmbFrameRate->blockSignals(true);
		m_cmbFrameRate->addItem(QString("%1 fps").arg(framerate), framerate);
		m_cmbFrameRate->blockSignals(false);

	};
	AddFramerate(15.0f);
	AddFramerate(24.0f);
	AddFramerate(25.0f);
	AddFramerate(29.97f);
	AddFramerate(30.0f);
	AddFramerate(50.0f);
	AddFramerate(60.0f);
	AddFramerate(120.0f);
}

void SettingsScreenGamePanel::SetPanelEnabled(bool enabled, bool manual)
{
	//不用再检测
	if (manual && m_cmbGame->isVisible() && enabled)
	{
		//_GameIndexChanged(m_cmbGame->currentIndex());
		if (m_cmbGame->currentIndex() == 0)
		{
            FSMessageBox::informationOk(tr(sFilmoraScreen), tr("Please choose a game"), RecorderViewsMgr::GetInstance()->GetRecordWidget(), 500, 200);
			emit signalPanelSetChekced(false);
		}
	}
}

void SettingsScreenGamePanel::SetRadioButtonChecked(bool checked)
{
    auto preRadioButtonChecked = GetRadioButtonChecked();
    if (!checked && preRadioButtonChecked)
	{
		m_bPreState = SettingsDataManager::GetInstance()->GetGameData()->GetDeviceEnabled();
	}

	SettingsScreenBasePanel::SetRadioButtonChecked(checked);
	if (checked)
	{
		_GameIndexChanged(m_cmbGame->currentIndex(), false);
	}

    m_lbFrameRate->setChecked(checked);
    if (preRadioButtonChecked != GetRadioButtonChecked())
    {
        QTimer::singleShot(0, [&](){
            if (GetRadioButtonChecked())
            {
                emit signalPanelSetChekced(m_bPreState);
            }
        });
    }


}

bool SettingsScreenGamePanel::GetRadioButtonChecked()
{
    return m_title->isChecked();
}

void SettingsScreenGamePanel::GameIndexChanged(int index)
{
	
	gSetContentChanged();
	_GameIndexChanged(index, true);
}

void SettingsScreenGamePanel::FramerateIndexChanged(int index)
{
	gSetContentChanged();
	_FramerateIndexChanged(index);
}

void SettingsScreenGamePanel::_GameIndexChanged(int index, bool emitPanelSetChekced)
{
	//auto NoAvialbleGame = [&](){
	//	sComboBoxSetCurrentIndex(m_cmbGame, 0);
	//	m_cmbFrameRate->clear();
	//	sComboBoxAddItem(m_cmbFrameRate, sNoAvailable);
	//	emit signalPanelSetChekced(false);
	//	m_cmbFrameRate->setEnabled(false);
	//};

	auto data = SettingsDataManager::GetInstance()->GetGameData();
	if (index != 0)
	{
		//bool enabled = data->SetGamenName(m_cmbGame->currentData(Qt::UserRole).toString(), 
		//	m_cmbGame->currentData(Qt::UserRole + 1).toString(),
		//	(HWND)m_cmbGame->currentData(Qt::UserRole + 2).toInt(),
		//	m_cmbGame->currentData(Qt::UserRole + 3).toInt()
		//	);
        bool enabled = data->SetGamenName(m_cmbGame->currentData().value<ProcessData>());
		if (!enabled)
		{
            FSMessageBox::informationOk(tr(sFilmoraScreen), tr("Please check whether the game is running properly?"), RecorderViewsMgr::GetInstance()->GetRecordWidget());
			//m_cmbGame->setCurrentIndex(0);
			//sComboBoxSetCurrentIndex(m_cmbGame, 0);
			//emit signalPanelSetChekced(false);

			NoAvialbleGame();
		}
		else
		{
			
			if (m_cmbFrameRate->count() <= 1)
			{
				int preFramerate = m_cmbFrameRate->currentData().toFloat();
				if (preFramerate <= 0)
					preFramerate = data->GetFramerate();
				ResetFrameratesComboBox();
				index = sComboBoxItemDataIndexFloat(m_cmbFrameRate, preFramerate);
				index = qMax(0, index);
				sComboBoxSetCurrentIndex(m_cmbFrameRate, index);
			}
			m_cmbFrameRate->setEnabled(true);
			if (emitPanelSetChekced)
				emit signalPanelSetChekced(true);
		}
	}
	else
	{
		//// 只是为了保存gamename
		NoAvialbleGame();

	}

}

void SettingsScreenGamePanel::_FramerateIndexChanged(int index)
{
	auto data = SettingsDataManager::GetInstance()->GetGameData();
	data->SetFramerate(m_cmbFrameRate->currentData().toFloat());
}

//////////////////////////////////////////////////////////////////////////
SettingsAdvancedGeneralPanel::SettingsAdvancedGeneralPanel(QWidget *parent)
:FSBaseWidget(parent)
{
	m_lbRecordFile = new FSLabel(this);
	m_lbRecordFile->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    m_lbRecordFile->setObjectName("SettingsAdvancedPanel_mlb");
	m_leRecordFile = new QLineEdit(this);
	m_leRecordFile->setReadOnly(true);
    m_leRecordFile->setObjectName("SettingsAdvancedPanel_mle");
	m_btnRecordFile = new FSBaseButton("OptionGeneralPanel_BtnRecordFile", this);
	connect(m_btnRecordFile, &BaseButton::Clicked, this, [&](){

        QString path = QFileDialog::getExistingDirectory(this, tr("Selected"), SettingsDataManager::GetInstance()->GetAdvancedData()->GetRecordFilePath());
		if (!path.isEmpty())
		{
			m_leRecordFile->setText(path);
			m_leRecordFile->setCursorPosition(0);
			m_leRecordFile->setToolTip(path);
			SettingsDataManager::GetInstance()->GetAdvancedData()->SetRecordFilePath(path);
            SettingsDataManager::GetInstance()->SaveSetting();
		}
	});


	m_ckbGPU = new QCheckBox(this);
    m_ckbGPU->setObjectName("SettingsAdvancedPanel_m_ckbGPU");
	connect(m_ckbGPU, &QCheckBox::toggled, this, [&](bool checked){
		SettingsDataManager::GetInstance()->GetAdvancedData()->SetSupportGPU(checked);
        SettingsDataManager::GetInstance()->SaveSetting();

	});

    m_ckbAutoHideRecorder = new QCheckBox(this);
    m_ckbAutoHideRecorder->setObjectName("SettingsAdvancedPanel_m_ckbGPU");
    connect(m_ckbAutoHideRecorder, &QCheckBox::toggled, this, [&](bool checked){
        SettingsDataManager::GetInstance()->GetAdvancedData()->SetAutoHideRecording(checked);
        SettingsDataManager::GetInstance()->SaveSetting();

    });

	m_lbGPU1 = new FSLabel(this);
	m_lbGPU2 = new FSLabel(this);
	m_lbGPU3 = new FSLabel(this);
	m_lbGPU4 = new FSLabel(this);

	m_lbGPU1->setObjectName("SettingsAdvancedGeneralPanel_GPU1");
	m_lbGPU2->setObjectName("SettingsAdvancedGeneralPanel_GPU2");
	m_lbGPU3->setObjectName("SettingsAdvancedGeneralPanel_GPU3");
	m_lbGPU4->setObjectName("SettingsAdvancedGeneralPanel_GPU4");

	m_lbGPU1->setVisible(false);
	m_lbGPU2->setVisible(false);
	m_lbGPU3->setVisible(false);
	m_lbGPU4->setVisible(false);
	RetranslateUi();


}

void SettingsAdvancedGeneralPanel::RetranslateUi()
{
	m_lbRecordFile->setText(tr("Save Recordings to"));
    m_ckbGPU->setText(tr("GPU Acceleration"));
    m_ckbAutoHideRecorder->setText(tr("Auto Hide Control When Recording"));

    resizeEvent(nullptr);
}

void SettingsAdvancedGeneralPanel::resizeEvent(QResizeEvent *event)
{
	const int lbHeight = 30;
	const int btnWidth = 22;

	const int lbWidth = 140;
	const int spacing = 20;

	int left = 0;
	int top = 0;

    int wTemp = GetRetranslateUiTextLenght(m_lbRecordFile->font(), m_lbRecordFile->text(), lbWidth, 10);
    m_lbRecordFile->setGeometry(left, top, wTemp, lbHeight);
    left = m_lbRecordFile->geometry().right() + 20;
	int tmpWidth = width() - left - 30;
	m_leRecordFile->setGeometry(left, top, tmpWidth, lbHeight);
	left = m_leRecordFile->geometry().right();
	m_btnRecordFile->setGeometry(left, top, 29, 30);

	left = 0;
	top = m_btnRecordFile->geometry().bottom() + spacing;
    m_ckbGPU->setGeometry(left, top, width(), lbHeight);
    top = m_ckbGPU->geometry().bottom() + spacing;
    m_ckbAutoHideRecorder->setGeometry(left, top, width(), lbHeight);
    top = m_ckbAutoHideRecorder->geometry().bottom() + spacing;


	const int gpuw = 100;
	const int gpuh = 100;
	m_lbGPU1->setGeometry(left, top, 85, gpuh);
	left = m_lbGPU1->geometry().right();
	m_lbGPU2->setGeometry(left, top, 85, gpuh);
	left = m_lbGPU2->geometry().right() ;
	m_lbGPU3->setGeometry(left, top, 120, gpuh);
	left = m_lbGPU3->geometry().right() ;
	m_lbGPU4->setGeometry(left, top, 120, gpuh);
	

}

void SettingsAdvancedGeneralPanel::showEvent(QShowEvent *event)
{

	// init ui
	auto data = SettingsDataManager::GetInstance()->GetAdvancedData();
	m_leRecordFile->setText(data->GetRecordFilePath());
	m_leRecordFile->setCursorPosition(0);
	m_leRecordFile->setToolTip(m_leRecordFile->text());
	m_ckbGPU->blockSignals(true);
	m_ckbGPU->setChecked(data->GetSuportGPU());
	m_ckbGPU->blockSignals(false);

    m_ckbAutoHideRecorder->blockSignals(true);
    m_ckbAutoHideRecorder->setChecked(data->IsAutoHideRecorder());
    m_ckbAutoHideRecorder->blockSignals(false);



}

void SettingsAdvancedGeneralPanel::hideEvent(QHideEvent *event)
{
    FSBaseWidget::hideEvent(event);
}

//////////////////////////////////////////////////////////////////////////
SettingsAdvancedHotKeysPanel::SettingsAdvancedHotKeysPanel(QWidget *parent)
:FSBaseWidget(parent)
{
	m_enabled = true;

	m_lbStartStop = new FSLabel(this);
	m_leStartStop = new QLineEdit(this);

	m_lbPauseResume = new FSLabel(this);
	m_lePauseResume = new QLineEdit(this);

	m_lbFullscreen = new FSLabel(this);
	m_leFullscreen = new QLineEdit(this);

    m_lbAddMarker = new FSLabel(this);
    m_leAddMarker = new QLineEdit(this);

    m_lbStartStop->setObjectName("SettingsAdvancedPanel_mlb");
    m_lbPauseResume->setObjectName("SettingsAdvancedPanel_mlb");
    m_lbFullscreen->setObjectName("SettingsAdvancedPanel_mlb");
    m_lbAddMarker->setObjectName("SettingsAdvancedPanel_mlb");
    m_leFullscreen->setObjectName("SettingsAdvancedPanel_mle");
    m_lePauseResume->setObjectName("SettingsAdvancedPanel_mle");
    m_leStartStop->setObjectName("SettingsAdvancedPanel_mle_startstop");
    m_leAddMarker->setObjectName("SettingsAdvancedPanel_mle");

	m_leStartStop->installEventFilter(this);
	m_lePauseResume->installEventFilter(this);
    m_leFullscreen->installEventFilter(this);
    m_leAddMarker->installEventFilter(this);

	m_leStartStop->setContextMenuPolicy(Qt::NoContextMenu);
	m_lePauseResume->setContextMenuPolicy(Qt::NoContextMenu);
    m_leFullscreen->setContextMenuPolicy(Qt::NoContextMenu);
    m_leAddMarker->setContextMenuPolicy(Qt::NoContextMenu);

	RetranslateUi();

	
}


void SettingsAdvancedHotKeysPanel::resizeEvent(QResizeEvent *event)
{
	const int lbHeight = 30;
	int lbWidth = 140;
	const int spacing = 14;
	const int leWidth = 270;
    const int widthOffset = 270;
	int left = 0;
	int top = 0;
	//lbWidth = StringOperation::GetTextTrueLenght(m_lbStartStop->font(), m_lbStartStop->text());
    lbWidth = width() - widthOffset;
	m_lbStartStop->setGeometry(left, top, lbWidth, lbHeight);
    left = width() - widthOffset;
	m_leStartStop->setGeometry(left, top, leWidth, lbHeight);
	top = m_leStartStop->geometry().bottom() + spacing;
	left = 0;

	//lbWidth = StringOperation::GetTextTrueLenght(m_lbPauseResume->font(), m_lbPauseResume->text());
	m_lbPauseResume->setGeometry(left, top, lbWidth, lbHeight);
    left = width() - widthOffset;
	m_lePauseResume->setGeometry(left, top, leWidth, lbHeight);
	top = m_lePauseResume->geometry().bottom() + spacing;
	left = 0;

	//lbWidth = StringOperation::GetTextTrueLenght(m_lbFullscreen->font(), m_lbFullscreen->text());
	m_lbFullscreen->setGeometry(left, top, lbWidth, lbHeight);
    left = width() - widthOffset;
	m_leFullscreen->setGeometry(left, top, leWidth, lbHeight);
    top = m_leFullscreen->geometry().bottom() + spacing;
    left = 0;

    m_lbAddMarker->setGeometry(left, top, lbWidth, lbHeight);
    left = width() - widthOffset;
    m_leAddMarker->setGeometry(left, top, leWidth, lbHeight);

}

void SettingsAdvancedHotKeysPanel::RetranslateUi()
{
	m_lbStartStop->setText(tr("Start/Stop"));
	m_lbPauseResume->setText(tr("Pause/Resume"));
    m_lbFullscreen->setText(tr("Full Screen Recording"));
    m_lbAddMarker->setText(tr("Add Marker"));
}

static QString sGetControlString(Qt::KeyboardModifiers modifier)
{
    QString text;
    if (modifier & Qt::ShiftModifier)
    {
        text += "Shift+";
    }
    if (modifier & Qt::ControlModifier)
    {
        text += "Ctrl+";
    }
    if (modifier & Qt::AltModifier)
    {
        text += "Alt+";
    }
    return text;
}


bool SettingsAdvancedHotKeysPanel::eventFilter(QObject *watcher, QEvent *event)
{
	if (watcher == m_leStartStop || watcher == m_lePauseResume || watcher == m_leFullscreen || watcher == m_leAddMarker)
	{
		if (event->type() == QEvent::KeyPress)
		{
			QKeyEvent *pKeyEvent = static_cast<QKeyEvent*>(event);
			QString ddd = pKeyEvent->text();
			if (!sIsKeyControl(pKeyEvent->key()))
			{
				QLineEdit *curLe = qobject_cast<QLineEdit*>(watcher);
				//QString text = MKCollecter::GetInstance()->GetHotKeyString();
                QString text = sGetControlString(pKeyEvent->modifiers()) + MKCollecter::GetInstance()->GetKeyString(pKeyEvent->nativeVirtualKey());
				bool conflict = false;
				QString hotkeyErrString;
				if (curLe != m_leStartStop)
				{
					conflict |= (text == m_leStartStop->text());
					if (conflict)
					{
						hotkeyErrString = QString(tr("Please input different hotkey, F10 is set for Start/Stop"));
						hotkeyErrString.replace("F10", m_leStartStop->text());
					}
				}
				if (!conflict && curLe != m_lePauseResume)
				{
					conflict |= (text == m_lePauseResume->text());
					if (conflict)
					{
						hotkeyErrString = QString(tr("Please input different hotkey, F10 is set for Pause/Resume"));
						hotkeyErrString.replace("F10", m_lePauseResume->text());
					}
				}
				if (!conflict && curLe != m_leFullscreen)
				{
					conflict |= (text == m_leFullscreen->text());
					if (conflict)
					{
						hotkeyErrString = QString(tr("Please input different hotkey, F10 is set for Full screen recording"));
						hotkeyErrString.replace("F10", m_leFullscreen->text());
					}
				}
                if (!conflict && curLe != m_leAddMarker)
                {
                    conflict |= (text == m_leAddMarker->text());
                    if (conflict)
                    {
                        hotkeyErrString = QString(tr("Please input different hotkey, F10 is set for Full screen recording"));
                        hotkeyErrString.replace("F10", m_leAddMarker->text());
                    }
                }

				if (!conflict)
				{
					curLe->setText(text);
					if (curLe == m_leStartStop)
						SettingsDataManager::GetInstance()->GetAdvancedData()->SetStartStop(text);
					else if (curLe == m_lePauseResume)
						SettingsDataManager::GetInstance()->GetAdvancedData()->SetPauseResume(text);
                    else if (curLe == m_leFullscreen)
                        SettingsDataManager::GetInstance()->GetAdvancedData()->SetFullScreen(text);
                    else if (curLe == m_leAddMarker)
                        SettingsDataManager::GetInstance()->GetAdvancedData()->SetAddMarker(text);

                    SettingsDataManager::GetInstance()->SaveSetting();

				}
				else
				{
					FSMessageBox::informationOk(tr(sFilmoraScreen), hotkeyErrString, this);
				}
			}
			return true;
		}
		else if (event->type() == QEvent::FocusIn)
		{
			SettingsDataManager::GetInstance()->GetAdvancedData()->SetHotKeyEnabled(false);
		}
		else if (event->type() == QEvent::FocusOut)
		{
			SettingsDataManager::GetInstance()->GetAdvancedData()->SetHotKeyEnabled(true);
		}
	}

	return false;
}


void SettingsAdvancedHotKeysPanel::showEvent(QShowEvent *event)
{
	// init ui
	auto data = SettingsDataManager::GetInstance()->GetAdvancedData();
	m_leStartStop->setText(data->GetStartStop());
	m_lePauseResume->setText(data->GetPauseResume());
	m_leFullscreen->setText(data->GetFullScreen());
    m_leAddMarker->setText(data->GetAddMarker());
	setFocus();
}


PreviewBase::PreviewBase(QWidget *parent)
:FSBaseDialog("", "", parent)
, m_bManualClose(true)
, m_titleNameOffset(60)
{
    resize(340, 330);
    setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);

	m_titlebar->SetButtons(TopLevelWindowTitleBar::Close);
	m_titlebar->GetButton(TopLevelWindowTitleBar::Close)->setObjectName("FSRecorder_TitleBar_CloseButton");

	m_titlebar->SetResizeble(false);
	m_titlebar->SetAutoCloseWindow(false);
    m_titlebar->setObjectName("MicrophonePreview_m_titlebar");
    connect(m_titlebar, &TopLevelWindowTitleBar::DashedMove, this, &PreviewBase::sigDashedMove);
    connect(m_titlebar, &TopLevelWindowTitleBar::StartDashedMove, this, &PreviewBase::sigStartDashedMove);
    connect(m_titlebar, &TopLevelWindowTitleBar::EndDashedMove, this, &PreviewBase::sigEndDashedMove);


    setObjectName("PreviewBase");
	connect(m_titlebar, &TopLevelWindowTitleBar::ButtonClick, this, [&](int id){
        if (id == TopLevelWindowTitleBar::Close)
        {
            m_bManualClose = true;
            hide();
            emit signalManulClose();
        }
	});


	m_display = new QWidget(this);

    m_btnLock = new BaseStateButton(0, "", this);
    m_btnLock->AddButton(0, "RecordPanel_Title_PinUnlock");
    m_btnLock->AddButton(1, "RecordPanel_Title_Pinlock");
    connect(m_btnLock, &BaseStateButton::Clicked, this, [&](int id){
        m_titlebar->GetControl()->SetMoveable(id);
        signalLock(!id);
    });




}

PreviewBase::~PreviewBase()
{
}

bool PreviewBase::GetLock()
{
    return m_btnLock->CurrentButtonId();
}


void PreviewBase::setVisible(bool visible, bool force)
{
    if (force)
    {
        FSBaseDialog::setVisible(visible);
    }
    else
    {
        if (visible)
        {
            FSBaseDialog::setVisible(true);
        }
        else
        {
            // 非LOCK且在录制区内
            if (!GetLock() && PrimaryScreenManager::GetInstance()->PanelInRecordingRectangle(this))
            {    
                FSBaseDialog::setVisible(false);             
            }
        }
    }

}

bool PreviewBase::IsManualClose()
{
    return m_bManualClose;
}

void PreviewBase::SetManualClose()
{
    m_bManualClose = true;
    hide();
    emit signalManulClose();
}

void PreviewBase::SetResolution(QSize size)
{
	m_resolution = size;
}

HWND PreviewBase::GetHWND()
{
	return (HWND)m_display->winId();
}

void PreviewBase::resizeEvent(QResizeEvent *event)
{
	const int titleHeight = 30;

	QRect rc = rect().adjusted(1, 1, -1, -1);

	m_titlebar->setGeometry(rc.left(), rc.top(), rc.width(), titleHeight);
    m_btnLock->setGeometry(rc.width() - 60, rc.top(), titleHeight, titleHeight);
    m_btnLock->raise();
	int top = m_titlebar->geometry().bottom() + 1;

	int viewHeight = rc.height() - top;

	//auto data = SettingsDataManager::GetInstance()->GetCameraData();
	//auto size = data->GetResolution();
	float ratioTemp = 1.0 * m_resolution.width() / m_resolution.height();
	float ratio = 1.0 * rc.width() / viewHeight;
	int displayWidth = 0;
	int displayHeight = 0;
	if (ratio > ratioTemp)
	{

		displayHeight = viewHeight;
		displayWidth = viewHeight * ratioTemp;
	}
	else
	{
		displayWidth = rc.width();
		displayHeight = rc.width() / ratioTemp;
	}

	
	m_display->setGeometry(rc.left() + (rc.width() - displayWidth) / 2, top + (viewHeight - displayHeight) / 2, displayWidth, displayHeight);


	//m_display->setGeometry(0, top, width(), height() - top);
}

void PreviewBase::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.fillRect(rect().adjusted(1,1,-1,-1), Qt::black);
}

void PreviewBase::showEvent(QShowEvent *event)
{
	//auto data = SettingsDataManager::GetInstance()->GetCameraData();
	//SetTitle(QString::fromWCharArray(data->GetCurrentDevice().displayName));
	//data->AddHWnd((HWND)m_display->winId());
	//data->Start();
    m_titlebar->GetButton(TopLevelWindowTitleBar::Close)->setToolTip(tr("Close"));
    m_btnLock->GetButton(0)->setToolTip(tr("Lock"));
    m_btnLock->GetButton(1)->setToolTip(tr("UnLock"));
    m_bManualClose = false;
	emit signalShow();
}

void PreviewBase::hideEvent(QHideEvent *event)
{

	emit signalHide();

}


//////////////////////////////////////////////////////////////////////////
MicrophonePreview::MicrophonePreview(QWidget *parent)
:PreviewBase(parent)
{
	//setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
	//resize(340, 120);
	//m_titlebar->SetButtons(TopLevelWindowTitleBar::Close);
	//m_titlebar->GetButton(TopLevelWindowTitleBar::Close)->setObjectName("FSRecorder_TitleBar_CloseButton");
	//m_titlebar->SetResizeble(false);
	//m_titlebar->SetAutoCloseWindow(false);
 //   m_titlebar->setObjectName("MicrophonePreview_m_titlebar");
	//connect(m_titlebar, &TopLevelWindowTitleBar::ButtonClick, this, [&](int id){
	//	if (id == TopLevelWindowTitleBar::Close)
	//		hide();
	//});
    setObjectName("MicrophonePreview");
    m_display->hide();

	auto data = SettingsDataManager::GetInstance()->GetAudioData();
    m_slider = new ToolBarSlider(this);
	m_slider->SetChunkHeight(16);
    m_slider->setValue(data->GetAudioVolume() * 100);
    m_slider->setObjectName("RecorderFSBaseToolBar_pToolBarSlider");
    connect(m_slider, &ToolBarSlider::valueChanged, this, [&](int value){
		SettingsDataManager::GetInstance()->GetAudioData()->SetAudioVolume(1.0 * value / 100);
	});


    m_volume1 = new VolumeBar(this);


	connect(data, &AudioData::signalVolumeChanged, this, [&](int value){
		if (isVisible())
		{
			m_volume1->SetValue(value);
		}
	});

}

void MicrophonePreview::resizeEvent(QResizeEvent *event)
{
	PreviewBase::resizeEvent(event);

	const int titleHeight = 30;
	const int margins = 10;
	const int spacing = 20;
	int left = margins;
	int h = 10;

	QRect rc = rect().adjusted(1, 1, -1, -1);
	
	//m_titlebar->setGeometry(rc.left(), rc.top(), rc.width(), titleHeight);
    int top = m_titlebar->geometry().bottom() + 30;
	int w = rc.left() + (rc.width() - 2 * margins - spacing) / 2;

	m_slider->setGeometry(left, top, w, 16);
	left = m_slider->geometry().right() + spacing;
	m_volume1->setGeometry(left, top+4, w, 7);
}

void MicrophonePreview::showEvent(QShowEvent *event)
{
	auto data = SettingsDataManager::GetInstance()->GetAudioData();
    QString name = QString::fromWCharArray(data->GetCurrentDevice().displayName);
    QString STR = StringOperation::GetElidedString(m_titlebar->GetTitleLabel()->font(), width() - m_titleNameOffset, name+"fdsfdsfsdfsdf");
    SetTitle(STR);
    m_titlebar->setToolTip(name);

    PreviewBase::showEvent(event);
}


//////////////////////////////////////////////////////////////////////////
CameraPreview::CameraPreview(QWidget *parent)
:PreviewBase(parent)
{
	setObjectName("camera_Preview");
}

void CameraPreview::resizeEvent(QResizeEvent *event)
{
	auto data = SettingsDataManager::GetInstance()->GetCameraData();
	SetResolution(data->GetResolution());
	PreviewBase::resizeEvent(event);
}

void CameraPreview::showEvent(QShowEvent *event)
{
    auto data = SettingsDataManager::GetInstance()->GetCameraData();
    QString name = QString::fromWCharArray(data->GetCurrentDevice().displayName);
    QString STR = StringOperation::GetElidedString(m_titlebar->GetTitleLabel()->font(), width() - m_titleNameOffset, name);
    SetTitle(STR);
    m_titlebar->setToolTip(name);
	data->AddHWnd(GetHWND());
	data->Start();
	PreviewBase::showEvent(event);
}

void CameraPreview::hideEvent(QHideEvent *event)
{
	SettingsDataManager::GetInstance()->GetCameraData()->Stop();
	PreviewBase::hideEvent(event);
}

//////////////////////////////////////////////////////////////////////////
GamePreview::GamePreview(QWidget *parent)
:PreviewBase(parent)
{
    m_ratio = 16.0 / 9;

    // 游戏录制分辨可能会变化，实现去检测
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, [&](){
        QSize size = SettingsDataManager::GetInstance()->GetGameData()->GetSize();
        float ratioTemp = 1.0 * size.width() / size.height();
        if (!qFuzzyCompare(ratioTemp, m_ratio))
        {
            m_ratio = ratioTemp;
            resizeEvent(nullptr);
        }
    });
    m_timer->start(500);
}

void GamePreview::resizeEvent(QResizeEvent *event)
{
	auto data = SettingsDataManager::GetInstance()->GetGameData();

	SetResolution(GetSize());
	//SetResolution(data->GetResolution());
	PreviewBase::resizeEvent(event);
}

void GamePreview::showEvent(QShowEvent *event)
{
	auto data = SettingsDataManager::GetInstance()->GetGameData();
    QString name = data->GetGameName();
    QString STR = StringOperation::GetElidedString(m_titlebar->GetTitleLabel()->font(), width() - m_titleNameOffset, name);
    SetTitle(STR);
    m_titlebar->setToolTip(name);
	data->AddHWnd(GetHWND());
	data->Start();

    if (!m_timer->isActive())
        m_timer->start();

	PreviewBase::showEvent(event);
}

void GamePreview::hideEvent(QHideEvent *event)
{
	SettingsDataManager::GetInstance()->GetGameData()->Stop();
    m_timer->stop();
	PreviewBase::hideEvent(event);
}

QSize GamePreview::GetSize()
{
    int w = 0;
    int h = 0;

    if (1.0 * width() / height() > m_ratio)
    {
        w = height() * m_ratio;
        h = height();
    }
    else
    {
        w = width();
        h = width() / m_ratio;
    }

    return QSize(w, h);
}
