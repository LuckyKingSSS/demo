#ifndef UNDOCOMMAND_H
#define UNDOCOMMAND_H

#include <QtCore/QObject>
//#include <QtWidgets/QUndoCommand>
//

#include "inc_FProject/FUndoRedo.h"

#include "inc_FProject/FTrack.h"



class  UpdateCommand : public FUndoCommand
{
    Q_OBJECT
public:
    UpdateCommand();

    void Undo() override;
    void Redo() override;
    
Q_SIGNALS:
    void sigTrackUpdate();

private:

};


class  AddTimelineCommand : public FUndoCommand
{
	Q_OBJECT
public:
	AddTimelineCommand(FTrackPtr track, FNLETimelinePtr clip, int frames);
	~AddTimelineCommand();
	void Undo() override;
	void Redo() override;
private:
	FTrackPtr m_track;
	FNLETimelinePtr m_clip;
	int m_frames;
	int m_preFrames;

};


class MoveTimelineCommand : public FUndoCommand
{
	Q_OBJECT
public:
    MoveTimelineCommand(FTrackPtr srcTrack, FTrackPtr destTrack, FNLETimelinePtr clip, int frames);

	void Undo() override;
	void Redo() override;

protected:
    FTrackPtr m_srcTrack;
    FTrackPtr m_destTrack;
    FNLETimelinePtr m_clip;
    int m_frames;
    int m_preFrames;
};

class  RemoveTimelineCommand : public FUndoCommand
{
	Q_OBJECT
public:
	RemoveTimelineCommand(FTrackPtr track, FNLETimelinePtr clip);
	~RemoveTimelineCommand();
	void Undo() override;
	void Redo() override;
private:
	FTrackPtr m_track;
	FNLETimelinePtr m_clip;
};



class SplitClipCommand : public FUndoCommand
{
	Q_OBJECT
public:
	SplitClipCommand(FTrackPtr track, FNLETimelinePtr clip, int frames);

	void Undo() override;
	void Redo() override;

private:
	FTrackPtr m_track;
	FNLETimelinePtr m_clip;
	FNLETimelinePtr m_clipAfter;
	int m_frames;
};

//class FSequence;
//class AdjustClipCommand : public FUndoCommand
//{
//	Q_OBJECT
//public:
//	AdjustClipCommand(FSequence *manager, FNLETimelinePtr clip, int newStartPos, int newLen);
//	
//	void Undo() override;
//	void Redo() override;
//
//private:
//	FSequence *m_sequence;
//	FNLETimelinePtr m_clip;
//	int m_newStartPos;
//	int m_newLen;
//
//	int m_oldStartPos;
//	int m_oldLen;
//};
//
//
class InsertTrackCommand : public FUndoCommand
{
	Q_OBJECT
public:
	InsertTrackCommand(FSequence *sequence, int trackIndex, FTrackPtr track);
	
	void Undo() override;
	void Redo() override;

private:
    FSequence *m_sequence;
	FTrackPtr m_track;
	int m_trackIndex;
};

class RemoveTrackCommand : public FUndoCommand
{
	Q_OBJECT
public:
	RemoveTrackCommand(FSequence *manager, FTrackPtr track);

	void Undo() override;
	void Redo() override;

private:
	FSequence *m_sequence;
	FTrackPtr m_track;
    int m_preIndex;
};
//
// 轨道重建的信息使用单独的commond
// 可在增加/删除多个轨道后才发出signalTrackRebuild信号
// Undo使用队列信号确保其它command都执行后才发出信号
class RebuildTrackCommand : public FUndoCommand
{
	Q_OBJECT
public:
	RebuildTrackCommand(FSequence *sequence );

	void Undo() override;
	void Redo() override;

Q_SIGNALS:
    void sigTrackRebuild();

private:
	FSequence *m_sequence;

};

