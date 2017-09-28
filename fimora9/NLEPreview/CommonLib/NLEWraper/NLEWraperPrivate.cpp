#include "stdafx.h"
#include "NLEWraperPrivate.h"
#include "PlayerMaterProvider.h"
//#include "qt_windows.h"
#include "WSCreateInstance.h"

NLEWraperPrivate::NLEWraperPrivate(QObject *parent)
	: QObject(parent)
{
	m_pMediaPlayerControl = NULL;
	m_pPlayerMaterProvider = new PlayerMaterProvider(this);
}

NLEWraperPrivate::~NLEWraperPrivate()
{

}
