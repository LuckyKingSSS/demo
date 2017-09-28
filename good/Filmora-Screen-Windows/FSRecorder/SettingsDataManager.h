
/*
*Author:		zhy
*Date:			2016.03.21
*Description:	FilmoraScreen设置面板数据管理
*/

#ifndef OPTIONDATAMANAGER_H
#define OPTIONDATAMANAGER_H


#include <QtCore/QObject>
#include <QtCore/QString>

#include "Include/NLEComPtr.h"

#include "ICaptureSource.h"
#include "ICaptureEngine.h"
#include "Interface/INLEPlayer.h"
#include "ICapturePlayer.h"
#include "cMouseStreamCtrl.h"
#include "inc_FSCommonlib/ExternalAPI.h"
class DesktopData;
class GameData;
class CameraData;
class AudioData;
class AdvancedData;
class MouseData;
class KeyData;
class SettingBaseData;

class SettingsDataManager : public QObject
{
	Q_OBJECT
public:
	static SettingsDataManager *GetInstance();
	void Release();

	DesktopData* GetDesktopData();
	GameData* GetGameData();
	CameraData* GetCameraData();
	AudioData* GetAudioData();
	AdvancedData* GetAdvancedData();
	MouseData* GetMouseData();
    KeyData* GetKeyData();

	// 获取设置XML
	QString GetSettingString();

	// 停止录制调用
	void StopRecord();
    void StartRecord();
    void Clear();
public:
	SettingsDataManager(QObject *parent);
	void SetScreenChecked(bool enabled);
	bool GetScreenChecked();
	void SetAudioChecked(bool enabled);
	bool GetAudioChecked();
	void SetCameraChecked(bool enabled);
	bool GetCameraChecked();

    bool GetDeviceException();
    void SetDeviceException(bool ex);

	void SaveSetting();
private:
	void LoadProjectSetting(QString xml);
	void LoadSetting();
    void InitData();

protected:
	DesktopData *m_desktopData;
    MouseData *m_mouseData;
    KeyData *m_keyData;
	GameData *m_gameData;
	CameraData *m_cameraData;
	AudioData *m_audioData;
	AdvancedData *m_advancedData;
	QList<SettingBaseData*> m_datas;

	bool m_screenChecked;
	bool m_audioChecked;
	bool m_cameraChecked;

    bool m_bDeviceException;
};

class SettingBaseData : public QObject
{
	Q_OBJECT
public:
	SettingBaseData(QObject *parent):QObject(parent){ m_enabled = false; }

	virtual bool GetCaptureStream(NLEComPtr<IUnknown> &pCaptureStream, CaptureStreamSetting &pCaptureStreamSetting){ return false; }
	
	// 当前设备是否有效
	virtual bool GetDeviceEnabled(){ 
		return m_enabled; 
	}
	virtual void SetDeviceEnabled(bool enabled){
		m_enabled = enabled; 
	}

	// 当前设备是否被选择，设备被选择不一定有效还大类的checkbox选中
	virtual bool GetDeviceSelected(){
		return m_enabled; 
	}

	// 停止录制
	virtual void StopRecord() {}
    virtual void StartRecord() {}
    virtual void Clear() {}
    // 防止在构造中使用SettingsDataManager::Instance死循环
    SettingsDataManager* GetParent(){ return (qobject_cast<SettingsDataManager*>(parent())); }
protected:
	bool m_enabled;
};

class DesktopData : public SettingBaseData
{
	Q_OBJECT
public:
	enum CaputerType{ CaputerType_Fullscreen, CaputerType_Custom };
public:
	DesktopData(QObject *parent);

	bool GetCaptureStream(NLEComPtr<IUnknown> &pCaptureStream, CaptureStreamSetting &pCaptureStreamSetting) override;

    bool GetUnRegisterCaptureStream(NLEComPtr<IUnknown> &pCaptureStream, CaptureStreamSetting &pCaptureStreamSetting);

	bool GetDeviceEnabled() override;

	CaputerType GetType();
	void SetType(CaputerType type);

	float GetFramerate();
	void SetFramerate(float framerate);

	QRect GetCropRect();
	void SetCropRect(QRect rect);

    void Clear() override;

    int GetDesktopMaxFramerate();
private:
	CaputerType m_type;
	float m_framerate;
	QRect m_rect;
    QByteArray m_unregisterPNG;
};

class IMouseStreamCtrl;
#define MAX_CURSOR_TYPE 16
class MouseData : public SettingBaseData
{

	Q_OBJECT
public:
	MouseData(QObject *parent = 0);
	~MouseData();

	bool GetCaptureStream(NLEComPtr<IUnknown> &pCaptureStream, CaptureStreamSetting &pCaptureStreamSetting) override;
	bool GetDeviceEnabled() override;
	NLEComPtr<IMouseStreamCtrl> GetIMouseStreamCtrl();

