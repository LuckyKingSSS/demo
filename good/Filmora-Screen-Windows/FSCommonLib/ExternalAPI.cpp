#include "stdafx.h"
#include "inc_FSCommonlib/ExternalAPI.h"
#include "inc_CommonLib/FileOperation.h"
#include "inc_CommonLib/NLEWraper.h"
#include "inc_CommonLib/PlayerMaterProvider.h"
#include "inc_CommonLib/externalapi.h"
#include "inc_CommonLib/PathOperation.h"

#include "inc_CommonLib/StringOperation.h"


#include "INLEFactory.h"
#include "INLESerialize.h"
#include "INLEMouseSetting.h"
#include "NLEMouseSettingHelper.h"
#include "cCaptureStream.h"
#include "INLEMouseEffectManager.h"
#include "INLEClonable.h"
#include "NLEKey.h"

#include <QtWinExtras/QtWinExtras>

#ifdef WIN32
#include <windows.h>
#include <tlhelp32.h>
#endif


FSCOMMONLIB_EXPORT QString FSGetFilmoraScreenDirectory()
{
	QString filmoraScreenDir = QProcessEnvironment::systemEnvironment().value("ProgramData");
	filmoraScreenDir += QString("/Wondershare Filmora Scrn");
    filmoraScreenDir = QDir::toNativeSeparators(filmoraScreenDir);
	QDir dir(filmoraScreenDir); 
	if (!dir.exists())
	{
		dir.mkpath(filmoraScreenDir);
	}

	return filmoraScreenDir;
}

FSCOMMONLIB_EXPORT QString FSGetProjectFileThumbnailPath()
{
	auto path = FSGetFilmoraScreenDirectory() + "/projthumbnail";
	QDir dir(path);
	if (!dir.exists())
	{
		dir.mkdir(path);
	}

	return path;
}


FSCOMMONLIB_EXPORT QVariant FSGetSettingsTest(const QString &key, QVariant defaultvalue /*= QVariant()*/)
{
    QSettings settings(FSGetFilmoraScreenDirectory() + "/FilmoraScrn.ini", QSettings::IniFormat);
    settings.beginGroup("Test");
    auto test = settings.value(key, defaultvalue);
    settings.endGroup();
    return test;
}

FSCOMMONLIB_EXPORT void FSFindTargetFile(const QString &filepath)
{
	QString cmd = QString("explorer /select, \"%0\"").arg(QDir::toNativeSeparators(filepath));
	QProcess::startDetached(cmd);
    //qDebug() << cmd;
}


QString FSGetSettingsPath()
{
	QString settingPath = FSGetFilmoraScreenDirectory();
	settingPath += "/FilmoraScreen.xml";
	if (!FileOperation::IsFileExist(settingPath))
	{
		FileOperation::CreateNewFile(settingPath);
	}

	return settingPath;
}


FSCOMMONLIB_EXPORT HRESULT SetVideoFilterProperty(NLEComPtr<INLEVideoFilter> pFilter, LPCWSTR pPropFriendName, NLEFilterPropertyType type, CNLEValue val)
{
	HRESULT hr = S_OK;

	NLEComPtr<INLEProperties> pFilterProp = NLEWraper::GetInstance().GetPlayerMaterProvider()->NLEGetProperties(pFilter);

	WCHAR pszName[128];
	int nPropCount = pFilter->GetFilterPropertyCount();
	for (int nPropIndex = 0; nPropIndex < nPropCount; nPropIndex++)
	{
		NLEComPtr<INLEProperties> pParamProp = NULL;
		hr = pFilter->GetFilterProperty(nPropIndex, &pParamProp); CHECK(hr);

		hr = pParamProp->GetString(NLEKey::Filter::Property::kFriendName, pszName, 128); CHECK(hr);
		if (wcscmp(pszName, pPropFriendName) == 0)
		{
			switch (type)
			{
			case NLE_FILTER_PROPERTY_TYPE_BOOLEAN:
			case NLE_FILTER_PROPERTY_TYPE_INT:
				pParamProp->SetInt(NLEKey::Filter::Property::kValue_Current, val);
				break;
			case NLE_FILTER_PROPERTY_TYPE_DOUBLE:
				pParamProp->SetDouble(NLEKey::Filter::Property::kValue_Current, val);
				break;
			case NLE_FILTER_PROPERTY_TYPE_FPOINT:
				pParamProp->SetPointF(NLEKey::Filter::Property::kValue_Current, val);
				break;
			case NLE_FILTER_PROPERTY_TYPE_FSIZE:
				pParamProp->SetSizeF(NLEKey::Filter::Property::kValue_Current, val);
				break;
			case NLE_FILTER_PROPERTY_TYPE_FRECT:
				pParamProp->SetRectF(NLEKey::Filter::Property::kValue_Current, val);
				break;
			case NLE_FILTER_PROPERTY_TYPE_FCOLOR:
				pParamProp->SetColorF(NLEKey::Filter::Property::kValue_Current, val);
				break;

			}
			//pParamProp->SetValue(NLEKey::Filter::Property::kValue_Current, val);
		}
	}
	return hr;
}


