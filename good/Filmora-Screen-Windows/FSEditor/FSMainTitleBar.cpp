#include "FSMainTitleBar.h"
#include "FSEditor.h"
#include "SettingDataMgr.h"
#include "inc_CommonLib/PathOperation.h"
#include "inc_FSCommonlib/gathread.h"
#include "CommunicatorClient.h"
#include "inc_CommonLib/StringOperation.h"

FSMainTitleBar::FSMainTitleBar(FSEditor *parent)
	: TopLevelWindowTitleBar(parent)
{
	m_bRegistered = false;
	m_bDisableConflics = false;
	m_pBtnIcon = new BaseButton("FSEditor_MainTitleBar_IconButton", this);
	m_pBtnIcon->setAttribute(Qt::WA_TransparentForMouseEvents);
	m_pMenuBar = new QMenuBar(this);
	m_pMenuBar->setObjectName("FSEditor_MainTitleBar_MenuBar");
    m_pMenuBar->setAttribute(Qt::WA_StyledBackground);

	m_pMenuFile = new QMenu(this);
	m_pMenuFile->setFixedWidth(320);
	m_pMenuEdit = new QMenu(this);
	m_pMenuEdit->setFixedWidth(320);
	m_pMenuExport = new QMenu(this);
	m_pMenuExport->setFixedWidth(210);
	m_pMenuHelp = new QMenu(this);
	m_pMenuHelp->setFixedWidth(270);
	m_pMenuOpenRecent = new QMenu(this);

	QAction *pAct = nullptr;
	for (int i = ActionID_File_NewProject; i <= ActionID_File_Close; ++i)
	{
		if (i == ActionID_File_AchiveProject) continue;
		pAct = new QAction(this);
		pAct->setProperty("id", i);
		m_pMenuFile->addAction(pAct);	
		m_lstActions.push_back(pAct);
		if (i == ActionID_File_OpenProject)
		{
			m_pMenuFile->addMenu(m_pMenuOpenRecent);
		}
		if (i == ActionID_File_OpenProject 
			|| i == ActionID_File_SaveProjectAs
			|| i == ActionID_File_ImportMediaFolder
			|| i == ActionID_File_Settings)
		{
			m_pMenuFile->addSeparator();
		}
	}
    for (int i = ActionID_Edit_Undo; i <= ActionID_Edit_SnapToGuides; ++i)
	{
		pAct = new QAction(this);
		pAct->setProperty("id", i);
		m_pMenuEdit->addAction(pAct);
		m_lstActions.push_back(pAct);
		if (i == ActionID_Edit_Redo
            || i == ActionID_Edit_Delete)
		{
			m_pMenuEdit->addSeparator();
		}
        if (i == ActionID_Edit_SnapToGuides)
        {
            pAct->setCheckable(true);
            pAct->setShortcut(QKeySequence(Qt::ALT + Qt::Key_S));
            pAct->setShortcutContext(Qt::ApplicationShortcut);
            pAct->setChecked(SettingDataMgr::GetInstance()->GetSnapToGuides());
        }
	}
	for (int i = ActionID_Export_CreateVideo; i <= ActionID_Export_CreateVideo; ++i)
	{
		pAct = new QAction(this);
		pAct->setProperty("id", i);
		m_pMenuExport->addAction(pAct);
		m_lstActions.push_back(pAct);
	}
	for (int i = ActionID_Help_OnlineHelp; i <= ActionID_Help_About; ++i)
	{
		pAct = new QAction(this);
		pAct->setProperty("id", i);
		m_pMenuHelp->addAction(pAct);
		m_lstActions.push_back(pAct);
		if (i == ActionID_Help_ProductPage)
		{
			m_pMenuHelp->addSeparator();
		}
	}

	m_pMenuBar->addMenu(m_pMenuFile);
	m_pMenuBar->addMenu(m_pMenuEdit);
	m_pMenuBar->addMenu(m_pMenuExport);
	m_pMenuBar->addMenu(m_pMenuHelp);

	m_pProjectName = new FSTitleBarProjectName(this);

	m_pBtnRegister = new FSIconButton(this);
    m_pBtnRegister->setFocusPolicy(Qt::NoFocus);
	m_pBtnRegister->setObjectName("FSEditor_MainTitleBar_RegisterButton");
	m_pBtnPurchase = new FSIconButton(this);
    m_pBtnPurchase->setFocusPolicy(Qt::NoFocus);
	m_pBtnPurchase->setObjectName("FSEditor_MainTitleBar_PurchaseButton");
	ConnectSignals();
	RetranslateUi();
    setFixedHeight(34);
}