	void PushMouseMoveFrame(QPoint pt,bool bFirstPush = false);
	void PushLButtonClickedFrame(QPoint pt);
	void PushCurrentCursorFrame();

	QImage GetCursorImg(const ICONINFO &iconinfo);
private:
	CaptureDeviceInfo *m_CaptureDeviceInfo;
	NLEComPtr<IUnknown> m_pCurStream;
	Mouse_Frame m_preFrame;

	QImage m_Iimg;
	QImage m_CommonCursor;

	QImage m_tempImg;

	LPCTSTR m_CursorResArray[MAX_CURSOR_TYPE];
	HCURSOR m_CursorHandleArray[MAX_CURSOR_TYPE];
};

class IKeybStreamCtrl;
class KeyData : public SettingBaseData
{
    Q_OBJECT
public:
    KeyData(QObject *parent);

    bool GetCaptureStream(NLEComPtr<IUnknown> &pCaptureStream, CaptureStreamSetting &pCaptureStreamSetting) override;

    bool GetDeviceEnabled();

    void PushKey(int vk, int mk);
    bool PushMarker(int vk, int mk);

private:
    bool PushNormalKey(wchar_t key[]);
    bool PushHotKey(wchar_t key[]);
    bool _PushKey(int type, wchar_t key[]);
    void GetHotKey(int vk, int mk, wchar_t key[]);
private:
    CaptureDeviceInfo m_curDevice;
    NLEComPtr<IUnknown> m_captureStream;
    NLEComPtr<IKeybStreamCtrl> m_streamCtrl;

};

class GameBlackWhitelist;
class GameData : public SettingBaseData
{
	Q_OBJECT
public:
	GameData(QObject *parent);

	bool GetCaptureStream(NLEComPtr<IUnknown> &pCaptureStream, CaptureStreamSetting &captureStreamSetting) override;

	bool GetDeviceEnabled() override;

    bool SetGamenName(const ProcessData &data);
	QString GetGameName();
    QString GetGameProcessName();

	float GetFramerate();
	void SetFramerate(float framerate);
    QSize GetSize();

	void ShowActiveGameWindow();

    bool IsGameFullScreen();

	Q_INVOKABLE void DevicesChanged();

	// type: 0预览 1录制， 开始前必须设置
	void SetStreamType(int type);

    QList<QString> GetGameBlacklist();
    QList<QString> GetGameWhitelist();

    // 标记是否有游戏录制， 解决开始录制倒计时未完成，就主动关闭游戏，提示显示为game问题
    void SetHaveGameRecord();
    bool GetHaveGameRecord();


Q_SIGNALS:
	void signalCurrentDeviceLost(QString deviceName);

public:
	void Start();
	void Stop();
	bool AddHWnd(HWND winId);
private:
    void InitGameBlackWhitelist();
    void Clear() override;
    void InitWSCaptureHookPath();
private:
	NLEComPtr<IUnknown> m_captureStream;
	QString m_gameName;
	float m_framerate;
    QList<QString> m_gameBlacklist;
    QList<QString> m_gameWhitelist;

	//QString m_processID;
	HWND m_gameHwnd;
    DWORD m_gameProcessId;
	NLEComPtr<ICapturePlayer> m_ICapturePlayer;

    ProcessData m_processData;

    bool m_markHaveGameRecord;
    QString m_WSCaptureHookPath;

    GameBlackWhitelist *m_gameBlackWhiteList;
};

typedef QList<CaptureDeviceInfo> CaptureDeviceInfos;
Q_DECLARE_METATYPE(CaptureDeviceInfo)

class CameraData : public SettingBaseData
{
	Q_OBJECT
public:
	CameraData(QObject *parent);
	~CameraData();

	bool GetCaptureStream(NLEComPtr<IUnknown> &pCaptureStream, CaptureStreamSetting &pCaptureStreamSetting) override;

	bool GetDeviceEnabled() override;
	bool GetDeviceSelected() override;

	CaptureDeviceInfo GetCurrentDevice();
	void SetCurrentDevice(const CaptureDeviceInfo &device);
	void SetCurrentDevice(QString deviceName);
	CaptureDeviceInfos GetDevices();

	QSize GetResolution();
	QList<QSize> GetResolutions();
	void SetResolution(QSize size);

	float GetFramerate();
	QList<float> GetFramerates();
	void SetFramerate(float framerate);

    QSize GetDefaultResoluction(const QList<QSize> &resolutions);


Q_SIGNALS:
	void signalDeviceChagned();
	void signalCurrentDeviceLost(QString deviceName);
public:
	void Start();
	void Stop();
	bool AddHWnd(HWND winId);

