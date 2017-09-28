#ifndef FSEXPORT_H
#define FSEXPORT_H

#include <QtWidgets/QWidget>


class FSExportBar;
class FSButton;
class QStackedWidget;
class FSExportLocalPanel;
class FSExportHelper;
class FSNLETimeline;
class FSExportHelperManager;

class  FSExport : public QWidget 
{
	Q_OBJECT

signals:
    void SigHide();
    void SigDone(int reg);
    void RegisterResaultSig(bool resault);
public:
    FSExport(QList<FSNLETimeline *>pTimelinelist, QWidget *parent = 0);
	~FSExport();
    void SetRecorderExportState(bool bRecorderState);
	void TransfromGaInfo(FSExportHelperManager *pHelp);

    void SetDefaultExportRsolution(QSize resolution);
    void SetExportFileName(QString name);

public Q_SLOTS:
   void slotExportProfileChange(int iIndex);
protected:
    void resizeEvent(QResizeEvent *e);

private:
    void ConnectSignals();
    void OnExportClickedSlot();



private:
    bool            mbRecorderState;
    QString         mbDefaultFileName;
    FSExportBar    *mpExportBar;
    FSButton       *mpExportBtn;
    QStackedWidget *mpStackedWidget;
    FSExportLocalPanel *mpExportLocal;
    QList<FSNLETimeline *> mpNLETimelineList;
	QString m_strFileType; //导出文件的类型
};


#endif // FSEXPORT_H
