#include "stdafx.h"

#include "inc_CommonLib/FileOperation.h"

#include "SettingsDataManager.h"
#include "CaptureManger.h"


#include "inc_MKCollecter/MKCollecter.h"
#include "inc_CommonLib/WSCreateInstance.h"
#include "inc_FSCommonlib/ExternalAPI.h"
#include "ProjectFilesMgr.h"
#include "cVideoStreamCtrl.h"
#include "Interface/C++/cudacheck/wsCUDA.h"
#include "inc_CommonLib/PathOperation.h"
#include "inc_Commonlib/ExternalAPI.h"

#include "FSRecorder.h"
#include "cKeybStreamCtrl.h"

#include "PrimaryScreenManager.h"
#include <malloc.h>
bool operator == (const CaptureDeviceInfo &l, const CaptureDeviceInfo &r)
{
	return QString::fromWCharArray(l.deviceID) == QString::fromWCharArray(r.deviceID);
}

bool operator != (const CaptureDeviceInfo &l, const CaptureDeviceInfo &r)
{
	return QString::fromWCharArray(l.deviceID) != QString::fromWCharArray(r.deviceID);
}
static void sPrintAddRef(IUnknown *p)
{
	if (p)
	{
		p->AddRef();
		//qDebug() << "AddRef" << p->Release();
	}

}

QString GetDefaultRecordFiles()
{
	QString dir = QApplication::applicationDirPath() + "/Data/RecordFiles";
	QStringList dirs = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation);
	if (dirs.size() > 0)
	{
		dir = dirs[0] + "/FilmoraScreen/RecordFiles";
		QDir ddd(dir);
		if (!ddd.exists())
		{
			ddd.mkpath(dir);
		}
	}
	return dir;
}


static SettingsDataManager *s_SettingsDataManager = nullptr;
SettingsDataManager * SettingsDataManager::GetInstance()
{
	if (s_SettingsDataManager == nullptr)
		s_SettingsDataManager = new SettingsDataManager(0);
	return s_SettingsDataManager;

}

void SettingsDataManager::Release()
{
	SAFE_DELETE(s_SettingsDataManager);
}

bool SettingsDataManager::GetScreenChecked()
{
	return m_screenChecked;
}

void SettingsDataManager::SetScreenChecked(bool enabled)
{
	m_screenChecked = enabled;
}

void SettingsDataManager::SetAudioChecked(bool enabled)
{
	m_audioChecked = enabled;
}

bool SettingsDataManager::GetAudioChecked()
{
	return m_audioChecked;
}

void SettingsDataManager::SetCameraChecked(bool enabled)
{
	m_cameraChecked = enabled;
}

bool SettingsDataManager::GetCameraChecked()
{
	return m_cameraChecked;
}

bool SettingsDataManager::GetDeviceException()
{
    return m_bDeviceException;
}

void SettingsDataManager::SetDeviceException(bool ex)
{
    m_bDeviceException = ex;
}

SettingsDataManager::SettingsDataManager(QObject *parent)
:QObject(parent)
{
    InitData();

	m_desktopData = new DesktopData(this);
	m_gameData = new GameData(this);
	m_cameraData = new CameraData(this);
	m_audioData = new AudioData(this);
	m_advancedData = new AdvancedData(this);
	m_mouseData = new MouseData(this);
    m_keyData = new KeyData(this);

	m_datas.push_back(m_desktopData);
	m_datas.push_back(m_gameData);
	m_datas.push_back(m_cameraData);
	m_datas.push_back(m_audioData);
	m_datas.push_back(m_advancedData);
    m_datas.push_back(m_mouseData);
    m_datas.push_back(m_keyData);

	connect(ProjectMgr::GetInstance(), &ProjectMgr::sigLoaded, this, [&](){
		auto settings = ProjectMgr::GetInstance()->GetSettingString();
		LoadProjectSetting(settings);
		m_cameraData->DevicesChanged();
        m_audioData->DevicesChanged();
	}
	);

	// 设备增加/删除
	NLECaptureManger::GetInstance()->GetICaptureMgr()->SetCallback([&](void *pUserData, int32_t nMsg, int32_t wParam, int32_t lParam)->HRESULT{

		SettingsDataManager *pThis = (SettingsDataManager*)pUserData;

        if (nMsg == CAPTURE_MESSAGE_AUDIO_DEVICE_CHANGED_LOST || nMsg == CAPTURE_MESSAGE_AUDIO_DEVICE_CHANGED_FOUND)
		{
            bool curDeviceLost = false;
            if (nMsg == CAPTURE_MESSAGE_AUDIO_DEVICE_CHANGED_LOST)
            {
                CaptureDeviceInfo info;
                NLECaptureManger::GetInstance()->GetICaptureMgr()->GetDevice(wParam, &info);
                auto curInfo = pThis->GetAudioData()->GetCurrentDevice();
                curDeviceLost = (curInfo == info);
            }
            
            QMetaObject::invokeMethod(pThis->GetAudioData(), "DevicesChanged", Qt::QueuedConnection, Q_ARG(bool, curDeviceLost));
			
		}
		else if (nMsg == CAPTURE_MESSAGE_WEBCAM_LOST || nMsg == CAPTURE_MESSAGE_WEBCAM_FOUND)
		{
            // 不能去掉sleep,去掉插入第2个摄像头refresh不出来设备
			::Sleep(3000);
            bool curDeviceLost = false;
            if (nMsg == CAPTURE_MESSAGE_WEBCAM_LOST)
            {
                curDeviceLost = (wParam >= 0);
            }

            QMetaObject::invokeMethod(pThis->GetCameraData(), "DevicesChanged", Qt::QueuedConnection, Q_ARG(bool, curDeviceLost));
		}
		else if (nMsg == CAPTURE_MESSAGE_GAMELOST)
		{
			QMetaObject::invokeMethod(pThis->GetGameData(), "DevicesChanged", Qt::QueuedConnection);
		}
		return 0;
	}, this);


	LoadSetting();
}



DesktopData* SettingsDataManager::GetDesktopData()
{
	return m_desktopData;
}


GameData* SettingsDataManager::GetGameData()
{
	return m_gameData;
}

CameraData* SettingsDataManager::GetCameraData()
{
	return m_cameraData;
}

AudioData* SettingsDataManager::GetAudioData()
{
	return m_audioData;
}

AdvancedData* SettingsDataManager::GetAdvancedData()
{
	return m_advancedData;
}

MouseData* SettingsDataManager::GetMouseData()
{
	return m_mouseData;
}

KeyData* SettingsDataManager::GetKeyData()
{
    return m_keyData;
}

static const char *sScreen = "Screen";
static const char *sEnabled = "enabled";
static const char *sSelectIndex = "selectIndex";
static const char *sScreenCapture = "ScreenCapture";
static const char *sCaptureType = "CaptureType";
static const char *sCropRect = "CropRect";

static const char *sFrameRate = "FrameRate";
static const char *sGame = "Game";
static const char *sGameName = "GameName";

static const char *sCamera = "Camera";
static const char *sCameraName = "CameraName";
static const char *sResolution = "Resolution";
static const char *sResolutionWidth = "ResolutionWidth";
static const char *sResolutionHeight = "ResolutionHeight";

static const char *sAudio = "Audio";
static const char *sMicrophone = "Microphone";
static const char *sComputerAudio = "ComputerAudio";

static const char *sAdvanced = "Advanced";
//static const char *sGeneral = "General";
static const char *sSaveRecordingPath = "SaveRecordingPath";
static const char *sGPU = "GPU";
static const char *sAutoHideRecording = "AutoHideRecording";

//static const char *sHotKeys = "HotKeys";
static const char *sStartStop = "StartStop";
static const char *sPauseResume = "PauseResume";
static const char *sFullscreen = "Fullscreen";
static const char *sAddMarker = "AddMarker";

static const char *sSaveProjectDirctory = "SaveProjectDirctory";


#include <QtXml/QtXml>


void sElementAppendChild(QDomDocument doc, QDomElement parent, QString tag, QVariant text)
{
	auto elem = doc.createElement(tag);
	auto textNote = doc.createTextNode(text.toString());
	elem.appendChild(textNote);
	parent.appendChild(elem);
}

static void sElementAddAttribute(QDomDocument doc, QDomElement tag, QString key, QString value)
{
	auto enabledAttr = doc.createAttribute(key);
	enabledAttr.setValue(value);
	tag.setAttributeNode(enabledAttr);
}

static QString Bool2String(bool enabled)
{
	return enabled ? "1" : "0";
};

QDomElement sGetChildElementByTag(QDomElement parent, QString tag)
{
	auto elems = parent.elementsByTagName(tag);
	if (!elems.isEmpty())
	{
		return elems.at(0).toElement();
	}

	return QDomElement();
}

static bool sGetElementAttributeEnabled(QDomElement elem)
{
	return elem.attribute(sEnabled, false).toInt() != 0;
}

static bool sGetElementAttributeEnabled(QString attri, QDomElement elem)
{
	return elem.attribute(attri, false).toInt() != 0;
}




void SettingsDataManager::LoadProjectSetting(QString xml)
{
	QDomDocument doc("SettingMessage");
	QString errorMsg;
	if (!doc.setContent(xml, &errorMsg))
		return;

	auto docElem = doc.documentElement();

	auto screenElem = sGetChildElementByTag(docElem, sScreen);
	if (!screenElem.isNull())
	{
		SetScreenChecked(sGetElementAttributeEnabled(screenElem));
		int index = screenElem.attribute(sSelectIndex, 0).toInt();
		// screen caputer
		auto elem = sGetChildElementByTag(screenElem, sScreenCapture);
		if (!elem.isNull())
		{
			auto data = GetDesktopData();
			data->SetDeviceEnabled(index == 0);
			auto typeElem = sGetChildElementByTag(elem, sCaptureType);
			if (!typeElem.isNull())
				data->SetType((DesktopData::CaputerType)typeElem.text().toInt());
			auto framerateElem = sGetChildElementByTag(elem, sFrameRate);
			if (!framerateElem.isNull())
				data->SetFramerate(framerateElem.text().toFloat());

			auto cropRectElem = sGetChildElementByTag(elem, sCropRect);
			if (!cropRectElem.isNull())
			{
				auto cropRectStringlist = cropRectElem.text().split(" ");
				if (cropRectStringlist.size() == 4)
				{
					data->SetCropRect(QRect(cropRectStringlist[0].toInt(), cropRectStringlist[1].toInt(), cropRectStringlist[2].toInt(), cropRectStringlist[3].toInt()));
				}

			}
		}
		// game
		elem = sGetChildElementByTag(screenElem, sGame);
		if (!elem.isNull())
		{
			auto data = GetGameData();
			data->SetDeviceEnabled(index != 0);
			auto gameNameElem = sGetChildElementByTag(elem, sGameName);
            if (!gameNameElem.isNull())
            {
                ProcessData processData;
                processData.name = gameNameElem.text();
                data->SetGamenName(processData);
            }
			auto framerateElem = sGetChildElementByTag(elem, sFrameRate);
			if (!framerateElem.isNull())
				data->SetFramerate(framerateElem.text().toFloat());
		}
	}

	// camera
	auto cameraElem = sGetChildElementByTag(docElem, sCamera);
	if (!cameraElem.isNull())
	{
		auto data = GetCameraData();
		SetCameraChecked(sGetElementAttributeEnabled(cameraElem));
		auto nameElem = sGetChildElementByTag(cameraElem, sCameraName);
		if (!nameElem.isNull())
		{
			data->SetCurrentDevice(nameElem.text());
			data->SetDeviceEnabled(true);
			auto resolution = sGetChildElementByTag(cameraElem, sResolution);
			if (!resolution.isNull())
			{
				bool okw = false;
				bool okh = false;
				auto resolutionWidth = sGetChildElementByTag(resolution, sResolutionWidth).text().toInt(&okw);
				auto resolutionHeight = sGetChildElementByTag(resolution, sResolutionHeight).text().toInt(&okh);
				if (okw && okh)
					data->SetResolution(QSize(resolutionWidth, resolutionHeight));
			}

			auto framerateElem = sGetChildElementByTag(cameraElem, sFrameRate);
			if (!framerateElem.isNull())
				data->SetFramerate(framerateElem.text().toFloat());
		}

	}

	// audio
	auto audioElem = sGetChildElementByTag(docElem, sAudio);
	if (!audioElem.isNull())
	{
		auto data = GetAudioData();
		SetAudioChecked(sGetElementAttributeEnabled(audioElem));

		auto nameElem = sGetChildElementByTag(audioElem, sMicrophone);
		if (!nameElem.isNull())
		{
			data->SetDeviceEnabled(true);
			data->SetCurrentDevice(nameElem.text());
		}

		auto onElem = sGetChildElementByTag(audioElem, sComputerAudio);
		if (!onElem.isNull())
			data->SetRecordSystemAudio(onElem.text().toInt());
	}

}

