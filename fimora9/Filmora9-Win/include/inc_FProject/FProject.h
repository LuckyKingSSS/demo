#ifndef FPROJECT_H
#define FPROJECT_H

#include <QtCore/QString>

#include "inc_FProject/fproject_global.h"
#include "inc_FProject/FProjectFactory.h"


class FProjectData;
class FUndoManager;

class  FPROJECT_EXPORT FProject
{
public:
    FProjectData* GetProjectData();

    FUndoManager* GetUndoManager();

    void Release();

public:
    FProject();
    ~FProject();
    
    bool Load(const QString &path);
    bool Save(const QString &path);

private:
    FProjectData *m_projectData;
    FUndoManager *m_undoManager;
};

typedef QList<FProject*> FProjects;





#endif // FPROJECT_H
