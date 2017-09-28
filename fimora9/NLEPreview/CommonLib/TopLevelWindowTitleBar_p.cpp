
#include "TopLevelWindowTitleBar_p.h"

#include "inc_CommonLib/CommonWidgets.h"
#include "inc_CommonLib/DesktopMgr.h"

#include <QtCore/QtCore>
#include <QtWidgets/QtWidgets>
#include <QtGui/QtGui>

TopLevelWindowTitleBar::TopLevelWindowTitleBar(QWidget *parent)
:QWidget(parent)
{
	if (parent)
	{
		parent->setWindowFlags(Qt::Window | Qt::CustomizeWindowHint | Qt::WindowSystemMenuHint);
	}
	m_p = new TopLevelWindowTitleBar_p(this);
	m_p->SetControlWidget(this, parent);
}


void TopLevelWindowTitleBar::SetTitleName(QString name)
{
	m_p->SetTitleName(name);
}

QString TopLevelWindowTitleBar::GetTitleName()
{
	return m_p->GetTitleName();
}
void TopLevelWindowTitleBar::SetTitleNameBold(bool enable)
{
	m_p->SetTitleNameBold(enable);

}

QLabel* TopLevelWindowTitleBar::GetTitleLabel()
{
	return m_p->GetTitleLabel();

}
void TopLevelWindowTitleBar::SetTitleNameSize(int size)
{
	m_p->SetTitleNameSize(size);

}

void TopLevelWindowTitleBar::SetTitleNameAlign(Qt::Alignment flag)
{
	m_p->SetTitleNameAlign(flag);

}
void TopLevelWindowTitleBar::InitWindowMaximumButtonState(bool maximum)
{
	m_p->InitWindowMaximumButtonState(maximum);
}


void TopLevelWindowTitleBar::SetResizeble(bool enable)
{
	m_p->GetConrtol()->SetResizeable(enable);
}


void TopLevelWindowTitleBar::SetTitleBarColor(QColor color)
{
	m_p->SetTitleBarColor(color);
}

void TopLevelWindowTitleBar::SetButtons(int button)
{
	m_p->SetButtons(button);
}

void TopLevelWindowTitleBar::SetButtonWidth(int width)
{
	m_p->SetButtonWidth(width);

}
void TopLevelWindowTitleBar::SetDragMaximum(bool enable)
{
	m_p->GetConrtol()->SetDragMaximum(enable);
}


void TopLevelWindowTitleBar::SetDashlineColor(QColor color)
{
	m_p->GetConrtol()->SetDashlineColor(color);
}


void TopLevelWindowTitleBar::SetAutoCloseWindow(bool enable)
{
	m_p->SetAutoCloseWindow(enable);
}


void TopLevelWindowTitleBar::SetTitleIcon(QPixmap pixmap)
{
	m_p->SetTitleIcon(pixmap);
}
bool TopLevelWindowTitleBar::IsMaximized()
{
	return m_p->GetConrtol()->IsMaximized();
}

void TopLevelWindowTitleBar::ShowMaximized()
{
	m_p->GetConrtol()->ShowMaximized();
}

void TopLevelWindowTitleBar::ShowNormal()
{
	m_p->GetConrtol()->ShowNormal();
}

void TopLevelWindowTitleBar::ShowMinimized()
{
	m_p->GetConrtol()->ShowMinimized();
}


void TopLevelWindowTitleBar::CloseWindow()
{
	m_p->GetConrtol()->CloseWindow();
}

void TopLevelWindowTitleBar::AddSuffixButton(QWidget *btn)
{
	m_p->AddSuffixButton(btn);
}

void TopLevelWindowTitleBar::AddSuffixSpace(int space)
{
	m_p->AddSuffixSpace(space);
}


TopLevelWindowTitleBarResizeMoveControl* TopLevelWindowTitleBar::GetControl()
{
	return m_p->GetConrtol();
}


void TopLevelWindowTitleBar::SetSystemBorderVisible(bool visible)
{
	auto widget = parentWidget();
	if (widget)
	{
		if (visible)
		{
			widget->setWindowFlags(Qt::Window | Qt::CustomizeWindowHint | Qt::WindowSystemMenuHint);
		}
		else
			widget->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
	}

	m_p->GetConrtol()->SetSystemBorderVisible(visible);
}


bool TopLevelWindowTitleBar::GetSystemBorderVisible()
{
	return m_p->GetConrtol()->GetSystemBorderVisible();
}

BaseButton* TopLevelWindowTitleBar::GetButton(int id)
{
	return m_p->GetButton(id);
}

void TopLevelWindowTitleBar::SetSystemMenuEnabled(bool enabled)
{
	m_p->SetSystemMenuEnabled(enabled);
}
//////////////////////////////////////////////////////////////////////////

