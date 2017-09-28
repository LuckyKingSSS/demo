#ifndef FSMATTERVIEWPRIVATE_H
#define FSMATTERVIEWPRIVATE_H

#include <QWidget>
#include "inc_FSCommonlib/FSCommonWidget.h"
#include "inc_FSMatterView/FSMatterView.h"
class CategoryBar;
class MediaPage;
class AnnotationPage;
class MouseEffectPage;
class IFSMatterViewManager;
class FSMatterViewPrivate : public FSBaseWidget
{
	Q_OBJECT
public:
	FSMatterViewPrivate(QWidget *parent);
	~FSMatterViewPrivate();
	IFSMatterViewManager *GetMatterViewManager();
	QString GetProjectString();
	void LoadProjectString(QString strMatter);

	MediaPage* GetMediaPage();
	void UnselectAll();
protected:
	void ConnectSignals();
	bool eventFilter(QObject *, QEvent *) override;
	void resizeEvent(QResizeEvent *e) override;

    void RetranslateUi() override;
    void SetShowHideButtonState(bool showState);
private:
	CategoryBar *m_pCategoryBar;
	MediaPage *m_pMediaPage;
	AnnotationPage *m_pAnnotationPage;
	MouseEffectPage *m_pMouseEffectPage;
	QStackedWidget *m_pStackedWidget;

	IFSMatterViewManager *m_pMatterViewManager;

    FSStateBaseButton *m_pBtnShowHideMatterView;
};

class MVListWidget;
class BaseButton;
class FSNLETimeline;
class MVListItemData;
struct MediaConfInfo;
class MediaPage : public FSBaseWidget
{
	Q_OBJECT
public:
	MediaPage(QWidget *parent);
	QString GetConfigString();
	void LoadProjectString(QString strMatter);
	bool CanDelete();
	void Delete();
	void UnselectAllItem();
	QPixmap Get1stItemPixmap();
Q_SIGNALS:
	void sigActivate();
protected:
	void ImportMediaFiles();
	void ImportMediaFolder();
	void ConnectSignals();
	void InitList();
	void InsertNewItem();
	void AddVideoItem(const MediaConfInfo &fileinfo, FSNLETimeline *timeline);
	void AddAudioItem(const MediaConfInfo &fileinfo, FSNLETimeline *timeline);
	void AddImageItem(const MediaConfInfo &fileinfo, FSNLETimeline *timeline);
	void AddRecordItem(const MediaConfInfo &fileinfo, FSNLETimeline *timeline);
	void AddItems(QStringList files);
	void AddItems(QList<MediaConfInfo> files);
	void CheckFiles(QStringList &files);
	void DeleteSelectedItems();
	QStringList GetItemSourceFiles();
	void RetranslateUi() override;
	void resizeEvent(QResizeEvent *e) override;
    QString GetPreOpenDir();
    void SavePreOpenDir(QString);
protected slots:
void OnFilterAll();
void OnFilterVideo();
void OnFilterAudio();
void OnFilterImage();
void OnItemDrag(MVListItemData *pData);

protected:
	enum SortAction
	{
		SortAction_Name = 0,
		SortAction_Duration,
		SortAction_Type,
		SortAction_DateCreate,
		SortAction_Ascending,
		SortAction_Descending,
	};
	void SetSortActionChecked(SortAction sa);

	enum FilterAction
	{
		FilterAction_All = 0,
		FilterAction_Video,
		FilterAction_Audio,
		FilterAction_Image,
	};
	void SetFilterActionChecked(FilterAction fa);
	void UpdateButtons();
protected slots:
	void SortByName();
	void SortByDuration();
	void SortByType();
	void SortByDateCreate();
	void SortByCurrentAction();
	void DeleteItemConfirm();
	void DisableDeleteItemConfirm();
private:
	MVListWidget *m_pListWidget;
	QSortFilterProxyModel *m_pSortFilterModel;
	BaseButton *m_pBtnImport;
	BaseButton *m_pBtnFilter;
	BaseButton *m_pBtnSort;
	
