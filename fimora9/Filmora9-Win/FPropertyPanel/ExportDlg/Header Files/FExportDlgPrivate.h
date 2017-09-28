#ifndef FEXPORTDLGPRIVATE_H
#define FEXPORTDLGPRIVATE_H

#include <QWidget>
#include "inc_FCommonLib\FBaseWidget.h"

class FNLETimeline;
class FExportBar;
class FExportDlgPrivate : public FBaseWidget
{
	Q_OBJECT

public:
	FExportDlgPrivate(QWidget *parent);
	~FExportDlgPrivate();
	void initUI();

protected:
	void paintEvent(QPaintEvent *event)override;
	void resizeEvent(QResizeEvent *event)override;

private:
	FExportBar* m_ExportBar;
	QStackedWidget* m_ExportSettingWidget;
	FNLETimeline * m_timeline;
};

#endif // FEXPORTDLGPRIVATE_H
