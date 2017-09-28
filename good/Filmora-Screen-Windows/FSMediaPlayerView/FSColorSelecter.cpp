#include "stdafx.h"
#include "FSColorSelecter.h"
#include "inc_CommonLib/TopLevelWindowTitleBar.h"



FSColorSelecter::FSColorSelecter(QWidget *parent)
    : FSBaseDialog("",parent)
    , m_ColorBoxWidth(13)
    , m_ColorBoxSpace(2)
{
    CreateUI();
    InitColorList();
    setFixedSize(m_ColorBoxWidth * 8 + m_ColorBoxSpace * 9, m_ColorBoxWidth * 6 + m_ColorBoxSpace * 7 + 23);
    setMouseTracking(true);
    RetranslateUi();
}

FSColorSelecter::~FSColorSelecter()
{

}

void FSColorSelecter::AddColor(QColor color)
{

}

QColor FSColorSelecter::GetCurrentColor()
{
    return m_CurrentColor;
}

void FSColorSelecter::SetCurrentColor(QColor color)
{
    m_CurrentColor = color;
    m_pLabelCurrentColor->SetColor(color);
}


void FSColorSelecter::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.fillRect(QRect(0, 0, width(), height()), QColor("#13171B"));

    for (int i = 0; i < m_ColorList.size(); i++)
    {
        painter.fillRect(m_BoxRectMap.value(m_ColorList.at(i)), QColor(m_ColorList.at(i)));
    }
    painter.setPen(QColor("#ff0000"));
    painter.drawRect(m_HonverRect);
}


void FSColorSelecter::resizeEvent(QResizeEvent *e)
{
    m_pButtonMore->setGeometry(m_ColorBoxSpace, 6 * m_ColorBoxWidth + 7 * m_ColorBoxSpace+4, width() - 30 - 3 * m_ColorBoxSpace, 14);
    m_pLabelCurrentColor->setGeometry(m_pButtonMore->geometry().right() + m_ColorBoxSpace, 6 * m_ColorBoxWidth + 7 * m_ColorBoxSpace, 30, 20);
}

void FSColorSelecter::mouseMoveEvent(QMouseEvent *e)
{

    for (int i = 0; i < m_ColorList.size(); i++)
    {
        if (m_BoxRectMap.value(m_ColorList.at(i)).contains(e->pos()))
        {
            m_HonverRect = m_BoxRectMap.value(m_ColorList.at(i));
            update();
            break;
        }
    }
    return QWidget::mouseMoveEvent(e);
}

void FSColorSelecter::mousePressEvent(QMouseEvent *e)
{
    for (int i = 0; i < m_ColorList.size(); i++)
    {
        if (m_BoxRectMap.value(m_ColorList.at(i)).contains(e->pos()))
        {
            m_CurrentColor = QColor(m_ColorList.at(i));
            emit CurrentColorChanged(m_CurrentColor);
            setVisible(false);
            break;
        }
    }
    return QWidget::mousePressEvent(e);
}

void FSColorSelecter::RetranslateUi()
{
    m_pButtonMore->setText(tr("More..."));
}

void FSColorSelecter::CreateUI()
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint | Qt::Popup);
    m_pButtonMore        = new FSButton(this);
    m_pLabelCurrentColor = new FSColorItem(m_CurrentColor,this);

    connect(m_pLabelCurrentColor,&FSColorItem::clicked, [this](){
        emit CurrentColorChanged(m_CurrentColor);
        setVisible(false);
    });

    m_pLabelCurrentColor->setFixedSize(30, 20);
    m_pButtonMore->setFixedHeight(14);
    connect(m_pButtonMore, &FSButton::clicked, [this](){
        QColor color=QColorDialog::getColor(Qt::white, nullptr, "");
        m_CurrentColor = color.isValid() ? color : m_CurrentColor;
        emit CurrentColorChanged(m_CurrentColor);      
    });
    m_pButtonMore->setObjectName("FSColorSelecter_m_pButtonMore");
    m_titlebar->setVisible(false);
}

void FSColorSelecter::InitColorList()
{
    m_ColorList.push_back( "#000000");
    m_ColorList.push_back( "#aa0000");
    m_ColorList.push_back( "#005500");
    m_ColorList.push_back( "#aa5500");
    m_ColorList.push_back( "#00aa00");
    m_ColorList.push_back( "#aaaa00");
    m_ColorList.push_back( "#00ff00");
    m_ColorList.push_back( "#aaff00");

    m_ColorList.push_back( "#00007f");
    m_ColorList.push_back( "#aa007f");
    m_ColorList.push_back( "#00557f");
    m_ColorList.push_back( "#aa557f");
    m_ColorList.push_back( "#00aa7f");
    m_ColorList.push_back( "#aaaa7f");
    m_ColorList.push_back( "#00ff7f");
    m_ColorList.push_back( "#aaff7f");

    m_ColorList.push_back( "#0000ff");
    m_ColorList.push_back( "#aa00ff");
    m_ColorList.push_back( "#0055ff");
    m_ColorList.push_back( "#aa55ff");
    m_ColorList.push_back( "#00aaff");
    m_ColorList.push_back( "#aaaaff");
    m_ColorList.push_back( "#00ffff");
    m_ColorList.push_back( "#aaffff");

    m_ColorList.push_back( "#550000");
    m_ColorList.push_back( "#ff0000");
    m_ColorList.push_back( "#555500");
    m_ColorList.push_back( "#ff5500");
    m_ColorList.push_back( "#55aa00");
    m_ColorList.push_back( "#ffaa00");
    m_ColorList.push_back( "#55ff00");
    m_ColorList.push_back( "#ffff00");

    m_ColorList.push_back( "#55007f");
    m_ColorList.push_back( "#ff007f");
    m_ColorList.push_back( "#55557f");
    m_ColorList.push_back( "#ff557f");
    m_ColorList.push_back( "#55aa7f");
    m_ColorList.push_back( "#ffaa7f");
    m_ColorList.push_back( "#55ff7f");
    m_ColorList.push_back( "#ffff7f");

    m_ColorList.push_back( "#5500ff");
    m_ColorList.push_back( "#ff00ff");
    m_ColorList.push_back( "#5555ff");
    m_ColorList.push_back( "#ff55ff");
    m_ColorList.push_back( "#55aaff");
    m_ColorList.push_back( "#ffaaff");
    m_ColorList.push_back( "#55ffff");
    m_ColorList.push_back( "#ffffff");
    for (int i = 0; i < m_ColorList.size(); i++)
    {           
        m_BoxRectMap[m_ColorList.at(i)]=QRect(i % 8 * m_ColorBoxWidth + (i % 8 + 1)*m_ColorBoxSpace, i / 8 * m_ColorBoxWidth + (i / 8 + 1)*m_ColorBoxSpace, m_ColorBoxWidth, m_ColorBoxWidth);
    }
}


