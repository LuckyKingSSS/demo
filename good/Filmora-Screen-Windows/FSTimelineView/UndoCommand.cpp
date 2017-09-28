#include "stdafx.h"
#include "UndoCommand.h"

#include "INLEMouseEffectManager.h"


#include "TimelineManager.h"
#include "inc_FSCommonlib/FSNLETimeline.h"



//////////////////////////////////////////////////////////////////////////
AddClipCommand::AddClipCommand(TimelineTrackPtr track, ClipPtr clip, int frames)
:UndoCommand()
, m_clip(clip)
, m_track(track)
, m_frames(frames)
, m_preFrames(clip->GetStartAtTrack())
{

}

AddClipCommand::~AddClipCommand()
{

}

void AddClipCommand::Undo()
{
	m_clip->SetStartAtTrack(m_preFrames);
	m_track->RemoveClip(m_clip);
}

void AddClipCommand::Redo()
{
	m_track->AddClip(m_frames, m_clip);
}

//////////////////////////////////////////////////////////////////////////
RemoveClipCommand::RemoveClipCommand(TimelineTrackPtr track, ClipPtr clip)
:UndoCommand()
, m_track(track)
, m_clip(clip)

{

}

RemoveClipCommand::~RemoveClipCommand()
{
    emit m_track->GetManager()->GetPlayerEditManager()->sigTimelineRemoved(m_clip->GetNLETimeline());
}

void RemoveClipCommand::Undo()
{
	m_track->AddClip(m_clip->GetStartAtTrack(), m_clip);
}

void RemoveClipCommand::Redo()
{
	m_track->RemoveClip(m_clip);
}

//////////////////////////////////////////////////////////////////////////
SplitClipCommand::SplitClipCommand(TimelineTrackPtr track, ClipPtr clip, int frames)
:UndoCommand()
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

//////////////////////////////////////////////////////////////////////////
AdjustClipCommand::AdjustClipCommand(TimelineManager *manager, ClipPtr clip, int newStartPos, int newLen)
:UndoCommand()
, m_manager(manager)
, m_clip(clip)
, m_newStartPos(newStartPos)
, m_newLen(newLen)
, m_oldLen(m_clip->GetLenAtTrack())
, m_oldStartPos(m_clip->GetStartAtTrack())
{

}

void AdjustClipCommand::Undo()
{
	m_manager->_AdjustClip(m_clip, m_oldStartPos, m_oldLen);
}

void AdjustClipCommand::Redo()
{
	m_manager->_AdjustClip(m_clip, m_newStartPos, m_newLen);
}

//////////////////////////////////////////////////////////////////////////
InsertTrackCommand::InsertTrackCommand(TimelineManager *manager, int trackIndex, TimelineTrackPtr track)
:UndoCommand()
, m_manager(manager)
, m_track(track)
, m_trackIndex(trackIndex)
{

}

void InsertTrackCommand::Undo()
{
	m_manager->_RemoveTrack(m_track);
}

void InsertTrackCommand::Redo()
{
	m_manager->_InsertTrack(m_trackIndex, m_track);
}

void InsertTrackCommand::undo()
{
	Undo();
	//emit signalTrackRebuild();
}

void InsertTrackCommand::redo()
{
	Redo();
	//emit signalTrackRebuild();
}

//////////////////////////////////////////////////////////////////////////
RemoveTrackCommand::RemoveTrackCommand(TimelineManager *manager, TimelineTrackPtr track)
:UndoCommand()
, m_manager(manager)
, m_track(track)
{

}

void RemoveTrackCommand::Undo()
{
	m_manager->_InsertTrack(m_track->GetBeforeRemoveIndex(), m_track);
}

void RemoveTrackCommand::Redo()
{
	m_manager->_RemoveTrack(m_track);
}

void RemoveTrackCommand::undo()
{
	Undo();
	//emit signalTrackRebuild();
}

void RemoveTrackCommand::redo()
{
	Redo();
	//emit signalTrackRebuild();
}