typedef int(_stdcall * funGetGpu)();
void SettingsDataManager::LoadSetting()
{
	QString path = FSGetSettingsPath();
	QFile file(path);
	if (!file.open(QIODevice::ReadOnly))
		return;

	QDomDocument doc;
	QString errorMsg;
	if (!doc.setContent(&file, &errorMsg))
	{
		AutoGetGpuCap gpu;
		ACCELERATETYPE type = (ACCELERATETYPE)gpu.GetGpuCap();
		bool bInitSupportGpu = (type == HARDWARE_ACCELERATE_NVENC || type == HARDWARE_ACCELERATE_VCE);
		GetAdvancedData()->SetSupportGPU(bInitSupportGpu);
		return;
	}
		

	auto docElem = doc.documentElement();

	/// advanced
	auto advancedElem = sGetChildElementByTag(docElem, sAdvanced);
	if (!advancedElem.isNull())
	{
		auto data = GetAdvancedData();

		auto elem = sGetChildElementByTag(advancedElem, sSaveRecordingPath);
		if (!elem.isNull())
			data->SetRecordFilePath(elem.text());
        
		elem = sGetChildElementByTag(advancedElem, sGPU);
		if (!elem.isNull())
			data->SetSupportGPU(elem.text().toInt());

        elem = sGetChildElementByTag(advancedElem, sAutoHideRecording);
        if (!elem.isNull())
            data->SetAutoHideRecording(elem.text().toInt());

		elem = sGetChildElementByTag(advancedElem, sStartStop);
		if (!elem.isNull())
			data->SetStartStop(elem.text());

		elem = sGetChildElementByTag(advancedElem, sPauseResume);
		if (!elem.isNull())
			data->SetPauseResume(elem.text());

		elem = sGetChildElementByTag(advancedElem, sFullscreen);
		if (!elem.isNull())
			data->SetFullScreen(elem.text());

        elem = sGetChildElementByTag(advancedElem, sAddMarker);
        if (!elem.isNull())
            data->SetAddMarker(elem.text());

		elem = sGetChildElementByTag(advancedElem, sSaveProjectDirctory);
		if (!elem.isNull())
			data->SetProjectSaveDirectory(elem.text());

	}

}

void SettingsDataManager::InitData()
{
    m_screenChecked = true;
    m_audioChecked = true;
    m_cameraChecked = false;
    m_bDeviceException = false;
}

void SettingsDataManager::SaveSetting()
{
	QDomDocument doc("Setting");
	auto root = doc.createElement("Setting");
	doc.appendChild(root);

	// advanced
	auto advancedElem = doc.createElement(sAdvanced);
	root.appendChild(advancedElem);
	sElementAppendChild(doc, advancedElem, sSaveRecordingPath, GetAdvancedData()->GetRecordFilePath());
    sElementAppendChild(doc, advancedElem, sGPU, Bool2String(GetAdvancedData()->GetSuportGPU()));
    sElementAppendChild(doc, advancedElem, sAutoHideRecording, Bool2String(GetAdvancedData()->IsAutoHideRecorder()));
	sElementAppendChild(doc, advancedElem, sStartStop, GetAdvancedData()->GetStartStop());
	sElementAppendChild(doc, advancedElem, sPauseResume, GetAdvancedData()->GetPauseResume());
    sElementAppendChild(doc, advancedElem, sFullscreen, GetAdvancedData()->GetFullScreen());
    sElementAppendChild(doc, advancedElem, sAddMarker, GetAdvancedData()->GetAddMarker());
	sElementAppendChild(doc, advancedElem, sSaveProjectDirctory, GetAdvancedData()->GetProjectSaveDirectory());



	QFile file(FSGetSettingsPath());
	if (!file.open(QIODevice::WriteOnly))
		return;
	auto settingXml = doc.toString(4);
	file.write(settingXml.toUtf8());
}


QString SettingsDataManager::GetSettingString()
{
	QDomDocument doc;
	auto root = doc.createElement("SettingMessage");
	doc.appendChild(root);

	auto screenElem = doc.createElement(sScreen);
	root.appendChild(screenElem);

	sElementAddAttribute(doc, screenElem, sEnabled, Bool2String(GetScreenChecked()));
	int selectIndex = 0;
	if (GetDesktopData()->GetDeviceSelected()) 
		selectIndex = 0;
	else if (GetGameData()->GetDeviceSelected()) 
		selectIndex = 1;
	// screen
	sElementAddAttribute(doc, screenElem, sSelectIndex, QString::number(selectIndex));
	auto screenCaptureElem = doc.createElement(sScreenCapture);
	screenElem.appendChild(screenCaptureElem);
	sElementAppendChild(doc, screenCaptureElem, sCaptureType, GetDesktopData()->GetType());
	sElementAppendChild(doc, screenCaptureElem, sFrameRate, GetDesktopData()->GetFramerate());
	auto cropRect = GetDesktopData()->GetCropRect();
	sElementAppendChild(doc, screenCaptureElem, sCropRect, QString("%1 %2 %3 %4").arg(cropRect.x()).arg(cropRect.y()).arg(cropRect.width()).arg(cropRect.height()));


	// game
	auto gameElem = doc.createElement(sGame);
	screenElem.appendChild(gameElem);
	sElementAppendChild(doc, gameElem, sGameName, GetGameData()->GetGameName());
	sElementAppendChild(doc, gameElem, sFrameRate, GetGameData()->GetFramerate());

	// camera
	auto cameraElem = doc.createElement(sCamera);
	root.appendChild(cameraElem);
	sElementAddAttribute(doc, cameraElem, sEnabled, Bool2String(GetCameraData()->GetDeviceEnabled()));
	sElementAppendChild(doc, cameraElem, sCameraName, QString::fromWCharArray(GetCameraData()->GetCurrentDevice().displayName));
	auto resolutionElem = doc.createElement(sResolution);
	cameraElem.appendChild(resolutionElem);
	sElementAppendChild(doc, resolutionElem, sResolutionWidth, GetCameraData()->GetResolution().width());
	sElementAppendChild(doc, resolutionElem, sResolutionHeight, GetCameraData()->GetResolution().height());
	sElementAppendChild(doc, cameraElem, sFrameRate, GetCameraData()->GetFramerate());

	// audio
	auto audioElem = doc.createElement(sAudio);
	root.appendChild(audioElem);
	sElementAddAttribute(doc, audioElem, sEnabled, Bool2String(GetAudioChecked()));
	sElementAppendChild(doc, audioElem, sMicrophone, QString::fromWCharArray(GetAudioData()->GetCurrentDevice().displayName));
	sElementAppendChild(doc, audioElem, sComputerAudio, Bool2String(GetAudioData()->GetRecordSystemAudio()));

	SaveSetting();

	auto settingText = doc.toString(4);
	return settingText;
}

void SettingsDataManager::StopRecord()
{
	for (auto data : m_datas)
	{
		data->StopRecord();
	}
}

void SettingsDataManager::StartRecord()
{
    for (auto data : m_datas)
    {
        data->StartRecord();
    }
}

void SettingsDataManager::Clear()
{
    InitData();
    for (auto data : m_datas)
    {
        data->Clear();
    }
}

//////////////////////////////////////////////////////////////////////////
DesktopData::DesktopData(QObject *parent)
: SettingBaseData(parent)
{
    QFile file(":/FSRecorder/Resources/light/unregister.png");
    if (file.open(QIODevice::ReadOnly))
        m_unregisterPNG = file.readAll();
    else
        Q_ASSERT(false);
    Clear();
}

bool DesktopData::GetCaptureStream(NLEComPtr<IUnknown> &pCaptureStream, CaptureStreamSetting &pCaptureStreamSetting)
{
	if (!GetDeviceEnabled()) return false;

	auto devices = NLECaptureManger::GetInstance()->GetDevicesFromType(eDTDesktop);
	if (devices.isEmpty())
		return false;

	NLECaptureManger::GetInstance()->GetICaptureMgr()->CreateCaptureStream(&devices[0], &pCaptureStream);
	
	if (pCaptureStream == nullptr)
		return false;

	memset(&pCaptureStreamSetting, 0, sizeof(pCaptureStreamSetting));

	pCaptureStreamSetting.uStreamType = ST_VIDEO;
	pCaptureStreamSetting.video.dwFourCC = SettingsDataManager::GetInstance()->GetAdvancedData()->GetSuportGPUParam();
	pCaptureStreamSetting.video.dFrameRate = GetFramerate();
	pCaptureStreamSetting.video.nWidth = 0;
	pCaptureStreamSetting.video.nHeight = 0;

	pCaptureStreamSetting.vidEffect.cropRect.left = m_rect.left() * gGetDevicePixelRatio();
    pCaptureStreamSetting.vidEffect.cropRect.top = m_rect.top()* gGetDevicePixelRatio();
    pCaptureStreamSetting.vidEffect.cropRect.right = m_rect.right()* gGetDevicePixelRatio();
    pCaptureStreamSetting.vidEffect.cropRect.bottom = m_rect.bottom()* gGetDevicePixelRatio();
    pCaptureStreamSetting.vidEffect.uScreenID = PrimaryScreenManager::GetInstance()->PrimaryScreenIndex();

	return true;
}
#include "inc_UpdateRegister/RegisterMain.h"

