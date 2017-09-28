#include "stdafx.h"
#include "imagetextbutton.h"

ImageTextButton::ImageTextButton(int id, const QString &objectname , QWidget *parent)
: QWidget(parent)
{
	resize(300, 300);
	m_imageAlign = Left;
	m_imageWidth = 0;
	
	m_btnImage = new BaseButton(id, objectname, this);
	connect(m_btnImage, &BaseButton::Clicked, this, &ImageTextButton::Clicked);

	m_lbText = new QLabel(this);
	m_lbText->setObjectName("ImageTextButton_Text");

	SetImageAlign(Left);
}

void ImageTextButton::SetText(QString text)
{
	m_lbText->setText(text);
}

void ImageTextButton::SetImageAlign(int align)
{
	m_imageAlign = align;
	if (m_imageAlign == Left || m_imageAlign == Right)
	{
		m_lbText->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	}
	else
	{
		m_lbText->setAlignment(Qt::AlignCenter);

	}


	if (isVisible())
		resizeEvent(nullptr);
}

void ImageTextButton::SetTextAlign(Qt::Alignment align)
{
	m_lbText->setAlignment(align);
}

void ImageTextButton::SetImageWidth(int width)
{
	m_imageWidth  = width;
}

ImageTextButton::~ImageTextButton()
{

}

void ImageTextButton::enterEvent(QEvent *event)
{
	QEvent hoverEvent = QEvent(QEvent::Enter);
	QApplication::sendEvent(m_lbText, &hoverEvent);
	m_lbText->update();

	QWidget::enterEvent(event);
}

void ImageTextButton::leaveEvent(QEvent *event)
{
	QEvent hoverEvent = QEvent(QEvent::Leave);
	QApplication::sendEvent(m_lbText, &hoverEvent);
	m_lbText->update();

	QWidget::leaveEvent(event);

}

void ImageTextButton::mouseMoveEvent(QMouseEvent *event)
{

}

void ImageTextButton::resizeEvent(QResizeEvent *event)
{
	int textheight = 22;
	int left = 0;
	int top = 0;
	if (m_imageAlign == Left)
	{
		m_btnImage->setGeometry(left, top, height(), height());
		left = m_btnImage->geometry().right();
		m_lbText->setGeometry(left, top, width() - left, height());
	}
	else if (m_imageAlign == Top)
	{
		m_btnImage->setGeometry(left, top, width(), width());
		top = m_btnImage->geometry().bottom();
		m_lbText->setGeometry(left, top, width(), height() - top);
	}
	else if (m_imageAlign == Right)
	{
		int w = width() - height();
		m_lbText->setGeometry(left, top, w, height());
		left = m_lbText->geometry().right();
		m_btnImage->setGeometry(left, top, height(), height());

	}
	else if (m_imageAlign == Bottom)
	{
		int h = height() - width();
		m_lbText->setGeometry(left, top, width(), h);
		top = m_lbText->geometry().bottom();
		m_btnImage->setGeometry(left, top, width(), width());
	}
}

void ImageTextButton::mouseReleaseEvent(QMouseEvent *event)
{
	if (rect().contains(mapFromGlobal(QCursor::pos())))
		emit Clicked(m_btnImage->GetId(), mapToGlobal(QPoint(0, 0)));

	QWidget::mouseReleaseEvent(event);
}

//////////////////////////////////////////////////////////////////////////

ImageTextButton2::ImageTextButton2(int id, const QString &objName, QWidget *parent)
:QWidget(parent)
{
	m_bState = BUTTONSTATE_NORMAL;
	m_id = id;
	setObjectName(objName);
}

ImageTextButton2::~ImageTextButton2()
{

}

void ImageTextButton2::SetText(const QString &text)
{
	m_text = text;
}

QString ImageTextButton2::GetText()
{
	return m_text;
}

void ImageTextButton2::SetButtonState(BUTTONSTATE s)
{
	m_bState = s;
	update();
}

int ImageTextButton2::GetId()
{
	return m_id;
}

QPixmap ImageTextButton2::GetNormalImage()
{
	return normalImage;
}

void ImageTextButton2::SetNormalImage(QPixmap px)
{
	normalImage = px;
}

QPixmap ImageTextButton2::GetSelectedImage()
{
	return selectedImage;
}

void ImageTextButton2::SetSelectedImage(QPixmap px)
{
	selectedImage = px;
}

QColor ImageTextButton2::GetNormalTextColor()
{
	return normalTextColor;
}

void ImageTextButton2::SetNormalTextColor(QColor clr)
{
	normalTextColor = clr;
}

QColor ImageTextButton2::GetSelectedTextColor()
{
	return selectedTextColor;
}

void ImageTextButton2::SetSelectedTextColor(QColor clr)
{
	selectedTextColor = clr;
}

void ImageTextButton2::mousePressEvent(QMouseEvent *)
{
	m_bState = BUTTONSTATE_SELECTED;
	emit sigSelected(m_id);
	update();
}

void ImageTextButton2::paintEvent(QPaintEvent *e)
{
	static const int TextH = 20;
	QPainter painter(this);
	QPixmap pix;
	QColor clr;

	if (m_bState == BUTTONSTATE_NORMAL)
	{
		pix = normalImage;
		clr = normalTextColor;
	}
	else if (m_bState == BUTTONSTATE_SELECTED)
	{
		pix = selectedImage;
		clr = selectedTextColor;
	}

	QRect rcAll = rect();

	int l = (rcAll.width() - pix.width()) * 0.5;
	int t = (rcAll.height() - pix.height() - TextH) * 0.5;

	QRect rcImg(l, t, pix.width(), pix.height());
	QRect rcText(0, rcImg.bottom() + 10, rcAll.width(), TextH);

	painter.drawPixmap(rcImg,pix);
	
	painter.setPen(QPen(clr));
	QTextOption textOp(Qt::AlignTop | Qt::AlignHCenter);
	painter.drawText(rcText, m_text, textOp);
}

