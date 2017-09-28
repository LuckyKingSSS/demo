
#include <QtCore/QFile>
#include <QtCore/QProcessEnvironment>
#include <QtCore/QDir>
#include <QtCore/QTextStream>
#include <QtXml/QDomDocument>
#include "inc_CommonLib/externalapi.h"
#include "inc_CommonLib/FileOperation.h"
#include "inc_CommonLib/StringOperation.h"

#ifdef WIN32
#include "qt_windows.h"
#endif


static QWidget* g_MainWindow = nullptr;
QWidget* gGetMainWindow()
{
	return g_MainWindow;
}

void gSetMainWindow(QWidget *w)
{
	g_MainWindow = w;
}

static QWidget* g_MatterView = nullptr;
QWidget* gGetMatterView()
{
	return g_MatterView;
}

void gSetMatterView(QWidget *w)
{
	g_MatterView = w;
}

QString gFrame2String(int frame, float fps /*= 25.0f*/)
{
	int temp = frame;
	int hour = temp / (60 * 60 * fps);
	temp = temp % int(60 * 60 * fps);
	int minute = temp / (60 * fps);
	temp = temp % int(60 * fps);
	int second = temp / (fps);
	int frames = temp % int(fps);

	return QString("%1:%2:%3.%4").arg(hour, 2, 10, QChar('0')).arg(minute, 2, 10, QChar('0'))
		.arg(second, 2, 10, QChar('0')).arg(frames, 2, 10, QChar('0'));
}

QString gFrame2StringSecond(int frame, float fps /*= 25.0f*/)
{
	int temp = frame;
	int hour = temp / (60 * 60 * fps);
	temp = temp % int(60 * 60 * fps);
	int minute = temp / (60 * fps);
	temp = temp % int(60 * fps);
	int second = temp / (fps);
	//int frames = temp % int(fps);

	return QString("%1:%2:%3").arg(hour, 2, 10, QChar('0')).arg(minute, 2, 10, QChar('0'))
		.arg(second, 2, 10, QChar('0'));
}

QString gGetStyleSheet(QString path)
{
	QFile file(path);
	if (!file.open(QIODevice::ReadOnly))
		return QString();

	QString ret =  file.readAll();
	return ret;
}
#include <QtWidgets/QApplication>

float gGetDevicePixelRatio()
{
	return qApp->devicePixelRatio();
}

QString gGetCommonLibStyles()
{
	return gGetStyleSheet(":/CommonLib/Resources/styles.qss");
}


static bool g_bArg = false;
void gSetArgLang(bool bArg /*= false*/)
{
	g_bArg = bArg;
}

bool gIsArgLang()
{
	return g_bArg;
}

bool gIsH264Coder(int fourcc)
{
	QString sFourcc = gMakeFourCCString(fourcc);
	if (sFourcc.contains("H264", Qt::CaseInsensitive) || sFourcc.contains("AVC", Qt::CaseInsensitive))
	{
		return true;
	}
	return false;
}

QString gMakeFourCCString(int fouccc)
{
	char s[4];
	s[0] = fouccc & 0xff;
	s[1] = (fouccc >> 8) & 0xff;
	s[2] = (fouccc >> 16) & 0xff;
	s[3] = fouccc >> 24;
	return QString::fromLatin1(s, 4);
}

quint64 gGetDiskFreeSpace(const QString &driver)
{
	quint64 nSpace = INT_MAX;
#ifdef WIN32
	LPCWSTR lpcwstrDriver = (LPCWSTR)driver.utf16();
	ULARGE_INTEGER liFreeBytesAvailable, liTotalBytes, liTotalFreeBytes;
	if (GetDiskFreeSpaceEx(lpcwstrDriver, &liFreeBytesAvailable, &liTotalBytes, &liTotalFreeBytes))
	{
		nSpace =liTotalFreeBytes.QuadPart / 1024 / 1024;
	}
#endif
	return nSpace;
}

COMMONLIB_EXPORT int gGetRetranslateUiTextLenght(QFont font, QString text, int minWidth, int extraWidth /*= 0*/)
{
    const int btnGoWidth = minWidth;
    int btnExportNeedWidth = StringOperation::GetTextTrueLenght(font, text);
    if (btnExportNeedWidth > btnGoWidth)
        btnExportNeedWidth += extraWidth;
    else
        btnExportNeedWidth = btnGoWidth;

    return btnExportNeedWidth;
}

QString GetConfigfileDir()
{

	QString path = QProcessEnvironment::systemEnvironment().value("ProgramData");
	path += QString("/Wondershare Video Editor/Resources/ActionCam");
	QDir dir(path);
	if (!dir.exists())
		dir.mkdir(path);
	return path;
}

void CreateSettingsFile(const QString &filePath)
{
	FileOperation::CreateNewFile(filePath);

	QDomDocument doc;
	QString strHeader("version=\"1.0\" encoding=\"UTF-8\"");
	doc.appendChild(doc.createProcessingInstruction("xml", strHeader));

	QDomElement root = doc.createElement("Settings");

	doc.appendChild(root);

	QFile file(filePath);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) return;
	QTextStream outStream(&file);
	doc.save(outStream, 4);
	file.close();
}

QString GetSettingFilePath()
{
	return GetConfigfileDir() + "/Settings.xml";
}