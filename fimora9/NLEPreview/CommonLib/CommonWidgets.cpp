
#include "inc_CommonLib/CommonWidgets.h"
#include "inc_CommonLib/externalapi.h"
#include "inc_CommonLib/StringOperation.h"

#include <QtGui/QMouseEvent>
#include <QtWidgets/QStackedWidget>
#include <QPainter>
#include <QtWidgets/QLabel>
#include <QDebug>
BaseButton::BaseButton(int id, const QString &objectname, QWidget *parent)
: QWidget(parent)
, m_id(id)
{
	setAttribute(Qt::WA_StyledBackground);
	setObjectName(objectname);
	setFocusPolicy(Qt::NoFocus);
	connect(this, &BaseButton::clicked, this, [&](){
		emit Clicked(m_id, mapToGlobal(QPoint(0, 0)));
	});
}

BaseButton::BaseButton(const QString &objectname, QWidget *parent)
:QWidget(parent)
, m_id(0)
{
	setAttribute(Qt::WA_StyledBackground);
	setObjectName(objectname);
	setFocusPolicy(Qt::NoFocus);
	connect(this, &BaseButton::clicked, this, [&](){
		emit Clicked(m_id, mapToGlobal(QPoint(0, 0)));
	});
}

void BaseButton::SetEnabled(bool enable)
{
	setEnabled(enable);
}
int BaseButton::GetId()
{ 
	return m_id; 
}

void BaseButton::mousePressEvent(QMouseEvent *event)
{
	event->accept();
    emit Pressed(m_id, mapToGlobal(QPoint(0, 0)));
}

void BaseButton::mouseReleaseEvent(QMouseEvent *event)
{
	if (rect().contains(mapFromGlobal(QCursor::pos())) && event->button() == Qt::LeftButton)
		emit clicked();
}


//////////////////////////////////////////////////////////////////////////


BaseStateButton::BaseStateButton(int id, const QString &objectname, QWidget *parent)
:BaseButton(id, objectname, parent)
{
	setAttribute(Qt::WA_StyledBackground);
	setStyleSheet("BaseStateButton{background-color:rgb(0,0,0, 0)}");
	m_curIndex = 0;
	//m_stacked = new QStackedWidget(this);
}

void BaseStateButton::AddButton(int id, const QString &objectName /*= ""*/)
{
	auto btn = new BaseButton(id, objectName, this);
	connect(btn, &BaseButton::Clicked, this, [&](int id){
		++m_curIndex;
		if (m_curIndex >= m_btns.size())
			m_curIndex = 0;
		resizeEvent(nullptr);
		emit Clicked(id, mapToGlobal(QPoint(0, 0)));
	});

	m_btns.push_back(btn);

}

void BaseStateButton::AddButton(BaseButton *button)
{
	Q_ASSERT(button);
	button->setParent(this);
	connect(button, &BaseButton::Clicked, this, [&](int id){
		++m_curIndex;
		if (m_curIndex >= m_btns.size())
			m_curIndex = 0;
		resizeEvent(nullptr);
		emit Clicked(id, mapToGlobal(QPoint(0, 0)));
	});

	m_btns.push_back(button);
}

//void BaseStateButton::AddFirstButton(int id, const QString &objectName /*= ""*/)
//{
//	Q_ASSERT(m_secondBtn);
//	m_firstBtn = new BaseButton(id, objectName);
//	connect(m_firstBtn, &BaseButton::Clicked, this, [&](int id){
//		//m_stacked->setCurrentIndex((m_stacked->currentIndex() + 1) % m_stacked->count());
//		emit Clicked(id, mapToGlobal(QPoint(0, 0)));
//	});
//}
//
//void BaseStateButton::AddSecondButton(int id, const QString &objectName /*= ""*/)
//{
//	Q_ASSERT(m_secondBtn);
//	m_secondBtn = new BaseButton(id, objectName);
//	connect(m_secondBtn, &BaseButton::Clicked, this, [&](int id){
//		//m_stacked->setCurrentIndex((m_stacked->currentIndex() + 1) % m_stacked->count());
//		emit Clicked(id, mapToGlobal(QPoint(0, 0)));
//	});
//}

int BaseStateButton::CurrentButtonId()
{
	return m_btns.at(m_curIndex)->GetId();
}

int BaseStateButton::BaseStateButtonId()
{
	return m_id;
}

