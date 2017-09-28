#include "stdafx.h"
#include "TimelineManager.h"

#include "inc_CommonLib/NLEWraper.h"

#include "inc_FSCommonlib/FSNLETimeline.h"

#include "NLEKey.h"

#include "INLEMouseEffectManager.h"
#include "INLEKeyboardStreamManager.h"
#include "AudioWaveMgr.h"

#define INVALID_TIMELINEINDEX(index, list) Q_ASSERT(index>=0 && index < list.size())

static LPCWSTR Mark_TimelineDetach = L"Mark_TimelineDetack";
static LPCWSTR Mark_TimelineMute = L"Mark_TimelineMute";

static Clips ClipsPtr2Clips(const ClipsPtr &clipsPtr)
{
	Clips retClips;
	for (auto temp : clipsPtr)
	{
		retClips.push_back(temp.get());
	}
	return retClips;
}


static TimelineTracks sTracksPtr2Tracks(const TimelineTracksPtr &tracks)
{
	TimelineTracks tempTracks;
	for (auto temp : tracks)
	{
		tempTracks.push_back(temp.get());
	}
	return tempTracks;
}

static TimelineManager *s_TimelineManager = nullptr;
TimelineManager* TimelineManager::GetInstance()
{
	if (s_TimelineManager == nullptr)
		s_TimelineManager = new TimelineManager(0);
	return s_TimelineManager;

}

TimelineManager::TimelineManager(QObject *parent)
: QObject(parent)
{
	m_pShiftSelectBaseClip = nullptr;
	m_pRootTimeline = nullptr;
	m_playerEditManager = new PlayerEditorManager(this, this);
	m_pTitleBarManager = new TitleBarManager(this, this);
	m_pMarkPointMgr = new MarkPointMgr(this,this);
	m_pClipMarkerMgr = new ClipMarkerMgr(this, this);
	InitClipBkColor();

	Reset();
}

TimelineManager::~TimelineManager()
{

}

void TimelineManager::Reset()
{
	m_tracks.clear();
	m_curFrames = 0;
	m_canPaste = false;
	
	if (!m_pRootTimeline)
	{
		m_pRootTimeline = new FSNLETimeline(NLEWraper::GetInstance().GetPlayerMaterProvider()->CreateEmptyTimeline());
	}
	else
	{
		m_pRootTimeline->GetSrc()->RemoveAll();
		m_pRootTimeline->UpdateTrackMarks(QList<MarkPointPtr>());
	}
	m_pRootTimeline->SetRenderFrameRate(25.0f);
	m_UndoStack = new QUndoStack(this);
	SetUndoLimit(30);
	_InsertTrack(0, TimelineTrackPtr(new TimelineTrack()));
	GetMarkPointMgr()->Clear();
	emit signalTrackRebuild();
}

void TimelineManager::Load(INLETimelineBase *rootTimeline)
{
	Q_ASSERT(rootTimeline);

	QList<NLEComPtr<INLETimelineBase>> timelines;
	NLEComPtr<INLEIterator> pIterator = NULL;
	rootTimeline->QueryInterface(IID_INLEIterator, (void**)&pIterator);

	if (SUCCEEDED(pIterator->MoveToBegin()))
	{
		for (; pIterator->Current(); pIterator->MoveToNext())
		{
			NLEComPtr<INLETimelineBase> pTimelineTemp;
			pIterator->Current()->QueryInterface(IID_INLETimelineBase, (void**)&pTimelineTemp);
			Q_ASSERT(pTimelineTemp);
			timelines.push_back(pTimelineTemp);
		}
	}
	
	for (auto tl : timelines)
	{
		FSNLETimeline *fstl = new FSNLETimeline(tl);
		int nLevel = fstl->GetLevel();
		auto track = GetTrack(nLevel);
		if (!track)
		{
			int nCnt = m_tracks.size();
			if (nCnt - 1 < nLevel)
			{
				for (int i = 0; i < nLevel - nCnt + 1;++i)
				{
					_InsertTrack(m_tracks.size(), TimelineTrackPtr(new TimelineTrack()));
				}
			}
			track = GetTrack(nLevel);
		}
		Q_ASSERT(track);

		fstl->SetType(fstl->GetType());
		auto clip = CreateClipFromTimeline(fstl);
		track->AddClip(fstl->GetPosition(), clip);

		if (fstl->GetType() == TimelineType_VideoAudio || fstl->GetType() == TimelineType_Audio)
		{
			AudioWaveMgr::GetInstance()->Add(fstl);
		}
	}

	FSNLETimeline rootTl(rootTimeline);
	RestoreHidenFreezeTrackFromRootTimeine(&rootTl);
	GetMarkPointMgr()->InitMarkPoints(rootTl.CreateTrackMarks());

	emit signalTrackRebuild();
	emit signalTrackUpdate();
}

void TimelineManager::Detach()
{
	//kSource_HasVideoStream
	//kSource_HasAudioStream
	auto clips = GetSelected();
	Q_ASSERT(!clips.isEmpty());

	BeginComposite();

	// clips都是已经排过序的，从上到下从左到右
	int clipTrack = -1;
	int newTrack = -1;
	bool hadInsertTrack = false;
	for (auto clip : clips)
	{
		if (CanDetack(clip))
		{
			int track = clip->GetIndex();
			// 增加新轨道
			if (track != clipTrack)
			{
				clipTrack = track;
				newTrack = track + 1;
				//InsertTrack(newTrack);
				auto track = TimelineTrackPtr(new TimelineTrack());
				PushCommand(new InsertTrackCommand(this, newTrack, track));
				hadInsertTrack = true;
			}

			auto newTimeline = clip->GetNLETimeline()->Clone();
			auto audioClip = AddClip(newTrack, clip->GetStartAtTrack(), newTimeline,false);
			PushCommand(new DetachClipCommand(this, GetClipPtrFromClip(clip), GetClipPtrFromClip(audioClip)));
			PushCommand(new EmitSelectChangeCommand(this));
		}
	}
	if (hadInsertTrack)
		PushCommand(new TrackRebuildCommand(this));

	EndComposite();
}

bool TimelineManager::CanDetack(Clip *clip)
{
	Q_ASSERT(clip);
	Q_ASSERT(GetSelected().contains(clip));

	auto timeline = clip->GetNLETimeline();
	auto pro = timeline->GetProp();
	INT nVideo = 0;
	pro->GetInt(kSource_HasVideoStream, &nVideo);
	INT nAudio = 0;
	pro->GetInt(kSource_HasAudioStream, &nAudio);

	INT nDetach = 0;
	pro->GetInt(Mark_TimelineDetach, &nDetach);

	return nVideo && nAudio && nDetach != 1;
}

void TimelineManager::AddMouseFilter(Clip *clip, FSNLETimeline *filter, MouseEvent type, int level)
{
	Q_ASSERT(filter);
	Q_ASSERT(CanAddMouseFilter(clip));
	PushCommand(new AddMouseFilterCommand(this, GetClipPtrFromClip(clip), ClipPtr(new Clip(filter)), type, level));
}


bool TimelineManager::CanAddMouseFilter(Clip *clip)
{
	Q_ASSERT(clip);
	auto prop = clip->GetNLETimeline()->GetProp();
	INT nStream = 0;
	prop->GetInt(kSource_ScreenRecordingHasMouseStream, &nStream);
	return nStream == 1;
}

void TimelineManager::AddMouseSound(Clip *clip, QString soundPath)
{
	Q_ASSERT(clip);
	PushCommand(new AddMouseSoundCommand(this, GetClipPtrFromClip(clip), soundPath));
}

void TimelineManager::Mute(bool enabled)
{
	auto clips = GetSelected();
	Q_ASSERT(!clips.isEmpty());

	BeginComposite();
	for (auto clip : clips)
	{
		bool canMute = false;
		bool hadMute = false;
		CanMute(clip, canMute, hadMute);
		if (canMute)
		{
			PushCommand(new MuteClipCommand(this, GetClipPtrFromClip(clip), enabled));
		}
	}
	EndComposite();
}

void TimelineManager::CanMute(Clip *clip, bool &canMute, bool &hadMute)
{
	Q_ASSERT(clip);
	Q_ASSERT(GetSelected().contains(clip));

	auto timeline = clip->GetNLETimeline();
	auto pro = timeline->GetProp();
	INT nVideo = 0;
	pro->GetInt(kSource_HasVideoStream, &nVideo);
	INT nAudio = 0;
	pro->GetInt(kSource_HasAudioStream, &nAudio);

	INT nDetach = 0;
	pro->GetInt(Mark_TimelineDetach, &nDetach);

	canMute = (nVideo && nAudio && nDetach != 1);
	if (canMute)
	{
		INT bMute = 0;
		pro->GetInt(kRender_AudioEnable, &bMute);
		hadMute = bMute == 0 ? true : false;
	}
}


FSNLETimeline* TimelineManager::GetRootTimeline()
{
	return m_pRootTimeline;
}

QList<TimelineTrack*> TimelineManager::GetHidenTracks()
{
	QList<TimelineTrack*> tracks;
	for (auto track : m_tracks)
	{
		if (track->IsHiden())
			tracks.push_back(track.get());
	}
	return tracks;
}

QList<TimelineTrack*> TimelineManager::GetFreezenTracks()
{
	QList<TimelineTrack*> tracks;
	for (auto track : m_tracks)
	{
		if (track->IsFreeze())
			tracks.push_back(track.get());
	}
	return tracks;
}

