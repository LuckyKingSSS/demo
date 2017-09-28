#ifndef TVCONTROL_H
#define TVCONTROL_H

#include <QObject>
#include "TimelineManager.h"
#include "inc_CommonLib/TimelineRuler.h"
class TrackWidget;
typedef QList<TrackWidget*> TrackWidgets;
class DragClipBase;
typedef QList<DragClipBase> DragClipList;
class DragClipBase
{
public:
	int trackIndex;
	int startPos;
	int length;
	FSNLETimeline *timeline;
	Clip *clip;
	DragClipBase() :trackIndex(-1), startPos(0), length(0), timeline(nullptr), clip(nullptr){}
	bool IsNull(){ return !clip && !timeline; }
	DragClipBase& DragClipBase::operator=(const DragClipBase &other)
	{
		if (&other != this)
		{
			trackIndex = other.trackIndex;
			startPos = other.startPos;
			length = other.length;
			clip = other.clip;
			timeline = other.timeline;
		}
		return *this;
	}
};

class UIDragClips
{
public:
	enum{ DragType_None, DragType_MatterView, DragType_Timeline, };
public:
	UIDragClips();
	void SetDragType(int nType);
	int GetDragType();
	void SetClipList(DragClipList lst);
	DragClipList GetClipsList();
	bool IsNull();

	int GetTopestIndex();
	int GetLowestIndex();
	int GetTrackCnt();
	void Clear();
	UIDragClips& operator=(const UIDragClips &other);
private:
	DragClipList m_lstDagClips;
	int m_nDragType;
	QPoint m_ptStartDrag;
};

class TVTrackView;
class VirtualTrackAssistor;
class TVDragControl : public QObject
{
	Q_OBJECT

public:
	static TVDragControl* GetInstance();
	void Init(TVTrackView *pTrackView);

	void DragEnter(QDragEnterEvent *e);
	void DragMove(QPoint curPoint);
	void DragLeave(QDragLeaveEvent *e);
	void DropDown();

	void SetDraging(bool bDraging);
	bool IsDraging();
	void SetDraginTrackIndex(int index);
	int GetDraginTrackIndex();

	UIDragClips GetDraginUIClips();
	DragClipList GetDragingClips(int trackIndex);
	void SetViewportRect(QRect rcViewport);
	QRect GetViewportRect();

	void SetDragEnterPoint(QPoint pt);
	QList<int> GetAdsorbList();
protected:
	TVDragControl(QObject *parent = 0);
	TimelineRuler* Ruler();
	

	bool IsCollision(DragClipList lst,bool bReject);  //bReject 是否排除lst本身
	DragClipList MatterViewList2DragList(QList<FSNLETimeline*> lst, int startPos, int startTrack);
	DragClipList Selected2DragList();
	bool DoDragMove(int frameOffset,int trackOffset); //返回值：true 没有碰撞到  false 发生碰撞
	int GetMostLeftPos(DragClipList lst);
	bool FindGoodPos(DragClipList preGoodList, DragClipList &notGoodList);
	DragClipBase GetHitClip(Clip *clip);
	void DoAdsorbtion(DragClipList &lst);

	void StartFilter();
	void StopFilter();
	void DropClear();
	bool eventFilter(QObject *watched, QEvent *event) override;

Q_SIGNALS:
	void sigCompositeFinish();
private:
	TVTrackView *m_pTrackView;
	TrackWidget *m_pTrackWidget;
	int m_nDraginTrackIndex;


	bool m_bDraging;
	UIDragClips m_dragInClips;
	UIDragClips m_dragInClipsCopy;		  //最原始拖进来时候的状态
	UIDragClips m_preDropableDragInClips; //上一个可放下的状态
	TimelineManager *m_pTimelineMgr;
	VirtualTrackAssistor *m_pTrackAssitor;

	int m_dragTrackIndex;
	QPoint m_dragEnterPoint;
	int m_nDragEnterFrame;
	QRect m_rcViewPortGlobal;

	DragClipBase m_hitClip;
	QList<int> m_lstAdsorbPoints;
};

class VirtualTrackAssistor : public QObject
{
	Q_OBJECT
public:
	VirtualTrackAssistor(QObject *parent = 0);
	void CreateTrack(int nIndex);
	void CreateTracks(int nCnt);
	int GetCreatedCnt();
	void Clear();
private:
	QList<int> m_lstCreatedTrack;
};

class TVTrimControl : public QObject
{
	Q_OBJECT
public:
	static TVTrimControl* GetInstance();
	bool StartTrim(QPoint point, TrackWidget *item);
	void MoveTrim(QPoint point, TrackWidget *item);
	void EndTrim(QPoint point);

