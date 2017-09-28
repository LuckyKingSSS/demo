#ifndef FSEQUENCE_H
#define FSEQUENCE_H

#include "inc_FProject/FNLETimeline.h"
#include "inc_FProject/FTrack.h"

class FUndoCommand;

class FPROJECT_EXPORT FSequence : public FNLETimeline
{
    Q_OBJECT
public:

    // 获取时间线总时长
    int GetDuration();
    // 设置当前帧
    void SetCurrentFrames(int frames);

    int GetCurrentFrames();

    // 增加片段
    void AddTimeline(int trackIndex, int frames, FNLETimelinePtr timeline);
    // 移动片段
    void MoveTimeline(int trackIndex, int frames, FNLETimelinePtr timeline);

    FTracks GetTracks();
    // 插入轨道
    void InsertTrack(int trackIndex);
    // 移除轨道
    void RemoveTrack(int index);

    // 删除选择片段
    void RemoveClips();
    bool CanRemove();

    // 获取选择所有片段
    FNLETimelines GetSelected();
    // 获取位置frames中选择的片段
    FNLETimelines GetSelected(int frames);
    // 获取轨道trackIndex位置frames选择的片段
    FNLETimelinePtr GetSelected(int trackIndex, int frames);
    // 当前鼠标点中选择的片段
    FNLETimelinePtr GetCurrentSelected();
    void SetCurrentSelected(FNLETimelinePtr timeline);

    // 选择片段trackIndex:第几轨道, frames:第几帧, enabled:true为选择false为不选择,multiSelected:是否是多选
    void SingleSelect(int trackIndex, int frame);
    void CtrlSelect(int trackIndex, int frame);
    void ShiftSelect(int trackIndex, int frame);
    void RectangleSelect(int trackFrom, int trackTo, int frameFrom, int frameTo);
    void SetSelected(int trackIndex, int frames, bool enabled, bool multiSelected = false);

    // 清空所有选择
    void UnselectAll();
    // 选择所有片段
    void SelectedAll();


    void Split();
    bool CanSplit();


    //// 删除选择片段
    //void RemoveClips();
    //bool CanRemove();

    //// trim片段 leftSide:trim片段的左边true, frames:当前trim中在时间线上的帧
    //void AdjustClip(bool leftSide, int frames);

    FSequence* Clone() override;

Q_SIGNALS:
    // 轨道更新信号
    void sigTrackUpdate();
    // 轨道重建信号
    void sigTrackRebuild();
    // 片段选择改变信号
    void sigSelectionChanged();

    void sigPause();

    //void sigDoCommand();

    //void sigPasted(FSNLETimeline *timeline);

public:
    void _InsertTrack(int trackIndex, FTrackPtr track);
    void _RemoveTrack(FTrackPtr track);
    void _ResetTimelineLevel();

protected:
    FSequence(NLEComPtr<INLETimelineBase> timeline, QObject *parent = 0);
    ~FSequence();

protected:
    void PushCommand(FUndoCommand *commond);
    void BeginComposite();
    void EndComposite();

    void PushUpdateCommond();
    void PushRebuildTrackCommond();

protected:
    FTracks m_tracks;
    FNLETimelinePtr m_curSelectedTimeline;

    int m_currentFrames;

    friend class FNLETimelineFactory;

};

typedef NLEComPtr<FSequence> FSequencePtr;

#endif // FNLETIMELINE_H
