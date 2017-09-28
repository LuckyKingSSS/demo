#ifndef SELECTIONWIDGET_H
#define SELECTIONWIDGET_H

#include <QtWidgets/QWidget>

class SelectionWidget : public QWidget
{
	Q_OBJECT

public:
	SelectionWidget(QWidget *parent);
	~SelectionWidget();
	void Show(QRect rc, bool bShow);
protected:
	void paintEvent(QPaintEvent *e) override;
private:
	QRect m_rcSelection;
};

#endif // SELECTIONWIDGET_H
