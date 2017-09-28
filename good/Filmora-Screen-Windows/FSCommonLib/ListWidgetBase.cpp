#include "stdafx.h"
#include "inc_FSCommonlib/ListWidgetBase.h"
#include "inc_FSCommonlib/ListItemDataBase.h"

MVListWidget::MVListWidget(QWidget *parent)
	: QListWidget(parent)
	, m_bNeedItemFix(false)
{
	SetItemSize(120, 93);
	setLayoutMode(QListView::Batched);
	setViewMode(QListView::IconMode);
	setResizeMode(QListView::Fixed);
	setDragEnabled(false);
	setFocusPolicy(Qt::FocusPolicy::NoFocus);
	SetSingleSelect(false);
	setSelectionRectVisible(false);
	m_pSelectionRect = new SelectionRect(this);
	m_pSelectionRect->setVisible(false);
	SetSpacing(10);
	setSortingEnabled(false);
    setObjectName("MVListWidget");
}

MVListWidget::~MVListWidget()
{

}

void MVListWidget::AddItem(MVListItemData *pItemData, MVItemWidgetBase *pItemWidget)
{
	pItemData->setSizeHint(QSize(m_nItemWidgetW, m_nItemWidgetH));
	pItemWidget->SetItemData(pItemData);
	addItem(pItemData);
	setItemWidget(pItemData, pItemWidget);

	//这里为了解决AddItem后，第一个item会不显示的bug
	//doItemsLayout();
	repaint();
	QTimer::singleShot(0, [&]{
		resize(width(), height() + 5);
		resize(width(), height() - 5);
	});
}

void MVListWidget::InsertItem(int row, MVListItemData *pItemData, MVItemWidgetBase *pItemWidget)
{
	pItemData->setSizeHint(QSize(m_nItemWidgetW, m_nItemWidgetH));
	pItemWidget->SetItemData(pItemData);
	insertItem(row, pItemData);
	setItemWidget(pItemData, pItemWidget);
	//pItemWidget->setVisible(true);
	//doItemsLayout();
}

void MVListWidget::RemoveItem(MVListItemData *pItemData)
{
	blockSignals(true);
	int nRow = row(pItemData);
	removeItemWidget(pItemData);
	model()->removeRow(nRow);
	blockSignals(false);
	repaint();
	QTimer::singleShot(0, [&]{
		resize(width(), height() + 5);
		resize(width(), height() - 5);
	});
}

void MVListWidget::RemoveSelectedItems()
{
	for (auto item : selectedItems())
	{
		MVListItemData *pItemdata = dynamic_cast<MVListItemData *>(item);
		if (pItemdata) RemoveItem(pItemdata);
	}
}

void MVListWidget::UnselectAllItems()
{
	for (auto item : selectedItems())
		setItemSelected(item, false);
}

QList<MVListItemData *> MVListWidget::Items()
{
	QList<MVListItemData *> lstItems;
	int cnt = count();
	for (int i = 0; i < cnt; ++i)
	{
		MVListItemData *pItemdata = dynamic_cast<MVListItemData *>(item(i));
		if (pItemdata) lstItems.push_back(pItemdata);
	}
	return lstItems;
}

void MVListWidget::SetItemSize(int w, int h)
{
	m_nItemWidgetW = w;
	m_nItemWidgetH = h;
}

void MVListWidget::SetItemHidden(MVListItemData *pItemData, bool bHidden)
{
	setItemSelected(pItemData, false);
	pItemData->setHidden(bHidden);
}

void MVListWidget::SetSingleSelect(bool b)
{
	setSelectionMode(b ? QAbstractItemView::SingleSelection : QAbstractItemView::ExtendedSelection);
}

void MVListWidget::SetItemWidgetSize(int w, int h)
{
	m_nItemWidgetW = w;
	m_nItemWidgetH = h;
}

void MVListWidget::SetSpacing(int space)
{
	setSpacing(space);
}

void MVListWidget::ItemConfuseFix()
{
	setViewMode(QListView::ListMode);
	doItemsLayout();
	setViewMode(QListView::IconMode);
	doItemsLayout();
}

void MVListWidget::SetNeedItemFix(bool bNeed)
{
	m_bNeedItemFix = bNeed;
}

void MVListWidget::mousePressEvent(QMouseEvent *e)
{
	QListWidget::mousePressEvent(e);
	if (!itemAt(e->pos()))
	{
		UnselectAllItems();
		if (e->button() == Qt::LeftButton)
			m_ptPress = e->pos();
	}
}

