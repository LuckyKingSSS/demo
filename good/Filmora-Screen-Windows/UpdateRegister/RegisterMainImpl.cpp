#include "stdafx.h"
#include "RegisterMainImpl.h"
#include "inc_UpdateRegister/UpdateRegister_global.h"
#include "inc_UpdateRegister/RegisterMain.h"
#include "CommonWidgets.h"
#include "RegHelper.h"
#include "InternalAPI.h"
#include <QLineEdit>
#include <QApplication>
#include <QPainter>

// 注册回调函数
void RegisterMainImpl::RegCheckFunc(CBSRegCheckResult checkResult, wchar_t* pszMessage){
	m_bReturn = true;
	SetButtonsEnable(true);
	checkResult == crError ? m_pCheckTimer->Check() : m_pCheckTimer->StopCheck();
	if (!m_bDone && (checkResult == crOK || checkResult == crError)){  // 以注册成功处理
		if (m_checkType == CheckOnRegist){
			emit signalCheckOnRegist(true, QDateTime::currentDateTime());
			if (checkResult == crOK) m_messageBox->Information(tr("Wondershare Filmora Scrn"), QString::fromStdWString(pszMessage));
			else m_messageBox->Information(tr("Wondershare Filmora Scrn"), tr("Wondershare Filmora Scrn has been registered!"));  // 网络错误
			m_messageBox->disconnect();
			connect(m_messageBox, &URMessageBox::Ok, this, [&](){
				emit Close();
			});
			connect(m_messageBox, &URMessageBox::Close, this, [&](){
				emit Close();
			});
		}
		else if (m_checkType == CheckOnStart){
			emit signalCheckOnStart(true);
		}
		//注册成功
		emit singnalSucessReg(QDateTime::currentDateTime());
	}
	else if (!m_bDone && checkResult == crWarning){  // 警告用户
		m_messageBox->Information(tr("Wondershare Filmora Scrn"), QString::fromStdWString(pszMessage));
		m_messageBox->disconnect();
		if (m_checkType == CheckOnRegist){
			emit signalCheckOnRegist(true, QDateTime::currentDateTime());
			connect(m_messageBox, &URMessageBox::Ok, this, [&](){
				m_messageBox->Information(tr("Wondershare Filmora Scrn"), tr("Wondershare Filmora Scrn has been registered!"));
				emit Close();
			});
			connect(m_messageBox, &URMessageBox::Close, this, [&](){
				emit Close();
			});
		}
		else if (m_checkType == CheckOnStart){
			emit signalCheckOnStart(true);
		}
		//注册成功
		emit singnalSucessReg(QDateTime::currentDateTime());
	}
	else if (checkResult == crQuit || checkResult == crRefund){  // 非法用户
		if (m_checkType == CheckOnStart) emit signalCheckOnStart(false);
		else if (m_checkType == CheckOnRegist) emit signalCheckOnRegist(false, QDateTime::currentDateTime());
		m_messageBox->Information(tr("Wondershare Filmora Scrn"), QString::fromStdWString(pszMessage));
		m_regHelper->DoUnregistered();
		/*清除注册信息，让用户重新输入*/
		m_email->clear();
		m_regCode->clear();
		/*清空配置文件中的Email和RegKeyCode*/
		m_settings.beginGroup("UpdateRegister");
		m_settings.setValue("Email", "");
		m_settings.setValue("RegKeyCode", "");
		m_settings.endGroup();
		/*往注册表写入卸载跳转URL(注册不成功)*/
		//QString UninstallTrialJumpPage = m_cbsUrlReader->ReadCustomerUrl(GetLocalLanguage(), "UninstallTrialJumpPage");
		//QSettings regSetting(QSettings::NativeFormat, QSettings::SystemScope, "Wondershare", m_cbsUrlReader->ReadCustomerUrl(GetLocalLanguage(), "PID"));  // m_prodId
		//regSetting.setValue("UninstallJumpPageTrial", UninstallTrialJumpPage);
		QSettings fsSetting(QSettings::NativeFormat, QSettings::SystemScope, "Wondershare", m_cbsUrlReader->ReadCustomerUrl("ENG", "PID"));
		fsSetting.setValue("Register", "0");
		if (checkResult == crRefund) emit ReFund();
	}
}

