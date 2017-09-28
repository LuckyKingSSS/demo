#include "stdafx.h"


#include "inc_CommonLib/TimelineRuler.h"
#include "inc_FProject/FDateDefine.h"


#include "FActionLayer.h"
#include "FLayer.h"
#include "FTracksView.h"

FActionLayerManager::FActionLayerManager(QObject *parent)
:FLayerManager(parent)
{
    AddLayer(new FSelectionLayer(this));
    AddLayer(new FMoveTimelineLayer(this));
}


bool FActionLayerManager::mousePressEvent(FTracksView *trackView, QMouseEvent *event)
{
    for (auto layer : GetLayers())
    {
        bool accept = qobject_cast<FActionLayer*>(layer)->mousePressEvent(trackView, event);
        if (accept)
            break;
    }

    return false;

}

bool FActionLayerManager::mouseMoveEvent(FTracksView *trackView, QMouseEvent *event)
{
    for (auto layer : GetLayers())
    {
        bool accept = qobject_cast<FActionLayer*>(layer)->mouseMoveEvent(trackView, event);
        if (accept)
            break;
    }

    return false;
}

bool FActionLayerManager::mouseReleaseEvent(FTracksView *trackView, QMouseEvent *event)
{
    for (auto layer : GetLayers())
    {
        bool accept = qobject_cast<FActionLayer*>(layer)->mouseReleaseEvent(trackView, event);
        if (accept)
            break;
    }

    return false;
}

bool FActionLayerManager::dragEnterEvent(FTracksView *trackView, QDragEnterEvent *event)
{
    for (auto layer : GetLayers())
    {
        bool accept = qobject_cast<FActionLayer*>(layer)->dragEnterEvent(trackView, event);
        if (accept)
            break;
    }

    return false;
}

bool FActionLayerManager::dragMoveEvent(FTracksView *trackView, QDragMoveEvent *event)
{
    for (auto layer : GetLayers())
    {
        bool accept = qobject_cast<FActionLayer*>(layer)->dragMoveEvent(trackView, event);
        if (accept)
            break;
    }

    return false;
}

bool FActionLayerManager::dragLeaveEvent(FTracksView *trackView, QDragLeaveEvent *event)
{
    for (auto layer : GetLayers())
    {
        bool accept = qobject_cast<FActionLayer*>(layer)->dragLeaveEvent(trackView, event);
        if (accept)
            break;
    }

    return false;
}

bool FActionLayerManager::dropEvent(FTracksView *trackView, QDropEvent *event)
{
    for (auto layer : GetLayers())
    {
        bool accept = qobject_cast<FActionLayer*>(layer)->dropEvent(trackView, event);
        if (accept)
            break;
    }

    return false;
}


bool FActionLayerManager::paintEvent(QPainter *painter, QRect rect, FTracksView *trackView, QPaintEvent *event)
{
    for (auto layer : GetLayers())
    {
        bool accept = qobject_cast<FActionLayer*>(layer)->paintEvent(painter, rect, trackView, event);
        if (accept)
            break;
    }

    return false;
}

//////////////////////////////////////////////////////////////////////////
FActionLayer::FActionLayer(QObject *parent)
: FLayer(parent)
{

}

bool FActionLayer::mousePressEvent(FTracksView *trackView, QMouseEvent *event)
{
    return false;

}

bool FActionLayer::mouseMoveEvent(FTracksView *trackView, QMouseEvent *event)
{
    return false;

}

bool FActionLayer::mouseReleaseEvent(FTracksView *trackView, QMouseEvent *event)
{
    return false;

}

bool FActionLayer::dragEnterEvent(FTracksView *trackView, QDragEnterEvent *event)
{
    return false;

}

bool FActionLayer::dragMoveEvent(FTracksView *trackView, QDragMoveEvent *event)
{
    return false;

}

bool FActionLayer::dragLeaveEvent(FTracksView *trackView, QDragLeaveEvent *event)
{
    return false;

}

