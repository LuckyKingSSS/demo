#ifndef FTYPELABEL_H
#define FTYPELABEL_H

#include "..\include\inc_FCommonLib\FCommonWidgets.h"

class FTypeLabel : public FLabel
{
	Q_OBJECT

public:
	FTypeLabel(QWidget *parent);
	~FTypeLabel();


Q_SIGNALS:
	void sigClicked(QString strType);
protected:
	void mousePressEvent(QMouseEvent *event)override;

private:
	
};

#endif // FTYPELABEL_H
