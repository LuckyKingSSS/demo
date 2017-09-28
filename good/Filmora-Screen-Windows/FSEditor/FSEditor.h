#ifndef FSEDITOR_H
#define FSEDITOR_H

#include <QtWidgets/QWidget>
#include "inc_FSCommonlib/FSCommonWidget.h"


class UpperView;
class DownView;
class FSTimelineView;
class MainControler;
class FSMainTitleBar;
class FSTitleBarControl;
class FSIconButton;
class FSEditor : public FSBaseWidget
{
    Q_OBJECT

public:
    FSEditor(QWidget *parent = 0);
    ~FSEditor();
    void ShowMaximized();
    UpperView *GetUpperView();
    FSTimelineView *GetTimelineView();
    bool CanExport();
	void RetranslateUi() override;
protected:
    void OnExport();
    void OnGotoRecorder();
    void OnShowEditor();
    void OnHideEditor();
    void OnImportFiles(QStringList lst);
    void OnGetProjectName(QString projName);

    void resizeEvent(QResizeEvent *e) override;
    void closeEvent(QCloseEvent *e) override;
    void showEvent(QShowEvent *e) override;
private:
    FSMainTitleBar *m_pTitleBar;
    QPushButton *m_pBtnRecorder;
    FSIconButton *m_PBtnExport;
    UpperView *m_pUpperView;
    DownView *m_pDownView;
    FSTimelineView *m_pTimelineView;
    QSplitter *m_pSplitter;
    MainControler *m_pMainControler;
    FSTitleBarControl *m_pTitleBarControl;
    bool m_bCanExport;
};

class FSMatterView;
class IFSEditMediaPlayer;
class IFSPropertyView;
class FSMeidaPlayerBar;
class UpperView : public FSBaseWidget
{
    Q_OBJECT

public:
    UpperView(QWidget *parent = 0);
    ~UpperView();

    FSMatterView *GetMatterView();
    IFSEditMediaPlayer *GetMediaPlayerView();
    IFSPropertyView *GetPropertyView();

    void ResizePlayer();
   // void SetBaseSize();
protected:
    void resizeEvent(QResizeEvent *e) override;
private:
    void OnPropertyViewShowSlot(bool bshow);
    void OnMatterViewShowSlot(bool bshow);
private:
    FSMatterView        *m_pMatterView;
    IFSEditMediaPlayer *m_pMediaEditView;
    IFSPropertyView    *m_pPropertyView;
    QSplitter          *m_pSplitter;

};


class DownView : public FSBaseWidget
{
    Q_OBJECT

public:
    DownView(IFSEditMediaPlayer *pPlayer, QWidget *parent);

    FSTimelineView *GetFSTimelineView();
    FSMeidaPlayerBar  *GetFSMediaPlayerBar();

protected:
    void resizeEvent(QResizeEvent *event);

private:
    FSMeidaPlayerBar   *m_pMeidaPlayerBar;
    FSTimelineView *m_pTimelineView;

};

class AudioVolumeSlider;
class ToolBarSlider;
class BaseButton;
class IFSEditMediaPlayer;

class FSMeidaPlayerBar :public FSBaseWidget{
    Q_OBJECT
    enum PlayerToolBar
    {
        ToolBar_Stop = 1,
        ToolBar_Play,
        ToolBar_Pause,
        ToolBar_Forward,
        ToolBar_Backward,
        ToolBar_Volume,
        ToolBar_SnapShort,
        ToolBar_FullScreen,
        ToolBar_Resume,
        ToolBar_ViewScale,
        ToolBar_ViewCut,
    };

public:
    FSMeidaPlayerBar(IFSEditMediaPlayer *pPlayer, QWidget *pParent);
    ~FSMeidaPlayerBar();


    void OnAddTimeLine();
    void OnDurationChangedSlot(int duration);

    void ResizePlayer();
protected:
    void RetranslateUi() override;

private:
    void CreateUI();

    void ConnectSignals();
    void CreateBtn(int Id, const QString &objName, QWidget *parent);
    void CreateStateBtn(int firstId, const QString &objName1, int secId, const QString &objName2);
    void OnVolumeValueChanged(int value);
    void BtnClicked(int id, QPoint leftTop);
    void InitTimeText();
    void ToolBarPlayButtonStateChanged(int id);
    void EnableWidgets(bool bEnable);
    void UpdateTimeText(int duration, int currentFrame, float fps);
    void OnPlayed();
    void OnPaused();
    void OnStoped();
    void OnLoaded();
    void OnPlaying(int frames);
    void OnSeeked(int frames);
    void SetDuration(int duration);
    void OnVolumeClicked(QPoint pos);

private slots:
    void slotButtonGroupClicked(int id);
    void slotResolutionChanged(int index);
    

private:
    QMap<int, BaseButton* > m_ToolBarBtnMap;
    ToolBarSlider          *m_pProgressSlider;
    AudioVolumeSlider      *m_pSliderEditVolume;
    FSLabel                *m_pLabelPreViewDuration;
    FSLabel                *m_pLabelEditDuration;
    FSButton               *m_pButtonViewCut;
    FSButton               *m_pButtonViewScale;
    IFSEditMediaPlayer     *m_pMediaPlayer;
    FSComboBox             *m_pComBoxScale;
    FSComboBox             *m_pComBoxResolution;
    bool                    m_bSeek;
    int                     m_CurrentVoluem;
};

#endif // FSEDITOR_H
