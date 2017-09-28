#include "stdafx.h"
#include "TVControl.h"
#include "TimelineClip.h"
#include "TVTrackView.h"
#include "inc_FSCommonlib/FSNLETimeline.h"
#include "inc_CommonLib/NLEWraper.h"
#include "inc_CommonLib/PlayerMaterProvider.h"
#include "INLEMouseEffectManager.h"
#include "INLEKeyboardStreamManager.h"
#include "TimelineViewPrivate.h"
#include "AudioWaveMgr.h"

UIDragClips::UIDragClips()
	:m_nDragType(DragType_None)
{

}

void UIDragClips::SetDragType(int nType)
{
	m_nDragType = nType;
}

int UIDragClips::GetDragType()
{
	return m_nDragType;
}

void UIDragClips::SetClipList(DragClipList lst)
{
	m_lstDagClips = lst;
}

DragClipList UIDragClips::GetClipsList()
{
	return m_lstDagClips;
}

bool UIDragClips::IsNull()
{
	return m_lstDagClips.isEmpty();
}


int UIDragClips::GetTopestIndex()
{
	if (m_lstDagClips.isEmpty())
		return -1;
	int nMax = 0;
	for (auto clip : m_lstDagClips)
	{
		nMax = qMax(nMax, clip.trackIndex);
	}
	return nMax;
}

int UIDragClips::GetLowestIndex()
{
	if (m_lstDagClips.isEmpty())
		return -1;
	int nMin = 65535;
	for (auto clip : m_lstDagClips)
	{
		nMin = qMin(nMin, clip.trackIndex);
	}
	return nMin;
}

int UIDragClips::GetTrackCnt()
{
	if (m_lstDagClips.isEmpty())
		return 0;
	int nMin = 65535;
	int nMax = 0;
	for (auto clip : m_lstDagClips)
	{
		nMin = qMin(nMin, clip.trackIndex);
		nMax = qMax(nMax, clip.trackIndex);
	}
	return nMax - nMin + 1;
}

void UIDragClips::Clear()
{
	//m_nDragType = DragType_None;
	m_lstDagClips.clear();
}

UIDragClips& UIDragClips::operator=(const UIDragClips &other)
{
	if (&other != this)
	{
		m_lstDagClips = other.m_lstDagClips;
		m_nDragType = other.m_nDragType;
		m_ptStartDrag = other.m_ptStartDrag;
	}
	return *this;
}

QPoint RealPoint(QPoint p)
{
	return QPoint(p.x() - TRACK_WIDGET_HEADER_WIDTH, p.y());
}
//////////////////////////////////////////////////////////////////////////
TVDragControl::TVDragControl(QObject *parent)
	: QObject(parent)
{
	m_bDraging = false;
	m_pTimelineMgr = TimelineManager::GetInstance();
	//m_nAsorbPos = -1;
	m_nDraginTrackIndex = 0;
	m_dragTrackIndex = 0;
	m_pTrackAssitor = new VirtualTrackAssistor(this);
}

TimelineRuler* TVDragControl::Ruler()
{
	return TrackBuilder::GetInstance()->GetRuler();
}

TVDragControl* TVDragControl::GetInstance()
{
	static TVDragControl ins;
	return &ins;
}

void TVDragControl::Init(TVTrackView *pTrackView)
{
	m_pTrackView = pTrackView;
}

bool TVDragControl::IsCollision(DragClipList lst, bool bReject)
{
	Clips lst2Reject;
	if (bReject)
	{
		for (auto clipbase : lst)
			lst2Reject.push_back(clipbase.clip);
	}
	for (auto clipbase : lst)
	{
		int nTrackIndex = clipbase.trackIndex;
		int nFrame = clipbase.startPos;

		auto track = m_pTimelineMgr->GetTrack(nTrackIndex);
		if (track)
		{
			if (!track->IsEditable())
				return true;
			Clips clips = bReject ? track->GetRejectClips(lst2Reject) : track->GetClips();
			QRect clipRect(nFrame, 0, clipbase.length, 2);
			for (auto trackClip : clips)
			{
				if (clipRect.intersects(QRect(trackClip->GetStartAtTrack(), 0, trackClip->GetLenAtTrack(), 2)))
				{
					return true;
				}
			}
		}
	}
	return false;
}

DragClipList TVDragControl::MatterViewList2DragList(QList<FSNLETimeline*> lst,int startPos,int startTrack)
{
	Q_ASSERT(!lst.isEmpty());
	DragClipList list;

	struct highleveltl
	{
		int rootIndex;
		int startPos;
		int level;
		FSNLETimeline *tl;
	};
	QList<highleveltl> tls;

	for (int rootIndex = 0; rootIndex < lst.size(); ++rootIndex)
	{
		auto rootTimeline = lst[rootIndex];
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
				hlt.level = startTrack + i;
				hlt.tl = tl;
				tls.push_back(hlt);
			}
		}
		else
		{
			hlt.level = startTrack;
			hlt.tl = rootTimeline;
			tls.push_back(hlt);
		}
		
	}
	for (int i = 0; i < tls.size(); ++i)
	{
		if (tls[i].level == startTrack)
		{
			DragClipBase pClipBase;
			pClipBase.trackIndex = tls[i].level;
			pClipBase.startPos = startPos;
			tls[i].startPos = startPos;
			pClipBase.length = tls[i].tl->GetPlayLen();
			pClipBase.timeline = tls[i].tl;
			startPos += pClipBase.length;

			list.push_back(pClipBase);
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
		if (tls[i].level != startTrack)
		{
			DragClipBase pClipBase;
			pClipBase.trackIndex = tls[i].level;
			pClipBase.startPos = GetStartPos(tls, tls[i].rootIndex);
			tls[i].startPos = pClipBase.startPos;
			pClipBase.length = tls[i].tl->GetPlayLen();
			pClipBase.timeline = tls[i].tl;
			list.push_back(pClipBase);
		}
	}
	bool bCollision = IsCollision(list, false);

	int nNeedCreateTrackCnt = bCollision ? (list.last().trackIndex - list.first().trackIndex + 1) : (list.last().trackIndex - m_pTimelineMgr->GetTrackCount() + 1);
	m_pTrackAssitor->CreateTracks(nNeedCreateTrackCnt);
	if (bCollision)
	{
		int minTrackIndex = INT_MAX;
		for (auto dragClip : list)
			minTrackIndex = qMin(dragClip.trackIndex, minTrackIndex);
		int nDelta = m_pTimelineMgr->GetTrackCount() - nNeedCreateTrackCnt - minTrackIndex;
		for (int i = 0; i < list.size(); ++i)
			list[i].trackIndex += nDelta;
	}
	
	return list;
}

