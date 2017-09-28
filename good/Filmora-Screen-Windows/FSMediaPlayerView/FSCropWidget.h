#ifndef FSVIEWCUTWIDGET_H
#define FSVIEWCUTWIDGET_H

#include <QtWidgets/QLabel>

class FSVideoPicEditHelper;

class FSCropWidget : public QLabel
{
    Q_OBJECT
    enum Radio{
        CUSTOM,
        R16_9,
        R4_3
    };

    enum ActionType{
        TRANSLATE,
        SCALE
    };

    enum ControlItem{
        TOP_LEFT=0,
        TOP_MID,
        TOP_RIGHT,
        RIGHT_MID,
        BOTTOM_RIGHT,
        BOTTOM_MID,
        BOTTOM_LEFT,      
        LEFT_MID,
        ANGEL_PT,
        CENTER

    };

public:
    FSCropWidget(QWidget *parent);
    ~FSCropWidget();

    QRectF CropRect();

protected:
    void paintEvent(QPaintEvent *e);
    bool eventFilter(QObject *obj, QEvent *e);
    void resizeEvent(QResizeEvent *e);
    void mouseMoveEvent(QMouseEvent *ev);
    void mousePressEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);


private:
    QWidget             *mpParentWidget;
    //FSVideoPicEditHelper *mpDrawHelper;
    QList<QRectF>   mControlItemList;         
    QPair<int, int> mActionInfo;
    QPointF         mPointLast;
    QPointF         mPointOffset;

    int             mWidht;
    QTransform      mTransform;
    QPoint          mCenterPoint;
    int             mAngle;


    
};

#endif // FSVIEWCUTWIDGET_H
