#include "inc_FProject/FProject.h"
#include "inc_FProject/FProjectData.h"
#include "inc_FProject/FUndoRedo.h"

FProject::FProject()
{
    m_projectData = new FProjectData;
    m_undoManager = new FUndoManager(nullptr);
}

FProject::~FProject()
{
    delete m_projectData;
    delete m_undoManager;
}

FProjectData* FProject::GetProjectData()
{
    return m_projectData;
}

FUndoManager* FProject::GetUndoManager()
{
    return m_undoManager;
}

void FProject::Release()
{
    delete this;
}

bool FProject::Load(const QString &path)
{
    return true;

}

bool FProject::Save(const QString &path)
{
    return true;

}