void FSMainTitleBar::EnableEditShortCut(bool bEnable)
{
	auto copy = GetAction(ActionID_Edit_Copy);
	auto cut = GetAction(ActionID_Edit_Cut);
	auto paste = GetAction(ActionID_Edit_Paste);
	auto del = GetAction(ActionID_Edit_Delete);

	static bool copyEnable = false;
	static bool cutEnable = false;
	static bool pasteEnable = false;
	static bool delEnable = false;

	if (!bEnable)
	{
		copyEnable = copy->isEnabled();
		copy->setEnabled(false);
		cutEnable = cut->isEnabled();
		cut->setEnabled(false);
		pasteEnable = paste->isEnabled();
		paste->setEnabled(false);
		delEnable = del->isEnabled();
		del->setEnabled(false);
	}
	else
	{
		copy->setEnabled(copyEnable);
		cut->setEnabled(cutEnable);
		paste->setEnabled(pasteEnable);
		del->setEnabled(delEnable);
	}
	m_bDisableConflics = !bEnable;
}

QMenu* FSMainTitleBar::GetMenuFile()
{
	return m_pMenuFile;
}

QMenu* FSMainTitleBar::GetMenuEdit()
{
	return m_pMenuEdit;
}

QMenu* FSMainTitleBar::GetMenuExport()
{
	return m_pMenuExport;
}

QMenu* FSMainTitleBar::GetMenuHelp()
{
	return m_pMenuHelp;
}

QMenu* FSMainTitleBar::GetMenuRecentOpen()
{
	return m_pMenuOpenRecent;
}

void FSMainTitleBar::SetActionEnabled(ActionID id, bool bEnabled)
{
	if (m_bDisableConflics)
	{
		if (id == ActionID_Edit_Copy ||
			id == ActionID_Edit_Cut ||
			id == ActionID_Edit_Delete ||
			id == ActionID_Edit_Paste)
			return;
	}
	auto act = GetAction(id);
	Q_ASSERT(act);
	if (!act) return;
	act->setEnabled(bEnabled);
}

void FSMainTitleBar::SetProjectName(const QString &prjName)
{
	m_pProjectName->SetProjectName(prjName);
}

void FSMainTitleBar::SetRegistered(bool bRegestered)
{
	m_bRegistered = bRegestered;
	m_pProjectName->SetRegistered(bRegestered);
	RetranslateUi();
}

QAction* FSMainTitleBar::GetAction(ActionID id)
{
	for (auto act : m_lstActions)
	{
		if (act->property("id").toInt() == (int)id)
		{
			return act;
		}
	}
	return nullptr;
}

void FSMainTitleBar::ConnectSignals()
{
	for (auto act : m_lstActions)
	{
		connect(act, &QAction::triggered, this, [&]{
            auto act = qobject_cast<QAction*>(sender());
            Q_ASSERT(act);
            emit sigActionTriggled(act->property("id").toInt(), act->isChecked());
		});
	}
	connect(m_pBtnRegister, &FSIconButton::clicked, this, [&]{
		emit sigRegister(); 
		//上传GA数据
		GAThread::getInstance()->insertEventItem(Register, "Register_entrance", "editor");
	});
	connect(m_pBtnPurchase, &FSIconButton::clicked, this, [&]{
		emit sigPuschase(); 
		//上传GA数据
		GAThread::getInstance()->insertEventItem(Buy, "Buy_entrance", "editor");
	});
	//
}

