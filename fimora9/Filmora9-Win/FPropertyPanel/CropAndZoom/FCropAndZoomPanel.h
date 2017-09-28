#ifndef FCROPANDZOOMPANEL_H
#define FCROPANDZOOMPANEL_H

#include "FPropertyDialog.h"
#include "..\include\inc_FPropertyPanel\fpropertypanel_global.h"

class FLabel;
class FPowerToolPlayer;
class FRadioButton;
class FLineEdit;
class FButton;
class FCropPlayer;
class  FPROPERTYPANEL_EXPORT FCropAndZoomPanel : public FPropertyDialog
{
	Q_OBJECT

public:
	FCropAndZoomPanel(FNLETimeline *timeline, QWidget *parent);
	~FCropAndZoomPanel();
	void SetTimeline(FNLETimeline* timeline);

Q_SIGNALS:
	void signalOK();
	void signalCancel();

private:
	void InitUI();
	void InitConnections();

private:
	FLabel* m_CropLabel;
	FLabel* m_PanZoomLabel;
	FCropPlayer* m_PreViewPlayer;
	FLabel* m_ConstrainLabel;
	FRadioButton* m_ManuallyBtn;
	FRadioButton* m_SixteenToNineBtn;
	FRadioButton* m_FourToThreeBtn;
	FLineEdit* m_HorizonPixels;
	FLabel*  m_CrossLabel;
	FLineEdit* m_VerticalPixels;
	FButton* m_ResetBtn;
	FButton* m_OKBtn;
	FButton* m_CancleBtn;
};

#endif // FCROPANDZOOMPANEL_H
