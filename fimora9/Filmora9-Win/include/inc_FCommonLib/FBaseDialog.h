#ifndef FBASEDIALOG_H
#define FBASEDIALOG_H

#include "inc_FCommonLib/fcommonlib_global.h"
#include "inc_CommonLib/TopLevelWindowTitleBar.h"
#include "inc_CommonLib/BaseDialog.h"

class FCOMMONLIB_EXPORT FBaseDialog : public BaseDialog
{
    Q_OBJECT

public:
    FBaseDialog(QWidget *parent);
    ~FBaseDialog();

    void SetTitle(QString title);

    void SetBorderWidth(int width);
    int GetBorderWidth();

    void SetBorderColor(QColor color);
    QColor GetBorderColor();

    int DoModal();

protected:
    void resizeEvent(QResizeEvent *event);
};

#endif // FBASEDIALOG_H
