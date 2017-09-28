#include "ProjectSettingDialog.h"
#include "inc_CommonLib/TopLevelWindowTitleBar.h"
#include "inc_CommonLib/DragValueChangedHelper.h"
#include "inc_CommonLib/ColorSelecterComboBox.h"
#include "SettingDataMgr.h"

#include "CommunicatorClient.h"
#include "inc_FSCommonlib/DesktopRectangleMgr.h"

static int sComboBoxItemDataIndexFloat(QComboBox *cmb, float value)
{
    for (int i = 0; i < cmb->count(); ++i)
    {
        auto v = cmb->itemData(i).toFloat();
        if (qFuzzyCompare(v, value))
            return i;
    }
    return -1;
}


ProjectSettingDialog::ProjectSettingDialog(QWidget *parent)
: FSBaseDialog(tr("Project Settings"), "", parent)
{

    m_mgr = SettingDataMgr::GetInstance()->GetProejctSettingsMgr();

    resize(780, 600);
    setObjectName("SettingDialog");

    m_titlebar->setObjectName("FSEditor_SettingDialog_TitleBar");
    m_titlebar->SetSystemBorderVisible(false);
    m_titlebar->SetButtons(TopLevelWindowTitleBar::Close);
    m_titlebar->GetButton(TopLevelWindowTitleBar::Close)->setObjectName("TitleBar_CloseButton");
    m_titlebar->setAttribute(Qt::WA_StyledBackground);
    m_titlebar->GetControl()->SetBorderColor(QColor(102, 102, 102));
    m_titlebar->GetControl()->SetBorderWidth(1);
    m_titlebar->GetTitleLabel()->setContentsMargins(15, 0, 0, 0);

    m_lbProjectName = new FSLabel(this);
    m_leProjectname = new FSFileNameLineEdit("FSEditor_SettingDialog_SavePathEidt", this);

    m_lbPreset = new FSLabel(this);
    m_cmbPreset = new FSComboBox(this);

    m_lbWidth = new FSLabel(this);
    m_leWidth = new ResolutionLineEdit("FSEditor_SettingDialog_SavePathEidt", this);
    m_leWidth->setRange(320, 5120);
    m_lbWidthSuffix = new FSLabel(this);


    m_chkLockAspect = new QCheckBox(this);
    m_chkLockAspect->setFocusPolicy(Qt::NoFocus);


    m_lbHeight = new FSLabel(this);
    m_leHeight = new ResolutionLineEdit("FSEditor_SettingDialog_SavePathEidt", this);
    m_leHeight->setRange(240, 2880);
    m_lbHeightSuffix = new FSLabel(this);


    m_lbPlaybackResoultion = new FSLabel(this);
    m_cmbPlaybackResoultion = new FSComboBox(this);


    m_lbBackgroundColor = new FSLabel(this);
    m_cmbColor = new ColorSelecterComboBox(this);
    m_cmbColor->SetBackgroundColor(QColor("#313C48"));
    m_cmbColor->setFocusPolicy(Qt::NoFocus);

    m_lbProjectName->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_lbPreset->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_lbWidth->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_lbHeight->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_lbPlaybackResoultion->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_lbBackgroundColor->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    m_lbWidthSuffix->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_lbHeightSuffix->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_lbWidthSuffix->setText("px");
    m_lbHeightSuffix->setText("px");

    m_btnOK = new FSButton(this);
    m_btnCancel = new FSButton(this);

    m_btnOK->setFocusPolicy(Qt::NoFocus);
    m_btnCancel->setFocusPolicy(Qt::NoFocus);
    m_btnOK->setObjectName("SettingDialog_m_pBtn");
    m_btnCancel->setObjectName("SettingDialog_m_pBtn");

    Init();
    ConnectSignals();
    RetranslateUi();

    setFocus();
}

ProjectSettingDialog::~ProjectSettingDialog()
{

}