void BaseStateButton::SetCurrentButtonId(int id)
{
	m_curIndex = 0;
	for (auto btn : m_btns)
	{
		if (btn->GetId() == id)
		{
			break;
		}
		++m_curIndex;
	}
	resizeEvent(nullptr);
}

void BaseStateButton::Restore()
{
	m_curIndex = 0;
	resizeEvent(nullptr);
}

void BaseStateButton::ClickCurrentButton()
{
	auto curBtn = m_btns.at(m_curIndex);
	curBtn->Clicked(curBtn->GetId(), mapToGlobal(QPoint(0, 0)));
}

BaseButton* BaseStateButton::GetButton(int id)
{
	for (auto btn : m_btns)
	{
		if (btn->GetId() == id)
		{
			return btn;
		}
	}
	return nullptr;
}

void BaseStateButton::resizeEvent(QResizeEvent *event)
{
	for (int i = 0; i < m_btns.size(); ++i)
	{
		m_btns.at(i)->setVisible(i == m_curIndex);
		m_btns.at(i)->setGeometry(rect());
	}
}

void BaseStateButton::showEvent(QShowEvent *event)
{

}




//////////////////////////////////////////////////////////////////////////
ToolBarSlider::ToolBarSlider(QWidget *parent, Qt::Orientation orientation)
:QWidget(parent)
{
	setObjectName("CommonLib_ToolBarSlider");

	m_orientation = orientation;

	m_duration = 100;
	m_value = 0;
	m_position = 0;
	m_offset = 0;
	m_chunkWidth = 14;
	m_canSeek = true;
	m_grooveHeight = 3;

	m_bStartSeek = true;
	
	m_leftToRight = true;
	m_bottomToTop = true;

	m_bMousePress = false;

	m_bg = new QWidget(this);
	m_bg->setObjectName("CommonLib_ToolBarSlider_bg");

	m_groove = new QWidget(this);
	m_groove->setObjectName("CommonLib_ToolBarSlider_groove");


	m_chunk = new QWidget(this);
	m_chunk->setObjectName("CommonLib_ToolBarSlider_chunk");

	

	//setStyleSheet(GetCommonLibStyles());
}

void ToolBarSlider::setValue(float value, bool queueRepaint)
{
	if (canSeek())
	{
		m_value = qBound(0.0f, value, m_duration);
		m_position = 1.0 * m_value / m_duration * GetWidth();

        if (queueRepaint)
            QMetaObject::invokeMethod(this, "ResizeChunk", Qt::QueuedConnection);
        else
            ResizeChunk();
	}
}

void ToolBarSlider::setDuration(float duration)
{
	m_duration = qMax(1.0f, duration /*- 1*/);
}



float ToolBarSlider::value()
{
	return m_value;
}

float ToolBarSlider::duration()
{
	return m_duration;
}

bool ToolBarSlider::canSeek()
{
	return m_canSeek;
}
int ToolBarSlider::Height()
{
	return m_chunkWidth;
}

void	ToolBarSlider::SetChunkHeight(int height)
{
	m_chunkWidth = height;
}

void ToolBarSlider::SetGrooveHeight(int h)
{
	m_grooveHeight = h;
}

bool ToolBarSlider::ButtonPressDown()
{
	return m_bMousePress;
}

void ToolBarSlider::SetButtonPressDown(bool press)
{
	m_bMousePress = press;
}

void ToolBarSlider::SetLeftToRight(bool left)
{
	m_leftToRight = left;
}

void ToolBarSlider::SetBottomToTop(bool bottom)
{
	m_bottomToTop = bottom;
}

void ToolBarSlider::ResizeChunk()
{
	resizeEvent(nullptr);
}

void ToolBarSlider::SetCanSeek(bool can)
{
	m_canSeek = can;
}

