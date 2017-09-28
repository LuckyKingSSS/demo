
#include "inc_FProject/FSequence.h"

#include <QtCore/QRect>

#include "UndoCommand.h"

#include "inc_FProject/FTrack.h"
#include "inc_FProject/FDateDefine.h"

#define INVALID_TIMELINEINDEX(index, list) Q_ASSERT(index>=0 && index < list.size())

FSequence::FSequence(NLEComPtr<INLETimelineBase> timeline, QObject *parent)
:FNLETimeline(timeline, parent)
{
    m_currentFrames = 0;
}

FSequence::~FSequence()
{

}

int FSequence::GetDuration()
{
    return 0;
}

void FSequence::SetCurrentFrames(int frames)
{
    m_currentFrames = frames;
}

int FSequence::GetCurrentFrames()
{
    return m_currentFrames;
}

void FSequence::AddTimeline(int trackIndex, int frames, FNLETimelinePtr timeline)
{
    INVALID_TIMELINEINDEX(trackIndex, m_tracks);

    auto track = m_tracks[trackIndex];
    //auto clip = CreateClipFromTimeline(timeline);
    BeginComposite();
    PushCommand(new AddTimelineCommand(track, timeline, frames));
    PushUpdateCommond();
    EndComposite();
    timeline->SetSelected(true);

    //if (bEmitSelectChange) 
    //    PushCommand(new EmitSelectChangeCommand(this));

    //emit GetPlayerEditManager()->sigTimelineAdded(timeline);
}

void FSequence::MoveTimeline(int trackIndex, int frames, FNLETimelinePtr timeline)
{
    INVALID_TIMELINEINDEX(trackIndex, m_tracks);

    FTrackPtr srcTrackPtr = timeline->GetTrack();
    //for (auto temp : m_tracks)
    //{
    //    if (temp->HaveClip(timeline))
    //    {

    //        srcTrackPtr = temp;
    //        break;
    //    }
    //}
    Q_ASSERT(srcTrackPtr);

    auto destTrack = m_tracks[trackIndex];
    BeginComposite();
    PushCommand(new MoveTimelineCommand(srcTrackPtr, destTrack, timeline, frames));
    PushUpdateCommond();
    EndComposite();

}

void FSequence::InsertTrack(int trackIndex)
{
    Q_ASSERT(trackIndex >= 0 && trackIndex <= m_tracks.size());

    FTrackPtr track = new FTrack(this);
    BeginComposite();
    PushCommand(new InsertTrackCommand(this, trackIndex, track));
    PushRebuildTrackCommond();
    EndComposite();
}

void FSequence::RemoveTrack(int index)
{
    INVALID_TIMELINEINDEX(index, m_tracks);

    BeginComposite();
    PushCommand(new RemoveTrackCommand(this, m_tracks[index]));
    PushRebuildTrackCommond();
    EndComposite();
}

void FSequence::RemoveClips()
{
    BeginComposite();
    bool bRemoved = false;
    for (auto temp : m_tracks)
    {
        for (auto timeline : temp->GetSelected())
        {
            bRemoved = true;
            PushCommand(new RemoveTimelineCommand(temp, timeline));
        }
    }
    if (bRemoved)
    {
        PushCommand(new SelectChangeCommand(this));
    }

    //RemoveTailEmptyTrack();

    EndComposite();
}

bool FSequence::CanRemove()
{
    bool bCan = false;
    for (auto temp : m_tracks)
    {
        auto timelines = temp->GetSelected();
        if (timelines.size() > 0)
        {
            return true;
        }
    }
    //if (GetMarkPointMgr()->GetSelected())
    //{
    //    return true;
    //}
    return false;
}

FNLETimelines FSequence::GetSelected()
{
    FNLETimelines timelines;
    for (auto temp : m_tracks)
    {
        timelines << temp->GetSelected();
    }
    return timelines;
}

FNLETimelines FSequence::GetSelected(int frames)
{
    FNLETimelines timelines;
    for (auto temp : m_tracks)
    {
        auto timeline = temp->GetSelected(frames);
        if (timeline)
            timelines.push_back(timeline);
    }
    return timelines;
}

