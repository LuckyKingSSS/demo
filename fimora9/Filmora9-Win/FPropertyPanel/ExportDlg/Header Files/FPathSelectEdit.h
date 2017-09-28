#ifndef FPATHSELECTEDIT_H
#define FPATHSELECTEDIT_H

#include "inc_FCommonLib/FCommonWidgets.h"
class FButton;
class QHBoxLayout;
class FPathSelectEdit : public FLineEdit
{
	Q_OBJECT

public:
	FPathSelectEdit(QWidget *parent);
	~FPathSelectEdit();
	void initUI();
	void initConnectios();

protected:
	void paintEvent(QPaintEvent *event)override;
private:
	FButton*  m_FindTargetBtn;
	QHBoxLayout*  m_hLayout;
	QString m_strSelectedPath; //用户选择的路径
};

#endif // FPATHSELECTEDIT_H