FSCOMMONLIB_EXPORT CNLEValue GetVideoFilterProperty(NLEComPtr<INLEVideoFilter> pFilter, LPCWSTR pPropFriendName, NLEFilterPropertyType type)
{
    HRESULT hr = S_OK;

    NLEComPtr<INLEProperties> pFilterProp = NLEWraper::GetInstance().GetPlayerMaterProvider()->NLEGetProperties(pFilter);
    CNLEValue val;
    WCHAR pszName[128];
    int nPropCount = pFilter->GetFilterPropertyCount();
    for (int nPropIndex = 0; nPropIndex < nPropCount; nPropIndex++)
    {
        NLEComPtr<INLEProperties> pParamProp = NULL;
        hr = pFilter->GetFilterProperty(nPropIndex, &pParamProp); CHECK(hr);

        hr = pParamProp->GetString(NLEKey::Filter::Property::kFriendName, pszName, 128); CHECK(hr);
        if (wcscmp(pszName, pPropFriendName) == 0)
        {
            switch (type)
            {
            case NLE_FILTER_PROPERTY_TYPE_BOOLEAN:
            case NLE_FILTER_PROPERTY_TYPE_INT:
            {
                int nValue = 0;
                pParamProp->GetInt(NLEKey::Filter::Property::kValue_Current, &nValue);
                val.Set(nValue);
                break;
            }
            case NLE_FILTER_PROPERTY_TYPE_DOUBLE:
            {
                double dValue = 0;
                pParamProp->GetDouble(NLEKey::Filter::Property::kValue_Current, &dValue);
                val.Set(dValue);
                break;
            }
            case NLE_FILTER_PROPERTY_TYPE_FPOINT:
            {
                NLEPointF point;
                pParamProp->GetPointF(NLEKey::Filter::Property::kValue_Current, &point);
                val.Set(point);
                break;
            }
            case NLE_FILTER_PROPERTY_TYPE_FSIZE:
            {
                NLESizeF size;
                pParamProp->GetSizeF(NLEKey::Filter::Property::kValue_Current, &size);
                val.Set(size);
                break;
            }
            case NLE_FILTER_PROPERTY_TYPE_FRECT:
            {
                NLERectF rect;
                pParamProp->GetRectF(NLEKey::Filter::Property::kValue_Current, &rect);
                val.Set(rect);
                break;
            }
            case NLE_FILTER_PROPERTY_TYPE_FCOLOR:
            {
                NLEColorF color;
                pParamProp->GetColorF(NLEKey::Filter::Property::kValue_Current, &color);
                val.Set(color);
                break;
            }

            }
        }
    }
    return val;
}

FSCOMMONLIB_EXPORT QString gGetCommonStyleSheet()
{
    return gGetStyleSheet(":/FSCommonLib/Resources/commonLibStyle.qss");
}

FSCOMMONLIB_EXPORT void CustomLogOutput(QString logString)
{
    QString strLogFilePath = QString("%1/%2.log").arg(qApp->applicationDirPath()).arg("log");
    QFileInfo fileLogInfo;
    fileLogInfo.setFile(strLogFilePath);
    // if log file size > 1M, recreate file
    if (fileLogInfo.size() > 10485760)
    {
        QDir dir;
        dir.remove(strLogFilePath);
    }

    QFile logFile(strLogFilePath);
    if (logFile.open(QIODevice::Text | QIODevice::Append))
    {
        QTextStream textStream(&logFile);
        textStream << QDateTime::currentDateTime().toString(Qt::ISODate) << ":" << logString << endl;
        logFile.close();
    }
}

FSCOMMONLIB_EXPORT QString GetFSRecorderTitleName()
{
    return QObject::tr("Wondershare Filmora Scrn");
}


FSCOMMONLIB_EXPORT QString GetFSEditTitleName()
{
    return QObject::tr("Wondershare Filmora Scrn") + " ";
}