    Q_INVOKABLE void DevicesChanged(bool curDeviceLost = false);
private:
    void BuildDevice(bool curDeviceLost);

	void SetCaptureStreamProperty(NLEComPtr<IUnknown> captureStream, QString deviceName);
    void Clear() override;

private:
	CaptureDeviceInfo m_curDevice;
	CaptureDeviceInfos m_devices;


	NLEComPtr<IUnknown> m_captureStream;
	//NLEComPtr<IVideoStreamCtrl> m_streamCtrl;
	NLEComPtr<ICapturePlayer> m_ICapturePlayer;
	QSize m_resolution;
	float m_framerate;

};

class AudioData : public SettingBaseData
{
	Q_OBJECT
public:
	AudioData(QObject *parent);
	~AudioData();

	bool GetCaptureStream(NLEComPtr<IUnknown> &pCaptureStream, CaptureStreamSetting &pCaptureStreamSetting) override;
	bool GetSystemAudioCaptureStream(NLEComPtr<IUnknown> &pCaptureStream, CaptureStreamSetting &pCaptureStreamSetting);
	// 麦克风
	bool GetDeviceEnabled() override;
	void SetAudioVolume(float volume);
	float GetAudioVolume();

	// 系统声音
	bool GetRecordSystemAudio();
	bool GetRecordSystemAudioSelected();
	void SetRecordSystemAudio(bool enabled);	

	CaptureDeviceInfo GetCurrentDevice();
	void SetCurrentDevice(const CaptureDeviceInfo &device);
	void SetCurrentDevice(QString deviceName);
	CaptureDeviceInfos GetDevices();

    Q_INVOKABLE void DevicesChanged(bool curDeviceLost = false);


Q_SIGNALS:
	void signalDeviceChagned();
	void signalCurrentDeviceLost(QString deviceName);
	void signalVolumeChanged(int value);

private:
    void BuildDevice(bool curDeviceLost, bool bFirstInit);

	bool  GetCaptureStream(const CaptureDeviceInfo &device, NLEComPtr<IUnknown> &pCaptureStream, CaptureStreamSetting &captureStreamSetting, bool needVolume);

	void StopRecord() override;
    void Clear() override;
private:
	CaptureDeviceInfo m_curDevice;
	CaptureDeviceInfos m_devices;

	bool m_bRecordSystemAudio;
	NLEComPtr<ICaptureAudioVolume> m_ICaptureAudioVolume;
	//NLEComPtr<IAudioVolumeSetting> m_IAudioVolumeSetting;
	NLEComPtr<IUnknown> m_captureStream;
	float m_audioVolume;


};

class AdvancedData : public SettingBaseData
{
	Q_OBJECT
public:

	AdvancedData(QObject *parent);

public:
Q_SIGNALS :
	void signalStartStop();
	void signalPauseResume();
	void signalFullscreen();
    void signalAddMarker(int vk, int mk);


public:
	QString GetRecordFilePath();
	void SetRecordFilePath(QString path);


	void SetScreenSaver(bool enabled);
	bool GetSuportGPU();
	void SetSupportGPU(bool enabled);
    DWORD GetSuportGPUParam();


    void SetAutoHideRecording(bool b);
    bool IsAutoHideRecorder();


	// HotKeys
	QString GetStartStop();
	void SetStartStop(QString keys);

	QString GetPauseResume();
	void SetPauseResume(QString keys);

	QString GetFullScreen();
	void SetFullScreen(QString keys);

    QString GetAddMarker();
    void SetAddMarker(QString keys);

	void SetHotKeyEnabled(bool enabled);

	// other settings
	QString GetProjectSaveDirectory();
	void SetProjectSaveDirectory(QString dir);

    void StartRecord() override;

private:
	bool m_enabled;

	QString m_recordFilePaht;
	bool m_bSuportGPU;
    bool m_bAutoHideRecording;

	QString m_StartStop;
	QString m_PauseResume;
    QString m_Fullscreen;
    QString m_AddMarker;

	QString m_projectSaveDirectory;

    QElapsedTimer m_elapsedTimer;
};


class GameBlackWhitelist : public QThread
{
    Q_OBJECT
public:
    GameBlackWhitelist(QObject *parent);

    QList<QString> GetGameBlacklist();
    QList<QString> GetGameWhitelist();

protected:
    void run() override;
private:
    bool InitFromNetwork();
    void SaveXml();
    void InitFromXml();
    QList<QString> ParseXMLContent(QByteArray xmlContent);
    void AddExtraBlackList();
private:
    QString m_whiteUrl;
    QString m_blackUrl;
    QList<QString> m_gameBlacklist;
    QList<QString> m_gameWhitelist;
    QMutex m_mutex;
};



#endif // OPTIONDATAMANAGER_H
