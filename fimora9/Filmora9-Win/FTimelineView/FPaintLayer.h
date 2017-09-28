#ifndef FPAINTLAYER_H
#define FPAINTLAYER_H

#include "FLayer.h"


#include "inc_FProject/FTrack.h"

class FTrackWidget;

class FPaintLayerManager : public FLayerManager
{
    Q_OBJECT

public:
    FPaintLayerManager(QObject *parent);


    bool paintEvent(QPainter *painter, QRect rect, FTrackPtr track, FTrackWidget *trackWidget, QPaintEvent *event);

};


class FPaintLayer : public FLayer
{
    Q_OBJECT

public:
    FPaintLayer(QObject *parent);

    virtual bool paintEvent(QPainter *painter, QRect rect, FTrackPtr track, FTrackWidget *trackWidget, QPaintEvent *event);

private:
    
};



class FTimelinePaintLayer : public FPaintLayer
{
    Q_OBJECT

public:
    FTimelinePaintLayer(QObject *parent);

    bool paintEvent(QPainter *painter, QRect rect, FTrackPtr track, FTrackWidget *trackWidget, QPaintEvent *event) override;

private:

};




#endif // FPAINTLAYER_H
