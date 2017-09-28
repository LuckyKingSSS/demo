
#include "inc_FProject/FVideo.h"
#include "FVideo_p.h"

FNLEVideo::FNLEVideo(NLEComPtr<INLETimelineBase> timeline, QObject *parent)
:FMediaTimeline(timeline, parent)
{
}

NLEComPtr<FNLETimeline> FNLEVideo::clone()
{
    Q_ASSERT(false);
    return 0;
}


//////////////////////////////////////////////////////////////////////////
