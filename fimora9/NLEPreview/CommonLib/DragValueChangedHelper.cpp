#include "stdafx.h"

#include "inc_CommonLib/DragValueChangedHelper.h"
#include "DragValueChangedHelper_p.h"

DragValueChangedHelper::DragValueChangedHelper(QWidget *parent)
: QObject(parent)
, m_parent(parent)
, m_cursor(QPixmap(":/CommonLib/Resources/normal/cursor_transparent.png"))
{
    m_min = -std::numeric_limits<double>::max();
    m_max = std::numeric_limits<double>::max();
    m_value = 0.0;

    m_step = 1.0;
    m_fastStep = 10.0;
    m_slowStep = 1.0;
    m_loop = true;

    m_bDrag = true;

    m_dragDistance = 1;
    m_dragFastDistance = 1;
    m_dragSlowDistance = 1;

    m_hoverCursor = QCursor(Qt::SizeHorCursor);


    m_currentPos;
    m_mouserPressPos;
    m_dragCursorPos;
    m_startDrag = false;
    m_curDragDistance = 0;
    m_startValueChanged = false;
    m_dragIncreaseStep = 0;
    m_dragRight = true;

    m_setAppCursorCount = 0;

    Q_ASSERT(parent);
    qApp->installEventFilter(this);
}

void DragValueChangedHelper::SetRange(double minvalue, double maxvalue)
{
    //Q_ASSERT(maxvalue >= minvalue);
    m_min = minvalue;
    m_max = maxvalue;
}

void DragValueChangedHelper::SetValue(double value)
{
    //Q_ASSERT(value >= m_min && m_value <= m_max);
    m_value = value;
}

void DragValueChangedHelper::SetDragEnabled(bool enabled)
{
    m_bDrag = enabled;
}

void DragValueChangedHelper::SetStep(double step)
{
    m_step = step;
}

void DragValueChangedHelper::SetLoop(bool loop)
{
    m_loop = loop;
}

void DragValueChangedHelper::SetInteractiveRectangle(QRect rect)
{
    //Q_ASSERT(rect.isValid());
    m_InteractiveRect = rect;
}

void DragValueChangedHelper::SetHoverCursor(QCursor cursor)
{
    m_hoverCursor = cursor;
}
void DragValueChangedHelper::SetStepDistance(int distance)
{
    m_dragDistance = distance;
}
void DragValueChangedHelper::SetFastDistance(int distance)
{
    m_dragFastDistance = distance;
}

void DragValueChangedHelper::SetSlowDistance(int distance)
{
    m_dragSlowDistance = distance;
}

void DragValueChangedHelper::SetFastStep(double step)
{
    m_fastStep = step;
}

void DragValueChangedHelper::SetSlowStep(double step)
{
    m_slowStep = step;
}


void DragValueChangedHelper::GetRange(double &minvalue, double &maxvalue)
{
    minvalue = m_min;
    maxvalue = m_max;
}


double DragValueChangedHelper::GetValue()
{
    return m_value;
}

bool DragValueChangedHelper::GetDragEnabled()
{
    return m_bDrag;
}

bool DragValueChangedHelper::GetLoop()
{
    return m_loop;
}

double DragValueChangedHelper::GetStep()
{
    return m_step;
}

double DragValueChangedHelper::GetFastStep()
{
    return m_fastStep;
}

double DragValueChangedHelper::GetSlowStep()
{
    return m_slowStep;
}

int DragValueChangedHelper::GetStepDistance()
{
    return m_dragDistance;
}

int DragValueChangedHelper::GetFastDistance()
{
    return m_dragFastDistance;
}

int DragValueChangedHelper::GetSlowDistance()
{
    return m_dragSlowDistance;
}

QRect DragValueChangedHelper::GetInteractiveRectangle()
{
    return m_InteractiveRect;
}

QCursor DragValueChangedHelper::GetHoverCursor()
{
    return m_hoverCursor;
}

void DragValueChangedHelper::SetOverrideCursor(QCursor cursor)
{
    qApp->setOverrideCursor(cursor);
    ++m_setAppCursorCount;
}

