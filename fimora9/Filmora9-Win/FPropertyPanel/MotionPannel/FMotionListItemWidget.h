#ifndef FMOTIONLISTITEMWIDGET_H
#define FMOTIONLISTITEMWIDGET_H

#include <QPixmap>
#include <QRect>
#include "..\include\inc_FCommonLib\FBaseWidget.h"
#include "..\include\inc_FPropertyPanel\fpropertypanel_global.h"
#include "..\include\inc_FCommonLib\FCommonWidgets.h"



class FLabel;
class FButton;
class QPushButton;
class FPROPERTYPANEL_EXPORT FMotionListItemWidget : public FBaseWidget
{
	Q_OBJECT

public:
	FMotionListItemWidget(QWidget *parent, const QString strFilePath);
	~FMotionListItemWidget();
	void SetLabelText(QString strText){ m_Text->setText(strText); };
	void SetIconWidgetImage(const QIcon icon);
	QRect GetIconWidgetSize();

protected:
	void resizeEvent(QResizeEvent *event)override;

private:
	void InitUI();

private:
	FLabel* m_IConWidget;
	FLabel*  m_Text;
	QString m_StrBackgroundPath;
};

#endif // FMOTIONLISTITEMWIDGET_H
