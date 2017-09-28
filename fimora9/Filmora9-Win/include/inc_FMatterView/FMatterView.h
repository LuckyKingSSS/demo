#ifndef FMATTERVIEW_H
#define FMATTERVIEW_H

#include "inc_FMatterView/fmatterview_global.h"
#include "inc_FCommonLib/FBaseWidget.h"

class FMatterViewPrivate;
class FMediaData;

class FMATTERVIEW_EXPORT FMatterView : public FBaseWidget
{
public:
    FMatterView(QWidget *parent = nullptr);
    ~FMatterView();

	// º”‘ÿ◊ ‘¥
	void Load(FMediaData*);

Q_SIGNALS:
	void sigExport();

private:
	FMatterViewPrivate *m_pImpl;
};

#endif // FMATTERVIEW_H
