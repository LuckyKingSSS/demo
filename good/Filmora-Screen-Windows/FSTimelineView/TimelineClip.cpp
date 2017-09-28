#include "stdafx.h"
#include "TimelineClip.h"

#include "inc_FSCommonlib/FSNLETimeline.h"
#include "TimelineManager.h"
#include "AudioWaveMgr.h"

//Clip::Clip()
//{
//	Init();
//}


Clip::Clip(FSNLETimeline *timeline)
{
	Q_ASSERT(timeline);
	m_timeline = timeline;
	m_selected = false;
	m_index = 0;
	m_startAtTrack = 0;
	m_track = nullptr;
}

Clip::~Clip()
{
	delete m_timeline;
}


int Clip::GetStartAtTrack()
{
	return m_startAtTrack;
}

int Clip::GetEndAtTrack()
{
	return m_startAtTrack + GetLenAtTrack() - 1;
}

int Clip::GetLenAtTrack()
{
	return m_timeline->GetPlayLen();
}

int Clip::GetStartAtSrc()
{
	return m_timeline->GetTrimin();
}

int Clip::GetSrcLen()
{
	return m_timeline->GetTotalLength();

}

void Clip::SetStartAtTrack(int pos)
{
	m_startAtTrack = pos;
}

bool Clip::GetSelected()
{
	return m_selected;
}

void Clip::SetSelected(bool selected)
{
	m_selected = selected;
	m_timeline->UnselectMarker();
}

int Clip::GetType() const
{
	return m_timeline->GetType();
}

QString Clip::GetName() const
{
	return m_timeline->GetName();
}

FSNLETimeline* Clip::GetNLETimeline()
{
	return m_timeline;
}

Clip* Clip::Clone()
{
	Clip *clip = new Clip(m_timeline);
	clip->CloneAble(this);
	return clip;
}

void Clip::SetTimelineTrack(TimelineTrack *track)
{
	m_track = track;
}

TimelineTrack* Clip::GetTimelineTrack()
{
	return m_track;
}

void Clip::SetCopyIndex(int index)
{
	m_index = index;
}

int Clip::GetCopyIndex()
{
	return m_index;

}

int Clip::GetIndex()
{
	//return m_index;
	int index = m_track->GetIndex();
	Q_ASSERT(index >= 0);
	return index;
}


Clip* Clip::NextClip()
{
	if (m_track == nullptr)
		return nullptr;

	int index = m_track->GetClipIndex(this);
	Q_ASSERT(index != -1);
	if (index == m_track->GetClips().size() - 1)
		return nullptr;
	else
		return m_track->GetClips()[index + 1];
}

Clip* Clip::PreClip()
{
	if (m_track == nullptr)
		return nullptr;

	int index = m_track->GetClipIndex(this);
	Q_ASSERT(index != -1);
	if (index == 0)
		return nullptr;
	else
		return m_track->GetClips()[index - 1];
}

void Clip::CloneAble(Clip* pAnther)
{
	m_timeline = pAnther->m_timeline->Clone();
	m_selected = pAnther->m_selected;
	m_index = pAnther->m_index;
	m_startAtTrack = pAnther->m_startAtTrack;
	m_track = pAnther->m_track;

	// Clone出来的长度回复到了源长，主要设置一下（底层BUG还未修改，暂时这样）
	m_timeline->SetRange(pAnther->m_timeline->GetTrimin(), pAnther->m_timeline->GetTrimout());
}


//void Clip::SetStartAtSrc(int pos)
//{
//	m_startAtTrack = pos;
//}


//////////////////////////////////////////////////////////////////////////

TimelineTrack::TimelineTrack()
{
	m_beforeRemoveIndex = 0;
	m_bHide = false;
	m_bFreeze = false;
}

Clips TimelineTrack::GetClips()
{
	Clips clips;
	for (auto temp : m_clips)
	{
		clips.push_back(temp.get());
	}
	return clips;
}


Clips TimelineTrack::GetRejectClips(Clips rejectClips)
{
	Clips allClips = GetClips();
	for (auto rejectClip : rejectClips)
	{
		allClips.removeOne(rejectClip);
	}
	return allClips;
}