ClipPtr TimelineManager::CreateClipFromTimeline(FSNLETimeline *timeline)
{
	auto clip = ClipPtr(new Clip(timeline));

	return clip;
}

void TimelineManager::PushCommand(UndoCommand *pCommond)
{
	connect(pCommond, &UndoCommand::signalTrackUpdate, this, &TimelineManager::signalTrackUpdate, Qt::UniqueConnection);
	connect(pCommond, &UndoCommand::signalTrackRebuild, this, &TimelineManager::signalTrackRebuild, Qt::UniqueConnection);
	connect(pCommond, &UndoCommand::sigPause, this, &TimelineManager::sigPause, Qt::UniqueConnection);
	m_UndoStack->push(pCommond);
	emit sigDoCommand();
}

void TimelineManager::PushCommand(QUndoCommand *pCommond)
{
	m_UndoStack->push(pCommond);

}

void TimelineManager::RemoveClip(Clip *clip)
{
	for (auto track : m_tracks)
	{
		for (auto tempClip : track->GetClipsPtr())
		{
			if (tempClip.get() == clip)
			{
				PushCommand(new RemoveClipCommand(track, tempClip));
				return;
			}
		}
	}
}

Clips TimelineManager::GetClips(int frames)
{
	Clips clips;
	for (auto track : m_tracks)
	{
		auto clip = track->GetClip(frames);
		if (clip)
			clips.push_back(clip);
	}
	return clips;
}

Clips TimelineManager::GetClips()
{
	Clips clips;
	for (auto track : m_tracks)
	{
		clips += track->GetClips();
	}
	return clips;
}

Clips TimelineManager::GetEditableClips()
{
	Clips clips;
	for (auto track : m_tracks)
	{
		if (track->IsEditable())
			clips += track->GetClips();
	}
	return clips;
}

Clips TimelineManager::GetEditableClips(int frames)
{
	Clips clips;
	for (auto track : m_tracks)
	{
		if (track->IsEditable())
		{
			auto clip = track->GetClip(frames);
			if (clip)
				clips.push_back(clip);
		}
	}
	return clips;
}

void TimelineManager::InitClipBkColor()
{
	m_allBkColors.push_back(ClipColor(QColor(78, 104, 115), "#4E6873", "#4E6873",":/FSTimelineView/light/Resources/light/color1.png"));
	m_allBkColors.push_back(ClipColor(QColor(50, 102, 130), "#326682", "#326682", ":/FSTimelineView/light/Resources/light/color2.png"));
	m_allBkColors.push_back(ClipColor(QColor(60, 103, 76), "#3C674C", "#3C674C", ":/FSTimelineView/light/Resources/light/color3.png"));
	m_allBkColors.push_back(ClipColor(QColor(79, 131, 47), "#4F832F", "#4F832F", ":/FSTimelineView/light/Resources/light/color4.png"));
	m_allBkColors.push_back(ClipColor(QColor(136, 115, 44), "#88732C", "#88732C", ":/FSTimelineView/light/Resources/light/color5.png"));
	m_allBkColors.push_back(ClipColor(QColor(116, 88, 76), "#74584C", "#74584C", ":/FSTimelineView/light/Resources/light/color6.png"));
	m_allBkColors.push_back(ClipColor(QColor(130, 50, 55), "#823237", "#823237", ":/FSTimelineView/light/Resources/light/color7.png"));
	m_allBkColors.push_back(ClipColor(QColor(111, 80, 113), "#6F5071", "#6F5071", ":/FSTimelineView/light/Resources/light/color8.png"));

	m_defaultBkColor.insert(TimelineType_VideoAudio, m_allBkColors[0].clr);
	m_defaultBkColor.insert(TimelineType_Video, m_allBkColors[1].clr);
	m_defaultBkColor.insert(TimelineType_Audio, m_allBkColors[2].clr);
	m_defaultBkColor.insert(TimelineType_Image, m_allBkColors[5].clr);
	m_defaultBkColor.insert(TimelineType_Annotaions_Caption, m_allBkColors[7].clr);
	m_defaultBkColor.insert(TimelineType_Annotaions_Overlay, m_allBkColors[7].clr);
}

void TimelineManager::ManumalEmitSelectedChanged()
{
	QTimer::singleShot(0, [&]{emit signalSelectionChanged(); });
}

Clips TimelineManager::GetClipsWithout(Clip *pClip)
{
	auto clips = GetClips();
	clips.removeOne(pClip);
	return clips;
}

Clip* TimelineManager::AddClip(int trackIndex, int frames, FSNLETimeline *timeline, bool bEmitSelectChange /*= true*/)
{
	INVALID_TIMELINEINDEX(trackIndex, m_tracks);

	auto track = m_tracks[trackIndex];
	auto clip = CreateClipFromTimeline(timeline);
	PushCommand(new AddClipCommand(track, clip, frames));
	clip->SetSelected(true);
	if(bEmitSelectChange) PushCommand(new EmitSelectChangeCommand(this));
	emit GetPlayerEditManager()->sigTimelineAdded(timeline);
	return clip.get();
}

void TimelineManager::MoveClip(int trackIndex, int frames, Clip* clip)
{
	INVALID_TIMELINEINDEX(trackIndex, m_tracks);

	ClipPtr clipPtr = GetClipPtrFromClip(clip);
    TimelineTrackPtr srcTrackPtr;
	for (auto temp : m_tracks)
	{
		if (temp->HaveClip(clipPtr))
		{
            
            srcTrackPtr = temp;
			break;
		}
	}
    Q_ASSERT(srcTrackPtr);

	auto destTrack = m_tracks[trackIndex];
    PushCommand(new MoveClipCommand(srcTrackPtr, destTrack, clipPtr, frames));


}

void TimelineManager::Copy()
{
	m_pCopyedClips.clear();

	auto clips = GetSelected();

	for (auto temp : clips)
	{
		ClipPtr newClip = ClipPtr(temp->Clone());
		newClip->SetCopyIndex(temp->GetIndex());
		m_pCopyedClips.push_back(newClip);
	}

	m_canPaste = true;
	emit signalTrackUpdate();
}

bool TimelineManager::CanCopy()
{
	return !GetSelected().isEmpty();
}

void TimelineManager::Paste()
{
	int frames = m_curFrames;
	if (!m_pCopyedClips.isEmpty())
	{
		BeginComposite();
		
		ClipsPtr tempCopyedClips;
		for (auto temp : m_pCopyedClips)
		{
			tempCopyedClips.push_back(ClipPtr(temp->Clone()));
		}

		auto curSelected = GetSelected();
		for (auto temp : curSelected)
		{
			temp->SetSelected(false);
			//PushCommand(new SelectedClipCommand(this, GetClipPtrFromClip(temp), false, true));
		}

		int startFrames = m_pCopyedClips[0]->GetStartAtTrack();
		int endFrames = startFrames + m_pCopyedClips[0]->GetLenAtTrack();
		// 计算整体长度和起始帧
		for (auto temp : m_pCopyedClips)
		{
			int tempStartFrames = temp->GetStartAtTrack();
			startFrames = qMin(startFrames, tempStartFrames);
			endFrames = qMax(endFrames, tempStartFrames + temp->GetLenAtTrack());
		}

		// 获取现有的可插入clip的轨道
		TimelineTracksPtr newTimelineTracks;
		for (int i = m_tracks.size() - 1; i >= 0; --i)
		{
			auto track = m_tracks[i];
			if (track->HintCollision(frames, endFrames - startFrames))
			{
				break;
			}
			newTimelineTracks.push_front(track);
		}

		// 计算总共需要多少条轨道
		int minCopyedTrackIndex = m_pCopyedClips[0]->GetCopyIndex();
		int maxCopyedTrackIndex = m_pCopyedClips[0]->GetCopyIndex();
		for (auto temp : m_pCopyedClips)
		{
			minCopyedTrackIndex = qMin(minCopyedTrackIndex, temp->GetCopyIndex());
			maxCopyedTrackIndex = qMax(maxCopyedTrackIndex, temp->GetCopyIndex());
		}
		int needNewTrackCount = maxCopyedTrackIndex - minCopyedTrackIndex + 1;

		bool needCreateNewTrack = false;
		// 不够新轨道
		if (needNewTrackCount > newTimelineTracks.size())
		{
			needCreateNewTrack = true;
			int newTimelineTracksCount = newTimelineTracks.size();
			for (int i = 0; i < needNewTrackCount - newTimelineTracksCount; ++i)
			{
				auto tempNewTrack = TimelineTrackPtr(new TimelineTrack());
				newTimelineTracks.push_back(tempNewTrack);
				PushCommand(new InsertTrackCommand(this, GetTrackCount(), tempNewTrack));
			}
		}
		else if (needNewTrackCount < newTimelineTracks.size())
		{
			newTimelineTracks = newTimelineTracks.mid(0, needNewTrackCount);
		}


		//int startIndex = m_pCopyedClips.first()->GetIndex();
		for (auto temp : tempCopyedClips)
		{
			PushCommand(new AddClipCommand(newTimelineTracks[temp->GetCopyIndex() - minCopyedTrackIndex], temp, frames + temp->GetStartAtTrack() - startFrames));
			emit sigPasted(temp->GetNLETimeline());
		}



		if (needCreateNewTrack)
			PushCommand(new TrackRebuildCommand(this));

		PushCommand(new EmitSelectChangeCommand(this));
		EndComposite();
	}

}

bool TimelineManager::CanPaste()
{
	return m_canPaste;
}

