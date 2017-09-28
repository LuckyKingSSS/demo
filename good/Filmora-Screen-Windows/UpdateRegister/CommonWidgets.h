#ifndef COMMONWIDGETS_H
#define COMMONWIDGETS_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>

//图按钮
class ImageButton :public QWidget
{
	Q_OBJECT

public:
	ImageButton(QWidget *parent = 0);

protected:
	void mousePressEvent(QMouseEvent *) override;

signals:
	void Clicked();
};

/*URBaseWidget*/
class URBaseWidget : public QWidget
{
	Q_OBJECT

public:
	URBaseWidget(QWidget *parent);

protected:
	bool event(QEvent *e) override;
	virtual void RetranslateUi();
};

/*TitleBar*/
class TitleBar : public URBaseWidget
{
	Q_OBJECT

public:
	TitleBar(QWidget *parent = 0);
	//设置Title
	void SetTitle(QString title);

protected:
	void resizeEvent(QResizeEvent *) override;
	void mousePressEvent(QMouseEvent *) override;
	void mouseMoveEvent(QMouseEvent *) override;

signals:
	void Close();
	void Move(int x, int y);

private:
	QLabel *m_label;
	ImageButton *m_closeBtn;
	QPoint m_pressPoint;
	QWidget *m_parent;
};

//消息框
class URMessageBox : public QDialog  // UpdateRegisterMessageBox
{
	Q_OBJECT

public:
	enum ButtonMode{OkMode, YesCancelMode};

public:
	URMessageBox(ButtonMode mode = OkMode, QWidget *parent = 0);
	void Information(QString title, QString message);

signals:
	void Close();
	void Ok();
	void Yes();
	void Cancel();

protected:
	void resizeEvent(QResizeEvent *) override;
	bool event(QEvent*) override;
	virtual void RetranslateUi();

private:
	ButtonMode m_mode;
	TitleBar *m_titleBar;
	QPushButton *m_okBtn;
	QPushButton *m_yesBtn;
	QPushButton *m_cancelBtn;
	QLabel *m_label;
	QWidget *m_parent;
};



#endif // COMMONWIDGETS_H
