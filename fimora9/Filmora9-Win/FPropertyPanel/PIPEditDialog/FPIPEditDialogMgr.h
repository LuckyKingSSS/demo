#ifndef FPIPEDITDIALOGMGR_H
#define FPIPEDITDIALOGMGR_H

#include "FPanelMgr.h"
#include "FPropertyCommon.h"
#include "NLEType_c.h"
#include "inc_FProject/FNLETimeline.h"

class FPIPEditDialogMgr : public FPanelMgr
{
	Q_OBJECT

public:
	FPIPEditDialogMgr(QObject *parent);
	~FPIPEditDialogMgr();

	/*Mask*/
	void SetMaskPath(QString);
	QStringList GetMaskThumbFile();
	QStringList GetMaskFile();
	void SetMaskEffect(int);
	void SetMaskWidth(float);
	void SetMaskHeight(float);
	void SetMaskLeft(float);
	void SetMaskTop(float);

	/*Effect*/
	// Chroma Key
	void SetCurrentColor(QColor);
	QColor GetCurrentColor();
	void SetIntensityLevel(int);
	int GetIntensityLevel();

	// Shadow
	void SetShadowDistance(int);
	int GetShadowDistance();
	void SetShadowBlur(int);
	int GetShadowBlur();
	void SetShadowTransparency(int);
	int GetShadowTransparency();
	void SetShadowColor(QColor);
	QColor GetShadowColor();
	void SetShadowDirection(DIRECTION);
	DIRECTION GetShadowDirection();

	// Border
	void SetBorderDistance(int);
	int GetBorderDistance();
	void SetBorderBlur(int);
	int GetBorderBlur();
	void SetBorderTransparency(int);
	int GetBorderTransparency();
	void SetBorderGradientColorStart(QColor);
	QColor GetBorderGradientColorStart();
	void SetBorderGradientColorEnd(QColor);
	QColor GetBorderGradientColorEnd();
	void SetBorderGradientDirection(DIRECTION);
	DIRECTION GetBorderGradientDirection();

	// Flip
	void SetRotate(FPropertyRotate::RotateMode);  // Ω«∂»÷µ

private:
	void Init();
	void RotateFlipHorizontal();
	void RotateFlipVertical();

private:
	/*Mask*/
	QString m_MaskPath;
	QStringList m_lstMaskThumbFile;
	QStringList m_lstMaskFile;

	/*Effect*/
	// Chroma Key
	QColor m_CurrentColor;
	int m_IntensityLevel;
	// Shadow
	int m_ShadowDistance;
	int m_ShadowBlur;
	int m_ShadowTransparency;
	QColor m_ShadowColor;
	DIRECTION m_ShadowDirection;
	// Border
	int m_BorderDistance;
	int m_BorderBlur;
	int m_BorderTransparency;
	QColor m_GradientColorStart;
	QColor m_GradientColorEnd;
	DIRECTION m_GradientDirection;

	NLEComPtr<FNLETimeline> m_pFilter;
};

#endif // FPIPEDITDIALOGMGR_H
