#ifndef LISTWIDGETBASE_H
#define LISTWIDGETBASE_H

#include "inc_FSCommonlib/fscommonlib_global.h"
#include "inc_FSCommonlib/FSCommonWidget.h"
#include <QObject>
#include <QListWidget>

class MVItemWidgetBase;
class MVListItemData;
class SelectionRect;
class FSCOMMONLIB_EXPORT MVListWidget : public QListWidget
{
	Q_OBJECT

public:
	MVListWidget(QWidget *parent);
	~MVListWidget();

	void AddItem(MVListItemData *pItemData, MVItemWidgetBase *pItemWidget);
	void InsertItem(int row, MVListItemData *pItemData, MVItemWidgetBase *pItemWidget);
	void RemoveItem(MVListItemData *pItemData);
	void RemoveSelectedItems();
	void UnselectAllItems();
	QList<MVListItemData *> Items();
	void SetItemSize(int w, int h); //整个item大小

	void SetItemHidden(MVListItemData *pItemData, bool bHidden);
	void SetSingleSelect(bool b);
	void SetItemWidgetSize(int w, int h);
	void SetSpacing(int space);
	void ItemConfuseFix();
	void SetNeedItemFix(bool bNeed);
Q_SIGNALS:
	void sigBlankRClicked(QPoint pt);
	void sigItemRClicked(QPoint pt, MVListItemData *pItemData);
protected:
	void mousePressEvent(QMouseEvent *e) override;
	void mouseMoveEvent(QMouseEvent *e) override;
	void mouseReleaseEvent(QMouseEvent *e) override;
	void resizeEvent(QResizeEvent *e) override;
private:
	int m_nItemWidgetW;
	int m_nItemWidgetH;
	SelectionRect *m_pSelectionRect;
	QPoint m_ptPress;

	bool m_bNeedItemFix;
};

class SelectionRect : public FSBaseWidget
{
	Q_OBJECT
public:
	SelectionRect(QWidget *parent);
	void SetRect(QRect rc);
protected:
	void paintEvent(QPaintEvent *e) override;
private:
	QRect m_rcSelected;
};

class BaseButton;
class SelectedWidget;
class TitleWidget;
class FSCOMMONLIB_EXPORT MVItemWidgetBase : public FSBaseWidget
{
	Q_OBJECT
public:
	MVItemWidgetBase(QWidget *parent);
	~MVItemWidgetBase();
	void SetItemData(MVListItemData* pItemData);
	MVListItemData* GetItemData();
	void Rename();
	void SetRenamable(bool bEnable);
	void SetSelectable(bool bEnable);
    QString GetTitle();
	void SetTitle(QString);

	virtual void Drag();
Q_SIGNALS:
	void sigAddBtnClicked(MVItemWidgetBase*);
	void sigEndEdit(MVItemWidgetBase* pWidget, QString text);
	void sigDragBegin(MVItemWidgetBase* pWidget);
	void sigDragEnd(MVItemWidgetBase* pWidget);
protected:
	QPixmap GetAspectPixmap(QPixmap src, int w, int h, bool bBlackBackgound = true);

	virtual void resizeEvent(QResizeEvent *e) override;
	virtual void enterEvent(QEvent *e) override;
	virtual void leaveEvent(QEvent *e) override;
	virtual void mousePressEvent(QMouseEvent *e) override;
	virtual void mouseMoveEvent(QMouseEvent *e) override;
	virtual void mouseReleaseEvent(QMouseEvent *e) override;

protected:
	MVListItemData* m_pItemData;
	QLabel *m_pLbThumbnail;
	TitleWidget *m_pLbTitle;
	QLabel *m_pLbIcon;
	BaseButton *m_pBtnAdd;
	SelectedWidget *m_pSelectedWidget;
	QPoint m_ptPress;

	bool m_bSelectable;
};

class FSCOMMONLIB_EXPORT TitleWidget : public FSBaseWidget
{
	Q_OBJECT
public:
	TitleWidget(MVItemWidgetBase *parent);

	void SetText(const QString &text);
	void SetEditable(bool b);
	void StartEdit();
Q_SIGNALS:
	void sigEndEdit(QString text);
protected:
	void EndEdit();

	void resizeEvent(QResizeEvent *e) override;
	void mousePressEvent(QMouseEvent *e) override;
	void mouseReleaseEvent(QMouseEvent *e) override;
    void showEvent(QShowEvent *event);
	bool eventFilter(QObject *watched, QEvent *e) override;

    QString GetElideText(const QString &text);
protected:
	QLabel *m_pLbText;
	QLineEdit *m_pEdit;

	bool m_bEditable;
	bool m_bReady2Edit;
	bool m_bEditing;
    QString m_Text;
	MVItemWidgetBase *m_pParentWidget;
};

class FSCOMMONLIB_EXPORT SelectedWidget : public FSBaseWidget
{
	Q_OBJECT
public:
	enum State
	{
		State_Normal = 0,
		State_Hover,
		State_Selected,
	};
	SelectedWidget(QWidget *parent);
	void SetState(State);
	State GetState();
protected:
	void paintEvent(QPaintEvent *e) override;
private:
	State m_state;
};

class FSCOMMONLIB_EXPORT AddNewItemWidget : public MVItemWidgetBase
{
	Q_OBJECT
public:
	AddNewItemWidget(QWidget *parent);
Q_SIGNALS:
	void sigMouseEnter();

protected:
	void enterEvent(QEvent *e) override;
	void resizeEvent(QResizeEvent *e) override;

};

#endif // LISTWIDGETBASE_H
