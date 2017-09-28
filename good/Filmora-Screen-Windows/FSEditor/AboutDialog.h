#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include "inc_FSCommonlib/FSCommonWidget.h"

class AboutDialog : public FSBaseDialog
{
	Q_OBJECT

public:
	AboutDialog(QWidget *parent);

protected:
	void RetranslateUi() override;
	void resizeEvent(QResizeEvent *event) override;
private:
	QLabel *m_pLbFSLogo;
	QLabel *m_pLbWSLogo;
	QLabel *m_pLbProductName;
	QLabel *m_pLbVersion;
	QLabel *m_pLbText1;
	QLabel *m_pLbText2;
	QLabel *m_pLbWebSite;
	QLabel *m_pLbCopyRight;
	
	//QWidget *m_pLine;
};

#endif // ABOUTDIALOG_H
