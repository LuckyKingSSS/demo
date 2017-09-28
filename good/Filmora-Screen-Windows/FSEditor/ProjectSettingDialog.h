#ifndef PROJECTSETTINGDIALOG_H
#define PROJECTSETTINGDIALOG_H

#include "inc_FSCommonlib/FSCommonWidget.h"

class ProjectSettingsMgr;
class ResolutionLineEdit;
class ColorSelecterComboBox;
class ProjectSettingDialog : public FSBaseDialog
{
    Q_OBJECT

public:
    ProjectSettingDialog(QWidget *parent);
    ~ProjectSettingDialog();

protected:
    void resizeEvent(QResizeEvent *event);

protected:
    void RetranslateUi() override;

    void Init();
    void ConnectSignals();
    void SetLockAspectString(QSize size);
    void SetResolutoinIndex(QSize resolution);
    void ResolutionLineEditFinished();
    QString GetAspectStringFromSize(QSize size);
private slots: 
    void slotPresetIndexChanged(int index);
    void slotPlaybackIndexChanged(int index);

    void slotResolutionWidthValueChanged(double value);
    void slotResolutionHeightValueChanged(double value);

private:
    FSLabel *m_lbProjectName;
    FSLineEdit *m_leProjectname;

    FSLabel *m_lbPreset;
    FSComboBox *m_cmbPreset;

    FSLabel *m_lbWidth;
    ResolutionLineEdit *m_leWidth;
    FSLabel *m_lbWidthSuffix;

    QCheckBox *m_chkLockAspect;

    FSLabel *m_lbHeight;
    ResolutionLineEdit *m_leHeight;
    FSLabel *m_lbHeightSuffix;

    FSLabel *m_lbPlaybackResoultion;
    FSComboBox *m_cmbPlaybackResoultion;

    FSLabel *m_lbBackgroundColor;
    ColorSelecterComboBox *m_cmbColor;

    FSButton *m_btnOK;
    FSButton *m_btnCancel;


    QList<QSize> m_presetList;
    QList<std::tuple<QString, float>> m_playbackList;

    ProjectSettingsMgr *m_mgr;
    float m_currentResolutionRatio;
};

#include "inc_FSCommonlib/FSCommonWidget.h"
class IDragValueChangedHelper;
class ResolutionLineEdit : public FSDoubleSpinBox
{
    Q_OBJECT
public:
    ResolutionLineEdit(QString objectName, QWidget *parent);

};



#endif // PROJECTSETTINGDIALOG_H