TopLevelWindowTitleBar_p::TopLevelWindowTitleBar_p(QWidget *parent)
:QWidget(parent)
, m_autoCloseWindow(true)
, m_btnWidth(0)
{
	setObjectName("TopLevelWindowTitleBar");
	setAttribute(Qt::WA_StyledBackground);

	m_titleBarColor = QColor(250, 250, 250);
	m_buttonEnum = TopLevelWindowTitleBar::Minimized | TopLevelWindowTitleBar::Normal | TopLevelWindowTitleBar::Close;

	m_title = new MagicLabel(this);
	m_title->setObjectName("TopLevelWindowTitleBar_TitleName");
	m_title->setAttribute(Qt::WA_TranslucentBackground);
	m_title->setStyleSheet("padding-left:1px;");  //日文的标题栏文字会被遮掉1个像素

	m_icon = new QLabel(this);
	m_icon->setObjectName("TopLevelWindowTitleBar_p_icon");
	m_icon->setEnabled(false);

	m_control = qobject_cast<TopLevelWindowTitleBarResizeMoveControl_p*>(TopLevelWindowTitleBarResizeMoveControl::CreateControl());
    connect(m_control, &TopLevelWindowTitleBarResizeMoveControl::StartDashedMove, this, [&, this](QPoint point){
        emit qobject_cast<TopLevelWindowTitleBar*>(this->parent())->StartDashedMove(point);
    });
    connect(m_control, &TopLevelWindowTitleBarResizeMoveControl::DashedMove, this, [&, this](QPoint point){
        emit qobject_cast<TopLevelWindowTitleBar*>(this->parent())->DashedMove(point);
    });
    connect(m_control, &TopLevelWindowTitleBarResizeMoveControl::EndDashedMove, this, [&, this](QPoint point){
        emit qobject_cast<TopLevelWindowTitleBar*>(this->parent())->EndDashedMove(point);
    });

	//m_control->SetControlWidget(this, parent);
	CreateButton(TopLevelWindowTitleBar::Minimized, "TopLevelWindowTitleBar_Minimized");
	CreateStateButton(TopLevelWindowTitleBar::Normal, "TopLevelWindowTitleBar_Normal", TopLevelWindowTitleBar::Maximized, "TopLevelWindowTitleBar_Maximized");
	CreateButton(TopLevelWindowTitleBar::Close, "TopLevelWindowTitleBar_Close");

	parent->installEventFilter(this);

	m_systemMenuControl = new SystemMenuControl(this);

}



void TopLevelWindowTitleBar_p::SetControlWidget(QWidget *titleBar, QWidget *toplevelWindow)
{
	m_control->SetControlWidget(titleBar, toplevelWindow);
}

void TopLevelWindowTitleBar_p::SetTitleName(QString name)
{
	m_title->setText(name);
}

QString TopLevelWindowTitleBar_p::GetTitleName()
{
	return m_title->text();
}

void TopLevelWindowTitleBar_p::SetTitleNameBold(bool enable)
{
	QFont font = m_title->font();
	font.setBold(enable);
	m_title->setFont(font);
}

void TopLevelWindowTitleBar_p::SetTitleNameSize(int size)
{
	QFont font = m_title->font();
	font.setPixelSize(size);
	m_title->setFont(font);
}

void TopLevelWindowTitleBar_p::SetTitleNameAlign(Qt::Alignment flag)
{
	m_title->setAlignment(flag);
}

QLabel* TopLevelWindowTitleBar_p::GetTitleLabel()
{
	return m_title;
}

void TopLevelWindowTitleBar_p::InitWindowMaximumButtonState(bool maximum)
{
	SetMininMaximButton(!maximum ? TopLevelWindowTitleBar::Maximized : TopLevelWindowTitleBar::Normal);
}



void TopLevelWindowTitleBar_p::SetTitleBarColor(QColor color)
{
	m_titleBarColor = color;
}

void TopLevelWindowTitleBar_p::SetButtons(int button)
{
	m_buttonEnum = button;
}

void TopLevelWindowTitleBar_p::SetButtonWidth(int width)
{
	m_btnWidth = width;
}