bool DesktopData::GetUnRegisterCaptureStream(NLEComPtr<IUnknown> &pCaptureStream, CaptureStreamSetting &pCaptureStreamSetting)
{
    {
        RegisterMain dlg(0);
        if (dlg.CheckRegister())
        {
            return false;
        }
    }


    auto devices = NLECaptureManger::GetInstance()->GetDevicesFromType(eDTUnRegister);
    if (devices.isEmpty())
        return false;

    NLECaptureManger::GetInstance()->GetICaptureMgr()->CreateCaptureStream(&devices[0], &pCaptureStream);

    if (pCaptureStream == nullptr)
        return false;

    memset(&pCaptureStreamSetting, 0, sizeof(pCaptureStreamSetting));
        
    pCaptureStreamSetting.uStreamType = ST_VIDEO;
    pCaptureStreamSetting.video.dwFourCC = 'GERU';
    
    pCaptureStreamSetting.vidUnreg.pPngData = (BYTE*)m_unregisterPNG.data();
    pCaptureStreamSetting.vidUnreg.nPngDataSize = m_unregisterPNG.size();

    return true;
}

bool DesktopData::GetDeviceEnabled()
{
	return SettingsDataManager::GetInstance()->GetScreenChecked() && SettingBaseData::GetDeviceEnabled();
}

DesktopData::CaputerType DesktopData::GetType()
{
	return m_type;
}

void DesktopData::SetType(CaputerType type)
{
	m_type = type;
}

float DesktopData::GetFramerate()
{
	return m_framerate;
}

void DesktopData::SetFramerate(float framerate)
{
	m_framerate = framerate;
}

QRect DesktopData::GetCropRect()
{
	return m_rect;
}

void DesktopData::SetCropRect(QRect rect)
{
	m_rect = rect;
}

void DesktopData::Clear()
{
    m_enabled = true;
    m_type = CaputerType_Custom;
    m_framerate = 25;// GetDesktopMaxFramerate();
}

int DesktopData::GetDesktopMaxFramerate()
{
    return ::GetDeviceCaps(GetDC(nullptr), VREFRESH);
}

//////////////////////////////////////////////////////////////////////////
static LPBYTE GetBitmapData(HBITMAP hBmp, BITMAP &bmp)
{
	if (!hBmp)
		return NULL;

	if (GetObject(hBmp, sizeof(bmp), &bmp) != 0) {
		UINT bitmapDataSize = bmp.bmHeight*bmp.bmWidth*bmp.bmBitsPixel;
		bitmapDataSize >>= 3;

		LPBYTE lpBitmapData = (LPBYTE)malloc(bitmapDataSize);
		GetBitmapBits(hBmp, bitmapDataSize, lpBitmapData);

		return lpBitmapData;
	}

	return NULL;
}
static inline BYTE BitToAlpha(LPBYTE lp1BitTex, int pixel, bool bInvert)
{
	BYTE pixelByte = lp1BitTex[pixel / 8];
	BYTE pixelVal = pixelByte >> (7 - (pixel % 8)) & 1;

	if (bInvert)
		return pixelVal ? 0xFF : 0;
	else
		return pixelVal ? 0 : 0xFF;
}

LPBYTE GetCursorData(HICON hIcon, ICONINFO &ii, UINT &width, UINT &height)
{
	BITMAP bmpColor, bmpMask;
	LPBYTE lpBitmapData = NULL, lpMaskData = NULL;

	if (lpBitmapData = GetBitmapData(ii.hbmColor, bmpColor)) {
		if (bmpColor.bmBitsPixel < 32) {
			free(lpBitmapData);
			return NULL;
		}

		if (lpMaskData = GetBitmapData(ii.hbmMask, bmpMask)) {
			int pixels = bmpColor.bmHeight*bmpColor.bmWidth;
			bool bHasAlpha = false;

			//god-awful horrible hack to detect 24bit cursor
			for (int i = 0; i < pixels; i++) {
				if (lpBitmapData[i * 4 + 3] != 0) {
					bHasAlpha = true;
					break;
				}
			}

			if (!bHasAlpha) {
				for (int i = 0; i < pixels; i++) {
					lpBitmapData[i * 4 + 3] = BitToAlpha(lpMaskData, i, false);
				}
			}

			free(lpMaskData);
		}

		width = bmpColor.bmWidth;
		height = bmpColor.bmHeight;
	}
	else if (lpMaskData = GetBitmapData(ii.hbmMask, bmpMask)) {
		bmpMask.bmHeight /= 2;

		int pixels = bmpMask.bmHeight*bmpMask.bmWidth;
		lpBitmapData = (LPBYTE)malloc(pixels * 4);
		ZeroMemory(lpBitmapData, pixels * 4);

		UINT bottom = bmpMask.bmWidthBytes*bmpMask.bmHeight;

		for (int i = 0; i < pixels; i++) {
			BYTE transparentVal = BitToAlpha(lpMaskData, i, false);
			BYTE colorVal = BitToAlpha(lpMaskData + bottom, i, true);

			if (!transparentVal)
				lpBitmapData[i * 4 + 3] = colorVal; //as an alternative to xoring, shows inverted as black
			else
				*(LPDWORD)(lpBitmapData + (i * 4)) = colorVal ? 0xFFFFFFFF : 0xFF000000;
		}

		free(lpMaskData);

		width = bmpMask.bmWidth;
		height = bmpMask.bmHeight;
	}

	return lpBitmapData;
}
MouseData::MouseData(QObject *parent)
	:SettingBaseData(parent)
{
	m_Iimg.load(":/FSRecorder/Resources/light/mouse_i.png");
	m_CommonCursor.load(":/FSRecorder/Resources/light/aero_arrow.png", "png");
	m_CaptureDeviceInfo = nullptr;
	m_pCurStream = nullptr;
	ZeroMemory(&m_preFrame, sizeof(Mouse_Frame));
	m_preFrame.hs_x = -1;
	m_preFrame.hs_y = -1;
	QList<CaptureDeviceInfo> lst = NLECaptureManger::GetInstance()->GetDevicesFromType(eDTCurser);
	if (!lst.isEmpty())
	{
		m_CaptureDeviceInfo = new CaptureDeviceInfo;
		*m_CaptureDeviceInfo = lst[0];
	}


	LPCTSTR CursorResArray[MAX_CURSOR_TYPE] =
	{
		IDC_APPSTARTING,
		IDC_ARROW,
		IDC_CROSS,
		IDC_HAND,
		IDC_HELP,
		IDC_IBEAM,
		IDC_ICON,
		IDC_NO,
		IDC_SIZE,
		IDC_SIZEALL,
		IDC_SIZENESW,
		IDC_SIZENS,
		IDC_SIZENWSE,
		IDC_SIZEWE,
		IDC_UPARROW,
		IDC_WAIT
	};
	for (int i = 0; i < MAX_CURSOR_TYPE; i++)
	{
		m_CursorResArray[i] = CursorResArray[i];
		m_CursorHandleArray[i] = LoadCursor(NULL, CursorResArray[i]);
	}
}

MouseData::~MouseData()
{
	SAFE_DELETE(m_CaptureDeviceInfo);
}

bool MouseData::GetCaptureStream(NLEComPtr<IUnknown> &pCaptureStream, CaptureStreamSetting &pCaptureStreamSetting)
{
	if (!GetDeviceEnabled()) return false;
	
	m_pCurStream = nullptr;
	if (!m_CaptureDeviceInfo) return false;
	HRESULT hr = NLECaptureManger::GetInstance()->GetICaptureMgr()->CreateCaptureStream(m_CaptureDeviceInfo, &pCaptureStream);
	if (SUCCEEDED(hr))
	{
		m_pCurStream = pCaptureStream;
		return true;

	}
	return false;
}

bool MouseData::GetDeviceEnabled()
{
	return SettingsDataManager::GetInstance()->GetDesktopData()->GetDeviceEnabled();
}


NLEComPtr<IMouseStreamCtrl> MouseData::GetIMouseStreamCtrl()
{
	if (!m_pCurStream) return nullptr;
	NLEComPtr<IMouseStreamCtrl> pMouseStreamCtrl = nullptr;
	if (SUCCEEDED(m_pCurStream->QueryInterface(IID_IMouseStreamCtrl, (void**)&pMouseStreamCtrl)))
	{
		return pMouseStreamCtrl;
	}
	return nullptr;
}

void MouseData::PushMouseMoveFrame(QPoint pt, bool bFirstPush /*= false*/)
{
	auto topleft = RecordHelper::GetInstance()->RecordRect().topLeft() * gGetDevicePixelRatio();
	//pt = pt * gGetDevicePixelRatio();
	pt = pt - topleft;

	NLEComPtr<IMouseStreamCtrl> pMouseStreamCtrl = GetIMouseStreamCtrl();
	if (!pMouseStreamCtrl) return;

	BOOL ret = true;
	CURSORINFO pci;
	ZeroMemory(&pci, sizeof(CURSORINFO));
	pci.cbSize = sizeof(CURSORINFO);
	ret = GetCursorInfo(&pci);
	if (!ret) return;

	ICONINFO iconif;
	ZeroMemory(&iconif, sizeof(ICONINFO));
	ret = GetIconInfo(pci.hCursor, &iconif);
	if (!ret) return;

	Mouse_Frame mouseFrame;
	ZeroMemory(&mouseFrame, sizeof(Mouse_Frame));
	mouseFrame.x = pt.x();
	mouseFrame.y = pt.y();
	mouseFrame.hs_x = iconif.xHotspot;
	mouseFrame.hs_y = iconif.yHotspot;

	LPBYTE pdata = nullptr;
	if (bFirstPush ||
		(iconif.xHotspot != m_preFrame.hs_x || iconif.yHotspot != m_preFrame.hs_y))
	{
		//m_tempImg = GetCursorImg(iconif);
		//if (m_tempImg.isNull())
		//{
		//	DestroyCursor(pci.hCursor);
		//	::DeleteObject(iconif.hbmColor);
		//	::DeleteObject(iconif.hbmMask);
		//	return;
		//}
		//mouseFrame.size = m_tempImg.width() * m_tempImg.height() * 4;
		//mouseFrame.icon_data = (int8_t *)m_tempImg.constBits();
		//mouseFrame.w = m_tempImg.width();
		//mouseFrame.h = m_tempImg.height();
		//m_preFrame = mouseFrame;

		UINT w, h;
		pdata = GetCursorData(pci.hCursor, iconif, w, h);

		mouseFrame.size = w * h * 4;
		mouseFrame.icon_data = (int8_t *)pdata;
		mouseFrame.w = w;
		mouseFrame.h = h;
		m_preFrame = mouseFrame;
	}
	else
	{
		mouseFrame.size = 0;
		mouseFrame.icon_data = 0;
		mouseFrame.w = m_preFrame.w;
		mouseFrame.h = m_preFrame.h;
	}
	if (pdata) free(pdata);
	DestroyCursor(pci.hCursor);
	::DeleteObject(iconif.hbmColor);
	::DeleteObject(iconif.hbmMask);
	pMouseStreamCtrl->PushFrame(&mouseFrame);
}

