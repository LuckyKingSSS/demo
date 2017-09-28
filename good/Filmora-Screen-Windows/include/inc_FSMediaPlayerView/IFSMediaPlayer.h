#ifndef IFSMEDIAPLAYER_H
#define IFSMEDIAPLAYER_H

#include "inc_FSMediaPlayerView/fsmediaplayerview_global.h"
#include <QtWidgets/QWidget>
#include "inc_FSTimelineView/UndoCommandBase.h"


class FSMediaPlayer;
class FSNLETimeline;

/************************************************************
*     没有编辑功能的播放器
*************************************************************/
class FSMEDIAPLAYVIEW_EXPORT IFSMediaPlayer : public QWidget
{
    Q_OBJECT


signals:
    void RulerSeekCallback(int frames);
    void StartRulerSeekCallback(bool bPlaying);
    void EndRulerSeekCallback(bool bPlaying);

public:
    IFSMediaPlayer(QWidget *parent/*,QWidget *pMainWindow*/);
    ~IFSMediaPlayer();
    bool Open(FSNLETimeline *timeline);
    bool Close();
    void Play();
    void Stop();
    void Pause();
    void Seek(int frames);
    void SetDuration(int duration);

    void Forward();
    void Backward();
    void Restore();
    int  Duration();

    bool HaveSource();
	bool IsPlaying();
    void SetChunkHeight(int height);
    void Update();

private:
    FSMediaPlayer *mpFSMediaPlayer;
};

/************************************************************
*     带编辑功能的播放器
*************************************************************/
class FSEditPlayerView;
class IPlayerEditorManager;
class ITimelineViewManager;
class IFSEditHelperManager;
class ITitleBarManager;
class IMediaPlayerControl;
class FSMeidaPlayerBar;
class FSMediaScrollArea;

class FSMEDIAPLAYVIEW_EXPORT IFSEditMediaPlayer : public QWidget
{
    Q_OBJECT

signals:
    void sigChangeScale(bool bZoom);
    void sigMenuChangeScale(QString scale);
    void sigEnableEditShotCut(bool bEnable);
    void sigBackForwedShortCutTriged(int frames);
    void sigEditModeChanged(bool bScaleMode);
    void sigProjectSettingTriggered();
    void sigResolutionChanged(float resolutionScale);

public:
    IFSEditMediaPlayer(QWidget *parent);
    ~IFSEditMediaPlayer();
    bool Open(FSNLETimeline *timeline);
    bool Close();
    void Play();
    void Stop();
    void Pause();
    void Seek(int frames);
    void SetDuration(int duration);

    void Forward();
    void Backward();
    void Restore();
    int  Duration();
    bool HaveSource();
    int  CurrentFrame();
    bool IsPlaying();

    void OnMediaPlayerBarStartChange();
    void OnMediaPlayerBarEndChange();
    void OnMediaPlayerBarChange(int frame);

    void OnMediaPlayerBarFullScreen();
    void OnMediaPlayerBarCropView(bool bScaleMode);

    void SetPlayerEditManager(IPlayerEditorManager*);
    void SetTimelineViewManager(ITimelineViewManager*);
    void SetTitleBarMannager(ITitleBarManager *);
    ITimelineViewManager* GetTimelineViewManager();

    IFSEditHelperManager *GetEditHelperManarger();

    IMediaPlayerControl * GetMediaPlayerControl();
    void SetChunkHeight(int height);
    void SetScale(bool bFit,float scale);
    void GetSnapShortImg(QImage & image);
    void ResetMartPoint();

    void SetBaseRenderSize(QSize size);
    void SetPlayBackResolution(float resulotion);
    float GetPlayBackResolution();

    void SetPlayerBackgroundColor(QColor color);

private:
    FSEditPlayerView      *m_pEditPlayer;
    FSMediaScrollArea     *m_pEditArea;
};


class FSPropertyView;
class FSMEDIAPLAYVIEW_EXPORT IFSPropertyView :public QWidget{
    Q_OBJECT
signals:
    void sigShowPropertyView(bool bshow);
    void sigSpeedChanged(FSNLETimeline *timeline,float scale);
	void sigDenoise(FSNLETimeline *timeline);
public:
    IFSPropertyView(QWidget *pParent);
    ~IFSPropertyView();


public:
    void SetEditorHelperManager(IFSEditHelperManager * pHelperManager);
    void SetPlayerEditManager(IPlayerEditorManager* pPlayerEditorManager);

private:
    FSPropertyView *m_pPropertyView;
};



class IFSEditHelper;
class FSEditHelperManager;

class FSMEDIAPLAYVIEW_EXPORT IFSEditHelperManager :public QObject{
    Q_OBJECT

public:
    IFSEditHelperManager(QObject *pParent);
    ~IFSEditHelperManager();

    virtual void EnableSnap(bool bEnable) = 0;
	virtual void SetMouseEffectDraging(bool bDraging)= 0;
    virtual bool CanAddMouseEffect()=0;

signals:
    void CurrentEditHelperChanged(IFSEditHelper* pHelper, bool updataTimelin);
    void TransformInfoChanged(IFSEditHelper *);
    void EndChanged(UndoCommand  *pCmd);
    void TextChanged(const QString &strText);
    void MouseEffictAdded(int Efficttype);
};



FSMEDIAPLAYVIEW_EXPORT QString GetPlayerStyleSheet();



#endif // FSMEDIAPLAYER_H