DragClipList TVDragControl::Selected2DragList()
{
	auto selectedClips = m_pTimelineMgr->GetSelected();
	Q_ASSERT(!selectedClips.isEmpty());
	DragClipList list;
	for (auto clip : selectedClips)
	{
		DragClipBase dragClip;
		dragClip.startPos = clip->GetStartAtTrack();
		dragClip.length = clip->GetLenAtTrack();
		dragClip.trackIndex = clip->GetTimelineTrack()->GetIndex();
		dragClip.clip = clip;
		list.push_back(dragClip);
	}
	return list;
}

bool TVDragControl::DoDragMove(int frameOffset, int trackOffset)
{
	auto clips = m_dragInClips.GetClipsList();
	auto copyClips = clips;
	
	for (int i = 0; i < copyClips.size(); ++i)
	{
		copyClips[i].startPos += frameOffset;
		copyClips[i].trackIndex += trackOffset;
	}
	//到顶了，就不能再上去了
	int offsetY = copyClips.last().trackIndex - (m_pTimelineMgr->GetTrackCount() - 1);
	if (offsetY > 0 && m_pTrackAssitor->GetCreatedCnt() >= m_dragInClips.GetTrackCnt())
	{
		for (int i = 0; i < copyClips.size(); ++i)
			copyClips[i].trackIndex -= offsetY;
	}
	//到底了，就不能再下去了
	offsetY = 0 - copyClips.first().trackIndex;
	if (offsetY > 0)
	{
		for (int i = 0; i < copyClips.size(); ++i)
			copyClips[i].trackIndex += offsetY;
	}

	//到最左边了，就不能再过去了
	int offsetX = 0 - GetMostLeftPos(copyClips);
	if (offsetX > 0)
	{
		for (int i = 0; i < copyClips.size(); ++i)
			copyClips[i].startPos += offsetX;
	}

	if (IsCollision(copyClips, true))
	{
		if (!FindGoodPos(m_preDropableDragInClips.GetClipsList(), copyClips))
		{
			m_dragInClips = m_preDropableDragInClips;
			return false;
		}
	}
		
	int nNeedCreateTrackCnt = copyClips.last().trackIndex - m_pTimelineMgr->GetTrackCount() + 1;

	m_pTrackAssitor->CreateTracks(qMin(nNeedCreateTrackCnt,m_dragInClips.GetTrackCnt()- m_pTrackAssitor->GetCreatedCnt()));

	

	//吸附功能
	DoAdsorbtion(copyClips);

	//修改clips的数据
	m_dragInClips.SetClipList(copyClips);
	m_preDropableDragInClips = m_dragInClips;

	//qDebug() << "dragClip Startpos: " << m_dragInClips.GetClipsList()[0].startPos;
	return true;
}

int TVDragControl::GetMostLeftPos(DragClipList lst)
{
	int minX = INT_MAX;
	for (auto dragClip : lst)
	{
		minX = qMin(minX, dragClip.startPos);
	}
	return minX;
}

bool TVDragControl::FindGoodPos(DragClipList preGoodList, DragClipList &notGoodList)
{
	Q_ASSERT(preGoodList.size() > 0 && notGoodList.size() > 0);
	int xoffset = notGoodList[0].startPos - preGoodList[0].startPos;
	int yoffset = notGoodList[0].trackIndex - preGoodList[0].trackIndex;

	for (int i = 0; i < notGoodList.size(); ++i)
		notGoodList[i].trackIndex -= yoffset;

	for (int frame = 0; frame < qAbs(xoffset); frame++)
	{
		for (int i = 0; i < notGoodList.size(); ++i)
		{
			xoffset > 0 ? notGoodList[i].startPos-- : notGoodList[i].startPos++;
		}
		if (!IsCollision(notGoodList, true))
			return true;
	}
	return false;
}

DragClipBase TVDragControl::GetHitClip(Clip *clip)
{
	for (auto dragClip : m_dragInClips.GetClipsList())
	{
		if (dragClip.clip == clip)
			return dragClip;
	}
	return DragClipBase();
}



static Clips GetRejectClips(Clips allClips, DragClipList rejectClips)
{
	Clips rejects;
	for (auto reject : rejectClips)
		rejects.push_back(reject.clip);
	Clips res;
	for (auto clip : allClips)
	{
		if (!rejects.contains(clip))
			res.push_back(clip);
	}
	return res;
}