void MVListWidget::mouseMoveEvent(QMouseEvent *e)
{
	QListWidget::mouseMoveEvent(e);
	if (!m_ptPress.isNull() && 
		selectionMode() != QAbstractItemView::SingleSelection)
	{
		QRect rc(m_ptPress, e->pos());
		m_pSelectionRect->SetRect(rc);
		if (!m_pSelectionRect->isVisible()) m_pSelectionRect->setVisible(true);
	}
}

void MVListWidget::mouseReleaseEvent(QMouseEvent *e)
{
	QListWidget::mouseReleaseEvent(e);
	m_ptPress.setX(0);
	m_ptPress.setY(0);
	m_pSelectionRect->setVisible(false);

	if (e->button() == Qt::RightButton)
	{
		auto pt = e->pos();
		if (!rect().contains(pt))
			return;

		QListWidgetItem *pItemData = itemAt(pt);
		if (pItemData)
		{
			MVListItemData *pData = dynamic_cast<MVListItemData *>(pItemData);
			emit sigItemRClicked(mapToGlobal(pt), pData);
		}
		else
		{
			emit sigBlankRClicked(mapToGlobal(pt));
		}
	}

}

void MVListWidget::resizeEvent(QResizeEvent *e)
{
	QListWidget::resizeEvent(e);
	m_pSelectionRect->setGeometry(rect());
	doItemsLayout();
	if (m_bNeedItemFix)
	{
		m_bNeedItemFix = false;
		ItemConfuseFix();
	}
}

//////////////////////////////////////////////////////////////////////////
SelectionRect::SelectionRect(QWidget *parent)
	:FSBaseWidget(parent)
{
	setAttribute(Qt::WA_TranslucentBackground);
	setAttribute(Qt::WA_TransparentForMouseEvents);
}

void SelectionRect::SetRect(QRect rc)
{
	m_rcSelected = rc;
	update();
}

void SelectionRect::paintEvent(QPaintEvent *e)
{
	QPainter p(this);
	QPen pen;
	pen.setStyle(Qt::DotLine);
	pen.setColor(Qt::gray);
	pen.setWidth(2);
	p.setPen(pen);
	p.drawRect(m_rcSelected);
}

//////////////////////////////////////////////////////////////////////////
MVItemWidgetBase::MVItemWidgetBase(QWidget *parent)
	:FSBaseWidget(parent)
{
	m_bSelectable = true;
	m_pItemData = nullptr;
	m_pLbThumbnail = new QLabel(this);
	m_pLbTitle = new TitleWidget(this);
	m_pLbIcon = new QLabel(this);
	m_pLbIcon->setVisible(false);
	m_pBtnAdd = new BaseButton("MatterView_ListWidget_ItemWidget_AddButton", this);
	m_pBtnAdd->setVisible(false);
	m_pSelectedWidget = new SelectedWidget(this);

	QListWidget* pList = (QListWidget*)parent;
	connect(pList, &QListWidget::itemSelectionChanged, this, [&]{
		if (m_pItemData && m_pItemData->isSelected())
		{
			if (m_bSelectable)
			{
				m_pSelectedWidget->SetState(SelectedWidget::State_Selected);
			}
			else
			{
				m_pItemData->setSelected(false);
			}
		}
		else
			m_pSelectedWidget->SetState(SelectedWidget::State_Normal);
	});
	connect(m_pBtnAdd, &BaseButton::clicked, this, [&]{
		emit sigAddBtnClicked(this);
	});

	connect(m_pLbTitle, &TitleWidget::sigEndEdit, this, [&](QString text){
		emit sigEndEdit(this, text);
	});
}

MVItemWidgetBase::~MVItemWidgetBase()
{

}

void MVItemWidgetBase::SetItemData(MVListItemData* pItemData)
{
	if (!pItemData) return;
	m_pItemData = pItemData;
	//m_pLbThumbnail->setPixmap(m_pItemData->GetItemDataBase()->GetThumbnail());
	m_pLbTitle->SetText(m_pItemData->GetItemDataBase()->GetTitle());
	m_pLbIcon->setPixmap(m_pItemData->GetItemDataBase()->GetIcon());
}

MVListItemData* MVItemWidgetBase::GetItemData()
{
	return m_pItemData;
}

void MVItemWidgetBase::Rename()
{
	m_pLbTitle->StartEdit();
}

void MVItemWidgetBase::SetRenamable(bool bEnable)
{
	m_pLbTitle->SetEditable(bEnable);
}

void MVItemWidgetBase::SetSelectable(bool bEnable)
{
	m_bSelectable = bEnable;
}

QString MVItemWidgetBase::GetTitle()
{
    return m_pItemData->GetItemDataBase()->GetTitle();
}

