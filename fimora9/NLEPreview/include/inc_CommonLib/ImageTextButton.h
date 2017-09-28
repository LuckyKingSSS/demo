#ifndef HORVEREVENT_H
#define HORVEREVENT_H

#include <QtWidgets/QWidget>

#include "inc_CommonLib/CommonWidgets.h"

class COMMONLIB_EXPORT ImageTextButton : public QWidget
{
	Q_OBJECT
public:
	enum ImageAlign{Left, Top, Right, Bottom};
public:
	ImageTextButton(int id, const QString &objectname = "", QWidget *parent = nullptr);
	void SetText(QString text);
	void SetImageAlign(int align);
	void SetTextAlign(Qt::Alignment align);
	void SetImageWidth(int width);

	~ImageTextButton();
Q_SIGNALS:
	void Clicked(int id, QPoint p);
protected:
	void enterEvent(QEvent *event);
	void leaveEvent(QEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void resizeEvent(QResizeEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);

protected:
	QLabel *m_lbText;
	BaseButton *m_btnImage;
	int m_imageAlign;
	int m_imageWidth;
	int m_textAlign;
};

//////////////////////////////////////////////////////////////////////////
class COMMONLIB_EXPORT ImageTextButton2 : public QWidget
{
	Q_OBJECT
	Q_PROPERTY(QPixmap normalImage READ GetNormalImage WRITE SetNormalImage)
	Q_PROPERTY(QPixmap selectedImage READ GetSelectedImage WRITE SetSelectedImage)
	Q_PROPERTY(QColor normalTextColor READ GetNormalTextColor WRITE SetNormalTextColor)
	Q_PROPERTY(QColor selectedTextColor READ GetSelectedTextColor WRITE SetSelectedTextColor)
public:
	enum BUTTONSTATE
	{
		BUTTONSTATE_NORMAL = 0,
		BUTTONSTATE_SELECTED,
	};

	ImageTextButton2(int id,const QString &objName ,QWidget *parent = nullptr);
	~ImageTextButton2();

	void SetText(const QString &text);
	QString GetText();
	void SetButtonState(BUTTONSTATE s);
	int GetId();


	QPixmap GetNormalImage();
	void SetNormalImage(QPixmap px);
	QPixmap GetSelectedImage();
	void  SetSelectedImage(QPixmap px);
	QColor GetNormalTextColor();
	void SetNormalTextColor(QColor clr);
	QColor GetSelectedTextColor();
	void SetSelectedTextColor(QColor clr);

Q_SIGNALS:
	void sigSelected(int id);
protected:
	void mousePressEvent(QMouseEvent *) Q_DECL_OVERRIDE;
	void paintEvent(QPaintEvent *e) Q_DECL_OVERRIDE;
protected:
	QPixmap normalImage;
	QPixmap selectedImage;
	QColor normalTextColor;
	QColor selectedTextColor;

	QString m_text;
	BUTTONSTATE m_bState;
	int m_id;
};



#endif // HORVEREVENT_H
