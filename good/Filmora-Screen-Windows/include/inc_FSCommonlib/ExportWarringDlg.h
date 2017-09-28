#ifndef EXPORTWARRINGDLG_H
#define EXPORTWARRINGDLG_H

#include "inc_FSCommonlib/fscommonlib_global.h"
#include "inc_FSCommonlib/FSCommonWidget.h"


class RegisterMain;
class FSCOMMONLIB_EXPORT ExportWarringDlg : public FSBaseDialog
{
	Q_OBJECT
public:
	ExportWarringDlg(QWidget *parent);
	~ExportWarringDlg();


    bool IsRegister();
protected:
	void RetranslateUi() override;
	void resizeEvent(QResizeEvent *e) override;
private:
	QLabel *m_pLbLogo;
	QLabel *m_pLbContent;
	QLabel *m_pLbTrialVersion;
	QLabel *m_pLbRegisterVersion;
	QLabel *m_pLbTrialImg;
	QLabel *m_pLbRegisterImg;

	QPushButton *m_pBtnRegister;
	QPushButton *m_pBtnBuyNow;
	QPushButton *m_pBtnEvaluate;

    RegisterMain   *m_pRegisterChecker;

    QString mOderPageUrl;
	
};

#endif // EXPORTWARRINGDLG_H
