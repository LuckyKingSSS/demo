#include "stdafx.h"
#include "FSListWidgetPrivate.h"


ListWidget::ListWidget(QWidget *parent)
:QWidget(parent)
{
	m_p = new FSListWidgetPrivate(this);
	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(m_p);
	layout->setContentsMargins(0, 0, 0, 0);
	setLayout(layout);

	connect(m_p, &FSListWidgetPrivate::signalItemClicked, this, &ListWidget::signalItemClicked);
	connect(m_p, &FSListWidgetPrivate::signalItemDoubleClicked, this, &ListWidget::signalItemDoubleClicked);
	connect(m_p, &FSListWidgetPrivate::signalItemSelectedChanged, this, &ListWidget::signalItemSelectedChanged);
}


void ListWidget::AddItem(QWidget *item)
{
	m_p->AddItem(item);
}

void ListWidget::InsertItem(int index, QWidget *item)
{
    m_p->InsertItem(index, item);
}

void ListWidget::RemoveItem(int index)
{
	m_p->RemoveItem(index);
}

void ListWidget::RemoveItem(QWidget *item)
{
	m_p->RemoveItem(item);
}

void ListWidget::ClearItems()
{
	m_p->ClearItems();
}

void ListWidget::ClearSelection()
{
	m_p->_ClearSelection();

}

int ListWidget::Size()
{
    return m_p->Size();
}

void ListWidget::SetCurrentItem(QWidget *item)
{
	m_p->SetCurrentItem(item);

}

QWidget* ListWidget::ChildAt(QPoint point)
{
	return m_p->ChildAt(point);

}

QList<QWidget*> ListWidget::GetSelectedItems()
{
	return m_p->GetSelecteds();
}


QList<QWidget*> ListWidget::GetAllItems()
{
	return m_p->GetAllItems();
}

void ListWidget::ScrollToItem(QWidget *item)
{
	m_p->ScrollToItem(item);

}

void ListWidget::SetSelectedBorderColor(QColor color)
{
	m_p->SetSelectedBorderColor(color);
}

void ListWidget::SetSelectedBorderColor(int index, QColor color)
{
	m_p->SetSelectedBorderColor(index, color);

}

void ListWidget::SetSelectedBorderWidth(int width)
{
	m_p->SetSelectedBorderWidth(width);
}


void ListWidget::SetItemSize(QSize size)
{
	m_p->SetItemSize(size);

}

QSize ListWidget::GetItemSize()
{
	return m_p->GetItemSize();

}

void ListWidget::setObjectName(QString name)
{
	m_p->setObjectName(name);
}

//////////////////////////////////////////////////////////////////////////
FSListWidgetPrivate::FSListWidgetPrivate(QWidget *parent)
:QWidget(parent)
, m_mousePress(false)
, m_canDragRect(false)
, m_scrolBarVTop(0)
, m_scrollBarVWidth(6)
, m_curItem(nullptr)
{
	setAttribute(Qt::WA_StyledBackground, true);

	setMouseTracking(true);
	m_selectedColor = QColor(250, 0, 0);
	m_selectedWidth = 1;

	m_itemSize = QSize(130, 97);
	m_listWidgetDragRect = new ListWidgetDragRect(this);

	m_scrollBarV = new QScrollBar(this);
	connect(m_scrollBarV, &QScrollBar::valueChanged, this, [&](int value){
		m_scrolBarVTop = -value;
		resizeEvent(nullptr);
	});


	qApp->installEventFilter(this);
}



void FSListWidgetPrivate::AddItem(QWidget *widget)
{
    InsertItem(Size(), widget);
}

void FSListWidgetPrivate::InsertItem(int index, QWidget *widget)
{
    Q_ASSERT(index <= Size());
    auto item = new PreviewBaseItem(widget, this);
    item->show();
    item->SetSelectedBorderColor(m_selectedColor);
    item->SetSelectedBorderWidth(m_selectedWidth);
    //m_items.push_back(item);
    m_items.insert(index, item);
    ResizeItems();
}

void FSListWidgetPrivate::RemoveItem(int index)
{
	Q_ASSERT(index >= 0 && index < m_items.size());
	auto item = m_items[index];
	m_items.removeAt(index);
	delete item;

	ResizeItems();

}


int FSListWidgetPrivate::Size()
{
    return m_items.size();
}

