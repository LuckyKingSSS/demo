#include "stdafx.h"

#include "inc_FSCommonlib/VolumeSlider.h"

#include "VolumeSlider_p.h"
#include "CommonWidgets.h"

VolumeSlider::VolumeSlider(QWidget *parent)
: QWidget(parent)
{

	m_value = 0;
	m_min = -51;
	m_max = 100;
	m_position = 0;
	m_chunkWidth = 8;
	m_grooveHeight = 2;

	m_horizonal = false;
	resize(20, 100);

}



void VolumeSlider::SetValue(int value)
{
	m_value = qBound(m_min, value, m_max);
	if (m_horizonal)
		m_position = 1.0 * (m_value - m_min) / (m_max - m_min) * GetWidth();
	else
		m_position = GetWidth() -  1.0 * (m_value - m_min) / (m_max - m_min) * GetWidth();

	update();
}

void VolumeSlider::SetRange(int minvalue, int maxvalue)
{
	Q_ASSERT(maxvalue > minvalue);
	m_min = minvalue;
	m_max = maxvalue;
}

void VolumeSlider::GetRange(int &minvalue, int &maxvalue)
{
	minvalue = m_min;
	maxvalue = m_max;
}

int VolumeSlider::GetValue()
{
	return m_value;
}

void VolumeSlider::SetChunkWidth(int w)
{
	Q_ASSERT(w > 0);
	m_chunkWidth = w;
}

void VolumeSlider::SetGrooveHeight(int h)
{
	Q_ASSERT(h > 0);
	m_grooveHeight = h;
}

void VolumeSlider::SetHorizontal(bool horizontal)
{
	m_horizonal = horizontal;
}

bool VolumeSlider::GetHorizontal()
{
	return m_horizonal;
}

void VolumeSlider::DoModal(QPoint p)
{
	//if (m_show)
	//{
	//	hide();
	//}
	//else
	//{
	//	m_show = true;

		p.setY(p.y() - height());
		move(p);
		show();

	//}
}

int VolumeSlider::GetWidth()
{
	if (m_horizonal)
		return width() - m_chunkWidth;
	else
		return height() - m_chunkWidth;
}

void VolumeSlider::resizeEvent(QResizeEvent *event)
{
	SetValue(m_value);
}

void VolumeSlider::mousePressEvent(QMouseEvent *event)
{
	int value = CalculateValue(event->pos());
	if (value != m_value)
	{
		m_value = value;
		emit ValueChanged(m_value);
	}
	update();
}

//void Slider::mouseReleaseEvent(QMouseEvent *event)
//{
//}
//
void VolumeSlider::mouseMoveEvent(QMouseEvent *event)
{
	mousePressEvent(event);

}

void VolumeSlider::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Left)
	{
		SetValue(GetValue() - 1);
	}
	else if (event->key() == Qt::Key_Right)
	{
		SetValue(GetValue() + 1);

	}
}

void VolumeSlider::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	int left = 0;
	int top = (height() - m_grooveHeight) / 2;
	int h = m_grooveHeight;

	//painter.fillRect(rect(), Qt::black);
	if (m_horizonal)
	{
		painter.fillRect(QRect(left , top, width(), h), Qt::gray);
		painter.fillRect(QRect(left, top, m_position, h), Qt::black);
		painter.fillRect(QRect(m_position, (height() - m_chunkWidth) / 2, m_chunkWidth, m_chunkWidth), Qt::black);
	}
	else
	{
		painter.setBrush(QColor(0, 0, 0));
		left = (width() - m_grooveHeight) / 2;
		painter.fillRect(QRect(left, 0, m_grooveHeight, height()), Qt::gray);
		painter.fillRect(QRect(left, m_position, m_grooveHeight, height() - m_position), Qt::black);
		painter.fillRect(QRect((width() - m_chunkWidth) / 2, m_position, m_chunkWidth, m_chunkWidth), Qt::black);
	}

}

int VolumeSlider::CalculateValue(QPoint p)
{
	int position = m_horizonal ? p.x() :  p.y();
	m_position = qBound(0, position - m_chunkWidth / 2, GetWidth());
	int value = 0;
	if (m_horizonal)
	{
		value = qBound<int>(m_min, 1.0 * m_position / GetWidth() * (m_max - m_min) + m_min, m_max);
	}
	else
	{
		value = qBound<int>(m_min, 1.0 * (GetWidth() - m_position) / GetWidth() * (m_max - m_min) + m_min, m_max);
	}
	return value;
}

int VolumeSlider::CalculatePosition(int value)
{
	m_position = (float)value / (m_max - m_min) * GetWidth();

	return m_position;
}


//////////////////////////////////////////////////////////////////////////
AudioVolumeSlider::AudioVolumeSlider(QWidget *parent)
:QDialog(parent)
{
	setWindowFlags(Qt::Popup);
	setObjectName("AudioVolumeSliderDialog");
	m_show = false;
	m_slider = new ToolBarSlider(this, Qt::Vertical);
	m_slider->setObjectName("myVolueSlider");
	m_slider->setDuration(200.00);
	m_slider->setValue(100);
	resize(20, 100);
	connect(m_slider, &ToolBarSlider::valueChanged, this, &AudioVolumeSlider::ValueChanged);
}

void AudioVolumeSlider::SetValue(int value)
{
	m_slider->setValue(value);
}

int AudioVolumeSlider::GetValue()
{
	return m_slider->value();
}

void AudioVolumeSlider::SetRange(int min, int max)
{
    m_slider->setDuration(max - min);
}

void AudioVolumeSlider::SetHorizontal(bool horizontal)
{
	if (horizontal)
	{
		m_slider->close();
		m_slider = new ToolBarSlider(this, Qt::Horizontal);
		m_slider->setObjectName("myVolueSlider");
		m_slider->setDuration(200.00);
		m_slider->setValue(100.00);
		//重新设置大小
		resize(100, 20);
	}
	else
	{
		m_slider->close();
		m_slider = new ToolBarSlider(this, Qt::Vertical);
		m_slider->setObjectName("myVolueSlider");
		m_slider->setDuration(200.00);
		m_slider->setValue(100.00);
	}
}

void AudioVolumeSlider::DoModal(QPoint p)
{
	p.setY(p.y() - height());
	move(p);
	show();
}

void AudioVolumeSlider::resizeEvent(QResizeEvent *event)
{
	const int space = 5;
	m_slider->setGeometry(space, space, width() - 2 * space, height() - 2 * space);
	m_slider->SetChunkHeight(width() - 2 * space);
}