#include "stdafx.h"
#include "FAudioMixerBaseWidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

/*FColorGradientSlider*/
FColorGradientSlider::FColorGradientSlider(Qt::Orientation orientation /*= Qt::Horizontal*/, QWidget *parent /*= 0*/)
	: FBaseWidget(parent)
	, m_orientation(orientation)
	, m_mousePressEnabled(false)
	, m_minValue(0)
	, m_maxValue(100)
	, m_curValue(0)
{
	setAttribute(Qt::WA_StyledBackground);
}

FColorGradientSlider::~FColorGradientSlider()
{

}

void FColorGradientSlider::SetMousePressEnabled(bool enabled)
{
	m_mousePressEnabled = enabled;
}

void FColorGradientSlider::SetRange(float min, float max)
{
	if (max <= min) return;
	m_minValue = min;
	m_maxValue = max;
}

void FColorGradientSlider::SetValue(float value)
{
	m_curValue = value;
	paintEvent(nullptr);
}

void FColorGradientSlider::paintEvent(QPaintEvent *)
{
	QPainter p(this);
	if (m_orientation == Qt::Horizontal){
		QLinearGradient linear(QPointF(0, 0), QPointF(width(), 0));
		linear.setColorAt(0, Qt::black);
		linear.setColorAt(0.5, Qt::yellow);
		linear.setColorAt(0.8, QColor(201, 153, 178));
		linear.setColorAt(1, QColor(111, 15, 252));
		QBrush brush(linear);
		p.fillRect(QRect(0, 0, GetLen(), height()), brush);
	}
	else{
		QLinearGradient linear(QPointF(0, height()), QPointF(0, 0));
		linear.setColorAt(0, Qt::black);
		linear.setColorAt(0.5, Qt::yellow);
		linear.setColorAt(0.8, QColor(201, 153, 178));
		linear.setColorAt(1, QColor(111, 15, 252));
		QBrush brush(linear);
		p.fillRect(QRect(0, height(), width(), GetLen()), brush);
	}
}

void FColorGradientSlider::mousePressEvent(QMouseEvent *event)
{
	if (m_mousePressEnabled){
		QPoint pos = event->pos();
		if (m_orientation == Qt::Horizontal){
			m_curValue = 1.0 * pos.x() / width() * (m_maxValue - m_minValue);
		}
		else{
			m_curValue = (m_maxValue - m_minValue) - 1.0 * pos.y() / height() * (m_maxValue - m_minValue);
		}
		paintEvent(nullptr);
	}
}

float FColorGradientSlider::GetLen()
{
	if (m_orientation == Qt::Horizontal){
		return m_curValue / (m_maxValue - m_minValue) * width();
	}
	else{
		return m_curValue / (m_maxValue - m_minValue) * height();
	}
}


/*FSoundChannelPanel*/
FSoundChannelPanel::FSoundChannelPanel(QWidget *parent /*= 0*/)
	: FBaseWidget(parent)
	, m_soundChannel(None_Channel)
	, m_bPressed(false)
	, m_curValue(0)
	, m_drag(0, 0)
{
	m_pLeftLbl = new FLabel(this);
	m_pLeftLbl->setAlignment(Qt::AlignLeft | Qt::AlignCenter);
	m_pLeftLbl->setText("L");
	m_pRightLbl = new FLabel(this);
	m_pRightLbl->setAlignment(Qt::AlignRight | Qt::AlignCenter);
	m_pRightLbl->setText("R");
}

FSoundChannelPanel::~FSoundChannelPanel()
{
	
}

void FSoundChannelPanel::SetValue(float value)
{
	m_curValue = value;
	float R = sqrt(qPow(m_drag.x() - m_center.x(), 2) + qPow(m_drag.y() - m_center.y(), 2));   // 活动圆与大圆圆心的距离 
	if (m_curValue == 0){
		m_soundChannel = None_Channel;
	}
	else if (m_curValue < 0){
		m_soundChannel = Left_Channel;
		m_drag.setX(m_center.x() - R*qSin(qFabs(m_curValue) / 100 * (2 / 3 * qAcos(-1))));
		m_drag.setY(m_center.y() - R*qCos(qFabs(m_curValue) / 100 * (2 / 3 * qAcos(-1))));
	}
	else {
		m_soundChannel = Right_Channel;
		m_drag.setX(m_center.x() + R*qSin(m_curValue / 100 * (2 / 3 * qAcos(-1))));
		m_drag.setY(m_center.y() - R*qCos(m_curValue / 100 * (2 / 3 * qAcos(-1))));
	}
	update();
}

float FSoundChannelPanel::GetValue()
{
	return m_curValue;
}

