#include "stdafx.h"
#include "FSExportHelperManager.h"
#include "inc_FSCommonlib/FSNLETimeline.h"
#include "inc_CommonLib/NLEWraper.h"
#include "inc_CommonLib/externalapi.h"
#include "FSExportSettingData.h"
#include "inc_FSCommonlib/gathread.h"

FSExportHelperManager::FSExportHelperManager(QList<FSNLETimeline *> timeList, bool bRecorderState, ExportSettingInfo &info, QObject *parent)
    : QObject(parent)
    , mTimeLineList(timeList)
    , mExportInfo(info)
    , mCurrentProgress(0)
    , mSuccessFileNum(0)
    , mFailedFileNum(0)
{
   
    m_HINSTANCE = LoadLibrary(L"wsCUDA.dll");
    for (auto timeLine : mTimeLineList)
    {
        FSExportHelper *pHelper = new FSExportHelper(timeLine, this);
        pHelper->SetGpuInfo(m_HINSTANCE);
        pHelper->SetSettingInfo(mExportInfo);
        if (bRecorderState)
        {
            QFileInfo info(mExportInfo.filename);
            QFileInfo info1(timeLine->GetFilePath());
            QString path = QString("%1%2%3.%4").arg(info.path()).arg(QDir::separator()).arg(info1.completeBaseName()).arg(info.suffix());
            path = QDir::toNativeSeparators(path);
            pHelper->SetFileName(path.split(QDir::separator(), QString::SkipEmptyParts).join(QDir::separator()));
        }       
        connect(pHelper, &FSExportHelper::sigExportState, this, &FSExportHelperManager::OnExportState);
        connect(pHelper, &FSExportHelper::sigThumbnail, this, &FSExportHelperManager::sigThumbnail);
        mHelperList.push_back(pHelper);
    }
}

FSExportHelperManager::~FSExportHelperManager()
{
    qDeleteAll(mHelperList);
    if (FSExportSettingDataMgr::GetInstance()->IsEnabledGPU())
    {
        if (m_HINSTANCE)
            FreeLibrary(m_HINSTANCE);
    }

}


void FSExportHelperManager::Start()
{
    if (!mHelperList.isEmpty())
    {

        mHelperList.first()->Start();
    }
    else
    {
        emit sigFinished();
    }
}

void FSExportHelperManager::Pause()
{
    if (!mHelperList.isEmpty())
    {
        mHelperList.first()->Pause();
    }
}

void FSExportHelperManager::Resume()
{
    if (!mHelperList.isEmpty())
    {
        mHelperList.first()->Resume();
    }
}

void FSExportHelperManager::Stop()
{
    if (!mHelperList.isEmpty())
    {
        mHelperList.first()->Stop();
    }
}

void FSExportHelperManager::SetRegisterFlag(bool bRegister)
{
    if (!mHelperList.isEmpty())
        QFile::remove(mHelperList.first()->GetWorkmarkPath());

    for (auto item:mHelperList)
    {
        item->SetRegisterFlag(bRegister);
    }
}

int FSExportHelperManager::GetVideoWidth()
{
    if (!mHelperList.isEmpty())
    {
        return mHelperList.first()->GetVideoWidth();
    }
    return 1;
}

int FSExportHelperManager::GetVideoHeight()
{
    if (!mHelperList.isEmpty())
    {
        return mHelperList.first()->GetVideoHeight();
    }
    return 1;
}


int FSExportHelperManager::GetSuccessFileNum()
{
    return mSuccessFileNum;
}

int FSExportHelperManager::GetFailedFileNum()
{
    return mFailedFileNum;
}

bool FSExportHelperManager::HaveUnFinsihedTask()
{
    return !mHelperList.isEmpty();
}

bool FSExportHelperManager::IsConverting()
{
    if (!mHelperList.isEmpty())
    {
        return mHelperList.first()->IsConverting();
    }
    return false;
}

int FSExportHelperManager::TaskNum()
{
    return mHelperList.size();
}

QString FSExportHelperManager::GetFileName()
{
    return mFileName;
}

int FSExportHelperManager::GetExportFileDuration()
{
	int iTotalTime = 0;
	for (auto itemHelper : mHelperList)
	{
		int iCurDuaration = itemHelper->GetDuration();
		if (iCurDuaration > iTotalTime)
		{
			iTotalTime = iCurDuaration;
		}
	}
	return iTotalTime;
}

int FSExportHelperManager::GetTotalFrames()
{
    int iTotalFrame = 0;
    for (auto itemHelper : mHelperList)
    {
        iTotalFrame += itemHelper->GetPlayLen();
    }
    return iTotalFrame;
}

void FSExportHelperManager::OnExportState(FSExportHelper::FSEXPORT_STATE state, int progress)
{
    FSExportHelper *pHelper = static_cast<FSExportHelper *>(sender());

    mFileName = pHelper->GetFileName();
    switch (state)
    {
    case FSExportHelper::EXPORT_STATE_PROGRESS:
        emit sigExportState(FSExportHelper::EXPORT_STATE_PROGRESS, mCurrentProgress + progress);
        break;
    case FSExportHelper::EXPORT_STATE_NORMAL_FINISH:
        mHelperList.removeOne(pHelper);
        mSuccessFileNum++;
        if (mHelperList.isEmpty())
        {
            QFile::remove(pHelper->GetWorkmarkPath());
            emit sigFinished();
        }
        else
        {
            mCurrentProgress += pHelper->GetPlayLen();
            mHelperList.first()->Start();
        }        
        pHelper->deleteLater();
        break;
    case FSExportHelper::EXPORT_STATE_OCCUR_ERROR:
        pHelper->Stop();
        mHelperList.removeOne(pHelper);
        mFailedFileNum++;
        QFile::remove(pHelper->GetFileName());
        emit sigExportState(FSExportHelper::EXPORT_STATE_OCCUR_ERROR, mCurrentProgress + progress);
        mCurrentProgress += pHelper->GetPlayLen();
        pHelper->deleteLater();
        break;
    default:
        break;
    }
}

bool FSExportHelperManager::IsEnoughDiskSpace(QString rootPath)
{
    qint64 size=0,size1=0;
    for (auto itemHelper:mHelperList)
    {
        size+=itemHelper->GetEstimateDiskSpace();
    }
    size1 = gGetDiskFreeSpace(rootPath);
	//上传文件大小数据
	QString strFileRange = GAThread::getInstance()->getExportFileRange(size);
	GAThread::getInstance()->insertEventItem(Export_Profile, "Export_FileSizeRange", strFileRange);
    return size1 > size;
}
