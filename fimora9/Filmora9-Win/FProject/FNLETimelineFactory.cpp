#include "inc_FProject/FNLETimelineFactory.h"

#include "inc_CommonLib/NLEWraper.h"
#include "inc_CommonLib/PlayerMaterProvider.h"
#include "inc_FProject/FNLETimeline.h"
#include "inc_FProject/FSequence.h"

static FNLETimelineFactory *s_FNLETimelineFactory = nullptr;
FNLETimelineFactory * FNLETimelineFactory::GetInstance()
{
    if (s_FNLETimelineFactory == nullptr)
    {
        s_FNLETimelineFactory = new FNLETimelineFactory();
    }
    return s_FNLETimelineFactory;
}

void FNLETimelineFactory::Release()
{
    if (s_FNLETimelineFactory)
    {
        delete s_FNLETimelineFactory;
        s_FNLETimelineFactory = nullptr;
    }
}

NLEComPtr<FNLETimeline> FNLETimelineFactory::CreateEmptyTimeline()
{
    auto baseTimeline = NLEWraper::GetInstance().GetPlayerMaterProvider()->CreateEmptyTimeline();
    NLEComPtr<FNLETimeline> fnletimeline = new FNLETimeline(baseTimeline);
    return fnletimeline;
}


NLEComPtr<FNLETimeline> FNLETimelineFactory::CreateTimeline(const QString &path)
{
    auto baseTimeline = NLEWraper::GetInstance().GetPlayerMaterProvider()->CreateMediaTimeline(path);
    NLEComPtr<FNLETimeline> fnletimeline = nullptr;
    if(baseTimeline)
    {
        fnletimeline = new FNLETimeline(baseTimeline);
    }
    return fnletimeline;
}

NLEComPtr<FNLETimeline> FNLETimelineFactory::CreateTimeline(NLEComPtr<INLETimelineBase> timeline)
{
    Q_ASSERT(timeline);
    NLEComPtr<FNLETimeline> fnletimeline = new FNLETimeline(timeline);
    return fnletimeline;
}

NLEComPtr<FSequence> FNLETimelineFactory::CreateSequence()
{
    auto baseTimeline = NLEWraper::GetInstance().GetPlayerMaterProvider()->CreateEmptyTimeline();
    NLEComPtr<FSequence> fnletimeline = new FSequence(baseTimeline);
    return fnletimeline;
}

NLEComPtr<FSequence> FNLETimelineFactory::CreateSequenceFromTimeline(NLEComPtr<FNLETimeline> timeline)
{
    auto sequence = CreateSequence();
    sequence->AddTimeline(0, 0, timeline);
    return sequence;
}

NLEComPtr<INLETimelineBase> FNLETimelineFactory::CloneTimeline(NLEComPtr<INLETimelineBase> timeline)
{
    Q_ASSERT(timeline);
    return NLEWraper::GetInstance().GetPlayerMaterProvider()->CloneTimeline(timeline);

}
