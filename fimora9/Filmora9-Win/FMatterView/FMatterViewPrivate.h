#ifndef FMATTERVIEWPRIVATE_H
#define FMATTERVIEWPRIVATE_H

#include "inc_FCommonLib/FBaseWidget.h"
#include "FMatterType.h"

class FMatterListView;
class FMatterIconView;
class FMatterBottomBar;
class FMatterViewMgr;
class FMediaData;

class FMatterViewPrivate : public FBaseWidget
{
	Q_OBJECT

public:
	FMatterViewPrivate(QWidget *parent = 0);
	~FMatterViewPrivate();

	// º”‘ÿ◊ ‘¥
	void Load(FMediaData*);

Q_SIGNALS:
	void sigExport();

protected:
	void resizeEvent(QResizeEvent *) override;

private:
	void slotMatterTypeChanged(MatterType);

private:
	FMatterViewMgr *m_pMgr;
	FMatterListView *m_pListView;
	FMatterIconView *m_pIconView;
	FMatterBottomBar *m_pBottomBar;
};

#endif // FMATTERVIEWPRIVATE_H
