#include "stdafx.h"
#include "FMatterListView.h"
#include "inc_FCommonLib/FBaseWidget.h"
#include "inc_FCommonLib/FCommonWidgets.h"
#include <QPainter>

/*FMatterListView*/
FMatterListView::FMatterListView(QWidget *parent /*= nullptr*/)
	: FBaseWidget(parent)
	//, m_CurrentCategory(0)
	, m_FoldState(Unfold)
	, m_FoldWidth(20)
	, m_UnfoldWidth(185)
{
	setFocusPolicy(Qt::ClickFocus);
	m_pScrollArea = new FScrollArea(this);
	m_pScrollArea->setWidgetResizable(true);
	m_pScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	m_pCategoryList = new FMatterCategoryList(0);
	m_pScrollArea->setWidget(m_pCategoryList);
	m_pAddCategoryBtn = new FImageButton(0, "FImageButton_AddFolder", this);
	m_pDeleteCategoryBtn = new FImageButton(1, "FImageButton_DeleteFolder", this);
	m_pFoldBtn = new FStateButton(0, "FStateButton_Fold", this);
	m_pFoldBtn->AddButton(FoldState::Unfold, "FStateButton_LeftArrow");
	m_pFoldBtn->AddButton(FoldState::Fold, "FStateButton_RightArrow");

	//connect(m_pCategoryList, &FMatterCategoryList::sigCategoryClicked, this, [&](int id){
	//	m_CurrentCategory = id;
	//});
	connect(m_pFoldBtn, &FStateButton::Clicked, this, &FMatterListView::slotFoldButtonClicked);
	connect(m_pAddCategoryBtn, &FImageButton::Clicked, this, &FMatterListView::slotAddCategoryButtonClicked);
	connect(m_pDeleteCategoryBtn, &FImageButton::Clicked, this, &FMatterListView::slotDeleteCategoryButtonClicked);
	connect(m_pCategoryList, &FMatterCategoryList::sigCategoryDeleted, this, &FMatterListView::slotCategoryDeleted);
	connect(m_pCategoryList, &FMatterCategoryList::sigCategoryAdded, this, &FMatterListView::slotCategoryAdded);
	connect(m_pCategoryList, &FMatterCategoryList::sigResize, this, &FMatterListView::slotCategoryListResize);
}

FMatterListView::~FMatterListView()
{

}

void FMatterListView::InitCategory(QList<QPair<QString, int>>* pCategories)
{
	m_pCategoryList->InitCategory(pCategories);
}

int FMatterListView::WidthHint()
{
	if (m_FoldState == Unfold){
		return m_UnfoldWidth;
	}
	else {
		return m_FoldWidth;
	}
}

void FMatterListView::InsertCategory(const QString& name, int nums, int index)
{
	m_pCategoryList->InsertCategory(name, nums, index);
}

void FMatterListView::resizeEvent(QResizeEvent *)
{
	int left, top;
	left = top = 0;
	const int FoldBtnWidth = m_FoldWidth;  // 20
	const int FolderBtnHeight = 25;
	if (m_FoldState == Unfold){
		m_pScrollArea->show();
		m_pCategoryList->show();
		m_pAddCategoryBtn->show();
		m_pDeleteCategoryBtn->show();
		m_pScrollArea->setGeometry(left, top, width() - FoldBtnWidth, height() - FolderBtnHeight - 5);
		//m_pCategoryList->setGeometry(left, top, width() - FoldBtnWidth, height() - FolderBtnHeight - 5);
		m_pAddCategoryBtn->setGeometry(left, height() - FolderBtnHeight, FolderBtnHeight, FolderBtnHeight);
		left = m_pAddCategoryBtn->geometry().right() + 5;
		m_pDeleteCategoryBtn->setGeometry(left, height() - FolderBtnHeight, FolderBtnHeight, FolderBtnHeight);
		left = m_pCategoryList->geometry().right();
		top = 0;
		m_pFoldBtn->setGeometry(width() - FoldBtnWidth, top, FoldBtnWidth, FoldBtnWidth);
	}
	else{
		m_pScrollArea->hide();
		//m_pCategoryList->hide();
		m_pDeleteCategoryBtn->hide();
		m_pAddCategoryBtn->hide();
		m_pFoldBtn->setGeometry(left, top, FoldBtnWidth, FoldBtnWidth);
	}
}

