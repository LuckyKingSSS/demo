#ifndef FMATTERLISTVIEW_H
#define FMATTERLISTVIEW_H

#include "inc_FCommonLib/FBaseWidget.h"

class FScrollArea;
class FImageButton;
class FStateButton;

/*FMatterListView*/
class FMatterCategoryList;
class FMenu;
class FMatterListView : public FBaseWidget
{
	Q_OBJECT

public:
	enum FoldState{Unfold, Fold};

public:
	FMatterListView(QWidget *parent = nullptr);
	~FMatterListView();

	// 初始化分类
	void InitCategory(QList<QPair<QString, int>>*);
	int WidthHint();
	void InsertCategory(const QString& name, int nums, int index);

Q_SIGNALS:
	void sigFoldStateChanged();

protected:
	void resizeEvent(QResizeEvent *) override;

private:
	void slotFoldButtonClicked(int);
	void slotAddCategoryButtonClicked();
	void slotDeleteCategoryButtonClicked();
	void slotCategoryDeleted();
	void slotCategoryAdded();
	void slotCategoryListResize(const QSize&);

private:
	//int m_CurrentCategory;
	FoldState m_FoldState;
	FScrollArea *m_pScrollArea;
	FMatterCategoryList *m_pCategoryList;
	FImageButton *m_pAddCategoryBtn;
	FImageButton *m_pDeleteCategoryBtn;
	FStateButton *m_pFoldBtn;
	const int m_FoldWidth;
	const int m_UnfoldWidth;
};

/*FMatterCategory*/
class FButton;
class FLineEdit;
class FMatterCategory : public FBaseWidget
{
	Q_OBJECT

public:
	enum CategoryState{ Category_Opened, Category_Closed };

public:
	/*
	 *id: 资源id
	 *name: 资源名
	 *nums: 资源个数
	*/
	FMatterCategory(int id, const QString& name, int nums, QWidget *parent = nullptr);
	~FMatterCategory();

	void SetCategoryName(const QString&);
	void SetCategoryState(CategoryState);
	bool Modifiable();
	CategoryState GetCategoryState();
	void OpenCategory();
	void CloseCategory();
	void Edit();
	int Count();
	int GetCategoryID();
	void SetCategoryID(int);

Q_SIGNALS:
	void sigClicked(int);
	void sigEditingFinished(int, const QString&);
	void sigNameChanged(const QString &);

protected:
	void resizeEvent(QResizeEvent *) override;
	void paintEvent(QPaintEvent *) override;
	void enterEvent(QEvent *) override;
	void leaveEvent(QEvent *) override;

private:
	void slotEditingFinished();
	QString GetDisplayString(const QString&);

private:
	int m_CategoryID;
	int m_CategoryNums;
	bool m_bModifiable;
	bool m_bHover;
	bool m_bEditing;
	bool m_bToolTipVisible;
	CategoryState m_CategoryState;
	FButton *m_pTextBtn;
	FLineEdit *m_pLineEdit;
	QString m_CategoryName;
};

/*FMatterCategoryList*/
class FMatterCategoryList : public FBaseWidget
{
	Q_OBJECT

public:
	FMatterCategoryList(QWidget *parent = nullptr);
	~FMatterCategoryList();

	// 初始化分类
	void InitCategory(QList<QPair<QString, int>>*);

	/*
	*指定位置插入分类
	*name: 分类名
	*nums: 资源个数
	*index: 位置
    */
	void InsertCategory(const QString& name, int nums, int index);
	
	/* 
	*删除分类
	*id: 分类id
	*/
	void DeleteCategory(int id);
	void DeleteCategory();
	int Count();
	void RenameCategory(int id);
	int CategoryAt(const QPoint&);
	bool CategoryModifiableAt(int id);
	int GetInsertableIndex();
	int CurrentCategory();

Q_SIGNALS:
	void sigCategoryClicked(int);
	void sigCategoryDeleted(int);
	void sigCategoryAdded(int);
	void sigResize(const QSize&);

protected:
	void resizeEvent(QResizeEvent *) override;
	void contextMenuEvent(QContextMenuEvent *) override;

private:
	void UpdateCategoryIDOnDeleting();
	void UpdateCategoryIDOnAdding();
	void slotCategoryClicked(int);
	void slotRenameActionTriggered();
	void slotDeleteActionTriggered();

private:
	int m_CurrentCategory;
	QList<FMatterCategory*>* m_pCategories;
	FMenu *m_pMenu;
	QAction *m_pRenameAction;
	QAction *m_pDeleteAction;
};

#endif // FMATTERLISTVIEW_H
