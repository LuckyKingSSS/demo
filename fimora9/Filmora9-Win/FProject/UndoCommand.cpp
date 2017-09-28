#include "stdafx.h"

#include "UndoCommand.h"

#include "inc_FProject/FSequence.h"

//////////////////////////////////////////////////////////////////////////


UpdateCommand::UpdateCommand()
:FUndoCommand()
{

}

void UpdateCommand::Undo()
{
    QTimer::singleShot(0, [&]{
        emit sigTrackUpdate();
    });

}

void UpdateCommand::Redo()
{
    QTimer::singleShot(0, [&]{
        emit sigTrackUpdate();
    });

}



//////////////////////////////////////////////////////////////////////////
AddTimelineCommand::AddTimelineCommand(FTrackPtr track, FNLETimelinePtr clip, int frames)
:FUndoCommand()
, m_clip(clip)
, m_track(track)
, m_frames(frames)
, m_preFrames(clip->GetStartAtTrack())
{

}

AddTimelineCommand::~AddTimelineCommand()
{

}

void AddTimelineCommand::Undo()
{
	m_clip->SetStartAtTrack(m_preFrames);
	m_track->RemoveTimeline(m_clip);
}

void AddTimelineCommand::Redo()
{
    m_track->AddTimeline(m_frames, m_clip);
}

//////////////////////////////////////////////////////////////////////////
MoveTimelineCommand::MoveTimelineCommand( FTrackPtr srcTrack, FTrackPtr destTrack, FNLETimelinePtr clip, int frames)
:FUndoCommand()
, m_srcTrack(srcTrack)
, m_destTrack(destTrack)
, m_clip(clip)
, m_preFrames(clip->GetStartAtTrack())
, m_frames(frames)
{

}

void MoveTimelineCommand::Undo()
{
    m_destTrack->RemoveTimeline(m_clip);
    m_srcTrack->AddTimeline(m_preFrames, m_clip);
}

void MoveTimelineCommand::Redo()
{
    m_srcTrack->RemoveTimeline(m_clip);
    m_destTrack->AddTimeline(m_frames, m_clip);
}

//////////////////////////////////////////////////////////////////////////
RemoveTimelineCommand::RemoveTimelineCommand(FTrackPtr track, FNLETimelinePtr clip)
:FUndoCommand()
, m_track(track)
, m_clip(clip)

{

}

RemoveTimelineCommand::~RemoveTimelineCommand()
{
    
}

void RemoveTimelineCommand::Undo()
{
    m_track->AddTimeline(m_clip->GetStartAtTrack(), m_clip);
}

void RemoveTimelineCommand::Redo()
{
	m_track->RemoveTimeline(m_clip);
}
//
//////////////////////////////////////////////////////////////////////////
SplitClipCommand::SplitClipCommand(FTrackPtr track, FNLETimelinePtr clip, int frames)
:FUndoCommand()
, m_track(track)
, m_clip(clip)
, m_frames(frames)
, m_clipAfter(nullptr)
{

}

void SplitClipCommand::Undo()
{
	m_track->UnSplit(m_clip, m_clipAfter);
}

void SplitClipCommand::Redo()
{
	if (m_clipAfter)
	{
		m_track->Split(m_clip, m_clipAfter);
	}
	else
	{
		m_clipAfter = m_track->Split(m_clip, m_frames);
	}
}

////////////////////////////////////////////////////////////////////////////
//AdjustClipCommand::AdjustClipCommand(FSequence *sequence, FNLETimelinePtr clip, int newStartPos, int newLen)
//:FUndoCommand()
//, m_sequence(sequence)
//, m_clip(clip)
//, m_newStartPos(newStartPos)
//, m_newLen(newLen)
//, m_oldLen(m_clip->GetLenAtTrack())
//, m_oldStartPos(m_clip->GetStartAtTrack())
//{
//
//}
//
//void AdjustClipCommand::Undo()
//{
//	m_sequence->_AdjustClip(m_clip, m_oldStartPos, m_oldLen);
//}
//
//void AdjustClipCommand::Redo()
//{
//	m_sequence->_AdjustClip(m_clip, m_newStartPos, m_newLen);
//}
//
//////////////////////////////////////////////////////////////////////////
InsertTrackCommand::InsertTrackCommand(FSequence *sequence, int trackIndex, FTrackPtr track)
:FUndoCommand()
, m_sequence(sequence)
, m_track(track)
, m_trackIndex(trackIndex)
{

}

