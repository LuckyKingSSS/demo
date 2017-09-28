#include "inc_FProject/FTrack.h"

#include "inc_FProject/FSequence.h"

FTrack::FTrack(FSequence *sequence)
:FRef()
{
    m_sequence = sequence;
}

FTrack::~FTrack()
{
    int i = 0;
}

FNLETimelines FTrack::GetTimelines()
{
    return m_timelines;
}

FNLETimelinePtr FTrack::GetTimeline(int frames)
{
    FNLETimelinePtr timeline;
    for (auto tempTimeline : GetTimelines())
    {
        if (frames >= tempTimeline->GetStartAtTrack() && frames < tempTimeline->GetStartAtTrack() + tempTimeline->GetLenAtTrack())
        {
            timeline = tempTimeline;
            break;
        }
    }

    return timeline;
}

FNLEDragTimelines FTrack::GetDragTimelines()
{
    return m_dragTimelines;
}

void FTrack::SetDragTimelines(FNLEDragTimelines timelines)
{
    m_dragTimelines = timelines;
}

int FTrack::GetIndex()
{
    auto tracks = m_sequence->GetTracks();
    for (int i = 0; i < tracks.size(); ++i)
    {
        if (this == tracks[i])
            return i;
    }
    Q_ASSERT(false);
    return -1;
}

int FTrack::GetDuration()
{
    int duration = 0;
    if (!m_timelines.isEmpty())
    {
        auto clip = m_timelines.last();
        duration = clip->GetStartAtTrack() + clip->GetLenAtTrack();
    }
    return duration;
}

void FTrack::AddTimeline(int frames, FNLETimelinePtr timeline)
{
    //clip->SetSelected(true);
    timeline->SetStartAtTrack(frames);
    timeline->SetTrack(this);
    //if (!clip->GetNLETimeline()->GetBackgroundColor().isValid())
    //    clip->GetNLETimeline()->SetBackgroundColor(m_manager->GetClipDefaultColor(clip.get()));

   // FSNLETimeline *pSrc1 = clip->GetNLETimeline();
    int trimin = timeline->GetTrimin();
    int trimout = timeline->GetTrimout();
    m_sequence->AddClip(timeline, frames, GetIndex());
    m_sequence->SetClipRange(timeline, trimin, trimout);

    bool haveInsert = false;
    for (int i = 0; i < m_timelines.size(); ++i)
    {
        if (frames < m_timelines[i]->GetStartAtTrack())
        {
            m_timelines.insert(i, timeline);
            haveInsert = true;
            break;
        }
    }
    if (!haveInsert)
    {
        m_timelines.push_back(timeline);
    }

    //auto type = clip->GetNLETimeline()->GetType();
    //if (type == TimelineType_VideoAudio || type == TimelineType_Audio)
    //{
    //    AudioWaveMgr::GetInstance()->Add(clip->GetNLETimeline());
    //}
}

void FTrack::RemoveTimeline(FNLETimelinePtr timeline)
{
    m_timelines.removeOne(timeline);
    m_sequence->RemoveClip(timeline);
}

void FTrack::MoveTimeline(int frames, FNLETimelinePtr timeline)
{
    RemoveTimeline(timeline);
    AddTimeline(frames, timeline);
}

bool FTrack::CanSplit(int frames)
{
    // 可split最小帧数
    const int minSplitFrames = 1;
    for (auto temp : m_timelines)
    {
        if (temp->GetSelected() && (frames >= temp->GetStartAtTrack() + minSplitFrames && (frames <= temp->GetStartAtTrack() + temp->GetLenAtTrack() - minSplitFrames)))
        {
            return true;
        }
    }
    return false;
}

