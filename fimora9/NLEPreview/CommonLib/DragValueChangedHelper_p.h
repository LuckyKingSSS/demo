#ifndef DRAGVALUECHANGEDHELPER_P
#define DRAGVALUECHANGEDHELPER_P


#include <QtCore/QtCore>
#include <QtWidgets/QtWidgets>

class DragValueChangedHelper : public QObject
{
    Q_OBJECT

public:
    DragValueChangedHelper(QWidget *parent);

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
protected:
    bool eventFilter(QObject *watcher, QEvent *event);

    void SetOverrideCursor(QCursor cursor);
    void RestoreOverrideCursor();
private:
    QWidget *m_parent;

    int m_dragDistance;
    int m_dragFastDistance;
    int m_dragSlowDistance;
    bool m_loop;

    double m_min;
    double m_max;
    double m_step;
    double m_fastStep;
    double m_slowStep;
    double m_value;

    bool m_bDrag;
    QRect m_InteractiveRect;
    QCursor m_cursor;

    QCursor m_hoverCursor;
    QCursor m_preCursor;

    QElapsedTimer m_dragElapsedTimer;

    QPoint m_currentPos;
    QPoint m_mouserPressPos;
    QPoint m_dragCursorPos;
    bool m_startDrag;
    int m_curDragDistance;
    bool m_startValueChanged;
    double m_dragIncreaseStep;
    bool m_dragRight;
    int m_setAppCursorCount;
};



#endif
