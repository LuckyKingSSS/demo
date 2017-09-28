#ifndef FSMAINTITLEBAR_H
#define FSMAINTITLEBAR_H

#include <QObject>
#include "inc_CommonLib/TopLevelWindowTitleBar.h"
#include "inc_FSCommonlib/FSCommonWidget.h"

class QLabel;
class QMenuBar;
class QMenu;
class FSTitleBarProjectName;
class FSEditor;
class BaseButton;
class FSIconButton;
class FSMainTitleBar : public TopLevelWindowTitleBar
{
	Q_OBJECT
public:
	enum ActionID
	{
		ActionID_File_NewProject = 0,
		ActionID_File_OpenProject,
		ActionID_File_SaveProject,
		ActionID_File_SaveProjectAs,
		ActionID_File_AchiveProject,
		ActionID_File_ImportMediaFiles,
		ActionID_File_ImportMediaFolder,
        ActionID_File_ProjectSettings,
        ActionID_File_Settings,
		ActionID_File_Close,

		ActionID_Edit_Undo,
		ActionID_Edit_Redo,
		ActionID_Edit_Cut,
		ActionID_Edit_Copy,
		ActionID_Edit_Paste,
        ActionID_Edit_Delete,
        ActionID_Edit_SnapToGuides,

		ActionID_Export_CreateVideo,

		ActionID_Help_OnlineHelp,
		ActionID_Help_ProductFAQ,
		ActionID_Help_SupportCenter,
		ActionID_Help_NewFeatureRequest,
		ActionID_Help_CheckForUpdate,
		ActionID_Help_ProductPage,
		ActionID_Help_About,
	};

public:
	void EnableEditShortCut(bool bEnable);
public:
	FSMainTitleBar(FSEditor *parent);
	QMenu* GetMenuFile();
	QMenu* GetMenuEdit();
	QMenu* GetMenuExport();
	QMenu* GetMenuHelp();
	QMenu* GetMenuRecentOpen();

	void SetActionEnabled(ActionID id, bool bEnabled);
	void SetProjectName(const QString &prjName);

	void SetRegistered(bool bRegestered);
Q_SIGNALS:
	void sigActionTriggled(int id, bool checked);
	void sigRegister();
	void sigPuschase();
protected:
	QAction* GetAction(ActionID id);
	void ConnectSignals();
	void RetranslateUi();
	void resizeEvent(QResizeEvent *e) override;
	bool event(QEvent *e) override;
private:
	BaseButton *m_pBtnIcon;
	QMenuBar *m_pMenuBar;
	QMenu	*m_pMenuFile;
	QMenu	*m_pMenuEdit;
	QMenu	*m_pMenuExport;
	QMenu	*m_pMenuHelp;
	QMenu *m_pMenuOpenRecent;
	FSTitleBarProjectName *m_pProjectName;
	QList<QAction*> m_lstActions;

	FSIconButton *m_pBtnRegister;
	FSIconButton *m_pBtnPurchase;

	bool m_bDisableConflics;
	bool m_bRegistered;
};

class FSTitleBarProjectName : public FSBaseWidget
{
	Q_OBJECT
public:
	FSTitleBarProjectName(QWidget *parent);

	void SetProjectName(const QString &projectName);
	QString ProjectName();
	void SetRegistered(bool bRegistered);
protected:
	void RetranslateUi() override;
	void resizeEvent(QResizeEvent *e) override;
private:
	QLabel *m_pLbProjectName;
	QString m_strProjectName;
	bool m_bRegistered;
};



#endif // FSMAINTITLEBAR_H