void ToolBarSlider::resizeEvent(QResizeEvent *event)
{
	int chunktop = 0;
	if (m_orientation == Qt::Horizontal)
	{
		if (m_chunkWidth == 0)
			m_chunkWidth = height();
		chunktop = (height() - m_chunkWidth) / 2;
		if (m_leftToRight)
		{
			int bgtop = (rect().height() - m_grooveHeight) / 2;
			m_bg->setGeometry(0, bgtop, rect().width(), m_grooveHeight);
			m_groove->setGeometry(0, bgtop, qMax(1, m_position), m_grooveHeight);
			m_chunk->setGeometry(m_position, chunktop, m_chunkWidth, m_chunkWidth);
		}
		else
		{
			int bgtop = (rect().height() - m_grooveHeight) / 2;
			m_bg->setGeometry(0, bgtop, rect().width(), m_grooveHeight);

			m_groove->setGeometry(width() - m_position, bgtop, qMax(1, m_position), m_grooveHeight);
			m_chunk->setGeometry(GetWidth() - m_position, chunktop, m_chunkWidth, m_chunkWidth);
		}

	}
	else
	{
		if (m_chunkWidth == 0)
			m_chunkWidth = width();

		chunktop = (width() - m_chunkWidth) / 2;
		if (m_bottomToTop)
		{
			int bgtop = (rect().width() - m_grooveHeight) / 2;
			m_bg->setGeometry(bgtop, 0, m_grooveHeight, rect().height());

			m_groove->setGeometry(bgtop, height() - m_position, m_grooveHeight, qMax(1, m_position));
			m_chunk->setGeometry(chunktop, GetWidth() - m_position, m_chunkWidth, width());
		}
		else
		{
			int bgtop = (rect().width() - m_grooveHeight) / 2;
			m_bg->setGeometry(bgtop, 0, m_grooveHeight, rect().height());
			m_groove->setGeometry(bgtop, 0, m_grooveHeight, qMax(1, m_position));
			m_chunk->setGeometry(chunktop, m_position, m_chunkWidth, width());
		}

	}

}

void ToolBarSlider::mousePressEvent(QMouseEvent *event)
{
	m_bMousePress = true;

	if (!CanChanged())
		return;
	SetCanSeek(false);

	if (childAt(event->pos()) == m_chunk)
	{
		m_offset = event->pos().x() - m_position - m_chunk->width() / 2;
	}
		
	//qDebug() << "mousePressEvent0: value" << m_value;
	CalculatePostion(event->pos() - QPoint(m_offset, 0));
	//qDebug() << "mousePressEvent1: value" << m_value;
	ResizeChunk();
	if (NeedEmitValue())
	{
		emit startValueChanged();
		emit valueChanged(value());
	}

}

void ToolBarSlider::mouseReleaseEvent(QMouseEvent *event)
{
	m_bMousePress = false;
	if (!CanChanged())
		return;
	SetCanSeek(true);
	//qDebug() << "mouseReleaseEvent0: value" << m_value;
	CalculatePostion(event->pos() - QPoint(m_offset, 0));
	//qDebug() << "mouseReleaseEvent1: value" << m_value;
	ResizeChunk();
	if (NeedEmitValue())
	{
		emit valueChanged(value());
		emit endValueChanged();
		if (!m_bStartSeek)
		{
			emit endDrag();
		}
		m_bStartSeek = true;
	}
	m_offset = 0;
}

void ToolBarSlider::mouseMoveEvent(QMouseEvent *event)
{
	if (!CanChanged())
		return;

	CalculatePostion(event->pos() - QPoint(m_offset,0));
	ResizeChunk();
	if (NeedEmitValue())
	{
		if (m_bStartSeek)
		{
			emit startDrag();
			m_bStartSeek = false;
		}
		emit valueChanged(value());
	}

}


bool ToolBarSlider::event(QEvent *event)
{
	if (event->type() == QEvent::Resize)
	{
		CalculatePositionFromValue(qBound(0.0f, value(), m_duration));
	}
	return QWidget::event(event);
}

int ToolBarSlider::GetWidth()
{
	if (m_orientation == Qt::Horizontal)
		return qMax(0, width() - m_chunkWidth);
	else
		return qMax(0, height() - m_chunkWidth);
}

void ToolBarSlider::CalculatePostion(int mousePos)
{
	m_position = qBound(0, mousePos - m_chunkWidth / 2, GetWidth());// max(0, min(mousePos - m_chunkWidth / 2, getWidth()));
	m_value = qBound<float>(0, 1.0 * m_position / GetWidth() * m_duration, m_duration);
}
void ToolBarSlider::CalculatePostion(QPoint p)
{
	int mousePos = 0;
	if (m_orientation == Qt::Horizontal)
	{
		if (m_leftToRight)
			mousePos = p.x();
		else
			mousePos = width() - p.x();
	}
	else
	{
		if (m_bottomToTop)
			mousePos = height() - p.y();
		else
			mousePos = p.y();
	}
	m_position = qBound(0, mousePos - m_chunkWidth / 2, GetWidth());
	m_value = qBound<float>(0, 1.0 * m_position / GetWidth() * m_duration, m_duration);
}