static RegisterMainImpl* s_RegisterMainImpl = nullptr;

void __stdcall RegCheckCallBackFun(CBSRegCheckResult checkResult, wchar_t* pszMessage)
{
	s_RegisterMainImpl->RegCheckFunc(checkResult, pszMessage);
}

RegisterMainImpl::RegisterMainImpl(QWidget *parent)
: URBaseWidget(parent)
, m_bReturn(false)
, m_bDone(false)
, m_registerDLL(0)
, m_regHelper(0)
, m_cbsHelper(0)
, m_checkType(CheckOnStart)
, m_settings(UpdateRegisterInit::GetConfigFileDir() + "/UpdateRegister.ini", QSettings::IniFormat)
{
	s_RegisterMainImpl = this;
	m_pCheckTimer = new CheckTimer;
	m_cbsUrlReader = new CBSUrlReader(this);
	m_captionLabel = new QLabel(/*tr("Register Your Software"),*/ this);
	m_captionLabel->setObjectName("RegisterMainImpl_CaptionLabel");
	m_captionLabel->setAlignment(Qt::AlignCenter);
	m_emailLabel = new QLabel(/*tr("Licensed e-mail:"),*/ this);
	m_emailLabel->setObjectName("RegisterMainImpl_EmailLabel");
	m_regCodeLabel = new QLabel(/*tr("Registration code:"),*/ this);
    m_email = new LineEditInvalidInputAlert(this);
	m_email->installEventFilter(this);  // test
    m_regCode = new LineEditInvalidInputAlert(this);
	m_regCode->installEventFilter(this);  // test
	m_registerBtn = new QPushButton(/*tr("Register"),*/ this);
	m_registerBtn->setObjectName("RegisterBtn");
	m_buyNowBtn = new QPushButton(/*tr("Buy Now"),*/ this);
	m_buyNowBtn->setObjectName("BuyNowBtn");
	m_trialBtn = new QPushButton(/*tr("Free Trial"),*/ this);
	m_buyNowBtn->setObjectName("FreeTrialBtn");
	m_closeBtn = new QPushButton(/*tr("Close"),*/ this);
	m_closeBtn->setObjectName("CloseBtn");
	m_titleBar = new TitleBar(this);
	m_titleBar->setObjectName("RegisterTitleBar");
	m_messageBox = new URMessageBox(URMessageBox::OkMode, 0);
	m_messageBox->setObjectName("Register_MessageBox");

	connect(m_registerBtn, &QPushButton::clicked, this, [&](){
		QDateTime time = QDateTime::currentDateTime();
		emit singalStartRegister(time);
		SetButtonsEnable(false);
		RegisterFunc();
	});
	connect(m_buyNowBtn, &QPushButton::clicked, this, &RegisterMainImpl::BuyNowFunc);
	connect(m_trialBtn, &QPushButton::clicked, this, &RegisterMainImpl::TrialFunc);
	connect(m_closeBtn, &QPushButton::clicked, this, &RegisterMainImpl::CloseFunc);
	connect(m_pCheckTimer, &CheckTimer::TimeOut, this, [&](){
		QString qEmail = QString::fromStdString(m_regHelper->GetEmail());
		std::wstring wstr_email = qEmail.toStdWString();
		wchar_t *email = (wchar_t*)(wstr_email.c_str());
		std::wstring wstr_version = m_curVersion.toStdWString();
		wchar_t* version = (wchar_t*)(wstr_version.c_str());
		m_cbsHelper->Init(GetProductID(), version, NULL);  // 注册之前，先Init
		m_cbsHelper->RegCheck(email, _GetLocalLanguage(), rcCheckcontinually); // en:英文
	});
	connect(m_titleBar, &TitleBar::Close, this, [&](){
		emit Close();
	});
	connect(m_titleBar, &TitleBar::Move, this, &RegisterMainImpl::Move);

	LoadRegisterDLL();

	setObjectName("RegisterMainImpl");
	setAttribute(Qt::WA_StyledBackground);
	setStyleSheet(UpdateRegisterInit::gGetStyleSheet(":/UpdateRegister/Resources/UpdateRegister.css"));

	RetranslateUi();
}