void FMatterListView::slotFoldButtonClicked(int id)
{
	if (id == FoldState::Unfold){
		m_FoldState = FoldState::Fold;
	}
	else{
		m_FoldState = FoldState::Unfold;
	}
	emit sigFoldStateChanged();
}
// 处理添加新分类逻辑
void FMatterListView::slotAddCategoryButtonClicked()
{
	int index = m_pCategoryList->GetInsertableIndex();
	m_pCategoryList->InsertCategory("MY ALBUM", 0, index);
}

void FMatterListView::slotDeleteCategoryButtonClicked()
{
	m_pCategoryList->DeleteCategory();
}

void FMatterListView::slotCategoryDeleted()
{
	m_pDeleteCategoryBtn->setEnabled(m_pCategoryList->Count());
}

void FMatterListView::slotCategoryAdded()
{
	m_pDeleteCategoryBtn->setEnabled(m_pCategoryList->Count());
}

void FMatterListView::slotCategoryListResize(const QSize& s)
{
	m_pScrollArea->widget()->resize(s);
}

/*FMatterCategory*/
FMatterCategory::FMatterCategory(int id, const QString& name, int nums, QWidget *parent /*= nullptr*/)
	:FBaseWidget(parent)
	, m_CategoryID(id)
	, m_CategoryName(name)
	, m_CategoryNums(nums)
	, m_bModifiable(true)
	, m_bHover(false)
	, m_bEditing(false)
	, m_bToolTipVisible(false)
{
	m_pTextBtn = new FButton(this);
	m_pTextBtn->setStyleSheet("FButton{text-align:left;border:0px}");
	m_pLineEdit = new FLineEdit(this);
	connect(m_pLineEdit, &FLineEdit::editingFinished, this, &FMatterCategory::slotEditingFinished);
	connect(m_pLineEdit, &FLineEdit::returnPressed, this, &FMatterCategory::slotEditingFinished);
	connect(m_pTextBtn, &FButton::clicked, this, [&](){
		m_CategoryState = Category_Opened;
		emit sigClicked(m_CategoryID);
	});
	setToolTip(m_CategoryName);
	m_pTextBtn->show();
	m_pLineEdit->hide();
}

FMatterCategory::~FMatterCategory()
{

}

void FMatterCategory::SetCategoryName(const QString& name)
{
	m_CategoryName = name;
	setToolTip(m_CategoryName);
	m_pTextBtn->setText(GetDisplayString(m_CategoryName));
	m_pLineEdit->setText(m_CategoryName);
}

void FMatterCategory::SetCategoryState(CategoryState state)
{
	m_CategoryState = state;
	update();
}

bool FMatterCategory::Modifiable()
{
	return m_bModifiable;
}

FMatterCategory::CategoryState FMatterCategory::GetCategoryState()
{
	return m_CategoryState;
}

void FMatterCategory::OpenCategory()
{
	m_CategoryState = Category_Opened;
	update();
}

void FMatterCategory::CloseCategory()
{
	m_CategoryState = Category_Closed;
	update();
}

void FMatterCategory::Edit()
{
	m_bEditing = true;
	update();
	m_pTextBtn->hide();
	m_pLineEdit->setText(m_CategoryName);
	m_pLineEdit->selectAll();
	m_pLineEdit->setFocus();
	m_pLineEdit->show();
}

int FMatterCategory::Count()
{
	return m_CategoryNums;
}

int FMatterCategory::GetCategoryID()
{
	return m_CategoryID;
}

void FMatterCategory::SetCategoryID(int id)
{
	m_CategoryID = id;
}

void FMatterCategory::resizeEvent(QResizeEvent *)
{
	int left, top;
	left = top = 0;
	m_pTextBtn->setGeometry(left, top, width(), height() - 1);
	m_pTextBtn->setText(GetDisplayString(m_CategoryName));
	m_pLineEdit->setGeometry(left, top, width(), height() - 1);
	m_pLineEdit->setText(m_CategoryName);
}

