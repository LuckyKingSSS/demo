#include "inc_FPropertyPanel\fpropertypanel_global.h"
#ifndef MEDIACROPDIALOG_H
#define MEDIACROPDIALOG_H

//#include <QDialog>

class MediaCropView;
class FNLETimeline;
class TopLevelWindowTitleBar;
class FPROPERTYPANEL_EXPORT MediaCropDialog : public QDialog
{
	Q_OBJECT

public:
	MediaCropDialog(FNLETimeline *pDestTimeLine, QWidget *parent = 0);
	~MediaCropDialog();

	int DoModal();

protected: 
	void resizeEvent(QResizeEvent *event) override;
	void slotOnOk();
	void slotOnCancel();
private:
	MediaCropView *m_pCropView;
	bool m_bSaved;
	TopLevelWindowTitleBar *m_titleBar;

};

#endif // MEDIACROPDIALOG_H
