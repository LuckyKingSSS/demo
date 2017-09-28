#ifndef FLAYER_H
#define FLAYER_H

#include <QObject>

class FLayer;
typedef QList<FLayer*> FLayers;


class FLayer : public QObject
{
    Q_OBJECT

public:
    FLayer(QObject *parent = nullptr);

private:

};


class FLayerManager : public FLayer
{
    Q_OBJECT

public:
    FLayerManager(QObject *parent);

    // 后加的层优先级越代
    void AddLayer(FLayer *layer);
    void RemoveLayer(FLayer *layer);
    FLayers GetLayers();

private:
    FLayers m_layers;

};



class PainterRestore
{
public:
    PainterRestore(QPainter *painter)
    {
        m_painter = painter;
        m_painter->save();
    }
    ~PainterRestore()
    {
        m_painter->restore();
    }
    QPainter *m_painter;
};



#endif // FLAYER_H
