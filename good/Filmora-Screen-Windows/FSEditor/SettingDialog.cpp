#include "SettingDialog.h"
#include "inc_CommonLib/TopLevelWindowTitleBar.h"

#include "SettingDataMgr.h"

//////////////////////////////////////////////////////////////////////////

enum {Panel_General, Panel_Other, Panel_Canvas};

SettingDialog::SettingDialog(QWidget *parent)
:FSBaseDialog(tr("Preferences"), parent)
{
    setObjectName("SettingDialog");
    resize(540, 360);
    
    m_titlebar->setObjectName("FSEditor_SettingDialog_TitleBar");
    m_titlebar->SetSystemBorderVisible(false);
    m_titlebar->SetButtons(TopLevelWindowTitleBar::Close);
    m_titlebar->GetButton(TopLevelWindowTitleBar::Close)->setObjectName("TitleBar_CloseButton");
    m_titlebar->setAttribute(Qt::WA_StyledBackground);
    m_titlebar->GetControl()->SetBorderColor(QColor(102, 102, 102));
    m_titlebar->GetControl()->SetBorderWidth(1);
    m_titlebar->GetTitleLabel()->setContentsMargins(15, 0, 0, 0);
    m_pLbLine = new QLabel(this);
    m_pLbLine->setObjectName("SettingDialog_m_pLbLine");

    m_pBtnGroup = new QButtonGroup(this);
    
    auto CreateButton = [&](int type, QString objectName){
        auto btn = new QPushButton(this);
        btn->setObjectName(objectName);
        btn->setCheckable(true);
        m_pBtnGroup->addButton(btn, type);
        m_btns.push_back(btn);
        return btn;
    };

    m_pBtnGanunal = CreateButton(Panel_General, "SettingDialog_m_pBtnOhter");
    m_pBtnOhter = CreateButton(Panel_Other, "SettingDialog_m_pBtnOhter");
    m_pBtnCanvas = CreateButton(Panel_Canvas, "SettingDialog_m_pBtnOhter");
    m_pBtnCanvas->hide();

    m_pBtnGanunal->setChecked(true);

    m_pBtnOK = new QPushButton(this);
    m_pBtnCancel = new QPushButton(this);
    m_pBtnCancel->setObjectName("SettingDialog_m_pBtn");
    m_pBtnOK->setObjectName("SettingDialog_m_pBtn");

    ConnectSignals();

    auto CreatePanel = [&](int type, SettingBasePanel *panel){
        m_stackedWidget->addWidget(panel);
        m_panels.insert(type, panel);
    };

    m_stackedWidget = new QStackedWidget(this);
    CreatePanel(Panel_General, new SettingGeneralPanel(this));
    CreatePanel(Panel_Other, new SettingOtherPanel(this));
    //CreatePanel(Panel_Canvas, new SettingCanvasPanel(this));


    ShowPage(Panel_General);

    RetranslateUi();
}

void SettingDialog::Resize()
{
    resizeEvent(nullptr);
}


void SettingDialog::ConnectSignals()
{
    connect(m_pBtnGroup, SIGNAL(buttonClicked(int)), this, SLOT(ShowPage(int)));

    connect(m_pBtnOK, &QPushButton::clicked, this, [&]{
        for (int i = 0; i < m_stackedWidget->count(); ++i)
        {
            (qobject_cast<SettingBasePanel*>(m_stackedWidget->widget(i)))->OnOK();
        }
        SettingDataMgr::GetInstance()->Save();
        SettingDataMgr::GetInstance()->sigOk();
        done(1);
    });
    connect(m_pBtnCancel, &QPushButton::clicked, this, [&]{
        for (int i = 0; i < m_stackedWidget->count(); ++i)
        {
            (qobject_cast<SettingBasePanel*>(m_stackedWidget->widget(i)))->OnCancel();
        }
        done(0);
    });
}

void SettingDialog::ShowPage(int type)
{
    Q_ASSERT(m_panels.contains(type));
    m_stackedWidget->setCurrentWidget(m_panels[type]);
}