void TimelineManager::Cut()
{
	m_pCopyedClips.clear();

	auto clips = GetSelected();

	for (auto temp : clips)
	{
		temp->SetCopyIndex(temp->GetIndex());
		m_pCopyedClips.push_back(GetClipPtrFromClip(temp));
	}

	//Copy();
	RemoveClips();

	m_canPaste = true;

}

bool TimelineManager::CanCut()
{
	return CanCopy();
}

void TimelineManager::Split()
{
	int frames = GetCurrentFrames();
	BeginComposite();
	for (auto temp : m_tracks)
	{
		if (temp->CanSplit(frames))
		{
			auto clip = temp->GetSelectedPtr(frames);
			PushCommand(new SplitClipCommand(temp, clip, frames));
			PushCommand(new EmitSelectChangeCommand(this));
		}
	}
	EndComposite();
}

bool TimelineManager::CanSplit()
{
	int frames = GetCurrentFrames();
	bool canSplit = false;
	for (auto temp : m_tracks)
	{
		if (temp->CanSplit(frames))
		{
			canSplit = true;
			break;
		}
	}
	return canSplit;
}

void TimelineManager::AdjustClip(bool leftSide, int frames)
{
	// 只可能选中一个
	auto clips = GetSelected();
	Q_ASSERT(!clips.isEmpty());

	auto clip = GetClipPtrFromClip(clips[0]);
	int newStart = 0;
	int newLen = 0;

	if (leftSide)
	{
		newStart = frames;
		newLen = clip->GetLenAtTrack() + clip->GetStartAtTrack() - newStart;
	}
	else
	{
		newStart = clip->GetStartAtTrack();
		newLen = frames - clip->GetStartAtTrack();
	}


	PushCommand(new AdjustClipCommand(this, clip, newStart, newLen));

}

QPoint TimelineManager::GetTrimRange(bool bLeftSide)
{
	int leftoffset = INT_MAX;
	int rightoffset = INT_MAX;
	auto selectClips = GetSelected();
	for (auto clip : selectClips)
	{
		if (bLeftSide)
		{
			//往左拖动
			//1.trimin长度
			int trimIn = clip->GetNLETimeline()->GetTrimin();
			auto tlType = clip->GetNLETimeline()->GetType();
			if (tlType == TimelineType_Image || 
				tlType == TimelineType_Annotaions_Caption ||
				tlType == TimelineType_Annotaions_Overlay ||
				tlType == TimelineType_Annotaions_Element)
				trimIn = INT_MAX;
			//2.startpos离上一个clip的长度
			auto preClip = clip->PreClip();
			int leftMargin = preClip ? clip->GetStartAtTrack() - preClip->GetEndAtTrack() - 1 : clip->GetStartAtTrack();
			leftoffset = qMin(leftoffset, qMin(trimIn, leftMargin));
			//往右拖动

			rightoffset = qMin(rightoffset, clip->GetLenAtTrack() - 1);
		}
		else
		{
			//往左拖动
			leftoffset = qMin(leftoffset,clip->GetLenAtTrack() - 1);
			//往右拖动
			int trimout = clip->GetNLETimeline()->GetTotalLength() - clip->GetNLETimeline()->GetTrimout() - 1;
			auto tlType = clip->GetNLETimeline()->GetType();
			if (tlType == TimelineType_Image || 
				tlType == TimelineType_Annotaions_Caption ||
				tlType == TimelineType_Annotaions_Overlay ||
				tlType == TimelineType_Annotaions_Element)
				trimout = INT_MAX;

			auto nextClip = clip->NextClip();
			int rightMargin = nextClip ? nextClip->GetStartAtTrack() - clip->GetEndAtTrack() - 1 : INT_MAX;
			rightoffset = qMin(rightoffset, qMin(trimout, rightMargin));
		}
	}
	return QPoint(-leftoffset, rightoffset);
}

void TimelineManager::Trim(Clip* clip, int startPos, int length)
{
	PushCommand(new AdjustClipCommand(this, GetClipPtrFromClip(clip), startPos, length));
}

void TimelineManager::_AdjustClip(ClipPtr clip, int startPos, int len)
{
	int nOldStartPos = clip->GetStartAtTrack();

	int nDelta = startPos - nOldStartPos;

	//
	FSNLETimeline *pSrc = clip->GetNLETimeline();
	int nNewTrimin = pSrc->GetTrimin() + nDelta;
	nNewTrimin = nNewTrimin < 0 ? 0 : nNewTrimin;
	int nNewLen = len;

	clip->SetStartAtTrack(startPos);
	m_pRootTimeline->SetClipPos(pSrc, startPos); 
	m_pRootTimeline->SetClipRange(pSrc, nNewTrimin, nNewTrimin + nNewLen - 1);
	pSrc->SetType(pSrc->GetType());
}

void TimelineManager::RemoveClips()
{
	BeginComposite();
	bool bRemoved = false;
	for (auto temp : m_tracks)
	{
		for (auto clip : temp->GetSelectedPtr())
		{
			bRemoved = true;
			PushCommand(new RemoveClipCommand(temp, clip));
		}
	}
	if (bRemoved)
	{
		PushCommand(new EmitSelectChangeCommand(this));
	}
	
	RemoveTailEmptyTrack();

	EndComposite();
}


bool TimelineManager::CanRemove()
{
	bool bCan = false;
	for (auto temp : m_tracks)
	{
		auto clips = temp->GetSelected();
		if (clips.size() > 0)
		{
			return true;
		}
	}
	if (GetMarkPointMgr()->GetSelected())
	{
		return true;
	}
	return false;
}

Clips TimelineManager::GetSelected()
{
	ClipsPtr clips;
	for (auto temp : m_tracks)
	{
		clips << temp->GetSelectedPtr();
	}
	return ClipsPtr2Clips(clips);
}

Clips TimelineManager::GetSelected(int frames)
{
	ClipsPtr clips;
	for (auto temp : m_tracks)
	{
		auto clip = temp->GetSelectedPtr(frames);
		if (clip)
			clips.push_back(clip);
	}
	return ClipsPtr2Clips(clips);
}

Clip* TimelineManager::GetSelected(int trackIndex, int frames)
{
	auto track = m_tracks[trackIndex];
	return track->GetSelectedPtr(frames).get();
}

Clips TimelineManager::GetUnselected()
{
	Clips unselectClips;
	auto allClips = GetClips();
	auto selectedClips = GetSelected();
	for (auto clip : allClips)
	{
		if (!selectedClips.contains(clip))
			unselectClips.push_back(clip);
	}
	return unselectClips;
}

void TimelineManager::SingleSelect(int nTrackIndex, int nFrame)
{
	auto preSelectedClips = GetSelected();
	if (nTrackIndex < 0)
	{
		for (auto sClip : preSelectedClips)
			sClip->SetSelected(false);
		if (preSelectedClips.size() > 0)
			ManumalEmitSelectedChanged();
		emit signalTrackUpdate();
		return;
	}

	Q_ASSERT(nTrackIndex >= 0 && nTrackIndex < m_tracks.size());
	auto hitClip = m_tracks[nTrackIndex]->GetClip(nFrame);
	if (!hitClip)
	{
		for (auto sClip : preSelectedClips)
			sClip->SetSelected(false);
		if (preSelectedClips.size() > 0) 
			ManumalEmitSelectedChanged();
		emit signalTrackUpdate();
		return;
	}
	SetShiftSelectBaseClip(hitClip);

	bool bSelectionChanged = false;
	if (preSelectedClips.contains(hitClip) && preSelectedClips.size() > 1)   //选择多个，点击当前clip
		bSelectionChanged = true;
	if (!preSelectedClips.contains(hitClip))								 //当前clip没有被选中
		bSelectionChanged = true;

	for (auto sClip : preSelectedClips)
		sClip->SetSelected(false);

	hitClip->SetSelected(true);

	if (bSelectionChanged) 
		ManumalEmitSelectedChanged();

	emit signalTrackUpdate();
}

void TimelineManager::BeginRectangleSelect()
{
	m_lstRectangleSelectsTmpClips = GetSelected();
}

void TimelineManager::RectangleSelect(int nTrackFrom, int nTrackTo, int nFrameFrom, int nFrameTo)
{
	//return;
	
	Q_ASSERT(nTrackFrom >= 0 && nTrackFrom < m_tracks.size());
	Q_ASSERT(nTrackTo >= 0 && nTrackTo < m_tracks.size());
	Q_ASSERT(nTrackFrom <= nTrackTo);
	Q_ASSERT(nFrameFrom >= 0 && nFrameTo >= 0 && nFrameTo >= nFrameFrom);
	Clips selectedClips;

	auto preSelectedClips = GetSelected();
	for (int i = nTrackFrom; i <= nTrackTo; ++i)
	{
		if (!m_tracks[i]->IsEditable())
			continue;
		auto insides = m_tracks[i]->GetInsideClips(nFrameFrom, nFrameTo);
		selectedClips += insides;
	}
	for (auto clip : preSelectedClips)
		clip->SetSelected(false);
	for (auto clip : selectedClips)
		clip->SetSelected(true);
	//if (preSelectedClips.size() != selectedClips.size())
	//	//m_bRectangleSelectChanged = true;
	//	ManumalEmitSelectedChanged();
	//emit signalTrackUpdate();
}

void TimelineManager::EndRectangleSelect()
{
	auto selectClips = GetSelected();
	if (m_lstRectangleSelectsTmpClips != selectClips)
		ManumalEmitSelectedChanged();
	m_lstRectangleSelectsTmpClips.clear();
}

