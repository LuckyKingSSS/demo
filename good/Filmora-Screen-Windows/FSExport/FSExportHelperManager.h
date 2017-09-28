#ifndef FSEXPORTHELPERMANAGER_H
#define FSEXPORTHELPERMANAGER_H

#include <QObject>
#include "FSExportHelper.h"

class FSNLETimeline;
class FSExportHelperManager : public QObject
{
    Q_OBJECT

signals:
    void sigExportState(FSExportHelper::FSEXPORT_STATE eState, int nProgress);
    void sigThumbnail(QPixmap pix);
    void sigFinished();

public:
    FSExportHelperManager(QList<FSNLETimeline *> timeList, bool bRecorderState,ExportSettingInfo &info,QObject *parent);
    ~FSExportHelperManager();
    void Start();
    void Pause();
    void Resume();
    void Stop();
    void SetRegisterFlag(bool bRegister);
    int GetVideoWidth();
    int GetVideoHeight();
    int GetSuccessFileNum();
    int GetFailedFileNum();
    bool HaveUnFinsihedTask();
    bool IsConverting();
    bool IsEnoughDiskSpace(QString rootPath);
    int TaskNum();
    QString GetFileName();
	//获取导出视频的总时长
	int GetExportFileDuration();
    int GetTotalFrames();
	QList<FSExportHelper *> getExportHelper(){ return mHelperList; };

protected:
    void OnExportState(FSExportHelper::FSEXPORT_STATE state, int progress);

   

private:
    QList<FSExportHelper *> mHelperList;
    QList<FSNLETimeline *>  mTimeLineList;
    ExportSettingInfo       mExportInfo;
    int                     mCurrentProgress;
    int                     mSuccessFileNum;
    int                     mFailedFileNum;
    QString                 mFileName;
    bool                    m_bIsRegister;
    HINSTANCE               m_HINSTANCE;
};

#endif // FSEXPORTHELPERMANAGER_H