FNLETimelinePtr FSequence::GetSelected(int trackIndex, int frames)
{
    auto track = m_tracks[trackIndex];
    return track->GetTimeline(frames);
}

FNLETimelinePtr FSequence::GetCurrentSelected()
{
    return m_curSelectedTimeline;
}

void FSequence::SetCurrentSelected(FNLETimelinePtr timeline)
{
    m_curSelectedTimeline = timeline;
}

void FSequence::SetSelected(int trackIndex, int frame, bool enabled, bool multiSelected /*= false*/)
{
    INVALID_TIMELINEINDEX(trackIndex, m_tracks);

    auto preclips = GetSelected();
    if (!multiSelected && !preclips.isEmpty())
    {
        UnselectAll();
    }

    auto curClip = m_tracks[trackIndex]->GetTimeline(frame);
    if (curClip)
    {
        bool preSelected = curClip->GetSelected();
        curClip->SetSelected(enabled);

        emit sigTrackUpdate();

        if (preclips.contains(curClip))
        {
            // 已被选中状态不一样
            if (preSelected != enabled)
                emit sigSelectionChanged();
        }
        else
        {
            // 未被选中现在被选中
            if (enabled)
                emit sigSelectionChanged();
        }
    }
}

void FSequence::SingleSelect(int trackIndex, int frame)
{
    INVALID_TIMELINEINDEX(trackIndex, m_tracks);

    auto preTimelines = GetSelected();

    auto curTimeline = m_tracks[trackIndex]->GetTimeline(frame);

    for (auto temp : preTimelines)
    {
        temp->SetSelected(false);
    }

    if (curTimeline)
    {
        curTimeline->SetSelected(true);
    }

    
    if (!(preTimelines.size() == 1 && preTimelines.contains(curTimeline)))
    {
        emit sigSelectionChanged();
    }
    
   
    SetCurrentSelected(curTimeline);
}

void FSequence::CtrlSelect(int trackIndex, int frame)
{
    INVALID_TIMELINEINDEX(trackIndex, m_tracks);

    auto selectedTimeline = m_tracks[trackIndex]->GetTimeline(frame);
    if (selectedTimeline)
    {
        selectedTimeline->SetSelected(!selectedTimeline->GetSelected());
        emit sigSelectionChanged();
    }

}

void FSequence::ShiftSelect(int trackIndex, int frame)
{

    INVALID_TIMELINEINDEX(trackIndex, m_tracks);

    auto selecteds = GetSelected();
   
    auto pressSelected = m_tracks[trackIndex]->GetTimeline(frame);

    auto UnSelectAllFun = [&](FNLETimelines selecteds){
        for (auto temp : selecteds)
        {
            temp->SetSelected(false);
        }
    };

    if (!selecteds.isEmpty())
    {
        UnSelectAllFun(selecteds);
        if (pressSelected)
        {
            int startTrackIndex = -1;
            if (GetCurrentSelected())
                startTrackIndex = GetCurrentSelected()->GetIndex();
            else
                startTrackIndex = selecteds.first()->GetIndex();

            int endTrackIndex = pressSelected->GetIndex();

            if (startTrackIndex > endTrackIndex)
                std::swap(startTrackIndex, endTrackIndex);

            for (int i = startTrackIndex; i <= endTrackIndex; ++i)
            {
                for (auto tempTimeline : m_tracks[i]->GetTimelines())
                {
                    tempTimeline->SetSelected(true);
                }
            }

        }
        if (selecteds != GetSelected())
        {
            emit sigSelectionChanged();
        }

    }
    else
    {
        if (pressSelected)
        {
            SingleSelect(trackIndex, frame);
        }
    }

}

