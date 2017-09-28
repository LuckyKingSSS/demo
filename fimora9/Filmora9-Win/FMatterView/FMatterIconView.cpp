#include "stdafx.h"
#include "inc_FCommonLib/FCommonWidgets.h"
#include "FMatterIconView.h"
#include "FMatterThumbnail.h"
#include <QPainter>

/*FMatterIconView*/
FMatterIconView::FMatterIconView(QWidget *parent /*= nullptr*/)
	: FBaseWidget(parent)
{
	setFocusPolicy(Qt::ClickFocus);
	m_pTitleBar = new FMatterTitleBar(this);
	m_pStackedWidget = new QStackedWidget(this);
	m_pImportView = new FImportMediaView(0);
	m_pStackedWidget->addWidget(m_pImportView);
	//auto Thumbnail = new FMatterThumbnailBase;
	//auto Thumbnail = new FMediaThumbnail;
	auto Thumbnail = new FThumbnailDownload;
	Thumbnail->SetTitle("Wildlife");
	m_pStackedWidget->addWidget(Thumbnail);
	m_pStackedWidget->setCurrentWidget(Thumbnail);
	//m_pStackedWidget->setCurrentWidget(m_pImportView);
	m_pImportView->SetImage(":/FMatterView/Resources/Import Files.png");
	m_pImportView->SetText(tr("Import Media Files Here"));
}

FMatterIconView::~FMatterIconView()
{

}

void FMatterIconView::resizeEvent(QResizeEvent *)
{
	int left = 0, top = 0;
	const int TitleBarHeight = 25;
	m_pTitleBar->setGeometry(left, top, width(), TitleBarHeight);
	top = m_pTitleBar->geometry().bottom() + 5;
	m_pStackedWidget->setGeometry(left, top, width(), height() - top);
}

