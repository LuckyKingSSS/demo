#ifndef FUNDOREDO_H
#define FUNDOREDO_H

#include <QtWidgets/QUndoGroup>
#include <QtWidgets/QUndoStack>
#include <QtWidgets/QUndoCommand>

#include "inc_FProject/fproject_global.h"



class FUndoRedo;
class  FPROJECT_EXPORT FUndoManager : public QUndoGroup
{
    Q_OBJECT
public:
    FUndoManager(QObject *parent);

    FUndoRedo* GetSequneceUndo();

private:
    FUndoRedo *m_sequence;
};


class FUndoCommand;
class FPROJECT_EXPORT FUndoRedo : public QUndoStack
{
    Q_OBJECT
public:
    FUndoRedo(QObject *parent);

    void PushCommand(FUndoCommand *commond);

    void Undo();
    void Redo();
    bool CanUndo();
    bool CanRedo();

    void BeginComposite();
    void EndComposite();

    void SetActive(bool active = true);
    bool IsActive();
};


class FPROJECT_EXPORT FUndoCommand : public QObject, public QUndoCommand
{
    Q_OBJECT
public:
    FUndoCommand();

public:
    virtual void Undo();
    virtual void Redo();


private:
    void undo() override;
    void redo() override;

};



#endif // FPROJECT_H
