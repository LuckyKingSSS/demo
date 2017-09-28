#ifndef FFOLDWIDGET_H
#define FFOLDWIDGET_H

#include "..\include\inc_FCommonLib\FBaseWidget.h"

class FFoldWidgetItem;
class FFoldWidgetPrivate;

class FFoldWidget : public FBaseWidget
{
	Q_OBJECT

public:
	FFoldWidget(QWidget *parent);
	~FFoldWidget();	
	void AddItem(FFoldWidgetItem* item);
	void AddItems(const QList<FFoldWidgetItem*> items);
	void InsertItem(int iRow, FFoldWidgetItem* item);
	void InsertItems(int iRow, const QList<FFoldWidgetItem*> items);
	void SetShowMode(bool bShowMode);
	int Count();
	FFoldWidgetItem* TakeItem(int iRow);
	FFoldWidgetItem* Item(int iRow)const;

Q_SIGNALS:
	void sigUpdateSize(int iNewHeight);

protected:
	void resizeEvent(QResizeEvent *event)override;

private:
	void Init();
	void InitConnections();
private:
	FFoldWidgetPrivate* m_FoldWidgetPrivate;
};

#endif // FFOLDWIDGET_H
