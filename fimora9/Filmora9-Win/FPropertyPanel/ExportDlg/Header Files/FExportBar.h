#ifndef FEXPORTBAR_H
#define FEXPORTBAR_H

#include <QWidget>
#include "..\include\inc_FCommonLib\FBaseWidget.h"
class QButtonGroup;
class QVBoxLayout;


class FExportBar : public FBaseWidget
{
	Q_OBJECT

public:
	enum FExportBtnType{
		FE_BTN_LOCAL,
		FE_BTN_PHONE,
		FE_BTN_YUOTOBU,
		FE_BTN_FACEBOOK,
		FE_BTN_VIMEO,
		FE_BTN_DVD
	};

	FExportBar(QWidget *parent);
	~FExportBar();
	FExportBtnType GetCurrentBtn();
	void initUI();
signals:
	void ButtonClicked(int iBtnId);

private:
	void CreateBtn(int btnId, const QString &ObjName);
private:
	QButtonGroup *m_pBtnGroup;
	QVBoxLayout  *m_pVLayout;
};

#endif // FEXPORTBAR_H
