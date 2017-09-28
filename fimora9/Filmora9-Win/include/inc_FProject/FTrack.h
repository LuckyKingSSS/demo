#ifndef FTRACK_H
#define FTRACK_H

#include "inc_FProject/FNLETimeline.h"

class FSequence;

class FPROJECT_EXPORT FTrack : public FRef
{
public:
    FTrack(FSequence *sequence);
    ~FTrack();

    FSequence* GetSequence();

    // 获取所有片段
    FNLETimelines GetTimelines();
    // 获取位置frames处片段
    FNLETimelinePtr GetTimeline(int frames);

    FNLEDragTimelines GetDragTimelines();
    void SetDragTimelines(FNLEDragTimelines timelines);

    // 获取当前轨道Index
    int GetIndex();

    // 获取轨道时长(帧)
    int GetDuration();

    void AddTimeline(int frames, FNLETimelinePtr timeline);

    void RemoveTimeline(FNLETimelinePtr timeline);

    void MoveTimeline(int frames, FNLETimelinePtr timeline);



    //获取排除rejectClips之后的clips
    //Clips GetRejectClips(Clips rejectClips);

    //获取包含在你FromFrame到nEndFrame之间的clip
    //Clips GetInsideClips(int nFromFrame, int nEndFrame);

    //获取包含在你FromFrame到nEndFrame之间和之外的clip
   // QPair<Clips, Clips> GetInsideOutsizeClips(int nFromFrame, int nEndFrame);

    //获取nFrame右边的clips，包含nFrame
    //Clips GetRightClips(int nFrame);
    //获取nFrame左边的clips，包含nFrame
    //Clips GetLeftClips(int nFrame);

    // 获取选择片段
    FNLETimelines GetSelected();
    FNLETimelinePtr GetSelected(int frame);
    //获取未选中片段
    FNLETimelines GetUnselected();

public:
    bool CanSplit(int frames);
    FNLETimelinePtr Split(FNLETimelinePtr clip, int frames);
    void Split(FNLETimelinePtr clip, FNLETimelinePtr clip2);
    void UnSplit(FNLETimelinePtr clip, FNLETimelinePtr clip2);
private:
    FNLETimelines m_timelines;
    FNLEDragTimelines m_dragTimelines;
    FSequence *m_sequence;
};

typedef NLEComPtr<FTrack> FTrackPtr;
typedef QList<FTrackPtr> FTracks;

#endif // FNLETIMELINE_H