void MouseData::PushLButtonClickedFrame(QPoint pt)
{
	auto topleft = RecordHelper::GetInstance()->RecordRect().topLeft() * gGetDevicePixelRatio();
	//pt = pt * gGetDevicePixelRatio();
	pt = pt - topleft;
	//pt = pt - RecordHelper::GetInstance()->RecordRect().topLeft();
	NLEComPtr<IMouseStreamCtrl> pMouseStreamCtrl = GetIMouseStreamCtrl();
	if (!pMouseStreamCtrl) return;
	Mouse_Frame mouseFrame;
	memset(&mouseFrame, 0, sizeof(Mouse_Frame));
	mouseFrame.x = pt.x();
	mouseFrame.y = pt.y();
	mouseFrame.event = 1;
	pMouseStreamCtrl->PushFrame(&mouseFrame);
}

void MouseData::PushCurrentCursorFrame()
{
	CURSORINFO pci;
	ZeroMemory(&pci, sizeof(CURSORINFO));
	pci.cbSize = sizeof(CURSORINFO);
	GetCursorInfo(&pci);
	PushMouseMoveFrame(QPoint(pci.ptScreenPos.x,pci.ptScreenPos.y),true);
}

extern QImage qt_imageFromWinHBITMAP(HDC hdc, HBITMAP bitmap, int w, int h);
QImage MouseData::GetCursorImg(const ICONINFO &iconinfo)
{
	qDebug() << "type" << iconinfo.fIcon;
	HDC hdc = GetDC(0);
	if (hdc == nullptr) return QImage();

	if (iconinfo.hbmMask && !iconinfo.hbmColor)  
	{
		if (iconinfo.xHotspot == 8 && iconinfo.yHotspot == 9) // I
			return m_Iimg;
		return m_CommonCursor;
	}

	HDC memdc = CreateCompatibleDC(hdc);
	HDC memdc1 = CreateCompatibleDC(hdc);
	HDC memdc2 = CreateCompatibleDC(hdc);
	SelectObject(memdc, iconinfo.hbmMask);//白底黑鼠标
	SelectObject(memdc1, iconinfo.hbmColor);//黑底彩色鼠标
	BITMAP bmp;
	GetObject(iconinfo.hbmMask, sizeof(BITMAP), (LPBYTE)&bmp);

	int bitmapW = bmp.bmWidth;
	int bitmapH = bmp.bmHeight;

	HBITMAP bitmap = CreateCompatibleBitmap(hdc, bitmapW, bitmapH);
	SelectObject(memdc2, bitmap);

	BitBlt(memdc2, 0, 0, bitmapW, bitmapH, memdc, 0, 0, SRCAND);
	BitBlt(memdc2, 0, 0, bitmapW, bitmapH, memdc1, 0, 0, SRCPAINT);
	QImage img = qt_imageFromWinHBITMAP(hdc, bitmap, bitmapW, bitmapH);

	ReleaseDC(0, memdc);
	ReleaseDC(0, memdc1);
	ReleaseDC(0, memdc2);
	ReleaseDC(0, hdc);
	return img;
}



//////////////////////////////////////////////////////////////////////////

GameData::GameData(QObject *parent)
: SettingBaseData(parent)
{
    InitWSCaptureHookPath();

    qRegisterMetaType<ProcessData>("ProcessData");
    m_gameBlackWhiteList = new GameBlackWhitelist(this);

    Clear();
    //InitGameBlackWhitelist();

}

bool GameData::GetCaptureStream(NLEComPtr<IUnknown> &pCaptureStream, CaptureStreamSetting &captureStreamSetting)
{
	if (!GetDeviceEnabled()) 
		return false;

	if (m_captureStream)
	{
		pCaptureStream = m_captureStream;
		memset(&captureStreamSetting, 0, sizeof(captureStreamSetting));
		captureStreamSetting.uStreamType = ST_VIDEO;
        captureStreamSetting.video.dwFourCC = SettingsDataManager::GetInstance()->GetAdvancedData()->GetSuportGPUParam();
		captureStreamSetting.video.dFrameRate = GetFramerate();
		captureStreamSetting.video.nWidth = 0;
		captureStreamSetting.video.nHeight = 0;
		captureStreamSetting.vidEffect.cropRect.left = 0;
		captureStreamSetting.vidEffect.cropRect.right = 0;
		captureStreamSetting.vidEffect.cropRect.top = 0;
		captureStreamSetting.vidEffect.cropRect.bottom = 0;
		return true;
	}
	else
		return false;
}


bool GameData::GetDeviceEnabled()
{
	return SettingsDataManager::GetInstance()->GetScreenChecked() && SettingBaseData::GetDeviceEnabled() && m_captureStream;
}

#include "cGameStreamCtrl.h"

bool GameData::SetGamenName(const ProcessData &data)
{

    m_processData = data;
	m_captureStream = nullptr;
    m_gameName = data.name;
    m_gameHwnd = data.hwnd;
    QString processID = QString::number(data.processId);
    m_gameProcessId = data.processId;

	if (processID.isEmpty())
		return false;

	auto devices = NLECaptureManger::GetInstance()->GetDevicesFromType(EDeviceType::eDTGame);
	if (devices.isEmpty())
		return false;

	NLEComPtr<IUnknown> captureStream;
	NLECaptureManger::GetInstance()->GetICaptureMgr()->CreateCaptureStream((CaptureDeviceInfo*)&devices[0], &captureStream);


	NLEComPtr<IGameStreamCtrl> pGameStream;
	captureStream->QueryInterface(IID_IGameStreamCtrl, (void **)&pGameStream); // 获取流接口

	auto gameData = QString("{\"pid\":%1, \"hookdllpath\":\"%2\"}").arg(processID).arg(m_WSCaptureHookPath).toStdWString();
	if (SUCCEEDED(pGameStream->SetTargetInfo(const_cast<wchar_t*>(gameData.c_str()), gameData.size())))
	{
		m_captureStream = captureStream;
		SetFramerate(GetFramerate());
	}
	else
	{
		m_gameName.clear();
		return false;
	}

	return true;
}


QString GameData::GetGameName()
{
	return m_gameName;
}

QString GameData::GetGameProcessName()
{
    return m_processData.processName;
}

float GameData::GetFramerate()
{
	return m_framerate;
}

void GameData::SetFramerate(float framerate)
{
	m_framerate = framerate;
	if (m_captureStream)
	{
		NLEComPtr<IGameStreamCtrl> pGameStream;
		m_captureStream->QueryInterface(IID_IGameStreamCtrl, (void **)&pGameStream); // 获取流接口
		pGameStream->SetFramerate(framerate);
	}
}

QSize GameData::GetSize()
{
    QSize size(1920, 1080);
    if (m_captureStream)
    {
        NLEComPtr<IGameStreamCtrl> pGameStream;
        m_captureStream->QueryInterface(IID_IGameStreamCtrl, (void **)&pGameStream); // 获取流接口
        int w = 0;
        int h = 0;
        pGameStream->GetSize(&w, &h);
        size = QSize(w, h);
    }
    return size;
}

void GameData::ShowActiveGameWindow()
{
	// 设置为1，进行录制
	SetStreamType(1);

	typedef    void    (WINAPI    *PROCSWITCHTOTHISWINDOW)    (HWND, BOOL);
	PROCSWITCHTOTHISWINDOW    SwitchToThisWindow;
	HMODULE    hUser32 = GetModuleHandle(L"user32");
	SwitchToThisWindow = (PROCSWITCHTOTHISWINDOW)GetProcAddress(hUser32, "SwitchToThisWindow");
    

    if (SwitchToThisWindow)
	{
        // 游戏窗口到全屏，窗口句柄会被不一样，失败重新查找句柄
		SwitchToThisWindow(m_gameHwnd, TRUE);
        auto dwError = GetLastError();
        if (dwError != 0)
        {
            m_gameHwnd = ProcessManager::FindMainWindow(m_gameProcessId);
            SwitchToThisWindow(m_gameHwnd, TRUE);
        }
	}
	else
	{
		SetForegroundWindow(m_gameHwnd);
		ShowWindow(m_gameHwnd, SW_NORMAL);

	}
	
    // 设置录制等待时间，超过此时间刚认为无法获取游戏数据，游戏录制失败
    if (m_captureStream)
    {
        NLEComPtr<IGameStreamCtrl> pGameStream;
        m_captureStream->QueryInterface(IID_IGameStreamCtrl, (void **)&pGameStream); // 获取流接口

        QList<QString> badGames;
        badGames.push_back("csgo.exe");
        badGames.push_back("overwatch.exe");
        if (badGames.contains(m_processData.processName.toLower()))
        {
            pGameStream->SetRecordWaitTime(20000);
        }

    }
    
}

bool GameData::IsGameFullScreen()
{
    if (m_gameHwnd)
    {
        RECT rc;
        ::GetWindowRect(m_gameHwnd, &rc);
        QRect gameRect(rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top);
        QRect desktopRect = QApplication::desktop()->screenGeometry();
        auto foregroundHwnd = GetForegroundWindow();
        return gameRect == desktopRect /*&& foregroundHwnd == m_gameHwnd*/;
    }
    else
        return false;
}

Q_INVOKABLE void GameData::DevicesChanged()
{
	QString gameName = GetGameName();
    SetGamenName(ProcessData());
	emit signalCurrentDeviceLost(gameName);
}

void GameData::SetStreamType(int type)
{
	if (m_captureStream)
	{
		NLEComPtr<IGameStreamCtrl> pGameStream;
		m_captureStream->QueryInterface(IID_IGameStreamCtrl, (void **)&pGameStream); // 获取流接口
		pGameStream->SetStreamType(type);
	}
}

QList<QString> GameData::GetGameBlacklist()
{
    return m_gameBlackWhiteList->GetGameBlacklist();
}

QList<QString> GameData::GetGameWhitelist()
{
    return m_gameBlackWhiteList->GetGameWhitelist();
}

void GameData::SetHaveGameRecord()
{
    m_markHaveGameRecord = GetDeviceEnabled();
}

bool GameData::GetHaveGameRecord()
{
    return m_markHaveGameRecord;
}