FSCOMMONLIB_EXPORT QString loadFontFamilyFromFiles(const QString &fontFilePath)
{
    QString font = "";

    QFile fontFile(fontFilePath);
    if (!fontFile.open(QIODevice::ReadOnly))
    {
        return font;
    }

    int loadedFontID = QFontDatabase::addApplicationFontFromData(fontFile.readAll());
    QStringList loadedFontFamilies = QFontDatabase::applicationFontFamilies(loadedFontID);
    if (!loadedFontFamilies.empty())
    {
        font = loadedFontFamilies.at(0);
    }
    fontFile.close();
    return font;
}

static QWidget *g_FsEditorView = nullptr;
FSCOMMONLIB_EXPORT void SetEditorMainView(QWidget *pView)
{
	g_FsEditorView = pView;
}

FSCOMMONLIB_EXPORT QWidget* GetEditorMainView()
{
	return g_FsEditorView;
}


#include <cguid.h>
#include <guiddef.h>
#include <shtypes.h>
#include <atlcomcli.h>
#include <ShObjIdl.h>
#include <ShlObj.h>
HRESULT GetShellThumbnailImage(LPCWSTR pszPath, HBITMAP* pThumbnail, SIZE size)
{
    HRESULT hr;

    *pThumbnail = NULL;

    LPITEMIDLIST pidlItems = NULL, pidlURL = NULL, pidlWorkDir = NULL;
    WCHAR szBasePath[MAX_PATH], szFileName[MAX_PATH];
    WCHAR* p;
    wcscpy(szBasePath, pszPath);
    p = wcsrchr(szBasePath, L'\\');
    if (p) *(p + 1) = L'\0';
    wcscpy(szFileName, pszPath + (p - szBasePath) + 1);

    while (TRUE)
    {
        CComPtr<IShellFolder> psfDesktop;
        hr = SHGetDesktopFolder(&psfDesktop);
        if (FAILED(hr)) break;

        CComPtr<IShellFolder> psfWorkDir;
        hr = psfDesktop->ParseDisplayName(NULL, NULL, szBasePath, NULL, &pidlWorkDir, NULL);
        if (FAILED(hr)) break;
        hr = psfDesktop->BindToObject(pidlWorkDir, NULL, IID_IShellFolder, (LPVOID*)&psfWorkDir);
        if (FAILED(hr)) break;

        hr = psfWorkDir->ParseDisplayName(NULL, NULL, szFileName, NULL, &pidlURL, NULL);
        if (FAILED(hr)) break;

        // query IExtractImage   
        CComPtr<IExtractImage> peiURL;
        hr = psfWorkDir->GetUIObjectOf(NULL, 1, (LPCITEMIDLIST*)&pidlURL, IID_IExtractImage, NULL, (LPVOID*)&peiURL);
        if (FAILED(hr)) break;

        // define thumbnail properties   
        //SIZE size = { 200, 200 };
        DWORD dwPriority = 0, dwFlags = IEIFLAG_ASPECT;
        WCHAR pszImagePath[MAX_PATH];
        hr = peiURL->GetLocation(pszImagePath, MAX_PATH, &dwPriority, &size, 16, &dwFlags);
        if (FAILED(hr)) break;

        // generate thumbnail   
        hr = peiURL->Extract(pThumbnail);
        if (FAILED(hr)) break;

        break;
    }

    // free allocated structures  
    if (pidlWorkDir) CoTaskMemFree(pidlWorkDir);
    if (pidlURL) CoTaskMemFree(pidlURL);
    return hr;
}




FSCOMMONLIB_EXPORT QPixmap GetThumbnailImageFromFile(QString path, int width, int height)
{
    Q_ASSERT(width > 0);
    Q_ASSERT(height > 0);
    Q_ASSERT(FileOperation::IsFileExist(path));

    QPixmap pixmap;

    HBITMAP hbitmap;
    SIZE sSize;
    sSize.cx = width;
    sSize.cy = height;
    path = QDir::toNativeSeparators(path);
    auto pathWString = path.toStdWString();
    if (SUCCEEDED(GetShellThumbnailImage(pathWString.c_str(), &hbitmap, sSize)))
        pixmap = QtWin::fromHBITMAP(hbitmap);

    return pixmap;
}

FSCOMMONLIB_EXPORT void sSetFont(QWidget *w)
{    
    #ifdef WIN32
    // 保持字体在dpi为96时大小
    HDC dc = GetDC((HWND)w->winId());
    if (dc)
    {
        int logpixelsy = GetDeviceCaps(dc, LOGPIXELSY);
        int fontPointSize = 9 - (int)(1.0 * (logpixelsy - 96) / 96 * 9 / 2);
        QFont font(loadFontFamilyFromFiles(":/FSCommonLib/Resources/Lato-Regular.ttf"), fontPointSize);
        qApp->setFont(font);
    }
    #endif
}


