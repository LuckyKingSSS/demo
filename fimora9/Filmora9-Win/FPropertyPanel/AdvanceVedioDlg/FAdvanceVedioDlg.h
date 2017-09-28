#ifndef FADVANCEVEDIODLG_H
#define FADVANCEVEDIODLG_H

#include "FPropertyDialog.h"
#include "inc_FPropertyPanel\fpropertypanel_global.h"

class FAdvanceVedioMgr;
class FColorSettingWnd;
class FPreviewPlayer;
class FNLETimeline;

class FPROPERTYPANEL_EXPORT FAdvanceVedioDlg : public FPropertyDialog
{
	Q_OBJECT

public:
	FAdvanceVedioDlg(FNLETimeline* timeline, QWidget *parent);
	FAdvanceVedioDlg(FNLETimeline *timeline, FNLETimeline *rootTimeline, QWidget *parent);
	~FAdvanceVedioDlg();

Q_SIGNALS:
		void sigUserConfirmSetting();

private:
	void InitUI();
	void InitData();
	void InitConnections();

private:
	FColorSettingWnd* m_ColorSettingWnd;
	FPreviewPlayer* m_PreviewPlayer;
	FAdvanceVedioMgr* m_DataMgr;
	FNLETimeline* m_Timeline;
	FNLETimeline* m_RootTimeline;
};

#endif // FADVANCEVEDIODLG_H