	QAction *m_pActImportFile;
	QAction *m_pActImportFolder;
	QAction *m_pActFilterAll;
	QAction *m_pActFilterVideo;
	QAction *m_pActFilterAudio;
	QAction *m_pActFilterImage;
	QAction *m_pActSortName;
	QAction *m_pActSortDuration;
	QAction *m_pActSortType;
	QAction *m_pActSortDateCreated;
	QAction *m_pActSortAscending;
	QAction *m_pActSortDescending;

	QAction *m_pActAdd2Timeline;
	QAction *m_pActRename;
	QAction *m_pActDelete;
    QAction *m_pActDetails;
    QAction *m_pActFindTarget;
	QMenu *m_pMenuImport;
	QMenu *m_pMenuFilter;
	QMenu *m_pMenuSort;
	QMenu *m_pMenuItemRClicked;

	QString m_strConfig;
	QString m_strPreOpenDir;
	MVListItemData *m_pCurClickItem;
	SortAction m_curSortAction;
	bool m_bDeleteConfirm;

	QString m_strFileLostTip;

    FSLabel *m_lbTitle;

};

struct MediaConfInfo
{
	QString path;
	QString title;
};


struct OverlayData
{
	QString name;
	QString resfile;
	QString thumbnailfile;
};

struct CaptionData
{
	QString name;
	QString resfile;
	QString thumbnailfile;
	bool canEditText;
};

class AnnotationDataHelper;
class AnnotationData;
class AnnotationItemData;
class AnnotationPage : public FSBaseWidget
{
	Q_OBJECT
public:
	AnnotationPage(QWidget *parent);
	void UnselectAllItem();
protected:
	void ConnectSignals();
	void InitList();
	void InitAnotations();
	void InitOverlays();

    void AddItem(AnnotationItemData *item);
    void resizeEvent(QResizeEvent *e) override;
    void RetranslateUi() override;
    void TranslateText() override;
protected slots:
	void OnItemDrag(MVListItemData *pData);
    void OnShowItemFromCatalog(QString catalog);
    void OnMenuPopup(QPoint point);
    void OnAddFavorite(bool add);
private:
	MVListWidget *m_pListWidget;
    FSLabel *m_lbTitle;
    FSComboBox *m_pCombCatalog;


    AnnotationDataHelper *m_dataHelper;
};

class MouseEffectPage : public FSBaseWidget
{
	Q_OBJECT
public:
	MouseEffectPage(QWidget *parent);
	void UnselectAllItem();
protected:
	void ConnectSignals();
	void InitList();

	void InitHightline();
	void InitLeftClick();
	QString GetSoundFile();
	void RetranslateUi() override;
	void resizeEvent(QResizeEvent *e) override;
protected slots:
	void OnComboxIndextChanged(int);
private:
	QLabel *m_pLbType;
    FSComboBox *m_pCombType;
	MVListWidget *m_pListWidget;
    FSLabel *m_lbTitle;
};


class MVConfirmDlg : public FSMessageBox
{
	Q_OBJECT
public:
	MVConfirmDlg(QWidget *parent);
	static std::tuple<bool, FSMessageBox::Button> question(QWidget *parent);
protected:
	bool IsCheck();
	void resizeEvent(QResizeEvent *e) override;
protected:
	QCheckBox *m_pCbDontShow;
	QLabel *m_pLbDonShow;
};


class MatterViewManager : public IFSMatterViewManager
{
	Q_OBJECT

public:
	static MatterViewManager* GetInstance();
	void SetFSMatterViewPrivate(FSMatterViewPrivate* pFSMatterViewPrivate);

	void ImportMediaFiles() override;
	void ImportMediaFolder() override;
	void UnselectAll() override;
	bool CanDelete() override;
	void Delete() override;

	QString GetProjectString() override;
	void LoadProjectString(QString strMatter) override;
	void Clear() override;
	QPixmap Get1stItemPixmap() override;
Q_SIGNALS:
	void sigCBImportMediaFiles();
	void sigCBImportMediaFolder();
private:
	MatterViewManager(QObject *parent = 0);
	FSMatterViewPrivate* m_pFSMatterViewPrivate;
};


class FSPageTitle : public FSLabel
{
    Q_OBJECT
public:
    FSPageTitle(QWidget *parent) :FSLabel(parent)
    {
        setObjectName("FSMatterViewPrivate_Page_Title");
        setAlignment(Qt::AlignCenter);
    }
};

#endif // FSMATTERVIEWPRIVATE_H
