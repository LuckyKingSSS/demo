#ifndef FSEXPORTLOCALPANEL_H
#define FSEXPORTLOCALPANEL_H

#include <QtWidgets/QWidget>
#include "FSExportDataDef.h"

class FSFormatDataWidget;
class FSOptionWidget;

class FSExportLocalPanel : public QWidget
{
    Q_OBJECT


public:
    FSExportLocalPanel(QWidget *parent);
    ~FSExportLocalPanel();
    void CheckUserInput();
    ExportSettingInfo GetExportSettingInfo();
    void SetRecorderExportState(bool bRecorderExport);


    void SetDefaultExportRsolution(QSize resolution);
    void SetExportFileName(QString name);
	void initConnections();

Q_SIGNALS:
	void signalPageChange(int iRow);
private:
    void CreateUI();

private:
    FSFormatDataWidget *mpFormatDataWidget;
    FSOptionWidget     *mpOptionWidget;
};

#endif // FSEXPORTLOCALPANEL_H