void TopLevelWindowTitleBar_p::resizeEvent(QResizeEvent *event)
{
	int left = 0;
	int top = 2;
	const int space = 2;
	int btntop = top;
	int btnWidth = height() ;
	int btnHeight = height() - 2 * btntop;
	if (m_btnWidth > 0)
	{
		btnWidth = m_btnWidth;
		btnHeight = m_btnWidth;
		btntop = (height() - btnHeight) / 2;
	}

	const int buttonSize = 3;
	//const int buttonsWidth = buttonSize * (btnWidth + space) + 5;

	if (m_icon->isEnabled())
	{
		m_icon->setGeometry(left, btntop, btnHeight, btnHeight);
		left = m_icon->geometry().right();
	}
	else
	{
		left += 2;
	}


	m_title->setGeometry(left, btntop, width(), btnHeight);
	//m_title->setGeometry(left, 0, width(), height());


	// 右边额外widgets
	auto btns = GetButtons();
	int buttonsWidth = btns.size() * (btnWidth + space);

	for (auto btn : m_extraWidgetsRight)
	{
		if (!btn->property("extraSpace").isValid())
		{
			buttonsWidth += btnWidth + space;
		}
		else
		{
			int tempw = btn->property("extraSpace").toInt();
			buttonsWidth += tempw;
		}
	}


	left = width() - buttonsWidth;

	for (auto btn : m_extraWidgetsRight)
	{
		if (!btn->property("extraSpace").isValid())
		{
			btn->setGeometry(left, btntop, btnWidth, btnHeight);
			left = btn->geometry().right() + space;
		}
		else
		{
			int tempw = btn->property("extraSpace").toInt();
			btn->setGeometry(left, btntop, tempw, btnHeight);
			left = btn->geometry().right();
		}
	}

	for (auto btn : btns)
	{
		btn->setGeometry(left, btntop, btnWidth, btnHeight);
		left = btn->geometry().right() + space;
	}
}




void TopLevelWindowTitleBar_p::mouseDoubleClickEvent(QMouseEvent *event)
{
	if (GetButton(TopLevelWindowTitleBar::Normal)->isVisible())
		(qobject_cast<BaseStateButton*>(GetButton(TopLevelWindowTitleBar::Normal)))->ClickCurrentButton();
}

bool TopLevelWindowTitleBar_p::eventFilter(QObject *watched, QEvent *event)
{
	QWidget *parent = parentWidget();
	if (watched == parent)
	{
		if (event->type() == QEvent::Resize)
		{
			setGeometry(parent->rect());
			m_systemMenuControl->UpdateMenuState();
		}
	}
	return false;
}

//void TopLevelWindowTitleBar_p::paintEvent(QPaintEvent *event)
//{
//	QPainter painter(this);
//	painter.fillRect(rect(), m_titleBarColor);
//}

void TopLevelWindowTitleBar_p::SetMininMaximButton(int id)
{
	(qobject_cast<BaseStateButton*>(GetButton(TopLevelWindowTitleBar::Normal)))->SetCurrentButtonId(id);
}



void TopLevelWindowTitleBar_p::SetAutoCloseWindow(bool enable)
{
	m_autoCloseWindow = enable;
}

void TopLevelWindowTitleBar_p::SetTitleIcon(QPixmap pixmap)
{
	m_icon->setPixmap(pixmap);
	m_icon->setEnabled(true);
}

void TopLevelWindowTitleBar_p::AddSuffixButton(QWidget *btn)
{
	Q_ASSERT(!m_extraWidgetsRight.contains(btn));

	m_extraWidgetsRight.push_back(btn);
}

void TopLevelWindowTitleBar_p::AddSuffixSpace(int space)
{
	Q_ASSERT(space > 0);
	QWidget *w = new QWidget(this);
	w->setProperty("extraSpace", space);
	m_extraWidgetsRight.push_back(w);
}


void TopLevelWindowTitleBar_p::SetSystemMenuEnabled(bool enabled)
{
	m_systemMenuControl->SetEnabled(enabled);
}

TopLevelWindowTitleBarResizeMoveControl_p* TopLevelWindowTitleBar_p::GetConrtol()
{
	return m_control;
}

bool TopLevelWindowTitleBar_p::PopupMenu(QPoint point)
{
	return m_systemMenuControl->PopupMenu(point);
}

void TopLevelWindowTitleBar_p::CreateButton(int id, QString objectName)
{
	auto btn = new BaseButton(id, objectName, this);
	connect(btn, &BaseButton::Clicked, this, [&](int id, QPoint p){
		ButtonClicked(id);
	});
	m_buttons.insert(id, btn);

}

void TopLevelWindowTitleBar_p::CreateStateButton(int firstId, QString firstObjectName, int secondId, QString secondObjectName)
{
	auto btn = new BaseStateButton(firstId, "", this);
	btn->AddButton(firstId, firstObjectName);
	btn->AddButton(secondId, secondObjectName);
	connect(btn, &BaseStateButton::Clicked, this, [&](int id, QPoint p){
		ButtonClicked(id);

	});
	m_buttons.insert(firstId, btn);
}

BaseButton* TopLevelWindowTitleBar_p::GetButton(int id)
{
	auto btn = m_buttons[id];
	Q_ASSERT(btn);
	return btn;
}

QList<BaseButton*> TopLevelWindowTitleBar_p::GetButtons()
{
	QList<BaseButton*> btns;
	for (auto beg = m_buttons.begin(); beg != m_buttons.end(); ++beg)
	{
		if (m_buttonEnum & TopLevelWindowTitleBar::None)
		{
			beg.value()->hide();
		}
		else
		{
			if (beg.key() & m_buttonEnum ||
				(m_buttonEnum & TopLevelWindowTitleBar::Maximized) && (beg.key() & TopLevelWindowTitleBar::Normal))
			{
				btns.push_back(beg.value());
				beg.value()->show();
			}
			else
				beg.value()->hide();
		}

	}
	return btns;
}