void FSListWidgetPrivate::RemoveItem(QWidget *item)
{
	for (auto temp : m_items)
	{
		if (item == temp->GetItem())
		{
			RemoveItem(m_items.indexOf(temp));
			break;
		}
	}
}

void FSListWidgetPrivate::ClearItems()
{
	qDeleteAll(m_items);
	m_items.clear();
}

QWidget* FSListWidgetPrivate::ChildAt(QPoint point)
{
	auto item = ChildAtInternal(point);
	return item ? item->GetItem() : nullptr;
}

QList<QWidget*> FSListWidgetPrivate::GetSelecteds()
{
	QList<QWidget*> items;
	for (auto item : _GetSelecteds())
	{
		items.push_back(item->GetItem());
	}
	return items;
}

QList<QWidget*> FSListWidgetPrivate::GetAllItems()
{
	QList<QWidget*> items;
	for (auto temp : m_items)
	{
		items.push_back(temp->GetItem());
	}
	return items;
}

void FSListWidgetPrivate::ScrollToItem(QWidget *item)
{
	for (auto temp : m_items)
	{
		if (temp->GetItem() == item)
		{
			int pos = temp->geometry().bottom();
			m_scrollBarV->setValue(qMax(0, pos - height() + 10));
			break;
		}
	}
}

void FSListWidgetPrivate::SetItemSize(QSize size)
{
	m_itemSize = size;
}

QSize FSListWidgetPrivate::GetItemSize()
{
	return m_itemSize;
}

void FSListWidgetPrivate::SetSelectedBorderColor(QColor color)
{
	m_selectedColor = color;
}

void FSListWidgetPrivate::SetSelectedBorderColor(int index, QColor color)
{
	Q_ASSERT(index >= 0 && index < m_items.size());

	m_items[index]->SetSelectedBorderColor(color);
	
}

void FSListWidgetPrivate::SetSelectedBorderWidth(int width)
{
	m_selectedWidth = width;
}
void FSListWidgetPrivate::SetCurrentItem(QWidget *item)
{
	for (auto temp : m_items)
	{
		if (item == temp->GetItem())
		{
			SetSelected(temp, true, false);
			break;
		}
	}
}

void FSListWidgetPrivate::resizeEvent(QResizeEvent *event)
{
	const int margins = 15;
	const int spacing = 28;

	int left = margins;
	int top = margins + m_scrolBarVTop;

	bool newLineHaveItem = false;
	int scrollbarMaxValue = margins;
	for (auto item : m_items)
	{
		item->setGeometry(left, top, m_itemSize.width(), m_itemSize.height());
		left = item->geometry().right() + spacing;
		if (left + m_itemSize.width() > width() - margins - m_scrollBarVWidth)
		{
			left = margins;
			top = item->geometry().bottom() + margins;
			scrollbarMaxValue += (m_itemSize.height() + margins);
			newLineHaveItem = false;
		}
		else
		{
			newLineHaveItem = true;
		}
	}
	left = width() - m_scrollBarVWidth;
	m_scrollBarV->setGeometry(left, 0, m_scrollBarVWidth, height() - 2 * 0);


	m_listWidgetDragRect->raise();
	m_listWidgetDragRect->setGeometry(rect());

	if (newLineHaveItem)
		scrollbarMaxValue = scrollbarMaxValue + m_itemSize.height();

	int scrollRange = qMax(0, scrollbarMaxValue - height());
	m_scrollBarV->setRange(0, scrollRange);
	//m_scrollBarV->setVisible(scrollRange);
}
void FSListWidgetPrivate::showEvent(QShowEvent *event)
{
	ResizeItems();
}

