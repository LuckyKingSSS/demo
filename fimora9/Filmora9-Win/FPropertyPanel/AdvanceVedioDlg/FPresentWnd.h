#ifndef FPRESENTWND_H
#define FPRESENTWND_H


#include "..\include\inc_FCommonLib\FBaseWidget.h"
#include "..\include\inc_FCommonLib\FCommonWidgets.h"
#include "..\include\inc_FPropertyPanel\fpropertypanel_global.h"

class FPresentWndMgr;
class FNLETimeline;
class FPROPERTYPANEL_EXPORT FPresentWnd : public FBaseWidget
{
	Q_OBJECT

public:
	FPresentWnd(QWidget *parent);
	~FPresentWnd();
	void SetTimeline(FNLETimeline* timeline){ m_Timeline = timeline; };


public Q_SLOTS:
void  slotFrameChanged(QPixmap curPicture);


private:
	void initUI();
	void initData();
	void insertItem(QString strColorType);


private:
	FComboBox* m_ColorTypeCombo;
	QListWidget* m_ColorList;
	FPresentWndMgr* m_DataMgr;
	FNLETimeline* m_Timeline;
};

#endif // FPRESENTWND_H
