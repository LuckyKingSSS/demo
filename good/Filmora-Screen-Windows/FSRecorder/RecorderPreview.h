#pragma once

#include "inc_FSCommonlib/FSCommonWidget.h"
#include "inc_FSCommonlib/FSNLETimeline.h"

#include "Include/NLEComPtr.h"

#include "inc_FSCommonlib/FSListWidget.h"
#include "inc_FSCommonlib/MediaImportDlg.h"
#include "inc_FSCommonlib/ListWidgetBase.h"
class IFSMediaPlayer;
class PreivewListWidget;
class RecorderPreview : public FSBaseWidget
{
	Q_OBJECT

public:
	RecorderPreview(QWidget *parent);
	~RecorderPreview();

	void AddItem(QString path);
	void SetCurrentItem(QString path);
	void Reset();
	QPixmap GetFirstItemThumbnail();
Q_SIGNALS:
	void signalCaptureMore();
	void signalExport2Editor(QList<QString> paths);
	void signalExport2Local(QList<QString> paths);

protected:
	void resizeEvent(QResizeEvent *event);
	void showEvent(QShowEvent *event);
    void hideEvent(QHideEvent *event);

protected:
	void RetranslateUi() override;
	void SetExportButtonEnabled(bool isItemSelect);
	void RemoveItems(QList<QString> paths);

private slots:
	void ExportCurrentIndexChanged();

private:
	PreivewListWidget *m_previewList;
	IFSMediaPlayer *m_playerView;
    FSComboBox *m_cmbExport;
	FSButton *m_btnGo;
    QShortcut *m_spaceShortcut;

	FSButton *m_btnAdd;


};

class PreviewItemData;

class ListWidget;
class RecordingPreviewDataManager;
class PreivewListWidget : public FSBaseWidget
{
	Q_OBJECT
public:
	PreivewListWidget(QWidget *parent);
	~PreivewListWidget();

	bool IsItemSelected();
	QList<QString> GetSelectedPaths();
	QList<QString> GetAllPaths();
	void ClearSelectedItems();
	void ClearItems();
	void AddItem(QString path);
	void RemoveItem(QString path);
	void SetCurrentItem(QString path);

Q_SIGNALS:
	void signalLoadTimeline(QString path, FSNLETimeline *timeline);
	void signalCaptureMore();
	void signalItemSelectedChanged(bool isItemSelected);
    void signalDeleteItem();

private:
	void AddItem(PreviewItemData * data);
	FSNLETimeline* CreateNLETimeline(QString path);
    Q_INVOKABLE void LoadProjectItems();
    void DeleteSelectionItem();
protected:
	void resizeEvent(QResizeEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void showEvent(QShowEvent *event);
    void RetranslateUi();
    void paintEvent(QPaintEvent *event);

private:
	ListWidget *m_listWidget;
	RecordingPreviewDataManager *m_manager;
	bool m_needLoad;
    QString m_needLoadSetCurrentPath;
	FSNLETimeline *m_timeline;


    QAction *m_actDetails;
    QAction *m_actFindTarget;
    QAction *m_actDelete;

    QString m_recordGamePath;
    QShortcut *m_deleteShortcut;

};

class SelectedWidget;
class CaptureMoreItem : public FSBaseWidget
{
	Q_OBJECT
public:
	CaptureMoreItem(QWidget *parent);
protected:
	void resizeEvent(QResizeEvent *event);
    void RetranslateUi();

private:
	FSLabel *m_lbMore;
	FSLabel *m_lbMoreText;
};

class PreviewItem : public FSBaseWidget
{
	Q_OBJECT
public:

	PreviewItem(PreviewItemData *data, QWidget *parent);
	~PreviewItem();

	void SetState(SelectedWidget::State s);
	PreviewItemData* GetData();

protected:
	void resizeEvent(QResizeEvent *event);
	void paintEvent(QPaintEvent *event);
	void enterEvent(QEvent *e) override;
	void leaveEvent(QEvent *e) override;


protected:
    FSButton *m_title;
	QLabel *m_time;
	QLabel *m_icon;
	PreviewItemData *m_data;
	SelectedWidget* m_pSelectedWidget;


};

class PreviewItemData
{
public:
	QString name;
	QString path;
	FSNLETimeline *timeline;
	QPixmap icon;
	QString duration;

public:
	PreviewItemData(){
		timeline = nullptr;
		duration = QString("00:00:00");
	}
	~PreviewItemData(){
		SAFE_DELETE(timeline);
	}
};
typedef QList<PreviewItemData*> PreviewItemDatas;

class RecordingPreviewDataManager : public QObject
{
	Q_OBJECT
public:
	static RecordingPreviewDataManager* GetInstance();
	~RecordingPreviewDataManager();

	QString GetSettingString();
	QStringList GetAllPaths();

public:
	PreviewItemDatas GetItemDatas();
	void AddData(PreviewItemData *data);
	void RemoveData(PreviewItemData *data,bool bSetContentChanged = true);
    void Clear();
Q_SIGNALS:
	void signalLoad();

protected:
	RecordingPreviewDataManager(QObject *parent = 0);
private:
	void LoadProjectSetting(QString xml);
private:
	PreviewItemDatas m_datas;
};



class PreviewMediaImportDlg : public MediaImportDlg
{
	Q_OBJECT
public:
	PreviewMediaImportDlg(const QStringList &list, PreviewItemDatas datas, QWidget *parent);

Q_SIGNALS:
	void signalImport(PreviewItemData *data);
private:
	PreviewItemDatas m_datas;
};