void FSequence::RectangleSelect(int trackFrom, int trackTo, int frameFrom, int frameTo)
{
    FNLETimelines selectedtimelines;
    for (int i = trackFrom; i <= trackTo; ++i)
    {
        auto trackData = m_tracks[i];
        auto dstRect = QRect(frameFrom, 0, frameTo - frameFrom + 1, 10);
        for (auto timeline : trackData->GetTimelines())
        {
            auto clipRect = QRect(timeline->GetStartAtTrack(), 0, timeline->GetLenAtTrack(), 10);
            if (clipRect.intersects(dstRect))
                selectedtimelines.push_back(timeline);
        }

    }

    auto preSelectedTimelines = GetSelected();
    for (auto timeline : preSelectedTimelines)
        timeline->SetSelected(false);
    for (auto timeline : selectedtimelines)
        timeline->SetSelected(true);

    if (preSelectedTimelines != selectedtimelines)
    {
        emit sigSelectionChanged();
    }
}

void FSequence::UnselectAll()
{
    bool selected = false;
    auto timelines = GetSelected();
    for (auto temp : timelines)
    {
        selected |= temp->GetSelected();
        temp->SetSelected(false);
    }

    SetCurrentSelected(nullptr);

    emit sigTrackUpdate();

    if (selected)
        emit sigSelectionChanged();
}

void FSequence::SelectedAll()
{
    bool unSelected = false;
    //FNLETimelines timelines;
    for (auto temp : m_tracks)
    {
        for (auto timeline : temp->GetTimelines())
        {
            unSelected |= !timeline->GetSelected();
            timeline->SetSelected(true);
        }
    }
    emit sigTrackUpdate();
    if (unSelected)
        emit sigSelectionChanged();
}


void FSequence::Split()
{
    int frames = GetCurrentFrames();
    BeginComposite();
    for (auto temp : m_tracks)
    {
        if (temp->CanSplit(frames))
        {
            auto timeline = temp->GetTimeline(frames);
            PushCommand(new SplitClipCommand(temp, timeline, frames));
            //PushCommand(new EmitSelectChangeCommand(this));
        }
    }
    PushUpdateCommond();
    EndComposite();
}

bool FSequence::CanSplit()
{
    int frames = GetCurrentFrames();
    bool canSplit = false;
    for (auto temp : m_tracks)
    {
        if (temp->CanSplit(frames))
        {
            canSplit = true;
            break;
        }
    }
    return canSplit;
}

FTracks FSequence::GetTracks()
{
    return m_tracks;
}

FSequence* FSequence::Clone()
{
    Q_ASSERT(false);
    return 0;
}

void FSequence::_InsertTrack(int trackIndex, FTrackPtr track)
{
    m_tracks.insert(trackIndex, track);
    for (auto timeline : track->GetTimelines())
    {
        AddClip(timeline, timeline->GetStartAtTrack(), track->GetIndex());
    }
}

void FSequence::_RemoveTrack(FTrackPtr track)
{
    int preIndex = track->GetIndex();
    //track->SetBeforeRemoveIndex(preIndex);
    m_tracks.removeAt(preIndex);
    for (auto timeline : track->GetTimelines())
    {
        RemoveClip(timeline);
    }
}

void FSequence::_ResetTimelineLevel()
{
    for (auto track : GetTracks())
    {
        int nIndex = track->GetIndex();
        for (auto timeline : track->GetTimelines())
        {
            timeline->SetLevel(nIndex);
        }
    }
}

void FSequence::PushCommand(FUndoCommand *commond)
{
    FSEQUNECE_UNDO->PushCommand(commond);
}

void FSequence::BeginComposite()
{
    FSEQUNECE_UNDO->BeginComposite();

}

void FSequence::EndComposite()
{
    FSEQUNECE_UNDO->EndComposite();

}

void FSequence::PushUpdateCommond()
{
    auto *commond = new UpdateCommand;
    connect(commond, &UpdateCommand::sigTrackUpdate, this, &FSequence::sigTrackUpdate);
    PushCommand(commond);
}

void FSequence::PushRebuildTrackCommond()
{
    auto *commond = new RebuildTrackCommand(this);
    connect(commond, &RebuildTrackCommand::sigTrackRebuild, this, &FSequence::sigTrackRebuild);
    PushCommand(commond);
}
