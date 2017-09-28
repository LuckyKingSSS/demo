#include "inc_FProject/FProjectFactory.h"

#include "inc_FProject/FProject.h"

static FProjectFactory *s_FProjectFactory = nullptr;
FProjectFactory * FProjectFactory::GetInstance()
{
    if (s_FProjectFactory == nullptr)
    {
        s_FProjectFactory = new FProjectFactory;
    }
    return s_FProjectFactory;
}

void FProjectFactory::Release()
{
    if (s_FProjectFactory)
    {
        delete s_FProjectFactory;
        s_FProjectFactory = nullptr;
    }
}

FProject * FProjectFactory::CreateProject()
{
    Q_ASSERT(m_curProject == nullptr);

    FProject *project = new FProject();
    m_curProject = project;

    return project;
}

FProject* FProjectFactory::CreateProject(const QString &path)
{
    Q_ASSERT(m_curProject == nullptr);
    FProject *project = new FProject();
    m_curProject = project;
    return project;
}

FProject* FProjectFactory::GetCurrentProject()
{
    return m_curProject;
}

void FProjectFactory::DestoryProject(FProject *project)
{
    delete project;
    m_curProject = nullptr;
}

FProjectFactory::FProjectFactory()
{
    m_curProject = nullptr;
}
