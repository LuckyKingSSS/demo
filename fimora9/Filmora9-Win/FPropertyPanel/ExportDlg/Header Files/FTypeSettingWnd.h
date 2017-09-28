#ifndef FTYPESETTINGWND_H
#define FTYPESETTINGWND_H

#include "..\include\inc_FCommonLib\FBaseWidget.h"
#include "FListCtrl.h"

class FSettingDataContr;
class FTypeSettingWnd : public FBaseWidget
{
	Q_OBJECT

public:
	FTypeSettingWnd(QWidget *parent);
	~FTypeSettingWnd();
	void initUI();
	void paintEvent(QPaintEvent *event)override;
	void initConnections();

Q_SIGNALS:
	void sigItemSelect(QString strType);

protected:
	void resizeEvent(QResizeEvent *event)override;

private:
	FListCtrl*  m_ListCtrl;
	FSettingDataContr* m_pDataControl;//数据中心
};

#endif // FTYPESETTINGWND_H
