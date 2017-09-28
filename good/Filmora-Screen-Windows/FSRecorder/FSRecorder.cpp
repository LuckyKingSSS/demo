#include "stdafx.h"
#include "FSRecorder.h"
#include "RecordPanel.h"

#include <QPainter>
#include <QMouseEvent>
#include "inc_MKCollecter/MKCollecter.h"
#include "ICaptureEngine.h"
#include "ICaptureSource.h"
#include "CaptureManger.h"
#include "INLECaptureMgr.h"
#include "SettingsDataManager.h"
#include "inc_CommonLib/TopLevelWindowTitleBar.h"
#include "ProjectFilesMgr.h"
#include "ProjectListWidget.h"
#include "ExternalAPI.h"
#include "FSSettingsWidget.h"
#include "SettingsDataManager.h"
#include "RecorderViewsMgr.h"
#include "inc_CommonLib/PathOperation.h"
#include "inc_CommonLib/XmlOperation.h"
#include "inc_CommonLib/FileOperation.h"
#include "inc_FSExport/IFSExport.h"
#include "inc_CommonLib/NLEWraper.h"
#include "inc_FSCommonlib/FSCommonWidget.h"
#include "Communicator.h"
#include "ProjectFilesMgr.h"
#include "inc_UpdateRegister/UpdateMain.h"
#include "inc_UpdateRegister/RegisterMain.h"
#include "UpdateRegisterPurchaseHelper.h"
#include "AppStartController.h"
#include <QtConcurrent/QtConcurrent>
#include "SkinMgr.h"
#include "inc_FSCommonlib/ExternalAPI.h"
#include "inc_FSCommonlib/TUATracker.h"
#include "inc_FSCommonlib/gathread.h"
#include <tchar.h>
#include "PrimaryScreenManager.h"
#include "PreviewAttachmentManager.h"

#include "ProjectAutoSaveMgr.h"

extern bool DoClose(QWidget* parentWidget);

//////////////////////////////////////////////////////////////////////////

#include <QFontDatabase>
FSRecorder::FSRecorder(QWidget *parent)
	: FSBaseWidget(parent)
{
    resize(950, 534);

    const QString appName = GetFSRecorderTitleName();
	//qApp->setApplicationDisplayName(appName);
	QIcon icon = QIcon(":/FSRecorder/Resources/light/filmora scrn.ico");
	setWindowIcon(icon);
    setWindowTitle(appName);
	RecorderViewsMgr::GetInstance()->SetFSRecorder(this);
	gFSSetRecordMainWindow(this);
	ProjectFilesMgr::GetInstance();
	SettingsDataManager::GetInstance();
	m_pTitleBar = new TopLevelWindowTitleBar(this);
	m_pTitleBar->GetControl()->SetBorderColor(QColor(102, 102, 102));
	m_pTitleBar->SetResizeble(false);
	m_pTitleBar->SetSystemBorderVisible(false);
	m_pTitleBar->SetButtons(TopLevelWindowTitleBar::None);
	m_pTitleBar->SetDragMaximum(false);
	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);
    m_pBtnMinimize = new QPushButton(this);
    m_pBtnMinimize->setObjectName("FSRecorder_TitleBar_MinimizeButton");
    m_pBtnClose = new QPushButton(this);
    m_pBtnClose->setObjectName("FSRecorder_TitleBar_CloseButton");
    m_pLabelIcon = new FSLabel(this);
	m_pLabelIcon->setObjectName("FSRecorder_TitleBar_m_pLabelIcon");
	m_pLabelIcon->setAttribute(Qt::WA_TransparentForMouseEvents);
	m_pLabelIcon->lower();
    //m_pLabelIcon->hide();
    //m_pLabelIcon->lower();

	m_pHelper = RecordHelper::GetInstance();
	m_pMaskWidget = new RecordMask(m_pHelper);
	m_pRecordWidget = new RecordWidget(m_pHelper);
	m_pActionWidget = new ActionWidget;

	m_pCameraPriview = new CameraPreview(0);
	RecorderViewsMgr::GetInstance()->SetCameraPreview(m_pCameraPriview);
	m_pGamePriview = new GamePreview(0);
	RecorderViewsMgr::GetInstance()->SetGamePreview(m_pGamePriview);
	m_pAudioPreivew = new MicrophonePreview(0);
	RecorderViewsMgr::GetInstance()->SetAudioPreview(m_pAudioPreivew);
	m_pRecordPanel = new RecordPanel();
    m_pRecorderMainView = new FSRecorderMainView(this);
	m_pButtonBar = new ButtonBar(m_pTitleBar, this);
	//m_pButtonBar->setVisible(false);
    connect(m_pRecorderMainView, &FSRecorderMainView::signalShowPage, this, [&](FSRecorderMainView::Page page){
        m_pLabelIcon->setVisible(page != FSRecorderMainView::Page_GettingStart);
		m_pButtonBar->SetCurrentPage(page);
		if (page == FSRecorderMainView::Page_GettingStart)
		{
			//程序第一次显示界面
			LARGE_INTEGER litmp;
			LONGLONG  Qpart1,Qpart2;
			double  dfFreq, dfMinus, dfTime;
			QueryPerformanceFrequency(&litmp);//取得高精度运行计数器的频率f,单位是每秒多少次（n/s)  
			dfFreq = (double)litmp.QuadPart;
			QueryPerformanceCounter(&litmp);//取得高精度运行计数器的数值  
			Qpart2 = litmp.QuadPart; //终止计时 
			Qpart1 = GAThread::getInstance()->getAppStartTime();
			dfMinus = (double)(Qpart2 - Qpart1);//计算计数器值  
			qDebug() << " dfminus: "<<dfMinus;
			dfTime = dfMinus / dfFreq;
			qDebug() << " dfTime: " << dfTime;
			dfTime *= 1000;//将单位转化为ms
            //上传用户GA数据
			GAThread::getInstance()->insertEventItem(App_Launch, "Starup", QString::number(dfTime));
		}
    });
	connect(Communicator::GetInstance(), &Communicator::sigShowRecorder, this, [&]{
		RecorderViewsMgr::GetInstance()->ShowRecordWidget();
	},Qt::QueuedConnection);
	connect(Communicator::GetInstance(), &Communicator::sigClearRecorder, this, [&]{
		RecorderViewsMgr::GetInstance()->Clear();
	}, Qt::QueuedConnection);
	connect(Communicator::GetInstance(), &Communicator::sigLoadProject, this, [&](QString projFile){
		//ProjPreproccessor::GetInstance()->LoadProjectFromEditor(projFile);
        ProjectAutoSaveMgr::GetInstance()->LoadEditorProjectFromEditor(projFile);

	}, Qt::QueuedConnection);
	connect(Communicator::GetInstance(), &Communicator::sigLoadImportProject, this, [&](QString projFile){
		//ProjPreproccessor::GetInstance()->LoadImportProject(projFile);
        ProjectAutoSaveMgr::GetInstance()->LoadRecorderProjectFromEditor(projFile);

	}, Qt::QueuedConnection);
    connect(m_pBtnMinimize, &QPushButton::clicked, this, [&]{showMinimized(); });
    connect(m_pBtnClose, &QPushButton::clicked, this, [&]{
        if (m_pRecorderMainView->GetCurrentPage() == FSRecorderMainView::Page_Setting)
        {
            if (m_pRecordPanel->CanGo2PreviewWin() || Communicator::GetInstance()->IsEditorRunning())
            {
                //RecorderViewsMgr::GetInstance()->ShowMainView(FSRecorderMainView::Page_Preview);
                //m_pRecorderMainView->ShowPage(FSRecorderMainView::Page_Preview);
                RecorderViewsMgr::GetInstance()->ShowRecordWidget();

            }
            else
            {
                m_pRecorderMainView->ShowPage(FSRecorderMainView::Page_GettingStart);
            }
        }
        else if (m_pRecorderMainView->GetCurrentPage() == FSRecorderMainView::Page_GettingStart)
        {
            qApp->quit();
        }
        else if (m_pRecorderMainView->GetCurrentPage() == FSRecorderMainView::Page_Preview)
        {
            DoClose(this); 
        }
    });
	qApp->setQuitOnLastWindowClosed(false);
	setObjectName("FSRecorder");
    QFontDatabase base;
    QStringList list = base.families();
	SkinMgr::GetInstance()->SetSkin(SkinMgr::GetInstance()->GetInitType());
	//上传GA信息
	transfromPidInfo();
	connect(UpdateRegisterPurchaseHelper::GetInstance(), &UpdateRegisterPurchaseHelper::signalCheckOnStart, this, [&](bool bResult){
		GAThread::getInstance()->insertEventItem(UserDefineDimension, "", bResult ? "Registered" : "Unregisered", 1);
		qDebug() << "rsult is : " << bResult;
	});
	UpdateRegisterPurchaseHelper::GetInstance()->RegisterCheckOnStart();	
	RetranslateUi();

    PrimaryScreenManager::GetInstance();
    PreviwAttachmentManager::GetInstance();
}

FSRecorder::~FSRecorder()
{
	RecorderViewsMgr::GetInstance()->ShowPreviewWidgets(false);
	RecorderCenter::GetInstance()->EnableKeyboardHook(false);
	SettingsDataManager::GetInstance()->Release();
	NLECaptureManger::GetInstance()->Release();
}

void FSRecorder::RetranslateUi()
{
    setWindowTitle(GetFSRecorderTitleName());
    m_pBtnMinimize->setToolTip(tr("Minimize"));
    m_pBtnClose->setToolTip(tr("Close"));
    //m_pTitleBar->SetTitleName(tr("FilmoraScreen"));
}



void FSRecorder::resizeEvent(QResizeEvent *e)
{
	static const int titleBarH = 30;

	m_pTitleBar->setGeometry(2, 2, width() - 4, titleBarH);
	m_pButtonBar->setGeometry(width() - 300 - 70, 2, 300, titleBarH);

	static int ButtonW = 26;
	static int ButtonH = 26;
	m_pBtnClose->setGeometry(width() - ButtonW - 16, (titleBarH - ButtonH) * 0.5, ButtonW, ButtonH);
	m_pBtnMinimize->setGeometry(width() - 2 * ButtonW -18, (titleBarH - ButtonH) * 0.5, ButtonW, ButtonH);
    m_pLabelIcon->setGeometry(14, 8, 100, 18);

	m_pRecorderMainView->setGeometry(2,titleBarH,width() - 4,height() - titleBarH - 2);

	m_pMaskWidget->setGeometry(m_pHelper->DesktopRect());
	m_pRecordWidget->setGeometry(m_pHelper->DesktopRect());
	m_pActionWidget->setGeometry(m_pHelper->DesktopRect());
		
	int w = m_pRecordPanel->geometry().width();
	int h = w - 48;
	m_pGamePriview->setGeometry(m_pHelper->DesktopRect().right() - w - 80, 0, w, h);
	m_pCameraPriview->setGeometry(m_pHelper->DesktopRect().right() - w - 80, 0, w, h);
	h = 95;
	m_pAudioPreivew->setGeometry(m_pCameraPriview->geometry().left(), m_pCameraPriview->geometry().bottom() + 1, w, h);
}


void FSRecorder::closeEvent(QCloseEvent *e)
{
	QString strAverageTime = GAThread::getInstance()->getAverageTime();
	if (strAverageTime.compare("0"))
	{
		//有seek操作的时候才上传seek操作平均时长
		GAThread::getInstance()->insertEventItem(ClipsSeekSpeed, "SeekAverageTime", strAverageTime);
	}
	//FSBaseWidget::closeEvent(e);
    DoClose(this);
    e->ignore();
}

