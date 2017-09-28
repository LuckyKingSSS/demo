#ifndef FNLETIMELINEFACTORY_H
#define FNLETIMELINEFACTORY_H

#include "inc_FProject/fproject_global.h"
#include "NLEComPtr.h"

class FNLETimeline;
class INLETimelineBase;
class FSequence;

class FPROJECT_EXPORT FNLETimelineFactory
{
public:
    static FNLETimelineFactory *GetInstance();
    void Release();

    NLEComPtr<FNLETimeline> CreateEmptyTimeline();
    NLEComPtr<FNLETimeline> CreateTimeline(const QString &path);
    NLEComPtr<FNLETimeline> CreateTimeline(NLEComPtr<INLETimelineBase> timeline);

    NLEComPtr<FSequence> CreateSequence();
    NLEComPtr<FSequence> CreateSequenceFromTimeline(NLEComPtr<FNLETimeline> timeline);


    NLEComPtr<INLETimelineBase> CloneTimeline(NLEComPtr<INLETimelineBase> timeline);

};


#endif 