bool FActionLayer::dropEvent(FTracksView *trackView, QDropEvent *event)
{
    return false;

}


bool FActionLayer::paintEvent(QPainter *painter, QRect rect, FTracksView *trackWidget, QPaintEvent *event)
{
    return false;
}

//////////////////////////////////////////////////////////////////////////
FSelectionLayer::FSelectionLayer(QObject *parent)
:FActionLayer(parent)
{
    m_pressDown = false;
    m_bDecideSelectedNow = false;
}


bool FSelectionLayer::mousePressEvent(FTracksView *trackView, QMouseEvent *event)
{
    m_pressTimeline = nullptr;
    m_selectedRect = QRect();
    m_bDecideSelectedNow = true;

    m_pressPoint = event->pos();
    auto sequence = trackView->GetSequence();
    FTrackWidget *child = qobject_cast<FTrackWidget*>(trackView->childAt(m_pressPoint));
    int pressFrame = trackView->GetRuler()->Position2FramesF(event->pos().x() - trackView->GetHeadWidth());

    if (event->button() == Qt::LeftButton)
    {

        m_pressDown = true;

        bool bCtrl = QApplication::keyboardModifiers() & Qt::ControlModifier;
        bool bShift = QApplication::keyboardModifiers() & Qt::ShiftModifier;

        if (child)
        {
            // 记录被点中的timeline，为框选做准备
            m_pressTimeline = child->GetTrack()->GetTimeline(pressFrame);
            auto selectedTimeline = child->GetTrack()->GetTimeline(pressFrame);

            if (bCtrl)
            {
                if (selectedTimeline && selectedTimeline->GetSelected())
                {
                    m_bDecideSelectedNow = false;
                }
            }
            else if (bShift)
            {

            }
            else
            {
                if (selectedTimeline && selectedTimeline->GetSelected())
                {
                    m_bDecideSelectedNow = false;

                }
            }
        }

        // 是否立即进行选择操作
        if (m_bDecideSelectedNow)
        {

            if (bCtrl)
            {
                if (child)
                {
                    auto trackIndex = child->GetTrack()->GetIndex();
                    sequence->CtrlSelect(trackIndex, pressFrame);
                }
            }
            else if (bShift)
            {
                if (child)
                {
                    auto trackIndex = child->GetTrack()->GetIndex();
                    sequence->ShiftSelect(trackIndex, pressFrame);
                }
                else
                {
                    sequence->UnselectAll();
                }
            }
            else
            {
                if (child)
                {
                    auto trackIndex = child->GetTrack()->GetIndex();
                    sequence->SingleSelect(trackIndex, pressFrame);
                }
                else
                {
                    sequence->UnselectAll();
                }
            }

        }
    }
    else if (event->button() == Qt::RightButton)
    {
        if (child)
        {
            auto selectedTimeline = child->GetTrack()->GetTimeline(pressFrame);
            if (selectedTimeline)
            {
                if (!selectedTimeline->GetSelected())
                {
                    auto trackIndex = child->GetTrack()->GetIndex();
                    sequence->SingleSelect(trackIndex, pressFrame);
                }
                else
                {
                    
                }
            }
            else
            {
                sequence->UnselectAll();
            }
        }
    }

 
    return false;

}

bool FSelectionLayer::mouseMoveEvent(FTracksView *trackView, QMouseEvent *event)
{

    QPoint point = event->pos();
  

    // 框选
    if (m_pressDown && !m_pressTimeline)
    {
        m_selectedRect = QRect(m_pressPoint, point);

        // 计算谁被框选轨道了
        auto trackWidgets = trackView->GetTracks();
        FTrackWidgets selectedTrackWidgets;
        for (auto trackWidget : trackWidgets)
        {
            if (m_selectedRect.intersects(trackWidget->geometry()))
            {
                selectedTrackWidgets.push_back(trackWidget);
            }
        }

        if (!selectedTrackWidgets.isEmpty())
        {

            auto topTrack = selectedTrackWidgets.first();
            auto bottomTrack = selectedTrackWidgets.last();

            int topTrackIndex = topTrack->GetIndex();
            int bottomTrackIndex = bottomTrack->GetIndex();

            int offset = topTrack->GetHeadWidth();

            int leftFrame = trackView->GetRuler()->Position2FramesF(m_selectedRect.x() - offset);
            int rightFrame = trackView->GetRuler()->Position2FramesF(m_selectedRect.right() - offset);


            trackView->GetSequence()->RectangleSelect(topTrackIndex, bottomTrackIndex, leftFrame, rightFrame);

        }

        trackView->RectangleSelect(m_selectedRect);
        trackView->update();

        return true;
    }

    return false;
}