void FSRecorder::transfromPidInfo()
{
	QSettings* settingInfo = new QSettings(PathOperation::MakeWorkPath("UpdateRegister.ini"), QSettings::IniFormat);
	QString strPid = settingInfo->value("UpdateRegister/ProdID", QString("NULL")).toString();
	//上传GA数据
	GAThread::getInstance()->insertEventItem(UserDefineDimension, "", strPid, 2);
}

void FSRecorder::transfromRegisterInfo()
{
	connect(UpdateRegisterPurchaseHelper::GetInstance(), &UpdateRegisterPurchaseHelper::signalCheckOnStart, this, [&](bool bResult){
		GAThread::getInstance()->insertEventItem(UserDefineDimension, "", bResult?"Registered":"Unregisered",1);
	});
}

//bool FSRecorder::nativeEvent(const QByteArray &eventType, void *message, long *result)
//{
//	MSG *msg = static_cast<MSG *>(message);
//	if (msg->message == MSG_QUIT_LAST_APP)
//	{
//		bool bCancel = DoClose(0);
//		if (bCancel)
//		{
//			HWND preHwnd = (HWND)msg->wParam;
//			::SendMessage(preHwnd, MSG_REFUSE_QUIT, 0, 0);
//		}
//	}
//	else if (msg->message == MSG_REFUSE_QUIT)
//	{
//		AppStartController::GetInstance()->SetAnotherRefuse();
//	}
//	return false;
//}

//////////////////////////////////////////////////////////////////////////
static const int leftrightMaring = 69;
FSRecorderMainView::FSRecorderMainView(QWidget *parent /*= 0*/)
	:FSBaseWidget(parent)
{
	RecorderViewsMgr::GetInstance()->SetFSRecorderMainView(this);
	FSBaseWidget *m_pPage = new GettingStartPage(this);
	m_lstPages.push_back(m_pPage);
	m_pPage = new SetttingPage(this);
	m_lstPages.push_back(m_pPage);

	m_pPage = new PreviewPage(this);
	m_lstPages.push_back(m_pPage);

	m_pPageHeader = new PageHeader(this);
	ShowPage(Page_GettingStart);
    setAttribute(Qt::WA_TranslucentBackground);
}

FSRecorderMainView::~FSRecorderMainView()
{

}

QWidget* FSRecorderMainView::GetPageView(Page page)
{
	return m_lstPages[page];
}

void FSRecorderMainView::ShowPage(Page page)
{
	for (auto pageWidget : m_lstPages)
		pageWidget->setVisible(false);

	m_lstPages[page]->setVisible(true);
	m_pPageHeader->SetCurrentPage(page);
    signalShowPage(page);
}

FSRecorderMainView::Page FSRecorderMainView::GetCurrentPage()
{
    return m_pPageHeader->GetCurrentPage();
}

void FSRecorderMainView::showEvent(QShowEvent *e)
{
	static bool bFirst = true;
	if (bFirst)
	{
		bFirst = false;
		UpdateRegisterPurchaseHelper::GetInstance()->DoAutoUpdate();
	}
	SysTrayIcon::GetInstance()->SetVisible(false);
	FSBaseWidget::showEvent(e);
}

void FSRecorderMainView::resizeEvent(QResizeEvent *e)
{
	static const int HeaderViewH = 71;
    static const int HeaderViewH1 = 81;
	m_pPageHeader->setGeometry(leftrightMaring, 0, width() -leftrightMaring-32, HeaderViewH);
    for (int i = 0; i < m_lstPages.size(); i++)
    {
        if (i == 0)
            m_lstPages.at(i)->setGeometry(0, HeaderViewH, width(), height() - HeaderViewH);
        else
            m_lstPages.at(i)->setGeometry(0, HeaderViewH1, width(), height() - HeaderViewH1);
    }
		
}

//////////////////////////////////////////////////////////////////////////

QString sGetProjectDefaultPath()
{
    QString path = PathOperation::GetDocumentPath() + "/FilmoraScreen";
    QDir dir(path);
    if (!dir.exists())
    {
        dir.mkpath(path);
    }
    return path;
}

extern int sMenuWidth;
extern void SaveProjThumbnail();

ButtonBar::ButtonBar(QWidget *titleBar, QWidget *parent /*= 0*/)
	:FSBaseWidget(parent)
    , m_titleBar(titleBar)
{
	//setAttribute(Qt::WA_TransparentForMouseEvents);
	m_curPage = FSRecorderMainView::Page_GettingStart;
	m_pBtnRegistration = new FSIconButton(this);
	m_pBtnPurchase = new FSIconButton(this);
	m_pBtnMenu = new FSIconButton(this);

	m_pBtnRegistration->setObjectName("PageHeader_m_pBtnRegistration");
	m_pBtnPurchase->setObjectName("PageHeader_m_pBtnPurchase");
	m_pBtnMenu->setObjectName("PageHeader_m_pBtnMenu");
    m_pBtnMenu->setVisible(false);

	m_pMenu = new QMenu(this);
    m_pMenu->setFixedWidth(sMenuWidth);
	m_pActPreviewWindow = new QAction(this);
	m_pActCatureSetup = new QAction(this);
    m_pActGotoEditor = new QAction(this);
    m_pActSaveProject = new QAction(this);
    m_pActSaveAsProject = new QAction(this);
    m_pActExit = new QAction(this);
    InitSaveProjectDirectory();

	RetranslateUi();
	connect(UpdateRegisterPurchaseHelper::GetInstance(), &UpdateRegisterPurchaseHelper::sigRegisterResult, this, [&](bool bSucc){
		if (bSucc)
			Communicator::GetInstance()->TellRegisterResult(bSucc);
		RetranslateUi();
	});

	connect(m_pBtnRegistration, &QPushButton::clicked, this, [&]{
		UpdateRegisterPurchaseHelper::GetInstance()->Register();
		TransFromRegisterGaInfo();
	});
	connect(m_pBtnPurchase, &QPushButton::clicked, this, [&]{
		UpdateRegisterPurchaseHelper::GetInstance()->Purchase();
		//上传GA数据
		TransFromBuyGaInfo();
	});
	connect(m_pBtnMenu, &QPushButton::clicked, this, [&]{
		m_pMenu->clear();
		if (m_curPage == FSRecorderMainView::Page_Setting)
		{
			m_pMenu->addAction(m_pActPreviewWindow);
			m_pActPreviewWindow->setEnabled(RecorderViewsMgr::GetInstance()->GetRecordPanel()->CanGo2PreviewWin());
		}
		else if (m_curPage == FSRecorderMainView::Page_Preview)
		{
			m_pMenu->addAction(m_pActCatureSetup);
		}
		m_pMenu->addAction(m_pActGotoEditor);

        if (m_curPage == FSRecorderMainView::Page_Preview)
        {
            m_pMenu->addSeparator();
            m_pMenu->addAction(m_pActSaveProject);
            m_pMenu->addAction(m_pActSaveAsProject);
            m_pMenu->addAction(m_pActExit);
        }

		m_pMenu->exec(mapToGlobal(m_pBtnMenu->geometry().bottomLeft()));
	});
	connect(m_pActPreviewWindow, &QAction::triggered, this, [&]{
		RecorderViewsMgr::GetInstance()->ShowMainView(FSRecorderMainView::Page_Preview);
	});
	connect(m_pActCatureSetup, &QAction::triggered, this, [&]{
		RecorderViewsMgr::GetInstance()->ShowMainView(FSRecorderMainView::Page_Setting);
	});
	connect(m_pActGotoEditor, &QAction::triggered, this, [&]{
		RecorderViewsMgr::GetInstance()->GotoEditor();
	});

    connect(m_pActSaveProject, &QAction::triggered, this, [&]{

        if (ProjectMgr::GetInstance()->IsNewProject())
        {
            QString newpath = QFileDialog::getSaveFileName(RecorderViewsMgr::GetInstance()->GetFSRecorder(), tr("Please select directory"), GetSaveProjectDirectory(), "Project Files(*.wfsproj)");
            if (!newpath.isEmpty())
            {
                SaveProjectDirectory(newpath);
                SaveProjThumbnail();
                ProjectMgr::GetInstance()->SaveNew(newpath);
            }
        }
        else
        {
            if (ProjectMgr::GetInstance()->IsContentChanged())
            {
                ProjectMgr::GetInstance()->Save();
            }
        }
    });

    connect(m_pActSaveAsProject, &QAction::triggered, this, [&]{

        QString newpath = QFileDialog::getSaveFileName(RecorderViewsMgr::GetInstance()->GetFSRecorder(), tr("Please select directory"), GetSaveProjectDirectory(), "Project Files(*.wfsproj)");
        if (!newpath.isEmpty())
        {
            SaveProjectDirectory(newpath);
            SaveProjThumbnail();
            ProjectMgr::GetInstance()->SaveNew(newpath);
        }
    });


    connect(m_pActExit, &QAction::triggered, this, [&]{
        DoClose(RecorderViewsMgr::GetInstance()->GetFSRecorder());
    });


	connect(UpdateRegisterPurchaseHelper::GetInstance(), &UpdateRegisterPurchaseHelper::singnalSucessRegister, this, [&](bool bResult){
		//上传GA信息
		QDateTime curTime = QDateTime::currentDateTime();
		qint64 costTime = GAThread::getInstance()->getStartRegisterTime().secsTo(curTime);
		GAThread::getInstance()->insertEventItem(Register, "RegsterFinished", QString::number(costTime*1000));
		qDebug() << QString::number(costTime).append("s");
		if (bResult)
		{
			QDateTime time = QDateTime::currentDateTime();
			//读取软件的安装时间
			HKEY h;
			int iRec = RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\Wondershare Filmora Scrn", 0, KEY_ALL_ACCESS, &h);
			if (iRec == 0)
			{
				QSettings regSetting("HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\Wondershare\\Wondershare Filmora Scrn", QSettings::NativeFormat);
				QDateTime installTime = regSetting.value("InstallInfo", "fail to read").toDateTime();
				qint64 costDays = installTime.daysTo(time);
				GAThread::getInstance()->insertEventItem(Register, "RegisterDate", QString::number((costDays + 1)*1000));
			}
			else
			{
				QSettings regSetting("HKEY_LOCAL_MACHINE\\SOFTWARE\\Wondershare\\Wondershare Filmora Scrn", QSettings::NativeFormat);
				QDateTime installTime = regSetting.value("InstallInfo", "fail to read").toDateTime();
				qint64 costDays = installTime.daysTo(time);
				GAThread::getInstance()->insertEventItem(Register, "RegisterDate", QString::number((costDays + 1)*1000));
			}			
		}
		//上传验证接口的时间性能
		GAThread::getInstance()->insertEventItem(Register, "RegsterFinished", QString::number(costTime*1000));
	});
	//启动注册
	connect(UpdateRegisterPurchaseHelper::GetInstance(), &UpdateRegisterPurchaseHelper::singnalStartRegister, this, [&](QDateTime time){
		GAThread::getInstance()->setStartRegisterTime(time);
	});
}

void ButtonBar::SetCurrentPage(FSRecorderMainView::Page page)
{
	m_curPage = page;
	m_pBtnMenu->setVisible(m_curPage != FSRecorderMainView::Page_GettingStart);
	RetranslateUi();
}

int ButtonBar::Width()
{
	return 0;
}

