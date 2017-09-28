#include "stdafx.h"
#include "FSTextShadowSelecter.h"
#include "inc_CommonLib/TopLevelWindowTitleBar.h"

FSTextShadowSelecter::FSTextShadowSelecter(QWidget *parent)
    : QWidget(parent)
    , m_backgroundColor("#13181B")
{
    m_pButtonDrag = new QPushButton(this);
    m_pButtonDrag->setFocusPolicy(Qt::NoFocus);
    m_pButtonDrag->setCheckable(true);
    m_pShadowSelecter = new FSShadowSelecter(m_pButtonDrag);
    m_pShadowSelecter->SetBackGroundColor(m_backgroundColor);
    resize(50, 40);
    m_pButtonDrag->setObjectName("ComboBoxColorSelect_m_pButtonDrag");


    connect(m_pButtonDrag, &QPushButton::clicked, [this](){
        if (m_pButtonDrag->isChecked())
        {
            m_pShadowSelecter->move(mapToGlobal(QPoint(0, m_pButtonDrag->geometry().bottom())));
            m_pShadowSelecter->exec();
        }
    });

    connect(m_pShadowSelecter, &FSShadowSelecter::CurentIndexChanged, [this](int index){
        m_pShadowSelecter->done(0);
        m_pButtonDrag->setChecked(false);
        m_CurrentIndex = index;
        update();
        emit CurentIndexChanged(GetCurrentIndex());
    });

    m_pShadowSelecter->installEventFilter(this);
}

FSTextShadowSelecter::~FSTextShadowSelecter()
{

}

void FSTextShadowSelecter::SetBackgroundColor(QColor color)
{
    m_backgroundColor = color;
}

QColor FSTextShadowSelecter::GetBackgroundColor()
{
    return m_backgroundColor;
}

void FSTextShadowSelecter::SetCurrentIndex(int index)
{
    m_CurrentIndex = qBound(0, index, 11);
    m_pShadowSelecter->SetCurrentIndex(m_CurrentIndex);
    update();
}

int FSTextShadowSelecter::GetCurrentIndex()
{
    return (m_CurrentIndex==11)?m_CurrentIndex+1:m_CurrentIndex;
}

void FSTextShadowSelecter::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawImage(0,0,m_pShadowSelecter->GetCurrentItemImage().scaled(width()-10,height(),Qt::KeepAspectRatio,Qt::SmoothTransformation));
}

void FSTextShadowSelecter::mousePressEvent(QMouseEvent *)
{
    m_pButtonDrag->setChecked(!m_pButtonDrag->isChecked());
    m_pButtonDrag->clicked();
}

bool FSTextShadowSelecter::eventFilter(QObject *obj, QEvent *e)
{
    if (obj == m_pShadowSelecter&&e->type() == QEvent::MouseButtonPress)
    {
        if (!rect().contains(mapFromGlobal(m_pShadowSelecter->mapToGlobal(static_cast<QMouseEvent *>(e)->pos()))))
            m_pButtonDrag->setChecked(false);
    }
    return false;
}


void FSTextShadowSelecter::resizeEvent(QResizeEvent *e)
{
    m_pButtonDrag->setGeometry(width() - 15, 0, 15, height());
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
FSShadowSelecter::FSShadowSelecter(QWidget *pParent)
    :FSBaseDialog("",pParent)
    , m_BoxWidth(40)
    , m_BoxSpace(4)
{
    CreateUI();
    setMouseTracking(true);
}

FSShadowSelecter::~FSShadowSelecter()
{

}

void FSShadowSelecter::SetCurrentIndex(int index)
{
    m_CurrentIndex = qBound(0, index, 11);
}

void FSShadowSelecter::SetBackGroundColor(QColor color)
{
    m_backGroundColor = color;
}

QImage FSShadowSelecter::GetCurrentItemImage()
{
    return GetItemImage(m_CurrentIndex / 4, m_CurrentIndex % 4, { m_BoxWidth, m_BoxWidth });
}

void FSShadowSelecter::paintEvent(QPaintEvent *e)
{

    QPainter painter(this);
    painter.fillRect(rect(), m_backGroundColor);
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            painter.drawImage(j*m_BoxWidth + (j + 1)*m_BoxSpace, i*m_BoxWidth + (i + 1)*m_BoxSpace, GetItemImage(i, j, { m_BoxWidth, m_BoxWidth }));
        }
    }
    QPen pen;
    pen.setWidth(2);
    pen.setColor(Qt::red);
    painter.setPen(pen);
    painter.drawRect(m_Rect);
}


void FSShadowSelecter::mouseMoveEvent(QMouseEvent *e)
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            QRect RC = QRect(j*m_BoxWidth + (j + 1)*m_BoxSpace, i*m_BoxWidth + (i + 1)*m_BoxSpace, m_BoxWidth, m_BoxWidth);
            if (RC.contains(e->pos()))
            {
                m_Rect = RC.adjusted(-1, -1, 1, 1);
                update();
            }
                
        }
    }
    return QWidget::mouseMoveEvent(e);
}

void FSShadowSelecter::mousePressEvent(QMouseEvent *e)
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            QRect RC = QRect(j*m_BoxWidth + (j + 1)*m_BoxSpace, i*m_BoxWidth + (i + 1)*m_BoxSpace, m_BoxWidth, m_BoxWidth);
            if (RC.contains(e->pos()))
            {
                m_CurrentIndex = i * 4 + j;
                emit CurentIndexChanged(m_CurrentIndex);
                setVisible(false);
                break;
            }
        }
    }
    return QWidget::mousePressEvent(e);
}


void FSShadowSelecter::CreateUI()
{
    m_titlebar->setVisible(false);
    m_Image.load(":/FSMediaPlayerView/Resources/light/Shadow.png");
    setFixedSize(m_BoxWidth * 4 + m_BoxSpace * 5, m_BoxWidth * 3 + m_BoxSpace * 4);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint | Qt::Popup);
}

QImage FSShadowSelecter::GetItemImage(int row, int col, QSize size)
{
    QSize imageSize = m_Image.size();
    int w = imageSize.width()  / 4;
    int h = imageSize.height() / 3;
    row = qBound(0, row, 2);
    col = qBound(0, col, 3);
    return m_Image.copy(col*w, row*h, w, h).scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
}