bool FSelectionLayer::mouseReleaseEvent(FTracksView *trackView, QMouseEvent *event)
{
    if (m_pressDown)
    {
        if (!m_bDecideSelectedNow)
        {
            auto sequence = trackView->GetSequence();

            FTrackWidget *child = qobject_cast<FTrackWidget*>(trackView->childAt(event->pos()));

            bool bCtrl = QApplication::keyboardModifiers() & Qt::ControlModifier;
            int pressFrame = trackView->GetRuler()->Position2FramesF(event->pos().x() - trackView->GetHeadWidth());

            if (bCtrl)
            {
                if (child)
                {
                    auto trackIndex = child->GetTrack()->GetIndex();
                    sequence->CtrlSelect(trackIndex, pressFrame);
                }
            }
            //else if (bShift)
            //{
            //    if (child)
            //    {
            //        auto trackIndex = child->GetTrack()->GetIndex();
            //        sequence->ShiftSelect(trackIndex, pressFrame);
            //    }
            //    else
            //    {
            //        sequence->UnselectAll();
            //    }
            //}
            else
            {
                if (child)
                {
                    auto trackIndex = child->GetTrack()->GetIndex();
                    sequence->SingleSelect(trackIndex, pressFrame);
                }
                else
                {
                    sequence->UnselectAll();
                }
            }
        }

    }



    m_pressDown = false;
    m_pressTimeline = nullptr;
    m_selectedRect = QRect();
    m_bDecideSelectedNow = false;

    trackView->RectangleSelect(m_selectedRect);
    trackView->update();

    return false;
}

//bool FSelectionLayer::paintEvent(QPainter *painter, QRect rect, FTracksView *trackView, QPaintEvent *event)
//{
//    PainterRestore painterRestore(painter);
//
//    if (!m_selectedRect.isNull())
//    {
//        painter->drawRect(m_selectedRect);
//    }
//
//    return false;
//}

FMoveTimelineLayer::FMoveTimelineLayer(QObject *parent)
:FActionLayer(parent)
{
    m_pressDown = false;
}

bool FMoveTimelineLayer::mousePressEvent(FTracksView *trackView, QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_pressDown = true;
        m_pressTimeline = nullptr;

        m_pressPoint = event->pos();
        //const int trackHeadWidth = trackView->GetHeadWidth();
        int pressFrame = trackView->GetRuler()->Position2FramesF(event->pos().x() - trackView->GetHeadWidth());

        FTrackWidget *child = qobject_cast<FTrackWidget*>(trackView->childAt(m_pressPoint));
        if (child)
        {
            m_pressTimeline = child->GetTrack()->GetTimeline(pressFrame);
            //auto selectedTimeline = child->GetTrack()->GetTimeline(pressFrame);
        }

    }


    return false;

}

bool FMoveTimelineLayer::mouseMoveEvent(FTracksView *trackView, QMouseEvent *event)
{
    QPoint point = event->pos();

    if (m_pressDown)
    {
        FTrackWidget *child = qobject_cast<FTrackWidget*>(trackView->childAt(point));
        if (child)
        {
            auto selectedTimelines = trackView->GetSequence()->GetSelected();
            bool bSelected = !selectedTimelines.isEmpty();
            if (bSelected)
            {
                if ((m_pressPoint - event->pos()).manhattanLength() > 4)
                {
                    auto mimeData = new FDragMimeData;
                    mimeData->type = FDragMimeData::Timeline;
                    mimeData->timelines = selectedTimelines;

                    QDrag drag(this);
                    drag.setMimeData(mimeData);
                    drag.exec();
                    
                }
            }

        }
    }

    return false;
}