void ButtonBar::TransFromRegisterGaInfo()
{
	//上传GA信息
	if (m_curPage == FSRecorderMainView::Page_GettingStart)
	{
		GAThread::getInstance()->insertEventItem(Register, "Register_entrance", "startup");
	}
	else if (m_curPage == FSRecorderMainView::Page_Preview)
	{
		GAThread::getInstance()->insertEventItem(Register, "Register_entrance", "yourclips");
	}
	else if (m_curPage == FSRecorderMainView::Page_Setting)
	{
		GAThread::getInstance()->insertEventItem(Register, "Register_entrance", "setup");
	}
}

void ButtonBar::TransFromBuyGaInfo()
{
	//上传GA信息
	if (m_curPage == FSRecorderMainView::Page_GettingStart)
	{
		GAThread::getInstance()->insertEventItem(Buy, "Buy_entrance", "startup");
	}
	else if (m_curPage == FSRecorderMainView::Page_Preview)
	{
		GAThread::getInstance()->insertEventItem(Buy, "Buy_entrance", "yourclips");
	}
	else if (m_curPage == FSRecorderMainView::Page_Setting)
	{
		GAThread::getInstance()->insertEventItem(Buy, "Buy_entrance", "setup");
	}
}

void ButtonBar::RetranslateUi()
{
	QString strRes = UpdateRegisterPurchaseHelper::GetInstance()->IsRegistered() ? tr("Registered") : tr("Register");
	m_pBtnRegistration->setText(strRes);
	m_pBtnRegistration->setToolTip(tr("Register your software with registration code"));
	m_pBtnPurchase->setText(tr("Purchase"));
	m_pBtnPurchase->setToolTip(tr("Purchase product"));
	m_pBtnMenu->setText(tr("Menu"));

	m_pActPreviewWindow->setText(tr("Preview window"));
	m_pActCatureSetup->setText(tr("Capture setup"));
	m_pActGotoEditor->setText(tr("Go to editor"));

    m_pActSaveProject->setText(tr("Save Project"));
    m_pActSaveAsProject->setText(tr("Save Project as..."));
    m_pActExit->setText(tr("Exit"));

    resizeEvent(nullptr);
}

void ButtonBar::resizeEvent(QResizeEvent *e)
{
	static const int ButtonW = 90;
	static const int ButtonH = height();
	//注册按钮和购买按钮距离右边的的距离
	//int iMenuBtnToRightLen = width() - 206;
	//int iRegistBtnToRightLen = width() - 104;
	//int iPurchaseToRightLen = width() - 52;

    auto GetLent = [](QFont font, QString text)->int{
        return GetRetranslateUiTextLenght(font, text, ButtonW - 30) + 30;
    };
    int newLenght = GetLent(m_pBtnPurchase->font(), m_pBtnPurchase->text());
    int left = width() - newLenght;
    m_pBtnPurchase->setGeometry(left, 0, newLenght, ButtonH);

    newLenght = GetLent(m_pBtnRegistration->font(), m_pBtnRegistration->text());
    left = m_pBtnPurchase->geometry().left() - newLenght;
    m_pBtnRegistration->setGeometry(left, 0, newLenght, ButtonH);

    newLenght = GetRetranslateUiTextLenght(m_pBtnMenu->font(), m_pBtnMenu->text(), 70 - 30) + 30;
    left = m_pBtnRegistration->geometry().left() - newLenght;
    m_pBtnMenu->setGeometry(left, 0, newLenght, ButtonH);
}

void ButtonBar::mousePressEvent(QMouseEvent *event)
{
    QApplication::sendEvent(m_titleBar, event);
    
    //FSBaseWidget::mousePressEvent(event);
}

void ButtonBar::mouseMoveEvent(QMouseEvent *event)
{
    QApplication::sendEvent(m_titleBar, event);
    //FSBaseWidget::mouseMoveEvent(event);

}

void ButtonBar::mouseReleaseEvent(QMouseEvent *event)
{
    QApplication::sendEvent(m_titleBar, event);
    //FSBaseWidget::mouseReleaseEvent(event);

}

QString ButtonBar::GetSaveProjectDirectory()
{
    return m_strSaveProjectDirectory;
}

void ButtonBar::SaveProjectDirectory(QString path)
{
    m_strSaveProjectDirectory = path;
}

void ButtonBar::InitSaveProjectDirectory()
{
    m_strSaveProjectDirectory = sGetProjectDefaultPath();
    QString path = FSGetFilmoraScreenDirectory() + "/settingdatas.xml";
    QDomDocument doc;
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
        return;

    if (!doc.setContent(&file))
    {
        return;
    }
    QDomElement rootElem = doc.documentElement();
    QDomElement subElem = rootElem.firstChildElement("SavePath");
    if (!subElem.isNull())
    {
        m_strSaveProjectDirectory = subElem.text();
    }
}

//////////////////////////////////////////////////////////////////////////
PageHeader::PageHeader(QWidget *parent /*= 0*/)
	:FSBaseWidget(parent)
{
	m_curPage = FSRecorderMainView::Page_GettingStart;
	m_pLbTitle = new QLabel(this);
	m_pLbTitle->setObjectName("FSRecorderMainView_PageHeader_TitleLabel");
    m_pLbTitle->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	m_pBtnRegistration = new FSIconButton(this);
	m_pBtnRegistration->setVisible(false);
	m_pBtnPurchase = new FSIconButton(this);
	m_pBtnPurchase->setVisible(false);
	m_pBtnMenu = new FSIconButton(this);
	m_pBtnMenu->setVisible(false);

	m_pBtnRegistration->setObjectName("PageHeader_m_pBtnRegistration");
	m_pBtnPurchase->setObjectName("PageHeader_m_pBtnPurchase");
	m_pBtnMenu->setObjectName("PageHeader_m_pBtnMenu");

	m_pMenu = new QMenu(this);
	m_pActPreviewWindow = new QAction(this);
	m_pActCatureSetup = new QAction(this);
	m_pActGotoEditor = new QAction(this);

	RetranslateUi();
	connect(m_pBtnRegistration, &QPushButton::clicked, this, [&]{
		UpdateRegisterPurchaseHelper::GetInstance()->Register();
		
	});
	connect(m_pBtnPurchase, &QPushButton::clicked, this, [&]{
		UpdateRegisterPurchaseHelper::GetInstance()->Purchase();
		
	});
	connect(m_pBtnMenu, &QPushButton::clicked, this, [&]{
		m_pMenu->clear();
		if (m_curPage == FSRecorderMainView::Page_Setting)
		{
			m_pMenu->addAction(m_pActPreviewWindow);
			m_pActPreviewWindow->setEnabled(RecorderViewsMgr::GetInstance()->GetRecordPanel()->CanGo2PreviewWin());
		}
		else if (m_curPage == FSRecorderMainView::Page_Preview)
		{
			m_pMenu->addAction(m_pActCatureSetup);
		}
		m_pMenu->addAction(m_pActGotoEditor);
		m_pMenu->exec(mapToGlobal(m_pBtnMenu->geometry().bottomLeft()));
	});
	connect(m_pActPreviewWindow, &QAction::triggered, this, [&]{
		RecorderViewsMgr::GetInstance()->ShowMainView(FSRecorderMainView::Page_Preview);
	});
	connect(m_pActCatureSetup, &QAction::triggered, this, [&]{
		RecorderViewsMgr::GetInstance()->ShowMainView(FSRecorderMainView::Page_Setting);
	});
	connect(m_pActGotoEditor, &QAction::triggered, this, [&]{
		RecorderViewsMgr::GetInstance()->GotoEditor();
	});

}

void PageHeader::SetCurrentPage(FSRecorderMainView::Page page)
{
	m_curPage = page;
	RetranslateUi();
}

FSRecorderMainView::Page PageHeader::GetCurrentPage()
{
    return m_curPage;
}

void PageHeader::RetranslateUi()
{
	switch (m_curPage)
	{
	case FSRecorderMainView::Page_GettingStart:
        m_pLbTitle->setText("<img src = ':/FSRecorder/Resources/light/filmorascrn_logo.png' />");
		break;
	case FSRecorderMainView::Page_Setting:
		m_pLbTitle->setText(tr("Setup"));
		break;
	case FSRecorderMainView::Page_Preview:
		m_pLbTitle->setText(tr("Your Clips"));
		break;
	}
	m_pBtnRegistration->setText(tr("Register"));
    m_pBtnRegistration->setToolTip(tr("Register your software with registration code"));
	m_pBtnPurchase->setText(tr("Purchase"));
    m_pBtnPurchase->setToolTip(tr("Purchase product"));

	m_pActPreviewWindow->setText(tr("Preview window"));
	m_pActCatureSetup->setText(tr("Capture setup"));
	m_pActGotoEditor->setText(tr("Go to editor"));
}

void PageHeader::resizeEvent(QResizeEvent *e)
{
	static const int ButtonW = 100;
	static const int ButtonH = 22;
	m_pLbTitle->setGeometry(0, 10, 284, 48);
	int iWidth = width();
	//注册按钮和购买按钮距离右边的的距离
	int iMenuBtnToRightLen = width() - 329;
	int iRegistBtnToRightLen = width() - 219;
	int iPurchaseToRightLen = width() - 102;
	m_pBtnRegistration->setGeometry(iRegistBtnToRightLen, 24, ButtonW, ButtonH);
	m_pBtnPurchase->setGeometry(iPurchaseToRightLen, 24, ButtonW, ButtonH);
	m_pBtnMenu->setGeometry(iMenuBtnToRightLen, 24, ButtonW, ButtonH);
}

//////////////////////////////////////////////////////////////////////////
GettingStartPage::GettingStartPage(QWidget *parent /*= 0*/)
	:FSBaseWidget(parent)
{
	m_pBtnGetStart = new FSNormalButton("FSRecorderMainView_GettingStartPage_GetStartButton", this);
	m_pBtnQuickCapture = new QPushButton(this);
	m_pBtnQuickCapture->setObjectName("FSRecorderMainView_GettingStartPage_CaptureButton");
	m_pBtnVideoEditor = new QPushButton(this);
	m_pBtnVideoEditor->setObjectName("FSRecorderMainView_GettingStartPage_EditorButton");
	m_pProjectList = new ProjectListWidget(this);
	m_pProjectList->setObjectName("FSRecorderMainView_GettingStartPage_ProjectListCtrl");
	m_pProjectList->verticalScrollBar()->setObjectName("FSRecorderMainView_GettingStartPage_ProjectListCtrl_VScrollBar");
	ConnectSignals();
	RetranslateUi();
}