void FSMainTitleBar::RetranslateUi()
{
	QString strRes = m_bRegistered ? tr("Registered") : tr("Register");
	m_pBtnRegister->setText(strRes);
    m_pBtnRegister->setToolTip(tr("Register your software with registration code"));
	m_pBtnPurchase->setText(tr("Purchase"));
    m_pBtnPurchase->setToolTip(tr("Purchase product"));
	m_pMenuFile->setTitle(tr("File"));
	m_pMenuEdit->setTitle(tr("Edit"));
	m_pMenuExport->setTitle(tr("Export"));
	m_pMenuHelp->setTitle(tr("Help"));
	m_pMenuOpenRecent->setTitle(tr("Open Recent"));
	QString actText = "";
	for (auto act : m_lstActions)
	{
		ActionID id = (ActionID)act->property("id").toInt();
		switch (id)
		{
		case FSMainTitleBar::ActionID_File_NewProject:
			act->setShortcut(QKeySequence::New);
			actText = tr("New Project");
			break;
		case FSMainTitleBar::ActionID_File_OpenProject:
			act->setShortcut(QKeySequence::Open);
			actText = tr("Open Project");
			break;
		case FSMainTitleBar::ActionID_File_SaveProject:
			act->setShortcut(QKeySequence::Save);
			actText = tr("Save Project");
			break;
		case FSMainTitleBar::ActionID_File_SaveProjectAs:
			act->setShortcut(QKeySequence("Ctrl+Shift+S"));
			actText = tr("Save Project as..."); 
			break;
		case FSMainTitleBar::ActionID_File_AchiveProject:
			act->setShortcut(QKeySequence("Ctrl+Shift+A"));
            actText = tr("Archive Project");
			break;
		case FSMainTitleBar::ActionID_File_ImportMediaFiles:
			actText = tr("Import Media Files...");
			break;
		case FSMainTitleBar::ActionID_File_ImportMediaFolder:
			actText = tr("Import a Media Folder...");
			break;
        case FSMainTitleBar::ActionID_File_ProjectSettings:
            actText = tr("Project Settings");
            break;
        case FSMainTitleBar::ActionID_File_Settings:
            actText = tr("Preferences");
            break;
		case FSMainTitleBar::ActionID_File_Close:
			act->setShortcut(QKeySequence("Alt+F4"));
			actText = tr("Exit");
			break;
		case FSMainTitleBar::ActionID_Edit_Undo:
			act->setShortcut(QKeySequence::Undo);
			actText = tr("Undo");
			break;
		case FSMainTitleBar::ActionID_Edit_Redo:
			act->setShortcut(QKeySequence::Redo);
			actText = tr("Redo");
			break;
		case FSMainTitleBar::ActionID_Edit_Cut:
			act->setShortcut(QKeySequence::Cut);
			actText = tr("Cut");
			break;
		case FSMainTitleBar::ActionID_Edit_Copy:
			act->setShortcut(QKeySequence::Copy);
			actText = tr("Copy");
			break;
		case FSMainTitleBar::ActionID_Edit_Paste:
			act->setShortcut(QKeySequence::Paste);
			actText = tr("Paste");
			break;
        case FSMainTitleBar::ActionID_Edit_Delete:
            act->setShortcut(QKeySequence::Delete);
            actText = tr("Delete");
            break;
        case FSMainTitleBar::ActionID_Edit_SnapToGuides:
            //act->setShortcut(QKeySequence::Delete);
            actText = tr("Enable Canvas Snap");
            break;
		case FSMainTitleBar::ActionID_Export_CreateVideo:
			actText = tr("Create Video..."); 
			break;
		case FSMainTitleBar::ActionID_Help_OnlineHelp:
			act->setShortcut(QKeySequence::HelpContents);
			actText = tr("Online Help");
			break;
		case FSMainTitleBar::ActionID_Help_ProductFAQ:
			actText = tr("Product FAQ");
			break;
		case FSMainTitleBar::ActionID_Help_SupportCenter:
			actText = tr("Support Center");
			break;
		case FSMainTitleBar::ActionID_Help_NewFeatureRequest:
			actText = tr("New Feature Request");
			break;
		case FSMainTitleBar::ActionID_Help_CheckForUpdate:
			actText = tr("Check for Update...");
			break;
		case FSMainTitleBar::ActionID_Help_ProductPage:
			actText = tr("Product Page");
			break;
		case FSMainTitleBar::ActionID_Help_About:
			actText = tr("About...");
			break;
		}
		act->setShortcutContext(Qt::ApplicationShortcut);
		act->setText(actText);
	}

    resizeEvent(nullptr);
}