void SettingDialog::RetranslateUi()
{
    m_pBtnGanunal->setText(tr("General"));
    m_pBtnOhter->setText(tr("Others"));
    m_pBtnCanvas->setText(tr("Canvas"));

    m_pBtnOK->setText(tr("OK"));
    m_pBtnCancel->setText(tr("Cancel"));
}

void SettingDialog::showEvent(QShowEvent *e)
{
    //Init();
    FSBaseDialog::showEvent(e);
}

void SettingDialog::resizeEvent(QResizeEvent *e)
{
    static const int titlebarH = 34;
    m_titlebar->setGeometry(1, 1, width() - 2, 34);

    const int marging = 20;
    int left = marging;
    int t = titlebarH + 18;
    int h = 47;
    int w = 74;

    for (auto btn : m_btns)
    {
        btn->setGeometry(left, t, w, h);
        left = btn->geometry().right() + marging;
    }
    //m_pBtnGanunal->setGeometry(left, t, w , h);
    //left = m_pBtnGanunal->geometry().right() + marging;
    //m_pBtnOhter->setGeometry(left, t, w , h);
    left = marging;
    t = m_pBtnGanunal->geometry().bottom();
    m_pLbLine->setGeometry(left, t, width() - 2 * marging, 1);
    t = m_pBtnGanunal->geometry().bottom() + 30;
    


    left = marging;
    m_stackedWidget->setGeometry(left, t, width() - 2 * marging, height() - t - 80);


    t = height() - 80;
    w = 126;
    h = 40;
    left = width() - 2 * w - marging - 15;
    m_pBtnOK->setGeometry(left, t, w, h);

    left = m_pBtnOK->geometry().right() + 12;
    m_pBtnCancel->setGeometry(left, t, w, h);
}

//////////////////////////////////////////////////////////////////////////
SettingBasePanel::SettingBasePanel(QWidget *parent)
:FSBaseWidget(parent)
{

}

//////////////////////////////////////////////////////////////////////////
SettingGeneralPanel::SettingGeneralPanel(QWidget *parent)
:SettingBasePanel(parent)
{
    m_pLbSaveProjTo = new QLabel(this);
    m_pLbSaveProjTo->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    m_pEditSavePath = new QLineEdit(this);
    m_pEditSavePath->setReadOnly(true);
    m_pEditSavePath->setObjectName("FSEditor_SettingDialog_SavePathEidt");
    m_pBtnOpen = new BaseButton("FSEditor_SettingDialog_OpenButton", this);
    m_pLbLang = new QLabel(this);
    m_pLbLang->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    m_pCombLang = new FSComboBox(this);
    m_pCombLang->setObjectName("FSEditor_SettingDialog_LanguageCombobox");

    connect(m_pBtnOpen, &BaseButton::clicked, this, [&]{
        QString dir = QFileDialog::getExistingDirectory(this, tr("Please select directory"), SettingDataMgr::GetInstance()->GetProjSavePath());
        if (!dir.isEmpty())
        {
            m_pEditSavePath->setText(dir);
        }
    });

    Init();
    RetranslateUi();
}

QString SettingGeneralPanel::GetSaveProjToPath()
{
    return m_pLbSaveProjTo->text();
}

QString SettingGeneralPanel::GetLang()
{
    return m_pCombLang->currentText();

}

void SettingGeneralPanel::OnOK()
{
    SettingDataMgr::GetInstance()->SetProjSavePath(m_pEditSavePath->text());
    SettingDataMgr::GetInstance()->SetLang(m_pCombLang->currentText());
}

void SettingGeneralPanel::OnCancel()
{

}

void SettingGeneralPanel::RetranslateUi()
{
    m_pLbSaveProjTo->setText(tr("Save project file to:"));
    m_pLbLang->setText(tr("UI language:"));
}

void SettingGeneralPanel::Init()
{
    m_pEditSavePath->setText(SettingDataMgr::GetInstance()->GetProjSavePath());
    m_pCombLang->clear();
    for (int i = 0; i < sizeof(g_LangInfos) / sizeof(LangConstInfo); ++i)
    {
        m_pCombLang->addItem(g_LangInfos[i].fullname);
    }
    m_pCombLang->setCurrentText(SettingDataMgr::GetInstance()->GetLangFullName());


}

