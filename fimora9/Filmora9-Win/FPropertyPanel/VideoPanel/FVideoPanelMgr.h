#ifndef FVIDEOPANELMGR_H
#define FVIDEOPANELMGR_H

#include "FPanelMgr.h"
#include "FPropertyCommon.h"
#include "inc_FProject/FNLETimeline.h"


class FVideoPanel;
class FNLETimeline;
class FMenu;

class FVideoPanelMgr : public FPanelMgr
{
	Q_OBJECT

public:
	FVideoPanelMgr(QObject *parent = 0);
	~FVideoPanelMgr();

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
	// Speed操作
	float GetSpeedValue();
	void SetSpeedValue(float);
	// play video clip in reverse
	bool PlayInReverse();
	void EnablePlayInReverse(bool);

private:
	void RotateFlipHorizontal();
	void RotateFlipVertical();

private:
	NLEComPtr<FNLETimeline> m_pFilter;
};

#endif // FVIDEOPANELMGR_H