void TopLevelWindowTitleBar_p::ButtonClicked(int id)
{
	emit qobject_cast<TopLevelWindowTitleBar*>(parent())->ButtonClick((TopLevelWindowTitleBar::Button)id);

	switch (id)
	{
	case TopLevelWindowTitleBar::Minimized:
		m_control->ShowMinimized();
		break;
	case TopLevelWindowTitleBar::Normal:
	{
		m_control->ShowNormal();
		SetMininMaximButton(TopLevelWindowTitleBar::Maximized);
	}
		break;
	case TopLevelWindowTitleBar::Maximized:
	{
		m_control->ShowMaximized();
		SetMininMaximButton(TopLevelWindowTitleBar::Normal);
	}
		break;
	case TopLevelWindowTitleBar::Close:
		if (m_autoCloseWindow)
			m_control->CloseWindow();
		break;
	default:
		Q_ASSERT(false);
	}

	m_systemMenuControl->UpdateMenuState();
}

//////////////////////////////////////////////////////////////////////////



TopLevelWindowTitleBarResizeMoveControl* TopLevelWindowTitleBarResizeMoveControl::CreateControl()
{
	return new TopLevelWindowTitleBarResizeMoveControl_p();
}

//////////////////////////////////////////////////////////////////////////
TopLevelWindowTitleBarResizeMoveControl_p::TopLevelWindowTitleBarResizeMoveControl_p()
:TopLevelWindowTitleBarResizeMoveControl()
{
	m_hint = false;
	m_resizeing = false;
	m_mousePress = false;
	m_left = m_top = m_right = m_bottom = false;

	m_maximizedDragResize = false;
	m_titleMousePress = false;
	m_maximizedDragResize_fullscreen = false;

	m_titleBar = nullptr;
	m_toplevelWindow = nullptr;
	m_toplevelWindowCanResize = true;
	m_toplevelWindowFirstShow = true;
	m_toplevelDragMaximum = true;

	m_borderColor = QColor(73, 73, 250);

	m_isMaximized = false;

	m_systemBorder = true;
	m_borderWidth = 1;
    m_toplevelWindowCanMove = true;
	m_maxGeometry = QApplication::desktop()->availableGeometry();
}

void TopLevelWindowTitleBarResizeMoveControl_p::SetControlWidget(QWidget *titleBar, QWidget *toplevelWindow)
{
	Q_ASSERT(titleBar);
	Q_ASSERT(toplevelWindow);
	m_titleBar = titleBar;
	m_toplevelWindow = toplevelWindow;
	setParent(titleBar);
	m_dashedWidget = new DashlineWidget(titleBar);
	qApp->installEventFilter(this);
}



QWidget* TopLevelWindowTitleBarResizeMoveControl_p::GetTitleBar()
{
	return m_titleBar;
}

QWidget* TopLevelWindowTitleBarResizeMoveControl_p::GetTopLevelWindow()
{
	return m_toplevelWindow;
}

void TopLevelWindowTitleBarResizeMoveControl_p::SetResizeable(bool enable)
{
	m_toplevelWindowCanResize = enable;
}

void TopLevelWindowTitleBarResizeMoveControl_p::SetMoveable(bool enable)
{
    m_toplevelWindowCanMove = enable;
}

void TopLevelWindowTitleBarResizeMoveControl_p::SetDragMaximum(bool enable)
{
	m_toplevelDragMaximum = enable;
}

void TopLevelWindowTitleBarResizeMoveControl_p::SetDashlineColor(QColor color)
{
	m_dashedWidget->SetColor(color);
}

void TopLevelWindowTitleBarResizeMoveControl_p::SetBorderColor(QColor color)
{
	m_borderColor = color;
}
QColor TopLevelWindowTitleBarResizeMoveControl_p::GetBorderColor()
{
    return m_borderColor;
}

int TopLevelWindowTitleBarResizeMoveControl_p::GetBorderWidth()
{
    return m_borderWidth;
}



void TopLevelWindowTitleBarResizeMoveControl_p::SetBorderWidth(int width)
{
	m_borderWidth = width;
}

void TopLevelWindowTitleBarResizeMoveControl_p::SetSystemBorderVisible(bool visible)
{
	m_systemBorder = visible;
}

bool TopLevelWindowTitleBarResizeMoveControl_p::GetSystemBorderVisible()
{
	return m_systemBorder;
}