void SettingGeneralPanel::showEvent(QShowEvent *event)
{
    //Init();
    FSBaseWidget::showEvent(event);
}

void SettingGeneralPanel::resizeEvent(QResizeEvent *event)
{
    const int margins = 2;
    int left = margins;
    int top = margins;
    int h = 30;
    int w = 126;
    w = GetRetranslateUiTextLenght(m_pLbSaveProjTo->font(), m_pLbSaveProjTo->text(), w);
    m_pLbSaveProjTo->setGeometry(left, top, w, h);
    left = m_pLbSaveProjTo->geometry().right() + 22;

    m_pEditSavePath->setGeometry(left, top, 240, h);
    left = m_pEditSavePath->geometry().right();

    m_pBtnOpen->setGeometry(left, top, 30, h);


    top = m_pLbSaveProjTo->geometry().bottom() + 30;
    left = margins;
    m_pLbLang->setGeometry(left, top, m_pLbSaveProjTo->width(), m_pLbSaveProjTo->height());
    left = m_pEditSavePath->geometry().left();
    m_pCombLang->setGeometry(left, top, 130, h);


}

//////////////////////////////////////////////////////////////////////////
SettingOtherPanel::SettingOtherPanel(QWidget *parent)
:SettingBasePanel(parent)
{
    m_pChbUpgrade = new QCheckBox(this);
    m_pChbGPUAcce = new QCheckBox(this);


    Init();
    RetranslateUi();
}

void SettingOtherPanel::OnOK()
{
    SettingDataMgr::GetInstance()->SetAutoUpgrade(m_pChbUpgrade->isChecked());
    SettingDataMgr::GetInstance()->SetGpuAcceleration(m_pChbGPUAcce->isChecked());
}

void SettingOtherPanel::OnCancel()
{

}

void SettingOtherPanel::RetranslateUi()
{
    m_pChbUpgrade->setText(tr("Check for upgrades automatically"));
    m_pChbGPUAcce->setText(tr("GPU acceleration"));
}

void SettingOtherPanel::Init()
{
    m_pChbUpgrade->setChecked(SettingDataMgr::GetInstance()->IsAutoUpgrade());
    m_pChbGPUAcce->setChecked(SettingDataMgr::GetInstance()->IsGpuAcceleration());
}

void SettingOtherPanel::showEvent(QShowEvent *event)
{
    //Init();
    FSBaseWidget::showEvent(event);
}

void SettingOtherPanel::resizeEvent(QResizeEvent *event)
{

    const int margins = 20;
    int left = margins;
    int top = 2;
    int w = width() - 2*margins;
    int h = 24;
    m_pChbUpgrade->setGeometry(left, top, w, h);
    top = m_pChbUpgrade->geometry().bottom() + 30;
    m_pChbGPUAcce->setGeometry(left, top, m_pChbUpgrade->width(), m_pChbUpgrade->height());

}

//////////////////////////////////////////////////////////////////////////
SettingCanvasPanel::SettingCanvasPanel(QWidget *parent)
:SettingBasePanel(parent)
{
    m_pChbAttach = new QCheckBox(this);
    m_pChbAttach->setChecked(SettingDataMgr::GetInstance()->GetSnapToGuides());

    connect(m_pChbAttach, &QCheckBox::stateChanged, this, [&](int state){
        if (state == Qt::Checked)
        {

        }
        else
        {

        }
    });

    RetranslateUi();
}

void SettingCanvasPanel::OnOK()
{
    SettingDataMgr::GetInstance()->SetSnapToGuides(m_pChbAttach->isChecked());
}

void SettingCanvasPanel::OnCancel()
{

}

void SettingCanvasPanel::RetranslateUi()
{
    m_pChbAttach->setText(tr("Canvas Attach"));
}

void SettingCanvasPanel::Init()
{



}

void SettingCanvasPanel::resizeEvent(QResizeEvent *event)
{
    const int margins = 20;
    int left = margins;
    int top = 2;
    int w = width() - 2 * margins;
    int h = 24;
    m_pChbAttach->setGeometry(left, top, w, h);
}