void TimelineManager::SetShiftSelectBaseClip(Clip *clip)
{
	m_pShiftSelectBaseClip = clip;
}

void TimelineManager::ShiftSelect(int nTrackIndex, int nFrame)
{
	auto preSelectedClips = GetSelected();
	if (nTrackIndex < 0)
	{
		for (auto sClip : preSelectedClips)
			sClip->SetSelected(false);
		if (preSelectedClips.size() > 0)
			ManumalEmitSelectedChanged();
		emit signalTrackUpdate();
		return;
	}

	Q_ASSERT(nTrackIndex >= 0 && nTrackIndex < m_tracks.size());
	auto hitClip = m_tracks[nTrackIndex]->GetClip(nFrame);
	if (!hitClip)
	{
		for (auto sClip : preSelectedClips)
			sClip->SetSelected(false);
		if (preSelectedClips.size() > 0)
			ManumalEmitSelectedChanged();
		emit signalTrackUpdate();
		return;
	}

	if (!m_pShiftSelectBaseClip)												//基准clip不存在
		m_pShiftSelectBaseClip = hitClip;
	auto allClips = GetSelected();
	if (m_pShiftSelectBaseClip && !allClips.contains(m_pShiftSelectBaseClip))	//基准clip已被删除
		m_pShiftSelectBaseClip = hitClip;
	if (m_pShiftSelectBaseClip && !m_pShiftSelectBaseClip->GetSelected())       //基准clip被取消选择
		m_pShiftSelectBaseClip = hitClip;

	if (m_pShiftSelectBaseClip == hitClip && preSelectedClips.size() == 1) 
		return;

	int trackIndex1 = m_pShiftSelectBaseClip->GetIndex();
	int trackIndex2 = hitClip->GetIndex();
	int start1 = m_pShiftSelectBaseClip->GetStartAtTrack();
	int start2 = hitClip->GetStartAtTrack();

	Clips selectedClips;
	if (trackIndex1 == trackIndex2)
	{
		selectedClips = m_tracks[trackIndex1]->GetInsideClips(qMin(start1, start2), qMax(start1, start2));
	}
	else
	{
		int minIndex, maxIndex, minStart, maxStart;
		if (trackIndex1 > trackIndex2)
		{
			minIndex = trackIndex2;
			minStart = start2;
			maxIndex = trackIndex1;
			maxStart = start1;
		}
		else
		{
			minIndex = trackIndex1;
			minStart = start1;
			maxIndex = trackIndex2;
			maxStart = start2;
		}
		for (int i = minIndex; i <= maxIndex; ++i)
		{
			if (i == minIndex)
				selectedClips += m_tracks[i]->GetLeftClips(minStart);
			else if (i == maxIndex)
				selectedClips += m_tracks[i]->GetRightClips(maxStart);
			else
			{
				//排除不可编辑的track
				if (m_tracks[i]->IsEditable())
					selectedClips += m_tracks[i]->GetClips();
			}
				
		}
	}

	for (auto clip : preSelectedClips)
		clip->SetSelected(false);
	for (auto clip : selectedClips)
		clip->SetSelected(true);
	if (selectedClips != preSelectedClips)
		ManumalEmitSelectedChanged();
	emit signalTrackUpdate();
}

void TimelineManager::CtrlSelect(int nTrackIndex, int nFrame)
{
	if (nTrackIndex < 0) return;
	Q_ASSERT(nTrackIndex >= 0 && nTrackIndex < m_tracks.size());
	auto hitClip = m_tracks[nTrackIndex]->GetClip(nFrame);
	if (!hitClip) return;
	hitClip->SetSelected(!hitClip->GetSelected());
	ManumalEmitSelectedChanged();
	emit signalTrackUpdate();
}

void TimelineManager::SelectTrack(int nTrackIndex)
{
	if (nTrackIndex < 0) return;
	Q_ASSERT(nTrackIndex >= 0 && nTrackIndex < m_tracks.size());
	auto unselectClips = m_tracks[nTrackIndex]->GetUnselected();

	for (auto clip : unselectClips)
		clip->SetSelected(true);
	if (unselectClips.size() > 0)
	{
		ManumalEmitSelectedChanged();
		emit signalTrackUpdate();
	}
}

void TimelineManager::MultiSelect(QList<FSNLETimeline*> timelines)
{
	bool bSelectChanged = false;
	auto preSelectedClips = GetSelected();
	Clips preUnselectClips = GetUnselected();

	for (auto preSelected : preSelectedClips)
	{
		if (!timelines.contains(preSelected->GetNLETimeline()))
		{
			preSelected->SetSelected(false);
			bSelectChanged = true;
		}
	}
	for (auto preUnselect : preUnselectClips)
	{
		if (timelines.contains(preUnselect->GetNLETimeline()))
		{
			preUnselect->SetSelected(true);
			bSelectChanged = true;
		}
	}
	if (bSelectChanged)
	{
		ManumalEmitSelectedChanged();
	}
	emit signalTrackUpdate();
}

void TimelineManager::UnselectAll()
{
	bool selected = false;
	auto clips = GetSelected();
	for (auto temp : clips)
	{
		selected |= temp->GetSelected();
		temp->SetSelected(false);
	}
	emit signalTrackUpdate();

	if (selected)
		ManumalEmitSelectedChanged();
}


void TimelineManager::SelectedAll()
{
	bool unSelected = false;
	ClipsPtr clips;
	for (auto temp : m_tracks)
	{
		for (auto clip : temp->GetClips())
		{
			unSelected |= !clip->GetSelected();
			clip->SetSelected(true);
		}
	}
	emit signalTrackUpdate();
	if (unSelected)
		ManumalEmitSelectedChanged();
}

void TimelineManager::SelectAllEditableClips()
{
	bool unSelected = false;
	ClipsPtr clips;
	for (auto temp : m_tracks)
	{
		if (!temp->IsEditable())
			continue;
		for (auto clip : temp->GetClips())
		{
			unSelected |= !clip->GetSelected();
			clip->SetSelected(true);
		}
	}
	emit signalTrackUpdate();
	if (unSelected)
		ManumalEmitSelectedChanged();
}

TimelineTracks TimelineManager::GetTracks()
{
	return sTracksPtr2Tracks(m_tracks);
}

TimelineTrack* TimelineManager::GetTrack(int trackIndex)
{
	//INVALID_TIMELINEINDEX(trackIndex, m_tracks);
	if (trackIndex < 0 || trackIndex >= m_tracks.size())
	{
		return nullptr;
	}

	return m_tracks[trackIndex].get();
}

int TimelineManager::GetTrackCount()
{
	return m_tracks.size();
}

void TimelineManager::InsertTrack(int trackIndex)
{
	Q_ASSERT(trackIndex >= 0 && trackIndex <= m_tracks.size());

	auto track = TimelineTrackPtr(new TimelineTrack());
	BeginComposite();
	PushCommand(new InsertTrackCommand(this, trackIndex, track));
	PushCommand(new TrackRebuildCommand(this));
	EndComposite();
}

//void TimelineManager::_InsertTrack(int trackIndex)
//{
//	auto track = TimelineTrackPtr(new TimelineTrack());
//	track->SetManager(this);
//	m_tracks.insert(trackIndex, track);
//}

void TimelineManager::_InsertTrack(int trackIndex, TimelineTrackPtr track)
{
	track->SetManager(this);
	m_tracks.insert(trackIndex, track);
	for (auto clip : track->GetClipsPtr())
	{
		GetRootTimeline()->AddClip(clip->GetNLETimeline(), clip->GetStartAtTrack(), track->GetIndex());
	}
}

void TimelineManager::_RemoveTrack(TimelineTrackPtr track)
{
	int preIndex = track->GetIndex();
	track->SetBeforeRemoveIndex(preIndex);
	m_tracks.removeAt(preIndex);
	for (auto clip : track->GetClipsPtr())
	{
		GetRootTimeline()->RemoveClip(clip->GetNLETimeline());
	}
}


void TimelineManager::_Mute(ClipPtr clip, bool mute)
{
	auto prop = clip->GetNLETimeline()->GetProp();
	prop->SetInt(kRender_AudioEnable, !mute);
	prop->SetInt(Mark_TimelineMute,  mute ? 1 : 0);
	//clip->GetNLETimeline()->SetType(!mute ? TimelineType_VideoAudio : TimelineType_Video);
	emit m_playerEditManager->sigMute(clip->GetNLETimeline(), mute);
}

void TimelineManager::_Detach(ClipPtr videoClip, ClipPtr audioClip)
{
	auto videoProp = videoClip->GetNLETimeline()->GetProp();
	videoProp->SetInt(kRender_AudioEnable, 0);
	videoProp->SetInt(Mark_TimelineDetach, 1);
	videoClip->GetNLETimeline()->SetType(TimelineType_Video);
	auto audioProp = audioClip->GetNLETimeline()->GetProp();
	audioProp->SetInt(kRender_VideoEnable, 0);
	audioProp->SetInt(kRender_AudioEnable, 1);
	audioProp->SetInt(Mark_TimelineDetach, 1);
	audioClip->GetNLETimeline()->SetType(TimelineType_Audio);

	if (!videoClip->GetNLETimeline()->IsBkColorChangeByUser())
	{
		videoClip->GetNLETimeline()->SetBackgroundColor(GetClipDefaultColor(videoClip.get()));
	}
	if (!audioClip->GetNLETimeline()->IsBkColorChangeByUser())
	{
		audioClip->GetNLETimeline()->SetBackgroundColor(GetClipDefaultColor(audioClip.get()));
	}
}