FNLETimelinePtr FTrack::Split(FNLETimelinePtr timeline, int frames)
{
    auto timeline2 = timeline->clone();

    auto rootTimeline = GetSequence();
    //auto pSrc1 = timeline;
    int trimin = timeline->GetTrimin();
    int trimout = timeline->GetTrimout();

    //auto pSrc2 = timeline2;

    int nOffset = frames - timeline->GetStartAtTrack();
    rootTimeline->SetClipRange(timeline, timeline->GetTrimin(), timeline->GetTrimin() + nOffset - 1);

    timeline2->SetStartAtTrack(timeline->GetStartAtTrack() + nOffset);
    rootTimeline->AddClip(timeline2, timeline->GetStartAtTrack() + nOffset, GetIndex());
    rootTimeline->SetClipRange(timeline2, trimin + nOffset, trimout);

    int nIndex = timeline->GetIndex();
    m_timelines.insert(nIndex + 1, timeline2);

    //m_manager->SetSelected(GetIndex(), frames, true, true);
    //timeline2->SetType(timeline2->GetType());

    timeline->SetSelected(false);
    timeline2->SetSelected(true);

    //auto type = timeline2->GetNLETimeline()->GetType();
    //if (type == TimelineType_VideoAudio || type == TimelineType_Audio)
    //{
    //    AudioWaveMgr::GetInstance()->Add(timeline2->GetNLETimeline());
    //}

    return timeline2;
}

void FTrack::Split(FNLETimelinePtr timeline1, FNLETimelinePtr timeline2)
{
    auto rootTimeline = GetSequence();

    //NLETimeline *pClipSrc = timeline1;
    int trimin = timeline1->GetTrimin();
    int trimout = timeline1->GetTrimout();

    //auto pSrc2 = timeline2;
    int trimin2 = timeline2->GetTrimin();
    int trimout2 = timeline2->GetTrimout();

    rootTimeline->SetClipRange(timeline1, trimin, trimin + timeline1->GetLenAtTrack() - timeline2->GetLenAtTrack() - 1);

    rootTimeline->AddClip(timeline2, timeline2->GetStartAtTrack(), GetIndex());
    rootTimeline->SetClipRange(timeline2, trimin2, trimout2);

    int nIndex = timeline1->GetIndex();
    m_timelines.insert(nIndex + 1, timeline2);

    //timeline2->SetType(timeline2->GetType());

    timeline1->SetSelected(false);
    timeline2->SetSelected(true);

    //auto type = timeline2->GetType();
    //if (type == TimelineType_VideoAudio || type == TimelineType_Audio)
    //{
    //    AudioWaveMgr::GetInstance()->Add(clip2->GetNLETimeline());
    //}
}

void FTrack::UnSplit(FNLETimelinePtr timeline1, FNLETimelinePtr timeline2)
{
   // NLETimeline *pSrc = clip->GetNLETimeline();

    int nLen1 = timeline1->GetLenAtTrack();
    int nLen2 = timeline2->GetLenAtTrack();

    auto rootTimeline = GetSequence();
    rootTimeline->SetClipRange(timeline1, timeline1->GetTrimin(), timeline1->GetTrimin() + nLen1 + nLen2 - 1);
    rootTimeline->RemoveClip(timeline2);
    m_timelines.removeOne(timeline2);

    timeline1->SetSelected(true);
}

FSequence* FTrack::GetSequence()
{
    return m_sequence;
}

FNLETimelines FTrack::GetSelected()
{
    FNLETimelines selecteds;
    for (auto tempTimeline : GetTimelines())
    {
        if (tempTimeline->GetSelected())
        {
            selecteds.push_back(tempTimeline);
        }
    }
    return selecteds;
}

FNLETimelinePtr FTrack::GetSelected(int frame)
{
    for (auto tempTimeline : GetTimelines())
    {
        if (tempTimeline->GetSelected() && frame >= tempTimeline->GetStartAtTrack() && frame < tempTimeline->GetStartAtTrack() + tempTimeline->GetLenAtTrack())
        {
            return tempTimeline;
        }
    }
    return nullptr;
}

FNLETimelines FTrack::GetUnselected()
{
    FNLETimelines unselecteds;
    for (auto tempTimeline : GetTimelines())
    {
        if (!tempTimeline->GetSelected())
        {
            unselecteds.push_back(tempTimeline);
        }
    }

    return unselecteds;
}
