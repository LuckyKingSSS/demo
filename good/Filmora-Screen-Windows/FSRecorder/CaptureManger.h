#ifndef CAPTUREMANGER_H
#define CAPTUREMANGER_H

#include <QObject>

#include "Include/NLEComPtr.h"

#include "ICaptureSource.h"
#include "ICaptureEngine.h"


class ICaptureMgrEx;
class ICaptureEngineEx;
class NLECaptureManger : public QObject
{
	Q_OBJECT

public:
	static NLECaptureManger* GetInstance();
	void Release();
	QList<CaptureDeviceInfo> GetDevicesFromType(EDeviceType type);

	NLEComPtr<ICaptureMgrEx> GetICaptureMgr();
	NLEComPtr<ICaptureEngineEx> GetICaptureEngine();
public:
	NLECaptureManger(QObject *parent);
	~NLECaptureManger();

private:
	NLEComPtr<ICaptureMgrEx> m_ICaptureMgr;
	NLEComPtr<ICaptureEngineEx> m_ICaptureEngine;
};

#endif // CAPTUREMANGER_H
