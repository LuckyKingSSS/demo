#ifndef MAINCONTROLER_H
#define MAINCONTROLER_H

#include <QObject>
#include "inc_FSCommonlib/FSCommonWidget.h"
class FSMatterView;
class FSTimelineView;
class IFSEditMediaPlayer;
class IFSPropertyView;
class ITimelineViewManager;
class IPlayerEditorManager;
class FSMeidaPlayerBar;
class FSMainTitleBar;
class MainControler : public QObject
{
	Q_OBJECT

public:
	MainControler(FSMatterView *matterview,
		FSTimelineView *timelineview,
		IFSEditMediaPlayer *mediaplayerview,
		IFSPropertyView *propertyview,
        FSMeidaPlayerBar *pMediaPayerBar,
		FSMainTitleBar *pMainTitleBar,
		QObject *parent);
	~MainControler();
protected:
	void ConnectSignals();
private:
	FSMatterView *m_pMatterView;
	FSTimelineView *m_pTimelineView;
	IFSEditMediaPlayer *m_pEditMediaPlayerView;
	IFSPropertyView *m_pPropertyView;
    FSMeidaPlayerBar *m_pMediaPayerBar;
	ITimelineViewManager* m_pTimelineViewManager;
	IPlayerEditorManager* m_pPlayerEditorManager;
	FSMainTitleBar *m_pMainTitleBar;
};

class FSNLETimeline;
class IFSMediaPlayer;
class TopLevelWindowTitleBar;
class MVPlayer : public FSBaseDialog
{
	Q_OBJECT
public:
	MVPlayer(const QString &trtitle, const char *title, QWidget *parent);
	void SetSrcTimeline(FSNLETimeline *timeline);
protected:
	void closeEvent(QCloseEvent *e) override;
	void showEvent(QShowEvent *e) override;
	void resizeEvent(QResizeEvent *e) override;
protected:
	IFSMediaPlayer *m_pIFSMediaPlayer;
	FSNLETimeline *m_pTimeline;
	TopLevelWindowTitleBar *m_pTitleBar;
};

#endif // MAINCONTROLER_H
