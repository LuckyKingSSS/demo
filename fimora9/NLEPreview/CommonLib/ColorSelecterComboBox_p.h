#ifndef COLORSELECTERWIDGET_P
#define COLORSELECTERWIDGET_P

#include <QtWidgets/QPushButton>
#include <QtCore/QMap>
#include "inc_CommonLib/BaseDialog.h"


class ColorSelecterItem :public QPushButton{
    Q_OBJECT
public:
    ColorSelecterItem(QColor color, QWidget *pParent);
    ~ColorSelecterItem();

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


class ColorSelecter : public BaseDialog
{
    Q_OBJECT

signals :
    void CurrentColorChanged(QColor color);

public:
    ColorSelecter(QWidget *parent);
    ~ColorSelecter();

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
    QPushButton     *m_pButtonMore;
    ColorSelecterItem  *m_pLabelCurrentColor;

    QColor         m_CurrentColor;
    QList<QString> m_ColorList;
    int            m_ColorBoxWidth;
    int            m_ColorBoxSpace;
    QMap<QString, QRect> m_BoxRectMap;
    QRect          m_HonverRect;

};

#endif