static QList<int> GetKeyPoints(DragClipList rejectLst)
{
	QList<int> keyPoints;
	keyPoints.push_back(0);
	keyPoints.push_back(TimelineManager::GetInstance()->GetCurrentFrames());
	for (auto mp : TimelineManager::GetInstance()->GetMarkPointMgr()->GetMarkPoints())
	{
		keyPoints.push_back(mp->GetFrame());
	}

	auto clips = GetRejectClips(TimelineManager::GetInstance()->GetClips(), rejectLst);
	for (auto clip : clips)
	{
		keyPoints.push_back(clip->GetStartAtTrack());
		keyPoints.push_back(clip->GetEndAtTrack() + 1);
		keyPoints += clip->GetNLETimeline()->GetTrackMarkPos();
	}
	return keyPoints;
}

void TVDragControl::DoAdsorbtion(DragClipList &lst)
{
	m_lstAdsorbPoints.clear();
	if (m_hitClip.IsNull())
		return;

	for (auto clip : lst)
		if (m_hitClip.clip == clip.clip)
			m_hitClip = clip;

	QList<int> keyPoints = GetKeyPoints(DragClipList()/* m_dragInClips.GetClipsList()*/);
	int adsrobDistance = Ruler()->Duration2FramesF(10);

	int minDuration = INT_MAX;
	bool bReduse = true;
	bool bStart = true;
	int start = m_hitClip.startPos;
	int end = m_hitClip.startPos + m_hitClip.length;

	for (auto keyPt : keyPoints)
	{
		int startD = start - keyPt;
		int endD = end - keyPt;
		int minPre = minDuration;
		minDuration = qMin(minDuration, qMin(qAbs(startD), qAbs(endD)));
		if ((minDuration <= adsrobDistance) && (minDuration != minPre))
		{
			bReduse = qAbs(startD) < qAbs(endD) ? startD > 0 : endD > 0;
			bStart = qAbs(startD) < qAbs(endD);
			m_lstAdsorbPoints.clear();
			m_lstAdsorbPoints.push_back(keyPt);
		}
	}

	if (minDuration > adsrobDistance)
		return;
	DragClipList copyLst = lst;
	for (int i = 0; i < copyLst.size(); ++i)
		bReduse ? copyLst[i].startPos -= minDuration : copyLst[i].startPos += minDuration;
	if (IsCollision(copyLst, true))
	{
		m_lstAdsorbPoints.clear();
		return;
	}
	
	if (m_lstAdsorbPoints.size() > 0)
	{
		for (auto keyPt : keyPoints)
		{
			if (bStart)
			{
				auto newEnd = bReduse ? (end - minDuration) : (end + minDuration);
				if (newEnd == keyPt)
				{
					m_lstAdsorbPoints.push_back(keyPt);
					break;
				}
			}
			else
			{
				auto newStart = bReduse ? (start - minDuration) : (start + minDuration);
				if (newStart == keyPt)
				{
					m_lstAdsorbPoints.push_back(newStart);
					break;
				}
			}
		}
	}
	lst = copyLst;
}

void TVDragControl::DragEnter(QDragEnterEvent *e)
{
	if (IsDraging())
	{
		StopFilter();
		e->accept();
		return;
	}
	
	//qDebug() << "DragEnter";
	m_pTimelineMgr->BeginComposite();
	auto mimeData = e->mimeData();
	if (mimeData->hasFormat("MatterView_BaseItem"))
	{
		TrackWidget* child = qobject_cast<TrackWidget*>(m_pTrackView->childAt(e->pos()));
		TimelineManager::GetInstance()->UnselectAll();
		QList<FSNLETimeline*> *pList = reinterpret_cast<QList<FSNLETimeline*>*>(mimeData->property("MatterView_BaseItem").value<qptrdiff>());
		auto lstRes = MatterViewList2DragList(*pList,Ruler()->Position2FramesF(e->pos().x() - TRACK_WIDGET_HEADER_WIDTH),child ? child->GetTrackData()->GetIndex() : m_pTimelineMgr->GetTrackCount());
		m_dragInClips.SetDragType(UIDragClips::DragType_MatterView);
		m_dragInClips.SetClipList(lstRes);
		m_preDropableDragInClips = m_dragInClips;
		m_dragTrackIndex = m_dragInClips.GetLowestIndex();
		SetDragEnterPoint(e->pos());
		m_hitClip = m_dragInClips.GetClipsList().first();
	}
	else if (mimeData->hasFormat("TimelineView_ClipBase"))
	{
		auto lstRes = Selected2DragList();
		m_dragInClips.SetDragType(UIDragClips::DragType_Timeline);
		m_dragInClips.SetClipList(lstRes);
		m_dragTrackIndex = m_nDraginTrackIndex;
		TrackWidget* child = qobject_cast<TrackWidget*>(m_pTrackView->childAt(m_dragEnterPoint));
		Q_ASSERT(child);
		auto hitclip = child->GetTrackData()->GetClip(Ruler()->Position2FramesF(m_dragEnterPoint.x() - TRACK_WIDGET_HEADER_WIDTH));
		m_hitClip = GetHitClip(hitclip);
	}
	else
		Q_ASSERT(false);
	
	SetDraging(true);
	m_dragInClipsCopy = m_dragInClips;
	e->accept();
	TrackBuilder::GetInstance()->UpdateItems();
}

