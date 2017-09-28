#include "stdafx.h"


#include "inc_CommonLib/WSCreateInstance.h"

#include "CaptureManger.h"

static NLECaptureManger *sCaptureManger = nullptr;
NLECaptureManger* NLECaptureManger::GetInstance()
{
	if (sCaptureManger == nullptr)
	{
		sCaptureManger = new NLECaptureManger(0);
	}
	return sCaptureManger;
}

void NLECaptureManger::Release()
{
	SAFE_DELETE(sCaptureManger);
}

NLEComPtr<ICaptureMgrEx> NLECaptureManger::GetICaptureMgr()
{
	return m_ICaptureMgr;
}


NLEComPtr<ICaptureEngineEx> NLECaptureManger::GetICaptureEngine()
{
	return m_ICaptureEngine;
}

QList<CaptureDeviceInfo> NLECaptureManger::GetDevicesFromType(EDeviceType type)
{
	QList<CaptureDeviceInfo> infos;
	auto captureMgr = GetICaptureMgr();
	int deviceCount = 0;
	captureMgr->RefreshDevices();
	captureMgr->GetDeviceCount(&deviceCount);
	for (int i = 0; i < deviceCount; ++i)
	{
		CaptureDeviceInfo info;
		captureMgr->GetDevice(i, &info);
		if (info.eDeviceType == type)
		{
			infos.push_back(info);
		}
	}
	return infos;
}

NLECaptureManger::NLECaptureManger(QObject *parent)
	: QObject(parent)
{
	WSCreateInstance::Instance()->wsCreateInstance(CLSID_CCaptureMgrEx, nullptr, 0, IID_ICaptureMgrEx, (void**)&m_ICaptureMgr);
	Q_ASSERT(m_ICaptureMgr);
	m_ICaptureMgr->RefreshDevices();
	WSCreateInstance::Instance()->wsCreateInstance(CLSID_CCaptureEngineEx, nullptr, 0, IID_ICaptureEngineEx, (void**)&m_ICaptureEngine);
	Q_ASSERT(m_ICaptureEngine);
}

NLECaptureManger::~NLECaptureManger()
{

}
