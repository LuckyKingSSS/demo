#ifndef NLEWRAPERPRIVATE_H
#define NLEWRAPERPRIVATE_H

#include <QObject>
class WSCreateInstance;
class PlayerMaterProvider;
class IMediaPlayerControl;
class NLEWraperPrivate : public QObject
{
	Q_OBJECT

public:
	NLEWraperPrivate(QObject *parent);
	~NLEWraperPrivate();

	PlayerMaterProvider *m_pPlayerMaterProvider;
	IMediaPlayerControl *m_pMediaPlayerControl;
};

#endif // NLEWRAPERPRIVATE_H
