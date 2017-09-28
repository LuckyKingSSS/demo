#ifndef FCOLORADJUSTPANEL_H
#define FCOLORADJUSTPANEL_H

#include "..\include\inc_FCommonLib\FBaseWidget.h"
#include "..\include\inc_FPropertyPanel\fpropertypanel_global.h"

class FHistogramPannel;
class FFoldWidget;
class FFoldWidgetItem;
class FNLETimeline;
class FWhiteBanlacePannel;
class F3DLUTPannel;
class FColorAdjustWidget;
class FLightAdjustPannel;
class FHslAdjustPannel ;
class FVignettePannel;
class FColorPalette;
class QScrollArea;

class FPROPERTYPANEL_EXPORT FColorAdjustPanel : public FBaseWidget
{
	Q_OBJECT
public:
	FColorAdjustPanel(QWidget *parent);
	~FColorAdjustPanel();
	void SetTimeline(FNLETimeline* timeline);

private:
	void InitUI();
	void InitConnections();

Q_SIGNALS:
	void sigTimelineChanged();

private:
	FHistogramPannel* m_HistogramPannel;
	FColorPalette*    m_ColorPalettePanel;
	FNLETimeline* m_Timeline;
	QScrollArea*      m_ScrollArea;
};

class FColorPalette :public FBaseWidget
{
	Q_OBJECT
public:
	FColorPalette(QWidget *parent);
	~FColorPalette(){};
	void SetTimeline(FNLETimeline* timeline){ m_Timeline = timeline; }
public Q_SLOTS:
	void slotOnTempratuerChange(int iValue);
	void slotOnTintChange(int iValue);
	void slotOnLUTChange(QString strLUTName);
	void slotOnColorExposureChange(int iValue);
	void slotOnColorBrightChange(int iValue);
	void slotOnColorContrastChange(int iValue);
	void slotOnColorVibranceChange(int iValue);
	void slotOnColorSaturationChange(int iValue);
	void slotOnLightHighlightsChange(int iValue);
	void slotOnLightShadowsChange(int iValue);
	void slotOnLightWhitesChange(int iValue);
	void slotOnLightBlacksChange(int iValue);
	void slotOnHSLHueChange(int iValue);
	void slotOnSaturationChange(int iValue);
	void slotOnLuminanceChange(int iValue);
	void slotOnVignetteAmountChange(int iValue);
	void slotOnVignetteSizeChange(int iValue);
	void slotOnVignetteRoundnessChange(int iValue);
	void slotOnVignetteFeatherChange(int iValue);
	void slotOnVignetteExposureChange(int iValue);
	void slotOnVignetteHiglightsChange(int iValue);

public Q_SLOTS:
	void OnFoldWidgetCollapse();

Q_SIGNALS:
	void sigTimelineChanged();

private:
	void InitUI();
	void InitConnections();
	FFoldWidgetItem* CreateWhiteBalanceItem();
	FFoldWidgetItem* Create3DLutItem();
	FFoldWidgetItem* CreateColorItem();
	FFoldWidgetItem* CreateLightItem();
	FFoldWidgetItem* CreateHslItem();
	FFoldWidgetItem* CreateVignetteItem();

private:
	FFoldWidget* m_AdjustFoldWidget;
	FNLETimeline* m_Timeline;
	FFoldWidgetItem* m_WhiteBalanceItem;
	FFoldWidgetItem* m_LutItem;
	FFoldWidgetItem* m_ColorItem;
	FFoldWidgetItem* m_LightItem;
	FFoldWidgetItem* m_HslItem;
	FFoldWidgetItem* m_VignetteItem;
	FWhiteBanlacePannel* m_WhiteBalancePannel;
	F3DLUTPannel * m_LutPannel;
	FColorAdjustWidget * m_ColorAdjustPannel;
	FLightAdjustPannel * m_LightAdustPannel;
	FHslAdjustPannel * m_HslAdjustPannel;
	FVignettePannel * m_VignettPannel;
	
};
#endif // FCOLORADJUSTPANEL_H
