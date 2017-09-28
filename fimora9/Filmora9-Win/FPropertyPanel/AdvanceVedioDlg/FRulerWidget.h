#ifndef FRULERWIDGET_H
#define FRULERWIDGET_H

#include "inc_FCommonLib\FBaseWidget.h"
#include "inc_FCommonLib\FCommonWidgets.h"
#include "..\include\inc_FPropertyPanel\fpropertypanel_global.h"

class FScaleWidget;
enum ColorType
{
   type_Red,
   type_Orange,
   type_Yellow,
   type_Green,
   type_Magenta,
   type_Purple,
   type_Blue,
   type_Cyan
};

class FPROPERTYPANEL_EXPORT FRulerWidget : public FBaseWidget
{
	Q_OBJECT

public:
	FRulerWidget(QWidget *parent,QColor beginColor, QColor endColor);
	~FRulerWidget();
	void setCurColorType(ColorType curType){ backgroundColorType = curType; repaint(); };
	void setLabelText(QString strLabel){ m_LabelName->setText(strLabel); repaint(); };
	void setRange(int iMinValue,int iMaxValue);
	void setRange(float fMinValue,float fMaxValue);

protected:
	void resizeEvent(QResizeEvent *e)override;
	void paintEvent(QPaintEvent *e)override;
	void mousePressEvent(QMouseEvent *e)override;
	void mouseMoveEvent(QMouseEvent *e)override;

Q_SIGNALS:
	void sigValueChanged(int iVlaue);
	void sigStrValueChanged(QString strValue);

private: 
	void initUI();
	void initData();
	void drawMaskLayer(QPainter& painter);
	float getPosByValue(int iValue);
	int  getValueByPos(float fPos);
	

private:
	FLabel* m_LabelName;
	FLabel* m_LabelValue;
	FScaleWidget* m_ScaleWidget;
	ColorType backgroundColorType;
	float m_fCurXpos;   //当前游标所在位置的X坐标
	QRectF m_FillRect;  //蒙版区域
	int m_CurValue;     //当前游标对应的数值
	QColor m_BeginColor; //渐变的起点颜色
	QColor m_EndColor;   //渐变的终点颜色
};

class FScaleWidget : public FBaseWidget
{
  Q_OBJECT

public:
	FScaleWidget(QWidget *parent);
	~FScaleWidget(){};
	void setRange(int iMinValue, int iMaxValue);
	int getMinValue(){ return m_iMinumNum; };
	int getmaxValue(){ return m_iMaxMunNum; };
	float getMinValueF(){ return m_fMinumNum; };
	float getMaxValueF(){ return m_fMaxMunNum; };
	void setRange(float fMinValue, float fMaxValue);
	void setDivideNum(int iDivNum){ m_iDivideNum = iDivNum;};

protected:
	void paintEvent(QPaintEvent *e)override;
private:
	void setScaleSize();
	void initData();
private:
	int   m_iDivideNum; //等分数
	int   m_iMinumNum;
	int   m_iMaxMunNum;
	float m_fMinumNum;
	float m_fMaxMunNum;
	float m_fSmallScaleLen;
	float m_fBigScalelen;
};

#endif // FRULERWIDGET_H
