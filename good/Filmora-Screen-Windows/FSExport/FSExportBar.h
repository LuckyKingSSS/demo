#ifndef FSEXPORTBAR_H
#define FSEXPORTBAR_H

#include <QtWidgets/QWidget>

class FSButton;
class QButtonGroup;
class QVBoxLayout;

class FSExportBar : public QWidget
{
    Q_OBJECT

public:
    enum FSExportBtn{
        FSE_BTN_LOCAL,
        FSE_BTN_PHONE,
        FSE_BTN_YUOTOBU,
        FSE_BTN_FACEBOOK
    };
signals:
    void ButtonClicked(int btnId);

public:
    FSExportBar(QWidget *parent);
    ~FSExportBar();

    FSExportBtn GetCurrentBtn();

private:
    void CreateBtn(int btnId, const QString &ObjName);

private:
    QButtonGroup *mpBtnGroup;
    QVBoxLayout  *mpVL;
};

#endif // FSEXPORTBAR_H
