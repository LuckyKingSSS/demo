#ifndef MINIRECORDPANEL_H
#define MINIRECORDPANEL_H

#include "inc_FSCommonlib/FSCommonWidget.h"

class TopLevelWindowTitleBar;
class RecordButton;
class CountBackwardCtrl;
class MiniPanelAttachmentManager;
class VolumeBar;
class MiniRecordPanel : public FSBaseWidget
{
    Q_OBJECT

public:
    MiniRecordPanel(QWidget *parent);
    ~MiniRecordPanel();

    void SetToolTip();

    void Attach();

protected:
    void resizeEvent(QResizeEvent *event);
    void showEvent(QShowEvent *event);

private:
    TopLevelWindowTitleBar *m_pTitleBar;
    BaseStateButton *m_pBtnRecord;
    BaseStateButton *m_pBtnPauseResume;
    CountBackwardCtrl *m_pCountBackwardCtrl;
    BaseButton *m_pBtnDelete;

    BaseButton *m_pBtnMinimized;
    BaseButton *m_pBtnMaxPanel;
    MiniPanelAttachmentManager *m_attachMgr;

    VolumeBar *m_volumeBar;
private:
    friend class SyncRecordPanelAndMiniPanelState;
    friend class MiniPanelAttachmentManager;
};

class RecordPanel;
class SyncRecordPanelAndMiniPanelState : public QObject
{
    Q_OBJECT
private:
    enum{Type_Panel, Type_MiniPanel};
public:
    SyncRecordPanelAndMiniPanelState(RecordPanel *panel, MiniRecordPanel *miniPanel, QObject *parent);

    void StartRecorder();
    void StopRecorder();

    void ButtonRecordSetCurrentId(int id);
    void ButtonRecordSetEnabled(bool enabled);

    void ButtonPauseResumeSetCurrentId(int id);
    void ButtonPauseResumeSetEnabled(bool enabled);

    void ButtonDeleteSetEnabled(bool enabled);
    void ButtonDeleteSetToolTip(QString tooltip);

    void CountBackwardCtrlStart();
    void CountBackwardCtrlPause();
    void CountBackwardCtrlResume();
    void CountBackwardCtrlStop();

    void setVisible(bool enabled);

    QWidget* GetRealRecordWidget();


signals:
    void sigButtonRecordClicked(int id);
    void sigButtonPauseResumeClicked(int id);
    void sigButtonDeleteClicked(QWidget *parent);

protected:
    bool eventFilter(QObject *watcher, QEvent *event);
    
private:
    MiniRecordPanel *m_pMiniPanel;
    RecordPanel *m_pRecordPanel;
    int m_currentPanelType;
    int m_startRecordPanelType;
    QPoint m_globalPos;
};

class DashlineWidget;
class MiniPanelAttachmentManager : public QObject
{
    Q_OBJECT
public:
    MiniPanelAttachmentManager(MiniRecordPanel *parent);

    bool GetAttach();
    void SetAttach(bool attach);

    void Attach();

private:
    void RecorderSizeChanged(QRect rc);
    QPoint GetAttachPosition();
    QRect GetAttachRectangle();
    bool GetCanAttach(QPoint position);

protected:
    bool eventFilter(QObject *watcher, QEvent *event);
   
private:
    MiniRecordPanel *m_miniPanel;
    QRect m_recordRectangle;
    bool m_attach;
    DashlineWidget *m_pAttachHintDashline;

};



class MiniVolumeBar : public VolumeBar
{
    Q_OBJECT
public:
    MiniVolumeBar(QWidget *parent) :VolumeBar(parent){
        m_pValueWidget->setObjectName("MiniRecordPanel_MiniVolumeBar");
        setObjectName("");
    }

};


#endif // MINIRECORDPANEL_H
