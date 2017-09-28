#ifndef BASEWIDGET_H
#define BASEWIDGET_H

#include "inc_FSCommonlib/fscommonlib_global.h"
#include "inc_CommonLib/BaseDialog.h"
#include "inc_CommonLib/CommonWidgets.h"
#include <QtWidgets/QtWidgets>


class FSCOMMONLIB_EXPORT FSBaseWidget : public QWidget
{
	Q_OBJECT

public:
	FSBaseWidget(QWidget *parent);
	virtual void RetranslateUi();
    virtual void TranslateText();
protected:
	
	bool event(QEvent *e) override;
};


class FSCOMMONLIB_EXPORT FSBaseDialog : public BaseDialog
{
	Q_OBJECT

public:
	FSBaseDialog(const QString &title, QWidget *parent);
	FSBaseDialog(const QString &trtitle, const char *title, QWidget *parent);
	virtual void RetranslateUi();
protected:

	bool event(QEvent *e) override;
	void resizeEvent(QResizeEvent *event);
	void Init();

private:
	std::string m_title;
};


// 图片按钮
class FSCOMMONLIB_EXPORT FSBaseButton:public BaseButton
{
    Q_OBJECT
signals:
    void pressed();
public:
    FSBaseButton(int id, const QString &objectname, QWidget *parent);
    FSBaseButton(const QString &objectname, QWidget *parent);

private:
    bool eventFilter(QObject *, QEvent *);
};

//显示图片和文件的按钮
class FSCOMMONLIB_EXPORT FSNormalButton :public FSBaseButton
{
	Q_OBJECT
	signals :
	void pressed();
public:
	FSNormalButton(int id, const QString &objectname, QWidget *parent);
	FSNormalButton(const QString &objectname, QWidget *parent);
	void setText(QString strText);
protected:
	void resizeEvent(QResizeEvent *event)override;
private:
	bool eventFilter(QObject *, QEvent *);
	QLabel* m_TextLabel;// 显示文字的标签
	QString m_strText;
};

// 状态按钮
class FSCOMMONLIB_EXPORT FSStateBaseButton : public BaseStateButton
{
    Q_OBJECT
public:
    FSStateBaseButton(int id, const QString &objectname, QWidget *parent = nullptr);
	//void AddButton(BaseButton *button);

};

// 文字按钮
class FSCOMMONLIB_EXPORT FSButton : public QPushButton
{
	Q_OBJECT
public:
	FSButton(QWidget *parent);

};


class FSCOMMONLIB_EXPORT FSAction : public QAction
{
	Q_OBJECT
public:
	FSAction(QObject *parent);

};

class FSCOMMONLIB_EXPORT FSMenu : public QMenu
{
	Q_OBJECT
public:
	FSMenu(QWidget *parent);

};

class FSCOMMONLIB_EXPORT FSLabel : public QLabel
{
	Q_OBJECT
public:
	FSLabel(QWidget *parent);


};


class FSCOMMONLIB_EXPORT FSListWidget :public QListWidget
{
    Q_OBJECT
public:
    FSListWidget(const QString &objName,QWidget *pParent);
};

class FSCOMMONLIB_EXPORT FSLineEdit :public QLineEdit
{
    Q_OBJECT
public:
    FSLineEdit(QWidget *pParent);
    FSLineEdit(const QString &objName, QWidget *pParent);

    void SetTextBlockSignal(const QString &text);

protected:
    bool eventFilter(QObject * watched, QEvent * event) override;
    void focusInEvent(QFocusEvent *event);
    void focusOutEvent(QFocusEvent *event);

};

class FSCOMMONLIB_EXPORT FSFileNameLineEdit :public FSLineEdit
{
    Q_OBJECT
public:
    FSFileNameLineEdit(QWidget *pParent);
    FSFileNameLineEdit(const QString &objName, QWidget *pParent);

protected:
    void keyPressEvent(QKeyEvent *event);

private:
    QString m_deleteString;
};


class FSCOMMONLIB_EXPORT FSComboBox :public QComboBox
{
    Q_OBJECT
public:
    FSComboBox(const QString &objName,QWidget *pParent);
    FSComboBox(QWidget *pParent);

    void EnableWheelEvent(bool bEnable);

    void SetAutoFixedMenuWidth(bool enabled);
protected:

