#pragma once



class VolumeSlider : public QWidget
{
	Q_OBJECT

public:
	VolumeSlider(QWidget *parent);

	void SetValue(int value);
	void SetRange(int minvalue, int maxvalue);
	void GetRange(int &minvalue, int &maxvalue);
	int GetValue();
	void SetChunkWidth(int w);
	void SetGrooveHeight(int h);
	void SetHorizontal(bool horizontal);
	bool GetHorizontal();

	void DoModal(QPoint p);

Q_SIGNALS:
	void ValueChanged(int value);

protected:
	void resizeEvent(QResizeEvent *event);
	void mousePressEvent(QMouseEvent *event);
	//void mouseReleaseEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void keyPressEvent(QKeyEvent *event);
	void paintEvent(QPaintEvent *event);

private:
	int GetWidth();
	int CalculateValue(QPoint p);
	int CalculatePosition(int value);
private:
	int m_value;
	int m_min;
	int m_max;
	int	m_position;
	int m_chunkWidth;
	int m_grooveHeight;

	bool m_horizonal;
};


