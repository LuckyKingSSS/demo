#ifndef MEDIAIMPORTDLG_H
#define MEDIAIMPORTDLG_H

#include "inc_FSCommonlib/FSCommonWidget.h"

class TopLevelWindowTitleBar;
class FSNLETimeline;
class ImportHelper;
class FSCOMMONLIB_EXPORT MediaImportDlg : public FSBaseDialog
{
	Q_OBJECT

public:
	MediaImportDlg(const QStringList &list, QWidget *parent = 0);
	~MediaImportDlg();
	int DoImport();

    void SetRange(int minvalue, int maxvalue);
Q_SIGNALS:
	void sigImported(QString srcfile, FSNLETimeline *timeline);
protected:
	void resizeEvent(QResizeEvent *event) override;
	void closeEvent(QCloseEvent *e) override;
protected:
	//TopLevelWindowTitleBar *m_pTitleBar;

	QPushButton *m_pBtnCancel;
	QProgressBar *m_pProgress;
	QLabel *m_pLbName;
	QLabel *m_pLbProgress;
	ImportHelper *m_pImportHelper;
};

class QThread;
class ImportHelper : public QObject
{
	Q_OBJECT
public:
	ImportHelper(const QStringList &list, QObject *parent = 0);
	void StartImport();
	void Cancel();
Q_SIGNALS:
	void sigName(QString name);
	void sigImportProgress(int cur, int total);
	void sigImported(QString srcfile, FSNLETimeline *timeline);
	void sigFinish();
protected:
	Q_INVOKABLE void _Import();
	QString GetName(QString path);
private:
	QStringList m_lstImportFiles;
	QThread *m_pThread;
	bool m_bCanceled;
};


#endif // MEDIAIMPORTDLG_H
