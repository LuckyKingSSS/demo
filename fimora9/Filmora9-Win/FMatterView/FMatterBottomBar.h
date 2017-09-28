#ifndef FMATTERBOTTOMBAR_H
#define FMATTERBOTTOMBAR_H

#include "inc_FCommonLib/FBaseWidget.h"
#include "FMatterType.h"
class FLabel;
class FMatterBottomBarItem;

class FMatterBottomBar : public FBaseWidget
{
	Q_OBJECT

public:
	FMatterBottomBar(QWidget *parent = 0);
	~FMatterBottomBar();

Q_SIGNALS:
	void sigMatterTypeChanged(MatterType);
	void sigExport();

protected:
	void resizeEvent(QResizeEvent *) override;

private:
	void slotItemClicked(int);

private:
	QList<FMatterBottomBarItem*> m_Items;
};


/*FMatterBottomBarItem*/
class FMatterBottomBarItem : public FBaseWidget
{
	Q_OBJECT

public:
	FMatterBottomBarItem(int id = 0, QWidget *parent = 0);
	~FMatterBottomBarItem();

	void SetText(const QString&);
	void SetImageLabelObjectName(const QString&);

Q_SIGNALS:
	void Clicked(int);

protected:
	void resizeEvent(QResizeEvent *) override;
	void mousePressEvent(QMouseEvent *) override;

private:
	int m_Id;
	FLabel *m_pImageLbl;
	FLabel *m_pTextLbl;
};

#endif // FMATTERBOTTOMBAR_H
