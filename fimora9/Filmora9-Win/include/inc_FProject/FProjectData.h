#ifndef FPROJECTDATA_H
#define FPROJECTDATA_H

#include "inc_FProject/fproject_global.h"

#include "inc_FProject/FMediaData.h"

class FPROJECT_EXPORT FProjectData
{
public:
    FProjectData();


    FMediaData* GetMediaData();


private:
    FMediaFolder m_mediaData;
};


#endif 