	bool IsTriming();
	DragClipList GetTrimingList();
	DragClipList GetTrimingList(int trackIndex);

	QList<int> GetAdsorbList();
Q_SIGNALS:
	void sigBeginTrim();
	void sigTriming(QPoint pt,int startPos,int lenght);
	void sigEndTrim();
protected:
	TVTrimControl(QObject *parent = 0);
	TimelineRuler* Ruler();
	void HintTriming(QPoint point, TrackWidget *item, int hintMargins = 3);
	void CalcTrimDuration();
	
	void DoAdsorbtion(int &newoffset);
	void EmitTrimSignal(QPoint pt);

private:
	TrackWidget *m_pTrackWidget;
	bool m_bTriming;
	bool m_bLeftDrag;
	bool m_bRightDrag;
	Clip *m_pHitClip;
	QPoint m_ptTrimRange;  //x为负值，代表向左拖动的最大长度，y有正值，代表向右拖动的最大长度
	QPoint m_ptPress;

	DragClipList m_lstTriming;
	DragClipList m_lstTrimingCopy;

	QList<int> m_lstAdsorbPoints;
};

class TrackBuilder : public QObject
{
	Q_OBJECT
public:
	static TrackBuilder* GetInstance();
	void SetTrackView(TVTrackView *pView);
	void SetRuler(TimelineRuler *pRuler);
	TimelineRuler *GetRuler();
	void Rebuild();
	void UpdateItems(bool bPartUpdate = false);

	TrackWidgets GetTrackWidgets();	
	TrackWidget* GetTrackWidget(int nIndex);
	TVTrackView* GetTrackView();
	int Duration();
	int Height();

Q_SIGNALS:
	void Update();
private:
	TrackBuilder(QObject *parent = 0);
	TrackWidget* CreateTrack(TimelineTrack* trackData);

private:
	TVTrackView *m_pTrackView;
	TimelineRuler *m_pRuler;
	TimelineManager *m_pTimelineMgr;
	TrackWidgets m_trackWidgets;

};

class MouseEffectDragClip
{
public:
	enum 
	{
		MouseEffectDragClip_None = 0,
		MouseEffectDragClip_Sound,
		MouseEffectDragClip_Filter,
	};
	int dragtype;
	QString soundfile;
	FSNLETimeline *filterTimeline;

	MouseEffectDragClip();
	void Clear();
};

class TVMouseEffectDragCtrl : public QObject
{
	Q_OBJECT
public:
	enum{type1 = 0,type2,type3};

	static TVMouseEffectDragCtrl* GetInstance();
	void Init(TVTrackView *pTrackView);
	bool IsTipAcceptable();

	void DragEnter(QDragEnterEvent *e);
	void DragMove(QDragMoveEvent *e);
	void DragLeave(QDragLeaveEvent *e);
	void DropDown(QDropEvent *e);

	Clip* GetTargetClip();
	void SetDragTip(bool bDraging);
protected:
	TVMouseEffectDragCtrl(QObject *parent = 0);
	Clip* _Acceptable(QPoint pos);

private:
	bool m_bTipAcceptable;
	TVTrackView *m_pTrackView;
	MouseEffectDragClip m_DragClip;

	Clip* m_pTargetClip;
};

class ChangeSpeedControl : public QObject
{
	Q_OBJECT
public:
	static ChangeSpeedControl* GetInstance();

	void StartChange(QPoint pt,TrackWidget *child);
	void MoveChange(QPoint pt, TrackWidget *child);
	void EndChange(QPoint pt);
	bool IsChanging();
	void HitBorder(QPoint point, TrackWidget *item, int hintMargins = 3);

	DragClipBase GetDragClip(int nTrackIndex);
	int GetAdsorbPos();

Q_SIGNALS:
	void sigChanging(QPoint pt,int duration,int scale);
	void sigEndChange();
protected:
	ChangeSpeedControl(QObject *parent = 0);
	TimelineRuler* Ruler();
	void DoAdsorbtion(int &newoffset);
	void EmitChangingSignal(QPoint pt);
private:
	DragClipBase m_DragClip;
	DragClipBase m_DragClipCopy;
	bool m_bChanging;

	bool m_bLeftDrag;
	bool m_bRightDrag;
	Clip *m_pHitClip;
	QPoint m_ptPress;
	TrackWidget* m_pTrackWidget;
	QPoint m_DragRange;

	QCursor m_RespeedCursor;
	int m_nAdsorbPos;
};

#endif // TVCONTROL_H