void MVItemWidgetBase::SetTitle(QString title)
{
	m_pLbTitle->SetText(title);
	m_pItemData->GetItemDataBase()->SetTitle(title);
}

void MVItemWidgetBase::Drag()
{
}

QPixmap MVItemWidgetBase::GetAspectPixmap(QPixmap src, int w, int h, bool bBlackBackgound)
{
	QPixmap backPix(w, h);
	if (bBlackBackgound)
		backPix.fill(Qt::black);
	else
		backPix.fill(Qt::transparent);
	QPainter p(&backPix);

	QPixmap scaledPix = src.scaled(w, h, Qt::KeepAspectRatio);
	QRect rcDraw((w - scaledPix.width()) * 0.5, (h - scaledPix.height()) * 0.5, scaledPix.width(), scaledPix.height());
	p.drawPixmap(rcDraw, scaledPix);

	return backPix;
}

void MVItemWidgetBase::resizeEvent(QResizeEvent *e)
{
	static const int titleH = 17;
	static const int addBtnW = 24;
	static const int addBtnH = addBtnW;
	static const int iconW = 20;
	static const int iconH = 20;
	m_pLbThumbnail->setGeometry(0, 0, width(), height() - titleH);
	m_pLbThumbnail->setPixmap(GetAspectPixmap(m_pItemData->GetItemDataBase()->GetThumbnail(), m_pLbThumbnail->geometry().width(), m_pLbThumbnail->geometry().height()));

	int l = (width() - addBtnW) * 0.5;
	int t = (m_pLbThumbnail->geometry().height() - addBtnH) * 0.5;
	m_pBtnAdd->setGeometry(l, t, addBtnW, addBtnH);

	t = 0;
	l = width() - iconW;
	m_pLbIcon->setGeometry(l, t, iconW, iconH);
	//m_pLbIcon->setPixmap(GetAspectPixmap(m_pItemData->GetItemDataBase()->GetIcon(), m_pLbIcon->geometry().width(), m_pLbIcon->geometry().height(), false));

	t = m_pLbThumbnail->geometry().bottom() + 1;
	m_pLbTitle->setGeometry(0, t, width(), titleH);
	m_pSelectedWidget->setGeometry(m_pLbThumbnail->geometry());
}

void MVItemWidgetBase::enterEvent(QEvent *e)
{
	if (!m_pItemData->isSelected())
		m_pSelectedWidget->SetState(SelectedWidget::State_Hover);
	FSBaseWidget::enterEvent(e);
}

void MVItemWidgetBase::leaveEvent(QEvent *e)
{
	if (!m_pItemData->isSelected())
		m_pSelectedWidget->SetState(SelectedWidget::State_Normal);
	FSBaseWidget::enterEvent(e);
}

void MVItemWidgetBase::mousePressEvent(QMouseEvent *e)
{
	if (e->button() == Qt::LeftButton)
	{
		m_ptPress = e->pos();
	}
	FSBaseWidget::mousePressEvent(e);
}

void MVItemWidgetBase::mouseMoveEvent(QMouseEvent *e)
{
	if ((e->pos() - m_ptPress).manhattanLength() > QApplication::startDragDistance())
	{
		emit sigDragBegin(this);
		Drag();
		emit sigDragEnd(this);
	}
}

void MVItemWidgetBase::mouseReleaseEvent(QMouseEvent *e)
{
	FSBaseWidget::mouseReleaseEvent(e);
}

//////////////////////////////////////////////////////////////////////////
TitleWidget::TitleWidget(MVItemWidgetBase *parent)
	:FSBaseWidget(parent)
{
	m_bEditable = true;
	m_bEditing = false;
	m_pParentWidget = parent;
	m_pEdit = new QLineEdit(this);
	m_pEdit->setObjectName("MatterView_MediaPage_ItemWidget_TitleWidget_Editor");
	m_pEdit->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	m_pEdit->setVisible(false);
	m_pLbText = new QLabel(this);
	m_pLbText->setObjectName("MatterView_MediaPage_ItemWidget_TitleWidget_Label");
	m_pLbText->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	qApp->installEventFilter(this);
}

void TitleWidget::SetText(const QString &text)
{
    m_Text = text;
    m_pLbText->setText(GetElideText(text));
}

void TitleWidget::SetEditable(bool b)
{
	m_bEditable = b;
}

void TitleWidget::StartEdit()
{
	if (!m_bEditable) return;
	m_bEditing = true;
	m_pLbText->setVisible(false);
	m_pEdit->setVisible(true);
	m_pEdit->setText(m_Text);
	m_pEdit->selectAll();
	m_pEdit->setFocus();
}