void FSoundChannelPanel::mousePressEvent(QMouseEvent *event)
{
	QPoint p = event->pos();
	float dx, dy;
	dx = p.x() - m_drag.x();
	dy = p.y() - m_drag.y();
	if (dx*dx + dy*dy <= 16) m_bPressed = true;
}

void FSoundChannelPanel::mouseMoveEvent(QMouseEvent *event)
{
	if (m_bPressed){
		QPoint p = event->pos();
		float r = sqrt(qPow(p.x() - m_center.x(), 2) + qPow(p.y() - m_center.y(), 2));  // 鼠标拖动点与大圆圆心的距离
		float y = m_center.y() + (m_centerRadius - 9)*(p.y() - m_center.y()) / r;
		float x = m_center.x() + (m_centerRadius - 9)*(p.x() - m_center.x()) / r;
		float R = sqrt(qPow(x - m_center.x(), 2) + qPow(y - m_center.y(), 2));   // 活动圆与大圆圆心的距离 
		if (y - m_center.y() <= R / 2){
			m_drag.setX(x);
			m_drag.setY(y);
			if ((x > m_center.x() - 0.000002) && (x < m_center.x() + 0.000002)) {
				m_soundChannel = None_Channel;
				m_curValue = 0;
			}
			else if (x < m_center.x()) {
				m_soundChannel = Left_Channel;
				m_curValue = -qAcos((m_center.y() - y) / R) / (qAcos(-1) * 2 / 3) * 100;  // qAcos(-1) => pie
			}
			else {
				m_soundChannel = Right_Channel;
				qDebug() << "1= " << qAcos((m_center.y() - y) / R);
				m_curValue = qAcos((m_center.y() - y) / R) / (qAcos(-1) * 2 / 3) * 100;
				qDebug() << "[mouseMoveEvent]= " << m_curValue;
			}
			emit sigValueChanged(m_curValue);
			update();
		}
	}
}

void FSoundChannelPanel::mouseReleaseEvent(QMouseEvent *)
{
	m_bPressed = false;
}

void FSoundChannelPanel::resizeEvent(QResizeEvent *)
{
	m_pLeftLbl->setGeometry(0, 0, 10, height());
	m_pRightLbl->setGeometry(width() - 10, 0, 10, height());
	QPointF oldCenter = m_center;
	m_center = QPointF(1.0 * width() / 2, 1.0 * height() / 2);
	float oldRadius = m_centerRadius;
	m_centerRadius = qMin(1.0 * (width() - 20) / 2, 1.0 * height() / 2);
	// 重新确定活动圆的圆心
	m_drag.setX(m_center.x() + (m_centerRadius - 9)*(m_drag.x() - oldCenter.x()) / (oldRadius - 9));
	m_drag.setY(m_center.y() + (m_centerRadius - 9)*(m_drag.y() - oldCenter.y()) / (oldRadius - 9));
}

void FSoundChannelPanel::paintEvent(QPaintEvent *)
{
	QPainter p(this);
	// 反走样
	p.setRenderHint(QPainter::Antialiasing, true);
	// 画最外层紫色圆
	p.setPen(QPen(QColor(132, 127, 234), 12));
	p.drawEllipse(m_center, m_centerRadius - 10, m_centerRadius - 10);
	// 设置画笔颜色、宽度
	p.setPen(QPen(QColor(22, 34, 46), 4));
	// 设置画刷颜色
	p.setBrush(QColor(190, 197, 203));
	// 绘圆
	p.drawEllipse(m_center, m_centerRadius - 15, m_centerRadius - 15);
	// 画活动小圆
	p.setPen(QPen(QColor(22, 34, 46), 2));
	p.drawEllipse(m_drag, 3, 3);
	// 绘制圆弧
	DrawArc(&p);
}

void FSoundChannelPanel::showEvent(QShowEvent *)
{
	if (m_soundChannel == None_Channel){
		m_drag.setX(m_center.x());
		m_drag.setY(m_centerRadius - 14);
	}
	else if (m_soundChannel == Left_Channel) {
		m_drag.setX(m_center.x() - (m_centerRadius - 9)*qSin(qFabs(m_curValue) / 100 * (2 / 3 * qAcos(-1))));
		m_drag.setY(m_center.y() - (m_centerRadius - 9)*qCos(qFabs(m_curValue) / 100 * (2 / 3 * qAcos(-1))));
	}
	else if (m_soundChannel == Right_Channel){
		m_drag.setX(m_center.x() + (m_centerRadius - 9)*qSin(m_curValue / 100 * (2 / 3 * qAcos(-1))));
		m_drag.setY(m_center.y() - (m_centerRadius - 9)*qCos(m_curValue / 100 * (2 / 3 * qAcos(-1))));
	}
	update();
}

