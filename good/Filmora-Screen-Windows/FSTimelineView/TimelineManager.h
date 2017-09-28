#ifndef TIMELINECONTROL_H
#define TIMELINECONTROL_H

#include <QtCore/QObject>
#include <memory>

#include "cMouseStreamCtrl.h"

#include "inc_FSTimelineView/fstimelineview.h"
#include "inc_FSCommonlib/FSNLETimeline.h"

#include "TimelineClip.h"
#include "UndoCommand.h"

class TimelineTrack;
class FSNLETimeline;
class UndoCommand;
class PlayerEditorManager;
class TitleBarManager;
struct ClipColor;
class MarkPointMgr;
class ClipMarkerMgr;
/*
	时间线控制器
*/
class TimelineManager : public QObject
{
	Q_OBJECT
public:
	static TimelineManager* GetInstance();
	void Reset();
	void Load(INLETimelineBase *rootTimeline);
	// 增加片段
	Clip* AddClip(int trackIndex, int frames, FSNLETimeline *timeline,bool bEmitSelectChange = true);

	// 移动片段
	void MoveClip(int trackIndex, int frames, Clip* clip);

	// 删除选择片段
	void RemoveClips();
	bool CanRemove();

	// trim片段 leftSide:trim片段的左边true, frames:当前trim中在时间线上的帧
	void AdjustClip(bool leftSide, int frames);

	//获取可以trim的范围，leftSide:trim片段的左边true, 返回值x表示向左拖动的最大长度，负值，y表示向右拖动的最大长度，正值
	QPoint GetTrimRange(bool bLeftSide);

	//trim片段,startPos，length新的开始位置和长度
	void Trim(Clip* clip, int startPos, int length);


	// split当前选择的片段
	void Split();
	bool CanSplit();

	// 拷贝当前选择片段
	void Copy();
	bool CanCopy();

	// 粘贴当前选择片段
	void Paste();
	bool CanPaste();

	// 剪切当前选择片段
	void Cut();
	bool CanCut();
	
	// 获取选择所有片段
	Clips GetSelected();
	// 获取位置frames中选择的片段
	Clips GetSelected(int frames);
	// 获取轨道trackIndex位置frames选择的片段
	Clip* GetSelected(int trackIndex, int frames);

	//获取为选中的clip
	Clips GetUnselected();

	void SingleSelect(int nTrackIndex,int nFrame);

	void BeginRectangleSelect();
	void RectangleSelect(int nTrackFrom, int nTrackTo, int nFrameFrom, int nFrameTo);
	void EndRectangleSelect();

	void SetShiftSelectBaseClip(Clip *clip);
	void ShiftSelect(int nTrackIndex, int nFrame);
	void CtrlSelect(int nTrackIndex, int nFrame); 
	void SelectTrack(int nTrackIndex);
	//提供给播放器编辑区使用
	void MultiSelect(QList<FSNLETimeline*> timelines);
	// 清空所有选择
	void UnselectAll();
	// 选择所有片段
	void SelectedAll();
	//选择所有可编辑片段
	void SelectAllEditableClips();


	// 获取所有轨道
	TimelineTracks GetTracks();
	TimelineTrack* GetTrack(int index);
	int GetTrackCount();

	Clips GetClips();
	Clips GetEditableClips();
	Clips GetClipsWithout(Clip *pClip);

	// 插入轨道
	void InsertTrack(int trackIndex);
	// 移除轨道
	void RemoveTrack(int index);
	// 移除所有空轨道
	void RemoveAllEmptyTrack();
	// 移除最后面的空轨道
	void RemoveTailEmptyTrack();
	// 是否有空轨道
	bool HasEmptyTrack();

	// 获取时间线总时长
	int GetDuration();
	// 设置当前帧
	void SetCurrentFrames(int frames);
	int GetCurrentFrames();

	// Undo/Redo操作
	bool CanUndo();
	bool CanRedo();
	void Undo();
	void Redo();
	void BeginComposite();
	void EndComposite();
	void SetUndoLimit(int limit);

	// Mute所有选择片段, 只Mute符合条件片段
	void Mute(bool enabled);
	void CanMute(Clip *clip, bool &canMute, bool &hadMute);

	// Detach所有选择片段, 只detach符合条件片段
	void Detach();
	bool CanDetack(Clip *clip);

	// 给鼠标流增加Filter filter:鼠标filter, type:鼠标状态类型, level:此filter的排序
	void AddMouseFilter(Clip *clip, FSNLETimeline *filter, MouseEvent type = MouseEvent::MouseEvent_None, int level = 0);
	bool CanAddMouseFilter(Clip *clip);