void GameData::InitGameBlackWhitelist()
{
    QString path = FSGetFilmoraScreenDirectory();
    path += "/GameBlackWhiteList.xml";
    if (!FileOperation::IsFileExist(path))
    {
        FileOperation::CreateNewFile(path);
        QFile file(path);
        if (file.open(QIODevice::WriteOnly))
        {
            QString n("\n\t");
            QByteArray explample;
            auto AddTag = [&](QString text) {
                explample += text + n;
            };
            auto AddLine = [&](QString text) {
                explample += QString("<Game>%1</Game>").arg(text) + n;
            };

            AddTag("<GameBlackWhiteList>");
            //白名单
            AddTag("<WhiteList>");
            AddLine("csgo.exe");
            AddLine("overwatch.exe");
            AddTag("</WhiteList>");
            // 黑名单
            AddTag("<BlackList>");
            AddLine("chrome.exe");
           // AddTag("</BlackList>");

            explample += QString("</BlackList>") + "\n";
            explample += "</GameBlackWhiteList>";

            file.write(explample);
            file.close();
        }

    }


    m_gameBlacklist.push_back("");
    m_gameBlacklist.push_back(QString(QApplication::applicationName() + QString(".exe")).toLower());
    m_gameBlacklist.push_back(QString("fseditor.exe"));

    // 过滤常用应用
    m_gameBlacklist << "";
    m_gameBlacklist << QString(QApplication::applicationName() + QString(".exe")).toLower();
    m_gameBlacklist << "fseditor.exe";
    m_gameBlacklist << "chrome.exe";
    m_gameBlacklist << "firefox.exe";
    m_gameBlacklist << "iexplore.exe";
    m_gameBlacklist << "explorer.exe";
    m_gameBlacklist << "opera.exe";
    m_gameBlacklist << "winword.exe";
    m_gameBlacklist << "excel.exe";
    m_gameBlacklist << "powerpnt.exe";
    m_gameBlacklist << "acrord32.exe";
    m_gameBlacklist << "notepad.exe";
    m_gameBlacklist << "notepad++.exe";
    m_gameBlacklist << "filmora.exe";
    m_gameBlacklist << "itunes.exe";
    m_gameBlacklist << "foxmail.exe";

    m_gameBlacklist << "squishide.exe";
    m_gameBlacklist << "squishserver.exe";
    m_gameBlacklist << "communicator.exe";
    m_gameBlacklist << "ucbrowser.exe";
    m_gameBlacklist << "devenv.exe";


    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
        return ;

    QDomDocument doc;
    QString errorMsg;
    if (!doc.setContent(&file, &errorMsg))
        return ;

    auto docElem = doc.documentElement();

    auto whiteListElem = docElem.elementsByTagName("WhiteList");
    if (!whiteListElem.isEmpty())
    {
        auto elems = whiteListElem.at(0).childNodes();
        for (int i = 0; i < elems.size(); ++i)
        {
            QDomElement tempElem = elems.at(i).toElement();
            auto game = tempElem.text().toLower();
            if (!game.endsWith(QString(".exe")))
                game += QString(".exe");
            if (!m_gameWhitelist.contains(game))
                m_gameWhitelist.push_back(game);

        }
    }
    auto blackListElem = docElem.elementsByTagName("BlackList");
    if (!blackListElem.isEmpty())
    {
        auto elems = blackListElem.at(0).childNodes();
        for (int i = 0; i < elems.size(); ++i)
        {
            QDomElement tempElem = elems.at(i).toElement();
            auto game = tempElem.text().toLower();
            if (!game.endsWith(QString(".exe")))
                game += QString(".exe");
            if (!m_gameBlacklist.contains(game))
                m_gameBlacklist.push_back(game);

        }
    }

}

void GameData::Clear()
{
    m_gameName = "CS";
    m_framerate = 60.0f;
    m_ICapturePlayer = nullptr;
    m_markHaveGameRecord = false;
    m_gameHwnd = 0;
    
}

void GameData::InitWSCaptureHookPath()
{
    // 将WSCaptureHook.dll WSCaptureHook_64.dll拷贝到临时目录，游戏注入时使用此处的DLL，如找不到此处的DLL将使用FilmoraScreen程序目录的DLL
    // 解决当进行游戏录制时进行FilmoraScreen升级、安装无法删除WSCaptureHook.dll的问题
    QString hookDir = FSGetFilmoraScreenDirectory() + "/WSCaptureHook";
    QDir dir(hookDir);
    if (!dir.exists())
    {
        dir.mkpath(hookDir);
    }
    QString WSCaptureHookPath = QApplication::applicationDirPath() + "/WSCaptureHook.dll";
    QString WSCaptureHookPath_64 = QApplication::applicationDirPath() + "/WSCaptureHook_64.dll";

    QString copyToWSCaptureHookPath = hookDir + "/WSCaptureHook.dll";
    QString copyToWSCaptureHookPath_64 = hookDir + "/WSCaptureHook_64.dll";

    auto CopyToWSCaptureHook = [](QString path, QString copyToPath)->bool{
        QFile file(path);
        if (file.open(QIODevice::ReadOnly))
        {
            QFile toFile(copyToPath);
            if (toFile.open(QIODevice::ReadWrite))
            {
                qint64 copySize = toFile.write(file.readAll());
                return copySize == file.size();
            }
        }
        return false;
    };

    CopyToWSCaptureHook(WSCaptureHookPath, copyToWSCaptureHookPath);
    CopyToWSCaptureHook(WSCaptureHookPath_64, copyToWSCaptureHookPath_64);

    if (QFile::exists(WSCaptureHookPath) && QFile::exists(WSCaptureHookPath_64))
        m_WSCaptureHookPath = hookDir.replace("\\", "/");
        
    
}

void GameData::Start()
{
	if (m_ICapturePlayer)
	{
		SetStreamType(0);
		bool ret = SUCCEEDED(m_ICapturePlayer->Start());
	}
}

void GameData::Stop()
{
	if (m_ICapturePlayer)
	{
		m_ICapturePlayer->ClearAllViews();
		m_ICapturePlayer->Stop();
		m_ICapturePlayer->SetCaptureStream(nullptr);
	}
}

bool GameData::AddHWnd(HWND winId)
{
	if (m_captureStream == nullptr)
		return false;

	bool newStream = false;
	if (m_ICapturePlayer == nullptr)
	{
		newStream = true;
		NLEComPtr<ICapturePlayer> capturePlayerTemp;
		WSCreateInstance::Instance()->wsCreateInstance(CLSID_CCapturePlayer, nullptr, 0, IID_ICapturePlayer, (void**)&m_ICapturePlayer);
		if (m_ICapturePlayer == nullptr)
			return false;

		m_ICapturePlayer->Init();
	}


	SetStreamType(0);
	m_ICapturePlayer->SetCaptureStream(m_captureStream);

	int id;
	m_ICapturePlayer->SetDisplayWindow(winId, &id);

	return true;
}

//////////////////////////////////////////////////////////////////////////

CameraData::CameraData(QObject *parent)
:SettingBaseData(parent)
{
	

    Clear();
}


CameraData::~CameraData()
{
	Stop();
	m_captureStream = nullptr;
}

bool CameraData::GetCaptureStream(NLEComPtr<IUnknown> &pCaptureStream, CaptureStreamSetting &captureStreamSetting)
{
	if (!GetDeviceEnabled()) return false;

	if (m_captureStream)
	{
		pCaptureStream = m_captureStream;
		memset(&captureStreamSetting, 0, sizeof(captureStreamSetting));
		captureStreamSetting.uStreamType = ST_VIDEO;
		captureStreamSetting.video.dwFourCC = '462C';
		captureStreamSetting.video.dFrameRate = GetFramerate();
		captureStreamSetting.video.nWidth = 0;
		captureStreamSetting.video.nHeight = 0;
		captureStreamSetting.vidEffect.cropRect.left = 0;
		captureStreamSetting.vidEffect.cropRect.right = 0;
		captureStreamSetting.vidEffect.cropRect.top = 0;
		captureStreamSetting.vidEffect.cropRect.bottom = 0;
		return true;
	}
	else
		return false;
	//NLEComPtr<ICaptureStreamEx> p;
	//m_captureStream->QueryInterface(IID_ICaptureStreamEx, (void**)&p);
	//if (p)
	//{
	//	p->Init();
	//	p->Start();
	//}
}

bool CameraData::GetDeviceEnabled()
{
	auto device = GetCurrentDevice();
	bool cameraEnabled = !QString::fromWCharArray(device.deviceID).isEmpty();
	return ((SettingsDataManager*)parent())->GetCameraChecked() && cameraEnabled;
}

bool CameraData::GetDeviceSelected()
{
	auto device = GetCurrentDevice();
	return!QString::fromWCharArray(device.deviceID).isEmpty();
}

CaptureDeviceInfo CameraData::GetCurrentDevice()
{
	return m_curDevice;
}

void CameraData::SetCurrentDevice(const CaptureDeviceInfo &device)
{
	if (device != m_curDevice)
	{
		//sPrintAddRef(m_captureStream);
		m_captureStream = nullptr;
		NLECaptureManger::GetInstance()->GetICaptureMgr()->CreateCaptureStream((CaptureDeviceInfo*)&device, &m_captureStream);
		
		NLEComPtr<ICaptureStream> pCaptureStream;
		m_captureStream->QueryInterface(IID_ICaptureStreamEx, (void **)&pCaptureStream); // 获取流接口
		pCaptureStream->Init(); 

		//SetCaptureStreamProperty(m_captureStream, QString::fromWCharArray(device.displayName));
		
	}

	m_curDevice = device;
}



CaptureDeviceInfos CameraData::GetDevices()
{
	return m_devices;
}

void CameraData::SetCaptureStreamProperty(NLEComPtr<IUnknown> captureStream, QString deviceName)
{
	NLEComPtr<IVideoStreamCtrl> streamCtrl;
	NLEComPtr<ICaptureStream> pCaptureStream;

	captureStream->QueryInterface(IID_ICaptureStreamEx, (void **)&pCaptureStream); // 获取流接口
	pCaptureStream->Init(); 
	captureStream->QueryInterface(IID_IVideoStreamCtrl, (void **)&streamCtrl); // 获取视频控制接口

	QSize size;// = OptionDataManager::GetInstance()->GetWebCamDeviceData()->GetResolution(deviceName);
	float framerate = 0;// = OptionDataManager::GetInstance()->GetWebCamDeviceData()->GetFrameRate(deviceName);

	if (!size.isEmpty())
		streamCtrl->SetSize(size.width(), size.height());
	if (framerate > 1.0f)
		streamCtrl->SetFramerate(framerate);
}


void CameraData::Clear()
{
    m_enabled = false;
    m_framerate = 25.0f;
    memset(&m_curDevice, 0, sizeof(m_curDevice));
    BuildDevice(false);
    SetFramerate(GetFramerate());
}

void CameraData::SetCurrentDevice(QString deviceName)
{
	for (auto temp : GetDevices())
	{
		if (QString::fromWCharArray(temp.displayName) == deviceName)
		{
			SetCurrentDevice(temp);
			break;
		}
	}
}


QSize CameraData::GetResolution()
{
	int32_t w = 0, h = 0;
	if (m_captureStream)
	{
		
		NLEComPtr<IVideoStreamCtrl> streamCtrl;
		m_captureStream->QueryInterface(IID_IVideoStreamCtrl, (void **)&streamCtrl);
		streamCtrl->GetSize(&w, &h);
	}

	return QSize(w, h);
}

QList<QSize> CameraData::GetResolutions()
{
	QList<QSize> resolutions;
	
	if (m_captureStream)
	{
		int32_t wList[128], hList[128], sz_cnt = 128;
		NLEComPtr<IVideoStreamCtrl> streamCtrl;
		m_captureStream->QueryInterface(IID_IVideoStreamCtrl, (void **)&streamCtrl);
		streamCtrl->GetSizeList(wList, hList, &sz_cnt);

		for (int i = 0; i < sz_cnt; ++i)
		{
			resolutions.push_back(QSize(wList[i], hList[i]));
		}
	}

	return resolutions;
}