void FSoundChannelPanel::DrawArc(QPainter *p)
{
	// p->setPen(QPen(QColor(125, 140, 156), 5));
	p->setPen(QPen(Qt::red, 5));
	float R = sqrt(qPow(m_drag.x() - m_center.x(), 2) + qPow(m_drag.y() - m_center.y(), 2));   // 活动圆与大圆圆心的距离 
	QRectF rectangle(m_center.x() - R, m_center.y() - R, R * 2, R * 2);
	if (m_soundChannel == Left_Channel){
		int startAngle = qFabs(m_curValue) / 100 * 120;
		p->drawArc(rectangle, 90 * 16, startAngle * 16);
	}
	else if (m_soundChannel == Right_Channel){
		int startAngle = qFabs(m_curValue) / 100 * 120;
		qDebug() << "m_curValue= " << m_curValue;
		p->drawArc(rectangle, (90 - startAngle) * 16, startAngle * 16);
	}
}

/*FMarkSlider*/
FMarkSlider::FMarkSlider(QWidget *parent /*= 0*/)
	: FBaseWidget(parent)
	, m_curValue(0)
{
	// 从下到上 0.10,0.12,0.16,0.08,0.11,0.13,0.15,0.15
	m_scale[0] = 0.10;
	m_scale[1] = 0.12;
	m_scale[2] = 0.16;
	m_scale[3] = 0.08;
	m_scale[4] = 0.11;
	m_scale[5] = 0.13;
	m_scale[6] = 0.15;
	m_scale[7] = 0.15;
	m_pSlider = new ToolBarSlider(this, Qt::Vertical);
	m_pSlider->setDuration(1000);
	m_pSlider->SetChunkHeight(3);
	CalculateMarkValueInner();
	connect(m_pSlider, &ToolBarSlider::valueChanged, this, &FMarkSlider::slotValueChanged);
}

FMarkSlider::~FMarkSlider()
{

}

void FMarkSlider::SetMarkValue(float *pValues)
{
	for (int i = 0; i < 9; ++i){
		m_markValue[i] = pValues[i];
	}
}

void FMarkSlider::SetValue(float value)
{
	float v;
	m_curValue = qBound(m_markValue[0], value, m_markValue[8]);
	int i = 8;
	for (; i >= 0; --i){
		if (m_curValue > m_markValue[i]) break;
	}
	// 外部值转化为内部值
	v = m_markValueInner[i] + (m_curValue - m_markValue[i]) / (m_markValue[i + 1] - m_markValue[i]) * (m_markValueInner[i + 1] - m_markValueInner[i]);
	m_pSlider->setValue(v);
}

float FMarkSlider::GetValue()
{
	return m_curValue;
}

void FMarkSlider::SetSliderObjectName(QString name)
{
	m_pSlider->setObjectName(name);
}

void FMarkSlider::resizeEvent(QResizeEvent *)
{
	int left, top;
	const int SliderWidth = 5;
	left = width() - SliderWidth;
	top = 0;
	m_pSlider->setGeometry(left, top, SliderWidth, height());
}

void FMarkSlider::paintEvent(QPaintEvent *)
{
	QPainter p(this);
	float scale = 0;
	float h = height() - 1;
	p.drawLine(QPointF(0, h), QPointF(width() - 3, h));
	for (auto s : m_scale){
		scale += s;
		h = (1 - scale) * height();
		p.drawLine(QPointF(0, h), QPointF(width() - 3, h));
	}
}

void FMarkSlider::CalculateMarkValueInner()
{
	m_markValueInner[0] = 0;
	float scale = 0;
	for (int i = 1; i <= 8; ++i){
		scale += m_scale[i - 1];
		m_markValueInner[i] = 1000 * scale;
	}
}

void FMarkSlider::slotValueChanged(float value)
{
	int i = 8;
	for (; i >= 0; --i){
		if (value >= m_markValueInner[i]) break;
	}
	// 内部值转化为外部值
	m_curValue = m_markValue[i] + (value - m_markValueInner[i]) / (m_markValueInner[i + 1] - m_markValueInner[i]) * (m_markValue[i + 1] - m_markValue[i]);
	emit sigValueChanged(m_curValue);
}


/*FMarkSliderRuler*/
FMarkSliderRuler::FMarkSliderRuler(float *pValues /*= 0*/, QWidget *parent /*= 0*/)
	:FBaseWidget(parent)
{
	// 从下到上 0.10,0.12,0.16,0.08,0.11,0.13,0.15,0.15
	m_scales[0] = 0.10;
	m_scales[1] = 0.12;
	m_scales[2] = 0.16;
	m_scales[3] = 0.08;
	m_scales[4] = 0.11;
	m_scales[5] = 0.13;
	m_scales[6] = 0.15;
	m_scales[7] = 0.15;
	FLabel *pLable = 0;
	for (int i = 0; i < 9; ++i){
		m_values[i] = pValues[i];
		m_lstLbls.append(pLable =  new FLabel(this));
		pLable->setAlignment(Qt::AlignRight);
		pLable->setText(QString::number(m_values[i]));
	}
}