void DragValueChangedHelper::RestoreOverrideCursor()
{
    while (m_setAppCursorCount-- > 0)
    {
        qApp->restoreOverrideCursor();
    }
    m_setAppCursorCount = 0;
}
bool DragValueChangedHelper::eventFilter(QObject *watcher, QEvent *event)
{

    auto CurrentPositionFun = [&](QPoint pos){
        return m_parent->mapFromGlobal(pos);
    };

    auto PosInSpinBoxFun = [&](QPoint point){
        if (m_InteractiveRect.isValid())
        {
            return m_parent->isVisible() && m_parent->isEnabled() && m_InteractiveRect.contains(CurrentPositionFun(point));
        }
        else
            return m_parent->isVisible() && m_parent->isEnabled() && m_parent->rect().contains(CurrentPositionFun(point));
    };

    if (m_bDrag)
    {

        static QMouseEvent preMousePressEvent = QMouseEvent(QEvent::MouseButtonPress, QPoint(5, 5), Qt::NoButton, (Qt::MouseButtons)Qt::LeftButton, Qt::NoModifier);

        int dragDistance = m_dragDistance;
        double step = m_step;
        int resetdragDistance = 5;


        if (event->type() == QEvent::MouseMove)
        {
            QMouseEvent *pEvent = static_cast<QMouseEvent*>(event);
            if (/*PosInSpinBoxFun(pEvent) ||*/ m_startDrag)
            {
                if (qApp->mouseButtons() && Qt::LeftButton)
                {
                    auto curX = pEvent->globalPos();

                    if (qApp->keyboardModifiers() & Qt::ShiftModifier)
                    {
                        dragDistance = m_dragFastDistance;
                        step = m_fastStep;
                        resetdragDistance = qMax(resetdragDistance, m_dragFastDistance);
                    }
                    else if (qApp->keyboardModifiers() & Qt::ControlModifier)
                    {
                        dragDistance = m_dragSlowDistance;
                        step = m_slowStep;
                        resetdragDistance = qMax(resetdragDistance, m_dragSlowDistance);
                    }
                    if (m_startValueChanged)
                    {
                        m_dragIncreaseStep = 0;
                        m_dragElapsedTimer.restart();
                    }

                    if (m_dragElapsedTimer.elapsed() > 200)
                    {
                        m_dragIncreaseStep = 0;
                    }
                    bool dragRight = true;
                    if (m_curDragDistance > dragDistance)
                    {
                        if (curX.x() > m_currentPos.x())
                        {
                            m_value = m_value + step + m_dragIncreaseStep;
                        }
                        else if (curX.x() < m_currentPos.x())
                        {
                            m_value = m_value - step - m_dragIncreaseStep;
                            dragRight = false;
                        }
                        else
                        {
                            return false;
                        }

                        if (m_loop)
                        {
                            if (m_value < m_min)
                                m_value = m_max;
                            else if (m_value > m_max)
                                m_value = m_min;
                        }
                        else
                        {
                            m_value = qBound(m_min, m_value, m_max);
                        }

                        if (m_startValueChanged)
                        {
                            //qApp->setOverrideCursor(m_cursor);
                            emit sigStartValueChanged(m_value);
                            m_startValueChanged = false;
                        }

                        emit sigValueChanged(m_value);
                        m_currentPos = curX;
                        m_curDragDistance = 0;
                        if (m_dragRight != dragRight)
                            m_dragIncreaseStep = 0;
                        else
                            m_dragIncreaseStep += step;
                        m_dragRight = dragRight;
                        m_dragElapsedTimer.restart();
                    }


                    m_curDragDistance = qAbs(curX.x() - m_currentPos.x());
                    if (m_curDragDistance > resetdragDistance)
                    {
                        QCursor::setPos(m_dragCursorPos);
                        m_currentPos = m_dragCursorPos;
                    }

                    return true;
                }
            }

        }
        else if (event->type() == QEvent::MouseButtonPress)
        {

            QMouseEvent *pEvent = static_cast<QMouseEvent*>(event);
            if (PosInSpinBoxFun(pEvent->globalPos()) && !m_startDrag)
            {
                SetOverrideCursor(m_cursor);
                QPoint ppp = qApp->desktop()->availableGeometry().bottomRight() / 2;
                QCursor::setPos(ppp);
                m_currentPos = ppp;
                m_dragCursorPos = ppp;
                m_mouserPressPos = pEvent->globalPos();
                m_startDrag = true;
                m_startValueChanged = true;

                preMousePressEvent = QMouseEvent(*pEvent);
                return true;
            }
        }
        else if (event->type() == QEvent::MouseButtonRelease)
        {
            QMouseEvent *pEvent = static_cast<QMouseEvent*>(event);

            if (m_startDrag)
            {
                QCursor::setPos(m_mouserPressPos);
                emit sigEndValueChanged(m_value);
                m_dragIncreaseStep = 0;
                
                // 未拖拽改变过数据, 模拟鼠标按下弹起事件给控件
                if (m_startValueChanged)
                {
                    qApp->sendEvent(watcher, &preMousePressEvent);
                    RestoreOverrideCursor();
                    SetDragEnabled(false);
                }
                else
                {
                    //RestoreOverrideCursor();
                    qApp->restoreOverrideCursor();
                }

            }
            m_startDrag = false;

        }

        if (watcher == m_parent)
        {
            if (m_parent->isVisible() && m_parent->isEnabled())
            {
                if (event->type() == QEvent::Enter)
                {
                    SetOverrideCursor(m_hoverCursor);

                }
                else if (event->type() == QEvent::Leave)
                {
                    RestoreOverrideCursor();
                }
                else if (event->type() == QEvent::FocusOut)
                {
                    SetDragEnabled(true);

                }
            }

        }
    }

    if (watcher == m_parent)
    {
        if (event->type() == QEvent::FocusOut)
        {
            SetDragEnabled(true);
            if (PosInSpinBoxFun(QCursor::pos()))
            {
                SetOverrideCursor(m_hoverCursor);
            }
        }
       
    }


    return false;
}

