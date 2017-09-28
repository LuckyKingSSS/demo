#ifndef FTIMELINEVIEW_H
#define FTIMELINEVIEW_H

#include <QtWidgets/QWidget>

#include "inc_FTimelineView/ftimelineview_global.h"
#include "inc_FProject/FSequence.h"

class FTimelineViewPrivate;

class FTIMELINEVIEW_EXPORT FTimelineView : public QWidget
{
public:
    FTimelineView(QWidget *parent);
    
    bool Load(FSequencePtr sequence);
    

private:
    FTimelineViewPrivate *m_p;
};

#endif // FTIMELINEVIEW_H
