#ifndef DRAGSPINBOX_H
#define DRAGSPINBOX_H

#include "inc_CommonLib/commonlib_global.h"
#include <QtWidgets/QWidget>
#include <QtCore/QObject>


class DragValueChangedHelper;
class COMMONLIB_EXPORT IDragValueChangedHelper : public QObject
{
    Q_OBJECT

public:
    IDragValueChangedHelper(QWidget *parent);

    void SetRange(double minvalue, double maxvalue);
    void SetValue(double value);
    void SetDragEnabled(bool enabled);
    void SetLoop(bool loop);

    void SetStep(double step);
    void SetFastStep(double step);
    void SetSlowStep(double step);

    void SetStepDistance(int distance);
    void SetFastDistance(int distance);
    void SetSlowDistance(int distance);

    void SetInteractiveRectangle(QRect rect);
    void SetHoverCursor(QCursor cursor);

    void GetRange(double &minvalue, double &maxvalue);
    double GetValue();
    bool GetDragEnabled();
    bool GetLoop();
    double GetStep();
    double GetFastStep();
    double GetSlowStep();

    int GetStepDistance();
    int GetFastDistance();
    int GetSlowDistance();

    QRect GetInteractiveRectangle();
    QCursor GetHoverCursor();
Q_SIGNALS:
    void sigValueChanged(double value);
    void sigStartValueChanged(double value);
    void sigEndValueChanged(double value);

private:
    DragValueChangedHelper *m_p;
};

#endif // DRAGSPINBOX_H
