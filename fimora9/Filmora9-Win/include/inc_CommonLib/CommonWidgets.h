#pragma once

#include "inc_CommonLib/commonlib_global.h"
#include <QtWidgets/QPushButton>
#include <QtCore/QList>
#include <QtWidgets/QWidget>
#include <QtWidgets/QLabel>

class COMMONLIB_EXPORT BaseButton : public QWidget
{
	Q_OBJECT
public:
	BaseButton(int id, const QString &objectname, QWidget *parent);
	BaseButton(const QString &objectname, QWidget *parent);

	void SetEnabled(bool enable);
	int GetId();

Q_SIGNALS:
	void Clicked(int id, QPoint p);
    void Pressed(int id, QPoint p);
private:
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
Q_SIGNALS:
	void clicked();
protected:
	int m_id;
};


class QStackedWidget;
class COMMONLIB_EXPORT BaseStateButton : public BaseButton
{
	Q_OBJECT
public:
	BaseStateButton(int id, const QString &objectname = "", QWidget *parent = nullptr);

	// 一个按钮表示一个状态，当前只显示一个按钮，第一个增加是默认按钮（状态），点击一次依次切换到下个按钮
	void AddButton(int id, const QString &objectName = "");
	void AddButton(BaseButton *button);

	int CurrentButtonId();
	int BaseStateButtonId();

	void SetCurrentButtonId(int id);
	void Restore();
	void ClickCurrentButton();
	BaseButton* GetButton(int id);
protected:
	void resizeEvent(QResizeEvent *event);
	void showEvent(QShowEvent *event);

private:
	QList<BaseButton*> m_btns;
	int m_curIndex;
};



class COMMONLIB_EXPORT ToolBarSlider : public QWidget
{
	Q_OBJECT
public:
	ToolBarSlider(QWidget *parent, Qt::Orientation orientation = Qt::Horizontal);
	void setValue(float value, bool queueRepaint = true); // 使用队列刷新界面
	void setDuration(float duration);
	float value();
	float duration();
	bool canSeek();
	int Height();
	void SetGrooveHeight(int h);
	bool ButtonPressDown();
	void SetButtonPressDown(bool press);
	void SetLeftToRight(bool left);
	void SetBottomToTop(bool bottom);
	void SetChunkHeight(int height);
Q_SIGNALS:
	void valueChanged(double);
	void startValueChanged();
	void endValueChanged();
	// 当拖拽时才发送
	void startDrag();
	void endDrag();
private slots:
	void ResizeChunk();
	void SetCanSeek(bool can);
protected:
	void resizeEvent(QResizeEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	bool event(QEvent *event);

	int GetWidth();
	void CalculatePostion(int mousePos);
	void CalculatePostion(QPoint p);
	void CalculatePositionFromValue(double value);

	bool NeedEmitValue();
	bool CanChanged();

	
private:
	float m_duration;
	float m_value;

	Qt::Orientation m_orientation;
	QWidget *m_bg;
	QWidget *m_groove;
	QWidget *m_chunk;

	int	m_position;
	int m_offset;
	int m_chunkWidth;
	int m_grooveHeight;

	bool m_canSeek;
	bool m_bStartSeek;

	bool m_leftToRight;
	bool m_bottomToTop;

	bool m_bMousePress;


	
};

// 时间线Scale slider
class BaseButton;
class COMMONLIB_EXPORT ScaleUnitSlider : public QWidget
{
	Q_OBJECT
public:
	ScaleUnitSlider(QWidget *parent);
	void setDuration(int duration);
	void setValue(int value);
	int value();
	int duration();
	void SetGrooveHeight(int h);
	void SetChunkHeight(int height);

	void Add();
	void Sub();

	bool ButtonPressDown();

	BaseButton* GetZoomInButton();
	BaseButton* GetZoomOutButton();

Q_SIGNALS:
	void valueChanged(int value);
	void startValueChanged();
	void endValueChanged();
protected:
	void resizeEvent(QResizeEvent *event);

protected:
	BaseButton *m_sub;
	BaseButton *m_add;
	ToolBarSlider *m_slider;
};



class COMMONLIB_EXPORT MidSlider : public QWidget
{
	Q_OBJECT
public:
	MidSlider(QWidget *parent = nullptr);

	void SetValue(int value);
	void SetRange(int minvalue, int maxvalue);
	void GetRange(int &minvalue, int &maxvalue);
	void SetRange(int minvalue, int midvalue, int maxvalue, float midrate = 0.5f);
	int GetValue();
	void SetChunkWidth(int w);
	void SetBackgroundHeight(int h);

	//步进的大小
	void SetStepOver(float fStep);
	// 限制最大操作值
	void SetMaxmumLimit(int maxvalue);
	// 限制最小操作值
	void SetMinmumLimit(int minvalue);
Q_SIGNALS:
	void StartValueChanged(int value);
	void ValueChanged(int value);
	void EndValueChanged(int value);
protected:
	void resizeEvent(QResizeEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	bool event(QEvent *event);
	void keyPressEvent(QKeyEvent *event);

	int GetWidth();
	int GetLeftWidth();
	int GetRightWidth();
	int CalculateValue(int pos);
	int CalculatePosition(int value);
	int GetMin();
	int GetMax();
	bool IsLimitValue();
private slots:
	void ResizeChunk();
private:
	int m_value;
	int m_valueTemp;
	int m_min;
	int m_max;
	int m_mid;
	float m_midrate;
	float m_stepover;
	//int m_duration;

	QWidget *m_bg;
	QWidget *m_groove;
	QWidget *m_chunk;

	int	m_position;
	int m_chunkWidth;
	int m_backgroundHeight;

	int m_maxmumLimit;
	int m_minmumLimit;
};



// 编辑颜色slider
class PureSliderPrivate;
class COMMONLIB_EXPORT PureSlider : public QWidget
{
	Q_OBJECT
public:
	PureSlider(QWidget *parent);
	~PureSlider();
	void SetRange(int nStart, int nEnd);
	void GetRange(int *start, int *end);
	void SetValue(int nValue);
	int	 GetValue();
	void SetBorderColor(const QColor &clr);
	void SetSubpageColor(const QColor &clr);
	void SetAddpageColor(const QColor &clr);

Q_SIGNALS:
	void sigValueChanged(int v);
protected:
	void mousePressEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
	void mouseMoveEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
	void paintEvent(QPaintEvent *e) Q_DECL_OVERRIDE;
private:
	PureSliderPrivate *m_p;
};

class QLabel;
class COMMONLIB_EXPORT SetColorParamWidget : public QWidget
{
	Q_OBJECT
public:
	SetColorParamWidget(const QString &title,QWidget *parent);
	~SetColorParamWidget();

	void SetRange(int nStart, int nEnd);
	void GetRange(int *start, int *end);
	void SetValue(int nValue);
	int	 GetValue();
	//void SetBorderColor(const QColor &clr);
	//void SetSubpageColor(const QColor &clr);
	//void SetAddpageColor(const QColor &clr);
Q_SIGNALS:
	void sigValueChanged(int v);

protected:
	void resizeEvent(QResizeEvent *e) Q_DECL_OVERRIDE;
private:
	MidSlider *m_pSlider;
	QLabel *m_pLabelTitle;
	QLabel *m_pLabelParam;
};

class COMMONLIB_EXPORT MagicLabel : public QLabel
{
	Q_OBJECT
public:
	explicit MagicLabel(const QString &text, QWidget *parent = 0);
	explicit MagicLabel(QWidget *parent = 0);
};

