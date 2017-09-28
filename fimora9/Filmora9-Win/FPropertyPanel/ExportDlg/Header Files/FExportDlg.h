#ifndef FEXPORTDLG_H
#define FEXPORTDLG_H

#include "inc_FPropertyPanel/fpropertypanel_global.h"
#include "FPropertyDialog.h"
#include "FExportDlgPrivate.h"


class FPROPERTYPANEL_EXPORT FExportDlg : public FPropertyDialog
{
	Q_OBJECT

public:
	FExportDlg(QWidget *parent);
	~FExportDlg();
	void initUI();

protected:
	void resizeEvent(QResizeEvent *e)override;
	int getWndTitleHeight();
	void paintEvent(QPaintEvent *event)override;

private:
	FExportDlgPrivate* m_pExportDlgPrivate;
};

#endif // FEXPORTDLG_H
