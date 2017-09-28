#ifndef UNDOCOMMAND_H
#define UNDOCOMMAND_H

#include <QtCore/QObject>
#include <QtWidgets/QUndoCommand>

#include "TimelineClip.h"
#include "inc_FSTimelineView/UndoCommandBase.h"
#include "inc_FSCommonlib/FSNLETimeline.h"





class  AddClipCommand : public UndoCommand
{
	Q_OBJECT
public:
	AddClipCommand(TimelineTrackPtr track, ClipPtr clip, int frames);
	~AddClipCommand();
	void Undo() override;
	void Redo() override;
private:
	TimelineTrackPtr m_track;
	ClipPtr m_clip;
	int m_frames;
	int m_preFrames;

};


class MoveClipCommand : public UndoCommand
{
	Q_OBJECT
public:
    MoveClipCommand(TimelineTrackPtr srcTrack, TimelineTrackPtr destTrack, ClipPtr clip, int frames);

	void Undo() override;
	void Redo() override;

protected:
    TimelineTrackPtr m_srcTrack;
    TimelineTrackPtr m_destTrack;
    ClipPtr m_clip;
    int m_frames;
    int m_preFrames;
};

class  RemoveClipCommand : public UndoCommand
{
	Q_OBJECT
public:
	RemoveClipCommand(TimelineTrackPtr track, ClipPtr clip);
	~RemoveClipCommand();
	void Undo() override;
	void Redo() override;
private:
	TimelineTrackPtr m_track;
	ClipPtr m_clip;
};


class SplitClipCommand : public UndoCommand
{
	Q_OBJECT
public:
	SplitClipCommand(TimelineTrackPtr track, ClipPtr clip, int frames);

	void Undo() override;
	void Redo() override;

private:
	TimelineTrackPtr m_track;
	ClipPtr m_clip;
	ClipPtr m_clipAfter;
	int m_frames;
};

class TimelineManager;
class AdjustClipCommand : public UndoCommand
{
	Q_OBJECT
public:
	AdjustClipCommand(TimelineManager *manager, ClipPtr clip, int newStartPos, int newLen);
	
	void Undo() override;
	void Redo() override;

private:
	TimelineManager *m_manager;
	ClipPtr m_clip;
	int m_newStartPos;
	int m_newLen;

	int m_oldStartPos;
	int m_oldLen;
};


class InsertTrackCommand : public UndoCommand
{
	Q_OBJECT
public:
	InsertTrackCommand(TimelineManager *manager, int trackIndex, TimelineTrackPtr track);
	
	void Undo() override;
	void Redo() override;

protected:
	void undo() override;
	void redo() override;
private:
	TimelineManager *m_manager;
	TimelineTrackPtr m_track;
	int m_trackIndex;
};

class RemoveTrackCommand : public UndoCommand
{
	Q_OBJECT
public:
	RemoveTrackCommand(TimelineManager *manager, TimelineTrackPtr track);

	void Undo() override;
	void Redo() override;
protected:
	void undo() override;
	void redo() override;
private:
	TimelineManager *m_manager;
	TimelineTrackPtr m_track;

};

// 轨道重建的信息使用单独的commond
// 可在增加/删除多个轨道后才发出signalTrackRebuild信号
// Undo使用队列信号确保其它command都执行后才发出信号
class TrackRebuildCommand : public UndoCommand
{
	Q_OBJECT
public:
	TrackRebuildCommand(TimelineManager *manager );

	void Undo() override;
	void Redo() override;
protected:
	void undo() override;
	void redo() override;
private:
	TimelineManager *m_manager;

};

class EmitSelectChangeCommand : public UndoCommand
{
	Q_OBJECT
public:
	EmitSelectChangeCommand(TimelineManager *manager);
	void Undo() override;
	void Redo() override;
private:
	TimelineManager *m_manager;
};


class  SelectedClipCommand : public UndoCommand
{
	Q_OBJECT
public:
	SelectedClipCommand(TimelineManager *manager, ClipPtr clip, bool enabled, bool multiSelected);

	void Undo() override;
	void Redo() override;

	void redo() override;
	void undo() override;
private:
	TimelineManager *m_manager;
	ClipPtr m_clip;
	bool m_enabled;
	bool m_multiSelected;
};



class  DetachClipCommand : public UndoCommand
{
	Q_OBJECT
public:
	DetachClipCommand(TimelineManager *manager, ClipPtr videoClip, ClipPtr audioClip);
	void Undo() override;
	void Redo() override;
private:
	TimelineManager *m_manager;
	ClipPtr m_videoClip;
	ClipPtr m_audioClip;
};

class  MuteClipCommand : public UndoCommand
{
	Q_OBJECT
public:
	MuteClipCommand(TimelineManager *manager, ClipPtr clip, bool mute);
	void Undo() override;
	void Redo() override;
private:
	TimelineManager *m_manager;
	ClipPtr m_clip;
	bool m_mute;
};


#include "cMouseStreamCtrl.h"