void GettingStartPage::ConnectSignals()
{
	connect(m_pBtnGetStart, &FSNormalButton::clicked, this, [&]{

		//上传按钮事件信息
		GAThread::getInstance()->insertEventItem(Start_Up, "NextStep", "GetStarted");
		((FSRecorderMainView*)parent())->ShowPage(FSRecorderMainView::Page_Setting);
		//上传页面切换信息
		GAThread::getInstance()->insertEventItem(Change_Page, "Setup", "");

        ProjectAutoSaveMgr::GetInstance()->LoadProject(nullptr);
	});
	connect(m_pBtnQuickCapture, &QPushButton::clicked, this, [&]{
		//上传按钮事件信息
		GAThread::getInstance()->insertEventItem(Start_Up, "NextStep", "QuickCapture");
		auto recentInfo = ProjectFilesMgr::GetInstance()->GetRecentInfos();
		if (recentInfo.size() > 0)  //setting应用最近一次配置
		{
			ProjectMgr::GetInstance()->Load(recentInfo[0]->filepath,true);
		}

        ProjectAutoSaveMgr::GetInstance()->LoadProject(nullptr);
		RecorderViewsMgr::GetInstance()->ShowRecordWidget();
	});
	connect(m_pBtnVideoEditor, &QPushButton::clicked, this, [&]{
		//上传按钮事件信息
		GAThread::getInstance()->insertEventItem(Start_Up, "NextStep", "VideoEditor");
		GAThread::getInstance()->startTime();
		GAThread::getInstance()->setIsStartFromStartup(true);
		RecorderViewsMgr::GetInstance()->GotoEditor();
        ProjectAutoSaveMgr::GetInstance()->LoadProject(nullptr);
    });

	connect(m_pProjectList, &ProjectListWidget::sigItemClicked, this, [&](MVListItemData *item){
		if (item->GetItemDataBase()->GetType() != ItemType_NewProj) return;
		//上传按钮点击事件
		GAThread::getInstance()->insertEventItem(Start_Up, "NextStep", "CreateNew");
		((FSRecorderMainView*)parent())->ShowPage(FSRecorderMainView::Page_Setting);
		//上传页面切换信息
		GAThread::getInstance()->insertEventItem(Change_Page, "Setup", "");
        ProjectAutoSaveMgr::GetInstance()->LoadProject(nullptr);
    
    });
	connect(m_pProjectList, &ProjectListWidget::sigItemDoubleClicked, this, [&](MVListItemData *item){
		if (item->GetItemDataBase()->GetType() == ItemType_NewProj) return;
		ProjectFileInfo* itemInfo = ((OpenProjData*)item->GetItemDataBase())->GetProjectFileInfo();
		if (!QFile::exists(itemInfo->filepath))
		{
			FSMessageBox dlg(tr("Filmora Scrn"), tr("The project file does not exist and can not be opened."), Qt::AlignCenter, FSMessageBox::OK, this);
			dlg.SetIconObjectName("MessageBox_Icon_Information");
			dlg.DoModal();

			ProjectFilesMgr::GetInstance()->Remove(itemInfo);
			m_pProjectList->RemoveItem(item);
			return;
		}
		//上传GA信息
		GAThread::getInstance()->insertEventItem(Start_Up, "NextStep", "Project");
		GAThread::getInstance()->startTime();
		GAThread::getInstance()->setIsStartFromStartup(true);
		GAThread::getInstance()->setLoadProjectType(true);

        ProjectAutoSaveMgr::GetInstance()->LoadProject(itemInfo);
        //ProjectFilesMgr::GetInstance()->SetCurrent(itemInfo);
        //ProjPreproccessor::GetInstance()->LoadProject(itemInfo->filepath);
	});
}

void GettingStartPage::showEvent(QShowEvent *e)
{
	ProjectFilesMgr::GetInstance()->GetInfos().size() > 0 ? AppearStatus(1) : AppearStatus(0);
}

void GettingStartPage::resizeEvent(QResizeEvent *e)
{
		int l = leftrightMaring + 283;
		int t = 134;
		int w = 246;
		int h = 50;
		m_pBtnGetStart->setGeometry(l, t, w, h);

		l = leftrightMaring + 192;
		w = 206;
		h = 50;
		t = height() - 56 - h;

		//对于有无工程，需要做不同的处理
		if (ProjectFilesMgr::GetInstance()->GetInfos().length() > 0)
		{
			m_pBtnQuickCapture->setGeometry(l, t, w, h);
			m_pBtnVideoEditor->setGeometry(m_pBtnQuickCapture->geometry().right() + 20, t, w, h);
		}
		else
		{
			//没有工程的情况下，按钮下移
			m_pBtnQuickCapture->setGeometry(l, t - 98, w, h);
			m_pBtnVideoEditor->setGeometry(m_pBtnQuickCapture->geometry().right() + 20, t - 98, w, h);
		}
	l = 32;
	t = 53;
	w = width() - 42;
	h = height() - 163;
	m_pProjectList->setGeometry(l, 10, w, h);
}

void GettingStartPage::AppearStatus(int nState)
{
	m_nAppreaState = nState;
	m_pBtnGetStart->setVisible(nState == 0);
	m_pProjectList->setVisible(nState == 1);
}

void GettingStartPage::RetranslateUi()
{
	m_pBtnGetStart->setText(tr("Get Started"));
    m_pBtnGetStart->setToolTip(tr("To start new workflow for recording and editing"));
	m_pBtnQuickCapture->setText(tr("Quick Capture"));
    m_pBtnQuickCapture->setToolTip(tr("Go to recorder"));
	m_pBtnVideoEditor->setText(tr("Video Editor"));
    m_pBtnVideoEditor->setToolTip(tr("Open video editor"));
}

//////////////////////////////////////////////////////////////////////////
SetttingPage::SetttingPage(QWidget *parent /*= 0*/)
	: FSBaseWidget(parent)
{
    m_pSettingWidget = new FSSettingsWidget(this);
	m_pbtnCapture = new FSButton(this);
	m_pbtnCapture->setObjectName("FSRecorderMainView_SetttingPage_CaptureButton");	
	ConnectSignals();
	RetranslateUi();
}

void SetttingPage::ConnectSignals()
{
	connect(m_pbtnCapture, &QPushButton::clicked, this, [&]{
		RecorderViewsMgr::GetInstance()->ShowRecordWidget();
	});
	connect(m_pSettingWidget, &FSSettingsWidget::signalCaptureEnabled, this, [&](bool bEnable){
		m_pbtnCapture->setEnabled(bEnable);
	});
}

void SetttingPage::resizeEvent(QResizeEvent *e)
{
	int l = 0;
	int t = 0;
	int w = width();
	int h = height();
	m_pSettingWidget->setGeometry(l, t, w, h);

	w = 150;
	h = 45;
	m_pbtnCapture->setGeometry(width()-w-64, 337, w, h);

}

void SetttingPage::showEvent(QShowEvent *e)
{
	m_pbtnCapture->setEnabled(m_pSettingWidget->GetCaptureEnabled());
}

void SetttingPage::RetranslateUi()
{
	m_pbtnCapture->setText(tr("Capture") + "...");
    m_pbtnCapture->setToolTip(tr("Complete recording device setup and open recoder"));
}

//////////////////////////////////////////////////////////////////////////
#include "RecorderPreview.h"
PreviewPage::PreviewPage(QWidget *parent /*= 0*/)
	: FSBaseWidget(parent)
{
    m_preview = new RecorderPreview(this);
    ConnectSignals();
}

void PreviewPage::Reset()
{
	m_preview->Reset();
}

QPixmap PreviewPage::GetFirstItemThumbnail()
{
	return m_preview->GetFirstItemThumbnail();
}

void PreviewPage::slotRecordStop(QString path)
{
    m_preview->AddItem(path);
    //if (!(SettingsDataManager::GetInstance()->GetGameData()->GetDeviceEnabled() && 
    //    SettingsDataManager::GetInstance()->GetGameData()->IsGameFullScreen()))
    //{
    //   FSFindTargetFile(path);
    //   QTimer::singleShot(2000, [&](){
    //        RecorderViewsMgr::GetInstance()->ShowMainView(FSRecorderMainView::Page_Preview);

    //    });
    //}
}

void PreviewPage::slotCaptureMore()
{
	RecorderViewsMgr::GetInstance()->ShowRecordWidget();
}

void PreviewPage::slotExport2Editor(QStringList paths)
{
	QTimer::singleShot(0, [&,paths]{
		RecorderViewsMgr::GetInstance()->GotoEditor();

		auto transTimer = new QTimer;
		connect(transTimer, &QTimer::timeout, this, [&,paths,transTimer]{
			if (Communicator::GetInstance()->IsEditShowed())
			{
				transTimer->stop();
				Communicator::GetInstance()->TranslteRecordFiles(paths);
				delete transTimer;
			}
		});
		transTimer->start(100);
	});
}
	

void PreviewPage::slotExport2Local(QStringList paths)
{
	QList<FSNLETimeline *> pTimelinelist;

	for (auto path: paths)
	{
		auto timeline = NLEWraper::GetInstance().GetPlayerMaterProvider()->CreateTimelineForScreenRecording(path);
        if (timeline == nullptr)
            continue;

		int childCount = 0;
		timeline->GetClipCount(&childCount);
		if (childCount > 0)
		{
			auto fsTimeline = new FSNLETimeline(timeline);
			fsTimeline->SetFilePath(path);
			pTimelinelist.push_back(fsTimeline);
		}
		else
		{
			// 输出时普通视频要放在空的timeline下面
			auto rootTimeline = NLEWraper::GetInstance().GetPlayerMaterProvider()->CreateEmptyTimeline();
			rootTimeline->AddClip(timeline);
			NLEPropComPtr propComp = NLEWraper::GetInstance().GetPlayerMaterProvider()->NLEGetProperties(timeline);
			propComp->SetLevel(0);
			propComp->SetPosition(0);
			auto fsTimeline = new FSNLETimeline(rootTimeline);
			fsTimeline->SetFilePath(path);
			pTimelinelist.push_back(fsTimeline);
		}
	}
	IFSExport exportDlg(pTimelinelist, this);
    if (pTimelinelist.size() == 1)
        exportDlg.SetRecorderExportState(false);

	connect(&exportDlg, &IFSExport::RegisterResaultSig, this, [&](bool bRegistered){
		if (bRegistered)
			UpdateRegisterPurchaseHelper::GetInstance()->RebuildRegisterMain(true);
	});

	exportDlg.DoModal();
	qDeleteAll(pTimelinelist);
}

void PreviewPage::ConnectSignals()
{
	connect(m_preview, &RecorderPreview::signalCaptureMore, this, &PreviewPage::slotCaptureMore, Qt::UniqueConnection);
	connect(m_preview, &RecorderPreview::signalExport2Editor, this, &PreviewPage::slotExport2Editor, Qt::UniqueConnection);
	connect(m_preview, &RecorderPreview::signalExport2Local, this, &PreviewPage::slotExport2Local, Qt::UniqueConnection);
	bool ret = connect(RecorderViewsMgr::GetInstance()->GetRecordPanel(), &RecordPanel::sigNormalStop, this, &PreviewPage::slotRecordStop, Qt::UniqueConnection);
    
    // RecordPanel() 创建要在FSRecorderMainView前面
    Q_ASSERT(ret);
}

void PreviewPage::showEvent(QShowEvent *e)
{
	//ConnectSignals();
	RecorderViewsMgr::GetInstance()->GetRecordPanel()->SetPreviewWindowShowed();
}

void PreviewPage::resizeEvent(QResizeEvent *e)
{
	m_preview->setGeometry(0, 0, width(), height());
}