void InsertTrackCommand::Undo()
{
	m_sequence->_RemoveTrack(m_track);
}

void InsertTrackCommand::Redo()
{
	m_sequence->_InsertTrack(m_trackIndex, m_track);
}



//////////////////////////////////////////////////////////////////////////
RemoveTrackCommand::RemoveTrackCommand(FSequence *sequence, FTrackPtr track)
:FUndoCommand()
, m_sequence(sequence)
, m_track(track)
, m_preIndex(-1)
{

}

void RemoveTrackCommand::Undo()
{
    m_sequence->_InsertTrack(m_preIndex, m_track);
}

void RemoveTrackCommand::Redo()
{
    m_preIndex = m_track->GetIndex();
    m_sequence->_RemoveTrack(m_track);
}

//////////////////////////////////////////////////////////////////////////
RebuildTrackCommand::RebuildTrackCommand(FSequence *sequence)
:FUndoCommand()
, m_sequence(sequence)
{

}

void RebuildTrackCommand::Undo()
{
	QTimer::singleShot(0, [&](){
		m_sequence->_ResetTimelineLevel();
		emit sigTrackRebuild();
	});
}

void RebuildTrackCommand::Redo()
{
    m_sequence->_ResetTimelineLevel();
	emit sigTrackRebuild();
}


//////////////////////////////////////////////////////////////////////////
SelectChangeCommand::SelectChangeCommand(FSequence *sequence)
	:m_sequence(sequence)
{

}

void SelectChangeCommand::Undo()
{
	QTimer::singleShot(0, [&](){
        emit m_sequence->sigSelectionChanged();
    });
}

void SelectChangeCommand::Redo()
{
	emit m_sequence->sigSelectionChanged();
}
//
////////////////////////////////////////////////////////////////////////////
//SelectedClipCommand::SelectedClipCommand(FSequence *sequence, FNLETimelinePtr clip, bool enabled, bool multiSelected)
//:FUndoCommand()
//, m_sequence(sequence)
//, m_clip(clip)
//, m_enabled(enabled)
//, m_multiSelected(multiSelected)
//{
//
//}
//
//void SelectedClipCommand::Undo()
//{
//	m_sequence->SingleSelect(m_clip->GetIndex(), m_clip->GetStartAtTrack());
//}
//
//void SelectedClipCommand::Redo()
//{
//	m_sequence->SingleSelect(m_clip->GetIndex(), m_clip->GetStartAtTrack());
//}
//
//void SelectedClipCommand::redo()
//{
//	Redo();
//	//QTimer::singleShot(0, [&]{
//	//	emit signalTrackUpdate();
//	//});
//}
//
//void SelectedClipCommand::undo()
//{
//	Undo();
//	//QTimer::singleShot(0, [&]{
//	//	emit signalTrackUpdate();
//	//});
//}
//
////////////////////////////////////////////////////////////////////////////
//DetachClipCommand::DetachClipCommand(FSequence *sequence, FNLETimelinePtr videoClip, FNLETimelinePtr audioClip)
//:FUndoCommand()
//, m_sequence(sequence)
//, m_videoClip(videoClip)
//, m_audioClip(audioClip)
//{
//
//}
//
//void DetachClipCommand::Undo()
//{
//	m_sequence->_UnDetach(m_videoClip, m_audioClip);
//
//}
//
//void DetachClipCommand::Redo()
//{
//	m_sequence->_Detach(m_videoClip, m_audioClip);
//
//}
//
////////////////////////////////////////////////////////////////////////////
//MuteClipCommand::MuteClipCommand(FSequence *sequence, FNLETimelinePtr clip, bool mute)
//:FUndoCommand()
//, m_sequence(sequence)
//, m_clip(clip)
//, m_mute(mute)
//{
//
//}
//
//void MuteClipCommand::Undo()
//{
//	m_sequence->_Mute(m_clip, !m_mute);
//}
//
//void MuteClipCommand::Redo()
//{
//	m_sequence->_Mute(m_clip, m_mute);
//}
//
////////////////////////////////////////////////////////////////////////////
//
//AddMouseFilterCommand::AddMouseFilterCommand(FSequence *sequence, FNLETimelinePtr clip, FNLETimelinePtr filter, MouseEvent type, int level)
//:FUndoCommand()
//, m_sequence(sequence)
//, m_clip(clip)
//, m_filter(filter)
//, m_type(type)
//, m_level(level)
//{
//
//}
//
//void AddMouseFilterCommand::Undo()
//{
//	m_sequence->_RemoveMouseFilter(m_clip, m_filter);
//}
//
//void AddMouseFilterCommand::Redo()
//{
//	m_sequence->_AddMouseFilter(m_clip, m_filter, m_type, m_level);
//}
//
////////////////////////////////////////////////////////////////////////////
//AddMouseSoundCommand::AddMouseSoundCommand(FSequence *sequence, FNLETimelinePtr clip, QString soundPath)
//	:FUndoCommand()
//	, m_sequence(sequence)
//	, m_clip(clip)
//	, m_soundPath(soundPath)
//{
//
//}
//
//void AddMouseSoundCommand::Undo()
//{
//	m_sequence->_RemoveMouseSound(m_clip);
//}
//
//void AddMouseSoundCommand::Redo()
//{
//	m_sequence->_AddMouseSound(m_clip, m_soundPath);
//}
//

