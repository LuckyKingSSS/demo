#ifndef FSCOLORSELECTER_H
#define FSCOLORSELECTER_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QPushButton>
#include <QtGui/QColor>

#include "inc_CommonLib/commonlib_global.h"


class ColorSelecter;
class COMMONLIB_EXPORT ColorSelecterComboBox :public QWidget{
    Q_OBJECT

public:
    ColorSelecterComboBox(QWidget *pParent);
    ~ColorSelecterComboBox();

    void SetCurrentColor(QColor color);
    QColor GetCurrentColor();

    void SetBackgroundColor(QColor color);
    QColor GetBackgroundColor();

Q_SIGNALS:
    void CurrentColorChanged(QColor color);

protected:
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *);
    void mousePressEvent(QMouseEvent *);
    bool eventFilter(QObject *, QEvent *);

private:
    QPushButton        *m_pButtonDrag;
    ColorSelecter *m_pColorSelecter;
    QColor           m_CurrentColor;
    QColor          m_backgroundColor;
};

#endif // FSCOLORSELECTER_H