void TopLevelWindowTitleBarResizeMoveControl_p::MouseMove(QPoint p)
{
	if (!IsMaximized() && !Resizeing() && m_toplevelWindowCanResize && !GetSystemBorderVisible())
	{
		const int margins = 4;
		QRect previewRect = m_toplevelWindow->geometry();
		QRect interRect = previewRect.adjusted(margins, margins, -margins, -margins);
		if (!interRect.contains(p) && previewRect.contains(p))
		{
			m_left = (p.x() >= previewRect.x() && p.x() <= interRect.x());
			m_top = (p.y() >= previewRect.y() && p.y() <= interRect.y());
			m_right = (p.x() >= interRect.right() && p.x() <= previewRect.right());
			m_bottom = (p.y() >= interRect.bottom() && p.y() <= previewRect.bottom());

			if (m_left && m_top || m_right && m_bottom)
				m_toplevelWindow->setCursor(Qt::SizeFDiagCursor);
			else if (m_left && m_bottom || m_top && m_right)
				m_toplevelWindow->setCursor(Qt::SizeBDiagCursor);
			else if (m_left || m_right)
				m_toplevelWindow->setCursor(Qt::SizeHorCursor);
			else if (m_top || m_bottom)
				m_toplevelWindow->setCursor(Qt::SizeVerCursor);
			else
				m_toplevelWindow->setCursor(Qt::ArrowCursor);

			m_hint = (m_left || m_top || m_right || m_bottom);
		}
		else
		{
			m_hint = false;
			m_toplevelWindow->setCursor(Qt::ArrowCursor);
		}
	}

}