void TimelineManager::_UnDetach(ClipPtr videoClip, ClipPtr audioClip)
{
	auto videoProp = videoClip->GetNLETimeline()->GetProp();
	int mute;
	videoProp->GetInt(Mark_TimelineMute, &mute);
	videoProp->SetInt(kRender_AudioEnable, !mute);
	videoProp->Delete(Mark_TimelineDetach);
	videoClip->GetNLETimeline()->SetType(TimelineType_VideoAudio);
	auto audioProp = audioClip->GetNLETimeline()->GetProp();
	audioProp->SetInt(kRender_VideoEnable, 1);
	audioProp->Delete(Mark_TimelineDetach);

	if (!videoClip->GetNLETimeline()->IsBkColorChangeByUser())
	{
		videoClip->GetNLETimeline()->SetBackgroundColor(GetClipDefaultColor(videoClip.get()));
	}
}

void TimelineManager::_RemoveMouseFilter(ClipPtr clip, ClipPtr filter)
{
	NLEComPtr<INLEMouseEffectManager> mouseEffectManager;
	clip->GetNLETimeline()->GetSrc()->QueryInterface(IID_INLEMouseEffectManager, (void**)&mouseEffectManager);
	Q_ASSERT(mouseEffectManager);
	mouseEffectManager->RemoveEffectFilter(filter->GetNLETimeline()->GetSrc());
	emit m_playerEditManager->sigMouseEffectRemove(clip->GetNLETimeline(), filter->GetNLETimeline()->GetSrc().p);
}

void TimelineManager::_AddMouseFilter(ClipPtr clip, ClipPtr filter, MouseEvent type /*= MouseEvent::MouseEvent_None*/, int level /*= 0*/)
{
	NLEComPtr<INLEMouseEffectManager> mouseEffectManager;
	clip->GetNLETimeline()->GetSrc()->QueryInterface(IID_INLEMouseEffectManager, (void**)&mouseEffectManager);
	Q_ASSERT(mouseEffectManager);
	auto tltype = filter->GetNLETimeline()->GetType();
	auto prop = filter->GetNLETimeline()->GetProp();
	if (tltype == TimelineType_MouseHightlineFilter)
	{
		prop->SetInt(Key_MouseFilterType, Value_MouseFilterType_Highlight);
	}
	else if (tltype == TimelineType_MouseClickRingsFilter)
	{
		prop->SetInt(Key_MouseFilterType, Value_MouseFilterType_Rings);
	}
	mouseEffectManager->AddEffectFilter(filter->GetNLETimeline()->GetSrc(), type, level);
	NLERange srcRange;
	clip->GetNLETimeline()->GetProp()->GetSourceRange(&srcRange);
	prop->SetRenderRange(srcRange);
	emit m_playerEditManager->sigMouseEffectAdded(clip->GetNLETimeline(), filter->GetNLETimeline()->GetSrc().p);
}

void TimelineManager::_AddMouseSound(ClipPtr clip, QString soundPath)
{
	NLEComPtr<INLEMouseEffectManager> pINLEMouseEffectManager = nullptr;
	clip->GetNLETimeline()->GetSrc()->QueryInterface(IID_INLEMouseEffectManager, (void**)&pINLEMouseEffectManager);
	Q_ASSERT(pINLEMouseEffectManager);
	if (pINLEMouseEffectManager)
	{
		std::wstring str = soundPath.toStdWString();
		pINLEMouseEffectManager->SetSoundFilePath(str.c_str());
		emit m_playerEditManager->sigMouseEffectAdded(clip->GetNLETimeline(), nullptr);
	}
}

void TimelineManager::_RemoveMouseSound(ClipPtr clip)
{
	NLEComPtr<INLEMouseEffectManager> pINLEMouseEffectManager = nullptr;
	clip->GetNLETimeline()->GetSrc()->QueryInterface(IID_INLEMouseEffectManager, (void**)&pINLEMouseEffectManager);
	Q_ASSERT(pINLEMouseEffectManager);
	if (pINLEMouseEffectManager)
	{
		pINLEMouseEffectManager->SetSoundFilePath(L"");
		emit m_playerEditManager->sigMouseEffectRemove(clip->GetNLETimeline(), nullptr);
	}
}

void TimelineManager::_ResetAllClipLevel()
{
	for (auto track : GetTracks())
	{
		int nIndex = track->GetIndex();
		for (auto clip : track->GetClips())
		{
			clip->GetNLETimeline()->SetLevel(nIndex);
		}
	}
}

void TimelineManager::_HideTrack(int trackIndex)
{
	Q_ASSERT(trackIndex < m_tracks.size() && trackIndex >= 0);
	m_tracks[trackIndex]->Hide();
	ManumalEmitSelectedChanged();
}

void TimelineManager::_ShowTrack(int trackIndex)
{
	Q_ASSERT(trackIndex < m_tracks.size() && trackIndex >= 0);
	m_tracks[trackIndex]->Show();
	ManumalEmitSelectedChanged();
}

void TimelineManager::_FreezeTrack(int trackIndex)
{
	Q_ASSERT(trackIndex < m_tracks.size() && trackIndex >= 0);
	m_tracks[trackIndex]->Freeze();
	ManumalEmitSelectedChanged();
}

void TimelineManager::_UnfreezeTrack(int trackIndex)
{
	Q_ASSERT(trackIndex < m_tracks.size() && trackIndex >= 0);
	m_tracks[trackIndex]->Unfreeze();
	ManumalEmitSelectedChanged();
}

float TimelineManager::_ChangeSpeed(ClipPtr clip, int newStart, int newLen)
{
	if (!clip->GetNLETimeline()->IsRenderScaleEnable())
		clip->GetNLETimeline()->SetRenderScaleEnable(true);

	int OrinLen = 1.0 * clip->GetLenAtTrack() / clip->GetNLETimeline()->GetRenderScale();

	float newScale = 1.0 *  OrinLen / newLen;
	_ChangeSpeedStandar(clip, newStart, newScale);
	return newScale;
}

void TimelineManager::_ChangeSpeedStandar(ClipPtr clip, int start, float scale)
{
	float oldScale = clip->GetNLETimeline()->GetRenderScale();

	float realScale = 1.0 / scale;
	clip->GetNLETimeline()->SetRenderScale(realScale);
	clip->SetStartAtTrack(start);
	clip->GetNLETimeline()->SetPosition(start);
	AudioWaveMgr::GetInstance()->Add(clip->GetNLETimeline());


	auto markers = clip->GetNLETimeline()->GetMarkers();
	for (int i = 0; i < markers.size(); ++i)
	{
		markers[i] *= (realScale / oldScale);
	}
	clip->GetNLETimeline()->SetMarkers(markers);
}

int TimelineManager::_SetSpeedScale(ClipPtr clip, float scale)
{
	Clip *nextClip = clip->NextClip();
	int nextPos = nextClip ? nextClip->GetStartAtTrack() : INT_MAX;
	_ChangeSpeedStandar(clip, clip->GetStartAtTrack(), scale);

	int trimlen = qMax(clip->GetEndAtTrack() - nextPos + 1, 0);
	if (trimlen > 0)
		m_pRootTimeline->SetClipRange(clip->GetNLETimeline(), clip->GetNLETimeline()->GetTrimin(), clip->GetNLETimeline()->GetTrimout() - trimlen);
	return trimlen;
}

void TimelineManager::_UnsetSpeedScale(ClipPtr clip, float scale, int trimlen)
{
	if (trimlen > 0)
		m_pRootTimeline->SetClipRange(clip->GetNLETimeline(), clip->GetNLETimeline()->GetTrimin(), clip->GetNLETimeline()->GetTrimout() + trimlen);
	_ChangeSpeedStandar(clip, clip->GetStartAtTrack(), scale);
}

ClipPtr TimelineManager::GetClipPtrFromClip(Clip *clip)
{
	ClipPtr clipPtr;
	for (auto track : m_tracks)
	{
		auto clipsPtr = track->GetClipsPtr();
		for (auto temp : clipsPtr)
		{
			if (temp.get() == clip)
			{
				clipPtr = temp;
				break;
			}
		}
	}

	Q_ASSERT(clipPtr);
	return clipPtr;
}

ClipPtr TimelineManager::GetClipPtrWithTimeline(FSNLETimeline *timeline)
{
	ClipPtr clipPtr;
	for (auto track : m_tracks)
	{
		auto clipsPtr = track->GetClipsPtr();
		for (auto ptr : clipsPtr)
		{
			if (ptr->GetNLETimeline() == timeline)
			{
				clipPtr = ptr;
				break;
			}
		}
	}
	Q_ASSERT(clipPtr);
	return clipPtr;
}

void TimelineManager::RemoveTrack(int trackIndex)
{
	INVALID_TIMELINEINDEX(trackIndex, m_tracks);

	BeginComposite();
	PushCommand(new RemoveTrackCommand(this, m_tracks[trackIndex]));
	PushCommand(new TrackRebuildCommand(this));
	EndComposite();
}

void TimelineManager::RemoveAllEmptyTrack()
{
	BeginComposite();
	for (auto track : GetTracks())
	{
		if (track->GetClips().isEmpty())
		{
			RemoveTrack(track->GetIndex());

			if (GetTrackCount() == 1)
				break;
		}
	}
	EndComposite();
}

