#include "stdafx.h"
#include <string>
#include "FColorAdjustPanel.h"
#include "FHistogramPannel.h"
#include "FFoldWidgetPrivate.h"
#include "FFoldWidgetItem.h"
#include "FFoldWidget.h"
#include "FFoldWidgetItemPrivate.h"

#include "inc_FProject/FNLETimeline.h"

FColorAdjustPanel::FColorAdjustPanel(QWidget *parent)
	: FBaseWidget(parent)
{
	InitUI();
	InitConnections();
}

FColorAdjustPanel::~FColorAdjustPanel()
{

}

void FColorAdjustPanel::SetTimeline(FNLETimeline* timeline)
{
	 m_Timeline = timeline;
	 m_ColorPalettePanel->SetTimeline(timeline);
}

void FColorAdjustPanel::InitUI()
{
	m_HistogramPannel = new FHistogramPannel(nullptr);
	m_HistogramPannel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	m_HistogramPannel->setFixedHeight(135);

	m_ColorPalettePanel = new FColorPalette(nullptr);
	m_ScrollArea = new QScrollArea(nullptr);
	m_ScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	m_ScrollArea->setWidgetResizable(true);
	m_ScrollArea->setWidget(m_ColorPalettePanel);
	m_ScrollArea->setMinimumHeight(450);
	m_ScrollArea->setStyleSheet("border-width: 0px");
	m_ScrollArea->setFrameShape(QFrame::NoFrame);
	QVBoxLayout* vLayout = new QVBoxLayout();
	vLayout->addWidget(m_HistogramPannel);
	vLayout->addWidget(m_ScrollArea,1);
	vLayout->setContentsMargins(1, 1, 1, 1);
	vLayout->setSpacing(2);
	setLayout(vLayout);
}

void FColorAdjustPanel::InitConnections()
{
	connect(m_ColorPalettePanel, &FColorPalette::sigTimelineChanged, this, &FColorAdjustPanel::sigTimelineChanged);
}


void FColorPalette::slotOnTempratuerChange(int iValue)
{
	emit sigTimelineChanged();
}

void FColorPalette::slotOnTintChange(int iValue)
{
	emit sigTimelineChanged();
}

void FColorPalette::slotOnLUTChange(QString strLUTName)
{
	emit sigTimelineChanged();
}

void FColorPalette::slotOnColorExposureChange(int iValue)
{
	emit sigTimelineChanged();
}

void FColorPalette::slotOnColorBrightChange(int iValue)
{
	NLEPropComPtr pTimelineProp =  m_Timeline->GetProp();
	Q_ASSERT(pTimelineProp->Self() != nullptr);
	pTimelineProp->SetInt(NLEKey::PostProcess::kBrightness, iValue);
	emit sigTimelineChanged();
}

void FColorPalette::slotOnColorContrastChange(int iValue)
{
	NLEPropComPtr pTimelineProp = m_Timeline->GetProp();
	Q_ASSERT(pTimelineProp->Self() != nullptr);
	pTimelineProp->SetInt(NLEKey::PostProcess::kContrast, iValue);
	emit sigTimelineChanged();
}

void FColorPalette::slotOnColorVibranceChange(int iValue)
{
	emit sigTimelineChanged();
}

void FColorPalette::slotOnColorSaturationChange(int iValue)
{
	NLEPropComPtr pTimelineProp = m_Timeline->GetProp();
	Q_ASSERT(pTimelineProp->Self() != nullptr);
	pTimelineProp->SetInt(NLEKey::PostProcess::kSaturation, iValue);
	emit sigTimelineChanged();
}

void FColorPalette::slotOnLightHighlightsChange(int iValue)
{
	emit sigTimelineChanged();
}

void FColorPalette::slotOnLightShadowsChange(int iValue)
{
	emit sigTimelineChanged();
}

void FColorPalette::slotOnLightWhitesChange(int iValue)
{
	emit sigTimelineChanged();
}

void FColorPalette::slotOnLightBlacksChange(int iValue)
{
	emit sigTimelineChanged();
}

void FColorPalette::slotOnHSLHueChange(int iValue)
{
	NLEPropComPtr pTimelineProp = m_Timeline->GetProp();
	Q_ASSERT(pTimelineProp->Self() != nullptr);
	pTimelineProp->SetInt(NLEKey::PostProcess::kHue, iValue);
	emit sigTimelineChanged();
}

void FColorPalette::slotOnSaturationChange(int iValue)
{
	emit sigTimelineChanged();
}

