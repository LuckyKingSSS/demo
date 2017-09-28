#ifndef FPROPERTYDIALOG_H
#define FPROPERTYDIALOG_H

#include "inc_FCommonLib/FBaseDialog.h"
#include "inc_FPropertyPanel/fpropertypanel_global.h"

class FNLETimeline;
class FPROPERTYPANEL_EXPORT FPropertyDialog : public FBaseDialog
{
    Q_OBJECT

public:
	FPropertyDialog(FNLETimeline *timeline, QWidget *parent);
	FPropertyDialog(FNLETimeline *timeline, FNLETimeline *rootTimeline, QWidget *parent);
    ~FPropertyDialog();

    FNLETimeline* GetTimeline();
    FNLETimeline* GetRootTimeline();
    virtual void SetTimeline(FNLETimeline *timeline);
	virtual void SetRootTimeline(FNLETimeline *timeline);

protected:
	void resizeEvent(QResizeEvent *event)override;

private:
    FNLETimeline *m_timeline;
    FNLETimeline *m_rootTimeline;

};

#endif // FPROPERTYDIALOG_H