void TVDragControl::DragMove(QPoint curPoint)
{
	if (m_dragInClips.GetDragType() == UIDragClips::DragType_None)
	{
		SetDraging(false);
		m_pTimelineMgr->EndComposite();
		return;
	}
	//qDebug() << "DragMove";
	m_dragInClips = m_dragInClipsCopy;

	auto child = m_pTrackView->childAt(curPoint);
	TrackWidget* thisTrack = qobject_cast<TrackWidget*>(child);
	if (!thisTrack && child)
	{
		//隐藏和冻结按钮，是BaseStateButton,有两层widget
		thisTrack = qobject_cast<TrackWidget*>(child->parent()->parent());
	}

	int trackOffset = thisTrack ? thisTrack->GetTrackData()->GetIndex() - m_dragTrackIndex : m_pTimelineMgr->GetTrackCount() - m_dragTrackIndex;
	int frameOffset = Ruler()->Position2FramesF(curPoint.x()) - m_nDragEnterFrame;

	//qDebug() << "frameOffset" << frameOffset;
	
	DoDragMove(frameOffset, trackOffset);
	TrackBuilder::GetInstance()->UpdateItems();
}

void TVDragControl::DragLeave(QDragLeaveEvent *e)
{
	if (m_dragInClips.GetDragType() == UIDragClips::DragType_MatterView)
	{
		m_dragInClips.Clear();
		TrackBuilder::GetInstance()->UpdateItems();
	}
	StartFilter();
	e->accept();
}

void TVDragControl::DropDown()
{
	for (auto clip : m_dragInClips.GetClipsList())
	{
		if (m_dragInClips.GetDragType() == UIDragClips::DragType_MatterView)
		{
			auto tl = clip.timeline;
			if (tl->GetType() == TimelineType_VideoAudio || tl->GetType() == TimelineType_Audio)
			{
				AudioWaveMgr::GetInstance()->Add(tl);
			}
			auto res = m_pTimelineMgr->AddClip(clip.trackIndex, clip.startPos, tl);
		}
		else if (m_dragInClips.GetDragType() == UIDragClips::DragType_Timeline)
		{
			m_pTimelineMgr->MoveClip(clip.trackIndex, clip.startPos,clip.clip);
		}
	}

	if (m_dragInClips.GetDragType() == UIDragClips::DragType_MatterView)
	{
		if (!m_dragInClips.GetClipsList().isEmpty())
		{
			auto lastOne = m_dragInClips.GetClipsList().last();
			for (auto clip : m_dragInClips.GetClipsList())
			{
				if (clip.trackIndex == lastOne.trackIndex)
				{
					m_pTimelineMgr->SetShiftSelectBaseClip(m_pTimelineMgr->GetClipWithTimline(clip.timeline));
					break;
				}
			}
		}
		
	}

	SetDraging(false);
	m_dragInClips.Clear();
	m_dragInClipsCopy.Clear();
	m_pTrackAssitor->Clear();
	m_pTimelineMgr->RemoveTailEmptyTrack();
	m_pTimelineMgr->EndComposite();
	TrackBuilder::GetInstance()->UpdateItems();
}

void TVDragControl::SetDraging(bool bDraging)
{
	if (m_bDraging == bDraging)
		return;
	m_bDraging = bDraging;

	if (m_dragInClips.GetDragType() == UIDragClips::DragType_Timeline)
		ScrollMgr::GetInstance()->AutoScroll(m_bDraging);
}

bool TVDragControl::IsDraging()
{
	return m_bDraging;
}

void TVDragControl::SetDraginTrackIndex(int index)
{
	m_nDraginTrackIndex = index;
}

int TVDragControl::GetDraginTrackIndex()
{
	return m_nDraginTrackIndex;
}

UIDragClips TVDragControl::GetDraginUIClips()
{
	return m_dragInClips;
}

DragClipList TVDragControl::GetDragingClips(int trackIndex)
{
	DragClipList lst;
	auto allClips = m_dragInClips.GetClipsList();
	for (auto clip : allClips)
	{
		if (clip.trackIndex == trackIndex)
			lst.push_back(clip);
	}
	return lst;
}

void TVDragControl::SetViewportRect(QRect rcViewport)
{
	m_rcViewPortGlobal = rcViewport;
}

QRect TVDragControl::GetViewportRect()
{
	return m_rcViewPortGlobal;
}

void TVDragControl::SetDragEnterPoint(QPoint pt)
{
	m_dragEnterPoint = pt;
	m_nDragEnterFrame = Ruler()->Position2FramesF(m_dragEnterPoint.x());
}

QList<int> TVDragControl::GetAdsorbList()
{
	return m_lstAdsorbPoints;
}

void TVDragControl::StartFilter()
{
	qApp->installEventFilter(this);
}

void TVDragControl::StopFilter()
{
	qApp->removeEventFilter(this);
}


bool TVDragControl::eventFilter(QObject *watched, QEvent *event)
{
	if (event->type() == QEvent::MouseButtonRelease)
	{
		StopFilter();
		DropDown();
	}
	else if (event->type() == QEvent::DragMove)
	{
		//if (m_dragInClipsCopy.GetDragType() == UIDragClips::DragType_MatterView)
		//	return false;
		//QPoint cursorPos = QCursor::pos();
		//if (cursorPos.y() < m_rcViewPortGlobal.top())
		//	cursorPos.setY(m_rcViewPortGlobal.top() + 1);
		//else if (cursorPos.y() > m_rcViewPortGlobal.bottom())
		//	cursorPos.setY(m_rcViewPortGlobal.bottom()-1);

		//if (cursorPos.x() < m_rcViewPortGlobal.left())
		//	cursorPos.setX(m_rcViewPortGlobal.left() + 1);
		//else if (cursorPos.x() > m_rcViewPortGlobal.right())
		//	cursorPos.setX(m_rcViewPortGlobal.right() - 1);
		//DragMove(m_pTrackView->mapFromGlobal(cursorPos));
	}
	return false;
}