FSCOMMONLIB_EXPORT int GetRetranslateUiTextLenght(QFont font, QString text, int minWidth, int extraWidth)
{
    const int btnGoWidth = minWidth;
    int btnExportNeedWidth = StringOperation::GetTextTrueLenght(font, text);
    if (btnExportNeedWidth > btnGoWidth)
        btnExportNeedWidth += extraWidth;
    else
        btnExportNeedWidth = btnGoWidth;

    return btnExportNeedWidth;
}

#include <string.h>
#include <psapi.h>

std::vector<QString> VEC_CapturableModule = { "d3d9.dll", "d3d10.dll", "d3d10_1.dll", "d3d11.dll", "dxgi.dll", "d3d8.dll", "opengl32.dll", "java.dll" };

BOOL isCapturableProcess(DWORD processId)
{
	//return true;

	BOOL ret = FALSE;


	auto hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, processId);
	if (hProcess == INVALID_HANDLE_VALUE)
		return ret;
	DWORD dwSize = MAX_PATH;
	HMODULE hMods[1024];
	DWORD count;
	if (EnumProcessModulesEx(hProcess, hMods, sizeof(hMods), &count, LIST_MODULES_ALL))
	{
		for (UINT i = 0; i < (count / sizeof(HMODULE)); i++)
		{
			char szFileName[MAX_PATH];
			if (GetModuleFileNameExA(hProcess, hMods[i], szFileName, _countof(szFileName) - 1))
			{
				QFileInfo fileInfo(szFileName);
				QString ext = fileInfo.fileName();
				auto iter = std::find_if(VEC_CapturableModule.begin(), VEC_CapturableModule.end(), [&](QString v){
					return v == ext;
				});
				if (iter != VEC_CapturableModule.end())
				{
					ret = TRUE;
					break;
				}
			}
		}
	}

	return ret;
}





typedef QList<std::tuple<HWND, QString, DWORD>> EnumHWndsArgList;
typedef struct EnumHWndsArg
{
	EnumHWndsArgList *vecHWnds;
	DWORD dwProcessId;
}EnumHWndsArg, *LPEnumHWndsArg;


static BOOL CALLBACK lpEnumFunc(HWND hwnd, LPARAM lParam)
{
	EnumHWndsArg *pArg = (LPEnumHWndsArg)lParam;
	DWORD  processId;
	DWORD threadId = GetWindowThreadProcessId(hwnd, &processId);
	wchar_t  szCap[255] = { 0 };

	//::SendMessageW(hwnd, WM_GETTEXT, 255, (LPARAM)szCap);
	DWORD pResult = 0;
	//::SendMessageTimeoutW(hwnd, WM_GETTEXT, 255, (LPARAM)szCap, SMTO_NORMAL, 10, 0);
	//::GetWindowTextW(hwnd, szCap, 255);
	//QString szCapString = QString::fromWCharArray(szCap);
	//if (!szCapString.isEmpty())
	//	qDebug() << szCapString;
	QString szCapString;
	if (processId == pArg->dwProcessId && ::IsWindowVisible(hwnd) /*&& !szCapString.isEmpty()*/)
	{
		::GetWindowTextW(hwnd, szCap, 255);
		QString szCapString = QString::fromWCharArray(szCap);
		pArg->vecHWnds->push_back(std::make_tuple(hwnd, szCapString, threadId));

	}
	return TRUE;
}

static void GetHWndsByProcessID(DWORD processID, EnumHWndsArgList &vecHWnds)
{
	EnumHWndsArg wi;
	wi.dwProcessId = processID;
	wi.vecHWnds = &vecHWnds;
	EnumWindows(lpEnumFunc, (LPARAM)&wi);
}

bool operator == (const ProcessData &l, const ProcessData &r)
{
	return l.name == r.name && l.processId == r.processId;
}

