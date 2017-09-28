#ifndef FPIPEDITDIALOG_H
#define FPIPEDITDIALOG_H

#include "FPropertyDialog.h"
#include "inc_FPropertyPanel/fpropertypanel_global.h"
#include "FPIPEditBaseWidget.h"
#include "FPIPEditDialogMgr.h"

class FPROPERTYPANEL_EXPORT FPIPEditDialog : public FPropertyDialog
{
	Q_OBJECT

public:
	FPIPEditDialog(QWidget *parent = 0);
	~FPIPEditDialog();

signals:
	void sigOk();
	void sigCancel();

protected:
	void resizeEvent(QResizeEvent *event) override;
	void showEvent(QShowEvent *) override;

private:
	void ConnectSignals();

private:
	FPIPEditDialogMgr *m_pMgr;
	FPIPEditSettingPanel *m_pSettingPanel;
	FPIPEditPlayer *m_pPlayer;
};

#endif // FPIPEDITDIALOG_H
