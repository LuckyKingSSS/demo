#ifndef FCOLLAPSEHEADERBASE_H
#define FCOLLAPSEHEADERBASE_H

#include "inc_FCommonLib\FBaseWidget.h"
#include "..\include\inc_FPropertyPanel\fpropertypanel_global.h"

class FButton;
class FPROPERTYPANEL_EXPORT FCollapseHeaderBase : public FBaseWidget
{
	Q_OBJECT

public:
	FCollapseHeaderBase(QWidget *parent);
	~FCollapseHeaderBase();
	void SetCurState(bool bCollapse){ m_bIsCollapse = bCollapse; };
	bool GetCurState(){ return m_bIsCollapse; };

protected:
	void mousePressEvent(QMouseEvent *e)override;

Q_SIGNALS:
	void sigCollapse(bool bIsColapse);
	void sigEnable(bool bEnable);
	void sigRefresh();

private:
	bool m_bIsCollapse;
};


class FPROPERTYPANEL_EXPORT FContexBaseWidget :public FBaseWidget
{
	Q_OBJECT
public:
	FContexBaseWidget(QWidget *parent);
	~FContexBaseWidget();
	virtual void resetConfig();
};

#endif // FCOLLAPSEHEADERBASE_H