Clips TimelineTrack::GetInsideClips(int nFromFrame, int nEndFrame)
{
	auto clips = GetClips();
	auto dstRect = QRect(nFromFrame, 0, nEndFrame - nFromFrame + 1, 10);
	Clips insides;
	for (auto clip : clips)
	{
		auto clipRect = QRect(clip->GetStartAtTrack(), 0, clip->GetLenAtTrack(), 10);
		if (clipRect.intersects(dstRect))
			insides.push_back(clip);
	}
	return insides;
}

QPair<Clips, Clips> TimelineTrack::GetInsideOutsizeClips(int nFromFrame, int nEndFrame)
{
	auto clips = GetClips();
	auto dstRect = QRect(nFromFrame, 0, nEndFrame - nFromFrame + 1, 10);
	Clips insides, outsides;
	for (auto clip : clips)
	{
		auto clipRect = QRect(clip->GetStartAtTrack(), 0, clip->GetLenAtTrack(), 10);
		if (clipRect.intersects(dstRect))
			insides.push_back(clip);
		else
			outsides.push_back(clip);
	}
	return QPair<Clips, Clips>(insides, outsides);
}

Clips TimelineTrack::GetRightClips(int nFrame)
{
	Clips clips;
	for (auto clip : m_clips)
	{
		if (clip.get()->GetStartAtTrack() >= nFrame)
			clips.push_back(clip.get());
	}
	return clips;
}

Clips TimelineTrack::GetLeftClips(int nFrame)
{
	Clips clips;
	for (auto clip : m_clips)
	{
		if (clip.get()->GetStartAtTrack() <= nFrame)
			clips.push_back(clip.get());
	}
	return clips;
}

Clips TimelineTrack::GetSelected()
{
	Clips clips;
	for (auto temp : GetSelectedPtr())
	{
		if (temp->GetSelected())
			clips.push_back(temp.get());
	}
	return clips;
}

Clips TimelineTrack::GetUnselected()
{
	Clips clips;
	for (auto temp : m_clips)
	{
		if (!temp->GetSelected())
		{
			clips.push_back(temp.get());
		}
	}
	return clips;
}

int TimelineTrack::GetIndex()
{
	for (int i = 0; i < m_manager->GetTrackCount(); ++i)
	{
		if (this == m_manager->GetTrack(i))
			return i;
	}
	Q_ASSERT(false);
	return -1;
}

int TimelineTrack::GetDuration()
{
	int duration = 0;
	if (!m_clips.isEmpty())
	{
		auto clip = m_clips.last();
		duration = clip->GetStartAtTrack() + clip->GetLenAtTrack();
	}
	return duration;
}

TimelineManager* TimelineTrack::GetManager()
{
	return m_manager;
}



void TimelineTrack::AddClip(int frames, ClipPtr clip)
{
	//clip->SetSelected(true);
	clip->SetStartAtTrack(frames);
	clip->SetTimelineTrack(this);
	if (!clip->GetNLETimeline()->GetBackgroundColor().isValid())
		clip->GetNLETimeline()->SetBackgroundColor(m_manager->GetClipDefaultColor(clip.get()));

	FSNLETimeline *pSrc1 = clip->GetNLETimeline();
	int trimin = pSrc1->GetTrimin();
	int trimout = pSrc1->GetTrimout();
	m_manager->GetRootTimeline()->AddClip(clip->GetNLETimeline(), frames, GetIndex());
	m_manager->GetRootTimeline()->SetClipRange(pSrc1, trimin , trimout);

	bool haveInsert = false;
	for (int i = 0; i < m_clips.size(); ++i)
	{
		if (frames < m_clips[i]->GetStartAtTrack())
		{
			m_clips.insert(i, clip);
			haveInsert = true;
			break;
		}
	}
	if (!haveInsert)
	{
		m_clips.push_back(clip);
	}

	auto type = clip->GetNLETimeline()->GetType();
	if (type == TimelineType_VideoAudio || type == TimelineType_Audio)
	{
		AudioWaveMgr::GetInstance()->Add(clip->GetNLETimeline());
	}
}

