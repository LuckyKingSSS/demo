#ifndef PROCESSMANAGER_H
#define PROCESSMANAGER_H

#include <QObject>
#include "inc_FSCommonlib/MediaImportDlg.h"
class QProcess;
class FSLoadingEditorWin;
class Communicator : public QObject
{
	Q_OBJECT

public:
	static Communicator* GetInstance();
	bool IsEditorRunning();
	bool IsEditShowed();
	bool GotoEditor();
	void HideEditor();
	void TranslteRecordFiles(QStringList files);
	void EditerLoadProjectString(const QString &projstring);
	std::tuple<QString,QString,QString> GetEditorProjectString();
	QString GetEditorThumbnail();
	
    bool IsSaving();

	void TellContentChanged(bool bChanged);
	void TellRecentOpens();
	void TellIsNewProject(bool isNewProj);
	void TellProjectName(QString prjName);
	void TellSaveProjectFinish();
	void TellRegisterResult(bool bSucc);
    int ShowMessageBox(QString title, QString text, int buttons);
Q_SIGNALS:
	void sigShowRecorder();
	void sigClearRecorder();
	void sigLoadProject(QString projFile);
	void sigLoadImportProject(QString projFile);
	void sigRegisterResult(bool bSucc);
protected slots:
	void OnEditorExit(int ec, QProcess::ExitStatus ps);
    void OnProcessStartError(QProcess::ProcessError err);
protected:
	bool SendMsg(QString msg);
	QString SendMsgWaitForResponse(QString msg);
	void OnRecvMsg();
private:
	Communicator(QObject *parent = 0);
	QProcess *m_pEditorProc;
	QString m_strResponse;
	bool m_bSaving;
	bool m_bEditorShowed;
	FSLoadingEditorWin *m_pFSLoadingEditorWin;
    bool m_bProcessStartError;
};
struct ProjectFileInfo;
class ProjPreproccessor : public QObject
{
	Q_OBJECT
public:
	enum QuestionSavingRes
	{
		QuestionSavingRes_Saved = 0,
		QuestionSavingRes_UserRefuse,
		QuestionSavingRes_UserCancel,
		QuestionSavingRes_UserNoSavePath,
	};
	static ProjPreproccessor* GetInstance();
    // loadImport在Editor打开工程如果工程是Preivew工程，提示是否把YourClips全导入到Editor上并在显示Editor
	void LoadProject(QString projFile, bool loadImport = false);
	void LoadProjectFromEditor(QString projFile);
	void LoadImportProject(QString projFile);
    ProjectFileInfo* AddProj2Recent(QString projFile);
    QuestionSavingRes DoQuestionSaving();

    void SetLoadFromEditor(bool b);
    bool GetLoadFromEditor();
protected:
	ProjPreproccessor(QObject *parent = 0);
	bool CheckFiles(QString projFile);
    bool m_bLoadFromEditor;
};

class FSLoadingEditorWin : public MediaImportDlg
{
	Q_OBJECT
public:
	FSLoadingEditorWin(QWidget *parent);

	void Show(QString projName);
	void Hide();
};

#endif // PROCESSMANAGER_H