/*FMatterTitleBar*/
FMatterTitleBar::FMatterTitleBar(QWidget* parent /*= nullptr*/)
	:FBaseWidget(parent)
	, m_Icon(":/FMatterView/Resources/favorite_off.png")
{
	m_pImportBtn = new FButton(this);
	m_pRecordBtn = new FButton(this);
	m_pTypeBtn = new FTypeButton(0, this);
	m_pSortBtn = new FImageButton(0, "FImageButton_Sort", this);
	m_pThumbnailBtn = new FImageButton(1, "FImageButton_Thumbnail", this);
	m_pSearchBar = new FLineEdit(this);

	m_pImportMenu = new FMenu(this);
	m_pRecordMenu = new FMenu(this);
	m_pTypeMenu = new FMenu(this);
	m_pSortMenu = new FMenu(this);
	m_pThumbnailMenu = new FMenu(this);

	m_ImportMenuActs.append(m_pImportMenu->addAction(tr("Import Media Files...")));
	m_ImportMenuActs.append(m_pImportMenu->addAction(tr("Import a Media Folder...")));
	m_ImportMenuActs.append(m_pImportMenu->addAction(tr("Import from a Camera or a Phone...")));
	m_pImportMenu->addSeparator();
	m_ImportMenuActs.append(m_pImportMenu->addAction(tr("Download Media from Facebook...")));
	m_ImportMenuActs.append(m_pImportMenu->addAction(tr("Download Photos from Instagram...")));
	m_ImportMenuActs.append(m_pImportMenu->addAction(tr("Download Photos from Flickr...")));

	m_RecordMenuActs.append(m_pRecordMenu->addAction(tr("Record from Webcam...")));
	m_RecordMenuActs.append(m_pRecordMenu->addAction(tr("Record PC Screen...")));
	m_RecordMenuActs.append(m_pRecordMenu->addAction(tr("Record a voiceover...")));

	m_pTypeActionGroup = new QActionGroup(this);
	m_pTypeAllAct = m_pTypeMenu->addAction(tr("All"));
	m_pTypeActionGroup->addAction(m_pTypeAllAct);
	m_pTypeVideoAct = m_pTypeMenu->addAction(tr("Video"));
	m_pTypeActionGroup->addAction(m_pTypeVideoAct);
	m_pTypeImageAct = m_pTypeMenu->addAction(tr("Image"));
	m_pTypeActionGroup->addAction(m_pTypeImageAct);
	m_pTypeAudioAct = m_pTypeMenu->addAction(tr("Audio"));
	m_pTypeActionGroup->addAction(m_pTypeAudioAct);
	m_pTypeActionGroup->setExclusive(true);

	m_pSortActionGroup = new QActionGroup(this);
	m_pSortByNameAct =  m_pSortMenu->addAction(tr("Name"));
	m_pSortActionGroup->addAction(m_pSortByNameAct);
	m_pSortByDurationAct = m_pSortMenu->addAction(tr("Duration"));
	m_pSortActionGroup->addAction(m_pSortByDurationAct);
	m_pSortByTypeAct = m_pSortMenu->addAction(tr("Type"));
	m_pSortActionGroup->addAction(m_pSortByTypeAct);
	m_pSortByDateCreatedAct = m_pSortMenu->addAction(tr("Date Created"));
	m_pSortActionGroup->addAction(m_pSortByDateCreatedAct);
	m_pSortMenu->addSeparator();
	m_pSortActionGroup->setExclusive(true);

	m_pSortOrderActionGroup = new QActionGroup(this);
	m_pAscendingAct = m_pSortMenu->addAction(tr("Ascending"));
	m_pSortOrderActionGroup->addAction(m_pAscendingAct);
	m_pDescendingAct = m_pSortMenu->addAction(tr("Descending"));
	m_pSortOrderActionGroup->addAction(m_pDescendingAct);
	m_pSortOrderActionGroup->setExclusive(true);

	m_pThumbnailSizeActionGroup = new QActionGroup(this);
	m_pLargeThumbnailAct = m_pThumbnailMenu->addAction(tr("Large Thumbnail"));
	m_pThumbnailSizeActionGroup->addAction(m_pLargeThumbnailAct);
	m_pMiddleThumbnailAct = m_pThumbnailMenu->addAction(tr("Middle Thumbnail"));
	m_pThumbnailSizeActionGroup->addAction(m_pMiddleThumbnailAct);
	m_pSmallThumbnailAct = m_pThumbnailMenu->addAction(tr("Small Thumbnail"));
	m_pThumbnailSizeActionGroup->addAction(m_pSmallThumbnailAct);
	m_pThumbnailSizeActionGroup->setExclusive(true);

	m_pImportBtn->setText(tr("IMPORT"));
	m_pRecordBtn->setText(tr("RECORD"));
	m_pSearchBar->setText(tr("Search"));

	ConnectSignals();
	m_pTypeAllAct->trigger();
	m_pSortByNameAct->trigger();
	m_pAscendingAct->trigger();
	m_pLargeThumbnailAct->trigger();
}

FMatterTitleBar::~FMatterTitleBar()
{

}

void FMatterTitleBar::SetLeftButtonsVisible(bool bVisible)
{
	m_pImportBtn->setVisible(bVisible);
	m_pRecordBtn->setVisible(bVisible);
	m_pTypeBtn->setVisible(bVisible);
	m_pSortBtn->setVisible(bVisible);
}

void FMatterTitleBar::resizeEvent(QResizeEvent *)
{
	int left = 0, top = 0;
	m_pImportBtn->setGeometry(left, top, 80, height());
	left = m_pImportBtn->geometry().right() + 30;
	m_pRecordBtn->setGeometry(left, top, 80, height());
	left = width() - 90;
	m_pSearchBar->setGeometry(left, top, 90, height());
	left = left - (5 + 20);
	m_pThumbnailBtn->setGeometry(left, top, 20, height());
	left = left - (5 + 20);
	m_pSortBtn->setGeometry(left, top, 20, height());
	left = left - (5 + 20);
	m_pTypeBtn->setGeometry(left, top, 20, height());
}