void FColorPalette::slotOnLuminanceChange(int iValue)
{
	emit sigTimelineChanged();
}

void FColorPalette::slotOnVignetteAmountChange(int iValue)
{
	emit sigTimelineChanged();
}

void FColorPalette::slotOnVignetteSizeChange(int iValue)
{
	emit sigTimelineChanged();
}

void FColorPalette::slotOnVignetteRoundnessChange(int iValue)
{
	emit sigTimelineChanged();
}

void FColorPalette::slotOnVignetteFeatherChange(int iValue)
{
	emit sigTimelineChanged();
}

void FColorPalette::slotOnVignetteExposureChange(int iValue)
{
	emit sigTimelineChanged();
}

void FColorPalette::slotOnVignetteHiglightsChange(int iValue)
{
	emit sigTimelineChanged();
}

void FColorPalette::OnFoldWidgetCollapse()
{
   
}

FFoldWidgetItem* FColorPalette::CreateWhiteBalanceItem()
{
	FFoldWidgetItem* folderItem = new FFoldWidgetItem(m_AdjustFoldWidget);
	FHeaderWidget* header = new FHeaderWidget(folderItem);
	header->SetCheckBoxName(tr("White Balance"));
	m_WhiteBalancePannel = new FWhiteBanlacePannel(folderItem);
	folderItem->SetCustomHeader(header);
	folderItem->SetContainsWidget(m_WhiteBalancePannel);
	return folderItem;
}

FFoldWidgetItem* FColorPalette::Create3DLutItem()
{
	FFoldWidgetItem* folderItem = new FFoldWidgetItem(m_AdjustFoldWidget);
	FHeaderWidget* header = new FHeaderWidget(folderItem);
	header->SetCheckBoxName(tr("3DLUT"));
	m_LutPannel = new F3DLUTPannel(folderItem);
	folderItem->SetCustomHeader(header);
	folderItem->SetContainsWidget(m_LutPannel);
	return folderItem;
}

FFoldWidgetItem* FColorPalette::CreateColorItem()
{
	FFoldWidgetItem* folderItem = new FFoldWidgetItem(m_AdjustFoldWidget);
	FHeaderWidget* header = new FHeaderWidget(folderItem);
	header->SetCheckBoxName(tr("Color"));
	m_ColorAdjustPannel = new FColorAdjustWidget(folderItem);
	folderItem->SetCustomHeader(header);
	folderItem->SetContainsWidget(m_ColorAdjustPannel);
	return folderItem;
}

FFoldWidgetItem* FColorPalette::CreateLightItem()
{
	FFoldWidgetItem* folderItem = new FFoldWidgetItem(m_AdjustFoldWidget);
	FHeaderWidget* header = new FHeaderWidget(folderItem);
	header->SetCheckBoxName(tr("Light"));
	m_LightAdustPannel = new FLightAdjustPannel(folderItem);
	folderItem->SetCustomHeader(header);
	folderItem->SetContainsWidget(m_LightAdustPannel);
	return folderItem;
}

FFoldWidgetItem* FColorPalette::CreateHslItem()
{
	FFoldWidgetItem* folderItem = new FFoldWidgetItem(m_AdjustFoldWidget);
	FHeaderWidget* header = new FHeaderWidget(folderItem);
	header->SetCheckBoxName(tr("HSL"));
	m_HslAdjustPannel = new FHslAdjustPannel(folderItem);
	folderItem->SetCustomHeader(header);
	folderItem->SetContainsWidget(m_HslAdjustPannel);
	return folderItem;
}

FFoldWidgetItem* FColorPalette::CreateVignetteItem()
{
	FFoldWidgetItem* folderItem = new FFoldWidgetItem(m_AdjustFoldWidget);
	FHeaderWidget* header = new FHeaderWidget(folderItem);
	header->SetCheckBoxName(tr("Vignette"));
	m_VignettPannel = new FVignettePannel(folderItem);
	folderItem->SetCustomHeader(header);
	folderItem->SetContainsWidget(m_VignettPannel);
	return folderItem;
}

FColorPalette::FColorPalette(QWidget *parent) :FBaseWidget(parent)
{
	InitUI();
	InitConnections();
}