    // 调整弹出框显示所有字体
    void FixedMenuWidth();

    void mousePressEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *e);

private:
    bool m_autoFixedMenuWidth;
    bool m_bEnableWheel;
};


class FSCOMMONLIB_EXPORT FSMessageBox :public FSBaseDialog
{
	Q_OBJECT
public:
	// TitleBar_Close标题栏关闭按钮
	enum Button{TitleBar_Close = 0x0, Yes = 0x1, No = 0x2, OK = 0x4, Cancel = 0x8};
public:
    FSMessageBox(const QString &title, const QString &text, Qt::AlignmentFlag flag = Qt::AlignRight, int buttons = Button::Yes, QWidget *parent = nullptr); // 支持Qt::AlignLeft Qt::AlignCenter Qt::AlignRight
	
    static Button information(const QString & title, const QString & text, int buttons, QWidget * parent, Qt::AlignmentFlag flag = Qt::AlignRight, int w = 580, int h = 245);

    static Button informationOk(const QString & title, const QString & text, QWidget * parent, int w = 580, int h = 245);
	//全局DoModal
	static Button informationOkEx(const QString & title, const QString & text,int w = 580, int h = 245);

    static Button informationYes(const QString & title, const QString & text, QWidget * parent, int w = 580, int h = 245);
	static Button informationYesEx(const QString & title, const QString & text, int w = 580, int h = 245);
    static Button questionYesNo(const QString & title, const QString & text, QWidget * parent, int w = 580, int h = 245);
	static Button questionYesNoEx(const QString & title, const QString & text, int w = 580, int h = 245);
    static Button questionYesNoCancel(const QString & title, const QString & text, QWidget * parent, int w = 580, int h = 245);
	static Button questionYesNoCancelEx(const QString & title, const QString & text,int w = 580, int h = 245);

	

public:
	void SetButtonText(Button id, QString text);
	QPushButton* GetButton(Button id);

	void SetIconObjectName(QString objectName);
	void SetIconVisible(bool visible);

protected:
	void CreateButtons(Button buttons);
	void resizeEvent(QResizeEvent *event);
	bool eventFilter(QObject *watcher, QEvent *event);
    virtual void RetranslateUi();
public:
	FSLabel *m_lbIcon;
	QTextEdit *m_lbText;

	Button m_buttons;
	QList<QPushButton*> m_bnts;

	QHBoxLayout *m_btnLayout;
	QWidget *m_layoutWidget;
};


class FSCOMMONLIB_EXPORT FSMEssageConfirmDlg : public FSMessageBox
{
    Q_OBJECT
public:
    FSMEssageConfirmDlg(const QString &title, const QString &text, const QString & confirmText, Qt::AlignmentFlag flag = Qt::AlignRight, int buttons = Button::Yes, QWidget *parent = nullptr);

    static std::tuple<bool, FSMessageBox::Button> questionYesNo(const QString & title, const QString & text, const QString & confirmText, bool bChecked, QWidget *parent);
    
public:
    void SetChecked(bool checked);
    bool IsCheck();
protected:
    void resizeEvent(QResizeEvent *e) override;
    void showEvent(QShowEvent *e);
protected:
    QCheckBox *m_pCbDontShow;
    QLabel *m_pLbDonShow;
};


//////////////////////////////////////////////////////////////////////////
class QHBoxLayout;
class CategoryBarButton;
class FSCOMMONLIB_EXPORT CategoryBar : public FSBaseWidget
{
	Q_OBJECT
		Q_PROPERTY(QColor normalTextColor READ GetNormalTextColor WRITE SetNormalTextColor)
		Q_PROPERTY(QColor selectedTextColor READ GetSelectedTextColor WRITE SetSelectedTextColor)
		Q_PROPERTY(QColor normalBackgroundColor READ GetNormalBackgroundColor WRITE SetNormalBackgroundColor)
		Q_PROPERTY(QColor selectedBackgroundColor READ GetSelectedBackgroundColor WRITE SetSelectedBackgroundColor)
		Q_PROPERTY(QColor normalBottomLineColor READ GetNormalBottomLineColor WRITE SetNormalBottomLineColor)
		Q_PROPERTY(QColor selectedBottomLineColor READ GetSelectedBottomLineColor WRITE SetSelectedBottomLineColor)
public:
	CategoryBar(const QString &objname, QWidget *parent);
	void AddItem(int id, const QString &objname, const QString &text, const char *sztext);
	void SelectItem(int id);
	void SetBottomLineHeight(int h);
    QList<CategoryBarButton*> GetItems();

