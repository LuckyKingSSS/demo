#ifndef TIMELINECLIP_H
#define TIMELINECLIP_H

#include <memory>

class FSNLETimeline;
class TimelineTrack;

/*
	时间线片段
*/
class Clip
{
public:
	// 片段在轨道上位置
	int GetStartAtTrack();

	int GetEndAtTrack();
	
	//片段长度
	int GetLenAtTrack();

	// 片段的trimin
	int GetStartAtSrc();
	// 片段原始时长
	int GetSrcLen();

	// 设置片段位置(此处只保存变量, 当片段被真正增加到NLETimeline时使用此变量设置位置)
	void SetStartAtTrack(int pos);

	// 片段是否选择
	bool GetSelected();

	// 获取片段类型
	int GetType() const;
	// 获取片段Name
	QString GetName() const;

	// 获取片段所在轨道Index
	int GetIndex();

	// 下一个片段
	Clip* NextClip();
	// 上一个片段
	Clip* PreClip();

	FSNLETimeline* GetNLETimeline();


protected:
	virtual void CloneAble(Clip* pAnther);

public:
	Clip(FSNLETimeline *timeline);
	~Clip();

	virtual Clip* Clone();

	void SetTimelineTrack(TimelineTrack *track);
	TimelineTrack* GetTimelineTrack();
	// 获取clip曾在轨道,clip不在轨道上,用在Copy和Cut出来的clip上
	void SetCopyIndex(int index);
	int GetCopyIndex();
	void SetSelected(bool selected);

private:
	FSNLETimeline *m_timeline;
	bool m_selected;
	int m_index;
	int m_startAtTrack;

	TimelineTrack *m_track;

};

typedef QList<Clip*> Clips;
typedef std::shared_ptr<Clip> ClipPtr;
typedef QList<ClipPtr> ClipsPtr;


class TimelineManager;

/*
	时间线轨道
*/
class TimelineTrack
{
public:
	// 获取所有片段
	Clips GetClips();
	//获取排除rejectClips之后的clips
	Clips GetRejectClips(Clips rejectClips);

	//获取包含在你FromFrame到nEndFrame之间的clip
	Clips GetInsideClips(int nFromFrame, int nEndFrame);

	//获取包含在你FromFrame到nEndFrame之间和之外的clip
	QPair<Clips,Clips> GetInsideOutsizeClips(int nFromFrame, int nEndFrame);

	//获取nFrame右边的clips，包含nFrame
	Clips GetRightClips(int nFrame);
	//获取nFrame左边的clips，包含nFrame
	Clips GetLeftClips(int nFrame);

	// 获取选择片段
	Clips GetSelected();
	//获取未选中片段
	Clips GetUnselected();

	// 获取当前轨道Index
	int GetIndex();

	// 获取轨道时长(帧)
	int GetDuration();

	// 获取位置frames处片段
	Clip* GetClip(int frames);

	void Hide();
	void Show();
	void Freeze();
	void Unfreeze();
	bool IsHiden();
	bool IsFreeze();
	bool IsEditable();

	bool IsEmpty();
	TimelineManager* GetManager();

private:
	void SetManager(TimelineManager *manager);
	bool HaveClip(ClipPtr clip);
	bool CanSplit(int frames);

public:
	TimelineTrack();

	void AddClip(int frames, ClipPtr clip);
	void RemoveClip(ClipPtr clip);
	void MoveClip(int frames, ClipPtr clip);

	ClipsPtr GetSelectedPtr();
	ClipPtr GetSelectedPtr(int frames);
	ClipPtr GetClipPtr(int frames);
	ClipsPtr GetClipsPtr();
	void RemoveClips();

	ClipPtr Split(ClipPtr clip, int frames);
	void Split(ClipPtr clip, ClipPtr clip2);
	void UnSplit(ClipPtr clip, ClipPtr clip2);

	void AdjustClip(int leftSide, int pos);


	int GetBeforeRemoveIndex();
	void SetBeforeRemoveIndex(int index);
	bool HintCollision(int frames, int duration);
	int GetClipIndex(Clip *clip);

private:
	ClipsPtr m_clips;
	TimelineManager *m_manager;
	int m_beforeRemoveIndex;
	friend class TimelineManager;

	bool m_bHide;
	bool m_bFreeze;
};
typedef QList<TimelineTrack*> TimelineTracks;
typedef std::shared_ptr<TimelineTrack> TimelineTrackPtr;
typedef QList<TimelineTrackPtr> TimelineTracksPtr;


#endif // TIMELINECLIP_H
