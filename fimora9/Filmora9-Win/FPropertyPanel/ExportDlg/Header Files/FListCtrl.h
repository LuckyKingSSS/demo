#ifndef FLISTCTRL_H
#define FLISTCTRL_H

#include "..\include\inc_FCommonLib\FBaseWidget.h"
#include "..\include\inc_FCommonLib\FCommonWidgets.h"
class FLabel;
class FListCtrl : public FBaseWidget
{
	Q_OBJECT

public:
	FListCtrl(QWidget *parent);
	~FListCtrl();
	void setHeader(QString strText);
	void setListData(const QList<QString>& listData);
	void initUI();

Q_SIGNALS:
	void sigItemSelect(QString strType);

protected:
	void showEvent(QShowEvent *e)override;
	void paintEvent(QPaintEvent *event)override;
	void resizeEvent(QResizeEvent *event)override;
private:
	FLabel* m_pTitle;
	QList<QString> m_strList;
};

#endif // FLISTCTRL_H