void ProjectSettingDialog::resizeEvent(QResizeEvent *event)
{
    const int titleHeight = 30;
    const int space = 5;
    QRect rc = rect().adjusted(1, 1, -1, -1);
    int left = rc.left();
    int top = rc.top();

    m_titlebar->setGeometry(left, top, rc.width(), titleHeight);
    top = m_titlebar->geometry().bottom() + 40;
    
    int lineeditLeft = 270;
    int lbWidth = lineeditLeft - space;
    const int lbHeight = 30;
    const int heightSpace = 20;
    const int lineeditWidth = 75;

    m_lbProjectName->setGeometry(left, top, lbWidth, lbHeight);
    left = m_lbProjectName->geometry().right() + space;
    m_leProjectname->setGeometry(left, top, 300, lbHeight);
    left = 1;
    top = m_leProjectname->geometry().bottom() + 50;

    m_lbPreset->setGeometry(left, top, lbWidth, lbHeight);
    left = m_lbPreset->geometry().right() + space;
    m_cmbPreset->setGeometry(left, top, 300, lbHeight);
    left = 1;
    top = m_cmbPreset->geometry().bottom() + heightSpace;


    m_lbWidth->setGeometry(left, top, lbWidth, lbHeight);
    left = m_lbWidth->geometry().right() + space;
    m_leWidth->setGeometry(left, top, lineeditWidth, lbHeight);
    left = m_leWidth->geometry().right() + space;
    m_lbWidthSuffix->setGeometry(left, top, lineeditWidth, lbHeight);
    left = 1;
    top = m_lbWidthSuffix->geometry().bottom() + heightSpace;


    m_lbHeight->setGeometry(left, top, lbWidth, lbHeight);
    left = m_lbHeight->geometry().right() + space;
    m_leHeight->setGeometry(left, top, lineeditWidth, lbHeight);
    left = m_leHeight->geometry().right() + space;
    m_lbHeightSuffix->setGeometry(left, top, lineeditWidth, lbHeight);

    left = m_lbHeightSuffix->geometry().right();
    top = (m_lbHeightSuffix->geometry().bottom() - m_lbWidthSuffix->geometry().top()) / 2 + m_lbWidthSuffix->geometry().top();


    m_chkLockAspect->setGeometry(left, top, rc.width() - left, lbHeight);

    left = 1;
    top = m_lbHeightSuffix->geometry().bottom() + heightSpace;
    m_lbPlaybackResoultion->setGeometry(left, top, lbWidth, lbHeight);
    left = m_lbPlaybackResoultion->geometry().right() + space;
    m_cmbPlaybackResoultion->setGeometry(left, top, lineeditWidth, lbHeight);

    left = 1;
    top = m_cmbPlaybackResoultion->geometry().bottom() + heightSpace;
    m_lbBackgroundColor->setGeometry(left, top, lbWidth, lbHeight);
    left = m_lbBackgroundColor->geometry().right() + space;
    m_cmbColor->setGeometry(left, top, lineeditWidth, lbHeight);


    const int btnWidth = 100;
    left = width() - 2 * btnWidth - space - 20;
    top = height() - 45;
    m_btnOK->setGeometry(left, top, btnWidth, lbHeight);
    left = m_btnOK->geometry().right() + space;
    m_btnCancel->setGeometry(left, top, btnWidth, lbHeight);


}

void ProjectSettingDialog::RetranslateUi()
{
    m_lbProjectName->setText(tr("Export Name:"));
    m_lbPreset->setText(tr("Preset:"));
    m_lbWidth->setText(tr("Width:"));
    m_lbHeight->setText(tr("Height:"));
    m_lbPlaybackResoultion->setText(tr("Playback resolution:"));
    m_lbBackgroundColor->setText(tr("Background Color:"));

    SetLockAspectString(QSize(m_leWidth->value(), m_leHeight->value()));
    //m_chkLockAspect->setText(QString(tr("Lock Aspect Ratio")));

    m_btnOK->setText(tr("OK"));
    m_btnCancel->setText(tr("Cancel"));
}