void CameraData::SetResolution(QSize size)
{
	if (m_captureStream)
	{
		m_resolution = size;

		NLEComPtr<IVideoStreamCtrl> streamCtrl;
		m_captureStream->QueryInterface(IID_IVideoStreamCtrl, (void **)&streamCtrl);
		streamCtrl->SetSize(size.width(), size.height());
        
    }
    
}

float CameraData::GetFramerate()
{
	if (m_captureStream)
	{
		NLEComPtr<IVideoStreamCtrl> streamCtrl;
		m_captureStream->QueryInterface(IID_IVideoStreamCtrl, (void **)&streamCtrl);
		streamCtrl->GetFramerate(&m_framerate);
	}

	return m_framerate;
}

QList<float> CameraData::GetFramerates()
{
	QList<float> framerates;
	if (m_captureStream)
	{
		float framerateList[128]; int32_t fr_cnt = 128;
		NLEComPtr<IVideoStreamCtrl> streamCtrl;
		m_captureStream->QueryInterface(IID_IVideoStreamCtrl, (void **)&streamCtrl);
		streamCtrl->GetFramerateList(framerateList, &fr_cnt);

		for (int i = 0; i < fr_cnt; ++i)
		{
			framerates.push_back(framerateList[i]);
		}
	}

	return framerates;
}

void CameraData::SetFramerate(float framerate)
{
	m_framerate = framerate;

	if (m_captureStream)
	{
		NLEComPtr<IVideoStreamCtrl> streamCtrl;
		m_captureStream->QueryInterface(IID_IVideoStreamCtrl, (void **)&streamCtrl);
		streamCtrl->SetFramerate(framerate);
	}
}

QSize CameraData::GetDefaultResoluction(const QList<QSize> &resolutions)
{
    QSize defaultResoluction(640, 480);
    if (!resolutions.isEmpty() && !resolutions.contains(defaultResoluction))
        defaultResoluction = resolutions[qMax(0, resolutions.size() / 2 - 1)];

    return defaultResoluction;
}


void CameraData::Start()
{
	if (m_ICapturePlayer)
	{
		bool ret =  SUCCEEDED(m_ICapturePlayer->Start());
	}
}

void CameraData::Stop()
{

    if (m_ICapturePlayer)
	{
        m_ICapturePlayer->Stop();

        // 如果摄像头是异常被关闭的，需要调用ICaptureEngineEx->Stop才可以清流（底层坑）
        if (SettingsDataManager::GetInstance()->GetDeviceException())
            NLECaptureManger::GetInstance()->GetICaptureEngine()->Stop();
        m_ICapturePlayer->ClearAllViews();
		m_ICapturePlayer->SetCaptureStream(nullptr);
	}
}

bool CameraData::AddHWnd(HWND winId)
{
	bool newStream = false;
	if (m_ICapturePlayer == nullptr)
	{
		newStream = true;
		NLEComPtr<ICapturePlayer> capturePlayerTemp;
		WSCreateInstance::Instance()->wsCreateInstance(CLSID_CCapturePlayer, nullptr, 0, IID_ICapturePlayer, (void**)&m_ICapturePlayer);
		if (m_ICapturePlayer == nullptr)
			return false;

		m_ICapturePlayer->Init();
	}


	bool ret = m_ICapturePlayer->SetCaptureStream(m_captureStream);

	int id;
	m_ICapturePlayer->SetDisplayWindow(winId, &id);

	return true;

}

void CameraData::DevicesChanged(bool curDeviceLost)
{

    BuildDevice(curDeviceLost);
	emit signalDeviceChagned();
}

void CameraData::BuildDevice(bool curDeviceLost)
{
	bool preDeviceEnabled = GetDeviceEnabled();
	m_devices = NLECaptureManger::GetInstance()->GetDevicesFromType(EDeviceType::eDTWebCam);
	auto curDevice = GetCurrentDevice();
	if (!m_devices.isEmpty())
	{
		bool deviceExits = false;
		for (auto temp : GetDevices())
		{
			if (temp == curDevice)
			{
				deviceExits = true;
				break;
			}
		}
		if (!deviceExits)
		{
			SetCurrentDevice(m_devices[0]);
            auto resoluctions = GetResolutions();
            SetResolution(GetDefaultResoluction(resoluctions));

		}
		SetDeviceEnabled(true);
	}
	else
	{
		memset(&m_curDevice, 0, sizeof(m_curDevice));
		SetDeviceEnabled(false);
	}

    if (curDeviceLost)
    {
        GetParent()->SetDeviceException(true);
        emit signalCurrentDeviceLost(QString::fromWCharArray(curDevice.displayName));
    }
	//// 当前设备异常
	//if (!QString::fromWCharArray(curDevice.displayName).isEmpty() && preDeviceEnabled)
	//{
 //       bool curDeviceLost = true;
	//	for (auto temp : GetDevices())
	//	{
	//		if (temp == curDevice)
	//		{
 //               curDeviceLost = false;
	//			break;
	//		}
	//	}
 //       if (curDeviceLost)
	//	{
 //           GetParent()->SetDeviceException(true);
	//		emit signalCurrentDeviceLost(QString::fromWCharArray(curDevice.displayName));
	//	}
	//}
}

//////////////////////////////////////////////////////////////////////////

AudioData::AudioData(QObject *parent)
:SettingBaseData(parent)
, m_ICaptureAudioVolume(nullptr)
{
	
    Clear();
}

AudioData::~AudioData()
{
	if (m_ICaptureAudioVolume)
	{
		m_ICaptureAudioVolume->Stop();
		m_ICaptureAudioVolume = nullptr;
	}
	m_captureStream = nullptr;
}

bool AudioData::GetCaptureStream(NLEComPtr<IUnknown> &pCaptureStream, CaptureStreamSetting &captureStreamSetting)
{
	if (!GetDeviceEnabled()) return false;

	auto device = GetCurrentDevice();
	return GetCaptureStream(device, pCaptureStream, captureStreamSetting, true);
}

bool AudioData::GetCaptureStream(const CaptureDeviceInfo &device, NLEComPtr<IUnknown> &pCaptureStream, CaptureStreamSetting &captureStreamSetting, bool needVolume)
{
	NLECaptureManger::GetInstance()->GetICaptureMgr()->CreateCaptureStream((CaptureDeviceInfo*)&device, &pCaptureStream);
	if (pCaptureStream)
	{
		m_captureStream = pCaptureStream;

		memset(&captureStreamSetting, 0, sizeof(CaptureStreamSetting));
		captureStreamSetting.uStreamType = ST_AUDIO;
		captureStreamSetting.audio.dwFourCC = ' CAA';
		captureStreamSetting.audio.nBitrate = 128000;
		captureStreamSetting.audio.nChannels = 2;
		captureStreamSetting.audio.nBitsPerSample = 16;
		captureStreamSetting.audio.nSampleRate = 44100;


		// 设置麦克风音量
		if (needVolume)
			SetAudioVolume(GetAudioVolume());
		return true;
	}

	return false;
}

void AudioData::StopRecord()
{
	m_captureStream = nullptr;
}

void AudioData::Clear()
{
    m_bRecordSystemAudio = true;
    if (m_ICaptureAudioVolume)
        m_ICaptureAudioVolume->Stop();
    m_ICaptureAudioVolume = nullptr;
    memset(&m_curDevice, 0, sizeof(m_curDevice));
    m_audioVolume = 1.0f;
    BuildDevice(false, true);
}

bool AudioData::GetSystemAudioCaptureStream(NLEComPtr<IUnknown> &pCaptureStream, CaptureStreamSetting &pCaptureStreamSetting)
{
	if (!GetRecordSystemAudio()) return false;

	auto audios = NLECaptureManger::GetInstance()->GetDevicesFromType(EDeviceType::eDTAudioOutput);
	if (!audios.isEmpty())
	{
		auto audio = audios[0];
		return GetCaptureStream(audio, pCaptureStream, pCaptureStreamSetting, false);
	}

	return false;
}



bool AudioData::GetDeviceEnabled()
{
	return SettingsDataManager::GetInstance()->GetAudioChecked() && SettingBaseData::GetDeviceEnabled();
}

void AudioData::SetAudioVolume(float volume)
{
	m_audioVolume = qBound(0.0f, volume, 1.0f);

	if (m_captureStream)
	{
		NLEComPtr<IAudioVolumeSetting> pIAudioVolumeSetting;
		m_captureStream->QueryInterface(IID_IAudioVolumeSetting, (void**)&pIAudioVolumeSetting);
		pIAudioVolumeSetting->SetVolume(volume);
	}
	if (m_ICaptureAudioVolume)
	{
		NLEComPtr<IAudioVolumeSetting> pIAudioVolumeSetting;
		m_ICaptureAudioVolume->QueryInterface(IID_IAudioVolumeSetting, (void**)&pIAudioVolumeSetting);
		if (pIAudioVolumeSetting)
			pIAudioVolumeSetting->SetVolume(volume);
	}
}

float AudioData::GetAudioVolume()
{

	return m_audioVolume;
}

bool AudioData::GetRecordSystemAudio()
{
	return SettingsDataManager::GetInstance()->GetAudioChecked() && m_bRecordSystemAudio;
}

bool AudioData::GetRecordSystemAudioSelected()
{
	return m_bRecordSystemAudio;
}

void AudioData::SetRecordSystemAudio(bool enabled)
{
	m_bRecordSystemAudio = enabled;
}

CaptureDeviceInfo AudioData::GetCurrentDevice()
{
	return m_curDevice;
}

void AudioData::SetCurrentDevice(const CaptureDeviceInfo &device)
{
	m_curDevice = device;

	if (m_ICaptureAudioVolume)
	{
		m_ICaptureAudioVolume->Stop();
		//sPrintAddRef(m_ICaptureAudioVolume);
		m_ICaptureAudioVolume = nullptr;
	}
	auto mgr = NLECaptureManger::GetInstance()->GetICaptureMgr();
	mgr->GetAudioDeviceVolume(&m_curDevice, &m_ICaptureAudioVolume);
	if (m_ICaptureAudioVolume)
	{
		m_ICaptureAudioVolume->Init(25, [&](void *pUserData, int32_t nMsg, int32_t wParam, int32_t lParam)->HRESULT{
			AudioData *pThis = (AudioData*)pUserData;
			//if (pThis->GetDeviceEnabled())
			{
				pThis->signalVolumeChanged(wParam);
			}

			return 0;
		}, this);

		SetAudioVolume(GetAudioVolume());
		m_ICaptureAudioVolume->Start();
	}
}

CaptureDeviceInfos AudioData::GetDevices()
{
	return m_devices;
}

void AudioData::DevicesChanged(bool curDeviceLost)
{
    BuildDevice(curDeviceLost, false);
	emit signalDeviceChagned();
}


void AudioData::SetCurrentDevice(QString deviceName)
{
	for (auto temp : GetDevices())
	{
		if (QString::fromWCharArray(temp.displayName) == deviceName)
		{
			SetCurrentDevice(temp);
			break;
		}
	}
}


