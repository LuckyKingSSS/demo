#ifndef FSTITLEBARCONTROL_H
#define FSTITLEBARCONTROL_H

#include <QObject>
class FSMainTitleBar;
class FSMatterView;
class FSTimelineView;
class FSEditor;
class RegisterMain;
class UpdateMain;
class CBSUrlReader;
class IFSEditMediaPlayer;
class FSTitleBarControl : public QObject
{
	Q_OBJECT

public:
	FSTitleBarControl(FSMainTitleBar *titleBar,
	FSMatterView *matterview,
	IFSEditMediaPlayer *mediaPlayerView,
	FSTimelineView *timelineview,
	QObject *parent);
	~FSTitleBarControl();
	void SetEditorView(FSEditor *view);
	void LoadProjFromMenu(const QString &projfile);
	void RebuildRegisterMain(bool bTellRecorder = false);
Q_SIGNALS:
	void sigCreateVideo();
	void sigClose();

public slots:
	void OnRegistered(bool bSucc);
protected:
	bool IsRecorderProj(const QString &projfile);
	void OnActionTriggled(int id, bool checked);
	void OnMenuEditAbout2Show(bool bOnlyTimeline = false);
	void OnMenuFileAbout2Show();
	void OnMenuRecentOpenAbout2Show();
	void OnMenuExportAbout2Show();
	void OnRegister();
	void OnPuschase();
	void OnCheckUpdate();
	void OnBeginInstall();
	void OnInstallStarted();
private:
	FSMainTitleBar *m_pTitleBar;
	FSMatterView *m_pMatterView;
	IFSEditMediaPlayer *m_pMediaplayView;
	FSTimelineView *m_pTimelineView;
	FSEditor *m_pEditorView;

	RegisterMain *m_pRegisterMain;
	UpdateMain *m_pUpdateMain;
	CBSUrlReader *m_pCBSUrlReader;

	bool m_bFocusInTimeline;
	bool m_bOnlyDeleteTimeline;
};


#endif // FSTITLEBARCONTROL_H
