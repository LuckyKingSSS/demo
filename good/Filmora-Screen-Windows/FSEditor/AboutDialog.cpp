#include "AboutDialog.h"
#include "inc_CommonLib/TopLevelWindowTitleBar.h"
#include "inc_CommonLib/StringOperation.h"
#include "nle_version.h"
//#include <QSettings>

AboutDialog::AboutDialog(QWidget *parent)
	: FSBaseDialog(tr("About"),"About",parent)
{
	resize(550, 380);
	m_titlebar->GetButton(TopLevelWindowTitleBar::Close)->setObjectName("FSEditor_TitleBar_CloseButton");
	m_pLbFSLogo = new QLabel(this);
	m_pLbFSLogo->setObjectName("FSEditor_AboutDialog_Logo");
	m_pLbWSLogo = new QLabel(this);
	m_pLbWSLogo->setObjectName("FSEditor_AboutDialog_WsLogo");

	m_pLbProductName = new QLabel(this);
	m_pLbProductName->setObjectName("FSEditor_AboutDialog_ProductNameLabel");
	m_pLbVersion = new QLabel(this);
	m_pLbVersion->setObjectName("FSEditor_AboutDialog_VersionLabel");
	m_pLbVersion->setAlignment(Qt::AlignLeft);
	m_pLbText1 = new QLabel(this);
	m_pLbText1->setObjectName("FSEditor_AboutDialog_Text1Label");
	m_pLbText1->setAlignment(Qt::AlignTop | Qt::AlignLeft);
	m_pLbText1->setWordWrap(true);
	m_pLbText2 = new QLabel(this);
	m_pLbText2->setObjectName("FSEditor_AboutDialog_Text2Label");
	m_pLbText2->setAlignment(Qt::AlignTop | Qt::AlignLeft);
	m_pLbText2->setWordWrap(true);
	m_pLbWebSite = new QLabel(this);
	m_pLbWebSite->setObjectName("FSEditor_AboutDialog_WebSiteLabel");
	m_pLbWebSite->setAlignment(Qt::AlignTop | Qt::AlignLeft);
	m_pLbWebSite->setText(QString("<style> \
						  		a:link{text-decoration:underline; color:#B73057;} \
								a:hover{text-decoration:underline; color:red;} \
								a:active{text-decoration:none; color:yellow;} \
								a:visited{text-decoration:none; color:green;} \
						</style> \
						<a href=http://%1>%1</a>").arg(tr("www.wondershare.com")));

	m_pLbCopyRight = new QLabel(this);
	m_pLbCopyRight->setObjectName("FSEditor_AboutDialog_CopyRightLabel");
	m_pLbCopyRight->setAlignment(Qt::AlignTop | Qt::AlignLeft);
	RetranslateUi();
    m_titlebar->GetControl()->SetBorderColor(QColor(102,102,102));
    m_titlebar->GetControl()->SetBorderWidth(1);
    m_titlebar->setObjectName("FSEditor_SettingDialog_TitleBar");
    setObjectName("AboutDialog");

	connect(m_pLbWebSite, &QLabel::linkActivated, this, [&](QString url){
		QDesktopServices::openUrl(QUrl(url));
	});
}

void AboutDialog::RetranslateUi()
{
	m_pLbFSLogo->setPixmap(QPixmap(":/FSEditor/Resources/light/filmorascrn_logo.png"));
	m_pLbWSLogo->setPixmap(QPixmap(":/FSEditor/Resources/light/ws_new_logo.png"));
	m_pLbProductName->setText(tr("Wondershare Filmora Scrn"));
	QSettings setting(QApplication::applicationDirPath() + "/UpdateRegister.ini", QSettings::IniFormat);
	setting.beginGroup("UpdateRegister");
	QString Version = setting.value("Version").toString();
	if (Version.count(".") == 3){
		Version = Version.mid(0, Version.lastIndexOf("."));
	}
	setting.endGroup();
	m_pLbVersion->setText(tr("Version:") + Version);

	/////////////底层版本号////////////////////////
	QString vvv = m_pLbVersion->text() + QString("    %1").arg(NLE_VERSION_STRING);
	m_pLbVersion->setText(vvv);
	//////////////////////////////////////////////

	m_pLbText1->setText(tr("Wondershare Technology Corp. is a hi-tech corporation committing to provide global customers with industry-leading software application. Driven by its vision:\"Wonderful Software, Wonderful Life\", Wondershare is dedicated to provide global customers with a diversity of superb software products and services for windows, macintosh, iOS, android and other operating system."));
	m_pLbText2->setText(tr("For more information about Wondershare and Wondershare Products. please visit: "));
	QFontMetrics fontSize(tr("For more information about Wondershare and Wondershare Products. please visit: "));
	int iHeight = fontSize.height();
	m_pLbText2->setFixedHeight(iHeight);
	m_pLbCopyRight->setText(tr("Copyright %1 2017 Wondershare. All rights reserved.").arg(QChar(0x00a9)));

    SetTitle(tr("About"));
}

void AboutDialog::resizeEvent(QResizeEvent *event)
{
	m_titlebar->setGeometry(1, 1, width()-2, 34);

	int margin = 20;

	int w = 271;
	int h = 48;
	int l = margin;
	int t = 60;
	m_pLbFSLogo->setGeometry(l, t, w, h);

	l = margin;
	t = m_pLbFSLogo->geometry().bottom() + 10;
	w = width() - 2 * margin;
	h = 18;
	m_pLbProductName->setGeometry(l, t, w, h);

	l = margin;
	t = m_pLbProductName->geometry().bottom() + 8;
	w = width();
	h = 16;
	m_pLbVersion->setGeometry(l, t, w, h);

	l = margin;
	t = m_pLbVersion->geometry().bottom() + 20;
	w = width() - 2 * margin;
	h = 105;
	m_pLbText1->setGeometry(l, t, w, h);

	l = margin;
	t = m_pLbText1->geometry().bottom() + 11;
	w = width() - 2 * margin;
	h = 12;
	m_pLbText2->setGeometry(l, t, w, h);

	l = margin;
	t = m_pLbText2->geometry().bottom() + 8;
	w = width() - 2 * margin;
	h = 12;
	m_pLbWebSite->setGeometry(l, t, w, 20);

	w = 120;
	h = 17;
	l = margin;
	t = 352;
	m_pLbWSLogo->setGeometry(l, t, w, h);


	w = StringOperation::GetTextTrueLenght(m_pLbCopyRight->font(), m_pLbCopyRight->text());
	h = m_pLbWSLogo->geometry().height();
	l = width() - w - margin;
	t = m_pLbWSLogo->geometry().top();
	m_pLbCopyRight->setGeometry(l, t, w, h);

}