//
////////////////////////////////////////////////////////////////////////////
//HideTrackCommand::HideTrackCommand(FSequence *sequence, int trackIndex)
//	:FUndoCommand()
//	, m_sequence(sequence)
//	, m_trackIndex(trackIndex)
//{
//
//}
//
//void HideTrackCommand::Undo()
//{
//	m_sequence->_ShowTrack(m_trackIndex);
//}
//
//void HideTrackCommand::Redo()
//{
//	m_sequence->_HideTrack(m_trackIndex);
//}
//
////////////////////////////////////////////////////////////////////////////
//ShowTrackCommand::ShowTrackCommand(FSequence *sequence, int trackIndex)
//	:FUndoCommand()
//	, m_sequence(sequence)
//	, m_trackIndex(trackIndex)
//{
//
//}
//
//void ShowTrackCommand::Undo()
//{
//	m_sequence->_HideTrack(m_trackIndex);
//}
//
//void ShowTrackCommand::Redo()
//{
//	m_sequence->_ShowTrack(m_trackIndex);
//}
//
////////////////////////////////////////////////////////////////////////////
//FreezeTrackCommand::FreezeTrackCommand(FSequence *sequence, int trackIndex)
//	:FUndoCommand()
//	, m_sequence(sequence)
//	, m_trackIndex(trackIndex)
//{
//
//}
//
//void FreezeTrackCommand::Undo()
//{
//	m_sequence->_UnfreezeTrack(m_trackIndex);
//}
//
//void FreezeTrackCommand::Redo()
//{
//	m_sequence->_FreezeTrack(m_trackIndex);
//}
//
////////////////////////////////////////////////////////////////////////////
//UnfreezeTrackCommand::UnfreezeTrackCommand(FSequence *sequence, int trackIndex)
//	:FUndoCommand()
//	, m_sequence(sequence)
//	, m_trackIndex(trackIndex)
//{
//
//}
//
//void UnfreezeTrackCommand::Undo()
//{
//	m_sequence->_FreezeTrack(m_trackIndex);
//}
//
//void UnfreezeTrackCommand::Redo()
//{
//	m_sequence->_UnfreezeTrack(m_trackIndex);
//}
//
////////////////////////////////////////////////////////////////////////////
//FreezeFrameCommond::FreezeFrameCommond(FSequence *sequence, FTrackPtr newTrack, FNLETimelinePtr clip, int frame, int freezeLen)
//	:FUndoCommand()
//	, m_sequence(sequence)
//	, m_newTrack(newTrack)
//	, m_clip(clip)
//	, m_frame(frame)
//	, m_freezeLen(freezeLen)
//{
//	Q_ASSERT(frame >= m_clip->GetStartAtTrack() && frame <= m_clip->GetEndAtTrack());
//}
//
//void FreezeFrameCommond::Undo()
//{
//	auto orinTrack = m_newTrack ? m_sequence->GetTrack(m_newTrack->GetIndex() - 1) : m_FreezeClip->GetTimelineTrack();
//
//	auto track = m_FreezeClip->GetTimelineTrack();
//	track->RemoveClip(m_FreezeClip);
//	if (m_leftClip) track->RemoveClip(m_leftClip);
//	if (m_rightClip) track->RemoveClip(m_rightClip);
//	
//	orinTrack->AddClip(m_clip->GetStartAtTrack(),m_clip);
//}
//
//void FreezeFrameCommond::Redo()
//{
//	if (!m_FreezeClip)
//	{
//		Clip* freezeclip = m_clip.get()->Clone();
//		m_FreezeClip = FNLETimelinePtr(freezeclip);
//		
//		m_FreezeClip->GetNLETimeline()->SetFreezeFrame(m_frame, m_freezeLen);
//		m_FreezeClip->GetNLETimeline()->SetFreezeEnable(true);
//		m_FreezeClip->SetStartAtTrack(m_frame);
//		m_FreezeClip->SetSelected(true);
//		m_FreezeClip->GetNLETimeline()->SetType(TimelineType_Image);
//		m_FreezeClip->GetNLETimeline()->SetBackgroundColor(m_sequence->GetClipDefaultColor(m_FreezeClip.get()));
//		if (m_clip->GetStartAtTrack() < m_frame)
//		{
//			Clip* leftclip = m_clip.get()->Clone();
//			m_leftClip = FNLETimelinePtr(leftclip);
//			m_leftClip->GetNLETimeline()->SetTrimout(m_clip->GetNLETimeline()->GetTrimin() + m_frame - m_clip->GetStartAtTrack() - 1);
//			m_leftClip->SetSelected(false);
//		}
//
//		if (m_clip->GetEndAtTrack() > m_frame)
//		{
//			Clip* rightclip = m_clip.get()->Clone();
//			m_rightClip = FNLETimelinePtr(rightclip);
//			m_rightClip->GetNLETimeline()->SetTrimin(m_clip->GetNLETimeline()->GetTrimin() + m_frame - m_clip->GetStartAtTrack() + 1);
//			m_rightClip->GetNLETimeline()->SetPosition(m_frame + 1 + m_freezeLen);
//			m_rightClip->SetStartAtTrack(m_frame + 1 + m_freezeLen);
//			m_rightClip->SetSelected(false);
//			m_rightClip->GetNLETimeline()->SetType(m_rightClip->GetNLETimeline()->GetType());
//		}
//	}
//	auto thisTrack = m_clip->GetTimelineTrack();
//	thisTrack->RemoveClip(m_clip);
//	auto track = m_newTrack ? m_newTrack.get() : thisTrack;
//	track->AddClip(m_FreezeClip->GetStartAtTrack(), m_FreezeClip);
//	if (m_leftClip) track->AddClip(m_leftClip->GetStartAtTrack(), m_leftClip);
//	if (m_rightClip) track->AddClip(m_rightClip->GetStartAtTrack(), m_rightClip);
//}
//
////////////////////////////////////////////////////////////////////////////
//ChangeSpeedCommand::ChangeSpeedCommand(FSequence *sequence, FNLETimelinePtr clip, int newStart, int newLen)
//	:FUndoCommand()
//	,m_sequence(sequence)
//	, m_clip(clip)
//	, m_newStart(newStart)
//	, m_newLen(newLen)
//	, m_newScale(-1.0)
//	,m_bChanged(false)
//{
//	m_oldStart = clip->GetStartAtTrack();
//	m_oldScale = clip->GetNLETimeline()->GetRenderScale();
//}
//
//void ChangeSpeedCommand::Undo()
//{
//	m_sequence->_ChangeSpeedStandar(m_clip, m_oldStart, m_oldScale);
//}
//
//void ChangeSpeedCommand::Redo()
//{
//	if (m_bChanged)
//	{
//		m_sequence->_ChangeSpeedStandar(m_clip, m_newStart, m_newScale);
//	}
//	else
//	{
//		m_newScale = m_sequence->_ChangeSpeed(m_clip, m_newStart, m_newLen);
//		m_bChanged = true;
//	}
//}
//
////////////////////////////////////////////////////////////////////////////
//SetSpeedScaleCommand::SetSpeedScaleCommand(FSequence *sequence, FNLETimelinePtr clip, float scale)
//	:FUndoCommand()
//	,m_sequence(sequence)
//	, m_clip(clip)
//	, m_newScale(scale)
//	, m_trimLen(0)
//{
//	m_oldScale = clip->GetNLETimeline()->GetRenderScale();
//}
//
//void SetSpeedScaleCommand::Undo()
//{
//	m_sequence->_UnsetSpeedScale(m_clip, m_oldScale, m_trimLen);
//}
//
//void SetSpeedScaleCommand::Redo()
//{
//	m_trimLen = m_sequence->_SetSpeedScale(m_clip, m_newScale);
//}
//
////////////////////////////////////////////////////////////////////////////
//MarkerOperationCommand::MarkerOperationCommand(QObject *parent)
//	:FUndoCommand(parent)
//{
//
//}
//
//void MarkerOperationCommand::undo()
//{
//	Undo();
//	QTimer::singleShot(0, [&]{emit signalTrackUpdate(); });
//}
//
//void MarkerOperationCommand::redo()
//{
//	Redo();
//	QTimer::singleShot(0, [&]{emit signalTrackUpdate(); });
//}
//
////////////////////////////////////////////////////////////////////////////
//AddTLMarkCommond::AddTLMarkCommond(FSequence *sequence, int frame)
//	:MarkerOperationCommand()
//	, m_sequence(sequence)
//	, m_frame(frame)
//{
//}
//
//void AddTLMarkCommond::Undo()
//{
//	m_sequence->GetMarkPointMgr()->_Remove(m_addedMark);
//}
//
//void AddTLMarkCommond::Redo()
//{
//	if (m_addedMark)
//		m_sequence->GetMarkPointMgr()->_Add(m_addedMark);
//	else
//		m_addedMark = m_sequence->GetMarkPointMgr()->_Add(m_frame);
//
//}
//
////////////////////////////////////////////////////////////////////////////
//RemoveTLMarkCommond::RemoveTLMarkCommond(FSequence *sequence, MarkPointPtr markPoint)
//	:FUndoCommand()
//	, m_sequence(sequence)
//	, m_markPoint(markPoint)
//{
//
//}
//
//void RemoveTLMarkCommond::Undo()
//{
//	m_sequence->GetMarkPointMgr()->_Add(m_markPoint);
//}
//
//void RemoveTLMarkCommond::Redo()
//{
//	m_sequence->GetMarkPointMgr()->_Remove(m_markPoint);
//}
//
////////////////////////////////////////////////////////////////////////////
//MoveTLMarkCommond::MoveTLMarkCommond(FSequence *sequence, MarkPointPtr markPoint, int pos)
//	:FUndoCommand()
//	, m_sequence(sequence)
//	, m_markPoint(markPoint)
//	, m_newPos(pos)
//{
//	m_oldPos = m_markPoint->GetFrame();
//}
//
//void MoveTLMarkCommond::Undo()
//{
//	m_sequence->GetMarkPointMgr()->_Move(m_markPoint, m_oldPos);
//}
//
//void MoveTLMarkCommond::Redo()
//{
//	m_sequence->GetMarkPointMgr()->_Move(m_markPoint, m_newPos);
//}
//
////////////////////////////////////////////////////////////////////////////
//AddClipMarkerCommond::AddClipMarkerCommond(FSequence *sequence, FSNLETimeline *tl, int frame)
//	:MarkerOperationCommand()
//	, m_sequence(sequence)
//	, m_pTimeline(tl)
//	, m_pos(frame)
//{
//
//}
//
//void AddClipMarkerCommond::Undo()
//{
//	m_pTimeline->RemoveMarker(m_pos);
//}
//
//void AddClipMarkerCommond::Redo()
//{
//	m_pTimeline->AddMarker(m_pos);
//}
//
////////////////////////////////////////////////////////////////////////////
//RemoveClipMarkerCommond::RemoveClipMarkerCommond(FSequence *sequence, FSNLETimeline *tl, int frame)
//	:FUndoCommand()
//	, m_sequence(sequence)
//	, m_pTimeline(tl)
//	, m_pos(frame)
//{
//
//}
//
//void RemoveClipMarkerCommond::Undo()
//{
//	m_pTimeline->AddMarker(m_pos);
//}
//
//void RemoveClipMarkerCommond::Redo()
//{
//	m_pTimeline->RemoveMarker(m_pos);
//}
//
//
