#ifndef FSEXPORTHELPER_H
#define FSEXPORTHELPER_H

#include <QObject>
#include <functional>
#include "FSExportDataDef.h"
#include "Include/NLEComPtr.h"
#include "Interface/INLEMediaEncoder.h"

typedef void* (_stdcall * createCheckFourccObj)();
typedef int(_stdcall * CheckAcerFormate)(DWORD& dwFileFourCC, DWORD& dwVideoFourCC, DWORD& dwAudioFourCC, void * pCheckFourccObj);
typedef void(_stdcall * destroyCheckFourccObj)(void * pCheckFourccObj);
typedef int(_stdcall * AutoGetGpuCaps)();
typedef int(_stdcall * SetAcerType)(int nAccelerateType, const wchar_t* pszDescript, void * pCheckFourccObj);

class NLETimeline;
class INLEMediaEncoder;
class INLEProperties;
class FPSHelper;
class FSExportHelper : public QObject
{
    Q_OBJECT

public:
    enum FSEXPORT_STATE
    {
        EXPORT_STATE_PROGRESS = 0,
        EXPORT_STATE_NORMAL_FINISH,
        EXPORT_STATE_STOPPED_BY_USER,
        EXPORT_STATE_STOPPED_BY_CALLBACK,
        EXPORT_STATE_OCCUR_ERROR
    };
    FSExportHelper(NLETimeline *pTimeline, QObject *parent);
    ~FSExportHelper();
    void SetSettingInfo(const ExportSettingInfo &info);
    ExportSettingInfo GetSettingInfo();
    QString CheckAndGetFileName(const QString &filename);
    void SetFileName(const QString &filename);
    QString GetFileName();
    double GetEstimateDiskSpace(); //MB
    bool IsConverting();
    void Start();
    void Pause();
    void Resume();
    void Stop();
    void SetRegisterFlag(bool bRegister);
    QString GetWorkmarkPath();
    void GetWaterMarkSize(float &widthInOut, float &heightInOut);
    int GetFileFourCC();
    int GetSourceEncoderFourCC();
    QString GetResolution();
    double GetVideoFrameRate();
    int GetVideoBitrate();
    int GetVideoWidth();
    int GetVideoHeight();
    int GetSrcDuration();	//s
    int GetDuration();
    int GetPlayLen();
    void SetGpuInfo(HINSTANCE Hinstance);
	void getHardCodeCodec();
	void CheckfourCC(DWORD &filecc, DWORD &videocc, DWORD &audiocc);
	void TransfromGaInfo(QString strFileType, bool bIsEnabLeGpu);

Q_SIGNALS:
    void sigExportState(FSEXPORT_STATE eState, int nProgress);
    void sigThumbnail(QPixmap pix);
    void sigRetry();

protected:
    bool IsParamValid(const ExportSettingInfo &info);
    void _DoSetting(const ExportSettingInfo &info);

private:
    void InitEncoderCallBack(); 
    void ReCreateEncoder();
	

private:
    NLEEncoderCallback          mCallBack;
    NLEComPtr<INLEMediaEncoder> mpMediaEncoder;
    NLEComPtr<INLEProperties>   mpEncoderProp;
    NLEComPtr<INLEProperties>   mpTimelineProp;
    NLETimeline *               mpTimeline;
    bool                        mbConverting;
    bool                        mbRetried;
    bool                        mbRegiste;
    ExportSettingInfo           mSettingInfo;

    createCheckFourccObj    m_createCheckFourccObj;
    destroyCheckFourccObj   m_destroyCheckFourccObj;
    CheckAcerFormate        m_CheckAcerFormate;
    AutoGetGpuCaps          m_AutoGetGpuCaps;
    SetAcerType             m_SetAcerType;
    void                   *m_pCheckerObj;
    
};

class FPSHelper : public QObject
{
    Q_OBJECT
public:
    FPSHelper( QObject *parent);
    void Start(int totalFrames);
    QString GetLeaveTime(int progress);

private:
    QElapsedTimer mpTimer;
    int mTotalFrames;
    int mCurFrames;
    int mTimeCount;
    QString mLeaveTime;
};

#endif // FSEXPORTHELPER_H