	QColor GetNormalTextColor();
	void SetNormalTextColor(QColor clr);
	QColor GetSelectedTextColor();
	void SetSelectedTextColor(QColor clr);
	QColor GetNormalBackgroundColor();
	void SetNormalBackgroundColor(QColor clr);
	QColor GetSelectedBackgroundColor();
	void SetSelectedBackgroundColor(QColor clr);
	QColor GetNormalBottomLineColor();
	void SetNormalBottomLineColor(QColor clr);
	QColor GetSelectedBottomLineColor();
	void SetSelectedBottomLineColor(QColor clr);
protected:
Q_SIGNALS:
	void sigItemChanged(int oldid, int newid);
protected:
    void RetranslateUi() override;

    QList<CategoryBarButton*> m_lstItems;
	int m_curId;

private:
	QColor normalTextColor;
	QColor selectedTextColor;
	QColor normalBackgroundColor;
	QColor selectedBackgroundColor;
	QColor normalBottomLineColor;
	QColor selectedBottomLineColor;

    QHBoxLayout *m_pHL;
	int m_nBottomLineHeight;
};



class FSCOMMONLIB_EXPORT CategoryBarButton : public FSBaseWidget
{
	Q_OBJECT
		Q_PROPERTY(QPixmap normalImg READ GetNormalImage WRITE SetNormalImage)
		Q_PROPERTY(QPixmap selectedImg READ GetSelectedImage WRITE SetSelectedImage)
public:
	enum BUTTONSTATE
	{
		BUTTONSTATE_NORMAL = 0,
		BUTTONSTATE_SELECTED,
	};
	CategoryBarButton(int id, const QString &objname, QWidget *parent);
	void SetText(const QString &text);
	void SetColors(QColor normalTextColor,
		QColor selectedTextColor,
		QColor normalBackgroundColor,
		QColor selectedBackgroundColor,
		QColor normalBottomLineColor,
		QColor selectedBottomLineColor);
	void SetButtonState(BUTTONSTATE s);
	int GetId();

	QPixmap GetNormalImage();
	void SetNormalImage(QPixmap px);
	QPixmap GetSelectedImage();
	void  SetSelectedImage(QPixmap px);

	void SetBottomLineHeight(int h);

	int Width();
protected:
	void paintEvent(QPaintEvent *e) override;
	void mousePressEvent(QMouseEvent *e) override;
Q_SIGNALS:
	void sigSelected(int id);
private:
	QString m_text;
	QPixmap normalImg;
	QPixmap selectedImg;

	QColor normalTextColor;
	QColor selectedTextColor;
	QColor normalBackgroundColor;
	QColor selectedBackgroundColor;
	QColor normalBottomLineColor;
	QColor selectedBottomLineColor;

    

	int m_nBottomLineHeight;
	BUTTONSTATE m_state;
	int m_id;
};

//class FSCOMMONLIB_EXPORT FSLineEditEx : public FSLineEdit
//{
//	Q_OBJECT
//public:
//	FSLineEditEx(const QString &objName, QWidget *pParent);
//protected:
//	bool eventFilter(QObject * watched, QEvent * event) override;
//    void focusInEvent(QFocusEvent *event);
//    void focusOutEvent(QFocusEvent *event);
//
//};


class FSCOMMONLIB_EXPORT FSIconButton :public FSButton{
    Q_OBJECT
        Q_PROPERTY(QString normalImagePath READ GetNormalImagePath WRITE SetNormalImagePath)
        Q_PROPERTY(QString hoverImagePath READ GetHoverImagePath WRITE SetHoverImagePath)
        Q_PROPERTY(QString disabledImagePath READ GetDisabledImagePath WRITE SetDisabledImagePath)

public:
    FSIconButton(QWidget *pParent);
    ~FSIconButton();

    void SetEnable(bool bEnable);

protected:
    void resizeEvent(QResizeEvent *);

