#ifndef FSEXPORTPROGRESSDIALOG_H
#define FSEXPORTPROGRESSDIALOG_H

#include "inc_FSCommonlib/FSCommonWidget.h"
#include "FSExportHelperManager.h"

class FPSHelper;
class ElapsedTimeLabel;
class FSExportProgressDialog : public FSBaseDialog
{
    Q_OBJECT

public:
    enum ExitCode
    {
        ExitCode_Finish = 0,
        ExitCode_Cancel,
        ExitCode_Error_Occur,
    };

    FSExportProgressDialog(QWidget *parent);
    ~FSExportProgressDialog();

    void SetExportHelperMgr(FSExportHelperManager *pExportHelperMgr);

Q_SIGNALS:
	void singalExportFail();

protected:
    void resizeEvent(QResizeEvent *event);
    void closeEvent(QCloseEvent *e);

private:
    void CreateUI();
    void ConnectSignals();
    void TranslateUI();

    void OnCancelCilckedSlot();
    void OnPauseResumeClickedSlot();
    void OnFindTargetClickedSlot();
    void OnExportHelperStatesChanagedSlot(FSExportHelper::FSEXPORT_STATE state, int progress);
    //void OnThumbnail(QPixmap  pix);
    void OnFinished();



private:
    FSExportHelperManager *mpExportHelperMgr;
    FSLabel        *mpLabelStep;
    FSLabel        *mpLabelLeaveTime;
    ElapsedTimeLabel        *mpLabelElapsedTime;
    //FSLabel        *mpLabelMovie;
    FSButton       *mpButtonPauseResume;
    FSButton       *mpButtonCancel;
    FSButton       *mpButtonFindTarget;
    QProgressBar   *mpProgressBar;
    bool            m_bErrorOccur;

    FPSHelper *mpFPSHelper;

};


class FSConfirmDlg : public FSMessageBox
{
    Q_OBJECT
public:
    FSConfirmDlg(const QString &title, const QString &text, int buttons = Button::Yes, QWidget *parent = nullptr);
    static QPair<bool, FSMessageBox::Button> question(const QString & title, const QString & text, QWidget * parent);
protected:
    bool IsCheck();
    void resizeEvent(QResizeEvent *e) override;
protected:
    QCheckBox *m_pCbDontShow;
    QLabel *m_pLbDonShow;
};

class ElapsedTimeLabel : public FSLabel
{
    Q_OBJECT
public:
    ElapsedTimeLabel(QWidget *parent);

    void Start();
    void End();
private:
    void SetElapsedTime(int second);
private:
    QTimer *m_timer;
    int m_second;
};
#endif // FSEXPORTPROGRESSDIALOG_H
