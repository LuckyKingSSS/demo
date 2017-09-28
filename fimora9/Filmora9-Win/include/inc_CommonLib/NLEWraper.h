#ifndef NLEWRAPER_H
#define NLEWRAPER_H

#include "commonlib_global.h"
#include "inc_CommonLib/PlayerMaterProvider.h"
#include "inc_MediaPlayerView/MediaPlayerControl.h"


#include <QObject>

class NLEWraperPrivate;
class PlayerMaterProvider;
class IMediaPlayerControl;
class COMMONLIB_EXPORT NLEWraper : public QObject
{
	Q_OBJECT

public:
	static NLEWraper& GetInstance();
	~NLEWraper();

	void SetMeidaPlayerControl(IMediaPlayerControl* pControler);
	IMediaPlayerControl* GetMeidaPlayerControl();

	PlayerMaterProvider* GetPlayerMaterProvider();
private:
	NLEWraper(QObject *parent = nullptr);
	NLEWraperPrivate *m_p;
};

#endif // NLEWRAPER_H