void TVDragControl::DropClear()
{

}

//////////////////////////////////////////////////////////////////////////
VirtualTrackAssistor::VirtualTrackAssistor(QObject *parent /*= 0*/)
	:QObject(parent)
{
	
}

void VirtualTrackAssistor::CreateTrack(int nIndex)
{
	m_lstCreatedTrack.push_back(nIndex);
	TimelineManager::GetInstance()->InsertTrack(nIndex);
}

void VirtualTrackAssistor::CreateTracks(int nCnt)
{
	for (int i = 0; i < nCnt; ++i)
	{
		int index = TimelineManager::GetInstance()->GetTrackCount();
		CreateTrack(index);
	}
}

int VirtualTrackAssistor::GetCreatedCnt()
{
	return m_lstCreatedTrack.size();
}

void VirtualTrackAssistor::Clear()
{
	m_lstCreatedTrack.clear();
}

//////////////////////////////////////////////////////////////////////////
TVTrimControl::TVTrimControl(QObject *parent /*= 0*/)
	:QObject(parent)
{
	m_bLeftDrag = false;
	m_bRightDrag = false;
	m_bTriming = false;
}

TVTrimControl* TVTrimControl::GetInstance()
{
	static TVTrimControl ins;
	return &ins;
}

bool TVTrimControl::StartTrim(QPoint point, TrackWidget *item)
{
	HintTriming(point, item);
	m_bTriming = m_bLeftDrag || m_bRightDrag;
	m_ptPress = point;
	if (m_bTriming)
	{
		CalcTrimDuration();
		emit sigBeginTrim();
	}
	m_lstTrimingCopy = m_lstTriming;
	return m_bTriming;
}

void TVTrimControl::MoveTrim(QPoint point, TrackWidget *item)
{
	m_pTrackWidget = item;
	if (TrackBuilder::GetInstance()->GetTrackView()->IsDragScroll())
	{
		return;
	}
	if (!IsTriming())
	{
		if (!m_pTrackWidget->GetTrackData()->IsEditable())
			return;
		HintTriming(point, item);
		if (m_bLeftDrag || m_bRightDrag)
			m_pTrackWidget->setCursor(Qt::SplitHCursor);
		else
			m_pTrackWidget->setCursor(Qt::ArrowCursor);
		return;
	}

	int offset = point.x() - m_ptPress.x();
	int offsetframe = offset > 0 ? Ruler()->Duration2FramesF(qAbs(offset)) : (0 - Ruler()->Duration2FramesF(qAbs(offset)));
	offsetframe = qBound(m_ptTrimRange.x(), offsetframe, m_ptTrimRange.y());
	DoAdsorbtion(offsetframe); 
	m_lstTriming = m_lstTrimingCopy;
	for (int i = 0; i < m_lstTriming.size(); ++i)
	{
		if (m_bLeftDrag)
		{
			m_lstTriming[i].startPos += offsetframe;
			m_lstTriming[i].length -= offsetframe;
		}
		else
		{
			m_lstTriming[i].length += offsetframe;
		}
	}
	EmitTrimSignal(point);
	TrackBuilder::GetInstance()->UpdateItems();
}

void TVTrimControl::EndTrim(QPoint point)
{
	if (!IsTriming())
		return;
	TimelineManager::GetInstance()->BeginComposite();
	for (auto dragClip : m_lstTriming)
	{
		TimelineManager::GetInstance()->Trim(dragClip.clip, dragClip.startPos, dragClip.length);
	}
	TimelineManager::GetInstance()->EndComposite();
	m_bTriming = m_bLeftDrag = m_bRightDrag = false;
	m_lstTriming.clear();

	HintTriming(point,m_pTrackWidget);
	if (m_bLeftDrag || m_bRightDrag)
		m_pTrackWidget->setCursor(Qt::SplitHCursor);
	else
		m_pTrackWidget->setCursor(Qt::ArrowCursor);
	emit sigEndTrim();
	TrackBuilder::GetInstance()->UpdateItems();
}

bool TVTrimControl::IsTriming()
{
	return m_bTriming;
}


DragClipList TVTrimControl::GetTrimingList()
{
	return m_lstTriming;
}

DragClipList TVTrimControl::GetTrimingList(int trackIndex)
{
	DragClipList lst;
	for (auto trimClip : m_lstTriming)
	{
		if (trimClip.trackIndex == trackIndex)
		{
			lst.push_back(trimClip);
		}
	}
	return lst;
}

QList<int> TVTrimControl::GetAdsorbList()
{
	return m_lstAdsorbPoints;
}

TimelineRuler* TVTrimControl::Ruler()
{
	return TrackBuilder::GetInstance()->GetRuler();
}

void TVTrimControl::HintTriming(QPoint point, TrackWidget *item, int hintMargins)
{
	m_bLeftDrag = false;
	m_bRightDrag = false;
    auto realPoint = RealPoint(point);
	auto selectClips = item->GetTrackData()->GetSelected();
	
	int minHitDuration = hintMargins;
	for (auto clip : selectClips)
	{
		int startPos = Ruler()->Frames2PositionF(clip->GetStartAtTrack());
		int endPos = Ruler()->Frames2PositionF(clip->GetEndAtTrack() + 1);

		if (qAbs(realPoint.x() - startPos) < minHitDuration)
		{
			minHitDuration = realPoint.x() - startPos;
			m_pHitClip = clip;
			m_bLeftDrag = true;
			m_bRightDrag = false;
		}
		if (qAbs(realPoint.x() - endPos) < minHitDuration)
		{
			minHitDuration = realPoint.x() - endPos;
			m_pHitClip = clip;
			m_bLeftDrag = false;
			m_bRightDrag = true;
		}
	}
}

