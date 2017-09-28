#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include "inc_FSCommonlib/FSCommonWidget.h"


class QLineEdit;
class CategoryBar;
class SettingBasePanel;
class SettingDialog : public FSBaseDialog
{
    Q_OBJECT
public:
    SettingDialog(QWidget *parent);
    void Resize();
protected:
    void ConnectSignals();

    void RetranslateUi() override;
    void showEvent(QShowEvent *e) override;
    void resizeEvent(QResizeEvent *e) override;

    protected slots:
    void ShowPage(int page);

private:
    QLabel *m_pLbLine;

    QPushButton *m_pBtnOK;
    QPushButton *m_pBtnCancel;
    QButtonGroup *m_pBtnGroup;
    QPushButton *m_pBtnGanunal;
    QPushButton *m_pBtnOhter;
    QPushButton *m_pBtnCanvas;

    QStackedWidget *m_stackedWidget;
    QMap<int, SettingBasePanel*> m_panels;
    QList<QPushButton*> m_btns;
};


class SettingBasePanel : public FSBaseWidget
{
    Q_OBJECT
public:
    SettingBasePanel(QWidget *parent);

    virtual void OnOK() = 0;
    virtual void OnCancel() = 0;

};

class SettingGeneralPanel : public SettingBasePanel
{
    Q_OBJECT
public:
    SettingGeneralPanel(QWidget *parent);

protected:
    void OnOK() override;
    void OnCancel() override;

    void RetranslateUi() override;
    void Init();
    QString GetSaveProjToPath();
    QString GetLang();
protected:
    void showEvent(QShowEvent *event);
    void resizeEvent(QResizeEvent *event);
private:
    QLabel *m_pLbSaveProjTo;
    QLineEdit *m_pEditSavePath;
    BaseButton *m_pBtnOpen;
    QLabel *m_pLbLang;
    QLabel *m_pLbLine;
    FSComboBox *m_pCombLang;
};


class SettingOtherPanel : public SettingBasePanel
{
    Q_OBJECT
public:
    SettingOtherPanel(QWidget *parent);

protected:
    void OnOK() override;
    void OnCancel() override;
    void RetranslateUi() override;
    void Init();
protected:
    void showEvent(QShowEvent *event);
    void resizeEvent(QResizeEvent *event);
private:

    QCheckBox *m_pChbUpgrade;
    QCheckBox *m_pChbGPUAcce;

};



class SettingCanvasPanel : public SettingBasePanel
{
    Q_OBJECT
public:
    SettingCanvasPanel(QWidget *parent);

protected:
    void OnOK() override;
    void OnCancel() override;
    void RetranslateUi() override;
    void Init();
protected:
    void resizeEvent(QResizeEvent *event);
private:
    QCheckBox *m_pChbAttach;

};

#endif // SETTINGDIALOG_H