//////////////////////////////////////////////////////////////////////////
TrackRebuildCommand::TrackRebuildCommand(TimelineManager *manager)
:UndoCommand()
, m_manager(manager)
{

}

void TrackRebuildCommand::Undo()
{
	QTimer::singleShot(0, [&](){
		m_manager->_ResetAllClipLevel();
		emit signalTrackRebuild();
	});
}

void TrackRebuildCommand::Redo()
{
	m_manager->_ResetAllClipLevel();
	emit signalTrackRebuild();
}

void TrackRebuildCommand::undo()
{
	Undo();
}

void TrackRebuildCommand::redo()
{
	Redo();
}

//////////////////////////////////////////////////////////////////////////
EmitSelectChangeCommand::EmitSelectChangeCommand(TimelineManager *manager)
	:m_manager(manager)
{

}

void EmitSelectChangeCommand::Undo()
{
	QTimer::singleShot(0, [&](){emit m_manager->signalSelectionChanged();});
}

void EmitSelectChangeCommand::Redo()
{
	emit m_manager->signalSelectionChanged();
}

//////////////////////////////////////////////////////////////////////////
SelectedClipCommand::SelectedClipCommand(TimelineManager *manager, ClipPtr clip, bool enabled, bool multiSelected)
:UndoCommand()
, m_manager(manager)
, m_clip(clip)
, m_enabled(enabled)
, m_multiSelected(multiSelected)
{

}

void SelectedClipCommand::Undo()
{
	m_manager->SingleSelect(m_clip->GetIndex(), m_clip->GetStartAtTrack());
}

void SelectedClipCommand::Redo()
{
	m_manager->SingleSelect(m_clip->GetIndex(), m_clip->GetStartAtTrack());
}

void SelectedClipCommand::redo()
{
	Redo();
	//QTimer::singleShot(0, [&]{
	//	emit signalTrackUpdate();
	//});
}

void SelectedClipCommand::undo()
{
	Undo();
	//QTimer::singleShot(0, [&]{
	//	emit signalTrackUpdate();
	//});
}

//////////////////////////////////////////////////////////////////////////
DetachClipCommand::DetachClipCommand(TimelineManager *manager, ClipPtr videoClip, ClipPtr audioClip)
:UndoCommand()
, m_manager(manager)
, m_videoClip(videoClip)
, m_audioClip(audioClip)
{

}

void DetachClipCommand::Undo()
{
	m_manager->_UnDetach(m_videoClip, m_audioClip);

}

void DetachClipCommand::Redo()
{
	m_manager->_Detach(m_videoClip, m_audioClip);

}

//////////////////////////////////////////////////////////////////////////
MuteClipCommand::MuteClipCommand(TimelineManager *manager, ClipPtr clip, bool mute)
:UndoCommand()
, m_manager(manager)
, m_clip(clip)
, m_mute(mute)
{

}

void MuteClipCommand::Undo()
{
	m_manager->_Mute(m_clip, !m_mute);
}

void MuteClipCommand::Redo()
{
	m_manager->_Mute(m_clip, m_mute);
}

//////////////////////////////////////////////////////////////////////////

AddMouseFilterCommand::AddMouseFilterCommand(TimelineManager *manager, ClipPtr clip, ClipPtr filter, MouseEvent type, int level)
:UndoCommand()
, m_manager(manager)
, m_clip(clip)
, m_filter(filter)
, m_type(type)
, m_level(level)
{

}

void AddMouseFilterCommand::Undo()
{
	m_manager->_RemoveMouseFilter(m_clip, m_filter);
}

void AddMouseFilterCommand::Redo()
{
	m_manager->_AddMouseFilter(m_clip, m_filter, m_type, m_level);
}

//////////////////////////////////////////////////////////////////////////
AddMouseSoundCommand::AddMouseSoundCommand(TimelineManager *manager, ClipPtr clip, QString soundPath)
	:UndoCommand()
	, m_manager(manager)
	, m_clip(clip)
	, m_soundPath(soundPath)
{

}

