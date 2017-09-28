#ifndef FMEDIATIMELINE_H
#define FMEDIATIMELINE_H

#include "inc_FProject/FNLETimeline.h"

class FTimelineDraw;

class FPROJECT_EXPORT FMediaTimeline : public FNLETimeline
{
    Q_OBJECT
public:
    QString GetFilePath();

protected:
    FMediaTimeline(NLEComPtr<INLETimelineBase> timeline, QObject *parent);

};



#endif // FNLETIMELINE_H