    void leaveEvent(QEvent *);
    void enterEvent(QEvent *);

private:
    QString GetNormalImagePath();
    void SetNormalImagePath(QString path);
    QString GetHoverImagePath();
    void SetHoverImagePath(QString path);
    QString GetDisabledImagePath();
    void SetDisabledImagePath(QString path);

private:
    FSLabel *m_pBarLabel;
    int      m_State;
    QString  normalImagePath;
    QString  hoverImagePath;
    QString  disabledImagePath;
};


#include <QSharedMemory>

class FSCOMMONLIB_EXPORT OneApplication
{
public:
    OneApplication(QString key);
    ~OneApplication();

    bool IsOneApp();
    static void ShowWindow(QString wndName);
	static bool IsWindowsVisible(QString wndName);
private:
    QString m_mutexName;
    HANDLE m_handle;
};

class IDragValueChangedHelper;
class FSCOMMONLIB_EXPORT FSDoubleSpinBox :public QDoubleSpinBox
{
    Q_OBJECT
public:
    FSDoubleSpinBox(QWidget *pParent);
    ~FSDoubleSpinBox();

    void SetDragEnabled(bool enabled);
    void setValue(double val);
    void setRange(double min, double max);
    void setDecimals(int prec);
    void SetLoop(bool loop);

    IDragValueChangedHelper* GetHelper();

protected:
    void wheelEvent(QWheelEvent *event);
    bool eventFilter(QObject *watcher, QEvent *event);
    void focusInEvent(QFocusEvent *event);
    void focusOutEvent(QFocusEvent *event);
private slots:
    void SlotValueChanged(double value);

protected:
    IDragValueChangedHelper *m_dragHelper;

private:
    bool m_firstFocus;
};

class FSCOMMONLIB_EXPORT FSSpinBox :public QSpinBox
{
    Q_OBJECT
public:
    FSSpinBox(QWidget *pParent);
    ~FSSpinBox();
protected:
    void wheelEvent(QWheelEvent *event);
};



class FSCOMMONLIB_EXPORT CusMenuItemBase : public QWidget
{
	Q_OBJECT
public:
	CusMenuItemBase(QWidget *parent = nullptr);
	virtual void Resize(){};
	
Q_SIGNALS:
	void sigActivate();
protected:
	void SetLeftPadding(int margin);
	int GetLeftPadding();
private:
	int m_leftPadding;

	friend class CusMenu;
};

class FSCOMMONLIB_EXPORT CusMenu : public QMenu
{
	Q_OBJECT
public:
	CusMenu(QWidget *parent);
	void AddWidget(CusMenuItemBase *w);
	void SetLeftPadding(int padding);
	int GetLeftPadding();

	void clear();
protected:
	void OnAbout2Show();
	void OnAbout2Hide();
	bool eventFilter(QObject *, QEvent *) override;
private:
	QList<CusMenuItemBase*> m_widgets;
	QWidget *m_parent;

	//int m_itemHight;
	int m_leftPadding;
};

class ColorItem : public FSBaseWidget
{
	Q_OBJECT
public:
	ColorItem(QColor clr,QColor selectClr,QWidget *parent);
	void SetColor(QColor clr);
	QColor GetColor();
	void SetSelected(bool bSelected);
	bool IsSelected();
	void SetSelectBorderColor(QColor clr);
Q_SIGNALS:
	void sigCliked(QColor clr);
protected:
	void mousePressEvent(QMouseEvent *e) override;
	void enterEvent(QEvent *e) override;
	void leaveEvent(QEvent *e) override;
	void paintEvent(QPaintEvent *e) override;
private:
	QColor m_color;
	QColor m_selectedColor;
	bool m_bSelected;
	bool m_bHoverd;
};

class FSCOMMONLIB_EXPORT ColorMenuItem : public CusMenuItemBase
{
	Q_OBJECT
public:
	ColorMenuItem(QWidget *parent);
	void AddColor(QColor clr);
	void SetSelectColor(QColor clr);
	void SetToolTip(QColor clr, QString toolTip);
	void Resize() override;
Q_SIGNALS:
	void sigSelectedChanged(QColor clr);
protected:
	void showEvent(QShowEvent *e) override;
	void resizeEvent(QResizeEvent *e) override;
	void mousePressEvent(QMouseEvent *e) override;
private:
	QList<ColorItem*> m_items;

	static const int itemW = 18;
	static const int itemH = 18;
	static const int itemMargin = 10;
};


#endif // BASEWIDGET_H