void TimelineTrack::RemoveClip(ClipPtr clip)
{

	//Q_ASSERT(!m_clips.contains(clip));
	//clip->SetSelected(false);
	m_clips.removeOne(clip);
	m_manager->GetRootTimeline()->RemoveClip(clip->GetNLETimeline());
}

void TimelineTrack::MoveClip(int frames, ClipPtr clip)
{
	RemoveClip(clip);
	AddClip(frames, clip);
}



ClipsPtr TimelineTrack::GetSelectedPtr()
{
	ClipsPtr clips;
	for (auto temp : m_clips)
	{
		if (temp->GetSelected())
			clips.push_back(temp);
	}
	return clips;
}

ClipPtr TimelineTrack::GetSelectedPtr(int frames)
{
	ClipPtr clip;
	for (auto temp : m_clips)
	{
		if (temp->GetSelected() && frames >= temp->GetStartAtTrack() && (frames < temp->GetStartAtTrack() + temp->GetLenAtTrack()))
		{
			clip = temp;
			break;
		}
	}
	return clip;
}



ClipPtr TimelineTrack::GetClipPtr(int frames)
{
	ClipPtr clip;
	for (auto temp : m_clips)
	{
		if (frames >= temp->GetStartAtTrack() && (frames < temp->GetStartAtTrack() + temp->GetLenAtTrack()))
		{
			clip = temp;
			break;
		}
	}
	return clip;
}

void TimelineTrack::RemoveClips()
{
	auto clips = GetSelectedPtr();
	for (auto temp : clips)
	{
		RemoveClip(temp);
	}
}

ClipPtr TimelineTrack::Split(ClipPtr clip, int frames)
{
	ClipPtr clip2 = ClipPtr(clip->Clone());

	FSNLETimeline *rootTimeline = m_manager->GetRootTimeline();
	FSNLETimeline *pSrc1 = clip->GetNLETimeline();
	int trimin = pSrc1->GetTrimin();
	int trimout = pSrc1->GetTrimout();

	FSNLETimeline *pSrc2 = clip2->GetNLETimeline();

	int nOffset = frames - clip->GetStartAtTrack();
	rootTimeline->SetClipRange(pSrc1, pSrc1->GetTrimin(), pSrc1->GetTrimin() + nOffset - 1);

	clip2->SetStartAtTrack(clip->GetStartAtTrack() + nOffset);
	rootTimeline->AddClip(pSrc2, clip->GetStartAtTrack() + nOffset, GetIndex());
	rootTimeline->SetClipRange(pSrc2, trimin + nOffset, trimout);

	int nIndex = GetClipIndex(clip.get());
	m_clips.insert(nIndex + 1, clip2);

	//m_manager->SetSelected(GetIndex(), frames, true, true);
	pSrc2->SetType(pSrc2->GetType());

	clip->SetSelected(false);
	clip2->SetSelected(true);
	
	auto type = clip2->GetNLETimeline()->GetType();
	if (type == TimelineType_VideoAudio || type == TimelineType_Audio)
	{
		AudioWaveMgr::GetInstance()->Add(clip2->GetNLETimeline());
	}

	return clip2;
}

void TimelineTrack::Split(ClipPtr clip, ClipPtr clip2)
{
	FSNLETimeline *rootTimeline = m_manager->GetRootTimeline();
	NLETimeline *pClipSrc = clip->GetNLETimeline();
	int trimin = pClipSrc->GetTrimin();
	int trimout = pClipSrc->GetTrimout();

	FSNLETimeline *pSrc2 = clip2->GetNLETimeline();
	int trimin2 = pSrc2->GetTrimin();
	int trimout2 = pSrc2->GetTrimout();

	rootTimeline->SetClipRange(pClipSrc, trimin, trimin + clip->GetLenAtTrack() - clip2->GetLenAtTrack() - 1);

	rootTimeline->AddClip(pSrc2, clip2->GetStartAtTrack(), GetIndex());
	rootTimeline->SetClipRange(pSrc2, trimin2, trimout2);
	int nIndex = GetClipIndex(clip.get());
	m_clips.insert(nIndex + 1, clip2);
	pSrc2->SetType(pSrc2->GetType());

	clip->SetSelected(false);
	clip2->SetSelected(true);

	auto type = clip2->GetNLETimeline()->GetType();
	if (type == TimelineType_VideoAudio || type == TimelineType_Audio)
	{
		AudioWaveMgr::GetInstance()->Add(clip2->GetNLETimeline());
	}
}

