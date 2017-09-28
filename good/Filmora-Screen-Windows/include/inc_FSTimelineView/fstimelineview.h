#ifndef FSTIMELINEVIEW_H
#define FSTIMELINEVIEW_H

#include <QtWidgets/QWidget>
#include <QtCore/QList>

#include "fstimelineview_global.h"
class TimelineViewPrivate;
class FSNLETimeline;
class IPlayerEditorManager;
class ITimelineViewManager;
class ITitleBarManager;
class FSTIMELINEVIEW_EXPORT FSTimelineView : public QWidget
{
public:
	FSTimelineView(QWidget *parent);
	~FSTimelineView();
	IPlayerEditorManager* GetPlayerEditManager();
	ITimelineViewManager* GetTimelineViewManager();
	ITitleBarManager* GetTitleBarManager();
	void TransFromGaInfo();
private:
	TimelineViewPrivate *m_p;
};

class INLETimelineBase;
class FSTIMELINEVIEW_EXPORT ITimelineViewManager : public QObject
{
	Q_OBJECT
public:
	virtual void SetCurrentFrame(int frame) = 0;
	virtual int CurrentFrame() = 0;
	virtual FSNLETimeline* GetRootTimeline() = 0;
	virtual void FetchAudioWaveData(FSNLETimeline *audioTimeline) = 0;
	virtual void UpdateView() = 0;

	virtual QString GetProjectString() = 0;
	virtual void LoadProjectString(const QString projString) = 0;
	virtual void Clear() = 0;
	virtual void EnableShortcuts(bool bEnable) = 0;
	virtual void SetSpeedScale(FSNLETimeline* timeline,float scale) = 0;
	virtual void DenoiseLevelChanged(FSNLETimeline *timeline) = 0;
	virtual void SetMouseEffectDraging(bool bDraging) = 0;
Q_SIGNALS:
	void sigStartSeek();
	void sigEndSeek();
	void sigSeek(int frame);
	void sigDurationChanged(int d);
	void sigUnselectMatterView();
	void sigContentChanged(bool bChanged);
	void sigClear();
	void sigFocusIn();
};

class UndoCommand;
class FSTIMELINEVIEW_EXPORT IPlayerEditorManager : public QObject
{
	Q_OBJECT
public:
	virtual void AddTimeline(FSNLETimeline *timeline) = 0;
	virtual void AddTimelines(QList<FSNLETimeline*> timelines) = 0;
	virtual void RemoveSelected() = 0;
	virtual void AddMouseFilter(FSNLETimeline *timeline, FSNLETimeline *filter) = 0;
	virtual void AddMouseSound(FSNLETimeline *timeline, QString soundPath) = 0;

	virtual QList<FSNLETimeline*> GetTimelines() = 0;
	virtual QList<FSNLETimeline*> GetSelected() = 0;
	virtual QList<FSNLETimeline*> GetOtherSelected() = 0;
	virtual void SetSelected(QList<FSNLETimeline*> timelines) = 0;
	virtual void UnselectAll() = 0;

	virtual bool HasTimelines() = 0;
	virtual void SetDisplayName(QString ancestralGuid, QString displayName) = 0;
	virtual void SetShiftSelectBaseTimeline(FSNLETimeline* timeline) = 0;

	virtual void Undo() = 0;
	virtual void Redo() = 0;
    virtual void PushCommand(UndoCommand *pCommond) = 0;
	virtual void BeginComposite() = 0;
	virtual void EndComposite() = 0;

Q_SIGNALS:
	void signalTimelineUpdate();
	void signalSelectionChanged();
	void sigMouseEffectAdded(FSNLETimeline *timeline, INLETimelineBase *filter);
	void sigMouseEffectRemove(FSNLETimeline *timeline, INLETimelineBase *filter);
	void sigTimelineAdded(FSNLETimeline *timeline);
	void sigTimelineRemoved(FSNLETimeline *timeline);
	void sigPause();
	void sigMute(FSNLETimeline* timeline, bool mute);
};

class FSTIMELINEVIEW_EXPORT  ITitleBarManager: public QObject
{
	Q_OBJECT
public:
	virtual bool CanUndo() = 0;
	virtual void Undo() = 0;
	virtual bool CanRedo() = 0;
	virtual void Redo() = 0;
	virtual bool CanCut() = 0;
	virtual void Cut() = 0;
	virtual bool CanCopy() = 0;
	virtual void Copy() = 0;
	virtual bool CanPaste() = 0;
	virtual void Paste() = 0;
	virtual bool CanDelete() = 0;
	virtual void Delete() = 0;
};



#endif // FSTIMELINEVIEW_H