void AddMouseSoundCommand::Undo()
{
	m_manager->_RemoveMouseSound(m_clip);
}

void AddMouseSoundCommand::Redo()
{
	m_manager->_AddMouseSound(m_clip, m_soundPath);
}

//////////////////////////////////////////////////////////////////////////
MoveClipCommand::MoveClipCommand( TimelineTrackPtr srcTrack, TimelineTrackPtr destTrack, ClipPtr clip, int frames)
:UndoCommand()
, m_srcTrack(srcTrack)
, m_destTrack(destTrack)
, m_clip(clip)
, m_preFrames(clip->GetStartAtTrack())
, m_frames(frames)
{

}

void MoveClipCommand::Undo()
{
    m_destTrack->RemoveClip(m_clip);
    m_srcTrack->AddClip(m_preFrames, m_clip);
}

void MoveClipCommand::Redo()
{
    m_srcTrack->RemoveClip(m_clip);
    m_destTrack->AddClip(m_frames, m_clip);
}


//////////////////////////////////////////////////////////////////////////
HideTrackCommand::HideTrackCommand(TimelineManager *manager, int trackIndex)
	:UndoCommand()
	, m_manager(manager)
	, m_trackIndex(trackIndex)
{

}

void HideTrackCommand::Undo()
{
	m_manager->_ShowTrack(m_trackIndex);
}

void HideTrackCommand::Redo()
{
	m_manager->_HideTrack(m_trackIndex);
}

//////////////////////////////////////////////////////////////////////////
ShowTrackCommand::ShowTrackCommand(TimelineManager *manager, int trackIndex)
	:UndoCommand()
	, m_manager(manager)
	, m_trackIndex(trackIndex)
{

}

void ShowTrackCommand::Undo()
{
	m_manager->_HideTrack(m_trackIndex);
}

void ShowTrackCommand::Redo()
{
	m_manager->_ShowTrack(m_trackIndex);
}

//////////////////////////////////////////////////////////////////////////
FreezeTrackCommand::FreezeTrackCommand(TimelineManager *manager, int trackIndex)
	:UndoCommand()
	, m_manager(manager)
	, m_trackIndex(trackIndex)
{

}

void FreezeTrackCommand::Undo()
{
	m_manager->_UnfreezeTrack(m_trackIndex);
}

void FreezeTrackCommand::Redo()
{
	m_manager->_FreezeTrack(m_trackIndex);
}

//////////////////////////////////////////////////////////////////////////
UnfreezeTrackCommand::UnfreezeTrackCommand(TimelineManager *manager, int trackIndex)
	:UndoCommand()
	, m_manager(manager)
	, m_trackIndex(trackIndex)
{

}

void UnfreezeTrackCommand::Undo()
{
	m_manager->_FreezeTrack(m_trackIndex);
}

void UnfreezeTrackCommand::Redo()
{
	m_manager->_UnfreezeTrack(m_trackIndex);
}

//////////////////////////////////////////////////////////////////////////
FreezeFrameCommond::FreezeFrameCommond(TimelineManager *manager, TimelineTrackPtr newTrack, ClipPtr clip, int frame, int freezeLen)
	:UndoCommand()
	, m_manager(manager)
	, m_newTrack(newTrack)
	, m_clip(clip)
	, m_frame(frame)
	, m_freezeLen(freezeLen)
{
	Q_ASSERT(frame >= m_clip->GetStartAtTrack() && frame <= m_clip->GetEndAtTrack());
}

void FreezeFrameCommond::Undo()
{
	auto orinTrack = m_newTrack ? m_manager->GetTrack(m_newTrack->GetIndex() - 1) : m_FreezeClip->GetTimelineTrack();

	auto track = m_FreezeClip->GetTimelineTrack();
	track->RemoveClip(m_FreezeClip);
	if (m_leftClip) track->RemoveClip(m_leftClip);
	if (m_rightClip) track->RemoveClip(m_rightClip);
	
	orinTrack->AddClip(m_clip->GetStartAtTrack(),m_clip);
}

