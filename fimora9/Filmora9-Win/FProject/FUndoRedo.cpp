
#include "inc_FProject/FUndoRedo.h"

FUndoManager::FUndoManager(QObject *parent)
:QUndoGroup(parent)
{
    m_sequence = new FUndoRedo(this);

    this->addStack(m_sequence);
    m_sequence->SetActive();
}

FUndoRedo* FUndoManager::GetSequneceUndo()
{
    return m_sequence;
}

//////////////////////////////////////////////////////////////////////////
FUndoRedo::FUndoRedo(QObject *parent)
:QUndoStack(parent)
{

}

void FUndoRedo::PushCommand(FUndoCommand *commond)
{
    push(commond);
}

bool FUndoRedo::CanUndo()
{
    return canUndo();
}

bool FUndoRedo::CanRedo()
{
    return canRedo();
}

void FUndoRedo::Undo()
{
    undo();
}

void FUndoRedo::Redo()
{
    redo();
}

void FUndoRedo::BeginComposite()
{
    this->beginMacro("BeginComposite");
}

void FUndoRedo::EndComposite()
{
    this->endMacro();
}

void FUndoRedo::SetActive(bool active)
{
    setActive(active);
}

bool FUndoRedo::IsActive()
{
    return isActive();
}

//////////////////////////////////////////////////////////////////////////
FUndoCommand::FUndoCommand()
{

}


void FUndoCommand::Undo()
{
    
}

void FUndoCommand::Redo()
{

}

void FUndoCommand::undo()
{
    Undo();
}

void FUndoCommand::redo()
{
    Redo();
}
