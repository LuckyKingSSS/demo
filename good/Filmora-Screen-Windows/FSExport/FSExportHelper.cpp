#include "stdafx.h"
#include "FSExportHelper.h"
#include "inc_CommonLib/NLETimeline.h"
#include "inc_CommonLib/NLEWraper.h"
#include "inc_CommonLib/PlayerMaterProvider.h"
#include "inc_CommonLib/externalapi.h"
#include "inc_CommonLib/FileOperation.h"
#include "inc_CommonLib/PathOperation.h"
#include "FSExportSettingData.h"
#include "Include/NLEKey.h"
#include "Include/NLEGraphics.h"
#include "FSExportDataDef.h"
#include "FSExport.h"
#include "inc_FSCommonlib/gathread.h"

FSExportHelper::FSExportHelper(NLETimeline *pTimeline, QObject *parent)
    : QObject(parent)
    , mCallBack(NULL)
    , mpMediaEncoder(NULL)
    , mpEncoderProp(NULL)
    , mpTimelineProp(NULL)
    , mpTimeline(pTimeline)
    , mbConverting(false)
    , mbRetried(false)
    , mbRegiste(false)
{
    qRegisterMetaType<FSEXPORT_STATE>("FSEXPORT_STATE");
    Q_ASSERT(mpTimeline);
    mpTimelineProp = NLEWraper::GetInstance().GetPlayerMaterProvider()->NLEGetProperties(mpTimeline->GetSrc());
    Q_ASSERT(mpTimelineProp);
    mpMediaEncoder = NLEWraper::GetInstance().GetPlayerMaterProvider()->CreateMediaEncoder();
    Q_ASSERT(mpMediaEncoder);
    mpEncoderProp = NLEWraper::GetInstance().GetPlayerMaterProvider()->NLEGetProperties(mpMediaEncoder);
    Q_ASSERT(mpEncoderProp);
    InitEncoderCallBack();
    mpMediaEncoder->SetInput(pTimeline->GetSrc(), mCallBack, this);
    connect(this, &FSExportHelper::sigRetry, this, [&](){
		//上传GA信息
		QString strFileType = GetFileName().mid(GetFileName().lastIndexOf("\\") + 1);
		int iIndex = strFileType.indexOf(".");
		strFileType = strFileType.mid(iIndex + 1);
		TransfromGaInfo(strFileType,false);
        ReCreateEncoder();
        SetSettingInfo(mSettingInfo);
        SetRegisterFlag(mbRegiste);
        Start();
    });

}

FSExportHelper::~FSExportHelper()
{
    m_destroyCheckFourccObj(m_pCheckerObj);
}

void FSExportHelper::SetSettingInfo(const ExportSettingInfo &info)
{
    mSettingInfo = info;
    SetFileName(info.filename);
    _DoSetting(mSettingInfo);
}

ExportSettingInfo FSExportHelper::GetSettingInfo()
{
    return mSettingInfo;
}

QString FSExportHelper::CheckAndGetFileName(const QString &filename)
{
    QString dir = FileOperation::GetFileDir(filename);
    QString fileBasename = FileOperation::GetFileBaseName(filename);
    QString format = FileOperation::GetFileSuffixName(filename);
    QString sFileName=filename;
    int i = 1;
    while (FileOperation::IsFileExist(sFileName))
    {
        QString tmp = QString("%0%1.").arg(fileBasename).arg(i);
        sFileName = dir + tmp + format;
        ++i;
    }
    return sFileName;
}

void FSExportHelper::SetFileName(const QString &filename)
{
    mSettingInfo.filename = CheckAndGetFileName(QDir::toNativeSeparators(filename));
    //CustomLogOutput(QString("SetFileName:%1").arg(mSettingInfo.filename).arg(false));
    mpMediaEncoder->SetOutputFile((BSTR)mSettingInfo.filename.utf16());
}

QString FSExportHelper::GetFileName()
{
    return mSettingInfo.filename;
}

double FSExportHelper::GetEstimateDiskSpace()
{
    //MB
    NLElonglong size;
    mpMediaEncoder->GetEstimateDiskSpace(&size);
    return size / 1024.0 / 1024.0;
}

bool FSExportHelper::IsConverting()
{
    return mbConverting;
}

void FSExportHelper::Start()
{
    mbConverting = true;
    mpMediaEncoder->Start();
}

void FSExportHelper::Pause()
{
    mpMediaEncoder->Pause();
}

void FSExportHelper::Resume()
{
    mpMediaEncoder->Resume();
}

void FSExportHelper::Stop()
{
    mbConverting = false;
    mpMediaEncoder->Stop();
}

