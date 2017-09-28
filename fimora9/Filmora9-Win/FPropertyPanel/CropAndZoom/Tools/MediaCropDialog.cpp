#include "stdafx.h"
#include "MediaCropDialog.h"
#include "inc_MediaPlayerView/MediaPlayerView.h"

#include "inc_CommonLib/TopLevelWindowTitleBar.h"


MediaCropDialog::MediaCropDialog(NLETimeline *pDestTimeLine, QWidget *parent)
	: QDialog(parent)
{

	resize(960, 600);
	setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

	m_titleBar = new TopLevelWindowTitleBar(this);
	m_titleBar->SetTitleName(tr("Crop"));
	m_titleBar->SetButtons(TopLevelWindowTitleBar::Close);
	m_titleBar->SetDragMaximum(false);
	m_titleBar->SetResizeble(false);

	m_pCropView = new MediaCropView(pDestTimeLine,this);
	connect(m_pCropView, &MediaCropView::signalOK, this, [&](){
		done(1);
	});
	connect(m_pCropView, &MediaCropView::signalCancel, this, [&](){
		done(0);
	});
}

MediaCropDialog::~MediaCropDialog()
{
	
}

void MediaCropDialog::resizeEvent(QResizeEvent *event)
{
	const int margins = 3;
	int widthWidth = width() - 2 * margins;
	m_titleBar->setGeometry(margins, margins, widthWidth, 22);
	int top = m_titleBar->geometry().bottom();
	m_pCropView->setGeometry(margins, top, widthWidth, height() - 22 - 2 *margins);
}

int MediaCropDialog::DoModal()
{
	return exec();
}