void FreezeFrameCommond::Redo()
{
	if (!m_FreezeClip)
	{
		Clip* freezeclip = m_clip.get()->Clone();
		m_FreezeClip = ClipPtr(freezeclip);

		m_FreezeClip->GetNLETimeline()->SetMarkers(QList<int>());

		int newTrimin = m_FreezeClip->GetNLETimeline()->GetTrimin() + m_frame - m_FreezeClip->GetNLETimeline()->GetPosition();
		m_FreezeClip->GetNLETimeline()->SetRange(newTrimin, newTrimin);

		m_FreezeClip->GetNLETimeline()->SetFreezeEnable(true);
		m_FreezeClip->GetNLETimeline()->SetFreezeFrame(m_frame, m_freezeLen);
		m_FreezeClip->SetStartAtTrack(m_frame);
		m_FreezeClip->SetSelected(true);
		m_FreezeClip->GetNLETimeline()->SetType(TimelineType_Image);

		
		//m_FreezeClip->GetNLETimeline()->SetBackgroundColor(m_manager->GetClipDefaultColor(m_FreezeClip.get()));
		if (m_clip->GetStartAtTrack() < m_frame)
		{
			Clip* leftclip = m_clip.get()->Clone();
			m_leftClip = ClipPtr(leftclip);
			m_leftClip->GetNLETimeline()->SetTrimout(m_clip->GetNLETimeline()->GetTrimin() + m_frame - m_clip->GetStartAtTrack() - 1);
			m_leftClip->SetSelected(false);
		}

		if (m_clip->GetEndAtTrack() > m_frame)
		{
			Clip* rightclip = m_clip.get()->Clone();
			m_rightClip = ClipPtr(rightclip);
			m_rightClip->GetNLETimeline()->SetTrimin(m_clip->GetNLETimeline()->GetTrimin() + m_frame - m_clip->GetStartAtTrack() + 1);
			m_rightClip->GetNLETimeline()->SetPosition(m_frame + 1 + m_freezeLen);
			m_rightClip->SetStartAtTrack(m_frame + 1 + m_freezeLen);
			m_rightClip->SetSelected(false);
			m_rightClip->GetNLETimeline()->SetType(m_rightClip->GetNLETimeline()->GetType());
		}
	}
	auto thisTrack = m_clip->GetTimelineTrack();
	thisTrack->RemoveClip(m_clip);
	auto track = m_newTrack ? m_newTrack.get() : thisTrack;
	track->AddClip(m_FreezeClip->GetStartAtTrack(), m_FreezeClip);
	if (m_leftClip) track->AddClip(m_leftClip->GetStartAtTrack(), m_leftClip);
	if (m_rightClip) track->AddClip(m_rightClip->GetStartAtTrack(), m_rightClip);
}

//////////////////////////////////////////////////////////////////////////
ChangeSpeedCommand::ChangeSpeedCommand(TimelineManager *manager, ClipPtr clip, int newStart, int newLen)
	:UndoCommand()
	,m_manager(manager)
	, m_clip(clip)
	, m_newStart(newStart)
	, m_newLen(newLen)
	, m_newScale(-1.0)
	,m_bChanged(false)
{
	m_oldStart = clip->GetStartAtTrack();
	m_oldScale = clip->GetNLETimeline()->GetRenderScale();
}

void ChangeSpeedCommand::Undo()
{
	m_manager->_ChangeSpeedStandar(m_clip, m_oldStart, 1.0 / m_oldScale);
}

void ChangeSpeedCommand::Redo()
{
	if (m_bChanged)
	{
		m_manager->_ChangeSpeedStandar(m_clip, m_newStart, m_newScale);
	}
	else
	{
		m_newScale = m_manager->_ChangeSpeed(m_clip, m_newStart, m_newLen);
		m_bChanged = true;
	}
}

