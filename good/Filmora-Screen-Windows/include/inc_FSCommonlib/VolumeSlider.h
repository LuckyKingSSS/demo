#pragma once

#include "inc_FSCommonlib/fscommonlib_global.h"

#include <QtWidgets/QDialog>

class ToolBarSlider;
class FSCOMMONLIB_EXPORT AudioVolumeSlider : public QDialog
{
	Q_OBJECT
public:
	AudioVolumeSlider(QWidget *parent);

	void SetValue(int value);
	int GetValue();

    void SetRange(int min, int max);

	void SetHorizontal(bool horizontal);

	void DoModal(QPoint p);
Q_SIGNALS:
	void ValueChanged(int value);

protected:
	void resizeEvent(QResizeEvent *event);

private:
	ToolBarSlider *m_slider;
	bool m_show;
};