void RegisterMainImpl::RetranslateUi()
{
	m_captionLabel->setText(tr("Register Your Software"));
	m_emailLabel->setText(tr("Licensed e-mail:"));
	m_regCodeLabel->setText(tr("Registration code:"));
	m_registerBtn->setText(tr("Register"));
	m_buyNowBtn->setText(tr("Buy Now"));
	m_trialBtn->setText(tr("Free Trial"));
	m_closeBtn->setText(tr("Close"));
	QString res = CheckRegister() ? tr("Registered") : tr("Register");
	m_titleBar->SetTitle(res);
	resizeEvent(nullptr);
}

bool RegisterMainImpl::eventFilter(QObject *obj, QEvent *event)
{
	if (event->type() == QEvent::ContextMenu){
		QContextMenuEvent *contextMenu = (QContextMenuEvent *)event;
		QMenu *menu = NULL;
		obj == m_email ? menu = m_email->createStandardContextMenu() : menu = m_regCode->createStandardContextMenu();
		if (menu){
			menu->setFixedWidth(190);
			menu->exec(contextMenu->globalPos());
			delete menu;
			return true;
		}
	}
    else if (event->type() == QEvent::FocusOut)
    {
        if (obj == m_email)
        {
            QString text = m_email->text().trimmed();
            if (!text.isEmpty())
                m_email->Invalid(!text.contains("@"));
            else
                m_email->Invalid(false);
        }
        else if (obj == m_regCode)
        {
            QString text = m_regCode->text().trimmed();
            if (!text.isEmpty())
                m_regCode->Invalid(!IsHex(text));
            else
                m_regCode->Invalid(false);
        }
    }
	return URBaseWidget::eventFilter(obj, event);
}

RegisterMainImpl::~RegisterMainImpl()
{
	m_pCheckTimer->deleteLater();
	m_messageBox->deleteLater();
    m_regHelper->Release();  // 接口对象，使用Release()释放
	FreeLibrary(m_registerDLL);
}

void RegisterMainImpl::resizeEvent(QResizeEvent*)
{
	int left, top, right, bottom;
	left = top = right = bottom = 1;
	int const titleHeight = 30;
	m_titleBar->setGeometry(left, top, width() - left - right, titleHeight);
	top = m_titleBar->geometry().bottom() + 1;
	m_captionLabel->setGeometry(left, top, width() - left - right, 50);
	top = m_captionLabel->geometry().bottom() + 10;
	left = 20;
	int emailLabelWidth = InternalAPI::GetTextTrueLength(m_emailLabel->font(), m_emailLabel->text()) + 10;
	int regCodeLabelWidth = InternalAPI::GetTextTrueLength(m_regCodeLabel->font(), m_regCodeLabel->text()) + 10;
	int labelWidth = qMax(emailLabelWidth, regCodeLabelWidth);
	m_emailLabel->setGeometry(left, top, labelWidth, 30);
	m_email->setGeometry(m_emailLabel->geometry().right(), top, width() - m_emailLabel->geometry().right() - 20, 30);
	top = m_emailLabel->geometry().bottom() + 15;
	m_regCodeLabel->setGeometry(left, top, labelWidth, 30);
	m_regCode->setGeometry(m_regCodeLabel->geometry().right(), top, width() - m_regCodeLabel->geometry().right() - 20, 30);
	top = m_regCode->geometry().bottom() + 15;
	
	int closeBtnWidth = InternalAPI::GetTextTrueLength(m_closeBtn->font(), m_closeBtn->text()) + 40;
	int buyNowBtnWidth = InternalAPI::GetTextTrueLength(m_buyNowBtn->font(), m_buyNowBtn->text()) + 20;
	int registerBtnWidth = InternalAPI::GetTextTrueLength(m_registerBtn->font(), m_registerBtn->text()) + 20;
	int freeTrialBtnWidth = InternalAPI::GetTextTrueLength(m_trialBtn->font(), m_trialBtn->text()) + 20;
	m_registerBtn->setGeometry(left, top, registerBtnWidth, 30);
	left = width() - 20 - buyNowBtnWidth - freeTrialBtnWidth - 10;
	m_buyNowBtn->setGeometry(left, top, buyNowBtnWidth, 30);
	left = m_buyNowBtn->geometry().right() + 10;
	m_trialBtn->setGeometry(left, top, freeTrialBtnWidth, 30);
	left = width() - 20 - closeBtnWidth;
	m_closeBtn->setGeometry(left, top, closeBtnWidth, 30);
}

