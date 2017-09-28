#pragma once


#include <QObject>


void gFSSetRecordPanel(QWidget *widget);
QWidget* gFSGetRecordPanel();

void gFSSetRecordMainWindow(QWidget *widget);
QWidget* gFSRecordMainWindow();



class QElapsedTimer;
class VolumeBar : public QPushButton
{
    Q_OBJECT

public:
    VolumeBar(QWidget *parent = 0);
    ~VolumeBar();

    void SetFixSize(QSize size);

    void SetValue(int value);
    void SetRange(int max, int min);

protected:
    void resizeEvent(QResizeEvent *);
protected:
    QWidget *m_pValueWidget;
    int m_Max;
    int m_Min;
    int m_Value;
    QElapsedTimer *m_pTimer;
};


