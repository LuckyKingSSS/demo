#ifndef FMATTERICONVIEW_H
#define FMATTERICONVIEW_H

#include "inc_FCommonLib/FBaseWidget.h"

class FButton;
class FImageButton;
class FLineEdit;
class FMenu;
class FLabel;

/*FMatterIconView*/
class FMatterTitleBar;
class FImportMediaView;
class QStackedWidget;
class FMatterIconView : public FBaseWidget
{
	Q_OBJECT

public:
	FMatterIconView(QWidget *parent = nullptr);
	~FMatterIconView();

protected:
	void resizeEvent(QResizeEvent *) override;

private:
	FMatterTitleBar *m_pTitleBar;
	QStackedWidget *m_pStackedWidget;
	FImportMediaView *m_pImportView;
};

/*FMatterTitleBar*/
class FTypeButton;
class FMatterTitleBar : public FBaseWidget
{
	Q_OBJECT

public:
	FMatterTitleBar(QWidget* parent = nullptr);
	~FMatterTitleBar();

	void SetLeftButtonsVisible(bool);

Q_SIGNALS:
	// import
	void sigImportMediaFiles();
	void sigImportAMeidaFolder();
	void sigImportFromCameraOrPhone();
	void sigDownloadMediaFromFacebook();
	void sigDownloadPhotosFromInstagram();
	void sigDownloadPhotosFromFlickr();

	// record
	void sigRecordFromWebcam();
	void sigRecordPCScreen();
	void sigRecordAVoiceover();

	// type
	void sigTypeAll();
	void sigTypeVideo();
	void sigTypeImage();
	void sigTypeAudio();

	// sort
	void sigSortByName();
	void sigSortByDuration();
	void sigSortByType();
	void sigSortByDateCreated();
	void sigSortByAscending();
	void sigSortByDescending();

	// thumbnail size
	void sigLargeThumbnailSize();
	void sigMiddleThumbnailSize();
	void sigSmallThumbnailSize();

	// search bar
	void sigSearchBarChanged(const QString&);

protected:
	void resizeEvent(QResizeEvent *) override;

private:
	void ConnectSignals();

private:
	FButton *m_pImportBtn;
	FButton *m_pRecordBtn;
	FTypeButton *m_pTypeBtn;
	FImageButton *m_pSortBtn;
	FImageButton *m_pThumbnailBtn;
	FLineEdit *m_pSearchBar;
	FMenu *m_pImportMenu;
	FMenu *m_pRecordMenu;
	FMenu *m_pTypeMenu;
	FMenu *m_pSortMenu;
	FMenu *m_pThumbnailMenu;
	QList<QAction*> m_ImportMenuActs;
	QList<QAction*> m_RecordMenuActs;
	//QList<QAction*> m_TypeMenuActs;
	QActionGroup *m_pTypeActionGroup;
	QAction *m_pTypeAllAct;
	QAction *m_pTypeVideoAct;
	QAction *m_pTypeImageAct;
	QAction *m_pTypeAudioAct;
	//QList<QAction*> m_SortMenuActs;
	QActionGroup *m_pSortActionGroup;
	QAction *m_pSortByNameAct;
	QAction *m_pSortByDurationAct;
	QAction *m_pSortByTypeAct;
	QAction *m_pSortByDateCreatedAct;
	QActionGroup *m_pSortOrderActionGroup;
	QAction *m_pAscendingAct;
	QAction *m_pDescendingAct;
	//QList<QAction*> m_ThumbnailMenuActs;
	QActionGroup *m_pThumbnailSizeActionGroup;
	QAction *m_pLargeThumbnailAct;
	QAction *m_pMiddleThumbnailAct;
	QAction *m_pSmallThumbnailAct;
	QIcon m_Icon;
};

/*FTypeButton*/
class FTypeButton : public FBaseWidget
{
	Q_OBJECT

public:
	FTypeButton(int id = 0, QWidget* parent = nullptr);
	~FTypeButton();

	void SetImage(const QString&);

Q_SIGNALS:
	void clicked(int);

protected:
	void mousePressEvent(QMouseEvent *) override;
	void paintEvent(QPaintEvent *) override;

private:
	int m_ID;
	QString m_ImagePath;
};

/*FImportMediaView*/
class FImportMediaView : public FBaseWidget
{
	Q_OBJECT

public:
	FImportMediaView(QWidget *parent = nullptr);
	~FImportMediaView();

	void SetText(const QString&);
	void SetImage(const QString&);

Q_SIGNALS:
	void clicked();

protected:
	void resizeEvent(QResizeEvent *);
	void paintEvent(QPaintEvent *);
	void enterEvent(QEvent *);
	void leaveEvent(QEvent *);

private:
	QRect m_ImageRect;
	QString m_ImagePath;
	FLabel *m_pTextLbl;
};

#endif // FMATTERICONVIEW_H
