
#include "inc_FProject/FMediaTimeline.h"

FMediaTimeline::FMediaTimeline(NLEComPtr<INLETimelineBase> timeline, QObject *parent)
:FNLETimeline(timeline, parent)
{

}


QString FMediaTimeline::GetFilePath()
{
    return "";
}