void ToolBarSlider::CalculatePositionFromValue(double value)
{
	int mousePos = 0;
	if (m_orientation == Qt::Horizontal)
		mousePos = 1.0 * value / m_duration * width();
	else
		mousePos = 1.0 * value / m_duration * height();
	CalculatePostion(mousePos);
}



bool ToolBarSlider::NeedEmitValue()
{
	// 最小是1帧，
	return m_duration > 1;
}



bool ToolBarSlider::CanChanged()
{
	return duration() > 1;
}

//////////////////////////////////////////////////////////////////////////

ScaleUnitSlider::ScaleUnitSlider(QWidget *parent)
:QWidget(parent)
{
	setObjectName("CommonLib_ScaleUnitSlider");
	m_slider = new ToolBarSlider(this);
	connect(m_slider, &ToolBarSlider::valueChanged, this, [&](int value){
		if (m_slider->ButtonPressDown())
			emit valueChanged(value);
	});

	connect(m_slider, &ToolBarSlider::startValueChanged, this, &ScaleUnitSlider::startValueChanged);
	connect(m_slider, &ToolBarSlider::endValueChanged, this, &ScaleUnitSlider::endValueChanged);

	m_sub = new BaseButton(0, "CommonLib_ScaleUnitSlider_Sub", this);
	m_add = new BaseButton(1, "CommonLib_ScaleUnitSlider_Add", this);
	connect(m_sub, &BaseButton::Clicked, this, [&](){
		Sub();
	});
	connect(m_add, &BaseButton::Clicked, this, [&](){
		Add();
	});

	setDuration(780);
	setValue(435);

	//setStyleSheet(GetStyles());
}

void ScaleUnitSlider::setDuration(int duration)
{
	m_slider->setDuration(duration);
}


void ScaleUnitSlider::setValue(int value)
{
	m_slider->setValue(qBound(0, value, duration()));
}

int ScaleUnitSlider::value()
{
	return m_slider->value();
}

int ScaleUnitSlider::duration()
{
	return m_slider->duration();
}

void ScaleUnitSlider::SetGrooveHeight(int h)
{
	m_slider->SetGrooveHeight(h);
}

void ScaleUnitSlider::SetChunkHeight(int height)
{
	m_slider->SetChunkHeight(height);
}

void ScaleUnitSlider::Add()
{
	setValue(value() + 10);
	startValueChanged();
	m_slider->SetButtonPressDown(true);
	valueChanged(value());
	m_slider->SetButtonPressDown(false);
}

void ScaleUnitSlider::Sub()
{
	setValue(value() - 10);
	startValueChanged();
	m_slider->SetButtonPressDown(true);
	valueChanged(value());
	m_slider->SetButtonPressDown(false);

}

bool ScaleUnitSlider::ButtonPressDown()
{
	return m_slider->ButtonPressDown();
}

BaseButton* ScaleUnitSlider::GetZoomInButton()
{
	return m_add;
}

BaseButton* ScaleUnitSlider::GetZoomOutButton()
{
	return m_sub;
}


void ScaleUnitSlider::resizeEvent(QResizeEvent *event)
{
	const int margins = 10;
	int btnw = height();
	m_sub->setGeometry(0, 0, btnw, height());
	int left = m_sub->geometry().right() + margins;
	m_slider->setGeometry(left, 0, width() - 2 * btnw - 2 * margins, height());
	left = m_slider->geometry().right() + margins;
	m_add->setGeometry(left, 0, btnw, height());
}



//////////////////////////////////////////////////////////////////////////
class PureSliderPrivate
{
public:
	PureSliderPrivate()
		:m_clrSubpage(Qt::red)
		, m_clrAddpage(Qt::white)
		, m_clrBorder(Qt::gray)
	{
		m_nValue = 0;
		m_nStart = 0;
		m_nEnd = 100;
	}
private:
	friend class PureSlider;
	int m_nValue;
	int m_nStart;
	int m_nEnd;
	QColor m_clrBorder;
	QColor m_clrSubpage;
	QColor m_clrAddpage;
};