#include "inc_CommonLib/StringOperation.h"

void TitleWidget::EndEdit()
{
	m_bEditing = false;
	m_pEdit->setVisible(false);
	m_pLbText->setVisible(true);
	if (m_pEdit->text() == m_pLbText->text() || m_pEdit->text().trimmed().isEmpty()) return;
    m_Text = m_pEdit->text();
    m_pLbText->setText(GetElideText(m_pEdit->text()));
	m_pParentWidget->GetItemData()->GetItemDataBase()->SetTitle(m_pLbText->text());
	//emit((IFSMatterViewManager*)MatterViewManager::GetInstance())->sigContentChanged(true);
	emit sigEndEdit(m_Text);
}

void TitleWidget::resizeEvent(QResizeEvent *e)
{
	m_pLbText->setGeometry(rect());
    m_pEdit->setGeometry(rect());
    m_pLbText->setText(GetElideText(m_Text));
}

void TitleWidget::mousePressEvent(QMouseEvent *e)
{
	FSBaseWidget::mousePressEvent(e);
	if (e->button() == Qt::LeftButton)
	{
		if (m_bEditing)
			return;
		m_bReady2Edit = m_pParentWidget->GetItemData()->isSelected() ? true : false;
	}
}

void TitleWidget::mouseReleaseEvent(QMouseEvent *e)
{
	FSBaseWidget::mouseReleaseEvent(e);
	if (e->button() == Qt::LeftButton)
	{
		if (m_bEditing)
			return;
		if (m_bReady2Edit) StartEdit();
	}
}

void TitleWidget::showEvent(QShowEvent *event)
{
    int w = width();
    int w2 = m_pLbText->width();
    m_pLbText->setText(GetElideText(m_pLbText->text()));
    FSBaseWidget::showEvent(event);
}

bool TitleWidget::eventFilter(QObject *watched, QEvent *e)
{
	if (!m_bEditing) return false;
	if (e->type() == QEvent::KeyPress)
	{
		QKeyEvent *keyEvent = (QKeyEvent *)e;
		if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter)
		{
			EndEdit();
		}
	}
	else if (e->type() == QEvent::MouseButtonPress)
	{
		QMouseEvent *me = (QMouseEvent*)e;
		auto globalPt = me->globalPos();
		auto localPt = mapFromGlobal(globalPt);

		auto rc = rect();
		if (!rc.contains(localPt))
		{
			EndEdit();
		}	
	}
	return false;
}

QString TitleWidget::GetElideText(const QString &text)
{
    return StringOperation::GetElidedString(m_pLbText->font(), m_pLbText->width(), text);
}

//////////////////////////////////////////////////////////////////////////
SelectedWidget::SelectedWidget(QWidget *parent)
	:FSBaseWidget(parent)
{
	m_state = State_Normal;
	setAttribute(Qt::WA_TransparentForMouseEvents);
	setAttribute(Qt::WA_TranslucentBackground);
}

void SelectedWidget::SetState(State s)
{
	m_state = s;
	update();
}

SelectedWidget::State SelectedWidget::GetState()
{
	return m_state;
}

void SelectedWidget::paintEvent(QPaintEvent *e)
{
	QPainter p(this);
	QColor clr;
	if (m_state == State_Normal || m_state == State_Hover)
		clr = QColor(87, 192, 201);
	else if (m_state == State_Selected)
		clr = QColor(224, 104, 104);
    QPen pen;
    pen.setColor(clr);
    pen.setJoinStyle(Qt::RoundJoin);
    pen.setWidth(2);
    p.setPen(pen);
	if (m_state == State_Hover)
	{
		QRect rcTopLine(1, 1, width()-2, height()-2);
		p.drawRect(rcTopLine);
	}

	QRect rcBottomLine(0, height() - 3, width(), 3);
	p.fillRect(rcBottomLine, clr);
}


//////////////////////////////////////////////////////////////////////////
AddNewItemWidget::AddNewItemWidget(QWidget *parent)
	:MVItemWidgetBase(parent)
{
	m_pLbIcon->setVisible(false);
	m_pBtnAdd->setVisible(false);
	SetRenamable(false);
	SetSelectable(false);
    setObjectName("AddNewItemWidget");
}

void AddNewItemWidget::enterEvent(QEvent *e)
{
	emit sigMouseEnter();
	MVItemWidgetBase::enterEvent(e);
}

void AddNewItemWidget::resizeEvent(QResizeEvent *e)
{
	MVItemWidgetBase::resizeEvent(e);
	m_pLbTitle->setGeometry(0, m_pLbThumbnail->geometry().bottom() + 1, width(), m_pLbTitle->height());
}
