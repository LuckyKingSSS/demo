#ifndef FTOOLBARSLIDER_H
#define FTOOLBARSLIDER_H

#include <QWidget>

class ToolBarSlider;

class FToolBarSlider : public QWidget
{
	Q_OBJECT

public:
	FToolBarSlider(QWidget *parent = 0, Qt::Orientation orientation = Qt::Horizontal);
	~FToolBarSlider();

	void SetRange(float min, float max);
	void SetValue(float value, bool queueRepaint = true); // 使用队列刷新界面
	float Value();
	float Duration();
	bool CanSeek();
	int Height();
	void SetGrooveHeight(int h);
	bool ButtonPressDown();
	void SetButtonPressDown(bool press);
	void SetLeftToRight(bool left);
	void SetBottomToTop(bool bottom);
	void SetChunkHeight(int height);
	void SetSliderObjectName(QString name);
	float GetMinValue();
	float GetMaxValue();

Q_SIGNALS:
	void valueChanged(float);
	void startValueChanged();
	void endValueChanged();
	// 当拖拽时才发送
	void startDrag();
	void endDrag();

private:
	float m_minValue;
	float m_maxValue;
	ToolBarSlider *m_pToolBarSlider;
};

#endif // FTOOLBARSLIDER_H