//////////////////////////////////////////////////////////////////////////
RecordHelper::RecordHelper(QObject *parent)
: QObject(parent)
{
	m_rcCustom.setWidth(1280);
	m_rcCustom.setWidth(720);
	//m_rcDesktop = DesktopMgr::GetInstance()->MainScreen()->rect();
    //m_rcDesktop = PrimaryScreenManager::GetInstance()->PrimaryScreen();
	m_rcRecord = m_rcDesktop;
	m_rcPrevious = m_rcDesktop;
	m_Lock = false;
	m_ShiftLock = false;
	m_bRecordRectModified = false;
    m_bDraging = false;
	m_minW = 120;
	m_minH = 120;
	m_maxW = m_rcDesktop.width();
	m_maxH = m_rcDesktop.height();
	for (int i = DragableRectIndex_Left; i <= DragableRectIndex_Center; ++i)
		m_dragableRects.push_back(QRect());
	connect(MKCollecter::GetInstance(), &MKCollecter::sigKeyPressed, this, [&](int vk,int mk){
		if (vk == VK_LSHIFT || vk == VK_RSHIFT)
		{
			if(RecorderCenter::GetInstance()->GetReordState() == RecorderCenter::RecordState_None && !IsLock())
				SetShiftLock(true);
		}
			
	});
	connect(MKCollecter::GetInstance(), &MKCollecter::sigKeyUp, this, [&](int vk){
		if (vk == VK_LSHIFT || vk == VK_RSHIFT)
		{
			if (RecorderCenter::GetInstance()->GetReordState() == RecorderCenter::RecordState_None && !IsLock())
				SetShiftLock(false);
		}
	});
	CalcDragableRects();
}
RecordHelper* RecordHelper::GetInstance()
{
	static RecordHelper ins;
	return &ins;
}
void RecordHelper::DragAndMove(DragableRectIndex index, QPoint pos)
{
	if (index == RecordHelper::DragableRectIndex_Center) MoveRecordRect(pos);
	(IsLock() || GetShiftLock()) ? RestrictDrag(index, pos) : ManualDrag(index, pos);
	CalcDragableRects();
	m_bRecordRectModified = true;
	SettingsDataManager::GetInstance()->GetDesktopData()->SetCropRect(m_rcRecord);
	SettingsDataManager::GetInstance()->GetDesktopData()->SetType(m_rcRecord == DesktopRect() ? DesktopData::CaputerType_Fullscreen : DesktopData::CaputerType_Custom);
	if (m_rcRecord != DesktopRect())
		m_rcCustom = m_rcRecord;
	
	emit sigResolutionChangedByDrag(m_rcRecord);
	emit Update();
}

void RecordHelper::SetRecordRect(QRect rc)
{
	if (m_rcRecord == rc) return;
	m_rcPrevious = m_rcRecord;
	if (m_rcRecord != DesktopRect())
		m_rcCustom = m_rcRecord;
	m_rcRecord = rc;	
	CalcDragableRects();
	m_bRecordRectModified = true;
	SettingsDataManager::GetInstance()->GetDesktopData()->SetCropRect(m_rcRecord);
	SettingsDataManager::GetInstance()->GetDesktopData()->SetType(m_rcRecord == DesktopRect() ? DesktopData::CaputerType_Fullscreen : DesktopData::CaputerType_Custom);
	emit Update();
}

void RecordHelper::SetRecordRectLeft(int l)
{
	l = m_rcStartMove.left() + l - m_ptStartMove.x();
	if (l < 0) l = 0;
	if (l > m_rcRecord.right() - m_minW + 1) l = m_rcRecord.right() - m_minW + 1;
	m_rcRecord.setX(l);
}

void RecordHelper::SetRecordRectRight(int r)
{
	r = m_rcStartMove.right() + r - m_ptStartMove.x();
    if (r > m_rcDesktop.width()) r = m_rcDesktop.width();
	if (r < m_rcRecord.left() + m_minW - 1) r = m_rcRecord.left() + m_minW - 1;
	m_rcRecord.setRight(r);
}

void RecordHelper::SetRecordRectTop(int t)
{
	t = m_rcStartMove.top() + t - m_ptStartMove.y();
	if (t < 0) t = 0;
	if (t > m_rcRecord.bottom() - m_minH + 1) t = m_rcRecord.bottom() - m_minH + 1;
	m_rcRecord.setTop(t);
}

void RecordHelper::SetRecordRectBottom(int b)
{
	b = m_rcStartMove.bottom() + b - m_ptStartMove.y();
    if (b > m_rcDesktop.height()) b = m_rcDesktop.height();
	if (b < m_rcRecord.top() + m_minH - 1) b = m_rcRecord.top() + m_minH - 1;
	m_rcRecord.setBottom(b);
}

void RecordHelper::StartMove(QPoint pt)
{
	m_ptStartMove = pt;
	m_rcStartMove = m_rcRecord;
    m_bDraging = true;
}

void RecordHelper::StopMove()
{
    m_bDraging = false;
}

void RecordHelper::MoveRecordRect(QPoint pt)
{
	int disX = pt.x() - m_ptStartMove.x();
	int disY = pt.y() - m_ptStartMove.y();

	if (disX < 0)
	{
		int newL = m_rcStartMove.left() + disX;
		m_rcRecord.moveLeft(newL < 0 ? 0 : newL);
	}
	else
	{
		int newR = m_rcStartMove.right() + disX;
        m_rcRecord.moveRight(newR > m_rcDesktop.width() ? m_rcDesktop.width() : newR);
	}

	if (disY < 0)
	{
		int newT = m_rcStartMove.top() + disY;
		m_rcRecord.moveTop(newT < 0 ? 0 : newT);
	}
	else
	{
		int newB = m_rcStartMove.bottom() + disY;
        m_rcRecord.moveBottom(newB > m_rcDesktop.height() ? m_rcDesktop.height() : newB);
	}
}

void RecordHelper::SetDesktopRect(QRect rc)
{
    m_rcDesktop = rc;
    m_maxW = m_rcDesktop.width();
    m_maxH = m_rcDesktop.height();
}

QRect RecordHelper::DesktopRect()
{
	return m_rcDesktop;
}

QRect RecordHelper::RecordRect()
{
    return m_rcRecord;
}
void RecordHelper::SetPreviousRect(QRect rc)
{
    m_rcRecord = rc;
}


QRect RecordHelper::PreviousRect()
{
    auto rect = PrimaryScreenManager::GetInstance()->PrimaryScreen();
    if (m_rcPrevious.width() > rect.width() || m_rcPrevious.height() > rect.height())
    {
        m_rcPrevious = rect.intersected(QRect((rect.width() - 1280) / 2, (rect.height() - 720) / 2, 1280, 720));
    }
	return m_rcPrevious;
}

QRect RecordHelper::CustomRect()
{
	return m_rcCustom;
}

QRect RecordHelper::LeftMaskRect()
{
	QRect rc(0, m_rcRecord.top(), m_rcRecord.left(), m_rcRecord.height());
	return rc;
}

QRect RecordHelper::TopMaskRect()
{
	QRect rc(0, 0, m_rcDesktop.width(), m_rcRecord.top());
	return rc;
}

QRect RecordHelper::RightMaskRect()
{
    QRect rc(m_rcRecord.right() + 1, m_rcRecord.top(), m_rcDesktop.width() - m_rcRecord.right(), m_rcRecord.height());
	return rc;
}

QRect RecordHelper::BottomMaskRect()
{
	QRect rc(0, m_rcRecord.bottom() + 1, m_rcDesktop.width(), m_rcDesktop.bottom() - m_rcRecord.bottom());
	return rc;
}

QRect RecordHelper::DragableRect(DragableRectIndex index)
{
	return m_dragableRects[index];
}

void RecordHelper::ManualDrag(DragableRectIndex index, QPoint pos)
{
	if (index == RecordHelper::DragableRectIndex_Left)
	{
		SetRecordRectLeft(pos.x());
	}
	else if (index == RecordHelper::DragableRectIndex_Top)
	{
		SetRecordRectTop(pos.y());
	}
	else if (index == RecordHelper::DragableRectIndex_Right)
	{
		SetRecordRectRight(pos.x());
	}
	else if (index == RecordHelper::DragableRectIndex_Bottom)
	{
		SetRecordRectBottom(pos.y());
	}
	else if (index == RecordHelper::DragableRectIndex_LeftTop)
	{
		SetRecordRectLeft(pos.x());
		SetRecordRectTop(pos.y());
	}
	else if (index == RecordHelper::DragableRectIndex_RightTop)
	{
		SetRecordRectRight(pos.x());
		SetRecordRectTop(pos.y());
	}
	else if (index == RecordHelper::DragableRectIndex_RightBottom)
	{
		SetRecordRectRight(pos.x());
		SetRecordRectBottom(pos.y());
	}
	else if (index == RecordHelper::DragableRectIndex_LeftBottom)
	{
		SetRecordRectLeft(pos.x());
		SetRecordRectBottom(pos.y());
	}
}