void AudioData::BuildDevice(bool curDeviceLost, bool bFirstInit)
{
	// 获取microphone
    bool preDeviceEnabled = GetParent()->GetAudioChecked() && GetDeviceSelected();

	m_devices = NLECaptureManger::GetInstance()->GetDevicesFromType(EDeviceType::eDTAudioInput);
	bool haveAudioDevice = false;
	auto curDevice = GetCurrentDevice();

	if (!m_devices.isEmpty())
	{
		if (!bFirstInit)
		{
			bool deviceExits = false;
			for (auto temp : GetDevices())
			{
				if (temp == curDevice)
				{
					//SetCurrentDevice(temp);
					deviceExits = true;
					break;
				}
			}
			if (!deviceExits)
			{
                SetCurrentDevice(m_devices[0]);
				//memset(&m_curDevice, 0, sizeof(m_curDevice));
				SetDeviceEnabled(true);
			}
		}
		else
		{
			SetCurrentDevice(m_devices[0]);
			SetDeviceEnabled(true);
		}
		
	}
	else
	{
		memset(&m_curDevice, 0, sizeof(m_curDevice));
		SetDeviceEnabled(false);
	}

    //// 当前设备异常
    if (curDeviceLost)
    {
        GetParent()->SetDeviceException(true);
        emit signalCurrentDeviceLost(QString::fromWCharArray(curDevice.displayName));
    }

}

//////////////////////////////////////////////////////////////////////////

AdvancedData::AdvancedData(QObject *parent)
:SettingBaseData(parent)
, m_enabled(true)
{
	m_recordFilePaht = GetDefaultRecordFiles();

	m_bSuportGPU = true;
    m_bAutoHideRecording = false;

	SetStartStop("F10");
	SetPauseResume("F9");
	SetFullScreen("Shift+F");
    SetAddMarker("F8");

    m_elapsedTimer.start();
    connect(MKCollecter::GetInstance(), &MKCollecter::sigKeyPressed, this, [&](int vk, int mk){

        if (m_enabled && m_elapsedTimer.elapsed() > 2000)
		{
			QString text = MKCollecter::GetInstance()->GetHotKeyString();
			if (text == GetStartStop())
			{
				emit signalStartStop();
			}
			if (text == GetPauseResume())
				emit signalPauseResume();
            if (text == GetFullScreen())
                emit signalFullscreen();
            if (text == GetAddMarker())
                emit signalAddMarker(vk, mk);
		}

	});

}

QString AdvancedData::GetRecordFilePath()
{
	return m_recordFilePaht;
}

void AdvancedData::SetRecordFilePath(QString path)
{
	m_recordFilePaht = path;
}



void AdvancedData::SetScreenSaver(bool enabled)
{
	::SystemParametersInfo(SPI_SETSCREENSAVEACTIVE, enabled, NULL, 0);
}

bool AdvancedData::GetSuportGPU()
{
	return m_bSuportGPU;
}

void AdvancedData::SetSupportGPU(bool enabled)
{
	m_bSuportGPU = enabled;
}

DWORD AdvancedData::GetSuportGPUParam()
{
    return GetSuportGPU() ? 'CEWH' : '462C';
}

void AdvancedData::SetAutoHideRecording(bool b)
{
    m_bAutoHideRecording = b;
}

bool AdvancedData::IsAutoHideRecorder()
{
    return m_bAutoHideRecording;
}

QString AdvancedData::GetStartStop()
{
	return m_StartStop;
}

void AdvancedData::SetStartStop(QString keys)
{
	MKCollecter::GetInstance()->RemoveHotKey(m_StartStop);
	MKCollecter::GetInstance()->AddHotKey(keys);
	m_StartStop = keys;

}

QString AdvancedData::GetPauseResume()
{
	return m_PauseResume;
}

void AdvancedData::SetPauseResume(QString keys)
{
	MKCollecter::GetInstance()->RemoveHotKey(m_PauseResume);
	MKCollecter::GetInstance()->AddHotKey(keys); 
	m_PauseResume = keys;
}

QString AdvancedData::GetFullScreen()
{
	return m_Fullscreen;
}

void AdvancedData::SetFullScreen(QString keys)
{
	MKCollecter::GetInstance()->RemoveHotKey(m_Fullscreen);
	MKCollecter::GetInstance()->AddHotKey(keys);
	m_Fullscreen = keys;
}

QString AdvancedData::GetAddMarker()
{
    return m_AddMarker;
}


void AdvancedData::SetAddMarker(QString keys)
{
    MKCollecter::GetInstance()->RemoveHotKey(m_AddMarker);
    MKCollecter::GetInstance()->AddHotKey(keys);
    m_AddMarker = keys;
}

void AdvancedData::SetHotKeyEnabled(bool enabled)
{
	m_enabled = enabled;
	MKCollecter::GetInstance()->EnalbedHotKeyDeliver(!enabled);


}

QString AdvancedData::GetProjectSaveDirectory()
{
	return m_projectSaveDirectory;
}

void AdvancedData::SetProjectSaveDirectory(QString dir)
{
	m_projectSaveDirectory = dir;
}

void AdvancedData::StartRecord()
{
    m_elapsedTimer.restart();

}

//////////////////////////////////////////////////////////////////////////

// 按钮录制对照表
// http://confluence.wondershare.cn/pages/viewpage.action?pageId=15892617&focusedCommentId=15892788&refresh=1505127970472#comment-15892788

static std::map<int, wchar_t> s_HotKeyMap = { 
    { VK_ESCAPE, 0xE800 },

    { VK_F1, 0xE801 },
    { VK_F2, 0xE802 },
    { VK_F3, 0xE803 },
    { VK_F4, 0xE804 },
    { VK_F5, 0xE805 },
    { VK_F6, 0xE806 },
    { VK_F7, 0xE807 },
    { VK_F8, 0xE808 },
    { VK_F9, 0xE809 },
    { VK_F10, 0xE80A },
    { VK_F11, 0xE81B },
    { VK_F12, 0xE81C },

    { VK_OEM_3, 0xE80D },

    { '1', 0xE80E },
    { '2', 0xE80F },
    { '3', 0xE810 },
    { '4', 0xE811 },
    { '5', 0xE812 },
    { '6', 0xE813 },
    { '7', 0xE814 },
    { '8', 0xE815 },
    { '9', 0xE816 },
    { '0', 0xE817 },

    { VK_OEM_MINUS, 0xE818 },
    { VK_OEM_PLUS, 0xE819 },
    { 0x8, 0xE81A },
    { 0x9, 0xE81B },

    { 'Q', 0xE81C },
    { 'W', 0xE81D },
    { 'E', 0xE81E },
    { 'R', 0xE81F },
    { 'T', 0xE820 },
    { 'Y', 0xE821 },
    { 'U', 0xE822 },
    { 'I', 0xE823 },
    { 'O', 0xE824 },
    { 'P', 0xE825 },

    { VK_OEM_4, 0xE826 },
    { VK_OEM_6, 0xE827 },
    { VK_OEM_5, 0xE828 },

    { VK_CAPITAL, 0xE829 },
    { 'A', 0xE82A },
    { 'S', 0xE82B },
    { 'D', 0xE82C },
    { 'F', 0xE82D },
    { 'G', 0xE82E },
    { 'H', 0xE82F },
    { 'J', 0xE830 },
    { 'K', 0xE831 },
    { 'L', 0xE832 },

    { VK_OEM_1, 0xE833 },
    { VK_OEM_7, 0xE834 },

    { 0xD, 0xE8E835 }, //test enter



    { VK_LSHIFT, 0xE8E836 },
    { 'Z', 0xE8E837 },
    { 'X', 0xE8E838 },
    { 'C', 0xE8E839 },
    { 'V', 0xE8E83A },
    { 'B', 0xE8E83B },
    { 'N', 0xE8E83C },
    { 'M', 0xE8E83D },

    { VK_OEM_COMMA, 0xE8E83E },
    { VK_OEM_PERIOD, 0xE83F },

    { VK_OEM_2, 0xE840 },
    { VK_RSHIFT, 0xE836 },


    { VK_LCONTROL, 0xE842 },
    { VK_LWIN, 0xE843 },
    { VK_LMENU, 0xE844 },
    { VK_SPACE, 0xE845 },
    { VK_RMENU, 0xE844 },
    { VK_RWIN, 0xE843 },
    //{ VK_RMENU, 0xE842 },
    { VK_RCONTROL, 0xE849 },

    { VK_SNAPSHOT, 0xE84A },
    { VK_SCROLL, 0xE84B },
    { VK_PAUSE, 0xE84C },

    { VK_INSERT, 0xE84D },
    { VK_HOME, 0xE84E },
    { VK_PRIOR, 0xE84F },
    { VK_DELETE, 0xE850 },
    { VK_END, 0xE851 },
    { VK_NEXT, 0xE852 },

    { VK_UP, 0xE853 },
    { VK_LEFT, 0xE854 },
    { VK_DOWN, 0xE855 },
    { VK_RIGHT, 0xE856 },

    { VK_NUMLOCK, 0xE857 },
    { VK_DIVIDE, 0xE858 },
    { VK_MULTIPLY, 0xE859 },
    { VK_SUBTRACT, 0xE85A },

    { VK_NUMPAD7, 0xE85B },
    { VK_NUMPAD8, 0xE85C },
    { VK_NUMPAD9, 0xE85D },
    { VK_ADD, 0xE85E },
    { VK_NUMPAD4, 0xE85F },
    { VK_NUMPAD5, 0xE860 },
    { VK_NUMPAD6, 0xE861 },
    { VK_NUMPAD1, 0xE862 },
    { VK_NUMPAD2, 0xE863 },
    { VK_NUMPAD3, 0xE864 },

    { 0xD, 0xE865 }, // padnum enter
    { VK_NUMPAD0, 0xE866 },
    { VK_DECIMAL, 0xE867 },
    //{ '+', 0xE868 },
};

KeyData::KeyData(QObject *parent)
:SettingBaseData(parent)
{
    memset(&m_curDevice, 0, sizeof(m_curDevice));
    auto devices = NLECaptureManger::GetInstance()->GetDevicesFromType(EDeviceType::eDTKeybord);
    if (!devices.isEmpty())
    {
        m_curDevice = devices[0];
        SetDeviceEnabled(true);
    }
    else
        SetDeviceEnabled(false);
}

bool KeyData::GetCaptureStream(NLEComPtr<IUnknown> &pCaptureStream, CaptureStreamSetting &pCaptureStreamSetting)
{
    if (!GetDeviceEnabled()) 
        return false;

    m_captureStream = nullptr;
    m_streamCtrl = nullptr;

    HRESULT hr = NLECaptureManger::GetInstance()->GetICaptureMgr()->CreateCaptureStream(&m_curDevice, &pCaptureStream);
    if (SUCCEEDED(hr))
    {
        m_captureStream = pCaptureStream;
        m_captureStream->QueryInterface(IID_IKeybStreamCtrl, (void**)&m_streamCtrl);
        Q_ASSERT(m_streamCtrl);
        return true;

    }
    return false;
}