bool FMoveTimelineLayer::mouseReleaseEvent(FTracksView *trackView, QMouseEvent *event)
{
    m_pressDown = false;
    m_pressTimeline = nullptr;

    return false;
}

bool FMoveTimelineLayer::dragEnterEvent(FTracksView *trackView, QDragEnterEvent *event)
{
    auto mimeData = qobject_cast<const FDragMimeData*>(event->mimeData());
    if (mimeData)
    {
        if (mimeData->type == FDragMimeData::Timeline)
        {

            FSEQUNECE_UNDO->BeginComposite();

            event->accept();

            return true;
        }

    }
    return false;
}

bool FMoveTimelineLayer::dragMoveEvent(FTracksView *trackView, QDragMoveEvent *event)
{

    auto mimeData = qobject_cast<const FDragMimeData*>(event->mimeData());
    if (mimeData)
    {
        if (mimeData->type == FDragMimeData::Timeline)
        {
            auto timelines = mimeData->timelines;


            FTrackWidget *child = qobject_cast<FTrackWidget*>(trackView->childAt(event->pos()));

            Q_ASSERT(m_pressTimeline);

            auto tracks = trackView->GetSequence()->GetTracks();
            int trackOffset = child ? child->GetIndex() - m_pressTimeline->GetIndex() : tracks.size() - m_pressTimeline->GetIndex();
            int frameOffset = trackView->GetRuler()->Position2FramesF((event->pos().x() - m_pressPoint.x()));



            // 限制frameOffset
            for (auto temp : timelines)
            {
                int realyStartAtTrack = temp->GetStartAtTrack() + frameOffset;
                if (realyStartAtTrack < 0)
                {
                    frameOffset = frameOffset - realyStartAtTrack;
                }

                int realyTrackIndex = temp->GetIndex() + trackOffset;
                if (realyTrackIndex >= tracks.size())
                {
                    trackOffset = trackOffset - (realyTrackIndex - tracks.size()) - 1;
                }
                else if (realyTrackIndex < 0)
                {
                    trackOffset = trackOffset - realyTrackIndex;
                }
            }



            FNLEDragTimelines dragTimelines;
            for (auto temp : timelines)
            {
                FNLEDragTimeline dragTimeline(temp);
                dragTimeline.SetStartAtTrack(dragTimeline.GetStartAtTrack() + frameOffset);
                dragTimeline.SetIndex(dragTimeline.GetIndex() + trackOffset);
                dragTimelines.push_back(dragTimeline);
            }

            for (auto track : tracks)
            {
                track->SetDragTimelines(FNLEDragTimelines());
            }

            std::map<int, FNLEDragTimelines> map;
            for (FNLEDragTimeline &temp : dragTimelines)
            {
                int index = temp.GetIndex();
                map[index].push_back(temp);
            }

            for (auto key : map)
            {
                Q_ASSERT(key.first >= 0 && key.first < tracks.size());
                tracks[key.first]->SetDragTimelines(key.second);
            }

            trackView->update();

            event->accept();
            return true;
        }

    }

    return false;

}

bool FMoveTimelineLayer::dragLeaveEvent(FTracksView *trackView, QDragLeaveEvent *event)
{
    event->accept();
    return true;
}

