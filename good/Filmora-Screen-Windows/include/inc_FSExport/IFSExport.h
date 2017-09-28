#ifndef IFSEXPORT_H
#define IFSEXPORT_H

#include "inc_FSExport/fsexport_global.h"
#include "inc_FSCommonLib/FSCommonWidget.h"

class FSExport;
class FSNLETimeline;

class FSEXPORT_EXPORT IFSExport :public FSBaseDialog
{
	Q_OBJECT
signals:
    void RegisterResaultSig(bool isRegisted);
public:
    IFSExport(QList<FSNLETimeline *>pTimelinelist, QWidget *parent = 0);
    ~IFSExport();

    void SetRecorderExportState(bool bRecorderState);
    void SetExportFileName(QString name);
    void SetDefaultExportRsolution(QSize resolution);

private:
    void resizeEvent(QResizeEvent *event);
    void RetranslateUi() override;
private:
    FSExport *m_pFSExport;
};

FSEXPORT_EXPORT QString gGetExportStyleSheet();


#endif // IFSEXPORT_H
