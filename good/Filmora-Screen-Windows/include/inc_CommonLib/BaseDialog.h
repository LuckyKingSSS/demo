#ifndef BASEDIALOG_H
#define BASEDIALOG_H

#include <QtWidgets/QDialog>
#include "inc_CommonLib/commonlib_global.h"

class TopLevelWindowTitleBar;
class COMMONLIB_EXPORT BaseDialog : public QDialog
{
	Q_OBJECT

public:
	BaseDialog(const QString &title,QWidget *parent);
	~BaseDialog();
	void SetTitle(QString title);

	virtual int DoModal();
protected:
	void resizeEvent(QResizeEvent *event);

protected:
	TopLevelWindowTitleBar *m_titlebar;
	int m_titleHeight;
};

#endif // BASEDIALOG_H
	
