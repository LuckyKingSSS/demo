#ifndef FSEXPORT_OUTPUTSET
#define FSEXPORT_OUTPUTSET

#include "inc_CommonLib/BaseDialog.h"
#include <QColor>

class OutputSet :public BaseDialog
{
    Q_OBJECT
public:
    OutputSet(const QString & title,QWidget *pParent);
    ~OutputSet();

    void SetButtons(int buttons);
    void SetResizeble(bool beResizeble);
    void SetButtonWidth(int width);
};

#endif // FSEXPORT_OUTPUTSET