void RecordHelper::RestrictDrag(DragableRectIndex index, QPoint pos)
{
	double dRatio = m_fLockRatio;//1.0 * m_rcRecord.width() / m_rcRecord.height();//1.0 * m_rcStartMove.width() / m_rcStartMove.height();

	int l, t, b, r, h, w;
	if (index == RecordHelper::DragableRectIndex_Left)
	{
		int dis = pos.x() - m_ptStartMove.x();
		l = m_rcStartMove.left() + dis;
		if (l < 0) l = 0;
		if (l > m_rcStartMove.right() - m_minW + 1) l = m_rcStartMove.right() - m_minW + 1;
		w = m_rcStartMove.right() - l;
		h = w / dRatio;
		t = m_rcStartMove.top() + (m_rcStartMove.height() - h) * 0.5;
		if (t < 0)
		{
			t = 0;
			h = m_rcStartMove.height() + m_rcStartMove.top() * 2;
			w = h * dRatio;
			l = m_rcStartMove.right() - w + 1;
		}
		if ((t + h - 1 > m_maxH - 1))
		{
			b = m_maxH - 1;
			h = m_rcStartMove.height() + (b - m_rcStartMove.bottom()) * 2;
			w = h * dRatio;
			t = m_rcStartMove.top() - (b - m_rcStartMove.bottom());
			l = m_rcStartMove.right() - w + 1;
		}
		if (h < m_minH)
		{
			h = m_minH;
			w = h * dRatio;
			l = m_rcStartMove.right() - w + 1;
			t = m_rcStartMove.top() + (m_rcStartMove.height() - h) * 0.5;
		}
		m_rcRecord = QRect(l, t, w, h);
	}
	else if (index == RecordHelper::DragableRectIndex_Top)
	{
		int dis = pos.y() - m_ptStartMove.y();
		t = m_rcStartMove.top() + dis;
		if (t < 0) t = 0;
		if (t > m_rcStartMove.bottom() - m_minH + 1) t = m_rcStartMove.bottom() - m_minH + 1;
		h = m_rcStartMove.bottom() - t;
		w = h * dRatio;
		l = m_rcStartMove.left() + (m_rcStartMove.width() - w) * 0.5;
		if (l < 0)
		{
			l = 0;
			w = m_rcStartMove.width() + m_rcStartMove.left() * 2;
			h = w / dRatio;
			t = m_rcStartMove.bottom() - h + 1;
		}
		if ((l + w - 1 > m_maxW - 1))
		{
			r = m_maxW - 1;
			w = m_rcStartMove.width() + (r - m_rcStartMove.right()) * 2;
			h = w / dRatio;
			l = m_rcStartMove.left() - (r - m_rcStartMove.right());
			t = m_rcStartMove.bottom() - h + 1;
		}
		if (w < m_minW)
		{
			w = m_minW;
			h = w / dRatio;
			t = m_rcStartMove.bottom() - h + 1;
			l = m_rcStartMove.left() + (m_rcStartMove.width() - w) * 0.5;
		}
		m_rcRecord = QRect(l, t, w, h);
	}
	else if (index == RecordHelper::DragableRectIndex_Right)
	{
		int dis = pos.x() - m_ptStartMove.x();
		r = m_rcStartMove.right() + dis;
		if (r > m_maxW - 1 ) r = m_maxW - 1;
		if (r < m_rcStartMove.left() + m_minW - 1) r = m_rcStartMove.left() + m_minW - 1;
		l = m_rcStartMove.left();
		w = r - l + 1;
		h = w / dRatio;
		t = m_rcStartMove.top() + (m_rcStartMove.height() - h) * 0.5;
		if (t < 0)
		{
			t = 0;
			h = m_rcStartMove.height() + m_rcStartMove.top() * 2;
			w = h * dRatio;
		}
		if (t + h - 1 > m_maxH - 1)
		{
			int b = m_maxH - 1;
			h = m_rcStartMove.height() + (b - m_rcStartMove.bottom()) * 2;
			w = h * dRatio;
			t = m_rcStartMove.top() - (b - m_rcStartMove.bottom());
		}
		if (h < m_minH)
		{
			h = m_minH;
			w = h * dRatio;
			t = m_rcStartMove.top() + (m_rcStartMove.height() - h) * 0.5;
		}
		m_rcRecord = QRect(l, t, w, h);
	}
	else if (index == RecordHelper::DragableRectIndex_Bottom)
	{
		int dis = pos.y() - m_ptStartMove.y();
		b = m_rcStartMove.bottom() + dis;
		if (b > m_maxH -1 ) b = m_maxH -1;
		if (b < m_rcStartMove.top() + m_minH - 1) b = m_rcStartMove.top() + m_minH -1;
		t = m_rcStartMove.top();
		h = b - t + 1;
		w = h * dRatio;
		l = m_rcStartMove.left() + (m_rcStartMove.width() - w) * 0.5;
		if (l < 0)
		{
			l = 0;
			w = m_rcStartMove.width() + m_rcStartMove.left() * 2;
			h = w / dRatio;
		}
		if ((l + w - 1 > m_maxW - 1))
		{
			r = m_maxW - 1;
			w = m_rcStartMove.width() + (r - m_rcStartMove.right()) * 2;
			h = w / dRatio;
			l = m_rcStartMove.left() - (r - m_rcStartMove.right());
		}
		if (w < m_minW)
		{
			w = m_minW;
			h = w / dRatio;
			l = m_rcStartMove.left() + (m_rcStartMove.width() - w) * 0.5;
		}
		m_rcRecord = QRect(l, t, w, h);
	}
	else if (index == RecordHelper::DragableRectIndex_LeftTop)
	{
		//只Y轴起作用
		t = m_rcStartMove.top() + pos.y() - m_ptStartMove.y();
		if (t < 0) t = 0;
		if (t > m_rcStartMove.bottom() - m_minH + 1) t = m_rcStartMove.bottom() - m_minH + 1;
		h = m_rcStartMove.bottom() - t + 1;
		w = h * dRatio;
		l = m_rcStartMove.right() - w + 1;
		if (l < 0)
		{
			l = 0;
			w = m_rcStartMove.right() - l + 1;
			h = w / dRatio;
			t = m_rcStartMove.bottom() - h + 1;
		}
		if (l > m_rcStartMove.right() - m_minW + 1)
		{
			l = m_rcStartMove.right() - m_minW + 1;
			w = m_rcStartMove.right() - l + 1;
			h = w / dRatio;
			t = m_rcStartMove.bottom() - h + 1;
		}
		m_rcRecord = QRect(l, t, w, h);
	}
	else if (index == RecordHelper::DragableRectIndex_RightTop)
	{
		//只Y轴起作用
		t = m_rcStartMove.top() + pos.y() - m_ptStartMove.y();
		if (t < 0) t = 0;
		if (t > m_rcStartMove.bottom() - m_minH + 1) t = m_rcStartMove.bottom() - m_minH + 1;
		h = m_rcStartMove.bottom() - t + 1;
		w = h * dRatio;
		l = m_rcStartMove.left();

		r = l + w - 1;
		if (r > m_maxW - 1)
		{
			r = m_maxW - 1;
			w = r - l + 1;
			h = w / dRatio;
			t = m_rcStartMove.bottom() - h + 1;
		}
		if (r < l + m_minW - 1)
		{
			r = l + m_minW - 1;
			w = r - l + 1;
			h = w / dRatio;
			t = m_rcStartMove.bottom() - h + 1;
		}
		m_rcRecord = QRect(l, t, w, h);
	}
	else if (index == RecordHelper::DragableRectIndex_RightBottom)
	{
		//只Y轴起作用
		b = m_rcStartMove.bottom() + pos.y() - m_ptStartMove.y();
		if (b > m_maxH - 1) b = m_maxH - 1;
		if (b < m_rcStartMove.top() + m_minH - 1) b = m_rcStartMove.top() + m_minH - 1;
		l = m_rcStartMove.left();
		t = m_rcStartMove.top();
		h = b - t + 1;
		w = h * dRatio;

		r = l + w - 1;
		if (r > m_maxW - 1)
		{
			r = m_maxW - 1;
			w = r - l + 1;
			h = w / dRatio;
		}
		if (r < l + m_minW - 1)
		{
			r = l + m_minW - 1;
			w = r - l + 1;
			h = w / dRatio;
		}
		m_rcRecord = QRect(l, t, w, h);
	}
	else if (index == RecordHelper::DragableRectIndex_LeftBottom)
	{
		//只Y轴起作用
		b = m_rcStartMove.bottom() + pos.y() - m_ptStartMove.y();
		if (b > m_maxH - 1) b = m_maxH - 1;
		if (b < m_rcStartMove.top() + m_minH - 1) b = m_rcStartMove.top() + m_minH - 1;
		t = m_rcStartMove.top();
		h = b - t + 1;
		w = h * dRatio;
		l = m_rcStartMove.right() - w + 1;

		if (l < 0)
		{
			l = 0;
			w = m_rcStartMove.right() - l + 1;
			h = w / dRatio;
		}
		if (l > m_rcStartMove.right() - m_minW + 1)
		{
			l = m_rcStartMove.right() - m_minW + 1;
			w = m_minW;
			h = w / dRatio;
		}
		m_rcRecord = QRect(l, t, w, h);
	}
}

void RecordHelper::CalcDragableRects()
{
	static const int cornerRcW = 10;
	static const int centerRcW = 90;
	int t = m_rcRecord.top() + (m_rcRecord.height() - cornerRcW) * 0.5;
	m_dragableRects[DragableRectIndex_Left] = QRect(m_rcRecord.left(), t, cornerRcW, cornerRcW);

	m_dragableRects[DragableRectIndex_LeftTop] = QRect(m_rcRecord.left(), m_rcRecord.top(), cornerRcW, cornerRcW);

	int l = m_rcRecord.left() + (m_rcRecord.width() - cornerRcW) * 0.5;
	m_dragableRects[DragableRectIndex_Top] = QRect(l, m_rcRecord.top(), cornerRcW, cornerRcW);

	m_dragableRects[DragableRectIndex_RightTop] = QRect(m_rcRecord.right() - cornerRcW, m_rcRecord.top(), cornerRcW, cornerRcW);
	m_dragableRects[DragableRectIndex_Right] = QRect(m_rcRecord.right() - cornerRcW, t, cornerRcW, cornerRcW);
	m_dragableRects[DragableRectIndex_RightBottom] = QRect(m_rcRecord.right() - cornerRcW, m_rcRecord.bottom() - cornerRcW, cornerRcW, cornerRcW);
	m_dragableRects[DragableRectIndex_Bottom] = QRect(l, m_rcRecord.bottom() - cornerRcW, cornerRcW, cornerRcW);
	m_dragableRects[DragableRectIndex_LeftBottom] = QRect(m_rcRecord.left(), m_rcRecord.bottom() - cornerRcW, cornerRcW, cornerRcW);

	l = m_rcRecord.left() + (m_rcRecord.width() - centerRcW) * 0.5;
	t = m_rcRecord.top() + (m_rcRecord.height() - centerRcW) * 0.5;
	m_dragableRects[DragableRectIndex_Center] = QRect(l, t, centerRcW, centerRcW);
}

bool RecordHelper::IsLock()
{
	return m_Lock;
}

void RecordHelper::SetLock(bool bLock)
{
	if (m_Lock == bLock) return;
	m_Lock = bLock;
	UpdateRatio();
}

bool RecordHelper::GetShiftLock()
{
	return m_ShiftLock;
}

void RecordHelper::SetShiftLock(bool bLock)
{
	if (m_ShiftLock == bLock) return;
	m_ShiftLock = bLock;
	UpdateRatio();
	sigShiftLock(bLock);
}

int RecordHelper::GetMinW()
{
	return m_minW;
}

void RecordHelper::SetMinW(int w)
{
	m_minW = w;
}

int RecordHelper::GetMinH()
{
	return m_minH;
}

void RecordHelper::SetMinH(int h)
{
	m_minH = h;
}

int RecordHelper::GetMaxW()
{
	return m_maxW;
}

void RecordHelper::SetMaxW(int w)
{
	m_maxW = w;
}

int RecordHelper::GetMaxH()
{
	return m_maxH;
}

void RecordHelper::SetMaxH(int h)
{
	m_maxH = h;
}

double RecordHelper::GetRatio()
{
	return m_fLockRatio;
}

void RecordHelper::UpdateRatio()
{
	if (IsLock() || GetShiftLock())
	{
		m_fLockRatio = 1.0 * m_rcRecord.width() / m_rcRecord.height();;
	}
}

bool RecordHelper::IsRecordRectModified()
{
	return m_bRecordRectModified;
}

bool RecordHelper::IsDraging()
{
    return m_bDraging;
}

//////////////////////////////////////////////////////////////////////////////////////////////
RecordWidget::RecordWidget(RecordHelper *helper, QWidget *parent /*= 0*/)
:QWidget(parent)
{
	RecorderViewsMgr::GetInstance()->SetRecordWidget(this);
	m_PressIndex = (RecordHelper::DragableRectIndex) - 1;
	setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
	setAttribute(Qt::WA_TransparentForMouseEvents);
	setAttribute(Qt::WA_TranslucentBackground);
	setAttribute(Qt::WA_MouseTracking);
	m_pHelper = helper;
	connect(m_pHelper, &RecordHelper::sigResolutionChangedByEdit, this, [&](QRect rc){
		repaint();
	});
	connect(m_pHelper, &RecordHelper::Update, this, [&]{
		repaint();
	});

}

RecordWidget::~RecordWidget()
{

}

void RecordWidget::mousePressEvent(QMouseEvent *e)
{
	for (int i = RecordHelper::DragableRectIndex_Left; i <= RecordHelper::DragableRectIndex_Center; ++i)
	{
        QPoint p = this->mapFromGlobal(e->globalPos());
        if (m_pHelper->DragableRect((RecordHelper::DragableRectIndex)i).contains(p))
		{
			m_PressIndex = (RecordHelper::DragableRectIndex)i;
			m_pHelper->StartMove(p);

			//设置RecordPanel专栏提示信息
			if (m_PressIndex != RecordHelper::DragableRectIndex_Center) ((RecordPanel*)gFSGetRecordPanel())->SetCommentTip(0);
		}
	}
}