void FMatterCategory::paintEvent(QPaintEvent *)
{
	bool bUnderline = ((m_CategoryState == Category_Opened) || (m_bHover == true)) && (!m_bEditing);
	if (bUnderline){
		QPainter p(this);
		int w = m_pTextBtn->fontMetrics().width(m_pTextBtn->text());
		p.drawLine(QPoint(0, height() - 1), QPoint(w, height() - 1));
	}
}

void FMatterCategory::enterEvent(QEvent *)
{
	m_bHover = true;
	if (m_bToolTipVisible){
		setToolTipDuration(3000);
	}
	else{
		setToolTipDuration(1);
	}
	update();
}

void FMatterCategory::leaveEvent(QEvent *)
{
	m_bHover = false;
	update();
}

void FMatterCategory::slotEditingFinished()
{
	m_bEditing = false;
	update();
	m_CategoryName = m_pLineEdit->text();
	setToolTip(m_CategoryName);
	m_pTextBtn->setText(GetDisplayString(m_CategoryName));
	m_pLineEdit->hide();
	m_pTextBtn->show();
	emit sigEditingFinished(m_CategoryID, m_CategoryName);
}

QString FMatterCategory::GetDisplayString(const QString& text)
{
	QString strEnd = QString(" (%1)").arg(m_CategoryNums);
	int strEndWidth = m_pTextBtn->fontMetrics().width(strEnd);
	int strWidth = m_pTextBtn->fontMetrics().width(text);
	if (strWidth > m_pTextBtn->width() - strEndWidth){
		m_bToolTipVisible = true;
	}
	else{
		m_bToolTipVisible = false;
	}
	QString str = m_pTextBtn->fontMetrics().elidedText(text, Qt::ElideRight, m_pTextBtn->width() - strEndWidth, Qt::TextShowMnemonic);
	return str + strEnd;
}

/*FMatterCategoryList*/
FMatterCategoryList::FMatterCategoryList(QWidget *parent /*= nullptr*/)
	:FBaseWidget(parent)
	, m_CurrentCategory(0)
{
	setFocusPolicy(Qt::ClickFocus);
	m_pCategories = new QList < FMatterCategory* > ;
	m_pMenu = new FMenu(this);
	m_pRenameAction = m_pMenu->addAction(tr("Rename"), this, SLOT(FMatterListView::slotRenameActionTriggered));
	m_pDeleteAction = m_pMenu->addAction(tr("Delete Folder"), this, SLOT(FMatterListView::slotDeleteActionTriggered), QKeySequence::Delete);
	connect(m_pRenameAction, &QAction::triggered, this, &FMatterCategoryList::slotRenameActionTriggered);
	connect(m_pDeleteAction, &QAction::triggered, this, &FMatterCategoryList::slotDeleteActionTriggered);
}

FMatterCategoryList::~FMatterCategoryList()
{
	delete m_pCategories;
}

void FMatterCategoryList::InitCategory(QList<QPair<QString, int>>* pCategories)
{
	Q_ASSERT(pCategories);
	// 先清空数据
	m_pCategories->clear();
	// 添加
	for (int i = 0; i < pCategories->count(); ++i){
		InsertCategory((*pCategories)[i].first, (*pCategories)[i].second, i);
	}
}

void FMatterCategoryList::InsertCategory(const QString& name, int nums, int index)
{
	Q_ASSERT((index >= 0) && (index <= m_pCategories->count()));
	FMatterCategory *pCategory = new FMatterCategory(index, name, nums, this);
	connect(pCategory, &FMatterCategory::sigClicked, this, &FMatterCategoryList::slotCategoryClicked);
	m_pCategories->insert(index, pCategory);
	UpdateCategoryIDOnAdding();
	const int FolderHeight = 20;
	const int Gap = 2;
	int h = qMax(height(), m_pCategories->count() * (FolderHeight + Gap));
	resizeEvent(nullptr);
	emit sigResize(QSize(width(), h));
	emit sigCategoryAdded(index);
}