void ProjectSettingDialog::Init()
{
    m_leProjectname->setText(m_mgr->GetExportName());


    m_presetList.push_back(QSize(1024, 768));
    m_presetList.push_back(QSize(1280, 720));
    m_presetList.push_back(QSize(1280, 800));
    m_presetList.push_back(QSize(1360, 768));
    m_presetList.push_back(QSize(1920, 1080));
    m_presetList.push_back(QSize(2560, 1080));
    m_presetList.push_back(QSize(3840, 2160));


    QSize desktopSize = DesktopRectangleMgr::GetInstance()->PrimaryScreen().size();
    QSize resolution = m_mgr->GetResolution();
    if (desktopSize == resolution)
    {
        // 增加屏幕分辨率
        if (!m_presetList.contains(resolution))
        {
            for (int i = 0; i < m_presetList.size(); ++i)
            {
                if (m_presetList[i].width() > resolution.width())
                {
                    m_presetList.insert(i, resolution);
                    break;
                }
            }
        }
    }


    m_cmbPreset->addItem(tr("Custom"));
    for (QSize temp : m_presetList)
    {
        m_cmbPreset->addItem(QString("%1 x %2").arg(temp.width()).arg(temp.height()), temp);
    }


    //QSize resolution = m_mgr->GetResolution();
    m_leWidth->setValue(resolution.width());
    m_leHeight->setValue(resolution.height());
    SetResolutoinIndex(resolution);
    //int presetIndex = m_cmbPreset->findData(resolution);
    //presetIndex = qMax(0, presetIndex);
    //m_cmbPreset->setCurrentIndex(presetIndex);
    m_currentResolutionRatio = 1.0*resolution.width() / resolution.height();

    m_chkLockAspect->setChecked(m_mgr->GetLockAspect());
    SetLockAspectString(resolution);



    auto playbackAddItem = [&](QString text, float value){
        m_playbackList.push_back(std::make_tuple(text, value));

    };
    playbackAddItem(tr("Full"), 1.0f);
    playbackAddItem(("1/2"), 1.0f * 1 / 2);
    playbackAddItem(("1/4"), 1.0f * 1 / 4);
    playbackAddItem(("1/8"), 1.0f * 1 / 8);
    playbackAddItem(("1/16"), 1.0f * 1 / 16);
    for (auto temp : m_playbackList)
    {
        m_cmbPlaybackResoultion->addItem(std::get<0>(temp), std::get<1>(temp));
    }


    int playbackIndex = sComboBoxItemDataIndexFloat(m_cmbPlaybackResoultion, m_mgr->GetPlaybackResoultion());
    if (playbackIndex == -1)
    {
        playbackIndex = 0;
        m_mgr->SetPlaybackResoultion(1.0f);
    }
    m_cmbPlaybackResoultion->setCurrentIndex(playbackIndex);

    m_cmbColor->SetCurrentColor(m_mgr->GetBackgroundColor());

}

