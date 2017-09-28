#ifndef FPOWERTOOLDLG_H
#define FPOWERTOOLDLG_H

#include "FPropertyDialog.h"
#include "..\..\include\inc_FPropertyPanel\fpropertypanel_global.h"

class FPowerToolPlayer;
class FPowerToolSettingWidget;
class FPowerToolMgr;
class FPROPERTYPANEL_EXPORT FPowerToolDlg : public FPropertyDialog
{
	Q_OBJECT

public:
	FPowerToolDlg(QWidget *parent);
	~FPowerToolDlg();

private:
	void InitUI();
	void InitConnections();

private:
	FPowerToolPlayer* m_Player;
	FPowerToolSettingWidget* m_PowertoolWidget;
	FPowerToolMgr* m_PowerToolMgr;
};

#endif // FPOWERTOOLDLG_H
