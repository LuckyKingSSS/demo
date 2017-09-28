#ifndef FPHOTOPANELMGR_H
#define FPHOTOPANELMGR_H

#include "FPanelMgr.h"
#include "FPropertyCommon.h"
#include "inc_FCommonLib/FCommonWidgets.h"
#include "inc_FProject/FNLETimelineFactory.h"

class FPhotoPanelMgr : public FPanelMgr
{
	Q_OBJECT

public:
	FPhotoPanelMgr(QObject *parent);
	~FPhotoPanelMgr();

	// Rotate操作
	void RotateReset();
	void SetRotate(FPropertyRotate::RotateMode);
	// Contrast操作
	int GetContrastValue();
	void SetContrastValue(int);
	// Saturation操作
	int GetSaturationValue();
	void SetSaturationValue(int);
	// Brightness操作
	int GetBrightnessValue();
	void SetBrightnessValue(int);
	// Tint操作
	int GetTintValue();
	void SetTintValue(int);
	// 3D LUT操作

	// Enhance操作
	bool Enhanced();
	void EnableEnhance(bool);
	// Duration操作
	int GetDuration();
	void SetDuration(INT64);
	void DurationApplyToAll();

private:
	void RotateFlipHorizontal();
	void RotateFlipVertical();

private:
	NLEComPtr<FNLETimeline> m_pFilter;
};

#endif // FPHOTOPANELMGR_H
