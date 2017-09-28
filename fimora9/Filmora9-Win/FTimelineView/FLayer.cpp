#include "stdafx.h"
#include "FLayer.h"

#include "inc_CommonLib/TimelineRuler.h"

FLayerManager::FLayerManager(QObject *parent)
:FLayer(parent)
{

}


void FLayerManager::AddLayer(FLayer *layer)
{
    Q_ASSERT(layer);
    layer->setParent(this);
    m_layers.push_back(layer);
}

void FLayerManager::RemoveLayer(FLayer *layer)
{
    Q_ASSERT(layer);
    Q_ASSERT(m_layers.contains(layer));
    layer->setParent(nullptr);
    m_layers.removeOne(layer);
}

FLayers FLayerManager::GetLayers()
{
    return m_layers;
}


//////////////////////////////////////////////////////////////////////////
FLayer::FLayer(QObject *parent)
    : QObject(parent)
{

}