//////////////////////////////////////////////////////////////////////////
IDragValueChangedHelper::IDragValueChangedHelper(QWidget *parent)
:QObject(parent)
{
    m_p = new DragValueChangedHelper(parent);
    connect(m_p, &DragValueChangedHelper::sigValueChanged, this, &IDragValueChangedHelper::sigValueChanged);
    connect(m_p, &DragValueChangedHelper::sigStartValueChanged, this, &IDragValueChangedHelper::sigStartValueChanged);
    connect(m_p, &DragValueChangedHelper::sigEndValueChanged, this, &IDragValueChangedHelper::sigEndValueChanged);
}

void IDragValueChangedHelper::SetRange(double minvalue, double maxvalue)
{
    m_p->SetRange(minvalue, maxvalue);
}

void IDragValueChangedHelper::SetValue(double value)
{
    m_p->SetValue(value);

}

void IDragValueChangedHelper::SetDragEnabled(bool enabled)
{
    m_p->SetDragEnabled(enabled);

}

void IDragValueChangedHelper::SetLoop(bool loop)
{
    m_p->SetLoop(loop);

}

void IDragValueChangedHelper::SetStep(double step)
{
    m_p->SetStep(step);

}

void IDragValueChangedHelper::SetFastStep(double step)
{
    m_p->SetFastStep(step);

}

void IDragValueChangedHelper::SetSlowStep(double step)
{
    m_p->SetSlowStep(step);

}

void IDragValueChangedHelper::SetStepDistance(int distance)
{
    m_p->SetStepDistance(distance);

}

void IDragValueChangedHelper::SetFastDistance(int distance)
{
    m_p->SetFastDistance(distance);

}

void IDragValueChangedHelper::SetSlowDistance(int distance)
{
    m_p->SetSlowDistance(distance);

}

void IDragValueChangedHelper::SetInteractiveRectangle(QRect rect)
{
    m_p->SetInteractiveRectangle(rect);

}

void IDragValueChangedHelper::SetHoverCursor(QCursor cursor)
{
    m_p->SetHoverCursor(cursor);

}

void IDragValueChangedHelper::GetRange(double &minvalue, double &maxvalue)
{
    m_p->GetRange(minvalue, maxvalue);

}

double IDragValueChangedHelper::GetValue()
{
    return m_p->GetValue();
}

bool IDragValueChangedHelper::GetDragEnabled()
{
    return m_p->GetDragEnabled();

}

bool IDragValueChangedHelper::GetLoop()
{
    return m_p->GetLoop();

}

double IDragValueChangedHelper::GetStep()
{
    return m_p->GetStep();

}

double IDragValueChangedHelper::GetFastStep()
{
    return m_p->GetFastStep();

}

double IDragValueChangedHelper::GetSlowStep()
{
    return m_p->GetSlowStep();

}

int IDragValueChangedHelper::GetStepDistance()
{
    return m_p->GetStepDistance();

}

int IDragValueChangedHelper::GetFastDistance()
{
    return m_p->GetFastDistance();

}

int IDragValueChangedHelper::GetSlowDistance()
{
    return m_p->GetSlowDistance();

}

QRect IDragValueChangedHelper::GetInteractiveRectangle()
{
    return m_p->GetInteractiveRectangle();

}

QCursor IDragValueChangedHelper::GetHoverCursor()
{
    return m_p->GetHoverCursor();

}
