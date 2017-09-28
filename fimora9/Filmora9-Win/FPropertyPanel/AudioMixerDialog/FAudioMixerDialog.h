#ifndef FAUDIOMIXERDIALOG_H
#define FAUDIOMIXERDIALOG_H

#include "FPropertyDialog.h"
#include "FAudioMixerBaseWidget.h"
#include "FAudioMixerDialogMgr.h"
#include "inc_FCommonLib/FCommonWidgets.h"
#include "inc_CommonLib/CommonWidgets.h"
#include "inc_CommonLib/TopLevelWindowTitleBar.h"
#include "inc_FPropertyPanel/fpropertypanel_global.h"

class FPROPERTYPANEL_EXPORT FAudioMixerDialog : public FPropertyDialog
{
	Q_OBJECT

public:
	FAudioMixerDialog(QWidget *parent = 0);
	~FAudioMixerDialog();

signals:
	void sigOk();
	void sigCancel();

protected:
	void resizeEvent(QResizeEvent *) override;
	void showEvent(QShowEvent *) override;

private:
	void ConnectSignals();
	void Init();
	QString GetTimeString(int);  // hh:mm:ss

private:
	FAudioMixerDialogMgr *m_pPanelMgr;
	FAudioTrack *m_pAudioTrackVideo;
	FAudioTrack *m_pAudioTrackPIP;
	FAudioTrack *m_pAudioTrackMusic1;
	FAudioTrack *m_pAudioTrackMusic2;
	FMarkSliderRuler *m_pRuler1;
	FMarkSliderRuler *m_pRuler2;
	FMarkSlider *m_pMasterSlider;
	FColorGradientSlider *m_pMasterAudioTrackLeft;
	FColorGradientSlider *m_pMasterAudioTrackRight;
	FLineEdit *m_pMasterLineEdit;
	FLabel *m_pMasterLbl;
	FLabel *m_pMastImageLbl;
	FToolBarSlider *m_pDurationSlider;
	FStateButton *m_pPlayBtn;
	FImageButton *m_pStopBtn;
	FLabel *m_pPanLbl;
	FLabel *m_pDBLbl;
	FLabel *m_pTimeLbl;
	FLabel *m_pTotalTimeLbl;
	FButton *m_pOkBtn;
	FButton *m_pCancelBtn;
};

#endif // FAUDIOMIXERDIALOG_H
