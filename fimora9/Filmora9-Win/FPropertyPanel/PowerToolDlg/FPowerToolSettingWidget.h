#ifndef FPOWERTOOLSETTINGWIDGET_H
#define FPOWERTOOLSETTINGWIDGET_H

#include "..\include\inc_FCommonLib\FBaseWidget.h"
#include "..\AdvanceVedioDlg\FCollapseHeaderBase.h"
#include "..\..\include\inc_FPropertyPanel\fpropertypanel_global.h"


class FFoldWidget;
class FCheckBox;
class FTiltShiftWidget;
class FMosaicWidget;
class FFaceOffWidget;
class FPROPERTYPANEL_EXPORT FPowerToolSettingWidget : public FBaseWidget
{
	Q_OBJECT

public:
	FPowerToolSettingWidget(QWidget *parent);
	~FPowerToolSettingWidget();

private:
	void InitUI();
	void InitConnections();

private:
	FFoldWidget* m_FolderWidget;
	FTiltShiftWidget* m_TiltShiftWidget;
	FMosaicWidget*    m_FMosaicWidget;
	FFaceOffWidget*   m_FFaceOffWidget;
	FButton*  m_OKBtn;
	FButton*  m_CancleBtn;
};

class FTiltShiftWidget : public FContexBaseWidget
{
	Q_OBJECT
public:
	FTiltShiftWidget(QWidget *parent);
	~FTiltShiftWidget(){};
private:
	void InitUI();
	void InitConnections();
private:
	FCheckBox* m_CheckBox;
};


class ToolBarSlider;
class FLabel;
class FMosaicWidget : public FContexBaseWidget
{
    Q_OBJECT
public:
	FMosaicWidget(QWidget *parent);
	~FMosaicWidget(){};

private:
	void InitUI();
	void InitConnections();
private:
	FCheckBox* m_EnableChckebox;
	FButton* m_AddMosaicBtn;
	FButton* m_DeleteMosaicBtn;
	FLabel* m_PercentLabel;
	ToolBarSlider* m_Slider;
};


class FFaceOffWidget : public FContexBaseWidget
{
	Q_OBJECT
public:
	FFaceOffWidget(QWidget *parent);
	~FFaceOffWidget(){};

protected:
	void resizeEvent(QResizeEvent *event)override;
private:
	void InitUI();
	void InitConnections();
private:
	FCheckBox* m_EnableChckebox;
	QListWidget* m_FaceOff;
};

#endif // FPOWERTOOLSETTINGWIDGET_H