void ProjectSettingDialog::ConnectSignals()
{
    connect(m_cmbPreset, SIGNAL(currentIndexChanged(int)), this, SLOT(slotPresetIndexChanged(int)));
    connect(m_cmbPlaybackResoultion, SIGNAL(currentIndexChanged(int)), this, SLOT(slotPlaybackIndexChanged(int)));
    
    connect(m_leWidth, SIGNAL(valueChanged(double)), this, SLOT(slotResolutionWidthValueChanged(double)));


    connect(m_leWidth, &ResolutionLineEdit::editingFinished, this, [&](){
        ResolutionLineEditFinished();

    });
    
    connect(m_leHeight, SIGNAL(valueChanged(double)), this, SLOT(slotResolutionHeightValueChanged(double)));
    connect(m_leHeight, &ResolutionLineEdit::editingFinished, this, [&](){
        ResolutionLineEditFinished();

    });

    connect(m_chkLockAspect, &QCheckBox::toggled, this, [&](bool lock){

    });

    connect(m_cmbColor, &ColorSelecterComboBox::CurrentColorChanged, this, [&](QColor color){

    });

    connect(m_btnOK, &QPushButton::clicked, this, [&](){


        bool bProjectNameChanged = m_mgr->GetExportName() != m_leProjectname->text();
        bool bResolutionChanged = m_mgr->GetResolution() != QSize(m_leWidth->text().toInt(), m_leHeight->text().toInt());
        bool bLockAspectChanged = m_mgr->GetLockAspect() != m_chkLockAspect->isChecked();
        auto ddd = m_mgr->GetPlaybackResoultion();
        auto fff = m_cmbPlaybackResoultion->currentData().toFloat();
        bool bPlaybackChanged = !qFuzzyCompare(m_mgr->GetPlaybackResoultion(), m_cmbPlaybackResoultion->currentData().toFloat());
        bool bBackgroundColorChanged = m_cmbColor->GetCurrentColor() != m_mgr->GetBackgroundColor();

        m_mgr->SetExportName(m_leProjectname->text());
        m_mgr->SetResoultion(QSize(m_leWidth->text().toInt(), m_leHeight->text().toInt()));
        m_mgr->SetLockAspect(m_chkLockAspect->isChecked());
        m_mgr->SetPlaybackResoultion(m_cmbPlaybackResoultion->currentData().toFloat());
        m_mgr->SetBackgroundColor(m_cmbColor->GetCurrentColor());

        if (bPlaybackChanged | bResolutionChanged | bPlaybackChanged | bLockAspectChanged | bBackgroundColorChanged)
        {
            CommunicatorClient::GetInstance()->SetContentChanged(true);
        }

        if (bProjectNameChanged)
            m_mgr->sigExportNameChagned(m_mgr->GetExportName());
        if (bResolutionChanged)
            m_mgr->sigResolutionChanged(m_mgr->GetResolution());
        if (bPlaybackChanged)
            m_mgr->sigPlaybackResoultionChagned(m_mgr->GetPlaybackResoultion());
        if (bBackgroundColorChanged)
        {
            SettingDataMgr::GetInstance()->Save();
            m_mgr->sigBackgroundColorChanged(m_mgr->GetBackgroundColor());

        }

 

        done(1);
    });

    connect(m_btnCancel, &QPushButton::clicked, this, [&](){
        done(0);
    });

}

void ProjectSettingDialog::SetLockAspectString(QSize size)
{
    double ratio = 1.0*size.width() / size.height();
    if (qFuzzyCompare(ratio, 1.0 * 16 / 9))
    {
        m_chkLockAspect->setText(QString(tr("Lock Aspect Ratio %1")).arg("16:9(1.78)"));
    }
    else if (qFuzzyCompare(ratio, 1.0 * 4 / 3))
    {
        m_chkLockAspect->setText(QString(tr("Lock Aspect Ratio %1")).arg("4:3(1.33)"));

    }
    else
    {
        m_chkLockAspect->setText(QString(tr("Lock Aspect Ratio %1(%2)")).arg(GetAspectStringFromSize(size)).arg(1.0*size.width() / size.height(), 0, 'f', 2));
    }


    m_currentResolutionRatio = ratio;

}

void ProjectSettingDialog::SetResolutoinIndex(QSize resolution)
{
    int presetIndex = m_cmbPreset->findData(resolution);
    presetIndex = qMax(0, presetIndex);
    m_cmbPreset->blockSignals(true);
    m_cmbPreset->setCurrentIndex(presetIndex);
    m_cmbPreset->blockSignals(false);

}

