#ifndef FBASEWIDGET_H
#define FBASEWIDGET_H

#include <QtWidgets/QWidget>
#include "inc_FCommonLib/fcommonlib_global.h"

class FCOMMONLIB_EXPORT FBaseWidget : public QWidget
{
    Q_OBJECT

public:
    FBaseWidget(QWidget *parent);

    virtual void RetranslateUi();
    virtual void TranslateText();
	virtual HWND getHwnd();

protected:
    bool event(QEvent *event);

};



class FCOMMONLIB_EXPORT FUpdateBaseWidget : public FBaseWidget
{
    Q_OBJECT

public:
    FUpdateBaseWidget(QWidget *parent);

    virtual void Update();


};



#endif // FBASEWIDGET_H
