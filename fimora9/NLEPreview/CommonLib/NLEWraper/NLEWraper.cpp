#include "stdafx.h"
#include "NLEWraper.h"
#include "NLEWraperPrivate.h"
NLEWraper::NLEWraper(QObject *parent)
	: QObject(parent)
{
	m_p = new NLEWraperPrivate(this);
}

NLEWraper::~NLEWraper()
{

}

NLEWraper& NLEWraper::GetInstance()
{
	static NLEWraper s;
	return s;
}

PlayerMaterProvider* NLEWraper::GetPlayerMaterProvider()
{
	return m_p->m_pPlayerMaterProvider;
}

void NLEWraper::SetMeidaPlayerControl(IMediaPlayerControl* pControler)
{
	m_p->m_pMediaPlayerControl = pControler;
}

IMediaPlayerControl* NLEWraper::GetMeidaPlayerControl()
{
	return m_p->m_pMediaPlayerControl;
}