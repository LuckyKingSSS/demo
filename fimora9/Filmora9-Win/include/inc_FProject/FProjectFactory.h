#ifndef FPROJECTFACTORY_H
#define FPROJECTFACTORY_H

#include "inc_FProject/fproject_global.h"
#include "inc_CommonLib/NLETimeline.h"
#include "INLETimelineBase.h"


class FProject;
class FPROJECT_EXPORT FProjectFactory
{
public:
    static FProjectFactory *GetInstance();
    void Release();

    FProject* CreateProject();
    FProject* CreateProject(const QString &path);

    FProject* GetCurrentProject();
    void DestoryProject(FProject *project);

public:
    FProjectFactory();
private:
    FProject *m_curProject;
};


#endif 