//////////////////////////////////////////////////////////////////////////
SetSpeedScaleCommand::SetSpeedScaleCommand(TimelineManager *manager, ClipPtr clip, float scale)
	:UndoCommand()
	,m_manager(manager)
	, m_clip(clip)
	, m_newScale(scale)
	, m_trimLen(0)
{
	m_oldScale = clip->GetNLETimeline()->GetRenderScale();
}

void SetSpeedScaleCommand::Undo()
{
	m_manager->_UnsetSpeedScale(m_clip, 1.0 / m_oldScale, m_trimLen);
}

void SetSpeedScaleCommand::Redo()
{
	m_trimLen = m_manager->_SetSpeedScale(m_clip, m_newScale);
}

//////////////////////////////////////////////////////////////////////////
MarkerOperationCommand::MarkerOperationCommand(QObject *parent)
	:UndoCommand(parent)
{

}

void MarkerOperationCommand::undo()
{
	Undo();
	QTimer::singleShot(0, [&]{emit signalTrackUpdate(); });
}

void MarkerOperationCommand::redo()
{
	Redo();
	QTimer::singleShot(0, [&]{emit signalTrackUpdate(); });
}

//////////////////////////////////////////////////////////////////////////
AddTLMarkCommond::AddTLMarkCommond(TimelineManager *manager, int frame)
	:MarkerOperationCommand()
	, m_manager(manager)
	, m_frame(frame)
{
}

void AddTLMarkCommond::Undo()
{
	m_manager->GetMarkPointMgr()->_Remove(m_addedMark);
}

void AddTLMarkCommond::Redo()
{
	if (m_addedMark)
		m_manager->GetMarkPointMgr()->_Add(m_addedMark);
	else
		m_addedMark = m_manager->GetMarkPointMgr()->_Add(m_frame);

}

//////////////////////////////////////////////////////////////////////////
RemoveTLMarkCommond::RemoveTLMarkCommond(TimelineManager *manager, MarkPointPtr markPoint)
	:UndoCommand()
	, m_manager(manager)
	, m_markPoint(markPoint)
{

}

void RemoveTLMarkCommond::Undo()
{
	m_manager->GetMarkPointMgr()->_Add(m_markPoint);
}

void RemoveTLMarkCommond::Redo()
{
	m_manager->GetMarkPointMgr()->_Remove(m_markPoint);
}

//////////////////////////////////////////////////////////////////////////
MoveTLMarkCommond::MoveTLMarkCommond(TimelineManager *manager, MarkPointPtr markPoint, int pos)
	:UndoCommand()
	, m_manager(manager)
	, m_markPoint(markPoint)
	, m_newPos(pos)
{
	m_oldPos = m_markPoint->GetFrame();
}

void MoveTLMarkCommond::Undo()
{
	m_manager->GetMarkPointMgr()->_Move(m_markPoint, m_oldPos);
}

void MoveTLMarkCommond::Redo()
{
	m_manager->GetMarkPointMgr()->_Move(m_markPoint, m_newPos);
}

//////////////////////////////////////////////////////////////////////////
AddClipMarkerCommond::AddClipMarkerCommond(TimelineManager *manager, FSNLETimeline *tl, int frame)
	:MarkerOperationCommand()
	, m_manager(manager)
	, m_pTimeline(tl)
	, m_pos(frame)
{

}

void AddClipMarkerCommond::Undo()
{
	m_pTimeline->RemoveMarker(m_pos);
}

void AddClipMarkerCommond::Redo()
{
	m_pTimeline->AddMarker(m_pos);
}

//////////////////////////////////////////////////////////////////////////
RemoveClipMarkerCommond::RemoveClipMarkerCommond(TimelineManager *manager, FSNLETimeline *tl, int frame)
	:UndoCommand()
	, m_manager(manager)
	, m_pTimeline(tl)
	, m_pos(frame)
{

}

void RemoveClipMarkerCommond::Undo()
{
	m_pTimeline->AddMarker(m_pos);
}

void RemoveClipMarkerCommond::Redo()
{
	m_pTimeline->RemoveMarker(m_pos);
}