void FMatterTitleBar::ConnectSignals()
{
	connect(m_pImportBtn, &FButton::clicked, this, [&](){
		m_pImportMenu->exec(mapToGlobal(m_pImportBtn->geometry().bottomLeft()));
	});
	connect(m_pRecordBtn, &FButton::clicked, this, [&](){
		m_pRecordMenu->exec(mapToGlobal(m_pRecordBtn->geometry().bottomLeft()));
	});
	connect(m_pSortBtn, &FImageButton::clicked, this, [&](){
		m_pSortMenu->exec(mapToGlobal(m_pSortBtn->geometry().bottomLeft()));
	});
	connect(m_pTypeBtn, &FTypeButton::clicked, this, [&](){
		m_pTypeMenu->exec(mapToGlobal(m_pTypeBtn->geometry().bottomLeft()));
	});
	connect(m_pThumbnailBtn, &FImageButton::clicked, this, [&](){
		m_pThumbnailMenu->exec(mapToGlobal(m_pThumbnailBtn->geometry().bottomLeft()));
	});

	connect(m_pTypeActionGroup, &QActionGroup::triggered, this, [&](QAction* pAction){
		for (auto pAct : m_pTypeActionGroup->actions()){
			pAct == pAction ? pAct->setIcon(m_Icon) : pAct->setIcon(QIcon());
		}
		if (pAction == m_pTypeAllAct){
			m_pTypeBtn->SetImage(":/FMatterView/Resources/ALL.png");
		}
		else if (pAction == m_pTypeVideoAct){
			m_pTypeBtn->SetImage(":/FMatterView/Resources/Video.png");
		}
		else if (pAction == m_pTypeImageAct){
			m_pTypeBtn->SetImage(":/FMatterView/Resources/Image.png");
		}
		else if (pAction == m_pTypeAudioAct){
			m_pTypeBtn->SetImage(":/FMatterView/Resources/Audio.png");
		}
	});
	connect(m_pSortActionGroup, &QActionGroup::triggered, this, [&](QAction* pAction){
		for (auto pAct : m_pSortActionGroup->actions()){
			pAct == pAction ? pAct->setIcon(m_Icon) : pAct->setIcon(QIcon());
		}
	});
	connect(m_pSortOrderActionGroup, &QActionGroup::triggered, this, [&](QAction* pAction){
		for (auto pAct : m_pSortOrderActionGroup->actions()){
			pAct == pAction ? pAct->setIcon(m_Icon) : pAct->setIcon(QIcon());
		}
	});
	connect(m_pThumbnailSizeActionGroup, &QActionGroup::triggered, this, [&](QAction* pAction){
		for (auto pAct : m_pThumbnailSizeActionGroup->actions()){
			pAct == pAction ? pAct->setIcon(m_Icon) : pAct->setIcon(QIcon());
		}
	});

	connect(m_ImportMenuActs[0], &QAction::triggered, this, &FMatterTitleBar::sigImportMediaFiles);
	connect(m_ImportMenuActs[1], &QAction::triggered, this, &FMatterTitleBar::sigImportAMeidaFolder);
	connect(m_ImportMenuActs[2], &QAction::triggered, this, &FMatterTitleBar::sigImportFromCameraOrPhone);
	connect(m_ImportMenuActs[3], &QAction::triggered, this, &FMatterTitleBar::sigDownloadMediaFromFacebook);
	connect(m_ImportMenuActs[4], &QAction::triggered, this, &FMatterTitleBar::sigDownloadPhotosFromInstagram);
	connect(m_ImportMenuActs[5], &QAction::triggered, this, &FMatterTitleBar::sigDownloadPhotosFromFlickr);

	connect(m_RecordMenuActs[0], &QAction::triggered, this, &FMatterTitleBar::sigRecordFromWebcam);
	connect(m_RecordMenuActs[1], &QAction::triggered, this, &FMatterTitleBar::sigRecordPCScreen);
	connect(m_RecordMenuActs[2], &QAction::triggered, this, &FMatterTitleBar::sigRecordAVoiceover);

	connect(m_pTypeAllAct, &QAction::triggered, this, &FMatterTitleBar::sigTypeAll);
	connect(m_pTypeVideoAct, &QAction::triggered, this, &FMatterTitleBar::sigTypeVideo);
	connect(m_pTypeImageAct, &QAction::triggered, this, &FMatterTitleBar::sigTypeImage);
	connect(m_pTypeAudioAct, &QAction::triggered, this, &FMatterTitleBar::sigTypeAudio);

	connect(m_pSortByNameAct, &QAction::triggered, this, &FMatterTitleBar::sigSortByName);
	connect(m_pSortByDurationAct, &QAction::triggered, this, &FMatterTitleBar::sigSortByDuration);
	connect(m_pSortByTypeAct, &QAction::triggered, this, &FMatterTitleBar::sigSortByType);
	connect(m_pSortByDateCreatedAct, &QAction::triggered, this, &FMatterTitleBar::sigSortByDateCreated);
	connect(m_pAscendingAct, &QAction::triggered, this, &FMatterTitleBar::sigSortByAscending);
	connect(m_pDescendingAct, &QAction::triggered, this, &FMatterTitleBar::sigSortByDescending);

	connect(m_pLargeThumbnailAct, &QAction::triggered, this, &FMatterTitleBar::sigLargeThumbnailSize);
	connect(m_pMiddleThumbnailAct, &QAction::triggered, this, &FMatterTitleBar::sigMiddleThumbnailSize);
	connect(m_pSmallThumbnailAct, &QAction::triggered, this, &FMatterTitleBar::sigSmallThumbnailSize);
}