bool TopLevelWindowTitleBarResizeMoveControl_p::eventFilter(QObject *watched, QEvent *event)
{

	auto type = event->type();
	if (type == QEvent::MouseMove)
	{
		MouseMove(static_cast<QMouseEvent*>(event)->globalPos());
	}

	if (watched == m_toplevelWindow)
	{
		if (type == QEvent::MouseButtonPress && m_hint && m_toplevelWindowCanResize && !GetSystemBorderVisible())
		{
			m_mousePress = true;
			//m_curPoint = static_cast<QMouseEvent*>(event)->pos();
			m_dashedWidget->setGeometry(m_toplevelWindow->geometry());
			m_dashedWidget->ShowDashLine();

			SetResizeing(true);
		}
		else if (type == QEvent::MouseMove && m_toplevelWindowCanResize && !GetSystemBorderVisible())
		{
			if (m_mousePress && m_hint)
			{
				auto pEvent = static_cast<QMouseEvent*>(event);
				QRect rectmax = QApplication::desktop()->availableGeometry(pEvent->globalPos());
				m_maxGeometry = rectmax;

				QPoint globalPos = static_cast<QMouseEvent*>(event)->globalPos();
				QRect rect = m_toplevelWindow->geometry();

				int minWidth = m_toplevelWindow->minimumSize().width();
				int minHeight = m_toplevelWindow->minimumSize().height();
				if (m_left)
				{
					int x = globalPos.x();
					if (rect.right() - globalPos.x() < minWidth)
						x = rect.right() - minWidth;
					rect.setX(x);

				}

				if (m_top)
				{
					int y = globalPos.y();
					if (rect.bottom() - globalPos.y() < minHeight)
						y = rect.bottom() - minHeight;
					rect.setY(y);

				}

				if (m_right)
					rect.setRight(globalPos.x());
				if (m_bottom)
					rect.setBottom(globalPos.y());

				rect.setWidth(qMax(rect.width(), minWidth));
				rect.setHeight(qMax(rect.height(), minHeight));

				m_dashedWidget->MovePosition(rect);
			}

		}
		else if (type == QEvent::MouseButtonRelease && m_hint && m_toplevelWindowCanResize && !GetSystemBorderVisible())
		{
			m_mousePress = false;
			SetResizeing(false);
			m_hint = m_left = m_top = m_right = m_bottom = false;

			m_toplevelWindow->setGeometry(m_dashedWidget->geometry());
			m_normalGeometry = m_dashedWidget->geometry();
			m_dashedWidget->HideDashline();

		}
		else if (type == QEvent::Move)
		{
			auto ffff = m_toplevelWindow->geometry();
			if (!IsMaximized())
				m_normalGeometry = m_toplevelWindow->geometry();
		}
		else if (type == QEvent::Resize)
		{
			auto dd = m_toplevelWindow->geometry();
			int i = 0;
		}
		else if (type == QEvent::Show)
		{
			auto dd = m_toplevelWindow->geometry();
			int i = 0;
		}
		else if (type == QEvent::Paint)
		{
			if (m_toplevelWindowFirstShow)
			{
				m_toplevelWindowFirstShow = false;
				if (!IsMaximized())
					m_normalGeometry = m_toplevelWindow->geometry();
				else
				{
					QSize tempSize(1024, 610);
					auto size = m_toplevelWindow->minimumSize();
					if (size.isNull())
						size = tempSize;
					m_normalGeometry = QRect((QApplication::desktop()->availableGeometry().width() - size.width()) / 2,
						(QApplication::desktop()->availableGeometry().height() - size.height()) / 2,
						size.width(), size.height());
				}
			}

			if (!IsMaximized() && !GetSystemBorderVisible())
			{
				QPainter painter(m_toplevelWindow);
				QPen pen = painter.pen();
				pen.setWidth(m_borderWidth * 2);
				pen.setColor(m_borderColor);
				painter.setPen(pen);
				painter.setBrush(Qt::NoBrush);
				painter.drawRect(m_toplevelWindow->rect());
			}

		}

	}
	if (watched == m_titleBar && !m_hint)
	{

		if (type == QEvent::MouseButtonPress)
		{

			auto pEvent = static_cast<QMouseEvent*>(event);

			auto defaultTitleBar = GetDefaultTitleBar();
			if (defaultTitleBar)
			{
				if (defaultTitleBar->PopupMenu(pEvent->globalPos()))
					return true;
			}

            if (m_toplevelWindowCanMove)
            {
                m_titleMousePress = true;
                m_titleousePressPoint = static_cast<QMouseEvent*>(event)->pos();

            }
	
		}
		else if (type == QEvent::MouseMove)
		{
			auto pEvent = static_cast<QMouseEvent*>(event);
			QPoint offset = pEvent->pos() - m_titleousePressPoint;
			//qDebug() << "offset " << offset;


			QRect rectmax = QApplication::desktop()->availableGeometry(pEvent->globalPos());
			m_maxGeometry = rectmax;
			//qDebug() << "rectmax" << rectmax;


			if (m_titleMousePress)
			{
				if (IsMaximized())
				{
					if (offset.manhattanLength() > QApplication::startDragDistance())
					{
						QRect maximizedRect = rectmax;
						QRect rect = m_normalGeometry;

						QPoint lefttop = QPoint(/*m_maxGeometry.left() + */pEvent->globalPos().x() - (1.0 * m_titleousePressPoint.x() / maximizedRect.width()*rect.width()), pEvent->pos().y());


						qDebug() << "lefttop" << lefttop;
						rect.moveTopLeft(lefttop);

						if (rect.y() <= 2 && m_toplevelDragMaximum)
						{
							rect = rectmax;
							m_maximizedDragResize = false;
						}
						else
							m_maximizedDragResize = true;

						m_dashedWidget->ShowDashLine();
						m_dashedWidget->setGeometry(rect);


					}

				}
				else
				{
					auto curPos = m_toplevelWindow->geometry().topLeft() + offset;

					m_dashedWidget->ShowDashLine();
					if (curPos.y() <= 2 && m_toplevelDragMaximum)
					{
						m_dashedWidget->resize(rectmax.width(),rectmax.height());
						m_dashedWidget->move(rectmax.left(), rectmax.top());
						m_maximizedDragResize_fullscreen = true;
						m_maximizedDragResize = false;
					}
					else
					{

						m_dashedWidget->resize(m_toplevelWindow->size());
						m_dashedWidget->move(curPos);
						m_maximizedDragResize_fullscreen = false;
						m_maximizedDragResize = true;
					}


					//m_nlePreview->move(m_nlePreview->geometry().topLeft() + offset);
				}
			}

		}
		else if (type == QEvent::MouseButtonRelease)
		{

			auto defaultTitleBar = GetDefaultTitleBar();
			if (m_maximizedDragResize)
			{
				m_isMaximized = false;
				m_toplevelWindow->setGeometry(m_dashedWidget->geometry());
				m_normalGeometry = m_dashedWidget->geometry();
				if (defaultTitleBar)
					defaultTitleBar->SetMininMaximButton(TopLevelWindowTitleBar::Maximized);
			}
			else if (m_maximizedDragResize_fullscreen)
			{
				ShowMaximized();
				
				if (defaultTitleBar)
					defaultTitleBar->SetMininMaximButton(TopLevelWindowTitleBar::Normal);
			}
			m_dashedWidget->HideDashline();
			m_titleMousePress = false;
			m_mousePress = false;
			m_maximizedDragResize = false;
			m_maximizedDragResize_fullscreen = false;
		}


	}
    if (watched == m_dashedWidget)
    {
        if (event->type() == QEvent::Move)
        {
            emit DashedMove(static_cast<QMoveEvent*>(event)->pos());
        }
        else if (event->type() == QEvent::Show)
        {
            emit StartDashedMove(m_dashedWidget->geometry().topLeft());

        }
        else if (event->type() == QEvent::Hide)
        {
            emit EndDashedMove(m_dashedWidget->geometry().topLeft());
        }
    }
	return false;
}

bool TopLevelWindowTitleBarResizeMoveControl_p::CanResize()
{
	return !IsMaximized();
}

bool TopLevelWindowTitleBarResizeMoveControl_p::Resizeing()
{
	return m_resizeing;
}