void FMatterCategoryList::DeleteCategory(int id)
{
	Q_ASSERT((id >= 0) && (id < m_pCategories->count()));
	// 调整m_CurrentCategory
	if (m_CurrentCategory == (m_pCategories->count() - 1)){
		--m_CurrentCategory;
	}
	UpdateCategoryIDOnDeleting();
	delete (*m_pCategories)[id];
	m_pCategories->removeAt(id);
	emit sigCategoryDeleted(id);
	resizeEvent(nullptr);
	if (m_CurrentCategory != -1){
		slotCategoryClicked(m_CurrentCategory);
	}
}

void FMatterCategoryList::DeleteCategory()
{
	DeleteCategory(m_CurrentCategory);
}

int FMatterCategoryList::Count()
{
	return m_pCategories->count();
}

void FMatterCategoryList::RenameCategory(int id)
{
	Q_ASSERT((id >= 0) && (id < m_pCategories->count()));
	(*m_pCategories)[id]->Edit();
}

int FMatterCategoryList::CategoryAt(const QPoint& p)
{
	int id = -1;
	for (int i = 0; i < m_pCategories->count(); ++i){
		if ((*m_pCategories)[i]->geometry().contains(p)){
			id = i;
			break;
		}
	}
	return id;
}

bool FMatterCategoryList::CategoryModifiableAt(int id)
{
	Q_ASSERT((id >= 0) && (id < m_pCategories->count()));
	return (*m_pCategories)[id]->Modifiable();
}
// 返回可添加分类的下标，规则：最大的modifiable分类下标
int FMatterCategoryList::GetInsertableIndex()
{
	int index;
	for (index = m_pCategories->count() - 1; index >= 0; --index){
		if ((*m_pCategories)[index]->Modifiable()) break;
	}
	return index + 1;
}

int FMatterCategoryList::CurrentCategory()
{
	return m_CurrentCategory;
}

void FMatterCategoryList::resizeEvent(QResizeEvent *)
{
	int left, top;
	left = top = 0;
	for (auto pCategory : *m_pCategories){
		pCategory->setGeometry(left, top, width(), 20);
		pCategory->show();
		top = pCategory->geometry().bottom() + 2;
	}
}

void FMatterCategoryList::contextMenuEvent(QContextMenuEvent *e)
{
	int id = CategoryAt(e->pos());
	if (id != -1){
		slotCategoryClicked(id);
	}
	bool bModifiable = (*m_pCategories)[m_CurrentCategory]->Modifiable();
	m_pRenameAction->setEnabled(bModifiable);
	m_pDeleteAction->setEnabled(bModifiable);
	m_pMenu->exec(QCursor::pos());
}

void FMatterCategoryList::UpdateCategoryIDOnDeleting()
{
	// 从m_CurrentCategory + 1起，id减一
	for (int i = m_CurrentCategory + 1; i < m_pCategories->count(); ++i){
		(*m_pCategories)[i]->SetCategoryID(i - 1);
	}
}

void FMatterCategoryList::UpdateCategoryIDOnAdding()
{
	// 从m_CurrentCategory + 1起，id加一
	for (int i = m_CurrentCategory + 1; i < m_pCategories->count(); ++i){
		(*m_pCategories)[i]->SetCategoryID(i);
	}
}

void FMatterCategoryList::slotCategoryClicked(int id)
{
	m_CurrentCategory = id;
	for (auto pCategory : *m_pCategories){
		if (pCategory->GetCategoryID() != m_CurrentCategory){
			pCategory->SetCategoryState(FMatterCategory::Category_Closed);
		}
		else{
			pCategory->SetCategoryState(FMatterCategory::Category_Opened);
		}
	}
	emit sigCategoryClicked(m_CurrentCategory);
}

void FMatterCategoryList::slotRenameActionTriggered()
{
	RenameCategory(m_CurrentCategory);
}

void FMatterCategoryList::slotDeleteActionTriggered()
{
	DeleteCategory(m_CurrentCategory);
}