class  AddMouseFilterCommand : public UndoCommand
{
	Q_OBJECT
public:
	AddMouseFilterCommand(TimelineManager *manager, ClipPtr clip, ClipPtr filter, MouseEvent type, int level);
	void Undo() override;
	void Redo() override;
private:
	TimelineManager *m_manager;
	ClipPtr m_clip;
	ClipPtr m_filter;
	MouseEvent m_type;
	int m_level;
};

class AddMouseSoundCommand : public UndoCommand
{
	Q_OBJECT
public:
	AddMouseSoundCommand(TimelineManager *manager, ClipPtr clip, QString soundPath);
	void Undo() override;
	void Redo() override;
private:
	TimelineManager *m_manager;
	ClipPtr m_clip;
	QString m_soundPath;
};

class HideTrackCommand : public UndoCommand
{
	Q_OBJECT
public:
	HideTrackCommand(TimelineManager *manager, int trackIndex);
	void Undo() override;
	void Redo() override;
private:
	TimelineManager *m_manager;
	int m_trackIndex;
};

class ShowTrackCommand : public UndoCommand
{
	Q_OBJECT
public:
	ShowTrackCommand(TimelineManager *manager, int trackIndex);
	void Undo() override;
	void Redo() override;
private:
	TimelineManager *m_manager;
	int m_trackIndex;
};

class FreezeTrackCommand : public UndoCommand
{
	Q_OBJECT
public:
	FreezeTrackCommand(TimelineManager *manager, int trackIndex);
	void Undo() override;
	void Redo() override;
private:
	TimelineManager *m_manager;
	int m_trackIndex;
};

class UnfreezeTrackCommand : public UndoCommand
{
	Q_OBJECT
public:
	UnfreezeTrackCommand(TimelineManager *manager, int trackIndex);
	void Undo() override;
	void Redo() override;
private:
	TimelineManager *m_manager;
	int m_trackIndex;
};

class FreezeFrameCommond : public UndoCommand
{
	Q_OBJECT
public:
	FreezeFrameCommond(TimelineManager *manager, TimelineTrackPtr newTrack,ClipPtr clip,int frame,int freezeLen);
	void Undo() override;
	void Redo() override;
private:
	TimelineManager *m_manager;
	TimelineTrackPtr m_newTrack;
	ClipPtr m_clip;
	int m_frame;
	int m_freezeLen;

	ClipPtr m_leftClip;
	ClipPtr m_rightClip;
	ClipPtr m_FreezeClip;
};

class ChangeSpeedCommand : public UndoCommand
{
	Q_OBJECT
public:
	ChangeSpeedCommand(TimelineManager *manager, ClipPtr clip, int newStart, int newLen);
	void Undo() override;
	void Redo() override;
private:
	TimelineManager *m_manager;
	ClipPtr m_clip;
	int m_newStart;
	int m_newLen;
	float m_newScale;

	int m_oldStart;
	float m_oldScale;
	bool m_bChanged;
};

class SetSpeedScaleCommand : public UndoCommand
{
	Q_OBJECT
public:
	SetSpeedScaleCommand(TimelineManager *manager, ClipPtr clip, float scale);
	void Undo() override;
	void Redo() override;
private:
	TimelineManager *m_manager;
	ClipPtr m_clip;
	
	float m_oldScale;
	float m_newScale;
	int m_trimLen;
};


class MarkerOperationCommand : public UndoCommand
{
	Q_OBJECT
public:
	MarkerOperationCommand(QObject *parent = nullptr);

private:
	void undo();
	void redo();
};



class AddTLMarkCommond : public MarkerOperationCommand
{
	Q_OBJECT
public:
	AddTLMarkCommond(TimelineManager *manager, int frame);
	void Undo() override;
	void Redo() override;
private:
	TimelineManager *m_manager;
	int m_frame;

	MarkPointPtr m_addedMark;
};


class RemoveTLMarkCommond : public UndoCommand
{
	Q_OBJECT
public:
	RemoveTLMarkCommond(TimelineManager *manager, MarkPointPtr markPoint);
	void Undo() override;
	void Redo() override;
private:
	TimelineManager *m_manager;
	MarkPointPtr m_markPoint;
};

class MoveTLMarkCommond : public UndoCommand
{
	Q_OBJECT
public:
	MoveTLMarkCommond(TimelineManager *manager, MarkPointPtr markPoint,int pos);
	void Undo() override;
	void Redo() override;
private:
	TimelineManager *m_manager;
	MarkPointPtr m_markPoint;
	int m_newPos;
	int m_oldPos;
};

class AddClipMarkerCommond : public MarkerOperationCommand
{
	Q_OBJECT
public:
	AddClipMarkerCommond(TimelineManager *manager, FSNLETimeline *tl,int frame);
	void Undo() override;
	void Redo() override;
private:
	TimelineManager *m_manager;
	FSNLETimeline *m_pTimeline;
	int m_pos;
};

class RemoveClipMarkerCommond : public UndoCommand
{
	Q_OBJECT
public:
	RemoveClipMarkerCommond(TimelineManager *manager, FSNLETimeline *tl, int frame);
	void Undo() override;
	void Redo() override;
private:
	TimelineManager *m_manager;
	FSNLETimeline *m_pTimeline;
	int m_pos;
};




#endif // UNDOCOMMAND_H