bool FSListWidgetPrivate::eventFilter(QObject *watcher, QEvent *event)
{
	if (event->type() == QEvent::MouseMove)
	{
		QMouseEvent *pEvent = static_cast<QMouseEvent*>(event);
		if (rect().contains(mapFromGlobal(pEvent->globalPos())))
		{
			auto item = ChildAtInternal(mapFromGlobal(pEvent->globalPos()));
			for (auto temp : m_items)
			{
				temp->SetHover(temp == item);
			}
		}
	}
	return false;
}
void FSListWidgetPrivate::mousePressEvent(QMouseEvent *event)
{
	//if (event->button() == Qt::LeftButton)
	{
		m_mousePress = true;
		auto item = ChildAtInternal(event->pos());
		auto curSelecteds = _GetSelecteds();
		if (item)
		{
			bool controlModifier = qApp->keyboardModifiers() & Qt::ControlModifier;
			bool shiftModifier = qApp->keyboardModifiers() & Qt::ShiftModifier;

			if (controlModifier)
			{
				bool controlSelected = !item->GetSelected();
				// 第一个选
				if (curSelecteds.isEmpty())
				{
					if (controlSelected)
						_SetCurrentItem(item);
				}
				else
				{
					if (item == GetCurrentItem())
						_SetCurrentItem(controlSelected ? item : nullptr);
				}
				SetSelected(item, controlSelected, true);

			}
			else if (shiftModifier)
			{
				int startIndex = 0;
				int endIndex = 0;
				if (!curSelecteds.isEmpty())
				{
					auto curSelectedItem = GetCurrentItem();
					_ClearSelection();
					startIndex = m_items.indexOf(curSelectedItem);
					endIndex = m_items.indexOf(item);
					if (startIndex > endIndex)
						std::swap(startIndex, endIndex);
					for (int i = startIndex; i <= endIndex; ++i)
					{
						SetSelected(m_items[i], true, true);
					}
				}
				else
				{
					_SetCurrentItem(item);
					SetSelected(item, true, true);
				}
			}
			else
			{
				_SetCurrentItem(item);
				SetSelected(item, true, false);
			}

		}
		else
		{
			bool selectedChagned = !_GetSelecteds().isEmpty();
			_ClearSelection();
			_SetCurrentItem(nullptr);
			m_canDragRect = true;
			if (selectedChagned)
				emit signalItemSelectedChanged();
		}
		auto curItem = GetCurrentItem();
		if (curItem)
			emit signalItemClicked(curItem->GetItem());
		else
			emit signalItemClicked(nullptr);
	}
	m_pressPoint = event->pos();

	QWidget::mousePressEvent(event);
}

void FSListWidgetPrivate::mouseMoveEvent(QMouseEvent *event)
{
	if (m_canDragRect)
	{
		QRect dragRect(m_pressPoint, event->pos());
		m_listWidgetDragRect->SetRect(dragRect);
		auto preCurItems = _GetSelecteds();
		auto preCurrentItem = GetCurrentItem();
		_ClearSelection();
		for (auto item : m_items)
		{
			if (dragRect.intersects(item->geometry()))
			{
				item->SetSelected(true);
			}
		}
		auto curCurItems = _GetSelecteds();
		if (!curCurItems.isEmpty() && preCurrentItem == nullptr)
		{
			//  
			m_curItem = curCurItems[0];
		}
		if (preCurItems != _GetSelecteds())
			emit signalItemSelectedChanged();


	}

	QWidget::mouseMoveEvent(event);

}

void FSListWidgetPrivate::mouseReleaseEvent(QMouseEvent *event)
{

	PreviewBaseItem *curItem = nullptr;
	if (curItem)
	{
		//auto item = qobject_cast<PreviewItem*>(itemWidget(curItem));
		//if (item)
		//	emit singalItemClicked(item->GetData());
		//else
		//	emit singalItemClicked(nullptr);

	}
	m_listWidgetDragRect->SetRect(QRect());
	m_mousePress = false;
	m_canDragRect = false;

	QWidget::mouseReleaseEvent(event);

}

void FSListWidgetPrivate::mouseDoubleClickEvent(QMouseEvent *event)
{
	emit signalItemDoubleClicked(GetCurrentItem());
	QWidget::mouseDoubleClickEvent(event);

}

void FSListWidgetPrivate::wheelEvent(QWheelEvent *event)
{
	int scrollValue = m_scrollBarV->value();

	scrollValue = qBound(0, event->angleDelta().y() < 0 ? scrollValue + 20 : scrollValue - 20, m_scrollBarV->maximum());
	m_scrollBarV->setValue(scrollValue);
	QWidget::wheelEvent(event);
}

PreviewBaseItem* FSListWidgetPrivate::ChildAtInternal(QPoint point)
{
	QWidget *item = childAt(point);

	if ((qobject_cast<PreviewBaseItem*>(item)) == nullptr)
	{
		while (item && item->parentWidget())
		{
			auto temp = qobject_cast<PreviewBaseItem*>(item->parentWidget());
			if (temp)
			{
				item = temp;
				break;
			}
			item = item->parentWidget();
		}
	}

	return qobject_cast<PreviewBaseItem*>(item);
}

void FSListWidgetPrivate::ResizeItems()
{
	resizeEvent(nullptr);
}