void ProjectSettingDialog::ResolutionLineEditFinished()
{
    int width = m_leWidth->value();
    int height = m_leHeight->value();

    if (width == 0 || height == 0)
    {
        QSize size(1920, 1080);
        SetResolutoinIndex(QSize(1920, 1080));
        m_leWidth->blockSignals(true);
        m_leWidth->setValue(size.width());
        m_leWidth->blockSignals(false);
        m_leHeight->blockSignals(true);
        m_leHeight->setValue(size.height());
        m_leHeight->blockSignals(false);

    }
}

QString ProjectSettingDialog::GetAspectStringFromSize(QSize size)
{
    int w = size.width();
    int h = size.height();
    int i = 2;
    while (i < w && i < h)
    {
        while ((w % i == 0) && (h % i == 0))
        {
            w = w / i;
            h = h / i;
        }
        ++i;
    }

    if (w%h == 0)
    {
        w = w / h;
        h = 1;
    }
    else if (h%w == 0)
    {
        h = h / w;
        w = 1;
    }
    return QString("%1:%2").arg(w).arg(h);
}

void ProjectSettingDialog::slotPresetIndexChanged(int index)
{
    if (index != 0)
    {
        auto size = m_cmbPreset->itemData(index).toSize();
        m_leWidth->blockSignals(true);
        m_leWidth->setValue(size.width());
        m_leWidth->blockSignals(false);
        m_leHeight->blockSignals(true);
        m_leHeight->setValue(size.height());
        m_leHeight->blockSignals(false);
        SetLockAspectString(size);

    }
}

void ProjectSettingDialog::slotPlaybackIndexChanged(int index)
{

}

void ProjectSettingDialog::slotResolutionWidthValueChanged(double value)
{
    int width = m_leWidth->value();

    int height = m_leHeight->value();
    if (m_chkLockAspect->isChecked())
    {
        height = qRound(1.0f * width / m_currentResolutionRatio);
        if (height > m_leHeight->maximum() || height < m_leHeight->minimum())
        {
            /*if (height > m_leHeight->maximum())
                height = m_leHeight->maximum();
            else if (height < m_leHeight->minimum())
                height = m_leHeight->minimum();
*/
            height = qBound<int>(m_leHeight->minimum(), height, m_leHeight->maximum());

            width = height * m_currentResolutionRatio;
            m_leWidth->blockSignals(true);
            m_leWidth->setValue(width);
            m_leWidth->blockSignals(false);
        }

        m_leHeight->setValue(height);
    }

    SetResolutoinIndex(QSize(width, height));
    if (!m_chkLockAspect->isChecked())
        SetLockAspectString(QSize(width, height));
}

void ProjectSettingDialog::slotResolutionHeightValueChanged(double value)
{
    int height = m_leHeight->value();


    int width = m_leWidth->value();
    if (m_chkLockAspect->isChecked())
    {
        width = qRound(1.0*height * m_currentResolutionRatio);
        if (width > m_leWidth->maximum() || width < m_leWidth->minimum())
        {
            //if (width > m_leWidth->maximum())
            //    width = m_leWidth->maximum();
            //else
            //    width = m_leWidth->minimum();

            width = qBound<int>(m_leWidth->minimum(), width, m_leWidth->maximum());


            height = 1.0f*width / m_currentResolutionRatio;
            m_leHeight->blockSignals(true);
            m_leHeight->setValue(height);
            m_leHeight->blockSignals(false);
        }

        m_leWidth->setValue(width);
    }
    SetResolutoinIndex(QSize(width, height));
    if (!m_chkLockAspect->isChecked())
        SetLockAspectString(QSize(width, height));
}

//////////////////////////////////////////////////////////////////////////
ResolutionLineEdit::ResolutionLineEdit(QString objectName, QWidget *parent)
:FSDoubleSpinBox(parent)
{
    setObjectName(objectName);
    setButtonSymbols(QAbstractSpinBox::NoButtons);

    setRange(1, 4096);
    SetLoop(false);
    setDecimals(0);
}