void FSExportHelper::SetRegisterFlag(bool bRegister)
{
    mbRegiste = bRegister;
    if (!bRegister)
    {
        QString pPath = GetWorkmarkPath();
        QImage image(":/FSExport/Resources/light/WaterMask.png");
        if (!QFile::exists(pPath))
            image.save(pPath);
        float width = image.width();
        float height = image.height();
        GetWaterMarkSize(width, height);
        mpEncoderProp->SetString(NLEKey::Watermark::kFilePath, pPath.toStdWString().c_str());
        NLERectF rect((1 - width) / 2, (1 - height) / 2, width, height);
        mpEncoderProp->SetRectF(NLEKey::Watermark::kRegion, rect);
        mpEncoderProp->SetInt(NLEKey::Watermark::kOpacity,255);
    }
}

QString FSExportHelper::GetWorkmarkPath()
{      
    QString path = PathOperation::GetDocumentPath();
    if(path.isEmpty())
        path="C:";
    path = path + "/WaterMask.png";
    return path;
}

void FSExportHelper::GetWaterMarkSize(float &widthInOut, float &heightInOut)
{
    int width = widthInOut;
    int height = heightInOut;
    int ExportHeight = mSettingInfo.resolution.mHeight;
    int Exportwidth = mSettingInfo.resolution.mWidth;
    if (width < Exportwidth&&height < ExportHeight)
    {
        widthInOut = qBound(0.4, 1.0*width/Exportwidth,0.6);
        heightInOut = widthInOut*height*Exportwidth / (width*ExportHeight);
    }
    else
    {
        if (width*ExportHeight>height*Exportwidth)
        {
            widthInOut = 0.6;
            heightInOut = widthInOut*height*Exportwidth / (width*ExportHeight);
        }
        else{
            heightInOut = 0.6;
            widthInOut = heightInOut*width*ExportHeight / (height*Exportwidth);
        }
    }
}

int FSExportHelper::GetFileFourCC()
{
    int fourcc;
    mpTimelineProp->GetInt(NLEKey::Codec::kFourCC, &fourcc);
    return fourcc;
}

int FSExportHelper::GetSourceEncoderFourCC()
{
    int fourcc;
    mpTimelineProp->GetInt(NLEKey::Codec::Video::kFourCC, &fourcc);
    return fourcc;
}

QString FSExportHelper::GetResolution()
{
    NLESize size;
    mpTimelineProp->GetSize(NLEKey::Source::kSource_Size, &size);
    return QString("%0*%1").arg(size.Width).arg(size.Height);
}

double FSExportHelper::GetVideoFrameRate()
{
    double framerate = mpTimeline->GetSourceFramerate();
    return framerate;
}

int FSExportHelper::GetVideoBitrate()
{
    int nBitrate;
    mpTimelineProp->GetInt(NLEKey::Codec::Video::kBitrate, &nBitrate);
    return nBitrate;
}

int FSExportHelper::GetVideoWidth()
{
    int width;
    mpEncoderProp->GetInt(NLEKey::Codec::Video::kWidth, &width);
    return width;
}

int FSExportHelper::GetVideoHeight()
{
    int height;
    mpEncoderProp->GetInt(NLEKey::Codec::Video::kHeight, &height);
    return height;
}

int FSExportHelper::GetSrcDuration()
{
    int duration = mpTimeline->GetSrcLen() / mpTimeline->GetSourceFramerate();
    return duration;
}

int FSExportHelper::GetDuration()
{
    return mpTimeline->GetDuration();
}

int FSExportHelper::GetPlayLen()
{
    return mpTimeline->GetPlayLen();
}

void FSExportHelper::SetGpuInfo(HINSTANCE Hinstance)
{
    m_createCheckFourccObj = (createCheckFourccObj)GetProcAddress(Hinstance, "createCheckFourccObj");
    m_destroyCheckFourccObj = (destroyCheckFourccObj)GetProcAddress(Hinstance, "destroyCheckFourccObj");
    m_CheckAcerFormate = (CheckAcerFormate)GetProcAddress(Hinstance, "CheckAcerFormate");
    m_AutoGetGpuCaps = (AutoGetGpuCaps)GetProcAddress(Hinstance, "AutoGetGpuCaps");
    m_SetAcerType = (SetAcerType)GetProcAddress(Hinstance, "SetAcerType");
    m_pCheckerObj = m_createCheckFourccObj();
}

bool FSExportHelper::IsParamValid(const ExportSettingInfo &info)
{
    return true;
}