PureSlider::PureSlider(QWidget *parent)
:QWidget(parent)
{
	m_p = new PureSliderPrivate;
}

PureSlider::~PureSlider()
{
	SAFE_DELETE(m_p);
}

void PureSlider::SetRange(int nStart, int nEnd)
{
	if (nStart >= nEnd)
	{
		nStart = 0;
		nEnd = 100;
	}
	m_p->m_nStart = nStart;
	m_p->m_nEnd = nEnd;
	SetValue(nStart);
}

void PureSlider::GetRange(int *start, int *end)
{
	*start = m_p->m_nStart;
	*end = m_p->m_nEnd;
}

void PureSlider::SetValue(int nValue)
{
	nValue = qBound(m_p->m_nStart, nValue, m_p->m_nEnd);
	int oldValue = m_p->m_nValue;
	m_p->m_nValue = nValue;
	if (oldValue != nValue)
		emit sigValueChanged(nValue);
	update();
}

int PureSlider::GetValue()
{
	return m_p->m_nValue;
}

void PureSlider::SetBorderColor(const QColor &clr)
{
	m_p->m_clrBorder = clr;
	update();
}

void PureSlider::SetSubpageColor(const QColor &clr)
{
	m_p->m_clrSubpage = clr;
	update();
}

void PureSlider::SetAddpageColor(const QColor &clr)
{
	m_p->m_clrAddpage = clr;
	update();
}

void PureSlider::mousePressEvent(QMouseEvent *e)
{
	if (e->button() == Qt::LeftButton)
	{
		int nValue = (e->pos().x() * 1.0 / rect().width()) * (m_p->m_nEnd - m_p->m_nStart) + m_p->m_nStart;
		SetValue(nValue);
	}
	e->ignore();
}

void PureSlider::mouseMoveEvent(QMouseEvent *e)
{
	int xPos = e->pos().x();
	if (xPos < 0) xPos = 0;
	if (xPos > rect().width()) xPos = rect().width();
	int nValue = (xPos * 1.0 / rect().width()) * (m_p->m_nEnd - m_p->m_nStart) + m_p->m_nStart;
	SetValue(nValue);

	e->ignore();
}

void PureSlider::paintEvent(QPaintEvent *e)
{
	QPainter p(this);
	p.fillRect(rect(), m_p->m_clrAddpage);
	int nW = ((m_p->m_nValue - m_p->m_nStart) * 1.0 / (m_p->m_nEnd - m_p->m_nStart)) * rect().width();
	QRect subpageRect(0, 0, nW, rect().height());
	p.fillRect(subpageRect, m_p->m_clrSubpage);

	p.setPen(QPen(m_p->m_clrBorder));
	p.drawRect(rect().adjusted(0,0,-1,-1));

	if (!isEnabled())
	{
		p.fillRect(rect(),QColor(240,240,240,150));
	}
}
//


//////////////////////////////////////////////////////////////////////////
#include <QtWidgets/QShortcut>
MidSlider::MidSlider(QWidget *parent /*= nullptr*/)
:QWidget(parent)
{
	setObjectName("ActionCam_MidSlider");

	m_value = 0;
	m_min = 0;
	m_max = 100;
	m_mid = 50;
	m_midrate = 0.5f;
	m_stepover = 1.0f;

	m_position = 0;
	m_chunkWidth = 0;
	m_backgroundHeight = 2;

	m_minmumLimit = m_min;
	m_maxmumLimit = m_max;

	m_bg = new QWidget(this);
	m_bg->setObjectName("ActionCam_MidSlider_bg");

	m_groove = new QWidget(this);
	m_groove->setObjectName("ActionCam_MidSlider_groove");


	m_chunk = new QWidget(this);
	m_chunk->setObjectName("ActionCam_MidSlider_chunk");

	QShortcut *	sc = new QShortcut(QKeySequence(Qt::Key_Left), this);
	sc->setContext(Qt::WidgetShortcut);
	connect(sc, &QShortcut::activated, this, [&](){
		SetValue(GetValue() - m_stepover);
		emit ValueChanged(GetValue());
	});
	connect(sc, &QShortcut::activatedAmbiguously, this, [&](){
		SetValue(GetValue() - m_stepover);
		emit ValueChanged(GetValue());
	});

	sc = new QShortcut(QKeySequence(Qt::Key_Right), this);
	sc->setContext(Qt::WidgetShortcut);
	connect(sc, &QShortcut::activated, this, [&](){
		SetValue(GetValue() + m_stepover);
		emit ValueChanged(GetValue());
	});
	connect(sc, &QShortcut::activatedAmbiguously, this, [&](){
		SetValue(GetValue() + m_stepover);
		emit ValueChanged(GetValue());
	});

	//m_bg->setStyleSheet("background-color:white;");
	//m_groove->setStyleSheet("QWidget{background-color:rgb(91,192,222);}QWidget:disabled{background-color:rgb(178,220,232);}");
	//m_chunk->setStyleSheet("QWidget{background-color:rgb(91,192,222);}QWidget:disabled{background-color:rgb(178,220,232);}");

	setFocusPolicy(Qt::ClickFocus);

}

