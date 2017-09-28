#ifndef FSLISTWIDGET_H
#define FSLISTWIDGET_H


#include "inc_FSCommonlib/FSListWidget.h"


class PreviewBaseItem;
typedef QList<PreviewBaseItem*> PreviewBaseItems;
class ListWidgetDragRect;

class  FSListWidgetPrivate : public QWidget
{
	Q_OBJECT

public:
	FSListWidgetPrivate(QWidget *parent);


	void AddItem(QWidget *item);
    void InsertItem(int index, QWidget *item);
	void RemoveItem(int index);
	void RemoveItem(QWidget *item);
	void ClearItems();
	QWidget* ChildAt(QPoint point);
	void ScrollToItem(QWidget *item);

	QList<QWidget*> GetSelecteds();
	QList<QWidget*> GetAllItems();
    int Size();
	//QWidget* GetCurrentSelected();

	void SetItemSize(QSize size);
	QSize GetItemSize();

	void SetSelectedBorderColor(QColor color);
	void SetSelectedBorderColor(int index, QColor color);
	void SetSelectedBorderWidth(int width);

	void SetCurrentItem(QWidget *item);
	void _ClearSelection();

Q_SIGNALS:
	void signalItemClicked(QWidget *item);
	void signalItemSelectedChanged();
	void signalItemDoubleClicked(QWidget *item);

private:
	void SetSelected(PreviewBaseItem *item, bool selected, bool multiSelected);
	PreviewBaseItems _GetSelecteds();
	PreviewBaseItem* GetCurrentItem();
	void _SetCurrentItem(PreviewBaseItem *item);

protected:
	void resizeEvent(QResizeEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseDoubleClickEvent(QMouseEvent *event);

	void wheelEvent(QWheelEvent *event);
	void showEvent(QShowEvent *event);

	bool eventFilter(QObject *watcher, QEvent *event);
private:
	PreviewBaseItem* ChildAtInternal(QPoint point);
	void ResizeItems();

private:
	PreviewBaseItems m_items;
	QSize m_itemSize;
	PreviewBaseItem* m_curItem;

	QScrollBar *m_scrollBarV;
	int m_scrolBarVTop;
	int m_scrollBarVWidth;

	QColor m_selectedColor;
	int m_selectedWidth;
private:
	ListWidgetDragRect *m_listWidgetDragRect;
	bool m_mousePress;
	QPoint m_pressPoint;
	bool m_canDragRect;

};



class PreviewBaseItem : public QWidget
{
	Q_OBJECT
public:
	PreviewBaseItem(QWidget *item, QWidget *parent);

	void SetSelected(bool selected);
	bool GetSelected();

	QWidget* GetItem();

	void SetSelectedBorderColor(QColor color);
	void SetSelectedBorderWidth(int width);

	void SetHover(bool hover);
	bool GetHover();
	void SetHoverColor(QColor color);

protected:
	void paintEvent(QPaintEvent *event);
	void resizeEvent(QResizeEvent *event);

protected:
	QWidget *m_item;
	bool m_selected;
	QColor m_selectedColor;
	int m_selectedWidth;
	bool m_hover;
	QColor m_hoverColor;

};

class ListWidgetDragRect : public QWidget
{
	Q_OBJECT
public:
	ListWidgetDragRect(QWidget *parent);
	void SetRect(QRect rect);
protected:
	void paintEvent(QPaintEvent *event);
private:
	QRect m_rect;
};



#endif // FSLISTWIDGET_H
