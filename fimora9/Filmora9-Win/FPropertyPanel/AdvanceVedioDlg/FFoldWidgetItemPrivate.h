#ifndef FSHRINKWIDGET_H
#define FSHRINKWIDGET_H

#include <QtWidgets>
#include <QString>
#include "inc_FCommonLib\FBaseWidget.h"
#include "inc_FPropertyPanel\fpropertypanel_global.h"
#include "FCollapseHeaderBase.h"
#include "FRulerWidget.h"
class FHeaderWidget;
class FContexBaseWidget;
class FLabel;
class FButton;
class FCheckBox;

enum ShowMode
{
	Mode_ShowSingle,  //同一时间显示单个
	Mode_ShowMulti    //同一时间显示多个
};

class FPROPERTYPANEL_EXPORT FFoldWidgetItemPrivate : public FBaseWidget
{
	Q_OBJECT

public:
	FFoldWidgetItemPrivate(QWidget *parent);
	~FFoldWidgetItemPrivate();
	void SetContainsWidget(FContexBaseWidget* pWidget);
	FContexBaseWidget* GetItemCentralWidget(){ return  m_ContainWidget; };
	void SetCustomHeader(FCollapseHeaderBase* pHeaderWidegt);
	FCollapseHeaderBase* GetItemHeader(){ return m_Header; };
	void HideContainWidget();
	void ShowContainWidget();
	void SetShowMode(ShowMode showMode){ m_CurMode = showMode; };
	void SetCheckBoxName(QString strName);
	void SetIndex(int iIndex){ m_Index = iIndex; };
	int  GetIndex(){ return m_Index; };
	void SetHeaderTitleName(QString strTitleName);
	void SetHeaderHeight(int iHeight);


Q_SIGNALS:
	void sigOnCollapse(bool bIsCollapse);
	void sigResize(int iWidth, int iHeight);
	
public Q_SLOTS:
     void slotOnCollaspse(bool bIsCollapse);
protected:
	void resizeEvent(QResizeEvent *e)override;

private:
	void InitUI();
	void InitConnections();
private:
	FContexBaseWidget* m_ContainWidget;
	FCollapseHeaderBase* m_Header;
	ShowMode    m_CurMode;  //当前的显示模式
	int         m_Index;     //当前窗口在容器窗口中的序号标记，序号按照从1开始计数
};


class FPROPERTYPANEL_EXPORT FHeaderWidget :public  FCollapseHeaderBase
{
	Q_OBJECT
public:
	FHeaderWidget(QWidget *parent);
	~FHeaderWidget();
	void SetCheckBoxName(QString strName);
	
protected:
	//void mousePressEvent(QMouseEvent *event)override;

private:
	void InitUI();
	void InitData();
	void InitConnections();
private:
	FButton* m_CollapseBtn;
	FCheckBox* m_HeaderCheckBox;
	FButton* m_RefreshBtn;
};

class FPROPERTYPANEL_EXPORT FDefaultHeaderWidget :public  FCollapseHeaderBase
{
	Q_OBJECT
public:
	FDefaultHeaderWidget(QWidget *parent);
	~FDefaultHeaderWidget(){};
	void setTitleName(QString strTitleName){ m_TitleName->setText(strTitleName); };
private:
	void InitUI();
	void InitData();
	void InitConnections();
private:
	FButton* m_CollapseBtn; 
	FLabel*  m_TitleName;  //标题栏的名字
};


class FPROPERTYPANEL_EXPORT FWhiteBanlacePannel : public FContexBaseWidget
{
    Q_OBJECT
public:
	FWhiteBanlacePannel(QWidget *parent);
	void ResetConfig();
private:
	void InitUI();
	void InitConnections();
Q_SIGNALS:
	void sigOnTempratuerChange(int iValue);
	void sigOnTintChange(int iValue);

private:
	FRulerWidget* m_TemperatureRuler;
	FRulerWidget* m_TintRuler;
};

class F3DLUTDataMgr;
class F3DLUTPannel : public FContexBaseWidget
{
	Q_OBJECT
public:
	F3DLUTPannel(QWidget *parent);

Q_SIGNALS:
	void sigOnLUTChange(QString strValue);

private:
	void InitUI();
	void ResetConfig();
	void InitConnectios();
	void InitData();
private:
	FComboBox* m_Combo;
	FButton*   m_RevertBtn;
	F3DLUTDataMgr* m_DataMgr; //数据控制层
	QStringList m_DataToLoad;//待加载的数据
};


