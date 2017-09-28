#ifndef FSCOLORSELECTER_H
#define FSCOLORSELECTER_H

#include <QtWidgets/QDialog>
#include <QtWidgets/QPushButton>
#include "inc_FSCommonlib/FSCommonWidget.h"

class FSColorItem :public QPushButton{
    Q_OBJECT
public:
    FSColorItem(QColor color,QWidget *pParent);
    ~FSColorItem();

    QColor GetColor();
    void SetColor(QColor color);

protected:
    void paintEvent(QPaintEvent *e);
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);

private:
    QColor m_CurrentColor;
    QColor m_BorderColor;
};

class FSButton;
class QLabel;
class QListWidget;
class QGridLayout;
class FSColorSelecter : public FSBaseDialog
{
    Q_OBJECT

signals:
    void CurrentColorChanged(QColor color);

public:
    FSColorSelecter(QWidget *parent);
    ~FSColorSelecter();

    void AddColor(QColor color);

    QColor GetCurrentColor();
    void SetCurrentColor(QColor color);


protected:
    void paintEvent(QPaintEvent *e);
    void resizeEvent(QResizeEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void RetranslateUi();

private:
    void CreateUI();
    void InitColorList();

private:
    FSButton     *m_pButtonMore;
    FSColorItem  *m_pLabelCurrentColor;

    QColor         m_CurrentColor;
    QList<QString> m_ColorList;
    int            m_ColorBoxWidth;
    int            m_ColorBoxSpace;
    QMap<QString,QRect> m_BoxRectMap;
    QRect          m_HonverRect;
    
};


class FSComboBoxColorSelect :public QWidget{
    Q_OBJECT

signals :
    void CurrentColorChanged(QColor color);

public:
    FSComboBoxColorSelect(QWidget *pParent);
    ~FSComboBoxColorSelect();

    void SetCurrentColor(QColor color);

    QColor GetCurrentColor();

protected:
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *);
    void mousePressEvent(QMouseEvent *);
    bool eventFilter(QObject *, QEvent *);

private:
    FSButton        *m_pButtonDrag;
    FSColorSelecter *m_pColorSelecter;
    QColor           m_CurrentColor;
};

#endif // FSCOLORSELECTER_H