void FSListWidgetPrivate::_ClearSelection()
{
	for (auto temp : m_items)
	{
		temp->SetSelected(false);
	}
	//m_curItem = nullptr;
}

void FSListWidgetPrivate::SetSelected(PreviewBaseItem *item, bool selected, bool multiSelected)
{
	auto preCurItems = _GetSelecteds();
	if (!multiSelected)
	{
		_ClearSelection();
	}
	bool preSelected = item->GetSelected();
	item->SetSelected(selected);
	
	if (preCurItems.contains(item))
	{
		// 已被选中状态不一样
		auto preCurItem = preCurItems[preCurItems.indexOf(item)];
		if (preSelected != selected)
			emit signalItemSelectedChanged();
	}
	else
	{
		// 未被选中现在被选中
		if (selected)
			emit signalItemSelectedChanged();
	}

}

PreviewBaseItems FSListWidgetPrivate::_GetSelecteds()
{
	PreviewBaseItems items;
	for (auto temp : m_items)
	{
		if (temp->GetSelected())
			items.push_back(temp);
	}
	return items;
}

PreviewBaseItem* FSListWidgetPrivate::GetCurrentItem()
{
	if (m_curItem == nullptr)
	{
		auto selectedItems = _GetSelecteds();
		if (!selectedItems.isEmpty())
			m_curItem = selectedItems[0];
	}
	return m_curItem;
}

void FSListWidgetPrivate::_SetCurrentItem(PreviewBaseItem *item)
{
	m_curItem = item;
}

//////////////////////////////////////////////////////////////////////////
PreviewBaseItem::PreviewBaseItem(QWidget *item, QWidget *parent)
:QWidget(parent)
, m_item(item)
{
	m_item->setParent(this);

	m_selectedWidth = 1;
	m_selected = false;
	m_selectedColor = QColor(87, 192, 201);

	m_hover = false;
	m_hoverColor = QColor(87, 192, 201, 0);
}


void PreviewBaseItem::SetSelected(bool selected)
{
	m_selected = selected;
	update();
}

bool PreviewBaseItem::GetSelected()
{
	return m_selected;
}

QWidget* PreviewBaseItem::GetItem()
{
	return m_item;
}

void PreviewBaseItem::SetSelectedBorderColor(QColor color)
{
	m_selectedColor = color;
	update();
}

void PreviewBaseItem::SetSelectedBorderWidth(int width)
{
	m_selectedWidth = width;
}

void PreviewBaseItem::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	if (GetSelected() || GetHover())
	{
		auto pen = painter.pen();
		if (GetSelected())
			pen.setColor(m_selectedColor);
		else
			pen.setColor(m_hoverColor);

		pen.setWidth(2 * m_selectedWidth);
		painter.setPen(pen);
		painter.setBrush(Qt::NoBrush);
		painter.drawRect(rect());
	}
}

void PreviewBaseItem::resizeEvent(QResizeEvent *event)
{
	QRect rc = rect().adjusted(m_selectedWidth, m_selectedWidth, -m_selectedWidth, -m_selectedWidth);
	m_item->setGeometry(rc);
}


void PreviewBaseItem::SetHover(bool hover)
{
	m_hover = hover;
	update();
}

bool PreviewBaseItem::GetHover()
{
	return m_hover;
}

void PreviewBaseItem::SetHoverColor(QColor color)
{
	m_hoverColor = color;

}

//////////////////////////////////////////////////////////////////////////

ListWidgetDragRect::ListWidgetDragRect(QWidget *parent)
:QWidget(parent)
{
	setAttribute(Qt::WA_TransparentForMouseEvents, true);
}

void ListWidgetDragRect::SetRect(QRect rect)
{
	m_rect = rect; update();
}

void ListWidgetDragRect::paintEvent(QPaintEvent *event)
{
	if (!m_rect.isNull())
	{
		QRect backgroudRect = m_rect;
		if (m_rect.width() < 0)
			backgroudRect.setWidth(m_rect.width() + 1);
		if (m_rect.height() < 0)
			backgroudRect.setHeight(m_rect.height() + 1);

		QPainter painter(this);
		auto pen = painter.pen();
		pen.setColor(QColor(42, 127, 213));
		painter.fillRect(backgroudRect, QColor(195, 221, 246, 180));
		painter.setPen(pen);
		painter.setBrush(Qt::NoBrush);
		painter.drawRect(m_rect);
	}
}
