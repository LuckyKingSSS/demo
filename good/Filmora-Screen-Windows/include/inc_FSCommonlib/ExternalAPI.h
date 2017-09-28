#pragma once

#include "inc_FSCommonlib/fscommonlib_global.h"

#include <QtCore/QString>
#include <QtCore/QObject>
#include <QtCore/QProcess>

#include "INLEProperties.h"
#include "NLEComPtr.h"
#include "NLEValue.h"
#include "INLEVideoFilter.h"

FSCOMMONLIB_EXPORT QString FSGetFilmoraScreenDirectory();
FSCOMMONLIB_EXPORT QString FSGetProjectFileThumbnailPath();
// 测试或者隐藏功能标记获取
FSCOMMONLIB_EXPORT QVariant FSGetSettingsTest(const QString &key, QVariant defaultvalue);

FSCOMMONLIB_EXPORT void FSFindTargetFile(const QString &filepath);

FSCOMMONLIB_EXPORT QString FSGetSettingsPath();

FSCOMMONLIB_EXPORT HRESULT SetVideoFilterProperty(NLEComPtr<INLEVideoFilter> pFilter, LPCWSTR pPropFriendName, NLEFilterPropertyType type, CNLEValue val);

FSCOMMONLIB_EXPORT CNLEValue GetVideoFilterProperty(NLEComPtr<INLEVideoFilter> pFilter, LPCWSTR pPropFriendName, NLEFilterPropertyType type);

FSCOMMONLIB_EXPORT QString gGetCommonStyleSheet();

FSCOMMONLIB_EXPORT void CustomLogOutput(QString logString);


FSCOMMONLIB_EXPORT QString GetFSRecorderTitleName();
FSCOMMONLIB_EXPORT QString GetFSEditTitleName();


FSCOMMONLIB_EXPORT QString loadFontFamilyFromFiles(const QString &fontFilePath);


FSCOMMONLIB_EXPORT void SetEditorMainView(QWidget *pView);
FSCOMMONLIB_EXPORT QWidget* GetEditorMainView();


FSCOMMONLIB_EXPORT QPixmap GetThumbnailImageFromFile(QString path, int width, int height);
FSCOMMONLIB_EXPORT void sSetFont(QWidget *w);

FSCOMMONLIB_EXPORT int GetRetranslateUiTextLenght(QFont font, QString text, int minWidth, int extraWidth = 0);


#define RUNONCE static bool bRunOnce = false; if(!bRunOnce && (bRunOnce = true))

inline bool CloseDouble(double f1, double f2, double precision = 0.001)
{
	return fabs(f1 - f2) <= precision;
}

struct ProcessData
{
	QString name;
	QString processName;
	int processId;
	int threadId;
	HWND hwnd;

	ProcessData()
	{
		processId = 0;
		threadId = 0;
	}
};
typedef QList<ProcessData> ProcessDatas;
Q_DECLARE_METATYPE(ProcessData)

class FSCOMMONLIB_EXPORT ProcessManager
{
public:
	// 获取有窗口进程
	static ProcessDatas GetWindowProcess();
	// 获取所有进程
	static ProcessDatas GetProcess();

	static HWND FindMainWindow(DWORD processId);
	static DWORD FindProcessID(QString processName);
};

class FSCOMMONLIB_EXPORT AutoGetGpuCap : public QObject
{
	Q_OBJECT
public:
	AutoGetGpuCap(QObject *parent = 0);

	int GetGpuCap();

public slots:
void OnFinish(int exitCode, QProcess::ExitStatus exitStatus);
void OnError(QProcess::ProcessError error);

private:
	bool m_bFinished;
	bool m_bError;
};