class FColorAdjustWidget :public FContexBaseWidget
{
	Q_OBJECT
public:
	FColorAdjustWidget(QWidget *parent);

Q_SIGNALS:
	void sigOnColorBrightChange(int iValue);
	void sigOnColorExposureChange(int iValue);
	void sigOnColorContrastChange(int iValue);
	void sigOnColorVibranceChange(int iValue);
	void sigOnColorSaturationChange(int iValue);

private:
	void InitUI();
	void ResetConfig();
	void InitConnectios();
	void InitData();
private:

	FRulerWidget* m_ExposureRuler;
	FRulerWidget* m_BrightnessRuler;
	FRulerWidget* m_ContrastRuler;
	FRulerWidget* m_VibranceRuler;
	FRulerWidget* m_SaturationRuler;
};

class FLightAdjustPannel : public FContexBaseWidget
{
	Q_OBJECT
public:
	FLightAdjustPannel(QWidget *parent);

Q_SIGNALS:
	void sigOnLightHighlightsChange(int iVlaue);
	void sigOnLightShadowsChange(int iVlaue);
	void sigOnLightWhitesChange(int iVlaue);
	void sigOnLightBlacksChange(int iVlaue);

private:
	void InitUI();
	void ResetConfig();
	void InitConnectios();
	void InitData();
private:

	FRulerWidget* m_Highlights;
	FRulerWidget* m_Shadows;
	FRulerWidget* m_Whites;
	FRulerWidget* m_Blacks;
};


//圆形label
class FPROPERTYPANEL_EXPORT FRoundLabel : public FLabel
{
    Q_OBJECT
public:
	FRoundLabel(QWidget *parent, QColor color = QColor(222,58,60));
	void SetBackgroundColor(QColor color){ m_BackgroundColor = color; };
	QColor GetBackgroundColor(){ return m_BackgroundColor; };
	void SetChecked(bool bChecked){ m_bCurCheckState = bChecked; repaint(); };
	bool GetCheckState(){ return m_bCurCheckState; };

Q_SIGNALS:
	void sigOnChecked(QColor bakgroundColor);

protected:
	void paintEvent(QPaintEvent *e)override;
	void mousePressEvent(QMouseEvent *event)override;

private: 
	void InitUI();
private:
	QColor m_BackgroundColor; //背景色
	bool m_bCurCheckState;
};


class FPROPERTYPANEL_EXPORT  FHslAdjustPannel : public FContexBaseWidget
{
	Q_OBJECT
public:
	FHslAdjustPannel(QWidget *parent);

Q_SIGNALS:
	void sigOnHSLHueChange(int iValue);
	void sigOnSaturationChange(int iValue);
	void sigOnLuminanceChange(int iValue);

public Q_SLOTS:
	void slotOnColorChanged(QColor curColor);

private:
	void InitUI();
	void ResetConfig();
	void InitConnectios();
	void InitData();
private:

	FRoundLabel* m_RedColorLabel;
	FRoundLabel* m_OrangeColorLabel;
	FRoundLabel* m_YellowColorLabel;
	FRoundLabel* m_GreenLabel;
	FRoundLabel* m_MagentaLabel;
	FRoundLabel* m_PurpleLabel;
	FRoundLabel* m_BlueLabel;
	FRoundLabel* m_CyanLabel;
	QList<FRoundLabel*> m_AllColorLables;
	FRulerWidget* m_HueRuler;
	FRulerWidget* m_Saturation;
	FRulerWidget* m_Luminance;
	QColor  m_RedColor;
	QColor  m_OrangeColor;
	QColor  m_YellowColor;
	QColor  m_GreenColor;
	QColor  m_MagentaColor;
	QColor  m_PurpleColor;
	QColor  m_BlueColor;
	QColor  m_CyanColor;
};

class FPROPERTYPANEL_EXPORT FVignettePannel : public FContexBaseWidget
{
	Q_OBJECT
public:
	FVignettePannel(QWidget *parent);

Q_SIGNALS:
	void sigOnVignetteAmountChange(int iValue);
	void sigOnVignetteSizeChange(int iValue);
	void sigOnVignetteRoundnessChange(int iValue);
	void sigOnVignetteFeatherChange(int iValue);
	void sigOnVignetteExposureChange(int iValue);
	void sigOnVignetteHiglightsChange(int iValue);

private:
	void InitUI();
	void ResetConfig();
	void InitConnectios();
	void InitData();
private:

	FRulerWidget* m_AmountRuler;
	FRulerWidget* m_SizeRuler;
	FRulerWidget* m_RoundnessRuler;

	FRulerWidget* m_FeatherRuler;
	FRulerWidget* m_ExposureRuler;
	FRulerWidget* m_HighlightsRuler;
};


#endif // FSHRINKWIDGET_H