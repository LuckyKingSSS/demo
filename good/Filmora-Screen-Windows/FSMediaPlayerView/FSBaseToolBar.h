#ifndef FSBASETOOLBAR_H
#define FSBASETOOLBAR_H

#include <QtWidgets/QWidget>
#include <QMap>

class IMediaPlayerControl;
class BaseButton;
class ToolBarSlider;
class AudioVolumeSlider;
class NLETimeline;
class FSLabel;
class QSlider;

class FSBaseToolBar : public QWidget
{
    Q_OBJECT
public:
    enum ToolBar
    {
        ToolBar_Stop = 1,
        ToolBar_Play ,      
        ToolBar_Pause,       
        ToolBar_Forward,
        ToolBar_Backward,
        ToolBar_Volume,
        ToolBar_ViewScale,
        ToolBar_ViewCut,       
        ToolBar_FullScreen,
        ToolBar_FullScreenExit,     
    };

signals:
    void RulerSeekCallback(int frames);
    void StartRulerSeekCallback(bool bPlaying);
    void EndRulerSeekCallback(bool bPlaying);
    void VeiwCutSignal(bool bScalemode);
    void FullScreenSignal();
    void EndBackForword();

public:
    FSBaseToolBar(IMediaPlayerControl *control, QWidget *parent = nullptr);
    ~FSBaseToolBar();

    bool Close();
    bool Open(int bSource, NLETimeline *timeline);
    void Play();
    void Stop();
    void Pause();
    void Forward();
    void Backward();
    void Seek(int frames);
    void Restore();
    bool HaveSource();
    bool IsPlaying();
    void EnableWidgets(bool bEnable);

    void UpdateTimeText(int duration);

    void SetDuration(int duration);
    void SetFPS(float fps){}
    void SetRatio(float ratio){}

    int CurrentFrames();
    int Duration();
    bool IsSeeking();
    BaseButton* GetButton(int id);
    BaseButton* GetStateBtn(int id,int id1);
    void DblMouseBtnFullScreen();
    bool IsScaleState();

    void SetFullScreenBtnVisible(bool bVisible);
    void SetScaleBtnVisible(bool bVisible);
    void SetChunkHeight(int height);

protected:
    void mouseDoubleClickEvent(QMouseEvent *e);

    void OnStoped();
    void OnPlayed();
    void OnPaused();
    void OnForwarded();
    void OnBackwarded();

    void OnSeekCallback(int frames, int accurate);
    bool SeekManual(int frames, int accurate);

private:
    void CreateUI();
    void ConnectSignals();
    void CreateBtn(int Id, const QString &objName, QWidget *parent);
    void CreateStateBtn(int firstId, const QString &objName1, int secId, const QString &objName2);
    void ToolBarPlayButtonStateChanged(int id);
    void SetUiTipText();
    void BtnClicked(int id,QPoint leftTop);
    void OnVolumeClicked(QPoint pos);
    
    void OnVolumeValueChanged(int value);

    void InitTimeText();

private:
    IMediaPlayerControl *   mpMediaPlayerControl;
    QMap<int, BaseButton* > mToolBarBtnMap;
    ToolBarSlider          *mpProgressSlider;
    AudioVolumeSlider      *mpSliderEditVolume;
    FSLabel                *mpLabelPreViewDuration;
    FSLabel                *mpLabelEditDuration;
    QHBoxLayout            *m_pHL;
    bool                    mbPlaying;
    bool                    mbseekManual;
    int                     mlastSeekFrames;
    int                     mCurrentVoluem;
};

class FSRecorderBaseToolBar : public QWidget
{
    Q_OBJECT
public:
    enum RecorderToolBar
    {
        ToolBar_Play=1,
        ToolBar_Pause,
        ToolBar_VolumeOn,
        ToolBar_VolumeOff,
        ToolBar_FullScreen,
        ToolBar_FullScreenExit,
    };

signals:
    void RulerSeekCallback(int frames);
    void StartRulerSeekCallback(bool bPlaying);
    void EndRulerSeekCallback(bool bPlaying);
    void FullScreenSignal();

public:
    FSRecorderBaseToolBar(IMediaPlayerControl *control, QWidget *parent = nullptr);
    ~FSRecorderBaseToolBar();

    bool Close();
    bool Open(int bSource, NLETimeline *timeline);
    void Play();
    void Stop();
    void Pause();
    void Forward();
    void Backward();
    void Seek(int frames);
    void Restore();
    bool HaveSource();
    bool IsPlaying();
    void EnableWidgets(bool bEnable);

    void UpdateTimeText(int duration);

    void SetDuration(int duration);
    void SetFPS(float fps){}
    void SetRatio(float ratio){}

    int CurrentFrames();
    int Duration();
    bool IsSeeking();
    BaseButton* GetButton(int id);
    BaseButton* GetStateBtn(int id, int id1);
    void DblMouseBtnFullScreen();

    void SetFullScreenBtnVisible(bool bVisible);
    void SetScaleBtnVisible(bool bVisible);
    void SetChunkHeight(int height);

protected:
    void mouseDoubleClickEvent(QMouseEvent *e);

    void OnStoped();
    void OnPlayed();
    void OnPaused();
    void OnForwarded();
    void OnBackwarded();

    void OnSeekCallback(int frames, int accurate);
    bool SeekManual(int frames, int accurate);

private:
    void CreateUI();
    void ConnectSignals();
    void CreateBtn(int Id, const QString &objName, QWidget *parent);
    void CreateStateBtn(int firstId, const QString &objName1, int secId, const QString &objName2);
    void ToolBarPlayButtonStateChanged(int id);
    void SetUiTipText();
    void BtnClicked(int id, QPoint leftTop);

    void OnVolumeValueChanged(int value);
    void OnVolumeOnClicked();
    void OnVolumeOffClicked();

    void InitTimeText();

private:
    IMediaPlayerControl *   mpMediaPlayerControl;
    QMap<int, BaseButton* > mToolBarBtnMap;
    ToolBarSlider          *mpProgressSlider;
    FSLabel                *mpLabelPreViewDuration;
    ToolBarSlider          *mpSliderPreviewVolume;
    QHBoxLayout            *m_pHL;
    bool                    mbPlaying;
    bool                    mbseekManual;
    int                     mlastSeekFrames;
    int                     mCurrentVoluem;
	int                     m_iStartSeekTime;//启动seek时间
	int                     m_iEndSeekTime;//结束seek时间
};


#endif // FSBASETOOLBAR_H