ProcessDatas ProcessManager::GetWindowProcess()
{
	ProcessDatas processNames;

	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(pe32);
	//给系统中所有的进程拍一个快照
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE){
		return processNames;
	}

	//遍历进程快照，显示每个进程的信息
	BOOL bNext = Process32First(hProcessSnap, &pe32);
	while (bNext)
	{
		EnumHWndsArgList tempHWN;
		GetHWndsByProcessID(pe32.th32ProcessID, tempHWN);
		if (!tempHWN.empty() && isCapturableProcess(pe32.th32ProcessID))
		{
			ProcessData data;
			//data.name = std::get<1>(tempHWN[0]);
			data.name = (std::get<1>(tempHWN[0]));
			data.processName = QString::fromWCharArray(pe32.szExeFile);
			data.processId = pe32.th32ProcessID;
			data.hwnd = std::get<0>(tempHWN[0]);
			data.threadId = std::get<2>(tempHWN[0]);
			processNames.push_back(data);
		}

		bNext = Process32Next(hProcessSnap, &pe32);
	}
	CloseHandle(hProcessSnap);

	return processNames;

}

ProcessDatas ProcessManager::GetProcess()
{
	ProcessDatas processNames;

	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(pe32);
	//给系统中所有的进程拍一个快照
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE){
		return processNames;
	}

	//遍历进程快照，显示每个进程的信息
	BOOL bNext = Process32First(hProcessSnap, &pe32);
	while (bNext)
	{
		QString curName = QString::fromWCharArray(pe32.szExeFile);
		ProcessData data;
		data.name = curName;
		data.processId = pe32.th32ProcessID;
		if (!processNames.contains(data))
		{
			processNames.push_back(data);
		}

		bNext = Process32Next(hProcessSnap, &pe32);
	}
	CloseHandle(hProcessSnap);

	return processNames;
}




struct handle_data {
	unsigned long process_id;
	HWND best_handle;
};

BOOL IsMainWindow(HWND handle)
{
	return GetWindow(handle, GW_OWNER) == (HWND)0;// && IsWindowVisible(handle);
}

BOOL CALLBACK EnumWindowsCallback(HWND handle, LPARAM lParam)
{
	handle_data& data = *(handle_data*)lParam;
	unsigned long process_id = 0;
	GetWindowThreadProcessId(handle, &process_id);
	if (data.process_id != process_id || !IsMainWindow(handle)) {
		return TRUE;
	}
	data.best_handle = handle;
	return FALSE;
}

HWND FindMainWindow(unsigned long process_id)
{
	handle_data data;
	data.process_id = process_id;
	data.best_handle = 0;
	EnumWindows(EnumWindowsCallback, (LPARAM)&data);
	return data.best_handle;
}

HWND ProcessManager::FindMainWindow(DWORD processId)
{
	handle_data data;
	data.process_id = processId;
	data.best_handle = 0;
	EnumWindows(EnumWindowsCallback, (LPARAM)&data);
	return data.best_handle;
}

DWORD ProcessManager::FindProcessID(QString processName)
{
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(pe32);
	//给系统中所有的进程拍一个快照
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE){
		return 0;
	}

	//遍历进程快照，显示每个进程的信息
	BOOL bNext = Process32First(hProcessSnap, &pe32);
	while (bNext)
	{
		QString curName = QString::fromWCharArray(pe32.szExeFile);
		if (curName.toLower() == processName.toLower())
		{
			return pe32.th32ProcessID;
		}

		bNext = Process32Next(hProcessSnap, &pe32);
	}
	CloseHandle(hProcessSnap);

	return 0;
}

//////////////////////////////////////////////////////////////////////////
AutoGetGpuCap::AutoGetGpuCap(QObject *parent)
	:QObject(parent)
{
	m_bFinished = false;
	m_bError = false;
}

int AutoGetGpuCap::GetGpuCap()
{
	QString exeName = "GPUChecker.exe";
#ifdef _DEBUG
	exeName = "GPUCheckerD.exe";
#endif // _DEBUG


	auto gpuExe = PathOperation::MakeWorkPath(exeName);
	if (!QFile::exists(gpuExe)) return 0;
	QProcess process;
	connect(&process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(OnFinish(int, QProcess::ExitStatus)));
	connect(&process, SIGNAL(error(QProcess::ProcessError)), this, SLOT(OnError(QProcess::ProcessError)));
	process.setProgram(gpuExe);
	process.start();
	int exitcode = 0;
	while (true)
	{
		if (m_bFinished)
		{
			exitcode = process.exitCode();
			break;
		}
		if (m_bError)
		{
			exitcode = 0;
			break;
		}
		QApplication::processEvents();
	}
	return exitcode;
}

void AutoGetGpuCap::OnFinish(int exitCode, QProcess::ExitStatus exitStatus)
{
	m_bFinished = true;
}

void AutoGetGpuCap::OnError(QProcess::ProcessError error)
{
	m_bError = true;
}