/*FTypeButton*/
FTypeButton::FTypeButton(int id /*= 0*/, QWidget* parent /*= nullptr*/)
	:FBaseWidget(parent)
	, m_ID(id)
	, m_ImagePath("")
{

}

FTypeButton::~FTypeButton()
{

}

void FTypeButton::SetImage(const QString& path)
{
	m_ImagePath = path;
	update();
}

void FTypeButton::mousePressEvent(QMouseEvent *)
{
	emit clicked(m_ID);
}

void FTypeButton::paintEvent(QPaintEvent *)
{
	QPainter p(this);
	p.drawPixmap(rect(), QPixmap(m_ImagePath));
}

/*FImportMediaView*/
FImportMediaView::FImportMediaView(QWidget *parent /*= nullptr*/)
	:FBaseWidget(parent)
{
	m_pTextLbl = new FLabel(this);
	m_pTextLbl->setAlignment(Qt::AlignCenter);
}

FImportMediaView::~FImportMediaView()
{

}

void FImportMediaView::SetText(const QString& text)
{
	m_pTextLbl->setText(text);
}

void FImportMediaView::SetImage(const QString& path)
{
	m_ImagePath = path;
	update();
}

void FImportMediaView::resizeEvent(QResizeEvent *)
{
	const int w = 100;
	const int h = 70;
	const int textWdith = 160;
	m_ImageRect = QRect((width() - w) / 2.0, (height() - h) / 2.0, w, h);   // QSize(100, 70)
	int left, top;
	left = (width() - textWdith) / 2.0;
	top = m_ImageRect.bottom() + 10;
	m_pTextLbl->setGeometry(left, top, textWdith, 20);
}

void FImportMediaView::paintEvent(QPaintEvent *)
{
	QPainter p(this);
	p.drawPixmap(m_ImageRect, m_ImagePath);
}

void FImportMediaView::enterEvent(QEvent *)
{
	setCursor(Qt::PointingHandCursor);
}

void FImportMediaView::leaveEvent(QEvent *)
{
	setCursor(Qt::ArrowCursor);
}