void MidSlider::SetValue(int value)
{
	m_value = qBound(GetMin(), value, GetMax());

	//value = qAbs(m_value);
	if (m_value <= m_mid)
	{
		m_position = 1.0 * (m_value - m_min) / (m_mid - m_min) * GetLeftWidth();
	}
	else
	{
		m_position = 1.0 * (m_value - m_mid) / (m_max - m_mid) * GetRightWidth() + GetLeftWidth();
	}

	QMetaObject::invokeMethod(this, "ResizeChunk", Qt::QueuedConnection);
}

void MidSlider::SetRange(int minvalue, int midvalue, int maxvalue, float midrate)
{
	Q_ASSERT(midrate > 0.0 && midrate <= 1.0f);
	Q_ASSERT(maxvalue > midvalue && midvalue > minvalue);

	m_min = minvalue;
	m_mid = midvalue;
	m_max = maxvalue;
	m_midrate = midrate;

	m_maxmumLimit = maxvalue;
	m_minmumLimit = minvalue;
}

void MidSlider::SetRange(int minvalue, int maxvalue)
{
	SetRange(minvalue, (maxvalue + minvalue) / 2, maxvalue);
}

void MidSlider::GetRange(int &minvalue, int &maxvalue)
{
	minvalue = m_min;
	maxvalue = m_max;
}

int MidSlider::GetValue()
{
	return m_value;
}

void MidSlider::SetChunkWidth(int w)
{
	m_chunkWidth = w;
}

void MidSlider::SetBackgroundHeight(int h)
{
	m_backgroundHeight = h;
}

void MidSlider::SetStepOver(float fStep)
{
	m_stepover = fStep;
}

void MidSlider::SetMaxmumLimit(int maxvalue)
{
	m_maxmumLimit = maxvalue;
}

void MidSlider::SetMinmumLimit(int minvalue)
{
	minvalue = minvalue;
}

void MidSlider::resizeEvent(QResizeEvent *event)
{
	int top = 0;
	int h = height();
	int w = width();
	if (m_backgroundHeight > 0)
	{
		top = (height() - m_backgroundHeight) / 2;
		h = m_backgroundHeight;
	}
	if (m_chunkWidth == 0)
		m_chunkWidth = height();

	m_bg->setGeometry(0, top, width(), h);
	m_groove->setGeometry(0, top, qMax(0, m_position), h);
	m_chunk->setGeometry(m_position, 0, m_chunkWidth, height());
}

void MidSlider::mousePressEvent(QMouseEvent *event)
{
	int newvalue = CalculateValue(event->pos().x());

	if (newvalue != GetValue())
	{
		m_value = newvalue;
		emit StartValueChanged(m_value);
		emit ValueChanged(m_value);
		if (IsLimitValue())
			resizeEvent(nullptr);
	};
	if (!IsLimitValue())
		resizeEvent(nullptr);

}

void MidSlider::mouseReleaseEvent(QMouseEvent *event)
{
	emit EndValueChanged(m_value);
}

void MidSlider::mouseMoveEvent(QMouseEvent *event)
{
	m_valueTemp = CalculateValue(event->pos().x());
	if (m_valueTemp != GetValue())
	{
		m_value = m_valueTemp;
		emit ValueChanged(m_value);
		if (IsLimitValue())
			resizeEvent(nullptr);
	}
	else if (m_valueTemp == m_max || m_valueTemp == m_min) // 可拖到两边
	{
		resizeEvent(nullptr);
	}
	if (!IsLimitValue())
		resizeEvent(nullptr);
}

void MidSlider::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
	{
		clearFocus();
	}

}