void TimelineManager::RemoveTailEmptyTrack()
{
	BeginComposite();
	auto tracks = GetTracks();
	QList<int> lstEmpty;
	for (int i = tracks.size() - 1; i >= 1; i--)
	{
		if (tracks[i]->IsEmpty())
		{
			lstEmpty.push_back(i);
		}
		else
		{
			break;
		}
	}
	for (auto index : lstEmpty)
		RemoveTrack(index);
	EndComposite();
}

bool TimelineManager::HasEmptyTrack()
{
	if (GetTrackCount() == 1)
	{
		return false;
	}

	for (auto track : GetTracks())
	{
		if (track->GetClips().isEmpty())
			return true;
	}
	return false;
}

int TimelineManager::GetDuration()
{
	int duration = 0;
	for (auto track : GetTracks())
	{
		duration = qMax(duration, track->GetDuration());
	}
	return duration;
}

bool TimelineManager::CanUndo()
{
	return m_UndoStack->canUndo();
}

bool TimelineManager::CanRedo()
{
	
	return m_UndoStack->canRedo();

}

void TimelineManager::Undo()
{
	m_UndoStack->undo();
}

void TimelineManager::Redo()
{
	m_UndoStack->redo();
}

void TimelineManager::BeginComposite()
{
	m_UndoStack->beginMacro("UndoComposite");
}

void TimelineManager::EndComposite()
{
	m_UndoStack->endMacro();
}


void TimelineManager::SetCurrentFrames(int frames)
{
	m_curFrames = frames;
}

int TimelineManager::GetCurrentFrames()
{
	return m_curFrames;
}


PlayerEditorManager* TimelineManager::GetPlayerEditManager()
{
	return m_playerEditManager;
}

TitleBarManager* TimelineManager::GetTitleBarManager()
{
	return m_pTitleBarManager;
}

void TimelineManager::SetDisplayName(QString ancestralGuid, QString displayName)
{
	auto clips = GetClips();
	for (auto clip : clips)
	{
		if (clip->GetNLETimeline()->GetAncestralGuidString() == ancestralGuid)
		{
			clip->GetNLETimeline()->SetDisplayName(displayName);
		}
	}
	emit signalTrackUpdate();
}

void TimelineManager::HideTrack(int trackIndex)
{
	Q_ASSERT(trackIndex < m_tracks.size() && trackIndex >= 0);
	if (trackIndex >= m_tracks.size() || trackIndex < 0)
		return;

	auto selectClips = m_tracks[trackIndex]->GetSelected();
	for (auto clip : selectClips)
	{
		clip->SetSelected(false);
	}
	if (selectClips.size() > 0)
		ManumalEmitSelectedChanged();
	PushCommand(new HideTrackCommand(this, trackIndex));
}

void TimelineManager::ShowTrack(int trackIndex)
{
	Q_ASSERT(trackIndex < m_tracks.size() && trackIndex >= 0);
	if (trackIndex >= m_tracks.size() || trackIndex < 0)
		return;
	PushCommand(new ShowTrackCommand(this, trackIndex));
}

void TimelineManager::FreezeTrack(int trackIndex)
{
	Q_ASSERT(trackIndex < m_tracks.size() && trackIndex >= 0);
	if (trackIndex >= m_tracks.size() || trackIndex < 0)
		return;
	auto selectClips = m_tracks[trackIndex]->GetSelected();
	for (auto clip : selectClips)
	{
		clip->SetSelected(false);
	}
	if (selectClips.size() > 0)
		ManumalEmitSelectedChanged();
	PushCommand(new FreezeTrackCommand(this, trackIndex));
}

void TimelineManager::UnfreezeTrack(int trackIndex)
{
	Q_ASSERT(trackIndex < m_tracks.size() && trackIndex >= 0);
	if (trackIndex >= m_tracks.size() || trackIndex < 0)
		return;
	PushCommand(new UnfreezeTrackCommand(this, trackIndex));
}

void TimelineManager::SaveHidenFreezeTrack2RootTimeline()
{
	QList<int> hidens;
	QList<int> freezens;
	for (auto track : m_tracks)
	{
		if (track->IsHiden())
			hidens.push_back(track->GetIndex());

		if (track->IsFreeze())
			freezens.push_back(track->GetIndex());
	}
	GetRootTimeline()->SetHidenTracks(hidens);
	GetRootTimeline()->SetFreezenTracks(freezens);
}

void TimelineManager::RestoreHidenFreezeTrackFromRootTimeine(FSNLETimeline *srcRootTimeline)
{
	auto hidens = srcRootTimeline->GetHidenTracks();
	auto freezens = srcRootTimeline->GetFreezenTracks();
	for (auto track : m_tracks)
	{
		if (hidens.contains(track->GetIndex()))
			track->Hide();
		if (freezens.contains(track->GetIndex()))
			track->Freeze();
	}
}

bool TimelineManager::CanFreezeFrame(Clip *clip)
{
	if (!clip->GetSelected())
		return false;
	if (clip->GetNLETimeline()->GetType() != TimelineType_Video &&
		clip->GetNLETimeline()->GetType() != TimelineType_VideoAudio)
	{
		return false;
	}
	QRect rcClip(clip->GetStartAtTrack(), 0, clip->GetLenAtTrack(), 2);
	return rcClip.contains(m_curFrames, 0);
}

void TimelineManager::FreezeFrame(Clip* clip, int len/* = 2 * 25*/)
{
	int nAddFrames = len;
	auto track = clip->GetTimelineTrack();
	bool bNeedCreateNewTrack = false;
	QRect rcClip(clip->GetStartAtTrack(), 0, clip->GetLenAtTrack() + nAddFrames,10);
	for (auto c : track->GetClips())
	{
		if (c == clip) continue;
		if (rcClip.intersects(QRect(c->GetStartAtTrack(),0,c->GetLenAtTrack(),10)))
		{
			bNeedCreateNewTrack = true;
			break;
		}
	}

	BeginComposite();
	if (bNeedCreateNewTrack)
	{
		int newTrackIndex = clip->GetTimelineTrack()->GetIndex() + 1;
		InsertTrack(newTrackIndex);
		PushCommand(new FreezeFrameCommond(this, m_tracks[newTrackIndex], GetClipPtrFromClip(clip), m_curFrames, nAddFrames));
	}
	else
	{
		PushCommand(new FreezeFrameCommond(this, nullptr, GetClipPtrFromClip(clip), m_curFrames, nAddFrames));
	}
	EndComposite();
	AudioWaveMgr::GetInstance()->SetNeedReflesh(true);
}

QPoint TimelineManager::GetDragRange(Clip *clip, bool bLeftDrag)
{
	auto tlType = clip->GetNLETimeline()->GetType();
	if (tlType != TimelineType_Video && tlType != TimelineType_VideoAudio && tlType != TimelineType_Audio)
		return QPoint();

	int leftoffset = INT_MAX;
	int rightoffset = INT_MAX;
	if (bLeftDrag)
	{
		//往左拖动
		auto preClip = clip->PreClip();
		int leftMargin = preClip ? clip->GetStartAtTrack() - preClip->GetEndAtTrack() - 1 : clip->GetStartAtTrack();
		leftoffset = leftMargin; 
		//往右拖动
		rightoffset = clip->GetLenAtTrack() - 1;
	}
	else
	{
		//往左拖动
		leftoffset = clip->GetLenAtTrack() - 1;
		//往右拖动
		auto nextClip = clip->NextClip();
		int rightMargin = nextClip ? nextClip->GetStartAtTrack() - clip->GetEndAtTrack() - 1 : INT_MAX;
		rightoffset = rightMargin;
	}
	return QPoint(-leftoffset, rightoffset);
}

void TimelineManager::ChangeSpeed(Clip *clip, int newStart, int newLen)
{
	PushCommand(new ChangeSpeedCommand(this, GetClipPtrFromClip(clip), newStart, newLen));
}

void TimelineManager::SetSpeedScale(FSNLETimeline* timeline, float scale)
{
	auto clip = GetClipPtrWithTimeline(timeline);
	PushCommand(new SetSpeedScaleCommand(this,clip,scale));
}

void TimelineManager::SelectColorGroup(Clip *clip)
{
	if (!clip) return;
	auto clr = clip->GetNLETimeline()->GetBackgroundColor();

	Clips selecteds = GetSelected();
	Clips unselecteds = GetUnselected();
	bool bSelectChanged = false;
	for (auto c : selecteds)
	{
		if (c->GetNLETimeline()->GetBackgroundColor() != clr)
		{
			bSelectChanged = true;
			c->SetSelected(false);
		}
	}

	for (auto c : unselecteds)
	{
		if (c->GetNLETimeline()->GetBackgroundColor() == clr)
		{
			bSelectChanged = true;
			c->SetSelected(true);
		}
	}
	if (bSelectChanged) ManumalEmitSelectedChanged();
	emit signalTrackUpdate();
}

QColor TimelineManager::GetClipDefaultColor(Clip *clip)
{
	auto type = clip->GetNLETimeline()->GetType();
	for (auto key : m_defaultBkColor.keys())
	{
		if (key == type)
			return m_defaultBkColor[key];
	}
	return m_allBkColors.first().clr;
}

QColor TimelineManager::GetClipColor(QString name)
{
	for (auto clipClr : m_allBkColors)
	{
		if (clipClr.name == name)
		{
			return clipClr.clr;
		}
	}
	return QColor();
}

QList<ClipColor> TimelineManager::GetClipColorList()
{
	return m_allBkColors;
}

Clip* TimelineManager::GetClipWithTimline(FSNLETimeline* timeline)
{
	auto clipptr = GetClipPtrWithTimeline(timeline);
	return clipptr.get();
}

