#ifndef FDATEDEFINE_H
#define FDATEDEFINE_H

#include "inc_FProject/FProjectFactory.h"
#include "inc_FProject/FProject.h"
#include "inc_FProject/FProjectData.h"
#include "inc_FProject/FUndoRedo.h"

#include "inc_FProject/FNLETimeline.h"




#define FPROJECT_FACTORY     FProjectFactory::GetInstance()
#define FCURRENT_PROJECT     FProjectFactory::GetInstance()->GetCurrentProject()
#define FPROJECT_DATA        FCURRENT_PROJECT->GetProjectData()
#define FSEQUNECE_UNDO       FCURRENT_PROJECT->GetUndoManager()->GetSequneceUndo()



#include <QtCore/QMimeData>

class FPROJECT_EXPORT FDragMimeData : public QMimeData
{
    Q_OBJECT
public:
    enum DragType{ Unknow, MatterView, Timeline };

public:
    FDragMimeData();

    DragType type;
    FNLETimelines timelines;
};


#endif // FPROJECT_H