void TimelineTrack::UnSplit(ClipPtr clip, ClipPtr clip2)
{
	NLETimeline *pSrc = clip->GetNLETimeline();

	int nLen1 = clip->GetLenAtTrack();
	int nLen2 = clip2->GetLenAtTrack();

	auto rootTimeline = m_manager->GetRootTimeline();
	rootTimeline->SetClipRange(pSrc, pSrc->GetTrimin(), pSrc->GetTrimin() + nLen1 + nLen2 - 1);
	rootTimeline->RemoveClip(clip2->GetNLETimeline());
	m_clips.removeOne(clip2);

	clip->SetSelected(true);
}

void TimelineTrack::AdjustClip(int leftSide, int nPosTo)
{

}

ClipsPtr TimelineTrack::GetClipsPtr()
{
	return m_clips;
}

int TimelineTrack::GetBeforeRemoveIndex()
{
	return m_beforeRemoveIndex;
}

void TimelineTrack::SetBeforeRemoveIndex(int index)
{
	m_beforeRemoveIndex = index;
}

void TimelineTrack::SetManager(TimelineManager *manager)
{
	m_manager = manager;
}

bool TimelineTrack::HaveClip(ClipPtr clip)
{
	for (auto temp : m_clips)
	{
		if (temp == clip)
			return true;
	}
	return false;
}

bool TimelineTrack::CanSplit(int frames)
{
	// 可split最小帧数
	const int minSplitFrames = 1;
	for (auto temp : m_clips)
	{
		if (temp->GetSelected() && (frames >= temp->GetStartAtTrack() + minSplitFrames && (frames <= temp->GetStartAtTrack() + temp->GetLenAtTrack() - minSplitFrames)))
		{
			return true;
		}
	}
	return false;
}

int TimelineTrack::GetClipIndex(Clip *clip)
{
	for (int i = 0; i < m_clips.size(); ++i)
	{
		if (m_clips[i].get() == clip)
			return i;
	}
	return -1;
}

Clip* TimelineTrack::GetClip(int frames)
{
	Clip *clip = nullptr;
	for (auto tempClip : GetClips())
	{
		QRect clipRect(tempClip->GetStartAtTrack(), 0, tempClip->GetLenAtTrack(), 10);
		if (clipRect.contains(QPoint(frames, 5)))
		{
			clip = tempClip;
			break;
		}
	}

	return clip;
}

void TimelineTrack::Hide()
{
	m_bHide = true;
	for (auto clip : m_clips)
		clip->GetNLETimeline()->SetHiden(true);
}

void TimelineTrack::Show()
{
	m_bHide = false;
	for (auto clip : m_clips)
		clip->GetNLETimeline()->SetHiden(false);
}

void TimelineTrack::Freeze()
{
	m_bFreeze = true;
}

void TimelineTrack::Unfreeze()
{
	m_bFreeze = false;
}

bool TimelineTrack::IsHiden()
{
	return m_bHide;
}

bool TimelineTrack::IsFreeze()
{
	return m_bFreeze;
}

bool TimelineTrack::IsEditable()
{
	return !IsHiden() && !IsFreeze();
}

bool TimelineTrack::IsEmpty()
{
	return m_clips.isEmpty();
}

bool TimelineTrack::HintCollision(int frames, int duration)
{
	Q_ASSERT(duration > 0);
	QRect collisionRect(frames, 0, duration, 10);
	for (auto clip : GetClips())
	{
		if (collisionRect.intersects(QRect(clip->GetStartAtTrack(), 0, clip->GetLenAtTrack(), 10)))
		{
			return true;
		}
	}

	return false;
}