bool MidSlider::event(QEvent *event)
{
	if (event->type() == QEvent::Resize)
	{
		SetValue(GetValue());
	}
	return QWidget::event(event);
}

int MidSlider::GetWidth()
{
	return qMax(0, width() - m_chunkWidth);
}

int MidSlider::GetLeftWidth()
{
	return qMax<int>(0, m_midrate * (width() - m_chunkWidth));
}

int MidSlider::GetRightWidth()
{
	return qMax<int>(0, (1 - m_midrate) * (width() - m_chunkWidth));
}


int MidSlider::CalculateValue(int mousePos)
{
	int value = 0;
	m_position = qBound(0, mousePos - m_chunkWidth / 2, GetWidth());
	if (m_position <= m_midrate * GetWidth())
	{
		value = qBound<int>(GetMin(), 1.0 * m_position / GetLeftWidth() * (m_mid - m_min) + m_min, qMin(m_mid, GetMax()));
	}
	else
	{
		value = qBound<int>(GetMin(), 1.0 * (m_position - GetLeftWidth()) / GetRightWidth() * (m_max - m_mid) + m_mid, GetMax());
	}

	return value;

}



int MidSlider::CalculatePosition(int value)
{
	int pos = 0;
	if (value < m_mid)
	{
		pos = (float)value / (m_mid - m_min) * GetLeftWidth() ;
	}
	else
	{
		pos = (float)(value - m_mid) / (m_max - m_mid) * GetRightWidth() + GetLeftWidth() ;
	}
	return pos;
}

int MidSlider::GetMin()
{
	return qMax(m_min, m_minmumLimit);
}

int MidSlider::GetMax()
{
	return qMin(m_max, m_maxmumLimit);
}

bool MidSlider::IsLimitValue()
{
	return m_min != m_minmumLimit || m_max != m_maxmumLimit;
}

void MidSlider::ResizeChunk()
{
	resizeEvent(nullptr);
}

//////////////////////////////////////////////////////////////////////////
SetColorParamWidget::SetColorParamWidget(const QString &title, QWidget *parent)
: QWidget(parent)
{
	m_pSlider = new MidSlider(this);
	m_pLabelTitle = new QLabel(title, this);
	//m_pLabelTitle->setAlignment(Qt::AlignRight);

	m_pLabelParam = new QLabel(this);
	m_pLabelParam->setAlignment(Qt::AlignRight);
	m_pLabelParam->setText(QString("%0").arg(m_pSlider->GetValue()));

	connect(m_pSlider, &MidSlider::ValueChanged, this, [&](int nValue){
		m_pLabelParam->setText(QString("%0").arg(nValue));
		emit sigValueChanged(nValue);
	});
}

SetColorParamWidget::~SetColorParamWidget()
{

}

void SetColorParamWidget::SetRange(int nStart, int nEnd)
{
	m_pSlider->SetRange(nStart, nEnd);
}

void SetColorParamWidget::GetRange(int *start, int *end)
{
	m_pSlider->GetRange(*start, *end);
}

void SetColorParamWidget::SetValue(int nValue)
{
	m_pSlider->SetValue(nValue);
	m_pLabelParam->setText(QString("%0").arg(nValue));
}

int SetColorParamWidget::GetValue()
{
	return m_pSlider->GetValue();
}

void SetColorParamWidget::resizeEvent(QResizeEvent *e)
{
	static int const textH = 16;
	static int const spacingH = 10;
	static int const sliderH = 10;
	static int const textParamW = 25;

	m_pLabelTitle->setGeometry(0, 0,StringOperation::GetTextTrueLenght(m_pLabelTitle->font(),m_pLabelTitle->text()), textH);
	int top = m_pLabelTitle->geometry().bottom() + spacingH;
	m_pSlider->setGeometry(0, top, rect().width() - textParamW, sliderH);
	m_pLabelParam->setGeometry(m_pSlider->geometry().right(), top - 2, textParamW, textH);
}



//////////////////////////////////////////////////////////////////////////

MagicLabel::MagicLabel(const QString &text, QWidget *parent /*= 0*/)
:QLabel(text, parent)
{
	if (gIsArgLang())
		this->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
}

MagicLabel::MagicLabel(QWidget *parent /*= 0*/)
: QLabel(parent)
{
	if (gIsArgLang())
		this->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
}