FMarkSliderRuler::~FMarkSliderRuler()
{

}

void FMarkSliderRuler::resizeEvent(QResizeEvent *)
{
	float scale = 0;
	float h = height() - 1;
	m_lstLbls[0]->setGeometry(0, h - 10, 20, 20);
	for (int i = 1; i < 8; ++i){
		scale += m_scales[i - 1];
		h = (1 - scale) * height();
		m_lstLbls[i]->setGeometry(0, h - 5, 20, 20);
	}
	h = height() - 1;
	m_lstLbls[8]->setGeometry(0, 0, 20, 20);
}

void FMarkSliderRuler::paintEvent(QPaintEvent *)
{
	QPainter p(this);
	float scale = 0;
	float h = height() - 1;
	p.drawLine(QPointF(width() - 7, h), QPointF(width(), h));
	for (int i = 0; i < 8; ++i){
		scale += m_scales[i];
		h = (1 - scale) * height();
		p.drawLine(QPointF(width() -7, h), QPointF(width(), h));
	}
}


/*FAudioTrack*/
FAudioTrack::FAudioTrack(QWidget *parent)
	: FBaseWidget(parent)
{
	m_pTextLbl = new FLabel(this);
	m_pTextLbl->setAlignment(Qt::AlignCenter);
	m_pImageLbl = new FLabel(this);
	m_pSoundChannelPanel = new FSoundChannelPanel(this);
	m_pLineEdit = new FLineEdit(this);
	m_pMarkSlider = new FMarkSlider(this);
	m_pMarkSlider->SetSliderObjectName("FVideoPanel_Slider");
	m_pLeftAudioTrack = new FColorGradientSlider(Qt::Vertical, this);
	m_pLeftAudioTrack->setObjectName("FAudioTrack");
	m_pLeftAudioTrack->SetMousePressEnabled(true);
	m_pRightAudioTrack = new FColorGradientSlider(Qt::Vertical, this);
	m_pRightAudioTrack->setObjectName("FAudioTrack");
	m_pRightAudioTrack->SetMousePressEnabled(true);

	m_pLineEdit->setText(QString::number(m_pMarkSlider->GetValue(), 'f', 1));
	ConnectSignals();
}

FAudioTrack::~FAudioTrack()
{

}

void FAudioTrack::SetText(QString name)
{
	m_pTextLbl->setText(name);
}

void FAudioTrack::SetMarkValue(float *pValue)
{
	m_pMarkSlider->SetMarkValue(pValue);
}

void FAudioTrack::SetValue(float value)
{
	m_pMarkSlider->SetValue(value);
}

void FAudioTrack::SetImageLabelObjectName(QString name)
{
	m_pImageLbl->setObjectName(name);
}

int FAudioTrack::GetMarkSliderHeight()
{
	return m_pMarkSlider->height();
}

void FAudioTrack::resizeEvent(QResizeEvent *)
{
	int left, top;
	left = top = 0;
	m_pTextLbl->setGeometry(left, top, width(), 25);
	top = m_pTextLbl->geometry().bottom() + 1;
	m_pImageLbl->setGeometry(left, top, width(), 40);
	top = m_pImageLbl->geometry().bottom() + 5;
	m_pSoundChannelPanel->setGeometry(left, top, width(), width());
	top = m_pSoundChannelPanel->geometry().bottom() + 5;
	int w = (width() - 30) ? (width() - 30) : 30;
	m_pLineEdit->setGeometry(left + 10, top, w, 20);
	top = m_pLineEdit->geometry().bottom() + 5;
	left = 10;
	m_pMarkSlider->setGeometry(left, top, 10, height() - top);
	left = m_pMarkSlider->geometry().right() + 20;
	m_pLeftAudioTrack->setGeometry(left, top, 8, height() - top);
	left += 3 + 8;
	m_pRightAudioTrack->setGeometry(left, top, 8, height() - top);
}

void FAudioTrack::ConnectSignals()
{
	connect(m_pMarkSlider, &FMarkSlider::sigValueChanged, this, [&](float value){
		m_pLineEdit->setText(QString::number(value, 'f', 1));
	});
	connect(m_pLineEdit, &FLineEdit::editingFinished, this, [&](){
		float value = m_pLineEdit->text().toFloat();
		m_pMarkSlider->SetValue(value);
		emit m_pMarkSlider->sigValueChanged(m_pMarkSlider->GetValue());
	});
}