void TVTrimControl::CalcTrimDuration()
{
	auto selecteClips = TimelineManager::GetInstance()->GetSelected();
	for (auto clip : selecteClips)
	{
		DragClipBase dragClip;
		dragClip.startPos = clip->GetStartAtTrack();
		dragClip.trackIndex = clip->GetTimelineTrack()->GetIndex();
		dragClip.length = clip->GetLenAtTrack();
		dragClip.clip = clip;
		m_lstTriming.push_back(dragClip);
	}
	m_ptTrimRange = TimelineManager::GetInstance()->GetTrimRange(m_bLeftDrag);
}


void TVTrimControl::DoAdsorbtion(int &newoffset)
{
	int tmpOffset = newoffset;

	m_lstAdsorbPoints.clear();
	QList<int> keyPoints = GetKeyPoints(m_lstTriming);

	int adsrobDistance = Ruler()->Duration2FramesF(10);

	int minDuration = INT_MAX;
	int hitPos = m_bLeftDrag ? m_pHitClip->GetStartAtTrack() + tmpOffset : m_pHitClip->GetEndAtTrack() + 1 + tmpOffset;
	bool bReduse = true;
	int minTmp;
	for (auto keyPt : keyPoints)
	{
		int preMin = minDuration;
		minTmp = hitPos - keyPt;
		minDuration = qMin(minDuration, qAbs(minTmp));
		if ((minDuration <= adsrobDistance) && (minDuration != preMin))
		{
			bReduse = minTmp > 0;
			m_lstAdsorbPoints.clear();
			m_lstAdsorbPoints.push_back(keyPt);
		}
			
	}
	if (minDuration > adsrobDistance)
		return;
	bReduse ? tmpOffset -= minDuration : tmpOffset += minDuration;
	
	if ((tmpOffset < m_ptTrimRange.x()) || (tmpOffset > m_ptTrimRange.y()))
	{
		m_lstAdsorbPoints.clear();
		return;
	}
	newoffset = tmpOffset;
}

