#ifndef FFOLDWIDGETITEM_H
#define FFOLDWIDGETITEM_H
#include "..\include\inc_FCommonLib\FBaseWidget.h"
#include "FFoldWidgetItemPrivate.h"

class FContexBaseWidget;
class FCollapseHeaderBase;

class FFoldWidgetItem : public FBaseWidget
{
	Q_OBJECT

public:
	FFoldWidgetItem(QWidget *parent);
	~FFoldWidgetItem();
	void SetContainsWidget(FContexBaseWidget* pWidget);
	FContexBaseWidget* GetItemCentralWidget();
	void SetCustomHeader(FCollapseHeaderBase* pHeaderWidegt);
	FCollapseHeaderBase* GetItemHeader();
	void HideContainWidget();
	void ShowContainWidget();
	void SetShowMode(ShowMode showMode);
	void SetCheckBoxName(QString strName);
	void SetIndex(int iIndex);
	int  GetIndex();
	void SetHeaderTitleName(QString strTitleName);
	void SetHeaderHeight(int iHeight);

protected:
	void resizeEvent(QResizeEvent *event)override;

Q_SIGNALS:
	void sigOnCollapse(bool bState);

private:
	void InitConnections();

private:
	FFoldWidgetItemPrivate* m_pFoldItemPrivate;
};

#endif // FFOLDWIDGETITEM_H