void RecordWidget::mouseMoveEvent(QMouseEvent *e)
{
    QPoint p = this->mapFromGlobal(e->globalPos());
    if (m_PressIndex == -1)
	{
		Qt::CursorShape ct = Qt::ArrowCursor;
        for (int i = RecordHelper::DragableRectIndex_Left; i <= RecordHelper::DragableRectIndex_Center; ++i)
		{
			RecordHelper::DragableRectIndex index = (RecordHelper::DragableRectIndex)i;
			if (m_pHelper->DragableRect(index).contains(p))
			{
				if (index == RecordHelper::DragableRectIndex_Left || index == RecordHelper::DragableRectIndex_Right)
				{
					ct = Qt::SizeHorCursor;
				}
				else if (index == RecordHelper::DragableRectIndex_Top || index == RecordHelper::DragableRectIndex_Bottom)
				{
					ct = Qt::SizeVerCursor;
				}
				else if (index == RecordHelper::DragableRectIndex_LeftTop || index == RecordHelper::DragableRectIndex_RightBottom)
				{
					ct = Qt::SizeFDiagCursor;
				}
				else if (index == RecordHelper::DragableRectIndex_RightTop || index == RecordHelper::DragableRectIndex_LeftBottom)
				{
					ct = Qt::SizeBDiagCursor;
				}
				else if (index == RecordHelper::DragableRectIndex_Center)
				{
					ct = Qt::SizeAllCursor;
				}
				break;
			}
		}
		setCursor(QCursor(ct));
	}
	else
	{
        //p = e->globalPos();
        m_pHelper->DragAndMove(m_PressIndex, p);
	}
}

void RecordWidget::mouseReleaseEvent(QMouseEvent *e)
{
	m_PressIndex = (RecordHelper::DragableRectIndex) - 1;
	//设置RecordPanel专栏提示信息
	((RecordPanel*)gFSGetRecordPanel())->SetCommentTip(1);
	//::SetFocus((HWND)((RecordPanel*)gFSGetRecordPanel())->winId());
    m_pHelper->StopMove();
}

void RecordWidget::paintEvent(QPaintEvent *)
{
	QPainter p(this);
	p.save();

	QVector<qreal> dashes;
	dashes << 10 << 10;

	QPen pen;
	pen.setColor(Qt::red);
	pen.setWidth(1);
	pen.setDashPattern(dashes);
	p.setPen(pen);
	p.drawRect(m_pHelper->RecordRect());
	p.restore();

	p.save();
	QPen pen2;
	pen2.setColor(Qt::black);
	pen2.setWidth(1);
	p.setPen(pen2);
	p.setBrush(Qt::white);
	for (int i = RecordHelper::DragableRectIndex_Left; i <= RecordHelper::DragableRectIndex_Center; ++i)
	{
		if (i == RecordHelper::DragableRectIndex_Center)
		{
			p.drawPixmap(m_pHelper->DragableRect((RecordHelper::DragableRectIndex)i), QPixmap(":/FSRecorder/Resources/light/center_drag_icon.png"));
		}
		else
		{
			p.drawRect(m_pHelper->DragableRect((RecordHelper::DragableRectIndex)i));
		}
	}
	p.restore();
}

void RecordWidget::resizeEvent(QResizeEvent *e)
{
	repaint();
}

void RecordWidget::showEvent(QShowEvent *e)
{
	DesktopData::CaputerType type = SettingsDataManager::GetInstance()->GetDesktopData()->GetType();
	if (type == DesktopData::CaputerType_Fullscreen)
	{
		m_pHelper->SetRecordRect(m_pHelper->DesktopRect());
		repaint();
        emit m_pHelper->sigResolutionChangedByEdit(m_pHelper->DesktopRect());
    }
	else if (type == DesktopData::CaputerType_Custom)
	{
		//if (!RecordHelper::GetInstance()->IsRecordRectModified())
		{
			QRect rc = SettingsDataManager::GetInstance()->GetDesktopData()->GetCropRect();
			if (rc.isNull())
			{
                rc = PrimaryScreenManager::GetInstance()->DefaultRecordRect();
			}
            RecordHelper::GetInstance()->SetRecordRect(rc);
            emit RecordHelper::GetInstance()->sigResolutionChangedByDrag(rc);
            emit m_pHelper->sigResolutionChangedByEdit(rc);
        }
    }
}

//////////////////////////////////////////////////////////////////////////
RecordMask::RecordMask(RecordHelper *helper, QWidget *parent /*= 0*/)
:QWidget(parent)
{
	RecorderViewsMgr::GetInstance()->SetRecordMask(this);
	m_pHelper = helper;
	setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
	setAttribute(Qt::WA_TranslucentBackground);
	connect(m_pHelper, &RecordHelper::Update, this, [&]{update(); });
}

void RecordMask::showEvent(QShowEvent *e)
{
	SetWindowLong((HWND)winId(), GWL_EXSTYLE, GetWindowLong((HWND)winId(), GWL_EXSTYLE) | WS_EX_TRANSPARENT | WS_EX_LAYERED);
}

void RecordMask::paintEvent(QPaintEvent *e)
{
	QPainter p(this);
	QColor cr = QColor(0, 0, 0, 80);
	p.fillRect(m_pHelper->LeftMaskRect(), cr);
	p.fillRect(m_pHelper->TopMaskRect(), cr);
	p.fillRect(m_pHelper->RightMaskRect(), cr);
	p.fillRect(m_pHelper->BottomMaskRect(), cr);
}

//////////////////////////////////////////////////////////////////////////
RecorderCenter::RecorderCenter(QObject *parent /*= 0*/)
:QObject(parent)
{
	m_recorState = RecordState_None;
	connect(MKCollecter::GetInstance(), &MKCollecter::sigMouseMove, this, [&](QPoint pt){
		//QtConcurrent::run([&]{
			SettingsDataManager::GetInstance()->GetMouseData()->PushMouseMoveFrame(pt);
		//});
	});
	connect(MKCollecter::GetInstance(), &MKCollecter::sigLButtonDown, this, [&](QPoint pt){
		SettingsDataManager::GetInstance()->GetMouseData()->PushLButtonClickedFrame(pt);
	});

    connect(MKCollecter::GetInstance(), &MKCollecter::sigKeyPressed, this, [&](int vk, int mk){
        if (RecorderCenter::GetInstance()->GetReordState() != RecordState_Recording)
            return;

        SettingsDataManager::GetInstance()->GetKeyData()->PushKey(vk, mk);

        //QString vkKeyString = MKCollecter::GetInstance()->GetKeyString(vk);
        //QString shiftString("Shift");
        //QString altString("Alt");
        //QString ctrlString("Ctrl");
        //QString winString("Win");
        //// 组合键判断
        //if (((mk & MKCollecter::ModifyKey_Shift)) ||
        //    ((mk & MKCollecter::ModifyKey_Alt)) ||
        //    ((mk & MKCollecter::ModifyKey_Control)) || 
        //    ((mk & MKCollecter::ModifyKey_Win))
        //    )
        //{
        //    QString hotkeyString = MKCollecter::GetInstance()->GetHotKeyString();
        //    // 按两个控制键，HotKeyString需要去掉后面的Key(Ctrl+Shift+Shift)
        //    if (vkKeyString == shiftString || vkKeyString == altString || vkKeyString == ctrlString || vkKeyString == winString)
        //    {
        //        hotkeyString = hotkeyString.mid(0, hotkeyString.lastIndexOf("+"));
        //    }
        //    SettingsDataManager::GetInstance()->GetKeyData()->PushHotKey(hotkeyString);
        //}
        //else
        //{
        //    // 普通按键
        //    SettingsDataManager::GetInstance()->GetKeyData()->PushKey(MKCollecter::GetInstance()->GetKeyString(vk));
        //}
    });

    connect(SettingsDataManager::GetInstance()->GetAdvancedData(), &AdvancedData::signalAddMarker, this, [&](int vk, int mk){
        if (RecorderCenter::GetInstance()->GetReordState() != RecordState_Recording)
            return;

        SettingsDataManager::GetInstance()->GetKeyData()->PushMarker(vk, mk);

    });

}

RecorderCenter* RecorderCenter::GetInstance()
{
	static RecorderCenter ins;
	return &ins;
}

