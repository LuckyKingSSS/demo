#ifndef FNLEVIDEO_H
#define FNLEVIDEO_H

#include "inc_FProject/FMediaTimeline.h"


class FPROJECT_EXPORT FNLEVideo : public FMediaTimeline
{
    Q_OBJECT
public:
    NLEComPtr<FNLETimeline> clone() override;

protected:
    FNLEVideo(NLEComPtr<INLETimelineBase> timeline, QObject *parent);

};










#endif // FNLETIMELINE_H