void TVTrimControl::EmitTrimSignal(QPoint pt)
{
	for (auto dragClip : m_lstTriming)
	{
		if (dragClip.clip == m_pHitClip)
		{
			emit sigTriming(m_pTrackWidget->mapToGlobal(pt),dragClip.startPos,dragClip.length);
			break;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
TrackBuilder::TrackBuilder(QObject *parent)
	:QObject(parent)
{
	m_pTimelineMgr = TimelineManager::GetInstance();
	connect(AudioWaveMgr::GetInstance(), &AudioWaveMgr::sigFinish, this, [&]{
		UpdateItems();
	},Qt::QueuedConnection);
}

TrackBuilder* TrackBuilder::GetInstance()
{
	static TrackBuilder ins;
	return &ins;
}

void TrackBuilder::SetTrackView(TVTrackView *pView)
{
	m_pTrackView = pView;
}

void TrackBuilder::SetRuler(TimelineRuler *pRuler)
{
	m_pRuler = pRuler;
}

TimelineRuler * TrackBuilder::GetRuler()
{
	return m_pRuler;
}

void TrackBuilder::Rebuild()
{
	qDeleteAll(m_trackWidgets);
	m_trackWidgets.clear();
	TimelineTracks tracks = m_pTimelineMgr->GetTracks();
	for (auto track : tracks)
	{
		m_trackWidgets.push_back(CreateTrack(track));
	}
	m_pTrackView->Resize();
	UpdateItems();
}

void TrackBuilder::UpdateItems(bool bPartUpdate/* = false*/)
{
	//for (auto trackWidget : m_trackWidgets)
	//{
	//	trackWidget->repaint();
	//}
	emit Update();
}

TrackWidgets TrackBuilder::GetTrackWidgets()
{
	return m_trackWidgets;
}

TrackWidget* TrackBuilder::GetTrackWidget(int nIndex)
{
	for (auto item : m_trackWidgets)
	{
		if (item->GetTrackData()->GetIndex() == nIndex)
		{
			return item;
		}
	}
	return nullptr;
}

TVTrackView* TrackBuilder::GetTrackView()
{
	return m_pTrackView;
}

TrackWidget* TrackBuilder::CreateTrack(TimelineTrack* trackData)
{
	TrackWidget* pTrack = new TrackWidget(trackData, m_pTrackView);
	pTrack->SetRuler(m_pRuler);
	pTrack->show();
	return pTrack;
}

int TrackBuilder::Duration()
{
	int duration = 0;
	if (!m_trackWidgets.isEmpty())
	{
		auto iter = std::max_element(m_trackWidgets.begin(), m_trackWidgets.end(), [](TrackWidget* l, TrackWidget *r){
			return l->Duration() < r->Duration();
		});
		duration = (*iter)->Duration();
	}

	return duration;
}

int TrackBuilder::Height()
{
	int height = 0;
	for (auto item : m_trackWidgets)
	{
		height += item->Height();
	}
	return height;
}

//////////////////////////////////////////////////////////////////////////
MouseEffectDragClip::MouseEffectDragClip()
{
	Clear();
}


void MouseEffectDragClip::Clear()
{
	dragtype = MouseEffectDragClip_None;
	soundfile = "";
	filterTimeline = nullptr;
}

//////////////////////////////////////////////////////////////////////////
TVMouseEffectDragCtrl::TVMouseEffectDragCtrl(QObject *parent /*= 0*/)
	:QObject(parent)
{
	m_bTipAcceptable = false;
	m_pTargetClip = nullptr;
}

TVMouseEffectDragCtrl* TVMouseEffectDragCtrl::GetInstance()
{
	static TVMouseEffectDragCtrl ins;
	return &ins;
}

void TVMouseEffectDragCtrl::Init(TVTrackView *pTrackView)
{
	m_pTrackView = pTrackView;
}

bool TVMouseEffectDragCtrl::IsTipAcceptable()
{
	return m_bTipAcceptable;
}

void TVMouseEffectDragCtrl::DragEnter(QDragEnterEvent *e)
{
	auto mimeData = e->mimeData();
	if (mimeData->hasFormat("MatterView_MouseEffect_ClickSound"))
	{
		QString soundFile = mimeData->property("MatterView_MouseEffect_ClickSound_File").value<QString>();
		m_DragClip.dragtype = MouseEffectDragClip::MouseEffectDragClip_Sound;
		m_DragClip.soundfile = soundFile;
	}
	else if (mimeData->hasFormat("MatterView_MouseEffect_Filter"))
	{
		FSNLETimeline *timeline = reinterpret_cast<FSNLETimeline*>(mimeData->property("MatterView_MouseEffect_FilterTimeline").value<qptrdiff>());
		Q_ASSERT(timeline);
		m_DragClip.dragtype = MouseEffectDragClip::MouseEffectDragClip_Filter;
		m_DragClip.filterTimeline = timeline;
	}
	else
	{
		Q_ASSERT(false);
	}
	//m_bTipAcceptable = true;
	e->accept();
	TrackBuilder::GetInstance()->UpdateItems();
}

void TVMouseEffectDragCtrl::DragMove(QDragMoveEvent *e)
{
	m_pTargetClip = _Acceptable(e->pos());
	TrackBuilder::GetInstance()->UpdateItems();
	m_pTargetClip ? e->accept() : e->ignore();
}

void TVMouseEffectDragCtrl::DragLeave(QDragLeaveEvent *e)
{
	//m_bTipAcceptable = false;
	m_pTargetClip = nullptr;
	TrackBuilder::GetInstance()->UpdateItems();
	e->accept();
}

void TVMouseEffectDragCtrl::DropDown(QDropEvent *e)
{
	auto acceptClip= _Acceptable(e->pos());
	if (acceptClip)
	{
		if (m_DragClip.dragtype == MouseEffectDragClip::MouseEffectDragClip_Sound)
		{
			TimelineManager::GetInstance()->AddMouseSound(acceptClip, m_DragClip.soundfile);
		}
		else if (m_DragClip.dragtype == MouseEffectDragClip::MouseEffectDragClip_Filter)
		{
			TimelineManager::GetInstance()->AddMouseFilter(acceptClip, m_DragClip.filterTimeline);
		}
		TimelineManager::GetInstance()->SingleSelect(acceptClip->GetIndex(), acceptClip->GetStartAtTrack());
		//acceptClip->SetSelected(true);
	}
	m_bTipAcceptable = false;
	m_pTargetClip = nullptr;
	TrackBuilder::GetInstance()->UpdateItems();
	acceptClip ? e->accept() : e->ignore();
}

Clip* TVMouseEffectDragCtrl::GetTargetClip()
{
	return m_pTargetClip;
}

void TVMouseEffectDragCtrl::SetDragTip(bool bDraging)
{
	m_bTipAcceptable = bDraging;
	TrackBuilder::GetInstance()->UpdateItems();
}

Clip* TVMouseEffectDragCtrl::_Acceptable(QPoint pos)
{
	TrackWidget *child = qobject_cast<TrackWidget*>(m_pTrackView->childAt(pos));
	if (child)
	{
		int posX = pos.x() - TRACK_WIDGET_HEADER_WIDTH;
		int nFrame = TrackBuilder::GetInstance()->GetRuler()->Position2FramesF(posX);

		auto clip = child->GetTrackData()->GetClip(nFrame);
		if (clip)
		{
			if (clip->GetNLETimeline()->IsDesktopVideo())
			{
				return clip;
			}
		}
	}
	return nullptr;
}

//////////////////////////////////////////////////////////////////////////
ChangeSpeedControl::ChangeSpeedControl(QObject *parent /*= 0*/)
	:QObject(parent)
	, m_RespeedCursor(QPixmap(":/FSTimelineView/light/Resources/light/ReSpeed_Cursor.png"))
{
	m_bChanging = false;
	m_nAdsorbPos = -1;
}

TimelineRuler* ChangeSpeedControl::Ruler()
{
	return TrackBuilder::GetInstance()->GetRuler();
}

ChangeSpeedControl* ChangeSpeedControl::GetInstance()
{
	static ChangeSpeedControl ins;
	return &ins;
}

void ChangeSpeedControl::StartChange(QPoint pt, TrackWidget *child)
{
	HitBorder(pt, child);
	m_bChanging = m_bLeftDrag || m_bRightDrag;
	m_ptPress = pt;
	if (m_bChanging)
	{
		m_DragClip.startPos = m_pHitClip->GetStartAtTrack();
		m_DragClip.trackIndex = m_pHitClip->GetTimelineTrack()->GetIndex();
		m_DragClip.length = m_pHitClip->GetLenAtTrack();
		m_DragClip.clip = m_pHitClip;
		m_DragRange = TimelineManager::GetInstance()->GetDragRange(m_pHitClip,m_bLeftDrag);
	}
	m_DragClipCopy = m_DragClip;
}

void ChangeSpeedControl::MoveChange(QPoint pt, TrackWidget *child)
{
	m_pTrackWidget = child;
	if (TrackBuilder::GetInstance()->GetTrackView()->IsDragScroll())
	{
		return;
	}
	if (!IsChanging())
	{
		if (!m_pTrackWidget->GetTrackData()->IsEditable())
			return;
		HitBorder(pt, child);
		if (m_bLeftDrag || m_bRightDrag)
			m_pTrackWidget->setCursor(m_RespeedCursor);
		else
			m_pTrackWidget->setCursor(Qt::ArrowCursor);
		return;
	}

	int offset = pt.x() - m_ptPress.x();
	int offsetframe = offset > 0 ? Ruler()->Duration2FramesF(qAbs(offset)) : (0 - Ruler()->Duration2FramesF(qAbs(offset)));

	offsetframe = qBound(m_DragRange.x(), offsetframe, m_DragRange.y());
	DoAdsorbtion(offsetframe);
	m_DragClip = m_DragClipCopy;

	

	if (m_bLeftDrag)
	{
		m_DragClip.startPos += offsetframe;
		m_DragClip.length -= offsetframe;
	}
	else
	{
		m_DragClip.length += offsetframe;
	}

	EmitChangingSignal(pt);
	TrackBuilder::GetInstance()->UpdateItems();
}

void ChangeSpeedControl::EndChange(QPoint pt)
{
	if (!IsChanging())
		return;
	TimelineManager::GetInstance()->ChangeSpeed(m_DragClip.clip, m_DragClip.startPos, m_DragClip.length);
	m_bChanging = m_bLeftDrag = m_bRightDrag = false;
	m_DragClip = DragClipBase();
	m_nAdsorbPos = -1;
	HitBorder(pt, m_pTrackWidget);
	if (m_bLeftDrag || m_bRightDrag)
		m_pTrackWidget->setCursor(m_RespeedCursor);
	else
		m_pTrackWidget->setCursor(Qt::ArrowCursor);
	emit sigEndChange();
	TrackBuilder::GetInstance()->UpdateItems();
}

bool ChangeSpeedControl::IsChanging()
{
	return m_bChanging;
}

void ChangeSpeedControl::HitBorder(QPoint point, TrackWidget *item, int hintMargins)
{
	m_bLeftDrag = false;
	m_bRightDrag = false;
	auto realPoint = RealPoint(point);
	auto selectClips = item->GetTrackData()->GetSelected();

	int minHitDuration = hintMargins;
	for (auto clip : selectClips)
	{
		int startPos = Ruler()->Frames2PositionF(clip->GetStartAtTrack());
		int endPos = Ruler()->Frames2PositionF(clip->GetEndAtTrack() + 1);

		if (qAbs(realPoint.x() - startPos) < minHitDuration)
		{
			minHitDuration = realPoint.x() - startPos;
			m_pHitClip = clip;
			m_bLeftDrag = true;
			m_bRightDrag = false;
		}
		if (qAbs(realPoint.x() - endPos) < minHitDuration)
		{
			minHitDuration = realPoint.x() - endPos;
			m_pHitClip = clip;
			m_bLeftDrag = false;
			m_bRightDrag = true;
		}
	}
}

void ChangeSpeedControl::DoAdsorbtion(int &newoffset)
{
	int tmpOffset = newoffset;
	m_nAdsorbPos = -1;
	QList<int> keyPoints = GetKeyPoints(DragClipList() << m_DragClip);

	int adsrobDistance = Ruler()->Duration2FramesF(10);

	int minDuration = INT_MAX;
	int hitPos = m_bLeftDrag ? m_pHitClip->GetStartAtTrack() + tmpOffset : m_pHitClip->GetEndAtTrack() + 1 + tmpOffset;
	bool bReduse = true;
	int minTmp;
	for (auto keyPt : keyPoints)
	{
		int preMin = minDuration;
		minTmp = hitPos - keyPt;
		minDuration = qMin(minDuration, qAbs(minTmp));
		if ((minDuration <= adsrobDistance) && (minDuration != preMin))
		{
			bReduse = minTmp > 0;
			m_nAdsorbPos = keyPt;
		}

	}
	if (minDuration > adsrobDistance)
	{
		m_nAdsorbPos = -1;
		return;
	}
		
	bReduse ? tmpOffset -= minDuration : tmpOffset += minDuration;

	if ((tmpOffset < m_DragRange.x()) || (tmpOffset > m_DragRange.y()))
	{
		m_nAdsorbPos = -1;
		return;
	}
	newoffset = tmpOffset;
}

void ChangeSpeedControl::EmitChangingSignal(QPoint pt)
{
	auto globalPt = m_pTrackWidget->mapToGlobal(pt);
	float scale = (1.0 * m_DragClip.length / m_DragClipCopy.length) * m_DragClipCopy.clip->GetNLETimeline()->GetRenderScale();
	qDebug() << "scale" << scale;
	emit sigChanging(globalPt, m_DragClip.length, 1.0 / scale * 100);
}

DragClipBase ChangeSpeedControl::GetDragClip(int nTrackIndex)
{
	if (m_DragClip.trackIndex == nTrackIndex)
		return m_DragClip;
	return DragClipBase();
}

int ChangeSpeedControl::GetAdsorbPos()
{
	return m_nAdsorbPos;
}