void RecorderCenter::Start()
{
	NLEComPtr<ICaptureEngineEx> captureEngine = NLECaptureManger::GetInstance()->GetICaptureEngine();
	NLEComPtr<IUnknown> pStream;
	CaptureStreamSetting setting;
    HRESULT hr = S_OK;
	//是否录制屏幕
	bool bIsRecordScreen = false;
    // 1设备问题， 2游戏问题
    QString errorString(tr("Unable to record! An abnormal issue has been reported on the device."));
    if (SettingsDataManager::GetInstance()->GetGameData()->GetHaveGameRecord())
        errorString = QString(tr("Unable to access game data.Please check if your game is running properly."));

    SettingsDataManager::GetInstance()->SetDeviceException(false);

    memset(&setting, 0, sizeof(CaptureStreamSetting));
    if (SettingsDataManager::GetInstance()->GetDesktopData()->GetUnRegisterCaptureStream(pStream, setting))
    {
        captureEngine->AddCaptureStream(pStream, &setting);
    }

    pStream = nullptr;
    memset(&setting, 0, sizeof(CaptureStreamSetting));
    if (SettingsDataManager::GetInstance()->GetDesktopData()->GetCaptureStream(pStream, setting))
	{
		bIsRecordScreen = true;
        if (S_OK != captureEngine->AddCaptureStream(pStream, &setting))
            hr = E_FAIL;
	}
	pStream = nullptr;
    memset(&setting, 0, sizeof(CaptureStreamSetting));
    if (SettingsDataManager::GetInstance()->GetMouseData()->GetCaptureStream(pStream, setting))
	{
        if (S_OK != captureEngine->AddCaptureStream(pStream, nullptr))
            hr = E_FAIL;
	}
    pStream = nullptr;
    memset(&setting, 0, sizeof(CaptureStreamSetting));
    if (SettingsDataManager::GetInstance()->GetKeyData()->GetCaptureStream(pStream, setting))
    {
        if (S_OK != captureEngine->AddCaptureStream(pStream, nullptr))
            hr = E_FAIL;
    }

	memset(&setting, 0, sizeof(CaptureStreamSetting));
	pStream = nullptr;
	bool bIsGameMode = false;
	if (SettingsDataManager::GetInstance()->GetGameData()->GetCaptureStream(pStream, setting))
	{
		bIsGameMode = true;
		SettingsDataManager::GetInstance()->GetGameData()->ShowActiveGameWindow();
        if (S_OK != captureEngine->AddCaptureStream(pStream, &setting))
        {
			QString strName = SettingsDataManager::GetInstance()->GetGameData()->GetGameName();
			//获取无法获取游戏数据的游戏名称
			GAThread::getInstance()->insertEventItem(Recorder_Widget, "GameRecordNameWithoutData", strName);
            errorString = QString(tr("Unable to access game data.Please check if your game is running properly."));
            hr = E_FAIL;
        }
	}
	memset(&setting, 0, sizeof(CaptureStreamSetting));
	pStream = nullptr;
	bool bIsRecordCamera = false;
	if (SettingsDataManager::GetInstance()->GetCameraData()->GetCaptureStream(pStream, setting))
	{
		bIsRecordCamera = true;
        if (S_OK != captureEngine->AddCaptureStream(pStream, &setting))
            hr = E_FAIL;
	}
	memset(&setting, 0, sizeof(CaptureStreamSetting));
	pStream = nullptr;
	bool bIsRecordAudio = false;
	if (SettingsDataManager::GetInstance()->GetAudioData()->GetCaptureStream(pStream, setting))
	{
        if (S_OK != captureEngine->AddCaptureStream(pStream, &setting))
            hr = E_FAIL;
	}

	memset(&setting, 0, sizeof(CaptureStreamSetting));
	pStream = nullptr;
	bool bIsRecordSystemAudio = false;
	if (SettingsDataManager::GetInstance()->GetAudioData()->GetSystemAudioCaptureStream(pStream, setting))
	{
		bIsRecordSystemAudio = true;
        if (S_OK != captureEngine->AddCaptureStream(pStream, &setting))
            hr = E_FAIL;
	}
	m_strOutputFile = GenerateOutputFileName();
	auto str = m_strOutputFile.toStdWString();
    
    if (S_OK != hr)
    {
        QTimer::singleShot(0, [&, errorString](){
            emit sigStartFail(errorString);
            SetState(RecordState_None);
        });
        return;
    }

	hr = E_FAIL;
	captureEngine->SetOutputFile((wchar_t *)str.c_str(), '4PMC');
	captureEngine->Init();
	hr = captureEngine->Start();
    if (SUCCEEDED(hr))
	{
        SettingsDataManager::GetInstance()->StartRecord();
		if (SettingsDataManager::GetInstance()->GetDesktopData()->GetDeviceEnabled())
		{
			SettingsDataManager::GetInstance()->GetMouseData()->PushCurrentCursorFrame();
			EnableMouseHook(true);
		}
		SetState(RecordState_Recording);
	}
    else
    {
        QTimer::singleShot(0, [&, errorString](){
            emit sigStartFail(errorString);
            SetState(RecordState_None);
        });
    }
	//上传启动录制次数
	GAThread::getInstance()->insertEventItem(Recorder_Widget, "StartRecordingbuttonCount", "");
	//上传用户的操作数据
	GAThread::getInstance()->insertEventItem(Recorder_Device, "RecordScreen", bIsRecordScreen ? "YES" : "NO");
	DesktopData::CaputerType type =  SettingsDataManager::GetInstance()->GetDesktopData()->GetType();
	GAThread::getInstance()->insertEventItem(Recorder_Device, "ScreenMode", (type == DesktopData::CaputerType_Custom)?"Custom" : "FullScreen");
	float fRecordFps = SettingsDataManager::GetInstance()->GetDesktopData()->GetFramerate();
	GAThread::getInstance()->insertEventItem(Recorder_Device, "ScreenCaptureFrameRate", QString::number(fRecordFps));
	//获取录制屏幕分辨率
	QRect cropRect = SettingsDataManager::GetInstance()->GetDesktopData()->GetCropRect();
	QString strResolution = QString::number(cropRect.width()).append("*");
	strResolution.append(QString::number(cropRect.height()));
	if (type == DesktopData::CaputerType_Fullscreen)
	{
		//全屏录制情况
        //获取系统桌面分辨率
		QRect rect = QApplication::desktop()->screenGeometry();
		strResolution = QString("%1*%2").arg(rect.width()).arg(rect.height());
		GAThread::getInstance()->insertEventItem(Recorder_Device,"FullScreenResolution",strResolution);
	}
	else
	{
		GAThread::getInstance()->insertEventItem(Recorder_Device, "CustomResolution", strResolution);
	}
   //录制游戏是上传游戏相关信息
    if (bIsGameMode)
	{
		//录制游戏的名称
		QString strGameName;
		strGameName = SettingsDataManager::GetInstance()->GetGameData()->GetGameProcessName();
		GAThread::getInstance()->insertEventItem(Recorder_Device, "GameRecordName", strGameName);
		//获取录制视频的帧率
		QString strGameFrameRate;
		float fFps = SettingsDataManager::GetInstance()->GetGameData()->GetFramerate();
		strGameFrameRate = QString::number(fFps);
		GAThread::getInstance()->insertEventItem(Recorder_Device, "GameFrameRate", strGameFrameRate);
	}
	//上传是否录制音频信息
	GAThread::getInstance()->insertEventItem(Recorder_Device, "RecordAudio", bIsRecordAudio ? "YES" : "NO");
	GAThread::getInstance()->insertEventItem(Recorder_Device, "ComputerAudio", bIsRecordSystemAudio ? "YES" : "NO");
	if (bIsRecordAudio)
	{
		//上传麦克风的名字
		QString strMicrophone;
		wchar_t* microphoneName = SettingsDataManager::GetInstance()->GetAudioData()->GetCurrentDevice().displayName;
		strMicrophone = QString::fromWCharArray(microphoneName);
		GAThread::getInstance()->insertEventItem(Recorder_Device, "MicrophoneName", strMicrophone);
	}
	//是否使用摄像头
	GAThread::getInstance()->insertEventItem(Recorder_Device, "RecordCamera", bIsRecordCamera?"YES":"NO");
	if (bIsRecordCamera)
	{
		//上传摄像头的名字
		QString strCameraName;
		wchar_t* CameraName = SettingsDataManager::GetInstance()->GetCameraData()->GetCurrentDevice().displayName;
		strCameraName = QString::fromWCharArray(CameraName);
		GAThread::getInstance()->insertEventItem(Recorder_Device, "CameraDeviceName", strCameraName);
		//上传摄像头分辨率
		QString strCameraResolution;
		QSize resolution = SettingsDataManager::GetInstance()->GetCameraData()->GetResolution();
		strCameraResolution = QString::number(resolution.width()).append("*");
		strCameraResolution.append(QString::number(resolution.height()));
		GAThread::getInstance()->insertEventItem(Recorder_Device, "CameraResolution", strCameraResolution);
		//上传摄像头帧率
		QString strCameraFPS;
		float fCameraFps = SettingsDataManager::GetInstance()->GetCameraData()->GetFramerate();
		strCameraFPS = QString::number(fCameraFps);
		GAThread::getInstance()->insertEventItem(Recorder_Device, "CameraFrameRate", strCameraFPS);
	}
}

void RecorderCenter::Stop()
{

	NLECaptureManger::GetInstance()->GetICaptureEngine()->Stop();
	StopCameraStream();
	SettingsDataManager::GetInstance()->StopRecord();
	EnableMouseHook(false);
	SetState(RecordState_None);
	//上传用户操作
	GAThread::getInstance()->insertEventItem(Recorder_Widget, "StopRecordingbuttonCount", "");
	//传递录制结束时视频的信息
	GAThread::getInstance()->insertEventItem(Recorder_Widget, "RecordDurationRange", "");
}

void RecorderCenter::Abort()
{
	Stop();
	QFile::remove(m_strOutputFile);
	EnableMouseHook(false);
	SetState(RecordState_None);
}

void RecorderCenter::Pause()
{
	NLECaptureManger::GetInstance()->GetICaptureEngine()->Pause();
	EnableMouseHook(false);
	SetState(RecordState_Pause);
	//上传用户操作
	GAThread::getInstance()->insertEventItem(Recorder_Widget, "PauseRecordingbuttonCount", "");
}

void RecorderCenter::Resume()
{
	NLECaptureManger::GetInstance()->GetICaptureEngine()->Resume();
	if (SettingsDataManager::GetInstance()->GetDesktopData()->GetDeviceEnabled()) EnableMouseHook(true);
	SetState(RecordState_Recording);
	//上传用户操作
	GAThread::getInstance()->insertEventItem(Recorder_Widget, "ResumeRecordingbuttonCount", "");
}

RecorderCenter::RecordState RecorderCenter::GetReordState()
{
	return m_recorState;
}

QString RecorderCenter::GetOutputFile()
{
	return m_strOutputFile;
}

bool RecorderCenter::IsRecording()
{
    return GetReordState() == RecordState_Recording;
}

void RecorderCenter::EnableMouseHook(bool bEnable)
{
	MKCollecter::GetInstance()->SetMouseHook(bEnable);
}

void RecorderCenter::EnableKeyboardHook(bool bEnable)
{
	MKCollecter::GetInstance()->SetKeyboardHook(bEnable);
}

void RecorderCenter::StopCameraStream()
{
	NLEComPtr<IUnknown> pStream = nullptr;
	CaptureStreamSetting setting;
	if (!SettingsDataManager::GetInstance()->GetCameraData()->GetCaptureStream(pStream, setting))
		return;
	if (!pStream) return;
	CAPTURESTREAM_CONTROL_STOP;
	NLEComPtr<ICaptureStreamControl> p;
	pStream->QueryInterface(IID_ICaptureStreamControl, (void**)&p);
	if (p)
	{
		p->Control(CAPTURESTREAM_CONTROL_STOP, nullptr);
	}
}

void RecorderCenter::PrepareCameraStream()
{
	NLEComPtr<IUnknown> pStream = nullptr;
	CaptureStreamSetting setting;
	if (!SettingsDataManager::GetInstance()->GetCameraData()->GetCaptureStream(pStream, setting))
		return;
	if (!pStream) return;

	NLEComPtr<ICaptureStreamEx> p;
	pStream->QueryInterface(IID_ICaptureStreamEx, (void**)&p);
	if (p)
	{
		p->Init();
		p->Start();
	}
}

QString RecorderCenter::GenerateOutputFileName()
{
	QString s = SettingsDataManager::GetInstance()->GetAdvancedData()->GetRecordFilePath();
	QDir dir(s);
	if (!dir.exists())
	{
		dir.mkpath(s);
	}

	QDateTime date = QDateTime::currentDateTime();
	return s + QString("/Rec_%1.mp4").arg(date.toString(QString("hh.mm.ss yyyyMMdd")));
}

void RecorderCenter::SetState(RecordState state)
{
	if (m_recorState == state) return;
	m_recorState = state;
    emit sigRecordStateChange();

}

//////////////////////////////////////////////////////////////////////////
ActionWidget::ActionWidget(QWidget *parent /*= 0*/)
:QWidget(parent)
{
	RecorderViewsMgr::GetInstance()->SetActionWidget(this);
	m_bPause = false;
	setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
	setAttribute(Qt::WA_TranslucentBackground);
	connect(&m_timer, &QTimer::timeout, this, [&]{
		static int i = 0;
		if (m_bPause) return;
		m_clrCorner =  (++i) % 2 == 0 ?  Qt::red : Qt::yellow;
		update();
	});
}

void ActionWidget::ShowWidget()
{
	m_bPause = false;
	m_timer.start(500);
	show();
	raise();
}

void ActionWidget::HideWidget()
{
    if (RecorderCenter::GetInstance()->GetReordState() != RecorderCenter::RecordState_Recording)
    {
        m_timer.stop();
        hide();
    }

}

void ActionWidget::SetPaused(bool bPause)
{
	m_bPause = bPause;
}

void ActionWidget::paintEvent(QPaintEvent *e)
{
	const int a = 4;
	const int b = 30;
    const int offset = 1;
	QRect rc = RecordHelper::GetInstance()->RecordRect();
	QPainter p(this);
	p.fillRect(rc.left() - a, rc.top() - a, a, b, m_clrCorner);
	p.fillRect(rc.left() - a, rc.top() - a, b, a, m_clrCorner);
	p.fillRect(rc.left() - a, rc.bottom() - b + a, a, b, m_clrCorner);
	p.fillRect(rc.left() - a, rc.bottom() + 1, b, a, m_clrCorner);
	p.fillRect(rc.right() + 1, rc.top() - a, a, b, m_clrCorner);
	p.fillRect(rc.right() - b + a, rc.top() - a, b, a - 1, m_clrCorner);
	p.fillRect(rc.right() + 1, rc.bottom() - b + a, a, b, m_clrCorner);
	p.fillRect(rc.right() - b + a, rc.bottom() + 1, b, a, m_clrCorner);
}