void FSExportHelper::_DoSetting(const ExportSettingInfo &info)
{
    DWORD fileCC = MAKE_FOURCC(info.filefourcc.toLatin1());
    DWORD videoCC = MAKE_FOURCC(info.videofourcc.toLatin1());
    DWORD audioCC = MAKE_FOURCC(info.audiofourcc.toLatin1());

    CheckfourCC(fileCC, videoCC, audioCC);

    mpMediaEncoder->SetOutputFile((BSTR)info.filename.utf16());
    if (!info.filefourcc.isEmpty())
        mpEncoderProp->SetInt(NLEKey::Codec::kFourCC, fileCC);

    if (!info.videofourcc.isEmpty())
    {
        mpEncoderProp->SetInt(NLEKey::Codec::Video::kFourCC, videoCC);
    }        
    else
        mpEncoderProp->SetInt(NLEKey::Codec::kVideo_Disable, 1);

    if (info.resolution.mWidth>0)
        mpEncoderProp->SetInt(NLEKey::Codec::Video::kWidth, info.resolution.mWidth);

    if (info.resolution.mHeight>0)
        mpEncoderProp->SetInt(NLEKey::Codec::Video::kHeight, info.resolution.mHeight);

    if (info.videoframerate > 10e-4);
        mpEncoderProp->SetDouble(NLEKey::Codec::Video::kFrameRate, info.videoframerate);

    if (info.videobitrate>0)
        mpEncoderProp->SetInt(NLEKey::Codec::Video::kBitrate, info.videobitrate);

    if (!info.audiofourcc.isEmpty())
        mpEncoderProp->SetInt(NLEKey::Codec::Audio::kFourCC, audioCC);
    else
        mpEncoderProp->SetInt(NLEKey::Codec::kAudio_Disable, 1);

    if (info.audiochannel>0)
        mpEncoderProp->SetInt(NLEKey::Codec::Audio::kChannels, info.audiochannel);

    if (info.audiosamplerate>0)
        mpEncoderProp->SetInt(NLEKey::Codec::Audio::kSampleRate, info.audiosamplerate);

    if (info.audiobitrate>0)
        mpEncoderProp->SetInt(NLEKey::Codec::Audio::kBitrate, info.audiobitrate);
    qint64 duration = mpTimeline->GetDuration();
   
}