void FColorPalette::InitUI()
{
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	const int iFixHeight = 450;
	m_AdjustFoldWidget = new FFoldWidget(nullptr);
	m_AdjustFoldWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	//m_AdjustFoldWidget->setFixedHeight(iFixHeight);

	m_WhiteBalanceItem = CreateWhiteBalanceItem();
	m_AdjustFoldWidget->AddItem(m_WhiteBalanceItem);

	m_LutItem = Create3DLutItem();
	m_AdjustFoldWidget->AddItem(m_LutItem);

	m_ColorItem = CreateColorItem();
	m_AdjustFoldWidget->AddItem(m_ColorItem);

	m_LightItem = CreateLightItem();
	m_AdjustFoldWidget->AddItem(m_LightItem);

	m_HslItem = CreateHslItem();
	m_AdjustFoldWidget->AddItem(m_HslItem);

	m_VignetteItem = CreateVignetteItem();
	m_AdjustFoldWidget->AddItem(m_VignetteItem);

	QVBoxLayout* vLayout = new QVBoxLayout();
	vLayout->addWidget(m_AdjustFoldWidget);
	vLayout->setContentsMargins(1, 1, 1, 1);
	vLayout->setSpacing(2);
	setLayout(vLayout);
}

void FColorPalette::InitConnections()
{
	connect(m_WhiteBalancePannel, &FWhiteBanlacePannel::sigOnTempratuerChange, this, &FColorPalette::slotOnTempratuerChange);
	connect(m_WhiteBalancePannel, &FWhiteBanlacePannel::sigOnTintChange, this, &FColorPalette::slotOnTintChange);

	connect(m_LutPannel, &F3DLUTPannel::sigOnLUTChange, this, &FColorPalette::slotOnLUTChange);

	connect(m_ColorAdjustPannel, &FColorAdjustWidget::sigOnColorBrightChange, this, &FColorPalette::slotOnColorBrightChange);
	connect(m_ColorAdjustPannel, &FColorAdjustWidget::sigOnColorExposureChange, this, &FColorPalette::slotOnColorExposureChange);
	connect(m_ColorAdjustPannel, &FColorAdjustWidget::sigOnColorContrastChange, this, &FColorPalette::slotOnColorContrastChange);
	connect(m_ColorAdjustPannel, &FColorAdjustWidget::sigOnColorVibranceChange, this, &FColorPalette::slotOnColorVibranceChange);
	connect(m_ColorAdjustPannel, &FColorAdjustWidget::sigOnColorSaturationChange, this, &FColorPalette::slotOnColorSaturationChange);

	connect(m_LightAdustPannel, &FLightAdjustPannel::sigOnLightHighlightsChange, this, &FColorPalette::slotOnLightHighlightsChange);
	connect(m_LightAdustPannel, &FLightAdjustPannel::sigOnLightShadowsChange, this, &FColorPalette::slotOnLightShadowsChange);
	connect(m_LightAdustPannel, &FLightAdjustPannel::sigOnLightWhitesChange, this, &FColorPalette::slotOnLightWhitesChange);
	connect(m_LightAdustPannel, &FLightAdjustPannel::sigOnLightBlacksChange, this, &FColorPalette::slotOnLightBlacksChange);


	connect(m_HslAdjustPannel, &FHslAdjustPannel::sigOnHSLHueChange, this, &FColorPalette::slotOnHSLHueChange);
	connect(m_HslAdjustPannel, &FHslAdjustPannel::sigOnSaturationChange, this, &FColorPalette::slotOnSaturationChange);
	connect(m_HslAdjustPannel, &FHslAdjustPannel::sigOnLuminanceChange, this, &FColorPalette::slotOnLuminanceChange);

	connect(m_VignettPannel, &FVignettePannel::sigOnVignetteAmountChange, this, &FColorPalette::slotOnVignetteAmountChange);
	connect(m_VignettPannel, &FVignettePannel::sigOnVignetteSizeChange, this, &FColorPalette::slotOnVignetteSizeChange);
	connect(m_VignettPannel, &FVignettePannel::sigOnVignetteRoundnessChange, this, &FColorPalette::slotOnVignetteRoundnessChange);
	connect(m_VignettPannel, &FVignettePannel::sigOnVignetteFeatherChange, this, &FColorPalette::slotOnVignetteFeatherChange);
	connect(m_VignettPannel, &FVignettePannel::sigOnVignetteExposureChange, this, &FColorPalette::slotOnVignetteExposureChange);
	connect(m_VignettPannel, &FVignettePannel::sigOnVignetteHiglightsChange, this, &FColorPalette::slotOnVignetteHiglightsChange);


	connect(m_AdjustFoldWidget, &FFoldWidget::sigUpdateSize, this, [&](int iNewHeight){
		//resize(width(), iNewHeight);
		setFixedHeight(iNewHeight);
	});
}
