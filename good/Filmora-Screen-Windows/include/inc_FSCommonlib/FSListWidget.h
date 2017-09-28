#pragma once

#include "inc_FSCommonlib/fscommonlib_global.h"

#include <QtWidgets/QWidget>
#include <QtGui/QColor>
#include <QtCore/QList>

class FSListWidgetPrivate;
class FSCOMMONLIB_EXPORT ListWidget : public QWidget
{
	Q_OBJECT

public:
	ListWidget(QWidget *parent);
	

	void AddItem(QWidget *item);
    void InsertItem(int index, QWidget *item);
    void RemoveItem(int index);
	void RemoveItem(QWidget *item);
	void ClearItems();
	void ClearSelection(); 
    int Size();
	void SetCurrentItem(QWidget *item);

	QWidget* ChildAt(QPoint point);
	QList<QWidget*> GetSelectedItems();
	QList<QWidget*> GetAllItems();
	void ScrollToItem(QWidget *item);

	void SetSelectedBorderColor(QColor color);
	void SetSelectedBorderColor(int index, QColor color);
	void SetSelectedBorderWidth(int width);

	void SetItemSize(QSize size);
	QSize GetItemSize();

	void setObjectName(QString name);
Q_SIGNALS:
	void signalItemClicked(QWidget *item);
	void signalItemSelectedChanged();
	void signalItemDoubleClicked(QWidget *item);

private:
	FSListWidgetPrivate *m_p;
};