void RegisterMainImpl::showEvent(QShowEvent*)
{
	SetButtonsEnable(true);
	if (CheckRegister()){ //已注册过
		m_email->setText(m_regHelper->GetEmail());
		m_regCode->setText(m_regHelper->GetRegCode());
		m_email->setDisabled(true);
		m_regCode->setDisabled(true);
		m_registerBtn->hide();
		m_buyNowBtn->hide();
		m_trialBtn->hide();
		m_closeBtn->show();
	}
	else{
		m_email->clear();
		m_regCode->clear();
		m_email->setEnabled(true);
		m_regCode->setEnabled(true);
		m_registerBtn->show();
		m_buyNowBtn->show();
		m_trialBtn->show();
		m_closeBtn->hide();
	}
	RetranslateUi();
}

bool RegisterMainImpl::LoadRegisterDLL()
{
	if (!m_registerDLL){
		std::wstring wspath = QApplication::applicationDirPath().toStdWString() + L"/" + L"WS_Register.dll";  // WS_Register.dll放在app目录下
		m_registerDLL = LoadLibrary(wspath.c_str());
	}
	if (m_registerDLL){
		/*第一部分，创建IRegHelper对象*/
		CreateRegHelper regHelperFunc = NULL;
		regHelperFunc = (CreateRegHelper)GetProcAddress(m_registerDLL, "CreateRegHelper");
		if (regHelperFunc == NULL) return false;
		m_settings.beginGroup("System");
		QString CodeKey = m_settings.value("CodeKey").toString();
		QString Value1 = m_settings.value("Value1").toString();
		QString Value2 = m_settings.value("Value2").toString();
		QString GUID = m_settings.value("GUID").toString();
		m_settings.endGroup();

		std::string str_CodeKey = CodeKey.toStdString();
		char *pszCodeKey = (char*)(str_CodeKey.c_str());

		std::string str_Value1 = Value1.toStdString();
		char *pszValue1 = (char*)(str_Value1.c_str());

		std::string str_Value2 = Value2.toStdString();
		char *pszValue2 = (char*)(str_Value2.c_str());

		std::string str_GUID = GUID.toStdString();
		char *pszProgId = (char*)(str_GUID.c_str());

		regHelperFunc(pszCodeKey, pszValue1, pszValue2, pszProgId, m_regHelper);  // 创建IRegHelper对象
		if (m_regHelper == NULL) return false;

		/*第二部分，创建ICBSHelper对象*/
		CreateCBSHelper cbsHelperFunc = NULL;
		cbsHelperFunc = (CreateCBSHelper)GetProcAddress(m_registerDLL, "CreateCBSHelper");
		if (cbsHelperFunc == NULL) return false;
		cbsHelperFunc(m_cbsHelper);  // 创建ICBSHelper对象
		if (m_cbsHelper == NULL) return false;
		// 从配置文件中，读入产品Id、版本号
		m_settings.beginGroup("UpdateRegister");
		m_curVersion = m_settings.value("Version").toString();
		if (m_curVersion.count(".") == 3){ // 若版本号有四位，则只取前三位
			m_curVersion = m_curVersion.mid(0, m_curVersion.lastIndexOf("."));
		}
		m_prodId = m_settings.value("ProdID").toString();
		m_settings.endGroup();
		std::wstring wstr_version = m_curVersion.toStdWString();
		wchar_t *version = (wchar_t*)(wstr_version.c_str());
		m_cbsHelper->Init(GetProductID(), version, NULL);  // ProdID
		m_cbsHelper->Start();

		QString qEmail = QString::fromStdString(m_regHelper->GetEmail());
		std::wstring wstr_email = qEmail.toStdWString();
		wchar_t *email = (wchar_t*)(wstr_email.c_str());

		QString qRegCode = QString::fromStdString(m_regHelper->GetRegCode());
		std::wstring wstr_regCode = qRegCode.toStdWString();
		wchar_t *regCode = (wchar_t*)(wstr_regCode.c_str());

		m_cbsHelper->LocalRegister(email, regCode);
		m_cbsHelper->SetRegCheckCallBack(RegCheckCallBackFun);
		return true;
	}
	else{
		return false;
	}
}

