#ifndef FSTEXTSHADOWSELECTER_H
#define FSTEXTSHADOWSELECTER_H

#include <QtWidgets/QWidget>
#include "inc_FSCommonlib/FSCommonWidget.h"

class QPushButton;
class FSShadowSelecter;
class FSTextShadowSelecter : public QWidget
{
    Q_OBJECT

signals :
    void CurentIndexChanged(int index);

public:
    FSTextShadowSelecter(QWidget *parent);
    ~FSTextShadowSelecter();

    void SetBackgroundColor(QColor color);
    QColor GetBackgroundColor();

    void SetCurrentIndex(int index);
    int GetCurrentIndex();

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    bool eventFilter(QObject *, QEvent *);
    void resizeEvent(QResizeEvent *e);

private:
    QPushButton        *m_pButtonDrag;
    FSShadowSelecter   *m_pShadowSelecter;
    int                m_CurrentIndex;
    QColor             m_backgroundColor;
    
};

class FSShadowSelecter :public FSBaseDialog
{
    Q_OBJECT
signals:
    void CurentIndexChanged(int index);

public:
    FSShadowSelecter(QWidget *pParent);
    ~FSShadowSelecter();

    void SetCurrentIndex(int index);
    void SetBackGroundColor(QColor color);

    QImage GetCurrentItemImage();

protected:
    void paintEvent(QPaintEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);

private:
    void CreateUI();

    QImage GetItemImage(int row, int col,QSize size);

private:
    QImage m_Image;
    int    m_BoxWidth;
    int    m_BoxSpace;
    int    m_CurrentIndex;
    QColor m_backGroundColor;
    QRect  m_Rect;
};

#endif // FSTEXTSHADOWSELECTER_H
