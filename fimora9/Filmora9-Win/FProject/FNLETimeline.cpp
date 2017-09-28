#include "inc_FProject/FNLETimeline.h"
#include "NLEKey.h"

#include "inc_FProject/FNLETimelineFactory.h"
#include "inc_FProject/FTrack.h"

FNLETimeline::FNLETimeline(NLEComPtr<INLETimelineBase> timeline, QObject *parent /*= nullptr*/)
:NLETimeline(timeline, ExactType::ExactType_Unknown, parent)
, FRef()
{
    m_type = FNLETimeline::Unknown;
    m_selected = false;
    m_index = -1;
    m_track = nullptr;
}



FNLETimeline* FNLETimeline::Clone()
{
    Q_ASSERT(false);
    return 0;
}

FNLETimeline::Type FNLETimeline::GetType()
{
    return m_type;
}

void FNLETimeline::SetType(Type type)
{
    m_type = type;
}


int FNLETimeline::GetStartAtTrack()
{
    return GetPosition();
}

int FNLETimeline::GetEndAtTrack()
{
    return GetStartAtTrack() + GetLenAtTrack() - 1;

}

int FNLETimeline::GetLenAtTrack()
{
    return GetPlayLen();

}

int FNLETimeline::GetStartAtSrc()
{
    return GetTrimin();

}

int FNLETimeline::GetSrcLen()
{
    return GetTotalLength();

}

void FNLETimeline::SetStartAtTrack(int pos)
{
    SetPosition(pos);
}

bool FNLETimeline::GetSelected()
{
    return m_selected;
}

void FNLETimeline::SetSelected(bool selected)
{
    m_selected = selected;
}


int FNLETimeline::GetIndex()
{
    int index = m_track->GetIndex();
    Q_ASSERT(index >= 0);
    return index;
}

NLEComPtr<FNLETimeline> FNLETimeline::NextTimeline()
{
    Q_ASSERT(false);
    return 0;
}

NLEComPtr<FNLETimeline> FNLETimeline::PreTimeline()
{
    Q_ASSERT(false);
    return 0;
}

NLEComPtr<FNLETimeline> FNLETimeline::clone()
{
    NLEComPtr<INLETimelineBase> pCloned = FNLETimelineFactory::GetInstance()->CloneTimeline(m_pSrcTimeLine);
    FNLETimeline *pNewOne = new FNLETimeline(pCloned);
    pNewOne->m_strAncestralGuid = m_strAncestralGuid;
    pNewOne->m_type = m_type;
    pNewOne->m_index = m_index;
    pNewOne->m_selected = m_selected;
    pNewOne->m_track = m_track;
    return pNewOne;
}

//FNLETimeline* FNLETimeline::Clone()
//{
//    NLEComPtr<INLETimelineBase> pCloned = FNLETimelineFactory::GetInstance()->CloneTimeline(m_pSrcTimeLine);
//    FNLETimelinePtr pNewOne = new FNLETimeline(pCloned);
//    pNewOne->m_strAncestralGuid = m_strAncestralGuid;
//    return pNewOne;
//}

void FNLETimeline::SetTrack(FTrack* track)
{
    m_track = track;
}

FTrack* FNLETimeline::GetTrack()
{
    return m_track;
}

//STDMETHODIMP FNLETimeline::QueryInterface(REFIID riid, void **ppv)
//{
//    (*ppv) = this;
//    return S_OK;
//}
//
//STDMETHODIMP_(ULONG) FNLETimeline::Release()
//{
//    if (--m_ref == 0)
//    {
//        delete this;
//        return 0;
//    }
//    return m_ref;
//}
//
//STDMETHODIMP_(ULONG) FNLETimeline::AddRef()
//{
//    return ++m_ref;
//}


//////////////////////////////////////////////////////////////////////////
FNLEDragTimeline::FNLEDragTimeline(FNLETimelinePtr timeline)
{
    m_startAtTrack = 0;
    m_trackIndex = 0;
    m_lenAtTrack = 0;

    m_timeline = timeline;
    if (timeline)
    {
        m_startAtTrack = timeline->GetStartAtTrack();
        m_trackIndex = timeline->GetIndex();
        m_lenAtTrack = timeline->GetLenAtTrack();
    }

}


int FNLEDragTimeline::GetStartAtTrack()
{
    return m_startAtTrack;
}


int FNLEDragTimeline::GetLenAtTrack()
{
    return m_lenAtTrack;
}


void FNLEDragTimeline::SetStartAtTrack(int frames)
{
    m_startAtTrack = frames;
}

int FNLEDragTimeline::GetIndex()
{
    return m_trackIndex;
}

void FNLEDragTimeline::SetIndex(int index)
{
    m_trackIndex = index;
}

FNLETimelinePtr FNLEDragTimeline::GetTimeline()
{
    return m_timeline;
}
