#include "stdafx.h"
#include "FMatterThumbnail.h"
#include "inc_FCommonLib/FCommonWidgets.h"
#include <QPainter>

/*FMatterThumbnailBase*/
FMatterThumbnailBase::FMatterThumbnailBase(QWidget *parent)
	: FBaseWidget(parent)
	, m_bHover(false)
	, m_bClicked(false)
	, m_bAddable(false)
	, m_bEditable(true)
	, m_bTryEdit(false)
	, m_bBeginEdit(false)
{
	setMouseTracking(true);
	m_pTitleLbl = new FLabel(this);
	m_pTitleLbl->setAlignment(Qt::AlignLeft);
	m_pTitleLineEdit = new FLineEdit(this);
	m_pTitleLineEdit->hide();
	connect(m_pTitleLineEdit, &FLineEdit::editingFinished, this, &FMatterThumbnailBase::slotEditingFinished);
	connect(m_pTitleLineEdit, &FLineEdit::returnPressed, this, &FMatterThumbnailBase::slotEditingFinished);
}

FMatterThumbnailBase::~FMatterThumbnailBase()
{

}

void FMatterThumbnailBase::SetTitle(const QString& title)
{
	m_TitleStr = title;
	m_pTitleLbl->setText(GetDisplayString(m_TitleStr));
	m_pTitleLineEdit->setText(title);
}

void FMatterThumbnailBase::SetEditable(bool editable)
{
	m_bEditable = editable;
}

void FMatterThumbnailBase::Click()
{
	m_bClicked = true;
	m_bHover = true;
	update();
}

void FMatterThumbnailBase::Unclick()
{
	m_bClicked = false;
	m_bHover = false;
	update();
}

void FMatterThumbnailBase::Rename()
{
	if (!m_bEditable) return;
	m_pTitleLbl->hide();
	m_pTitleLineEdit->selectAll();
	m_pTitleLineEdit->setFocus();
	m_pTitleLineEdit->show();
}

void FMatterThumbnailBase::paintEvent(QPaintEvent *)
{
	QPainter p(this);
	p.setRenderHint(QPainter::Antialiasing, true);
	// 绘制缩略图
	p.drawPixmap(m_ThumbnailRect, QPixmap(":/FMatterView/Resources/Thumbnail.png"));  // for test
	// 绘制bottomline
	if (m_bHover || m_bClicked){
		p.fillRect(m_BottomlineRect, QColor(226, 91, 92));
	}
	else {
		p.fillRect(m_BottomlineRect, QColor(87, 192, 201));
	}
	// 绘制中心圆
	if (m_bAddable){
		p.setBrush(QColor(226, 91, 92));
		p.drawEllipse(m_AddPt, 10, 10);
		
		p.drawLine(QPoint(m_AddPt.x() - 3, m_AddPt.y()), QPoint(m_AddPt.x() + 3, m_AddPt.y()));
		p.drawLine(QPoint(m_AddPt.x(), m_AddPt.y() - 3), QPoint(m_AddPt.x(), m_AddPt.y() + 3));
	}
	else if (m_bHover){
		p.setBrush(QColor(87, 192, 201));
		p.drawEllipse(m_AddPt, 10, 10);
		
		p.drawLine(QPoint(m_AddPt.x() - 3, m_AddPt.y()), QPoint(m_AddPt.x() + 3, m_AddPt.y()));
		p.drawLine(QPoint(m_AddPt.x(), m_AddPt.y() - 3), QPoint(m_AddPt.x(), m_AddPt.y() + 3));
	}
}

void FMatterThumbnailBase::resizeEvent(QResizeEvent *)
{
	int left, top;
	const int TitleHeight = 25;
	const int BottomLineHeight = 3;
	left = 0, top = height() - TitleHeight;
	m_pTitleLbl->setGeometry(left, top, width(), TitleHeight);
	m_pTitleLineEdit->setGeometry(left, top, width(), TitleHeight);
	top = top - BottomLineHeight - 5;
	m_BottomlineRect = QRect(left, top, width(), BottomLineHeight);
	top = m_BottomlineRect.top();
	m_ThumbnailRect = QRect(left, 0, width(), top);
	m_AddPt = QPointF((m_ThumbnailRect.left() + m_ThumbnailRect.right()) / 2.0, (m_ThumbnailRect.top() + m_ThumbnailRect.bottom()) / 2.0);
}