bool FMoveTimelineLayer::dropEvent(FTracksView *trackView, QDropEvent *event)
{
    auto mimeData = qobject_cast<const FDragMimeData*>(event->mimeData());
    if (mimeData)
    {
        if (mimeData->type == FDragMimeData::Timeline)
        {

            auto timelines = mimeData->timelines;

            auto tracks = trackView->GetSequence()->GetTracks();
            auto sequence = trackView->GetSequence();

            for (auto temp : tracks)
            {
                auto dragTimelines = temp->GetDragTimelines();
                for (auto dragTemp : dragTimelines)
                {
                    //dragTemp.GetTimeline()->SetStartAtTrack(dragTemp.GetStartAtTrack());
                    sequence->MoveTimeline(dragTemp.GetIndex(), dragTemp.GetStartAtTrack(), dragTemp.GetTimeline());
                }

                // 清除拖拽
                temp->SetDragTimelines(FNLEDragTimelines());

            }


            FSEQUNECE_UNDO->EndComposite();

            trackView->update();
            event->accept();
            return true;
        }

    }

    return false;

}
//
////////////////////////////////////////////////////////////////////////////
//FTrimTimelineLayer::FTrimTimelineLayer(QObject *parent)
//:FActionLayer(parent)
//{
//    m_bTriming = false;
//}
//
//bool FTrimTimelineLayer::mousePressEvent(FTracksView *trackView, QMouseEvent *event)
//{
//    return false;
//}
//
//bool FTrimTimelineLayer::mouseMoveEvent(FTracksView *trackView, QMouseEvent *event)
//{
//
//    return false;
//}
//
//bool FTrimTimelineLayer::mouseReleaseEvent(FTracksView *trackView, QMouseEvent *event)
//{
//    return false;
//}
//
//void FTrimTimelineLayer::MoveTrim(QPoint point, FTrackWidget *item)
//{
//    Q_ASSERT(item);
//    //m_pTrackWidget = item;
//    auto ruler = item->Ruler();
//    if (!IsTriming())
//    {
//        HintTriming(point, item);
//        if (m_bLeftDrag || m_bRightDrag)
//            item->setCursor(Qt::SplitHCursor);
//        else
//            item->setCursor(Qt::ArrowCursor);
//    }
//    else
//    {
//        
//        int offset = point.x() - m_pressPoint.x();
//        int offsetframe = offset > 0 ? ruler->Duration2FramesF(qAbs(offset)) : (0 - ruler->Duration2FramesF(qAbs(offset)));
//        offsetframe = qBound(m_ptTrimRange.x(), offsetframe, m_ptTrimRange.y());
//        DoAdsorbtion(offsetframe);
//        m_lstTriming = m_lstTrimingCopy;
//        for (int i = 0; i < m_lstTriming.size(); ++i)
//        {
//            if (m_bLeftDrag)
//            {
//                m_lstTriming[i].startPos += offsetframe;
//                m_lstTriming[i].length -= offsetframe;
//            }
//            else
//            {
//                m_lstTriming[i].length += offsetframe;
//            }
//        }
//        EmitTrimSignal(point);
//        TrackBuilder::GetInstance()->UpdateItems();
//    }
//
//}
//
//bool FTrimTimelineLayer::IsTriming()
//{
//    return m_bTriming;
//
//}
//
//void FTrimTimelineLayer::HintTriming(QPoint point, FTrackWidget *item, int hintMargins /*= 3*/)
//{
//    m_bLeftDrag = false;
//    m_bRightDrag = false;
//    auto realPoint = point;
//
//    auto ruler = item->Ruler();
//    auto selectClips = item->GetTrack()->GetSelected();
//
//    int minHitDuration = hintMargins;
//    for (auto clip : selectClips)
//    {
//        int startPos = ruler->Frames2PositionF(clip->GetStartAtTrack());
//        int endPos = ruler->Frames2PositionF(clip->GetEndAtTrack() + 1);
//
//        if (qAbs(realPoint.x() - startPos) < minHitDuration)
//        {
//            minHitDuration = realPoint.x() - startPos;
//            m_pHitClip = clip;
//            m_bLeftDrag = true;
//            m_bRightDrag = false;
//        }
//        if (qAbs(realPoint.x() - endPos) < minHitDuration)
//        {
//            minHitDuration = realPoint.x() - endPos;
//            m_pHitClip = clip;
//            m_bLeftDrag = false;
//            m_bRightDrag = true;
//        }
//    }
//}