extern float gGetDevicePixelRatio();

void FSMainTitleBar::resizeEvent(QResizeEvent *event)
{
	const int space = 2;
	const int btnWidth = 33;
	const int btnHeight = 24;
	int left = space;
	int top = 0;

	const int buttonSize = 3;
	const int buttonsWidth = buttonSize * (btnWidth + space) + 5;

	// 图标
	m_pBtnIcon->setGeometry(12, 3, 100, 18);
	left = m_pBtnIcon->geometry().right() + space;

	// 菜单
    //int menuBarWidth = 240;
    int menuBarWidth = StringOperation::GetTextTrueLenght(m_pMenuBar->font(), m_pMenuFile->title()) +
        StringOperation::GetTextTrueLenght(m_pMenuBar->font(), m_pMenuEdit->title()) +
        StringOperation::GetTextTrueLenght(m_pMenuBar->font(), m_pMenuExport->title()) +
        StringOperation::GetTextTrueLenght(m_pMenuBar->font(), m_pMenuHelp->title()) + 3 * 25;

    //if (MultiLangMgr::GetInstance()->GetLangLocale() == Language_Locale_de)
    //    menuBarWidth = 275;
    
	m_pMenuBar->setGeometry(left, top + 2, menuBarWidth, height() - 4);
	left = m_pMenuBar->geometry().right() + space;

	//注册和购买按钮
	int w = 55;
	const int h = 24;
    w = GetRetranslateUiTextLenght(m_pBtnPurchase->font(), m_pBtnPurchase->text(), w - 23) + 23;
    left = width() - buttonsWidth - w;
	top = (height() - h) * 0.5;
    m_pBtnPurchase->setGeometry(left, top, w, h);
    w = 72;

    w = GetRetranslateUiTextLenght(m_pBtnRegister->font(), m_pBtnRegister->text(), w - 23) + 23;
	left = m_pBtnPurchase->geometry().left() - space - w;
	m_pBtnRegister->setGeometry(left, top, w, h);

	// 工程名
	//auto fff = gGetDevicePixelRatio();
	int al = m_pMenuBar->geometry().right();
	int ar = width() - m_pBtnRegister->geometry().left();
	left = qMax(al, ar);
	//w = width();
	w = width() - 2 * left;
	m_pProjectName->setGeometry(left, top, w, height());
}

bool FSMainTitleBar::event(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		RetranslateUi();
	}
	return TopLevelWindowTitleBar::event(e);
}


//////////////////////////////////////////////////////////////////////////
FSTitleBarProjectName::FSTitleBarProjectName(QWidget *parent)
	:FSBaseWidget(parent)
{
	m_bRegistered = false;
    setAttribute(Qt::WA_TransparentForMouseEvents, true);
	m_pLbProjectName = new QLabel(this);
	m_pLbProjectName->setObjectName("FSMainTitleBar_PrjectNameLabel");
	m_pLbProjectName->setAlignment(Qt::AlignCenter);
	RetranslateUi();
}

void FSTitleBarProjectName::SetProjectName(const QString &projectName)
{
	m_strProjectName = projectName;
	RetranslateUi();
}

QString FSTitleBarProjectName::ProjectName()
{
	return m_strProjectName;
}

void FSTitleBarProjectName::SetRegistered(bool bRegistered)
{
	if (m_bRegistered == bRegistered) return;
	m_bRegistered = bRegistered;
	RetranslateUi();
}

void FSTitleBarProjectName::RetranslateUi()
{

	QString text = (m_strProjectName.isEmpty() ? tr("Untitled Project") : m_strProjectName) + (m_bRegistered ? "" : ("(" + tr("Unregistered") + ")"));
	m_pLbProjectName->setText(text);
}


void FSTitleBarProjectName::resizeEvent(QResizeEvent *e)
{
	m_pLbProjectName->setGeometry(rect());
}