void FMatterThumbnailBase::mousePressEvent(QMouseEvent *e)
{
	m_bClicked = true;
	emit sigClicked();
	if (m_bAddable){
		emit sigAdd();
	}
	if (m_pTitleLbl->geometry().contains(e->pos())){
		if (m_bTryEdit){
			qint64 timeGap = qAbs(m_TryEditTime.msecsTo(QDateTime::currentDateTime()));
			if (timeGap < 1000*1.5){  // 两次点击间隔小于1秒
				m_bBeginEdit = true;
			}
			else{
				m_bBeginEdit = false;
			}
		}
		else{
			m_bTryEdit = true;
		}
		m_TryEditTime = QDateTime::currentDateTime();  // 记录当前时间
	}
	else{
		m_bTryEdit = false;
		m_bBeginEdit = false;
	}
	update();
}

void FMatterThumbnailBase::mouseReleaseEvent(QMouseEvent *)
{
	if (m_bBeginEdit){
		Rename();
	}
}

void FMatterThumbnailBase::mouseMoveEvent(QMouseEvent *e)
{
	QPoint pt = e->pos();
	qreal r = qSqrt(qPow(pt.x() - m_AddPt.x(), 2) + qPow(pt.y() - m_AddPt.y(), 2));
	if (r < 10){
		m_bAddable = true;
	}
	else{
		m_bAddable = false;
	}
	update();
}

void FMatterThumbnailBase::mouseDoubleClickEvent(QMouseEvent *)
{
	emit sigDoubleClicked();
}

void FMatterThumbnailBase::enterEvent(QEvent *)
{
	m_bHover = true;
	update();
}

void FMatterThumbnailBase::leaveEvent(QEvent *)
{
	m_bHover = false;
	update();
}

QString FMatterThumbnailBase::GetDisplayString(QString str)
{
	QString elidedStr = m_pTitleLbl->fontMetrics().elidedText(str, Qt::ElideRight, m_pTitleLbl->width(), Qt::TextShowMnemonic);
	return elidedStr;
}

void FMatterThumbnailBase::slotEditingFinished()
{
	m_TitleStr = m_pTitleLineEdit->text();
	m_pTitleLineEdit->hide();
	m_pTitleLbl->setText(GetDisplayString(m_TitleStr));
	m_pTitleLbl->show();
	emit sigTitleChanged(m_TitleStr);
}

/*FThumbnailDownload*/
FThumbnailDownload::FThumbnailDownload(QWidget* parent /*= nullptr*/)
	:FMatterThumbnailBase(parent)
{
	setToolTipDuration(3000);
	setToolTip(m_TitleStr + tr("Double click to download the online content"));
	m_pProgressBar = new QProgressBar(this);
	m_pProgressBar->setTextVisible(false);
	//m_pProgressBar->hide();
	SetEditable(false);  // 不可编辑
}

FThumbnailDownload::~FThumbnailDownload()
{

}

void FThumbnailDownload::resizeEvent(QResizeEvent *e)
{
	FMatterThumbnailBase::resizeEvent(e);
	int top = m_BottomlineRect.top();
	top = top - 5;
	m_pProgressBar->setGeometry(2, top, width() - 4, 3);
}

/*FMediaThumbnail*/
FMediaThumbnail::FMediaThumbnail(QWidget* parent /*= nullptr*/)
	:FMatterThumbnailBase(parent)
{
	m_pActionCamLbl = new FLabel(this);
	m_pActionCamLbl->setObjectName("FMediaThumbnail_ActionCam");
	m_pVideoLbl = new FLabel(this);
	m_pVideoLbl->setObjectName("FMediaThumbnail_Video");
	m_pActionCamLbl->hide();
	setToolTipDuration(3000);
}

FMediaThumbnail::~FMediaThumbnail()
{

}

void FMediaThumbnail::resizeEvent(QResizeEvent *e)
{
	FMatterThumbnailBase::resizeEvent(e);
	m_pActionCamLbl->setGeometry(0, 0, 24, 20);
	m_pVideoLbl->setGeometry(width() - 24, 0, 24, 20);
}

void FMediaThumbnail::mouseReleaseEvent(QMouseEvent *e)
{
	FMatterThumbnailBase::mouseReleaseEvent(e);
	if (m_pActionCamLbl->geometry().contains(e->pos())){
		emit sigEditInActionCam();
	}
}

void FMediaThumbnail::mouseMoveEvent(QMouseEvent *e)
{
	FMatterThumbnailBase::mouseMoveEvent(e);
	if (m_pActionCamLbl->geometry().contains(e->pos())){
		setToolTip(tr("Edit in Action Cam Tool"));
	}
	else{
		setToolTip(m_TitleStr);
	}
}

void FMediaThumbnail::enterEvent(QEvent *e)
{
	FMatterThumbnailBase::enterEvent(e);
	m_pActionCamLbl->show();
}

void FMediaThumbnail::leaveEvent(QEvent *e)
{
	FMatterThumbnailBase::leaveEvent(e);
	m_pActionCamLbl->hide();
}