bool KeyData::GetDeviceEnabled()
{
    return (GetParent()->GetDesktopData()->GetDeviceEnabled() || GetParent()->GetGameData()->GetDeviceEnabled()) && SettingBaseData::GetDeviceEnabled();

}

void KeyData::PushKey(int vk, int mk)
{
    //// 0x68代表+分隔符
    //int SPERATOR = 0xE868;
    wchar_t ikey[100] = { 0 };
    int i = 0;

    // 组合键判断, 是快捷键
    if (((mk & MKCollecter::ModifyKey_Shift)) ||
        ((mk & MKCollecter::ModifyKey_Alt)) ||
        ((mk & MKCollecter::ModifyKey_Control)) ||
        ((mk & MKCollecter::ModifyKey_Win))
        )
    {
        //if (mk & MKCollecter::ModifyKey_Shift)
        //{
        //    ikey[i++] = s_HotKeyMap[VK_LSHIFT];
        //    ikey[i++] = SPERATOR;
        //}
        //if (mk & MKCollecter::ModifyKey_Control)
        //{
        //    ikey[i++] = s_HotKeyMap[VK_LCONTROL];
        //    ikey[i++] = SPERATOR;

        //}
        //if (mk & MKCollecter::ModifyKey_Alt)
        //{
        //    ikey[i++] = s_HotKeyMap[VK_LMENU];
        //    ikey[i++] = SPERATOR;
        //}
        //if (mk & MKCollecter::ModifyKey_Win)
        //{
        //    ikey[i++] = s_HotKeyMap[VK_LWIN];
        //    ikey[i++] = SPERATOR;

        //}
        //QString vkKeyString = MKCollecter::GetInstance()->GetKeyString(vk);
        //QString shiftString("Shift");
        //QString altString("Alt");
        //QString ctrlString("Ctrl");
        //QString winString("Win"); 
        //if (vkKeyString == shiftString || vkKeyString == altString || vkKeyString == ctrlString || vkKeyString == winString)
        //{
        //    // 当只按了控制健
        //    // 去掉SPERATOR
        //    ikey[qMax(i - 1, 0)] = 0;
        //}
        //else
        //{
        //    ikey[i] = s_HotKeyMap[vk];
        //}
        GetHotKey(vk, mk, ikey);
        PushHotKey(ikey);
    }
    else
    {
        // 普通按键
        ikey[i] = s_HotKeyMap[vk];
        PushNormalKey(ikey);

    }
}

bool KeyData::PushNormalKey(wchar_t key[])
{
    return _PushKey(NLEKeyFrameType::NORMAL, key);
}

bool KeyData::PushHotKey(wchar_t key[])
{
    return _PushKey(NLEKeyFrameType::HOT_KEY, key);

}

bool KeyData::PushMarker(int vk, int mk)
{
    wchar_t ikey[10] = { 0 };
    GetHotKey(vk, mk, ikey);
    return _PushKey(NLEKeyFrameType::MARKER, ikey);
}

bool KeyData::_PushKey(int type, wchar_t key[])
{

    if (!GetDeviceEnabled())
        return false;

    QString output;
    for (int i = 0; i < 10; ++i)
    {
        if (key[i] != 0)
        {
            output += QString("%1").arg((int)key[i], 2, 16, QChar('0'));
        }
        else
            break;
    }
    qDebug() << QString("type:%1 key:%2").arg(type).arg(output);


    NLEKeyBoardFrame frame;
    frame.frameType = (NLEKeyFrameType)type;
	memcpy_s(frame.keyString, 100 * 2, key, 100 * 2);
    //wcscpy_s(frame.keyString, 100, key);
    frame.version = 1;
    HRESULT hr = m_streamCtrl->PushFrame(&frame);

    return SUCCEEDED(hr);
}


void KeyData::GetHotKey(int vk, int mk, wchar_t ikey[])
{
    // 0x68代表+分隔符
    int SPERATOR = 0xE868; 
    int i = 0;
    if (mk & MKCollecter::ModifyKey_Shift)
    {
        ikey[i++] = s_HotKeyMap[VK_LSHIFT];
        ikey[i++] = SPERATOR;
    }
    if (mk & MKCollecter::ModifyKey_Control)
    {
        ikey[i++] = s_HotKeyMap[VK_LCONTROL];
        ikey[i++] = SPERATOR;

    }
    if (mk & MKCollecter::ModifyKey_Alt)
    {
        ikey[i++] = s_HotKeyMap[VK_LMENU];
        ikey[i++] = SPERATOR;
    }
    if (mk & MKCollecter::ModifyKey_Win)
    {
        ikey[i++] = s_HotKeyMap[VK_LWIN];
        ikey[i++] = SPERATOR;

    }
    QString vkKeyString = MKCollecter::GetInstance()->GetKeyString(vk);
    QString shiftString("Shift");
    QString altString("Alt");
    QString ctrlString("Ctrl");
    QString winString("Win");
    if (vkKeyString == shiftString || vkKeyString == altString || vkKeyString == ctrlString || vkKeyString == winString)
    {
        // 当只按了控制健
        // 去掉SPERATOR
        ikey[qMax(i - 1, 0)] = 0;
    }
    else
    {
        ikey[i] = s_HotKeyMap[vk];
    }

}

#include <QtNetwork/QtNetwork>

GameBlackWhitelist::GameBlackWhitelist(QObject *parent)
:QThread(parent)
{
    m_whiteUrl = "http://resource.wondershare.com/002/668/Whitelist.list";
    m_blackUrl = "http://resource.wondershare.com/002/669/Blacklist.list";
    QTimer::singleShot(0, [&](){
        start();
    });
}

QList<QString> GameBlackWhitelist::GetGameBlacklist()
{
    QMutexLocker locker(&m_mutex);
    return m_gameBlacklist;
}

QList<QString> GameBlackWhitelist::GetGameWhitelist()
{
    QMutexLocker locker(&m_mutex);
    return m_gameWhitelist;
}

void GameBlackWhitelist::run()
{
    if(InitFromNetwork())
    {
        SaveXml();
    }
    else
    {
        InitFromXml();
    }
}

bool GameBlackWhitelist::InitFromNetwork()
{

    QByteArray retString;
    auto GetListFun = [&](QString url){
        bool networkNoError = false;
        retString.clear();

        QNetworkAccessManager manager;
        QEventLoop eventLoop;
        connect(&manager, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
        // 线程退出m_reply会被自动析构提
        QNetworkReply *replay = manager.get(QNetworkRequest(QUrl(url)));
        eventLoop.exec();
        if (replay->error() == QNetworkReply::NoError)
        {
            networkNoError = true;
            retString = replay->readAll();
            replay->deleteLater();
        }

        return networkNoError;
    };


    QMutexLocker locker(&m_mutex);
    bool bWhiteList = GetListFun(m_whiteUrl);
    if (bWhiteList)
    {
        //retString = "<WhiteList>"
        //    "<Game>csgo.exe</Game>"
        //    "<Game>hl.exe</Game>"
        //    "<Game>overwatch.exe</Game>"
        //    "</WhiteList>"
        //    ;
        m_gameWhitelist = ParseXMLContent(retString);
    }

    bool bBlackList = GetListFun(m_blackUrl);
    if (bBlackList)
    {
        //retString = "<BlackList>"
        //    "<Game>chrome.exe</Game>"
        //    "</BlackList>"
        //    ;

        m_gameBlacklist = ParseXMLContent(retString);
    }
    AddExtraBlackList();

    return bWhiteList && bBlackList;
}

void GameBlackWhitelist::SaveXml()
{
    QString path = FSGetFilmoraScreenDirectory();
    path += "/GameBlackWhiteList.xml";

    QFile file(path);
    if (file.open(QIODevice::WriteOnly))
    {
        auto List2StringFun = [&](const QList<QString> &l){
            QString temp;
            for (auto game : l)
            {
                temp += QString("<Game>%1</Game>").arg(game);
            }
            return temp;
        };
        QString content(QString("<GameBlackWhiteList><WhiteList>%1</WhiteList><BlackList>%2</BlackList></GameBlackWhiteList>").arg(List2StringFun(m_gameWhitelist)).arg(List2StringFun(m_gameBlacklist)));
        file.write(content.toUtf8());
    }
}

void GameBlackWhitelist::InitFromXml()
{
    QString path = FSGetFilmoraScreenDirectory();
    path += "/GameBlackWhiteList.xml";

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
        return;


    auto FindListFun = [&](const QByteArray &content, const QString &tag)->QByteArray{
        int start = content.indexOf(QString("<%1>").arg(tag));
        int end = content.indexOf(QString("</%1>").arg(tag));
        if (start != -1 && end != -1 && end > start)
        {
            return content.mid(start, end + tag.size() + 3 - start);
        }

        return "";
    };

    QByteArray xmlContent = file.readAll();
    QByteArray whiteList = FindListFun(xmlContent, "WhiteList");
    QByteArray blackList = FindListFun(xmlContent, "BlackList");
    m_gameWhitelist = ParseXMLContent(whiteList);
    m_gameBlacklist = ParseXMLContent(blackList);
    AddExtraBlackList();
}

QList<QString> GameBlackWhitelist::ParseXMLContent(QByteArray xmlContent)
{
    QList<QString> gameList;

    QDomDocument doc;
    QString errorMsg;
    if (!doc.setContent(xmlContent, &errorMsg))
        return gameList;

    auto whiteListElem = doc.documentElement();
    auto elems = whiteListElem.elementsByTagName("Game");
    for (int i = 0; i < elems.size(); ++i)
    {
        QDomElement tempElem = elems.at(i).toElement();
        auto game = tempElem.text().toLower();
        if (!game.endsWith(QString(".exe")))
            game += QString(".exe");
        if (!gameList.contains(game))
            gameList.push_back(game);

    }

    return gameList;
};

void GameBlackWhitelist::AddExtraBlackList()
{
    // 过滤常用应用
    m_gameBlacklist << "";
    m_gameBlacklist << QString(QApplication::applicationName() + QString(".exe")).toLower();
    m_gameBlacklist << "fseditor.exe";
    //m_gameBlacklist << "chrome.exe";
    //m_gameBlacklist << "firefox.exe";
    //m_gameBlacklist << "iexplore.exe";
    //m_gameBlacklist << "explorer.exe";
    //m_gameBlacklist << "opera.exe";
    //m_gameBlacklist << "winword.exe";
    //m_gameBlacklist << "excel.exe";
    //m_gameBlacklist << "powerpnt.exe";
    //m_gameBlacklist << "acrord32.exe";
    //m_gameBlacklist << "notepad.exe";
    //m_gameBlacklist << "notepad++.exe";
    //m_gameBlacklist << "filmora.exe";
    //m_gameBlacklist << "itunes.exe";
    //m_gameBlacklist << "foxmail.exe";

    //m_gameBlacklist << "squishide.exe";
    //m_gameBlacklist << "squishserver.exe";
    //m_gameBlacklist << "communicator.exe";
    //m_gameBlacklist << "ucbrowser.exe";
    //m_gameBlacklist << "devenv.exe";
}
