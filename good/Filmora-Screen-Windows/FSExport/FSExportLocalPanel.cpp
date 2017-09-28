#include "stdafx.h"
#include "FSExportLocalPanel.h"
#include "FSOptionWidget.h"
#include "FSFormatDataWidget.h"

FSExportLocalPanel::FSExportLocalPanel(QWidget *parent)
    : QWidget(parent)
{
    CreateUI();
	initConnections();
}

FSExportLocalPanel::~FSExportLocalPanel()
{

}

void FSExportLocalPanel::CheckUserInput()
{
    mpOptionWidget->CheckUserInput();
}

ExportSettingInfo FSExportLocalPanel::GetExportSettingInfo()
{
    ExportSettingInfo info;
    mpFormatDataWidget->GetExportSettingInfo(info);
    info.filename = mpOptionWidget->GetFilePathName()+info.filename;
    return info;
}

void FSExportLocalPanel::SetRecorderExportState(bool bRecorderExport)
{
    mpOptionWidget->SetRecorderExportState(bRecorderExport);
}

void FSExportLocalPanel::SetDefaultExportRsolution(QSize resolution)
{
    mpFormatDataWidget->SetDefaultExportRsolution(resolution);
}

void FSExportLocalPanel::SetExportFileName(QString name)
{
    mpOptionWidget->SetExportFileName(name);
}

void FSExportLocalPanel::initConnections()
{
	connect(mpFormatDataWidget, &FSFormatDataWidget::CurrentRowChanged, this, &FSExportLocalPanel::signalPageChange);
}

void FSExportLocalPanel::CreateUI()
{
    mpFormatDataWidget = new FSFormatDataWidget(tr("Format"), this);
    mpOptionWidget     = new FSOptionWidget(this,mpFormatDataWidget);
   
    QHBoxLayout *pHL   = new QHBoxLayout(this);
    pHL->addWidget(mpFormatDataWidget);
    pHL->addWidget(mpOptionWidget);
    pHL->setMargin(0);
    pHL->setSpacing(0);

    mpOptionWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mpFormatDataWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
}