bool RegisterMainImpl::CheckRegister()
{
	if (m_regHelper){
		return m_regHelper->GetRegistered();
	}
	return false;
}

void RegisterMainImpl::RegisterCheckOnStart()
{
	m_checkType = CheckOnStart;
	if (CheckRegister()){ 
		/*注册成功，则往注册表中写入注册成功的卸载跳转地址*/
		//QString UninstallJumpPage = m_cbsUrlReader->ReadCustomerUrl(GetLocalLanguage(), "UninstallJumpPage");
		//QSettings regSetting(QSettings::NativeFormat, QSettings::SystemScope, "Wondershare", m_cbsUrlReader->ReadCustomerUrl(GetLocalLanguage(), "PID"));
		//regSetting.setValue("UninstallJumpPage", UninstallJumpPage);
		QSettings fsSetting(QSettings::NativeFormat, QSettings::SystemScope, "Wondershare", m_cbsUrlReader->ReadCustomerUrl("ENG", "PID"));
		fsSetting.setValue("Register", "1");
		/*本地检测注册通过，则与服务器进一步验证*/
		QString qEmail = QString::fromStdString(m_regHelper->GetEmail());
		std::wstring wstr_email = qEmail.toStdWString();
		wchar_t *email = (wchar_t*)(wstr_email.c_str());
		std::wstring wstr_version = m_curVersion.toStdWString();
		wchar_t *version = (wchar_t*)(wstr_version.c_str());
		m_cbsHelper->Init(GetProductID(), version, NULL);  // 注册之前，先Init
		m_bDone = false;
		m_bReturn = false;
		m_cbsHelper->RegCheck(email, _GetLocalLanguage(), CBSRegCheckType::rcCheckOnStart);
	}
	else{
		emit signalCheckOnStart(false);
	}
}

void RegisterMainImpl::DoUnregistered()
{
	if (m_regHelper){
		m_regHelper->DoUnregistered();
	}
}