MarkPointMgr * TimelineManager::GetMarkPointMgr()
{
	return m_pMarkPointMgr;
}

ClipMarkerMgr * TimelineManager::GetClipMarkerMgr()
{
	return m_pClipMarkerMgr;
}

void TimelineManager::SetUndoLimit(int limit)
{
	m_UndoStack->setUndoLimit(qMax(0, limit));
}

//////////////////////////////////////////////////////////////////////////
PlayerEditorManager::PlayerEditorManager(TimelineManager *manager, QObject *parent)
: m_manager(manager)
{
	connect(m_manager, &TimelineManager::signalTrackUpdate, this, &PlayerEditorManager::signalTimelineUpdate);
	connect(m_manager, &TimelineManager::sigPause, this, &PlayerEditorManager::sigPause);
	connect(m_manager, &TimelineManager::signalSelectionChanged, this, [&](){
			emit signalSelectionChanged();
	});

}

void PlayerEditorManager::AddTimeline(FSNLETimeline *timeline)
{
	Q_ASSERT(timeline);

	m_manager->BeginComposite();
	int curFrames = m_manager->GetCurrentFrames();
	TimelineTrack *newTimelineTrack = nullptr;
	auto tracks = m_manager->GetTracks();
	for (int i = tracks.size() - 1; i >= 0; --i)
	{
		auto track = tracks[i];
		if (track->HintCollision(curFrames, timeline->GetPlayLen()))
		{
			break;
		}
		newTimelineTrack = track;
	}

	bool needCreateNewTrack = (newTimelineTrack == nullptr);
	if (needCreateNewTrack)
	{

		auto tempNewTrack = TimelineTrackPtr(new TimelineTrack());
		m_manager->PushCommand(new InsertTrackCommand(m_manager, m_manager->GetTrackCount(), tempNewTrack));
		newTimelineTrack = tempNewTrack.get();
		
	}

	m_manager->AddClip(newTimelineTrack->GetIndex(), m_manager->GetCurrentFrames(), timeline);

	if (needCreateNewTrack)
		m_manager->PushCommand(new TrackRebuildCommand(m_manager));

	m_manager->EndComposite();
}

void PlayerEditorManager::AddTimelines(QList<FSNLETimeline*> timelines)
{
	struct highleveltl
	{
		int rootIndex;
		int startPos;
		int level;
		FSNLETimeline *tl;
	};
	
	auto IsCollision = [](QList<highleveltl> lst, TimelineManager* manager)->bool
	{
		for (auto clipbase : lst)
		{
			auto track = manager->GetTrack(clipbase.level);
			if (track)
			{
				if (!track->IsEditable())
					return true;
				QRect clipRect(clipbase.startPos, 0, clipbase.tl->GetPlayLen(), 2);
				Clips clips = track->GetClips();
				for (auto trackClip : clips)
				{
					if (clipRect.intersects(QRect(trackClip->GetStartAtTrack(), 0, trackClip->GetLenAtTrack(), 2)))
						return true;
				}
			}
		}
		return false;
	};

	QList<highleveltl> tls;
	int nStartTrack = m_manager->GetTrackCount() - 1;
	for (int rootIndex = 0; rootIndex < timelines.size(); ++rootIndex)
	{
		auto rootTimeline = timelines[rootIndex];
		highleveltl hlt;
		hlt.rootIndex = rootIndex;
		if (rootTimeline->GetType() == TimelineType_Complex)
		{
			QList<NLEComPtr<INLETimelineBase>> timelines = NLEWraper::GetInstance().GetPlayerMaterProvider()->GetSubTimelines(rootTimeline->GetSrc());
			FSNLETimeline *pCameraTl = nullptr;
			FSNLETimeline *pDesktopGameTl = nullptr;
			for (int i = 0; i < timelines.size(); ++i)
			{
				auto tl = new FSNLETimeline(timelines[i]);
				tl->SetType(tl->GetNLETimelineType());
				tl->SetDisplayName(rootTimeline->GetDisplayName());
				bool bDeskTop = tl->IsDesktopVideo();
				if (bDeskTop)
				{
					NLEComPtr<INLEMouseEffectManager> pINLEMouseEffectManager = nullptr;
					tl->GetSrc()->QueryInterface(IID_INLEMouseEffectManager, (void**)&pINLEMouseEffectManager);
					if (pINLEMouseEffectManager)
					{
						pINLEMouseEffectManager->SetOpacity(1.0);
						pINLEMouseEffectManager->SetMouseScale(1.0);
					}

					NLEComPtr<INLEKeyboardStreamManager> pINLEKeyboardStreamManager = nullptr;
					tl->GetSrc()->QueryInterface(IID_INLEKeyBoardStreamManager, (void**)&pINLEKeyboardStreamManager);
					if (pINLEKeyboardStreamManager)
					{
						pINLEKeyboardStreamManager->CreateKeyboardCaptionTimeline();
						pINLEKeyboardStreamManager->PrepareKeyboardMarkers();
						int index;
						HRESULT hr = E_FAIL;
						QList<int> markers;
						do
						{
							hr = pINLEKeyboardStreamManager->GetNextMarkerPosition(index);
							if (SUCCEEDED(hr))
								markers.push_back(index);
						} while (SUCCEEDED(hr));
						tl->SetMarkers(markers);
					}
				}
				bool bGame = tl->IsGameVideo();
				if (bDeskTop || bGame) pDesktopGameTl = tl;
				bool bCamera = tl->IsCameraVideo();
				if (bCamera) pCameraTl = tl;
				if (pCameraTl && pDesktopGameTl)
					pCameraTl->SetDependency(pDesktopGameTl);
				hlt.level = nStartTrack + i;
				hlt.tl = tl;
				tls.push_back(hlt);
			}
		}
		else
		{
			hlt.level = nStartTrack;
			hlt.tl = rootTimeline;
			tls.push_back(hlt);
		}
	}
	int startPos = m_manager->GetCurrentFrames();
	for (int i = 0; i < tls.size(); ++i)
	{
		if (tls[i].level == nStartTrack)
		{
			tls[i].startPos = startPos;
			startPos += tls[i].tl->GetPlayLen();
		}
	}
	auto GetStartPos = [](QList<highleveltl> theList, int rootIndex)->int
	{
		for (auto l : theList)
		{
			if (l.rootIndex == rootIndex)
				return l.startPos;
		}
		return 0;
	};

	for (int i = 0; i < tls.size(); ++i)
	{
		if (tls[i].level != nStartTrack)
		{
			tls[i].startPos = GetStartPos(tls, tls[i].rootIndex);
		}
	}


	for (int i = nStartTrack; i >= 0; i--)
	{
		if (IsCollision(tls, m_manager))
			break;

		for (int j = 0; j < tls.size(); ++j)
			tls[j].level--;
	}

	for (int j = 0; j < tls.size(); ++j)
		tls[j].level++;

	m_manager->UnselectAll();
	m_manager->BeginComposite();
	int buildTrackCnt = tls.last().level - (m_manager->GetTrackCount() - 1);
	if (buildTrackCnt > 0)
	{
		for (int i = 0; i < buildTrackCnt; ++i)
		{
			int index = TimelineManager::GetInstance()->GetTrackCount();
			TimelineManager::GetInstance()->InsertTrack(index);
		}
	}
	for (int i = 0; i < tls.size(); ++i)
	{
		m_manager->AddClip(tls[i].level, tls[i].startPos, tls[i].tl);
	}

	auto GetTopLeftTimeline = [&](QList<highleveltl> theList)->FSNLETimeline *{
		if (theList.isEmpty()) return nullptr;
		auto lastOne = theList.last();
		for (auto one : theList)
		{
			if (one.level == lastOne.level)
				return one.tl;
		}
		return nullptr;
	};
	auto TopLeftTl = GetTopLeftTimeline(tls);
	m_manager->SetShiftSelectBaseClip(m_manager->GetClipWithTimline(TopLeftTl));
	m_manager->EndComposite();
}

void PlayerEditorManager::RemoveSelected()
{
	auto clips = m_manager->GetSelected(m_manager->GetCurrentFrames());
	if (!clips.isEmpty())
	{
		m_manager->BeginComposite();
		for (auto clip : clips)
		{
			m_manager->RemoveClip(clip);
		}
		m_manager->EndComposite();
	}

}

void PlayerEditorManager::AddMouseFilter(FSNLETimeline *tl, FSNLETimeline *filter)
{
	auto clip = m_manager->GetClipWithTimline(tl);
	m_manager->AddMouseFilter(clip, filter);
}

void PlayerEditorManager::AddMouseSound(FSNLETimeline *tl, QString soundPath)
{
	auto clip = m_manager->GetClipWithTimline(tl);
	m_manager->AddMouseSound(clip,soundPath);
}

QList<FSNLETimeline*> PlayerEditorManager::GetTimelines()
{
	QList<FSNLETimeline*> selectClips;
	auto clips = m_manager->GetEditableClips(m_manager->GetCurrentFrames());
	for (auto clip : clips)
	{
		selectClips.push_back(clip->GetNLETimeline());
	}
	return selectClips;
}

QList<FSNLETimeline*> PlayerEditorManager::GetSelected()
{
	QList<FSNLETimeline*> selectClips;
	auto clips = m_manager->GetSelected(m_manager->GetCurrentFrames());
	for (auto clip : clips)
	{
		selectClips.push_back(clip->GetNLETimeline());
	}
	return selectClips;
}