void TopLevelWindowTitleBarResizeMoveControl_p::SetResizeing(bool resize)
{
	m_resizeing = resize;

}

TopLevelWindowTitleBar_p* TopLevelWindowTitleBarResizeMoveControl_p::GetDefaultTitleBar()
{
	auto children = m_titleBar->children();
	for (auto titlebar : children)
	{
		if (titlebar->objectName() == "TopLevelWindowTitleBar")
			return qobject_cast<TopLevelWindowTitleBar_p*>(titlebar);
	}
	return nullptr;
}


bool TopLevelWindowTitleBarResizeMoveControl_p::IsMaximized()
{
	return m_isMaximized;
}

void TopLevelWindowTitleBarResizeMoveControl_p::ShowMaximized()
{
	m_isMaximized = true;
	m_toplevelWindow->show();
	m_toplevelWindow->setGeometry(m_maxGeometry);
	auto defaultTitleBar = GetDefaultTitleBar();
	if (defaultTitleBar)
		defaultTitleBar->InitWindowMaximumButtonState(true);
}

void TopLevelWindowTitleBarResizeMoveControl_p::ShowNormal()
{
	m_isMaximized = false;
	m_toplevelWindow->setGeometry(m_normalGeometry);
	auto defaultTitleBar = GetDefaultTitleBar();
	if (defaultTitleBar)
		defaultTitleBar->InitWindowMaximumButtonState(false);
}

void TopLevelWindowTitleBarResizeMoveControl_p::ShowMinimized()
{
	m_toplevelWindow->showMinimized();
}

void TopLevelWindowTitleBarResizeMoveControl_p::CloseWindow()
{
	m_toplevelWindow->close();
}

//////////////////////////////////////////////////////////////////////////
DashlineWidget::DashlineWidget(QWidget *parent)
:QWidget(parent)
{
	setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
	setAttribute(Qt::WA_TranslucentBackground);
	m_color = QColor(220, 220, 220, 180);

	HideDashline();
}

void DashlineWidget::SetColor(QColor color)
{
	m_color = color;
}

void DashlineWidget::MovePosition(QRect r)
{
	setGeometry(r);
	if (!isVisible())
		ShowDashLine();
}

void DashlineWidget::HideDashline()
{
	hide();
}

void DashlineWidget::ShowDashLine()
{
	show();
}

void DashlineWidget::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	QPen pen = painter.pen();
	//pen.setStyle(Qt::DashLine);
	pen.setWidth(8);
	pen.setColor(m_color);
	painter.setPen(pen);

	painter.drawRect(rect());
}


//////////////////////////////////////////////////////////////////////////
SystemMenuControl::SystemMenuControl(TopLevelWindowTitleBar_p *parent)
:QObject(parent)
, m_topLevelWindowTitleBar_p(parent)
, m_enabled(false)
{
	m_topLevelWindow = parent->parentWidget()->parentWidget();

}

SystemMenuControl::~SystemMenuControl()
{
	qApp->removeNativeEventFilter(this);
}

bool SystemMenuControl::PopupMenu(QPoint pos)
{
	if (m_enabled)
	{
#ifdef WIN32
		auto icon = m_topLevelWindowTitleBar_p->findChild<QLabel*>("TopLevelWindowTitleBar_p_icon");
		Q_ASSERT(icon);
		if (icon->isVisible() && icon->geometry().contains(icon->mapFromGlobal(pos)))
		{
			m_popupTick = GetTickCount();
			pos = icon->mapToGlobal(QPoint(0, icon->height()));
			DWORD WM_POPUPSYSTEMMENU = 0x313;
			::SendMessage((HWND)m_topLevelWindowTitleBar_p->GetConrtol()->GetTopLevelWindow()->winId(), WM_POPUPSYSTEMMENU, 0, MAKELONG(pos.x(), pos.y()));
			return true;
		}

#endif
	}
	return false;
}

bool SystemMenuControl::nativeEventFilter(const QByteArray & eventType, void * message, long * result)
{
	if (m_enabled)
	{
#ifdef WIN32
		MSG *msg = (MSG*)(message);

		if (msg->message == WM_SYSCOMMAND)
		{
			if (msg->wParam == RESTORE || msg->wParam == MAXIMIZE || msg->wParam == MINIMIZE)
			{
				emit triggered(msg->wParam);
				return true;
			}
		}
		if (msg->message == WM_UNINITMENUPOPUP)
		{
			if (GetTickCount() - m_popupTick <= GetDoubleClickTime())
			{
				POINT pt;
				GetCursorPos(&pt);
				QPoint pos(pt.x, pt.y);
				auto icon = m_topLevelWindowTitleBar_p->findChild<QLabel*>("TopLevelWindowTitleBar_p_icon");
				Q_ASSERT(icon);
				if (icon->isVisible() && icon->geometry().contains(icon->mapFromGlobal(pos)))
				{
					PostMessage((HWND)m_topLevelWindow->winId(), WM_CLOSE, 0, 0);
					return true;
				}
			}
		}
#endif
	}

	return false;
}

