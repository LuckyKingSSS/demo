#include "stdafx.h"
#include "inc_FSTimelineView/UndoCommandBase.h"

UndoCommand::UndoCommand(QObject *parent)
	: QObject(parent)
	, QUndoCommand()
{

}

UndoCommand::~UndoCommand()
{

}

void UndoCommand::Undo()
{

}

void UndoCommand::Redo()
{

}

void UndoCommand::undo()
{
	emit sigPause();
	Undo();
	QTimer::singleShot(0, [&]{emit signalTrackUpdate(); });
}

void UndoCommand::redo()
{
	emit sigPause();
	Redo();
    QTimer::singleShot(0, [&]{emit signalTrackUpdate(); });
	//emit signalTrackUpdate();
}