	//设置鼠标声音
	void AddMouseSound(Clip *clip,QString soundPath);

	
	// 获取时间线根NLETimeline
	FSNLETimeline* GetRootTimeline();

	QList<TimelineTrack*> GetHidenTracks();
	QList<TimelineTrack*> GetFreezenTracks();

	// 获取播放器编辑器控制器
	PlayerEditorManager* GetPlayerEditManager();
	TitleBarManager* GetTitleBarManager();

	void SetDisplayName(QString ancestralGuid, QString displayName);

	
	void HideTrack(int trackIndex);
	void ShowTrack(int trackIndex);
	void FreezeTrack(int trackIndex);
	void UnfreezeTrack(int trackIndex);
	void SaveHidenFreezeTrack2RootTimeline();
	void RestoreHidenFreezeTrackFromRootTimeine(FSNLETimeline *srcRootTimeline);


	bool CanFreezeFrame(Clip *clip);
	void FreezeFrame(Clip* clip,int len = 2 *25); //len : 定帧的长度

	QPoint GetDragRange(Clip *clip,bool bLeftDrag);
	void ChangeSpeed(Clip *clip,int newStart,int newLen);

	void SetSpeedScale(FSNLETimeline* timeline, float scale);


	void SelectColorGroup(Clip *clip);
	QColor GetClipDefaultColor(Clip *clip);
	QColor GetClipColor(QString name);
	QList<ClipColor> GetClipColorList();

	Clip* GetClipWithTimline(FSNLETimeline* timeline);
	MarkPointMgr *GetMarkPointMgr();
	ClipMarkerMgr *GetClipMarkerMgr();

Q_SIGNALS:
	// 轨道更新信号
	void signalTrackUpdate();
	// 轨道重建信号
	void signalTrackRebuild();
	// 片段选择改变信号
	void signalSelectionChanged();

	void sigPause();
	void sigDoCommand();

	void sigPasted(FSNLETimeline *timeline);


public:
	TimelineManager(QObject *parent);
	~TimelineManager();

	void _AdjustClip(ClipPtr clip, int startPos, int len);
	void _InsertTrack(int trackIndex, TimelineTrackPtr track);
	void _RemoveTrack(TimelineTrackPtr track);
	void _Mute(ClipPtr clip, bool mute);
	void _Detach(ClipPtr videoClip, ClipPtr audioClip);
	void _UnDetach(ClipPtr videoClip, ClipPtr audioClip);
	void _RemoveMouseFilter(ClipPtr clip, ClipPtr filter);
	void _AddMouseFilter(ClipPtr clip, ClipPtr filter, MouseEvent type = MouseEvent::MouseEvent_None, int level = 0);
	void _AddMouseSound(ClipPtr clip, QString soundPath);
	void _RemoveMouseSound(ClipPtr clip);
	void _ResetAllClipLevel();

	void _HideTrack(int trackIndex);
	void _ShowTrack(int trackIndex);
	void _FreezeTrack(int trackIndex);
	void _UnfreezeTrack(int trackIndex);
	float _ChangeSpeed(ClipPtr clip, int newStart, int newLen);
	void _ChangeSpeedStandar(ClipPtr clip, int start, float scale);
	int _SetSpeedScale(ClipPtr clip, float scale);
	void _UnsetSpeedScale(ClipPtr clip, float scale, int trimlen);
private:
	ClipPtr CreateClipFromTimeline(FSNLETimeline *timeline);
	ClipPtr GetClipPtrFromClip(Clip *clip);
	ClipPtr GetClipPtrWithTimeline(FSNLETimeline *timeline);
	void PushCommand(UndoCommand *pCommond);
	void PushCommand(QUndoCommand *pCommond);
	void RemoveClip(Clip *clip);
	Clips GetClips(int frames);
	Clips GetEditableClips(int frames);
	void InitClipBkColor();

	void ManumalEmitSelectedChanged();
private:
	FSNLETimeline *m_pRootTimeline;
	TimelineTracksPtr m_tracks;
	
	ClipsPtr m_pCopyedClips;
	QUndoStack *m_UndoStack;

	int m_curFrames;
	bool m_canPaste;

	PlayerEditorManager *m_playerEditManager;
	TitleBarManager *m_pTitleBarManager;

	Clip *m_pShiftSelectBaseClip;
	Clips m_lstRectangleSelectsTmpClips;

