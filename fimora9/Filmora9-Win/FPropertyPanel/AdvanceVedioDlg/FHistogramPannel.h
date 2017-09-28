#ifndef FHISTOGRAMPANNEL_H
#define FHISTOGRAMPANNEL_H

#include "inc_FCommonLib\FBaseWidget.h"

class FLabel;
class FMatrixDrawPannel;
class FHistogramPannel : public FBaseWidget
{
	Q_OBJECT

public:
	FHistogramPannel(QWidget *parent);
	~FHistogramPannel();

private:
	void InitUI();
	void InitConnections();

private:
	FLabel* m_pHeader; 
	FMatrixDrawPannel* m_pMatrixGraph; //矩阵图窗口
};


class FMatrixDrawPannel : public FBaseWidget
{
	Q_OBJECT

public:
	FMatrixDrawPannel(QWidget *parent);
	~FMatrixDrawPannel();

protected:
	void paintEvent(QPaintEvent *e)override;

private:
	void InitUI();
	void InitConnections();
	void GetGraphData();   //获取绘制矩阵图需要的数据

private:

};

#endif // FHISTOGRAMPANNEL_H