void RegisterMainImpl::RegisterFunc()
{
	m_checkType = CheckOnRegist;
	if (m_regHelper){
		QString email = m_email->text().trimmed();
		QString regCode = m_regCode->text().trimmed();
		std::string str_email = email.toStdString();
		std::string str_regCode = regCode.toStdString();
		char *pszEmail = (char*)(str_email.c_str());
		char *pszRegCode = (char*)(str_regCode.c_str());
		if (m_regHelper->DoRegistration(pszEmail, NULL, pszRegCode)){  // 本地注册成功
			/*往配置文件写入Email和RegKeyCode*/
			m_settings.beginGroup("UpdateRegister");
            m_settings.setValue("Email", email);
            m_settings.setValue("RegKeyCode", regCode);
			m_settings.endGroup();
			/*往注册表写入卸载跳转URL(注册成功)*/
			//QString UninstallJumpPage = m_cbsUrlReader->ReadCustomerUrl(GetLocalLanguage(), "UninstallJumpPage");
			//QSettings regSetting(QSettings::NativeFormat, QSettings::SystemScope, "Wondershare", m_cbsUrlReader->ReadCustomerUrl(GetLocalLanguage(), "PID"));
			//regSetting.setValue("UninstallJumpPage", UninstallJumpPage);
			QSettings fsSetting(QSettings::NativeFormat, QSettings::SystemScope, "Wondershare", m_cbsUrlReader->ReadCustomerUrl("ENG", "PID"));
			fsSetting.setValue("Register", "1");
			/*与服务器验证注册事务*/
			std::wstring wstr_email = email.toStdWString();
			wchar_t *lpszEmail = (wchar_t*)(wstr_email.c_str());
			if (m_cbsHelper && m_cbsHelper->Start()){  // 小助手启动成功
				std::wstring wstr_version = m_curVersion.toStdWString();
				wchar_t* version = (wchar_t*)(wstr_version.c_str());
				m_bReturn = false;
				m_bDone = false;
				m_cbsHelper->Init(GetProductID(), version, NULL);  // 注册之前，先Init
				m_cbsHelper->RegCheck(lpszEmail, _GetLocalLanguage(), CBSRegCheckType::rcCheckOnRegist);  // 注册时检测
				QTimer::singleShot(5000, [&](){
					if (!m_bReturn){
						m_bDone = true;
						SetButtonsEnable(true);
						emit signalCheckOnRegist(true, QDateTime::currentDateTime());
						m_messageBox->Information(tr("Wondershare Filmora Scrn"), tr("Wondershare Filmora Scrn has been registered!"));
						m_messageBox->disconnect();
						connect(m_messageBox, &URMessageBox::Ok, this, [&](){
							emit Close();
						});
						connect(m_messageBox, &URMessageBox::Close, this, [&](){
							emit Close();
						});
					}
				});
			}
			else{  // 小助手启动失败，同样提示注册成功
				SetButtonsEnable(true);
				emit signalCheckOnRegist(true, QDateTime::currentDateTime());
				m_messageBox->Information(tr("Wondershare Filmora Scrn"), tr("Wondershare Filmora Scrn has been registered!"));
				m_messageBox->disconnect();
				connect(m_messageBox, &URMessageBox::Ok, this, [&](){
					emit Close();
				});
				connect(m_messageBox, &URMessageBox::Close, this, [&](){
					emit Close();
				});
			}
		}
		else{  // 注册失败
			emit signalCheckOnRegist(false, QDateTime::currentDateTime());
			SetButtonsEnable(true);
			m_messageBox->Information(tr("Wondershare Filmora Scrn"), tr("Invalid licensed e-mail or registration code! Both the licensed e-mail and registration code are case-sensitive, please check your registration information or make sure your registration code matches the current version."));
		}
	}
	else {
		SetButtonsEnable(true);
	}
}

void RegisterMainImpl::BuyNowFunc()
{
	QDesktopServices::openUrl(QUrl(RegisterInit::GetBuyNowUrl())); //"https://store.wondershare.com/shop/buy/buy-video-editor.html?f=pro"
	emit BuyNow();
}

void RegisterMainImpl::TrialFunc()
{
	emit FreeTrial();
}

void RegisterMainImpl::CloseFunc()
{
	emit Close();
}

bool RegisterMainImpl::IsHex(QString text)
{
    for (int i = 0; i < text.size(); ++i)
    {
        QString temp = text[i];
        bool ok = false;
        temp.toInt(&ok, 16);
        if (!ok)
        {
            return false;
        }
    }
    return true;
}

wchar_t * RegisterMainImpl::AToW(char* src)  // 需要在函数外部，主动delete[]返回值
{
	int len = MultiByteToWideChar(CP_ACP, 0, src, strlen(src), NULL, 0);
	wchar_t* m_wchar = new wchar_t[len + 1];
	MultiByteToWideChar(CP_ACP, 0, src, strlen(src), m_wchar, len);
	m_wchar[len] = L'\0';
	return m_wchar;
}

char * RegisterMainImpl::WToA(wchar_t *src)  // 需要在函数外部，主动delete[]返回值
{
	int len = WideCharToMultiByte(CP_ACP, 0, src, wcslen(src), NULL, 0, NULL, NULL);
	char* m_char = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, src, wcslen(src), m_char, len, NULL, NULL);
	m_char[len] = '\0';
	return m_char;
}

