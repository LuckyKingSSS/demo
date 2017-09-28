#ifndef UNDOCOMMANDBASE_H
#define UNDOCOMMANDBASE_H

#include <QObject>
#include <QtWidgets/QUndoCommand>
#include "inc_FSTimelineView/fstimelineview_global.h"
class FSTIMELINEVIEW_EXPORT UndoCommand : public QObject, public QUndoCommand
{
	Q_OBJECT
public:
	UndoCommand(QObject *parent = nullptr);
	~UndoCommand();

Q_SIGNALS:
	void signalTrackUpdate();
	void signalTrackRebuild();
	void sigPause();
protected:
	virtual void Undo();
	virtual void Redo();

private:
	void undo() override;
	void redo() override;

};
#endif // UNDOCOMMANDBASE_H
