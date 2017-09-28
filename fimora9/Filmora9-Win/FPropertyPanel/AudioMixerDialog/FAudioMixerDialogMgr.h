#ifndef FAUDIOMIXERDIALOGMGR_H
#define FAUDIOMIXERDIALOGMGR_H

#include "FPanelMgr.h"

class FAudioMixerDialogMgr : public FPanelMgr
{
	Q_OBJECT

public:
	FAudioMixerDialogMgr(QObject *parent);
	~FAudioMixerDialogMgr();

	int GetDuration();
	void Stop();
	void Play();
	void Pause();
};

#endif // FAUDIOMIXERDIALOGMGR_H