QList<FSNLETimeline*> PlayerEditorManager::GetOtherSelected()
{
	QList<FSNLETimeline*> selectClips;
	auto allClips = m_manager->GetSelected();
	auto clips = m_manager->GetSelected(m_manager->GetCurrentFrames());
	for (auto temp : allClips)
	{
		if (!clips.contains(temp))
		{
			selectClips.push_back(temp->GetNLETimeline());
		}
	}
	return selectClips;
}

void PlayerEditorManager::SetSelected(QList<FSNLETimeline*> timelines)
{
	m_manager->MultiSelect(timelines);
}

void PlayerEditorManager::UnselectAll()
{
	m_manager->UnselectAll();
}

bool PlayerEditorManager::HasTimelines()
{
	return m_manager->GetRootTimeline()->GetClipCount() > 0;
}

void PlayerEditorManager::SetDisplayName(QString ancestralGuid, QString displayName)
{
	m_manager->SetDisplayName(ancestralGuid, displayName);
}

void PlayerEditorManager::SetShiftSelectBaseTimeline(FSNLETimeline* timeline)
{
	m_manager->SetShiftSelectBaseClip(m_manager->GetClipWithTimline(timeline));
}

void PlayerEditorManager::Undo()
{
	m_manager->Undo();
}

void PlayerEditorManager::Redo()
{
	m_manager->Redo();

}

void PlayerEditorManager::PushCommand(UndoCommand *pCommond)
{
	m_manager->PushCommand(pCommond);
}

void PlayerEditorManager::BeginComposite()
{
	m_manager->BeginComposite();

}

void PlayerEditorManager::EndComposite()
{
	m_manager->EndComposite();

}

//////////////////////////////////////////////////////////////////////////
TitleBarManager::TitleBarManager(TimelineManager *manager, QObject *parent)
{
	m_pTimelineMgr = manager;
}

bool TitleBarManager::CanUndo()
{
	return m_pTimelineMgr->CanUndo();
}

void TitleBarManager::Undo()
{
	m_pTimelineMgr->Undo();
}

bool TitleBarManager::CanRedo()
{
	return  m_pTimelineMgr->CanRedo();
}

void TitleBarManager::Redo()
{
	m_pTimelineMgr->Redo();
}

bool TitleBarManager::CanCut()
{
	return m_pTimelineMgr->CanCut();
}

void TitleBarManager::Cut()
{
	m_pTimelineMgr->Cut();
}

bool TitleBarManager::CanCopy()
{
	return m_pTimelineMgr->CanCopy();
}

void TitleBarManager::Copy()
{
	m_pTimelineMgr->Copy();
}

bool TitleBarManager::CanPaste()
{
	return m_pTimelineMgr->CanPaste();
}

void TitleBarManager::Paste()
{
	m_pTimelineMgr->Paste();
}

bool TitleBarManager::CanDelete()
{
	return m_pTimelineMgr->CanRemove();
}

void TitleBarManager::Delete()
{
	auto Tlmark = m_pTimelineMgr->GetMarkPointMgr()->GetSelected();
	if (Tlmark)
	{
		m_pTimelineMgr->GetMarkPointMgr()->Remove(Tlmark);
	}
	else 
	{
		bool bDeleteClipMark = false;
		auto selectedClips = m_pTimelineMgr->GetSelected();
		for (auto clip : selectedClips)
		{
			auto marker = clip->GetNLETimeline()->GetSelectedMarker();
			if (marker != -1)
			{
				bDeleteClipMark = true;
				m_pTimelineMgr->GetClipMarkerMgr()->Remove(clip->GetNLETimeline(), marker + (clip->GetNLETimeline()->GetPosition() - clip->GetNLETimeline()->GetTrimin()));
			}
		}
		if (!bDeleteClipMark)
			m_pTimelineMgr->RemoveClips();
	}
}
	

//////////////////////////////////////////////////////////////////////////
MarkPointMgr::MarkPointMgr(TimelineManager *manager, QObject *parent)
	:QObject(parent)
	, m_manager(manager)
{

}

void MarkPointMgr::InitMarkPoints(QList<MarkPointPtr> mps)
{
	m_markPoints = mps;
}

QList<MarkPointPtr> MarkPointMgr::GetMarkPoints()
{
	return m_markPoints;
}

void MarkPointMgr::Add(int frame)
{
	for (auto mp : m_markPoints)
	{
		if (mp->GetFrame() == frame)
			return;
	}
	m_manager->PushCommand(new AddTLMarkCommond(m_manager, frame));
}

void MarkPointMgr::Remove(MarkPoint* mp)
{
	m_manager->PushCommand(new RemoveTLMarkCommond(m_manager, Pt2Ptr(mp)));
}

void MarkPointMgr::RemoveAll()
{
	m_manager->BeginComposite();
	for (auto mp : m_markPoints)
		Remove(mp.get());
	m_manager->EndComposite();
}

void MarkPointMgr::Move(MarkPoint *mp, int newframe)
{
	m_manager->PushCommand(new MoveTLMarkCommond(m_manager, Pt2Ptr(mp),newframe));
}

void MarkPointMgr::SetColor(MarkPoint* mp, QColor clr)
{
	mp->SetColor(clr);
	emit sigUpdate();
}

void MarkPointMgr::SetSelected(MarkPoint *mp, bool bSelected)
{
	auto preSelect = GetSelected();
	if (preSelect == mp)
		return;
	if(preSelect) preSelect->SetSelected(false);
	mp->SetSelected(true);
	emit sigUpdate();
}

void MarkPointMgr::UnselectAll()
{
	auto preSelect = GetSelected();
	if (preSelect)
	{
		preSelect->SetSelected(false);
		emit sigUpdate();
	}
}

MarkPoint* MarkPointMgr::GetSelected()
{
	for(auto ptr : m_markPoints)
	{
		if (ptr->IsSelected())
		{
			return ptr.get();
		}
	}
	return nullptr;
}

void MarkPointMgr::Clear()
{
	m_markPoints.clear();
}

MarkPointPtr MarkPointMgr::Pt2Ptr(MarkPoint *mp)
{
	for (auto ptr : m_markPoints)
	{
		if (ptr.get() == mp)
		{
			return ptr;
		}
	}
	return MarkPointPtr();
}

MarkPointPtr MarkPointMgr::_Add(int frame)
{
	auto mp = MarkPointPtr(new MarkPoint(frame, QColor(255, 255, 255)));
	m_markPoints.push_back(mp);
	m_manager->GetRootTimeline()->UpdateTrackMarks(m_markPoints);
	emit sigUpdate();
	return mp;
}

void MarkPointMgr::_Add(MarkPointPtr mp)
{
	m_markPoints.push_back(mp);
	m_manager->GetRootTimeline()->UpdateTrackMarks(m_markPoints);
	emit sigUpdate();
}

void MarkPointMgr::_Remove(MarkPointPtr mp)
{
	mp->SetSelected(false);
	m_markPoints.removeOne(mp);
	m_manager->GetRootTimeline()->UpdateTrackMarks(m_markPoints);
	emit sigUpdate();
}

void MarkPointMgr::_Move(MarkPointPtr mp, int pos)
{
	if (mp)
		mp->SetFrame(pos);
	m_manager->GetRootTimeline()->UpdateTrackMarks(m_markPoints);
	emit sigUpdate();
}

//////////////////////////////////////////////////////////////////////////
ClipMarkerMgr::ClipMarkerMgr(TimelineManager *manager, QObject *parent)
	:QObject(parent)
	, m_manager(manager)
{

}

void ClipMarkerMgr::Add(FSNLETimeline*tl, int frame)
{
	if (!tl) return;
	if (!tl->CanAddMarker()) return;
	if (tl->GetMarkers().contains(frame))
		return;
	m_manager->PushCommand(new AddClipMarkerCommond(m_manager, tl, frame));
}

void ClipMarkerMgr::Remove(FSNLETimeline*tl, int frame)
{
	if (!tl) return;
	if (!tl->GetMarkers().contains(tl->TrackMarkPos2ClipMarkPos(frame))); 
	m_manager->PushCommand(new RemoveClipMarkerCommond(m_manager, tl, frame));
}

void ClipMarkerMgr::RemoveAll(FSNLETimeline*tl)
{
	if (!tl) return;
	m_manager->BeginComposite();
	for (auto mk : tl->GetMarkers())
		Remove(tl, tl->ClipMarkPos2TrackMarkPos(mk));
	m_manager->EndComposite();
}

void ClipMarkerMgr::SetSelected(FSNLETimeline*tl, int frame, bool bSelected)
{
	if (!tl) return;
	tl->SelectMarker(frame, bSelected);
	emit m_manager->signalTrackUpdate();
}

void ClipMarkerMgr::UnselectAll(FSNLETimeline*tl)
{
	if (!tl) return;
	if (tl->GetSelectedMarker() != -1)
	{
		tl->UnselectMarker();
		emit m_manager->signalTrackUpdate();
	}
}

void ClipMarkerMgr::UnselectAll()
{
	auto clips = m_manager->GetClips();
	for (auto clip : clips)
	{
		clip->GetNLETimeline()->UnselectMarker();
	}
	emit m_manager->signalTrackUpdate();
}

bool ClipMarkerMgr::CanAddMarker(FSNLETimeline* tl, int frame)
{
	if (tl->GetType() != TimelineType_Video &&
		tl->GetType() != TimelineType_Audio &&
		tl->GetType() != TimelineType_VideoAudio)
	{
		return false;
	}

	if (frame < tl->GetPosition() || frame > tl->GetPosition() + tl->GetPlayLen())
		return false;

	return !tl->GetMarkers().contains(frame);
}