void RegisterMainImpl::SetButtonsEnable(bool enabled)
{
	m_registerBtn->setEnabled(enabled);
	m_buyNowBtn->setEnabled(enabled);
	m_trialBtn->setEnabled(enabled);
	m_closeBtn->setEnabled(enabled);
}

wchar_t* RegisterMainImpl::_GetLocalLanguage()
{
	QString lang = GetLocalLanguage();
	if (lang == "ENG"){
		return L"en";
	}
	else if (lang == "DEU"){
		return L"de";
	}
	else if (lang == "PTG"){
		return L"pt";
	}
	else if (lang == "ESP"){
		return L"es";
	}
	else if (lang == "FRA"){
		return L"fr";
	}
	else if (lang == "ITA"){
		return L"it";
	}
	else if (lang == "JPN"){
		return L"ja";
	}
	return L"en";
}

QString RegisterMainImpl::GetLocalLanguage()
{
	QString filmoraScrnDir = InternalAPI::GetProgramDataPath();
	QString iniPath = filmoraScrnDir + "/FilmoraScrn.ini";
	if (!QFile(iniPath).exists()) { return "ENG"; }
	QSettings setting(iniPath, QSettings::IniFormat);
	setting.beginGroup("System");
	auto langname = setting.value("DefInstallLang").toString();
	setting.endGroup();
	return langname;
}

int RegisterMainImpl::GetProductID()
{
	return m_cbsUrlReader->ReadCustomerUrl(GetLocalLanguage(), "PID").toInt();
}

QString RegisterMainImpl::GetCurVersion()
{
	return m_curVersion;
}

//////////////////////////////////////////////////////////////////////////
CheckTimer::CheckTimer()
: QObject()
{
	m_pThread = new QThread;
	m_pTimer = new QTimer(this);
	m_pTimer->setInterval(1000 * 60);
	connect(m_pTimer, &QTimer::timeout, this, &CheckTimer::TimeOut);
	m_pThread->start();
	moveToThread(m_pThread);
}

CheckTimer::~CheckTimer()
{
	m_pThread->deleteLater();
}

void CheckTimer::Check()
{
	QMetaObject::invokeMethod(this, "_Check", Qt::QueuedConnection);
}

void CheckTimer::StopCheck()
{
	QMetaObject::invokeMethod(this, "_StopCheck", Qt::QueuedConnection);
}

Q_INVOKABLE void CheckTimer::_Check()
{
	m_pTimer->start();
}

Q_INVOKABLE void CheckTimer::_StopCheck()
{
	m_pTimer->stop();
}


//////////////////////////////////////////////////////////////////////////
LineEditInvalidInputAlert::LineEditInvalidInputAlert(QWidget *parent)
:QLineEdit(parent)
, m_invalid(false){
    m_alertWidget = new QWidget(this);
    m_alertWidget->setAttribute(Qt::WA_TransparentForMouseEvents);
    m_alertWidget->installEventFilter(this);
}

void LineEditInvalidInputAlert::Invalid(bool invalid)
{
    m_invalid = invalid;
    update();
}

void LineEditInvalidInputAlert::paintEvent(QPaintEvent *event)
{
    QLineEdit::paintEvent(event);  
}

void LineEditInvalidInputAlert::resizeEvent(QResizeEvent *event)
{
    QLineEdit::resizeEvent(event);
    m_alertWidget->setGeometry(rect());
}

bool LineEditInvalidInputAlert::eventFilter(QObject *watcher, QEvent *event)
{
    if (event->type() == QEvent::Paint)
    {
        if (watcher == m_alertWidget)
        {
            QPainter painter(m_alertWidget);
            QPen pen = painter.pen();
            pen.setColor(m_invalid ? QColor(200, 0, 0) : QColor(0, 0, 0, 0));
            painter.setPen(pen);
            painter.setBrush(Qt::NoBrush);
            painter.drawRect(rect().adjusted(0, 0, -1, -1));
        }
    }
    return false;
}