	QMap<TimelineType, QColor> m_defaultBkColor;
	QList<ClipColor> m_allBkColors;

	MarkPointMgr* m_pMarkPointMgr;
	ClipMarkerMgr *m_pClipMarkerMgr;
private:
	friend class PlayerEditorManager;
	friend class MarkPointMgr;
	friend class ClipMarkerMgr;
};

struct ClipColor
{
	QString name;
	QString tranme;
	QColor clr;
	QPixmap icon;
	ClipColor(QColor c, QString n, QString tr,QString iconPath) :name(n), tranme(tr), clr(c),icon(iconPath){}
};


//////////////////////////////////////////////////////////////////////////

/*
	播放器编辑器控制器
*/
class PlayerEditorManager : public IPlayerEditorManager
{
	Q_OBJECT
public:
	PlayerEditorManager(TimelineManager *manager, QObject *parent);
	
	void AddTimeline(FSNLETimeline *timeline) override;
	void AddTimelines(QList<FSNLETimeline*> timelines) override;
	void RemoveSelected() override;
	void AddMouseFilter(FSNLETimeline *tl, FSNLETimeline *filter) override;
	void AddMouseSound(FSNLETimeline *tl, QString soundPath) override;

	QList<FSNLETimeline*> GetTimelines() override;
	QList<FSNLETimeline*> GetSelected() override;
	QList<FSNLETimeline*> GetOtherSelected() override;
	void SetSelected(QList<FSNLETimeline*> timelines) override;
	void UnselectAll() override;
	bool HasTimelines() override;
	void SetDisplayName(QString ancestralGuid, QString displayName) override;
	void SetShiftSelectBaseTimeline(FSNLETimeline* timeline) override;

	void Undo() override;
	void Redo() override;
    void PushCommand(UndoCommand *pCommond) override;
	void BeginComposite() override;
	void EndComposite() override;



private:
	TimelineManager *m_manager;
	Clips m_selectedClips;
};


//////////////////////////////////////////////////////////////////////////

/*
提供给菜单栏使用
*/
class TitleBarManager : public ITitleBarManager
{
	Q_OBJECT
public:
	TitleBarManager(TimelineManager *manager, QObject *parent);

	bool CanUndo() override;
	void Undo() override;

	bool CanRedo() override;
	void Redo() override;

	bool CanCut() override;
	void Cut() override;

	bool CanCopy() override;
	void Copy() override;

	bool CanPaste() override;
	void Paste() override;

	bool CanDelete() override;
	void Delete() override;

private:
	TimelineManager *m_pTimelineMgr;
};


//////////////////////////////////////////////////////////////////////////
class MarkPointMgr : public QObject
{
	Q_OBJECT
public:
	MarkPointMgr(TimelineManager *manager, QObject *parent);
	void InitMarkPoints(QList<MarkPointPtr> mps);
	QList<MarkPointPtr> GetMarkPoints();
	void Add(int frame);
	void Remove(MarkPoint* mp);
	void RemoveAll();
	void Move(MarkPoint *mp, int newframe);
	void SetColor(MarkPoint* mp, QColor clr);
	void SetSelected(MarkPoint *mp, bool bSelected);
	void UnselectAll();

	MarkPoint* GetSelected();

	void Clear();
Q_SIGNALS:
	void sigUpdate();

private:
	MarkPointPtr Pt2Ptr(MarkPoint *mp);

	MarkPointPtr _Add(int frame);
	void _Add(MarkPointPtr mp);
	void _Remove(MarkPointPtr mp);
	void _Move(MarkPointPtr mp, int pos);
private:
	TimelineManager *m_manager;
	QList<MarkPointPtr> m_markPoints;

	friend class AddTLMarkCommond;
	friend class RemoveTLMarkCommond;
	friend class MoveTLMarkCommond;
};

class ClipMarkerMgr : public QObject
{
	Q_OBJECT
public:
	ClipMarkerMgr(TimelineManager *manager, QObject *parent);
	void Add(FSNLETimeline*tl, int frame);
	void Remove(FSNLETimeline*tl,int frame);
	void RemoveAll(FSNLETimeline*tl);
	void SetSelected(FSNLETimeline*tl, int frame,bool bSelected);
	void UnselectAll(FSNLETimeline*tl);
	void UnselectAll();

	bool CanAddMarker(FSNLETimeline* tl,int frame);
private:
	TimelineManager *m_manager;
	
	friend class AddClipMarkerCommond;
	friend class RemoveClipMarkerCommond;
};

#endif // TIMELINECONTROL_H