void SystemMenuControl::EnabledMenu(int id, bool enabled)
{
#ifdef WIN32
	auto hMenu = GetSystemMenu((HWND)m_topLevelWindow->winId(), false);
	EnableMenuItem(hMenu, id, MF_BYCOMMAND | (enabled ? MF_ENABLED : MF_DISABLED));
#endif
}

void SystemMenuControl::UpdateMenuState()
{
	if (m_enabled)
	{
		bool maximized = m_topLevelWindowTitleBar_p->GetConrtol()->IsMaximized();
		EnabledMenu(RESTORE, maximized);
		EnabledMenu(MAXIMIZE, !maximized);
	}

}

void SystemMenuControl::SetEnabled(bool enabled)
{
	m_enabled = enabled;
	if (m_enabled)
	{
#ifdef WIN32
		WCHAR szMinText[MAX_PATH] = { 0 }, szMaxText[MAX_PATH] = { 0 }, szResText[MAX_PATH] = { 0 };
		auto hMenu = GetSystemMenu((HWND)m_topLevelWindow->winId(), false);
		GetMenuString(hMenu, SC_MINIMIZE, szMinText, MAX_PATH - 1, MF_BYCOMMAND);
		GetMenuString(hMenu, SC_MAXIMIZE, szMaxText, MAX_PATH - 1, MF_BYCOMMAND);
		GetMenuString(hMenu, SC_RESTORE, szResText, MAX_PATH - 1, MF_BYCOMMAND);
		DeleteMenu(hMenu, SC_MINIMIZE, MF_BYCOMMAND);
		DeleteMenu(hMenu, SC_MAXIMIZE, MF_BYCOMMAND);
		DeleteMenu(hMenu, SC_RESTORE, MF_BYCOMMAND);
		EnableMenuItem(hMenu, SC_CLOSE, MF_BYCOMMAND | MF_ENABLED);


		auto CreateMenu = [&](HMENU hMenu, int id, int pos, LPWSTR text, HBITMAP bitmap)
		{
			MENUITEMINFO MenuInfo = { 0 };
			MenuInfo.cbSize = sizeof(MENUITEMINFO);
			MenuInfo.fMask = MIIM_ID | MIIM_STRING | MIIM_STATE | MIIM_BITMAP;
			MenuInfo.fType = MFT_STRING | MFT_BITMAP;
			MenuInfo.fState = MFS_ENABLED;
			MenuInfo.wID = id;
			MenuInfo.dwTypeData = text;
			//MenuInfo.cch = 2;
			MenuInfo.hbmpItem = bitmap;
			//MenuInfo.hbmpChecked = (HBITMAP)m_Bitmap[1].m_hObject;
			//MenuInfo.hbmpUnchecked = (HBITMAP)m_Bitmap[2].m_hObject;

			InsertMenuItem(hMenu, pos, true, &MenuInfo);
		};

		Q_GUI_EXPORT HBITMAP qt_pixmapToWinHBITMAP(const QPixmap &p, int hbitmapFormat = 0);
		CreateMenu(hMenu, RESTORE, 0, szResText, qt_pixmapToWinHBITMAP(QPixmap(":/CommonLib/Resources/normal/toplevelwindow_menu_restore.png"), 2));
		CreateMenu(hMenu, MAXIMIZE, 3, szMaxText, qt_pixmapToWinHBITMAP(QPixmap(":/CommonLib/Resources/normal/toplevelwindow_menu_maxmize.png"), 2));
		CreateMenu(hMenu, MINIMIZE, 3, szMinText, qt_pixmapToWinHBITMAP(QPixmap(":/CommonLib/Resources/normal/toplevelwindow_menu_minimize.png"), 2));

		UpdateMenuState();


		connect(this, &SystemMenuControl::triggered, [&](int id){

			auto control = m_topLevelWindowTitleBar_p->GetConrtol();
			if (id == RESTORE)
			{
				control->ShowNormal();
			}
			else if (id == MAXIMIZE)
			{
				control->ShowMaximized();
			}
			else if (id == MINIMIZE)
			{
				control->ShowMinimized();
			}
			UpdateMenuState();
		});


		//SetWindowLong((HWND)m_topLevelWindow->winId(), GWL_STYLE, GetWindowLong((HWND)m_topLevelWindow->winId(), GWL_STYLE) & ~WS_CAPTION);

		//SetWindowLong((HWND)parent->winId(), GWL_STYLE, GetWindowLong((HWND)parent->winId(), GWL_STYLE) &(~WS_CAPTION) | WS_DLGFRAME | WS_OVERLAPPED);
		//::SetWindowPos((HWND)m_topLevelWindow->winId(), NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);

		qApp->installNativeEventFilter(this);
#endif

	}
}