void FSExportHelper::InitEncoderCallBack()
{
    mCallBack = [](NLEEncoderCallbackType state, NLEEncoderCallbackData data, void *pContext)->HRESULT{
        FSExportHelper *pHelper = static_cast<FSExportHelper *>(pContext);
        Q_ASSERT(pHelper);
        switch (state)
        {
        case NLE_ENCODER_CALLBACK_TYPE_PROGRESS:
        {
            int nValue = 1.0 * data.nProgress / 10000 * pHelper->mpTimeline->GetPlayLen();
            emit pHelper->sigExportState(EXPORT_STATE_PROGRESS, nValue);

            if (data.image)
            {
                NLEMediaInfo Info;
                data.image->GetMediaInfo(&Info);
                QImage img((uchar*)data.image->GetData(), Info.video.nWidth, Info.video.nHeight, QImage::Format_ARGB32);
                QPixmap mPix = QPixmap::fromImage(img);
                emit pHelper->sigThumbnail(mPix);
            }
        }
            break;
        case NLE_ENCODER_CALLBACK_TYPE_STATE:
            switch (data.state)
            {
            case NLE_ENCODER_STATE_NORMAL_FINISH:
                pHelper->mbConverting = false;
                emit pHelper->sigExportState(EXPORT_STATE_NORMAL_FINISH, pHelper->mpTimeline->GetPlayLen());
                break;
            case NLE_ENCODER_STATE_STOPPED_BY_USER:
                pHelper->mbConverting = false;
                emit pHelper->sigExportState(EXPORT_STATE_STOPPED_BY_USER, 0);
                break;
            case NLE_ENCODER_STATE_STOPPED_BY_CALLBACK:
                pHelper->mbConverting = false;
                emit pHelper->sigExportState(EXPORT_STATE_STOPPED_BY_CALLBACK, 0);
                break;
            case NLE_ENCODER_STATE_OCCUR_ERROR:
                if (pHelper->mbRetried)
                {
                    pHelper->mbConverting = false;
                    emit pHelper->sigExportState(EXPORT_STATE_OCCUR_ERROR, 0);
                }
                else{
                    emit pHelper->sigRetry();
                }
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
        return S_OK;
    };
}

void FSExportHelper::CheckfourCC(DWORD &filecc, DWORD &videocc, DWORD &audiocc)
{
    if (!FSExportSettingDataMgr::GetInstance()->IsEnabledGPU())
    {
        mbRetried = true;
        return;
    }
    else if (FSExportSettingDataMgr::GetInstance()->IsEnabledGPU() && mbRetried)
    {
        return;
    }
    int GpuCaps = m_AutoGetGpuCaps();
    if (m_SetAcerType(GpuCaps, L"", m_pCheckerObj))
    {
        if (m_CheckAcerFormate(filecc, videocc, audiocc, m_pCheckerObj))
        {
            qDebug() << "CheckfourCC:succeed";
        }
    }
}

void FSExportHelper::ReCreateEncoder()
{
    if (mpMediaEncoder)
    {
        mpMediaEncoder=NULL;
    }
    mpMediaEncoder = NLEWraper::GetInstance().GetPlayerMaterProvider()->CreateMediaEncoder();
    Q_ASSERT(mpMediaEncoder);
    mpEncoderProp = NLEWraper::GetInstance().GetPlayerMaterProvider()->NLEGetProperties(mpMediaEncoder);
    Q_ASSERT(mpEncoderProp);
    mpMediaEncoder->SetInput(mpTimeline->GetSrc(), mCallBack, this);
    mbRetried = true;
}

void FSExportHelper::TransfromGaInfo(QString strFileType, bool bIsEnabLeGpu)
{
	ExportSettingInfo allSettingInfo = GetSettingInfo();
	GAThread::getInstance()->insertEventItem(Export_Profile, "Export_Profile", strFileType);
	//获取文件名称
	QString strName = allSettingInfo.filename.mid(allSettingInfo.filename.lastIndexOf("\\") + 1);
	GAThread::getInstance()->insertEventItem(Export_Profile, "Export_FileName", strName);
	GAThread::getInstance()->insertEventItem(Export_Profile, "Export_FilePath", allSettingInfo.filename);
	GAThread::getInstance()->insertEventItem(Export_Profile, "Export_FileFormat", strFileType);
	QString strFileRange = GAThread::getInstance()->getExportFileDurationRange(GetDuration());
	GAThread::getInstance()->insertEventItem(Export_Profile, "Export_FileDurationRange", strFileRange);
	QString strExportFileResolutionRange = GAThread::getInstance()->getFileResolutionRange(allSettingInfo.resolution.Area());
	GAThread::getInstance()->insertEventItem(Export_Profile, "Export_VideoResolutionRange", strExportFileResolutionRange);
	GAThread::getInstance()->insertEventItem(Export_Profile, "Export_VideoFramerate", QString::number(allSettingInfo.videoframerate));
	GAThread::getInstance()->insertEventItem(Export_Profile, "Export_VideoBitrate", QString::number(allSettingInfo.videobitrate));
	GAThread::getInstance()->insertEventItem(Export_Profile, "Export_AudioBitrate", QString::number(allSettingInfo.audiobitrate));
	GAThread::getInstance()->insertEventItem(Export_Profile, "Export_AudioChannel", QString::number(allSettingInfo.audiochannel));
	GAThread::getInstance()->insertEventItem(Export_Profile, "Export_AudioSampleRate", QString::number(allSettingInfo.audiosamplerate));
	GAThread::getInstance()->insertEventItem(Export_Profile, "Export_GPUEncoder", bIsEnabLeGpu ? "YES" : "NO");
	if (!bIsEnabLeGpu)
	{
		GAThread::getInstance()->insertEventItem(Export_Profile, "Export_VideoCodec", allSettingInfo.videofourcc);
		GAThread::getInstance()->insertEventItem(Export_Profile, "Export_AudioCodec", allSettingInfo.audiofourcc);
	}
	else
	{    //todo 使用硬编码是否需要做不同的处理？
		DWORD fileCC = MAKE_FOURCC(allSettingInfo.filefourcc.toLatin1());
		DWORD videoCC = MAKE_FOURCC(allSettingInfo.videofourcc.toLatin1());
		DWORD audioCC = MAKE_FOURCC(allSettingInfo.audiofourcc.toLatin1());
		CheckfourCC(fileCC, videoCC, audioCC);
		QString strVidioCC = gMakeFourCCString(videoCC);
		QString strAudioCC = gMakeFourCCString(audioCC);
		GAThread::getInstance()->insertEventItem(Export_Profile, "Export_VideoCodec", allSettingInfo.videofourcc);
		GAThread::getInstance()->insertEventItem(Export_Profile, "Export_AudioCodec", allSettingInfo.audiofourcc);
	}
}


FPSHelper::FPSHelper(QObject *parent)
:QObject(parent)
, mTotalFrames(0)
, mCurFrames(0)
, mTimeCount(0)
, mLeaveTime("00:00:00")
{

}


void FPSHelper::Start(int totalFrames)
{
    mTotalFrames = totalFrames;
    mCurFrames = 0;
    mTimeCount = 0;
    mpTimer.start();
}

QString FPSHelper::GetLeaveTime(int progress)
{
    qint64 elapsedTime = mpTimer.elapsed();
    if (elapsedTime > 1000)
    {
        
        int leaveSecond = qMax<int>(0, 1.0 * (mTotalFrames - progress) / (progress - mCurFrames));

        mLeaveTime = gFrame2StringSecond(leaveSecond * 25);

        mCurFrames = progress;

        mpTimer.restart();

    }

    return QString("%1 %2").arg(tr("Time remaining:"), -20, QChar(' ')).arg(mLeaveTime);

}
