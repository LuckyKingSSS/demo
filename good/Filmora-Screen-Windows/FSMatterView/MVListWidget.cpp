#include "stdafx.h"
#include "MVListWidget.h"
#include "MVGlobal.h"
#include "inc_FSCommonlib/ListItemDataBase.h"
#include "inc_FSCommonlib/FSNLETimeline.h"
#include "inc_FSMatterView/FSMatterView.h"
#include "FSMatterViewPrivate.h"
#include "MVListItemData.h"

//////////////////////////////////////////////////////////////////////////
MediaItemWidget::MediaItemWidget(QWidget *parent)
	:MVItemWidgetBase(parent)
{
	m_pLbIcon->setVisible(true);
    setObjectName("MediaItemWidget");
    RetranslateUi();

	connect(m_pBtnAdd, &BaseButton::clicked, this, [&]{
		if (!CheckSourceFile())
			emit sigSourceFileLost(this, m_pItemData->GetItemDataBase()->GetTimeline()->GetSourceFilePath());
	});
}

void MediaItemWidget::SetThumbnail(QPixmap px)
{
	m_pLbThumbnail->setPixmap(px);
}

void MediaItemWidget::Resize()
{
	resizeEvent(nullptr);
}

void MediaItemWidget::enterEvent(QEvent *e)
{
	m_pBtnAdd->setVisible(true);
	MVItemWidgetBase::enterEvent(e);
}

void MediaItemWidget::leaveEvent(QEvent *e)
{
	m_pBtnAdd->setVisible(false);
	MVItemWidgetBase::leaveEvent(e);
}

void MediaItemWidget::mousePressEvent(QMouseEvent *e)
{
	MVItemWidgetBase::mousePressEvent(e);
	if(!CheckSourceFile())
		emit sigSourceFileLost(this,m_pItemData->GetItemDataBase()->GetTimeline()->GetSourceFilePath());
	emit ((IFSMatterViewManager*)MatterViewManager::GetInstance())->sigUnselectTimelineView();
}

void MediaItemWidget::RetranslateUi()
{
    m_pBtnAdd->setToolTip(tr("Add to timeline at playhead"));
}

