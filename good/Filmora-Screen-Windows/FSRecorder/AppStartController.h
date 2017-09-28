#ifndef MULTIAPPCONTROLLER_H
#define MULTIAPPCONTROLLER_H

#include <QObject>

class QApplication;
class QLocalSocket;
class QLocalServer;
class AppStartController : public QObject
{
	Q_OBJECT

public:
	static AppStartController* GetInstance();
	~AppStartController();
	
	void Init(QApplication *app, int argc, char *argv[]);
	int Start();
	bool CheckStartDuration();

protected slots:
void OnNewConnect();
void OnServerRead();
protected:
	void ParseCmdLine(int argc, char *argv[]);
	bool QuitAnother();
	bool IsSameProject(QString projFile);
private:
	AppStartController(QObject *parent = 0);

	QApplication *m_appObj;
	QString m_strProjFile;
	bool m_bProjStart;

	bool m_bAnotherRefuse;
	bool m_bAnotherFinishQuit;

	bool m_bSameProject;
	bool m_bResponed;

	QLocalSocket *m_pConnectSocket;
	QLocalServer *m_pServer;
	QLocalSocket *m_pResponeSocket;
};

#endif // MULTIAPPCONTROLLER_H