///////////////////////////////////////////////////////////////////////////////////////////////
FSComboBoxColorSelect::FSComboBoxColorSelect(QWidget *pParent)
    :QWidget(pParent)
    , m_CurrentColor(QColor("#000000"))
{
    m_pButtonDrag    = new FSButton(this);
    m_pButtonDrag->setCheckable(true);
    m_pColorSelecter = new FSColorSelecter(m_pButtonDrag);
    setFixedSize(60, 20);
    m_pButtonDrag->setFixedSize(20, 20);
    m_pButtonDrag->setObjectName("FSComboBoxColorSelect_m_pButtonDrag");

    connect(m_pButtonDrag, &FSButton::clicked, [this](){
        if (m_pButtonDrag->isChecked())
        {
            m_pColorSelecter->move(mapToGlobal(QPoint(0, m_pButtonDrag->geometry().bottom())));
            m_pColorSelecter->SetCurrentColor(m_CurrentColor);
            m_pColorSelecter->exec();
        }       
    });

    connect(m_pColorSelecter, &FSColorSelecter::CurrentColorChanged, [this](QColor color){
        m_pColorSelecter->done(0);
        m_pButtonDrag->setChecked(false);
        m_CurrentColor = color;
        update();
        emit CurrentColorChanged(m_CurrentColor);
    });

    m_pColorSelecter->installEventFilter(this);
}

FSComboBoxColorSelect::~FSComboBoxColorSelect()
{

}

void FSComboBoxColorSelect::SetCurrentColor(QColor color)
{
    m_CurrentColor = color;
    m_pColorSelecter->SetCurrentColor(color);
    update();
}

QColor FSComboBoxColorSelect::GetCurrentColor()
{
    return m_CurrentColor;
}

void FSComboBoxColorSelect::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.setPen(QColor("#27313B"));
    painter.fillRect(rect(), QColor("#13181B"));
    painter.drawRect(QRect(0, 0, width()-1,height()-1));
    painter.fillRect(QRect(5,4,width()-m_pButtonDrag->width()-10,height()-8), m_CurrentColor);
}

void FSComboBoxColorSelect::resizeEvent(QResizeEvent *e)
{
    m_pButtonDrag->setGeometry(width() - m_pButtonDrag->width(), 0, 20, 20);
}

void FSComboBoxColorSelect::mousePressEvent(QMouseEvent *)
{
    m_pButtonDrag->setChecked(!m_pButtonDrag->isChecked());
    m_pButtonDrag->clicked();
}

bool FSComboBoxColorSelect::eventFilter(QObject *obj, QEvent *e)
{
    if (obj == m_pColorSelecter&&e->type() == QEvent::MouseButtonPress)
    {
        if (!rect().contains(mapFromGlobal(m_pColorSelecter->mapToGlobal(static_cast<QMouseEvent *>(e)->pos()))))
            m_pButtonDrag->setChecked(false);
    }
    return false;
}

FSColorItem::FSColorItem(QColor color, QWidget *pParent)
    :QPushButton(pParent)
    , m_CurrentColor(color)
    , m_BorderColor(QColor("#909090"))
{
    setFixedSize(13, 13);
}

FSColorItem::~FSColorItem()
{

}

QColor FSColorItem::GetColor()
{
    return m_CurrentColor;
}

void FSColorItem::SetColor(QColor color)
{
    m_CurrentColor = color;
}

void FSColorItem::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.fillRect(this->rect(),m_CurrentColor);
    painter.setPen(m_BorderColor);
    painter.drawRect(0, 0, width() - 1, height() - 1);
    QPen pen(QColor("#13171B"));
    pen.setWidth(3);
    painter.setPen(pen);
    painter.drawRect(1, 1, width() - 2, height() - 2);
}

void FSColorItem::enterEvent(QEvent *e)
{
    m_BorderColor = QColor("#ff0000");
    update();
    return QPushButton::enterEvent(e);
}

void FSColorItem::leaveEvent(QEvent *e)
{
    m_BorderColor = QColor("#ff0000");
    update();
    return QPushButton::leaveEvent(e);
}