bool MediaItemWidget::CheckSourceFile()
{
	if (m_pItemData->GetItemDataBase()->GetType() == MVItemDataType_Record ||
		m_pItemData->GetItemDataBase()->GetType() == MVItemDataType_Video ||
		m_pItemData->GetItemDataBase()->GetType() == MVItemDataType_Audio ||
		m_pItemData->GetItemDataBase()->GetType() == MVItemDataType_Imgage)
	{
		auto filepath = m_pItemData->GetItemDataBase()->GetTimeline()->GetSourceFilePath();
		if (!QFile::exists(filepath))
		{
			return  false;
		}
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
AnnotationItemWidget::AnnotationItemWidget( QWidget *parent)
	:MediaItemWidget(parent)
{
	m_pLbIcon->setVisible(false);
	m_pLbTitle->setVisible(false);
	SetRenamable(false);
    setAttribute(Qt::WA_StyledBackground);
    setObjectName("AnnotationItemWidget");

	m_favorite = new BaseStateButton(0,"AnnotationItemWidget_Favorite",this);
	m_favorite->AddButton(0, "AnnotationItemWidget_Favorite_AddButton");
	m_favorite->AddButton(1, "AnnotationItemWidget_Favorite_RemoveButton");
    m_favorite->setVisible(false);
    ShowFavorite(false, false);

	connect(m_favorite, &BaseStateButton::Clicked, this, [&](int id,QPoint pt){
        if (id == 0)
            emit sigAddFavorite();
		else if (id == 1)
            emit sigRemoveFavorite();
    });

}

void AnnotationItemWidget::Drag()
{
	emit sigDrag(GetItemData());
}

void AnnotationItemWidget::ShowFavorite(bool show, bool hadFavorite)
{
	m_favorite->SetCurrentButtonId(hadFavorite ? 1 : 0);
    if (!hadFavorite)
        m_favorite->setToolTip(tr("Add to favorite"));
    else
        m_favorite->setToolTip(tr("Remove from favorite"));
}

void AnnotationItemWidget::resizeEvent(QResizeEvent *e)
{
	static const int addBtnW = 24;
	static const int addBtnH = addBtnW;
	static int thumbnailW = 60;
	static int thumbnailH = 60;
	m_pLbThumbnail->setGeometry((width() - thumbnailW) * 0.5, (height() - thumbnailH) * 0.5 , thumbnailW, thumbnailH);
	m_pLbThumbnail->setPixmap(GetAspectPixmap(m_pItemData->GetItemDataBase()->GetThumbnail(), m_pLbThumbnail->geometry().width(), m_pLbThumbnail->geometry().height(),false));

	int l = (width() - addBtnW) * 0.5;
	int t = (height() - addBtnH) * 0.5;
	m_pBtnAdd->setGeometry(l, t, addBtnW, addBtnH);

	auto rc = geometry();
	m_pSelectedWidget->setGeometry(0,0,width(),height());

    const int favoriteWidth = 18;
    m_favorite->setGeometry(width() - favoriteWidth - 10, 10, favoriteWidth, favoriteWidth);

}

void AnnotationItemWidget::enterEvent(QEvent *e)
{
    m_pBtnAdd->setVisible(true);
    m_favorite->setVisible(true);
}

void AnnotationItemWidget::leaveEvent(QEvent *e)
{
    m_pBtnAdd->setVisible(false);
    m_favorite->setVisible(false);

}


void AnnotationItemWidget::mousePressEvent(QMouseEvent *e)
{
    MediaItemWidget::mousePressEvent(e);

    
}

void AnnotationItemWidget::mouseReleaseEvent(QMouseEvent *e)
{
    MediaItemWidget::mouseReleaseEvent(e);

    if (e->button() == Qt::RightButton)
    {
        emit sigMenuPopup(e->globalPos());
    }
}

//////////////////////////////////////////////////////////////////////////
MouseEffectItemWidget::MouseEffectItemWidget(QWidget *parent)
	:MVItemWidgetBase(parent)
{
	m_pLbIcon->setVisible(false);
	m_pBtnAdd->setVisible(false);
	SetRenamable(false);
    setObjectName("MouseEffectItemWidget");
}

void MouseEffectItemWidget::Drag()
{
	auto data = new QMimeData;
	data->setData("MatterView_MouseEffect", "");
	auto timeline = m_pItemData->GetItemDataBase()->GetTimeline();
	FSNLETimeline *cloned = nullptr;
	if (!timeline)
	{
		data->setData("MatterView_MouseEffect_ClickSound", "");

		ClickSoundData* pData = qobject_cast<ClickSoundData*>(m_pItemData->GetItemDataBase());
		Q_ASSERT(pData);
		data->setProperty("MatterView_MouseEffect_ClickSound_File", pData->GetSoundFile());
		
	}
	else
	{
		data->setData("MatterView_MouseEffect_Filter", "");
		cloned = timeline->Clone();
		cloned->SetRenderFrameRate(25);
		data->setProperty("MatterView_MouseEffect_FilterTimeline", qptrdiff(cloned));
	}
	QDrag drag(this);
	drag.setMimeData(data);
	if (drag.exec() == Qt::IgnoreAction)
	{
		if(cloned) cloned->deleteLater();
	}
}

void MouseEffectItemWidget::resizeEvent(QResizeEvent *e)
{
	MVItemWidgetBase::resizeEvent(e);
	m_pLbTitle->setGeometry(0, m_pLbThumbnail->geometry().bottom() + 1, width(), m_pLbTitle->height());
}


void MouseEffectItemWidget::mousePressEvent(QMouseEvent *e)
{
	MVItemWidgetBase::mousePressEvent(e);
	emit((IFSMatterViewManager*)MatterViewManager::GetInstance())->sigUnselectTimelineView();
}

//////////////////////////////////////////////////////////////////////////
VideoItemWidget::VideoItemWidget(QWidget *parent)
	:MediaItemWidget(parent)
{
	m_pLbTitle->SetEditable(true);
}

void VideoItemWidget::Drag()
{
	emit sigDrag(GetItemData());
}

//////////////////////////////////////////////////////////////////////////
AudioItemWidget::AudioItemWidget(QWidget *parent)
	:MediaItemWidget(parent)
{
	m_pLbTitle->SetEditable(true);
}

void AudioItemWidget::Drag()
{
	emit sigDrag(GetItemData());
}

//////////////////////////////////////////////////////////////////////////
ImageItemWidget::ImageItemWidget(QWidget *parent)
	:MediaItemWidget(parent)
{
	m_pLbTitle->SetEditable(true);
}

void ImageItemWidget::Drag()
{
	emit sigDrag(GetItemData());
}

//////////////////////////////////////////////////////////////////////////
RecordItemWidget::RecordItemWidget(QWidget *parent)
	:MediaItemWidget(parent)
{
	m_pLbTitle->SetEditable(true);
}

void RecordItemWidget::Drag()
{
	emit sigDrag(GetItemData());
}

