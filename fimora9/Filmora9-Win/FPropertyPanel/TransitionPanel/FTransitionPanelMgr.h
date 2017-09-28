#ifndef FTRANSITIONPANELMGR_H
#define FTRANSITIONPANELMGR_H

#include "FPanelMgr.h"

class FTransitionPanelMgr : public FPanelMgr
{
	Q_OBJECT

public:
	enum TransitionMode{
		None_Mode,
		Overlap_Mode, 
		Prefix_Mode, 
		Postfix_Mode 
	};

public:
	FTransitionPanelMgr(QObject *parent);
	~FTransitionPanelMgr();

	// Duration
	void SetDuration(int);
	int GetDuration();
	// Transition Mode
	void SetTransitionMode(TransitionMode);
	TransitionMode GetTransitionMode();

private:
	TransitionMode m_mode;
};

#endif // FTRANSITIONPANELMGR_H