class SelectChangeCommand : public FUndoCommand
{
    Q_OBJECT
public:
    SelectChangeCommand(FSequence *manager);
    void Undo() override;
    void Redo() override;
private:
    FSequence *m_sequence;
};
//
//
//class  SelectedClipCommand : public FUndoCommand
//{
//	Q_OBJECT
//public:
//	SelectedClipCommand(FSequence *manager, FNLETimelinePtr clip, bool enabled, bool multiSelected);
//
//	void Undo() override;
//	void Redo() override;
//
//	void redo() override;
//	void undo() override;
//private:
//	FSequence *m_sequence;
//	FNLETimelinePtr m_clip;
//	bool m_enabled;
//	bool m_multiSelected;
//};
//
//
//
//class  DetachClipCommand : public FUndoCommand
//{
//	Q_OBJECT
//public:
//	DetachClipCommand(FSequence *manager, FNLETimelinePtr videoClip, FNLETimelinePtr audioClip);
//	void Undo() override;
//	void Redo() override;
//private:
//	FSequence *m_sequence;
//	FNLETimelinePtr m_videoClip;
//	FNLETimelinePtr m_audioClip;
//};
//
//class  MuteClipCommand : public FUndoCommand
//{
//	Q_OBJECT
//public:
//	MuteClipCommand(FSequence *manager, FNLETimelinePtr clip, bool mute);
//	void Undo() override;
//	void Redo() override;
//private:
//	FSequence *m_sequence;
//	FNLETimelinePtr m_clip;
//	bool m_mute;
//};
//
//
//#include "cMouseStreamCtrl.h"
//
//class  AddMouseFilterCommand : public FUndoCommand
//{
//	Q_OBJECT
//public:
//	AddMouseFilterCommand(FSequence *manager, FNLETimelinePtr clip, FNLETimelinePtr filter, MouseEvent type, int level);
//	void Undo() override;
//	void Redo() override;
//private:
//	FSequence *m_sequence;
//	FNLETimelinePtr m_clip;
//	FNLETimelinePtr m_filter;
//	MouseEvent m_type;
//	int m_level;
//};
//
//class AddMouseSoundCommand : public FUndoCommand
//{
//	Q_OBJECT
//public:
//	AddMouseSoundCommand(FSequence *manager, FNLETimelinePtr clip, QString soundPath);
//	void Undo() override;
//	void Redo() override;
//private:
//	FSequence *m_sequence;
//	FNLETimelinePtr m_clip;
//	QString m_soundPath;
//};
//
//class HideTrackCommand : public FUndoCommand
//{
//	Q_OBJECT
//public:
//	HideTrackCommand(FSequence *manager, int trackIndex);
//	void Undo() override;
//	void Redo() override;
//private:
//	FSequence *m_sequence;
//	int m_trackIndex;
//};
//
//class ShowTrackCommand : public FUndoCommand
//{
//	Q_OBJECT
//public:
//	ShowTrackCommand(FSequence *manager, int trackIndex);
//	void Undo() override;
//	void Redo() override;
//private:
//	FSequence *m_sequence;
//	int m_trackIndex;
//};
//
//class FreezeTrackCommand : public FUndoCommand
//{
//	Q_OBJECT
//public:
//	FreezeTrackCommand(FSequence *manager, int trackIndex);
//	void Undo() override;
//	void Redo() override;
//private:
//	FSequence *m_sequence;
//	int m_trackIndex;
//};
//
//class UnfreezeTrackCommand : public FUndoCommand
//{
//	Q_OBJECT
//public:
//	UnfreezeTrackCommand(FSequence *manager, int trackIndex);
//	void Undo() override;
//	void Redo() override;
//private:
//	FSequence *m_sequence;
//	int m_trackIndex;
//};
//
//class FreezeFrameCommond : public FUndoCommand
//{
//	Q_OBJECT
//public:
//	FreezeFrameCommond(FSequence *manager, FTrackPtr newTrack,FNLETimelinePtr clip,int frame,int freezeLen);
//	void Undo() override;
//	void Redo() override;
//private:
//	FSequence *m_sequence;
//	FTrackPtr m_newTrack;
//	FNLETimelinePtr m_clip;
//	int m_frame;
//	int m_freezeLen;
//
//	FNLETimelinePtr m_leftClip;
//	FNLETimelinePtr m_rightClip;
//	FNLETimelinePtr m_FreezeClip;
//};
//
//class ChangeSpeedCommand : public FUndoCommand
//{
//	Q_OBJECT
//public:
//	ChangeSpeedCommand(FSequence *manager, FNLETimelinePtr clip, int newStart, int newLen);
//	void Undo() override;
//	void Redo() override;
//private:
//	FSequence *m_sequence;
//	FNLETimelinePtr m_clip;
//	int m_newStart;
//	int m_newLen;
//	float m_newScale;
//
//	int m_oldStart;
//	float m_oldScale;
//	bool m_bChanged;
//};
//
//class SetSpeedScaleCommand : public FUndoCommand
//{
//	Q_OBJECT
//public:
//	SetSpeedScaleCommand(FSequence *manager, FNLETimelinePtr clip, float scale);
//	void Undo() override;
//	void Redo() override;
//private:
//	FSequence *m_sequence;
//	FNLETimelinePtr m_clip;
//	
//	float m_oldScale;
//	float m_newScale;
//	int m_trimLen;
//};
//
//
//class MarkerOperationCommand : public FUndoCommand
//{
//	Q_OBJECT
//public:
//	MarkerOperationCommand(QObject *parent = nullptr);
//
//private:
//	void undo();
//	void redo();
//};
//
//
//
//class AddTLMarkCommond : public MarkerOperationCommand
//{
//	Q_OBJECT
//public:
//	AddTLMarkCommond(FSequence *manager, int frame);
//	void Undo() override;
//	void Redo() override;
//private:
//	FSequence *m_sequence;
//	int m_frame;
//
//	MarkPointPtr m_addedMark;
//};
//
//
//class RemoveTLMarkCommond : public FUndoCommand
//{
//	Q_OBJECT
//public:
//	RemoveTLMarkCommond(FSequence *manager, MarkPointPtr markPoint);
//	void Undo() override;
//	void Redo() override;
//private:
//	FSequence *m_sequence;
//	MarkPointPtr m_markPoint;
//};
//
//class MoveTLMarkCommond : public FUndoCommand
//{
//	Q_OBJECT
//public:
//	MoveTLMarkCommond(FSequence *manager, MarkPointPtr markPoint,int pos);
//	void Undo() override;
//	void Redo() override;
//private:
//	FSequence *m_sequence;
//	MarkPointPtr m_markPoint;
//	int m_newPos;
//	int m_oldPos;
//};
//
//class AddClipMarkerCommond : public MarkerOperationCommand
//{
//	Q_OBJECT
//public:
//	AddClipMarkerCommond(FSequence *manager, FSNLETimeline *tl,int frame);
//	void Undo() override;
//	void Redo() override;
//private:
//	FSequence *m_sequence;
//	FSNLETimeline *m_pTimeline;
//	int m_pos;
//};
//
//class RemoveClipMarkerCommond : public FUndoCommand
//{
//	Q_OBJECT
//public:
//	RemoveClipMarkerCommond(FSequence *manager, FSNLETimeline *tl, int frame);
//	void Undo() override;
//	void Redo() override;
//private:
//	FSequence *m_sequence;
//	FSNLETimeline *m_pTimeline;
//	int m_pos;
//};
//
//


#endif // UNDOCOMMAND_H
